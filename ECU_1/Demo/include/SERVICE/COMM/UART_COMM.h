#include "Std_Types.h"
#include "MCAL/USART/STM32F401cc_MCAL_USART.h"
#include"Error_states.h"

#define NUMBER_OF_REQUESTS              30


Sys_enuErrorStates_t Communication_Sender(void);

Sys_enuErrorStates_t Communication_Receiver(void);