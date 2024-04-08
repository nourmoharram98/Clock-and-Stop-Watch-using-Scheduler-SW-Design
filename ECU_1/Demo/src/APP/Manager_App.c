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
#include"APP/Clock_Date_App.h"
#include"APP/Manager.h"
extern unit_Info_t Digits[15];

u8 Global_Mode = // Clock mode - Stop watch mode
u8 Edit_mode  = //General - Digit - Idle
//to keep track of cursor position in LCD
u8 Global_X_pos=0;
u8 Global_Y_Pos=0;
/**
enum lel switches IDs hanstkhdmha 3lshan ne enable el readings menhom aw la 3la hasb mhtagen wla la
*/
enum{
    Up,
    Down,
    Left,
    Right,
    Edit,
    Mode,
    Ok
}Switches;
/**
array lel switch states
*/
u8 Switches_mode[7]=
{
    [Up] =0,
    [Down]=0,
    [Left]=0,
    [Right]=0,
    [Edit]=1,
    [Mode]=1,
    [Ok]=0,
}

u8 Manager(u8 flag_for_array_to_be_retreived)
{
    u8 Error_Status=0;you 
    //read the mode switch button if previous reading != current reading toggle the global mode state
    return Error_Status;
}   


u8 Application_Runnable(void)
{
    Clock_Application();
    Stop_WatchApplication();
    if(Globalmode==Clock_mode)
        call the LCD apis to display the clock mode by retrieving the data from externed array
        Manager(clock digits array)
    else
        call the LCD apis to display the stop watch mode by retrieiving the data from externed array
        Manager(stop watch digits array)

}

u8 Clock_Application(void)
{
 //not implemented in this file
}

u8 Stop_WatchApplication(void)
{
 //not implemented in this file
}

u8 Check_for_switch_runnable(void)
{

}

//up/down/right/left functions

/**
Ok button function:
change edit mode from (digit to global) or (global to operation)
*/