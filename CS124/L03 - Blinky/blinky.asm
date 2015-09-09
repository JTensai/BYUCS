;*******************************************************************************
;   CS 124 Lab 3 - Blinky
;
;   Author: Jordan Taylor
;   I declare that this is my own work.
;
;	Calculations:
;
;	2 + 2 + 2 + 4 + ((1 + 2)(45000)) + 4 + 5 + 100((1 + 2)(36000) + 2 + 3) + 2 =
;	21 + (3(45000)) + 100(3(36000) + 5) =
;	21 + 135000 + 100(108005) =
;	135021 + 10800500 =
;	135021 + 10800500 =
;	10935521 / 10 = 1093552.1 Hz / 1000000 = 1.0936 MHz
;
;	MCLK = 1.0936 MHz
;
;	1 + 1 + 1 + 1 + ((1 + 1)(45000)) + 1 + 1 + 100((1 + 1)(36000) + 1 + 1) + 1
;	7 + (2(45000)) + 100(2(36000) + 2)
;	90007 + 7200200 = 7290207
;	CPI = 10935521 / 7290207 = 1.5000
;
;
;	MIPS = MCLK / CPI = 1.0936 MHz / 1.5000 = 0.7291
;
;
;*******************************************************************************
           .cdecls C,"msp430.h"             ; MSP430

;------------------------------------------------------------------------------
            .text                           ; beginning of executable code
;------------------------------------------------------------------------------
RESET:      mov.w   #0x0280,SP              ; init stack pointer 					//cycles: 2
            mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; stop WDT								//cycles: 5
            bis.b   #0x01,&P1DIR            ; set P1.0 as output					//cycles: 4

tenth: 		.equ 36000
hundred: 	.equ 100
light:		.equ 45000

mainloop:	mov.w   #tenth,r13				; length of delay in subdelay			//cycles: 2
   			mov.w   #hundred,r14            ; run subdelay 100 times				//cycles: 2
            mov.w   #light,r15 			    ; length of time the light is on		//cycles: 2
			bis.b   #0x01,&P1OUT            ; turn light on							//cycles: 4

lightdelay: sub.w   #1,r15					;										//cycles: 1
			jnz     lightdelay				;										//cycles: 2
			bic.b   #0x01,&P1OUT            ; turn light off						//cycles: 4

delayloop:  call    #subdelay				;										//cycles: 5
			sub.w   #1,r14					;										//cycles: 1
			jnz		delayloop				;										//cycles: 2
            jmp     mainloop				;										//cycles: 2

subdelay:   sub.w   #1,r13					; subdelay length						//cycles: 1
			jnz		subdelay				;										//cycles: 2
			mov.w   #tenth,r13				; length of delay in subdelay			//cycles: 2
			ret								;										//cycles: 3


;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .word   RESET                   ; start address
            .end
