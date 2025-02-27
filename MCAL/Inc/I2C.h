#ifndef INC_I2C_H_
#define INC_I2C_H_

#include <MCAL/Inc/MCAL_Common.h>
#include <i2c.h>

#define BOARD_SLAVE_ID  			0x24

/*
 * CH1_I2C_SEL -  / 1 - BYPASS DISABLE / 0 - BYPASS ENABLE
 * CH1_13C_SEL -  / 0 - I2CMODE		   / 1 - I3CMODE
 */
#define CH1_I2C_SEL(in)    GPIO_Write(GPIOB, GPIO_PIN_1, (GPIO_PinState)in)
#define CH1_I3C_SEL(in)    GPIO_Write(GPIOB, GPIO_PIN_2, (GPIO_PinState)in)
#define CH2_I2C_SEL(in)    GPIO_Write(GPIOC, GPIO_PIN_5, (GPIO_PinState)in)
#define CH2_I3C_SEL(in)    GPIO_Write(GPIOB, GPIO_PIN_0, (GPIO_PinState)in)

#define I2C_NACK		1
#define I2C_ACK			0

/* I2C Speed Delay */
#define I2C_RATE_100K			310
#define I2C_RATE_200K			155
#define I2C_RATE_400K			75
#define I2C_RATE_1M				28
#define I2C_RATE_1_5M			16
#define I2C_RATE_2M				12
#define I2C_RATE_2_5M			9
#define I2C_RATE_3M				6

#define I2C_RL_Port        GPIOE

#define I2C1_SCL_Pin GPIO_PIN_2
#define I2C1_SDA_Pin GPIO_PIN_3
#define I2C2_SCL_Pin GPIO_PIN_4
#define I2C2_SDA_Pin GPIO_PIN_5

/* I2C Channel */
enum I2C_Num
{
	I2C_OFF = 0,
	I2C_CH1 = 1,
	I2C_CH2
};

/* I2C Speed enumeration --------- */
enum I2C_Speed
{
	I2C_100K = 0,
	I2C_200K = 1,
	I2C_400K = 2,
	I2C_1M = 3,
	I2C_2M = 4,
	I2C_3M = 5
};


struct I2C_Irq_Buf
{
	uint8_t RegAddr;
	uint8_t RxBuf[255];
	uint8_t TxBuf[255];
	uint8_t RxCnt;
	uint8_t TxCnt;
};



/**
 * @brief  I2C 인터페이스 초기화 함수
 *         - I2C2 및 I2C4 초기화
 *         - I2C 버퍼 초기화
 *         - I2C2 인터럽트 리스닝 활성화
 */
void I2C_Init(void);

/**
 * @brief  I2C 채널 및 속도 설정
 *         - 지정된 I2C 채널의 SDA, SCL 핀 설정
 *         - I2C 통신 속도 설정
 * @param  i2c_ch    I2C 채널 (I2C_CH1 또는 I2C_CH2)
 * @param  speed     I2C 속도 (I2C_100K, I2C_200K, ..., I2C_3M)
 */
void I2C_SetChannel(uint8_t i2c_ch, enum I2C_Speed speed);

/**
 * @brief  I2C 통신 속도 설정
 *         - 입력된 I2C 속도(enum I2C_Speed)에 따라 I2C 포트 속도 설정
 * @param  Speed  설정할 I2C 속도 (I2C_100K ~ I2C_3M)
 */
void I2C_SetSpeed(enum I2C_Speed Speed);

/**
 * @brief  I2C 16비트 데이터 쓰기
 *         - 16비트 주소(Addr)와 16비트 데이터(pData)를 I2C로 전송
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    16비트 메모리 주소
 * @param  pData   전송할 16비트 데이터 배열 포인터
 * @param  Size    전송할 데이터 개수
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Write(uint8_t Device, uint8_t Addr, uint8_t* pData, uint16_t Size);

/**
 * @brief  I2C 8비트 주소 데이터 쓰기
 *         - 8비트 주소(Addr)에 8비트 데이터(pData) 전송
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    8비트 레지스터 주소
 * @param  pData   전송할 8비트 데이터 배열 포인터
 * @param  Size    전송할 데이터 개수
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Write_16BIT(uint8_t Device, uint16_t Addr, uint16_t* pData, uint16_t Size);

/**
 * @brief  I2C 연속 데이터 쓰기 (Sequential Write)
 *         - 1바이트 또는 2바이트 주소(Addr)를 사용하여 여러 개의 데이터를 연속 전송
 * @param  Device    대상 I2C 장치 주소
 * @param  Addr      메모리 주소 (1 또는 2바이트)
 * @param  AddrSize  주소 크기 (1: 8비트, 2: 16비트)
 * @param  Size      전송할 데이터 크기 (최대 255바이트)
 * @param  pBuf      전송할 데이터 버퍼 포인터
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Write_Seq(uint8_t Device, uint16_t Addr, uint8_t AddrSize, uint32_t Size, uint8_t* pBuf);

/**
 * @brief  I2C 8비트 주소 읽기
 *         - 8비트 주소(Addr)에서 데이터를 읽어 pData에 저장
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    읽을 8비트 레지스터 주소
 * @param  pData   읽은 데이터를 저장할 버퍼 포인터
 * @param  Size    읽을 데이터 크기
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Read(uint8_t Device, uint8_t Addr, uint8_t* pData, uint16_t Size);

/**
 * @brief  I2C 16비트 주소 읽기
 *         - 16비트 주소(Addr)에서 16비트 데이터를 읽어 pData에 저장
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    16비트 메모리 주소
 * @param  pData   읽은 데이터를 저장할 포인터
 * @param  Size    읽을 데이터 개수 (16비트 단위)
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Read_16BIT(uint8_t Device, uint16_t Addr, uint16_t* pData, uint16_t Size);

/**
 * @brief  I2C 연속 데이터 읽기 (Sequential Read)
 *         - 1바이트 또는 2바이트 주소(Addr)를 사용하여 여러 개의 데이터를 연속 읽음
 * @param  Device    대상 I2C 장치 주소
 * @param  Addr      읽을 메모리 주소 (1 또는 2바이트)
 * @param  AddrSize  주소 크기 (1: 8비트, 2: 16비트)
 * @param  Size      읽을 데이터 크기 (최대 255바이트)
 * @param  pBuf      읽은 데이터를 저장할 버퍼 포인터
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Read_Seq(uint8_t Device, uint16_t Addr, uint8_t AddrSize, uint32_t Size, uint8_t* pBuf);

/**
 * @brief  I2C 연속 데이터 읽기 (SDRAM 저장용)
 *         - 1바이트 또는 2바이트 주소(Addr)를 사용하여 여러 개의 데이터를 SDRAM에 직접 저장
 * @param  Device    대상 I2C 장치 주소
 * @param  Addr      읽을 메모리 주소 (1 또는 2바이트)
 * @param  AddrSize  주소 크기 (1: 8비트, 2: 16비트)
 * @param  Size      읽을 데이터 크기 (최대 255바이트)
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Read_Seq_SDRAM(uint8_t Device, uint16_t Addr, uint8_t AddrSize, uint32_t Size);

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);

#endif /* INC_I2C_H_ */
