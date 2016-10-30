/*************************************************************
*版权信息：张腾威
*文件名字：TM1651
*版本V1.1
*MUC型号 stm32f030
*开发环境：keil
*最新时间：2016年1月27日14:58:21
*程序功能：数码管驱动和按键读取；驱动7段4位LED共阳数码管，当对应按键按下显示1--7
*免责声明：此程序仅供参考，若使用本例例程造成经济损失，本人不承担任何责任
*备注：按键程序有问题，执行按键程序容易出错
***************************************************************/

#include "tm1651.h" 

#define tm1651_delay_EN 0;

//亮度
//显示控制命令，开显示，脉冲宽度为11/16 0x08表示显示0|0x08|0x00
	//脉冲宽度 1/16-0b000 2/16-0b001 4/16-0b010 10/16-0b011 12/16-0b101 13/16-0b110 14/16-0b111
//MSB LSB 
//B7  B6 B5 B4 B3 B2  B1  B0
//B3 ： 0表示关闭显示 1表示开启显示  B2  B1  B0 组成亮度调级，数值越大，亮度越亮

 
  
  
/********************定义数据************************/
void tm1651_delay()
{
  U16 i ;
  for (i=60;i>0;i--)
  {}
}

/********************Start*************************/
void I2CStart (void)
{
 DIO_H ;
 CLK_H ;
  #ifdef tm1651_delay_EN
__nop() ;
  tm1651_delay();
  #endif
 DIO_H ;

 DIO_L ;
  #ifdef tm1651_delay_EN
 __nop() ;
  #endif
 CLK_L ;
}

/********************Stop*************************/
void I2CStop(void)
{
    CLK_L ;
  #ifdef tm1651_delay_EN
   tm1651_delay();
	__nop() ;
	__nop() ;
  #endif
	DIO_L ;
  #ifdef tm1651_delay_EN
   tm1651_delay();
	__nop() ;
	__nop() ;
  #endif
	CLK_H ;
  #ifdef tm1651_delay_EN
   tm1651_delay();
	__nop() ;
	__nop() ;
	__nop() ;
  #endif
	DIO_H ;
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
	CLK_L ;
	DIO_L ;
}

/***************发送8bit data start from low bit **************/
void I2CWritebyte(uint8_t  oneByte)
{
  unsigned char i;
  for(i=0;i<8;i++)
  {
    CLK_L ;
    if(oneByte&0x01) 
	  DIO_H ;
    
    else 
	  DIO_L ;
    #ifdef tm1651_delay_EN
    __nop() ;
     tm1651_delay();
    #endif
    CLK_H ;
    oneByte=oneByte>>1;
  }
  									//8bit translated 	 
	CLK_L ;								//check the data from the chip 's ack signal
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
  HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port ,TM1651_DIO_Pin,GPIO_PIN_SET) ;
	while( HAL_GPIO_ReadPin(TM1651_DIO_GPIO_Port ,TM1651_DIO_Pin));
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
	CLK_H ;
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
}



/************显示函数 地址自加一************/
void disp0(const uint8_t *p)	                      
{
 I2CStart();
 I2CWritebyte(0x40);				 //数据命令设置，地址自动加一，写数据到显示寄存器
 I2CStop();

 I2CStart();
 I2CWritebyte(0xc0);				 //地址命令设置，初始地址为00H
 for(uint8_t i=0;i<3;i++)		//发送4字节数据到显存
 {
 I2CWritebyte(*p);
 p++;
 }
 I2CStop();

// I2CStart();
// I2CWritebyte(0x88);			 //显示控制命令，开显示，脉冲宽度为11/16.(0x8C);	
// I2CStop();

}

/************显示函数 地址自加一************/
void disp_value(uint16_t number)	                      
{
 uint8_t buf[3];
 buf[0]  = number %10; //ge
 buf[1]  = (number /10)%10;
 buf[2]  = (number /100)%10;
  
 I2CStart();
 I2CWritebyte(0x40);				 //数据命令设置，地址自动加一，写数据到显示寄存器
 I2CStop();

 I2CStart();
 I2CWritebyte(0xc0);				 //地址命令设置，初始地址为00H
 for(uint8_t i=0;i<3;i++)		//发送4字节数据到显存
 {
  I2CWritebyte(CODE00[buf[i]]);
 }
 I2CStop();

// I2CStart();
// I2CWritebyte(0x8F);			 //显示控制命令，开显示，脉冲宽度为11/16 0x08表示显示0|0x08|0x00
//	//脉冲宽度 1/16-0b000 2/16-0b001 4/16-0b010 10/16-0b011 12/16-0b101 13/16-0b110 14/16-0b111
// I2CStop();
}

/************显示函数 固定地址写数据 ************/
void disp(uint8_t add, uint8_t  value)
{
 I2CStart();
 I2CWritebyte(0x44);			//数据命令设置，固定地址，写数据到显示寄存器	
 I2CStop();

 I2CStart();
  I2CWritebyte(add);				//地址命令设置，写入add对应地址

 I2CWritebyte(CODE00[value]);			//给add地址写数据
 I2CStop();

// I2CStart();
//  I2CWritebyte(0x80);				//显示控制命令，开显示，脉冲宽度为11/16 0x08表示显示0|0x08|0x00
//	//脉冲宽度 1/16-0b000 2/16-0b001 4/16-0b010 10/16-0b011 12/16-0b101 13/16-0b110 14/16-0b111
// I2CStop();

}

/***************读按键程序**************/
uint8_t read_key()
{
uint8_t rekey =0;
uint8_t   i ;
I2CStart();
I2CWritebyte(0x42);							 //写读指令0x42
DIO_H;
CLK_H;
for(i=0;i<8;i++)
{
	CLK_L;
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
	rekey>>=1;	   						//先读低位
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
	CLK_H;
   HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port ,TM1651_DIO_Pin,GPIO_PIN_SET) ;
	if( HAL_GPIO_ReadPin(TM1651_DIO_GPIO_Port ,TM1651_DIO_Pin))
	  rekey=rekey|0x80;
	else 
	  rekey=rekey|0x00;
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
}
	CLK_L;							  //判断芯片发过来的ACK应答信号
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
//使用开漏输出模式，在读取管脚电平的时候 先将管脚写高电平，然后读数
 HAL_GPIO_WritePin(TM1651_DIO_GPIO_Port ,TM1651_DIO_Pin,GPIO_PIN_SET) ;
	while( HAL_GPIO_ReadPin(TM1651_DIO_GPIO_Port ,TM1651_DIO_Pin));
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
	CLK_H;
  #ifdef tm1651_delay_EN
	__nop() ;
   tm1651_delay();
  #endif
	I2CStop();
	
	return rekey;
}




/************按键处理函数，按键数据低位在前高位在后ó************/
void tm1651_key_process()
{
 uint8_t temp =0 ;
 temp =   read_key() ;      //(0x10)			  //读取按键返回值
 
// static uint16_t i=0;
 if(temp!= 0xff)
 {     
   
  //disp0(CODE_Clean);					  // 清屏
  //    GUI_Clear();
  switch(temp)
  {
    
    case 0xf7 : pid_input_detect |=PID_Input_BIT7; break;// disp(0xc0,1);break;	GUI_DispDecMin(1);	  //K1与SG1对应的按键按下 显示1  A
    case 0xf6 : pid_input_detect |=PID_Input_BIT6;break;//disp(0xc0,2);break;		  //K1与SG2对应的按键按下 显示2	 B
    case 0xf5 : pid_input_detect |=PID_Input_BIT5;break;//disp(0xc0,3);break;		  //K1与SG3对应的按键按下 显示3  C
    case 0xf4 : pid_input_detect |=PID_Input_BIT4;break;//disp(0xc0,4);break;		  //K1与SG4对应的按键按下   D  
    case 0xf3 : pid_input_detect |=PID_Input_BIT3;break;//disp(0xc0,5);break;		  //K1与SG5对应的按键按下  E
    case 0xf2 : pid_input_detect |=PID_Input_BIT2;break;//disp(0xc0,6);break;		  //K1与SG6对应的按键按下  F
    case 0xf1 : pid_input_detect |=PID_Input_BIT1;break;//disp(0xc0,7);break;		  //KK1与SG7对应的按键按下  G // 
    default   : break;
  }
 }
}

void Tm1651_BackLight(uint16_t jibie)
{
  
  I2CStart();
  I2CWritebyte(0x88|jibie);			 //显示控制命令，开显示，脉冲宽度为11/16 0x08表示显示0|0x08|0x00
	//脉冲宽度 1/16-0b000 2/16-0b001 4/16-0b010 10/16-0b011 12/16-0b101 13/16-0b110 14/16-0b111
  //F 4bit 1表示开显示 0关显示 3bit 2bit 1bit表示脉冲宽度
  I2CStop();
}

void TM1651_Init(uint8_t backlight)
{
   I2CStart();
 I2CWritebyte(0x88|  backlight);			 //显示控制命令，开显示，脉冲宽度为11/16 0x08表示显示0|0x08|0x00
	//脉冲宽度 1/16-0b000 2/16-0b001 4/16-0b010 10/16-0b011 12/16-0b101 13/16-0b110 14/16-0b111
 I2CStop();
} 
