#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f0xx_hal.h"
#include "../bsp/hotter.h"

#define	PARA_START_ADDR   0x08003C00	//flash����ĵ�ַ��ʼ��¼


//-------------------------------------------------


typedef struct 
{
	uint8_t id;	//���������Ƿ��һ�β�д
	int16_t hotter1321_adjust_temperature;//Ŀ���¶�
	int16_t hotter1321_target_temperature;//У׼ƫ��ֵ
	_Bool temperature_type;//�¶ȵ�λ
}
Hotter_flash__TypeDef;




extern Hotter_flash__TypeDef hotter_flash;



extern void FlshPara_Save(void);
extern void FlshPara_Init(void);

#endif 
