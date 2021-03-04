#include <xd_k.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <uart.h>

#define  CONSOLEBUF_SIZE 128

uint8_t RxBuff[1];      
uint8_t DataBuff[128]; 
int RxLine=0;  

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

extern struct semaphore shell_sem;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    RxLine++;                      
    DataBuff[RxLine-1]=RxBuff[0];  
		xd_printf("%c" , RxBuff[0]);
    if(RxBuff[0]== '\r')            
    { 
        RxLine=0; 		
        xd_sem_release(&shell_sem);
    }
    
    RxBuff[0]=0;
    HAL_UART_Receive_IT(&huart1, (uint8_t *)RxBuff, 1); 
}
