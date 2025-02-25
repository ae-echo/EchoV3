#include <MCAL/Inc/DAC.h>


void DAC_Init(void)
{
	MX_DAC1_Init();
}

/**
 * @brief  DAC 출력 설정 함수
 *         - 지정된 채널(`ch`)에 DAC 값을 설정
 * @param  ch     출력할 DAC 채널 (`FRA_CH_A` = 1, `FRA_CH_B` = 2)
 * @param  value  출력할 DAC 값 (12비트 정렬)
 */
void DAC_Out(uint32_t ch, uint32_t value)
{

    if(ch == 1)
    {
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R,value);
    }
    else if(ch == 2)
    {
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);
    }
    else
    {
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
        HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);
    }
}
