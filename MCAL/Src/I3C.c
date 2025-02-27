#include <MCAL/Inc/I3C.h>


/* I3C Value */
uint32_t i3c_error;
HAL_StatusTypeDef i3c_status = HAL_OK;

/* I3C R/W Variable */
I3C_XferTypeDef aContextBuffers[2];
uint32_t aControlBuffer[0xFF];
uint8_t aTxBuffer[256] = {0x00, };
uint8_t aRxBuffer[256] = {0x00, };

I3C_PrivateTypeDef aPrivateDescriptor[1] =
{
{0x00, {NULL, 0}, {NULL, 0}, HAL_I3C_DIRECTION_READ},
};

/* I3C ENTDAA Variable */
uint64_t targetPayload;

void I3C_SetContext(I3C_XferTypeDef* context, I3C_PrivateTypeDef* desc, uint8_t* txBuf, uint8_t* rxBuf, uint16_t size, uint8_t addr, uint8_t dir);
uint8_t isPayloadMatched(const uint8_t* refPayload);

void I3C_Init(void)
{
	MX_I3C1_Init();
}

/**
 * @brief  I3C 채널 설정
 *         - 지정된 I3C 채널 핀 설정
 * @param  i2c_ch    I2C 채널 (I3C_CH1 또는 I3C_CH2)\
 */
void I3C_SetChannel(uint8_t i3c_ch)
{
	if(i3c_ch == I3C_CH1)
	{
		CH1_I2C_SEL(1);		// CH1 BYPASS DISABLE
		CH1_I3C_SEL(1);		// CH1 I3C MODE
		CH2_I2C_SEL(1);		// CH2 BYPASS DISABLE
		CH2_I3C_SEL(0);		// CH2 I2C MODE
	}
	else if(i3c_ch == I3C_CH2)
	{
		CH1_I2C_SEL(1);		// CH1 BYPASS DISABLE
		CH1_I3C_SEL(0);		// CH1 I2C MODE
		CH2_I2C_SEL(1);		// CH2 BYPASS DISABLE
		CH2_I3C_SEL(1);		// CH2 I3C MODE
	}
}

/**
 * @brief   I3C RSTDAA (Reset Dynamic Address Assignment) 명령 실행
 * @details 모든 I3C 슬레이브 디바이스의 Dynamic Address를 초기화하는 명령 (RSTDAA, 0x06)을 전송합니다.
 *          이 과정 후, 모든 슬레이브는 다시 ENTDAA(0x7E)를 통해 새로운 Dynamic Address를 할당받아야 합니다.
 * @param   flag_CurrComMod - 현재 통신 모드를 지정
 *            - `true`  : I3C 모드에서 실행
 *            - `false` : I2C 모드에서 실행
 * @return  uint32_t - I3C 에러 코드 (정상 실행 시 0)
 */
uint32_t I3C_RSTDAA(bool flag_CurrComMod)
{
	uint8_t id = 0x7E;
	uint8_t data = 0x06;
	unsigned int w_mode;

	if (flag_CurrComMod == _I2C_MODE_)
		w_mode = I2C_PRIVATE_WITHOUT_ARB_RESTART;
	else
		w_mode = I3C_PRIVATE_WITHOUT_ARB_RESTART;


	//use I3C_IDX_FRAME_1
	/* set trans data  */
	aTxBuffer[0] = data;

	/* set buffer */
	aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
	aContextBuffers[0].CtrlBuf.Size = 1;
	aContextBuffers[0].TxBuf.pBuffer = aTxBuffer;
	aContextBuffers[0].TxBuf.Size = 1;	//addr 2byte data 2byte


	aPrivateDescriptor[0].TargetAddr = id;
	aPrivateDescriptor[0].TxBuf.pBuffer = aTxBuffer;
	aPrivateDescriptor[0].TxBuf.Size =1;
	aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_WRITE;



	HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0],
			&aContextBuffers[0],
			aContextBuffers[0].CtrlBuf.Size,
			w_mode);

	HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]);

	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY)
	{
	}

	i3c_error = hi3c1.ErrorCode;
	return i3c_error;
}

/**
 * @brief  I3C Dynamic Address Assignment (ENTDAA)
 *         - ENUM을 통해 비교 기준을 선택하여 주소 할당
 * @param  dynAddr  할당할 Dynamic Address
 * @param  device   비교할 장치 타입 (I3C_DEVICE_DW9785, I3C_DEVICE_DW97XX 등)
 * @return I3C 에러 코드 반환
 */
uint32_t I3C_ENTDAA(uint8_t dynAddr, I3C_DeviceType device)
{
    uint8_t retryCount = 1;
    uint8_t isMatched = 0;
    uint32_t i3c_error;
    const uint8_t* refPayload = NULL;  // 비교할 기준 페이로드 포인터 (const 적용)

    // ENUM 값에 따라 비교 대상 선택
    switch (device)
    {
        case I3C_DEVICE_DW9785:
            refPayload = DW9785_payload;
            break;
        case I3C_DEVICE_DW97XX:
            refPayload = DW97xx_payload;
            break;
        default:
            return HAL_I3C_ERROR_INVALID_PARAM;  // 잘못된 입력
    }

    dynAddr >>= 1;  // Dynamic Address 변환

    do
    {
		targetPayload = 0;
        i3c_status = HAL_I3C_Ctrl_DynAddrAssign(&hi3c1, &targetPayload, I3C_RSTDAA_THEN_ENTDAA, 100);
        isMatched = isPayloadMatched(refPayload);

        if (i3c_status == HAL_BUSY)
        {
            HAL_I3C_Ctrl_SetDynAddr(&hi3c1, isMatched ? dynAddr : dynAddr + retryCount);
            if (!isMatched) retryCount++;
        }

    } while (i3c_status == HAL_BUSY);

    i3c_error = hi3c1.ErrorCode;
    return i3c_error;
}

/**
 * @brief  페이로드 비교 함수
 *         - 전역 targetPayload(64비트)와 기준(refPayload) 비교
 * @param  refPayload  비교할 기준값 (예: DW9785_payload)
 * @return 일치하면 1, 다르면 0 반환
 */
uint8_t isPayloadMatched(const uint8_t* refPayload)
{
    for (int i = 0; i < 8; i++)
    {
        uint8_t extractedByte = (targetPayload >> (i * 8)) & 0xFF; // targetPayload에서 바이트 추출

        if (extractedByte != refPayload[i])
        {
            return 0; // 불일치
        }
    }
    return 1; // 일치
}

/**
 * @brief  I3C SETDASA (Dynamic Address Assignment)
 *         - Broadcast 및 Direct CCC 명령을 실행하여 동적 주소 할당
 * @param  targetAddr  대상 I3C 장치 정적 주소 (예: LSM6DSO_STA)
 * @param  dynAddr     할당할 Dynamic Address
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t I3C_SETDASA(uint8_t targetAddr, uint8_t dynAddr)
{
    uint8_t diSecData[1] = {0x08};  // DISEC 데이터
    uint8_t setDasaData[1] = {dynAddr << 1};  // Dynamic Address를 Left Shift

    /* Broadcast CCC 프레임 설정 */
    I3C_CCCTypeDef broadcastCCC[] =
    {
        {0, Brodacast_DISEC, {diSecData, 1}, LL_I3C_DIRECTION_WRITE},  // Disable Events (0x01)
        {0, Brodacast_RST, {NULL, 0}, LL_I3C_DIRECTION_WRITE},      // Enter Activity State 0 (0x06)
    };

    /* Direct CCC 프레임 설정 */
	I3C_CCCTypeDef directCCC[] =
	{
		{targetAddr, Brodacast_SETDASA, {setDasaData, 1}, LL_I3C_DIRECTION_WRITE}  // SETDASA (0x87)
	};

	/* Context Buffer 설정 */
	aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
	aContextBuffers[0].CtrlBuf.Size    = COUNTOF(aControlBuffer);
	aContextBuffers[0].TxBuf.pBuffer   = aTxBuffer;
	aContextBuffers[0].TxBuf.Size      = 1;

	/* Broadcast CCC 전송 */
	if (HAL_I3C_AddDescToFrame(&hi3c1,
							   broadcastCCC,
							   NULL,
							   &aContextBuffers[0],
							   COUNTOF(broadcastCCC),
							   I3C_BROADCAST_WITHOUT_DEFBYTE_RESTART) != HAL_OK)
	{
		return HAL_I3C_ERROR_INVALID_PARAM;
	}

	if (HAL_I3C_Ctrl_TransmitCCC_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
	{
		return HAL_I3C_ERROR_NOT_ALLOWED;
	}

	while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY);

	/* SETDASA CCC 전송 */
	aContextBuffers[1].CtrlBuf.pBuffer = aControlBuffer;
	aContextBuffers[1].CtrlBuf.Size    = COUNTOF(aControlBuffer);
	aContextBuffers[1].TxBuf.pBuffer   = aTxBuffer;
	aContextBuffers[1].TxBuf.Size      = 1;

	if (HAL_I3C_AddDescToFrame(&hi3c1,
							   directCCC,
							   NULL,
							   &aContextBuffers[1],
							   COUNTOF(directCCC),
							   I3C_DIRECT_WITHOUT_DEFBYTE_STOP) != HAL_OK)
	{
		return HAL_I3C_ERROR_INVALID_PARAM;
	}

	if (HAL_I3C_Ctrl_TransmitCCC_IT(&hi3c1, &aContextBuffers[1]) != HAL_OK)
	{
		return HAL_I3C_ERROR_NOT_ALLOWED;
	}

	return HAL_I3C_ERROR_NONE; // 정상 수행
}

/**
 * @brief  I3C Private Read (8비트 주소 체계)
 *         - 지정된 I3C 장치에서 특정 레지스터 주소(addr)부터 size만큼 데이터를 읽음
 * @param  id    대상 I3C 장치의 8비트 주소 (7비트 ID 변환됨)
 * @param  addr  읽을 레지스터의 8비트 주소
 * @param  dat   읽은 데이터를 저장할 버퍼 (포인터)
 * @param  size  읽을 데이터의 크기 (바이트 단위)
 * @return I3C 에러 코드 (성공 시 HAL_I3C_ERROR_NONE)
 *
 * @note   1. 먼저 8비트 주소(addr)를 전송하여 읽기 대상 레지스터를 지정함
 *         2. 이후 해당 장치에서 size만큼 데이터를 읽어 dat 버퍼에 저장함
 *         3. 비동기 전송을 사용하며, 완료될 때까지 HAL_I3C_STATE_READY 상태를 확인함
 */
uint32_t I3C_Read(uint8_t id, uint8_t addr, uint8_t* dat, uint16_t size)
{
    uint8_t deviceAddr = id >> 1;  // ID 7-bit 변환

    /* 유효성 검사 */
    if (size == 0 || dat == NULL)
        return HAL_I3C_ERROR_INVALID_PARAM;

    /* Write 설정 (8bit 주소 전송) */
    aTxBuffer[0] = addr;  // 8bit 주소만 전송

    aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
    aContextBuffers[0].CtrlBuf.Size = 1;
    aContextBuffers[0].TxBuf.pBuffer = aTxBuffer;
    aContextBuffers[0].TxBuf.Size = 1;  // 8bit addr 전송

    aPrivateDescriptor[0].TargetAddr = deviceAddr;
    aPrivateDescriptor[0].TxBuf.pBuffer = aTxBuffer;
    aPrivateDescriptor[0].TxBuf.Size = 1;
    aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_WRITE;

    if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0],
                               &aContextBuffers[0], aContextBuffers[0].CtrlBuf.Size,
                               I3C_PRIVATE_WITHOUT_ARB_RESTART) != HAL_OK)
        return HAL_I3C_ERROR_INVALID_PARAM;

    if (HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
        return HAL_I3C_ERROR_NOT_ALLOWED;

    while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY);

    /* Read 설정 (`size` 만큼 데이터 수신) */
    aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
    aContextBuffers[0].CtrlBuf.Size = 1;
    aContextBuffers[0].RxBuf.pBuffer = dat;
    aContextBuffers[0].RxBuf.Size = size;  // size 바이트 읽기

    aPrivateDescriptor[0].TargetAddr = deviceAddr;
    aPrivateDescriptor[0].RxBuf.pBuffer = dat;
    aPrivateDescriptor[0].RxBuf.Size = size;
    aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_READ;

    if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0],
                               &aContextBuffers[0], aContextBuffers[0].CtrlBuf.Size,
                               I3C_PRIVATE_WITHOUT_ARB_STOP) != HAL_OK)
        return HAL_I3C_ERROR_INVALID_PARAM;

    if (HAL_I3C_Ctrl_Receive_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
        return HAL_I3C_ERROR_NOT_ALLOWED;

    while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY);

    return HAL_I3C_ERROR_NONE;  // 성공 시 에러 없음
}

/**
 * @brief  I3C Private Read (16비트 주소 체계)
 *         - 특정 ID의 I3C 장치에서 지정된 크기(size)만큼 16비트 데이터를 읽음
 * @param  id    대상 I3C 장치 주소
 * @param  addr  16비트 레지스터 주소
 * @param  dat   읽은 데이터를 저장할 버퍼 포인터 (`uint16_t` 배열)
 * @param  size  읽을 데이터 크기 (16비트 단위, size=3이면 6바이트 읽음)
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t I3C_Read_16BIT(uint8_t id, uint16_t addr, uint16_t* dat, uint16_t size)
{
    uint8_t deviceAddr = id >> 1;  // ID 7-bit 변환
    uint16_t byteSize = size * 2;  // 16비트 단위이므로 실제 읽을 바이트 크기

    /* 유효성 검사 */
    if (size == 0 || dat == NULL)
        return HAL_I3C_ERROR_INVALID_PARAM;

    /* 16비트 레지스터 주소 설정 (Big Endian) */
    aTxBuffer[0] = (uint8_t)(addr >> 8);  // 상위 바이트
    aTxBuffer[1] = (uint8_t)(addr & 0xFF);  // 하위 바이트

    /* Write 설정 (16비트 주소 전송, 2BYTE) */
    I3C_SetContext(&aContextBuffers[0], &aPrivateDescriptor[0], aTxBuffer, NULL, 2, deviceAddr, HAL_I3C_DIRECTION_WRITE);

    if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0], &aContextBuffers[0], 1, I3C_PRIVATE_WITHOUT_ARB_RESTART) != HAL_OK)
        return HAL_I3C_ERROR_INVALID_PARAM;

    if (HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
        return HAL_I3C_ERROR_NOT_ALLOWED;

    while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY);

    /* Read 설정 (size만큼 16비트 데이터 읽기) */
    I3C_SetContext(&aContextBuffers[0], &aPrivateDescriptor[0], NULL, (uint8_t*)dat, byteSize, deviceAddr, HAL_I3C_DIRECTION_READ);

    if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0], &aContextBuffers[0], 1, I3C_PRIVATE_WITHOUT_ARB_STOP) != HAL_OK)
        return HAL_I3C_ERROR_INVALID_PARAM;

    if (HAL_I3C_Ctrl_Receive_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
        return HAL_I3C_ERROR_NOT_ALLOWED;

    while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY);

    return HAL_I3C_ERROR_NONE;  // 성공 시 에러 없음
}

/**
 * @brief  I3C Private Write (데이터 전송)
 *         - 지정된 I3C 장치로 특정 레지스터 주소(addr)부터 size만큼 데이터를 전송
 * @param  id    대상 I3C 장치의 8비트 주소 (7비트 ID 변환됨)
 * @param  addr  데이터를 전송할 레지스터의 8비트 주소
 * @param  data  전송할 데이터 버퍼 (포인터)
 * @param  size  전송할 데이터 크기 (바이트 단위)
 * @return I3C 에러 코드 (성공 시 HAL_I3C_ERROR_NONE)
 *
 * @note   1. 8비트 주소(addr)를 먼저 전송하여 대상 레지스터를 지정
 *         2. 이후 데이터(data)를 연속 전송
 *         3. 비동기 전송을 사용하며, 완료될 때까지 HAL_I3C_STATE_READY 상태를 확인
 */
uint32_t I3C_Write(uint8_t id, uint8_t addr, uint8_t* data, uint16_t size)
{
    uint8_t deviceAddr = id >> 1;  // ID 7-bit 변환

    /* 유효성 검사 */
    if (size == 0 || data == NULL)
        return HAL_I3C_ERROR_INVALID_PARAM;

	//use I3C_IDX_FRAME_1
    /* addr(1바이트) + data(1바이트) 저장 버퍼 준비 */
    uint8_t aTxBuffer[1 + size];
    aTxBuffer[0] = addr;             // 8bit 주소 저장
    memcpy(&aTxBuffer[1], data, size); // 이후 데이터 복사

    /* Context & Descriptor 설정 */
    aContextBuffers[0].CtrlBuf.pBuffer = aControlBuffer;
    aContextBuffers[0].CtrlBuf.Size = 1;
    aContextBuffers[0].TxBuf.pBuffer = aTxBuffer;
    aContextBuffers[0].TxBuf.Size = 1 + size;  // addr(1B) + data(size)

    aPrivateDescriptor[0].TargetAddr = deviceAddr;
    aPrivateDescriptor[0].TxBuf.pBuffer = aTxBuffer;
    aPrivateDescriptor[0].TxBuf.Size = 1 + size;
    aPrivateDescriptor[0].Direction = HAL_I3C_DIRECTION_WRITE;

    /* I3C 전송 설정 및 실행 */
    if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0],
                               &aContextBuffers[0], aContextBuffers[0].CtrlBuf.Size,
                               I3C_PRIVATE_WITHOUT_ARB_RESTART) != HAL_OK)
        return HAL_I3C_ERROR_INVALID_PARAM;

    if (HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
        return HAL_I3C_ERROR_NOT_ALLOWED;

    while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY) {}

    return hi3c1.ErrorCode;  // 에러 코드 반환
}

/**
 * @brief  I3C Private Write (16비트 주소 + 16비트 데이터 전송)
 *         - 특정 ID의 I3C 장치에서 16비트 주소(addr)로 지정된 크기(size)만큼 16비트 데이터를 전송
 * @param  id    대상 I3C 장치 주소 (7-bit 변환됨)
 * @param  addr  16비트 레지스터 주소
 * @param  data  전송할 16비트 데이터 버퍼 포인터
 * @param  size  전송할 데이터 크기 (16비트 단위, size * 2BYTE 전송됨)
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t i3c_write_16BIT(uint8_t id, uint16_t addr, uint16_t* data, uint16_t size)
{
    uint8_t deviceAddr = id >> 1;  // ID 7-bit 변환

    if (size == 0 || data == NULL)
        return HAL_I3C_ERROR_INVALID_PARAM;

    aTxBuffer[0] = (uint8_t)(addr >> 8);
    aTxBuffer[1] = (uint8_t)(addr & 0xFF);

    for (uint16_t i = 0; i < size; i++)
    {
        aTxBuffer[2 + (i * 2)] = (uint8_t)(data[i] >> 8);
        aTxBuffer[3 + (i * 2)] = (uint8_t)(data[i] & 0xFF);
    }

    I3C_SetContext(&aContextBuffers[0], &aPrivateDescriptor[0], aTxBuffer, NULL, size * 2, deviceAddr, HAL_I3C_DIRECTION_WRITE);

    if (HAL_I3C_AddDescToFrame(&hi3c1, NULL, &aPrivateDescriptor[0], &aContextBuffers[0], 1, I3C_PRIVATE_WITHOUT_ARB_RESTART) != HAL_OK)
        return HAL_I3C_ERROR_INVALID_PARAM;

    if (HAL_I3C_Ctrl_Transmit_IT(&hi3c1, &aContextBuffers[0]) != HAL_OK)
        return HAL_I3C_ERROR_NOT_ALLOWED;

    while (HAL_I3C_GetState(&hi3c1) != HAL_I3C_STATE_READY);

    return hi3c1.ErrorCode;
}

/**
 * @brief  I3C Context 및 Descriptor 설정 함수
 *         - Write/Read 설정을 간소화
 * @param  context   설정할 Context Buffer 포인터
 * @param  desc      설정할 Private Descriptor 포인터
 * @param  txBuf     전송할 데이터 버퍼 (Write 시)
 * @param  rxBuf     수신할 데이터 버퍼 (Read 시)
 * @param  size      전송/수신할 데이터 크기
 * @param  addr      대상 I3C 장치 주소 (7-bit)
 * @param  dir       I3C 데이터 방향 (READ/WRITE)
 */
void I3C_SetContext(I3C_XferTypeDef* context, I3C_PrivateTypeDef* desc, uint8_t* txBuf, uint8_t* rxBuf, uint16_t size, uint8_t addr, uint8_t dir)
{
    context->CtrlBuf.pBuffer = aControlBuffer;
    context->CtrlBuf.Size = 1;
    context->TxBuf.pBuffer = txBuf;
    context->TxBuf.Size = (txBuf) ? size : 0;  // Write인 경우만 설정
    context->RxBuf.pBuffer = rxBuf;
    context->RxBuf.Size = (rxBuf) ? size : 0;  // Read인 경우만 설정

    desc->TargetAddr = addr;
    desc->TxBuf.pBuffer = txBuf;
    desc->TxBuf.Size = (txBuf) ? size : 0;
    desc->RxBuf.pBuffer = rxBuf;
    desc->RxBuf.Size = (rxBuf) ? size : 0;
    desc->Direction = dir;
}

/**
 * @brief  I3C 또는 I2C 장치 스캔 함수
 *         - 현재 통신 모드(`flag_CurrComMod`)에 따라 I2C 또는 I3C 장치를 검색
 * @param  flag_CurrComMod  현재 통신 모드 (true: I2C 모드, false: I3C 모드)
 * @return 감지된 장치 개수 (uint8_t)
 */
uint8_t I3C_Scan_Target(bool flag_CurrComMod)
{
    uint8_t cnt = 0;

    // 함수 포인터 설정 (I2C 또는 I3C에 따라 자동 선택)
    HAL_StatusTypeDef (*DeviceReadyFunc)(I3C_HandleTypeDef*, uint8_t, uint32_t, uint32_t) = NULL;

    if (flag_CurrComMod == _I2C_MODE_)
        DeviceReadyFunc = HAL_I3C_Ctrl_IsDeviceI2C_Ready;
    else if (flag_CurrComMod == _I3C_MODE_)
        DeviceReadyFunc = HAL_I3C_Ctrl_IsDeviceI3C_Ready;
    else
        return 0;  // 정의되지 않은 모드면 스캔하지 않음

    // I2C/I3C 장치 검색 실행
    for (uint8_t i = 0; i < 0x7F; i++)
    {
        if (NOT_USE_I3C_ADDR(i))  // 제외할 주소는 스킵
            continue;

        if (DeviceReadyFunc(&hi3c1, i, 1, 1) == HAL_OK)
        {
            cnt++;  // 스캔된 장치 개수 증가
        }
    }
    return cnt;
}
