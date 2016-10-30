/* verify:20160810am
用完了要清零
*/
#include "rotary.h"
/* USER CODE BEGIN Includes */
 
 
/* USER CODE END Includes */

/*
轮询法思路：
要确保1ms就能执行该函数一次
1.获得两脚的电平，与之前对比，出现不同视为
*/


void Rotary_scan(R0TARY_CTRL_TypeDef *rotary)
{
	rotary->elec_leval_a_now= rotary->read_rotary_a() ;
	rotary->elec_leval_b_now= rotary->read_rotary_b();
	
	if((rotary->elec_leval_a_now!= rotary->elec_leval_a_pre)||(rotary->elec_leval_b_now!=rotary->elec_leval_b_pre))
	{
		switch(rotary->N)
		{
			case 0:
				rotary->Spin_direction =no_direction  ;
				if(rotary->elec_leval_a_now)rotary->temple|=0x02; 
					rotary->temple|=rotary->elec_leval_b_now; 
					rotary->elec_leval_a_pre = rotary->elec_leval_a_now ;
					rotary->elec_leval_b_pre = rotary->elec_leval_b_now ;
					rotary->N=1;break;
			case 1:rotary->temple <<=2;
				if(rotary->elec_leval_a_now)rotary->temple|=0x02;
					rotary->temple|=rotary->elec_leval_b_now;
					rotary->elec_leval_a_pre = rotary->elec_leval_a_now ;
					rotary->elec_leval_b_pre = rotary->elec_leval_b_now ;
					rotary->N=2;break;
			case 2:rotary->temple <<=2;
				if(rotary->elec_leval_a_now)rotary->temple|=0x02;
					rotary->temple|=rotary->elec_leval_b_now;
					rotary->elec_leval_a_pre = rotary->elec_leval_a_now ;
					rotary->elec_leval_b_pre = rotary->elec_leval_b_now ;
					rotary->N=3;break;
			case 3: 
				switch(rotary->temple)
				{
					case 0X0B:rotary->Spin_direction =Spin_left ;break;//210--
					case 0x34:rotary->Spin_direction =Spin_left ;break;//75
					case 0x2d:rotary->Spin_direction =Spin_left ;break;//45--
					
					
					case 0x1e:rotary->Spin_direction =Spin_right ;break;//225--
					case 0x07:rotary->Spin_direction =Spin_right ;break;//135--
					case 0x38:rotary->Spin_direction =Spin_right ;break;//30
					
					
					default : break ;
				}
				rotary->N = 0; rotary->temple = 0; break ;

			default :rotary->N = 0; rotary->temple = 0; break ;
		}
	}
}


R0TARY_CTRL_TypeDef rotary_1 ;

uint8_t read_rotary_a(void)
{
	return HAL_GPIO_ReadPin(ROTARY_A_GPIO_Port ,ROTARY_A_Pin);
}

uint8_t read_rotary_b(void)
{
	return HAL_GPIO_ReadPin(ROTARY_B_GPIO_Port ,ROTARY_B_Pin);
}


/* USER CODE BEGIN EFP */
void Rotary_1_init(void)
{
	rotary_1.read_rotary_a = &read_rotary_a;
	rotary_1.read_rotary_b = &read_rotary_b;
	rotary_1.Spin_direction = no_direction ;
	rotary_1 .elec_leval_a_pre =1;
	rotary_1 .elec_leval_b_pre =1;
	rotary_1 .elec_leval_a_now =0;
	rotary_1 .elec_leval_b_now =0;
	rotary_1 .temple =0;
	rotary_1.N  =0;
}

void Rotary_1_scan_ISR(void)
{
	Rotary_scan(&rotary_1);
}


