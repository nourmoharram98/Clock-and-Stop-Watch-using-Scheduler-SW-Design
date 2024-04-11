#include"Std_Types.h"
#include"Error_states.h"
#include"MCAL/USART/STM32F401cc_MCAL_USART.h"

/**
 * @brief Macros used in implementation
 * 
 */
#define USART_NUMS                                  3
#define USART_MANTISSA_OFFSET                       4
#define USART_FRACTION_PRESERVE_MASK                0x0F
#define USART_PERI_ENABLE                           0x00002000
#define USART_RQST_STATE_BUSY                       1
#define USART_RQST_STATE_READY                      0
#define USART_TE_ENABLE                             0x00000008
#define USART_RE_ENABLE                             0x00000004
#define USART_TXE_STATUS                            0x00000080
#define USART_TXE_POSITION                          0x07
#define USART_TXEIE_ENABLE                          0x00000080
#define USART_TC_FLAG                               0x00000040
#define USART_RXNE_FLAG                             0x00000020
#define USART_RXNEIE_ENABLE                         0x00000020
/**
 * @brief USART Registers
 * 
 */
typedef struct 
{
    volatile u32 SR;
    volatile u32 DR;
    volatile u32 BRR;
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 CR3;
    volatile u32 GTPR;
}USART_PerRegs_t;

/**
 * @brief buffer data struct
 * 
 */
typedef struct 
{
    u8 *dataByte;
    u16 Pos_Index;
    u16 size;
}Buffer_t;

typedef struct 
{
    Buffer_t Buffer;
    u8 ReqState;
    Ptrtofunc CallBack;
}Tx_ReqHandling_t;
typedef struct 
{
    Buffer_t Buffer;
    u8 ReqState;
    Ptrtofunc CallBack;
}Rx_ReqHandling_t;


const USART_Configurations_t USARTs_Configurations[Number_Of_USARTS];
const u32 USART_CLKFreqs[Number_Of_USARTS];
Tx_ReqHandling_t tx_requests[Number_Of_USARTS];
Rx_ReqHandling_t rx_requests[Number_Of_USARTS];

void *USART_BaseAddresses[USART_NUMS]={
    (void *)0x40011000,(void *)0x40004400,(void *)0x40011400
};


Sys_enuErrorStates_t USART_Init(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    u32 Local_OVER8=0;
    f32 Local_USARTDIV=0;
    u32 Local_Mantissa=0;
    f32 Local_Fraction=0;
    f32 Local_Sampling_Rate=0;
    u16 Max_FractionBits_value=0;
    u32 Local_USART_BRR=0;
    u32 Local_USART_CR1=0;
    u32 Local_USART_CR2=0;
    for(u8 index=0;index<Number_Of_USARTS;index++)
    {
        Local_OVER8=USARTs_Configurations[index].OverSampling/OVERSAMPLING_8;
        Local_USARTDIV=(f32)USART_CLKFreqs[index]/(f32)(USARTs_Configurations[index].baudrate*(8*(2-Local_OVER8)));
        Local_Fraction=(((u32)(Local_USARTDIV*1000))%1000)/1000.0;
        Local_Mantissa= Local_USARTDIV - Local_Fraction;

        Local_Sampling_Rate=(USARTs_Configurations[index].OverSampling==OVERSAMPLING_16)?16:8;
        Max_FractionBits_value=(USARTs_Configurations[index].OverSampling==OVERSAMPLING_16)?15:7;
        Local_Fraction *=Local_Sampling_Rate;
        if(((u32)(Local_Fraction*10))%10>5)
        {
            Local_Fraction++;
            //Local_Fraction=(u32)(++Local_Fraction);
           // Local_Fraction=(u32)(++Local_Fraction);

        }
        else
        {
            Local_Fraction=(u32)Local_Fraction;
        }
        if(Local_Fraction>Max_FractionBits_value)
        {
            Local_Mantissa++;
            Local_Fraction=0;
        }
        Local_USART_BRR=(Local_Mantissa<<USART_MANTISSA_OFFSET) | ((u32)Local_Fraction & USART_FRACTION_PRESERVE_MASK); 
        Local_USART_CR1 = USART_PERI_ENABLE | USARTs_Configurations[index].WordLength | USARTs_Configurations[index].OverSampling;
        Local_USART_CR1 |= USARTs_Configurations[index].Parity_Status | USARTs_Configurations[index].Parity_Mode;
        Local_USART_CR2 = USARTs_Configurations[index].Stopbits;

        ((USART_PerRegs_t *)USART_BaseAddresses[USARTs_Configurations[index].USART_ID])->BRR=Local_USART_BRR;
        ((USART_PerRegs_t *)USART_BaseAddresses[USARTs_Configurations[index].USART_ID])->CR1=Local_USART_CR1;
        ((USART_PerRegs_t *)USART_BaseAddresses[USARTs_Configurations[index].USART_ID])->CR2=Local_USART_CR2;

    }
    Error_Status=OK;
    return Error_Status;
}


Sys_enuErrorStates_t USART_SendByte(USART_Request_t USART_Request)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    u16 TIME_OUT=5000;
    if(USART_Request.USART_ID>=Number_Of_USARTS)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(USART_Request.length>1)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(tx_requests[USART_Request.USART_ID].ReqState==USART_RQST_STATE_BUSY)
    {
        Error_Status=USART_RQST_STATE_ERROR;
    }
    else
    {
        tx_requests[USART_Request.USART_ID].ReqState=USART_RQST_STATE_BUSY;
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 |= USART_TE_ENABLE;
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->DR=*(USART_Request.PtrtoBuffer);

        while(~(((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->SR&USART_TC_FLAG)&&TIME_OUT)
        {
            TIME_OUT--;
        }

      
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 &= ~(USART_TE_ENABLE);

       // USART_Request.CallBack();
        tx_requests[USART_Request.USART_ID].ReqState=USART_RQST_STATE_READY;

        Error_Status=OK;
    }
    return Error_Status;
}

Sys_enuErrorStates_t USART_GetByte(USART_Request_t USART_Request)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    u32 TIME_OUT=100000;
    if(USART_Request.USART_ID>=Number_Of_USARTS)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(USART_Request.length>1)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(rx_requests[USART_Request.USART_ID].ReqState==USART_RQST_STATE_BUSY)
    {
        Error_Status=USART_RQST_STATE_ERROR;
    }
    else
    {
        rx_requests[USART_Request.USART_ID].ReqState=USART_RQST_STATE_BUSY;
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 |= USART_RE_ENABLE;
        while(~((((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->SR)&USART_RXNE_FLAG)&&TIME_OUT)
        {
            TIME_OUT--;
        }
        if(TIME_OUT==0)
        {
           if(((((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->SR)&USART_RXNE_FLAG)==0)
           {
                Error_Status=TIMEOUT_ERROR;
           }
           else
           {
                *(USART_Request.PtrtoBuffer)= ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->DR;
           }
        }
        else
        {
            *(USART_Request.PtrtoBuffer)= ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->DR;
        }

      
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 &= ~(USART_RE_ENABLE);

        //USART_Request.CallBack();
        rx_requests[USART_Request.USART_ID].ReqState=USART_RQST_STATE_READY;

        Error_Status=OK;
    }

    return Error_Status;
}

Sys_enuErrorStates_t USART_SendByteAsynchZC(USART_Request_t USART_Request)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(USART_Request.USART_ID>=Number_Of_USARTS)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(USART_Request.PtrtoBuffer==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else if(tx_requests[USART_Request.USART_ID].ReqState==USART_RQST_STATE_BUSY)
    {
        Error_Status=USART_RQST_STATE_ERROR;
    }
    else
    {
        Error_Status=OK;
        tx_requests[USART_Request.USART_ID].ReqState=USART_RQST_STATE_BUSY;
        tx_requests[USART_Request.USART_ID].Buffer.dataByte=USART_Request.PtrtoBuffer;
        tx_requests[USART_Request.USART_ID].Buffer.size=USART_Request.length;
        tx_requests[USART_Request.USART_ID].CallBack=USART_Request.CallBack;
        tx_requests[USART_Request.USART_ID].Buffer.Pos_Index=0;

        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 |= USART_TE_ENABLE;
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->DR=(tx_requests[USART_Request.USART_ID].Buffer.dataByte[0]);
        tx_requests[USART_Request.USART_ID].Buffer.Pos_Index++;
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 |= USART_TXEIE_ENABLE;


    }

    return Error_Status;
}

Sys_enuErrorStates_t USART_ReceiveByteAsynchZC(USART_Request_t USART_Request)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(USART_Request.USART_ID>=Number_Of_USARTS)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(USART_Request.PtrtoBuffer==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else if(rx_requests[USART_Request.USART_ID].ReqState==USART_RQST_STATE_BUSY)
    {
        Error_Status=USART_RQST_STATE_ERROR;
    }
    else
    {
        Error_Status=OK;
        rx_requests[USART_Request.USART_ID].ReqState=USART_RQST_STATE_BUSY;
        rx_requests[USART_Request.USART_ID].Buffer.dataByte=USART_Request.PtrtoBuffer;
        rx_requests[USART_Request.USART_ID].Buffer.size=USART_Request.length;
        rx_requests[USART_Request.USART_ID].CallBack=USART_Request.CallBack;
        rx_requests[USART_Request.USART_ID].Buffer.Pos_Index=0;

        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 |= USART_RE_ENABLE;
        ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART_Request.USART_ID])->CR1 |= USART_RXNEIE_ENABLE;
    }

    return Error_Status;
}



void USART1_IRQHandler(void)
{
    if(((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->SR&USART_TC_FLAG && tx_requests[USART1].ReqState==USART_RQST_STATE_BUSY)
    {
        if(tx_requests[USART1].Buffer.Pos_Index < tx_requests[USART1].Buffer.size)
        {
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->DR=tx_requests[USART1].Buffer.dataByte[tx_requests[USART1].Buffer.Pos_Index];
            tx_requests[USART1].Buffer.Pos_Index++;
        }
        else
        {
            //will enter here when null terminator is sent
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->CR1 &= ~(USART_TE_ENABLE);
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->CR1 &= ~(USART_TXEIE_ENABLE);
            tx_requests[USART1].ReqState=USART_RQST_STATE_READY;
            if(tx_requests[USART1].CallBack!=NULL_POINTER)
            {
                tx_requests[USART1].CallBack();
            }
            else
            {
                // do nothing
            }

        }
    }
    if(((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->SR&USART_RXNE_FLAG && rx_requests[USART1].ReqState==USART_RQST_STATE_BUSY)
    {
        if(rx_requests[USART1].Buffer.Pos_Index < rx_requests[USART1].Buffer.size)
        {
            rx_requests[USART1].Buffer.dataByte[rx_requests[USART1].Buffer.Pos_Index]=((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->DR;
            rx_requests[USART1].Buffer.Pos_Index++;
        }
        else
        {
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->CR1 &= ~(USART_RE_ENABLE);
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART1])->CR1 &= ~(USART_RXNEIE_ENABLE);
            rx_requests[USART1].ReqState=USART_RQST_STATE_BUSY;
            if(rx_requests[USART1].CallBack!=NULL_POINTER)
            {
                rx_requests[USART1].CallBack();
            }
            else
            {
                // do nothing
            }
        }
    }

}



void USART2_IRQHandler(void)
{
    if(((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->SR&USART_TC_FLAG && tx_requests[USART2].ReqState==USART_RQST_STATE_BUSY)
    {
        if(tx_requests[USART2].Buffer.Pos_Index < tx_requests[USART2].Buffer.size)
        {
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->DR=tx_requests[USART2].Buffer.dataByte[tx_requests[USART2].Buffer.Pos_Index];
            tx_requests[USART2].Buffer.Pos_Index++;
        }
        else
        {
            //will enter here when null terminator is sent
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->CR1 &= ~(USART_TE_ENABLE);
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->CR1 &= ~(USART_TXEIE_ENABLE);
            tx_requests[USART2].ReqState=USART_RQST_STATE_READY;
            if(tx_requests[USART2].CallBack!=NULL_POINTER)
            {
                tx_requests[USART2].CallBack();
            }
            else
            {
                // do nothing
            }

        }
    }
    if(((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->SR&USART_RXNE_FLAG && rx_requests[USART2].ReqState==USART_RQST_STATE_BUSY)
    {
        if(rx_requests[USART2].Buffer.Pos_Index < rx_requests[USART2].Buffer.size)
        {
            rx_requests[USART2].Buffer.dataByte[rx_requests[USART2].Buffer.Pos_Index]=((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->DR;
            rx_requests[USART2].Buffer.Pos_Index++;
        }
        else
        {
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->CR1 &= ~(USART_RE_ENABLE);
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART2])->CR1 &= ~(USART_RXNEIE_ENABLE);
            rx_requests[USART2].ReqState=USART_RQST_STATE_BUSY;
            if(rx_requests[USART2].CallBack!=NULL_POINTER)
            {
                rx_requests[USART2].CallBack();
            }
            else
            {
                // do nothing
            }
        }
    }
}



void USART6_IRQHandler(void)
{
    if(((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->SR&USART_TC_FLAG && tx_requests[USART6].ReqState==USART_RQST_STATE_BUSY)
    {
        if(tx_requests[USART6].Buffer.Pos_Index < tx_requests[USART6].Buffer.size)
        {
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->DR=tx_requests[USART6].Buffer.dataByte[tx_requests[USART6].Buffer.Pos_Index];
            tx_requests[USART6].Buffer.Pos_Index++;
        }
        else
        {
            //will enter here when null terminator is sent
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->CR1 &= ~(USART_TE_ENABLE);
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->CR1 &= ~(USART_TXEIE_ENABLE);
            tx_requests[USART6].ReqState=USART_RQST_STATE_READY;
            if(tx_requests[USART6].CallBack!=NULL_POINTER)
            {
                tx_requests[USART6].CallBack();
            }
            else
            {
                // do nothing
            }

        }
    }
    if(((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->SR&USART_RXNE_FLAG && rx_requests[USART6].ReqState==USART_RQST_STATE_BUSY)
    {
        if(rx_requests[USART6].Buffer.Pos_Index < rx_requests[USART6].Buffer.size)
        {
            rx_requests[USART6].Buffer.dataByte[rx_requests[USART6].Buffer.Pos_Index]=((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->DR;
            rx_requests[USART6].Buffer.Pos_Index++;
        }
        else
        {
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->CR1 &= ~(USART_RE_ENABLE);
            ((volatile USART_PerRegs_t *)USART_BaseAddresses[USART6])->CR1 &= ~(USART_RXNEIE_ENABLE);
            rx_requests[USART6].ReqState=USART_RQST_STATE_BUSY;
            if(rx_requests[USART6].CallBack!=NULL_POINTER)
            {
                rx_requests[USART6].CallBack();
            }
            else
            {
                // do nothing
            }
        }
    }
}