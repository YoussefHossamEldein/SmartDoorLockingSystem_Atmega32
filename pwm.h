 /* ==============================================================================
 *
 * Module       : PWM mode in TIMER0 Module
 *
 * File Name    : pwm.h
 *
 * Description  : header file for PWM mode in TIMER0 module
 *
 * Author       : Youssef Hossam
 *
 *==================================================================================
 */

#ifndef PWM_H_
#define PWM_H_
/************************ Includes Section ****************************/
#include "../../STD_UTILITIES/std_types.h"
/************************ Data Types Section **************************/


/************************ Macro Definition Section ********************/


/************************ Macro Functions Section *********************/


/************************ Functions Declaration Section ***************/
/*
 * ==========================================================
 * 	@brief  : Function that configure TIMER0 in PWM mode
 * 	@param  : function takes the required duty cycle
 * 	@retVal : function returns nothing
 * 	==========================================================
 */
void PWM_Timer0_Start(uint8 set_duty_cycle);

#endif /* PWM_H_ */
