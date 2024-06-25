 /*
 * =============================================================================
 *
 *
 * File Name   : HMI_ECU.c
 *
 * Description : Source file for human machine interface ECU.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/
/********************************* Includes Section *********************************************/

#include <avr/io.h> /* To be able to use I-bit in SREG */
#include "util/delay.h" /* to use the delay function */
#include "HMI_CONTROL_ECUs_COMMUNICATION_COMMANDS.h"
#include "HAL/LCD/lcd.h"
#include "HAL/KEYPAD/keypad.h"
#include "MCAL/UART/uart.h"
#include "MCAL/TIMER1/timer1.h"

/********************************  Macro Definition Section *************************************/

#define PASSWORD_SIZE     5  /* Macro for the maximum size of the system password */
#define MAX_TRIES	      3  /* Macro for the maximum number of entry trials */
#define DOOR_OPENING_TIME 15 /* Macro for the time of opening the door */
#define DOOR_WAITING_TIME 5  /* Macro for the time of door being opened */
#define DOOR_CLOSING_TIME 15 /* Macro for the closure time of door */


/********************************* Functions Declaration Section *******************************/
/*
 * ===================================================================================
 * @brief  : Function that takes the password entered by the user.
 * @param  : Function takes nothing.
 * @retVal : Function returns nothing
 * ===================================================================================
 */
void taking_user_password(void);
/*
 * ======================================================================================
 * @brief  : Function that send the password the control ECU to be checked.
 * @param  : Function takes pointer that points to the array saving the password entered.
 * @retVal : Function returns nothing.
 * ======================================================================================
 */
void send_password(uint8 *PasswordPtr);
/*
 * ======================================================================================
 * @brief  : Function that increment the timer1 teks.
 * @param  : Function takes nothing.
 * @retVal : Function returns nothing.
 * ======================================================================================
 */
void Timer_Teks(void);
/*
 * ======================================================================================
 * @brief  : Function to make the system enter emergency mode.
 * @param  : Function takes nothing.
 * @retVal : Function returns nothing.
 * ======================================================================================
 */
void Alarm_mode(void);

/********************************* Global Declaration Section ***********************************/

uint8 ENTERED_PASSWORD[PASSWORD_SIZE]; /* Array to save the password being entered by the user */

uint8 user_password_index = ZERO;     /* counter for loops */

uint8 timer_teks = ZERO;			 /* variable to count timer teks */

/* ECU Timer Module configurations specific for the application */

Timer1_ConfigType Timer1_AppConfig = {0,31250,F_CPU_256,CTC_TOP_ICR1};
/********************************* Application Main Function ************************************/


void main(){

/********************************* Local Declaration Section ************************************/
uint8 password_creation_flag = FALSE; /* true if the password is first created successfully */

uint8 password_is_correct_flag = FALSE; /* true when password entered by the user is correct */

uint8 Trials = ZERO; /* variable to count the number of trials user enter the password */

uint8 MAIN_CONFIRMATION_MATCHED_FLAG = FALSE;
/****************************** Modules Configuration Section **********************************/

	/* UART module device  configurations based on the application */

	UART_ConfigType App_Config = {BIT_DATA_8,PARITY_DISABLED,ONE_STOP_BIT,9600};


/*************************** Call back functions for modules ************************************/
	/* set the call back function for TIMER1 to Timer_teks function */
	Timer1_setCallBack(Timer_Teks);

/****************************** Module Drivers Initialization Section ******************************/

	/* Initialize UART Driver responsible for sending data to CONRTOL_ECU */

		UART_init(&App_Config);

		SREG |= 1<<7;	/* Enabling Global Interrupts Service */


	/* initialize LCD Driver responsible for displaying data on 2x16 character LCD */

	LCD_init();
	LCD_displayString("DOOR LOCKING");
	LCD_moveCursor(1,4);
	LCD_displayString("SYSTEM");
	_delay_ms(2000); /* display the previous for 2 seconds */

	/* send Byte as a question for the CONTROL_ECU to know if the password is created or not */

	UART_sendByte(IS_PASSWORD_CREATED);

	/* check the answer from the CONTROL_ECU if the password is created */

	if( UART_recieveByte() == PASSWORD_CREATED){
			password_creation_flag = TRUE;
	}
	while(1){
		/* if the system contain a password */
		if(password_creation_flag){
			Trials = ZERO; /* Resetting number of tries */

			LCD_clearScreen();


			/* Display the options menu on the LCD of locker screen*/
			/* give choice to the user if he wants to open the door or change the password */
			LCD_displayStringRowColumn(0,0,"+ : Open Door");

			LCD_displayStringRowColumn(1,0,"- : Change Pass");

			switch(KEYPAD_getPressedKey()){

			case '+' :
				do {

					password_is_correct_flag = FALSE;
					LCD_clearScreen();

					LCD_displayString("plz enter pass:");

					LCD_moveCursor(1,4);

					/* take the new password from the user */
					/* send to control ECU to be ready for receiving a new password */
					UART_sendByte(TAKE_PASSWORD);

					taking_user_password(); /* take the password from user */

					send_password(ENTERED_PASSWORD); /* send password to control ECU */

					_delay_ms(15);
					LCD_clearScreen();

					if(UART_recieveByte()== PASSWORD_IS_CORRECT){	/* If the password is correct */

					password_is_correct_flag = TRUE;

					/* Ask Control ECU to unlock the door */
					UART_sendByte(OPEN_DOOR);

					Timer1_init(&Timer1_AppConfig);

					LCD_clearScreen();

					LCD_displayStringRowColumn(0,0,"Opening...");

					while(timer_teks != DOOR_OPENING_TIME);

					timer_teks = 0;

					Timer1_deinit();

					UART_sendByte(ENTRY_TIME);

					Timer1_init(&Timer1_AppConfig);

					LCD_displayStringRowColumn(0,0,"Welcome :)  ");

					while(timer_teks != DOOR_WAITING_TIME);

					timer_teks = 0;

					Timer1_deinit();

					UART_sendByte(CLOSING_TIME);

					Timer1_init(&Timer1_AppConfig);

					LCD_displayStringRowColumn(0,0,"Closing...  ");

					while(timer_teks != DOOR_CLOSING_TIME);

					timer_teks = 0;

					Timer1_deinit();

										}
					else {	/* If the password is wrong */
						Trials++;

						LCD_clearScreen();

						LCD_displayString("Wrong Password!");

						LCD_moveCursor(1,0);

						LCD_displayString("Tries remain : ");

						LCD_intgerToString(MAX_TRIES-Trials);

						_delay_ms(2000);

						}
				}while(Trials < MAX_TRIES && !password_is_correct_flag);
				if(password_is_correct_flag == FALSE){

				UART_sendByte(ALARM_MODE);		/* Ask Control ECU to make alarm */
					Alarm_mode();
				}
					break;

			case '-' :
				/* Asking for the password while the user didn't enter correct password
							 * have remaining attempts */

			do {
						password_is_correct_flag = FALSE;		/* Reseting password correctness flag */

						LCD_clearScreen();
						LCD_displayString("Enter Password:");
						LCD_moveCursor(1,0);
						LCD_displayString("     ");
						MAIN_CONFIRMATION_MATCHED_FLAG = TRUE;
						_delay_ms(500);

						/* Getting Control ECU ready to receive the password */
						UART_sendByte(TAKE_PASSWORD);

						/* Taking the password from the user using keypad */
						taking_user_password();

						/* Sending the password to Control ECU to check it */
						send_password(ENTERED_PASSWORD);

						_delay_ms(15);

						if(UART_recieveByte() == PASSWORD_IS_CORRECT){		/* If the password is correct */

							password_creation_flag = FALSE;		/* Make the user able to create new password */
							password_is_correct_flag = TRUE;
							}

						else{	/* If the password is wrong */

							Trials++;

							LCD_clearScreen();

							LCD_displayString("Wrong Password!");

							LCD_moveCursor(1,0);

							LCD_displayString("Tries remain: ");

							LCD_intgerToString(3-Trials);

							_delay_ms(2000);


								}
							}	while((Trials < MAX_TRIES) && !password_is_correct_flag);


							if(password_is_correct_flag == FALSE){

								UART_sendByte(ALARM_MODE);		/* Ask Control ECU to make alarm */
								Alarm_mode();
							}

					break;

			}

		}
		/* Password is still not created so we go to the other condition to create a password */
		else {

			LCD_clearScreen();

			LCD_displayString("Enter new pass:");

			LCD_moveCursor(1,4);

			LCD_displayString("      ");
			LCD_moveCursor(1,4);

			MAIN_CONFIRMATION_MATCHED_FLAG = TRUE;


			/* Send to control ECU to be ready to receive the main password */
			UART_sendByte(TAKE_MAIN_PASSWORD);


			taking_user_password(); /* take the main password */

			send_password(ENTERED_PASSWORD); /* send the main password to control ECU */

			LCD_clearScreen();
			/* take a confirmation password from the user */

			LCD_displayString("Re-Enter Pass:");

			LCD_moveCursor(1,4);

			_delay_ms(15);
			/* send to control ECU to be ready for receiving the confirmation password */

			UART_sendByte(TAKE_CONFIRMATION_PASSWORD);

			LCD_moveCursor(1,4);
			taking_user_password(); /* take the confirmation password */

			send_password(ENTERED_PASSWORD); /* send confirmation password to control ECU */

			_delay_ms(15);

			/* send to control ecu to ask if the main and confirmation passwords are matched */

			UART_sendByte(ARE_PASSWORDS_MATCHED);
			/* check the control ecu answer */
			 MAIN_CONFIRMATION_MATCHED_FLAG = UART_recieveByte();

			if (MAIN_CONFIRMATION_MATCHED_FLAG == PASSWORDS_ARE_MATCHED){



				password_creation_flag  = TRUE;
				MAIN_CONFIRMATION_MATCHED_FLAG = FALSE;
			}
			else {

				LCD_clearScreen();

				LCD_displayString("   Unmatched!  ");

				_delay_ms(1000);
				}


		}
	}

}
/***************************** Functions Definition Section *********************************/
/*
 * ===================================================================================
 * @brief  : Function that takes the password entered by the user.
 * @param  : Function takes nothing.
 * @retVal : Function returns nothing
 * ===================================================================================
 */
void taking_user_password(void){



	for(user_password_index = 0; user_password_index < PASSWORD_SIZE; user_password_index++){
		ENTERED_PASSWORD[user_password_index] = KEYPAD_getPressedKey();
		if((ENTERED_PASSWORD[user_password_index] <= '9') && (ENTERED_PASSWORD[user_password_index] >= '0'))
			{
				LCD_displayCharacter(ENTERED_PASSWORD[user_password_index]);   /* display the pressed keypad switch */
				_delay_ms(500); /* Pressing time */

				LCD_sendCommand(LCD_MOVE_CURSOR_LEFT);

							LCD_displayCharacter('*');

			}
			else
			{
				user_password_index--;
				_delay_ms(15);
			}

	}
	while(KEYPAD_getPressedKey() != '=');
}
/*
 * ======================================================================================
 * @brief  : Function that send the password the control ECU to be checked.
 * @param  : Function takes pointer that points to the array saving the password entered.
 * @retVal : Function returns nothing.
 * ======================================================================================
 */
void send_password(uint8 *PasswordPtr){

	for(user_password_index = 0; user_password_index < PASSWORD_SIZE; user_password_index++){
		UART_sendByte(PasswordPtr[user_password_index]);
		_delay_ms(15);
	}
}
/*
 * ======================================================================================
 * @brief  : Function that increment the timer1 teks.
 * @param  : Function takes nothing.
 * @retVal : Function returns nothing.
 * ======================================================================================
 */
void Timer_Teks(void){
	timer_teks++;
}
/*
 * ======================================================================================
 * @brief  : Function to make the system enter emergency mode.
 * @param  : Function takes nothing.
 * @retVal : Function returns nothing.
 * ======================================================================================
 */
void Alarm_mode(void){
	LCD_clearScreen();
	Timer1_init(&Timer1_AppConfig);
	while(timer_teks != 60){
		LCD_displayString("    WARNING!    ");
		_delay_ms(500);
		LCD_clearScreen();
		_delay_ms(500);
	}
	timer_teks = 0;
	Timer1_deinit();
}
