#include "stm32f0xx_hal.h"

#ifndef __BUTTON_H
#define __BUTTON_H

//Ӳ������
//KEY_SET_PIN				mcu����	����ģʽ
//KEY_UP_PIN				mcu����	����ģʽ
//KEY_DOWN_PIN			mcu����	����ģʽ

typedef struct 
{
	GPIO_TypeDef*  GPIO_PORT;// CLK
	uint16_t  GPIO_PIN;// CLK
	
	uint8_t	 	Is_click;//YES��ʾ��������ˣ�NO��ʾû�е��
	uint8_t 	Is_press;//����ȷ��״̬���� YES��ʾ���£�NO��ʾû�а���
	uint16_t 	continue_press_times;
	uint8_t 	state_pre;//����֮ǰ��״̬����
	uint8_t 	state_now;//�������ڵ�״̬����
	uint8_t 	click_time;
	uint8_t 	press_dir;
}BUTTON_CTRL_TypeDef;



//ʹ��ȫ�ְ�����������ʹ����ֱ��ʹ���Ķ��������Ϣ����Ϊ���е����ݶ����ж��б������ˡ�
extern BUTTON_CTRL_TypeDef button_set;
extern BUTTON_CTRL_TypeDef button_up;
extern BUTTON_CTRL_TypeDef button_down;
//��ʼ���������ð���û���κδ洢��Ϣ
extern void Button_set_init(void);
extern void Button_up_init(void);
extern void Button_down_init(void);
//�ӿں���
//�������ܣ�����ɨ�裬���°���������״̬��Ϣ
extern void Button_set_ISR(void);
extern void Button_up_ISR(void);
extern void Button_down_ISR(void);
#endif
