#include "stm32f0xx_hal.h"

#ifndef _HOTER1321_H
#define _HOTER1321_H
//Ӳ������
//SolderHOT_Pin //���ȹܽţ��͵�ƽ���ȣ��ߵ�ƽֹͣ����
//Solder_ADC_Pin  //�ӷ���˿ �ȵ�ż �����˷ŷŴ��������
//HZ_50_Pin //28V������ �������

 typedef struct 
 {
	 
	void (*heat_en)(uint8_t);//���Ⱥ��� 

	 
	//����	
 
	uint16_t real_adc;
	int16_t real_temperature;
	uint8_t sensor_err;
	uint8_t hotter_err;
	uint8_t work_state;//heating,stop heating,cooling,keep constant temperature
	 
	 
	 //���Ʋ���
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

	
	//���ݷ���buf
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
