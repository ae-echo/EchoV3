#ifndef INC_MCAL_COMMON_H_
#define INC_MCAL_COMMON_H_

#include "main.h"
#include "stdbool.h"
#include "common.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>

#include <MCAL/Inc/ADC.h>
#include <MCAL/Inc/DAC.h>
#include <MCAL/Inc/FLASH.h>
#include <MCAL/Inc/FMC.h>
#include <MCAL/Inc/GPIO.h>
#include <MCAL/Inc/I2C.h>
#include <MCAL/Inc/I3C.h>
#include <MCAL/Inc/TIME.h>
#include <MCAL/Inc/UART.h>

#define MCAL_PRINTF(fmt, ...) PRINTF("<MCAL Layer> " fmt, ##__VA_ARGS__)

void MCAL_Init(void);

//해당 헤더에 LAYER PRINT 생성

#endif /* INC_MCAL_COMMON_H_ */
