
#include "APP/Switch_App.h"
#include "HAL/SWITCH/HAL_SWITCH.h"

#define PRESSED     1
#define NOT_PRESSED 0

uint32 MODE_CMD=NOT_PRESSED;
uint32 EDIT_CMD=NOT_PRESSED;
uint32 OK_CMD  =NOT_PRESSED;

void ControlSwitches_Runnable(void)
{
    HAL_SWITCH_enuGetSwitchState(SWITCH_MODE,&MODE_CMD);
    HAL_SWITCH_enuGetSwitchState(SWITCH_OK,&OK_CMD);


}
