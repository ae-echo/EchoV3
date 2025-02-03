/*
 * ldo.h
 *
 *  Created on: Jan 8, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_LDO_H_
#define MCAL_LDO_H_


#include "main.h"
#include "gpio.h"


typedef GPIO_TypeDef GPIO_Port1;

/*
#define SW_I2C_REF_PW_CH1_EN(in)      GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_6, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH1_CS(in)      GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_7, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH1_UD(in)      GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_4, (GPIO_PinState)in)


#define I3C_REF_PW_EN(in)             GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_10, (GPIO_PinState)in)
#define I3C_REF_PW_CS(in)  		      GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_11, (GPIO_PinState)in)
#define I3C_REF_PW_US(in)  		      GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_12, (GPIO_PinState)in)

#define SW_I2C_REF_PW_CH2_EN(in)      GPIO_Write(GPIO_GetPort(PORT_D), GPIO_PIN_5, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH2_CS(in)      GPIO_Write(GPIO_GetPort(PORT_D), GPIO_PIN_3, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH2_UD(in)      GPIO_Write(GPIO_GetPort(PORT_D), GPIO_PIN_4, (GPIO_PinState)in)


#define EXT_PW_OUT_CH1_EN(in)  		  GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_6, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_CS(in)  		  GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_7, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_UD(in)         GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_8, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_BYPASS_EN(in)  GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_9, (GPIO_PinState)in)  //DIFFORTE = 1 EXT INPUT PW BYPASS

#define EXT_PW_OUT_CH2_EN(in)  		  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_2, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_CS(in)  		  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_3, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_UD(in)  		  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_6, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_BYPASS_EN(in)  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_7, (GPIO_PinState)in)  //DIFFORTE = 1 EXT INPUT PW BYPASS
 */






void ldoUd(uint32_t type, uint8_t pow);
void ldoCs(uint32_t type, uint8_t pow);
void ldoEn(uint32_t type, uint8_t pow);


void LDO_controlup(uint32_t LDO,uint8_t count);
void LDO_controldown(uint32_t LDO,uint8_t count);
void LDO_ControlMain(uint32_t LDO,uint8_t count);
uint8_t LDO_Votage(uint8_t count);// MCP4011 : 0.78125+....     =((0.78125+3)*200/((0.78125+3)+200)+10)/10*0.8  you can apply c# and oled



#endif /* MCAL_LDO_H_ */
