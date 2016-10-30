/*date:20160716am
file:key.c
for:all the botton which press the connect,loosen the disconnect,and vice versa
author:ztw
pay attention:用完了要清零
本程序不使用中断，在主函数中每隔reflash_time 就执行一次更新按键数据程序，
*/

#include "../bsp/button.h"




void BUTTON_scan(BUTTON_CTRL_TypeDef *button)//推荐10ms扫描一次
{	
	/*获取数据*/
	button -> state_now =   !HAL_GPIO_ReadPin(button -> GPIO_PORT ,button -> GPIO_PIN); //获得当前按键状态
	//state_now = 0表示无按键按下，！0表示按键按下
	//END
	
	/*按键消抖*/
	if(button ->state_pre ==button -> state_now)//如果上次读取的数值
	{
		button -> Is_press =button -> state_now;
	}
	else 
	{
		button ->state_pre = button -> state_now; 
	} 
	//end
	
	/*连按时间计时功能*/
	//时间 = continue_press_times * 循环周期
	if(button -> press_dir==0)//正向
	{	
		if(button -> Is_press == YES)//按键按下
		{
			button -> press_dir=1;//方向变为正向
		}
	}
	else
	{
		if(button -> Is_press == YES)//按键仍然按下
		{
			button -> continue_press_times++;//连续按计时
			button ->click_time ++;//按击检测的连续按计时
		}
		else //按键松开
		{
			button -> press_dir=0;//方向变为正向
			button -> continue_press_times =0;//连续按计时清零
			
			if(button ->click_time <100)
				button ->Is_click =YES;
			button ->click_time =0;//计时清零
		}
	}
}






/* USER CODE BEGIN EV */

BUTTON_CTRL_TypeDef button_set;
BUTTON_CTRL_TypeDef button_up;
BUTTON_CTRL_TypeDef button_down;

/* USER CODE END EV */




/* USER CODE BEGIN EFP */
void Button_set_init(void)//一定要init后才能使用
{
	button_set.GPIO_PORT = SET_KEY_GPIO_Port ;//底层
	button_set.GPIO_PIN = SET_KEY_Pin;
	button_set.Is_click =	NO;
	button_set.Is_press = NO;
	button_set.continue_press_times =	0;	
	button_set.state_pre = 0;
	button_set.state_now = 0;
	button_set.click_time = 0;
	button_set.press_dir = 0;
}
void Button_up_init(void)//一定要init后才能使用
{
	button_up.GPIO_PORT = SET_KEY_GPIO_Port ;//底层
	button_up.GPIO_PIN = SET_KEY_Pin;
	button_up.Is_click =	NO;
	button_up.Is_press = NO;
	button_up.continue_press_times =	0;	
	button_up.state_pre = 0;
	button_up.state_now = 0;
	button_up.click_time = 0;
	button_up.press_dir = 0;
}
void Button_down_init(void)//一定要init后才能使用
{
	button_down.GPIO_PORT = SET_KEY_GPIO_Port ;//底层
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

