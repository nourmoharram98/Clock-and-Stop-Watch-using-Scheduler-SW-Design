#include "MCAL/GPIO/GPIO.h"

#include "HAL/LCD/LCD_Config.h"

const LCD_strLCDPinConfig_t LCD_strLCDpinConfig[LCD_PINs]=
{

    /*
    [NAME_OF_LED]= 
    {
        .Port=                 Select value from the following       ( GPIO_PORT_A , GPIO_PORT_B , GPIO_PORT_C )
        .Pin=                  Select value from the following       ( GPIO_PIN_0 , GPIO_PIN_1 , GPIO_PIN_2 , ... , GPIO_PIN_15 ) 
    }
    */

     [RS]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_8
    }

 /*-------*/
 ,
 /*RW pin*/
    [RW]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_9
    }
 /*-------*/
 ,
 /*ET pin*/
    [ET]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_10
    }
 /*-------*/
 ,
 /*Data 1st pin*/
    [LCD_D0]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_0
    }
 /*-------*/ 
 ,
 /*Data 2nd pin*/
    [LCD_D1]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_1
    }
 /*-------*/ 
 ,
 /*Data 3rd pin*/
    [LCD_D2]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_2
    }
 /*-------*/
 ,
 /*Data 4th pin*/
    [LCD_D3]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_3
    }
 /*-------*/ 
 ,
 /*Data 5th pin*/
    [LCD_D4]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_4
    }
 /*-------*/ 
 ,
 /*Data 6th pin*/
    [LCD_D5]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_5
    }
 /*-------*/ 
 ,
 /*Data 7th pin*/
    [LCD_D6]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_6
    }
 /*-------*/
 ,
 /*Data 8th pin*/
    [LCD_D7]=
    {
        .LCD_PortNumber = GPIO_PORT_A,
        .LCD_PinNumber  = GPIO_PIN_7
    }
 /*-------*/ 
};