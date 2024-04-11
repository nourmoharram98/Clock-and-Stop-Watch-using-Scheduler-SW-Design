// #include "Std_Types.h"
// #include "Bit_Mask.h"
// #include "HAL/LCD/HAL_LCD_CONFIG.h"
// #include"HAL/LCD/HAL_LCD.h"
// #include"MCAL/GPIO/GPIO.h"

// #define LCD_PORTA  	(void *)(0x40020000)
// #define LCD_PORTB	(void *)(0x40020400)
// #define LCD_PORTC  	(void *)(0x40020800)
// #define LCD_PORTD 	(void *)(0x40020C00)
// #define LCD_PORTE 	(void *)(0x40021000)
// #define LCD_PORTH	(void *)(0x40021C00)

// #define LCD_PIN0 	0x00000000
// #define LCD_PIN1 	0x00000001
// #define LCD_PIN2 	0x00000002
// #define LCD_PIN3 	0x00000003
// #define LCD_PIN4 	0x00000004
// #define LCD_PIN5 	0x00000005
// #define LCD_PIN6 	0x00000006
// #define LCD_PIN7 	0x00000007
// #define LCD_PIN8 	0x00000008
// #define LCD_PIN9	0x00000009
// #define LCD_PIN10 	0x0000000A
// #define LCD_PIN11 	0x0000000B
// #define LCD_PIN12 	0x0000000C
// #define LCD_PIN13 	0x0000000D
// #define LCD_PIN14 	0x0000000E
// #define LCD_PIN15 	0x0000000F

// // Define configurations for LCDs
// const HAL_LCD_Configuration_t LCD_Configurations[NumberOfLCDs] = {
//     [LCD_One] = {
//         .config = {.LCD_PIN_NUM_MODE=LCD_Eight_BitMode},  
//         .pins = {
//             [D0].Pin = LCD_PIN0,
//             [D1].Pin = LCD_PIN1,
//             [D2].Pin = LCD_PIN2,
//             [D3].Pin = LCD_PIN3,
//             [D4].Pin = LCD_PIN4,
//             [D5].Pin = LCD_PIN5,
//             [D6].Pin = LCD_PIN6,
//             [D7].Pin = LCD_PIN7,
//             [RS_8BIT].Pin = LCD_PIN8,
//             [RW_8BIT].Pin = LCD_PIN9,
//             [EN_8BIT].Pin = LCD_PIN10,
//             // Assign port addresses for LCD One (connected to Port A)
//             [D0].Port = LCD_PORTA,
//             [D1].Port = LCD_PORTA,
//             [D2].Port = LCD_PORTA,
//             [D3].Port = LCD_PORTA,
//             [D4].Port = LCD_PORTA,
//             [D5].Port = LCD_PORTA,
//             [D6].Port = LCD_PORTA,
//             [D7].Port = LCD_PORTA,
//             [RS_8BIT].Port = LCD_PORTA,
//             [RW_8BIT].Port = LCD_PORTA,
//             [EN_8BIT].Port = LCD_PORTA,
//         }
//     },
//     /*
//     [LCD_Two] = {
//         .config = LCD_Four_BitMode,  // Use Four_BitMode enum directly
//         .pins = {
//             [D4].Pin = LCD_PIN4,
//             [D5].Pin = LCD_PIN5,
//             [D6].Pin = LCD_PIN6,
//             [D7].Pin = LCD_PIN7,
//             [RS_4BIT].Pin = LCD_PIN8,
//             [RW_4BIT].Pin = LCD_PIN9,
//             [EN_4BIT].Pin = LCD_PIN10,
//             // Assign port addresses for LCD Two (connected to Port B)
//             [D4].Port = LCD_PORTB,
//             [D5].Port = LCD_PORTB,
//             [D6].Port = LCD_PORTB,
//             [D7].Port = LCD_PORTB,
//             [RS_4BIT].Port = LCD_PORTB,
//             [RW_4BIT].Port = LCD_PORTB,
//             [EN_4BIT].Port = LCD_PORTB,
//         }
//     }
//     */
// };