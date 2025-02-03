#include "bsw_config.h"

bool g_print_debug = true;



void UARTx_Send(UART_Number uartNum, uint8_t* pBuf, uint32_t nSize)
{
    UART_HandleTypeDef* huart = (uartNum == UART_NUM_1) ? &huart1 : &huart5;
    HAL_UART_Transmit_DMA(huart, pBuf, nSize);
}

void UARTx_SetExtSend(UART_Number uartNum)
{
	uart* uart = (uartNum == UART_NUM_1) ? &uart1 : &uart5;
    uart->Mode = UART1_EXT_SEND;
}

void UARTx_ExtSend(UART_Number uartNum, uint8_t* pBuf, uint32_t nSize)
{
	uart* uart = (uartNum == UART_NUM_1) ? &uart1 : &uart5;
	uart->ExtSize = nSize;
	uart->pExtBuf = pBuf;
    UART_HandleTypeDef* huart = (uartNum == UART_NUM_1) ? &huart1 : &huart5;

	HAL_UART_Transmit_DMA(huart, uart->pExtBuf, uart->ExtSize);
}

void UARTx_ExtRecv(UART_Number uartNum, uint32_t nSize)
{
	uart* uart = (uartNum == UART_NUM_1) ? &uart1 : &uart5;
	uart->Mode = UART1_EXT_RECV;	/* Set Extra Buffer Info ---- */
	uart->ExtSize = nSize;
	uart->Status = UART1_BUF_EMPTY;
}

void UARTx_SetDbg(bool set)
{
	g_print_debug = set;
}

/*
*	 Func    : int _printf(const char *fmt, ...)
*  Comment : UART printf must set print debug option
*/
int UARTx_Printf(const char *fmt, ...)
{
	g_print_debug = 1;

	if(g_print_debug)
	{
		int  i;
		va_list parm;
		char szBuf[256];
		va_start(parm, fmt);
		vsprintf(szBuf, fmt, parm);

		for(i = 0; i < 256 && szBuf[i] /* != null */; i++)
		{
			HAL_UART_Transmit(&huart5, (uint8_t*)&szBuf[i], 1, 1);
		}

		va_end(parm);
	}

	return 0;
}

uart* UARTx_GetInfo(UART_Number uartNum)
{
    return (uartNum == UART_NUM_1) ? &uart1 : &uart5;
}

void UARTx_ITOn(UART_Number uartNum)
{
	if(uartNum == UART_NUM_1)
	{
		HAL_UART_Receive_IT(&huart1, &g_uartRxBuffer, 1);
	}
	else if(uartNum == UART_NUM_5)
	{
		HAL_UART_Receive_IT(&huart5, &g_uartRxBuffer, 1);
	}
}
