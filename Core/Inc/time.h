/*
 * time.h
 *
 *  Created on: Jan 15, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_TIME_H_
#define MCAL_TIME_H_

#include "main.h"

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;


void MX_TIM6_Init(void);
void MX_TIM7_Init(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);


#endif /* MCAL_TIME_H_ */
