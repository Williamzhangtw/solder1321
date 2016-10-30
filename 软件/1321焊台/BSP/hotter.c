//verify date:2016/7/28pm
#include "hotter.h"

/* USER CODE BEGIN Includes */

 #include "../tool/adc_filter.h"
 #include "../tool/flash.h"

 
#if 0
void HotterWorkingState_ISR(HOTER_CTRL_TypeDef * hotter)
{

	hotter->minus_now =  hotter->real_temperature -hotter->target_temperature ;  
	switch(hotter->work_state )
	{
		case HEATTING : //����
			if ( hotter->minus_now  >=0 )
			{
				 hotter->work_state =OVER_HEAT ;
			
			}
			break ;

		case OVER_HEAT  : //����

			if (  hotter->minus_now  <0 )
			{
				
				 hotter->work_state =TEMP_BALANCE ;
			}
			break ;
			
		case NOT_HEAT : 	//ֹͣ����
			if (  hotter->minus_now  <=0 )
			{

				hotter->work_state  = TEMP_BALANCE ;
			}
			break ;
			
			
		case TEMP_BALANCE  ://ƽ��
			if (  hotter->minus_now  > hotter->temp_distinguish_dif)
				hotter->work_state  = NOT_HEAT  ; 
			if (  hotter->minus_now  < -hotter->temp_distinguish_dif )
				hotter->work_state  = HEATTING  ; 	
			break;
		
		default :
			break ;
	}
}
#endif








void HotterRealTemp_ISR(HOTER_CTRL_TypeDef * hotter)
{
	hotter->real_temperature   = hotter->real_adc*hotter->Ks+hotter->Bs- hotter ->adjust_temperature ; 
}

 
HOTER_CTRL_TypeDef hotter1321 ;
/*heated
*/
void Heat_hotter1321(BOOL  en)
{
  if(en)// heated
  {
    HAL_GPIO_WritePin(SolderHOT_GPIO_Port,SolderHOT_Pin,GPIO_PIN_RESET);
  }
  else // not heated
  {
    HAL_GPIO_WritePin(SolderHOT_GPIO_Port,SolderHOT_Pin,GPIO_PIN_SET);
  }
}

/* get adc
*/
void Hotter1321RealTemp_ISR (void)
{
	HotterRealTemp_ISR(&hotter1321);
}


void Hotter1321_init(void)
{

	hotter1321 .Ks = 0.3105;
	hotter1321 .Bs =-157;
	hotter1321.Lmax =480;
	hotter1321.Lmin =200,
	hotter1321.Cmin =-99;
	hotter1321.Cmax =99;
	hotter1321.real_adc = 0;
	hotter1321.real_temperature = 25;
	hotter1321.target_temperature = 300;
	hotter1321.adjust_temperature = 0;

	
	hotter1321 .sensor_err_adc = 2300;
 
	
	hotter1321.sensor_err =0;
	hotter1321.hotter_err =0;
	hotter1321.work_state =NOT_HEAT ;
 
	hotter1321 .minus_pre =0;
	hotter1321 .minus_now =0;
	hotter1321 .temp_distinguish_dif =10;
	hotter1321.heat_en  = Heat_hotter1321;


//����ִ�к���
	hotter1321.heat_en (DISABLE );
}

 







HOTER_CTRL_TypeDef hotterK ;
/*heated
*/
void Heat_hotterK(BOOL  en)
{
  if(en)// heated
  {
    HAL_GPIO_WritePin(AirHOT_GPIO_Port,AirHOT_Pin,GPIO_PIN_RESET);
  }
  else // not heated
  {
    HAL_GPIO_WritePin(AirHOT_GPIO_Port,AirHOT_Pin,GPIO_PIN_SET);
  }
}




void HotterKRealTemp_ISR (void)
{
	HotterRealTemp_ISR(&hotterK);
}


void HotterK_init(void)
{

	//�ײ�
	hotterK.heat_en  = Heat_hotterK;
	
	//����
	hotterK.real_adc = 0;	
	hotterK.real_temperature = 25;
	hotterK.sensor_err =0;
	hotterK.hotter_err =0;
	hotterK.work_state =HEATTING  ;
 	

	//���Ʋ���
	hotterK .Ks = 0.2557;
	hotterK .Bs =-7;
	hotterK.Lmax =480;
	hotterK.Lmin =100,
	hotterK.Cmin =-99;
	hotterK.Cmax =99;
	hotterK.target_temperature = 300;
	hotterK.adjust_temperature = 0;
	
	hotterK.sleep_temperature =90;
	
	hotterK .sensor_err_adc = 2300;
	hotterK .temp_distinguish_dif =20; 
	//���ݷ���buf
	hotterK .minus_pre =0;
	hotterK .minus_now =0;
	hotterK .temperature_pre =0;
	hotterK .heated_check_dir =0;
	
//����ִ�к���
	hotterK.heat_en (DISABLE );//������
}




 

 



