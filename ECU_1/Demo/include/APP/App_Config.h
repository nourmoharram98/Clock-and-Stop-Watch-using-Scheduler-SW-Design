/**
 * @file App.h
 * @author your name (you@domain.com)
 * @brief this file will carry the application initialization sequence 
 * @version 0.1
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include"Std_Types.h"


#define DIGIT_STATE_PRINT          0
#define DIGIT_STATE_NOT_PRINT      1


typedef struct 
{
          u8    value;
   const  u8    max_value;      
   const  u8    x_pos;
   const  u8    y_pos;
          u8    digit_state;  //digit_state -> ( print , not print ) 
}unit_Info_t;

typedef struct 
{
       const u8 Pattern; 
       const u8 x_pos;
       const u8 y_pos;
}FrameConstants_t;
