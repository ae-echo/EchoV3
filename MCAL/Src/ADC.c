#include <MCAL/Inc/ADC.h>


void ADC_Init(void)
{
	MX_ADC1_Init();
}


/**
 * @brief  ADC 값 읽기 함수
 *         - ADC 변환을 시작하고 완료될 때까지 대기한 후 변환된 값을 반환
 * @return 변환된 ADC 값 (uint32_t)
 */
uint32_t ADC_Get(void)
{
	  uint32_t value ;
	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 10);
	  value = HAL_ADC_GetValue(&hadc1);
	  HAL_ADC_Stop(&hadc1);
	  return  value;
}
