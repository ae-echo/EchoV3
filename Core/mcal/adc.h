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



uint32_t ADC_Get_ADC(uint16_t* adcDelay);  // just polling
//ADC_Get_ADC(10); diffort 10

void MX_ADC1_Init(void);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);



#endif /* MCAL_ADC_H_ */
