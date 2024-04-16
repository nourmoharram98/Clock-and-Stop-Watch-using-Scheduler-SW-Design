#ifndef GPIO_H_
#define GPIO_H_

#include "Std_types.h"

/*------------------------REGISTERS STRUCT---------------------*/
typedef struct
{
  volatile u32 GPIO_MODER;
  volatile u32 GPIO_OTYPER;
  volatile u32 GPIO_OSPEEDR;
  volatile u32 GPIO_PUPDR;
  volatile u32 GPIO_IDR;
  volatile u32 GPIO_ODR;
  volatile u32 GPIO_BSRR;
  volatile u32 GPIO_LCKR;
  volatile u32 GPIO_AFRL;
  volatile u32 GPIO_AFRH;
} GPIO_Port_t;


/*---------------------------GPIO PORTS------------------------*/
#define GPIO_PORT_A_BASE_ADDRESS 	(void *)0x40020000
#define GPIO_PORT_B_BASE_ADDRESS 	(void *)0x40020400
#define GPIO_PORT_C_BASE_ADDRESS 	(void *)0x40020800

#define GPIO_A							(( GPIO_Port_t*)(GPIO_PORT_A_BASE_ADDRESS))
#define GPIO_B							(( GPIO_Port_t*)(GPIO_PORT_B_BASE_ADDRESS))
#define GPIO_C							(( GPIO_Port_t*)(GPIO_PORT_C_BASE_ADDRESS))


/*---------------------------GPIO PINS-------------------------*/
#define GPIO_PIN_0 					0x00000000
#define GPIO_PIN_1 					0x00000001
#define GPIO_PIN_2 					0x00000002
#define GPIO_PIN_3 					0x00000003
#define GPIO_PIN_4 					0x00000004
#define GPIO_PIN_5 					0x00000005
#define GPIO_PIN_6 					0x00000006
#define GPIO_PIN_7 					0x00000007
#define GPIO_PIN_8 					0x00000008
#define GPIO_PIN_9 					0x00000009
#define GPIO_PIN_10 				0x0000000A
#define GPIO_PIN_11 				0x0000000B
#define GPIO_PIN_12 				0x0000000C
#define GPIO_PIN_13 				0x0000000D
#define GPIO_PIN_14 				0x0000000E
#define GPIO_PIN_15 				0x0000000F

/*---------------------------GPIO PIN MODES-------------------------*/
#define INPUT_FLOAT					0x00000000
#define INPUT_PU					0x00010000
#define INPUT_PD					0x00020000





#define OUTPUT_PP_FLOAT				0x00000001
#define OUTPUT_PP_PU				0x00010001
#define OUTPUT_PP_PD				0x00020001


#define OUTPUT_OD_FLOAT				0x00000101
#define OUTPUT_OD_PU				0x00010101
#define OUTPUT_OD_PD				0x00020101





#define ALT_FUNC_PP_FLOAT			0x00000002
#define ALT_FUNC_PP_PU				0x00010002
#define ALT_FUNC_PP_PD				0x00020002


#define ALT_FUNC_OD_FLOAT			0x00000102
#define ALT_FUNC_OD_PU				0x00010102
#define ALT_FUNC_OD_PD				0x00020102


/*---------------------------GPIO PIN SPEEDS-------------------------*/
#define GPIO_SPEED_LOW 				0x00000000
#define GPIO_SPEED_MED 				0x00000001
#define GPIO_SPEED_HIGH 			0x00000002
#define GPIO_SPEED_VHIGH 			0x00000003



/*---------------------------GPIO PIN STATES-------------------------*/
#define GPIO_STATE_RESET 			0x00000000
#define GPIO_STATE_SET 				0x00000001

#define LOGIC_HIGH        (1u)
#define LOGIC_LOW         (0u)


/*-----------------------------MASKS----------------------------*/

/*-------------------------CLEAR MODE BITS----------------------*/
#define GPIO_CLEAR_MODE_BITS		0x00000003
#define GPIO_CLEAR_TYPE_BITS		0x00000001
#define GPIO_CLEAR_PU_OR_PD_BITS	0x00000003
#define GPIO_CLEAR_SPEED_BITS		0x00000003

/*--------------------------GET MODE BITS-----------------------*/
#define GPIO_GET_MODE_BITS    		0x00000003
#define GPIO_GET_TYPE_BIT     		0x00000100
#define GPIO_GET_PUPD_BITS		    0x00030000


/*-------------------------------BSRR--------------------------*/
#define BSRR_OFFSET                 16
#define BSRR_BIT_VALUE              1


/*---------------------------GPIO STRUCT-------------------------*/
typedef struct
{
  u32 	Pin;
  void *Port;
  u32 	Speed;
  u32 	Mode;
} GPIO_Pin_t;


typedef enum {
  GPIO_enuOK,
  GPIO_enuNotOK
}GPIO_enuErrorStatus;

/*--------------------------FUNCTIONS PROTOTYPE-----------------*/


/*--------------------------------------------------------------*/
/*--------------------------GPIO_Init---------------------------*/
/*@brief: function to initialize the pin configurations---------*/
/*@param: PtrtoPinConfig----------------------------------------*/
/*@return:GPIO_enuErrorStatus-----------------------------------*/
/*@constraint: Clock must be enabled before initializing--------*/
/*--------------------------------------------------------------*/
GPIO_enuErrorStatus GPIO_Init(GPIO_Pin_t *PtrtoPinConfig);


/*--------------------------------------------------------------*/
/*-----------------------GPIO_SetPinValue-----------------------*/
/*@brief: function to set the pin values------------------------*/
/*@param: PtrtoGPIO_Port, GPIO_PinNumber, GPIO_PinStatus--------*/
/*@return:GPIO_enuErrorStatus-----------------------------------*/
/*@constraint: GPIO must be initialized before setting pins-----*/
/*--------------------------------------------------------------*/
GPIO_enuErrorStatus GPIO_SetPinValue(void *PtrtoGPIO_Port,u32 GPIO_PinNumber, u32 GPIO_PinStatus);

/*--------------------------------------------------------------*/
/*-----------------------GPIO_GetPinValue-----------------------*/
/*@brief: function to get the pin values------------------------*/
/*@param: PtrtoGPIO_Port, GPIO_PinNumber, PtrtoPinStatus--------*/
/*@return:GPIO_enuErrorStatus-----------------------------------*/
/*--------------------------------------------------------------*/
GPIO_enuErrorStatus GPIO_GetPinValue(void *PtrtoGPIO_Port,u32 GPIO_PinNumber, u8 *PtrtoPinStatus);

#endif /* GPIO_H_ */
