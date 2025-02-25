#ifndef __bsw_config_H
#define __bsw_config_H

#include <Mcal/Inc/uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

//#define PRINTF(...) UARTx_Printf(__VA_ARGS__)
#define PRINTF(fmt, ...) UARTx_Printf(__FILE__, __LINE__, fmt, ##__VA_ARGS__)



extern bool g_print_debug;

//int UARTx_Printf(const char *fmt, ...);
int UARTx_Printf(const char *file, int line, const char *fmt, ...);


#endif
