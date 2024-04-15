#include "Std_Types.h"

#include "MCAL/GPIO/GPIO.h"

#include "HAL/LCD/LCD.h"

/*Glossary
*    IMH = Initialization Manager Helper 
*/

/*--------------------------------------Enum for G_WriteState------------------------------*/
    typedef enum
    {
        WriteInit_State,
        Writing_State,
        Writing_Done
    }G_WriteState_t;
/*-----------------------------------------------------------------------------------------*/

/*--------------------------------------Enum for G_WriteState------------------------------*/
    typedef enum
    {
        NumberWriting_State,
        NumberWriting_Done
    }G_WriteNumberState_t;
/*-----------------------------------------------------------------------------------------*/

/*-------------------------------------Enum for Enable Pin state --------------------------*/
    typedef enum
    {
        LCD_EnablePin_OFF,
        LCD_EnablePin_ON
    }G_LCD_EnablePinState_t;
/*-----------------------------------------------------------------------------------------*/

/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Concated Commands>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    int ClearDisplay        = CONCAT( 0, 0, 0, 0, 0, 0, 0, 1);
    int ReturnHome          = CONCAT( 0, 0, 0, 0, 0, 0, 1, 0);
    int EntryModeSet        = CONCAT( 0, 0, 0, 0, 0, 1, Curser_MovDir, DisplayShiftDirection);
    int DisplayControl      = CONCAT( 0, 0, 0, 0, 1, Display_State, Curser_DisplayState, Curser_BlinkingState);
    int CursorDisplayShiftL = CONCAT( 0, 0, 0, 1, 1, 1, 0, 0);
	int CursorDisplayShiftR = CONCAT( 0, 0, 0, 1, 1, 0, 0, 0);
    int FunctionSet         = CONCAT( 0, 0, 1, Interface_DataLength, Lines_Number, Font_Size, 0, 0);
/*---------------------------------------------------------------------------------------*/

/*-------------------------------------Global Variables-----------------------------------*/
    G_LCD_EnablePinState_t G_LCD_EnablePinState = LCD_EnablePin_OFF ;
    G_WriteState_t G_WriteState;
    G_WriteNumberState_t   G_WriteNumberState;

    U8 G_CurserPositionRstring;        /* Curser Position Related to String being written */
    U8 G_LCD_Curser_Data;

/*----------------------------------------------------------------------------------------*/

/*------------------------------------static functions------------------------------------*/

    static LCD_enuErrorStatus_t LCD_enuWriteCommand  (U8 Copy_u8Command);

    static LCD_enuErrorStatus_t LCD_enuWriteData     (U8 Copy_u8Data);

        static void LCD_Init_Manager                 (void);

            static GPIO_Error_t LCD_IMH_PowerOn(void);

        static void LCD_WriteString_Proc             (void);
        
        static void LCD_WriteNumber_Proc             (void);

        static void LCD_Clear_Proc                   (void); 

        static void LCD_SetPostion_Proc              (void);

        static void LCD_CtrlEnablePin                (U8 Copy_LCDEnablePinState);     

/*--------------------------------------------------------------------------------------*/

/*--------------------------------Functions to be used by user--------------------------*/

    /*--------------------------------LCD Initilization Function----------------------------*/
        void LCD_InitAsync (void) 
        {
            /*UserReQ state*/
                UserREQ.State = REQ_Pending;
            /*-------------*/
            
            /*UserReQ Stage*/
                UserREQ.LCD_STAGE = Init_Stage;
            /*------------*/

            /*UserReQ 1st state*/
                UserREQ.InitStage = Power_On;
            /*----------------*/
        }
    /*--------------------------------------------------------------------------------------*/

    /*------------------------------------Write String Function-----------------------------*/
        LCD_enuErrorStatus_t LCD_enuWriteStringAsync(char* Copy_pchPattern , U8 Length)
        {
            LCD_enuErrorStatus_t Local_Error = LCD_enumNOK;

            /*Error Handling*/
                if (Copy_pchPattern == NULL)
                {
                    Local_Error = LCD_NullPointer;
                }
            /*------------*/

            /*Actual Implementation*/
                else if ( (UserREQ.LCD_STAGE == Operation_Stage) && (UserREQ.State == readyForNewRequest) ) 
                {
                    /*Local Error flag*/
                        Local_Error = LCD_enumOK;  
                    /*----------------*/

                    /*UserReQ state*/                
                        UserREQ.State=REQ_Pending;
                    /*------------*/

                    /*UserReQ Operation*/
                        UserREQ.Operation_Type = Write_String;
                    /*----------------*/
                
                    /*Setting data to be printed*/
                        UserREQ.String = Copy_pchPattern;
                        UserREQ.Length = Length;
                    /*------------------------*/
                }
                else
                {
                    Local_Error = LCD_enumNotReady;  
                }
            /*--------------------*/

            return Local_Error;        
        }
    /*--------------------------------------------------------------------------------------*/

    /*-----------------------------------Write Number Function------------------------------*/
        LCD_enuErrorStatus_t LCD_enuWriteNumber(U8 Copy_u8Number)
        {
            LCD_enuErrorStatus_t Local_Error = LCD_enumNOK;

            /*Actual Implementation*/
                if ( (UserREQ.LCD_STAGE == Operation_Stage) && (UserREQ.State == readyForNewRequest) ) 
                {
                    /*Local Error flag*/
                        Local_Error = LCD_enumOK;  
                    /*----------------*/

                    /*UserReQ state*/                
                        UserREQ.State=REQ_Pending;
                    /*------------*/

                    /*UserReQ Operation*/
                        UserREQ.Operation_Type = Write_Number;
                    /*----------------*/
                
                    /*Setting data to be printed*/
                        UserREQ.Number = Copy_u8Number;
                    /*------------------------*/
                }
                else
                {
                    Local_Error = LCD_enumNotReady;  
                }
            /*--------------------*/

            return Local_Error;   
        }
    /*--------------------------------------------------------------------------------------*/

    /*-------------------------------LCD Clear Screen Function------------------------------*/
        LCD_enuErrorStatus_t LCD_ClearScreenAsync(void)
        {
            /*Error Handling*/
                LCD_enuErrorStatus_t Local_Error = LCD_enumNOK;
            /*-------------*/

            /*Actual Implementation*/
                if ( (UserREQ.LCD_STAGE == Operation_Stage) && (UserREQ.State == readyForNewRequest) ) 
                {
                    /*UserReQ state*/
                        UserREQ.State = REQ_Pending;
                    /*-------------*/

                    /*UserReQ Operation*/
                        UserREQ.Operation_Type = Clear;
                    /*----------------*/

                    /*Error Handling*/
                        Local_Error = LCD_enumOK;           
                    /*-------------*/
                }
                else
                {
                    Local_Error = LCD_enumNotReady;  
                }
            /*---------------------*/

            return Local_Error;
        }
    /*--------------------------------------------------------------------------------------*/

    /*-----------------------------LCD GO to DDRAM Address Function-------------------------*/
        LCD_enuErrorStatus_t LCD_SetCursorPosAsync(U8 Copy_u8X, U8 Copy_u8Y)
        {
            LCD_enuErrorStatus_t LCD_enuErrorStatus = LCD_enumOK;

            U8 LOC_u8data = 0;

            /*Error Handling*/
                if(Copy_u8X >2 || Copy_u8X<1)
                {
                    LCD_enuErrorStatus = LCD_enumOutof_Range;
                }

                else if (Copy_u8Y>16 || Copy_u8Y<1)
                {
                    LCD_enuErrorStatus = LCD_enumOutof_Range;
                }
            /*--------------*/

            /*Actual Implementation*/

                else
                {

                /* Setting UserReQ */

                    /*UserReQ state*/                
                        UserREQ.State=REQ_Pending;
                    /*------------*/

                    /*UserReQ Operation*/
                        UserREQ.Operation_Type = SetPosition;
                    /*----------------*/

                /*---------------*/

                /* Getting Curser Data */
                    if (Copy_u8X == 1)
                    {
                        LOC_u8data = ( (LCD_SetCursor) + (Copy_u8Y-1) );
                    }

                    else if (Copy_u8X == 2)
                    {
                        LOC_u8data = ( (LCD_SetCursor) + 64 + (Copy_u8Y-1) );
                    }

                    G_LCD_Curser_Data = LOC_u8data ;
                /*---------------------*/

                }

                return LCD_enuErrorStatus;

            /*---------------------*/    
        }
    /*--------------------------------------------------------------------------------------*/

    /*---------------------------------LCD Get Status Function------------------------------*/
        LCD_enuErrorStatus_t LCD_getStatus (U8 *LCD_Status)
        {
            LCD_enuErrorStatus_t LCD_enuErrorStatus;

            if (LCD_Status == NULL)
            {
                LCD_enuErrorStatus = LCD_NullPointer;
            }    

            *LCD_Status = UserREQ.State;

            return LCD_enuErrorStatus;
        }
    /*--------------------------------------------------------------------------------------*/

    /*-----------------------------Get Operations State Function----------------------------*/
        LCD_enuErrorStatus_t Get_OperationState (U8 *Op_State)
        {
            LCD_enuErrorStatus_t LCD_enuErrorStatus;

            if (Op_State == NULL)
            {
                LCD_enuErrorStatus = LCD_NullPointer;
            }        

            *Op_State = UserREQ.Operation_Type;

            return LCD_enuErrorStatus;
        }
    /*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/

/*-------------------------------LCD_RUNNABLE MANAGER--------------------------*/
    void LCD_Runnable_Manager(void)
    {
        if (UserREQ.LCD_STAGE == Init_Stage && UserREQ.State == REQ_Pending)
        {
            LCD_Init_Manager();
        }
        else if (UserREQ.LCD_STAGE == Operation_Stage)
        {
            if(UserREQ.State == REQ_Pending)
            {
                switch (UserREQ.Operation_Type)
                {
                    case Write_String:
                        LCD_WriteString_Proc();                  
                    break;

                    case Clear:
                        LCD_Clear_Proc();                  
                    break;

                    case SetPosition:
                        LCD_SetPostion_Proc();             
                    break;    

                    case Write_Number:
                        LCD_WriteNumber_Proc();
                    break;

                    default:
                                    
                    break;

                }
            }
        }
    }
/*-----------------------------------------------------------------------------*/  

/*-------------------------------------Static functions---------------------------------*/
      
    /*-----------------------------------LCD Init Manager--------------------------*/
        static void LCD_Init_Manager (void)
        {

            switch(UserREQ.InitStage)
            {
                case Power_On:
                    LCD_IMH_PowerOn();                    
                break;

                case Function_Set: 
                    LCD_enuWriteCommand(FunctionSet);
                    if ( G_LCD_EnablePinState == LCD_EnablePin_OFF)
                    {
                        UserREQ.InitStage = Display;           
                    }
                break;

                case Display:
                    LCD_enuWriteCommand(DisplayControl);    
                    if ( G_LCD_EnablePinState == LCD_EnablePin_OFF)
                    {
                        UserREQ.InitStage = clear;            
                    }   
                break;

                case clear:
                    LCD_enuWriteCommand( ClearDisplay );
                    if ( G_LCD_EnablePinState == LCD_EnablePin_OFF)
                    {
                        UserREQ.InitStage = Entry;         
                    }   
                break;

                case Entry:
                    LCD_enuWriteCommand(EntryModeSet);
                    if ( G_LCD_EnablePinState == LCD_EnablePin_OFF)
                    {
                        UserREQ.InitStage = End;        
                    }          
                break;

                case End:
                    UserREQ.LCD_STAGE = Operation_Stage;
                    UserREQ.State = readyForNewRequest;
                break;
            }

        }
    /*-----------------------------------------------------------------------------*/  

    /*-----------------------------------LCD PowerOn-------------------------------*/
        static GPIO_Error_t LCD_IMH_PowerOn(void)
        {
            GPIO_Error_t LOC_Status = GPIO_NOK;

            UserREQ.State = REQ_Pending;

            extern const LCD_strLCDPinConfig_t LCD_strLCDpinConfig[LCD_PINs];
            GPIO_Pin_t   LCD;
            
            LCD.Pin_Mode  = GPIO_MODE_OP_PP;
            LCD.Pin_Speed = GPIO_SPEED_HIGH;

            U8 Index = 0;

            for (Index = 0 ; Index < LCD_PINs ; Index ++)
            {
                LCD.Port    = LCD_strLCDpinConfig[Index].LCD_PortNumber;
                LCD.Pin_num = LCD_strLCDpinConfig[Index].LCD_PinNumber;
                LOC_Status  = GPIO_Init(&LCD);
            }

            UserREQ.InitStage = Function_Set;

            return LOC_Status;
        }
    /*-----------------------------------------------------------------------------*/

    /*-----------------------------------Write Process-----------------------------*/
        static void LCD_WriteString_Proc(void)
        {
            switch (G_WriteState)
            {
                case WriteInit_State: 
                    G_CurserPositionRstring = 0;
                    G_WriteState = Writing_State;
                break;

                case Writing_State:
                    if ( (G_CurserPositionRstring) != (UserREQ.Length) )
                    {
                        LCD_enuWriteData( UserREQ.String[ G_CurserPositionRstring ] );

                        if( G_LCD_EnablePinState == LCD_EnablePin_OFF )
                        {
                            G_CurserPositionRstring++;
                        }
                    }
                    else
                    {
                        G_WriteState = Writing_Done;
                    }
                break;

                case Writing_Done:
                    UserREQ.State = readyForNewRequest;
                    UserREQ.Operation_Type  = None;
                    G_WriteState = WriteInit_State;
                break;

            }
        }
    /*-----------------------------------------------------------------------------*/

    /*--------------------------------Write Number Process-------------------------*/
        static void LCD_WriteNumber_Proc(void) // 2ms
        {
            // U8 LOC_u8Reversed = 1 ;

            switch (G_WriteNumberState)
            {
                case NumberWriting_State:
                    LCD_enuWriteData( UserREQ.Number+48 );//4

                    if( G_LCD_EnablePinState == LCD_EnablePin_OFF )
                    {
                        G_WriteNumberState = NumberWriting_Done;
                    }
                    else 
                    {
                        /*Wait till write command finish*/
                    }

                break;

                case NumberWriting_Done:
                    UserREQ.State = readyForNewRequest;
                    UserREQ.Operation_Type  = None;
                    G_WriteNumberState = NumberWriting_State;
                break;

            }  
        }
    /*-----------------------------------------------------------------------------*/

    /*----------------------------------Clear Process------------------------------*/
        static void LCD_Clear_Proc(void)
        {
            LCD_enuWriteCommand( ClearDisplay );

            if ((G_LCD_EnablePinState) == (LCD_EnablePin_OFF))
            {
                UserREQ.State = readyForNewRequest;
                UserREQ.Operation_Type  = None;
            }
        }
    /*-----------------------------------------------------------------------------*/

    /*---------------------------------SetPostion Process--------------------------*/
        static void LCD_SetPostion_Proc(void)
        {
            LCD_enuWriteCommand( G_LCD_Curser_Data );

            if (G_LCD_EnablePinState == LCD_EnablePin_OFF)
            {
                UserREQ.State = readyForNewRequest;
                UserREQ.Operation_Type  = None;
            }
        }
    /*-----------------------------------------------------------------------------*/

    /*----------------------------------Write Command------------------------------*/
        static LCD_enuErrorStatus_t LCD_enuWriteCommand(U8 Copy_u8Command)
        {
            extern const LCD_strLCDPinConfig_t LCD_strLCDpinConfig[LCD_PINs];

            LCD_enuErrorStatus_t LCD_enuErrorStatus = LCD_enumOK;

            GPIO_Error_t GPIO_Error_1 = GPIO_NOK;
            GPIO_Error_t GPIO_Error_2 = GPIO_NOK;
            GPIO_Error_t GPIO_Error_3 = GPIO_NOK;

            switch (G_LCD_EnablePinState)
            {
                case LCD_EnablePin_OFF:
                    /*Enable Trigger Pin*/
                        LCD_CtrlEnablePin(LCD_EnablePin_ON);
                    /*-----------------*/

                    /*Registers Set*/
                        GPIO_Error_1 = GPIO_Set_PinValue ( LCD_strLCDpinConfig [ RS ] .LCD_PortNumber , LCD_strLCDpinConfig [ RS ] .LCD_PinNumber , GPIO_STATE_RESET );
                        GPIO_Error_2 = GPIO_Set_PinValue ( LCD_strLCDpinConfig [ RW ] .LCD_PortNumber , LCD_strLCDpinConfig [ RW ] .LCD_PinNumber , GPIO_STATE_RESET );
                    /*-------------*/ 

                    /*Data Transfer*/
                    for (int LCD_iter = 3; LCD_iter < LCD_PINs; LCD_iter++)
                        {
                            U8 Local_Data_bit = ( ( Copy_u8Command & ( BIT00_MASK << (LCD_iter-3) ) ) >> (LCD_iter-3) );
                            GPIO_Error_3 = GPIO_Set_PinValue( LCD_strLCDpinConfig[ LCD_iter ].LCD_PortNumber , LCD_strLCDpinConfig[ LCD_iter ].LCD_PinNumber , Local_Data_bit );		
                        }
                    /*-------------*/

                    /*Change Global variable to the next stage*/
                        G_LCD_EnablePinState = LCD_EnablePin_ON;
                    /*--------------------------------------*/

                    /*--------Error return Mechanism--------*/
                        if (GPIO_Error_1 != GPIO_OK || GPIO_Error_2 != GPIO_OK || GPIO_Error_3 != GPIO_OK ) 
                        {
                            LCD_enuErrorStatus = LCD_enumNOK;
                        }
                        else 
                        {
                            LCD_enuErrorStatus = LCD_enumOK;
                        }
                        return LCD_enuErrorStatus;
                    /*--------------------------------------*/

                break;

                case LCD_EnablePin_ON :
                    /*Disable Trigger Pin*/
                        LCD_CtrlEnablePin(LCD_EnablePin_OFF);
                    /*-------------------*/

                    /*Change Global variable to the default stage*/
                        G_LCD_EnablePinState = LCD_EnablePin_OFF;
                    /*-------------------------------------------*/

                    return  LCD_enumNOK;                        

                break;
            }
            return LCD_enuErrorStatus;
        }
    /*-----------------------------------------------------------------------------*/

    /*-------------------------------------WriteData-------------------------------*/
        static LCD_enuErrorStatus_t LCD_enuWriteData(U8 Copy_u8Data)
        {
            extern const LCD_strLCDPinConfig_t LCD_strLCDpinConfig[];

            LCD_enuErrorStatus_t LCD_enuErrorStatus = LCD_enumOK;

            GPIO_Error_t GPIO_Error_1 = GPIO_NOK;
            GPIO_Error_t GPIO_Error_2 = GPIO_NOK;
            GPIO_Error_t GPIO_Error_3 = GPIO_NOK;

            switch (G_LCD_EnablePinState)
            {
                case LCD_EnablePin_OFF :
                    /*Enable Trigger Pin*/
                        LCD_CtrlEnablePin(LCD_EnablePin_ON);
                    /*-----------------*/

                    /*Registers Set*/
                        GPIO_Error_1 = GPIO_Set_PinValue ( LCD_strLCDpinConfig [ RS ] .LCD_PortNumber , LCD_strLCDpinConfig [ RS ] .LCD_PinNumber , GPIO_STATE_SET );
                        GPIO_Error_2 = GPIO_Set_PinValue ( LCD_strLCDpinConfig [ RW ] .LCD_PortNumber , LCD_strLCDpinConfig [ RW ] .LCD_PinNumber , GPIO_STATE_RESET );
                    /*-------------*/

                    /*Get & Send data*/
                    U8 Data_Iter;
                    for (Data_Iter = 0 ; Data_Iter<8 ; Data_Iter++)
                    {
                        /*Get needed Bit*/
                            U8 Local_Data_Bit = ( Copy_u8Data & (1 << Data_Iter ) ) ? GPIO_STATE_SET : GPIO_STATE_RESET ;
                        /*--------------*/

                        /*Set needed Bit*/
                            GPIO_Error_3 = GPIO_Set_PinValue ( LCD_strLCDpinConfig [ Data_Iter+3 ] .LCD_PortNumber , LCD_strLCDpinConfig [ Data_Iter+3 ] .LCD_PinNumber , Local_Data_Bit );
                        /*-------------*/
                    }
                    /*---------*/

                    /*Change Global variable to the next stage*/
                        G_LCD_EnablePinState = LCD_EnablePin_ON;
                    /*--------------------------------------*/

                    /*--------Error return Mechanism--------*/
                        if (GPIO_Error_1 != GPIO_OK || GPIO_Error_2 != GPIO_OK || GPIO_Error_3 != GPIO_OK ) 
                        {
                            LCD_enuErrorStatus = LCD_enumNOK;
                        }
                        else 
                        {
                            LCD_enuErrorStatus = LCD_enumOK;
                        }
                    /*--------------------------------------*/
                break;

                case LCD_EnablePin_ON :

                    /*Disable Trigger Pin*/
                        LCD_CtrlEnablePin(LCD_EnablePin_OFF);
                    /*-------------------*/

                    /*Change Global variable to the default stage*/
                        G_LCD_EnablePinState = LCD_EnablePin_OFF;
                    /*-------------------------------------------*/   

                    LCD_enuErrorStatus = LCD_enumNOK;           

                break;
            }
            return LCD_enuErrorStatus;
        }
    /*-----------------------------------------------------------------------------*/

    /*----------------------------------CtrlEnablePin------------------------------*/
        static void LCD_CtrlEnablePin(U8 Copy_LCDEnablePinState)
        {
            extern const LCD_strLCDPinConfig_t LCD_strLCDpinConfig[LCD_PINs];

            GPIO_Set_PinValue(LCD_strLCDpinConfig[ ET ].LCD_PortNumber, LCD_strLCDpinConfig[ ET ].LCD_PinNumber, Copy_LCDEnablePinState);
        }
    /*-----------------------------------------------------------------------------*/  

/*---------------------------------------------------------------------------------------*/