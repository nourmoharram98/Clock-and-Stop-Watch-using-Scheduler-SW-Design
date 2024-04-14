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
		[SWITCH_MODE]={
                            .port=GPIO_PORT_B,
                            .Switch_Pin=GPIO_PIN_10,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,
                        },
		[SWITCH_OK]={
                            .port=GPIO_PORT_B,
                            .Switch_Pin=GPIO_PIN_8,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,
        },
		[SWITCH_EDIT]={
                            .port=GPIO_PORT_B,
                            .Switch_Pin=GPIO_PIN_9,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,

        },
		[SWITCH_UP]={
                            .port=GPIO_PORT_B,
                            .Switch_Pin=GPIO_PIN_1,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,

        },
		[SWITCH_DOWN]={
                            .port=GPIO_PORT_B,
                            .Switch_Pin=GPIO_PIN_0,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,

        },
		[SWITCH_LEFT]={
                            .port=GPIO_PORT_B,
                            .Switch_Pin=GPIO_PIN_13,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,

        },
		[SWITCH_RIGHT]={
                            .port=GPIO_PORT_B,
                            .Switch_Pin=GPIO_PIN_14,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,

        }    
};

