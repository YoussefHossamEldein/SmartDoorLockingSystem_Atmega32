 /*==================================================================================
 *
 * Module      : dc motor
 *
 * File Name   : dc_motor.c
 *
 * Description : source file for dc motor module
 *
 * Author      : Youssef Hossam
 *
 *====================================================================================
 */
/***************************** Includes Section ***************************************/
#include "dc_motor.h"
#include "../../MCAL/TIMER0_PWM/pwm.h"

/**************************** Function Definition Section *****************************/

/*
 * ===========================================================================================
 * 	@brief  : Function that initialize DC motor
 * 	@param  : function takes nothing
 * 	@retVal : function returns nothing
 * 	===========================================================================================
 */
void DcMotor_Init(void){
	GPIO_setupPinDirection(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
}
/*
 * ===========================================================================================
 * 	@brief  : Function that makes the DC motor to stop or  rotate in the desired direction
 * 	@param  : function takes the desired motor state and speed
 * 	@retVal : function returns nothing
 * 	===========================================================================================
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed){
	PWM_Timer0_Start(speed); //generate duty cycle
		switch(state){
		case MotorStop:
			GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
			GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
			break;
		case ClockWise:
			GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_HIGH);
			GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_LOW);
			break;
		case AntiClockWise:
			GPIO_writePin(MOTOR_IN1_PORT_ID,MOTOR_IN1_PIN_ID,LOGIC_LOW);
			GPIO_writePin(MOTOR_IN2_PORT_ID,MOTOR_IN2_PIN_ID,LOGIC_HIGH);
		}



}
