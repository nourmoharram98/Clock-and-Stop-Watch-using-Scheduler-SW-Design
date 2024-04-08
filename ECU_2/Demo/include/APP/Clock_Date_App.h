/**
 * @file Clock_Date_App.h
 * @author your name (you@domain.com)
 * @brief this file will carry the apis for clock_date mode
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "Std_Types.h"

#define NUMBER_OF_DIGITS           15


#define DIGIT_STATE_OPERATING      1
#define DIGIT_STATE_EDITTING       2

#define DeciSecond_hundred         0
#define Second_unit                1
#define Second_tens                2

#define Minutes_unit               3
#define Minutes_tens               4

#define Hours_unit                 5
#define Hours_tens                 6

#define Day_unit                   7
#define Day_tens                   8

#define Month_unit                 9
#define Month_tens                 10

#define Years_thousand             11
#define Years_hundreds             12
#define Years_tens                 13
#define Years_unit                 14


#define Day_Tens_Position         7
#define Day_Units_Position        8

#define Month_Tens_Postion        10
#define Month_Units_Position      11

#define Year_Thousand_Position    13
#define Year_Hundred_Position     14
#define Year_Tens_Postion         15
#define Year_Units_Position       16

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
          u8    digit_state;
}unit_Info_t;