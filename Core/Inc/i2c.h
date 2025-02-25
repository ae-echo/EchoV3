/*
 * ic2.h
 *
 *  Created on: Jan 9, 2025
 *      Author: Kim Min Ki
 *
 * I2C2 -> Slave I2C
 * I2C4 -> Display I2C
 */

#ifndef MCAL_I2C_H_
#define MCAL_I2C_H_

#include "main.h"

struct I2C_PortSet
{
    GPIO_TypeDef* pPort;

	uint16_t SDA;
	uint16_t SCL;

	uint16_t Speed;
};

extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c4;
extern struct I2C_PortSet I2C_Port;

void MX_I2C2_Init(void);
void MX_I2C4_Init(void);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c);
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);

uint32_t I2C_GPIO_Write_Func(uint8_t Addr, uint8_t* pData, uint16_t size, uint32_t tmout, int pDataOffset);
uint32_t I2C_GPIO_Read_Func(uint8_t Addr, uint8_t* pData, uint16_t size, uint32_t tmout, int pDataOffset);
uint32_t I2C_GPIO_Read_SDRAM(uint8_t Addr, uint16_t size, uint32_t tmout, int pDataOffset);



#endif /* MCAL_I2C_H_ */
