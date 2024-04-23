#include"Std_Types.h"
#include"Error_states.h"
#include"HAL/LCD/HAL_LCD.h"
#include"MCAL/GPIO/GPIO.h"


/* Macros for CLCD Commands */
#define CLCD_CLEAR_DISPLAY                  0x01
#define CLCD_RETURN_HOME                    0x02
#define CLCD_ENTRY_MODE_SET                 0x06
#define CLCD_DISPLAY_BLINK_CURSOR           0x0f
#define CLCD_DISPLAY                        0x0c
#define CLCD_DISPLAY_CURSOR                 0x0d
#define CLCD_FUNCTION_SET_8BITMODE_TWO_LINES_MODE 	0x38

#define LCD_ENABLE_LOW          0x00
#define LCD_ENABLE_HIGH         0x01

#define MAX_BUFFER_SIZE         10
#define MAX_STRING_SIZE         32

#define END_OF_ROW1             16
#define END_OF_ROW2             33

/********************Location in DDRAM handler for Lines*********************/
#define INDEX_FOR_LINE_1 0X0
#define INDEX_FOR_LINE_2 0X40
#define INDEX_FOR_LINE_3 0X10
#define INDEX_FOR_LINE_4 0X50
#define WRITE_ON_DDRAM_INDEX 0X80

#define BUFFER_SIZE         100

typedef enum
{
    LCD_ReqReady,
    LCD_ReqBusy,
}LCD_UserRequestState;

typedef enum
{
    LCD_NoReq,
    LCD_ReqWrite,
    LCD_ReqClear,
    LCD_ReqSetPos,
    LCD_ReqWriteCommand,
    LCD_ReqWriteNumber,
}LCD_UserRequestType;


typedef enum
{
    LCD_WriteStart,
    LCD_WriteCharacter,
    LCD_WriteDone
} LCD_WriteState_t;

typedef enum
{
    LCD_SetPosStart,
    LCD_SetPos,
    LCD_SetPosEnd
} LCD_SetPosState_t;
typedef enum
{
    LCD_PowerOn,
    LCD_FunctionSet,
    LCD_DisplayControl,
    LCD_ClearDisplay,
    LCD_EntryModeSet,
    LCD_EndInit
} LCD_InitMode_t;


typedef struct
{
    u8 CurRowPostion;
    u8 CurColPostion;
}LCD_PosCord;

typedef struct
{
    const char *UserString; //buffer
    u8 Len;
    u8 Command;
    LCD_UserRequestState State;
    LCD_UserRequestType Type;
    LCD_PosCord CurrentPos;
    u8 number;
} LCD_UserRequest;

struct{
    LCD_UserRequest buffer[BUFFER_SIZE];
    int head;  // Index to write to
    int tail;  // Index to read from
    int count; // Number of elements in the buffer
} CircularBuffer;


/*******************************************************************************
 *                              Variables                                       *
 *******************************************************************************/
extern const HAL_LCD_Configuration_t LCD_Configurations[NumberOfLCDs];
static u32 Global_EnablePinState = LCD_ENABLE_LOW;
static LCD_State_t Global_LCD_State = LCD_Off; //trigger of LCD System (1)
static LCD_InitMode_t Global_LCD_InitMode = LCD_PowerOn; //(3)
static LCD_WriteState_t Global_WriteState = LCD_WriteStart;


static u8 Cursor_Location=0;
static u8 Overflow_flag=0;
/*******************************************************************************
 *                         Static Function Prototypes		                   *
 *******************************************************************************/
static Sys_enuErrorStates_t LCD_Init_Process(void);
static Sys_enuErrorStates_t LCD_PowerOnProcess(void);
static Sys_enuErrorStates_t LCD_Write_Process(void);
static Sys_enuErrorStates_t LCD_SetPosition_Process(void);
static Sys_enuErrorStates_t LCD_Helper_SetPosition(u8 *PTR_PostionDDRAM);
static Sys_enuErrorStates_t LCD_WriteCommand(char Copy_LCDCommand);
static Sys_enuErrorStates_t LCD_WriteData(char Copy_LCDData);
static void LCD_WriteNumber_Proc(void);// 2ms
static Sys_enuErrorStates_t LCD_ClearDisplayProcess(void);
static Sys_enuErrorStates_t LCD_WriteCommandProcess(void);

//static Sys_enuErrorStates_t LCD_Helper_Clear(void);
//static Sys_enuErrorStates_t LCD_OverFlowHandler(void);


/**
 * @brief functions APIs
 * 
 */



void LCD_Runnable(void)
{
   // Runnable_Execution_time();
    switch(Global_LCD_State)
    {
        case LCD_Init:
            LCD_Init_Process();
            break;
        case LCD_Operation:
            if(CircularBuffer.count)
            {
                switch(CircularBuffer.buffer[CircularBuffer.tail].Type)
                {
                    case LCD_ReqWrite:
                        LCD_Write_Process();
                        break;
                    case LCD_ReqClear:
                        LCD_ClearDisplayProcess();
                        break;
                    case LCD_ReqSetPos:
                        LCD_SetPosition_Process();
                        break;
                    case LCD_ReqWriteNumber:
                        LCD_WriteNumber_Proc();
                        break;
                    case LCD_ReqWriteCommand:
                        LCD_WriteCommandProcess();
                    default:
                        break;
                }
            }
            break;
        case LCD_Off:
            break;
        case LCD_OverFlow:
            //LCD_OverFlowHandler(); //overflow step(2)
            break;
        default:
            break;
    }
   // Runnable_Execution_time();

}


static Sys_enuErrorStates_t LCD_Init_Process(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    switch(Global_LCD_InitMode)
    {
        case LCD_PowerOn:
            Error_Status=LCD_PowerOnProcess();
            Global_LCD_InitMode=LCD_FunctionSet;
            break;
        case LCD_FunctionSet:
            Error_Status=LCD_WriteCommand(CLCD_FUNCTION_SET_8BITMODE_TWO_LINES_MODE);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_DisplayControl;
            }
            break;
        case LCD_DisplayControl:
            Error_Status=LCD_WriteCommand(CLCD_DISPLAY_BLINK_CURSOR);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_ClearDisplay;
            }
            break;
        case LCD_ClearDisplay:
            Error_Status=LCD_WriteCommand(CLCD_CLEAR_DISPLAY);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_EntryModeSet;
            }
            break;
        case LCD_EntryModeSet:
            Error_Status=LCD_WriteCommand(CLCD_ENTRY_MODE_SET);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_EndInit;
            }
            break;
        case LCD_EndInit:
            CircularBuffer.head=0;
            CircularBuffer.tail=0;
            CircularBuffer.count=0;
            Global_LCD_State=LCD_Operation; //(5)
        default:
            Error_Status=INVALID_INPUT_VALUE;
            break;
    }
    return Error_Status;
}



static Sys_enuErrorStates_t LCD_PowerOnProcess(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    GPIO_Pin_t Local_PinConfiguration={0};
    Local_PinConfiguration.Pin_Mode=GPIO_MODE_OP_PP;
    Local_PinConfiguration.Pin_Speed=GPIO_SPEED_MED;
    for(u8 index_p=0;index_p<LCD_Configurations[LCD_One].config.LCD_PIN_NUM_MODE;index_p++)
    {
        Local_PinConfiguration.Pin_num=LCD_Configurations[LCD_One].pins[index_p].Pin;
        Local_PinConfiguration.Port=LCD_Configurations[LCD_One].pins[index_p].Port;
        GPIO_Init(&Local_PinConfiguration);    
    }
    return Error_Status;
}

static Sys_enuErrorStates_t LCD_Write_Process(void)
{

    Sys_enuErrorStates_t Error_Status=NOT_OK;
    static u8 Local_character_tracker=0;
    switch(Global_WriteState)
    {
        case LCD_WriteStart:
            Local_character_tracker=0;
            Global_WriteState=LCD_WriteCharacter;
            Error_Status=OK;
            break;
         case LCD_WriteCharacter:
                if (Local_character_tracker < CircularBuffer.buffer[CircularBuffer.tail].Len)
                {
                    // Get the next character from the current string and send it to LCD_WriteData
                    u8 character = CircularBuffer.buffer[CircularBuffer.tail].UserString[Local_character_tracker];
                    LCD_WriteData(character);
                
                        if(Global_EnablePinState==LCD_ENABLE_LOW)
                        {
                            // Move to the next character in the current string
                            Local_character_tracker++;
                        }
                   
                }
                else
                {
                    Local_character_tracker = 0;
                    Global_WriteState=LCD_WriteDone;
                }
            Error_Status=OK;
            break;
        case LCD_WriteDone:
                CircularBuffer.buffer[CircularBuffer.tail].Type=LCD_NoReq;
                CircularBuffer.buffer[CircularBuffer.tail].State=LCD_ReqReady;
                CircularBuffer.buffer[CircularBuffer.tail].UserString=NULL_POINTER;
                CircularBuffer.buffer[CircularBuffer.tail].Len=0;
                CircularBuffer.tail++;
                CircularBuffer.count--;
                if(CircularBuffer.tail==BUFFER_SIZE)
                {
                    CircularBuffer.tail=0;
                }
                Global_WriteState=LCD_WriteStart;
            break;
        default:
            break;
    }
    return Error_Status;
}


static Sys_enuErrorStates_t LCD_SetPosition_Process(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    static u8 Local_Position_Command=0;
    LCD_Helper_SetPosition(&Local_Position_Command);
    LCD_WriteCommand(Local_Position_Command);
    if(Global_EnablePinState==LCD_ENABLE_LOW)
    {
        Local_Position_Command=0;
        Error_Status=OK;
        if(Overflow_flag)
        {
            Global_LCD_State=LCD_OverFlow; //overflow function is the caller of set position
        }
        else
        {
            CircularBuffer.buffer[CircularBuffer.tail].State=LCD_ReqReady; //normal setposition call
            CircularBuffer.buffer[CircularBuffer.tail].Type=LCD_NoReq;
            CircularBuffer.tail++;
            CircularBuffer.count--;
            if(CircularBuffer.tail==BUFFER_SIZE)
            {
                CircularBuffer.tail=0;
            }
        }
    }

   return Error_Status;
}


static Sys_enuErrorStates_t LCD_Helper_SetPosition(u8 *PTR_PostionDDRAM)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;
   u8 Local_PositioninDDRAM=0;
    if(PTR_PostionDDRAM==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        switch(CircularBuffer.buffer[CircularBuffer.tail].CurrentPos.CurRowPostion)
        {
            case LCD_DISPLAY_ROW1:
                Local_PositioninDDRAM=CircularBuffer.buffer[CircularBuffer.tail].CurrentPos.CurColPostion +INDEX_FOR_LINE_1;
                break;
            case LCD_DISPLAY_ROW2:
                Local_PositioninDDRAM=CircularBuffer.buffer[CircularBuffer.tail].CurrentPos.CurColPostion +INDEX_FOR_LINE_2;
                break;
            case LCD_DISPLAY_ROW3:
                Local_PositioninDDRAM=CircularBuffer.buffer[CircularBuffer.tail].CurrentPos.CurColPostion +INDEX_FOR_LINE_3;
                break;
            case LCD_DISPLAY_ROW4:
                Local_PositioninDDRAM=CircularBuffer.buffer[CircularBuffer.tail].CurrentPos.CurColPostion +INDEX_FOR_LINE_4;
                break;
            default:
                Error_Status=INVALID_INPUT_VALUE;
                break;
        }
        *PTR_PostionDDRAM=Local_PositioninDDRAM+WRITE_ON_DDRAM_INDEX;
    }
   return Error_Status;
}


static Sys_enuErrorStates_t LCD_ClearDisplayProcess(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    Error_Status=LCD_WriteCommand(CLCD_CLEAR_DISPLAY);
    if(Global_EnablePinState==LCD_ENABLE_LOW)
    {
        CircularBuffer.buffer[CircularBuffer.tail].State=LCD_ReqReady; //normal setposition call
        CircularBuffer.buffer[CircularBuffer.tail].Type=LCD_NoReq;
        CircularBuffer.tail++;
        CircularBuffer.count--;
        if(CircularBuffer.tail==BUFFER_SIZE)
        {
            CircularBuffer.tail=0;
        }
    }

   return Error_Status;
}


static Sys_enuErrorStates_t LCD_WriteCommandProcess(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    Error_Status=LCD_WriteCommand(CircularBuffer.buffer[CircularBuffer.tail].Command);
    if(Global_EnablePinState==LCD_ENABLE_LOW)
    {
        CircularBuffer.buffer[CircularBuffer.tail].State=LCD_ReqReady; //normal setposition call
        CircularBuffer.buffer[CircularBuffer.tail].Type=LCD_NoReq;
        CircularBuffer.buffer[CircularBuffer.tail].Command=0x00;
        CircularBuffer.tail++;
        CircularBuffer.count--;
        if(CircularBuffer.tail==BUFFER_SIZE)
        {
            CircularBuffer.tail=0;
        }
    }

   return Error_Status;
}

static Sys_enuErrorStates_t LCD_WriteCommand(char Copy_LCDCommand)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;
   GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RS_8BIT].Port,LCD_Configurations[LCD_One].pins[RS_8BIT].Pin,GPIO_STATE_RESET);
   GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RW_8BIT].Port,LCD_Configurations[LCD_One].pins[RW_8BIT].Pin,GPIO_STATE_RESET);
   if(Global_EnablePinState==LCD_ENABLE_LOW)
   {
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D0].Port,LCD_Configurations[LCD_One].pins[D0].Pin,(Copy_LCDCommand>>D0&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D1].Port,LCD_Configurations[LCD_One].pins[D1].Pin,(Copy_LCDCommand>>D1&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D2].Port,LCD_Configurations[LCD_One].pins[D2].Pin,(Copy_LCDCommand>>D2&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D3].Port,LCD_Configurations[LCD_One].pins[D3].Pin,(Copy_LCDCommand>>D3&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D4].Port,LCD_Configurations[LCD_One].pins[D4].Pin,(Copy_LCDCommand>>D4&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D5].Port,LCD_Configurations[LCD_One].pins[D5].Pin,(Copy_LCDCommand>>D5&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D6].Port,LCD_Configurations[LCD_One].pins[D6].Pin,(Copy_LCDCommand>>D6&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D7].Port,LCD_Configurations[LCD_One].pins[D7].Pin,(Copy_LCDCommand>>D7&1));

        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_STATE_SET);
        Global_EnablePinState=LCD_ENABLE_HIGH;
        Error_Status=OK;
   }
   else if(Global_EnablePinState==LCD_ENABLE_HIGH)
   {
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_STATE_RESET);
        Global_EnablePinState=LCD_ENABLE_LOW;
        Error_Status=OK;
   }
   return Error_Status;
}



static Sys_enuErrorStates_t LCD_WriteData(char Copy_LCDData)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RS_8BIT].Port,LCD_Configurations[LCD_One].pins[RS_8BIT].Pin,GPIO_STATE_SET);
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RW_8BIT].Port,LCD_Configurations[LCD_One].pins[RW_8BIT].Pin,GPIO_STATE_RESET);
        if(Global_EnablePinState==LCD_ENABLE_LOW)
        {
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_STATE_RESET);

            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D0].Port,LCD_Configurations[LCD_One].pins[D0].Pin,(Copy_LCDData>>D0&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D1].Port,LCD_Configurations[LCD_One].pins[D1].Pin,(Copy_LCDData>>D1&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D2].Port,LCD_Configurations[LCD_One].pins[D2].Pin,(Copy_LCDData>>D2&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D3].Port,LCD_Configurations[LCD_One].pins[D3].Pin,(Copy_LCDData>>D3&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D4].Port,LCD_Configurations[LCD_One].pins[D4].Pin,(Copy_LCDData>>D4&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D5].Port,LCD_Configurations[LCD_One].pins[D5].Pin,(Copy_LCDData>>D5&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D6].Port,LCD_Configurations[LCD_One].pins[D6].Pin,(Copy_LCDData>>D6&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D7].Port,LCD_Configurations[LCD_One].pins[D7].Pin,(Copy_LCDData>>D7&1));

            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_STATE_SET);
            Global_EnablePinState=LCD_ENABLE_HIGH;

            Error_Status=OK;
        }
        else if(Global_EnablePinState==LCD_ENABLE_HIGH)
        {
            Cursor_Location++;
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_STATE_RESET);
            Global_EnablePinState=LCD_ENABLE_LOW;
            Error_Status=OK;
        }
   
   return Error_Status; 
}

Sys_enuErrorStates_t LCD_InitAsync(void)
{
    Sys_enuErrorStates_t Error_Status=OK;
    Global_LCD_State=LCD_Init; //(2)
    return Error_Status;
}
Sys_enuErrorStates_t LCD_SetCursorPosAsync(u8 Copy_LCDPosx,u8 Copy_LCDPosy)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(Copy_LCDPosx>1 || Copy_LCDPosy >15)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(CircularBuffer.count==BUFFER_SIZE)
    {
        Error_Status=LCD_BUFFER_REQUESTS_FULL;
    }
    else
    {
        CircularBuffer.buffer[CircularBuffer.head].Type=LCD_ReqSetPos;
        CircularBuffer.buffer[CircularBuffer.head].CurrentPos.CurRowPostion=Copy_LCDPosx;
        CircularBuffer.buffer[CircularBuffer.head].CurrentPos.CurColPostion=Copy_LCDPosy;
        CircularBuffer.head++;
        CircularBuffer.count++;
        /*to make head overloop on the circular buffer*/
        if(CircularBuffer.head==BUFFER_SIZE&&CircularBuffer.count<BUFFER_SIZE)
        {
            CircularBuffer.head=0;
        }
    }
    return Error_Status;
}

Sys_enuErrorStates_t LCD_WriteStringAsync(const char *ptrToString,u8 String_size)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(ptrToString == NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else if(String_size >= MAX_STRING_SIZE)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(CircularBuffer.count==BUFFER_SIZE)
    {
        Error_Status=LCD_BUFFER_REQUESTS_FULL;
    }
    else
    {
        CircularBuffer.buffer[CircularBuffer.head].Type=LCD_ReqWrite;
        CircularBuffer.buffer[CircularBuffer.head].UserString=ptrToString;
        CircularBuffer.buffer[CircularBuffer.head].Len=String_size;
        CircularBuffer.head++;
        CircularBuffer.count++;
        if(CircularBuffer.head==BUFFER_SIZE&&CircularBuffer.count<BUFFER_SIZE)
        {
            CircularBuffer.head=0;
        }
    }
    return Error_Status;
}

Sys_enuErrorStates_t LCD_enuWriteNumber(u32 number)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(number<0)
    {
        Error_Status=NOT_OK;
    }
    else if(CircularBuffer.count==BUFFER_SIZE)
    {
        Error_Status=LCD_BUFFER_REQUESTS_FULL;
    }
    else
    {
        CircularBuffer.buffer[CircularBuffer.head].Type=LCD_ReqWriteNumber;
        CircularBuffer.buffer[CircularBuffer.head].number=number;
        CircularBuffer.head++;
        CircularBuffer.count++;
        if(CircularBuffer.head==BUFFER_SIZE&&CircularBuffer.count<BUFFER_SIZE)
        {
            CircularBuffer.head=0;
        }
    }
    return Error_Status;
}

Sys_enuErrorStates_t LCD_ClearScreenAsync(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(CircularBuffer.count==BUFFER_SIZE)
    {
        Error_Status=LCD_BUFFER_REQUESTS_FULL;
    }
    else
    {
        CircularBuffer.buffer[CircularBuffer.head].Type=LCD_ReqClear;
        CircularBuffer.head++;
        CircularBuffer.count++;
        if(CircularBuffer.head==BUFFER_SIZE&&CircularBuffer.count<BUFFER_SIZE)
        {
            CircularBuffer.head=0;
        }
    }
    return Error_Status;
}

Sys_enuErrorStates_t LCD_WriteCommandAsync(const char command)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(CircularBuffer.count==BUFFER_SIZE)
    {
        Error_Status=LCD_BUFFER_REQUESTS_FULL;
    }
    else
    {
        CircularBuffer.buffer[CircularBuffer.head].Type=LCD_ReqWriteCommand;
        CircularBuffer.buffer[CircularBuffer.head].Command=command;
        CircularBuffer.head++;
        CircularBuffer.count++;
        if(CircularBuffer.head==BUFFER_SIZE&&CircularBuffer.count<BUFFER_SIZE)
        {
            CircularBuffer.head=0;
        }
    }
    return Error_Status;
}
Sys_enuErrorStates_t LCD_GetStatus(u32* PtrToLCDstatus)
{
    Sys_enuErrorStates_t Error_Status=OK;
    if(PtrToLCDstatus==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        *PtrToLCDstatus=Global_LCD_State;
    }
    return Error_Status;
}
// Sys_enuErrorStates_t LCD_WriteSetCB(LCD_CBFUNC_t PtrTofunc)
// {
//     Sys_enuErrorStates_t Error_Status=OK;
    
//     return Error_Status;
// }
// Sys_enuErrorStates_t LCD_ClearSetCB(LCD_CBFUNC_t PtrTofunc)
// {
//     Sys_enuErrorStates_t Error_Status=OK;
    
//     return Error_Status;
// }
// Sys_enuErrorStates_t LCD_SetCursorSetCB(LCD_CBFUNC_t PtrTofunc)
// {
//     Sys_enuErrorStates_t Error_Status=OK;
    
//     return Error_Status;
// }
static void LCD_WriteNumber_Proc(void) // 2ms
{
    // U8 LOC_u8Reversed = 1 ;

    switch (Global_WriteState)
    {
        case LCD_WriteStart: 
            Global_WriteState = LCD_WriteCharacter;
        break;

        case LCD_WriteCharacter:
            LCD_WriteData(CircularBuffer.buffer[CircularBuffer.tail].number+48 );//4
            if( Global_EnablePinState == LCD_ENABLE_LOW )
            {
                Global_WriteState = LCD_WriteDone;
            }
            else 
            {
                /*Wait till write command finish*/
            }

        break;
        case LCD_WriteDone:
            CircularBuffer.buffer[CircularBuffer.tail].Type=LCD_NoReq;
            CircularBuffer.buffer[CircularBuffer.tail].State=LCD_ReqReady;
            CircularBuffer.buffer[CircularBuffer.tail].number=0;
            CircularBuffer.tail++;
            CircularBuffer.count--;
            if(CircularBuffer.tail==BUFFER_SIZE)
            {
                CircularBuffer.tail=0;
            }
            Global_WriteState=LCD_WriteStart;
        break;

    }  
}