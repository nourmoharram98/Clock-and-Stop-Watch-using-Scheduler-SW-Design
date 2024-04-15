#ifndef MCAL_USART_H
#define MCAL_USART_H

#include"./Std_Types.h"
#include"./Error_states.h"
#include "./MCAL/USART/MCAL_USART_CONFIG.h"


#define USART_PINS_NUM      2
#define USART_TX_PIN        0
#define USART_RX_PIN        1
/**
 * @brief Macros for USART IDs 
 * 
 */
#define USART1              0
#define USART2              1
#define USART6              2

/**
 * @brief Macros for USART Configurations
 * 
 */
#define OVERSAMPLING_16                 0x00000000
#define OVERSAMPLING_8                  0x00008000

#define USART_WORDLEN_8                 0x00000000
#define USART_WORDLEN_9                 0x00001000

#define USART_PARITY_ENABLE             0x00000400
#define USART_PARITY_DISABLE            0x00000000

#define USART_PARITY_SELEC_EVEN         0x00000000
#define USART_PARITY_SELEC_ODD          0x00000200
#define USART_PARITY_SELEC_OFF          0x00000000

#define USART_PARITYINT_ENABLE          0x00000100
#define USART_PARITYINT_DISABLE         0x00000000

#define USART_TXE_INT_ENABLE            0x00000080
#define USART_TXE_INT_DISABLE           0x00000000

#define USART_PARITY_INT_ENABLE         0x00000100
#define USART_PARITY_INT_DISABLE        0x00000000

#define USART_TRANSMISSION_COMP_INT_ENB 0x00000040
#define USART_TRANSMISSION_COMP_INT_DIS 0x00000000

#define USART_RECEIVE_ORE_INT_ENABLE    0x00000200
#define USART_RECEIVE_ORE_INT_DISABLE   0x00000000

#define USART_IDLE_INT_ENABLE           0x00000010
#define USART_IDLE_INT_DISABLE          0x00000000   

#define USART_ONE_STOP_BIT              0x00000000
#define USART_TWO_STOP_BITS             0x00002000
#define USART_HALF_STOP_BIT             0x00001000
#define USART_ONE_HALF_STOP_BIT         0x00003000

/**
 * @brief pointer to function
 * 
 */
typedef void (*Ptrtofunc)(void);


/**
 * @brief Struct for USART Struct of Configurations
 * 
 */
typedef struct 
{
    /*Configurations*/
    u8 USART_ID;
    u32 OverSampling;
    u32 WordLength;
    u32 Parity_Status;
    u32 Parity_Mode;
    u32 Stopbits;
    f32 baudrate;
}USART_Configurations_t;


/**
 * @brief struct for USART Requests
 * 
 */
typedef struct
{
    u8 USART_ID;
    u8 *PtrtoBuffer;
    u16 length;
    Ptrtofunc CallBack;
}USART_Request_t;

/**
 * @brief USART Function to initialize USART Peripherals in system
 * 
 */
Sys_enuErrorStates_t USART_Init(void);

Sys_enuErrorStates_t USART_Pins_Init(void);

Sys_enuErrorStates_t USART_SendByte(USART_Request_t USART_Request);

Sys_enuErrorStates_t USART_GetByte(USART_Request_t USART_Request);

Sys_enuErrorStates_t USART_SendByteAsynchZC(USART_Request_t USART_Request);

Sys_enuErrorStates_t USART_ReceiveByteAsynchZC(USART_Request_t USART_Request);











#endif