#include "SERVICE/COMM/UART_COMM.h"

#define DATA_SIZE 4 // Size of data in bits

//from USART.c and carry the received request
extern u8 Received_Request;



u8 Requests_Buffer[NUMBER_OF_REQUESTS];
//u8 Received_Requests[NUMBER_OF_REQUESTS];
volatile u8 ComMnager_counter=0;

// Communication Manager function
void TX_Communication_Manager(U8 RAW_DATA) 
{

    U8 processed_data = append_crc(RAW_DATA);

    // fill the buffer array with the data to be send via UART
    Requests_Buffer[ComMnager_counter] = processed_data;

    //increment the counter of requests buffer to move to next data
    ComMnager_counter++;

    //check if counter reached the limit of buffer to initialize it again
    if(ComMnager_counter==NUMBER_OF_REQUESTS)
    {
        ComMnager_counter=0;
    }
}

// Function to append CRC to data
U8 append_crc(U8 data) 
{
    U8 crc = calculate_crc(data);
    return (data << 4) | crc; // Append CRC to data
}

// Function to calculate CRC checksum
U8 calculate_crc(U8 data) 
{
    // CRC polynomial: x^4 + x^3 + 1, represented as 0b11011
    U8 crc_poly = 0b1101;
    U8 crc = 0; // Initialize CRC to 0

    // Perform CRC calculation
    for (int i = 0; i < DATA_SIZE; i++) 
    {
        // XOR CRC with current data bit
        crc ^= (data >> (DATA_SIZE - 1 - i)) & 0x01;

        if (crc & 0x01) 
        {
            crc ^= crc_poly;
        }
        crc >>= 1; // Shift CRC to the right
    }

    return crc & 0x0F; // Ensure CRC is 4 bits long
}

// Function to process received data and verify CRC
void process_received_data(U8 received_data) 
{
    //counter for tracking the buffer of received requests
    //static u8 Receiver_Counter=0;

   // Received_Requests[Receiver_Counter]=received_data;
    
    U8 received_crc = received_data & 0x0F; // Extract received CRC
    U8 received_data_only = received_data >> 4; // Extract received data

    U8 calculated_crc = calculate_crc(received_data_only); // Calculate CRC for received data

    // Verify CRC
    if (received_crc == calculated_crc) 
    {
        Command_Handler(received_data_only);
    } 
    else 
    {
        //Ignore Data  -> 
    }

}

Sys_enuErrorStates_t Communication_Sender(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    //static counter to keep track of the buffer index
    static u8 CommSender_counter=0;
    if(CommSender_counter != ComMnager_counter)
    {
         //copy the data from buffer to local variable
        u8 Copy_Data= Requests_Buffer[CommSender_counter];
        if(Copy_Data!=0)
        {
            //erase the data in the current index
            Requests_Buffer[CommSender_counter]=0;

            //increment the counter of requests buffer to move to next data
            CommSender_counter++;
            if(CommSender_counter==NUMBER_OF_REQUESTS)
            {
                CommSender_counter=0;
            }
        

            //create the local USART Request data to be sent to USART transmiter
            USART_Request_t Local_Data=
            {
                .USART_ID=USART1,
                .PtrtoBuffer=&Copy_Data,
                .length=1,
                .CallBack=NULL_POINTER
            };

            USART_SendByte(Local_Data);
            Error_Status=OK;
        }

    
    }
   
    return Error_Status;
}

Sys_enuErrorStates_t Communication_Receiver(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    // u8 Copy_Data_Received=0;
    // USART_Request_t Local_Data=
    // {
    //     .USART_ID=USART1,
    //     .PtrtoBuffer=&Copy_Data_Received,
    //     .length=1,
    //     .CallBack=NULL_POINTER
    // };
    // USART_GetByte(Local_Data);
    
    if(Received_Request!=0)
    { 
        process_received_data(Received_Request);    
        //Command_Handler(Received_Request);
        Received_Request=0;
        Error_Status=OK;
    }
    else
    {
        Error_Status=NOT_OK;
    }
    // if(Copy_Data_Received!=0)
    // { 
    //     process_received_data(Copy_Data_Received);    
    //     //Command_Handler(Received_Request);
    //     Copy_Data_Received=0;
    //     Error_Status=OK;

    // }
    // else
    // {
    //     Error_Status=NOT_OK;
    // }
    return Error_Status;

}