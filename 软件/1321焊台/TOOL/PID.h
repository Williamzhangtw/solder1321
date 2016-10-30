#ifndef __PID_H
#define __PID_H
#include "stm32f0xx_hal.h"

//全局变量


typedef struct  {
    int16_t settemp;        //定义设定温度
    int16_t actualtemp;     //定义实际温度
    int16_t err;            //定义温度差值
    int16_t err_last;       //定义上一个温度差值
    int32_t ht_time;        //定义加热时间
    uint16_t kp,ki,kd;       //定义比例、积分、微分系数
    int32_t  integral;       //定义积分值
		uint32_t gHeat_cnt;
} PID_TypeDef;



extern uint32_t Heating_Time(int16_t  real_temp,int16_t target_temp);
extern void Set_HeatingTime(uint32_t  heating_time);






#endif
