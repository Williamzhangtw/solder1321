/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SEN4_Pin GPIO_PIN_0
#define SEN4_GPIO_Port GPIOC
#define SEN3_Pin GPIO_PIN_1
#define SEN3_GPIO_Port GPIOC
#define HZ500_Pin GPIO_PIN_0
#define HZ500_GPIO_Port GPIOA
#define SET_KEY_Pin GPIO_PIN_1
#define SET_KEY_GPIO_Port GPIOA
#define TM1651_CLK_1_Pin GPIO_PIN_2
#define TM1651_CLK_1_GPIO_Port GPIOA
#define TM1651_DIO_1_Pin GPIO_PIN_3
#define TM1651_DIO_1_GPIO_Port GPIOA
#define SEN1_Pin GPIO_PIN_4
#define SEN1_GPIO_Port GPIOA
#define LAY2_Pin GPIO_PIN_5
#define LAY2_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_6
#define SW2_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_7
#define SW1_GPIO_Port GPIOA
#define SEN2_Pin GPIO_PIN_0
#define SEN2_GPIO_Port GPIOB
#define DOWN_KEY_Pin GPIO_PIN_10
#define DOWN_KEY_GPIO_Port GPIOB
#define UP_KEY_Pin GPIO_PIN_7
#define UP_KEY_GPIO_Port GPIOC
#define AirHOT_Pin GPIO_PIN_8
#define AirHOT_GPIO_Port GPIOA
#define TM1651_DIO_2_Pin GPIO_PIN_10
#define TM1651_DIO_2_GPIO_Port GPIOA
#define TM1651_CLK_2_Pin GPIO_PIN_3
#define TM1651_CLK_2_GPIO_Port GPIOB
#define SolderHOT_Pin GPIO_PIN_4
#define SolderHOT_GPIO_Port GPIOB
#define BEEP_EnH_Pin GPIO_PIN_8
#define BEEP_EnH_GPIO_Port GPIOB
#define LAY1_Pin GPIO_PIN_9
#define LAY1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef  unsigned    char BOOL;
 

#define TRUE  !0
#define FALSE 0

 

#define YES  !0
#define NO 0
/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
