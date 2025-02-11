/*
 * ic2.h
 *
 *  Created on: Jan 9, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_I2C_H_
#define MCAL_I2C_H_

#include "main.h"



extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c4;
extern GPIO_TypeDef *I2C_BASS;

struct I2C_Irq_Buf
{
	uint8_t RxBuf[255];
	uint8_t TxBuf[255];
	uint8_t RegAddr;
	uint8_t RxCnt;
	uint8_t TxCnt;
};


void I2C_Init(void);

/*
 *  set about gpio.c
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
 */



#define CH1_I2C_SDA_READ ((GPIO_GetPort(PORT_E)->IDR & GPIO_PIN_3)? 1 : 0)
#define CH2_I2C_SDA_READ ((GPIO_GetPort(PORT_E)->IDR & GPIO_PIN_5)? 1 : 0)


void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c);
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);

void MX_I2C2_Init(void);
void MX_I2C4_Init(void);


void SET_I2C_Speed(void);

void HW_I2C_Read(void);

void HW_I2C_Write(void);

void SW_I2C_Read(void);

void SW_I2C_Write(void);

void HW_I2C_Slave_Transmit(void);

void HW_I2C_Slave_Receive(void);

void HW_I2C_Slave_Adress(void);

void HW_I2C_Scan(I2C_HandleTypeDef* hi2c, uint8_t timeout);

void I2C_Delay(uint32_t delay);

uint8_t I2C_SCL_Read(uint8_t ch);

void SW_I2C_Scan(uint8_t ch ,uint8_t time);

void HW_I2C_Slave(void);

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);




#endif /* MCAL_I2C_H_ */
