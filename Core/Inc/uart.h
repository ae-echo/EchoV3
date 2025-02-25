/*
 * uart.h
 *
 *  Created on: Jan 6, 2025
 *      Author: Kim Min Ki
 */

#ifndef MCAL_UART_H_
#define MCAL_UART_H_

#include "main.h"

#define COM_PORT_BAUD		    2995200
#define DBG_PORT_BAUD		    921600

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;

void MX_USART1_UART_Init(void);
void MX_UART5_Init(void);


#endif /* MCAL_UART_H_ */
