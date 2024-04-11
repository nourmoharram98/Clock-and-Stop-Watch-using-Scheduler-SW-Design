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

unit_Info_t Stop_Watch_Digits[NUMBER_OF_DIGITS_STOPW_MODE]=
{
    [DeciSecond_hundred]={
        .value=0,
        .x_pos=1,  // x hia al row
        .y_pos=11,   // y hia al col
        .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Second_unit]={
        .value=0,
        .x_pos=1,
        .y_pos=9,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Second_tens]={
        .value=0,
        .x_pos=1,
        .y_pos=8,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Minutes_unit]={
        .value=0,
        .x_pos=1,
        .y_pos=6,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Minutes_tens]={
        .value=0,
        .x_pos=1,
        .y_pos=5,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Hours_unit]={
        .value=0,
        .x_pos=1,
        .y_pos=3,
                .digit_state=DIGIT_STATE_NOT_PRINT

    },
    [Hours_tens]={
        .value=0,
        .x_pos=1,
        .y_pos=2,
                .digit_state=DIGIT_STATE_NOT_PRINT

    }
};



void Stopwatch_Runnable(void)
{
    Stop_Watch_Digits[DeciSecond_hundred].value++;
        Stop_Watch_Digits[DeciSecond_hundred].digit_state=DIGIT_STATE_PRINT;

    if (Stop_Watch_Digits[DeciSecond_hundred].value > 9) 
    {
        Stop_Watch_Digits[DeciSecond_hundred].value = 0;
                Stop_Watch_Digits[DeciSecond_hundred].digit_state=DIGIT_STATE_PRINT;

        Stop_Watch_Digits[Second_unit].value++;
        Stop_Watch_Digits[Second_unit].digit_state=DIGIT_STATE_PRINT;

        if (Stop_Watch_Digits[Second_unit].value > 9) {
            Stop_Watch_Digits[Second_unit].value = 0;
            Stop_Watch_Digits[Second_unit].digit_state=DIGIT_STATE_PRINT;

            Stop_Watch_Digits[Second_tens].value++;
        Stop_Watch_Digits[Second_tens].digit_state=DIGIT_STATE_PRINT;

            if (Stop_Watch_Digits[Second_tens].value > 6) {
                Stop_Watch_Digits[Second_tens].value = 0;
                        Stop_Watch_Digits[Second_tens].digit_state=DIGIT_STATE_PRINT;

                Stop_Watch_Digits[Minutes_unit].value++;
        Stop_Watch_Digits[Minutes_unit].digit_state=DIGIT_STATE_PRINT;

                if (Stop_Watch_Digits[Minutes_unit].value > 9) {
                    Stop_Watch_Digits[Minutes_unit].value = 0;
                            Stop_Watch_Digits[Minutes_unit].digit_state=DIGIT_STATE_PRINT;

                    Stop_Watch_Digits[Minutes_tens].value++;
        Stop_Watch_Digits[Minutes_tens].digit_state=DIGIT_STATE_PRINT;

                    if (Stop_Watch_Digits[Minutes_tens].value > 6) {
                        Stop_Watch_Digits[Minutes_tens].value = 0;
                                Stop_Watch_Digits[Minutes_tens].digit_state=DIGIT_STATE_PRINT;

                        Stop_Watch_Digits[Hours_unit].value++;
        Stop_Watch_Digits[Hours_unit].digit_state=DIGIT_STATE_PRINT;

                        if (Stop_Watch_Digits[Hours_unit].value > 9) {
                            Stop_Watch_Digits[Hours_unit].value = 0;
                                    Stop_Watch_Digits[Hours_unit].digit_state=DIGIT_STATE_PRINT;

                            Stop_Watch_Digits[Hours_tens].value++;
        Stop_Watch_Digits[Hours_tens].digit_state=DIGIT_STATE_PRINT;

                            if (Stop_Watch_Digits[Hours_tens].value > 9) {
                                Stop_Watch_Digits[Hours_tens].value = 0;
                                        Stop_Watch_Digits[Hours_tens].digit_state=DIGIT_STATE_PRINT;

                            }
                        }
                    }
                }
            }
        }
    }
    // if (Stop_Watch_Digits[DeciSecond_hundred].value > 9)
    // {
    //     Stop_Watch_Digits[DeciSecond_hundred].value=0;
    //     Stop_Watch_Digits[Second_unit].value++;
    // }
    // if(Stop_Watch_Digits[Second_unit].value > 9)
    // {
    //     Stop_Watch_Digits[Second_unit].value=0;
    //     Stop_Watch_Digits[Second_tens].value++;
    // }
    // if(Stop_Watch_Digits[Second_tens].value>6)
    // {
    //     Stop_Watch_Digits[Second_tens].value=0;
    //     Stop_Watch_Digits[Minutes_unit].value++;
    // }
    // if(Stop_Watch_Digits[Minutes_unit].value>9)
    // {
    //     Stop_Watch_Digits[Minutes_unit].value=0;
    //     Stop_Watch_Digits[Minutes_tens].value++;
    // }
    // if(Stop_Watch_Digits[Minutes_tens].value>6)
    // {
    //     Stop_Watch_Digits[Minutes_tens].value=0;
    //     Stop_Watch_Digits[Hours_unit].value++;
    // }
    // if(Stop_Watch_Digits[Hours_unit].value>9)
    // {
    //     Stop_Watch_Digits[Hours_unit].value=0;
    //     Stop_Watch_Digits[Hours_tens].value++;
    // }
    // if(Stop_Watch_Digits[Hours_tens].value>9)
    // {
    //     Stop_Watch_Digits[Hours_tens].value=0;
    // }

}