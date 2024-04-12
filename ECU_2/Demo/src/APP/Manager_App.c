/*Comments*/

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



    // #include"APP/Clock_Date_App.h"
    // #include"APP/Manager.h"
    // extern unit_Info_t Digits[15];

    // u8 Global_Mode = // Clock mode - Stop watch mode
    // u8 Edit_mode  = //General - Digit - Idle
    // //to keep track of cursor position in LCD
    // u8 Global_X_pos=0;
    // u8 Global_Y_Pos=0;
    // /**
    // enum lel switches IDs hanstkhdmha 3lshan ne enable el readings menhom aw la 3la hasb mhtagen wla la
    // */
    // enum{
    //     Up,
    //     Down,
    //     Left,
    //     Right,
    //     Edit,
    //     Mode,
    //     Ok
    // }Switches;
    // /**
    // array lel switch states
    // */
    // u8 Switches_mode[7]=
    // {
    //     [Up] =0,
    //     [Down]=0,
    //     [Left]=0,
    //     [Right]=0,
    //     [Edit]=1,
    //     [Mode]=1,
    //     [Ok]=0,
    // }

        // u8 Manager(u8 flag_for_array_to_be_retreived)
    // {
    //     u8 Error_Status=0;you 
    //     //read the mode switch button if previous reading != current reading toggle the global mode state
    //     return Error_Status;
    // } 

/*--------*/

/*-----------------Includes----------------*/
    #include "Std_Types.h"
    #include "HAL/LCD/HAL_LCD.h"
    #include "APP/Manager.h"
/*-----------------------------------------*/

/*-----------------Hashes------------------*/
    #define Manager_Periodicity 6
/*-----------------------------------------*/

/*-------------------------Types Defs-------------------------*/

    /*-------------General Mode---------*/

        /*-----------Typedefs-------------*/

            typedef enum
            {
                Clock,
                StopWatch
            }Mode_t;
            
            typedef enum
            {
                INIT,
                RUN
            }states_t;

            // typedef struct 
            // {
            //     uint8 COUNTER;
            //     CLOCK_OPTION_t OPTION;
            // }strMODE_t;

        /*--------------------------------*/

        /*------- Initializations -------*/
            Mode_t   Mode = Clock;
        /*-------------------------------*/
        
    /*-----------------------------------*/

    /*-------------CLOCK Mode ---------*/ 

        /*-----------Typedefs-------------*/
            typedef enum 
            {
                CLOCK_OPERATING_INIT,
                CLOCK_OPERATING_RUN
            }CLOCK_OPERATING_STATES_t;

            typedef enum 
            {
                PRINT_FIRST_LINE,
                WAIT_1,
                SET_CUTSOR_SECOND_LINE,
                PRINT_SECOND_LINE,
                WAIT_2,
                END   
            }CLOCK_OPERATING_INIT_STATES_t;

            typedef enum
            {
                SET_CURSOR,
                WRITE_NUMBER
            }CLOCK_OPERATING_RUN_STATES_t;

            typedef enum 
            {
                CLOCK_OPTION_OPERATING,
                CLOCK_OPTION_EDITING
            }CLOCK_OPTION_t;
        /*--------------------------------*/

        /*------- Initializations -------*/

            //static strMODE_t CLOCK={.COUNTER=0,.OPTION=CLOCK_OPTION_OPERATING};

            static uint8                         DIGIT_ITERATOR             = 0;

            static CLOCK_OPERATING_STATES_t      CLOCK_OPERATING_STATE      = CLOCK_OPERATING_INIT;
            static CLOCK_OPERATING_INIT_STATES_t CLOCK_OPERATING_INIT_STATE = PRINT_FIRST_LINE;
            static CLOCK_OPERATING_RUN_STATES_t  CLOCK_OPERATING_RUN_STATE  = SET_CURSOR;
            static CLOCK_OPTION_t                CLOCK_OPTION               = CLOCK_OPTION_OPERATING;
            static U8                            CLK_COUNTER                    = 0;
        
            static uint8 CLOCK_EDITING_STATE;
            
        /*-------------------------------*/

    /*---------------------------------*/

    /*-------------STOPWATCH Mode-------*/ 

        /*-----------Typedefs-------------*/
            typedef enum 
            {
                STOPWATCH_OPERATING_INIT,
                STOPWATCH_OPERATING_RUN
            }STOPWATCH_OPERATING_STATES_t;

            typedef enum 
            {
                STOPWATCH_PRINT_FIRST_LINE,
                STOPWATCH_WAIT_1,
                STOPWATCH_SET_CUTSOR_SECOND_LINE,
                STOPWATCH_PRINT_SECOND_LINE,
                STOPWATCH_WAIT_2,
                STOPWATCH_END   
            }STOPWATCH_OPERATING_INIT_STATES_t;

            typedef enum
            {
                STOPWATCH_SET_CURSOR,
                STOPWATCH_WRITE_NUMBER
            }STOPWATCH_OPERATING_RUN_STATES_t;

            typedef enum 
            {
                STOPWATCH_OPTION_OPERATING,
                STOPWATCH_OPTION_EDITING
            }STOPWATCH_OPTION_t;
        /*--------------------------------*/

        /*---------Initializations ------*/

            //static strMODE_t STOPWATCH={.COUNTER=0,.OPTION=STOPWATCH_OPTION_OPERATING};

            static uint8 STOPWATCH_DIGIT_ITERATOR=0;

            static STOPWATCH_OPERATING_STATES_t      STOPWATCH_OPERATING_STATE      = STOPWATCH_OPERATING_INIT;
            static STOPWATCH_OPERATING_INIT_STATES_t STOPWATCH_OPERATING_INIT_STATE = STOPWATCH_PRINT_FIRST_LINE;
            static STOPWATCH_OPERATING_RUN_STATES_t  STOPWATCH_OPERATING_RUN_STATE  = STOPWATCH_SET_CURSOR;

            static uint8 CLOCK_EDITING_STATE;
            
        /*-------------------------------*/

    /*----------------------------------*/

/*------------------------------------------------------------*/

/*-----------------------Static Functions---------------------*/
    static void CLOCK_OPERATING_INIT_THREAD(void);
    static void CLOCK_OPERATING_RUN_THREAD(void);

    static void CLOCK_THREAD(void);
    static void STOPWATCH_THREAD(void);
/*------------------------------------------------------------*/

/*-----------------------General Manager----------------------*/
    void Application_Runnable(void)
    {
        switch (Mode)
        {
            case Clock:
                CLOCK_THREAD();
            break;

            case StopWatch:
                STOPWATCH_THREAD();
            break;    
        }
    }
/*------------------------------------------------------------*/

/*-------------------------CLOCK_THREAD-----------------------*/

    /*------------CLOCK_THREAD_Manager------------*/
        static void CLOCK_THREAD(void)
        {
            CLK_COUNTER+=Manager_Periodicity;   

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
    /*--------------------------------------------*/

    /*-----------CLK_print_frame_thread-----------*/
        static void CLOCK_OPERATING_INIT_THREAD() //period = 4
        {          
            switch(CLOCK_OPERATING_INIT_STATE)
            {

                case PRINT_FIRST_LINE:

                    LCD_enuWriteStringAsync("CLOCK 31/12/2000",16); // (16 x 2) x 2 = 64 ms  -> 70 ms
                    
                    CLOCK_OPERATING_INIT_STATE=WAIT_1;

                break;

                case WAIT_1:
                    if(CLK_COUNTER>=70)
                    {
                        CLK_COUNTER=0;
                    CLOCK_OPERATING_INIT_STATE=SET_CUTSOR_SECOND_LINE;
                    }
                break;

                case SET_CUTSOR_SECOND_LINE:
                
                    LCD_SetCursorPosAsync(2, 1);             // ( 1 x 2) x 2 = 4 ms take care about lcd refresh rate 16 ms            
                
                    CLOCK_OPERATING_INIT_STATE = PRINT_SECOND_LINE;
                    CLK_COUNTER=0;
                break;

                case PRINT_SECOND_LINE:
                    LCD_enuWriteStringAsync("  23:59:55:100",14); // (11 x 2) x 2 = 44 ms   -> 60 ms
                    CLOCK_OPERATING_INIT_STATE=WAIT_2;
                break;

                case WAIT_2:
                    if(CLK_COUNTER>=70)
                    {
                        CLK_COUNTER=0;
                        CLOCK_OPERATING_INIT_STATE=END;
                    }
                break;

                case END:
                    CLOCK_OPERATING_STATE = CLOCK_OPERATING_RUN;
                    CLK_COUNTER=0;
                    CLOCK_OPERATING_INIT_STATE=PRINT_FIRST_LINE;
                break;  
            }
        }   
    /*--------------------------------------------*/

    /*----------------CLK_RUN_THREAD--------------*/
        static void CLOCK_OPERATING_RUN_THREAD(void)
        {
            if(Clock_Date_Digits[DIGIT_ITERATOR].digit_state == DIGIT_STATE_PRINT)
            {
                switch (CLOCK_OPERATING_RUN_STATE)
                {
                    case SET_CURSOR:
                        LCD_SetCursorPosAsync(Clock_Date_Digits[DIGIT_ITERATOR].x_pos, Clock_Date_Digits[DIGIT_ITERATOR].y_pos);
                        CLOCK_OPERATING_RUN_STATE = WRITE_NUMBER;
                    break;

                    case WRITE_NUMBER:
                        LCD_enuWriteNumber(Clock_Date_Digits[DIGIT_ITERATOR].value);

                        CLOCK_OPERATING_RUN_STATE=SET_CURSOR;

                        Clock_Date_Digits[DIGIT_ITERATOR].digit_state = DIGIT_STATE_NOT_PRINT;
                        
                        DIGIT_ITERATOR++;
                        if(DIGIT_ITERATOR > 14)
                        {
                            DIGIT_ITERATOR= 0;
                        }
                    break; 
                }   
            }
            else // DIGIT_STATE_NOT_PRINT
            {
                DIGIT_ITERATOR++;
                if(DIGIT_ITERATOR > 14)
                {
                    DIGIT_ITERATOR = 0;
                    CLOCK_OPERATING_RUN_STATE=SET_CURSOR; // Reset mystate when wrapping around
                }
            }
        }
    /*--------------------------------------------*/
    
/*------------------------------------------------------------*/

/*-----------------------STOPWATCH_THREAD---------------------*/
    static void STOPWATCH_THREAD()
    {

    }
/*------------------------------------------------------------*/

