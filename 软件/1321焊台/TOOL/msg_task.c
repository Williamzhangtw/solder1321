/*2016-11-26
*/
#include "../tool/msg_task.h"

//���ܺ��� �ӿ�interface//
#include "../bsp/tm1650.h"
#include "../cantor/hotterctrl.h"
#include "../bsp/hotter.h"
#include "../tool/adc_filter.h"

void TaskISR(Task_TypeDef * task)//��ѯ��ʽ���д���
{
	for(uint16_t i=0;i<task->id_num;i++) //����������Ϣ�ṹ��
	{
		if(task ->msg[i].Is_enable == YES)
		{
			--task ->msg[i].time;
			if(task ->msg[i].time <1)//��ԭ�����е� ��ʱ����
			{
				task ->msg[i].Is_ready = YES;
				task ->msg[i].time= task ->msg[i].time_temp ;
			}
		}
		//task ->msg++;
	}	 
	//task ->msg=task ->msg - 120;
}

void TaskCtrl(Task_TypeDef * task,uint16_t id,uint8_t en)
{
	  
	for(uint16_t i=0; i < task->id_num  ;i++) 
	{
		if(task ->msg[i].id   == id)
		{
			task ->msg[i].Is_enable =en;return ;
		}
	}
	//task ->msg=task ->msg - sizeof(Msg_TypeDef)*task->id_num;
}


void TaskProcess( Task_TypeDef * task)
{
	for(uint16_t i=0; i< task->id_num  ;i++) 
	{
		if(task ->msg[i].Is_ready ==YES)
		{	
			task ->msg[i].Is_ready = NO;	
			if (task ->msg[i].funproc!=NULL)
			{
				task ->msg[i].funproc();					
			}
		}
//		task ->msg=task ->msg+12;//sizeof(Msg_TypeDef);
	//	task ->msg++;
	}
	//	task ->msg=task ->msg - 120;
}


Task_TypeDef task_systick;


Msg_TypeDef msg_systick[] =      //��Ϣ����Ϣ������
{	
	//id										Is_enable,	Is_ready,	time,		time_temp,		(*funproc)
	{Tm1650_2_show_MSG, 					NO,	NO,				100,		500,					&Button_1_ISR},
	{Button_1_MSG , 							YES,	NO,				50,		100,					&Tm1650_2_show_ISR},	
	{TempAdcFilter_MSG , 					NO,		NO,				1,			500,						NULL},

	{AirKPowerOn_MSG,							NO,		NO,				100,		0,						NULL},	
	{AirKTempResetPosition_MSG,		YES,		NO,				100,		500,						NULL},
	{AirKTime_MSG,								NO,		NO,				100,		500,						Button_1_ISR},
{Hotter1321RealTemp_MSG,			NO,		NO,				1,			0,					NULL},
	{Solder1321PowerOn_MSG,				YES,		NO,				100,		500,				NULL		},
	{Solder1321HeatingTime_msg,		NO,		NO,				100,		0,						NULL},
	{HotterKRealTemp_MSG,					YES,		NO,				1,			100,						Button_1_ISR},
//	{FanCtrl_MSG ,								NO,		NO,				100,		0,						&FanCtrl_ISR},
//	{Control_MSG,									NO,		NO,				10,			0,						&Control_ISR},
};



void TaskSystickInit(void)
{
	task_systick.msg = msg_systick;
	task_systick.id_num =   ArrayNum(msg_systick);
}



void HAL_SYSTICK_Callback()
{
	static	uint8_t 	StaticFlag_1ms		=0;
	static	uint8_t 	StaticFlag_10ms		=0;
	static	uint8_t 	StaticFlag_100ms	=0;
//  
	TaskISR(&task_systick);
	StaticFlag_1ms++;
  if (StaticFlag_1ms == 10) 
  { 
    StaticFlag_1ms =0;
    StaticFlag_10ms ++;  
  /* USER CODE  10ms  */
  }
  
  if (StaticFlag_10ms == 10) 
  { 
   StaticFlag_10ms =0;
   StaticFlag_100ms ++; 
//Tm1650_2_show_ISR();
		// 
    /* USER CODE 100ms */    
 
  }
  if (StaticFlag_100ms ==10)
  {
    StaticFlag_100ms =0;
		
		//HAL_GPIO_TogglePin(LAY2_GPIO_Port ,LAY2_Pin  );
    /* USER CODE 1s */
  }
}

