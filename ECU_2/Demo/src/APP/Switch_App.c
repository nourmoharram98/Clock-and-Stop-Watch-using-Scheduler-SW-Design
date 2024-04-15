
#include "APP/Switch_App.h"
#include "HAL/SWITCH/HAL_SWITCH.h"



uint32 switch_states[NUM_SWITCHES] = {SWITCH_RELEASED}; // Array to store switch states
uint32 previous_switch_states[NUM_SWITCHES] = {SWITCH_RELEASED}; // Array to store previous switch states


uint32 CMD[NUM_SWITCHES]={CMD_RESET};

void ControlSwitches_Runnable(void)
{
    // Get the current state of each switch
    HAL_SWITCH_enuGetSwitchState (SWITCH_MODE  , &switch_states[MODE_SWITCH_INDEX] );
    HAL_SWITCH_enuGetSwitchState (SWITCH_OK    , &switch_states[OK_SWITCH_INDEX]   );
    HAL_SWITCH_enuGetSwitchState (SWITCH_EDIT  , &switch_states[EDIT_SWITCH_INDEX] );

    HAL_SWITCH_enuGetSwitchState (SWITCH_UP    , &switch_states[UP_SWITCH_INDEX]   );
    HAL_SWITCH_enuGetSwitchState (SWITCH_DOWN  , &switch_states[DOWN_SWITCH_INDEX] );
    HAL_SWITCH_enuGetSwitchState (SWITCH_LEFT  , &switch_states[LEFT_SWITCH_INDEX] );
    HAL_SWITCH_enuGetSwitchState (SWITCH_RIGHT , &switch_states[RIGHT_SWITCH_INDEX]);

    for(int i=0;i<NUM_SWITCHES;i++)
    {
        if(switch_states[i]==SWITCH_PRESSED)
        {
            previous_switch_states[i]=switch_states[i];
        }
        else if(previous_switch_states[i]==SWITCH_PRESSED && switch_states[i]==SWITCH_RELEASED)
        {
            CMD[i]=CMD_SET;
            previous_switch_states[i]=SWITCH_RELEASED;
        }
        else if (previous_switch_states[i]==SWITCH_RELEASED && switch_states[i]==SWITCH_RELEASED)
        {
            CMD[i]=CMD_RESET;
        }
    }
}

