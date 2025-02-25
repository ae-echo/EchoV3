/*
 * i2c.c
 *
 *  Created on: Jan 9, 2025
 *      Author: Kim Min Ki
 */

#include <Mcal/Inc/fmc.h>
#include <Mcal/Inc/gpio.h>
#include <i2c.h>
#include <Mcal/Inc/uart.h>
#include "main.h"
#include "time.h"
#include "common.h"
#include "bsw_config.h"

/* SW I2C pin */
#define CH1_I2C_SCL(in)    GPIO_Write(GPIOE, GPIO_PIN_2, (GPIO_PinState)in)
#define CH1_I2C_SDA(in)    GPIO_Write(GPIOE, GPIO_PIN_3, (GPIO_PinState)in)
#define CH2_I2C_SCL(in)    GPIO_Write(GPIOE, GPIO_PIN_4, (GPIO_PinState)in)
#define CH2_I2C_SDA(in)    GPIO_Write(GPIOE, GPIO_PIN_5, (GPIO_PinState)in)

#define SDA_H			I2C_Port.pPort->BSRR = I2C_Port.SDA
#define SDA_L			I2C_Port.pPort->BSRR = (I2C_Port.SDA << 16)
#define	SCL_H			I2C_Port.pPort->BSRR = I2C_Port.SCL
#define SCL_L			I2C_Port.pPort->BSRR = (I2C_Port.SCL << 16)

#define SDA(in)	I2C_Port.pPort->BSRR = (in) ? I2C_Port.SDA : I2C_Port.SDA << 16
#define SCL(in)	I2C_Port.pPort->BSRR = (in) ? I2C_Port.SCL : I2C_Port.SCL << 16

#define I2C_ACK_BIT	(I2C_Port.pPort->IDR & I2C_Port.SDA) ? 0 : 1
#define I2C_DAT_BIT	(I2C_Port.pPort->IDR & I2C_Port.SDA) ? 1 : 0


I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c4;
struct I2C_PortSet		I2C_Port;



uint32_t I2C_GPIO_Write_Func(uint8_t Addr, uint8_t* pData, uint16_t size, uint32_t tmout, int pDataOffset)
{
    uint32_t i, tmp;
    uint32_t ack;

    // Start
    SDA(0);
    delay_i2c(tmout);     // 100ns
    ack = 0;
	tmp = Addr;
    SCL(0);

    // Start Addr
    for(i=0; i<8; i++)
    {
    	I2C_Port.pPort->BSRR  =  (Addr & 0x80) ? I2C_Port.SDA : (I2C_Port.SDA << 16);			// Check point

		SDA((tmp & 0x80) ? 1 : 0);
		delay_i2c(tmout);
		SCL(1);
		delay_i2c(tmout);
		SCL(0);
		tmp <<= 1;
    }

    // ACK Event
    delay_i2c(tmout); /*hold margin*/
	SDA(1); /*Set High-z*/
	delay_i2c(tmout);
	SCL(1);
	SCL(1);
	delay_i2c(tmout);
	ack = (I2C_Port.pPort->IDR & I2C_Port.SDA) ? 1 : 0;
	SCL(0);
	delay_i2c(tmout);

    while(size > 0)
    {
        //tmp = (*pData++);
		tmp = *pData;
		pData = pData + pDataOffset;

        for(i=0; i<8; i++)
        {
			SDA((tmp & 0x80) ? 1 : 0);
			delay_i2c(tmout);
			SCL(1);
			delay_i2c(tmout);
			SCL(0);
			tmp <<= 1;
        }
        size--;

        //ACK Event
        delay_i2c(tmout);	/*hold margin*/
		SDA(1);
		delay_i2c(tmout);
		SCL(1);
		delay_i2c(tmout);
		SCL(0);
    }

    // Stop
	SDA(0);
	delay_i2c(tmout);
	SCL(1);
	delay_i2c(tmout);
	SDA(1);

    return ack;
}
uint32_t I2C_GPIO_Read_Func(uint8_t Addr, uint8_t* pData, uint16_t size, uint32_t tmout, int pDataOffset)
{
	uint32_t ack;
	uint32_t i, tmp;

	// Start
	SDA(0);
	delay_i2c(tmout);
	ack = 0;
	tmp = Addr;
	SCL(0);

	for(i=0; i<8; i++)
	{
		SDA((tmp & 0x80) ? 1 : 0);
		delay_i2c(tmout);
		SCL(1);
		delay_i2c(tmout);
		SCL(0);
		tmp <<= 1;
	}

	// ACK Event
	delay_i2c(tmout); /*hold margin*/
	SDA(1);
	delay_i2c(tmout);
	SCL(1);
	delay_i2c(tmout);
	delay_i2c(tmout);
	ack = (I2C_Port.pPort->IDR & I2C_Port.SDA) ? 1 : 0;
	SCL(0);
	tmp = 0;

	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);

	while(size)
	{
		for(i=0; i<8; i++)
		{
			delay_i2c(tmout);
			SCL(1);
			delay_i2c(tmout);
			tmp |= I2C_DAT_BIT;
			SCL(0);
			if(i < 7) tmp <<= 1;
		}

		*pData = tmp;
		pData = pData + pDataOffset;
		tmp = 0;
		size--;

		delay_i2c(tmout);
		if(size)
		{
			SDA(0);
			delay_i2c(tmout);
			SCL(1);
			delay_i2c(tmout);
			SCL(0);
			__asm volatile("NOP");
			SDA(1);
			__asm volatile("NOP");
		}
		else
		{
			// NoACK Event
			delay_i2c(tmout);
			SCL(1);
			delay_i2c(tmout);
			SCL(0);
		}
	}

	// Stop
	SDA(0);
	delay_i2c(tmout);
	SCL(1);
	delay_i2c(tmout);
	SDA(1);
	return ack;
}
uint32_t I2C_GPIO_Read_SDRAM(uint8_t Addr, uint16_t size, uint32_t tmout, int pDataOffset)
{
	uint32_t ack;
	uint32_t i, tmp;
	int Count = 0;

	// Start
	SDA(0);
	delay_i2c(tmout);
	ack = 0;
	tmp = Addr;
	SCL(0);

	for(i=0; i<8; i++)
	{
		SDA((tmp & 0x80) ? 1 : 0);
		delay_i2c(tmout);
		SCL(1);
		delay_i2c(tmout);
		SCL(0);
		tmp <<= 1;
	}

	// ACK Event
	delay_i2c(tmout); /*hold margin*/
	SDA(1);
	delay_i2c(tmout);
	SCL(1);
	delay_i2c(tmout);
	ack = (I2C_Port.pPort->IDR & I2C_Port.SDA) ? 1 : 0;
	SCL(0);
	tmp = 0;

	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);
	delay_i2c(tmout);

	while(size)
	{
		for(i=0; i<8; i++)
		{
			delay_i2c(tmout);
						SCL(1);
						delay_i2c(tmout);
						tmp |= I2C_DAT_BIT;
						SCL(0);
			if(i < 7) tmp <<= 1;
		}

		FMC_Write(FMC_SDRAM_BLOCK_0 + Count++, 1, (u8*)&tmp);
		tmp = 0;
		size--;

		delay_i2c(tmout);
		if(size)
		{
			SDA(0);
			delay_i2c(tmout);
			SCL(1);
			delay_i2c(tmout);
			SCL(0);
			__asm volatile("NOP");
			SDA(1);
			__asm volatile("NOP");
		}
		else
		{
			// NoACK Event
			delay_i2c(tmout);
			SCL(1);
			delay_i2c(tmout);
			SCL(0);
		}
	}

	// Stop
	SDA(0);
	delay_i2c(tmout);
	SCL(1);
	delay_i2c(tmout);
	SDA(1);
	return ack;
}


/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00C035A6;
  hi2c2.Init.OwnAddress1 = 36;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /** I2C Fast mode Plus enable
  */
  if (HAL_I2CEx_ConfigFastModePlus(&hi2c2, I2C_FASTMODEPLUS_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x60808CD3;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
    PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB12     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
  else if(hi2c->Instance==I2C4)
  {
  /* USER CODE BEGIN I2C4_MspInit 0 */

  /* USER CODE END I2C4_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C4;
    PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_PCLK3;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**I2C4 GPIO Configuration
    PD12     ------> I2C4_SCL
    PD13     ------> I2C4_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C4;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C4_CLK_ENABLE();
  /* USER CODE BEGIN I2C4_MspInit 1 */

  /* USER CODE END I2C4_MspInit 1 */
  }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB12     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12);

    /* I2C2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
  else if(hi2c->Instance==I2C4)
  {
  /* USER CODE BEGIN I2C4_MspDeInit 0 */

  /* USER CODE END I2C4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C4_CLK_DISABLE();

    /**I2C4 GPIO Configuration
    PD12     ------> I2C4_SCL
    PD13     ------> I2C4_SDA
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_13);

  /* USER CODE BEGIN I2C4_MspDeInit 1 */

  /* USER CODE END I2C4_MspDeInit 1 */
  }

}

/**
  * @brief This function handles I2C2 Event interrupt.
  */
void I2C2_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_EV_IRQn 0 */

  /* USER CODE END I2C2_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c2);
  /* USER CODE BEGIN I2C2_EV_IRQn 1 */

  /* USER CODE END I2C2_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C2 Error interrupt.
  */
void I2C2_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C2_ER_IRQn 0 */

  /* USER CODE END I2C2_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c2);
  /* USER CODE BEGIN I2C2_ER_IRQn 1 */

  /* USER CODE END I2C2_ER_IRQn 1 */
}


