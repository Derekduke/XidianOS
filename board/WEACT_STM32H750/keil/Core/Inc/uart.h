/*
 * @Author: spaceman
 * @Date: 2021-03-01 20:32:11
 * @LastEditTime: 2021-03-01 22:16:23
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\board\WEACT_STM32H750\keil\Core\Inc\uart.h
 */

#ifndef __UART_H_
#define __UART_H_

#include <xd_k.h>
#include <stdarg.h>
#include <stdio.h>

#include "main.h"

extern uint8_t RxBuff[1];

void put_char(const char ch);
int get_char(void);

void xd_printf(const char* fmt , ...);

#endif
