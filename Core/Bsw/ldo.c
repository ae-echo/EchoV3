/*
 * ldo.c
 *
 *  Created on: Jan 8, 2025
 *      Author: Kim Min Ki
 */


#include "ldo.h"
#include "bsw_config.h"

uint8_t g_LDO_Selects[5] = {13, 13, 13, 13, 13};

//Voltage테이블
float VoltageTable[STEPMAX] =
{
		1.036453f,		1.096886f,		1.156859f,		1.216374f,		1.275439f,		1.334058f,		1.392235f,		1.449977f,
		1.507287f,		1.564171f,		1.620634f,		1.676679f,		1.732313f,		1.787538f,		1.842360f,		1.896783f,
		1.950812f,		2.004450f,		2.057702f,		2.110572f,		2.163064f,		2.215183f,		2.266931f,		2.318314f,
		2.369334f,		2.419997f,		2.470305f,		2.520262f,		2.569872f,		2.619138f,		2.668065f,		2.716655f,
		2.764912f,		2.812839f,		2.860441f,		2.907719f,		2.954678f,		3.001320f,		3.047649f,		3.093668f,
		3.139381f,		3.184789f,		3.229896f,		3.274706f,		3.319220f,		3.363443f,
};
/*
float VoltageTable[STEPMAX] =
{
		1.036453f,		1.096886f,		1.156859f,		1.216374f,		1.275439f,		1.334058f,		1.392235f,		1.449977f,
		1.507287f,		1.564171f,		1.620634f,		1.676679f,		1.732313f,		1.787538f,		1.842360f,		1.896783f,
		1.950812f,		2.004450f,		2.057702f,		2.110572f,		2.163064f,		2.215183f,		2.266931f,		2.318314f,
		2.369334f,		2.419997f,		2.470305f,		2.520262f,		2.569872f,		2.619138f,		2.668065f,		2.716655f,
		2.764912f,		2.812839f,		2.860441f,		2.907719f,		2.954678f,		3.001320f,		3.047649f,		3.093668f,
		3.139381f,		3.184789f,		3.229896f,		3.274706f,		3.319220f,		3.363443f,		3.407376f,		3.451023f,
		3.494386f,		3.537469f,		3.580273f,		3.622802f,		3.665059f,		3.707045f,		3.748763f,		3.790217f,
		3.831408f,		3.872339f,		3.913012f,		3.953431f,		3.993596f,		4.033512f,		4.073179f,		4.112600f
};
*/

/**
 * @brief  Voltage값을 입력 후 근사치의 Step값 리턴
 * @param  Inputvoltage 권장은 1V~4.2V 사이
 * @retval Step 값
 */
uint8_t LDO_GetStep(float Inputvoltage)
{
    uint8_t closestStep = 0;
    float minDifference = fabs(Inputvoltage - VoltageTable[0]);

    for (uint8_t step = 1; step < STEPMAX; step++) {
        float difference = fabs(Inputvoltage - VoltageTable[step]);
        if (difference < minDifference) {
            minDifference = difference;
            closestStep = step;
        }
    }

    return closestStep;
}

/**
 * @brief  Count 값을 입력 후 Voltage 값 확인용
 * @param  Step: 0~63 까지의 Step 값
 * @retval Voltage 값
 */
float LDO_GetVoltage(uint8_t Step)
{
	int R17 = 3;
	int R16 = 200;
	int R21 = 10;
	float MCP4011Offset = 0.78125;
	float MCP4011 = Step * MCP4011Offset;
	float R_s = MCP4011 + R17;
	float R_s2 = R16 * R_s /(R16 + R_s);
	float Voltage =  (R21 +R_s2) / R21 *0.8;

	return Voltage;
}

void ldoEn(LDO_Select type, GPIO_PinState PinState)
{
	switch (type)
	{
		case EXT_PW_OUT_CH1:
			EXT_PW_OUT_CH1_EN(PinState);
			break;
		case EXT_PW_OUT_CH2:
			EXT_PW_OUT_CH2_EN(PinState);
			break;
		case SW_I2C_REF_PW_CH1:
			SW_I2C_REF_PW_CH1_EN(PinState);
			break;
		case SW_I2C_REF_PW_CH2:
			SW_I2C_REF_PW_CH2_EN(PinState);
			break;
		case I3C_REF_PW:
			I3C_REF_PW_EN(PinState);
			break;
	}
}

void ldoCs(LDO_Select type, GPIO_PinState PinState)
{
	switch (type)
	{
		case EXT_PW_OUT_CH1:
			EXT_PW_OUT_CH1_CS(PinState);
			break;
		case EXT_PW_OUT_CH2:
			EXT_PW_OUT_CH2_CS(PinState);
			break;
		case SW_I2C_REF_PW_CH1:
			SW_I2C_REF_PW_CH1_CS(PinState);
			break;
		case SW_I2C_REF_PW_CH2:
			SW_I2C_REF_PW_CH2_CS(PinState);
			break;
		case I3C_REF_PW:
			I3C_REF_PW_CS(PinState);
			break;
	}
}

void ldoUd(LDO_Select type, GPIO_PinState PinState)
{
	 switch (type)
	{
		case EXT_PW_OUT_CH1:
			EXT_PW_OUT_CH1_UD(PinState);
			break;
		case EXT_PW_OUT_CH2:
			EXT_PW_OUT_CH2_UD(PinState);
			break;
		case SW_I2C_REF_PW_CH1:
			SW_I2C_REF_PW_CH1_UD(PinState);
		 	break;
		case SW_I2C_REF_PW_CH2:
			SW_I2C_REF_PW_CH2_UD(PinState);
			break;
		case I3C_REF_PW:
			I3C_REF_PW_UD(PinState);
			break;
	}
}

void LOD_SetVoltage(LDO_Select LDO, float Voltage)
{
	uint8_t step = LDO_GetStep(Voltage);
	if(g_LDO_Selects[LDO] > step)
	{
		LDO_controldown(LDO, g_LDO_Selects[LDO] - step);
		g_LDO_Selects[LDO] = g_LDO_Selects[LDO] - (g_LDO_Selects[LDO] - step);
	}
	else if(g_LDO_Selects[LDO] < step)
	{
		LDO_controlup(LDO, step - g_LDO_Selects[LDO]);
		g_LDO_Selects[LDO] = g_LDO_Selects[LDO] + (step - g_LDO_Selects[LDO]);
	}
	else
	{}

}

void LDO_ControlMain(LDO_Select LDO)
{
	LDO_controldown(LDO, 64);
	//LDO_controlup(LDO,Step);
	g_LDO_Selects[LDO] = 0;
}

void LDO_controldown(LDO_Select LDO, uint8_t Step) // odw MCP4011
{
	ldoEn(LDO,1);

	ldoUd(LDO,1); // UD PIN HIGH
	ldoCs(LDO,1); // CS PIN HIGH

	HAL_Delay(3);

    ldoCs(LDO,0); // CS PIN LOW
	delay_us(2);

    for (int i = 0; i < Step; i++)
    {
    	ldoUd(LDO,0); // UD PIN LOW
    	delay_us(2);

        ldoUd(LDO,1);  // UD PIN HIGH
        delay_us(2);
    }

    delay_us(2);                          // DELAY
    ldoCs(LDO,1); // CS PIN HIGH
    delay_us(10);
}


void LDO_controlup(LDO_Select LDO, uint8_t Step) // odw MCP4011
{
	ldoEn(LDO,1);

	ldoCs(LDO,1);  	// CS PIN HI
	ldoUd(LDO,0);	// UD PIN LOW

	HAL_Delay(3);

    ldoCs(LDO,0); 	// CS PIN LOW
	delay_us(2);

    for (int i = 0; i < Step; i++)
    {
    	ldoUd(LDO,1);	// UD PIN HIGH
    	delay_us(2);

        ldoUd(LDO,0);	// UD PIN LOW
        delay_us(2);
    }

    delay_us(2);
    ldoCs(LDO,1); // CS PIN HIGH
    delay_us(10);
}



