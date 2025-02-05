/*
 * gpio.h
 *
 *  Created on: Jan 3, 2025
 *      Author: Kim Min Ki
 */

#include "stm32h5xx_hal.h"

#include "main.h"

#ifndef MCAL_GPIO_H_
#define MCAL_GPIO_H_

/* --- LED Control defines ---------------------- */
#define LED1_ON		GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_12, GPIO_PIN_SET)
#define LED2_ON		GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_11, GPIO_PIN_SET)

#define LED1_OFF	GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_12, GPIO_PIN_RESET)
#define LED2_OFF	GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_11, GPIO_PIN_RESET)


#define LED1_TOG    GPIO_Toggle(GPIO_GetPort(PORT_A), GPIO_PIN_12);//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12)  //GPIO_GetPort(port)
#define LED2_TOG    GPIO_Toggle(GPIO_GetPort(PORT_A), GPIO_PIN_11);

#define LED1_STATE  GPIO_GetOutputState(GPIO_GetPort(PORT_A),  GPIO_PIN_12);
#define LED2_STATE  GPIO_GetOutputState(GPIO_GetPort(PORT_A),  GPIO_PIN_11);

#define R_LED_ON	LED1_ON
#define B_LED_ON	LED2_ON

#define R_LED_OFF	LED1_OFF
#define B_LED_OFF	LED2_OFF

#define R_LED_TOG	LED1_TOG
#define B_LED_TOG	LED2_TOG

/*--USER KEY-----------------------------------------*/

#define USER_KEY GPIO_Read(GPIO_GetPort(PORT_F),  GPIO_PIN_7)


/* ---LDO defines with EXT power bypass --- */
/*
 * --Softewere_I2C_REF_PW_CH1--
 * SW_I2C_REF_PW_CH1_EN
 * SW_I2C_REF_PW_CH1_CS
 * SW_I2C_REF_PW_CH1_UD
 *
 *--Softewere SW_I2C_REF_PW_CH2--
 * SW_I2C_REF_PW_CH2_EN
 * SW_I2C_REF_PW_CH2_CS
 * SW_I2C_REF_PW_CH2_UD
 *
 * --I3C_REF_PW--
 * I3C_REF_PW_EN =	VDDIO2_EN
 * I3C_REF_PW_CS =	VDDIO2_CS
 * I3C_REF_PW_US =	VDDIO2_US
 *
 * --EXT_PW_CH1--
 * EXT_PW_CH1_EN
 * EXT_PW_CH1_CS
 * EXT_PW_CH1_US
 * EXT_PW_CH1_BYPASS bypass : 1 / bypass off :0
 *
 * EXT_PW_CH2_EN
 * EXT_PW_CH2_CS
 * EXT_PW_CH2_UD
 * EXT_PW_CH2_BYPASS bypass : 1 / bypass off :0
 */

#define SW_I2C_REF_PW_CH1_EN(in)      GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_6, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH1_CS(in)      GPIO_Write(GPIO_GetPort(PORT_A), GPIO_PIN_7, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH1_UD(in)      GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_4, (GPIO_PinState)in)


#define SW_I2C_REF_PW_CH2_EN(in)      GPIO_Write(GPIO_GetPort(PORT_D), GPIO_PIN_5, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH2_CS(in)      GPIO_Write(GPIO_GetPort(PORT_D), GPIO_PIN_3, (GPIO_PinState)in)
#define SW_I2C_REF_PW_CH2_UD(in)      GPIO_Write(GPIO_GetPort(PORT_D), GPIO_PIN_4, (GPIO_PinState)in)



#define I3C_REF_PW_EN(in)             GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_10, (GPIO_PinState)in)
#define I3C_REF_PW_CS(in)  		      GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_11, (GPIO_PinState)in)
#define I3C_REF_PW_UD(in)  		      GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_12, (GPIO_PinState)in)


#define EXT_PW_OUT_CH1_EN(in)  		  GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_6, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_CS(in)  		  GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_7, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_UD(in)         GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_8, (GPIO_PinState)in)
#define EXT_PW_OUT_CH1_BYPASS_EN(in)  GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_9, (GPIO_PinState)in)  //DIFFORTE = 1 EXT INPUT PW BYPASS

#define EXT_PW_OUT_CH2_EN(in)  		  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_2, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_CS(in)  		  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_3, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_UD(in)  		  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_6, (GPIO_PinState)in)
#define EXT_PW_OUT_CH2_BYPASS_EN(in)  GPIO_Write(GPIO_GetPort(PORT_G), GPIO_PIN_7, (GPIO_PinState)in)  //DIFFORTE = 1 EXT INPUT PW BYPASS



 /* ---I2C I3C Signal Pass defines --- */

 /* CH1_I2C_SEL
  * CH1_I3C_SEL
  * CH2_I2C_SEL
  * CH2_I3C_SEL
  *
  */

#define CH1_I2C_SEL(in)    GPIO_Write(GPIO_GetPort(PORT_B), GPIO_PIN_1, (GPIO_PinState)in)
#define CH1_I3C_SEL(in)    GPIO_Write(GPIO_GetPort(PORT_B), GPIO_PIN_2, (GPIO_PinState)in)
#define CH2_I2C_SEL(in)    GPIO_Write(GPIO_GetPort(PORT_C), GPIO_PIN_5, (GPIO_PinState)in)
#define CH2_I3C_SEL(in)    GPIO_Write(GPIO_GetPort(PORT_B), GPIO_PIN_0, (GPIO_PinState)in)


/* SW I2C pin */
#define CH1_I2C_SCL(in)    GPIO_Write(GPIO_GetPort(PORT_E), GPIO_PIN_2, (GPIO_PinState)in)
#define CH1_I2C_SDA(in)    GPIO_Write(GPIO_GetPort(PORT_E), GPIO_PIN_3, (GPIO_PinState)in)
#define CH2_I2C_SCL(in)    GPIO_Write(GPIO_GetPort(PORT_E), GPIO_PIN_4, (GPIO_PinState)in)
#define CH2_I2C_SDA(in)    GPIO_Write(GPIO_GetPort(PORT_E), GPIO_PIN_5, (GPIO_PinState)in)







typedef GPIO_TypeDef GPIO_Port;






// GPIO 포트 열거형
typedef enum {
    PORT_A,
    PORT_B,
    PORT_C,
    PORT_D,
    PORT_E,
    PORT_F,
    PORT_G
} GPIO_PortName;






// GPIO 핀 상태
typedef enum {
    GPIO_LOW = 0,   // 핀 LOW 상태
    GPIO_HIGH =1      // 핀 HIGH 상태
} GPIO_State;


/* 함수의 경우 PascalCase 규칙에 따라 네이밍 */
/* Bad case 선언 : GPIO_Set_Pin() */

// GPIO 초기화
void GPIO_Init();

// GPIO 핀 상태 읽기
GPIO_State GPIO_Read(GPIO_Port *port, uint16_t pin);

// GPIO 핀 상태 설정
void GPIO_Write(GPIO_Port *port, uint16_t pin, GPIO_State state);

// GPIO 핀 상태 토글
void GPIO_Toggle(GPIO_Port *port, uint16_t pin);

uint8_t GPIO_GetOutputState(GPIO_TypeDef *port, uint16_t pin);

// GPIO Port 가져오기
GPIO_Port* GPIO_GetPort(GPIO_PortName portName);




#endif /* MCAL_GPIO_H_ */
