#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include <MCAL/Inc/MCAL_Common.h>

/**
 * @brief  128-bit (16바이트) 데이터를 Flash에서 읽어 DataBuffer에 저장하는 매크로
 * @param  ADDR  Flash에서 읽을 시작 주소 (16바이트 정렬 필수)
 * @param  BUF   읽은 데이터를 저장할 uint32_t[4] 배열
 */
#define FLASH_READ_128BIT(ADDR, BUF)      \
    do {                                  \
        (BUF)[0] = *(__IO uint32_t*)(ADDR);   \
        (BUF)[1] = *(__IO uint32_t*)(ADDR + 4);   \
        (BUF)[2] = *(__IO uint32_t*)(ADDR + 8);   \
        (BUF)[3] = *(__IO uint32_t*)(ADDR + 12);  \
    } while(0)

/**
 * @brief  128-bit(16바이트) 데이터에서 특정 바이트 추출
 * @param  BUF   Flash에서 읽은 128-bit 데이터를 저장한 uint32_t[4] 배열
 * @param  IDX   0~15 범위의 바이트 인덱스
 * @return 추출된 1바이트(uint8_t)
 */
#define FLASH_EXTRACT_BYTE(BUF, IDX)  \
    (uint8_t)(((BUF)[(IDX) / 4] >> (8 * ((IDX) % 4))) & 0xFF)


/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0 ((uint32_t)0x08000000)  /* Base @ of Sector 0, 16 Kbytes Bootloader 영역*/
#define ADDR_FLASH_SECTOR_1 ((uint32_t)0x08004000)  /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2 ((uint32_t)0x08008000)  /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3 ((uint32_t)0x0800C000)  /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4 ((uint32_t)0x08010000)  /* Base @ of Sector 4, 64 Kbytes Setting 영역*/
#define ADDR_FLASH_SECTOR_5 ((uint32_t)0x08020000)  /* Base @ of Sector 5, 128 Kbytes Firmware 영역*/
#define ADDR_FLASH_SECTOR_6 ((uint32_t)0x08040000)  /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7 ((uint32_t)0x08060000)  /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8 ((uint32_t)0x08080000)  /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9 ((uint32_t)0x080A0000)  /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10 ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11 ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes Boot Loader Start Address*/

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_12 ((uint32_t)0x08100000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_13 ((uint32_t)0x08104000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_14 ((uint32_t)0x08108000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_15 ((uint32_t)0x0810C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_16 ((uint32_t)0x08110000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_17 ((uint32_t)0x08120000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18 ((uint32_t)0x08140000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19 ((uint32_t)0x08160000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20 ((uint32_t)0x08180000) /* Base @ of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_21 ((uint32_t)0x081A0000) /* Base @ of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_22 ((uint32_t)0x081C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23 ((uint32_t)0x081E0000) /* Base @ of Sector 11, 128 Kbytes */

#define FLASH_WRP_SECTORS		(OB_WRP_SECTOR_4TO7 | OB_WRP_SECTOR_8TO11)

#define DATA_32                 ((uint32_t)0x87654321)

#define FLASH_FW_CHECKSUM_LEN			64

#define FLASH_USER_START_ADDR ADDR_FLASH_SECTOR_5                         	/* Start @ of user Flash area */
#define FLASH_USER_END_ADDR (ADDR_FLASH_SECTOR_23 + FLASH_SECTOR_SIZE - 1) 	/* End @ of user Flash area */
//#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_0   							/* Start @ of user Flash area */
//#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_0  +  Flash_GetSectorSize(ADDR_FLASH_SECTOR_0) -1 /* End @ of user Flash area : sector start address + sector size -1 */
#define FLASH_SETTING_ADDR 		ADDR_FLASH_SECTOR_4
#define FLASH_USER_FW_ADDR_0	ADDR_FLASH_SECTOR_5
#define FLASH_USER_FW_ADDR_1	ADDR_FLASH_SECTOR_6
#define FLASH_USER_FW_ADDR_2	ADDR_FLASH_SECTOR_7
#define FLASH_USER_FW_ADDR_3	ADDR_FLASH_SECTOR_8
#define FLASH_USER_FW_ADDR_4	ADDR_FLASH_SECTOR_9
#define FLASH_USER_FW_ADDR_5	ADDR_FLASH_SECTOR_10
#define FLASH_USER_FW_ADDR_6	ADDR_FLASH_SECTOR_11

enum
{
    FLASH_OK = 0,
    FLASH_FAIL = 1
};


/**
 * @brief  주어진 Flash 메모리 주소(Address)가 속한 섹터 번호를 반환
 * @param  Address  Flash 메모리 내 확인할 주소
 * @retval 해당 주소가 속한 섹터 번호 (0~23)
 *         주소가 유효하지 않으면 0xFFFFFFFF 반환
 */
uint32_t Flash_GetSector(uint32_t Address);

/**
 * @brief  Gets the bank of a given address
 * @param  Addr: Address of the FLASH Memory
 * @retval The bank of a given address
 */
uint32_t Flash_GetBank(uint32_t Addr);

/**
 * @brief  주어진 펌웨어 데이터를 Flash 메모리에 기록하는 함수
 * @param  fwData        Flash에 기록할 펌웨어 데이터 버퍼 (uint8_t 배열)
 * @param  flashAddress  Flash 메모리에 기록을 시작할 주소
 * @param  Size          기록할 데이터 크기 (바이트 단위)
 * @retval FLASH_OK(0) 또는 FLASH_FAIL(1) (기록 성공/실패 여부)
 */
int Flash_Write_Fw(uint8_t* fwData, uint32_t flashAddress, uint32_t size);

/**
 * @brief  지정된 Flash 주소에서 애플리케이션을 실행하는 함수 (부트 점프)
 * @param  startAddress  애플리케이션의 벡터 테이블 시작 주소
 * @note   현재 실행 중인 펌웨어에서 지정된 애플리케이션 영역으로 점프
 */
void Flash_JumpVectorTable(uint32_t startAddress);

/**
 * @brief  지정된 Flash 영역을 삭제하는 함수
 * @param  areaAddress  삭제할 영역의 시작 주소
 * @param  dataSize     삭제할 데이터 크기 (바이트 단위)
 * @note   지정된 크기에 해당하는 섹터 단위로 삭제 수행
 */
void Flash_Erase(uint32_t areaAddress, uint32_t dataSize);


#endif /* INC_FLASH_H_ */
