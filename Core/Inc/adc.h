/*
 * adc.h
 *
 *  Created on: Jan 6, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_ADC_H_
#define MCAL_ADC_H_

#include "main.h"

extern ADC_HandleTypeDef hadc1;

void MX_ADC1_Init(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);

#endif /* MCAL_ADC_H_ */
