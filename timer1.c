 /*============================================================================
 *
 * Module      : TIMER1
 *
 * File Name   : timer1.c
 *
 * Description : Source file for TIMER1 module.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/
/***************************************Includes Section *************************************/
#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>/* To be able to implement Timer1 ISR */


/********************************** Global Declarations Section******************************/
/* Global pointer to be used in the call back functions */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;


/******************************* Interrupt Service Routines Section  ************************/
ISR (TIMER1_OVF_vect){
	/* validate if the pointer is not NULL to avoid segmentation faults and program crashing */
	if(NULL_PTR != g_callBackPtr){
		(*g_callBackPtr)();
	}
}
ISR (TIMER1_COMPA_vect){
	/* validate if the pointer is not NULL to avoid segmentation faults and program crashing */
	if(NULL_PTR != g_callBackPtr){
			(*g_callBackPtr)();
		}
}


/***********************************Function Definition Section *****************************/
/*
 * ==============================================================================
 * @brief   :  Function to initialize the Timer driver.
 * @param   :  Function takes pointer to the configuration structure with type.
 * retVal   :  Function returns nothing.
 * ==============================================================================
 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr){
	/* Set timer intitial value */
		TCNT1 = Config_Ptr->initial_value;

	/*
	 * *********************************************************************
	 *  Configure timer control register TCCR1A
	 * 1. Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 * 2. FOC1A=1 FOC1B=0
	 * 3. CTC Mode WGM10=0 WGM11=0 (Mode Number 4).
	 * *******************************************************************
	 */
	TCCR1A = (1<<FOC1A);
	/*
	 * ******************************************************************************************
	 *  Configure timer control register TCCR1B
     *
	 * 	Notice that timer mode and prescaler are specific and configurable up to the application.
	 *
	 * *****************************************************************************************
	 */
	TCCR1B = Config_Ptr->prescaler | Config_Ptr->mode;


	if(Config_Ptr->mode == NORMAL_MODE){
		/* Enable Timer1 Overflow Interrupt */
		TIMSK |= (1<<TOIE1);
	}
	else {
		/* Set timer compare value */
		OCR1A = Config_Ptr->compare_value;

		/* Enable Timer1 Compare A Interrupt */
		TIMSK |= (1<<OCIE1A);
	}
}
/*
 * ==============================================================================
 * @brief   : Function to disable the Timer1.
 * @param   : Function takes nothing.
 * retVal   : Function returns nothing.
 * ==============================================================================
 */
void Timer1_deinit(void){
	TCNT1 = 0;
	TCCR1B = 0;
}
/*
 * ==============================================================================
 * @brief   : Function to set the Call Back function address.
 * @param   : Function takes  pointer to Call Back function
 * retVal   : Function returns nothing.
 * ==============================================================================
 */
void Timer1_setCallBack(void(*a_ptr)(void)){
	g_callBackPtr = a_ptr;
}
