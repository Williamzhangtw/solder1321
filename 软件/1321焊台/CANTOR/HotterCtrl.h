#include "stm32f0xx_hal.h"
#ifndef _HOTTERCTRL_H
#define _HOTTERCTRL_H

#include "../bsp/hotter.h"
#include "../bsp/button.h"
#include "../bsp/tm1650.h"

typedef   enum 
{
	TEMP_CHECK_READY,
    TEMP_CHECK,
	
	TEMP_IDLE_READY,
    TEMP_IDLE,
	
	TEMP_TARGET_SHOW_READY,
	TEMP_TARGET_SHOW, 
	
	TEMP_CTRL_READY,
    TEMP_CTRL,
	
	TEMP_TARGET_SET_READY,
    TEMP_TARGET_SET,
	
	TEMP_ALARM_READY,
    TEMP_ALARM,
	
	TEMP_ADJUST_WARNING_READY,
	TEMP_ADJUST_WARNING,
	
	TEMP_ADJUST_READY ,
    TEMP_ADJUST ,
	
	TEMP_RESET_POSITION_READY,
	TEMP_RESET_POSITION,
	
} HOTTER_STATUS_ENUM;

typedef struct 
{	
	TM1650_STRUCT *tm1650;//���ȿ�������ʾ�豸
	HOTER_CTRL_TypeDef 	*hotter;//���ȿ��������Ƶķ���о
	BUTTON_CTRL_TypeDef *button_set;
	BUTTON_CTRL_TypeDef *button_up;
	BUTTON_CTRL_TypeDef *button_down;
	
	_Bool hotter_ctrl_en;//enable ��ʾ ���ȿ����������������ʾ���ȿ��Ʋ�����
	
	HOTTER_STATUS_ENUM state;//��ʾ���ȿ������Ĺ���״̬ 
	uint8_t hz_50count;
	
	
	uint16_t time100ms;
	uint16_t heated_times;


	
	
	
}HOTTER_CTRL_Typedef;

extern HOTTER_CTRL_Typedef solder1321; 

extern void Solder1321_init(void);
extern void Solder1321Ctrl(void);
extern void Solder1321PowerOn_ISR(void);
extern void Solder1321HeatingTime_ISR(void) ;





#endif
