#include <MCAL/Inc/TIME.h>

void Time_init(void)
{
	MX_TIM6_Init();
	MX_TIM7_Init();
}

void delay_us(volatile uint32_t delay)
{
  for (volatile int i = 0; i < delay; i++)
    for (volatile int j = 0; j < 28; j++)
      ;
}

void delay_ms(uint32_t ms)
{
    /* user time delay implementation */
	HAL_Delay(ms + 1);
}

void delay_i2c(uint32_t tmout)
{
	while(tmout--)
		__asm volatile("NOP");
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7) //
    {
    	if(hi3c1.Instance->SER == 0x11)
    	{
    		HAL_I3C_DeInit(&hi3c1);
    		I3C_Init();
    	}
    	//LED2_TOG;
    	 //GPIO_Toggle(PORT_A, GPIO_PIN_11);
        //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_11); // GPIO

    }
    if (htim->Instance == TIM6) //
	{

    	//GPIO_Toggle(GPIO_GetPort(PORT_A), GPIO_PIN_12);
	    LED1_TOG;
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12); // GPIO

	}
}
