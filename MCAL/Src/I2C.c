#include <MCAL/Inc/I2C.h>


struct I2C_Irq_Buf	    I2C_Buf;

/**
 * @brief  I2C 인터페이스 초기화 함수
 *         - I2C2 및 I2C4 초기화
 *         - I2C 버퍼 초기화
 *         - I2C2 인터럽트 리스닝 활성화
 */
void I2C_Init(void)
{
	MX_I2C2_Init();
	MX_I2C4_Init();

	memset(&I2C_Buf, 0x00, sizeof(struct I2C_Irq_Buf));		// I2C Reg Buffer Init

    if (HAL_I2C_EnableListen_IT(&hi2c2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief  I2C 채널 및 속도 설정
 *         - 지정된 I2C 채널의 SDA, SCL 핀 설정
 *         - I2C 통신 속도 설정
 * @param  i2c_ch    I2C 채널 (I2C_CH1 또는 I2C_CH2)
 * @param  speed     I2C 속도 (I2C_100K, I2C_200K, ..., I2C_3M)
 */
void I2C_SetChannel(uint8_t i2c_ch, enum I2C_Speed speed)
{

	/* I2C GPIO Port Set --------- */
    I2C_Port.pPort = I2C_RL_Port;

    if(i2c_ch == I2C_CH1)
	{
		/* I2C Pin, SW Set ---------- */
		I2C_Port.SDA = I2C1_SDA_Pin;
		I2C_Port.SCL = I2C1_SCL_Pin;

		CH1_I2C_SEL(1);		// BYPASS DISABLE
		CH1_I3C_SEL(0);		// I2C MODE
	}
	else if(i2c_ch == I2C_CH2)
	{
		/* I2C Pin, SW Set ---------- */
		I2C_Port.SDA = I2C2_SDA_Pin;
		I2C_Port.SCL = I2C2_SCL_Pin;

		CH2_I2C_SEL(1);		// BYPASS DISABLE
		CH2_I3C_SEL(0);		// I2C MODE
	}

    I2C_SetSpeed(speed);
}

/**
 * @brief  I2C 통신 속도 설정
 *         - 입력된 I2C 속도(enum I2C_Speed)에 따라 I2C 포트 속도 설정
 * @param  Speed  설정할 I2C 속도 (I2C_100K ~ I2C_3M)
 */
void I2C_SetSpeed(enum I2C_Speed Speed)
{
	switch(Speed)
	{
		case I2C_100K:			I2C_Port.Speed = I2C_RATE_100K;		break;
		case I2C_200K:			I2C_Port.Speed = I2C_RATE_200K;		break;
		case I2C_400K:			I2C_Port.Speed = I2C_RATE_400K;		break;
		case I2C_1M:			I2C_Port.Speed = I2C_RATE_1M;		break;
		case I2C_2M:			I2C_Port.Speed = I2C_RATE_2M;		break;
		case I2C_3M:			I2C_Port.Speed = I2C_RATE_3M;		break;
		default:                I2C_Port.Speed = I2C_RATE_400K;		break;
	}
}

/**
 * @brief  I2C 16비트 데이터 쓰기
 *         - 16비트 주소(Addr)와 16비트 데이터(pData)를 I2C로 전송
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    16비트 메모리 주소
 * @param  pData   전송할 16비트 데이터 배열 포인터
 * @param  Size    전송할 데이터 개수
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Write_16BIT(uint8_t Device, uint16_t Addr, uint16_t* pData, uint16_t Size)
{
	uint32_t ret = I2C_NACK;
	uint8_t tBuf[4];
	uint8_t i;

	for(i = 0; i < Size; i++)
	{
		tBuf[0] = MSB(Addr+i);
        tBuf[1] = LSB(Addr+i);
		tBuf[2] = MSB(pData[i]);
        tBuf[3] = LSB(pData[i]);

		ret = I2C_GPIO_Write_Func(Device, tBuf, 4, I2C_Port.Speed, sizeof(uint8_t));
	}
	PRINTF("I2C Write %.4X %.4X \r\n", Addr, pData[0]);

	return ret;
}

/**
 * @brief  I2C 8비트 주소 데이터 쓰기
 *         - 8비트 주소(Addr)에 8비트 데이터(pData) 전송
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    8비트 레지스터 주소
 * @param  pData   전송할 8비트 데이터 배열 포인터
 * @param  Size    전송할 데이터 개수
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Write(uint8_t Device, uint8_t Addr, uint8_t* pData, uint16_t Size)
{
	uint32_t ret = I2C_NACK;
	uint8_t tBuf[3];
	uint8_t i;

	tBuf[0] = Addr;

	for(i = 0; i < Size; i++)
	{
		tBuf[1 + i] = pData[i];
	}

	ret = I2C_GPIO_Write_Func(Device, tBuf, (Size + 1), I2C_Port.Speed, sizeof(uint8_t));
	return ret;
}

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
uint32_t I2C_Write_Seq(uint8_t Device, uint16_t Addr, uint8_t AddrSize, uint32_t Size, uint8_t* pBuf)
{
	uint32_t ret = I2C_NACK;
	uint8_t tBuf[2 + 255];			// Max Addr 2bytes + Max Data 255bytes
	uint8_t i;

	if(AddrSize == 1)
	{
		tBuf[0] = LSB(Addr);
	}
	else if(AddrSize == 2)
	{
		tBuf[0] = MSB(Addr);
		tBuf[1] = LSB(Addr);
	}

	PRINTF("I2C Write Seq ID %.2X Addr %.4X AddrSize %d Data : ", Device, Addr, AddrSize);

	for(i = 0; i < Size; i++)
	{
		tBuf[AddrSize + i] = pBuf[i];
		PRINTF("%.2X ", tBuf[AddrSize + i]);
	}

	PRINTF(("\r\n"));

	ret = I2C_GPIO_Write_Func(Device, tBuf, (Size + AddrSize), I2C_Port.Speed, sizeof(uint8_t));

	return ret;
}

/**
 * @brief  I2C 8비트 주소 읽기
 *         - 8비트 주소(Addr)에서 데이터를 읽어 pData에 저장
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    읽을 8비트 레지스터 주소
 * @param  pData   읽은 데이터를 저장할 버퍼 포인터
 * @param  Size    읽을 데이터 크기
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Read(uint8_t Device, uint8_t Addr, uint8_t* pData, uint16_t Size)
{
	uint32_t ret = I2C_NACK;

	ret = I2C_GPIO_Write_Func(Device, &Addr, 1, I2C_Port.Speed, sizeof(uint8_t));
	if(ret != I2C_NACK)
	{
		ret = I2C_GPIO_Read_Func(Device|0x01, pData, Size, I2C_Port.Speed, sizeof(uint8_t));
	}
	return ret;
}

/**
 * @brief  I2C 16비트 주소 읽기
 *         - 16비트 주소(Addr)에서 16비트 데이터를 읽어 pData에 저장
 * @param  Device  대상 I2C 장치 주소
 * @param  Addr    16비트 메모리 주소
 * @param  pData   읽은 데이터를 저장할 포인터
 * @param  Size    읽을 데이터 개수 (16비트 단위)
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Read_16BIT(uint8_t Device, uint16_t Addr, uint16_t* pData, uint16_t Size)
{
	uint32_t ret = I2C_NACK;
	uint8_t tBuf[2];
	uint8_t rBuf[4];


	tBuf[0] = MSB(Addr);
	tBuf[1] = LSB(Addr);

	ret = I2C_GPIO_Write_Func(Device, tBuf, 2, I2C_Port.Speed, sizeof(uint8_t));
	if(ret != I2C_NACK)
	{
		ret = I2C_GPIO_Read_Func(Device|0x01, rBuf, Size*2, I2C_Port.Speed, sizeof(uint8_t));
	}

	*pData = MAKEWORD(rBuf[1], rBuf[0]);

	PRINTF("I2C Read %.4X %.4X \r\n", Addr, MAKEWORD(rBuf[1], rBuf[0]) );

	return ret;
}

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
uint32_t I2C_Read_Seq(uint8_t Device, uint16_t Addr, uint8_t AddrSize, uint32_t Size, uint8_t* pBuf)
{
	uint32_t ret = I2C_NACK;
    uint8_t tBuf[2] = {0, };

	if(AddrSize == 1)
	{
		tBuf[0] = LSB(Addr);
	}
	else if(AddrSize == 2)
	{
		tBuf[0] = MSB(Addr);
		tBuf[1] = LSB(Addr);
	}

	PRINTF("I2C Read Seq Addr %.4X AddrSize %d Size %d \r\n ", Addr, AddrSize, Size);

	I2C_GPIO_Write_Func(Device, tBuf, AddrSize, I2C_Port.Speed, sizeof(u8));
	ret = I2C_GPIO_Read_Func(Device|0x01, pBuf, Size, I2C_Port.Speed, sizeof(u8));


	return ret;
}

/**
 * @brief  I2C 연속 데이터 읽기 (SDRAM 저장용)
 *         - 1바이트 또는 2바이트 주소(Addr)를 사용하여 여러 개의 데이터를 SDRAM에 직접 저장
 * @param  Device    대상 I2C 장치 주소
 * @param  Addr      읽을 메모리 주소 (1 또는 2바이트)
 * @param  AddrSize  주소 크기 (1: 8비트, 2: 16비트)
 * @param  Size      읽을 데이터 크기 (최대 255바이트)
 * @return I2C 전송 결과 (ACK/NACK)
 */
uint32_t I2C_Read_Seq_SDRAM(uint8_t Device, uint16_t Addr, uint8_t AddrSize, uint32_t Size)
{
	uint32_t ret = I2C_NACK;
    uint8_t tBuf[2] = {0, };

	if(AddrSize == 1)
	{
		tBuf[0] = LSB(Addr);
	}
	else if(AddrSize == 2)
	{
		tBuf[0] = MSB(Addr);
		tBuf[1] = LSB(Addr);
	}

	PRINTF("I2C Read Seq Addr %.4X AddrSize %d Size %d \r\n ", Addr, AddrSize, Size);

	I2C_GPIO_Write_Func(Device, tBuf, AddrSize, I2C_Port.Speed, sizeof(uint8_t));
	ret = I2C_GPIO_Read_SDRAM(Device|0x01, Size, I2C_Port.Speed, sizeof(uint8_t));

	return ret;
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C2)
    {
        if (hi2c->XferSize == 1)
        {
            // Read 요청을 위한 Register Address 설정 단계
            I2C_Buf.RegAddr = I2C_Buf.RxBuf[0];
        }
        else if (hi2c->XferSize > 1)
        {
            // Write 요청: Master가 데이터를 보낸 경우
            //memcpy(&I2C_Buf.TxBuf[I2C_Buf.RegAddr], &I2C_Buf.RxBuf[1], hi2c->XferSize - 1);
        }

        // Read 완료 후 Listen 모드 재진입
        memset(I2C_Buf.RxBuf, 0, sizeof(I2C_Buf.RxBuf));
        HAL_I2C_EnableListen_IT(hi2c);
    }

}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{

    if (hi2c->Instance == I2C2)
    {
        if (AddrMatchCode != hi2c->Init.OwnAddress1)
        {
            // 잘못된 주소 요청 → NACK 전송
            hi2c->Instance->CR2 |= I2C_CR2_NACK;
            return;
        }

        // 기존 처리 로직...
        if (TransferDirection == I2C_DIRECTION_TRANSMIT) // Master가 Write 요청
        {
            HAL_I2C_Slave_Seq_Receive_IT(hi2c, I2C_Buf.RxBuf, sizeof(I2C_Buf.RxBuf), I2C_NEXT_FRAME);
        }
        else if (TransferDirection == I2C_DIRECTION_RECEIVE) // Master가 Read 요청
        {
            HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &I2C_Buf.TxBuf[I2C_Buf.RegAddr], sizeof(I2C_Buf.TxBuf), I2C_NEXT_FRAME);
        }
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}


