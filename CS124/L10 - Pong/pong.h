//	pong.h	08/07/2013
#ifndef PONG_H_
#define PONG_H_
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
// system equates
//
#define myCLOCK	12000000			// clock speed
#define WDT_CLK	32000				// 32 Khz WD clock (@1 Mhz)
#define	WDT_CTL	WDT_MDLY_32			// WDT SMCLK, ~32ms
#define	WDT_CPS	myCLOCK/WDT_CLK		// WD clocks / second count
#define DEBOUNCE_CNT	20

//------------------------------------------------------------------------------
// pong equates
//
#define WDT_ADC		WDT_CPS/10		// 100 ms

#define BEEP_COUNT		myCLOCK/3000
#define LONG_BEEP		myCLOCK/1300
#define BALL_START_SPEED	32000
#define POT_THRESHHOLD	3

#define BEEP TBCCR0 = BEEP_COUNT; TBCCR2 = BEEP_COUNT >> 1; TB0_tone_on = 20;

//------------------------------------------------------------------------------
// pong structs
//
typedef struct					// ball struct
{
	int x;						// horizontal position
	int y;						// vertical position
	int old_x;					// previous horizontal position
	int old_y;					// previous vertical position
} BALL;

typedef struct					// paddle struct
{
	int channel;				// ADC id
	int potValue;				// last ADC value
	int x;						// horizontal position
	int y;						// vertical position
	int old_y;					// previous vertical position
} PADDLE;

//------------------------------------------------------------------------------
// pong prototypes
//
void new_ball(int x, int y);
void delete_ball(BALL* ball);
int drawBall(BALL* ball);
void new_left_paddle(int channel, int x);
void new_right_paddle(int channel, int x);
void delete_paddle(PADDLE* paddle);
int drawPaddle(PADDLE* myPaddle);

void BEGIN_event();
void SCORE_event();
void ENDGAME_event();
void failure();
void victory();

void MOVE_BALL_event(BALL* ball);
void ADC_READ_event(PADDLE* paddle, char side);

#endif /*PONG_H_*/
