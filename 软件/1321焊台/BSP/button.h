#include "stm32f0xx_hal.h"

#ifndef __BUTTON_H
#define __BUTTON_H

//硬件描述
//KEY_SET_PIN				mcu上拉	输入模式
//KEY_UP_PIN				mcu上拉	输入模式
//KEY_DOWN_PIN			mcu上拉	输入模式

typedef struct 
{
	GPIO_TypeDef*  GPIO_PORT;// CLK
	uint16_t  GPIO_PIN;// CLK
	
	uint8_t	 	Is_click;//YES表示按键点击了，NO表示没有点击
	uint8_t 	Is_press;//按键确定状态容器 YES表示按下，NO表示没有按下
	uint16_t 	continue_press_times;
	uint8_t 	state_pre;//按键之前的状态容器
	uint8_t 	state_now;//按键现在的状态容器
	uint8_t 	click_time;
	uint8_t 	press_dir;
}BUTTON_CTRL_TypeDef;



//使用全局按键变量，在使用中直接使用阅读里面的信息，因为所有的数据都在中断中被更新了。
extern BUTTON_CTRL_TypeDef button_set;
extern BUTTON_CTRL_TypeDef button_up;
extern BUTTON_CTRL_TypeDef button_down;
//初始化按键，让按键没有任何存储信息
extern void Button_set_init(void);
extern void Button_up_init(void);
extern void Button_down_init(void);
//接口函数
//函数功能：按键扫描，更新按键的所有状态信息
extern void Button_set_ISR(void);
extern void Button_up_ISR(void);
extern void Button_down_ISR(void);
#endif
