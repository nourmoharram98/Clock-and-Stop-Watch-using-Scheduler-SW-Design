#ifndef LCD_H_
#define LCD_H_
	#include "HAL/LCD/LCD_Config.h"

    /*------------------------------Enum for error of LCD Driver----------------------------*/
        typedef enum{
            LCD_enumNOK,
            LCD_NullPointer,
            LCD_enumWrong_PortNo,
            LCD_enumWrong_PinNo,
            LCD_enumWrong_Value,
            LCD_enumOutof_Range,
            LCD_enumNotReady,
            LCD_enumOK
        }LCD_enuErrorStatus_t;
    /*--------------------------------------------------------------------------------------*/

    /*----------------------------------Enum for LCD States----------------------------------*/
        typedef enum
        {
            readyForNewRequest,
            REQ_Pending
        }State_t;
    /*---------------------------------------------------------------------------------------*/

    /*-----------------------------------Enum for LCD Stages-----------------------------------*/
        typedef enum
        {
            Init_Stage,
            Operation_Stage
        }LCD_STAGE_t;
    /*-----------------------------------------------------------------------------------------*/

    /*------------------------------------Enum for InitState-----------------------------------*/
        typedef enum
        {
            Power_On,
            Function_Set,
            Display,
            clear,
            Entry,
            End
        }InitStage_t;
    /*-----------------------------------------------------------------------------------------*/

    /*-----------------------------------Enum for Command Types-----------------------------*/
        typedef enum
        {
            None,
            Write_String,
            Write_Number,
            Clear,
            SetPosition,
            ShiftDisplay
        }Operation_Type_t;
    /*---------------------------------------------------------------------------------------*/

    /*---------------------------------------Struct for User REQ------------------------------*/
        struct 
        {
            const char *String;
            U8 Number;
            U8 Length;
            State_t State;  
            LCD_STAGE_t LCD_STAGE;
            InitStage_t InitStage;                   
            Operation_Type_t Operation_Type;        
        }UserREQ;
    /*----------------------------------------------------------------------------------------*/

    /*-----------------------------------------Commands--------------------------------------*/
        #define LCD_Home                      0x02          // return cursor to first position on first line 
        #define LCD_CGRAM                     0x40          // the first Place/Address at CGRAM
        #define LCD_SetCursor                 0x80          // set cursor position
        #define LCD_FunctionReset             0x30          // reset the LCD
    /*---------------------------------------------------------------------------------------*/
    
/*APIs*/
    /*--------------------------------LCD Initilization Function----------------------------*/
        void LCD_InitAsync (void);    
    /*--------------------------------------------------------------------------------------*/

    /*---------------------------------LCD Get Status Function------------------------------*/
        LCD_enuErrorStatus_t LCD_getStatus (U8 *LCD_Status);
    /*--------------------------------------------------------------------------------------*/

    /*-------------------------------LCD Clear Screen Function------------------------------*/
        LCD_enuErrorStatus_t LCD_ClearScreenAsync();
    /*--------------------------------------------------------------------------------------*/

    /*-----------------------------LCD GO to DDRAM Address Function-------------------------*/
         LCD_enuErrorStatus_t LCD_SetCursorPosAsync(U8 Copy_u8X, U8 Copy_u8Y);
    /*--------------------------------------------------------------------------------------*/

    /*------------------------------------Write String Function-----------------------------*/
        LCD_enuErrorStatus_t LCD_enuWriteStringAsync(char* Copy_pchPattern , U8 Length);
    /*--------------------------------------------------------------------------------------*/

    /*-----------------------------------Write Number Function------------------------------*/
		 LCD_enuErrorStatus_t LCD_enuWriteNumber(U8 Copy_u8Number);
    /*--------------------------------------------------------------------------------------*/

    /*-----------------------------Get Operations State Function----------------------------*/
        LCD_enuErrorStatus_t Get_OperationState (U8 *Op_State);
    /*--------------------------------------------------------------------------------------*/
/*****/

// /*Extra*/	
// 	/*-------------------------------ShiftDisplay+Cursor-------------------------------------*/
// 	    LCD_enuErrorStatus_t LCD_ShiftDisplay(ShiftDir_t ShiftDir);
// 	/*---------------------------------------------------------------------------------------*/	

//     /*-----------------------------------Write Number Function------------------------------*/
// 		 LCD_enuErrorStatus_t LCD_enuWriteNumber(U8 Copy_u8Number);
//     /*--------------------------------------------------------------------------------------*/    
// /******/    
// /*------*/



#endif