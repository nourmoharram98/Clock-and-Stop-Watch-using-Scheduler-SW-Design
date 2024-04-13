/**
 * @file HAL_LCD_CONFIG.h
 * @author Nour Moharram
 * @brief HAL_LCD_CONFIG.h file that carry 
 * @version 0.1
 * @date 2024-03-25
 *   
 */
#ifndef H_LCD_CONFIG
#define H_LCD_CONFIG

#include"Std_Types.h"

#define LCD_NUM_OF_ROWS         2   
#define LCD_NUM_OF_COLS         16

#define NUMBER_OF_PINS_4BITM    7
#define NUMBER_OF_PINS_8BITM    11

#define LCD_PINS                NUMBER_OF_PINS_8BITM


#define D0          0
#define D1          1
#define D2          2
#define D3          3
#define D4          4
#define D5          5
#define D6          6
#define D7          7

#define RS_4BIT     4
#define RW_4BIT     5
#define EN_4BIT     6

#define RS_8BIT     8
#define RW_8BIT     9
#define EN_8BIT     10


typedef enum
{
    LCD_Four_BitMode=7,
    LCD_Eight_BitMode=11,
}LCD_PIN_Mode_t;

typedef enum
{
    LCD_One,
    NumberOfLCDs,
}Number_Of_LCDs_t;
#endif