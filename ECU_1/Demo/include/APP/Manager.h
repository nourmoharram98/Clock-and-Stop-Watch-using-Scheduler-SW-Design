#include"Std_Types.h"
#include "APP/App_Config.h"
#include "APP/Clock_Date_App.h"
#include "APP/Stop_Watch_App.h"
#include "SERVICE/COMM/UART_COMM.h"
extern unit_Info_t Clock_Date_Digits[NUMBER_OF_DIGITS_CLK_MODE];
extern unit_Info_t Stop_Watch_Digits[NUMBER_OF_DIGITS_STOPW_MODE];

/*--------------------------------Types Defs-------------------------*/
typedef enum
{
    Init_Operation,

    Idle_Operation,

    GeneralEdit_Operation,

    DigitEdit_Operation,
}Operation_Types_t;


typedef enum
{
    print_frame,
    operation
}states_t;


typedef enum 
{
    print_first_line,
    wait1,
    set_cursor_second_line,
    print_second_line,
    wait2,
    end   
}print_frame_state_t;

typedef struct
{
    u8 USART_ID;
    //u8 *PtrtoBuffer;
    //u16 length;
    //Ptrtofunc CallBack;
}USART_MOCK_Request_t;


typedef struct 
{
    U8 DATA;
    Switch_Status_t Switch_Status;
    Switch_Status_t Switch_PrevStatus;
}Ctrl_Switches_Data_t;
/*-------------------------------------------------------------------*/

/*------------------------------Functions-----------------------------*/
void Application_Runnable(void);
/*--------------------------------------------------------------------*/
