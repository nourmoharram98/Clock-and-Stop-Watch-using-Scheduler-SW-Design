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
    static void DigitPrint(u8 digit,u8 DIGIT_STATE);

/*--------------------------------------------------------------------*/

/**
 * @brief Manager runnable that check for the system mode and its operation states
 * @note operation state must start at Init_Operation then move to Idle Operation then any state can come
 * in order to prevent any undefined behaviour since before Idle_Operation the digits not all printed yet
 */


/*-------------------------leap Year Handling------------------------*/
    u16 year;

    void calculateyear (void)
    {
        // Reconstruct the full year value
        year = Clock_Date_Digits[Years_thousand].value * 1000 +
            Clock_Date_Digits[Years_hundreds].value * 100 +
            Clock_Date_Digits[Years_tens].value * 10 +
            Clock_Date_Digits[Years_unit].value;
    }

    bool IsLeapYear_edit(u16 year) 
    {
        calculateyear();
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return true;
        } else {
            return false;
        }
    }


/*---------------------------------------------Modes Handlers-----------------------------------------------------*/

void GeneralEditMode(void)
{
    CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION();
    LCD_WriteCommandAsync(LCD_CURSOR_BLINK_ON);
    LCD_SetCursorPosAsync(EDIT_TEMP_VALUE.x_pos,EDIT_TEMP_VALUE.y_pos);
}
void DigitEditMode(void)
{
   // CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION();
    LCD_WriteCommandAsync(LCD_CURSOR_BLINK_ON);
    LCD_SetCursorPosAsync(EDIT_TEMP_VALUE.x_pos,EDIT_TEMP_VALUE.y_pos);

}


static void CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION(void)
{
    EDIT_TEMP_VALUE.x_pos=Clock_Date_Digits[EDIT_INDEX].x_pos;
    EDIT_TEMP_VALUE.y_pos=Clock_Date_Digits[EDIT_INDEX].y_pos;
    EDIT_TEMP_VALUE.value=Clock_Date_Digits[EDIT_INDEX].value;
}
/*---------------------------------------------Runnables-----------------------------------------------------*/

/*-------------------------------Manager Runnable------------------------------------*/
    void Manager_Runnable(void)
    {
       
       // Runnable_Execution_time();
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
                DigitPrint(EDIT_INDEX,DIGIT_STATE_PRINT);
                DisplayOnLCD(Mode);
                GeneralEditMode();
            break;

            case DigitEdit_Operation:
                DigitPrint(EDIT_INDEX,DIGIT_STATE_NOT_PRINT);
                DisplayOnLCD(Mode);
                DigitEditMode();
            break;
            
            default:
                /*Do Nothing*/
            break;
        }
      //  Runnable_Execution_time();
    }

/*-------------------------------Control Switch Runnable-----------------------------*/
    void ControlSwitches_Runnable(void)
    { 
      //  Runnable_Execution_time();

    /*Setting Switch Data To be sent*/
        static Ctrl_Switches_Data_t Ctrl_Switches_Data [7] = 
        {
            [SWITCH_MODE]=
            {
                .DATA = 7,
                .Switch_Status = Switch_Released,
                .Switch_PrevStatus = Switch_Released
            }
            ,
            [SWITCH_OK]=
            {
                .DATA = 5,
                .Switch_Status = Switch_Released,
                .Switch_PrevStatus = Switch_Released
            }
            ,
            [SWITCH_EDIT] 
            {
                .DATA = 6 ,
                .Switch_Status = Switch_Released,
                .Switch_PrevStatus = Switch_Released
            }
            ,
            [SWITCH_UP] 
            {
                .DATA = 4 ,
                .Switch_Status = Switch_Released,
                .Switch_PrevStatus = Switch_Released
            }
            ,
            [SWITCH_DOWN] 
            {
                .DATA = 2,
                .Switch_Status = Switch_Released,
                .Switch_PrevStatus = Switch_Released
            }
            ,
            [SWITCH_LEFT] 
            {
                .DATA = 3,
                .Switch_Status = Switch_Released,
                .Switch_PrevStatus = Switch_Released
            }
            , 
            [SWITCH_RIGHT] 
            {
                .DATA = 1,
                .Switch_Status = Switch_Released,
                .Switch_PrevStatus = Switch_Released
            }
        };
    /*------------------------------*/

        /*SWITCH Reading and Sending Data*/
        U8 Switches_Iter=0;

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
      //  Runnable_Execution_time();

    }
/*-------------------------------Sender Manager Runnable-----------------------------*/
    void Sender_Manager_Runnable(void)
    {
        //Runnable_Execution_time();
        Communication_Sender();
       // Runnable_Execution_time();

    }

/*-------------------------------Receiver Manager Runnable-------------------------- */
    void Receiver_Manager_Runnable(void)
    {
       // Runnable_Execution_time();
        Communication_Receiver();
       // Runnable_Execution_time();

    }

/*-------------------------------Print Frame Functions-------------------------------------------------------*/
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

static void DigitPrint(u8 digit,u8 DIGIT_STATE)
{
    Clock_Date_Digits[digit].digit_state=DIGIT_STATE;
}


/*-------------------------------DisplayOnLCD Function----------------------------------------------------------*/
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


/*-----------------------------------------Command Handler--------------------------------------------------*/
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
            Mode_Switch_Pressed();
        break;

        case EDIT_BUTTON_Data:
            Edit_Switch_Pressed();
        break;

        case OK_BUTTON_Data:
            OK_Switch_Pressed();
        break;

        case UP_BUTTON_Data:
            UP_Switch_Pressed();
        break;

        case DOWN_BUTTON_Data:
            Down_Switch_Pressed();
        break;

        case RIGHT_BUTTON_Data:  
            Right_Switch_Pressed();
        break;

        case LEFT_BUTTON_Data:
            Left_Switch_Pressed();
        break;

        default:
            /*Nothing*/
        break;
    }
    command=0;
}
/*-----------------------------------------Switches Handlers--------------------------------------------------*/

/**
 * @brief Mode switch function to toggle between mode
 * 
 */
void Mode_Switch_Pressed(void)
{
    if(Operation_type==Idle_Operation)
    {
        Mode ^=1;
        Operation_type=Init_Operation;

    }
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

            Clock_Date_Digits[EDIT_INDEX].value=EDIT_TEMP_VALUE.value; //for saving

            // for(int i=0;i<15;i++)
            // {
            //     Clock_Date_Digits[i].digit_state=DIGIT_STATE_PRINT;
            // }

            Operation_type=GeneralEdit_Operation;
            
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
                if (EDIT_INDEX <= 7)
                {
                    EDIT_INDEX= 8;
                }
                else 
                {
                    EDIT_INDEX= 0;
                }
            break;

            case DigitEdit_Operation:

                EDIT_TEMP_VALUE.value++;

                /*Minutes Reset Handling*/

                    /*Min Tens*/
                        if(EDIT_INDEX==Minutes_tens)
                        {
                            if(EDIT_TEMP_VALUE.value > 5)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*--------*/

                    /*Min Units*/
                        if(EDIT_INDEX==Minutes_unit)
                        {
                            if(EDIT_TEMP_VALUE.value>9)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*---------*/

                /*----------------------*/

                /*Hour reset Handling*/

                    /*Hour Tens*/
                        if(EDIT_INDEX==Hours_tens)
                        {
                            if(EDIT_TEMP_VALUE.value > 2)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*---------*/

                    /*Hour Units*/
                        if(EDIT_INDEX==Hours_unit)
                        {
                            if (Clock_Date_Digits[Hours_tens].value == 1)
                            {
                                if(EDIT_TEMP_VALUE.value>9)
                                {
                                    EDIT_TEMP_VALUE.value=0;
                                }
                            }

                            if (Clock_Date_Digits[Hours_tens].value == 2)
                            {
                                if(EDIT_TEMP_VALUE.value > 3)
                                {
                                    EDIT_TEMP_VALUE.value=0;
                                }
                            }
                            if (Clock_Date_Digits[Hours_tens].value == 0)
                            {
                                if(EDIT_TEMP_VALUE.value > 9)
                                {
                                    EDIT_TEMP_VALUE.value=0;
                                }
                            }
                        }
                    /*----------*/
                    
                /*-------------------*/

                /*Days reset Handling*/

                    /*Day tens*/
                            if (EDIT_INDEX==Day_tens)
                            {
                            /*Reset to zero*/
                                if ( Clock_Date_Digits[Day_unit].value > 0 )
                                {
                                    /*Feb*/
                                        if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2)
                                        {
                                            if (IsLeapYear_edit(year))
                                            {
                                                if ( EDIT_TEMP_VALUE.value > 2 )
                                                {
                                                    EDIT_TEMP_VALUE.value=0;
                                                }
                                            }
                                            else
                                            {
                                                if (Clock_Date_Digits[Day_unit].value > 8  )
                                                {
                                                    if(EDIT_TEMP_VALUE.value>1)
                                                    {
                                                        EDIT_TEMP_VALUE.value=0;
                                                    }
                                                }
                                                else if (Clock_Date_Digits[Day_unit].value < 8 && EDIT_TEMP_VALUE.value > 2)
                                                {
                                                    EDIT_TEMP_VALUE.value=0;
                                                }
                                            }
                                        }
                                    /*---*/

                                    /* 30 day months*/
                                        else if (
                                                    Clock_Date_Digits[Month_unit].value == 4 ||
                                                    Clock_Date_Digits[Month_unit].value == 6 ||   
                                                    Clock_Date_Digits[Month_unit].value == 9 ||    
                                                    (Clock_Date_Digits[Month_unit].value == 1
                                                    &&
                                                    Clock_Date_Digits[Month_tens].value == 1)  
                                                )

                                                {
                                                    if (Clock_Date_Digits[Day_unit].value==0 && EDIT_TEMP_VALUE.value>3)
                                                    {
                                                        EDIT_TEMP_VALUE.value=0;
                                                    }
                                                    else if ( Clock_Date_Digits[Day_unit].value>0 && EDIT_TEMP_VALUE.value>2 )
                                                    {
                                                        EDIT_TEMP_VALUE.value=0;
                                                    }
                                                }
                                    /*--------------*/

                                    /* 31 days month */
                                            else if ( (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value  == 1)   || 
                                                    Clock_Date_Digits[Month_unit].value  == 3                                                ||
                                                    Clock_Date_Digits[Month_unit].value  == 5                                                ||
                                                    Clock_Date_Digits[Month_unit].value  == 7                                                ||
                                                    Clock_Date_Digits[Month_unit].value  == 8                                                ||
                                                    (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 0)   ||                                     
                                                    (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 2)  
                                                    )
                                                    {
                                                        if ((Clock_Date_Digits[Day_unit].value > 1) && EDIT_TEMP_VALUE.value>2)
                                                        {
                                                            EDIT_TEMP_VALUE.value=0;
                                                        }
                                                        else if ((Clock_Date_Digits[Day_unit].value <= 1) && EDIT_TEMP_VALUE.value>3)
                                                        {
                                                            EDIT_TEMP_VALUE.value=0;
                                                        }
                                                    }
                                    /*---------------*/    
                                }    
                            /*------------*/    

                            /*Reset to one*/
                                else if ( Clock_Date_Digits[Day_unit].value == 0 )
                                {
                                    /*Feb*/
                                        if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2)
                                        {
                                            if (IsLeapYear_edit(year))
                                            {
                                                if (Clock_Date_Digits[Day_unit].value > 8 && EDIT_TEMP_VALUE.value > 2 )
                                                {
                                                    EDIT_TEMP_VALUE.value=1;
                                                }
                                                else if (Clock_Date_Digits[Day_unit].value < 8 && EDIT_TEMP_VALUE.value > 2)
                                                {
                                                    EDIT_TEMP_VALUE.value=1;
                                                }
                                            }
                                            else
                                            {
                                                if (Clock_Date_Digits[Day_unit].value > 7 && EDIT_TEMP_VALUE.value > 2 )
                                                {
                                                    EDIT_TEMP_VALUE.value=1;
                                                }
                                                else if (Clock_Date_Digits[Day_unit].value < 8 && EDIT_TEMP_VALUE.value > 2)
                                                {
                                                    EDIT_TEMP_VALUE.value=1;
                                                }
                                            }
                                        }
                                    /*---*/

                                    /* 30 day months*/
                                        else if (
                                                    Clock_Date_Digits[Month_unit].value == 4 ||
                                                    Clock_Date_Digits[Month_unit].value == 6 ||   
                                                    Clock_Date_Digits[Month_unit].value == 9 ||    
                                                    (Clock_Date_Digits[Month_unit].value == 1
                                                    &&
                                                    Clock_Date_Digits[Month_tens].value == 1)  
                                                )

                                                {
                                                    if (Clock_Date_Digits[Day_unit].value==0 && EDIT_TEMP_VALUE.value>3)
                                                    {
                                                        EDIT_TEMP_VALUE.value=1;
                                                    }
                                                    else if ( Clock_Date_Digits[Day_unit].value>0 && EDIT_TEMP_VALUE.value>2 )
                                                    {
                                                        EDIT_TEMP_VALUE.value=1;
                                                    }
                                                }
                                    /*--------------*/

                                    /* 31 days month */
                                            else if ( (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value  == 1)   || 
                                                    Clock_Date_Digits[Month_unit].value  == 3                                                ||
                                                    Clock_Date_Digits[Month_unit].value  == 5                                                ||
                                                    Clock_Date_Digits[Month_unit].value  == 7                                                ||
                                                    Clock_Date_Digits[Month_unit].value  == 8                                                ||
                                                    (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 0)   ||                                     
                                                    (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 2)  
                                                    )
                                                    {
                                                        if ((Clock_Date_Digits[Day_unit].value > 1) && EDIT_TEMP_VALUE.value>2)
                                                        {
                                                            EDIT_TEMP_VALUE.value=1;
                                                        }
                                                        else if ((Clock_Date_Digits[Day_unit].value <= 1) && EDIT_TEMP_VALUE.value>3)
                                                        {
                                                            EDIT_TEMP_VALUE.value=1;
                                                        }
                                                    }
                                    /*---------------*/    
                                }   
                            /*---------*/
                            }
                    /*--------*/   

                    /*Days Units*/
                        if(EDIT_INDEX==Day_unit)
                        {
                            if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2)
                            {
                                if(IsLeapYear_edit(year))
                                {
                                    if(EDIT_TEMP_VALUE.value>9)
                                    {
                                        EDIT_TEMP_VALUE.value = 0;
                                    }
                                }
                                else
                                {
                                    if ( (EDIT_TEMP_VALUE.value>8 && Clock_Date_Digits[Day_tens].value == 2) ||
                                            (EDIT_TEMP_VALUE.value >9 && Clock_Date_Digits[Day_tens].value < 2))
                                        {
                                            EDIT_TEMP_VALUE.value =0;
                                        }
                                }

                            }
                            else if ((Clock_Date_Digits[Month_unit].value == 4 || Clock_Date_Digits[Month_unit].value == 6 || Clock_Date_Digits[Month_unit].value == 9 || (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value==1))&& Clock_Date_Digits[Day_tens].value==3)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }   
                            else
                            {   
                                if (Clock_Date_Digits[Day_tens].value == 0)
                                {
                                    if(EDIT_TEMP_VALUE.value>9)
                                    {
                                        EDIT_TEMP_VALUE.value=1;
                                    }
                                }

                                if (Clock_Date_Digits[Day_tens].value > 0 && Clock_Date_Digits[Day_tens].value < 3 )
                                {
                                    if(EDIT_TEMP_VALUE.value>9)
                                    {
                                        EDIT_TEMP_VALUE.value=0;
                                    }
                                }

                                if(Clock_Date_Digits[Day_tens].value == 3)
                                {
                                    if ( EDIT_TEMP_VALUE.value>1 )
                                    {
                                        EDIT_TEMP_VALUE.value=0;
                                    }
                                }

                            }       
                        }       
                    /*----------*/   

                /*-------------------*/

                /*Month reset Handling*/

                    /*Month tens*/
                        if(EDIT_INDEX==Month_tens)
                        {
                            if(EDIT_TEMP_VALUE.value>1)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                            if(Clock_Date_Digits[Month_unit].value==0)
                            {
                                EDIT_TEMP_VALUE.value=1;
                            }
                            if(Clock_Date_Digits[Month_unit].value>2)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*----------*/    

                    /*Month unit*/
                        if(EDIT_INDEX == Month_unit)
                        {
                            if ( Clock_Date_Digits[Month_tens].value == 0 )
                            {
                                if (EDIT_TEMP_VALUE.value > 9)
                                {
                                    EDIT_TEMP_VALUE.value=1;
                                }
                            }

                            if ( Clock_Date_Digits[Month_tens].value == 1 )
                            {
                                if (EDIT_TEMP_VALUE.value > 2)
                                {
                                    EDIT_TEMP_VALUE.value=0;
                                }
                            }
                        }
                    /*----------*/

                /*--------------------*/

                /*Year reset Handling*/

                    /*year Thousand*/
                        if(EDIT_INDEX==Years_thousand)
                        {
                            if(EDIT_TEMP_VALUE.value>9)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*-------------*/

                    /*Year Hundred*/
                        if(EDIT_INDEX==Years_hundreds)
                        {
                            if(EDIT_TEMP_VALUE.value>9)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*------------*/

                    /*Year Tens*/
                        if(EDIT_INDEX==Years_tens)
                        {
                            if(EDIT_TEMP_VALUE.value>9)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*---------*/

                    /*Year Units*/
                        if(EDIT_INDEX==Years_unit)
                        {
                            if(EDIT_TEMP_VALUE.value>9)
                            {
                                EDIT_TEMP_VALUE.value=0;
                            }
                        }
                    /*----------*/

                /*-------------------*/
                LCD_enuWriteNumber(EDIT_TEMP_VALUE.value);
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

void Down_Switch_Pressed(void)
{
    if(Mode==Clock_Mode)
        {
            switch(Operation_type)
            {
                case GeneralEdit_Operation:
                    if (EDIT_INDEX <= 7)
                    {
                        EDIT_INDEX= 8;
                    }
                    else 
                    {
                        EDIT_INDEX= 0;
                    }
                break;

                case DigitEdit_Operation:
                    EDIT_TEMP_VALUE.value--;

                    /*Minutes Reset Handling*/

                        /*Min Tens*/
                            if(EDIT_INDEX==Minutes_tens)
                            {
                                if(EDIT_TEMP_VALUE.value < 0)
                                {
                                    EDIT_TEMP_VALUE.value=5;
                                }
                            }
                        /*--------*/

                        /*Min Units*/
                            if(EDIT_INDEX==Minutes_unit)
                            {
                                if(EDIT_TEMP_VALUE.value>9)
                                {
                                    EDIT_TEMP_VALUE.value=0;
                                }
                                if(EDIT_TEMP_VALUE.value<0)
                                {
                                    EDIT_TEMP_VALUE.value=9;
                                }
                            }
                        /*---------*/

                    /*----------------------*/

                    /*Hour reset Handling*/

                        /*Hour Tens*/
                            if(EDIT_INDEX==Hours_tens)
                            {
                                if(EDIT_TEMP_VALUE.value <0)
                                {
                                    EDIT_TEMP_VALUE.value=2;
                                }
                            }
                        /*---------*/

                        /*Hour Units*/
                            if(EDIT_INDEX==Hours_unit)
                            {
                                if (Clock_Date_Digits[Hours_tens].value == 1)
                                {
                                    if(EDIT_TEMP_VALUE.value<0)
                                    {
                                        EDIT_TEMP_VALUE.value=9;
                                    }
                                }

                                if (Clock_Date_Digits[Hours_tens].value == 2)
                                {
                                    if(EDIT_TEMP_VALUE.value <0)
                                    {
                                        EDIT_TEMP_VALUE.value=3;
                                    }
                                }
                                if (Clock_Date_Digits[Hours_tens].value == 0)
                                {
                                    if(EDIT_TEMP_VALUE.value <0)
                                    {
                                        EDIT_TEMP_VALUE.value=9;
                                    }
                                }
                            }
                        /*----------*/
                        
                    /*-------------------*/

                    /*Days reset Handling*/

                        /*Day tens*/
                                if (EDIT_INDEX==Day_tens)
                                {
                                /*Reset to zero*/
                                    if ( Clock_Date_Digits[Day_unit].value > 0 )
                                    {
                                        /*Feb*/
                                            if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2)
                                            {
                                                if (IsLeapYear_edit(year))
                                                {
                                                    if (Clock_Date_Digits[Day_unit].value > 0 && EDIT_TEMP_VALUE.value <0 )
                                                    {
                                                        EDIT_TEMP_VALUE.value=2;
                                                    }
                                                    else if (Clock_Date_Digits[Day_unit].value ==0 && EDIT_TEMP_VALUE.value <0)
                                                    {
                                                        EDIT_TEMP_VALUE.value=1;
                                                    }
                                                }
                                                else
                                                {
                                                    if (Clock_Date_Digits[Day_unit].value <= 8 && EDIT_TEMP_VALUE.value <0 )
                                                    {
                                                        EDIT_TEMP_VALUE.value=2;
                                                    }
                                                    else if (Clock_Date_Digits[Day_unit].value ==0 && EDIT_TEMP_VALUE.value <0)
                                                    {
                                                        EDIT_TEMP_VALUE.value=1;
                                                    }
                                                }
                                            }
                                        /*---*/

                                        /* 30 day months*/
                                            else if (
                                                        Clock_Date_Digits[Month_unit].value == 4 ||
                                                        Clock_Date_Digits[Month_unit].value == 6 ||   
                                                        Clock_Date_Digits[Month_unit].value == 9 ||    
                                                        (Clock_Date_Digits[Month_unit].value == 1
                                                        &&
                                                        Clock_Date_Digits[Month_tens].value == 1)  
                                                    )

                                                    {
                                                        if (Clock_Date_Digits[Day_unit].value==0 && EDIT_TEMP_VALUE.value<0)
                                                        {
                                                            EDIT_TEMP_VALUE.value=3;
                                                        }
                                                        else if ( Clock_Date_Digits[Day_unit].value>0 && EDIT_TEMP_VALUE.value<0 )
                                                        {
                                                            EDIT_TEMP_VALUE.value=2;
                                                        }
                                                    }
                                        /*--------------*/

                                        /* 31 days month */
                                                else if ( (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value  == 1)   || 
                                                        Clock_Date_Digits[Month_unit].value  == 3                                                ||
                                                        Clock_Date_Digits[Month_unit].value  == 5                                                ||
                                                        Clock_Date_Digits[Month_unit].value  == 7                                                ||
                                                        Clock_Date_Digits[Month_unit].value  == 8                                                ||
                                                        (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 0)   ||                                     
                                                        (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 2)  
                                                        )
                                                        {
                                                            if ((Clock_Date_Digits[Day_unit].value > 1) && EDIT_TEMP_VALUE.value<0)
                                                            {
                                                                EDIT_TEMP_VALUE.value=2;
                                                            }
                                                            else if ((Clock_Date_Digits[Day_unit].value <= 1) && EDIT_TEMP_VALUE.value<0)
                                                            {
                                                                EDIT_TEMP_VALUE.value=3;
                                                            }
                                                        }
                                        /*---------------*/    
                                    }    
                                /*------------*/    

                                /*Reset to one*/
                                    else if ( Clock_Date_Digits[Day_unit].value == 0 )
                                    {
                                        /*Feb*/
                                            if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2)
                                            {
                                                if ( EDIT_TEMP_VALUE.value <1 )
                                                {
                                                    EDIT_TEMP_VALUE.value=2;
                                                }
                                            }
                                        /*---*/

                                        /* 30 day months*/
                                            else if (
                                                        Clock_Date_Digits[Month_unit].value == 4 ||
                                                        Clock_Date_Digits[Month_unit].value == 6 ||   
                                                        Clock_Date_Digits[Month_unit].value == 9 ||    
                                                        (Clock_Date_Digits[Month_unit].value == 1
                                                        &&
                                                        Clock_Date_Digits[Month_tens].value == 1)  
                                                    )

                                                    {
                                                        if (EDIT_TEMP_VALUE.value<1)
                                                        {
                                                            EDIT_TEMP_VALUE.value=3;
                                                        }
                                                        else if ( Clock_Date_Digits[Day_unit].value>0 && EDIT_TEMP_VALUE.value<1 )
                                                        {
                                                            EDIT_TEMP_VALUE.value=2;
                                                        }
                                                    }
                                        /*--------------*/

                                        /* 31 days month */
                                                else if ( (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value  == 1)   || 
                                                        Clock_Date_Digits[Month_unit].value  == 3                                                ||
                                                        Clock_Date_Digits[Month_unit].value  == 5                                                ||
                                                        Clock_Date_Digits[Month_unit].value  == 7                                                ||
                                                        Clock_Date_Digits[Month_unit].value  == 8                                                ||
                                                        (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 0)   ||                                     
                                                        (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value  == 2)  
                                                        )
                                                        {
                                                            if (EDIT_TEMP_VALUE.value<1)
                                                            {
                                                                EDIT_TEMP_VALUE.value=3;
                                                            }
                                                        }
                                        /*---------------*/    
                                    }   
                                /*------------*/
                                }
                        /*--------*/   

                        /*Days Units*/
                            if(EDIT_INDEX==Day_unit)
                            {

                                /*Reset to Zero*/
                                        if (Clock_Date_Digits[Day_tens].value == 0)
                                        {
                                            if(EDIT_TEMP_VALUE.value<1)
                                            {
                                                EDIT_TEMP_VALUE.value=9;
                                            }
                                        }

                                        if (Clock_Date_Digits[Day_tens].value > 0 && Clock_Date_Digits[Day_tens].value < 3 )
                                        {
                                            if(EDIT_TEMP_VALUE.value<0)
                                            {
                                                EDIT_TEMP_VALUE.value=9;
                                            }
                                        }
                                /*-------------*/

                                /*Reset to One*/
                                    if (Clock_Date_Digits[Month_tens].value == 0 && Clock_Date_Digits[Month_unit].value == 2)
                                    {
                                        if(IsLeapYear_edit(year))
                                        {
                                            if (Clock_Date_Digits[Day_tens].value == 0)
                                            {
                                                if(EDIT_TEMP_VALUE.value<1)
                                                {
                                                    EDIT_TEMP_VALUE.value = 9;
                                                }
                                            }
                                            else if (Clock_Date_Digits[Day_tens].value > 0)
                                            {
                                                if(EDIT_TEMP_VALUE.value<0)
                                                {
                                                    EDIT_TEMP_VALUE.value = 9;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            if ( (EDIT_TEMP_VALUE.value<0 && Clock_Date_Digits[Day_tens].value == 2))
                                            {
                                                EDIT_TEMP_VALUE.value =8;
                                            }
                                            else if((EDIT_TEMP_VALUE.value <0 && Clock_Date_Digits[Day_tens].value < 2))
                                            {
                                                EDIT_TEMP_VALUE.value = 9;
                                            }
                                        }
                                    }
                                    else if ((Clock_Date_Digits[Month_unit].value == 4 || Clock_Date_Digits[Month_unit].value == 6 || Clock_Date_Digits[Month_unit].value == 9 || (Clock_Date_Digits[Month_tens].value == 1 && Clock_Date_Digits[Month_unit].value==1))&& Clock_Date_Digits[Day_tens].value==3)
                                    {
                                        EDIT_TEMP_VALUE.value=0;
                                    }   
                                    else
                                    {   
                                        if (Clock_Date_Digits[Day_tens].value == 0)
                                        {
                                            if(EDIT_TEMP_VALUE.value<1)
                                            {
                                                EDIT_TEMP_VALUE.value=9;
                                            }
                                        }

                                        if (Clock_Date_Digits[Day_tens].value > 0 && Clock_Date_Digits[Day_tens].value < 3 )
                                        {
                                            if(EDIT_TEMP_VALUE.value<0)
                                            {
                                                EDIT_TEMP_VALUE.value=9;
                                            }
                                        }

                                        if(Clock_Date_Digits[Day_tens].value == 3)
                                        {
                                            if ( EDIT_TEMP_VALUE.value<0 )
                                            {
                                                EDIT_TEMP_VALUE.value=1;
                                            }
                                        }
                                    }  
                                /*------------*/

      
                            }       
                        /*----------*/   

                    /*-------------------*/

                    /*Month reset Handling*/

                        /*Month tens*/
                            if(EDIT_INDEX==Month_tens)
                            {
                                if(EDIT_TEMP_VALUE.value<0)
                                {
                                    EDIT_TEMP_VALUE.value=1;
                                }
                                if(Clock_Date_Digits[Month_unit].value==0)
                                {
                                    EDIT_TEMP_VALUE.value=1;
                                }
                                if(Clock_Date_Digits[Month_unit].value>2)
                                {
                                    EDIT_TEMP_VALUE.value=0;
                                }
                            }
                        /*----------*/    

                        /*Month unit*/
                            if(EDIT_INDEX == Month_unit)
                            {
                                if ( Clock_Date_Digits[Month_tens].value == 0 )
                                {
                                    if (EDIT_TEMP_VALUE.value <1)
                                    {
                                        EDIT_TEMP_VALUE.value=9;
                                    }
                                }

                                if ( Clock_Date_Digits[Month_tens].value == 1 )
                                {
                                    if (EDIT_TEMP_VALUE.value <0)
                                    {
                                        EDIT_TEMP_VALUE.value=2;
                                    }
                                }
                            }
                        /*----------*/

                    /*--------------------*/

                    /*Year reset Handling*/

                        /*year Thousand*/
                            if(EDIT_INDEX==Years_thousand)
                            {
                                if(EDIT_TEMP_VALUE.value<0)
                                {
                                    EDIT_TEMP_VALUE.value=9;
                                }
                            }
                        /*-------------*/

                        /*Year Hundred*/
                            if(EDIT_INDEX==Years_hundreds)
                            {
                                if(EDIT_TEMP_VALUE.value<0)
                                {
                                    EDIT_TEMP_VALUE.value=9;
                                }
                            }
                        /*------------*/

                        /*Year Tens*/
                            if(EDIT_INDEX==Years_tens)
                            {
                                if(EDIT_TEMP_VALUE.value<0)
                                {
                                    EDIT_TEMP_VALUE.value=9;
                                }
                            }
                        /*---------*/

                        /*Year Units*/
                            if(EDIT_INDEX==Years_unit)
                            {
                                if(EDIT_TEMP_VALUE.value<0)
                                {
                                    EDIT_TEMP_VALUE.value=9;
                                }
                            }
                        /*----------*/

                    /*-------------------*/
                LCD_enuWriteNumber(EDIT_TEMP_VALUE.value);
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
        Reset_StopWatch();
        ChangePrintState(Mode,DIGIT_STATE_PRINT);

    }
}


void Right_Switch_Pressed(void)
{
    if(Mode==Clock_Mode&&Operation_type==GeneralEdit_Operation)
    {
        EDIT_INDEX++;
        if(EDIT_INDEX>11)
        {
            EDIT_INDEX=0;
        }
    }
    else
    {

    }
   
    
}

void Left_Switch_Pressed(void)
{
    if(Mode==Clock_Mode&&Operation_type==GeneralEdit_Operation)
    {
        EDIT_INDEX--;
        if(EDIT_INDEX<0)
        {
            EDIT_INDEX=11;
        }
    }
    else
    {

    }
  
}