/*
 * ldo.h
 *
 *  Created on: Jan 8, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_LDO_H_
#define MCAL_LDO_H_


#include "main.h"
#include <Mcal/Inc/gpio.h>


typedef GPIO_TypeDef GPIO_Port1;

#define SW_I2C_REF_PW_CH1_EN(in)      GPIO_Write(GPIOA, GPIO_PIN_6, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH1_CS(in)      GPIO_Write(GPIOA, GPIO_PIN_7, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH1_UD(in)      GPIO_Write(GPIOC, GPIO_PIN_4, (GPIO_PinState)in)

#define SW_I2C_REF_PW_CH2_EN(in)      GPIO_Write(GPIOD, GPIO_PIN_5, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH2_CS(in)      GPIO_Write(GPIOD, GPIO_PIN_3, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH2_UD(in)      GPIO_Write(GPIOD, GPIO_PIN_4, (GPIO_PinState)in)

#define I3C_REF_PW_EN(in)             GPIO_Write(GPIOC, GPIO_PIN_10, (GPIO_PinState)in)
#define I3C_REF_PW_CS(in)  		      GPIO_Write(GPIOC, GPIO_PIN_11, (GPIO_PinState)in)
#define I3C_REF_PW_UD(in)  		      GPIO_Write(GPIOC, GPIO_PIN_12, (GPIO_PinState)in)

#define EXT_PW_OUT_CH1_EN(in)  		  GPIO_Write(GPIOC, GPIO_PIN_6, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_CS(in)  		  GPIO_Write(GPIOC, GPIO_PIN_7, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_UD(in)         GPIO_Write(GPIOC, GPIO_PIN_8, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_BYPASS_EN(in)  GPIO_Write(GPIOC, GPIO_PIN_9, (GPIO_PinState)in)  //DIFFORTE = 1 EXT INPUT PW BYPASS

#define EXT_PW_OUT_CH2_EN(in)  		  GPIO_Write(GPIOG, GPIO_PIN_2, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_CS(in)  		  GPIO_Write(GPIOG, GPIO_PIN_3, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_UD(in)  		  GPIO_Write(GPIOG, GPIO_PIN_6, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_BYPASS_EN(in)  GPIO_Write(GPIOG, GPIO_PIN_7, (GPIO_PinState)in)  //DIFFORTE = 1 EXT INPUT PW BYPASS


//64까지 가능하나 3.4V 를 Max로 하고 동작시퀀스 구현
#define STEPMAX		46
#define STEMMIN		0

#include "time.h"
#include <math.h>


/*
 * CH1_I2C_SEL(1) 이여지 CH1 LDO조절가능 다른것도 동일
 *
 */
typedef enum
{
	EXT_PW_OUT_CH1 = 0,
	EXT_PW_OUT_CH2,
	SW_I2C_REF_PW_CH1,
	SW_I2C_REF_PW_CH2,
	I3C_REF_PW
} LDO_Select;



//Public
void LDO_ControlMain(LDO_Select LDO);
void LOD_SetVoltage(LDO_Select LDO, float Voltage);
float LDO_GetVoltage(uint8_t Step);
uint8_t LDO_GetStep(float Inputvoltage);


//Private
void ldoUd(LDO_Select type, GPIO_PinState PinStatus);
void ldoCs(LDO_Select type, GPIO_PinState PinStatus);
void ldoEn(LDO_Select type, GPIO_PinState PinStatus);
void LDO_controlup(LDO_Select LDO,uint8_t Step);
void LDO_controldown(LDO_Select LDO,uint8_t Step);


#endif /* MCAL_LDO_H_ */
