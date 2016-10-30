/*date:20160716am
file:key.c
for:all the botton which press the connect,loosen the disconnect,and vice versa
author:ztw
pay attention:������Ҫ����
������ʹ���жϣ�����������ÿ��reflash_time ��ִ��һ�θ��°������ݳ���
*/

#include "../bsp/button.h"




void BUTTON_scan(BUTTON_CTRL_TypeDef *button)//�Ƽ�10msɨ��һ��
{	
	/*��ȡ����*/
	button -> state_now =   !HAL_GPIO_ReadPin(button -> GPIO_PORT ,button -> GPIO_PIN); //��õ�ǰ����״̬
	//state_now = 0��ʾ�ް������£���0��ʾ��������
	//END
	
	/*��������*/
	if(button ->state_pre ==button -> state_now)//����ϴζ�ȡ����ֵ
	{
		button -> Is_press =button -> state_now;
	}
	else 
	{
		button ->state_pre = button -> state_now; 
	} 
	//end
	
	/*����ʱ���ʱ����*/
	//ʱ�� = continue_press_times * ѭ������
	if(button -> press_dir==0)//����
	{	
		if(button -> Is_press == YES)//��������
		{
			button -> press_dir=1;//�����Ϊ����
		}
	}
	else
	{
		if(button -> Is_press == YES)//������Ȼ����
		{
			button -> continue_press_times++;//��������ʱ
			button ->click_time ++;//����������������ʱ
		}
		else //�����ɿ�
		{
			button -> press_dir=0;//�����Ϊ����
			button -> continue_press_times =0;//��������ʱ����
			
			if(button ->click_time <100)
				button ->Is_click =YES;
			button ->click_time =0;//��ʱ����
		}
	}
}






/* USER CODE BEGIN EV */

BUTTON_CTRL_TypeDef button_set;
BUTTON_CTRL_TypeDef button_up;
BUTTON_CTRL_TypeDef button_down;

/* USER CODE END EV */




/* USER CODE BEGIN EFP */
void Button_set_init(void)//һ��Ҫinit�����ʹ��
{
	button_set.GPIO_PORT = SET_KEY_GPIO_Port ;//�ײ�
	button_set.GPIO_PIN = SET_KEY_Pin;
	button_set.Is_click =	NO;
	button_set.Is_press = NO;
	button_set.continue_press_times =	0;	
	button_set.state_pre = 0;
	button_set.state_now = 0;
	button_set.click_time = 0;
	button_set.press_dir = 0;
}
void Button_up_init(void)//һ��Ҫinit�����ʹ��
{
	button_up.GPIO_PORT = SET_KEY_GPIO_Port ;//�ײ�
	button_up.GPIO_PIN = SET_KEY_Pin;
	button_up.Is_click =	NO;
	button_up.Is_press = NO;
	button_up.continue_press_times =	0;	
	button_up.state_pre = 0;
	button_up.state_now = 0;
	button_up.click_time = 0;
	button_up.press_dir = 0;
}
void Button_down_init(void)//һ��Ҫinit�����ʹ��
{
	button_down.GPIO_PORT = SET_KEY_GPIO_Port ;//�ײ�
	button_down.GPIO_PIN = SET_KEY_Pin;
	button_down.Is_click =	NO;
	button_down.Is_press = NO;
	button_down.continue_press_times =	0;	
	button_down.state_pre = 0;
	button_down.state_now = 0;
	button_down.click_time = 0;
	button_down.press_dir = 0;
}

/* USER CODE END EFP */

void Button_set_ISR(void)
{
	BUTTON_scan(&button_set );
}
void Button_up_ISR(void)
{
	BUTTON_scan(&button_up );
}
void Button_down_ISR(void)
{
	BUTTON_scan(&button_down );
}

