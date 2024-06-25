 /*============================================================================
 *
 *
 * File Name   : CONTROL_ECU.h
 *
 * Description : Source file for the control ecu.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/

/******************************* Includes Section **********************************************/


#include "HMI_CONTROL_ECUs_COMMUNICATION_COMMANDS.h"
#include "avr/io.h"
#include "util/delay.h"
#include "HAL/EXTERNAL_EEPROM/external_eeprom.h"
#include "HAL/BUZZER/buzzer.h"
#include "HAL/DC_MOTOR/dc_motor.h"
#include "HAL/LCD/lcd.h"
#include "MCAL/I2C/twi.h"
#include "MCAL/UART/uart.h"
#include "MCAL/TIMER1/timer1.h"


/*****************************  Macro Definition Section **************************************/

#define PASSWORD_SIZE   5	 /* Macro for the password maximum size */
#define DOOR_OPENING_TIME 15 /* Macro for the time of opening the door */
#define DOOR_WAITING_TIME 5  /* Macro for the time of door being opened */
#define DOOR_CLOSING_TIME 15 /* Macro for the closure time of door */

/****************************** Functions Declaration Section **********************************/
/*
 * ==================================================================================
 * @brief   : Function to receive the password entered by the user in the HMU ECU.
 * @Param   : Function takes ptr for the array that save the password.
 * @retVal  : Function returns nothing.
 * ==================================================================================
 */
void receive_password(uint8 *PasswordPtr);
/*
 * ======================================================================================
 * @brief  : Function that increment the timer1 teks.
 * @param  : Function takes nothing.
 * @retVal : Function returns nothing.
 * ======================================================================================
 */
void timer1_tick(void);




/******************************* Global Declaration Section ***********************************/



uint8 Timer_teks = ZERO; /* variable to count timer teks */

/* Timer1 Configuration specific to this application */

Timer1_ConfigType Timer1_AppConfig = {0,31250,F_CPU_256,CTC_TOP_ICR1};


/********************************  Main Function **********************************************/

void main(){
/****************************** Local Declaration Section *************************************/

uint8 password_creation_flag = FALSE; /* password is created flag */

uint8 Opening_Password = ZERO;

uint8 Main_Password[PASSWORD_SIZE]; /* Array to save the main password upon creation case */

uint8 Confirmation_Password[PASSWORD_SIZE];/* Array to save the main password upon confirmation case */

uint8 MAIN_CONFIRMATION_MATCHED_FLAG; /* Main and confirmation passwords are matched */

uint8 user_password_index = ZERO; /* counter for loops */


/****************************** Modules Configuration Section *********************************/

	/* UART module configurations specific to this application */

	UART_ConfigType App_Config = {BIT_DATA_8,PARITY_DISABLED,ONE_STOP_BIT,9600};

	/* I2C module configurations specific to this application */

	TWI_ConfigType	I2C_AppConfig = {0x01 , BAUD_RATE_400KHZ};



	SREG |= 1<<7;		/* Enabling Global Interrupts Service */

/****************************** Modules Device Drivers Initialization Section *******************/

	/* initialize uart module with application configurations required */

	UART_init(&App_Config);

	TWI_init(&I2C_AppConfig);

	Timer1_setCallBack(timer1_tick);

	/* initialize buzzer module device  driver */

	Buzzer_init();

	/* initialize DC Motor Module device Driver */

	DcMotor_Init();

	/* initialize LCD Module Device Driver */
	LCD_init();

	LCD_displayString("Door Locked");

	EEPROM_writeByte(PASSWORD_CREATION_LOCATION , 0);

	_delay_ms(15);

	while(1){


		/* switch on the task wanted by the hmi ECU and do the required */

		switch(UART_recieveByte()){

		case IS_PASSWORD_CREATED : /* checking if the system password is created */

			password_creation_flag = FALSE; /* if the password is created the flag will be TRUE */

			/* reading password creation flag from EEPROM */

			EEPROM_readByte(PASSWORD_CREATION_LOCATION,&password_creation_flag);

			/* check if the password is created to inform the HMI */

			if (password_creation_flag){ /* Password is created */

				UART_sendByte(PASSWORD_CREATED); /* Telling the HMI that password is created */

				LCD_clearScreen();

				LCD_displayString("Password Created");

				_delay_ms(2000);
			}
			else /* password is not created */
			{
			UART_sendByte(PASSWORD_NOT_CREATED);/* Telling the HMI that password isn't created */

			LCD_clearScreen();

			LCD_displayString("Password isn't");

			LCD_displayStringRowColumn(1,1,"Created");

			_delay_ms(2000);

			}
			break;


		case TAKE_MAIN_PASSWORD : /* user is creating a password */


			LCD_clearScreen();

			LCD_displayString("Taking main");

			LCD_displayStringRowColumn(1,1,"Password...");

			_delay_ms(1000);

			receive_password(Main_Password);

			break;

		case TAKE_CONFIRMATION_PASSWORD : /* user making a confirmation password */

			LCD_clearScreen();

			LCD_displayString("Taking confirm-");

			LCD_displayStringRowColumn(1,1,"tory Password...");

			_delay_ms(1000);

			receive_password(Confirmation_Password);

			break;

		case ARE_PASSWORDS_MATCHED: /* checking if the main & confirmation passwords matched */

			LCD_clearScreen();

			LCD_displayString("Check matching...");

			_delay_ms(500);

			MAIN_CONFIRMATION_MATCHED_FLAG = TRUE;

			for(user_password_index = 0; user_password_index < PASSWORD_SIZE; user_password_index++){

			if(Main_Password[user_password_index] != Confirmation_Password[user_password_index]){
							/* passwords aren't match */

							MAIN_CONFIRMATION_MATCHED_FLAG = FALSE;

							break;
						}

					}
			if(MAIN_CONFIRMATION_MATCHED_FLAG ){

				/* sending to HMI ECU that passwords are matched */

				UART_sendByte(PASSWORDS_ARE_MATCHED);

				LCD_clearScreen();

				LCD_displayString("Passwords Are");

				LCD_displayStringRowColumn(1,1,"Matched");

				_delay_ms(2000);

				EEPROM_writeByte(PASSWORD_CREATION_LOCATION , MAIN_CONFIRMATION_MATCHED_FLAG);

				_delay_ms(15);

				for(user_password_index = 0 ; user_password_index < PASSWORD_SIZE ; user_password_index++){

				EEPROM_writeByte(user_password_index , Main_Password[user_password_index]);

				_delay_ms(10);
				}
			}
			else
			{ /* passwords are not matched */

				UART_sendByte(PASSWORDS_ARE_NOT_MATCHED);

				LCD_clearScreen();

				LCD_displayString("Passwords aren't");

				LCD_displayStringRowColumn(1,1,"Matched");

				_delay_ms(2000);
			}
			_delay_ms(10);
			break;

		case TAKE_PASSWORD : /* system contain a password and user is entering it to open door */

			LCD_clearScreen();

			LCD_displayString("Taking Password...");

			_delay_ms(2000);

			MAIN_CONFIRMATION_MATCHED_FLAG = TRUE;

			receive_password(Main_Password);

			for(user_password_index = 0 ; user_password_index < PASSWORD_SIZE ; user_password_index++){
				EEPROM_readByte(user_password_index , &Opening_Password);

				if(Main_Password[user_password_index] != Opening_Password)
				MAIN_CONFIRMATION_MATCHED_FLAG = 0;
				_delay_ms(10);
			}
			if(MAIN_CONFIRMATION_MATCHED_FLAG){

			UART_sendByte(PASSWORD_IS_CORRECT);	/* Informing HMI ECU that the password is correct */

			LCD_clearScreen();

			LCD_displayString("PASSWORD Correct");

			_delay_ms(2000);
			}

			else{
				UART_sendByte(PASSWORD_IS_NOT_CORRECT);		/* Informing HMI ECU that the password is wrong */

				LCD_clearScreen();

				LCD_displayString("Password is not");

				LCD_displayStringRowColumn(1,1,"correct");

				_delay_ms(2000);

			}
				break;
		case OPEN_DOOR	:							/* Opening the door */

					Timer1_init(&Timer1_AppConfig);

					 LCD_clearScreen();

					LCD_displayString("Door Opened");


					DcMotor_Rotate(ClockWise , 100);

					while(Timer_teks != DOOR_OPENING_TIME);		/* waiting 15 seconds*/


					Timer_teks = 0;

					Timer1_deinit();

					Timer1_init(&Timer1_AppConfig);


					DcMotor_Rotate(MotorStop , 0);

					while(Timer_teks != DOOR_WAITING_TIME);		/* waiting 5 seconds*/


					Timer_teks = 0;

					Timer1_deinit();

					Timer1_init(&Timer1_AppConfig);

					DcMotor_Rotate(AntiClockWise , 100);

					while(Timer_teks != DOOR_CLOSING_TIME);		/* waiting 15 seconds*/


					Timer_teks = 0;

					Timer1_deinit();

					DcMotor_Rotate(MotorStop , 0);
					LCD_clearScreen();
					LCD_displayString("Door Closed");
					_delay_ms(1000);
					break;

			case ALARM_MODE :				/* Alarm mode */

					Timer1_init(&Timer1_AppConfig);

					Buzzer_on();

					LCD_clearScreen();
					LCD_displayString("Warning!");
					while(Timer_teks != 60);	/* waiting 60 seconds*/
					Timer_teks = 0;

					Timer1_deinit();

					Buzzer_off();

					break;

		}
	}
}
/********************************** Functions Definition Section *****************************/

/*
 * ==================================================================================
 * @brief   : Function to receive the password entered by the user in the HMU ECU.
 * @Param   : Function takes ptr for the array that save the password.
 * @retVal  : Function returns nothing.
 * ==================================================================================
 */
void receive_password(uint8 *PasswordPtr){
uint8 user_password_index;
	for(user_password_index = 0; user_password_index < PASSWORD_SIZE; user_password_index++){
			PasswordPtr[user_password_index] = UART_recieveByte();
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
void timer1_tick(void){
	Timer_teks++;
}
