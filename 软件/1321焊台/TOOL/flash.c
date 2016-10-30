//data 2016年10月30日
#include "../tool/flash.h"
#include <string.h>
#include <math.h>
//全局变量



const Hotter_flash__TypeDef hotter_flash_default ={88,0,300,0};//用于机器设置重置

Hotter_flash__TypeDef hotter_flash;

/*function:存数据 
//返回值 存储是否正常，NO表示 没有出错，yes 表示异常
*/
uint8_t APP_DataSaveIsError(uint32_t start_address, uint16_t *data,int16_t data_num)
{
	uint8_t return_value = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t page_error = 0;
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = start_address;
	EraseInitStruct.NbPages     =1; // FLASH_PAGE_SIZE;1页有1KB的容量
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &page_error) != HAL_OK)
	{
		return_value = 1;//擦写失败
	}
	for(uint16_t  number_count = 0; number_count <data_num ;number_count ++)
	{
		HAL_FLASH_Program (FLASH_TYPEPROGRAM_HALFWORD, start_address+ (number_count<<1),*(data+number_count));
	}
	
	return return_value;
}

/*function： 读数据，从一个地址读取N个数据。
//参数：address 表示 读取数的地址，*data表示 存放的地址，num表示存放的个数
*/
void APP_DataRead(uint32_t address, uint16_t *data,int16_t num)
{

	for (uint16_t index= 0;index<num ;index++ ) 
	{
		*(data+index)  = *( uint16_t *)(address + (index<<1)  );
	}
	 
}


void FlshPara_Save(void)
{
 	FLASH_EraseInitTypeDef EraseInitStruct;

	uint32_t page_error = 0;
	uint16_t   size; 
	
	
	uint16_t *ptemp  = (uint16_t*)&hotter_flash;
	size = sizeof(Hotter_flash__TypeDef);
	
	hotter_flash .id =88;
	hotter_flash.hotter1321_target_temperature = hotter1321 .target_temperature; 
	hotter_flash.hotter1321_adjust_temperature  = hotter1321 .adjust_temperature; 
	
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = PARA_START_ADDR;
	EraseInitStruct.NbPages     =1;
	HAL_FLASHEx_Erase(&EraseInitStruct, &page_error);
 
	for(uint16_t  i = 0; i <size ;i +=2)
	{

		HAL_FLASH_Program (FLASH_TYPEPROGRAM_HALFWORD, PARA_START_ADDR+ i,*ptemp++);
	}
	memcpy(&hotter_flash,(Hotter_flash__TypeDef *)PARA_START_ADDR,size); 
 	HAL_FLASH_Lock();
}


void FlshPara_Init(void)
{
	
	FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t page_error = 0;
	uint16_t   size; 
	uint16_t *ptemp  = (uint16_t*)&hotter_flash;
  size = sizeof(Hotter_flash__TypeDef);
	memcpy(&hotter_flash,(Hotter_flash__TypeDef *)PARA_START_ADDR,size); 
	
	//检测是否需要重置
	_Bool default_set_flag =0;
	if (!HAL_GPIO_ReadPin(DOWN_KEY_GPIO_Port ,DOWN_KEY_Pin ))
	{
		HAL_Delay (10);
		if (!HAL_GPIO_ReadPin(DOWN_KEY_GPIO_Port ,DOWN_KEY_Pin ))//消抖
		{
			default_set_flag = 1;//确定需要重置
		}
	}

 	if ((hotter_flash.id!=88)||default_set_flag)
	{
		HAL_FLASH_Unlock();
		EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
		EraseInitStruct.PageAddress = PARA_START_ADDR;
		EraseInitStruct.NbPages     =1;  
		HAL_FLASHEx_Erase(&EraseInitStruct, &page_error);
		memcpy(&hotter_flash,&hotter_flash_default,size); 
		 
		for(uint8_t  i = 0; i <size ;i+=2)
		{
			HAL_FLASH_Program (FLASH_TYPEPROGRAM_HALFWORD, PARA_START_ADDR+ i,*ptemp++);
		}
		HAL_FLASH_Lock();
		memcpy(&hotter_flash,(Hotter_flash__TypeDef *)PARA_START_ADDR,size); 
		
	}
	
	hotter1321 .adjust_temperature = hotter_flash.hotter1321_adjust_temperature ;
	hotter1321 .target_temperature = hotter_flash.hotter1321_target_temperature ;

}

