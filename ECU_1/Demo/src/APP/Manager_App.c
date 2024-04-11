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

 #include "Std_Types.h"
 #include "HAL/LCD/HAL_LCD.h"
 #include "HAL/SWITCH/HAL_SWITCH.h"
 #include "APP/Manager.h"

 #define Manager_Periodicity 10

u32 Mode=Clock_Mode;
Operation_Types_t Operation_type=Init_Operation;

/*----------------------------Global Variables-----------------------*/
static uint8 counter= 0;
states_t  state  = print_frame ;
print_frame_state_t print_frame_state = print_first_line ;


// static void print_frame_thread() //period = 4
// {
    
//     switch(print_frame_state)
//     {

//         case print_first_line:

//             LCD_WriteStringAsync("CLOCK 31/12/9999",16); // (16 x 2) x 2 = 64 ms  -> 70 ms
            
//             print_frame_state=wait1;

//         break;

//         case wait1:
//             if(counter>=70)
//             {
//                 counter=0;
//                 print_frame_state=set_cursor_second_line;
//             }
//         break;

//         case set_cursor_second_line:
        
//             LCD_SetCursorPosAsync(1, 0);                    // ( 1 x 2) x 2 = 4 ms take care about lcd refresh rate 16 ms            
           
//             print_frame_state = print_second_line;
//             counter=0;
//         break;

//         case print_second_line:
//             LCD_WriteStringAsync("  23:59:55:100",14); // (11 x 2) x 2 = 44 ms   -> 60 ms
//             print_frame_state=wait2;
//         break;

//         case wait2:
//             if(counter>=70)
//             {
//                 counter=0;
//                 print_frame_state=end;
//             }
//         break;

//         case end:
//             state = operation;
//             counter=0;
//             print_frame_state=print_first_line;
//         break;  
//     }

// }

static void Print_ClockFrame()
{
    switch(print_frame_state)
    {

        case print_first_line:

            LCD_WriteStringAsync("CLOCK 11/04/2024",16); // (16 x 2) x 2 = 64 ms  -> 70 ms

            print_frame_state=wait1;

        break;

        case wait1:
            if(counter>=70)
            {
                counter=0;
                print_frame_state=set_cursor_second_line;
            }
        break;

        case set_cursor_second_line:
        
            LCD_SetCursorPosAsync(1, 0);                    // ( 1 x 2) x 2 = 4 ms take care about lcd refresh rate 16 ms            
           
            print_frame_state = print_second_line;
            counter=0;
        break;

        case print_second_line:
            LCD_WriteStringAsync("  09:15:32:100",14); // (11 x 2) x 2 = 44 ms   -> 60 ms

            print_frame_state=wait2;
        break;

        case wait2:
            if(counter>=70)
            {
                counter=0;
                print_frame_state=end;
            }
        break;

        case end:
            state = operation;
            counter=0;
            print_frame_state=print_first_line;
        break;  
    }
}

static void Print_StopWatchFrame()
{
    switch(print_frame_state)
    {

        case print_first_line:

            LCD_WriteStringAsync("STOPWATCH START",15); // (16 x 2) x 2 = 64 ms  -> 70 ms
            
            print_frame_state=wait1;

        break;

        case wait1:
            if(counter>=70)
            {
                counter=0;
                print_frame_state=set_cursor_second_line;
            }
        break;

        case set_cursor_second_line:
        
            LCD_SetCursorPosAsync(1, 0);                    // ( 1 x 2) x 2 = 4 ms take care about lcd refresh rate 16 ms            
           
            print_frame_state = print_second_line;
            counter=0;
        break;

        case print_second_line:
            LCD_WriteStringAsync("  00:00:00:000",14); // (11 x 2) x 2 = 44 ms   -> 60 ms

            print_frame_state=wait2;
        break;

        case wait2:
            if(counter>=70)
            {
                counter=0;
                print_frame_state=end;
            }
        break;

        case end:
            state = operation;
            counter=0;
            print_frame_state=print_first_line;
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

// static void Clock_Manager(void)
// {
//     switch(ModeOfOperation)
//     {
//         case Clk_Idle_Mode:
//             Display_OnLCD(Mode);
//             break;
//         case Clk_GeneralEdit_Mode:
//             break;
//         case Clk_DigitEdit_Mode:
//             break;
//         default:
//             break;
//     }
  
// }



// static void StopWatch_Manager(void)
// {
//     switch(ModeOfOperation)
//     {
//         case Clk_Idle_Mode:
//             Display_OnLCD(Mode);
//             break;
//         case Clk_GeneralEdit_Mode:
//             break;
//         case Clk_DigitEdit_Mode:
//             break;
//         default:
//             break;
//     }
// }

// static void GeneralEditMode(Modes_t Copy_Mode)
// {
//     //read the requests of up/down/right/left buttons
//     //read the request from edit button to move to digitedit operation 
//     //read the ok button to return to idle operation
// }
// static void DigitEditMode(Modes_t Copy_Mode)
// {
//     //read the requests of up/down buttons
//     //read the ok button to save the changes and return to idle operation
// }
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




void Manager_Runnable(void)
{
    counter+=Manager_Periodicity;   

    switch(Operation_type)
    {
        case Init_Operation:
            print_frame_thread(Mode);
            if(state==operation)
            {
                Operation_type=Idle_Operation;
            }
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
            break;
    }
  
}



// void Application_Runnable(void)
// {
//     counter+=Manager_Periodicity;   

//     switch (state)
//     {
//         case print_frame:
//             print_frame_thread();
//         break;

//         case operation:
//             Manager_Runnable();
//         break;

//         default:

//         break;
//     }
// // }
// void Application_Runnable(void)
// {
//     counter+=Manager_Periodicity;   

//     Manager();
// }

void ControlSwitches_Runnable(void)
{
    static u32 Local_counter=0;
    Local_counter+=100;

    if(Local_counter==10000)
    {
        LCD_ClearScreenAsync();
        Mode^=1;
        Operation_type=Init_Operation;
        Local_counter=0;
    }
    // static u32 Switch_Status=0; //released
    // static u32 Previous_Switch_Status=0;
    // HAL_SWITCH_enuGetSwitchState(SWITCH_NUMONE,&Switch_Status);
    // if(Switch_Status==1)
    // {
    //     Previous_Switch_Status=Switch_Status;
    // }
    // else if(Previous_Switch_Status==1 && Switch_Status==0)
    // {
    //     if(Mode==Clock_Mode)
    //     {
    //         Mode=StopWatch_Mode;
    //     }
    //     else
    //     {
    //         Mode=Clock_Mode;
    //     }
    //     Previous_Switch_Status=0;
    // }
}
