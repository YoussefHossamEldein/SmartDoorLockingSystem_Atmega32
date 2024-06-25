 /*==========================================================================
 *
 * Module      : BUZZER
 *
 * File Name   : buzzer.c
 *
 * Description : Source file for buzzer module.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/


/*************************** Includes Section *********************************/

#include "buzzer.h"
#include "../../MCAL/GPIO/gpio.h"
/*************************** Global Variables Section *************************/



/************************ Function Definition Section *************************/

/*======================================================================
 * @brief  : function to initialize the buzzer driver.
 * @param  : function takes nothing.
 * @retVal : function returns nothing.
 * ======================================================================
 */
void Buzzer_init(){
	/* initialize buzzer pin direction as output direction */
	GPIO_setupPinDirection(BUZZER_PIN_PORT_ID,BUZZER_PIN_PIN_ID,PIN_OUTPUT);
	/* Turn OFF buzzer at the beginning */
	GPIO_writePin(BUZZER_PIN_PORT_ID,BUZZER_PIN_PIN_ID,LOGIC_LOW);
}
/*======================================================================
 * @brief  : function to turn ON the buzzer on need.
 * @param  : function takes nothing.
 * @retVal : function returns nothing.
 * ======================================================================
 */
void Buzzer_on(){
	GPIO_writePin(BUZZER_PIN_PORT_ID,BUZZER_PIN_PIN_ID,LOGIC_HIGH);
}
/*======================================================================
 * @brief  : function to turn OFF the buzzer on need.
 * @param  : function takes nothing.
 * @retVal : function returns nothing.
 * ======================================================================
 */
void Buzzer_off(){
	GPIO_writePin(BUZZER_PIN_PORT_ID,BUZZER_PIN_PIN_ID,LOGIC_LOW);
}
