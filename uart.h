 /*==========================================================================
 *
 * Module      : UART
 *
 * File Name   : uart.h
 *
 * Description : header file for UART module.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/

#ifndef UART_H_
#define UART_H_
/****************************************** Includes Section **********************************/
#include "../../STD_UTILITIES/std_types.h"

/************************************* Data Types Section ************************************/

typedef enum {
	BIT_DATA_5,BIT_DATA_6 = 2,BIT_DATA_7 = 4,BIT_DATA_8 = 6,
}UART_BitData;
typedef enum {
	PARITY_DISABLED , PARITY_RESERVED = 0x10, EVEN_PARITY = 32, ODD_PARITY = 48,
}UART_Parity;
typedef enum {
	ONE_STOP_BIT, TWO_STOP_BIT = 8,
}UART_StopBit;

typedef struct {
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	uint32 baud_rate;
}UART_ConfigType;



/********************************* Macro Definition Section **************************/




/********************************* Macro Functions Section ***************************/



/******************************* Function Declaration Section ***************************/

/*=============================================================================================
 * @brief  :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 * @Param  : Function takes the decided baud rate to work with.
 * @RetVal : Function returns nothing.
 * ============================================================================================
 */
void UART_init(UART_ConfigType *Config_Ptr);

/*============================================================================
 * @brief  : Functional responsible for send byte to another UART device.
 * @Param  : Function takes the data to be sent to another UART device.
 * @RetVal : Function returns nothing.
 * ===========================================================================
 */
void UART_sendByte(const uint8 data);

/*==============================================================================
 * @brief  : Functional responsible for receive byte from another UART device.
 * @param  : Function takes nothing.
 * @RetVal : Function returns the data received.
 * =============================================================================
 */
uint8 UART_recieveByte(void);

/*==============================================================================
 * @brief  : Send the required string through UART to the other UART device.
 * @param  : function the string to be sent.
 * @RetVal : function returns nothing.
 * =============================================================================
 */
void UART_sendString(const uint8 *Str);

/*=====================================================================================================
 * @brief  : Receive the required string until the '#' symbol through UART from the other UART device.
 * @Param  : function takes pointer to uint8 which is the string received from the other UART device.
 * @RetVal : function returns nothing.
 * ====================================================================================================
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
