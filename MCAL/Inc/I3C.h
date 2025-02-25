#ifndef INC_I3C_H_
#define INC_I3C_H_

#include "main.h"
#include <i3c.h>
#include <stdbool.h>
#include "common.h"


#define _I2C_MODE_ false
#define _I3C_MODE_ true

#define Brodacast_DISEC 0x01
#define Brodacast_RST 0x06
#define Brodacast_SETDASA 0x87

/**
 * @brief  I2C 스캔 시 제외할 주소 확인 매크로
 *         - 특정 I2C 주소(예약되었거나 사용하지 않는 주소)를 제외
 * @param  i  검사할 I2C 주소 (0x00 ~ 0x7F)
 * @return 제외할 주소면 true(1), 아니면 false(0)
 */
#define NOT_USE_I3C_ADDR(i) ((i == 0x3E) || (i == 0x5E) || (i == 0x6E) || (i == 0x76) || \
                             (i == 0x7A) || (i == 0x7C) || (i == 0x7E) || (i == 0x7F))

//DWIC로 이동
// I3C 비교 대상 ENUM 정의
typedef enum {
    I3C_DEVICE_DW9785,
    I3C_DEVICE_DW97XX
} I3C_DeviceType;

//I3C 지원 IC별 c파일로 이동 h에는 extern으로 선언
// 전역 비교 대상 페이로드 정의
static const uint8_t DW9785_payload[8] = {0x05, 0x44, 0x00, 0x00, 0x00, 0x00, 0x06, 0xE4};
static const uint8_t DW97xx_payload[8] = {0x05, 0x44, 0x00, 0x00, 0x00, 0x00, 0x06, 0xE5};

extern I3C_HandleTypeDef hi3c1;

void I3C_Init(void);

/**
 * @brief   I3C RSTDAA (Reset Dynamic Address Assignment) 명령 실행
 * @details 모든 I3C 슬레이브 디바이스의 Dynamic Address를 초기화하는 명령 (RSTDAA, 0X06)을 전송합니다.
 *          이 과정 후, 모든 슬레이브는 다시 ENTDAA(0x7E)를 통해 새로운 Dynamic Address를 할당받아야 합니다.
 * @param   flag_CurrComMod - 현재 통신 모드를 지정
 *            - `true`  : I3C 모드에서 실행
 *            - `false` : I2C 모드에서 실행
 * @return  uint32_t - I3C 에러 코드 (정상 실행 시 0)
 */
uint32_t I3C_RSTDAA(bool flag_CurrComMod);

/**
 * @brief  I3C Dynamic Address Assignment (ENTDAA)
 *         - ENUM을 통해 비교 기준을 선택하여 주소 할당
 * @param  dynAddr  할당할 Dynamic Address
 * @param  device   비교할 장치 타입 (I3C_DEVICE_DW9785, I3C_DEVICE_DW97XX 등)
 * @return I3C 에러 코드 반환
 */
uint32_t I3C_ENTDAA(uint8_t dynAddr, I3C_DeviceType device);

/**
 * @brief  I3C SETDASA (Dynamic Address Assignment)
 *         - Broadcast 및 Direct CCC 명령을 실행하여 동적 주소 할당
 * @param  targetAddr  대상 I3C 장치 정적 주소 (예: LSM6DSO_STA)
 * @param  dynAddr     할당할 Dynamic Address
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t I3C_SETDASA(uint8_t targetAddr, uint8_t dynAddr);

/**
 * @brief  I3C Private Read (8비트 주소 체계)
 *         - 특정 ID의 I3C 장치에서 지정된 크기(size)만큼 데이터를 읽음
 * @param  id    대상 I3C 장치 주소
 * @param  addr  8비트 레지스터 주소
 * @param  dat   읽은 데이터를 저장할 버퍼 포인터
 * @param  size  읽을 데이터 크기
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t I3C_Read(uint8_t id, uint8_t addr, uint8_t* dat, uint16_t size);

/**
 * @brief  I3C Private Read (16비트 주소 체계)
 *         - 특정 ID의 I3C 장치에서 지정된 크기(size)만큼 16비트 데이터를 읽음
 * @param  id    대상 I3C 장치 주소
 * @param  addr  16비트 레지스터 주소
 * @param  dat   읽은 데이터를 저장할 버퍼 포인터 (`uint16_t` 배열)
 * @param  size  읽을 데이터 크기 (16비트 단위, size=3이면 6바이트 읽음)
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t I3C_Read_16BIT(uint8_t id, uint16_t addr, uint16_t* dat, uint16_t size);

/**
 * @brief  I3C Private Write (데이터 전송)
 *         - 특정 ID의 I3C 장치로 지정된 크기(size)만큼 데이터를 전송
 * @param  id    대상 I3C 장치 주소 (7-bit 변환됨)
 * @param  dat   전송할 데이터 버퍼 포인터
 * @param  size  전송할 데이터 크기 (BYTE 단위)
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t I3C_Write(uint8_t id, uint8_t *dat, uint16_t size);

/**
 * @brief  I3C Private Write (16비트 주소 + 16비트 데이터 전송)
 *         - 특정 ID의 I3C 장치에서 16비트 주소(addr)로 지정된 크기(size)만큼 16비트 데이터를 전송
 * @param  id    대상 I3C 장치 주소 (7-bit 변환됨)
 * @param  addr  16비트 레지스터 주소
 * @param  data  전송할 16비트 데이터 버퍼 포인터
 * @param  size  전송할 데이터 크기 (16비트 단위, size * 2BYTE 전송됨)
 * @return I3C 에러 코드 (HAL_I3C_ERROR_* 매크로 사용)
 */
uint32_t i3c_write_16BIT(uint8_t id, uint16_t addr, uint16_t* data, uint16_t size);

/**
 * @brief  I3C 또는 I2C 장치 스캔 함수
 *         - 현재 통신 모드(`flag_CurrComMod`)에 따라 I2C 또는 I3C 장치를 검색
 * @param  flag_CurrComMod  현재 통신 모드 (true: I2C 모드, false: I3C 모드)
 * @return 감지된 장치 개수 (uint8_t)
 */
uint8_t I3C_Scan_Target(bool flag_CurrComMod);

#endif /* INC_I3C_H_ */
