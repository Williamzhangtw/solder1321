/*            2016��10��30��
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
	hotterCtrl -> tm1650 ->Is_num = NO ;//��ʾ����
	hotterCtrl -> tm1650 ->word =(uint8_t*)CODE_Clean ;
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = NO ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	hotterCtrl ->time100ms =0;	//״̬����ʱ����
	hotterCtrl -> state = TEMP_CHECK  ;	
}


void TEMP_CHECK_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	if(hotterCtrl->time100ms >3)
	{

		
	}
}


void TEMP_IDLE_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl -> tm1650 ->Is_num = NO ;//��ʾ����
	hotterCtrl -> tm1650 ->word =(uint8_t*)OFF;//����OFF
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = NO ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	hotterCtrl -> state = TEMP_IDLE  ;	
}


void TEMP_IDLE_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{

	
}


void TEMP_TARGET_SHOW_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl ->time100ms =0;//���ں�������ʱ��
	//��ʾ
	hotterCtrl->tm1650->Is_num = YES ;//��ʾ����
	hotterCtrl->tm1650->num = &hotterCtrl->hotter->target_temperature ;//���ݣ�Ŀ���¶�
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl->tm1650->blink_en  =YES ;//��˸
	hotterCtrl->tm1650->bottom_dot_en =DISABLE ;//����ʾ���ȵ�
	//��ʾ
	hotterCtrl->state = TEMP_TARGET_SHOW ;
	 
}


void TEMP_TARGET_SHOW_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//1.5S�����CTRL
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
	//��ʾ
	hotterCtrl->tm1650->Is_num = YES ;//��ʾ��������
	hotterCtrl->tm1650->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl->tm1650->blink_en  =DISABLE ;	// 
	//��ʾ
	hotterCtrl ->time100ms =0;//���ں�������ʱ��
	hotterCtrl->state = TEMP_CTRL ;  
}


void TEMP_CTRL_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	
	//��ʾ����
	if(hotterCtrl->hotter->work_state ==TEMP_BALANCE )
	{
		hotterCtrl->tm1650->num = &hotterCtrl->hotter->target_temperature;
	}
	else 
	{
		hotterCtrl->tm1650->num =&hotterCtrl-> hotter->real_temperature;
	}
	//��ʾ����
	
	{

	hotterCtrl->hotter->minus_now = hotterCtrl-> hotter->real_temperature -hotterCtrl->hotter->target_temperature ;  
	switch(hotterCtrl->hotter->work_state )
	{
		case HEATTING : //����
			if (hotterCtrl-> hotter->minus_now  >=0 )
			{
				 hotterCtrl->hotter->work_state =OVER_HEAT ;
			
			}
			break ;

		case OVER_HEAT  : //����

			if (  hotterCtrl->hotter->minus_now  <0 )
			{
				
				hotterCtrl-> hotter->work_state =TEMP_BALANCE ;
			}
			break ;
			
		case NOT_HEAT : 	//ֹͣ����
			if ( hotterCtrl-> hotter->minus_now  <=0 )
			{

				hotterCtrl->hotter->work_state  = TEMP_BALANCE ;
			}
			break ;
			
			
		case TEMP_BALANCE  ://ƽ��
			if ( hotterCtrl->hotter->minus_now  >hotterCtrl-> hotter->temp_distinguish_dif)
				hotterCtrl->hotter->work_state  = NOT_HEAT  ; 
			if ( hotterCtrl-> hotter->minus_now  < -hotterCtrl->hotter->temp_distinguish_dif )
				hotterCtrl->hotter->work_state  = HEATTING  ; 	
			break;
		
		default :
			break ;
	}
	}
	
	
 	//���ȿ���
	if(hotterCtrl->hotter->real_adc	<	hotterCtrl->hotter->sensor_err_adc)
	{
		if( hotterCtrl->hotter->real_temperature	<	hotterCtrl->hotter->target_temperature)
		{
			hotterCtrl->tm1650->bottom_dot_en =ENABLE  ;//��ʾ������
		}
		else
		{
			hotterCtrl->tm1650 ->bottom_dot_en =DISABLE  ;//��ʾ������
		}
	}
	else //ADC������Χ S-E
	{
		
		hotterCtrl->hotter->sensor_err =1;
		hotterCtrl->state = TEMP_ALARM_READY ; 
	}
	//���ȿ���	
//	
	//�¶��ϲ�ȥ���
	if(hotterCtrl->hotter->heated_check_dir == 0 )
	{
		if(	hotterCtrl->hotter->work_state==HEATTING )
		{
			hotterCtrl->hotter->temperature_pre = hotterCtrl->hotter->real_temperature;//��¼��ǰʵʱ�¶�
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
 
	//�¶��ϲ�ȥ���
	
	

	{
		//���°������	
//		if(hotterCtrl->rotary->Spin_direction)
		{ 
			hotterCtrl-> state = TEMP_TARGET_SET_READY ;	
		}
		//���°������	
		
		//У�°������	
		if ((hotterCtrl->button_set->continue_press_times>300)&&(hotterCtrl->hotter->work_state ==TEMP_BALANCE ))
		{
			 hotterCtrl->state = TEMP_ADJUST_WARNING_READY  ;
		}
	}

 
}


void TEMP_RESET_POSITION_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num = &hotterCtrl ->hotter ->real_temperature ;
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =DISABLE ;//����ʾ���ȵ�
	//��ʾ
	hotterCtrl ->state = TEMP_RESET_POSITION ;
}

void TEMP_RESET_POSITION_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{

	
//	if(hotterCtrl ->hotter->real_temperature<hotterCtrl ->hotter->sleep_temperature)
//	{
		//��ʾ
		hotterCtrl -> tm1650 ->Is_num = NO ;
		hotterCtrl -> tm1650 ->word =(uint8_t *)SLP ;
		hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//�ܵ�
		hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//����
		hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	
		//��ʾ
//	}
	
}



void TEMP_TARGET_SET_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	hotterCtrl ->time100ms =0;//���ں�������ʱ��
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =&hotterCtrl ->hotter->target_temperature; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  =DISABLE  ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	//��ʾ
	hotterCtrl ->state = TEMP_TARGET_SET ;
}


void TEMP_TARGET_SET_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{

	
	//����������¶�����������
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
	����������¶����ӷ�����
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
	���ȷ�����¶��趨���
	*/
	if(hotterCtrl ->button_set->Is_press == YES )
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL_READY ;
	}
	
//�������δ�����ҳ���ʱ�䳬��xx�룬�¶��趨���
	if(hotterCtrl ->time100ms>25)
	{
		FlshPara_Save();
		hotterCtrl ->state = TEMP_CTRL_READY ;
	}
 
}



void TEMP_ADJUST_WARNING_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->word =(uint8_t*)CAL ; 
	hotterCtrl -> tm1650 ->dot_run_en =ENABLE  ;// 
	hotterCtrl -> tm1650 ->blink_en  = NO ;//����
	hotterCtrl -> tm1650 ->bottom_dot_en =NO ;//����ʾ���ȵ�
	//��ʾ
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
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = YES ;
	hotterCtrl -> tm1650 ->num =&hotterCtrl ->hotter ->adjust_temperature ; 
	hotterCtrl -> tm1650 ->dot_run_en =DISABLE   ;//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;// ��
	hotterCtrl -> tm1650 ->bottom_dot_en =NO;//����ʾ���ȵ�
	//��ʾ
//	hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
	hotterCtrl ->time100ms =0;//���ں�������ʱ��
	hotterCtrl -> state = TEMP_ADJUST ;
}
void TEMP_ADJUST_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{
	//����������¶�����������
//	if(hotterCtrl -> rotary->Spin_direction == Spin_left)
	{
		hotterCtrl ->time100ms =0;//���ں�������ʱ��
//		hotterCtrl -> rotary->Spin_direction = no_direction;
		-- hotterCtrl ->hotter ->adjust_temperature ;
		if( hotterCtrl ->hotter ->adjust_temperature <hotterCtrl ->hotter ->Cmin) 
		{			
			hotterCtrl ->hotter ->adjust_temperature   =  hotterCtrl ->hotter ->Cmin;
		}
	}
	
	/*
	����������¶����ӷ�����
	*/
//	if(hotterCtrl -> rotary->Spin_direction == Spin_right)
	{	
		hotterCtrl ->time100ms  =0;//���ں�������ʱ��
//		hotterCtrl -> rotary->Spin_direction = no_direction;
		++ hotterCtrl -> hotter ->adjust_temperature ;
		if( hotterCtrl -> hotter ->adjust_temperature  >hotterCtrl ->  hotter ->Cmax)  
		{
			hotterCtrl -> hotter ->adjust_temperature  =  hotterCtrl -> hotter ->Cmax;
		}
	}
	
	//���ȷ�����¶��趨���
	if(hotterCtrl -> button_set ->Is_press == YES )
	{
		FlshPara_Save();
//		hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
		FlshPara_Init();
		hotterCtrl -> state =TEMP_CTRL_READY ;
	}
		
//	�������δ�����ҳ���ʱ�䳬��xx��
	if(hotterCtrl ->time100ms >50)
	{	
//		hotterCtrl ->hotter ->adjust_temperature =hotter_flash.adjust_temperature; 
		FlshPara_Init();
		hotterCtrl -> state = TEMP_CTRL_READY ;
	}
 	
}







void TEMP_ALARM_READY_operate(HOTTER_CTRL_Typedef *hotterCtrl)
{	
	//��ʾ
	hotterCtrl -> tm1650 ->Is_num = NO ;
	hotterCtrl -> tm1650 ->dot_run_en =NO  ;	//���ܵ�
	hotterCtrl -> tm1650 ->blink_en  = ENABLE ;//��
	//��ʾ
	
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
			
		hotterCtrl -> tm1650 ->bottom_dot_en =ENABLE ;//��ʾ���ȵ�
		if(hotterCtrl -> hotter->real_temperature  >90)
		{
			hotterCtrl ->  state =TEMP_CTRL_READY ;
		}
	}
	else
	{
		hotterCtrl -> tm1650 ->bottom_dot_en =DISABLE ;//����ʾ���ȵ�
		if(hotterCtrl -> hotter->real_adc <hotterCtrl -> hotter->sensor_err_adc )
		{
			hotterCtrl ->  state =TEMP_CTRL_READY ;
		}
	}

}







// ���ݰ������¶��жϵȿ���״̬ת��
void TempCtrl(HOTTER_CTRL_Typedef *hotterCtrl) 
{
	switch (hotterCtrl->state) 
{		
		case TEMP_CHECK_READY://�¶ȼ�� ׼���׶�
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
	//�ṹ���ʼ��
	solder1321 .state =  TEMP_CHECK_READY ;//TEMP_CHECK_READY; 
	solder1321 .button_set 	= 	&button_set;
	solder1321 .button_up 	= 	&button_up;
	solder1321 .button_down = 	&button_down;
//	solder1321 .rotary =	&rotary_1 ;
	solder1321 .hotter =	&hotter1321 ;
	solder1321 .tm1650 =	&tm1650_2 ;
	//�ṹ���ʼ��
	
	//�˲�������ʼ��
	
	
	//���ܿ���	

	//���ܿ���	
	solder1321.time100ms  = 0;
	solder1321.heated_times  =0 ;



	
}


void Solder1321PowerOn_ISR(void)
{
	//TEMP_PowerOn_ISR(&solder1321);
	static uint8_t a=0;
	static uint8_t b=0;
	a = solder1321 .hz_50count ;
	if(a ==b)
	{

	}
	else
	{

		b =a;
	}
	
	if(solder1321.state!=TEMP_IDLE)
	{

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

