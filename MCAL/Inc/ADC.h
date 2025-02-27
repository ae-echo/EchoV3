#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <MCAL/Inc/MCAL_Common.h>
#include <adc.h>

void ADC_Init(void);

/**
 * @brief  ADC 값 읽기 함수
 *         - ADC 변환을 시작하고 완료될 때까지 대기한 후 변환된 값을 반환
 * @return 변환된 ADC 값 (uint32_t)
 */
uint32_t ADC_Get(void);

#endif /* INC_ADC_H_ */
