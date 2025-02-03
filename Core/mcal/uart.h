/*
 * uart.h
 *
 *  Created on: Jan 6, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_UART_H_
#define MCAL_UART_H_

#include "main.h"


/* USER CODE BEGIN Private defines */
#define COM_PORT_BAUD		    2995200
#define DBG_PORT_BAUD		    921600

#define UART1_PACKET_SIZE				12

#define UART1_BUF_EMPTY			0
#define UART1_BUF_FULL			1
#define UART1_EXT_BUF_FULL		2


typedef enum {
    UART_NUM_1 = 1,
    UART_NUM_5 = 5
} UART_Number;

enum UART1_MODE
{
    UART1_CMD_RECV = 0,
    UART1_EXT_RECV = 1,
	UART1_EXT_SEND
};


typedef struct
{
	uint32_t Status;
	uint32_t Mode;
	uint32_t Count;
	uint8_t Buf[20];

    uint32_t ExtCount;
	uint32_t ExtSize;
    uint8_t* pExtBuf;

}uart;


extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern uint8_t g_uartRxBuffer;
extern uart uart1;  // usart1
extern uart uart5;  // uart5

void UART1_Init(void);

void MX_USART1_UART_Init(void);
void MX_UART5_Init(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);




#endif /* MCAL_UART_H_ */
