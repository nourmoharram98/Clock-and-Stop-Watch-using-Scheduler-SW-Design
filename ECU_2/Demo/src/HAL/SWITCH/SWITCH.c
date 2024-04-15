/**
 * @file HAL_SWITCH.c
 * @author Nour Moharram
 * @brief implementation of the HAL Switch functions
 * @version 0.1
 * @date 2024-02-17
 */

#include "Std_Types.h"
#include"Error_states.h"
#include"MCAL/GPIO/GPIO.h"
#include"HAL/SWITCH/HAL_SWITCH.h"

#define SWITCH_STATUS_CHECKS_THRESHOLD				3

extern const SWITCH_CONFIGURATIONS arrOfSwitches[Number_Of_Switches];
volatile static u8 Switch_ActualStatus[Number_Of_Switches]={1};

/**
 * @brief enumeration for switch status if it's mode is internal pull up
 *
 */
typedef enum
{
	Switch_Pressed_InternalPU, /**< Switch_Pressed_InternalPU */
	Switch_Released_InternalPU,/**< Switch_Released_InternalPU */
}Switch_stateInternalPU_t;


/**
 * @brief enumeration for switch status if it's mode is external pull down
 *
 */
typedef enum
{
	Switch_Released_ExternalPD,/**< Switch_Released_ExternalPD */
	Switch_Pressed_ExternalPD, /**< Switch_Pressed_ExternalPD */
}Switch_stateExternalPD_t;


/**
 *@brief  : Function to Initialize the switch configurations
 *@param  : void
 *@return : void
 */
void HAL_SWITCH_Init(void)
{
	GPIO_Pin_t Switch;

	Switch.Pin_Speed = GPIO_SPEED_HIGH;

	for(u8 index=0;index<Number_Of_Switches;index++)
	{
		Switch.Port=arrOfSwitches[index].port;
		Switch.Pin_num=arrOfSwitches[index].Switch_Pin;
		Switch.Pin_Mode=arrOfSwitches[index].SWITCH_PIN_MODE;
		GPIO_Init(&Switch);
	}
}




/**
 *@brief  : Function to read the state of switch
 *@param  : switch ID and pointer to address where state will be assigned
 *@return : Error State
 @version : 1.2
 */
Sys_enuErrorStates_t HAL_SWITCH_enuGetSwitchState(u8 SWITCH,u32 *Switch_Status)
{
	Sys_enuErrorStates_t Error_Status=NOT_OK;
	if(Switch_Status==NULL_POINTER)
	{
		Error_Status=NULL_POINTER_ERROR;
	}
    else if (SWITCH < SWITCH_MODE || SWITCH >= Number_Of_Switches)
    {
        Error_Status = INVALID_INPUT_VALUE;
    }
	else
	{
		/**
		 * @brief read the IDR pin status from the global array of Switch_ActualStatus that carry the status 
		 * from the Switch_Debouncing_Runnable 
		 * @brief toggle the pin current status with the value of the Switch connection mode to get 
		 * the current switch status(pressed or released according to the truth table below)
		 *  
		 *            XOR                   PIN_CURRENT STATUS_HIGH(1)			PIN_CURRENT_STATUS_LOW(0)
		 * 
		 *            (1)
		 *  SWITCH_CONNECTION_MODE_INTPU 	   SWITCH_RELEASED(0)					SWITCH_PRESSED(1)
		 * 
		 *            (0)
		 *  SWITCH_CONNECTION_MODE_INTPD	   SWITCH_PRESSED(1)					SWITCH_RELEASED(0)
		 */
	
		//*Switch_Status= ! Switch_ActualStatus[SWITCH] ^ arrOfSwitches[SWITCH].SWITCH_CONNECTION_MODE;
        *Switch_Status = !( (Switch_ActualStatus[SWITCH]) ^ (( (arrOfSwitches[SWITCH].SWITCH_PIN_MODE)  >> FOUR_BIT_OFFSET ) & 0x01) ) ;
		Error_Status=OK;
	}
    return Error_Status;
}
//create runnable for debouncing switch come every 5 ms

void Switch_Debouncing_Runnable(void)
{
	U8 current_status=0;
	volatile static u32 Switch_PreviousStatus[Number_Of_Switches]={0};
	volatile static u8 Switch_Counters[Number_Of_Switches]={0};
	for(u8 index=0;index<Number_Of_Switches;index++)
	{
		GPIO_Get_PinValue(arrOfSwitches[index].port,arrOfSwitches[index].Switch_Pin,&current_status);
		if(current_status==Switch_PreviousStatus[index])
		{
			Switch_Counters[index]++;
		}
		else
		{
			Switch_Counters[index]=0;
		}
		if(Switch_Counters[index]==SWITCH_STATUS_CHECKS_THRESHOLD)
		{
			Switch_ActualStatus[index]=current_status;
			Switch_Counters[index]=0;
		}
		Switch_PreviousStatus[index]=current_status;
	}
}