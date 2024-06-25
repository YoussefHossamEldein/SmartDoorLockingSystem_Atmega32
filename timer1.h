 /*===========================================================================
 *
 * Module      : TIMER1
 *
 * File Name   : timer1.h
 *
 * Description : Header file for TIMER1 module.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/
#ifndef TIMER1_H_
#define TIMER1_H_

/*************************** Includes Section *********************************/
#include "../../STD_UTILITIES/std_types.h"


/*************************** Data Types Section *******************************/
/*
 * ********************************************************
 * defined enum for timer1 different prescaler values.
 * note that F_Timer = F_CPU/Prescaler_Value.
 * *******************************************************
 */
typedef enum {
	TIMER_STOP,
	NO_PRESCALING,
	F_CPU_8,
	F_CPU_64,
	F_CPU_256,
	F_CPU_1024,
	CLOCK_FALLING_EDGE,
	CLOCK_RISING_EDGE,
}Timer1_Prescaler;
/*
 * ********************************************************
 * defined enum for timer1 different modes.
 * *******************************************************
 */
typedef enum {
	 NORMAL_MODE,
	/*PWM_PHASE_CORRECT_8_BIT,
	PWM_PHASE_CORRECT_9_BIT,
	PWM_PHASE_CORRECT_10_BIT,
	CTC_TOP_OCR1A,
	PWM_FAST_8_BIT,
	PWM_FAST_9_BIT,
	PWM_FAST_10_BIT,
	PWM_PHASE_FREQUENCY_CORRECT,
	PWM_PHASE_FREQUENCY_CORRECT_TOP_ICR1,
	PWM_PHASE_FREQUENCY_CORRECT_TOP_OCR1A,
	PWM_PHASE_CORRECT_TOP_ICR1A,
	PWM_PHASE_CORRECT_TOP_OCR1A,*/

	CTC_TOP_ICR1 = 8,
	/*
	PWM_FAST_TOP_ICR1,
	PWM_FAST_TOP_OCR1A,
	*/
}Timer1_Mode;
/*
 * ********************************************************
 * defined type struct for timer1 configuration .
 * *******************************************************
 */
typedef struct {
	uint16 initial_value;
	uint16 compare_value;
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer1_ConfigType;


/************************** Macro Definition Section **************************/




/************************* Macro Functions Section ****************************/



/************************ Functions Declaration Section ***********************/
/*
 * ==============================================================================
 * @brief   :  Function to initialize the Timer driver.
 * @param   :  Function takes pointer to the configuration structure with type.
 * retVal   :  Function returns nothing.
 * ==============================================================================
 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr);
/*
 * ==============================================================================
 * @brief   : Function to disable the Timer1.
 * @param   : Function takes nothing.
 * retVal   : Function returns nothing.
 * ==============================================================================
 */
void Timer1_deinit(void);
/*
 * ==============================================================================
 * @brief   : Function to set the Call Back function address.
 * @param   : Function takes  pointer to Call Back function
 * retVal   : Function returns nothing.
 * ==============================================================================
 */
void Timer1_setCallBack(void(*a_ptr)(void));


#endif /* TIMER1_H_ */
