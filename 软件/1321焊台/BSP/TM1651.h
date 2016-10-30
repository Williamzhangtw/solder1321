
/***************************************************************
          CUBE application
0.<Sch>TM1651_DIO,must have a pull-up resistor(10K).   TM1651_CLK can use the mcu pull-up or  a pull-up resistor(10K).  the fact using mcu to pull-up doesn't work
1.<resources needed> 2GPIO 1st(TM1651_CLK)->OUTPUT With pull-up;2nd(TM1651_DIO)->OUTPUT With OD; 
  NOTE:
2.<CUBE SETTING> 
  PA3->①OUTPUT5:②With OD;
  PA4->①OUTPUT;②With pull-up;
3.<Add code> 
①add the code to the main.c what else needed;
#include "../drivers/bsp/tm1651.h"
②add the code below  to the mian.c 
 disp(0xc2,2);
 if use the key dective 
 put   
 tm1651_key_process();
 into the while {}

........The problem is working ......
attion:
IN the output with OD mode, If you need to read the volatage level,you need to do this first, 
 HAL_GPIO_WritePin(TM1651_DIO_Port ,TM1651_DIO_Pin,GPIO_PIN_SET) ;
*******************************************************************/
#ifndef __tm1651_H
#define __tm1651_H

#include "stm32f0xx_hal.h"

/********************定义控制端口**********************/
//sbit CLK=P0^1;								 //CLK
//sbit DIO=P0^0;								 //DIO
#define CLK_H HAL_GPIO_WritePin(TM1651_CLK_GPIO_Port,TM1651_CLK_Pin,GPIO_PIN_SET)
#define CLK_L HAL_GPIO_WritePin(TM1651_CLK_GPIO_Port,TM1651_CLK_Pin,GPIO_PIN_RESET)

#define DIO_H HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port,TM1651_DIO_Pin,GPIO_PIN_SET)
#define DIO_L HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port,TM1651_DIO_Pin,GPIO_PIN_RESET)

//#define DIO()   {HAL_GPIO_WritePin(GPIOA ,GPIO_PIN_4,GPIO_PIN_SET) ;HAL_GPIO_ReadPin(GPIOA ,GPIO_PIN_4)}



extern void disp(uint8_t add, uint8_t value);
extern void disp0(const uint8_t  *p); 
extern void tm1651_key_process(void);
extern void disp_value(uint16_t number)	;
extern void Tm1651_BackLight(uint16_t jibie);
extern void TM1651_Init(uint8_t backlight);

#endif

