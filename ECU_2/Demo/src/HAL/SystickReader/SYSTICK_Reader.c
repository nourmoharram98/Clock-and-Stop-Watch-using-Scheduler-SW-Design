#include"HAL/SystickReader/SYSTICK_Reader.h"
#include"MCAL/SYSTICK/SYSTICK.h"

u32 Runnable_Execution_time(void)
{
    u32 counter=Systick_STKVAL();
    return counter;
}