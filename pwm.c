 /* ==============================================================================
 *
 * Module        : PWM mode in TIMER0 Module
 *
 * File Name     : pwm.c
 *
 * Description   : source file for PWM mode in TIMER0 module
 *
 * Author        : Youssef Hossam
 *
 *==================================================================================
 */
/************************ Includes Section ****************************/

#include "pwm.h"
#include <avr/io.h>

/************************ Functions Definition Section ****************/

/*
 * ==========================================================
 * 	@brief  : Function that configure TIMER0 in PWM mode
 * 	@param  : function takes the required duty cycle
 * 	@retVal : function returns nothing
 * 	==========================================================
 */
void PWM_Timer0_Start(uint8 set_duty_cycle){
	TCNT0 = 0; /* Start from 0 */
	switch(set_duty_cycle){
	case 0:
		OCR0 = 0;
		break;
	case 25:
		OCR0  = 64;
		break;
	case 50:
		OCR0 = 128;
		break;
	case 75:
		OCR0 = 192;
		break;
	case 100:
		OCR0 = 255;
		break;
	}

	DDRB  = DDRB | (1<<PB3); //set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.
	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 |= (1<<WGM01) | (1<<WGM00) | (1<<COM01) | (1<<CS01);


}
