/**
 * @file Clock_Date_App.c
 * @author your name (you@domain.com)
 * @brief implementation of clock date mode and global variables, arrays and flags required for the mode
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */


typedef enum 
{
    false,
    true
}bool;


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



#include "Std_Types.h"
#include "./APP/Clock_Date_App.h"

typedef struct 
{
    u8 value;
    u8 x_pos;
    u8 y_pos;
    u8 digit_state;
}unit_Info_t;

unit_Info_t Digits[NUMBER_OF_DIGITS]=
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
    },
    [Day_unit]={
        .value=6,
        .x_pos=0,
        .y_pos=7
    },
    [Day_tens]={
        .value=1,
        .x_pos=0,
        .y_pos=6
    },
    [Month_unit]={
        .value=4,
        .x_pos=0,
        .y_pos=10
    },
    [Month_tens]={
        .value=0,
        .x_pos=0,
        .y_pos=9
    },
    [Years_thousand]={
        .value=2,
        .x_pos=0,
        .y_pos=12
    },
    [Years_hundreds]={
        .value=0,
        .x_pos=0,
        .y_pos=13
    },
    [Years_tens]={
        .value=0,
        .x_pos=0,
        .y_pos=14
    },
    [Years_unit]={
        .value=0,
        .x_pos=0,
        .y_pos=15
    },
};

struct
{
    U8 Row;
    U8 Column;
} Cursur_Position;


bool is_leap_year(int year) 
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
    {
        return true;
    }
    return false;
}

int days_in_month(int month, int year) 
{
    const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 1 && is_leap_year(year)) 
    {
        return 29;
    }
    return days[month];
}

void Clock_Date_Runnable(void)
{
    Digits[DeciSecond_hundred].value++;
    
    if (Digits[DeciSecond_hundred].value > 9)
    {
        Digits[DeciSecond_hundred].value=0;
        Digits[Second_unit].value++;
    }
    if(Digits[Second_unit].value > 9)
    {
        Digits[Second_unit].value=0;
        Digits[Second_tens].value++;
    }
    if(Digits[Second_tens].value>6)
    {
        Digits[Second_tens].value=0;
        Digits[Minutes_unit].value++;
    }
    if(Digits[Minutes_unit].value>9)
    {
        Digits[Minutes_unit].value=0;
        Digits[Minutes_tens].value++;
    }
    if(Digits[Minutes_tens].value>6)
    {
        Digits[Minutes_tens].value=0;
        Digits[Hours_unit].value++;
    }
    if(Digits[Hours_unit].value>9)
    {
        Digits[Hours_unit].value=0;
        Digits[Hours_tens].value++;
    }
    if((Digits[Hours_tens].value==2)&&(Digits[Hours_unit].value==4))
    {
        Digits[Hours_tens].value=0;
        Digits[Hours_unit].value=0;
        Digits[Day_unit].value++;
    }
    if(Digits[Day_unit].value>9)
    {
        Digits[Day_unit].value=1;
        Digits[Day_tens].value++;
    }
    if(Digits[Day_tens].value>3)
    {
        Digits[Day_tens].value=0;
        Digits[Month_unit].value++;
    }
    if(Digits[Month_unit].value>9)
    {
        Digits[Month_unit].value=1;
        Digits[Month_tens].value=1;
    }
    if((Digits[Month_tens].value==1)&&(Digits[Month_unit].value==2))
    {
        Digits[Month_unit].value=1;
        Digits[Month_tens].value=0;
        Digits[Years_unit].value++;
    }
    if(Digits[Years_unit].value>9)
    {
        Digits[Years_unit].value=0;
        Digits[Years_tens].value++;
    }
    if(Digits[Years_tens].value>9)
    {
        Digits[Years_tens].value=0;
        Digits[Years_hundreds].value++;
    }
}