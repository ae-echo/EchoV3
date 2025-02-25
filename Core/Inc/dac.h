/*
 * dac.h
 *
 *  Created on: Jan 7, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_DAC_H_
#define MCAL_DAC_H_

#include "main.h"

extern DAC_HandleTypeDef hdac1;

void MX_DAC1_Init(void);
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac);

#endif /* MCAL_DAC_H_ */
