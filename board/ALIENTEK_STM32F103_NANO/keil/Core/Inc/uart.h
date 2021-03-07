#ifndef __UART_H_
#define __UART_H_

#include <xd_k.h>
#include <stdarg.h>
#include <stdio.h>

#include "main.h"

extern uint8_t RxBuff[1];

void xd_printf(const char* fmt , ...);
void xd_console_output(const char* str);

void xd_printf(const char* fmt , ...);

#endif
