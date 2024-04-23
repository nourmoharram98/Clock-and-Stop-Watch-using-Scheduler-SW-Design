/**
 * @file Stop_Watch_App.c
 * @author your name (you@domain.com)
 * @brief implementation of stop watch mode and global variables, arrays and flags required for the mode
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include"APP/App_Config.h"
#include "./APP/Stop_Watch_App.h"
#include "Std_Types.h"

u8 StopWatch_Status=0;
extern u32 Mode;
unit_Info_t Stop_Watch_Digits[NUMBER_OF_DIGITS_STOPW_MODE]=
{
    [DeciSecond_hundred_SW]={
        .value=0,
        .x_pos=1,  // x hia al row
        .y_pos=11,   // y hia al col
        .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Second_unit_SW]={
        .value=0,
        .x_pos=1,
        .y_pos=9,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Second_tens_SW]={
        .value=0,
        .x_pos=1,
        .y_pos=8,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Minutes_unit_SW]={
        .value=0,
        .x_pos=1,
        .y_pos=6,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Minutes_tens_SW]={
        .value=0,
        .x_pos=1,
        .y_pos=5,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Hours_unit_SW]={
        .value=0,
        .x_pos=1,
        .y_pos=3,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Hours_tens_SW]={
        .value=0,
        .x_pos=1,
        .y_pos=2,
                .digit_state=DIGIT_STATE_NOT_PRINT

    }
};



void Stopwatch_Runnable(void)
{
    //Runnable_Execution_time();
    if(StopWatch_Status==1)
    {
        Stop_Watch_Digits[DeciSecond_hundred_SW].value++;
        Stop_Watch_Digits[DeciSecond_hundred_SW].digit_state=DIGIT_STATE_PRINT;

        if (Stop_Watch_Digits[DeciSecond_hundred_SW].value > 9) 
        {
            Stop_Watch_Digits[DeciSecond_hundred_SW].value = 0;
            Stop_Watch_Digits[DeciSecond_hundred_SW].digit_state=DIGIT_STATE_PRINT;

            Stop_Watch_Digits[Second_unit_SW].value++;
            Stop_Watch_Digits[Second_unit_SW].digit_state=DIGIT_STATE_PRINT;

            if (Stop_Watch_Digits[Second_unit_SW].value > 9) {
                Stop_Watch_Digits[Second_unit_SW].value = 0;
                Stop_Watch_Digits[Second_unit_SW].digit_state=DIGIT_STATE_PRINT;

                Stop_Watch_Digits[Second_tens_SW].value++;
                Stop_Watch_Digits[Second_tens_SW].digit_state=DIGIT_STATE_PRINT;

                if (Stop_Watch_Digits[Second_tens_SW].value > 5) {
                    Stop_Watch_Digits[Second_tens_SW].value = 0;
                    Stop_Watch_Digits[Second_tens_SW].digit_state=DIGIT_STATE_PRINT;

                    Stop_Watch_Digits[Minutes_unit_SW].value++;
                    Stop_Watch_Digits[Minutes_unit_SW].digit_state=DIGIT_STATE_PRINT;

                    if (Stop_Watch_Digits[Minutes_unit_SW].value > 9) {
                        Stop_Watch_Digits[Minutes_unit_SW].value = 0;
                        Stop_Watch_Digits[Minutes_unit_SW].digit_state=DIGIT_STATE_PRINT;

                        Stop_Watch_Digits[Minutes_tens_SW].value++;
                        Stop_Watch_Digits[Minutes_tens_SW].digit_state=DIGIT_STATE_PRINT;

                        if (Stop_Watch_Digits[Minutes_tens_SW].value > 5) 
                        {
                            Stop_Watch_Digits[Minutes_tens_SW].value = 0;
                            Stop_Watch_Digits[Minutes_tens_SW].digit_state=DIGIT_STATE_PRINT;

                            Stop_Watch_Digits[Hours_unit_SW].value++;
                            Stop_Watch_Digits[Hours_unit_SW].digit_state=DIGIT_STATE_PRINT;
                        }
                            if (Stop_Watch_Digits[Hours_unit_SW].value > 9) 
                            {
                                Stop_Watch_Digits[Hours_unit_SW].value = 0;
                                Stop_Watch_Digits[Hours_unit_SW].digit_state=DIGIT_STATE_PRINT;

                                Stop_Watch_Digits[Hours_tens_SW].value++;
                                Stop_Watch_Digits[Hours_tens_SW].digit_state=DIGIT_STATE_PRINT;
                            }
                                if (Stop_Watch_Digits[Hours_tens_SW].value > 9) 
                                {
                                    Stop_Watch_Digits[Hours_tens_SW].value = 0;
                                    Stop_Watch_Digits[Hours_tens_SW].digit_state=DIGIT_STATE_PRINT;

                                }
                            }
                        }
                    }
                }
            }
    else
    {
        //do nothing
    }
   // Runnable_Execution_time();
   
}

void Reset_StopWatch(void)
{
    for(u8 index=0;index<NUMBER_OF_DIGITS_STOPW_MODE;index++)
    {
        Stop_Watch_Digits[index].value=0;
    }
}