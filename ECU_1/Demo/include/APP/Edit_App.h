// #pragma once

// #include "Std_Types.h"

// #define NUMBER_OF_EDITABLE_POSITIONS 10 // Total number of editable positions

// #define GENERAL_EDIT_MODE 1
// #define EDIT_DIGIT_MODE 2
// #define NORMAL_DISPLAY_MODE 3



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

// typedef struct 
// {
//     u8 row;
//     u8 column;
// } Cursor_Position_t;

// // typedef struct {
// //     u16 year;
// //     u8 month;
// //     u8 day;
// // } Date;


// typedef struct {
//     u8 year_thousands; // Thousands digit of the year
//     u8 year_hundreds;  // Hundreds digit of the year
//     u8 year_tens;      // Tens digit of the year
//     u8 year_units;     // Units digit of the year
//     u8 month_tens;     // Tens digit of the month
//     u8 month_units;    // Units digit of the month
//     u8 day_tens;       // Tens digit of the day
//     u8 day_units;      // Units digit of the day
// } Date_t;


// typedef struct 
// {
//     Cursor_Position_t position;
//     Date_t date;
//     u8 mode; // Mode can be GENERAL_EDIT_MODE or EDIT_DIGIT_MODE
    
// }Edit_App_State_t;



// void Edit_App_Init(void);
// // void Edit_App_Runnable(void);
// void Edit_App_ProcessSwitch(u8 switchNumber);
// // void Edit_App_SendCursorPos(u8 row, u8 column); // Function to send cursor position over UART

// void GeneralEditMode(void);
// void DigitEditMode(void);
