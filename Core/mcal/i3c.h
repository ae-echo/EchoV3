/*
 * i3c.h
 *
 *  Created on: Jan 13, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_I3C_H_
#define MCAL_I3C_H_

#include "main.h"
#include <stdbool.h>


extern I3C_HandleTypeDef hi3c1;


uint32_t i3c_write(uint8_t id, uint16_t addr, uint16_t data , bool i3c_mode); // this copyed wbb3 code / no test
uint32_t I3C_ENTDAA(uint8_t DA, unsigned int DAA);// this copyed wbb3 code/ no test
uint32_t I3C_RSTDAA(bool flag_CurrComMod);// this copyed wbb3 code/ no test
uint32_t DW9785_Change_I3C_Mode(uint8_t id , bool i3c_mode);// this copyed wbb3 code/ no test
void DW9785_I2C_to_I3C(void);// this copyed wbb3 code/ no test
void delay_ms(uint32_t ms);

uint8_t Scan_I2C_Target(void);  // test done

uint8_t Scan_I3C_Target(void);  // test don

uint32_t DW9785_Change_I3C_Mode(uint8_t id , bool i3c_mode); // this copyed wbb3 code / no test


void MX_I3C1_Init(void);
void HAL_I3C_MspDeInit(I3C_HandleTypeDef* hi3c);
void HAL_I3C_MspInit(I3C_HandleTypeDef* hi3c);



#endif /* MCAL_I3C_H_ */
