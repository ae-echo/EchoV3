/*
 * flash.c
 *
 *  Created on: Feb 3, 2025
 *      Author: JungBongJun
 */

#include "flash.h"
#include "fmc.h"

uint32_t Flash_GetSector(uint32_t Address)
{
	  uint32_t sector = 0;

	  if ((Address >= FLASH_BASE) && (Address < FLASH_BASE + FLASH_BANK_SIZE))
	  {
	    sector = (Address & ~FLASH_BASE) / FLASH_SECTOR_SIZE;
	  }
	  else if ((Address >= FLASH_BASE + FLASH_BANK_SIZE) && (Address < FLASH_BASE + FLASH_SIZE))
	  {
	    sector = ((Address & ~FLASH_BASE) - FLASH_BANK_SIZE) / FLASH_SECTOR_SIZE;
	  }
	  else
	  {
	    sector = 0xFFFFFFFF; /* Address out of range */
	  }

	  return sector;
}

HAL_StatusTypeDef WriteFlash()
{
    /* Unlock to control */
    HAL_FLASH_Unlock();

    uint32_t Address = FLASH_USER_START_ADDR;

    while(Address < FLASH_USER_END_ADDR)
    {
        /* Writing data to flash memory */
    	//H5는 Quad 기준이므로 4-> 16으로 변경됨
        if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, Address, DATA_32) == HAL_OK)
        {
          Address = Address + 16;
        }
        else
        {
          uint32_t errorcode = HAL_FLASH_GetError();
          return HAL_ERROR;
        }
    }

    /* Lock flash control register */
    HAL_FLASH_Lock();

    return HAL_OK;
}

HAL_StatusTypeDef EraseFlash()
{

    uint32_t SectorError = 0;

    /* Unlock to control */
    HAL_FLASH_Unlock();

    /* Calculate sector index */
    uint32_t UserSector = Flash_GetSector(FLASH_USER_START_ADDR);
    uint32_t NbOfSectors = Flash_GetSector(FLASH_USER_END_ADDR) - UserSector + 1;

    /* Erase sectors */
    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Sector = UserSector;
    EraseInitStruct.NbSectors = NbOfSectors;

    if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
		uint32_t errorcode = HAL_FLASH_GetError();
		return HAL_ERROR;
    }

    //25-02-04 캐쉬관련한 매크로함수 미존재 직접 구현?
    /* Clear cache for flash */
//    __HAL_FLASH_DATA_CACHE_DISABLE();
//    __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
//
//    __HAL_FLASH_DATA_CACHE_RESET();
//    __HAL_FLASH_INSTRUCTION_CACHE_RESET();
//
//    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
//    __HAL_FLASH_DATA_CACHE_ENABLE();

    /* Lock flash control register */
    HAL_FLASH_Lock();

    return HAL_OK;
}

HAL_StatusTypeDef ReadFlash()
{
    __IO uint32_t data32 = 0 , MemoryProgramStatus = 0;
    uint32_t Address = FLASH_USER_START_ADDR;

    while(Address < FLASH_USER_END_ADDR)
    {
        data32 = *(__IO uint32_t*)Address;

        if(data32 != DATA_32)
          MemoryProgramStatus++;

        Address = Address + 4;
    }

    if(MemoryProgramStatus > 0)
        return HAL_ERROR;

    return HAL_OK;

}

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

uint32_t Flash_Write(void)
{
    uint32_t tBuf[4] = {0xAABBCCDD, 0xEEFF1122, 0x33445566, 0x77889900}; // 128-bit(16바이트) 데이터

    HAL_FLASH_Unlock();

    //FLASH_FLAG_OPERR, FLASH_FLAG_PGAERR
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_INCERR |
                            FLASH_FLAG_WRPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PGSERR );


    //FLASH_Erase_Sector(FLASH_SECTOR_1, FLASH_BANK_1);
    // FLASH 영역 지우기
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks = FLASH_BANK_1;
    EraseInitStruct.Sector = FLASH_SECTOR_1; // 섹터 1 삭제
    EraseInitStruct.NbSectors = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        return HAL_FLASH_GetError();
    }

    // FLASH 데이터 쓰기 (128-bit 단위, 16바이트 정렬 필요)
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, FLASH_USER_START_ADDR, (uint32_t)&tBuf) != HAL_OK)
    {
        return HAL_FLASH_GetError();
    }

    HAL_FLASH_Lock();

    return 0;
}

uint32_t* Flash_Read(void)
{
    static uint32_t buffer[4];
    uint32_t Address = FLASH_USER_START_ADDR;

    // 128-bit (16바이트) 단위 읽기
    FLASH_READ_128BIT(Address, buffer);

    return buffer;
}

/* Flash Set Config ------------- */
uint32_t Flash_SetConfig(uint32_t Size, uint8_t* pBuf)
{
    if(HAL_FLASH_Unlock() != HAL_OK)		return FLASH_FAIL;
    //HAL_FLASH_Unlock();

    //FLASH_FLAG_OPERR, FLASH_FLAG_PGAERR
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_INCERR |
                            FLASH_FLAG_WRPERR | FLASH_FLAG_PGSERR | FLASH_FLAG_PGSERR );

    //FLASH_Erase_Sector(FLASH_SECTOR_1, FLASH_BANK_1);
    // FLASH 영역 지우기
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks = FLASH_BANK_1;
    EraseInitStruct.Sector = FLASH_SECTOR_1; // 섹터 1 삭제
    EraseInitStruct.NbSectors = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        return HAL_FLASH_GetError();
    }

    uint32_t Address = FLASH_USER_START_ADDR;
    uint32_t DataBuffer[4];  // 128-bit(16바이트) 데이터를 저장할 버퍼

    for (uint32_t i = 0; i < Size; i += 16)  // 16바이트 단위로 프로그래밍
    {
        memset(DataBuffer, 0xFF, sizeof(DataBuffer));  // 빈 공간을 0xFF로 패딩

        // 남은 데이터가 16바이트 이하인 경우 패딩 적용
        uint32_t remaining = Size - i;
        memcpy(DataBuffer, &pBuf[i], (remaining >= 16) ? 16 : remaining);

        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, Address, (uint32_t)DataBuffer) != HAL_OK)
        {
            return HAL_FLASH_GetError();
        }

        Address += 16;  // 16바이트 정렬 유지
    }

    HAL_FLASH_Lock();

    return FLASH_OK;
}

uint32_t Flash_GetConfig(uint32_t Size, uint8_t* pBuf)
{
    uint32_t DataBuffer[4];
    uint32_t i, j;
    uint32_t Address = FLASH_USER_START_ADDR;

    for (i = 0; i < Size; i += 16)
    {
        FLASH_READ_128BIT(Address, DataBuffer);  // 128-bit(16바이트) 단위로 Flash 데이터 읽기

        for (j = 0; j < 16 && (i + j) < Size; j++)
        {
            pBuf[i + j] = FLASH_EXTRACT_BYTE(DataBuffer, j);  // 읽은 데이터에서 1바이트씩 추출
        }

        Address += 16;  // 16바이트 정렬 유지
    }

    return FLASH_OK;
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

uint8_t Flash_Write_Fw(uint32_t Size)
{
    uint8_t DataBuffer[16];  // 128비트(16바이트) 프로그래밍용 버퍼
    uint32_t Addr = FLASH_USER_FW_ADDR_0;
    uint32_t i, j;

    // 플래시 잠금 해제
    if (HAL_FLASH_Unlock() != HAL_OK)
    {
        return FLASH_FAIL;
    }

    // 플래시 플래그 초기화
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR |
                            FLASH_FLAG_PGSERR | FLASH_FLAG_INCERR);

    // 쓰기 보호 해제
    DisableWriteProtect();

    // 플래시 섹터 삭제
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t SectorError;

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.Banks = FLASH_BANK_1;         // Bank 설정
    EraseInitStruct.Sector = FLASH_SECTOR_5;      // 섹터 5부터 시작
    EraseInitStruct.NbSectors = 2;                // 섹터 갯수만큼

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return FLASH_FAIL;
    }

    // 데이터 쓰기
    for (i = 0; i < Size; i += 16)  // 16바이트(128비트) 단위로 반복
    {
        // 16바이트 버퍼 초기화
        for (j = 0; j < 16 && (i + j) < Size; j++)
        {
            FMC_Read(FMC_SDRAM_BLOCK_0 + i + j, 1, &DataBuffer[j]);
        }

        // 프로그래밍 수행
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, Addr + i, (uint32_t *)DataBuffer) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return FLASH_FAIL;
        }
    }

    // 쓰기 보호 재활성화
    EnableWriteProtect();

    // 플래시 잠금
    HAL_FLASH_Lock();

    return FLASH_OK;
}


__attribute__((naked, noreturn)) void BootJumpASM(uint32_t SP, uint32_t RH)
{
    __asm("MSR      MSP,r0");
    __asm("BX       r1");
}

void Flash_JumpVectorTable(void)
{
    uint32_t AppStack;
    uint32_t AppEntry;

    HAL_RCC_DeInit();

    /* Disable all enabled interrupts and pending interrupt requests in NVIC */
    for (uint8_t i = 0; i < 8; i++)
    {
        /* Disable all enabled interrupts in NVIC */
        NVIC->ICER[i] = 0xFFFFFFFF;
        /* Clear all pending interrupt requests in NVIC */
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /*
     * Disable SysTick and clear its exception pending bit,
     * if it is used in the bootloader, e. g. by the RTX.
     */
    SysTick->CTRL = 0;
    SCB->ICSR |= SCB_ICSR_PENDSTCLR_Msk;

    /* Disable individual fault handlers if the bootloader used them. */
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk |
                    SCB_SHCSR_BUSFAULTENA_Msk |
                    SCB_SHCSR_MEMFAULTENA_Msk);

    /* Activate the MSP, if the core is found to currently run with the PSP. */
    if (CONTROL_SPSEL_Msk & __get_CONTROL())
    {
        /* MSP is not active */
        __set_MSP(__get_PSP());
        __set_CONTROL(__get_CONTROL() & ~CONTROL_SPSEL_Msk);
    }

    // __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Get the application stack pointer (1st entry in the app vector table) */
    AppStack = (uint32_t) * ((__IO uint32_t *)FLASH_USER_START_ADDR);

    /* Get the application entry point (2nd entry in the app vector table */
    AppEntry = (uint32_t) * (__IO uint32_t *)(FLASH_USER_START_ADDR + 4);

    /*
     * The final part is to set the MSP to the value found in the user application vector table
     * and then load the PC with the reset vector value of the user application. This can't be
     * done in C, as it is always possible, that the compiler uses the current SP. But that
     * would be gone after setting the new MSP. So, a call to a small assembler function is done.
     */
    BootJumpASM(AppStack, AppEntry);
}

