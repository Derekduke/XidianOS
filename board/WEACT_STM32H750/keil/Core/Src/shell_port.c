/*
 * @Author: spaceman
 * @Date: 2021-03-01 21:05:09
 * @LastEditTime: 2021-03-05 17:46:37
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\board\WEACT_STM32H750\keil\Core\Src\shell_port.c
 */

#include "shell.h"
#include "uart.h"



#define TASK_SHELL_STACK_SIZE	512

ALIGN(XD_ALIGN_SIZE)
static xd_uint8_t xd_task_shell_stack[TASK_SHELL_STACK_SIZE];
struct xd_task shell_task;

struct semaphore shell_sem;


Shell shell;
char shellBuffer[512];

/**
 * @brief 用户shell写
 *
 * @param data 数据
 */
void userShellWrite(char data)
{
    put_char(data);
}


/**
 * @brief 用户shell读
 *
 * @param data 数据
 * @return char 状态
 */
signed char userShellRead(char *data)
{
    xd_sem_take(&shell_sem , &shell_task);
    *data = get_char();
    return *data!=0xff?0:-1;
}


/**
 * @brief 用户shell初始化
 *
 */
void userShellInit(void)
{
    shell.write = userShellWrite;
    shell.read = userShellRead;
    shellInit(&shell, shellBuffer, 512);

    xd_task_init( 10,
				&shell_task,
				shellTask,
				&shell,
				&xd_task_shell_stack[0],
				sizeof(xd_task_shell_stack),
				0
        );
    xd_sem_init(&shell_sem , "shell" , 0);
    xd_task_startup(&shell_task);
}
