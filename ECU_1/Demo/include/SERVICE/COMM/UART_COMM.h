#include "Std_Types.h"
#include "MCAL/USART/STM32F401cc_MCAL_USART.h"
#include"Error_states.h"

#define NUMBER_OF_REQUESTS              30


U8 append_crc(U8 data) ;

U8 calculate_crc(U8 data) ;

Sys_enuErrorStates_t Communication_Sender(void);

Sys_enuErrorStates_t Communication_Receiver(void);

extern void Command_Handler(u8 command);
extern void Toggle_Mode(void);
