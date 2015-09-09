//	pongEvents.c	08/08/2013
//******************************************************************************
//******************************************************************************
//	ALL THESE FUNCTIONS REQUIRE REWORK!
//******************************************************************************
//******************************************************************************
//
#include "msp430x22x4.h"
#include <stdlib.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "pong.h"

extern volatile int16 dx, dy;
extern volatile uint16 TB0_tone_on;

extern volatile uint16 WDT_adc_cnt;
extern volatile uint16 sec_timer;

extern volatile int button;
extern volatile int left_score;
extern volatile int right_score;
extern volatile int cur_ball_speed;
extern volatile int countdown;

extern volatile int new_game;

extern PADDLE* rightPaddle;				// right paddle object
extern PADDLE* leftPaddle;					// left paddle object

extern BALL* myBall;							// **replace with malloc'd struct**


void BEGIN_event(){

	// THE COUNTDOWN
	lcd_clear();
	lcd_mode(LCD_2X_FONT);
	countdown = 3;
	sec_timer = WDT_CPS;
	while (countdown > 0)
	{
		lcd_cursor(74, 72);
		printf("%d", countdown);
		while (countdown == 0)
		{
			lcd_cursor(67, 72);
			printf("GO!");
		}
	}
	lcd_mode(~LCD_2X_FONT);
	// END COUNTDOWN

	lcd_clear();						// clear LCD
	lcd_volume(360);					// **increase as necessary**

	left_score = 0;
	right_score = 0;

	// manufacture and draw ball
	drawBall(myBall);

	// manufacture and draw paddle
	drawPaddle(leftPaddle);
	drawPaddle(rightPaddle);

	// Draw playing field
	lcd_rectangle(-1, 0, 162, 160, 1);	// draw border
	lcd_rectangle(-1, 1, 162, 158, 1);	// draw border

	SCORE_event();
	dx = rand() % 2 ? -1 : 1;			// delta x
	dy = rand() % 2 ? -1 : 1;			// delta y
	WDT_adc_cnt = 1;					// start sampling the paddles
	cur_ball_speed = BALL_START_SPEED;
	TACCR0 = cur_ball_speed;			// interrupt rate
}


//------------------------------------------------------------------------------
//	ball factory (fix by malloc'ing ball struct)
//
void new_ball(int x, int y)
{
	myBall = (BALL *)malloc(sizeof(BALL));
	// initialize ball values
	myBall->x = x;						// set horizontal position
	myBall->y = y;						// set vertical position
	myBall->old_x = myBall->x;			// set old values
	myBall->old_y = myBall->y;
}

//------------------------------------------------------------------------------
//	paddle factory (fix by malloc'ing paddle struct)
//
void new_left_paddle(int channel, int x)
{
	leftPaddle = (PADDLE *)malloc(sizeof(PADDLE));

	leftPaddle->channel = channel;			// ADC input channel
	leftPaddle->x = x;						// horizontal paddle position
	leftPaddle->y = ADC_read(channel)/6.4;
	leftPaddle->old_y = 0;
}

void new_right_paddle(int channel, int x)
{
	rightPaddle = (PADDLE *)malloc(sizeof(PADDLE));

	rightPaddle->channel = channel;			// ADC input channel
	rightPaddle->x = x;						// horizontal paddle position
	rightPaddle->y = 160 - (ADC_read(channel)/6.4);
	rightPaddle->old_y = 0;
}

void SCORE_event(){

	lcd_mode(LCD_2X_FONT);
	lcd_cursor(100, 130);
	printf("%d", right_score);
	lcd_cursor(50, 130);
	printf("%d", left_score);
	lcd_mode(~LCD_2X_FONT);
	lcd_rectangle(80, 1, 2, 158, 1);	// draw border
}

void ENDGAME_event(){
	// After the Game
	lcd_cursor(40, 110);

	victory();
	if (left_score == 5)
	{
		printf("Player 1 Wins!");
	}
	else if (right_score == 5)
	{
		printf("Player 2 Wins!");
	}
	//victory();
	lcd_cursor(97, 4);
	printf("Play Again");


	left_score = 0;
	right_score = 0;
	//free(leftPaddle);
	//free(rightPaddle);
	if (first_time = 1)
	while (1) if (button == 1) break;	// pause
	button = 0;
}

void failure(){

	TBCCR0 = LONG_BEEP;
	TBCCR2 = LONG_BEEP >> 1;
	TB0_tone_on = 20;
}

void victory(){
	BEEP;
	BEEP;
	BEEP;
}

//------------------------------------------------------------------------------
//	ADC event - paddle potentiometer and draw paddle (fix)
//
void ADC_READ_event(PADDLE* paddle, char side)
{
	int pot = ADC_read(paddle->channel);	// sample potentiometer

	// check for paddle position change
	if ((abs(pot - paddle->potValue) > POT_THRESHHOLD))
	{
		paddle->potValue = pot;			// save old value
		if (side == 'l')
		{
			paddle->y = pot/6.4;
		}
		if (side == 'r')
		{
			paddle->y = 160 - (pot/6.4);
		}
		drawPaddle(paddle);				// draw paddle
	}
	return;
} // end ADC_READ_event


//------------------------------------------------------------------------------
//	TimerA event - move ball (fix)
//
void MOVE_BALL_event(BALL* ball)
{
	ball->x += dx;						// increment x coordinate
	ball->y += dy;						// update y coordinate
	if ((ball->y > 153) || (ball->y < 6))
	{
		dy = -dy;
	}

	drawBall(ball);						// ok, draw ball in new position
	volatile int paddle_pos;

	if (ball->x <= 6)
	{
		paddle_pos = leftPaddle->y;

		if ((ball->y >= paddle_pos-7) && (ball->y <= paddle_pos+8))		//Hits paddle
		{
			int hit_pos = (paddle_pos - ball->y)*(-1);
			if (hit_pos >= 6 && hit_pos <= 8) {
				dx = -dx;
				dy = 2;
				BEEP;
			}
			else if (hit_pos >= 2 && hit_pos <= 5) {
				dx = -dx;
				dy = 1;
				BEEP;
			}
			else if (hit_pos >= 0 && hit_pos <= 1){
				dx = -dx;
				dy = 0;
				BEEP;
			}
			else if (hit_pos >= -4 && hit_pos <= -1){
				dx = -dx;
				dy = -1;
				BEEP;
			}
			else if (hit_pos >= -7 && hit_pos <= -5){
				dx = -dx;
				dy = -2;
				BEEP;
			}
		}
		else {
			if (ball->x < -3)
			{
				right_score++;
				failure();
				SCORE_event();
				free(myBall);

				new_ball(153, rightPaddle->y);
				drawBall(myBall);
				dy = 1;
				dx = -1;
				cur_ball_speed = BALL_START_SPEED;
			}
		}
	}

	if ((ball->x >= 154))
	{
		paddle_pos = rightPaddle->y;

		if ((ball->y >= paddle_pos-7) && (ball->y <= paddle_pos+8))		//Hits paddle
		{
			int hit_pos = (paddle_pos - ball->y)*(-1);
			if (hit_pos >= 5 && hit_pos <= 7) {
				dx = -dx;
				dy = 2;
				BEEP;
			}
			else if (hit_pos >= 2 && hit_pos <= 4) {
				dx = -dx;
				dy = 1;
				BEEP;
			}
			else if (hit_pos >= 0 && hit_pos <= 1){
				dx = -dx;
				dy = 0;
				BEEP;
			}
			else if (hit_pos >= -4 && hit_pos <= -1){
				dx = -dx;
				dy = -1;
				BEEP;
			}
			else if (hit_pos >= -7 && hit_pos <= -5){
				dx = -dx;
				dy = -2;
				BEEP;
			}
		}
		else {
			if (ball->x > 162)											// misses paddle
			{
				left_score++;
				failure();
				SCORE_event();
				free(myBall);

				new_ball(7, leftPaddle->y);
				drawBall(myBall);
				dy = 1;
				dx = 1;
				cur_ball_speed = BALL_START_SPEED;
			}
		}
	}
}
