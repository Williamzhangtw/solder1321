#include "stm32f0xx_hal.h"

#ifndef _HOTER1321_H
#define _HOTER1321_H
//硬件描述
//SolderHOT_Pin //加热管脚，低电平加热，高电平停止加热
//Solder_ADC_Pin  //接发热丝 热电偶 经过运放放大后的输出脚
//HZ_50_Pin //28V交流脚 过零检测脚

 typedef struct 
 {
	GPIO_TypeDef*  GPIO_PORT;// CLK
	uint16_t  GPIO_PIN;// CLK
	//数据	
 
	uint16_t real_adc;//AD口采集到的ADC
	int16_t real_temperature;//经过转化后的实际温度
	uint8_t sensor_err;
	uint8_t hotter_err;
	uint8_t work_state;//heating,stop heating,cooling,keep constant temperature
	 
	 
	 //控制参数
	float Ks;//温度曲线的K
	int16_t Bs;//温度曲线的B
	int16_t Lmin;//温度下限
	int16_t Lmax;//温度上限
	int16_t Cmin;// 最小校准温度偏移值
	int16_t Cmax;//最大校准温度偏移值
	int16_t target_temperature;//目标温度
	int16_t adjust_temperature;//校准温度的偏移值
	int16_t sleep_temperature;//睡眠温度 
	uint16_t sensor_err_adc;//出错的ADC值
	uint8_t temp_distinguish_dif;//温度显示区分最小值

	
	//数据分析buf
	int16_t minus_pre;//real_temperature -.target_temperature ;
	int16_t minus_now;//real_temperature -.target_temperature ;
	int16_t temperature_pre;
	uint8_t heated_check_dir;
	
	_Bool temperature_type;//0 表示摄氏度, ！0表示华氏度
	
 }
 HOTER_CTRL_TypeDef;
 

 typedef enum 
 {
	HEATTING,//加热中
	OVER_HEAT,//过热了
	NOT_HEAT,//不加热
	TEMP_BALANCE,//恒温中
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
