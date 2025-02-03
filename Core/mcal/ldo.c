/*
 * ldo.c
 *
 *  Created on: Jan 8, 2025
 *      Author: Kim Min Ki
 */


#include "ldo.h"





uint8_t LDO_Voltage(uint8_t count)// MCP4011 : 0.78125+....     =((0.78125+3)*200/((0.78125+3)+200)+10)/10*0.8
{
	uint8_t Voltage = (((count*0.78125)+3)*200/(((count*0.78125)+3)+200)+10)/10*0.8*10;
}



static void delay_us(volatile uint32_t delay)
{
  for (volatile int i = 0; i < delay; i++)
    for (volatile int j = 0; j < 25; j++)
      ;
}


void ldoEn(uint32_t type, uint8_t pow)
{
		switch (type)
		{
		case 1:
				EXT_PW_OUT_CH1_EN(pow);
		break;

		case 2:
				EXT_PW_OUT_CH2_EN(pow);
		break;

		case 3:
			SW_I2C_REF_PW_CH1_EN(pow);
		break;
		case 4:
			SW_I2C_REF_PW_CH2_EN(pow) ;
		break;
		case 5:
			I3C_REF_PW_EN(pow)   ;
		break;
		}
}
void ldoCs(uint32_t type, uint8_t pow)

{
		switch (type)
		{
			case 1:
				EXT_PW_OUT_CH1_CS(pow);
				break;
			case 2:
				EXT_PW_OUT_CH2_CS(pow);
				break;
			case 3:
				SW_I2C_REF_PW_CH1_CS(pow)  ;
				break;
			case 4:
				SW_I2C_REF_PW_CH2_CS(pow);
				break;
			case 5:
				I3C_REF_PW_CS(pow) ;
				break;

		}
}

void ldoUd(uint32_t type, uint8_t pow)
{
	 switch (type)
		    {
		    case 1:
		    	EXT_PW_OUT_CH1_UD(pow);
		        break;

		    case 2:
		    	EXT_PW_OUT_CH2_UD(pow);
		        break;

	         case 3:
	        	 SW_I2C_REF_PW_CH1_UD(pow)  ;
			    	break;
			    case 4:
			    	SW_I2C_REF_PW_CH2_UD(pow)   ;
			    		    	break;
			    case 5:
			    	I3C_REF_PW_UD(pow);
			    		    	break;
		    }
}

void LDO_ControlMain(uint32_t LDO,uint8_t count)
{
	LDO_controldown(LDO,64);
	LDO_controlup(LDO,count);
}

void LDO_controldown(uint32_t LDO,uint8_t count) // odw MCP4011
{



	ldoUd(LDO,1); // UD PIN HIGH
	ldoCs(LDO,1); // CS PIN HIGH
    delay_us(10);                                       // DELAY

    ldoCs(LDO,0); // CS PIN LOW
    delay_us(10);                                          // DELAY

    for (int i = 0; i < count; i++)
    {
    	ldoUd(LDO,0); // UD PIN LOW
        delay_us(10);                                          // DELAY
        ldoUd(LDO,1);  // UD PIN HIGH
        delay_us(10);                                          // DELAY
    }
    delay_us(10);                                       // DELAY
    ldoCs(LDO,1); // CS PIN HIGH
    delay_us(10);

}


void LDO_controlup(uint32_t LDO,uint8_t count) // odw MCP4011
{


	//ldoEn(LDO,1);
	ldoEn(LDO,1);


	ldoCs(LDO,1);  // CS PIN HI
	ldoUd(LDO,0);// UD PIN LOW
    delay_us(10);                                         // DELAY

    ldoCs(LDO,0); // CS PIN LOW
    delay_us(10);                                          // DELAY
    for (int i = 0; i < count; i++)
    {
    	ldoUd(LDO,1); // UD PIN HIGH
        delay_us(10);                                        // DELAY

        ldoUd(LDO,0);// UD PIN LOW
        delay_us(10);                                          // DELAY
    }

    delay_us(10);                                       // DELAY
    ldoCs(LDO,1); // CS PIN HIGH
}



