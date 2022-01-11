/*
 * @Author: spaceman
 * @Date: 2021-03-05 17:15:57
 * @LastEditTime: 2021-03-05 18:03:00
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\kernel\src\kservice.c
 */
#include <kservice.h>
#include <stdarg.h>
#include <stdio.h>

__weak void xd_kprint_port(xd_uint8_t *ch)
{
    //You need to transplant the function
}

__weak void xd_ms_delay(xd_uint32_t time)
{
		//You need to transplant the function
}


static void __xd_kprint(const char *str)
{
    xd_uint32_t level;
    level = xd_interrupt_disable();
    while (*str != '\0')
    {
        xd_kprint_port((xd_uint8_t *)str++);
    }
    xd_interrupt_enable(level);
}

void xd_kprintf(const char *fmt, ...)
{
    va_list args;
    xd_uint8_t length;
    static char xd_buf[KPRINTF_BUFSIZE];
    va_start(args, fmt);
    length = vsnprintf(xd_buf, sizeof(xd_buf), fmt, args);
    if (length > KPRINTF_BUFSIZE)
        length = KPRINTF_BUFSIZE;
    __xd_kprint(xd_buf);
    va_end(args);
}

/**
 * This function will show the version of XidianOS
 */
void xd_show_version(void)
{
    xd_kprintf("\033[2J\033[1H");//clear console
    xd_kprintf("\r\n");
    xd_kprintf(" __   ___     _ _              ____   _____ \r\n");
    xd_kprintf(" \\ \\ / (_)   | (_)            / __ \\ / ____|\r\n");
    xd_kprintf("  \\ V / _  __| |_  __ _ _ __ | |  | | (___  \r\n");
    xd_kprintf("   > < | |/ _` | |/ _` | '_ \\| |  | |\\___ \\ \r\n");
    xd_kprintf("  / . \\| | (_| | | (_| | | | | |__| |____) |\r\n");
    xd_kprintf(" /_/ \\_\\_|\\__,_|_|\\__,_|_| |_|\\____/|_____/ \r\n");
    xd_kprintf("\r\n");
    xd_kprintf("Build:         "__DATE__" "__TIME__"\r\n");
    xd_kprintf("Version:       %d.%d.%d \r\n", XD_VERSION, XD_SUBVERSION, XD_REVISION);
    xd_kprintf("Copyright:     (c) 2021 \r\n");
    xd_kprintf("\r\n");
}

xd_int32_t xd_strncmp(const char *cs, const char *ct, xd_uint32_t count)
{
    register signed char __res = 0;

    while (count)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count --;
    }

    return __res;
}
