#ifndef INC_TIME_H_
#define INC_TIME_H_

#include <MCAL/Inc/MCAL_Common.h>
#include <time.h>

void Time_init(void);

void delay_us(volatile uint32_t delay);
void delay_ms(uint32_t ms);
void delay_i2c(volatile uint32_t delay);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* INC_TIME_H_ */
