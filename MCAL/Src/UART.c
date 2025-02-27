#include <MCAL/Inc/UART.h>

struct uart uart1;  // usart1
struct uart uart5;  // uart5

uint8_t g_uartRxBuffer;
bool g_print_debug;

void UART1_Init(void);

void UART_Init(void)
{
	MX_USART1_UART_Init();
	MX_UART5_Init();
	UART1_Init();
}

/**
 * @brief  UART1 초기화 함수
 *         - UART1의 버퍼 및 상태 변수를 초기화하고, 인터럽트 수신을 활성화
 */
void UART1_Init(void)
{
	uart1.Status = UART1_BUF_EMPTY;
	uart1.nCount = 0;
	uart1.Mode = UART1_CMD_RECV;
	uart1.ExtCount = 0;
	uart1.ExtSize = 0;

	g_uartRxBuffer = 0;

	memset(&uart1.Buf, 0x00, sizeof(uart1.Buf));
	uart1.pExtBuf = NULL;

	HAL_UART_Receive_IT(&huart1, &g_uartRxBuffer, 1);
}



/**
 * @brief  UART1 정보 반환 함수
 *         - UART1의 현재 상태 정보를 반환
 * @return `struct uart*` UART1 상태 정보 구조체 포인터
 */
struct uart* UART1_GetInfo(void)
{
	return &uart1;
}

/**
 * @brief  UART1 데이터 송신 함수
 *         - 지정된 버퍼(`pBuf`)의 데이터를 UART1을 통해 전송 (DMA 방식)
 * @param  pBuf  전송할 데이터 버퍼 포인터
 * @param  nSize 전송할 데이터 크기 (바이트 단위)
 */
void UART1_Send(uint8_t* pBuf, uint32_t nSize)
{
	HAL_UART_Transmit_DMA(&huart1, pBuf, nSize);
}

/**
 * @brief  UART1 확장 데이터 송신 함수
 *         - 확장 버퍼(`pExtBuf`)를 사용하여 UART1을 통해 데이터를 전송 (DMA 방식)
 * @param  pBuf  전송할 데이터 버퍼 포인터
 * @param  nSize 전송할 데이터 크기 (바이트 단위)
 */
void UART1_ExtSend(uint8_t* pBuf, uint32_t nSize)
{
	uart1.ExtSize = nSize;
	uart1.pExtBuf = pBuf;

	HAL_UART_Transmit_DMA(&huart1, uart1.pExtBuf, uart1.ExtSize);
}

/**
 * @brief  UART1 확장 수신 모드 설정 함수
 *         - UART1을 확장 데이터 수신 모드(`UART1_EXT_RECV`)로 설정하고, 버퍼 상태를 초기화
 * @param  nSize  확장 수신할 데이터 크기 (바이트 단위)
 */
void UART1_SetExtRecv(uint32_t nSize)
{
	uart1.Mode = UART1_EXT_RECV;

	/* Set Extra Buffer Info */
	uart1.ExtSize = nSize;
	uart1.Status = UART1_BUF_EMPTY;
}

/**
 * @brief  UART1 확장 송신 모드 설정 함수
 *         - UART1을 확장 데이터 송신 모드(`UART1_EXT_SEND`)로 설정
 */
void UART1_SetExtSend(void)
{
	uart1.Mode = UART1_EXT_SEND;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(uart1.Mode == UART1_CMD_RECV)
	{
		if(huart1.RxState == HAL_UART_STATE_READY)
		{
			uart1.Buf[uart1.nCount++] = g_uartRxBuffer;
			if(uart1.nCount >= UART1_PACKET_SIZE)
			{
				uart1.Status = UART1_BUF_FULL;
				uart1.nCount = 0;
			}

			HAL_UART_Receive_IT(&huart1, &g_uartRxBuffer, 1);
		}
	}
	else if(uart1.Mode == UART1_EXT_RECV)
	{
		if(huart1.RxState == HAL_UART_STATE_READY)
		{
			uart1.Status = UART1_BUF_FULL;
			uart1.ExtCount = 0;
		}
		HAL_UART_Receive_IT(&huart1, &g_uartRxBuffer, 1);
	}
}

int UARTx_Printf(const char *file, int line, const char *fmt, ...)

{
	g_print_debug = 1;

	if (g_print_debug)
	{
		va_list args;
		char buffer[256];

		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);

		// 파일명과 라인번호를 추가하여 출력
		char logBuffer[300];
		snprintf(logBuffer, sizeof(logBuffer), "[%s:%d] %s", file, line, buffer);

		HAL_UART_Transmit(&huart5, (uint8_t*)logBuffer, strlen(logBuffer), HAL_MAX_DELAY);
	}
    return 0;
}

