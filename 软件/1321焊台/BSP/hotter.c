//verify date:2016年10月30日
#include "../bsp/hotter.h"

/* USER CODE BEGIN Includes */
#include "../tool/adc_filter.h"
#include "../tool/flash.h"

 

void HotterWorkingState_ISR(HOTER_CTRL_TypeDef * hotter)
{
	hotter->minus_now =  hotter->real_temperature -hotter->target_temperature ;  
	switch(hotter->work_state )
	{
		case HEATTING : //加热
			if ( hotter->minus_now  >=0 )
			{
				 hotter->work_state =OVER_HEAT ;
			
			}
			break ;

		case OVER_HEAT  : //过热
			if (  hotter->minus_now  <0 )
			{
				 hotter->work_state =TEMP_BALANCE ;
			}
			break ;
			
		case NOT_HEAT : 	//停止加热
			if (  hotter->minus_now  <=0 )
			{
				hotter->work_state  = TEMP_BALANCE ;
			}
			break ;
			
		case TEMP_BALANCE  ://平衡
			if (  hotter->minus_now  > hotter->temp_distinguish_dif)
				hotter->work_state  = NOT_HEAT  ; 
			if (  hotter->minus_now  < -hotter->temp_distinguish_dif )
				hotter->work_state  = HEATTING  ; 	
			break;
		
		default :
			break ;
	}
}

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
    HAL_GPIO_WritePin(hotter1321.GPIO_PORT,hotter1321.GPIO_PIN,GPIO_PIN_RESET);
  }
  else // not heated
  {
    HAL_GPIO_WritePin(hotter1321.GPIO_PORT,hotter1321.GPIO_PIN,GPIO_PIN_SET);
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
	hotter1321 .GPIO_PORT = SolderHOT_GPIO_Port ;
	hotter1321 .GPIO_PIN =	SolderHOT_Pin ;
	
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


//开机执行函数

}

 




