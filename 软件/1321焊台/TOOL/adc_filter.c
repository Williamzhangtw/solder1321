//2016-7-28

#include "adc_filter.h"
  #include "adc.h"
  #include "../bsp/hotter.h"

uint16_t                       aResultDMA[2];



uint16_t AvgFilter(AVG_FILTER_Struct *avg,uint16_t value)
{
   
  uint32_t sum =0;
	avg->count++ ;
  if ( avg->count >= avg->num)
    avg->count = 0 ;
 
  *(avg->array+avg->count)  =  value ;
	

   for (uint16_t i=0; i< avg->num; i++)
  {
    sum += *(avg->array+i);
  }
  avg-> f_value =  sum>>7;
  return  avg-> f_value;  
}


AVG_FILTER_Struct avg_1;
AVG_FILTER_Struct avg_2;
uint16_t avg_array_1[128];
uint16_t avg_array_2[128];
void Avg_1_Init(void)
{
	avg_1 .num =128;
	avg_1.array = avg_array_1;
	avg_1 .count =0;
	avg_1 .f_value = 0;
}
void Avg_2_Init(void)
{
	avg_2 .num =128;
	avg_2.array = avg_array_2;
	avg_2 .count =0;
	avg_2 .f_value = 0;
}

void TempAdcFilter_ISR (void)
{
//	uint16_t adc;
////	HAL_ADC_Start(&hadc) ;
////	HAL_ADC_PollForConversion(&hadc,10);  
//	adc = 80; 
 HAL_GPIO_TogglePin(LAY2_GPIO_Port ,LAY2_Pin  );
//	hotterK.real_adc  =AvgFilter (&avg_1 ,aResultDMA[0] );
//	hotter1321.real_adc  =AvgFilter (&avg_2 ,aResultDMA[1] );
}
