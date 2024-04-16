#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

    #include "MCAL/GPIO/GPIO.h"

    /* ----------------Struct for the LCDPin Configuration--------------- */
        typedef struct
        {  
            void *LCD_PortNumber;     //Used in Config function
            U32   LCD_PinNumber;      //Used in Config function                        
        }LCD_strLCDPinConfig_t;
    /* ------------------------------------------------------------------ */

    /* -----------------Struct for the LCD Configuration----------------- */
        typedef struct
        {  
            LCD_strLCDPinConfig_t         LCD_strLCDPinConfig;     //Used in Config function                    
        }LCD_strLCDConfig_t;
    /* ------------------------------------------------------------------ */

    /*--------------------------UserDev Choices---------------------------*/

        /*-----------How many LCDs-----------*/
          #define  NUM_OF_LCDs  1
        /*-----------------------------------*/

        /*--------Curser Move Direction------*/

            /*Choose the direction of curser movement*/
                //Choices:
                //0 -> to the right                     
                //1 -> to the left
            /*---------------------------------------*/

            #define Curser_MovDir 1
        /*-----------------------------------*/

        /*-------------DisplayShift----------*/

            /*Choose the direction of Display Shift*/
                //Choices:
                //0 -> to the right                     
                //1 -> to the left
            /*-------------------------------------*/

            #define DisplayShiftDirection 0
        /*-----------------------------------*/

        /*---------Entire Data Display-------*/

            /*Choose Display state*/
                //Choices:
                //0 -> off                     
                //1 -> on
            /*--------------------*/        

            #define Display_State 1
        /*-----------------------------------*/

        /*-----------Curser Display----------*/

            /*Choose Curser Display state*/
                //Choices:
                //0 -> off                     
                //1 -> on
            /*---------------------------*/        

            #define Curser_DisplayState 1
        /*-----------------------------------*/   

        /*----------Curser Blinking----------*/

            /*Choose Curser Blinking state*/
                //Choices:
                //0 -> off                     
                //1 -> on
            /*---------------------------*/        

            #define Curser_BlinkingState 1
        /*-----------------------------------*/  

        /*------Curser or Display Shift------*/

            /*Curser or Curser+Display*/
                //Choices:
                //0 -> Curser                     
                //1 -> Curser+Display
            /*---------------------------*/  

                #define ShiftedElement 1

            /*Shifting direction*/
                //Choices:
                //0 -> Left                    
                //1 -> Right
            /*---------------------------*/  

                #define Shifting_Direction 1

        /*-----------------------------------*/

        /*--------Interface Data Length------*/

            /*Choose Interface Data Length*/
                //Choices:
                //0 -> 4-bit                     
                //1 -> 8-bit
            /*---------------------------*/        

            #define Interface_DataLength 1
        /*-----------------------------------*/    

        /*----------Number Of Lines----------*/

            /*Choose Number of lines*/
                //Choices:
                //0 -> 1 line                     
                //1 -> 2 lines
            /*---------------------------*/        

            #define Lines_Number 1
        /*-----------------------------------*/  

        /*-------------Font Size-------------*/

            /*Choose Font Size*/
                //Choices:
                //0 -> small size                     
                //1 -> BIG SIZE
            /*---------------------------*/        

            #define Font_Size 0
        /*-----------------------------------*/  

    /*--------------------------------------------------------------------*/
	
	/*-----------------------------Shifting Dir----------------------------*/
		typedef enum 
		{
			Left,
			Right
		}ShiftDir_t;
	/*---------------------------------------------------------------------*/

    /*-------------------------------LCDs List--------------------------- */
        #define LCD_1     0
    /*--------------------------------------------------------------------*/    

    /*--------------------------- LCD Pins List-------------------------- */

        #define RS       0
        #define RW       1
        #define ET       2

        #define LCD_D0   3
        #define LCD_D1   4
        #define LCD_D2   5
        #define LCD_D3   6
        #define LCD_D4   7
        #define LCD_D5   8
        #define LCD_D6   9
        #define LCD_D7   10  

        #if (Interface_DataLength == 1)
            #define LCD_PINs 11
        #elif (Interface_DataLength == 0)
            #define LCD_PINs 7
        #endif
    /*--------------------------------------------------------------------*/    

    /*------------------Concate for UserDev Configurations----------------*/

        #define CONCAT( bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0 )          CONCAT_Helper( bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0)
        #define CONCAT_Helper( bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0 )   0b##bit7##bit6##bit5##bit4##bit3##bit2##bit1##bit0
            
    /*--------------------------------------------------------------------*/

  #endif  