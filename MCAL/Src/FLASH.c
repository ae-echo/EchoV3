#include <MCAL/Inc/FLASH.h>

__attribute__((naked, noreturn)) void BootJumpASM(uint32_t SP, uint32_t RH);

/**
 * @brief  주어진 Flash 메모리 주소(Address)가 속한 섹터 번호를 반환
 * @param  Address  Flash 메모리 내 확인할 주소
 * @retval 해당 주소가 속한 섹터 번호 (0~23)
 *         주소가 유효하지 않으면 0xFFFFFFFF 반환
 */
uint32_t Flash_GetSector(uint32_t Address)
{
	  uint32_t sector = 0;

	  // Bank 1 영역 확인
	  if ((Address >= FLASH_BASE) && (Address < FLASH_BASE + FLASH_BANK_SIZE))
	  {
	    sector = (Address & ~FLASH_BASE) / FLASH_SECTOR_SIZE;
	  }
	  // Bank 2 영역 확인
	  else if ((Address >= FLASH_BASE + FLASH_BANK_SIZE) && (Address < FLASH_BASE + FLASH_SIZE))
	  {
	    sector = ((Address & ~FLASH_BASE) - FLASH_BANK_SIZE) / FLASH_SECTOR_SIZE;
	  }
	  // 주소가 Flash 메모리 범위를 벗어난 경우
	  else
	  {
	    sector = 0xFFFFFFFF; /* Address out of range */
	  }

	  return sector;
}

/**
 * @brief  Gets the bank of a given address
 * @param  Addr: Address of the FLASH Memory
 * @retval The bank of a given address
 */
uint32_t Flash_GetBank(uint32_t Addr)
{
  uint32_t bank = 0;

  if (READ_BIT(FLASH->OPTSR_CUR, FLASH_OPTSR_SWAP_BANK) == 0)
  {
    /* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
    /* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}

/**
 * @brief  주어진 펌웨어 데이터를 Flash 메모리에 기록하는 함수
 * @param  fwData        Flash에 기록할 펌웨어 데이터 버퍼 (uint8_t 배열)
 * @param  flashAddress  Flash 메모리에 기록을 시작할 주소
 * @param  Size          기록할 데이터 크기 (바이트 단위)
 * @retval FLASH_OK(0) 또는 FLASH_FAIL(1) (기록 성공/실패 여부)
 */
int Flash_Write_Fw(uint8_t* fwData, uint32_t flashAddress, uint32_t size)
{
    uint8_t DataBuffer[16]; // 16바이트(128비트) 버퍼
    uint32_t i, j;

    // Flash Unlock
    if (HAL_FLASH_Unlock() != HAL_OK)
        return FLASH_FAIL;

    // Flash 상태 플래그 초기화 (오류 플래그 제거)
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_INCERR);

    // 16바이트(128비트) 단위로 데이터를 Flash에 기록
    for (i = 0; i < size; i += 16)
    {
        // 16바이트 버퍼 초기화
        for (j = 0; j < 16 && (i + j) < size; j++)
        {
            DataBuffer[j] = fwData[i + j];
        }

        // 남은 공간은 0xFF로 패딩
        while (j < 16)
        {
            DataBuffer[j++] = 0xFF;
        }

        // Flash에 데이터 기록 (128비트 단위)
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, flashAddress + i, (uint32_t *)DataBuffer) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return FLASH_FAIL; // 기록 실패 시 종료
        }
    }

    return FLASH_OK;
}

/**
 * @brief  지정된 Flash 주소에서 애플리케이션을 실행하는 함수 (부트 점프)
 * @param  startAddress  애플리케이션의 벡터 테이블 시작 주소
 * @note   현재 실행 중인 펌웨어에서 지정된 애플리케이션 영역으로 점프
 */
void Flash_JumpVectorTable(uint32_t startAddress)
{
    uint32_t AppStack;
    uint32_t AppEntry;

    // 모든 클럭 설정 초기화 (시스템 리셋 없이 주변 장치 클럭 설정 해제)
    HAL_RCC_DeInit();

    /* NVIC의 모든 활성화된 인터럽트 및 대기 중인 인터럽트 비활성화 */
    for (uint8_t i = 0; i < 8; i++)
    {
        /* Disable all enabled interrupts in NVIC */
        NVIC->ICER[i] = 0xFFFFFFFF;
        /* Clear all pending interrupt requests in NVIC */
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /*
	* SysTick 비활성화 및 예외 플래그 클리어
	* 부트로더에서 RTX 같은 OS 사용 시 필요할 수 있음.
	*/
    SysTick->CTRL = 0;
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

    /* 부트로더에서 활성화된 개별 오류 핸들러 비활성화 */
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk |
                    SCB_SHCSR_BUSFAULTENA_Msk |
                    SCB_SHCSR_MEMFAULTENA_Msk);

    /* 현재 PSP(Stack Pointer) 사용 중이면 MSP(Main Stack Pointer)로 변경 */
    if (CONTROL_SPSEL_Msk & __get_CONTROL())
    {
    	/* MSP가 활성 상태가 아님 */
        __set_MSP(__get_PSP());
        __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);
    }

    // __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* 애플리케이션의 스택 포인터 주소 (벡터 테이블 첫 번째 항목) */
    AppStack = (uint32_t) * ((__IO uint32_t *)startAddress);

    /* 애플리케이션 진입점 주소 (벡터 테이블 두 번째 항목) */
    AppEntry = (uint32_t) * (__IO uint32_t *)(startAddress + 4);

    /*
     * MSP 값을 애플리케이션의 벡터 테이블에서 가져온 값으로 설정하고
     * PC(Program Counter)를 애플리케이션의 Reset Handler로 설정
     * 이를 위해 어셈블리 함수 `BootJumpASM`을 호출하여 변경.
     */
    BootJumpASM(AppStack, AppEntry);
}

/**
 * @brief  지정된 Flash 영역을 삭제하는 함수
 * @param  areaAddress  삭제할 영역의 시작 주소
 * @param  dataSize     삭제할 데이터 크기 (바이트 단위)
 * @note   지정된 크기에 해당하는 섹터 단위로 삭제 수행
 */
void Flash_Erase(uint32_t areaAddress, uint32_t dataSize)
{

	  FLASH_EraseInitTypeDef EraseInitStruct;
	  uint32_t SectorError;

	  /* Fill EraseInit structure*/
	  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;		// 섹터 단위 삭제
	  EraseInitStruct.Banks = Flash_GetBank(areaAddress);		// 대상 Flash Bank 설정
	  EraseInitStruct.Sector = Flash_GetSector(areaAddress);	// 시작 섹터 번호 설정
	  EraseInitStruct.NbSectors = Flash_GetSector(areaAddress + dataSize) - Flash_GetSector(areaAddress) + 1;	// 삭제할 섹터 개수 계산

	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	  {
	  }
}

/*
 *  EnableWriteProtect, DisableWriteProtect 검증필요
 *  Flash_Get_CheckSum 이 함수방식이 맞는지 의문
 */

HAL_StatusTypeDef EnableWriteProtect()
{
    FLASH_OBProgramInitTypeDef OBInit;

    HAL_FLASH_OB_Unlock();
    //HAL_FLASH_Unlock();

    OBInit.OptionType = OPTIONBYTE_WRP;
    OBInit.WRPState   = OB_WRPSTATE_ENABLE;
    OBInit.Banks      = FLASH_BANK_1;
    OBInit.WRPSector  = FLASH_WRP_SECTORS;
    HAL_FLASHEx_OBProgram(&OBInit);

    if (HAL_FLASH_OB_Launch() != HAL_OK)
    {
    return HAL_ERROR;
    }

    HAL_FLASH_OB_Lock();
    //HAL_FLASH_Lock();

    return HAL_OK;
}

HAL_StatusTypeDef DisableWriteProtect()
{
    FLASH_OBProgramInitTypeDef OBInit;

    HAL_FLASH_OB_Unlock();
    //HAL_FLASH_Unlock();

    OBInit.OptionType = OPTIONBYTE_WRP;
    OBInit.WRPState   = OB_WRPSTATE_DISABLE;
    OBInit.Banks      = FLASH_BANK_1;
    OBInit.WRPSector  = FLASH_WRP_SECTORS;
    HAL_FLASHEx_OBProgram(&OBInit);

    if (HAL_FLASH_OB_Launch() != HAL_OK)
    {
    return HAL_ERROR;
    }

    HAL_FLASH_OB_Lock();

    //HAL_FLASH_Lock();

    return HAL_OK;
}

uint8_t Flash_Get_CheckSum(void)
{
    uint32_t DataBuffer[4];
    uint32_t i, j;
    uint8_t sum = 0;
    uint32_t Address = FLASH_USER_FW_ADDR_0;

    for (i = 0; i < FLASH_FW_CHECKSUM_LEN; i += 16) // ✅ 16바이트 단위로 읽기
    {
        FLASH_READ_128BIT(Address, DataBuffer); // ✅ 128-bit(16바이트) 단위로 읽기

        for (j = 0; j < 16 && (i + j) < FLASH_FW_CHECKSUM_LEN; j++)
        {
            sum += FLASH_EXTRACT_BYTE(DataBuffer, j); // ✅ 바이트 추출 후 합산
        }

        Address += 16;  // ✅ 16바이트 정렬 유지
    }

    PRINTF("CheckSum : %X \r\n", sum);

    return sum;
}


__attribute__((naked, noreturn)) void BootJumpASM(uint32_t SP, uint32_t RH)
{
    __asm("MSR      MSP,r0");
    __asm("BX       r1");
}




