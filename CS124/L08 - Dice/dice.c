//******************************************************************************
//  Lab 8a - Dice
//
//  Name: Jordan Taylor
//			All work for this lab was done by me.

//  Description:
//
//	"Write a dice roller C program that waits for a switch to be pressed and then
//	 displays two dice which randomly change values. The dice roll begins rapidly
//	 and progressively slows down until it stops (after approximately 3-5
//	 seconds). A congratulatory ditty is played when doubles are rolled. If
//	 doubles are rolled twice in a row, output a raspberry tone. Write an
//	 assembly language function which returns a random number from 1 to 6 and
//	 call this function from your C program." 
//
//  Author:		Paul Roper, Brigham Young University
//	Revisions:	March 2010	Original code
//				07/28/2011	Added doTone
//				11/09/2012	lcd_wordImage
//
//  Built with Code Composer Studio Version: 4.2.3.00004
//*******************************************************************************
//
//                            MSP430F2274
//                  .-----------------------------.
//            SW1-->|P1.0^                    P2.0|<->LCD_DB0
//            SW2-->|P1.1^                    P2.1|<->LCD_DB1
//            SW3-->|P1.2^                    P2.2|<->LCD_DB2
//            SW4-->|P1.3^                    P2.3|<->LCD_DB3
//       ADXL_INT-->|P1.4                     P2.4|<->LCD_DB4
//        AUX INT-->|P1.5                     P2.5|<->LCD_DB5
//        SERVO_1<--|P1.6 (TA1)               P2.6|<->LCD_DB6
//        SERVO_2<--|P1.7 (TA2)               P2.7|<->LCD_DB7
//                  |                             |
//         LCD_A0<--|P3.0                     P4.0|-->LED_1 (Green)
//        i2c_SDA<->|P3.1 (UCB0SDA)     (TB1) P4.1|-->LED_2 (Orange) / SERVO_3
//        i2c_SCL<--|P3.2 (UCB0SCL)     (TB2) P4.2|-->LED_3 (Yellow) / SERVO_4
//         LCD_RW<--|P3.3                     P4.3|-->LED_4 (Red)
//   TX/LED_5 (G)<--|P3.4 (UCA0TXD)     (TB1) P4.4|-->LCD_BL
//             RX-->|P3.5 (UCA0RXD)     (TB2) P4.5|-->SPEAKER
//           RPOT-->|P3.6 (A6)          (A15) P4.6|-->LED 6 (R)
//           LPOT-->|P3.7 (A7)                P4.7|-->LCD_E
//                  '-----------------------------'
//
//******************************************************************************
//******************************************************************************
// includes
#include "msp430x22x4.h"
#include <stdlib.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "dice.h"

// global variables ------------------------------------------------------------
volatile int WDT_Sec_Cnt;				// WDT second counter
volatile int WDT_Delay;					// WDT delay counter
volatile int WDT_Tone_Cnt;				// WDT tone counter

volatile int switches;
volatile int countdown;
volatile int WDT_debounce_cnt;

volatile int doubles_count = 0;
volatile int timeout = 0;

// Tone Array
//const char WinTones[] = {TONE_E, 30, TONE_F, 30, TONE_Fs, 60};


// main ------------------------------------------------------------------------
void main(void)
{
	RBX430_init(_8MHZ);					// init board
	ERROR2(lcd_init());					// init LCD

	// configure Watchdog
	WDTCTL = WDT_CTL;					// Set Watchdog interval
	WDT_Sec_Cnt = WDT_1SEC_CNT;			// set WD 1 second counter
	WDT_Delay = 0;						// reset delay counter
	WDT_Tone_Cnt = 0;					// turn off tone
	IE1 |= WDTIE;						// enable WDT interrupt

	// configure h/w PWM for speaker
	P4SEL |= 0x20;						// P4.5 TB2 output
	TBR = 0;							// reset timer B
	TBCTL = TBSSEL_2 | ID_0 | MC_1;		// SMCLK, /1, UP (no interrupts)
	TBCCTL2 = OUTMOD_3;					// TB2 = set/reset.

	// switch debounce function -----------------------------------------
	P1IES |= 0x0f;                 	    // interrupt on h to l transition
	P1IE |= 0x0f;                    	// enable P1.0-3 interrupts


	// Initialize switch values
	switches = 0;
	countdown = 0;

	__bis_SR_register(GIE);				// enable interrupts

	lcd_clear();						// clear LCD
	lcd_wordImage(byu4_image, (160-75)/2, 110, 1);
	lcd_cursor(28, 25);				// position message
	lcd_printf("Press a button to");
	lcd_cursor(40, 10);
	lcd_printf("roll the dice");

	lcd_backlight(ON);

	timeout = 0;

	while (timeout < 1300){
		WDT_sleep(1);
		timeout++;
		if (timeout == 1299){
			lcd_backlight(OFF);
		}
	}
	while(switches == 0){}

	while(1){
		roll();
	}


} // end main()


void roll(void){
	uint8 oldDie = 0, oldDie2 = 0;
	uint8 random1, random2;
	lcd_mode(LCD_2X_FONT | LCD_PROPORTIONAL);
	if (countdown == 50){
		lcd_clear();
		lcd_wordImage(byu4_image, (160-75)/2, 110, 1);
		// draw double die frame LEFT
		lcd_rectangle(13, 45, 60, 60, 1);
		lcd_rectangle(14, 46, 58, 58, 1);

		// draw double die frame RIGHT
		lcd_rectangle(87, 45, 60, 60, 1);
		lcd_rectangle(88, 46, 58, 58, 1);

		lcd_backlight(ON);				// turn on LCD backlight
		countdown--;
	}
	while (countdown > 1){
		uint8 n = 6;
		lcd_backlight(OFF);
		doTone(BEEP, BEEP_CNT);			// output BEEP

		uint8 random_number = WDT_Sec_Cnt;
		random1 = random(n, random_number);
		drawDie(random1, &oldDie, 19, 51);// draw a die RIGHT
		lcd_backlight(ON);	// turn off LCD

		random_number = WDT_Sec_Cnt+100;
		random2 = random(n, random_number);
		drawDie(random2, &oldDie2, 94, 51);// draw a die LEFT

		WDT_sleep(200/countdown);

		countdown--;
		if ((P1IN ^ 0x0f) & 0x0f != 0){
			countdown = 50;
		}
	}
	while (countdown == 1){
		if (random1 == random2){
			victory();
			if (doubles_count % 2 == 1){
				rasberry();
			}
			doubles_count++;
		}
		else {
			doTone(myCLOCK/1386/12*10, 100);
		}
		lcd_cursor(30, 15);				// position message
		lcd_printf("Roll Again?");
		switches = 0;
		timeout = 0;
		while (timeout < 1300){
			WDT_sleep(1);
			timeout++;
			if (timeout == 1299){
				lcd_backlight(OFF);
			}
		}

		while(switches == 0){}
		lcd_clear();
	}
	return;
}


void victory(void){
	//volatile int i;
	//for ( i = 0; i < sizeof(WinTones)/sizeof(WinTones[0]); i = i+2){
	//	doTone(WinTones[i], WinTones[i+1]);
	//}



	doTone(TONE_E, 30);
	doTone(TONE_F, 30);
	doTone(TONE_Fs, 60);
	return;
}

void rasberry(void){
	doTone(TONE_F, 60);
	doTone(TONE_E, 60);
	doTone(TONE_Eb, 60);
	return;
}

//------------------------------------------------------------------------------
// output tone subroutine ------------------------------------------------------
void doTone(uint16 tone, uint16 time)
{
	TBCCR0 = tone;						// set beep frequency/duty cycle
	TBCCR2 = tone >> 1;					// 50% duty cycle
	WDT_Tone_Cnt = time;				// turn on speaker
	while (WDT_Tone_Cnt);				// wait for tone off
	return;
} // end doTone


//------------------------------------------------------------------------------
// Watchdog sleep subroutine ---------------------------------------------------
void WDT_sleep(uint16 sleep)
{
	if (sleep <= 0) return;
	WDT_Delay = sleep;					// set WD decrementer
	while (WDT_Delay);					// wait for time to expire
	return;
} // end WDT_sleep()


//------------------------------------------------------------------------------
// draw die --------------------------------------------------------------------
//
//	IN:		die = 1-6 new die
//			*old_die = ptr to variable to store previous die #
//			x = left most column of die (must be divisible by 3)
//			y = bottom row of die
//
//  dot bit positions:  01    02
//                      04 08 10
//                      20    40
//
const uint8 dice[] = {0x08, 0x41, 0x49, 0x63, 0x6b, 0x77};

void drawDie(uint8 die, uint8* old_die, int16 x, int16 y)
{
	uint8 new_die = die = dice[die-1];
	uint8 change = *old_die ^ new_die;		// 1 = change
	int flag = 1;

	// draw any new dots
	new_die &= change;						// 1 = turn on
	do
	{
		if (new_die & 0x01) lcd_wordImage(spot_image, x, y+32, flag);
		if (new_die & 0x02) lcd_wordImage(spot_image, x+30, y+32, flag);
		if (new_die & 0x04) lcd_wordImage(spot_image, x, y+16, flag);
		if (new_die & 0x08) lcd_wordImage(spot_image, x+15, y+16, flag);
		if (new_die & 0x10) lcd_wordImage(spot_image, x+30, y+16, flag);
		if( new_die & 0x20) lcd_wordImage(spot_image, x, y, flag);
		if (new_die & 0x40) lcd_wordImage(spot_image, x+30, y, flag);

		// blank any invalid dots
		new_die = *old_die & change;		// 1 = blank
	} while (flag--);

	//	update old die
	*old_die = die;
	return;
} // end outDie()


#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
	P1IFG &= ~0x0f;                  // P1.0-3 IFG cleared
	WDT_debounce_cnt = DEBOUNCE_CNT; // enable debounce
} // end Port_1_ISR

//------------------------------------------------------------------------------
// Watchdog Timer ISR ----------------------------------------------------------
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	// decrement delay (if non-zero)
	if (WDT_Delay && (--WDT_Delay == 0));

	// decrement tone counter - turn off tone when 0
	if (WDT_Tone_Cnt && (--WDT_Tone_Cnt == 0))
	{
		TBCCR0 = 0;
	}

	if (--WDT_Sec_Cnt == 0)
	{
		WDT_Sec_Cnt = WDT_1SEC_CNT;		// reset counter
		LED_GREEN_TOGGLE;				// toggle green LED
	}


	// check for switch debounce
	if (WDT_debounce_cnt && (--WDT_debounce_cnt == 0))
	{
		lcd_backlight(ON);
		switches = (P1IN ^ 0x0f) & 0x0f;
		countdown = 50;
		timeout = 1300;
	}

} // end WDT_ISR(void)
