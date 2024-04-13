#include "SERVICE/COMM/UART_COMM.h"

#define CRC_POLYNOMIAL 0xEDB88320  // CRC-32 polynomial
#define INITIAL_CRC_VALUE 0xFFFFFFFF

u8 Requests_Buffer[NUMBER_OF_REQUESTS];
u8 Received_Requests[NUMBER_OF_REQUESTS];
typedef struct 
{
    U8 original_data;
    U32 crc_checksum;
} DataWithCRC;

void Communication_Manager(U8 RAW_DATA)
{
    // Extracting the original data (lower 4 bits)
    U8 original_data = RAW_DATA & 0x0F;

    // Initialize CRC value
    U32 crc = INITIAL_CRC_VALUE;

    // Calculate CRC checksum for the original data
    crc = calculate_crc(original_data, crc);

    // Combine the original data and the CRC checksum
    U8 CRC_handler_1 = (crc >> 28);                      // Shift CRC to the lower 4 bits
    u8 CRC_handler_2 = (CRC_handler_1 << 4);            // Shift CRC to the higgher 4 bits
    
    U8 data_with_crc = (CRC_handler_2) | (original_data); // Shift CRC to upper 4 bits   1010                     
    // Now you can use the data_with_crc as needed
    // For example, send it over a communication channel
}

// Function to calculate CRC checksum using CRC-32 polynomial
U32 calculate_crc(U8 data, U32 crc)
{
    for (int i = 0; i < 8; ++i) 
    {
        if ((data ^ crc) & 1)
            crc = (crc >> 1) ^ CRC_POLYNOMIAL;
        else
            crc >>= 1;
        data >>= 1;
    }
    return crc;
}

// Function to process received data
void process_received_data(U8 recieved_data_with_crc)
{
    // Verify CRC checksum
    bool is_correct = verify_crc(recieved_data_with_crc);

    if (is_correct) 
    {
        // Data is correct
        // Do something with received_data.original_data //yara
    } 
    else 
    {
        // Data is corrupted
        // Handle error
    }
}

// Function to verify CRC checksum
bool verify_crc(U8 recieved_data_with_crc)
{
    U32 crc = INITIAL_CRC_VALUE;

    U8 RAW_Data = recieved_data_with_crc & 0x0F;

    // Calculate CRC checksum for the original data
    crc = calculate_crc( RAW_Data , crc );

    // Combine the original data and the CRC checksum
    U8 CRC_handler_1 = (crc >> 28);                               // Shift CRC to the lower 4 bits
    u8 CRC_handler_2 = (CRC_handler_1 << 4);                      // Shift CRC to the higgher 4 bits
    
    U8 data_with_crc_verifier = (CRC_handler_2) | (RAW_Data);     // Shift CRC to upper 4 bits      

    // Compare calculated CRC checksum with received CRC checksum
    return ( recieved_data_with_crc == data_with_crc_verifier );
}

Sys_enuErrorStates_t Communication_Sender(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    //static counter to keep track of the buffer index
    static u8 counter=0;

    //copy the data from buffer to local variable
    u8 Copy_Data= Requests_Buffer[counter];

    //erase the data in the current index
    Requests_Buffer[counter]=0;

    //increment the counter of requests buffer to move to next data
    counter++;

    //check if counter reached the limit of buffer to initialize it again
    if(counter==NUMBER_OF_REQUESTS)
    {
        counter=0;
    }

    //create the local USART Request data to be sent to USART transmiter
    USART_Request_t Local_Data={
        .USART_ID=USART1,
        .PtrtoBuffer=&Copy_Data,
        .length=1,
    };
    USART_SendByte(Local_Data);
    Error_Status=OK;
    return Error_Status;
}

Sys_enuErrorStates_t Communication_Receiver(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;

    //counter for tracking the buffer of received requests
    static u8 Receiver_Counter=0;

    // local variable to carry received data
    u8 Received_Data=0;

    //local USART Request to carry request configurations
    USART_Request_t Local_ReceivedData=
    {
        .USART_ID=USART1,
        .PtrtoBuffer=&Received_Data,
        .length=1,
    };

    //check USART Bus for new data
    USART_GetByte(Local_ReceivedData);

    //save the data to Received Requests buffer
    Received_Requests[Receiver_Counter]=Received_Data;


    Receiver_Counter++;

    //check if counter reach the threshold of  Requests 
    if(Receiver_Counter==NUMBER_OF_REQUESTS)
    {
        Receiver_Counter=0;
    }
    Error_Status=OK;
    return Error_Status;

}