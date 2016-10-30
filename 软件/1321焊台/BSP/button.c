/*date:20160716am
file:key.c
for:all the botton which press the connect,loosen the disconnect,and vice versa
author:ztw
pay attention:用完了要清零
本程序不使用中断，在主函数中每隔reflash_time 就执行一次更新按键数据程序，

*/

#include "../bsp/button.h"




void BUTTON_scan(BUTTON_CTRL_TypeDef *button)
{
	/*Interference prevention 带按键消抖获取按键状态*/
	
	//底层，获取数据
	button -> state_now = button -> read_button();//获得当前按键状态
	//底层，获取数据
	
	if(button ->state_pre ==button -> state_now)//如果上次读取的数值
	{
		button -> Is_press =button -> state_now;
	}
	else 
	{
		button ->state_pre = button -> state_now; 
	}
	
	

	if(button -> Is_support_click)
	{
		/*按击检测功能*/	
		if(button -> click_dir==0)
		{
			if(button -> Is_press==YES)
			{
				button ->click_time =0;
				button -> click_dir=1;
			}
		}
		else 
		{
			button ->click_time ++;
			if(button -> Is_press == NO)
			{
				if(button ->click_time <100)
				button ->Is_click =YES;
				button -> click_dir=0;
			}
			
		}		
	}
	/*连按时间计数功能*/
	if(button -> continue_press_dir==0)
	{
		if(button -> Is_press == YES)
		{
			button -> continue_press_times =0;
			button -> continue_press_dir=1;
		}
	}
	else
	{
		if(button -> Is_press == YES)
		{
			button -> continue_press_times++;
		}
		else
		{
			button -> continue_press_dir=0;
			button -> continue_press_times =0;
		}
		
	}
	

}






/* USER CODE BEGIN EV */

BUTTON_CTRL_TypeDef button_1;

/* USER CODE END EV */


uint8_t read_button_1(void)
{
	return !HAL_GPIO_ReadPin(SET_KEY_GPIO_Port ,SET_KEY_Pin);
}



/* USER CODE BEGIN EFP */
void Button_1_init(void)//一定要init后才能使用
{
	button_1 .read_button = &read_button_1 ;//底层
	button_1.Is_support_click = YES ;	
	button_1 .continue_press_times =0;	
	button_1.Is_press = NO ;
	button_1 .Is_click =NO;
	
	button_1 .state_pre =0;
	button_1 .state_now =0;
	button_1. click_dir = 0;
	button_1 .continue_press_dir =0;
}

/* USER CODE END EFP */

void Button_1_ISR(void)
{
	HAL_GPIO_TogglePin(LAY2_GPIO_Port ,LAY2_Pin  );
	//BUTTON_scan(&button_1 );
}
