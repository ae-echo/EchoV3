#include <MCAL/Inc/GPIO.h>



void GPIO_Init(void)
{
	MX_GPIO_Init();
}

/**
 * @brief  지정된 GPIO 핀의 상태를 읽는 함수
 * @param  port  GPIO 포트 (예: GPIOA, GPIOB 등)
 * @param  pin   읽을 GPIO 핀 번호 (예: GPIO_PIN_0)
 * @retval GPIO 상태 (GPIO_PIN_SET 또는 GPIO_PIN_RESET)
 */
GPIO_PinState GPIO_Read(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_PinState halState = HAL_GPIO_ReadPin(port, pin);
    return (GPIO_PinState)halState;
}

/**
 * @brief  지정된 GPIO 핀의 상태를 설정하는 함수
 * @param  port   GPIO 포트 (예: GPIOA, GPIOB 등)
 * @param  pin    설정할 GPIO 핀 번호 (예: GPIO_PIN_0)
 * @param  state  핀 상태 (GPIO_PIN_SET 또는 GPIO_PIN_RESET)
 */
void GPIO_Write(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state)
{
    HAL_GPIO_WritePin(port, pin, (state == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief  지정된 GPIO 핀의 상태를 토글(반전)하는 함수
 * @param  port  GPIO 포트 (예: GPIOA, GPIOB 등)
 * @param  pin   토글할 GPIO 핀 번호 (예: GPIO_PIN_0)
 */
void GPIO_Toggle(GPIO_TypeDef* port, uint16_t pin)
{
	HAL_GPIO_TogglePin(port, pin);
}
