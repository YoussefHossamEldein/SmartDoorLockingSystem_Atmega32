 /*==========================================================================
 *
 * Module      : UART
 *
 * File Name   : uart.c
 *
 * Description : Source file for UART module.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/

/************************************ Includes Section ***************************************/
#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "../../STD_UTILITIES/common_macros.h" /* To use the macros like SET_BIT */

/****************************** Function Definition Section **********************/


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
void UART_init(UART_ConfigType *Config_Ptr)
{
	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/ 
	UCSRB = (1<<RXEN) | (1<<TXEN);
	
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/ 	
	UCSRC = (1<<URSEL) | Config_Ptr->bit_data | Config_Ptr->stop_bit | Config_Ptr->parity;
	
	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / (Config_Ptr->baud_rate * 8UL))) - 1);

	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;
}

/*============================================================================
 * @brief  : Functional responsible for send byte to another UART device.
 * @Param  : Function takes the data to be sent to another UART device.
 * @RetVal : Function returns nothing.
 * ==========================================================================
 */
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now
	 */
	UDR = data;

}

/*=============================================================================
 * @brief  : Functional responsible for receive byte from another UART device.
 * @param  : Function takes nothing.
 * @RetVal : Function returns the data received.
 * =============================================================================
 */
uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the Rx buffer (UDR)
	 * The RXC flag will be cleared after read the data
	 */
    return UDR;		
}

/*============================================================================
 * @brief  : Send the required string through UART to the other UART device.
 * @param  : function the string to be sent.
 * @RetVal : function returns nothing.
 * ===========================================================================
 */
void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;

	/* Send the whole string */
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

/*=====================================================================================================
 * @brief  : Receive the required string until the '#' symbol through UART from the other UART device.
 * @Param  : function takes pointer to uint8 which is the string received from the other UART device.
 * @RetVal : function returns nothing.
 * ====================================================================================================
 */
void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;

	/* Receive the first byte */
	Str[i] = UART_recieveByte();

	/* Receive the whole string until the '#' */
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}

	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	Str[i] = '\0';
}
