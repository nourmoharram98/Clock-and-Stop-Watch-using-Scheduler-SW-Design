/**
 * @file App.c
 * @author your name (you@domain.com)
 * @brief implementation of initialization functions
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
*/

/**
 * @brief Implementation of the runnable that will iterate over the digits to print them on LCD 
 * @brief Implementation of the runnable that will check for the switches in the system
 * @brief Implementation of the function that will check for the global flag of the mode (Clock or Stop Watch) and toggle the flag of mode according to current state 
 * @brief Implementation of the function that will check for
 edit mode and enable w disable switches and call function of state 
 machine* @brief 
 * @brief Implementation of the function of edit mode state machine 
    in General edit mode :
        -user can use up/down/right/left/ok/edit switches
-if up/down/left/right switches is pressed the function will jump to its relative state
                                                      
            Switch  |    Relative state                                      |  Limitation
            --------|--------------------------------------------------------|--------------------------------
            Up      |    Decrement the value of cursor set position by 64    |  block decrement if at min row or set to max row
            Down    |    Increment the value of cursor set position by 64    |  block increment if at max row or 
            Left    |    Decrement the value of cursor set position by 1     |  block decrement if at min column or
            Right   |    Increment the value of cursor set position by 1     |  block decrement if at max column or ...
        
        -if ok switch pressed the function will change the edit mode to Idle mode
    in Digit Edit mode:
        
            Switch  |    Relative sate                                               |    Limitation
            --------|----------------------------------------------------------------|---------------------------
            Up      |  Decrement the value of the digit relative to cursor position  |  no decrement more than 0
            Down    |  Increment the value of the digit relative to cursor position  |  no increment more than 9

        -up/down switches are checked and their corresponding functions will be called
        -in case user incremented to boundary values value must overflow again
    in Idel Edit mode:
        - inhibit the functionality of the up/down/right/left switches
        - turn off cursor
 * @brief function for up switch - take in consideration the boundary values and overflow and mode of edit (General or Digit)
 * @brief function for down switch - take in consideration the boundary values and overflow and mode of edit (General or Digit)
 * @brief function for right switch - take in consideration the boundary values and overflow
 * @brief function for left switch - take in consideration the boundary values and overflow
 * 
 * 
*/

/*--------------------------------Includes----------------------------*/
    #include "Std_Types.h"
    #include "HAL/LCD/HAL_LCD.h"
    #include "HAL/SWITCH/HAL_SWITCH.h"
    #include "APP/Manager.h"
    #include "HAL/LED/LED.h"
    #include "MCAL/USART/STM32F401cc_MCAL_USART.h"
    #include "SERVICE/COMM/UART_COMM.h"
/*---------------------------------------------------------------------*/

/*---------------------Buttons Macros Raw-Data(Without CRC)------------*/
    #define MODE_BUTTON_Data                         7  //r
    #define OK_BUTTON_Data                           5  //Q  
    #define EDIT_BUTTON_Data                         6  //d
    #define UP_BUTTON_Data                           4  //G
    #define DOWN_BUTTON_Data                         2  //#
    #define RIGHT_BUTTON_Data                        1  //UNDEFINED SYMBOL
    #define LEFT_BUTTON_Data                         3  //'5'
/*---------------------------------------------------------------------*/

/*-----------------------------Stop Watch modes------------------------*/
    #define STOP_WATCH_STOP                          0
    #define STOP_WATCH_START                         1
/*---------------------------------------------------------------------*/

/*----------------------------Global Variables-----------------------*/
    /**
     * @brief Mode: carry the Mode of operation (Clock-Date or Stop-Watch)
     * @brief Operation_type: carry the type of running operation (check for the enumeration in Manager.h)
     * @brief StopWatch_Status: carry the status of stop Watch STOP_WATCH_STOP or STOP_WATCH_START
     */
    u32 Mode=Clock_Mode;
    Operation_Types_t Operation_type=Init_Operation;
    extern u8 StopWatch_Status;
    u8 Global_X_Pos=0;
    u8 Global_Y_Pos=6;

    static EDIT_CURSOR_t EDIT_TEMP_VALUE={.x_pos=1,.y_pos=7};
    static sint8 EDIT_INDEX=0;
/*--------------------------------------------------------------------*/

/*----------------------------STATIC FUNCTION-------------------------*/
    static void Print_ClockFrame               ( );
    static void Print_StopWatchFrame           ( );
    static void print_frame_thread             (Modes_t Copy_Mode);
    static void DisplayOnLCD                   (Modes_t Copy_Mode);
    static void ChangePrintState               (Modes_t Copy_Mode , u8 Copy_PrintState);
    static void CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION(void);

/*--------------------------------------------------------------------*/

/**
 * @brief Manager runnable that check for the system mode and its operation states
 * @note operation state must start at Init_Operation then move to Idle Operation then any state can come
 * in order to prevent any undefined behaviour since before Idle_Operation the digits not all printed yet
 */


void Manager_Runnable(void)
{
    u32 Alternative_Mode = Mode^1;
    ChangePrintState(Alternative_Mode,DIGIT_STATE_NOT_PRINT);
    
    switch(Operation_type)
    {
        case Init_Operation:
            print_frame_thread(Mode);
            ChangePrintState(Mode,DIGIT_STATE_PRINT);
            Operation_type=Idle_Operation;
        break;

        case Idle_Operation:
            DisplayOnLCD(Mode);
        break;

        case GeneralEdit_Operation:
            //ChangePrintState(Mode,DIGIT_STATE_PRINT);
            DisplayOnLCD(Mode);
            GeneralEditMode();
        break;

        case DigitEdit_Operation:
           // DisplayOnLCD(Mode);
           // DigitEditMode();
        break;
        
        default:
            /*Do Nothing*/
        break;
    }
  
}

static void print_frame_thread(Modes_t Copy_Mode) 
{
    switch(Copy_Mode)
    {
        case Clock_Mode:
            Print_ClockFrame();
        break;

        case StopWatch_Mode:
            Print_StopWatchFrame();
        break;

        default:
            /*Nothing*/
        break;
    }
}

static void Print_ClockFrame()
{
    LCD_ClearScreenAsync();
    LCD_WriteStringAsync("CLOCK   /  /    ",16); 
    LCD_SetCursorPosAsync(1, 0);                                 
    LCD_WriteStringAsync("    :  :  : 00",14); 
}

static void Print_StopWatchFrame()
{
    LCD_ClearScreenAsync();
    LCD_WriteStringAsync("STOPWATCH START",15); 
    LCD_SetCursorPosAsync(1, 0);                                
    LCD_WriteStringAsync("    :  :  : 00",14); 
}

static void DisplayOnLCD(Modes_t Copy_Mode)
{
    /**
    * @brief Disable the cursor and blink only
    * 
    */
    LCD_WriteCommandAsync(LCD_CURSOR_BLINK_OFF);
    switch(Copy_Mode)
    {
        case Clock_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_CLK_MODE;index++)
            {
                if(Clock_Date_Digits[index].digit_state==DIGIT_STATE_PRINT)
                {
                    LCD_SetCursorPosAsync(Clock_Date_Digits[index].x_pos,Clock_Date_Digits[index].y_pos);
                    LCD_enuWriteNumber(Clock_Date_Digits[index].value);
                    Clock_Date_Digits[index].digit_state=DIGIT_STATE_NOT_PRINT;
                }
            }
        break;

        case StopWatch_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_STOPW_MODE;index++)
            {
                if(Stop_Watch_Digits[index].digit_state==DIGIT_STATE_PRINT)
               {
                    LCD_SetCursorPosAsync(Stop_Watch_Digits[index].x_pos,Stop_Watch_Digits[index].y_pos);
                    LCD_enuWriteNumber(Stop_Watch_Digits[index].value);
                    Stop_Watch_Digits[index].digit_state=DIGIT_STATE_NOT_PRINT;
               }
            }
        break;

        default:
            /*Nothing*/
        break;
    }
}

static void ChangePrintState(Modes_t Copy_Mode,u8 Copy_PrintState)
{
    switch(Copy_Mode)
    {
        case Clock_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_CLK_MODE;index++)
            {
                Clock_Date_Digits[index].digit_state=Copy_PrintState;
            }
        break;

        case StopWatch_Mode:
            for(u8 index=0;index<NUMBER_OF_DIGITS_STOPW_MODE;index++)
            {
                Stop_Watch_Digits[index].digit_state=Copy_PrintState;
            }
        break;

        default:
            /*Nothing*/
        break;
    }
}

void ControlSwitches_Runnable(void)
{ 

    /*Setting Switch Data To be sent*/
    static Ctrl_Switches_Data_t Ctrl_Switches_Data [4] = 
    {
        [SWITCH_OK]=
        {
            .DATA = 5,
            .Switch_Status = Switch_Released,
            .Switch_PrevStatus = Switch_Released
        }
        ,
        [SWITCH_EDIT]=
        {
            .DATA = 6,
            .Switch_Status = Switch_Released,
            .Switch_PrevStatus = Switch_Released
        }
        ,
        [SWITCH_LEFT] 
        {
            .DATA = 4 ,
            .Switch_Status = Switch_Released,
            .Switch_PrevStatus = Switch_Released
        }
        ,
        // [SWITCH_EDIT] 
        // {
        //     .DATA = 3 ,
        //     .Switch_Status = Switch_Released,
        //     .Switch_PrevStatus = Switch_Released
        // }
        // ,
        [SWITCH_RIGHT] 
        {
            .DATA = 1,
            .Switch_Status = Switch_Released,
            .Switch_PrevStatus = Switch_Released
        }
        ,
        // [SWITCH_DOWN] 
        // {
        //     .DATA = 5,
        //     .Switch_Status = Switch_Released,
        //     .Switch_PrevStatus = Switch_Released
        // }
        // , 
        // [SWITCH_LEFT] 
        // {
        //     .DATA = 6,
        //     .Switch_Status = Switch_Released,
        //     .Switch_PrevStatus = Switch_Released
        // }
        // ,
        // [SWITCH_RIGHT] 
        // {
        //     .DATA = 7,
        //     .Switch_Status = Switch_Released,
        //     .Switch_PrevStatus = Switch_Released
        // }
    };
    /*------------------------------*/

    /*SWITCH Reading and Sending Data*/
    U8 Switches_Iter;

    for (Switches_Iter = 0 ;Switches_Iter < Number_Of_Switches ; Switches_Iter++)
    {
        /*Read Switch State*/
            HAL_SWITCH_enuGetSwitchState( Switches_Iter ,&Ctrl_Switches_Data[Switches_Iter].Switch_Status );
        /*---------------------*/ 

        /*Single Realise Press signal handling and sending data via uart*/
            if(Ctrl_Switches_Data[Switches_Iter].Switch_Status == Switch_Pressed)
            {
                Ctrl_Switches_Data[Switches_Iter].Switch_PrevStatus = Ctrl_Switches_Data[Switches_Iter].Switch_Status;
            }
            else if(Ctrl_Switches_Data[Switches_Iter].Switch_PrevStatus == Switch_Pressed && Ctrl_Switches_Data[Switches_Iter].Switch_Status == Switch_Released)
            {
                /*Send unique data to TX_Communication_Manager*/
                    TX_Communication_Manager(Ctrl_Switches_Data[Switches_Iter].DATA); 
                /*-------------------------------------------*/

                /*------reset the switch status---------*/
                    Ctrl_Switches_Data[Switches_Iter].Switch_PrevStatus=Ctrl_Switches_Data[Switches_Iter].Switch_Status;
                /*--------------------------------------*/
            }
        /*--------------------------------------------------------------*/    
    }
    /*-------------------------------*/

}

/**
 * @brief function used to handle the input requests received from USART.
 * @note  the function execute every 10 msec periodicity
 * @param command 
 */
void Command_Handler(u8 command)
{
    // static u8 Edit_counter=0;
    switch(command)
    {
        case MODE_BUTTON_Data:
            Toggle_Mode();
        break;

        case EDIT_BUTTON_Data:
            Edit_Switch_Pressed();
            /*Nothing*/
        break;

        case OK_BUTTON_Data:
            OK_Switch_Pressed();
        break;

        case UP_BUTTON_Data:
            UP_Switch_Pressed();
        break;

        case DOWN_BUTTON_Data:
            /*Nothing*/
        break;

        case RIGHT_BUTTON_Data:  
            Right_Switch_Pressed();
            /*Nothing*/
        break;

        case LEFT_BUTTON_Data:
            Left_Switch_Pressed();
            /*Nothing*/
        break;

        default:
            /*Nothing*/
        break;
    }
    command=0;
}

/**
 * @brief Mode switch function to toggle between mode
 * 
 */
void Toggle_Mode(void)
{
    Mode ^=1;
    Operation_type=Init_Operation;
}

/**
 * @brief Ok switch function
 * @note need to handle both general edit and digit edit operations
 */
void OK_Switch_Pressed(void)
{
    if(Mode==Clock_Mode)
    {
        switch (Operation_type)
        {
        case GeneralEdit_Operation:
            //return to Idle Operation
            Operation_type=Idle_Operation;
        break;

        case DigitEdit_Operation:
            //save the changes
        break;

        default:
            /*Nothing*/
        break;
        }
    }
    else if(Mode==StopWatch_Mode)
    {
        StopWatch_Status ^=1;
    }
    else
    {
        //do nothing
    }
}

/**
 * @brief UP switch function to mock functionality only (yara will handle another one)
 * 
 */
void UP_Switch_Pressed(void)
{
    if(Mode==Clock_Mode)
    {
        switch(Operation_type)
        {
            case GeneralEdit_Operation:
                if(Global_X_Pos==0)
                {
                    Global_X_Pos=1;
                }
                else if(Global_X_Pos==1)
                {
                    Global_X_Pos=0;
                }
                LCD_SetCursorPosAsync(Global_X_Pos,Global_Y_Pos);
            break;

            case DigitEdit_Operation:
                /*Nothing*/
            break;

            default:
                /*Nothing*/
            break;
        }
    }
    else
    {
        /*Nothing*/
    }
}

void GeneralEditMode(void)
{
    CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION();
    LCD_WriteCommandAsync(LCD_CURSOR_BLINK_ON);
    LCD_SetCursorPosAsync(EDIT_TEMP_VALUE.x_pos,EDIT_TEMP_VALUE.y_pos);
}


static void CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION(void)
{
    EDIT_TEMP_VALUE.x_pos=Clock_Date_Digits[EDIT_INDEX].x_pos;
    EDIT_TEMP_VALUE.y_pos=Clock_Date_Digits[EDIT_INDEX].y_pos;
    EDIT_TEMP_VALUE.value=Clock_Date_Digits[EDIT_INDEX].value;
}

void Sender_Manager_Runnable(void)
{
    Communication_Sender();
}


void Receiver_Manager_Runnable(void)
{
    Communication_Receiver();
}



void Edit_Switch_Pressed(void)
{
    if(Mode==Clock_Mode)
    {
        switch(Operation_type)
        {
            case Idle_Operation:
                Operation_type=GeneralEdit_Operation;
            break;
            
            case GeneralEdit_Operation:
                Operation_type=DigitEdit_Operation;
            break;
            
            case DigitEdit_Operation:
                Operation_type=GeneralEdit_Operation;
            break;

            default:
                /**/
            break;
        }
    }
    else if(Mode==StopWatch_Mode)
    {
        //reset stop watch
    }
}

void Down_Switch_Pressed(void)
{

}

void Right_Switch_Pressed(void)
{
    EDIT_INDEX++;
    if(EDIT_INDEX>11)
    {
        EDIT_INDEX=0;
    }
    
}
void Left_Switch_Pressed(void)
{
    EDIT_INDEX--;
    if(EDIT_INDEX<0)
    {
        EDIT_INDEX=11;
    }
}