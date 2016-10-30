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
	uint8_t (*power_on)(void);
	uint8_t (*reset)(void);//函数 
	
	HOTTER_STATUS_ENUM state;
	BUTTON_CTRL_TypeDef *button;
	TM1650_STRUCT *tm1650;
	HOTER_CTRL_TypeDef *hotter;
	
	uint8_t hz_50count;
	
	
	uint16_t time100ms;
	uint16_t heated_times;
	uint8_t Is_reset_position;//该位为1后不执行加热
	uint8_t Is_power_on;//dev control switch on
	uint8_t input_ctrl;

	
	
}HOTTER_CTRL_Typedef;

extern HOTTER_CTRL_Typedef solder1321; 

extern void Solder1321_init(void);
extern void Solder1321Ctrl(void);
extern void Solder1321PowerOn_ISR(void);
extern void Solder1321HeatingTime_ISR(void) ;

extern HOTTER_CTRL_Typedef airK;

extern void FanCtrl_ISR(void);
extern void AirK_init(void);
extern void AirKCtrl(void);
extern void AirKPowerOn_ISR(void);
extern void AirKTime_ISR(void) ;
extern void AirKTempResetPosition_ISR(void);
extern void Control_ISR(void);
#endif
