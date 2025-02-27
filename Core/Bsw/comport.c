#include "bsw_config.h"
#include "Comport.h"
#include <MCAL/Inc/MCAL_Common.h>



void Com_Init(COM_Handle* pCom)
{
//	pCom->bOpen = TRUE;
//	pCom->bExtSendMask = FALSE;
//
//	/* Com Packet struct Init --------- */
//	memset( &pCom->Tx, 0x00, COM_PACKET_SIZE);
//	pCom->Tx.Start = 0xAE;
//	pCom->Tx.ID = 0xFA;
//	pCom->Tx.End = 0xEC;
//
//	pCom->pRx = NULL;
}

//void Com_Recv(COM_Handle* pCom)
//{
//
//}
//
//void UARTx_Send(UART_Number uartNum, uint8_t* pBuf, uint32_t nSize)
//{
//    UART_HandleTypeDef* huart = (uartNum == UART_NUM_1) ? &huart1 : &huart5;
//    HAL_UART_Transmit_DMA(huart, pBuf, nSize);
//}
//
//void UARTx_SetExtSend(UART_Number uartNum)
//{
//	uart* uart = (uartNum == UART_NUM_1) ? &uart1 : &uart5;
//    uart->Mode = UART1_EXT_SEND;
//}
//
//void UARTx_ExtSend(UART_Number uartNum, uint8_t* pBuf, uint32_t nSize)
//{
//	uart* uart = (uartNum == UART_NUM_1) ? &uart1 : &uart5;
//	uart->ExtSize = nSize;
//	uart->pExtBuf = pBuf;
//    UART_HandleTypeDef* huart = (uartNum == UART_NUM_1) ? &huart1 : &huart5;
//
//	HAL_UART_Transmit_DMA(huart, uart->pExtBuf, uart->ExtSize);
//}
//
//void UARTx_ExtRecv(UART_Number uartNum, uint32_t nSize)
//{
//	uart* uart = (uartNum == UART_NUM_1) ? &uart1 : &uart5;
//	uart->Mode = UART1_EXT_RECV;	/* Set Extra Buffer Info ---- */
//	uart->ExtSize = nSize;
//	uart->Status = UART1_BUF_EMPTY;
//}
//
//void UARTx_SetDbg(bool set)
//{
//	g_print_debug = set;
//}
//
///*
//*	 Func    : int _printf(const char *fmt, ...)
//*  Comment : UART printf must set print debug option
//*/
////int UARTx_Printf(const char *fmt, ...)
//
//uart* UARTx_GetInfo(UART_Number uartNum)
//{
//    return (uartNum == UART_NUM_1) ? &uart1 : &uart5;
//}
//
//void UARTx_ITOn(UART_Number uartNum)
//{
//	if(uartNum == UART_NUM_1)
//	{
//		HAL_UART_Receive_IT(&huart1, &g_uartRxBuffer, 1);
//	}
//	else if(uartNum == UART_NUM_5)
//	{
//		HAL_UART_Receive_IT(&huart5, &g_uartRxBuffer, 1);
//	}
//}
