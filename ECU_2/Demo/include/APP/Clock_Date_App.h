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
#include "HAL/SystickReader/SYSTICK_Reader.h"
#define NUMBER_OF_DIGITS_CLK_MODE  15


// #define DIGIT_STATE_OPERATING      1
// #define DIGIT_STATE_EDITTING       2

// #define DIGIT_STATE_PRINT          1
// #define DIGIT_STATE_NOT_PRINT      2

#define Day_tens                   0
#define Day_unit                   1

#define Month_tens                 2
#define Month_unit                 3

#define Years_thousand             4
#define Years_hundreds             5
#define Years_tens                 6
#define Years_unit                 7

#define Hours_tens                 8
#define Hours_unit                 9

#define Minutes_tens               10
#define Minutes_unit               11

#define Second_tens                12
#define Second_unit                13

#define DeciSecond_hundred         14



// #define Day_Tens_Position         7
// #define Day_Units_Position        8

// #define Month_Tens_Postion        10
// #define Month_Units_Position      11

// #define Year_Thousand_Position    13
// #define Year_Hundred_Position     14
// #define Year_Tens_Postion         15
// #define Year_Units_Position       16

// #define Hours_Tens_Position       3
// #define Hours_Units_Position      4

// #define Minuts_Tens_Position      6
// #define Minuts_Units_Position     7 

// #define Seconds_Tens_Position     9
// #define Seconds_Units_Position    10

// #define Deci_Hundred_Position     12





void Clock_Date_Runnable(void);