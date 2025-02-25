/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <adc.h>
#include <dac.h>
#include <fmc.h>
#include <gpio.h>
#include <i2c.h>
#include <i3c.h>
#include <uart.h>
#include <time.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <MCAL/Inc/DAC.h>
#include <MCAL/Inc/ADC.h>
#include <MCAL/Inc/FLASH.h>
#include <MCAL/Inc/GPIO.h>
#include <MCAL/Inc/I2C.h>
#include <MCAL/Inc/I3C.h>
#include <MCAL/Inc/TIME.h>
#include <MCAL/Inc/UART.h>
#include <MCAL/Inc/FMC.h>

#include "ldo.h"

#include "ssd1306_conf.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

DCACHE_HandleTypeDef hdcache1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPDMA1_Init(void);
static void MX_GPDMA2_Init(void);
static void MX_DCACHE1_Init(void);
static void MX_ICACHE_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint32_t key_stable;

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	GPIO_Init();
	MX_GPDMA1_Init();
	MX_GPDMA2_Init();
	MX_DCACHE1_Init();
	MX_USART1_UART_Init();
	ADC_Init();
	DAC_Init();
	Time_init();
	MX_UART5_Init();
	MX_ICACHE_Init();
	MX_FMC_Init();
	/* USER CODE BEGIN 2 */

	I2C_Init();
	UART_Init();

//  LDO_controldown(I3C_REF_PW,64);
//  LDO_controlup(I3C_REF_PW,33) ;
	LDO_ControlMain(I3C_REF_PW);
	LOD_SetVoltage(I3C_REF_PW, 2.8);
	LDO_ControlMain(EXT_PW_OUT_CH1);
	LOD_SetVoltage(EXT_PW_OUT_CH1, 2.8);
	LDO_ControlMain(EXT_PW_OUT_CH2);
	LOD_SetVoltage(EXT_PW_OUT_CH2, 2.8);
	LDO_ControlMain(SW_I2C_REF_PW_CH1);
	LOD_SetVoltage(SW_I2C_REF_PW_CH1, 2.8);
	LDO_ControlMain(SW_I2C_REF_PW_CH2);
	LOD_SetVoltage(SW_I2C_REF_PW_CH2, 2.8);
	PRINTF(("ECHO V3 TestCode\n"));
	//CH1_I3C_SEL(1); //I2C MODE

	I3C_Init(); // the init need to set  LDO_controldown(5,64); LDO_controlup(5,33) ;  with  delay

	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim6);

	CH1_I3C_SEL(1); //I3C MODE
	CH1_I2C_SEL(1); //BYPASS DISABLE

	oLed_Init();  // you need to keep this logic

	I2C_SetChannel(I2C_CH1, I2C_1M);
	int presstime = 1500;
	int step = 2;
//  uint8_t Addr, Data;
//  uint32_t nAck;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		HAL_Delay(100);

		static uint32_t pressStartTime = 0;
		static uint8_t isPressed = 0;

		if (USER_KEY == BUTTON_PRESSED) {
			if (!isPressed) { // 처음 눌렸을 때
				pressStartTime = HAL_GetTick(); // 현재 시간 저장
				isPressed = 1;
			} else if ((HAL_GetTick() - pressStartTime) >= presstime) {
				// 2초 이상 눌렸다면 동작 수행
				PRINTF("KEYPRESS!\n");
				uint16_t data;

				if (step == 0) {
					CH1_I3C_SEL(0); //I2C MODE
					CH1_I2C_SEL(1); //BYPASS DISABLE
					HAL_Delay(10);

					PRINTF("I3C Mode Change!\n");

//	        	// I3C mode Change
//				data=0x0001;
//				I2C_Write_16BIT(0x48, 0xD02B, &data, 1);
//				delay_ms(10);
//
//				// Auto Hot-Join Disable
//				data=0x0000;
//				i3c_write(0x48, 0xD02C, &data, 1);
//				delay_ms(10);
//
//				//LDT ProTect OFF
//				data=0xCADD;
//				I2C_Write_16BIT(0x48, 0xDE93, &data, 1);
//				delay_ms(10);
//
//				//Store
//				data=0x0004;
//				I2C_Write_16BIT(0x48, 0xD012, &data, 1);
//				delay_ms(10);
//
//				//LDT ProTect ON
//				data=0x0000;
//				I2C_Write_16BIT(0x48, 0xDE93, &data, 1);	//LDT1 ON
//				delay_ms(10);
//
//				//Reset DW9785
//				data=0x0001;
//				I2C_Write_16BIT(0x48, 0xD002, &data, 1);
//				delay_ms(10);
//
//				data=0x0003;
//				I2C_Write_16BIT(0x48, 0xDD03, &data, 1);
//				delay_ms(10);
//
//				data=0x0003;
//				I2C_Write_16BIT(0x48, 0xDD04, &data, 1);
//				delay_ms(10);
					step++;
				} else if (step == 1) {
					PRINTF("RSTDAA!\n");
					CH1_I3C_SEL(1);
					delay_ms(10);

					I3C_RSTDAA(_I3C_MODE_);
					//I3C_Send_RSTDAA();
					step = 2;
				} else if (step == 2) {
					PRINTF("ENTDAA!\n");
					delay_ms(10);
					//TEMP_I3C_SETDASA();
					I3C_RSTDAA(_I2C_MODE_);
					I3C_SETDASA(0xD4, 0x32);
					//TEMP_I3C_ENTDAA(0x32);
					step = 3;
				} else if (step == 3) {
					PRINTF("I3C READ!\n");
					uint8_t data[6];
					I3C_Read(0x64, 0x01, data, 1);
					PRINTF("0x%x\n", data[0]);
					PRINTF("0x%x\n", data[1]);
					step = 1;
				}
				isPressed = 0; // 다시 눌릴 때까지 대기
			}
		} else {
			isPressed = 0; // 버튼이 떼어졌을 때 리셋
		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_CSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.CSIState = RCC_CSI_ON;
	RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_CSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 125;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}

	/** Configure the programming delay
	 */
	__HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
}

/**
 * @brief DCACHE1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_DCACHE1_Init(void) {

	/* USER CODE BEGIN DCACHE1_Init 0 */

	/* USER CODE END DCACHE1_Init 0 */

	/* USER CODE BEGIN DCACHE1_Init 1 */

	/* USER CODE END DCACHE1_Init 1 */
	hdcache1.Instance = DCACHE1;
	hdcache1.Init.ReadBurstType = DCACHE_READ_BURST_WRAP;
	if (HAL_DCACHE_Init(&hdcache1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN DCACHE1_Init 2 */

	/* USER CODE END DCACHE1_Init 2 */

}

/**
 * @brief GPDMA1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPDMA1_Init(void) {

	/* USER CODE BEGIN GPDMA1_Init 0 */

	/* USER CODE END GPDMA1_Init 0 */

	/* Peripheral clock enable */
	__HAL_RCC_GPDMA1_CLK_ENABLE();

	/* GPDMA1 interrupt Init */
	HAL_NVIC_SetPriority(GPDMA1_Channel0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);

	/* USER CODE BEGIN GPDMA1_Init 1 */

	/* USER CODE END GPDMA1_Init 1 */
	/* USER CODE BEGIN GPDMA1_Init 2 */

	/* USER CODE END GPDMA1_Init 2 */

}

/**
 * @brief GPDMA2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPDMA2_Init(void) {

	/* USER CODE BEGIN GPDMA2_Init 0 */

	/* USER CODE END GPDMA2_Init 0 */

	/* Peripheral clock enable */
	__HAL_RCC_GPDMA2_CLK_ENABLE();

	/* GPDMA2 interrupt Init */
	HAL_NVIC_SetPriority(GPDMA2_Channel0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(GPDMA2_Channel0_IRQn);

	/* USER CODE BEGIN GPDMA2_Init 1 */

	/* USER CODE END GPDMA2_Init 1 */
	/* USER CODE BEGIN GPDMA2_Init 2 */

	/* USER CODE END GPDMA2_Init 2 */

}

/**
 * @brief ICACHE Initialization Function
 * @param None
 * @retval None
 */
static void MX_ICACHE_Init(void) {

	/* USER CODE BEGIN ICACHE_Init 0 */

	/* USER CODE END ICACHE_Init 0 */

	/* USER CODE BEGIN ICACHE_Init 1 */

	/* USER CODE END ICACHE_Init 1 */

	/** Enable instruction cache (default 2-ways set associative cache)
	 */
	if (HAL_ICACHE_Enable() != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ICACHE_Init 2 */

	/* USER CODE END ICACHE_Init 2 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
