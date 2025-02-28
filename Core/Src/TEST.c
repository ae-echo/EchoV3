/*
 * TEST.c
 *
 *  Created on: Feb 27, 2025
 *      Author: JungBongJun
 */

#include <TEST.h>

uint32_t dacValue = 0;
uint32_t g_FmcOffset = 0;
uint8_t wfmcData = 0;
uint8_t rfmcData;
uint8_t wI2cData[4];
uint8_t rI2cData[4];
uint16_t wI2cData16[4];
uint8_t wI3cData[4];
uint8_t rI3cData[4];
uint8_t wUartBuf[4];


//I2C Write 동안 UART처리 미동작
//16BIT Seq 이상
void Function_Test(void)
{
	struct uart* pUart = UART1_GetInfo();

	if(pUart->Buf[0] != 0xAE)
		return;

	switch(pUart->Buf[1])
	{
		case 0x01:
			uint32_t adcValue;
			MCAL_PRINTF("ADC TEST\n");
			adcValue = ADC_Get();
			MCAL_PRINTF("ADC Value : %d\n", adcValue);
			break;
		case 0x02:
			MCAL_PRINTF("DAC TEST\n");
			DAC_Out(FRA_CH_A, dacValue);
			DAC_Out(FRA_CH_B, dacValue);
			MCAL_PRINTF("CH A : %d CH B : %d\n", dacValue, dacValue);
			dacValue += 200;
			if(dacValue > 4095)
				dacValue = 0;
			break;
		case 0x03:
			MCAL_PRINTF("FMC TEST\n");
			FMC_Write(FMC_SDRAM_BLOCK_0 + g_FmcOffset, 1, &wfmcData);
			FMC_Read(FMC_SDRAM_BLOCK_0 + g_FmcOffset, 1, &rfmcData);
			MCAL_PRINTF("ADDR : %.8X WData %.8X RData %.8X\n",
					FMC_SDRAM_BLOCK_0 + g_FmcOffset, wfmcData, rfmcData);
			g_FmcOffset+=4;
			wfmcData++;
			break;
		case 0x04:
			MCAL_PRINTF("GPIO TEST\n");
			B_LED_TOG;
			break;
		case 0x05:
			wI2cData[0] = 0x01;
			MCAL_PRINTF("I2C Master Send\n");
			I2C_SetChannel(I2C_CH1, I2C_1M);
			I2C_Write(0x24, 0x04, wI2cData, 1);
			break;
		case 0x06:
			MCAL_PRINTF("I2C Master SeqSend\n");
			I2C_SetChannel(I2C_CH1, I2C_1M);
			wI2cData[0] = 0x00;
			wI2cData[1] = 0x01;
			wI2cData[2] = 0x02;
			wI2cData[3] = 0x03;
			pUart->nCount = 0;
			memset(pUart->Buf, 0x00, sizeof(pUart->Buf));
			I2C_Write(0x24, 0x04, wI2cData, 4);
			break;
		case 0x07:
			MCAL_PRINTF("I2C Master Recv\n");
			I2C_SetChannel(I2C_CH1, I2C_1M);
			I2C_Read(0x24, 0x00, rI2cData, 1);
			MCAL_PRINTF("Recv Data : %.2X\n", rI2cData[0]);
			break;
		case 0x08:
			MCAL_PRINTF("I2C Mater SeqRecv\n");
			I2C_SetChannel(I2C_CH1, I2C_1M);
			I2C_Read(0x24, 0x00, rI2cData, 4);
			MCAL_PRINTF("Recv Data : %.2X\n", rI2cData[0]);
			MCAL_PRINTF("Recv Data : %.2X\n", rI2cData[1]);
			MCAL_PRINTF("Recv Data : %.2X\n", rI2cData[2]);
			MCAL_PRINTF("Recv Data : %.2X\n", rI2cData[3]);
			break;
		case 0x09:
			I2C_SetChannel(I2C_CH1, I2C_1M);
			wI2cData16[0] = 0x4321;
			MCAL_PRINTF("I2C Master I2C Write 16BIT\n");
			I2C_Write_16BIT(0x48, 0x1234, wI2cData16,1);
			break;
		case 0x0A:
			I2C_SetChannel(I2C_CH1, I2C_1M);
			wI2cData16[0] = 0x4321;
			wI2cData16[1] = 0x2345;
			wI2cData16[2] = 0x4358;
			wI2cData16[3] = 0x6715;
			MCAL_PRINTF("I2C Master I2C Write 16BIT Seq\n");
			I2C_Write_16BIT(0x48, 0x1234, wI2cData16,4);
			break;
		case 0x0B:
			MCAL_PRINTF("I3C RSTDAA, SETDASA\n");
			I3C_SetChannel(I3C_CH1);
			I3C_RSTDAA(_I2C_MODE_);
			I3C_SETDASA(0x6A, 0x32);
			break;
		case 0x0C:
			wI3cData[0] = 0x33;
			wI3cData[1] = 0x43;
			MCAL_PRINTF("I3C Write\n");
			I3C_SetChannel(I3C_CH1);
			I3C_Write(0x64, 0x01, wI3cData, 2);
			break;
		case 0x0D:
			wI3cData[0] = 0x05;
			wI3cData[1] = 0x43;
			wI3cData[2] = 0x42;
			wI3cData[3] = 0xF1;
			MCAL_PRINTF("I3C Write Seq\n");
			I3C_SetChannel(I3C_CH1);
			//I3C_Write(0x64, 0x01, wI3cData,4);
			break;
		case 0x0E:
			MCAL_PRINTF("I3C Read\n");
			I3C_SetChannel(I3C_CH1);
			I3C_Read(0x64, 0x01, rI3cData, 1);
			MCAL_PRINTF("Read Data : %.2X \n", rI3cData[0]);
			I3C_Read(0x64, 0x02, rI3cData, 1);
			MCAL_PRINTF("Read Data : %.2X \n", rI3cData[0]);
			break;
		case 0x0F:
			MCAL_PRINTF("I3C Read Seq\n");
			I3C_SetChannel(I3C_CH1);
			I3C_Read(0x64, 0x01, rI3cData, 4);
			MCAL_PRINTF("Read Data : %.2X %.2X %.2X %.2X \n",
					rI3cData[0], rI3cData[1], rI3cData[2], rI3cData[3]);
			break;
		case 0x10:
			MCAL_PRINTF("UART TEST\n");
			wUartBuf[0] = 0xAE;
			wUartBuf[1] = 0xEA;
			//UART1_Send(wUartBuf, 2);
			HAL_UART_Transmit_DMA(&huart1, wUartBuf, 2);
			break;

	}

	pUart->nCount = 0;
	memset(pUart->Buf, 0x00, sizeof(pUart->Buf));
}
