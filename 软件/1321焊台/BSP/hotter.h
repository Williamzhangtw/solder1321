#include "stm32f0xx_hal.h"

#ifndef _HOTER1321_H
#define _HOTER1321_H
//Ӳ������
//SolderHOT_Pin //���ȹܽţ��͵�ƽ���ȣ��ߵ�ƽֹͣ����
//Solder_ADC_Pin  //�ӷ���˿ �ȵ�ż �����˷ŷŴ��������
//HZ_50_Pin //28V������ �������

 typedef struct 
 {
	GPIO_TypeDef*  GPIO_PORT;// CLK
	uint16_t  GPIO_PIN;// CLK
	//����	
 
	uint16_t real_adc;//AD�ڲɼ�����ADC
	int16_t real_temperature;//����ת�����ʵ���¶�
	uint8_t sensor_err;
	uint8_t hotter_err;
	uint8_t work_state;//heating,stop heating,cooling,keep constant temperature
	 
	 
	 //���Ʋ���
	float Ks;//�¶����ߵ�K
	int16_t Bs;//�¶����ߵ�B
	int16_t Lmin;//�¶�����
	int16_t Lmax;//�¶�����
	int16_t Cmin;// ��СУ׼�¶�ƫ��ֵ
	int16_t Cmax;//���У׼�¶�ƫ��ֵ
	int16_t target_temperature;//Ŀ���¶�
	int16_t adjust_temperature;//У׼�¶ȵ�ƫ��ֵ
	int16_t sleep_temperature;//˯���¶� 
	uint16_t sensor_err_adc;//�����ADCֵ
	uint8_t temp_distinguish_dif;//�¶���ʾ������Сֵ

	
	//���ݷ���buf
	int16_t minus_pre;//real_temperature -.target_temperature ;
	int16_t minus_now;//real_temperature -.target_temperature ;
	int16_t temperature_pre;
	uint8_t heated_check_dir;
	
	_Bool temperature_type;//0 ��ʾ���϶�, ��0��ʾ���϶�
	
 }
 HOTER_CTRL_TypeDef;
 

 typedef enum 
 {
	HEATTING,//������
	OVER_HEAT,//������
	NOT_HEAT,//������
	TEMP_BALANCE,//������
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
