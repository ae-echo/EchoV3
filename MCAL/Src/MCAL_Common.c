#include <MCAL/Inc/MCAL_Common.h>

void MCAL_Init(void)
{
	GPIO_Init();
	ADC_Init();
	DAC_Init();
	Time_init();
	FMC_Init();
	UART_Init();
	I2C_Init();
	I3C_Init();	// the init need to set  LDO_controldown(5,64); LDO_controlup(5,33) ;  with  delay
}
