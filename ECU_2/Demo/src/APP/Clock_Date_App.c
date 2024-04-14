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

#include "./APP/Clock_Date_App.h"

/**
 * @brief LEAP YEAR AND MONTHS DAYS HANDLING
*/



unit_Info_t Clock_Date_Digits[NUMBER_OF_DIGITS]=
{
    [DeciSecond_hundred]=
    {
        .value=1,
        .x_pos=2,  // x hia al row
        .y_pos=12,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Second_unit]={
        .value=3,
        .x_pos=2,
        .y_pos=10,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Second_tens]={
        .value=5,
        .x_pos=2,
        .y_pos=9,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Minutes_unit]={
        .value=9,
        .x_pos=2,
        .y_pos=7,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Minutes_tens]={
        .value=5,
        .x_pos=2,
        .y_pos=6,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Hours_unit]={
        .value=3,
        .x_pos=2,
        .y_pos=4,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Hours_tens]={
        .value=2,
        .x_pos=2,
        .y_pos=3,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Day_unit]={
        .value=9,
        .x_pos=1,
        .y_pos=8,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Day_tens]={
        .value=2,
        .x_pos=1,
        .y_pos=7,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Month_unit]={
        .value=2,
        .x_pos=1,
        .y_pos=11,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Month_tens]={
        .value=0,
        .x_pos=1,
        .y_pos=10,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Years_thousand]={
        .value=2,
        .x_pos=1,
        .y_pos=13,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Years_hundreds]={
        .value=0,
        .x_pos=1,
        .y_pos=14,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Years_tens]={
        .value=0,
        .x_pos=1,
        .y_pos=15,
        .digit_state=DIGIT_STATE_PRINT
    },
    [Years_unit]={
        .value=0,
        .x_pos=1,
        .y_pos=16,
        .digit_state=DIGIT_STATE_PRINT
    }
};

static bool is_leap_year(int year) 
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) 
    {
        return true;
    }
    return false;
}

static int days_in_month(int month, int year) 
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

// Update Deci, Sec , Min , Hours and Days, months, Years

Clock_Date_Digits[DeciSecond_hundred].value++;
Clock_Date_Digits[DeciSecond_hundred].digit_state=DIGIT_STATE_PRINT;


    if (Clock_Date_Digits[DeciSecond_hundred].value > 9) 
    {
        Clock_Date_Digits[DeciSecond_hundred].value = 0;
        Clock_Date_Digits[DeciSecond_hundred].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Second_unit].value++;
        Clock_Date_Digits[Second_unit].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Second_unit].value > 9) 
    {
        Clock_Date_Digits[Second_unit].value = 0;
        Clock_Date_Digits[Second_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Second_tens].value++;
        Clock_Date_Digits[Second_tens].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Second_tens].value > 5) 
    {
        Clock_Date_Digits[Second_tens].value = 0;
        Clock_Date_Digits[Second_tens].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Minutes_unit].value++;
        Clock_Date_Digits[Minutes_unit].digit_state=DIGIT_STATE_PRINT;

    }

    if (Clock_Date_Digits[Minutes_unit].value > 9) 
    {
        Clock_Date_Digits[Minutes_unit].value = 0;
        Clock_Date_Digits[Minutes_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Minutes_tens].value++;
        Clock_Date_Digits[Minutes_tens].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Minutes_tens].value > 5) 
    {
        Clock_Date_Digits[Minutes_tens].value = 0;
        Clock_Date_Digits[Minutes_tens].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Hours_unit].value++;
        Clock_Date_Digits[Hours_unit].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Hours_unit].value > 9) 
    {
        Clock_Date_Digits[Hours_unit].value = 0;
        Clock_Date_Digits[Hours_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Hours_tens].value++;
        Clock_Date_Digits[Hours_tens].digit_state=DIGIT_STATE_PRINT;
    }

    if ((Clock_Date_Digits[Hours_tens].value >= 2) && (Clock_Date_Digits[Hours_unit].value > 3)) 
    {
        // Reset hours to 0 if it exceeds 23
        Clock_Date_Digits[Hours_tens].value = 0;
        Clock_Date_Digits[Hours_tens].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Hours_unit].value = 0;
        Clock_Date_Digits[Hours_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Day_unit].value++;
        Clock_Date_Digits[Day_unit].digit_state=DIGIT_STATE_PRINT;
    }


    // Update day, month, year

    int month = Clock_Date_Digits[Month_tens].value * 10 + Clock_Date_Digits[Month_unit].value;

    int year = Clock_Date_Digits[Years_thousand].value * 1000 + Clock_Date_Digits[Years_hundreds].value * 100 + Clock_Date_Digits[Years_tens].value * 10 + Clock_Date_Digits[Years_unit].value;

    int days_in_current_month = days_in_month(month - 1, year);

    if ( Clock_Date_Digits[Day_unit].value > 9 )
    {
        Clock_Date_Digits[Day_unit].value=0;
        Clock_Date_Digits[Day_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Day_tens].value++;
        Clock_Date_Digits[Day_tens].digit_state=DIGIT_STATE_PRINT;

    } 

    if ( Clock_Date_Digits[Day_tens].value * 10 + Clock_Date_Digits[Day_unit].value > days_in_current_month )
    {
        Clock_Date_Digits[Day_tens].value=0;
        Clock_Date_Digits[Day_tens].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Day_unit].value=1;
        Clock_Date_Digits[Day_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Month_unit].value++;
        Clock_Date_Digits[Month_unit].digit_state=DIGIT_STATE_PRINT;
    }

    if(Clock_Date_Digits[Month_unit].value>9)
    {
        Clock_Date_Digits[Month_unit].value=0;
        Clock_Date_Digits[Month_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Month_tens].value=1;
        Clock_Date_Digits[Month_tens].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value > 2) 
    { 
        Clock_Date_Digits[Month_tens].value = 0;
        Clock_Date_Digits[Month_tens].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Month_unit].value = 1; //reset to month one
        Clock_Date_Digits[Month_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Years_unit].value++;
        Clock_Date_Digits[Years_unit].digit_state=DIGIT_STATE_PRINT;
    }    

    if (Clock_Date_Digits[Years_unit].value > 9) 
    {
        Clock_Date_Digits[Years_unit].value = 0;
        Clock_Date_Digits[Years_unit].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Years_tens].value++;
        Clock_Date_Digits[Years_tens].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Years_tens].value > 9) 
    {
        Clock_Date_Digits[Years_tens].value = 0;
        Clock_Date_Digits[Years_tens].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Years_hundreds].value++;
        Clock_Date_Digits[Years_hundreds].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Years_hundreds].value > 9) 
    {
        Clock_Date_Digits[Years_hundreds].value = 0;
        Clock_Date_Digits[Years_hundreds].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Years_thousand].value++;
        Clock_Date_Digits[Years_thousand].digit_state=DIGIT_STATE_PRINT;
    }

    if (Clock_Date_Digits[Years_thousand].value > 9) 
    {
        Clock_Date_Digits[Years_thousand].value = 0;
        Clock_Date_Digits[Years_thousand].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Years_hundreds].value = 0;
        Clock_Date_Digits[Years_hundreds].digit_state=DIGIT_STATE_PRINT;


        Clock_Date_Digits[Years_tens].value     = 0;
        Clock_Date_Digits[Years_tens].digit_state=DIGIT_STATE_PRINT;

        Clock_Date_Digits[Years_unit].value     = 0;
        Clock_Date_Digits[Years_unit].digit_state     = DIGIT_STATE_PRINT;
    }
}