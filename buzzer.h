 /*==========================================================================
 *
 * Module      : BUZZER
 *
 * File Name   : buzzer.h
 *
 * Description : Header file for buzzer module.
 *
 * Author      : Youssef Hossam
 *
 *==============================================================================
*/

#ifndef BUZZER_H_
#define BUZZER_H_

/*************************** Includes Section *********************************/
#include "../../STD_UTILITIES/std_types.h"


/*************************** Data Types Section *******************************/


/************************** Macro Definition Section **************************/
#define BUZZER_PIN_PORT_ID				PORTC_ID
#define BUZZER_PIN_PIN_ID				PIN7_ID


/************************* Macro Functions Section ****************************/



/************************ Functions Declaration Section ***********************/
/*======================================================================
 * @brief  : function to initialize the buzzer driver.
 * @param  : function takes nothing.
 * @retVal : function returns nothing.
 * ======================================================================
 */
void Buzzer_init();
/*======================================================================
 * @brief  : function to turn ON the buzzer on need.
 * @param  : function takes nothing.
 * @retVal : function returns nothing.
 * ======================================================================
 */
void Buzzer_on();
/*======================================================================
 * @brief  : function to turn OFF the buzzer on need.
 * @param  : function takes nothing.
 * @retVal : function returns nothing.
 * ======================================================================
 */
void Buzzer_off();


#endif /* BUZZER_H_ */
