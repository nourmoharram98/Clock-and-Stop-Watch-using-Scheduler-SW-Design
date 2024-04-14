/*
 * Error_states.h
 *
 *  Created on: Feb 16, 2024
 *      Author: Nour Moharram
 */

#ifndef ERROR_STATES_H_
#define ERROR_STATES_H_


#define NULL_POINTER		(void *)0

typedef enum
{
	NOT_OK,
	OK,
	NULL_POINTER_ERROR,
	INVALID_INPUT_VALUE,
	TIMEOUT_ERROR,
	GPIO_PIN_NUM_ERROR,
	GPIO_MODE_ERROR,
	GPIO_SPEED_ERROR,
	GPIO_PORT_ERROR,
	GPIO_STATUS_ERROR,
	NVIC_Interrupt_Number_Error,
	NVIC_GROUP_NUMBER_ERROR,
	NVIC_SUBGROUP_NUMBER_ERROR,
	NVIC_PRIORITY_BITS_NUMBER_ERROR,
	SYSTICK_Timer_Expired,
	SYSTICK_TIMER_NotExpired,
	LED_CONNECTION_MODE_ERROR,
	LED_DEFAULT_STATUS_ERROR,
	USART_RQST_STATE_ERROR,
	LCD_BUFFER_REQUESTS_FULL,
}Sys_enuErrorStates_t;

#endif /* ERROR_STATES_H_ */