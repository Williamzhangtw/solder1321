#ifndef __LED_H
#define __LED_H

#include "stm32f0xx_hal.h"
#include "main.h"

#define LED_HEATED_ENABLE  	HAL_GPIO_WritePin(LAY2_GPIO_Port ,LAY2_Pin ,GPIO_PIN_SET )
#define LED_HEATED_DISABL  HAL_GPIO_WritePin(LAY2_GPIO_Port ,LAY2_Pin ,GPIO_PIN_RESET )






































#endif
