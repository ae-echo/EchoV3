/*
 * dac.h
 *
 *  Created on: Jan 7, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_DAC_H_
#define MCAL_DAC_H_

#include "main.h"

#define FRA_CH_A		1
#define FRA_CH_B 		2


extern DAC_HandleTypeDef hdac1;




void MX_DAC1_Init(void);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);






void DAC_Out(uint32_t* ch, uint32_t* value);   //
/*
 * DAC_Out(FRA_CH_A, uint32_t* value);
 * DAC_Out(FRA_CH_B, uint32_t* value);
 */



#endif /* MCAL_DAC_H_ */
