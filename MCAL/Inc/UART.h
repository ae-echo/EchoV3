#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include <uart.h>
#include "bsw_config.h"
#include <fmc.h>
#include <gpio.h>

/* USER CODE BEGIN Private defines */

#define UART1_PACKET_SIZE		12
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
	uint32_t nCount;
	uint8_t Buf[20];

    uint32_t ExtCount;
	uint32_t ExtSize;
    uint8_t* pExtBuf;

}uart;



extern uint8_t g_uartRxBuffer;
extern uart uart1;  // usart1
extern uart uart5;  // uart5

/**
 * @brief  UART1 초기화 함수
 *         - UART1의 버퍼 및 상태 변수를 초기화하고, 인터럽트 수신을 활성화
 */
void UART_Init(void);

/**
 * @brief  UART1 정보 반환 함수
 *         - UART1의 현재 상태 정보를 반환
 * @return `struct uart*` UART1 상태 정보 구조체 포인터
 */
struct uart* UART1_GetInfo(void);

/**
 * @brief  UART1 데이터 송신 함수
 *         - 지정된 버퍼(`pBuf`)의 데이터를 UART1을 통해 전송 (DMA 방식)
 * @param  pBuf  전송할 데이터 버퍼 포인터
 * @param  nSize 전송할 데이터 크기 (바이트 단위)
 */
void UART1_Send(uint8_t* pBuf, uint32_t nSize);

/**
 * @brief  UART1 확장 데이터 송신 함수
 *         - 확장 버퍼(`pExtBuf`)를 사용하여 UART1을 통해 데이터를 전송 (DMA 방식)
 * @param  pBuf  전송할 데이터 버퍼 포인터
 * @param  nSize 전송할 데이터 크기 (바이트 단위)
 */
void UART1_ExtSend(uint8_t* pBuf, uint32_t nSize);

/**
 * @brief  UART1 확장 수신 모드 설정 함수
 *         - UART1을 확장 데이터 수신 모드(`UART1_EXT_RECV`)로 설정하고, 버퍼 상태를 초기화
 * @param  nSize  확장 수신할 데이터 크기 (바이트 단위)
 */
void UART1_SetExtRecv(uint32_t nSize);

/**
 * @brief  UART1 확장 송신 모드 설정 함수
 *         - UART1을 확장 데이터 송신 모드(`UART1_EXT_SEND`)로 설정
 */
void UART1_SetExtSend(void);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);




#endif /* INC_UART_H_ */
