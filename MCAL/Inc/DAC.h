#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "main.h"
#include <dac.h>

#define FRA_CH_A		1
#define FRA_CH_B 		2

void DAC_Init(void);

/**
 * @brief  DAC 출력 설정 함수
 *         - 지정된 채널(`ch`)에 DAC 값을 설정
 * @param  ch     출력할 DAC 채널 (`FRA_CH_A` = 1, `FRA_CH_B` = 2)
 * @param  value  출력할 DAC 값 (12비트 정렬)
 */
void DAC_Out(uint32_t ch, uint32_t value);

#endif /* INC_DAC_H_ */
