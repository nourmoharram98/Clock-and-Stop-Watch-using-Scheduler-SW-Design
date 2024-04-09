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

#include "./APP/Stop_Watch_App.h"

unit_Info_t Stop_Watch_Digits[NUMBER_OF_DIGITS]=
{
    [DeciSecond_hundred]={
        .value=0,
        .x_pos=1,  // x hia al row
        .y_pos=12   // y hia al col
    },
    [Second_unit]={
        .value=0,
        .x_pos=1,
        .y_pos=10
    },
    [Second_tens]={
        .value=0,
        .x_pos=0,
        .y_pos=9
    },
    [Minutes_unit]={
        .value=0,
        .x_pos=1,
        .y_pos=7
    },
    [Minutes_tens]={
        .value=0,
        .x_pos=1,
        .y_pos=6
    },
    [Hours_unit]={
        .value=0,
        .x_pos=1,
        .y_pos=4
    },
    [Hours_tens]={
        .value=0,
        .x_pos=1,
        .y_pos=3
    }
};



void Stopwatch_Runnable(void)
{
        Stop_Watch_Digits[DeciSecond_hundred].value++;
    
    if (Stop_Watch_Digits[DeciSecond_hundred].value > 9)
    {
        Stop_Watch_Digits[DeciSecond_hundred].value=0;
        Stop_Watch_Digits[Second_unit].value++;
    }
    if(Stop_Watch_Digits[Second_unit].value > 9)
    {
        Stop_Watch_Digits[Second_unit].value=0;
        Stop_Watch_Digits[Second_tens].value++;
    }
    if(Stop_Watch_Digits[Second_tens].value>6)
    {
        Stop_Watch_Digits[Second_tens].value=0;
        Stop_Watch_Digits[Minutes_unit].value++;
    }
    if(Stop_Watch_Digits[Minutes_unit].value>9)
    {
        Stop_Watch_Digits[Minutes_unit].value=0;
        Stop_Watch_Digits[Minutes_tens].value++;
    }
    if(Stop_Watch_Digits[Minutes_tens].value>6)
    {
        Stop_Watch_Digits[Minutes_tens].value=0;
        Stop_Watch_Digits[Hours_unit].value++;
    }
    if(Stop_Watch_Digits[Hours_unit].value>9)
    {
        Stop_Watch_Digits[Hours_unit].value=0;
        Stop_Watch_Digits[Hours_tens].value++;
    }
    if(Stop_Watch_Digits[Hours_tens].value>9)
    {
        Stop_Watch_Digits[Hours_tens].value=0;
    }

}