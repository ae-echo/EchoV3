/*
 * i2c.c
 *
 *  Created on: Jan 9, 2025
 *      Author: Kim Min Ki
 */

#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "uart.h"


I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c4;





void SET_I2C_Speed(void)
{

}

void SW_I2C_Read(void)
{

}

void SW_I2C_Write(void)
{

}

void HW_I2C_Write(void)
{

	//HAL_I2C_Master_Transmit_IT(&hi2c1,20,TX_Buffer,1);
}

void HW_I2C_Read(void)
{
	//HAL_I2C_Master_Transmit_IT(&hi2c1,20,TX_Buffer,1);
	//HAL_I2C_Master_Receive_IT(&hi2c, DevAddress, pData, Size);
	//HAL_I2C_Master_Receive_IT(hi2c, DevAddress, pData, Size);
	//HAL_I2C_Slave_Receive_IT(&hi2c1 ,(uint8_t *)RX_Buffer, 1);
}





void HW_I2C_Scan(I2C_HandleTypeDef* hi2c, uint8_t timeout)
{
	 for (uint8_t address = 1; address < 128; address++) // I2C 주소 범위: 0x01 ~ 0x7F
	 {
		  HAL_StatusTypeDef result =HAL_I2C_IsDeviceReady(hi2c, (address << 1), 1, timeout);

		  if(result== HAL_OK)
		  {
			  uint8_t buffer[]={0xAE,0x06,0xf9,0xfe,address};
			   UART1_Send(buffer, 5);
		  }
	 }
}



void HW_I2C_Slave(void)
{
	uint8_t RX_Buffer [1] ;
	HAL_I2C_Slave_Receive_IT(&hi2c2 ,(uint8_t *)RX_Buffer, 1);
}





static void I2C_SDA(uint8_t ch,uint8_t state)
{
	switch (ch)
	{
	case 1:
		CH1_I2C_SDA(state);

		break;
	case 2:
		CH2_I2C_SDA(state);
		break;

	}
}

static void I2C_SCL(uint8_t ch,uint8_t state)
{
	switch (ch)
		{
		case 1:
			CH1_I2C_SCL(state);

			break;
		case 2:
			CH2_I2C_SCL(state);
			break;

		}
}

uint8_t I2C_SDA_Read(uint8_t ch)
{
	switch (ch)
			{
			case 1:
				return CH1_I2C_SDA_READ;

				break;
			case 2:
				return CH2_I2C_SDA_READ;
				break;

			}

}
static void asm_delay(void)
{
    __asm volatile("NOP");
    	//__asm volatile("NOP");
    	//__asm volatile("NOP");
    //	__asm volatile("NOP");
    //	__asm volatile("NOP");
}


uint8_t SW_I2C_ACK(uint8_t data, uint8_t ch ,uint8_t time)
{
	uint8_t ack;
	I2C_SDA(ch, 1);
	I2C_SCL(ch, 1);
	I2C_Delay(time);
	I2C_SDA(ch, 0);
	I2C_Delay(time);
	I2C_SCL(ch, 0);


	for (int i = 0; i < 8; i++)
	{
		if (data & 0x80) {
			I2C_SDA(ch,1 );
		} else {
			I2C_SDA(ch, 0);
		}
		data <<= 1;
		I2C_Delay(time);
		I2C_SCL(ch, 1); // 클럭 상승
		I2C_Delay(time);
		I2C_SCL(ch, 0); // 클럭 하강
		asm_delay();
	}

	// ACK/NACK 확인

	I2C_SDA(ch,1); // SDA를 입력으로 설정
	I2C_Delay(time);
	I2C_SCL(ch, 1); // 클럭 상승

	I2C_Delay(time);
	ack = I2C_SDA_Read(ch); // ACK: LOW, NACK: HIGH
	I2C_SCL(ch,0 ); // 클럭 하강

	I2C_SDA(ch,0  );
	I2C_Delay(time);
	I2C_SCL(ch, 1); // 클럭 상승
	I2C_Delay(time);
	I2C_SDA(ch, 1 );
	return ack;


}

void SW_I2C_Scan(uint8_t ch ,uint8_t time)
{
	 for (uint8_t address = 1; address < 128; address++) // I2C 주소 범위: 0x01 ~ 0x7F
		 {
			 uint8_t result =SW_I2C_ACK((address << 1),  ch ,time);

			  if(result== 0)
			  {
				  uint8_t buffer[]={0xAE,0x06,0xf9,0xfe,address};
				  UART1_Send(buffer, 5);
			  }
			  I2C_Delay(10);
		 }
}

void I2C_Delay(uint32_t delay)
{
  for (volatile int i = 0; i < delay; i++)
    for (volatile int j = 0; j < 25; j++)
      ;
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
  hi2c2.Init.OwnAddress1 = 128;
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
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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


void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C2) {

    	 HW_I2C_Slave();
        // 수신 데이터 처리
        //Process_Received_Data(rx_buffer, sizeof(rx_buffer));
    }
}


