#include "./HAL/LED/LED.h"

#define RED     0 
#define YELLOW  1
#define GREEN   2

#define PERIODICITY 100

#define RED_TIME_PERIOD     2000
#define YELLOW_TIME_PERIOD  700
#define GREEN_TIME_PERIOD   900

#define Cycle_Time          4300

void Runnable_TrafficLight_StateMachine (void)
{
    static U8 LOC_State     = RED;
    static U8 Prev_State    = RED;
    static U32 LOC_Counter  = 0;

    LOC_Counter += PERIODICITY;

    switch (LOC_State) 
    {
        case RED:

            if ( Prev_State == YELLOW && LOC_Counter >= Cycle_Time )
            {
                LOC_Counter = 0;
            }

            LED_SetStatus ( LED_RED    , LED_SET_ON  );
            LED_SetStatus ( LED_YELLOW , LED_SET_OFF );
            LED_SetStatus ( LED_GREEN  , LED_SET_OFF );

            if ( LOC_Counter >= RED_TIME_PERIOD )
            {
                LOC_State = YELLOW;
                Prev_State = RED;
            }

        break;  

        case YELLOW:
            LED_SetStatus ( LED_RED    , LED_SET_OFF  );
            LED_SetStatus ( LED_YELLOW , LED_SET_ON   );
            LED_SetStatus ( LED_GREEN  , LED_SET_OFF  );

            if ( LOC_Counter == (RED_TIME_PERIOD + YELLOW_TIME_PERIOD) || LOC_Counter >= Cycle_Time )
            {
                LOC_State = (Prev_State == RED) ? GREEN : RED;
                Prev_State = YELLOW;
            }
        break;  

        case GREEN:
            LED_SetStatus ( LED_RED    , LED_SET_OFF  );
            LED_SetStatus ( LED_YELLOW , LED_SET_OFF  );
            LED_SetStatus ( LED_GREEN  , LED_SET_ON   );

            if ( LOC_Counter >= RED_TIME_PERIOD + YELLOW_TIME_PERIOD + GREEN_TIME_PERIOD )
            {
                LOC_State = YELLOW;
                Prev_State = GREEN;
            }
        break;    

        default:

        break;
    }   
}