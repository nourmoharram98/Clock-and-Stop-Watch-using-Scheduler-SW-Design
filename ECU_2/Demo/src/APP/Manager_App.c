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

 #define Manager_Periodicity 6


/********************************************************************/

/*--------------------------------Types Defs-------------------------*/
/*-------------------------------- COMMANDS -------------------------*/
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

/*---------------------------- CLOCK Mode -------------------------*/ 

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
    static void CLOCK_OPERATING_INIT_THREAD(void);
    static void CLOCK_OPERATING_RUN_THREAD(void);
    static void CLOCK_THREAD(void);

    static void STOP_WATCH_THREAD(void);
    static void STOP_WATCH_INIT_THREAD(void);
    static void STOP_WATCH_RUN_THREAD(void);
/*-------------------------------------------------------------------*/






/*----------------------------Global Variables-----------------------*/

/*----------------------------General Modes -------------------------*/

MODE_t    MODE                                = MODE_STOPWATCH;
COMMAND_t COMMAND                             = COMMAND_IDLE;
/*-------------------------------------------------------------------*/

/*---------------------------- CLOCK Mode -------------------------*/

static strMODE_t CLOCK={.COUNTER=0,.OPTION=CLOCK_OPTION_OPERATING};

static uint8 CLOCK_DIGIT_ITERATOR=0;

static CLOCK_OPERATING_STATES_t      CLOCK_OPERATING_STATE      = CLOCK_OPERATING_INIT;
static INIT_STATES_t CLOCK_OPERATING_INIT_STATE = PRINT_FIRST_LINE;
static OPERATING_RUN_STATES_t  CLOCK_OPERATING_RUN_STATE  = SET_CURSOR;

    
/*-------------------------------------------------------------------*/

/*-------------------------- STOP WATCH MODE ------------------------*/
static strMODE_t STOP_WATCH={.COUNTER=0,.OPTION=STOP_WATCH_OPTION_OPERATING};

static uint8 STOP_WATCH_DIGIT_ITERATOR=0;

static STOP_WATCH_OPERATING_STATES_t STOP_WATCH_OPERATING_STATE=STOP_WATCH_INIT;
static INIT_STATES_t STOP_WATCH_INIT_STATE=SET_CURSOR_FIRST_LINE;
static OPERATING_RUN_STATES_t STOP_WATCH_OPERATING_RUN_STATE=SET_CURSOR;

/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/

/*********************************************************************/
void Application_Runnable(void)
{

    switch (MODE)
    {
        case MODE_CLOCK:
            CLOCK_THREAD();
        break;

        case MODE_STOPWATCH:
            STOP_WATCH_THREAD();
        break;    

    }

}

/*----------------------------------CLOCK_THREAD---------------------------------*/

    /*--------------------------CLOCK_THREAD_Manager---------------------------*/
        static void CLOCK_THREAD(void)
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

                break;
            }     
        }
    /*-------------------------------------------------------------------------*/

    /*--------------------------CLK_print_frame_thread-------------------------*/
        static void CLOCK_OPERATING_INIT_THREAD() //period = 4
        {
             CLOCK.COUNTER+=Manager_Periodicity; 
            switch(CLOCK_OPERATING_INIT_STATE)
            {

                case PRINT_FIRST_LINE:

                    LCD_enuWriteStringAsync("CLOCK 31/12/2000",16); // (16 x 2) x 2 = 64 ms  -> 70 ms
                    
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
                    LCD_enuWriteStringAsync("  23:59:55:100",14); // (11 x 2) x 2 = 44 ms   -> 60 ms
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
                    CLOCK_OPERATING_INIT_STATE=PRINT_FIRST_LINE;
                break;  
            }

        }   
    
    /*-------------------------------------------------------------------------*/

    /*------------------------------CLK_RUN_THREAD-----------------------------*/


        static void CLOCK_OPERATING_RUN_THREAD(void)
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

    /*-------------------------------------------------------------------------*/
    
/*-------------------------------------------------------------------------------*/
static void STOP_WATCH_THREAD()
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
            switch(STOP_WATCH_INIT_STATE)
            {
                case SET_CURSOR_FIRST_LINE:
                    LCD_SetCursorPosAsync(1, 1);
                    STOP_WATCH_INIT_STATE=PRINT_FIRST_LINE;
                break;
                case PRINT_FIRST_LINE:

                    LCD_enuWriteStringAsync("STOP-WATCH",10); // (16 x 2) x 2 = 64 ms  -> 70 ms
                    
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
                    LCD_enuWriteStringAsync("  00:00:00:000",14); // (11 x 2) x 2 = 44 ms   -> 60 ms
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
}


