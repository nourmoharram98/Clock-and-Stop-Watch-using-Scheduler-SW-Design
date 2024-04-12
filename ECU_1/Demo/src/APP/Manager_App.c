/**
 * @file App.c
 * @author your name (you@domain.com)
 * @brief implementation of initialization functions
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
*/

/**
 * @brief Implementation of the runnable that will iterate over the digits to print them on LCD 
 * @brief Implementation of the runnable that will check for the switches in the system
 * @brief Implementation of the function that will check for the global flag of the mode (Clock or Stop Watch) and toggle the flag of mode according to current state 
 * @brief Implementation of the function that will check for
 edit mode and enable w disable switches and call function of state 
 machine* @brief 
 * @brief Implementation of the function of edit mode state machine 
    in General edit mode :
        -user can use up/down/right/left/ok/edit switches
-if up/down/left/right switches is pressed the function will jump to its relative state
                                                      
            Switch  |    Relative state                                      |  Limitation
            --------|--------------------------------------------------------|--------------------------------
            Up      |    Decrement the value of cursor set position by 64    |  block decrement if at min row or set to max row
            Down    |    Increment the value of cursor set position by 64    |  block increment if at max row or 
            Left    |    Decrement the value of cursor set position by 1     |  block decrement if at min column or
            Right   |    Increment the value of cursor set position by 1     |  block decrement if at max column or ...
        
        -if ok switch pressed the function will change the edit mode to Idle mode
    in Digit Edit mode:
        
            Switch  |    Relative sate                                               |    Limitation
            --------|----------------------------------------------------------------|---------------------------
            Up      |  Decrement the value of the digit relative to cursor position  |  no decrement more than 0
            Down    |  Increment the value of the digit relative to cursor position  |  no increment more than 9

        -up/down switches are checked and their corresponding functions will be called
        -in case user incremented to boundary values value must overflow again
    in Idel Edit mode:
        - inhibit the functionality of the up/down/right/left switches
        - turn off cursor
 * @brief function for up switch - take in consideration the boundary values and overflow and mode of edit (General or Digit)
 * @brief function for down switch - take in consideration the boundary values and overflow and mode of edit (General or Digit)
 * @brief function for right switch - take in consideration the boundary values and overflow
 * @brief function for left switch - take in consideration the boundary values and overflow
 * 
 * 
*/

/*--------------------------------Includes----------------------------*/
    #include "Std_Types.h"
    #include "HAL/LCD/HAL_LCD.h"
    #include "HAL/SWITCH/HAL_SWITCH.h"
    #include "APP/Manager.h"
    #include "HAL/LED/LED.h"
/*--------------------------------------------------------------------*/

/*----------------------------Global Variables-----------------------*/
    u32 Mode=Clock_Mode;
    Operation_Types_t Operation_type=Init_Operation;
    extern u8 StopWatch_Status;
/*--------------------------------------------------------------------*/

/*----------------------------STATIC FUNCTION-------------------------*/
    static void Print_ClockFrame();
    static void Print_StopWatchFrame();
    static void print_frame_thread(Modes_t Copy_Mode);
    static void DisplayOnLCD(Modes_t Copy_Mode);
    static void ChangePrintState(Modes_t Copy_Mode,u8 Copy_PrintState);
/*--------------------------------------------------------------------*/

void Manager_Runnable(void)
{
    u32 Previous_Mode = Mode^1;
    ChangePrintState(Previous_Mode,DIGIT_STATE_NOT_PRINT);
    
    switch(Operation_type)
    {
        case Init_Operation:
            print_frame_thread(Mode);
            ChangePrintState(Mode,DIGIT_STATE_PRINT);
            Operation_type=Idle_Operation;
        break;

        case Idle_Operation:
            DisplayOnLCD(Mode);
        break;

        case GeneralEdit_Operation:
            //GeneralEditMode(Mode);
        break;

        case DigitEdit_Operation:
           // DigitEditMode(Mode);
        break;
        
        default:
            /*Do Nothing*/
        break;
    }
  
}

static void print_frame_thread(Modes_t Copy_Mode) //period = 4
{
    switch(Copy_Mode)
    {
        case Clock_Mode:
            Print_ClockFrame();
            break;

        case StopWatch_Mode:
            Print_StopWatchFrame();
            break;

        default:

        break;
    }
   

}

static void Print_ClockFrame()
{
    LCD_ClearScreenAsync();
    LCD_WriteStringAsync("CLOCK   /  /    ",16); 
    LCD_SetCursorPosAsync(1, 0);                                 
    LCD_WriteStringAsync("    :  :  : 00",14); 
}

static void Print_StopWatchFrame()
{
    LCD_ClearScreenAsync();
    LCD_WriteStringAsync("STOPWATCH START",15); 
    LCD_SetCursorPosAsync(1, 0);                                
    LCD_WriteStringAsync("    :  :  : 00",14); 
}

static void DisplayOnLCD(Modes_t Copy_Mode)
{
    switch(Copy_Mode)
    {
        case Clock_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_CLK_MODE;index++)
            {
                if(Clock_Date_Digits[index].digit_state==DIGIT_STATE_PRINT)
                {
                    LCD_SetCursorPosAsync(Clock_Date_Digits[index].x_pos,Clock_Date_Digits[index].y_pos);
                    LCD_enuWriteNumber(Clock_Date_Digits[index].value);
                    Clock_Date_Digits[index].digit_state=DIGIT_STATE_NOT_PRINT;
                }
            }
        break;

        case StopWatch_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_STOPW_MODE;index++)
            {
                if(Stop_Watch_Digits[index].digit_state==DIGIT_STATE_PRINT)
               {
                    LCD_SetCursorPosAsync(Stop_Watch_Digits[index].x_pos,Stop_Watch_Digits[index].y_pos);
                    LCD_enuWriteNumber(Stop_Watch_Digits[index].value);
                    Stop_Watch_Digits[index].digit_state=DIGIT_STATE_NOT_PRINT;
               }
            }
        break;

        default:

        break;
    }
}

static void ChangePrintState(Modes_t Copy_Mode,u8 Copy_PrintState)
{
    switch(Copy_Mode)
    {
        case Clock_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_CLK_MODE;index++)
            {
                Clock_Date_Digits[index].digit_state=Copy_PrintState;
            }
        break;

        case StopWatch_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_STOPW_MODE;index++)
            {
                Stop_Watch_Digits[index].digit_state=Copy_PrintState;
            }
        break;

        default:

        break;
    }
}

void ControlSwitches_Runnable(void)
{
    
    u32 Switch_Mode_Status=0;
    u32 Switch_Ok_Status=0;
    static u32 Previous_state_Mode=0;
    static u32 Switch_Ok_Prev_state=0;

    HAL_SWITCH_enuGetSwitchState( SWITCH_NUMONE , &Switch_Mode_Status );
    HAL_SWITCH_enuGetSwitchState(SWITCH_NUMTWO,&Switch_Ok_Status);
    
    if(Switch_Mode_Status==1)
    {
        Previous_state_Mode=Switch_Mode_Status;
    }
    else if(Previous_state_Mode==1 && Switch_Mode_Status==0)
    {
        //Send unique data via uart
        Mode ^=1;
        Operation_type=Init_Operation;
        Previous_state_Mode=Switch_Mode_Status;
    }



    if( Switch_Ok_Status==1 )
    {
        Switch_Ok_Prev_state=Switch_Ok_Status;
    }
    else if( ( Switch_Ok_Prev_state==1 && Switch_Ok_Status==0 ) )
    {
        //send unique data via uart

        StopWatch_Status^=1;

        Switch_Ok_Prev_state=Switch_Ok_Status;

        switch ( Mode )
        {
            case Clock_Mode :

                switch (Operation_type)
                {
                    case GeneralEdit_Operation :
                        Operation_type = Idle_Operation ;
                    break;

                    case DigitEdit_Operation :
                        Operation_type = GeneralEdit_Operation;
                    break;

                    default :

                    break;
                }

            break;

            case StopWatch_Mode :
                StopWatch_Status ^=1;
            break;

            default:
                /*Do Nothing*/
            break;
        }

    }
}
