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
#include "HAL/SystickReader/SYSTICK_Reader.h"


#define NUMBER_OF_DIGITS_STOPW_MODE           7



#define Hours_tens_SW                 6
#define Hours_unit_SW                 5

#define Minutes_tens_SW               4
#define Minutes_unit_SW               3

#define Second_tens_SW                2
#define Second_unit_SW                1

#define DeciSecond_hundred_SW         0



#define Hours_Tens_Position       3
#define Hours_Units_Position      4

#define Minuts_Tens_Position      6
#define Minuts_Units_Position     7 

#define Seconds_Tens_Position     9
#define Seconds_Units_Position    10

#define Deci_Hundred_Position     12




void Stopwatch_Runnable(void);

void Reset_StopWatch(void);
