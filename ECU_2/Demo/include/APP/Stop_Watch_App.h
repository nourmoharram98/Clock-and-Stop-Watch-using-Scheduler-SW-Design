/**
 * @file Stop_Watch_App.h
 * @author your name (you@domain.com)
 * @brief this file will carry the apis required for stopwatch mode
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "Std_Types.h"



#define NUMBER_OF_DIGITS           7


#define DIGIT_STATE_PRINT          1
#define DIGIT_STATE_NOT_PRINT      2

#define DeciSecond_hundred         0
#define Second_unit                1
#define Second_tens                2

#define Minutes_unit               3
#define Minutes_tens               4

#define Hours_unit                 5
#define Hours_tens                 6


#define Hours_Tens_Position       3
#define Hours_Units_Position      4

#define Minuts_Tens_Position      6
#define Minuts_Units_Position     7 

#define Seconds_Tens_Position     9
#define Seconds_Units_Position    10

#define Deci_Hundred_Position     12



typedef struct 
{
          u8    value;
   const  u8    max_value;      
   const  u8    x_pos;
   const  u8    y_pos;
          u8    digit_state;  //digit_state -> ( print , not print ) 
}unit_Info_t;