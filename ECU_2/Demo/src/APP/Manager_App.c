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

 #include "Std_Types.h"
 #include "HAL/LCD/LCD.h"
 #include "APP/Manager.h"
 #include "APP/Clock_Date_App.h"

 #define Manager_Periodicity 4


/********************************************************************/

/*------------------------------Types Defs---------------------------*/
    /**/
/*-------------------------------------------------------------------*/

/*------------------------------ COMMANDS ---------------------------*/
    typedef enum 
    {
        COMMAND_EDIT,
        COMMAND_OK  ,
        COMMAND_MODE,
        COMMAND_UP,
        COMMAND_DOWN,
        COMMAND_LEFT,
        COMMAND_RIGHT,
        COMMAND_IDLE
    }COMMAND_t;
/*-------------------------------------------------------------------*/

/*----------------------------General Modes -------------------------*/
    typedef enum
    {
        MODE_CLOCK,
        MODE_STOPWATCH
    }MODE_t;
    
    typedef struct 
    {
        uint8 COUNTER;
        uint8 OPTION; //running operating or editing
    }strMODE_t;

    typedef enum 
    {
        SET_CURSOR_FIRST_LINE,
        PRINT_FIRST_LINE,
        WAIT_1,
        SET_CURSOR_SECOND_LINE,
        PRINT_SECOND_LINE,
        WAIT_2,
        END   
    }INIT_STATES_t;
    
    typedef enum
    {
        SET_CURSOR,
        WRITE_NUMBER
    }OPERATING_RUN_STATES_t;
/*-------------------------------------------------------------------*/

/*---------------------------- CLOCK Mode ---------------------------*/ 
    typedef enum 
    {
        CLOCK_OPTION_OPERATING,
        CLOCK_OPTION_EDITING
    }CLOCK_OPTION_t;

    typedef enum 
    {
        CLOCK_OPERATING_INIT,
        CLOCK_OPERATING_RUN
    }CLOCK_OPERATING_STATES_t;

    typedef enum 
    {
        CLOCK_EDITING_GENERAL,
        CLOCK_EDITING_DIGIT
    }CLOCK_EDITING_STATE_t ;

    typedef enum 
    {
        GENERAL_EDIT_PRINT1,
        GENERAL_EDIT_PRINT2,
        GENERAL_EDIT_SET_CURSOR
    }GENERAL_EDIT_STATE_t;

    typedef struct
    {
        uint8 x_pos;
        uint8 y_pos;
        sint8 value;
    }EDIT_CURSOR_t;

    typedef enum 
    {
        DIGIT_EDIT_PRINT1,
        DIGIT_EDIT_PRINT2,
        DIGIT_EDIT_SET_CURSOR1,
        DIGIT_EDIT_PRINT3,
        DIGIT_EDIT_SET_CURSOR2
    }DIGIT_EDIT_STATE_t;
/*-------------------------------------------------------------------*/

/*-------------------------- STOP/WATCH Mode ------------------------*/ 
    typedef enum 
    {
        STOP_WATCH_OPTION_OPERATING
    }STOP_WATCH_OPTION_t;

    typedef enum 
    {
        STOP_WATCH_INIT,
        STOP_WATCH_RUN
    }STOP_WATCH_OPERATING_STATES_t;
/*-------------------------------------------------------------------*/

/*---------------------------Static Functions------------------------*/
    static void CLOCK_PROGRAM(void);//CLOCK MAIN PROGRAM
        static void CLOCK_OPERATING_PROCESS(void);//CLOCK OPERATING PROCESS
            static void CLOCK_OPERATING_INIT_THREAD(void);
            static void CLOCK_OPERATING_RUN_THREAD(void);
                static void CLOCK_OPERATING_RUN_PRINT_FUNCTION(void);
                static void CLOCK_OPERATING_RUN_MODE_CMD_FUNCTION(void);
                static void CLOCK_OPERATING_RUN_EDIT_CMD_FUNCTION(void);

        static void CLOCK_EDITING_PROCESS(void);
            static void CLOCK_EDITING_GENERAL_THREAD(void);
                static void CLOCK_EDITING_GENERAL_EDIT_CMD_FUNCTION(void);
                static void CLOCK_EDITING_GENERAL_OK_CMD_FUNCTION(void);
                static void CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION(void);
                static  void CLOCK_EDITING_GENERAL_EDIT_RIGHT_CMD_FUNCTION(void);
                static void CLOCK_EDITING_GENERAL_EDIT_LEFT_CMD_FUNCTION(void);

            static void CLOCK_EDITING_DIGIT_THREAD(void);
                static void CLOCK_EDITING_DIGIT_OK_CMD_FUNCTION(void);
                static void CLOCK_EDITING_DIGIT_EDIT_CMD_FUNCTION(void);
                static void CLOCK_EDITING_DIGIT_UP_CMD_FUNCTION(void);
                static void CLOCK_EDITING_DIGIT_DOWN_CMD_FUNCTION(void);


    static void STOP_WATCH_PROGRAM(void);
        static void STOP_WATCH_OPERATING_PROCESS(void);
            static void STOP_WATCH_INIT_THREAD(void);
            static void STOP_WATCH_RUN_THREAD(void);
/*-------------------------------------------------------------------*/

/*-------------------------leap Year Handling------------------------*/
    LCD_enuErrorStatus_t status;
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
/*-------------------------------------------------------------------*/

/*----------------------------Global Variables-----------------------*/
    /**/
/*-------------------------------------------------------------------*/

/*----------------------------General Modes -------------------------*/
    MODE_t    PROGRAM_MODE                        = MODE_CLOCK;
    COMMAND_t COMMAND                             = COMMAND_IDLE;
/*-------------------------------------------------------------------*/

/*---------------------------- CLOCK Mode ---------------------------*/
    static strMODE_t CLOCK={.COUNTER=0,.OPTION=CLOCK_OPTION_OPERATING};

    static uint8 CLOCK_DIGIT_ITERATOR=0;

    static CLOCK_OPERATING_STATES_t      CLOCK_OPERATING_STATE      = CLOCK_OPERATING_INIT;
    static INIT_STATES_t CLOCK_OPERATING_INIT_STATE = SET_CURSOR_FIRST_LINE;
    static OPERATING_RUN_STATES_t  CLOCK_OPERATING_RUN_STATE  = SET_CURSOR;

    static CLOCK_EDITING_STATE_t         CLOCK_EDITING_STATE=CLOCK_EDITING_GENERAL;
    static EDIT_CURSOR_t EDIT_TEMP_VALUE={.x_pos=1,.y_pos=7};
    static sint8 EDIT_INDEX=0;
    static GENERAL_EDIT_STATE_t GENERAL_EDIT_STATE=GENERAL_EDIT_PRINT1;
    static DIGIT_EDIT_STATE_t   DIGIT_EDIT_STATE=0;
/*-------------------------------------------------------------------*/

/*-------------------------- STOP WATCH MODE ------------------------*/
    static strMODE_t STOP_WATCH={.COUNTER=0,.OPTION=STOP_WATCH_OPTION_OPERATING};

    static uint8 STOP_WATCH_DIGIT_ITERATOR=0;

    static STOP_WATCH_OPERATING_STATES_t STOP_WATCH_OPERATING_STATE=STOP_WATCH_INIT;
    static INIT_STATES_t STOP_WATCH_INIT_STATE=SET_CURSOR_FIRST_LINE;
    static OPERATING_RUN_STATES_t STOP_WATCH_OPERATING_RUN_STATE=SET_CURSOR;
/*-------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
    void Application_Runnable(void)
    {
        switch (PROGRAM_MODE)
        {
            case MODE_CLOCK:
                CLOCK_PROGRAM();
            break;

            case MODE_STOPWATCH:
                STOP_WATCH_PROGRAM();
            break;    

        }
    }
/*--------------------------------------------------------------------*/

/**************************** CLOCK IMPLEMENTATION ************************************/

    static void CLOCK_PROGRAM(void)
    {
        switch (CLOCK.OPTION)
        {
            case CLOCK_OPTION_OPERATING:
                CLOCK_OPERATING_PROCESS();
            break;

            case CLOCK_OPTION_EDITING:
                CLOCK_EDITING_PROCESS();
            break;

            default:
                /*Nothing*/
            break;
        }   
    }
        
    static void CLOCK_OPERATING_PROCESS(void)
    {
        switch (CLOCK_OPERATING_STATE)
        {
            case CLOCK_OPERATING_INIT:
                CLOCK_OPERATING_INIT_THREAD();
            break;

            case CLOCK_OPERATING_RUN:
                CLOCK_OPERATING_RUN_THREAD();
            break;

            default:
                /*Nothing*/
            break;
        }              
    }
            
    static void CLOCK_OPERATING_INIT_THREAD(void) //period = 4
    {
        CLOCK.COUNTER+=Manager_Periodicity;

        for(int i=0;i<15;i++)
        {
            Clock_Date_Digits[i].digit_state=DIGIT_STATE_PRINT;
        }

        switch(CLOCK_OPERATING_INIT_STATE)
        {
            case SET_CURSOR_FIRST_LINE:
                LCD_ClearScreenAsync();
                CLOCK_OPERATING_INIT_STATE=PRINT_FIRST_LINE;
            break;

            case PRINT_FIRST_LINE:
                LCD_enuWriteStringAsync("CLOCK   /  /    ",16); // (16 x 2) x 2 = 64 ms  -> 70 m
                CLOCK_OPERATING_INIT_STATE=WAIT_1;
            break;

            case WAIT_1:
                if(CLOCK.COUNTER>=70)
                {
                    CLOCK.COUNTER=0;
                    CLOCK_OPERATING_INIT_STATE=SET_CURSOR_SECOND_LINE;
                }
            break;

            case SET_CURSOR_SECOND_LINE:
                LCD_SetCursorPosAsync(2, 1);             // ( 1 x 2) x 2 = 4 ms take care about lcd refresh rate 16 ms
                CLOCK_OPERATING_INIT_STATE = PRINT_SECOND_LINE;
                CLOCK.COUNTER=0;
            break;

            case PRINT_SECOND_LINE:
                LCD_enuWriteStringAsync("    :  :  :   ",14); // (11 x 2) x 2 = 44 ms   -> 60 ms
                CLOCK_OPERATING_INIT_STATE=WAIT_2;
            break;

            case WAIT_2:
                if(CLOCK.COUNTER>=70)
                {
                    CLOCK.COUNTER=0;
                    CLOCK_OPERATING_INIT_STATE=END;
                }
            break;

            case END:
                CLOCK_OPERATING_STATE = CLOCK_OPERATING_RUN;
                CLOCK.COUNTER=0;
                CLOCK_OPERATING_INIT_STATE=SET_CURSOR_FIRST_LINE;
            break;  
        }

    }   

    static void CLOCK_OPERATING_RUN_THREAD(void)
    {
        CLOCK_OPERATING_RUN_PRINT_FUNCTION   (); // FUNCTION USED TO PRINT ALL CHANGED DIGITS
        CLOCK_OPERATING_RUN_MODE_CMD_FUNCTION(); // FUNCTION USED TO CHANGE TO THE OTHER PROGRAM MODE
        CLOCK_OPERATING_RUN_EDIT_CMD_FUNCTION(); // FUNCTION USED TO CHANGE TO EDITING PROCESS
    }

    static void CLOCK_OPERATING_RUN_PRINT_FUNCTION(void)
    {
        if(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].digit_state == DIGIT_STATE_PRINT)
        {
            switch (CLOCK_OPERATING_RUN_STATE)
            {
                case SET_CURSOR:
                    LCD_SetCursorPosAsync(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].x_pos, Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].y_pos);
                    CLOCK_OPERATING_RUN_STATE = WRITE_NUMBER;
                break;

                case WRITE_NUMBER:
                    LCD_enuWriteNumber(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].value);

                    CLOCK_OPERATING_RUN_STATE=SET_CURSOR;

                    Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].digit_state = DIGIT_STATE_NOT_PRINT;
                    
                    CLOCK_DIGIT_ITERATOR++;
                    if(CLOCK_DIGIT_ITERATOR > 14)
                    {
                        CLOCK_DIGIT_ITERATOR= 0;
                    }
                break; 
            }   
        }
        else // DIGIT_STATE_NOT_PRINT
        {
            CLOCK_DIGIT_ITERATOR++;
            if(CLOCK_DIGIT_ITERATOR > 14)
            {
                CLOCK_DIGIT_ITERATOR = 0;
                CLOCK_OPERATING_RUN_STATE=SET_CURSOR; // Reset mystate when wrapping around
            }
        }
    }

    static void CLOCK_OPERATING_RUN_MODE_CMD_FUNCTION(void)
    {
        if(CMD[MODE_SWITCH_INDEX]==1)
        {
            PROGRAM_MODE=MODE_STOPWATCH;
            STOP_WATCH_OPERATING_STATE      = STOP_WATCH_INIT;
            STOP_WATCH_OPERATING_RUN_STATE  = SET_CURSOR;                

        }
    }

    static void CLOCK_OPERATING_RUN_EDIT_CMD_FUNCTION(void)
    {
        if(CMD[EDIT_SWITCH_INDEX]==1)
        {
            CLOCK.OPTION=CLOCK_OPTION_EDITING;
            EDIT_INDEX=0;
        }
    }

    static void CLOCK_EDITING_PROCESS(void)
    {
        switch (CLOCK_EDITING_STATE)
        {
            case CLOCK_EDITING_GENERAL:
                CLOCK_EDITING_GENERAL_THREAD();
            break;

            case CLOCK_EDITING_DIGIT:
                CLOCK_EDITING_DIGIT_THREAD();
            break;
                    
            default:
                /*Nothing*/
            break;
        }          
    }

    static void CLOCK_EDITING_GENERAL_THREAD(void)
    {
        CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION();

        if(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].digit_state == DIGIT_STATE_PRINT)
        {
            switch (GENERAL_EDIT_STATE)
            {
            case GENERAL_EDIT_PRINT1:
                LCD_SetCursorPosAsync(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].x_pos, Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].y_pos);
                GENERAL_EDIT_STATE=GENERAL_EDIT_PRINT2;
            break;

            case GENERAL_EDIT_PRINT2:
                LCD_enuWriteNumber(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].value);
                GENERAL_EDIT_STATE=GENERAL_EDIT_SET_CURSOR;
            break;

            case GENERAL_EDIT_SET_CURSOR://set edit cusror
                LCD_SetCursorPosAsync(EDIT_TEMP_VALUE.x_pos,EDIT_TEMP_VALUE.y_pos);
                GENERAL_EDIT_STATE=GENERAL_EDIT_PRINT1;
                Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].digit_state = DIGIT_STATE_NOT_PRINT;
                CLOCK_DIGIT_ITERATOR++;
                if(CLOCK_DIGIT_ITERATOR > 14)
                {
                    CLOCK_DIGIT_ITERATOR= 0;
                }               
            break;
            
            default:
                /*Nothing*/
            break;
            }                    
        }
        else
        {
            CLOCK_DIGIT_ITERATOR++;
            if(CLOCK_DIGIT_ITERATOR > 14)
            {
                CLOCK_DIGIT_ITERATOR = 0;
                GENERAL_EDIT_STATE=GENERAL_EDIT_PRINT1; // Reset mystate when wrapping around
            }
        }

        CLOCK_EDITING_GENERAL_OK_CMD_FUNCTION();
        CLOCK_EDITING_GENERAL_EDIT_CMD_FUNCTION();
        CLOCK_EDITING_GENERAL_EDIT_LEFT_CMD_FUNCTION();
        CLOCK_EDITING_GENERAL_EDIT_RIGHT_CMD_FUNCTION();
    }

    static void CLOCK_EDITING_GENERAL_OK_CMD_FUNCTION(void)
    {
        if(CMD[OK_SWITCH_INDEX]==1)
        {
            CLOCK.OPTION=CLOCK_OPTION_OPERATING;
        }   
    }

    static void CLOCK_EDITING_GENERAL_EDIT_CMD_FUNCTION(void)
    {
        if(CMD[EDIT_SWITCH_INDEX]==1)
        {
            CLOCK_EDITING_STATE=CLOCK_EDITING_DIGIT;

        }   
    } 

    static void CLOCK_EDITING_GENERAL_EDIT_RIGHT_CMD_FUNCTION(void)
    {
        if(CMD[LEFT_SWITCH_INDEX]==1)
        {
            EDIT_INDEX--;
            if(EDIT_INDEX<0)
            {
                EDIT_INDEX=11;
            }
        }   
    }

    static void CLOCK_EDITING_GENERAL_EDIT_LEFT_CMD_FUNCTION(void)
    {
        if(CMD[RIGHT_SWITCH_INDEX]==1)
        {
            EDIT_INDEX++;
            if(EDIT_INDEX>11)
            {
                EDIT_INDEX=0;
            }

        } 
    }

    static void CLOCK_EDITING_GENERAL_EDIT_CURSOR_SYNC_FUNCTION(void)
    {
        EDIT_TEMP_VALUE.x_pos=Clock_Date_Digits[EDIT_INDEX].x_pos;
        EDIT_TEMP_VALUE.y_pos=Clock_Date_Digits[EDIT_INDEX].y_pos;
        EDIT_TEMP_VALUE.value=Clock_Date_Digits[EDIT_INDEX].value;
    }
            
    static void CLOCK_EDITING_DIGIT_THREAD(void)
    {
        if(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].digit_state == DIGIT_STATE_PRINT)
        {
            Clock_Date_Digits[EDIT_INDEX].digit_state=DIGIT_STATE_NOT_PRINT;//disable printing the real value of the edit digit

            switch (DIGIT_EDIT_STATE)
            {
                case DIGIT_EDIT_PRINT1:
                    LCD_SetCursorPosAsync(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].x_pos, Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].y_pos);
                    DIGIT_EDIT_STATE=DIGIT_EDIT_PRINT2;
                break;

                case DIGIT_EDIT_PRINT2:
                    LCD_enuWriteNumber(Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].value);
                    DIGIT_EDIT_STATE=DIGIT_EDIT_SET_CURSOR1;
                break;

                case DIGIT_EDIT_SET_CURSOR1:
                    LCD_SetCursorPosAsync(EDIT_TEMP_VALUE.x_pos,EDIT_TEMP_VALUE.y_pos);
                    DIGIT_EDIT_STATE=DIGIT_EDIT_PRINT3;
                break;

                case DIGIT_EDIT_PRINT3:
                    LCD_enuWriteNumber(EDIT_TEMP_VALUE.value);
                    DIGIT_EDIT_STATE=DIGIT_EDIT_SET_CURSOR2;
                break;

                case DIGIT_EDIT_SET_CURSOR2:
                    LCD_SetCursorPosAsync(EDIT_TEMP_VALUE.x_pos,EDIT_TEMP_VALUE.y_pos);
                    Clock_Date_Digits[CLOCK_DIGIT_ITERATOR].digit_state = DIGIT_STATE_NOT_PRINT;
                    CLOCK_DIGIT_ITERATOR++;
                    if(CLOCK_DIGIT_ITERATOR > 14)
                    {
                        CLOCK_DIGIT_ITERATOR= 0;
                    }
                    DIGIT_EDIT_STATE=DIGIT_EDIT_PRINT1;

                break;

                default:
                    /*Nothing*/
                break;
            }
        }
        else
        {
            CLOCK_DIGIT_ITERATOR++;
            if(CLOCK_DIGIT_ITERATOR > 14)
            {
                CLOCK_DIGIT_ITERATOR = 0;
                GENERAL_EDIT_STATE=GENERAL_EDIT_PRINT1; // Reset mystate when wrapping around
            }
        }

        CLOCK_EDITING_DIGIT_UP_CMD_FUNCTION();
        CLOCK_EDITING_DIGIT_DOWN_CMD_FUNCTION();
        CLOCK_EDITING_DIGIT_OK_CMD_FUNCTION();
        CLOCK_EDITING_DIGIT_EDIT_CMD_FUNCTION();

    }

    static void CLOCK_EDITING_DIGIT_OK_CMD_FUNCTION(void)
    {
        if(CMD[OK_SWITCH_INDEX]==1)
        {
            Clock_Date_Digits[EDIT_INDEX].value=EDIT_TEMP_VALUE.value; //for saving

            for(int i=0;i<15;i++)
            {
                Clock_Date_Digits[i].digit_state=DIGIT_STATE_PRINT;
            }

            CLOCK_EDITING_STATE=CLOCK_EDITING_GENERAL;
        }   
    }
    
    static void CLOCK_EDITING_DIGIT_EDIT_CMD_FUNCTION(void)
    {
        if(CMD[EDIT_SWITCH_INDEX]==1)
        {
            CLOCK_EDITING_STATE=CLOCK_EDITING_GENERAL;

            Clock_Date_Digits[EDIT_INDEX].digit_state=DIGIT_STATE_PRINT;

        } 
    }

    static void CLOCK_EDITING_DIGIT_UP_CMD_FUNCTION(void)
    {
        if(CMD[UP_SWITCH_INDEX]==1)
        {
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

        }   

    }

    static void CLOCK_EDITING_DIGIT_DOWN_CMD_FUNCTION(void)
    {
        if(CMD[DOWN_SWITCH_INDEX]==1)
        {
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
                                if(EDIT_TEMP_VALUE.value<0)
                                {
                                    EDIT_TEMP_VALUE.value = 9;
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
        }   
    }  
/*------------------------------------------------------------------------------------*/    
                
/**************************** STOP WATCH IMPLEMENTATION *******************************/
    static void STOP_WATCH_PROGRAM()
    {
        STOP_WATCH_OPERATING_PROCESS();
    }

    static void STOP_WATCH_OPERATING_PROCESS(void)
    {
        switch (STOP_WATCH_OPERATING_STATE)
        {
            case STOP_WATCH_INIT:
                STOP_WATCH_INIT_THREAD();
                break;

            case STOP_WATCH_RUN:
                STOP_WATCH_RUN_THREAD();
                break;

            default:
                break;
        }             
    }
    
static void STOP_WATCH_INIT_THREAD(void)
{
    STOP_WATCH.COUNTER+=Manager_Periodicity;

    for(int i=0;i<7;i++)
    {
        Stop_Watch_Digits[i].digit_state=DIGIT_STATE_PRINT;
    }     

    switch(STOP_WATCH_INIT_STATE)
    {
        case SET_CURSOR_FIRST_LINE:
            LCD_ClearScreenAsync();
            STOP_WATCH_INIT_STATE=PRINT_FIRST_LINE;
        break;

        case PRINT_FIRST_LINE:
            LCD_enuWriteStringAsync("STOPWATCH",9); // (16 x 2) x 2 = 64 ms  -> 70 ms
            STOP_WATCH_INIT_STATE=WAIT_1;
        break;

        case WAIT_1:
            if(STOP_WATCH.COUNTER>=70)
            {
                STOP_WATCH.COUNTER=0;
                STOP_WATCH_INIT_STATE=SET_CURSOR_SECOND_LINE;
            }
        break;

        case SET_CURSOR_SECOND_LINE:
            LCD_SetCursorPosAsync(2, 1);             // ( 1 x 2) x 2 = 4 ms take care about lcd refresh rate 16 ms            
            STOP_WATCH_INIT_STATE = PRINT_SECOND_LINE;
            STOP_WATCH.COUNTER=0;
        break;

        case PRINT_SECOND_LINE:
            LCD_enuWriteStringAsync("    :  :  : 00",14); // (11 x 2) x 2 = 44 ms   -> 60 ms
            STOP_WATCH_INIT_STATE=WAIT_2;
        break;

        case WAIT_2:
            if(STOP_WATCH.COUNTER>=70)
            {
                STOP_WATCH.COUNTER=0;
                STOP_WATCH_INIT_STATE=END;
            }
        break;

        case END:
            STOP_WATCH_OPERATING_STATE = STOP_WATCH_RUN;
            STOP_WATCH.COUNTER=0;
            STOP_WATCH_INIT_STATE=SET_CURSOR_FIRST_LINE;
            STOP_WATCH_OPERATING_RUN_STATE=SET_CURSOR;
        break;  
    }
}

static void STOP_WATCH_RUN_THREAD(void)
{
    if(Stop_Watch_Digits[STOP_WATCH_DIGIT_ITERATOR].digit_state == DIGIT_STATE_PRINT)
    {
        switch (STOP_WATCH_OPERATING_RUN_STATE)
        {
            case SET_CURSOR:
                LCD_SetCursorPosAsync(Stop_Watch_Digits[STOP_WATCH_DIGIT_ITERATOR].x_pos, Stop_Watch_Digits[STOP_WATCH_DIGIT_ITERATOR].y_pos);
                STOP_WATCH_OPERATING_RUN_STATE = WRITE_NUMBER;
            break;

            case WRITE_NUMBER:
                LCD_enuWriteNumber(Stop_Watch_Digits[STOP_WATCH_DIGIT_ITERATOR].value);

                STOP_WATCH_OPERATING_RUN_STATE=SET_CURSOR;

                Stop_Watch_Digits[STOP_WATCH_DIGIT_ITERATOR].digit_state = DIGIT_STATE_NOT_PRINT;
                
                STOP_WATCH_DIGIT_ITERATOR++;
                if(STOP_WATCH_DIGIT_ITERATOR > 6)
                {
                    STOP_WATCH_DIGIT_ITERATOR= 0;
                }
            break; 
        }   
    }
    else // DIGIT_STATE_NOT_PRINT
    {
        STOP_WATCH_DIGIT_ITERATOR++;
        if(STOP_WATCH_DIGIT_ITERATOR > 6)
        {
            STOP_WATCH_DIGIT_ITERATOR = 0;
            STOP_WATCH_OPERATING_RUN_STATE=SET_CURSOR; // Reset mystate when wrapping around
        }
    }
    
    if(CMD[MODE_SWITCH_INDEX]==1)
    {
        PROGRAM_MODE=MODE_CLOCK;
        CLOCK_OPERATING_STATE      = CLOCK_OPERATING_INIT;
        CLOCK_OPERATING_RUN_STATE  = SET_CURSOR;                

    } 
    if(CMD[OK_SWITCH_INDEX]==1)
    {
        STOP_WATCH_OPTION = (STOP_WATCH_OPTION == 0) ? 1 : 0;
    }
    if(CMD[EDIT_SWITCH_INDEX]==1)
    {
    for(int i=0;i<7;i++)
    {
        Stop_Watch_Digits[i].value=0;
        Stop_Watch_Digits[i].digit_state=DIGIT_STATE_PRINT;
    }   
    }
}


