/**
 * @file HAL_LED.h
 * @author Nour Moharram
 * @brief HAL LCD.h file that carry the declarations of the interfaces of LCD in addition to 
 * important macros
 * @version 0.1
 * @date 2024-03-25
 *   
 */
#ifndef H_LCD
#define H_LCD

#include"Std_Types.h"
#include"Error_states.h"
#include"HAL/LCD/HAL_LCD_CONFIG.h"
#include"HAL/SystickReader/SYSTICK_Reader.h"
#define LCD_DISPLAY_ROW1        0
#define LCD_DISPLAY_ROW2        1
#define LCD_DISPLAY_ROW3        2
#define LCD_DISPLAY_ROW4        3


#define LCD_DISPLAY_COL_START   0
#define LCD_DISPLAY_COL_END     15

/**
 * @brief Macros for enabling and disabling the cursor and blink
 * 
 */
#define LCD_CURSOR_BLINK_ON     0x0f
#define LCD_CURSOR_BLINK_OFF    0x0c

typedef void (*LCD_CBFUNC_t)(void);

typedef enum
{
    LCD_Off,
    LCD_Init,
    LCD_Operation,
    LCD_OverFlow,
}LCD_State_t;

typedef struct 
{
    u32 Pin;
    void* Port;    
}HAL_LCD_PinConfig_t;

typedef struct 
{
    LCD_PIN_Mode_t LCD_PIN_NUM_MODE;
} HAL_LCD_Config_t;

typedef struct 
{
    HAL_LCD_Config_t config;
    HAL_LCD_PinConfig_t pins[LCD_PINS];
} HAL_LCD_Configuration_t;
/**
 * @brief functions APIs
 * f
 */
Sys_enuErrorStates_t LCD_InitAsync(void);
Sys_enuErrorStates_t LCD_ClearScreenAsync(void);
Sys_enuErrorStates_t LCD_SetCursorPosAsync(u8 Copy_LCDPosx,u8 Copy_LCDPosy);
Sys_enuErrorStates_t LCD_WriteStringAsync(const char *ptrToString,u8 String_size);
Sys_enuErrorStates_t LCD_WriteCommandAsync(const char command);
Sys_enuErrorStates_t LCD_GetStatus(u32* PtrToLCDstatus);

Sys_enuErrorStates_t LCD_WriteSetCB(LCD_CBFUNC_t PtrTofunc);
Sys_enuErrorStates_t LCD_ClearSetCB(LCD_CBFUNC_t PtrTofunc);
Sys_enuErrorStates_t LCD_SetCursorSetCB(LCD_CBFUNC_t PtrTofunc);
Sys_enuErrorStates_t LCD_enuWriteNumber(u32 number);



#endif