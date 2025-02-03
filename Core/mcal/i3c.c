/*
 * i3c.c
 *
 *  Created on: Jan 13, 2025
 *      Author: Kim Min Ki
 */

#include "i3c.h"
#include <stdbool.h>


I3C_HandleTypeDef hi3c1;

/* I3C Value */
uint32_t i3c_error;
#define _I2C_MODE_ false
#define _I3C_MODE_ true
HAL_StatusTypeDef i3c_status = HAL_OK;

/* I3C R/W Variable */
I3C_XferTypeDef aContextBuffers[1];
uint32_t aControlBuffer[0xFF];
uint8_t aTxBuffer[256] = {0x00, };
uint8_t aRxBuffer[256] = {0x00, };

I3C_PrivateTypeDef aPrivateDescriptor[1] =
{
{0x00, {NULL, 0}, {NULL, 0}, HAL_I3C_DIRECTION_READ},
};

/* I3C ENTDAA Variable */
uint64_t TargetPayload;
uint8_t DW9785_payload[8] = {0x05,0x44,0x00,0x00,0x00,0x00,0x06,0xE4};
uint8_t payload[8];


/*ST EXAMPLE*/
/* USER CODE BEGIN PTD */
#define Direct_SETDASA 0x87
#define Brodacast_DISEC 0x01
#define Brodacast_RST 0x06

#define LPS22HH_DYNAMIC_ADDR 0x32
/* USER CODE END PTD */

/* USER CODE BEGIN PD */
#define I3C_IDX_FRAME_1         0U  /* Index of Frame 1 */
#define I3C_IDX_FRAME_2         1U  /* Index of Frame 2 */
/* USER CODE END PD */



uint8_t Scan_I2C_Target(void)
{
	uint8_t cnt =0;

	for(uint8_t i=0; i<0x7F; i++)
	{
		if(i == 0x3E | i ==0x5E | i ==0x6E | i ==0x76 | i ==0x7A | i ==0x7C | i ==0x7E| i ==0x7F);
		else{
			i3c_status = HAL_I3C_Ctrl_IsDeviceI2C_Ready(&hi3c1, i, 1, 1);
			if(i3c_status != HAL_OK)
			{

			}
			else
			{
				cnt=i;
			}


		}
	}
	return cnt;
}


uint8_t Scan_I3C_Target(void)
{
	uint8_t cnt =0;

	for(uint8_t i=0; i<0x7F; i++)
	{
		if(i == 0x3E | i ==0x5E | i ==0x6E | i ==0x76 | i ==0x7A | i ==0x7C | i ==0x7E | i ==0x7F){}  ///  i3c 에서 0x3E | i ==0x5E | i ==0x6E | i ==0x76 | i ==0x7A | i ==0x7C | i ==0x7E | i ==0x7F 이거 사용하면 먹통됨 ccc 보내서 초기화 필요
		else{
			i3c_status = HAL_I3C_Ctrl_IsDeviceI3C_Ready(&hi3c1, (i), 1, 1);
			if(i3c_status != HAL_OK){}
			else
			{
				cnt=i;
			}
		}
	}
	return cnt;
}




void DW9785_I2C_to_I3C(void)
{
	DW9785_Change_I3C_Mode(1,_I2C_MODE_);
	I3C_RSTDAA(_I2C_MODE_);
	I3C_ENTDAA(2, I3C_RSTDAA_THEN_ENTDAA);
}

void delay_ms(uint32_t ms)
{
    /* user time delay implementation */
	HAL_Delay(ms + 1);
}




uint32_t I3C_RSTDAA(bool flag_CurrComMod)
{


	uint8_t id = 0x7E;
	uint8_t data = 0x06;
	unsigned int w_mode;

	if (flag_CurrComMod == _I2C_MODE_)
		w_mode = I2C_PRIVATE_WITHOUT_ARB_RESTART;
	else
		w_mode = I3C_PRIVATE_WITHOUT_ARB_RESTART;


	//use I3C_IDX_FRAME_1
	/* set trans data  */
	aTxBuffer[0] = data;

	/* set buffer */
	aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
	aContextBuffers[0].CtrlBuf.Size = 1;
	aContextBuffers[0].TxBuf.pBuffer = aTxBuffer;
	aContextBuffers[0].TxBuf.Size = 1;	//addr 2byte data 2byte


	aPrivateDescriptor[0].TargetAddr = id;
	aPrivateDescriptor[0].TxBuf.pBuffer = aTxBuffer;
	aPrivateDescriptor[0].TxBuf.Size =1;
	aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_WRITE;



	HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0],
			&aContextBuffers[0],
			aContextBuffers[0].CtrlBuf.Size,
			w_mode);

	HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]);

	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY)
	{
	}

	i3c_error = hi3c1.ErrorCode;
	return i3c_error;
}

uint32_t I3C_ENTDAA(uint8_t DA, unsigned int DAA)
{
	uint8_t cnt =1;
	uint8_t dw9785=0;
	DA = DA>>1;
	do
	{
		TargetPayload = 0;
		i3c_status = HAL_I3C_Ctrl_DynAddrAssign(&hi3c1, &TargetPayload, DAA, 100);
		dw9785=1;
		for (int i = 0; i < 8; i++) {

			payload[i] = (TargetPayload >> (i * 8)) & 0xFF;	//dw9785 비교
			if(DW9785_payload[i]!=payload[i])
			{
				dw9785 =0;
				break;
			}

		}
		if (i3c_status == HAL_BUSY && dw9785==1)
		{
			HAL_I3C_Ctrl_SetDynAddr(&hi3c1, DA);
		}
		else if(i3c_status == HAL_BUSY)
		{
			HAL_I3C_Ctrl_SetDynAddr(&hi3c1, DA+cnt);
			cnt++;
		}

	} while (i3c_status == HAL_BUSY);


	i3c_error = hi3c1.ErrorCode;
	return i3c_error;
}

uint32_t i3c_write(uint8_t id, uint16_t addr, uint16_t data , bool i3c_mode)
{
	unsigned int w_mode;

	if (i3c_mode == _I2C_MODE_) {
		w_mode = I2C_PRIVATE_WITHOUT_ARB_RESTART;
	} else {
		w_mode = I3C_PRIVATE_WITHOUT_ARB_RESTART;
	}

	//use I3C_IDX_FRAME_1
	/* set trans data  */
	aTxBuffer[0] = (uint8_t)((uint16_t) addr >> 8);	//addr_hi
	aTxBuffer[1] = (uint8_t) addr;					//addr_low
	aTxBuffer[2] = (uint8_t)((uint16_t) data >> 8);	//data hi
	aTxBuffer[3] = (uint8_t) data;					//data	low


	/* set buffer */
	aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
	aContextBuffers[0].CtrlBuf.Size = 1;
	aContextBuffers[0].TxBuf.pBuffer = aTxBuffer;
	aContextBuffers[0].TxBuf.Size = 4;	//addr 2byte data 2byte


	aPrivateDescriptor[0].TargetAddr = id>>1;
	aPrivateDescriptor[0].TxBuf.pBuffer = aTxBuffer;
	aPrivateDescriptor[0].TxBuf.Size =4;
	aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_WRITE;



	HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0],
			&aContextBuffers[0],
			aContextBuffers[0].CtrlBuf.Size,
			w_mode);

	HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]);

	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY)
	{
	}


	i3c_error = hi3c1.ErrorCode;
	return i3c_error;

}




uint32_t DW9785_Change_I3C_Mode(uint8_t id , bool i3c_mode)
{

	// I3C mode Change
	i3c_write(id,0xD02B, 0x0001,i3c_mode);
	delay_ms(10);

	// Auto Hot-Join Disable
	i3c_write(id,0xD02C, 0x0000,i3c_mode);
	delay_ms(10);

	/* Store SEQ*/
	//LDT ProTect OFF
	i3c_write(id,0xDE93, 0xCADD,i3c_mode);
	delay_ms(10);

	//Store
	i3c_write(id,0xD012, 0x0004,i3c_mode);
	delay_ms(10);

	//LDT ProTect ON
	i3c_write(id,0xDE93, 0x0000,i3c_mode);
	delay_ms(10);

	//reset
	i3c_write(id,0xD002, 0x0001,i3c_mode);	//Logic Reset
	delay_ms(10);
	i3c_write(id,0xDD03, 0x0003,i3c_mode);	//SDA Strength
	delay_ms(10);
	i3c_error = i3c_write(id,0xDD04, 0x0003,i3c_mode);	//SCL Strength
	delay_ms(10);

	return i3c_error;

}

/**
  * @brief I3C1 Initialization Function
  * @param None
  * @retval None
  */


void MX_I3C1_Init(void)
{

  /* USER CODE BEGIN I3C1_Init 0 */

  /* USER CODE END I3C1_Init 0 */

  I3C_FifoConfTypeDef sFifoConfig = {0};
  I3C_CtrlConfTypeDef sCtrlConfig = {0};

  /* USER CODE BEGIN I3C1_Init 1 */

  /* USER CODE END I3C1_Init 1 */
  hi3c1.Instance = I3C1;
  hi3c1.Mode = HAL_I3C_MODE_CONTROLLER;
  hi3c1.Init.CtrlBusCharacteristic.SDAHoldTime = HAL_I3C_SDA_HOLD_TIME_1_5;
  hi3c1.Init.CtrlBusCharacteristic.WaitTime = HAL_I3C_OWN_ACTIVITY_STATE_0;
  hi3c1.Init.CtrlBusCharacteristic.SCLPPLowDuration = 0x09;
  hi3c1.Init.CtrlBusCharacteristic.SCLI3CHighDuration = 0x09;
  hi3c1.Init.CtrlBusCharacteristic.SCLODLowDuration = 0x7c;
  hi3c1.Init.CtrlBusCharacteristic.SCLI2CHighDuration = 0x7c;
  hi3c1.Init.CtrlBusCharacteristic.BusFreeDuration = 0x6a;
  hi3c1.Init.CtrlBusCharacteristic.BusIdleDuration = 0xf8;
  if (HAL_I3C_Init(&hi3c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure FIFO
  */
  sFifoConfig.RxFifoThreshold = HAL_I3C_RXFIFO_THRESHOLD_1_4;
  sFifoConfig.TxFifoThreshold = HAL_I3C_TXFIFO_THRESHOLD_1_4;
  sFifoConfig.ControlFifo = HAL_I3C_CONTROLFIFO_DISABLE;
  sFifoConfig.StatusFifo = HAL_I3C_STATUSFIFO_DISABLE;
  if (HAL_I3C_SetConfigFifo(&hi3c1, &sFifoConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure controller
  */
  sCtrlConfig.DynamicAddr = 0;
  sCtrlConfig.StallTime = 0x00;
  sCtrlConfig.HotJoinAllowed = DISABLE;
  sCtrlConfig.ACKStallState = DISABLE;
  sCtrlConfig.CCCStallState = DISABLE;
  sCtrlConfig.TxStallState = DISABLE;
  sCtrlConfig.RxStallState = DISABLE;
  sCtrlConfig.HighKeeperSDA = DISABLE;
  if (HAL_I3C_Ctrl_Config(&hi3c1, &sCtrlConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I3C1_Init 2 */

  /* USER CODE END I3C1_Init 2 */

}



/**
* @brief I3C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi3c: I3C handle pointer
* @retval None
*/
void HAL_I3C_MspInit(I3C_HandleTypeDef* hi3c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hi3c->Instance==I3C1)
  {
  /* USER CODE BEGIN I3C1_MspInit 0 */

  /* USER CODE END I3C1_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I3C1;
    PeriphClkInitStruct.I3c1ClockSelection = RCC_I3C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_I3C1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I3C1 GPIO Configuration
    PB8     ------> I3C1_SCL
    PB9     ------> I3C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF3_I3C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I3C1 interrupt Init */
    HAL_NVIC_SetPriority(I3C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I3C1_EV_IRQn);
    HAL_NVIC_SetPriority(I3C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I3C1_ER_IRQn);
  /* USER CODE BEGIN I3C1_MspInit 1 */

  /* USER CODE END I3C1_MspInit 1 */

  }

}

/**
* @brief I3C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi3c: I3C handle pointer
* @retval None
*/
void HAL_I3C_MspDeInit(I3C_HandleTypeDef* hi3c)
{
  if(hi3c->Instance==I3C1)
  {
  /* USER CODE BEGIN I3C1_MspDeInit 0 */

  /* USER CODE END I3C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I3C1_CLK_DISABLE();

    /**I3C1 GPIO Configuration
    PB8     ------> I3C1_SCL
    PB9     ------> I3C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* I3C1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(I3C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I3C1_ER_IRQn);
  /* USER CODE BEGIN I3C1_MspDeInit 1 */

  /* USER CODE END I3C1_MspDeInit 1 */
  }

}


