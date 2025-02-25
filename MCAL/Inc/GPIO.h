#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <gpio.h>
#include "main.h"

/* --- LED Control defines ---------------------- */
#define LED1_ON		GPIO_Write(GPIOA, GPIO_PIN_12, GPIO_PIN_SET)
#define LED2_ON		GPIO_Write(GPIOA, GPIO_PIN_11, GPIO_PIN_SET)

#define LED1_OFF	GPIO_Write(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET)
#define LED2_OFF	GPIO_Write(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET)


#define LED1_TOG    GPIO_Toggle(GPIOA, GPIO_PIN_12);
#define LED2_TOG    GPIO_Toggle(GPIOA, GPIO_PIN_11);

#define LED1_STATE  GPIO_Read(GPIOA,  GPIO_PIN_12);
#define LED2_STATE  GPIO_Read(GPIOA,  GPIO_PIN_11);

#define R_LED_ON	LED1_ON
#define B_LED_ON	LED2_ON

#define R_LED_OFF	LED1_OFF
#define B_LED_OFF	LED2_OFF

#define R_LED_TOG	LED1_TOG
#define B_LED_TOG	LED2_TOG

/*--USER KEY-----------------------------------------*/

#define USER_KEY GPIO_Read(GPIOF,  GPIO_PIN_7)
#define BUTTON_PRESSED 0  // 눌림 상태 (풀업 저항 사용 가정)
#define BUTTON_RELEASED 1 // 떼어진 상태

// GPIO 초기화
void GPIO_Init(void);

/**
 * @brief  지정된 GPIO 핀의 상태를 읽는 함수
 * @param  port  GPIO 포트 (예: GPIOA, GPIOB 등)
 * @param  pin   읽을 GPIO 핀 번호 (예: GPIO_PIN_0)
 * @retval GPIO 상태 (GPIO_HIGH 또는 GPIO_LOW)
 */
GPIO_PinState GPIO_Read(GPIO_TypeDef *port, uint16_t pin);

/**
 * @brief  지정된 GPIO 핀의 상태를 설정하는 함수
 * @param  port   GPIO 포트 (예: GPIOA, GPIOB 등)
 * @param  pin    설정할 GPIO 핀 번호 (예: GPIO_PIN_0)
 * @param  state  핀 상태 (GPIO_PIN_SET 또는 GPIO_PIN_RESET)
 */
void GPIO_Write(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state);

/**
 * @brief  지정된 GPIO 핀의 상태를 토글(반전)하는 함수
 * @param  port  GPIO 포트 (예: GPIOA, GPIOB 등)
 * @param  pin   토글할 GPIO 핀 번호 (예: GPIO_PIN_0)
 */
void GPIO_Toggle(GPIO_TypeDef *port, uint16_t pin);

#endif /* INC_GPIO_H_ */
