/**
 * @file Stop_Watch_App.c
 * @author your name (you@domain.com)
 * @brief implementation of stop watch mode and global variables, arrays and flags required for the mode
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

struct  
{
    U8 Hours_Value;
    U8 Minuts_Value;
    U8 Seconds_Value;
    U8 Deci_Value;

}Stopwatch_Data;

void Stopwatch_Runnable(void)
{
    extern LCD;
    
    if(LCD.Stopwatch.state=Operation)
    {
        Stopwatch_Data.Deci_Value++;
        
        if (Stopwatch_Data.Deci_Value > 9)
        {
            Stopwatch_Data.Deci_Value = 0;
            Stopwatch_Data.Seconds_Value++;
        }

        if (Stopwatch_Data.Seconds_Value > 59)
        {
            Stopwatch_Data.Seconds_Value = 0;
            Stopwatch_Data.Minuts_Value++;
        }
        
        if (Stopwatch_Data.Minuts_Value > 59)
        {
            Stopwatch_Data.Minuts_Value = 0;
            Stopwatch_Data.Hours_Value++;
        }
        
        if (Stopwatch_Data.Hours_Value > 99)
        {
            Stopwatch_Data.Deci_Value    = 0;
            Stopwatch_Data.Seconds_Value = 0;
            Stopwatch_Data.Minuts_Value  = 0;
            Stopwatch_Data.Hours_Value   = 0;
        }
    }
}