 /*==================================================================================
 *
 * Module      : dc motor
 *
 * File Name   : dc_motor.h
 *
 * Description : header file for dc motor module
 *
 * Author      : Youssef Hossam
 *
 *====================================================================================
 */
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_
/************************* Includes Section *********************************/
#include "../../STD_UTILITIES/std_types.h"
#include "../../MCAL/GPIO/gpio.h"


/************************ Data Types Declaration  Section ****************************/
	typedef enum {
		MotorStop,
		ClockWise,
		AntiClockWise
	}DcMotor_State;
/************************ Macro Definitions Section ************************/
#define MOTOR_IN1_PORT_ID			PORTD_ID
#define MOTOR_IN1_PIN_ID			PIN2_ID


#define MOTOR_IN2_PORT_ID           PORTD_ID
#define MOTOR_IN2_PIN_ID			PIN3_ID

/********************** Functions Declaration Section **********************/
/*
 * ===========================================================================================
 * 	@brief  : Function that initialize DC motor
 * 	@param  : function takes nothing
 * 	@retVal : function returns nothing
 * 	===========================================================================================
 */
void DcMotor_Init(void);
/*
 * ===========================================================================================
 * 	@brief  : Function that makes the DC motor to stop or  rotate in the desired direction
 * 	@param  : function takes the desired motor state and speed
 * 	@retVal : function returns nothing
 * 	===========================================================================================
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */
