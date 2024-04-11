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
 #include "HAL/LCD/LCD.h"
 #include "APP/Manager.h"

 #define Manager_Periodicity 6
// #include"APP/Clock_Date_App.h"
// #include"APP/Manager.h"
// extern unit_Info_t Digits[15];

// u8 Global_Mode = // Clock mode - Stop watch mode
// u8 Edit_mode  = //General - Digit - Idle
// //to keep track of cursor position in LCD
// u8 Global_X_pos=0;
// u8 Global_Y_Pos=0;
// /**
// enum lel switches IDs hanstkhdmha 3lshan ne enable el readings menhom aw la 3la hasb mhtagen wla la
// */
// enum{
//     Up,
//     Down,
//     Left,
//     Right,
//     Edit,
//     Mode,
//     Ok
// }Switches;
// /**
// array lel switch states
// */
// u8 Switches_mode[7]=
// {
//     [Up] =0,
//     [Down]=0,
//     [Left]=0,
//     [Right]=0,
//     [Edit]=1,
//     [Mode]=1,
//     [Ok]=0,
// }

// u8 Manager(u8 flag_for_array_to_be_retreived)
// {
//     u8 Error_Status=0;you 
//     //read the mode switch button if previous reading != current reading toggle the global mode state
//     return Error_Status;
// } 

/********************************************************************/

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

    typedef enum
    {
        SetCursor,
        WriteNumber
    }Operation_State_t;

    typedef enum
    {
        Clock,
        StopWatch
    }Mode_t;
/*-------------------------------------------------------------------*/

/*---------------------------Static Functions------------------------*/
    static void print_frame_thread(void);
    static void operation_thread(void);
    static void CLOCK_THREAD(void);
    //static void STOPWATCH_THREAD(void);
/*-------------------------------------------------------------------*/

/*----------------------------Global Variables-----------------------*/
    static uint8 CLKcounter                       = 0;
    static Operation_State_t Operation_State      = SetCursor;

    Mode_t   Mode                                = Clock;
    states_t state                               = print_frame ;
    print_frame_state_t print_frame_state        = print_first_line ;
/*-------------------------------------------------------------------*/

void Application_Runnable(void)
{

    switch (Mode)
    {
        case Clock:
            CLOCK_THREAD();
        break;

        case StopWatch:
            //STOPWATCH_THREAD();
        break;    

    }

}

/*-------------------------------------------------------------------*/

static void CLOCK_THREAD(void)
{
    CLKcounter+=Manager_Periodicity;   

    switch (state)
    {
        case print_frame:
            print_frame_thread();
        break;

        case operation:
            operation_thread();
        break;

        default:

        break;
    }     
}

static void print_frame_thread() //period = 4
{
    
    switch(print_frame_state)
    {

        case print_first_line:

            LCD_enuWriteStringAsync("CLOCK 31/12/2000",16); // (16 x 2) x 2 = 64 ms  -> 70 ms
            
            print_frame_state=wait1;

        break;

        case wait1:
            if(CLKcounter>=70)
            {
                CLKcounter=0;
                print_frame_state=set_cursor_second_line;
            }
        break;

        case set_cursor_second_line:
        
            LCD_SetCursorPosAsync(2, 1);             // ( 1 x 2) x 2 = 4 ms take care about lcd refresh rate 16 ms            
           
            print_frame_state = print_second_line;
            CLKcounter=0;
        break;

        case print_second_line:
            LCD_enuWriteStringAsync("  23:59:55:100",14); // (11 x 2) x 2 = 44 ms   -> 60 ms
            print_frame_state=wait2;
        break;

        case wait2:
            if(CLKcounter>=70)
            {
                CLKcounter=0;
                print_frame_state=end;
            }
        break;

        case end:
            state = operation;
            CLKcounter=0;
            print_frame_state=print_first_line;
        break;  
    }

}

/********************************************************************/


/********************************************************************/

static uint8 i=0;

static void operation_thread(void)//every 8ms
{
    if(Clock_Date_Digits[i].digit_state == DIGIT_STATE_PRINT)
    {
        switch (Operation_State)
        {
            case SetCursor:
                LCD_SetCursorPosAsync(Clock_Date_Digits[i].x_pos, Clock_Date_Digits[i].y_pos);

                Operation_State = WriteNumber;
            break;

            case WriteNumber:
                LCD_enuWriteNumber(Clock_Date_Digits[i].value);

                Operation_State=SetCursor;

                Clock_Date_Digits[i].digit_state = DIGIT_STATE_NOT_PRINT;
                
                i++;
                if(i > 14)
                {
                    i = 0;
                }
            break; 


        }   
    }
    else // DIGIT_STATE_NOT_PRINT
    {
        i++;
        if(i > 14)
        {
            i = 0;
            Operation_State=SetCursor; // Reset mystate when wrapping around
        }
    }
}

/********************************************************************/



