#include "MCAL/GPIO/GPIO.h"
#include "HAL/LED/LED.h"

extern const LED_strLEDConfig_t LED_arrOfStrLEDs[NUM_OF_LEDS];

GPIO_enuErrorStatus LED_Init ()
{
	GPIO_enuErrorStatus LOC_Status = GPIO_enuNotOK;
	GPIO_Pin_t Led;

	Led.Mode  = OUTPUT_PP_FLOAT;
	Led.Speed = GPIO_SPEED_HIGH;

	for ( U8 idx = 0 ; idx < NUM_OF_LEDS ; idx++ )
	{
		Led.Pin         = LED_arrOfStrLEDs[idx].Pin;
		Led.Port            = LED_arrOfStrLEDs[idx].Port;
		LOC_Status          = GPIO_Init (&Led);

		if (LOC_Status == GPIO_enuOK)
		{
			LOC_Status = GPIO_SetPinValue(LED_arrOfStrLEDs[idx].Port , LED_arrOfStrLEDs[idx].Pin  , LED_arrOfStrLEDs[idx].Connection ^ LED_arrOfStrLEDs[idx].Init_Value);
		}
	}
	return LOC_Status;
}

GPIO_enuErrorStatus LED_SetStatus ( LEDs_t LED_NAME , U32 LED_STATUS )
{
	GPIO_enuErrorStatus LOC_Status = GPIO_enuNotOK;

	LOC_Status = GPIO_SetPinValue(LED_arrOfStrLEDs[LED_NAME].Port , LED_arrOfStrLEDs[LED_NAME].Pin  , LED_arrOfStrLEDs[LED_NAME].Connection ^ LED_STATUS);

	return LOC_Status;
};