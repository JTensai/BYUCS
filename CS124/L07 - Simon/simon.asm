       .title  "simon.asm"
;*******************************************************************************
;  Project:  simon.asm
;  Author:   Jordan Taylor
;  All work done for this lab was done by me.
;
;  Description: A MSP430 assembly language program that plays the game of Simon.
;
;    1. Each round of the game starts by the LEDs flashing several times.
;    2. The colored LEDs (along with the associated tones) then flash one at
;       a time in a random sequence.
;    3. The push button switches are sampled and compared with the original
;       sequence of colors/tones.
;    4. The sampling the switches continues until the end of the sequence is
;       successfully reached or a mistake is made.
;    5. Some congratulatory tune is played if the sequence is correct or some
;       raspberry sound is output if a mistake is made.
;    6. If the complete sequence is successfully reproduced, the game is
;       repeated with one additional color/tone added to the sequence.
;       Otherwise, the game starts over with the default number of colors/tones.
;
;  Requirements:
;		Timer_B output (TB2) is used for hardware PWM of the transducer (buzzer).
;		Subroutines in at least one assembly and one C file are used by your program.
;		ALL subroutines must be correctly implemented using Callee-Save protocol.
;
;  Bonus:
;
;      -Port 1 interrupts are used to detect a depressed switch.
;	   -Use LCD to display round, score, highest score, and lowest score.
;	   -Turn on LCD backlight with any activity.
;	   -Turn off LCD backlight after 5 seconds of inactivity.
;
;*******************************************************************************
;   constants and equates
;
        .cdecls C,LIST,"msp430x22x4.h"
;
;*******************************************************************************
;
;                            MSP430F2274
;                  .-----------------------------.
;            SW1-->|P1.0^                    P2.0|<->LCD_DB0
;            SW2-->|P1.1^                    P2.1|<->LCD_DB1
;            SW3-->|P1.2^                    P2.2|<->LCD_DB2
;            SW4-->|P1.3^                    P2.3|<->LCD_DB3
;       ADXL_INT-->|P1.4                     P2.4|<->LCD_DB4
;        AUX INT-->|P1.5                     P2.5|<->LCD_DB5
;        SERVO_1<--|P1.6 (TA1)               P2.6|<->LCD_DB6
;        SERVO_2<--|P1.7 (TA2)               P2.7|<->LCD_DB7
;                  |                             |
;         LCD_A0<--|P3.0                     P4.0|-->LED_1 (Green)
;        i2c_SDA<->|P3.1 (UCB0SDA)     (TB1) P4.1|-->LED_2 (Orange) / SERVO_3
;        i2c_SCL<--|P3.2 (UCB0SCL)     (TB2) P4.2|-->LED_3 (Yellow) / SERVO_4
;         LCD_RW<--|P3.3                     P4.3|-->LED_4 (Red)
;   TX/LED_5 (G)<--|P3.4 (UCA0TXD)     (TB1) P4.4|-->LCD_BL
;             RX-->|P3.5 (UCA0RXD)     (TB2) P4.5|-->SPEAKER
;           RPOT-->|P3.6 (A6)          (A15) P4.6|-->LED 6 (R)
;           LPOT-->|P3.7 (A7)                P4.7|-->LCD_E
;                  '-----------------------------'
;
;******************************************************************************
;    Define some LED macros

    .asg "bis.b    #0x01,&P4OUT",LED1_ON
    .asg "bic.b    #0x01,&P4OUT",LED1_OFF

    .asg "bis.b    #0x02,&P4OUT",LED2_ON
    .asg "bic.b    #0x02,&P4OUT",LED2_OFF

    .asg "bis.b    #0x04,&P4OUT",LED3_ON
    .asg "bic.b    #0x04,&P4OUT",LED3_OFF

    .asg "bis.b    #0x08,&P4OUT",LED4_ON
    .asg "bic.b    #0x08,&P4OUT",LED4_OFF

myCLOCK .equ    8000000                 ; 1.2 Mhz clock
WDT_CTL .equ    WDT_MDLY_32             ; WD configuration (Timer, SMCLK, 32 ms)
WDT_CPI .equ    32000                   ; WDT Clocks Per Interrupt (@1 Mhz)
WDT_IPS .equ    myCLOCK/WDT_CPI         ; WDT Interrupts Per Second
STACK   .equ    0x0600                  ; top of stack

TONE    .equ    2000                    ; buzzer tone
DELAY   .equ    20*6                    ; delay count
SHORTDELAY .equ	20*2
VERYSHORT	.equ	30

TONE_C .equ myCLOCK/1308/12*10
TONE_D .equ myCLOCK/1386/12*10
TONE_E .equ myCLOCK/1468/12*10
TONE_F .equ myCLOCK/1556/12*10

;*******************************************************************************
;       RAM section
;
        .bss    WDTSecCnt,2             ; watchdog counts/second
        .bss    WDT_delay,2             ; watchdog delay counter
        .bss	rand,2

        .ref	RBX430_init
        .ref	rand16
        .ref	setrandSeed

;*******************************************************************************
;       ROM section
;
        .text                           ; code Section
reset:  mov.w   #STACK,SP               ; Initialize stack pointer
		mov.w	#1,r12
        call    #RBX430_init            ; set clock to 8Mhz

;       Set Watchdog interval
        mov.w   #WDT_CTL,&WDTCTL        ; Set Watchdog interrupt interval
        mov.w   #WDT_IPS,WDTSecCnt
        mov.b   #WDTIE,&IE1             ; Enable WDT interrupt

;       enable buzzer to use Timer B PWM
        clr.w   &TBR                    ; Timer B SMCLK, /1, up mode
        mov.w   #TBSSEL_2|ID_0|MC_1,&TBCTL
        mov.w   #OUTMOD_3,&TBCCTL2      ; output mode = set/reset
       	bis.b  #0x20,&P4DIR             ; P4.5 output (buzzer)
       	bis.b  #0x20,&P4SEL             ; select alternate output (TB2) for P4.5

;       enable interrupts
        bis.w   #GIE,SR                 ; enable interrupts

		call	#getSwitch				; start game when button pressed

		mov.w	#3,r13
start:	bis.b	#0x0f,&P4OUT
		mov.w	#TONE_D, r12
		call 	#toneON
		call 	#veryshort
		bic.b   #0x0f,&P4OUT			; turns light off
		call 	#toneOFF				; turns tone off
		call 	#veryshort
		dec.w	r13
		  jnz	start
		call	#delay


		mov.w	WDTSecCnt, rand				; moves random starting point into rand, based on current value of the watchdog second counter

begingame:	mov.w	#3, r10					; r10 keeps track of round number

roundloop:	mov.w	r10, r14				; r14 is step counter, set to round number at start of each round
			mov.w	rand, r12				; moves the random starting number into r12
			call	#setrandSeed			; sets sequence

showloop:	cmp.w	#0,r14
			  jeq	playgame
			call	#rand16
			and.w 	#0x03, r12				; makes r12 0,1,2,3

zero:		cmp.w	#0, r12
			  jne	one
			call 	#green
			jmp		clear

one:		cmp.w	#1, r12
			  jne	two
			call  	#orange
			jmp		clear

two:		cmp.w	#2, r12
			  jne	three
			call  	#yellow
			jmp		clear

three:		cmp.w	#3, r12
			call  	#red

clear:		dec.w	r14
			jmp		showloop


playgame:	mov.w	r10, r14				; r14 is step counter, set to round number at start of each round
			mov.w	rand, r12				; random starting spot
			call	#setrandSeed			; sets sequence

gameloop:	cmp.w	#0, r14
			  jeq	playdone
			call	#getSwitch				; switch will be either 1,2,4,8
			call	#rand16
			and.w 	#0x03, r12				; makes r12 0,1,2,3

chkzero:	cmp.w	#1, r4					; checks if button 1 was pressed
			  jne	chkone					; if button1 wasn't pressed, check if button 2 was pressed
			call	#green					; if button1 was pressed, turn its light and sound on
			cmp.w	#0, r12					; check if 0 is the first value in the sequence
			  jne	gameover				; if it is not, then game over
			dec.w	r14						; if it is, then move to the next sequence
			jmp		gameloop

chkone:		cmp.w	#2, r4
			  jne	chktwo
			call	#orange
			cmp.w	#1, r12
			  jne	gameover
			dec.w	r14
			jmp		gameloop

chktwo:		cmp.w	#4, r4
			  jne	chkthree
			call	#yellow
			cmp.w	#2, r12
			  jne	gameover
			dec.w	r14
			jmp		gameloop

chkthree:	cmp.w	#8, r4
			call	#red
			cmp.w	#3, r12
			  jne	gameover
			dec.w	r14
			jmp		gameloop


playdone:	add.w	#1,r10
			call	#shortdelay
			call	#winner
			call	#winner
			call	#winner
			call	#delay
			jmp		roundloop

gameover:	call 	#shortdelay
			call 	#loser
			call 	#loser
			call 	#loser
			call	#delay
			mov.w	#3, r13
			jmp		start


green:	push	r12
		mov.w	#1, r12
		  call	#LEDs
		mov.w	#TONE_C, r12
		call	#activate
		pop		r12
		ret

orange:	push	r12
		mov.w	#2, r12
		  call	#LEDs
		mov.w	#TONE_D, r12
		call	#activate
		pop		r12
		ret

yellow:	push	r12
		mov.w	#4, r12
		  call	#LEDs
		mov.w	#TONE_E, r12
		call	#activate
		pop		r12
		ret

red:	push	r12
		mov.w	#8, r12
		  call	#LEDs
		mov.w	#TONE_F, r12
		call	#activate
		pop		r12
		ret

loser:	push	r12
		mov.w	#8, r12
		  call	#LEDs
		mov.w	#TONE_F, r12
		call	#quick
		pop		r12
		ret

winner:	push	r12
		mov.w	#1, r12
		  call	#LEDs
		mov.w	#TONE_C, r12
		call	#quick
		pop		r12
		ret

activate:
		call 	#toneON
		call 	#delay
		bic.b   #0x0f,&P4OUT			; turns light off
		call 	#toneOFF				; turns tone off
		call 	#shortdelay
		ret

quick:	call 	#toneON
		call 	#veryshort
		bic.b   #0x0f,&P4OUT			; turns light off
		call 	#toneOFF				; turns tone off
		call 	#veryshort
		ret
;*******************************************************************************
;       turn on an LED
;
LEDs:	push	r12
        bic.b   #0x0f,&P4OUT            ; turn off LED's
        and.w	#0x0f,r12
        bis.b	r12,&P4OUT
        pop		r12
        ret

;*******************************************************************************
;       delay
;
delay:
		mov.w   #DELAY,WDT_delay        ; set WD delay counter
        bis.w   #LPM0,SR                ; goto sleep
        ret                             ; I'm awake - return

shortdelay:
		mov.w   #SHORTDELAY,WDT_delay   ; set WD delay counter
        bis.w   #LPM0,SR                ; goto sleep
        ret                             ; I'm awake - return

veryshort:
		mov.w   #VERYSHORT,WDT_delay    ; set WD delay counter
        bis.w   #LPM0,SR                ; goto sleep
        ret                             ; I'm awake - return
;*******************************************************************************
;       get switch subroutine
;
DB_DELAY	.equ	1200*10/13

getSwitch:	push	r15

deb_02:		mov.b	&P1IN,r4
			xor.b	#0x0f,r4
			and.b	#0x0f,r4
		  	  jne	deb_02

deb_04:		mov.w	#DB_DELAY, r15

deb_06:		mov.b	&P1IN,r4
			xor.b	#0x0f,r4
			and.b	#0x0f,r4
			  jeq	deb_04
			dec.w	r15
			  jne	deb_06
			pop		r15
			ret

;*******************************************************************************
;    enable/disable tone
;
toneON:
        push    r12
        rra.w   r12                     ; tone / 2
        mov.w   r12,&TBCCR2             ; use TBCCR2 as 50% duty cycle
        pop     r12
        mov.w   r12,&TBCCR0             ; start clock
        ret

toneOFF:
        mov.w   #0,&TBCCR0              ; Timer B off
        ret


;*******************************************************************************
;       Interrupt Service Routines
;
WDT_ISR:                                ; Watchdog Timer ISR
        cmp.w   #0,WDT_delay            ; delaying?
          jeq   WDT_02                  ; n
        dec.w   WDT_delay               ; y, wake-up processor?
          jne   WDT_02                  ; n
        bic.w   #LPM0,0(SP)             ; y, clear low-power bits for reti

WDT_02:
        dec.w   WDTSecCnt               ; decrement counter, 0?
          jne   WDT_04                  ; n
        mov.w   #WDT_IPS, WDTSecCnt     ; y, re-initialize counter
        xor.b   #0x40,&P4OUT            ; toggle P4.6

WDT_04: reti                            ; return from interrupt


;*******************************************************************************
;       Interrupt vector sections

        .sect   ".int10"                ; WDT vector section
        .word   WDT_ISR                 ; address of WDT ISR

        .sect   ".reset"                ; reset vector section
        .word   reset                   ; address of reset
        .end
