 
#include "pid.h"

//  全局变量




/*====================================================================================================/
PID 
=====================================================================================================*/
PID_TypeDef   pid_1={0,0,0,0,0,0,15,2,1,0};


/*******************************************************************************
函数名: Pid_Realize 
函数作用: PID自整定加热时间
输入参数:temp当前温度
返回参数:返回数据单位/50us 
*******************************************************************************/
uint16_t Pid_Realize(int16_t  temp)
{
    uint8_t integral_en = 0,dError_en = 1;
    

    pid_1.actualtemp   = temp;
    pid_1.err          = pid_1.settemp - pid_1.actualtemp;    //温差

		if(pid_1.err >= 500)  integral_en = 0;//实际需求，在刚开始加热过程中 积分项容易超标，让它复原
    else 
		{
        integral_en = 1;
        pid_1.integral    += pid_1.err;//积分项
    }
////////////////////////////////////////////////////////////////////////////////
//降温去积分
    if(pid_1.settemp < pid_1.actualtemp) 
		{
//			int16_t negative_err = 0;
//      negative_err = pid_1.actualtemp - pid_1.settemp;
//      if(negative_err > 20)
			{
				pid_1.integral = 0; //过冲5℃
				dError_en = 0; //清空 积分项，微分项
				integral_en = 0;
      }
    }
////////////////////////////////////////////////////////////////////////////////
    if(pid_1.err <= 30) dError_en = 0;
    else dError_en = 1;//掉温慢就去除 微分项
		
    pid_1.ht_time     = pid_1.kp * pid_1.err + pid_1.ki * integral_en * pid_1.integral + pid_1.kd * (pid_1.err - pid_1.err_last)*dError_en;
   
//标准的PID计算公式 PID计数值 =  比例项 + 积分项 + 微分项
		 pid_1.err_last    = pid_1.err;//
		
    if(pid_1.ht_time <= 0)          pid_1.ht_time = 0;
		
    else if(pid_1.ht_time > 30*200) pid_1.ht_time = 30*200;
    
    return pid_1.ht_time;

}

/*******************************************************************************
函数名: Heating_Time 
函数作用: 计算加热标志，返回加热时间
输入参数:temp当前温度，wk_temp 工作温度
返回参数:返回数据单位/50us 
*******************************************************************************/
uint32_t Heating_Time(int16_t  real_temp,int16_t target_temp)
{
    uint32_t heat_timecnt;

    pid_1.settemp = target_temp;
//    if(wk_temp > temp) {
//        if(wk_temp - temp >= 18)gHt_flag = 0;//加热
//        else gHt_flag = 2;//恒温
//    } else {
//        if(temp - wk_temp <= 18)gHt_flag = 2;//恒温
//        else gHt_flag = 1;//降温
//    }

    heat_timecnt = Pid_Realize(real_temp);//Sub_data * 1000;

    return heat_timecnt;
}

/*******************************************************************************
Name		: Set_HeatingTime
Funtion	:set the time to heat
Para		:heating_time  
return	:NULL
*******************************************************************************/
void Set_HeatingTime(uint32_t  heating_time)
{
   pid_1.gHeat_cnt = heating_time;
}

/*******************************************************************************
函数名: Get_HeatingTime
函数作用:读取加热时间
输入参数:NULL 
返回参数:加热时间
*******************************************************************************/
uint32_t Get_HeatingTime(void)
{
   return pid_1.gHeat_cnt;
}
