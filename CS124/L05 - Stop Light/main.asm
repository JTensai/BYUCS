;*******************************************************************************
;   CS 124 Lab 5 - Stop Light
;
;	Jordan Taylor
;	I hereby certify that the code presented is my own work.
;
;*******************************************************************************
			.cdecls	C,"msp430.h"			; MSP430

			.text

RESET:		mov.w	#0x0280,SP				; initialize stack pointer
			mov.w	#WDTPW+WDTHOLD,&WDTCTL	; stop the watchdog

tenth		.equ	35600
green		.equ	50
red			.equ	100
slowdelay	.equ	10
slowcount	.equ	7
fastdelay	.equ	2
fastcount	.equ	21

mainloop:	mov.w	#green,r14
			bis.b	#0x40,&P1DIR
			bis.b	#0x40,&P1OUT
			call	#greenlight

			mov.w	#slowcount,r11
			mov.w	#slowdelay,r12
			call	#slowblink

			mov.w	#fastcount,r11
			mov.w	#fastdelay,r12
			call	#fastblink

			mov.w	#red,r13
			bis.b	#0x01,&P1DIR
			bis.b	#0x01,&P1OUT
			call	#redlight

			jmp		mainloop

greenlight:	call	#subdelay
			sub.w	#1,r14
			jnz		greenlight
			ret

slowloop:	call	#subdelay
			sub.w	#1,r12
			jnz		slowloop
			mov.w	#slowdelay,r12

slowblink:	xor.b	#0x40,&P1OUT
			sub.w	#1,r11
			jnz		slowloop
			bic.b	#0x40,&P1OUT
			ret

fastloop:	call 	#subdelay
			sub.w	#1,r12
			jnz		fastloop
			mov.w	#fastdelay,r12
			xor.b	#0x40,&P1OUT

fastblink:	sub.w	#1,r11
			jnz		fastloop
			bic.b	#0x40,&P1OUT
			ret

redlight:	call 	#subdelay
			sub.w	#1,r13
			jnz		redlight
			bic.b	#0x01,&P1OUT
			ret

subdelay:	push	r15
			mov.w	#tenth,r15
thedelay:	sub.w	#1,r15					; tenth of a second delay
			jnz		thedelay
			pop		r15
			ret


			.sect	".reset"
			.word	RESET
			.end
