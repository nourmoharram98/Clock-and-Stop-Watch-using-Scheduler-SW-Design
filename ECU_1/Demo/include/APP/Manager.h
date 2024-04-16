#pragma once

#include"Std_Types.h"
#include "APP/App_Config.h"
#include "APP/Clock_Date_App.h"
#include "APP/Stop_Watch_App.h"
#include "SERVICE/COMM/UART_COMM.h"
#include "HAL/SWITCH/HAL_SWITCH.h"
#include "HAL/SystickReader/SYSTICK_Reader.h"
/**
 * @brief array that carry Clock-date digits Data
 * 
 */
extern unit_Info_t Clock_Date_Digits[NUMBER_OF_DIGITS_CLK_MODE];
/**
 * @brief array that carry Stop watch digits Data
 * 
 */
extern unit_Info_t Stop_Watch_Digits[NUMBER_OF_DIGITS_STOPW_MODE];

/*--------------------------------Types Defs-------------------------*/
    /**
     * @brief enumeration for the operation states in the system
     * 
     */
    typedef enum
    {
        Init_Operation,

        Idle_Operation,

        GeneralEdit_Operation,

        DigitEdit_Operation,
    }Operation_Types_t;

    /**
     * @brief Struct for the Switches Data and status
     * 
     */
    typedef struct 
    {
        U8 DATA;
        u32 Switch_Status;
        u32 Switch_PrevStatus;
    }Ctrl_Switches_Data_t;

    typedef struct
    {
        uint8 x_pos;
        uint8 y_pos;
        sint8 value;
    }EDIT_CURSOR_t;
/*-------------------------------------------------------------------*/

/*------------------------------Functions-----------------------------*/
    void Manager_Runnable(void);
    void TX_Communication_Manager(U8 RAW_DATA);
    void GeneralEditMode(void);
    void DigitEditMode(void);
    void Mode_Switch_Pressed(void);
    void Command_Handler(u8 command);
    void OK_Switch_Pressed(void);
    void UP_Switch_Pressed(void);
    void Down_Switch_Pressed(void);
    void Edit_Switch_Pressed(void);
    void Right_Switch_Pressed(void);
    void Left_Switch_Pressed(void);

/*--------------------------------------------------------------------*/
