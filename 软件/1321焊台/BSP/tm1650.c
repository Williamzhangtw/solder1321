/*2016-10-28AM
*/
#include "../bsp/tm1650.h"
//从右往左显示
const		uint8_t 	CODE00[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};// 0-9
const		uint8_t		CODE_Clean[4]={0x00,0x00,0x00};//黑屏
const		uint8_t		CODE01[4]={0x40,0x40,0x40};//" - - - "
const 	uint8_t  	S_E[4]={0x79,0x40,0x6D};//" S - E(0xF5) "传感器错误 sensor error
const 	uint8_t  	H_E[4]={0x79,0x40,0x76};//" H - E(0xF5) "发热芯错误 hotter error
const   uint8_t 	SLP[4]={0x73,0x38,0x6D};//休眠 
const 	uint8_t  	OFF[4]={0x71,0x71,0x3F};//" OFF "关机
const 	uint8_t  	CAL[4]={0x38,0x77,0x39};//" CAL "校准
const 	uint8_t  	MENU01[4]={0x06,0x3F,0x5C,};//"-01"
const		uint8_t  	MENU02[4]={0x5B,0x3F,0x5C,};//"-02"
const 	uint8_t  	MENU03[4]={0x4F,0x3F,0x5C,};//"-03"
const 	uint8_t  	MENU04[4]={0x66,0x3F,0x5C,};//"-04"
const		uint8_t  	MENU05[4]={0x6D,0x3F,0x5C,};//"-05"
const 	uint8_t  	MENU06[4]={0x7D,0x3F,0x5C,};//"-06"
const 	uint8_t  	MENU07[4]={0x07,0x3F,0x5C,};//"-07"
const 	uint8_t  	MENU08[4]={0x7F,0x3F,0x5C,};//"-08"
const 	uint8_t  	MENU09[4]={0x6F,0x3F,0x5C,};//"-09"
const 	uint8_t  	MENU10[4]={0x3F,0x06,0x5C,};//"-10"
const 	uint8_t  	_1_[4]={0x40,0x06,0x40};//"-1-"
const 	uint8_t  	_2_[4]={0x40,0x5B,0x40};//"-2-"
const 	uint8_t 	_888[4] = {0x7F,0x7F,0x7F};//"888"
const 	uint8_t 	xianfu[4] = {0x09,0x09,0x09};//二二二
const 	uint8_t 	hui[4] = {0x39,0x09,0x0f};//口


/************ START信号*******************************/
void FD650_START(TM1650_STRUCT *tm1650)
{
	HAL_GPIO_WritePin(tm1650 ->CLK_GPIO_PORT,tm1650 ->CLK_GPIO_PIN,GPIO_PIN_SET);  
	HAL_GPIO_WritePin(tm1650 ->DIO_GPIO_PORT,tm1650 ->DIO_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(tm1650 ->DIO_GPIO_PORT,tm1650 ->DIO_GPIO_PIN,GPIO_PIN_RESET);    
	HAL_GPIO_WritePin(tm1650 ->CLK_GPIO_PORT,tm1650 ->CLK_GPIO_PIN,GPIO_PIN_RESET);
}
/************ STOP信号********************************/
void FD650_STOP(TM1650_STRUCT *tm1650)
{
	HAL_GPIO_WritePin(tm1650 ->CLK_GPIO_PORT,tm1650 ->CLK_GPIO_PIN,GPIO_PIN_SET);  
	HAL_GPIO_WritePin(tm1650 ->DIO_GPIO_PORT,tm1650 ->DIO_GPIO_PIN,GPIO_PIN_RESET); 
	HAL_GPIO_WritePin(tm1650 ->DIO_GPIO_PORT,tm1650 ->DIO_GPIO_PIN,GPIO_PIN_SET);
}
/************写1个字节给TM1650***************************/
void FD650_write( TM1650_STRUCT *tm1650,uint8_t data1)
{
 	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(data1&0x80)
			HAL_GPIO_WritePin(tm1650 ->DIO_GPIO_PORT,tm1650 ->DIO_GPIO_PIN,GPIO_PIN_SET);
		else 
			HAL_GPIO_WritePin(tm1650 ->DIO_GPIO_PORT,tm1650 ->DIO_GPIO_PIN,GPIO_PIN_RESET);
		
		HAL_GPIO_WritePin(tm1650 ->CLK_GPIO_PORT,tm1650 ->CLK_GPIO_PIN,GPIO_PIN_SET); 
		data1<<=1;
		HAL_GPIO_WritePin(tm1650 ->CLK_GPIO_PORT,tm1650 ->CLK_GPIO_PIN,GPIO_PIN_RESET);
	}
		HAL_GPIO_WritePin(tm1650 ->DIO_GPIO_PORT,tm1650 ->DIO_GPIO_PIN,GPIO_PIN_SET);
		HAL_GPIO_WritePin(tm1650 ->CLK_GPIO_PORT,tm1650 ->CLK_GPIO_PIN,GPIO_PIN_SET);  
		HAL_GPIO_WritePin(tm1650 ->CLK_GPIO_PORT,tm1650 ->CLK_GPIO_PIN,GPIO_PIN_RESET);
}


/***********发送命令信号***********************************/
void FD650_send(TM1650_STRUCT *tm1650,uint8_t date1,uint8_t date2)
{
 	FD650_START(tm1650);
	FD650_write(tm1650,date1);
	FD650_write(tm1650,date2);
	FD650_STOP(tm1650);
}

void Tm1650_show_ISR(TM1650_STRUCT *tm1650)
{
	if(tm1650->blink_en && (tm1650->blink_dir ==0 ))
	{

		tm1650->word =(uint8_t *)CODE_Clean ;
		tm1650->disp_value[0] = *tm1650->word ++;
		tm1650->disp_value[1] = *tm1650->word ++;
		tm1650->disp_value[2] = *tm1650->word ++;
		tm1650->disp_value[3] = *tm1650->word ;
	}

	else
	{
		if(tm1650->Is_num ==YES)//整数显示
		{
			if(*tm1650->num>=0)
			{
				tm1650->disp_value[0] = CODE00[ *tm1650->num %10];
				tm1650->disp_value[1] = CODE00[ *tm1650->num /10%10];
				tm1650->disp_value[2] = CODE00[ *tm1650->num /100%10];
				tm1650->disp_value[3] = CODE00[ *tm1650->num /1000%10];
			}
			else//负数显示
			{
				tm1650->num_fab = __fabs (*tm1650->num);
				tm1650->disp_value[0]	=	CODE00[tm1650->num_fab%10];
				tm1650->disp_value[1]	= CODE00[tm1650->num_fab/10%10];
				tm1650->disp_value[2] =  CODE00[tm1650->num_fab/100%10];
				tm1650->disp_value[3] =0x40;
			}
		}
		else//文字显示
		{
			tm1650->word =  (uint8_t *)tm1650->word;
			tm1650->disp_value[0] = *tm1650->word ;
			tm1650->disp_value[1] = *(tm1650->word +1);
			tm1650->disp_value[2] = *(tm1650->word+2) ;
			tm1650->disp_value[3] = *(tm1650->word+3) ;
		}

	}
	tm1650->times_100ms_inside++;
	if(tm1650->times_100ms_inside%3==0)
	{
		if(tm1650->blink_dir==0)
			tm1650->blink_dir=1;
		else
			tm1650->blink_dir=0;
	}



	if(tm1650->bottom_dot_en )
	{
		tm1650->disp_value[0] |=0x80;
	}
	else
	{
		if(tm1650->dot_run_en )
		{
			if(tm1650->times_100ms_inside %2==0)
			{
				if((tm1650->dot_run_bit++)>2)
				tm1650->dot_run_bit=0;
			}
			tm1650->disp_value [tm1650->dot_run_bit]|=0x80;
		}
	}


	FD650_send(tm1650,0X68,tm1650->disp_value[1] ); //GID1  3   1
	FD650_send(tm1650,0X6A,tm1650->disp_value[2]);  //GID2  2   2
	FD650_send(tm1650,0X6C,tm1650->disp_value[3]);  //GID3  1   3
	FD650_send(tm1650,0X6E,tm1650->disp_value[0]);  //GID4  0   0

}




TM1650_STRUCT tm1650_2;



void Tm1650_2_init(void)
{

	tm1650_2.CLK_GPIO_PORT = TM1651_CLK_2_GPIO_Port;
	tm1650_2.CLK_GPIO_PIN = TM1651_CLK_2_Pin;
	tm1650_2.DIO_GPIO_PORT = TM1651_DIO_2_GPIO_Port;
	tm1650_2.DIO_GPIO_PIN = TM1651_DIO_2_Pin;
	tm1650_2 .blink_dir =0;
	tm1650_2 .dot_run_bit=0;
	//函数
	FD650_send(&tm1650_2,0x48,0x11);//设定8段显示，设定显示亮度0xab;a(1-7-0亮度依次递增)
}


void Tm1650_2_show_ISR(void)
{
	Tm1650_show_ISR(&tm1650_2);
}










