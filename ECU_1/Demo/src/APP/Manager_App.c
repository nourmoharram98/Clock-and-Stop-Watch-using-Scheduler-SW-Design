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

uint8 command;

typedef enum
{
    Clock,
    Stopwatch
}Option_t;

typedef enum
{
    Operation,
    Edit
}Mode_t;

typedef enum
{
    Operating,
    idle
}State_t;

typedef enum 
{
    First_Display,
    Consequentive_Display
}DisplayEventType_t

struct 
{
    DisplayEventType_t DisplayEventType;
    Option_t Option;
    Mode_t   Mode;
    State_t  State;
};

void Manager_Rnnable (void)
{
    switch (DisplayEventType)
    {
        case : First_Display

            LCD_WriteStringAsync("CLOCK 01/01/1990",16);
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

uint8 command;

typedef enum
{
    Clock,
    Stopwatch
}Option_t;

typedef enum
{
    Operation,
    Edit
}Mode_t;

typedef enum
{
    Operating,
    idle
}State_t;

typedef enum 
{
    First_Display,
    Consequentive_Display
}DisplayEventType_t

struct 
{
    DisplayEventType_t DisplayEventType;
    Option_t Option;
    Mode_t   Mode;
    State_t  State;
};

void Manager_Rnnable (void)
{
    switch (DisplayEventType)
    {
        case : First_Display

            LCD_WriteStringAsync("CLOCK 01/01/1990",16);
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

uint8 command;

typedef enum
{
    Clock,
    Stopwatch
}Option_t;

typedef enum
{
    Operation,
    Edit
}Mode_t;

typedef enum
{
    Operating,
    idle
}State_t;

typedef enum 
{
    First_Display,
    Consequentive_Display
}DisplayEventType_t

struct 
{
    DisplayEventType_t DisplayEventType;
    Option_t Option;
    Mode_t   Mode;
    State_t  State;
};

void Manager_Rnnable (void)
{
    switch (DisplayEventType)
    {
        case : First_Display

            LCD_WriteStringAsync("CLOCK 01/01/1990",16);
            LCD_WriteStringAsync(" Clock_Date_Data.Hours_Value+'0' ",16);
            
            LCD_SetCursorPosAsync(1,0);
            LCD_WriteStringAsync("  00:00:00:000  ",16);

        break;

        case : Consequentive_Display

            

        break;

        default :
        break;
    }
}
            LCD_SetCursorPosAsync(1,0);
            LCD_WriteStringAsync("  00:00:00:000  ",16);

        break;

        case : Consequentive_Display

            

        break;

        default :
        break;
    }
}
            LCD_SetCursorPosAsync(1,0);
            LCD_WriteStringAsync("  00:00:00:000  ",16);

        break;

        case : Consequentive_Display

            

        break;

        default :
        break;
    }
}