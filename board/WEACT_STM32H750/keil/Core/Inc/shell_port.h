/*
 * @Author: spaceman
 * @Date: 2021-03-01 21:05:15
 * @LastEditTime: 2021-03-01 21:08:30
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\board\WEACT_STM32H750\keil\Core\Inc\shell_port.h
 */

#ifndef __SHELL_PORT_H__
#define __SHELL_PORT_H__

#include <xd_k.h>
#include "shell.h"
#include "uart.h"

extern Shell shell;

void userShellInit(void);
#endif
