/**
  ******************************************************************************
  * File Name          : FMC.h
  * Description        : This file provides code for the configuration
  *                      of the FMC peripheral.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FMC_H
#define __FMC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SDRAM_HandleTypeDef hsdram2;

/* USER CODE BEGIN Private defines */

/* Private define for SDRAM ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)
#define REFRESH_COUNT       ((uint32_t)0x0569)   /* SDRAM refresh counter (90MHz SD clock) */

/* Exported constants ------------------------------------------------------------------*/
#define SDRAM_BANK_ADDR                 ((uint32_t)0xD0000000)

/* #define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_8 */
#define SDRAM_MEMORY_WIDTH            FMC_SDRAM_MEM_BUS_WIDTH_16

/* #define SDCLOCK_PERIOD                   FMC_SDRAM_CLOCK_PERIOD_2 */
#define SDCLOCK_PERIOD                FMC_SDRAM_CLOCK_PERIOD_3

#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF)

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

//#define FMC_SDRAM_BLOCK						     (

#define FMC_SDRAM_BLOCK_0     	((uint32_t)0x00080000) /* SDRAM Block 0 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_1     	((uint32_t)0x00100000) /* SDRAM Block 1 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_2     	((uint32_t)0x00180000) /* SDRAM Block 2 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_3     	((uint32_t)0x00200000) /* SDRAM Block 3 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_4     	((uint32_t)0x00280000) /* SDRAM Block 4 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_5     	((uint32_t)0x00380000) /* SDRAM Block 5 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_6     	((uint32_t)0x00400000) /* SDRAM Block 6 - 512 Kbyte */
#define FMC_SDRAM_BLOCK_7     	((uint32_t)0x00480000) /* SDRAM Block 7 - 512 Kbyte */


//#define FMC_SDRAM_BLOCK_0     	((uint32_t)0xD0080000) /* SDRAM Block 0 - 512 Kbyte */
//#define FMC_SDRAM_BLOCK_1     	((uint32_t)0xD0100000) /* SDRAM Block 1 - 512 Kbyte */
//#define FMC_SDRAM_BLOCK_2     	((uint32_t)0xD0180000) /* SDRAM Block 2 - 512 Kbyte */
//#define FMC_SDRAM_BLOCK_3     	((uint32_t)0xD0200000) /* SDRAM Block 3 - 512 Kbyte */
//#define FMC_SDRAM_BLOCK_4     	((uint32_t)0xD0280000) /* SDRAM Block 4 - 512 Kbyte */
//#define FMC_SDRAM_BLOCK_5     	((uint32_t)0xD0380000) /* SDRAM Block 5 - 512 Kbyte */
//#define FMC_SDRAM_BLOCK_6     	((uint32_t)0xD0400000) /* SDRAM Block 6 - 512 Kbyte */
//#define FMC_SDRAM_BLOCK_7     	((uint32_t)0xD0480000) /* SDRAM Block 7 - 512 Kbyte */

enum
{
    FMC_OK = 0,
    FMC_FAIL = 1
};

enum
{
	FMC_BLOCK_0 = 0,
	FMC_BLOCK_1 = 1,
	FMC_BLOCK_2,
	FMC_BLOCK_3,
	FMC_BLOCK_4,
	FMC_BLOCK_5,
	FMC_BLOCK_6,
	FMC_BLOCK_7

};

//#define SDRAM_

/* USER CODE END Private defines */

void MX_FMC_Init(void);
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* hsdram);
void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* hsdram);
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);
void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* hsdram);
void HAL_FMC_MspDeInit(void);
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* hsdram);
void HAL_FMC_MspInit(void);


/* USER CODE BEGIN Prototypes */
uint32_t Read_SDRAM_32bit(uint32_t sdram_address);
void Wirte_SDRAM_32bit(uint32_t write_data,uint32_t sdram_address);
/*
 * minki
 *
 * SDRAM REad wirte
 */


uint32_t SDRAM_RW_Func(uint32_t mode, uint32_t bit, uint32_t data);
uint32_t SDRAM_RW_BYTE(uint32_t mode, uint32_t data);
uint32_t FMC_Write(uint32_t Offset, uint32_t Size, uint8_t* pBuf);
uint32_t FMC_Read(uint32_t Offset, uint32_t Size, uint8_t* pBuf);
uint8_t* FMC_GetBufAddr();
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__FMC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
