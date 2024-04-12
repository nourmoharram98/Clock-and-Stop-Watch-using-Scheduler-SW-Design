#include"Std_Types.h"
#include "APP/App_Config.h"
#include "APP/Clock_Date_App.h"
#include "APP/Stop_Watch_App.h"



extern unit_Info_t Clock_Date_Digits[NUMBER_OF_DIGITS_CLK_MODE];
extern unit_Info_t Stop_Watch_Digits[NUMBER_OF_DIGITS_STOPW_MODE];




typedef enum
{
    Init_Operation,

    Idle_Operation,

    GeneralEdit_Operation,

    DigitEdit_Operation,
}Operation_Types_t;


/*--------------------------------Types Defs-------------------------*/
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
/*-------------------------------------------------------------------*/




/*------------------------------Functions-----------------------------*/
void Application_Runnable(void);

