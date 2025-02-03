#ifndef __bsw_config_H
#define __bsw_config_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include "uart.h"

#define PRINTF(s)		UARTx_Printf s

extern bool g_print_debug;

int _printf(const char *fmt, ...);

#endif
