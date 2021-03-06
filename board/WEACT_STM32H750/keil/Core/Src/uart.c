/*
 * @Author: spaceman
 * @Date: 2021-03-01 20:31:13
 * @LastEditTime: 2021-03-05 17:46:05
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\board\WEACT_STM32H750\keil\Core\Src\uart.c
 */
#include "uart.h"

#define  CONSOLEOUTBUF_SIZE 128
#define  CONSOLEINBUF_SIZE  128


static char rxbuff[CONSOLEINBUF_SIZE] = {0};
static xd_uint16_t get_idx = 0;
static xd_uint16_t put_idx = 0;

extern UART_HandleTypeDef huart1;

void xd_kprint_port(const char *ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)ch, 1, 0xFFFF);
}



void put_char(const char ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
}

unsigned char get_char(void)
{
    char res;
    if(get_idx == put_idx) return 0xff;//只能卡死在这里 不然收不到数据

    res = rxbuff[get_idx++];
    if(get_idx >= CONSOLEINBUF_SIZE)
        get_idx = 0;
    return res;
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

void xd_printf(const char* fmt , ...)
{
    va_list args;
    xd_uint8_t length;
    static char xd_buf[CONSOLEOUTBUF_SIZE];
    va_start(args , fmt);
    length = vsnprintf(xd_buf , sizeof(xd_buf) , fmt , args);
    if(length > CONSOLEOUTBUF_SIZE)
        length = CONSOLEOUTBUF_SIZE;
    xd_console_output(xd_buf);
    va_end(args);
}

extern struct semaphore shell_sem;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t tempbuff;
    HAL_UART_Receive_IT(&huart1, &tempbuff, 1);

    xd_sem_release(&shell_sem);

    if(get_idx - put_idx == 1)//get_idx 在前一个位置
        get_idx++;//会丢弃最后一个数据

    rxbuff[put_idx++] = tempbuff;
    if(put_idx >= CONSOLEINBUF_SIZE)
        put_idx = 0;
}

