#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f0xx_hal.h"
#include "../bsp/hotter.h"

#define	PARA_START_ADDR   0x08003C00	//flash从这的地址开始记录


//-------------------------------------------------


typedef struct 
{
	uint8_t id;	//用于区分是否第一次擦写
	int16_t hotter1321_adjust_temperature;//目标温度
	int16_t hotter1321_target_temperature;//校准偏移值
	_Bool temperature_type;//温度单位
}
Hotter_flash__TypeDef;




extern Hotter_flash__TypeDef hotter_flash;



extern void FlshPara_Save(void);
extern void FlshPara_Init(void);

#endif 
