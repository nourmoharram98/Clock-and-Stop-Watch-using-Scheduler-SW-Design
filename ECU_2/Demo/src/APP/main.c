#include "MCAL/RCC/STM32F401cc_HAL_RCC.h"

#include "MCAL/GPIO/GPIO.h"

#include "HAL/LED/LED.h"

#include "HAL/LCD/HAL_LCD.h"

#include "HAL/SWITCH/HAL_SWITCH.h"

#include "MCAL/NVIC/NVIC.h"

#include "MCAL/SYSTICK/SYSTICK.h"

#include "MCAL/USART/STM32F401cc_MCAL_USART.h"

#include "SERVICE/SCHEDULER/SCHED.h"

#define RCC_TEST                0
#define GPIO_TEST               0
#define LED_SWITCH_TEST         0
#define NVIC_TEST               0
#define SYSTICK_TEST            0
#define SCHEDULER_TEST          0
#define SCHEDULER_LCD_TEST      0
#define DEMO_TEST_1             1 


#if RCC_TEST
int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_EnableClock				( CLK_PLL );
    RCC_DisableClock            ( CLK_PLL );
    RCC_EnableClock             ( CLK_PLLI2S );

    RCC_DisableClock            ( CLK_PLLI2S );

    RCC_PLL_SRCConfig		    ( PLLSRC_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_ReadStatusClock			( READ_HSI_STATUS );
    RCC_ReadStatusClock			( READ_HSE_STATUS );
    RCC_ReadStatusClock			( READ_PLL_STATUS );
    RCC_ReadStatusClock			( READ_PLLI2S_STATUS );

    RCC_SetSYSCLK 				( SYSCLK_HSE );

    RCC_AHB_PREscaler			( AHB_PRE_2 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

    RCC_ResetAHB1Peripheral	    ( AHB1ENR_GPOIA	);

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

    RCC_SetAHB2Peripheral		( AHB2ENR_OTGFS );

    RCC_ResetAHB2Peripheral      ( AHB2ENR_OTGFS );

    RCC_SetAHB2Peripheral		( AHB2ENR_OTGFS );

    RCC_APB1_PREscaler			( APB_PRE1_2 );

    RCC_SetAPB1Peripheral		( APB1ENR_PWR );

    RCC_ResetAPB1Peripheral		( APB1ENR_PWR );

    RCC_APB2_PREscaler			( APB_PRE2_2 );

    RCC_SetAPB2Peripheral		( APB2ENR_ADC1);

    RCC_ResetAPB2Peripheral		( APB2ENR_ADC1);

    RCC_SetAPB2Peripheral		( APB2ENR_ADC1);


    while (1)
    {

    };

    return 0;
}
#endif

#if GPIO_TEST
int main ()
{

    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_SetSYSCLK 				( SYSCLK_PLL );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);


    GPIO_Pin_t Led;

    Led.Port = GPIO_PORT_A;
    Led.Pin_num = GPIO_PIN_0;
    Led.Pin_Mode = GPIO_MODE_OP_PP_PD;
    Led.Pin_Speed = GPIO_SPEED_MED;

    GPIO_Init( &Led );

    GPIO_Pin_t Switch;

    Switch.Port = GPIO_PORT_A;
    Switch.Pin_num = GPIO_PIN_7;
    Switch.Pin_Mode = GPIO_MODE_IN_PD;
    Switch.Pin_Speed = GPIO_SPEED_MED; // don't care in this case asln
    GPIO_Init( &Switch );    

    U8 Switch_Status;

    while (1)
    {
        
        GPIO_Get_PinValue(GPIO_PORT_A, GPIO_PIN_7, &Switch_Status);

        if (Switch_Status)
        {
            GPIO_Set_PinValue( GPIO_PORT_A , GPIO_PIN_0 , GPIO_STATE_SET);
        }
        else 
        {
            GPIO_Set_PinValue( GPIO_PORT_A , GPIO_PIN_0 , GPIO_STATE_RESET);
        }
    }


}
#endif

# if LED_SWITCH_TEST
int main ()
{

    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_SetSYSCLK 				( SYSCLK_PLL );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

    LED_Init (  );

    SWITCH_Init(  );

    U8 Switch_State;
    Switch_State =0;

    while(1)
    {
        
        SWITCH_ReadState ( SWITCH_Number_0 , &Switch_State );
        if ( Switch_State )
        {
            LED_SetStatus( LED_Number_0 , LED_SET_OFF );
        }
        else 
        {
            LED_SetStatus( LED_Number_0 , LED_SET_ON );
        }

    }
}
#endif

#if NVIC_TEST

void USART1_IRQHandler(void)
{
    LED_Init (  );
}

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_SetSYSCLK 				( SYSCLK_PLL );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

/**
 * NVIC TESTING
*/
    NVIC_CFG_SetSubGroup(TWO_SUBGROUP);

    NVIC_CFG_SetPriority (NVIC_USART1_IRQn, 1,0);

    NVIC_CTRL_EnableIRQ(NVIC_USART1_IRQn);

    NVIC_CTRL_SetIRQPending(NVIC_USART1_IRQn);

    for(;;)
    {

    }
    
}
#endif

#if SYSTICK_TEST

#define OFF 0
#define ON  1

void Toggle_Led(void)
{
    static U8 status =OFF;
    if(status==OFF)
    {
        LED_SetStatus(LED_Number_0,ON);
    }
    else 
    {
        LED_SetStatus(LED_Number_0,OFF);
    }
    status = ~status;
}

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSI );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

     LED_Init (  );

/**
 * SysTick TESTING
*/
    SYSTICK_Init                (  );
 
    SYSTICK_SetTickTimeMS       ( 1000 );

    Systick_vidRegisterCallBack ( Toggle_Led );

    SYSTICK_Start               ( Periodic );   

    for(;;)
    {

    }
    
}

#endif

#if SCHEDULER_TEST

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_SetSYSCLK 				( SYSCLK_HSI );
    RCC_AHB_PREscaler			( AHB_PRE_1 );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA );

    SysTick_SetClockSource      (SysTick_CLOCK_SOURCE_AHB_8);
    SysTick_SetCurrentVal       (0);
    SysTick_EnableInterrupt     ( );

    LED_Init                    ( );

    SCHED_Init();

    SCHED_Start();
}

#endif

#if SCHEDULER_LCD_TEST

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_SetSYSCLK 				( SYSCLK_HSI );
    RCC_AHB_PREscaler			( AHB_PRE_1 );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA );

    LCD_InitAsync();  

    Schedular_Init              (  );

    Schedular_Start             (  );
}

#endif

#if DEMO_TEST_1
int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_SetSYSCLK 				( SYSCLK_HSI );
    RCC_AHB_PREscaler			( AHB_PRE_1 );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIB );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIC );
    RCC_SetAPB2Peripheral       (APB2ENR_USART1);
    SysTick_SetClockSource(SysTick_CLOCK_SOURCE_AHB_8);
    SysTick_SetCurrentVal(0);
    SysTick_EnableInterrupt();
    HAL_SWITCH_Init();
    LCD_InitAsync();
    LED_Init();
    USART_Pins_Init();
    NVIC_EnableIRQ(USART1_IRQn);
    USART_Init();
    u8 x;
    USART_Request_t Requestone={
        .length=1,
        .PtrtoBuffer=&x,
        .USART_ID=USART1
    };
   // USART_SendByte(Requestone);
   //USART_SendByteAsynchZC(Requestone);
      USART_ReceiveByteAsynchZC(Requestone);
//     LED_SetStatus(Nour_LED,LED_SET_ON);

//    while(1)
//    {
//         if(x=='M')
//         {
//             LED_SetStatus(Nour_LED,LED_SET_ON);
//         }
//         if(x=='N')
//         {
//             LED_SetStatus(Nour_LED,LED_SET_OFF);
//         }
//    }
    
    SCHED_Init                  ();
    SCHED_Start                 ();

}

#endif