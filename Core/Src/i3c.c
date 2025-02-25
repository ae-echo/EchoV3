/*
 * i3c.c
 *
 *  Created on: Jan 13, 2025
 *      Author: Kim Min Ki
 */

#include <i3c.h>

I3C_HandleTypeDef hi3c1;


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
	  hi3c1.Init.CtrlBusCharacteristic.SCLI3CHighDuration = I3C_12_5MHZ;
	  hi3c1.Init.CtrlBusCharacteristic.SCLODLowDuration = I3C_2MHZ;
	  hi3c1.Init.CtrlBusCharacteristic.SCLI2CHighDuration = I3C_2MHZ;
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
    /* I3C1 interrupt DeInit */
  /* USER CODE BEGIN I3C1_MspDeInit 1 */

  /* USER CODE END I3C1_MspDeInit 1 */
  }

}



//uint32_t I3C_ENTDAA1(uint8_t DA, unsigned int DAA)
//{
//	uint8_t cnt =1;
//	uint8_t dw9785=0;
//	DA = DA>>1;
//	do
//	{
//		targetPayload = 0;
//		i3c_status = HAL_I3C_Ctrl_DynAddrAssign(&hi3c1, &targetPayload, DAA, 100);
//		dw9785=1;
//		for (int i = 0; i < 8; i++) {
//
//			payload[i] = (targetPayload >> (i * 8)) & 0xFF;	//dw9785 비교
//			if(DW9785_payload[i]!=payload[i])
//			{
//				dw9785 =0;
//				break;
//			}
//
//		}
//		if (i3c_status == HAL_BUSY && dw9785==1)
//		{
//			HAL_I3C_Ctrl_SetDynAddr(&hi3c1, DA);
//		}
//		else if(i3c_status == HAL_BUSY)
//		{
//			HAL_I3C_Ctrl_SetDynAddr(&hi3c1, DA+cnt);
//			cnt++;
//		}
//
//	} while (i3c_status == HAL_BUSY);
//
//
//	i3c_error = hi3c1.ErrorCode;
//	return i3c_error;
//}

//#define TARGET1_DYN_ADDR        0x32
//#define I3C_CCC_SETDASA 0x87  // SETDASA CCC 코드
//#define LSM6DSO_STA (0xD4>>1)
//uint8_t aDISEC_data[1]   = {0x08};
//uint8_t aSETDASA_LSM6DSO_data[1]   = {(TARGET1_DYN_ADDR << 1)};
//
///* Descriptor for broadcast CCC */
//I3C_CCCTypeDef aBroadcast_CCC[] =
//{
//    /*   Target Addr              CCC Value    CCC data + defbyte pointer  CCC size + defbyte         Direction        */
//    {0,                0x01,           {aDISEC_data,           1},              LL_I3C_DIRECTION_WRITE},
//    {0,                0x06,          {NULL,                  0},              LL_I3C_DIRECTION_WRITE},
//};
//
//I3C_CCCTypeDef aDirect_CCC[] =
//{
//    /*   Target Addr              CCC Value        CCC data + defbyte pointer   CCC size + defbyte        Direction        */
//    {LSM6DSO_STA,         0x87,        {aSETDASA_LSM6DSO_data, 1},      LL_I3C_DIRECTION_WRITE},
//};
//
//uint32_t TEMP_I3C_SETDASA() {
//	aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
//	aContextBuffers[0].CtrlBuf.Size    = COUNTOF(aControlBuffer);
//	aContextBuffers[0].TxBuf.pBuffer   = aTxBuffer;
//	aContextBuffers[0].TxBuf.Size      = 1;
//
//	if (HAL_I3C_AddDescToFrame(&hi3c1,
//							 aBroadcast_CCC,
//							 NULL,
//							 &aContextBuffers[0],
//							 COUNTOF(aBroadcast_CCC),
//							 I3C_BROADCAST_WITHOUT_DEFBYTE_RESTART) != HAL_OK)
//	{
//	/* Error_Handler() function is called when error occurs. */
//	Error_Handler();
//	}
//
//	aContextBuffers[1].CtrlBuf.pBuffer = aControlBuffer;
//	aContextBuffers[1].CtrlBuf.Size    = COUNTOF(aControlBuffer);
//	aContextBuffers[1].TxBuf.pBuffer   = aTxBuffer;
//	aContextBuffers[1].TxBuf.Size      = 1;
//
//	if (HAL_I3C_Ctrl_TransmitCCC_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
//	{
//	/* Error_Handler() function is called when error occurs. */
//	Error_Handler();
//	}
//
//	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY)
//	{
//	}
//
//	/* SETDASA CCC 프레임 추가 */
//	if (HAL_I3C_AddDescToFrame(&hi3c1,
//	                           aDirect_CCC,
//	                           NULL,
//	                           &aContextBuffers[1],
//							   COUNTOF(aDirect_CCC),
//	                           I3C_DIRECT_WITHOUT_DEFBYTE_STOP) != HAL_OK)
//	{
//	    Error_Handler(); // 오류 처리
//	}
//
//	/* SETDASA 명령 전송 */
//	if (HAL_I3C_Ctrl_TransmitCCC_IT(&hi3c1, &aContextBuffers[1]) != HAL_OK)
//	{
//	    Error_Handler(); // 오류 처리
//	}
//
//}

//uint32_t TEMP_I3C_READ(uint8_t id, uint8_t addr, uint8_t* dat, uint16_t size)
//{
//	unsigned int w_mode ;
//	unsigned int r_mode ;
//
//	w_mode = I3C_PRIVATE_WITHOUT_ARB_RESTART;
//	r_mode = I3C_PRIVATE_WITHOUT_ARB_STOP;
//
//	aTxBuffer[0] = addr;
//	//aTxBuffer[0] = (uint8_t)((uint16_t) addr >> 8);
//	//aTxBuffer[1] = (uint8_t) addr;
//
//	//write setting
//	aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
//	aContextBuffers[0].CtrlBuf.Size = 1;
//	aContextBuffers[0].TxBuf.pBuffer = aTxBuffer;
//	aContextBuffers[0].TxBuf.Size = size;	//addr
//
//	aPrivateDescriptor[0].TargetAddr = id>>1;
//	aPrivateDescriptor[0].TxBuf.pBuffer = aTxBuffer;
//	aPrivateDescriptor[0].TxBuf.Size =size;	//addr
//	aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_WRITE;
//
//	HAL_I3C_AddDescToFrame(&hi3c1,
//			NULL,
//			&aPrivateDescriptor[0],
//			&aContextBuffers[0],
//			aContextBuffers[0].CtrlBuf.Size,
//			w_mode);
//
//	HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]);
//
//	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY)
//	{
//	}
//
//	//read setting
//	aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
//	aContextBuffers[0].CtrlBuf.Size = 1;
//	aContextBuffers[0].RxBuf.pBuffer = dat;
//	aContextBuffers[0].RxBuf.Size = 2;
//
//	aPrivateDescriptor[0].TargetAddr = id>>1;
//	aPrivateDescriptor[0].RxBuf.pBuffer = dat;
//	aPrivateDescriptor[0].RxBuf.Size = 2;	//addr
//	aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_READ;
//
//	HAL_I3C_AddDescToFrame(&hi3c1,
//			NULL,
//			&aPrivateDescriptor[0],
//			&aContextBuffers[0],
//			aContextBuffers[0].CtrlBuf.Size,
//			r_mode);	//i2c set
//
//	HAL_I3C_Ctrl_Receive_IT(&hi3c1, &aContextBuffers[0]);
//
//	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY)
//	{
//	}
//
//	  return (uint16_t)(((uint16_t)aRxBuffer[0] << 8) + (uint16_t)aRxBuffer[1]);
//}
//


