#include "stm32f0xx_hal.h"
#ifndef __ROTARY_H
#define __ROTARY_H



//硬件描述
//  rotary_a_PIN		:mcu上拉；输入模式
//  rotary_B_PIN		:mcu上拉；输入模式




typedef enum {
	no_direction = 0x00 ,
	Spin_left = 0x01 ,
	Spin_right = 0x02 ,
}Spin_direction_enum;


typedef struct 
{
	uint8_t (*read_rotary_a)(void);//底层//如果出现空指针，将会导致严重错误
	uint8_t (*read_rotary_b)(void);//底层
	
	uint8_t Spin_direction;//数据层
	
	//数据分析层
	uint8_t  elec_leval_a_pre;
	uint8_t  elec_leval_b_pre;
	uint8_t  elec_leval_a_now;
	uint8_t  elec_leval_b_now;
	uint8_t  temple;
	uint8_t  N;
	//数据分析层
}R0TARY_CTRL_TypeDef;

//应用1

//extern R0TARY_CTRL_TypeDef rotary_1;
extern void Rotary_1_init(void);
extern void Rotary_1_scan_ISR(void);
#endif 
