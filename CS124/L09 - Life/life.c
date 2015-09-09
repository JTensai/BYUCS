//	life.c	07/29/2013
//******************************************************************************
//  The Game of Life
//
//	Jordan Taylor
//	All work for this lab was done by me.
//
//  Lab Description:
//
//  The universe of the Game of Life is an infinite two-dimensional orthogonal
//  grid of square cells, each of which is in one of two states, alive or dead.
//  With each new generation, every cell interacts with its eight neighbors,
//  which are the cells horizontally, vertically, or diagonally adjacent
//  according to the following rules:
//
//  1. A live cell stays alive (survives) if it has 2 or 3 live neighbors,
//     otherwise it dies.
//  2. A dead cell comes to life (birth) if it has exactly 3 live neighbors,
//     otherwise it stays dead.
//
//  An initial set of patterns constitutes the seed of the simulation. Each
//  successive generation is created by applying the above rules simultaneously
//  to every cell in the current generation (ie. births and deaths occur
//  simultaneously.)  See http://en.wikipedia.org/wiki/Conway's_Game_of_Life
//
//  Author:		Paul Roper, Brigham Young University
//  Revisions:	June 2013	Original code
//              07/12/2013	life_pr, life_cr, life_nr added
//              07/23/2013	generations/seconds added
//              07/29/2013	100 second club check
//
//  Built with Code Composer Studio Version: 5.3.0.00090
//******************************************************************************
//  Lab hints:
//
//  The life grid (uint8 life[80][10]) is an 80 row x 80 column bit array.  A 0
//  bit is a dead cell while a 1 bit is a live cell.  The outer cells are always
//  dead.  A boolean cell value is referenced by:
//
//         life[row][col >> 3] & (0x01 << (col % 8))
//
//  Each life cell maps to a 2x2 lcd pixel.
//
//                    00       01       02           07       08      09
//  life[79][0-9]  00000000 00000000 00000000 ... 00000000 00000000 00000000
//  life[78][0-9]  0xxxxxxx xxxxxxxx xxxxxxxx ... xxxxxxxx xxxxxxxx xxxxxxx0
//  life[77][0-9]  0xxxxxxx xxxxxxxx xxxxxxxx ... xxxxxxxx xxxxxxxx xxxxxxx0
//                 ...
//  life[02][0-9]  0xxxxxxx xxxxxxxx xxxxxxxx ... xxxxxxxx xxxxxxxx xxxxxxx0
//  life[01][0-9]  0xxxxxxx xxxxxxxx xxxxxxxx ... xxxxxxxx xxxxxxxx xxxxxxx0
//  life[00][0-9]  00000000 00000000 00000000 ... 00000000 00000000 00000000
//
//  The next generation can be made directly in the life array if the previous
//  cell values are held in the life_pr (previous row), life_cr (current row),
//  and life_nr (next row) arrays and used to count cell neighbors.
//
//  life_pr[0-9]  0xxxxxxx xxxxxxxx xxxxxxxx ... xxxxxxxx xxxxxxxx xxxxxxx0
//  life_cr[0-9]  0xxxxxxx xxxxxxxx xxxxxxxx ... xxxxxxxx xxxxxxxx xxxxxxx0
//  life_nr[0-9]  0xxxxxxx xxxxxxxx xxxxxxxx ... xxxxxxxx xxxxxxxx xxxxxxx0
//
//  Begin each new row by moving life_cr values to life_pr, life_nr values to
//  life_cr, and loading life_nr with the row-1 life values.  Then for each
//  column, use these saved values in life_pr, life_cr, and life_nr to
//  calculate the number of cell neighbors of the current row and make changes
//  directly in the life array.
//
//  life_pr[0-9] = life_cr[0-9]
//  life_cr[0-9] = life_nr[0-9]
//  life_nr[0-9] = life[row-1][0-9]
//
//******************************************************************************
//******************************************************************************
// includes --------------------------------------------------------------------
#include "msp430x22x4.h"
#include <stdlib.h>
#include <string.h>
#include "RBX430-1.h"
#include "RBX430_lcd.h"
#include "life.h"

// defined constants -----------------------------------------------------------
#define myCLOCK			16000000			// 1.2 Mhz clock
#define	WDT_CTL			WDT_MDLY_32		// WD configuration (SMCLK, ~32 ms)
#define WDT_CPI			32000			// WDT Clocks Per Interrupt (@1 Mhz)
#define	WDT_1SEC_CNT	myCLOCK/WDT_CPI	// WDT counts/second
#define DEBOUNCE_CNT	20

// global variables ------------------------------------------------------------
volatile uint16 WDT_Sec_Cnt;			// WDT second counter
volatile uint16 seconds;				// # of seconds
volatile uint16 generation;				// current generation
volatile uint16 WDT_debounce_cnt;

volatile int button;					// tracks last button pressed
volatile int i, i2;

uint8 life[NUM_ROWS][NUM_COLS/8];		// 80 x 80 life grid
uint8 life_pr[NUM_COLS/8];				// previous row
uint8 life_cr[NUM_COLS/8];				// current row
uint8 life_nr[NUM_COLS/8];				// next row

//------------------------------------------------------------------------------
// main ------------------------------------------------------------------------
void main(void)
{

	RBX430_init(_16MHZ);				// init board
	ERROR2(lcd_init());					// init LCD

	// configure Watchdog
	WDTCTL = WDT_CTL;					// Set Watchdog interval
	WDT_Sec_Cnt = WDT_1SEC_CNT;			// set WD 1 second counter
	IE1 |= WDTIE;						// enable WDT interrupt

	// enable switch interrupts
	P1IES |= 0x0f;						// interrupt on h to l transition
	P1IE |= 0x0f;						// enable P1.0-3 interrupts


	lcd_clear();						// clear LCD
	lcd_backlight(ON);					// turn on LCD backlight
	lcd_rectangle(0, 0, NUM_COLS*2, NUM_ROWS*2, 1);	// draw border

	__bis_SR_register(GIE);				// enable interrupts

	// output splash screen & wait for switch
	lcd_wordImage(life_image, (159-126)/2, 50, 1);
	lcd_mode(LCD_PROPORTIONAL | LCD_2X_FONT);
	lcd_cursor(10, 20);
	printf("Press Any Key");
	lcd_mode(0);

	while (1) if (button) break;		// wait for button press

	while (1)							// new pattern seed
	{
		WDT_Sec_Cnt = WDT_1SEC_CNT;		// set WD 1 second counter

		while (button == 1){
			button = 0;
			clear_screen();

			draw_rle_pattern(65, 30, gosper_glider_gun);
			draw_rle_pattern(60, 5, pulsar);
			draw_rle_pattern(4, 7, beacon_maker);
			draw_rle_pattern(4, 30, hexapole);
			draw_rle_pattern(4, 40, hexapole);
			draw_rle_pattern(4, 50, hexapole);
			draw_rle_pattern(4, 60, hexapole);
			draw_rle_pattern(15, 65, loafer);

			play();

			while (1) {if (button) break;}
		}

		while (button == 2){
			button = 0;
			clear_screen();
			draw_rle_pattern(60, 5, pulsar);            // sun
			draw_rle_pattern(31, 2, bird);              // bird
			draw_rle_pattern(40, 20, hwss);             // bigger bird
			draw_rle_pattern(11, 29, glider);           // blows bird apart
			draw_rle_pattern(65, 65, loafer);

			for (i2 = 2; i2 < 78; i2 += 3)                 // draw ground
			{
			   draw_rle_pattern(7, i2, block);           // block
			}

			play();

			while (1) {if (button) break;}
		}

		while (button == 4){
			button = 0;
			clear_screen();
			draw_rle_pattern(65, 10, gosper_glider_gun);
			draw_rle_pattern(40, 10, lwss);             // LWSS

			draw_rle_pattern(20, 10, pulsar);           // pulsar
			draw_rle_pattern(20, 35, pulsar);           // pulsar
			draw_rle_pattern(20, 60, pulsar);           // pulsar

			draw_rle_pattern(10, 10, block);            // block
			draw_rle_pattern(10, 18, beehive);          // beehive
			draw_rle_pattern(10, 26, loaf);             // loaf
			draw_rle_pattern(10, 34, boat);             // boat
			draw_rle_pattern(10, 42, toad);             // toad
			draw_rle_pattern(10, 50, beacon);           // beacon
			draw_rle_pattern(10, 58, blinker);          // blinker
			draw_rle_pattern(10, 66, by_flop);          // by_flop

			play();

			while (1) {if (button) break;}
		}

		while (button == 8){
			button = 0;
			clear_screen();
			draw_rle_pattern(40, 40, hexapole);

			play();

			while (1) {if (button) break;}
		}
	}
} // end main()

void clear_screen(void){
	lcd_clear();
	//lcd_volume(250);
	seconds = 0;					// clear second counter
	generation = 0;					// generation counter
	uint16 row, col;

	for (row = NUM_ROWS-2; row; --row)
	{
		for (col = NUM_COLS-2; col; --col)
		{
			life[row][col] = 0;
		}
	}
	lcd_rectangle(0, 0, NUM_COLS*2, NUM_ROWS*2, 1);
}


void play(void){
	while (seconds < 100){
		memcpy(life_pr, life[79], sizeof(uint8)*10);
		memcpy(life_cr, life[78], sizeof(uint8)*10);
		memcpy(life_nr, life[77], sizeof(uint8)*10);

		uint16 row, col;
		int topleft = 0, top = 0, topright = 0, left = 0, current = 0, right = 0, bottomleft = 0, bottom = 0, bottomright = 0, neighbors = 0;

		// for each live row (78 down to 1)
		for (row = NUM_ROWS-2; row; --row)
		{
			//increment row, but not the first time
			if (life_cr != life[78]){
				memcpy(life_pr, life_cr, sizeof(uint8)*10);
				memcpy(life_cr, life_nr, sizeof(uint8)*10);
				memcpy(life_nr, life[row-1], sizeof(uint8)*10);
			}
			// for each live column (78 down to 1)
			for (col = NUM_COLS-2; col; --col)
			{
				if (row == 78 && col == 78){
					topleft = cell_value(life_pr, col - 1);
					top = cell_value(life_pr, col);
					topright = cell_value(life_pr, col + 1);

					left = cell_value(life_cr, col - 1);
					current = cell_value(life_cr, col);
					right = cell_value(life_cr, col + 1);

					bottomleft = cell_value(life_nr, col - 1);
					bottom = cell_value(life_nr, col);
					bottomright = cell_value(life_nr, col + 1);
				}
				else {
					topright = top;
					top = topleft;
					topleft = cell_value(life_pr, col - 1);

					current = left;
					left = cell_value(life_cr, col - 1);
					right = cell_value(life_cr, col + 1);

					bottomright = bottom;
					bottom = bottomleft;
					bottomleft = cell_value(life_nr, col - 1);
				}

				neighbors = topleft + top + topright + left + right + bottomleft + bottom + bottomright;

				if (current == 1){
					if (neighbors == 2 || neighbors == 3){
						cell_birth(row, col);
					}
					else {
						cell_death(row, col);
					}
				}
				else {
					if (neighbors == 3){
						cell_birth(row, col);
					}
				}
			}
		}
		LED_RED_ON
		int score = generation/seconds;
		int prev_score;
		if (seconds == 0){
			score = generation/4;
		}
		if (prev_score > 9){
			lcd_cursor(146, 1);
			printf("  ");
		}
		if (score < 10){
			lcd_cursor(152, 1);
		}
		else {
			lcd_cursor(146, 1);
		}
		printf("%d", score);
		prev_score = score;
		lcd_cursor(2, 1);							// output life generation
		printf("%d/%d", ++generation, seconds);
		LED_RED_OFF
		if (button) break;
	}
}

void draw_rle_pattern(int row, int col, const uint8* object){
	const uint8* str = object;
	const uint8* ptr;
	int number = 0;
	int new_row;
	int current_col = col;
	for (ptr = str; *ptr; ptr++){
		if (*ptr != 'y') continue;
		ptr = ptr + 4;
		if ((*ptr < '0') && (*ptr > '9')) continue;		// Find beginning of number
		while ((*ptr >= '0') && (*ptr <= '9')){
			number = number * 10 + (*ptr++ - '0');		// Convert to decimal
		}
		new_row = number + row;
		break;
	}

	while (*ptr++ != '\n');

	while (*ptr != '!'){
		number = 0;
		while ((*ptr >= '0') && (*ptr <= '9')){
			number = number * 10 + (*ptr++ - '0');		// Convert to decimal
		}
		if (*ptr == 'o'){
			if (number == 0){
				cell_birth(new_row, current_col);
				current_col = current_col + 1;
			}
			else {
				for (i = 0; i < number; i++){
					cell_birth(new_row, current_col);
					current_col = current_col + 1;
				}
			}
		}
		else if (*ptr == 'b'){
			if (number == 0){
				//cell_death(new_row, current_col);
				current_col = current_col + 1;
			}
			else {
				for (i = 0; i < number; i++){
					//cell_death(new_row, current_col+i);
					current_col = current_col + 1;
				}
			}
		}
		else if (*ptr == '$'){
			if (number == 0){
				new_row--;
				current_col = col;
			}
			else {
				for (i = 0; i < number; i++){
					new_row--;
					current_col = col;
				}
			}
		}
		*ptr++;
	}
}

//------------------------------------------------------------------------------
// Port 1 interrupt service routine ---------------------------------
#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void)
{
   P1IFG &= ~0x0f;                  // P1.0-3 IFG cleared
   WDT_debounce_cnt = DEBOUNCE_CNT; // enable debounce
} // end Port_1_ISR

// Watchdog Timer ISR ----------------------------------------------------------
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	if (--WDT_Sec_Cnt == 0)
	{
		WDT_Sec_Cnt = WDT_1SEC_CNT;		// reset counter
		++seconds;						// up seconds counter
		LED_GREEN_TOGGLE;				// toggle green LED
	}

	// check for switch debounce
	if (WDT_debounce_cnt && (--WDT_debounce_cnt == 0))
	{
		button = (P1IN ^ 0x0f) & 0x0f;
	}
} // end WDT_ISR(void)
