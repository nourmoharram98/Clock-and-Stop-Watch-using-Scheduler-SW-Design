/**
 * @file HAL_SWITCH_CONFIG.c
 * @author Nour Moharram
 * @brief carry the implementation of the array of switch configurations
 * @version 0.1
 * @date 2024-02-17
 */

#include "./MCAL/GPIO/GPIO.h"  //Integration Aly & Nour
#include"HAL/SWITCH/HAL_SWITCH.h"


/*array for switches configurations*/
const SWITCH_CONFIGURATIONS arrOfSwitches[Number_Of_Switches]={
		[MODE_SWITCH]={
                            .port=GPIO_PORT_A,
                            .Switch_Pin=GPIO_PIN_11,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,
                        }
};

