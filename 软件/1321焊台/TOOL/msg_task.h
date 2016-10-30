#ifndef __MSG_TASK_API_H_
#define __MSG_TASK_API_H_

#include "stm32f0xx_hal.h"

#define  ArrayNum(a)     (sizeof(a)/sizeof(a[0]))

/**************delayms_task*****************************/

//1 定义消息
typedef enum          //在这里定义系统消息
{
	Tm1650_1_show_MSG =0,
	Tm1650_2_show_MSG,	
	Button_1_MSG,
	Rotary_1_scan_MSG,   
	Hotter1321AdcFilter_MSG,
	Hotter1321PowerOnScan_MSG,
	Hotter1321RealTemp_MSG,
	Hotter1321WorkingState_MSG,
	
	
	
	TempAdcFilter_MSG,
	HotterKPowerOnScan_MSG,
	HotterKRealTemp_MSG,
	HotterKWorkingState_MSG,
	HotterKHeatTime_MSG,
	HotterKResetPosition_MSG,
	
	Solder1321PowerOn_MSG,
	Solder1321HeatingTime_msg,
	
	
	AirKPowerOn_MSG,
	AirKTime_MSG,
	AirKTempResetPosition_MSG,	
	
	FanCtrl_MSG,
	MsgDelaymsLast,
	
	Control_MSG,
	
	
}MsgDelaymsID;


////2.定义 消息参数体


typedef struct  
{
	uint8_t			id;
	uint8_t			Is_enable;//控制该消息任务是否被执行
	uint8_t			Is_ready;//控制该消息 是否时间到了 ，可以执行了
	uint16_t		time;	
	uint16_t		time_temp;
	void (*funproc)(void);
}Msg_TypeDef;



typedef struct  
{
	Msg_TypeDef *msg;
	uint8_t id_num;
}Task_TypeDef;



//消息体函数
void Msg_delayms_process(void);
void TaskISR(Task_TypeDef * task);
//extern void TaskCtrl(Task_TypeDef * task,uint16_t id,uint8_t en);
void TaskProcess( Task_TypeDef * task);

////主函数消息
//typedef enum          //在这里定义系统消息
//{
//	SOLDER1321_MSG=0,
//	AIRK_MSG,
//	TASK_SYSTICK_MSG,
//}MsgMainID;
//void TaskMainkInit(void);

extern Msg_TypeDef msg_systick[];

extern Task_TypeDef task_systick ;



extern void TaskSystickInit(void);

//extern Task_TypeDef task_main;
//extern void TaskSystickProcess(void);
//void Msg_main_process(void);
//void mainTask_ISR(void);



extern void HAL_SYSTICK_Callback(void);
extern BOOL HalTimeDelay (uint8_t *p,uint16_t count,BOOL *data_reset);
extern BOOL HalTimeDelaySimple (uint8_t *p,uint8_t time_count);
extern void TaskCtrl(Task_TypeDef * task,uint16_t id,uint8_t en);












#endif
