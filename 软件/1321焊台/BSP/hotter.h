#include "stm32f0xx_hal.h"

#ifndef _HOTER1321_H
#define _HOTER1321_H
//硬件描述
//SolderHOT_Pin //加热管脚，低电平加热，高电平停止加热
//Solder_ADC_Pin  //接发热丝 热电偶 经过运放放大后的输出脚
//HZ_50_Pin //28V交流脚 过零检测脚

 typedef struct 
 {
	 
	void (*heat_en)(uint8_t);//加热函数 

	 
	//数据	
 
	uint16_t real_adc;
	int16_t real_temperature;
	uint8_t sensor_err;
	uint8_t hotter_err;
	uint8_t work_state;//heating,stop heating,cooling,keep constant temperature
	 
	 
	 //控制参数
	float Ks;//with welding head,the  k value of linear equation of the sensing heed with the welding head outside.
	int16_t Bs;
	int16_t Lmin;//minimum range
	int16_t Lmax;//max range
	int16_t Cmin;// adjust temperture min
	int16_t Cmax;//adjust temperture max
	int16_t target_temperature;
	int16_t adjust_temperature;
	int16_t sleep_temperature;	 
	uint16_t sensor_err_adc;
	uint8_t temp_distinguish_dif;

	
	//数据分析buf
	int16_t minus_pre;//real_temperature -.target_temperature ;
	int16_t minus_now;//real_temperature -.target_temperature ;
	int16_t temperature_pre;
	uint8_t heated_check_dir;
 }HOTER_CTRL_TypeDef;
 

 typedef enum 
 {
	HEATTING,
	OVER_HEAT,
	NOT_HEAT,
	TEMP_BALANCE,
 }
Hotter_state_enum;
 
 

//user//
extern 	HOTER_CTRL_TypeDef hotter1321;
extern 	void 	Hotter1321_init	(void);
// 
extern 	void	Hotter1321RealTemp_ISR (void);
// 
 
extern HOTER_CTRL_TypeDef hotterK ;
extern 	void 	HotterK_init	(void); 
extern 	void	HotterKRealTemp_ISR (void);
  
#endif
