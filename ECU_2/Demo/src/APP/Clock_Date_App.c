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

#define unit_time_ms 100


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

struct  
{
    U8 Day_Value[2];
    U8 Month_Value[2];
    U16 Year_Value[4];

    U8 Hours_Value[2];
    U8 Minuts_Value[2];
    U8 Seconds_Value[2];
    U8 Deci_Value;

}Clock_Date_Data;

struct
{
    U8 Row;
    U8 Column;
} Cursur_Position;

enum
{
    Day,
    Month,
    Year,

    Hours,
    Minuts,
    Seconds,
    Deci
};


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
    Clock_Date_Data.Deci_Value++;
    
    if (Clock_Date_Data.Deci_Value > 9)
    {
        Clock_Date_Data.Deci_Value = 0;
        Clock_Date_Data.Seconds_Value++;
    }

    if (Clock_Date_Data.Seconds_Value > 59)
    {
        Clock_Date_Data.Seconds_Value = 0;
        Clock_Date_Data.Minuts_Value++;
    }
    
    if (Clock_Date_Data.Minuts_Value > 59)
    {
        Clock_Date_Data.Minuts_Value = 0;
        Clock_Date_Data.Hours_Value++;
    }
    
    if (Clock_Date_Data.Hours_Value > 23)
    {
        Clock_Date_Data.Deci_Value    = 0;
        Clock_Date_Data.Seconds_Value = 0;
        Clock_Date_Data.Minuts_Value  = 0;
        Clock_Date_Data.Hours_Value   = 0;

        Clock_Date_Data.Day_Value++;
    }    

    int max_days = days_in_month(Clock_Date_Data.Month_Value, Clock_Date_Data.Year_Value);

    if (Clock_Date_Data.Day_Value > max_days)
    {
        Clock_Date_Data.Day_Value = 1;
        Clock_Date_Data.Month_Value++;
    }

    if (Clock_Date_Data.Month_Value > 12)
    {
        Clock_Date_Data.Month_Value = 1;
        Clock_Date_Data.Year_Value++;
    }
}