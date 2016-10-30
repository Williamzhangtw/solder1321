/*            2016/07/18
*/
#include "../cantor/hotterctrl.h"
#include "../tool/msg_task.h"
#include "../bsp/tm1650.h"
#include "../tool/flash.h"
#include <string.h>
#include <math.h>
#include "../tool/adc_filter.h"


void TempTime_ISR(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl ->time100ms++;
	hotterCtrl ->heated_times++;
}


void TEMP_CHECK_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	hotterCtrl -> tm1650 ->Is_num = NO ;//显示文字
	hotterCtrl -> tm1650 ->word =(uint8_t*)CODE_Clean ;
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	hotterCtrl ->time100ms =0;	//状态机计时清零
	hotterCtrl -> state = TEMP_CHECK  ;	
}


void TEMP_CHECK_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(hotterCtrl->time100ms >3)
	{
		if(hotterCtrl-> Is_power_on)
		{
			 hotterCtrl ->  state = TEMP_TARGET_SHOW_READY  ;
		}
		else 
		{
			hotterCtrl ->  state = TEMP_IDLE_READY  ;
		}
			
		
	}
}


void TEMP_IDLE_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	hotterCtrl -> tm1650 ->Is_num = NO ;//显示文字
	hotterCtrl -> tm1650 ->word =(uint8_t*)OFF;//内容OFF
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	hotterCtrl -> state = TEMP_IDLE  ;	
}


void TEMP_IDLE_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(hotterCtrl-> Is_power_on)
	{
		 hotterCtrl ->  state = TEMP_TARGET_SHOW_READY  ;
	}
}


void TEMP_TARGET_SHOW_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl ->time100ms =0;//用于后面程序计时用
	hotterCtrl -> hotter->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl->tm1650->Is_num = YES ;//显示数字
	hotterCtrl->tm1650->num = &hotterCtrl->hotter->target_temperature ;//内容：目标温度
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl->tm1650->blink_en  =YES ;//闪烁
	hotterCtrl->tm1650->bottom_dot_en =DISABLE ;//不显示加热点
	//显示
	hotterCtrl->state = TEMP_TARGET_SHOW ;
	 
}


void TEMP_TARGET_SHOW_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//1.5S后进入CTRL
	if(hotterCtrl ->time100ms>15)
	{
		hotterCtrl->state = TEMP_CTRL_READY  ;
	}
 
}
void TEMP_CTRL_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	
	
	
	hotterCtrl-> heated_times=0;
	
	
	hotterCtrl->hotter->sensor_err =0;
	hotterCtrl->hotter->hotter_err =0;
	hotterCtrl->hotter->heated_check_dir = 0;
	//显示
	hotterCtrl->tm1650->Is_num = YES ;//显示数字类型
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl->tm1650->blink_en  =DISABLE ;	// 
	//显示
	hotterCtrl ->time100ms =0;//用于后面程序计时用
	hotterCtrl->state = TEMP_CTRL ;  
}


void TEMP_CTRL_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	
	//显示内容
	if(hotterCtrl->hotter->work_state ==TEMP_BALANCE )
	{
		hotterCtrl->tm1650->num = &hotterCtrl->hotter->target_temperature;
	}
	else 
	{
		hotterCtrl->tm1650->num =&hotterCtrl-> hotter->real_temperature;
	}
	//显示内容
	
	{

	hotterCtrl->hotter->minus_now = hotterCtrl-> hotter->real_temperature -hotterCtrl->hotter->target_temperature ;  
	switch(hotterCtrl->hotter->work_state )
	{
		case HEATTING : //加热
			if (hotterCtrl-> hotter->minus_now  >=0 )
			{
				 hotterCtrl->hotter->work_state =OVER_HEAT ;
			
			}
			break ;

		case OVER_HEAT  : //过热

			if (  hotterCtrl->hotter->minus_now  <0 )
			{
				
				hotterCtrl-> hotter->work_state =TEMP_BALANCE ;
			}
			break ;
			
		case NOT_HEAT : 	//停止加热
			if ( hotterCtrl-> hotter->minus_now  <=0 )
			{

				hotterCtrl->hotter->work_state  = TEMP_BALANCE ;
			}
			break ;
			
			
		case TEMP_BALANCE  ://平衡
			if ( hotterCtrl->hotter->minus_now  >hotterCtrl-> hotter->temp_distinguish_dif)
				hotterCtrl->hotter->work_state  = NOT_HEAT  ; 
			if ( hotterCtrl-> hotter->minus_now  < -hotterCtrl->hotter->temp_distinguish_dif )
				hotterCtrl->hotter->work_state  = HEATTING  ; 	
			break;
		
		default :
			break ;
	}
	}
	
	
 	//加热控制
	if(hotterCtrl->hotter->real_adc	<	hotterCtrl->hotter->sensor_err_adc)
	{
		if( hotterCtrl->hotter->real_temperature	<	hotterCtrl->hotter->target_temperature)
		{
			hotterCtrl->hotter->heat_en(ENABLE );	
			hotterCtrl->tm1650->bottom_dot_en =ENABLE  ;//表示加热中
		}
		else
		{
			hotterCtrl->hotter ->heat_en(DISABLE );
			hotterCtrl->tm1650 ->bottom_dot_en =DISABLE  ;//表示不加热
		}
	}
	else //ADC超出范围 S-E
	{
		
		hotterCtrl->hotter->sensor_err =1;
		hotterCtrl->state = TEMP_ALARM_READY ; 
	}
	//加热控制	
//	
	//温度上不去检查
	if(hotterCtrl->hotter->heated_check_dir == 0 )
	{
		if(	hotterCtrl->hotter->work_state==HEATTING )
		{
			hotterCtrl->hotter->temperature_pre = hotterCtrl->hotter->real_temperature;//记录当前实时温度
			hotterCtrl->hotter->heated_check_dir=1;	
 			hotterCtrl-> heated_times =0;
		}
		hotterCtrl->tm1650->blink_en  =DISABLE   ;
	}
	else 
	{
		if(	hotterCtrl->hotter->work_state==HEATTING )
		{
		  	if((hotterCtrl->hotter->temperature_pre -hotterCtrl->hotter->real_temperature )>100)
			{
				hotterCtrl->tm1650->blink_en  =ENABLE ;		
			}
			else
			{
				hotterCtrl->tm1650->blink_en  =DISABLE   ;
			}	
				
			if((hotterCtrl-> heated_times>100)&&(hotterCtrl->hotter->real_temperature <90) &&(__fabs (hotterCtrl->hotter->temperature_pre -hotterCtrl->hotter->real_temperature )<4))
			{		
				hotterCtrl-> state = TEMP_ALARM_READY ;
				hotterCtrl->hotter->hotter_err=1;
			} 
		}
		else
		{

			hotterCtrl->hotter->heated_check_dir=0; 
		}	
		
	}
 
	//温度上不去检查
	
	
	if(hotterCtrl ->input_ctrl ==YES)
	{
		//调温按键检查	
//		if(hotterCtrl->rotary->Spin_direction)
		{ 
			hotterCtrl-> state = TEMP_TARGET_SET_READY ;	
		}
		//调温按键检查	
		
		//校温按键检查	
		if ((hotterCtrl->button->continue_press_times>300)&&(hotterCtrl->hotter->work_state ==TEMP_BALANCE ))
		{
			 hotterCtrl->state = TEMP_ADJUST_WARNING_READY  ;
		}
	}

 
}


void TEMP_RESET_POSITION_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num = &hotterCtrl ->hotter ->real_temperature ;
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =DISABLE ;//不显示加热点
	//显示
	hotterCtrl ->state = TEMP_RESET_POSITION ;
}

void TEMP_RESET_POSITION_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(!hotterCtrl -> Is_reset_position)
	{
		hotterCtrl ->state = TEMP_CTRL_READY;
	}
//	if(hotterCtrl ->hotter->real_temperature<hotterCtrl ->hotter->sleep_temperature)
//	{
		//显示
		hotterCtrl -> tm1650 ->Is_num = NO ;
		hotterCtrl -> tm1650 ->word =(uint8_t *)SLP ;
		hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//跑灯
		hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//不闪
		hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	
		//显示
//	}
	
}



void TEMP_TARGET_SET_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl ->time100ms =0;//用于后面程序计时用
	hotterCtrl -> hotter->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =&hotterCtrl ->hotter->target_temperature; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	//显示
	hotterCtrl ->state = TEMP_TARGET_SET ;
}


void TEMP_TARGET_SET_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{

	
	//如果左旋，温度做减法运算
// 	if(hotterCtrl ->rotary->Spin_direction == Spin_left)
		
 		
	{
	    
//		hotterCtrl ->rotary->Spin_direction = no_direction ;
		if(--hotterCtrl ->hotter->target_temperature<=hotterCtrl ->hotter ->Lmin)
		{
			hotterCtrl ->hotter->target_temperature=hotterCtrl ->hotter ->Lmin;
		}
		hotterCtrl ->time100ms =0;
	}
	/*
	如果右旋，温度做加法运算
	*/
//	if(hotterCtrl ->rotary->Spin_direction == Spin_right)
	{

//		hotterCtrl ->rotary->Spin_direction = no_direction ;
		if(++hotterCtrl ->hotter->target_temperature >hotterCtrl ->hotter->Lmax )
		{
			hotterCtrl ->hotter->target_temperature =hotterCtrl ->hotter->Lmax ;
		}
		hotterCtrl ->time100ms =0;
	}
	/*
	如果确定，温度设定完成
	*/
	if(hotterCtrl ->button->Is_press == YES )
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL_READY ;
	}
	
//如果持续未按并且持续时间超过xx秒，温度设定完成
	if(hotterCtrl ->time100ms>25)
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL_READY ;
	}
 
}



void TEMP_ADJUST_WARNING_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
	//显示
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->word =(uint8_t*)CAL ; 
	hotterCtrl -> tm1650 ->dot_run_en =ENABLE  ;// 
	hotterCtrl -> tm1650 ->blink_en  = NO ;//不闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO ;//不显示加热点
	//显示
	hotterCtrl ->state = TEMP_ADJUST_WARNING ;
	
	hotterCtrl ->time100ms =0;
}


void TEMP_ADJUST_WARNING_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if (hotterCtrl ->time100ms >15)
	{ 		
		 hotterCtrl ->state = TEMP_ADJUST_READY ;
	}
 
}




void TEMP_ADJUST_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//显示
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =&hotterCtrl ->hotter ->adjust_temperature ; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE   ;//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;// 闪
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//不显示加热点
	//显示
	hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
//	hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
	hotterCtrl ->time100ms =0;//用于后面程序计时用
	hotterCtrl -> state = TEMP_ADJUST ;
}
void TEMP_ADJUST_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//如果左旋，温度做减法运算
//	if(hotterCtrl -> rotary->Spin_direction == Spin_left)
	{
		hotterCtrl ->time100ms =0;//用于后面程序计时用
//		hotterCtrl -> rotary->Spin_direction = no_direction;
		-- hotterCtrl ->hotter ->adjust_temperature ;
		if( hotterCtrl ->hotter ->adjust_temperature <hotterCtrl ->hotter ->Cmin) 
		{			
			hotterCtrl ->hotter ->adjust_temperature   =  hotterCtrl ->hotter ->Cmin;
		}
	}
	
	/*
	如果右旋，温度做加法运算
	*/
//	if(hotterCtrl -> rotary->Spin_direction == Spin_right)
	{	
		hotterCtrl ->time100ms  =0;//用于后面程序计时用
//		hotterCtrl -> rotary->Spin_direction = no_direction;
		++ hotterCtrl -> hotter ->adjust_temperature ;
		if( hotterCtrl -> hotter ->adjust_temperature  >hotterCtrl ->  hotter ->Cmax)  
		{
			hotterCtrl -> hotter ->adjust_temperature  =  hotterCtrl -> hotter ->Cmax;
		}
	}
	
	//如果确定，温度设定完成
	if(hotterCtrl -> button ->Is_press == YES )
	{
		FlshPara_Save();
//		hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
		FlshPara_Init();
		hotterCtrl -> state =TEMP_CTRL_READY ;
	}
		
//	如果持续未按并且持续时间超过xx秒
	if(hotterCtrl ->time100ms >50)
	{	
//		hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
		FlshPara_Init();
		hotterCtrl -> state = TEMP_CTRL_READY ;
	}
 	
}







void TEMP_ALARM_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{	
	//显示
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->dot_run_en =NO  ;	//不跑灯
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;//闪
	//显示
	
	hotterCtrl -> state = TEMP_ALARM ;
}
void TEMP_ALARM_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{

	if(hotterCtrl->hotter ->sensor_err )
	{
		hotterCtrl -> tm1650 ->word =(uint8_t *)S_E  ; 
	}
	else
	{
		hotterCtrl -> tm1650 ->word =(uint8_t *)H_E  ; 
	}

	
	if(hotterCtrl->hotter ->hotter_err )
	{
			
		hotterCtrl -> tm1650 ->bottom_dot_en =ENABLE ;//显示加热点
		hotterCtrl -> hotter ->heat_en(ENABLE );//加热
		if(hotterCtrl -> hotter->real_temperature  >90)
		{
			hotterCtrl ->  state =TEMP_CTRL_READY ;
		}
	}
	else
	{
		hotterCtrl -> tm1650 ->bottom_dot_en =DISABLE ;//不显示加热点
		hotterCtrl -> hotter ->heat_en(DISABLE );//不加热
		if(hotterCtrl -> hotter->real_adc <hotterCtrl -> hotter->sensor_err_adc )
		{
			hotterCtrl ->  state =TEMP_CTRL_READY ;
		}
	}

}


void TEMP_PowerOn_ISR(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl-> Is_power_on =  hotterCtrl ->power_on();
	if(hotterCtrl->state!=TEMP_IDLE)
	{
		if(!hotterCtrl-> Is_power_on)
		{
			hotterCtrl->state =TEMP_IDLE_READY ;
		}
	}
	
}

void TempResetPosition_ISR(HOTTER_CTRL_Typedef *hotterCtrl)
{

	hotterCtrl-> Is_reset_position = hotterCtrl ->reset ();
	if((hotterCtrl->state!= TEMP_IDLE )&&(hotterCtrl->state!= TEMP_IDLE_READY ))
	{	
		
		if(hotterCtrl->state!=TEMP_RESET_POSITION)
		{
			if(hotterCtrl->Is_reset_position)
			{
				hotterCtrl->state = TEMP_RESET_POSITION_READY;
			}
		}
	}	
	
		
} 


// 根据按键、温度判断等控制状态转换
void TempCtrl(HOTTER_CTRL_Typedef *hotterCtrl) 
{
	switch (hotterCtrl->state) 
{		
		case TEMP_CHECK_READY:
			TEMP_CHECK_READY_operate(hotterCtrl);
			break;
		
		case TEMP_CHECK:
			TEMP_CHECK_operate(hotterCtrl);
			break;
		
		case TEMP_IDLE_READY:
			TEMP_IDLE_READY_operate(hotterCtrl);
			break;	
		case TEMP_IDLE:
			TEMP_IDLE_operate(hotterCtrl);
			break;
		
		case TEMP_TARGET_SHOW_READY :
			TEMP_TARGET_SHOW_READY_operate(hotterCtrl);
			break ;
		
		case TEMP_TARGET_SHOW :
			TEMP_TARGET_SHOW_operate(hotterCtrl);
			break ;
		
		case TEMP_CTRL_READY:
			TEMP_CTRL_READY_operate(hotterCtrl);
			break;
		case TEMP_CTRL:
			TEMP_CTRL_operate(hotterCtrl);
			break;
		
		case TEMP_TARGET_SET_READY:
			 TEMP_TARGET_SET_READY_operate(hotterCtrl);
			break;		
		case TEMP_TARGET_SET:
			 TEMP_TARGET_SET_operate(hotterCtrl);
			break;
		
		case TEMP_ALARM_READY:
			TEMP_ALARM_READY_operate(hotterCtrl);
			break;			
		case TEMP_ALARM:
			TEMP_ALARM_operate(hotterCtrl);
			break;	 
	
		case TEMP_ADJUST_WARNING_READY :
			TEMP_ADJUST_WARNING_READY_operate(hotterCtrl);
			break ;		
		case TEMP_ADJUST_WARNING :
			TEMP_ADJUST_WARNING_operate(hotterCtrl);
			break ;
		
		case TEMP_ADJUST_READY:
			TEMP_ADJUST_READY_operate(hotterCtrl);
			break;		
		case TEMP_ADJUST:
			TEMP_ADJUST_operate(hotterCtrl);
			break;	
		case TEMP_RESET_POSITION_READY:
			TEMP_RESET_POSITION_READY_operate(hotterCtrl);
			break;		
		case TEMP_RESET_POSITION:
			TEMP_RESET_POSITION_operate(hotterCtrl);
			break;	
		default:
			break;
	}
	
}








HOTTER_CTRL_Typedef solder1321;
void Solder1321_init(void)	
{	
	//结构体初始化
	solder1321 .state =  TEMP_CHECK_READY ;//TEMP_CHECK_READY; 
	solder1321 .button = 	&button_1 ;
//	solder1321 .rotary =	&rotary_1 ;
	solder1321 .hotter =	&hotter1321 ;
	solder1321 .tm1650 =	&tm1650_2 ;
	//结构体初始化
	
	//滤波函数初始化
	
	
	//功能开启	

	//功能开启	
	solder1321.time100ms  = 0;
	solder1321.heated_times  =0 ;
	solder1321.Is_reset_position  = 0;
	solder1321.Is_power_on = 0;
	solder1321.input_ctrl =NO;
	solder1321.hotter ->heat_en(DISABLE );
}


void Solder1321PowerOn_ISR(void)
{
	//TEMP_PowerOn_ISR(&solder1321);
	static uint8_t a=0;
	static uint8_t b=0;
	a = solder1321 .hz_50count ;
	if(a ==b)
	{
		solder1321.Is_power_on = NO;
	}
	else
	{
		solder1321.Is_power_on = YES;
		b =a;
	}
	
	if(solder1321.state!=TEMP_IDLE)
	{
		if(!solder1321. Is_power_on)
		{
			solder1321.state =TEMP_IDLE_READY ;
		}
	}
	
}


void Solder1321Ctrl(void)
{
	TempCtrl(&solder1321);
}


void Solder1321HeatingTime_ISR(void) 
{
	TempTime_ISR(&solder1321 );
}














HOTTER_CTRL_Typedef airK;


uint8_t read_air_reset(void)
{
	return !HAL_GPIO_ReadPin(LAY2_GPIO_Port ,LAY2_Pin);
}

uint8_t read_air_power_on(void)
{
	return !HAL_GPIO_ReadPin(SW2_GPIO_Port ,SW2_Pin);
}




void AirK_init(void)	
{	
	
	airK .power_on =&read_air_power_on;
	airK.reset  =& read_air_reset;

	//结构体初始化
	airK .state =  TEMP_CHECK_READY  ; 
	airK .button = 	&button_1 ;
//	airK .rotary =	&rotary_1 ;

	airK .hotter =	&hotterK ;
	
	airK.time100ms  = 0;
	airK.heated_times  =0 ;
	airK.Is_reset_position  = 0;
	airK.Is_power_on = 0;
	airK.input_ctrl =YES;
	airK.hotter ->heat_en(DISABLE );
	//结构体初始化
	
	
	//滤波函数初始化

	
	 
	//功能开启	

	//功能开启

}
void AirKPowerOn_ISR(void)
{
//	if((airK.state!= TEMP_CHECK_READY  )&&(airK.state!= TEMP_CHECK ))
//	{
		TEMP_PowerOn_ISR(&airK );
//	}
}

void AirKCtrl(void)
{
	TempCtrl(&airK);
}


void FanCtrl_ISR(void)
{
	static uint8_t dir =0;
	if(airK .state == TEMP_IDLE )
	{
		HAL_GPIO_WritePin(SW2_GPIO_Port,SW2_Pin,GPIO_PIN_SET );
		dir = 1;
	}
	
	else
	{
		
		if(airK.Is_reset_position)
		{
			if(airK .hotter->real_temperature <airK .hotter ->sleep_temperature)
			{
				HAL_GPIO_WritePin(SW2_GPIO_Port,SW2_Pin,GPIO_PIN_SET );
			}
			else 
			{
				if (dir ==1)
				{
					HAL_GPIO_WritePin(SW2_GPIO_Port,SW2_Pin,GPIO_PIN_RESET );
					dir =0;
				}
			}
		}
		else
		{
			HAL_GPIO_WritePin(SW2_GPIO_Port,SW2_Pin,GPIO_PIN_RESET );
		}
	}
			
	
	
}


void AirKTime_ISR(void) 
{
	TempTime_ISR(&airK );
}



void AirKTempResetPosition_ISR(void)
{

	TempResetPosition_ISR(&airK );
}


void Control_ISR(void)
{
//	if(airK.Is_power_on &&solder1321 .Is_power_on )
//	{
		if((airK.state ==TEMP_CTRL) &&(solder1321 .state ==TEMP_CTRL ))
		{
			if(button_1 .Is_click )
			{
				button_1 .Is_click =0;
				airK.input_ctrl=!airK.input_ctrl;
				solder1321.input_ctrl=!solder1321.input_ctrl;
			}
		}
//	}
	if(airK.Is_power_on&&(solder1321 .Is_power_on ==NO))
	{
		airK.input_ctrl=YES;
		solder1321.input_ctrl=NO;
	}
	if(solder1321.Is_power_on&&(airK .Is_power_on ==NO))
	{
		airK.input_ctrl=NO;
		solder1321.input_ctrl=YES;
	}
	if (airK.Is_reset_position )
	{
		airK.input_ctrl=NO;
		solder1321.input_ctrl=YES;		
	}
}
