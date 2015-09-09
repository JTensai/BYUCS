//	pong.c	08/08/2013
//******************************************************************************
//  Pong
//
//	Jordan Taylor
//	All work for this lab was done by me :D
//
//  Description:
//
//	Write a C program to play a 2-player game of Pong, a two-dimensional sports
//	game which simulates table tennis. Use the potentiometers to control the
//	paddles moving vertically on each side of the screen. The paddles and walls
//	reflect a ball back and forth at the correct speed and reflection angle.
//	A point is scored when one side fails to return the ball to the other side.
//	Show the score in real time on the LCD display.
//
//	Divide the paddle into segments to change the ball's angle of return.
//	For example, the center segments return the ball a 90° angle in relation to
//	the paddle, while the outer segments return the ball at smaller angles.
//	Make the ball accelerate the more it is returned back and forth between
//	paddles; missing the ball resets the speed.
//
//  Author:		Paul Roper, Brigham Young University
//  Revisions:	March 2010	Original code, split into files
//              08/05/2013	updated
//			08/08/2013	dx,dy signed values
//
//  Built with Code Composer Studio Version: 5.3.0.00090
//*******************************************************************************
//******************************************************************************
//
#include "msp430x22x4.h"
#include <stdlib.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "pong.h"

//------------------------------------------------------------------------------
// add service routine events here (bit additive):
#define MOVE_BALL		0x0001
#define ADC_READ		0x0002
#define SCORE			0x0004
#define ENDGAME			0x0008
#define BEGIN			0x0010

//------------------------------------------------------------------------------
// pong global variables
volatile uint16 WDT_cps_cnt;			// watchdog counters
volatile uint16 WDT_adc_cnt;
volatile uint16 WDT_score_cnt;
volatile uint16 sec_timer;
volatile uint16 WDT_debounce_cnt;

volatile int button = 0;
volatile int left_score = 0;
volatile int right_score = 0;
volatile int cur_ball_speed;
volatile int countdown;


volatile int new_game;

volatile uint16 TB0_tone_on;			// PWM tones

volatile uint16 sys_event;				// pending events
volatile int16 dx, dy;					// ball delta change

extern const uint16 pong_image[];		// 2 paddle image

PADDLE* leftPaddle;					// left paddle object
PADDLE* rightPaddle;				// right paddle object
BALL* myBall;

//------------------------------------------------------------------------------


void main(void)
{
	// 	MSP430 Clock - Set DCO to 12 MHz:
	ERROR2(RBX430_init(_12MHZ));			// init board
	ERROR2(lcd_init());						// init LCD
	ERROR2(ADC_init());						// init ADC

	// configure watchdog
	WDTCTL = WDT_CTL;						// set Watchdog interval
	IE1 |= WDTIE;							// Enable WDT interrupt
	WDT_adc_cnt = 0;
	WDT_cps_cnt = WDT_CPS;
	WDT_score_cnt = WDT_CPS/2;

	// configure switches
	P1IES |= 0x0f;						// interrupt on h to l transition
	P1IE |= 0x0f;						// enable P1.0-3 interrupts

	// configure TimerA to move the ball
	TAR = 0;								// reset timer
	TACCR0 = 0;								// interrupt rate
	TACTL = TASSEL_2|ID_3|MC_1|TAIE;		// SMCLK/8, up mode, interrupt enable

	// configure TimerB h/w PWM for speaker
	P4SEL |= 0x20;							// P4.5 TB2 output
	TBR = 0;								// reset timer B
	TBCTL = TBSSEL_2 | ID_0 | MC_1;			// SMCLK, /1, UP (no interrupts)
	TBCCTL2 = OUTMOD_3;						// TB2 = set/reset

	__bis_SR_register(GIE);					// enable interrupts

	lcd_clear();							// clear LCD
	lcd_backlight(ON);						// turn on LCD
	lcd_wordImage(pong_image, 25, 35, 1);
	lcd_mode(LCD_2X_FONT);					// turn 2x font on
	lcd_volume(360);
	lcd_cursor(20, 30);						// set display coordinates
	printf("P O N G");
	lcd_mode(~LCD_2X_FONT);					// turn 2x font off
	lcd_cursor(30, 5);
	printf("PRESS SWITCH 1");
	while (1) if (button == 1) break;			// wait for a switch
	button = 0;

	// COUNTDOWN HERE
	//-----------------------------------------------------------
	//	play forever
	while (1)
	{
		//new_game = 1;
		new_ball(80, 50);
		new_left_paddle(LEFT_POT, 0);
		new_right_paddle(RIGHT_POT, 155);
		sys_event = 0;						// no events pending

		//-----------------------------------------------------------
		//	event service routine loop
		//-----------------------------------------------------------
		while (1)
		{
			// disable interrupts while checking sys_event
			_disable_interrupts();

			// if event pending, enable interrupts
			if (sys_event) _enable_interrupt();

			// else enable interrupts and goto sleep
			else __bis_SR_register(LPM0_bits | GIE);

			//-------------------------------------------------------
			//	I'm AWAKE!!!  What needs service?
			if (sys_event & BEGIN)
			{
				sys_event &= ~BEGIN;
				BEGIN_event();
			}
			else if (sys_event & MOVE_BALL)				// timer A event
			{
				sys_event &= ~MOVE_BALL;			// clear TimerA event
				MOVE_BALL_event(myBall);				// update ball position
			}
			else if (sys_event & ADC_READ)			// read ADC event
			{
				sys_event &= ~ADC_READ;				// clear ADC event
				ADC_READ_event(leftPaddle, 'l');
				ADC_READ_event(rightPaddle, 'r');		// process new
			}
			else if (sys_event & SCORE)
			{
				sys_event &= ~SCORE;
				SCORE_event();
			}
			else if (sys_event & ENDGAME)
			{
				sys_event &= ~ENDGAME;
				ENDGAME_event();
				//sys_event = 0;
			}
			else									// ????
			{
				ERROR2(10);							// unrecognized event
			}
		}
	}
} // end main


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Timer A1 interrupt service routine
//
#pragma vector = TIMERA1_VECTOR
__interrupt void TIMERA1_ISR(void)
{
	TACTL &= ~TAIFG;					// acknowledge interrupt
	TACCR0 = cur_ball_speed;			// adjust ball speed
	sys_event |= MOVE_BALL;				// signal move ball event
	__bic_SR_register_on_exit(LPM0_bits);
	return;
} // end TIMERA1_VECTOR

#pragma vector = PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
   P1IFG &= ~0x0f;                  // P1.0-3 IFG cleared
   WDT_debounce_cnt = DEBOUNCE_CNT; // enable debounce
}


//------------------------------------------------------------------------------
//	Watchdog Timer ISR
//
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	if (WDT_cps_cnt && (--WDT_cps_cnt == 0))				// 1 second
	{
		LED_GREEN_TOGGLE;				// toggle green LED
		WDT_cps_cnt = WDT_CPS;			// reset counter to 1s
	}

	if (sec_timer && (--sec_timer == 0))					// 1 second?
	{
		if (countdown > -1){
			countdown--;
		}
		sec_timer = WDT_CPS;			// reset counter
	}

	if (WDT_score_cnt && (--WDT_score_cnt == 0))
	{
		sys_event |= SCORE;					// signal SCORE event
		WDT_score_cnt = WDT_CPS/2;			// reset counter to .5s
		if (cur_ball_speed > 2000)
		{
			cur_ball_speed = cur_ball_speed/1.025;
		}
		else
		{
			cur_ball_speed = 2000;
		}
	}

	if (new_game == 1)
	{
		sys_event |= BEGIN;
		new_game = 0;
	}

	if (left_score == 5 || right_score == 5)
	{
		sys_event |= ENDGAME;
	}

	// decrement tone counter - turn off tone when 0
	if (TB0_tone_on && (--TB0_tone_on == 0)) TBCCR0 = 0;

	// decrement adc event counter - signal to read adc
	if (WDT_adc_cnt && (--WDT_adc_cnt == 0))
	{
		WDT_adc_cnt = WDT_ADC;
		sys_event |= ADC_READ;			// signal ADC event
	}
	// check for switch debounce
	if (WDT_debounce_cnt && (--WDT_debounce_cnt == 0))
	{
		button = (P1IN ^ 0x0f) & 0x0f;
		new_game = 1;
	}

	// wake up processor if there are any events pending
	if (sys_event) __bic_SR_register_on_exit(LPM0_bits);
	return;

} // end WDT_ISR


//------------------------------------------------------------------------------
// un-initialized MSP430F2274 interrupt vectors
#pragma vector=PORT2_VECTOR,TIMER0_A0_VECTOR,NMI_VECTOR,	\
		USCIAB0RX_VECTOR,USCIAB0TX_VECTOR,TIMERB0_VECTOR,TIMERB1_VECTOR
__interrupt void ISR_trap(void)
{
	ERROR2(SYS_ERR_ISR);				// unrecognized event
	WDTCTL = 0;							// Write to WDT with a wrong password
}
