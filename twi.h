 /*==========================================================================
 *
 * Module      : I2C
 *
 * File Name   : twi.h
 *
 * Description : Header file for I2C module.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/

#ifndef TWI_H_
#define TWI_H_
/***************************** Includes Section *************************************************/
#include "../../STD_UTILITIES/std_types.h"

/***************************** Data Types Section **********************************************/
typedef enum {
	BAUD_RATE_500KHZ = 0,
	BAUD_RATE_400KHZ = 2,
	BAUD_RATE_250KHZ = 8,
    BAUD_RATE_200KHZ = 12,
    BAUD_RATE_160KHZ = 17,
    BAUD_RATE_125KHZ = 24,
}TWI_BaudRate;

typedef struct {
	uint8 address;
	TWI_BaudRate bit_rate;
}TWI_ConfigType;



/****************************** Macro Definition Section **********************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/************************** Function Declaration Section ************************/
void TWI_init(const TWI_ConfigType *Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
