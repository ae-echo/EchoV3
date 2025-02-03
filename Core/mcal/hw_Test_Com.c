/*
 * hw_Test_Com.c
 *
 *  Created on: Jan 6, 2025
 *      Author: Kim Min Ki
 */

#include "adc.h"
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "i3c.h"
#include "fmc.h"
#include "hw_Test_Com.h"

//uint8_t comLength1;
//uint8_t comLength2;

void hw_test_main(uint8_t* buffer, uint8_t* count)
{
	//comLength1=buffer[1];
	//comLength2=buffer[2];
	switch ( buffer[3]) {
	    case 0x01:
	    	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)buffer, count); ; // ACK check
	    		break;
	    case 0x02:

	    	 UART1_send_32bit(ADC_Get_ADC(10), 4);

	    		break;
	    case 0x03:

	    	  DAC_Out(1, UART1_reciver_16bit_to_32bit(buffer, count));//DAC control ch1

	    	    break;
	    case 0x04:
	    	  DAC_Out(2, UART1_reciver_16bit_to_32bit(buffer, count)); //DAC control ch2

	    	    break;
	    case 0x05:
	    			LDO_ControlMain(1,buffer[4]); //2800
	    			break;
	    case 0x06:
	    			LDO_ControlMain(2,buffer[4]);
	    			break;
	    case 0x07:
	    	    	LDO_ControlMain(3,buffer[4]);
	    	    	    break;
	    case 0x08:
	    	    	LDO_ControlMain(4,buffer[4]);
	    	    	    break;
	    case 0x09:
	    	    	LDO_ControlMain(5,buffer[4]);
	    	    	    break;

	    case 0x0A:   // gpio bypass1 /2
	    	       if(buffer[4]==0x01)EXT_PW_OUT_CH1_BYPASS_EN(0);
	    	       else if(buffer[4]==0x02)EXT_PW_OUT_CH1_BYPASS_EN(1);
	    	       else if(buffer[4]==0x03)EXT_PW_OUT_CH2_BYPASS_EN(0) ;
	    	       else if(buffer[4]==0x04)EXT_PW_OUT_CH2_BYPASS_EN(1) ;

	    	      // void SW_I2C_Scan(uint8_t ch ,uint8_t time)

						break;
		case 0x0B:// ch1 12c /13c
				   if(buffer[4]==0x01) CH1_I2C_SEL(0);
				   else if(buffer[4]==0x02) CH1_I2C_SEL(1);
				   else if(buffer[4]==0x03)CH1_I3C_SEL(0)  ;
				   else if(buffer[4]==0x04)CH1_I3C_SEL(1) ;
						break;
		case 0x0C:// ch2 12c /13c
			       if(buffer[4]==0x01) CH2_I2C_SEL(0);
				   else if(buffer[4]==0x02) CH2_I2C_SEL(1);
				   else if(buffer[4]==0x03)CH2_I3C_SEL(0);
				   else if(buffer[4]==0x04)CH2_I3C_SEL(1);
			       	   break;

		case 0x0D:


			if( buffer[4]==0x00)SW_I2C_Scan(1 , 5);
			else if( buffer[4]==0x01)SW_I2C_Scan(2 , 5);
			else if( buffer[4]==0x04)HW_I2C_Scan(&hi2c4,2);
			else if( buffer[4]==0x02)HW_I2C_Scan(&hi2c2,2);

			break;

		case 0x0E:
			/*
		        uint32_t sdramBuffer;
		        uint32_t sdramBufferdata;
		        sdramBuffer=  (buffer[5] << 24) | (buffer[6] << 16) | (buffer[7] << 8) | buffer[8];
		        sdramBufferdata=(buffer[9] << 24) | (buffer[10] << 16) | (buffer[11] << 8) | buffer[12];
			      if(buffer[4]==0x00)
			      {

			    	  Wirte_SDRAM_32bit(sdramBufferdata,sdramBuffer);
			      }
			      if(buffer[4]==0x01)
			      {
			    	  UART1_send_32bit(Read_SDRAM_32bit(sdramBuffer), 4);
			      }*/

			      //51 0D F2 0E 01 D0 00 00 00  12 34 56 78 AE
			      //          R: 1 W :0         data 32bit
			      //510DF20E01D000000012345678AE

					       	  break;

		case 0x0F:

			    if(buffer[4]==0x00)
			    {
			    	uint8_t address =Scan_I2C_Target();
			    	uint8_t buffer_d[]={0xAE,0x06,0xf9,0xfe,address};
			    	UART1_Send(buffer_d, 5);
			    }
			    else if(buffer[4]==0x01)
			    {
			    	uint8_t address  =Scan_I3C_Target();
			    	uint8_t buffer_d[]={0xAE,0x06,0xf9,0xfe,address};
					UART1_Send(buffer_d, 5);
			    }
			    else if(buffer[4]==0x02)
			    {

			    	if(DW9785_Change_I3C_Mode(0x48, 0)==1)
			    	{
			    		//HAL_Delay(100);
			    		//uint8_t address  =Scan_I3C_Target();
			    		//uint8_t buffer_d[]={0xAE,0x06,0xf9,0xfe,address};
			    		//UART1_Send(buffer_d, 5);

			    		//I3C_ENTDAA(buffer[5],I3C_RSTDAA_THEN_ENTDAA);
			    	}

			    }







		break;




	    default: ;
	}





	//uart1.Buf, uart1.Count



}
