#include <xd_k.h>
#include <stdarg.h>
#include <stdio.h>

#define  CONSOLEBUF_SIZE 128

extern UART_HandleTypeDef huart1;
void xd_printf(const char* fmt , ...)
{
    va_list args;
    xd_uint8_t length;
    static char xd_buf[CONSOLEBUF_SIZE];
    va_start(args , fmt);
    length = vsnprintf(xd_buf , sizeof(xd_buf) , fmt , args);
    if(length > CONSOLEBUF_SIZE)
        length = CONSOLEBUF_SIZE;
    xd_console_output(xd_buf);
    va_end(args);
}

void xd_console_output(const char* str)
{
    xd_uint32_t level;
    level = xd_interrupt_disable();
    while(*str != '\0')
    {
        if(*str == '\n')
        {
            HAL_UART_Transmit(&huart1 , (xd_uint8_t*)'\r' , 1 , 1000);
        }
        HAL_UART_Transmit(&huart1 , (xd_uint8_t*)(str++) , 1 , 1000);
    }
    xd_interrupt_enable(level);
}
