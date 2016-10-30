#include "stm32f0xx_hal.h"
#ifndef __tm1650_H
#define __tm1650_H


/*硬件描述
TM1651_CLK:PULLUP;INPUT
TM1651_DIO:PULLUP;INPUT*/

/*显示文字定义
*/
extern const uint8_t 	CODE00[10];
extern const uint8_t  CODE_Clean[4];
extern const uint8_t  CODE01[4];
extern const uint8_t  S_E[4];
extern const uint8_t  H_E[4];
extern const uint8_t  OFF[4];
extern const uint8_t  CAL[4];
extern const uint8_t  MENU01[4];
extern const uint8_t  MENU02[4];
extern const uint8_t  MENU03[4];
extern const uint8_t  MENU04[4];
extern const uint8_t  MENU05[4];
extern const uint8_t  MENU06[4];
extern const uint8_t  MENU07[4];
extern const uint8_t  MENU08[4];
extern const uint8_t  MENU09[4];
extern const uint8_t  MENU10[4];
extern const uint8_t  _1_[4];
extern const uint8_t  _2_[4];
extern const uint8_t _888[4] ;
extern const uint8_t CODE_Clean_Dot[4] ;
extern const uint8_t SLP[4];


typedef struct 
{
	GPIO_TypeDef*  CLK_GPIO_PORT;// CLK
	uint16_t  CLK_GPIO_PIN;// CLK
	
	GPIO_TypeDef*  DIO_GPIO_PORT;//DIO
	uint16_t  DIO_GPIO_PIN;//DIO
	
	uint8_t 	Is_num;//显示类型 YES表示数字，否则为文字
	int16_t 	*num;//显示数字内容容器
	uint8_t 	*word;//显示文字内容容器
	uint16_t	 num_fab;
	//显示样式控制
	uint8_t 	dot_run_en;//跑灯效果
	uint8_t 	bottom_dot_en;//加热指示
	uint8_t 	blink_en;//闪烁显示
	//显示样式控制
	//内部
	//显示控制
	uint8_t 	times_100ms_inside;
	uint8_t	 disp_value[4];
	uint8_t	 blink_dir;
	uint8_t 	dot_run_bit;
	//显示控制
	//内部
}TM1650_STRUCT;






extern TM1650_STRUCT tm1650_2;
extern void Tm1650_2_show_ISR(void);
extern void Tm1650_2_init(void);




#endif
