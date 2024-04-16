/**
 * @file HAL_SWITCH.h
 * @author Nour Moharram
 * @brief carry the important enumeraitons, macros and function prototypes of the SWITCH driver to be used by user
 * @version 0.1
 * @date 2024-02-17
 * 
 */

#ifndef HAL_SWITCH_H_
#define HAL_SWITCH_H_

#include "Std_Types.h"
#include "Error_states.h"
#include "HAL/SWITCH/HAL_SWITCH_CONFIG.h"
#include"HAL/SystickReader/SYSTICK_Reader.h"


/**
 * @brief Macros for switch mode
 *
 */
// #define SWITCH_MODE_INTERNALPULLUP				GPIO_PIN_INPUT_PULLUP
// #define SWITCH_MODE_EXTERNALPULLDOWN			GPIO_PIN_INPUT_PULLDOWN

#define FOUR_BIT_OFFSET								0x00000004

#define SWITCH_PIN_MODE_INTERNALPULLUP				0x10000008
#define SWITCH_PIN_MODE_INTERNALPULLDOWN			0x10000010

#define SWITCH_CONNECTION_MODE_INTPU				0x00000001
#define SWITCH_CONNECTION_MODE_INTPD				0x00000000

#define SWITCH_RELEASED							0x00000000
#define SWITCH_PRESSED							0x00000001


/**
 * @brief enumeration for switch status
 *
 */
typedef enum
{
	Switch_Released, /**< Switch_Released */
	Switch_Pressed   /**< Switch_Pressed */
}Switch_Status_t;


/**
 * @brief Struct for switch configuration
 *
 */
typedef struct
{
    void *port;
	u32 Switch_Pin;
	u32 SWITCH_PIN_MODE;
	u32 SWITCH_CONNECTION_MODE;
}SWITCH_CONFIGURATIONS;

/**
 *@brief  : Function to Initialize the switch configurations
 *@param  : void
 *@return : void
 */
void HAL_SWITCH_Init(void);


/**
 *@brief  : Function to read the state of switch
 *@param  : switch ID and pointer to address where state will be assigned
 *@return : Error State
 */
Sys_enuErrorStates_t HAL_SWITCH_enuGetSwitchState(u8 SWITCH,u32 *Switch_Status);

/**
 * @brief Function to get the switch status after solving the transient effect 
 * @details Function used:
 *  1-one global array [Switch_ActualStatus] :to carry the actual status of the pin to be shared between
 * 	[Switch_Debouncing_Runnable] & [HAL_SWITCH_enuGetSwitchState]:
 *  2-two local static arrays :
 * 			A- Switch_PreviousStatus : to carry the previous status of the switches
 * 			B- Switch_Counters to carry the counts of times when the previous status was equal to current status
 * 3- when counts reach the [SWITCH_STATUS_CHECKS_THRESHOLD] it will change the actual status of the switch to the current status 
 */
void Switch_Debouncing_Runnable(void);

#endif