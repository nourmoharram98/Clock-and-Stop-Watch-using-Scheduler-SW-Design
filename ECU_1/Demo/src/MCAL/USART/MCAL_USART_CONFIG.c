#include "./MCAL/USART/STM32F401cc_MCAL_USART.h"
#include "./MCAL/USART/MCAL_USART_CONFIG.h"
#include "./MCAL/GPIO/GPIO.h"


const USART_Configurations_t USARTs_Configurations[Number_Of_USARTS]=
{
    [USART_One]={
        .USART_ID=USART1,
        .OverSampling=OVERSAMPLING_16,
        .WordLength=USART_WORDLEN_8,
        .Parity_Status=USART_PARITY_DISABLE,
        .Parity_Mode=USART_PARITY_SELEC_OFF,
        .Stopbits=USART_ONE_STOP_BIT,
        .baudrate=9600
    },
    // [USART_Two]={
    //     .USART_ID=USART2,
    //     .OverSampling=OVERSAMPLING_16,
    //     .WordLength=USART_WORDLEN_8,
    //     .Parity_Status=USART_PARITY_DISABLE,
    //     .Parity_Mode=USART_PARITY_SELEC_OFF,
    //     .Stopbits=USART_ONE_STOP_BIT,
    //     .baudrate=9600
    // }
};


const GPIO_Pin_t USART_Pins[USART_PINS_NUM]={
    [USART_TX_PIN]={
        .Pin_Mode=GPIO_MODE_AF_PP,
        .Pin_Speed=GPIO_SPEED_MED,
        .Port=GPIO_PORT_B,
        .Pin_num=GPIO_PIN_6
    },
    [USART_RX_PIN]={
        .Pin_Mode=GPIO_MODE_AF_PP,
        .Pin_Speed=GPIO_SPEED_MED,
        .Port=GPIO_PORT_B,
        .Pin_num=GPIO_PIN_7
    }
};

const u32 USART_CLKFreqs[Number_Of_USARTS]={16000000,16000000};