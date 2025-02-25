/*
 * i3c.h
 *
 *  Created on: Jan 13, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_I3C_H_
#define MCAL_I3C_H_

#include "main.h"

#define I3C_1MHZ 0xef
#define I3C_2MHZ 0x71
#define I3C_3MHZ 0x49
#define I3C_6MHZ 0x1F
#define I3C_10MHZ 0x0E
#define I3C_12_5MHZ 0x09

extern I3C_HandleTypeDef hi3c1;


void MX_I3C1_Init(void);
void HAL_I3C_MspInit(I3C_HandleTypeDef* hi3c);
void HAL_I3C_MspDeInit(I3C_HandleTypeDef* hi3c);

#endif /* MCAL_I3C_H_ */
