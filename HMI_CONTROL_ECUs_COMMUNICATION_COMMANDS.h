 /*==========================================================================
 *
 *
 * File Name   : HMI_CONTROL_ECUs_COMMUNICATION_COMMANDS.h
 *
 * Description : Header file for the communication commands between HMI and CONTROL ECUs.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/

#ifndef HMI_CONTROL_ECUS_COMMUNICATION_COMMANDS_H_
#define HMI_CONTROL_ECUS_COMMUNICATION_COMMANDS_H_
/************************************* Includes Section ****************************************/


/************************************* Macro Definition Section ********************************/
#define IS_PASSWORD_CREATED						'A'
#define PASSWORD_CREATED						'B'
#define PASSWORD_NOT_CREATED					'C'
#define TAKE_MAIN_PASSWORD						'D'
#define TAKE_CONFIRMATION_PASSWORD				'E'
#define ARE_PASSWORDS_MATCHED					'F'
#define PASSWORDS_ARE_MATCHED					'G'
#define PASSWORDS_ARE_NOT_MATCHED				'H'
#define TAKE_PASSWORD							'I'
#define PASSWORD_IS_CORRECT						'J'
#define PASSWORD_IS_NOT_CORRECT				    'K'
#define OPEN_DOOR								'L'
#define CLOSE_DOOR								'M'
#define ALARM_MODE								'N'
#define TAKE_PASSWORD_2							'O'
#define ENTRY_TIME								'P'
#define CLOSING_TIME							'Q'

/************************************* Macro Functions Section *********************************/




/************************************ Function Declaration Section *****************************/


#endif /* HMI_CONTROL_ECUS_COMMUNICATION_COMMANDS_H_ */
