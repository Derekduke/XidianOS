/*
 * @Author: spaceman
 * @Date: 2021-03-01 21:05:09
 * @LastEditTime: 2021-03-01 22:20:16
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\board\WEACT_STM32H750\keil\Core\Src\shell_port.c
 */

#include "shell.h"
// #include "serial.h"
// #include "stm32f4xx_hal.h"
#include "uart.h"



#define TASK_SHELL_STACK_SIZE	512

ALIGN(XD_ALIGN_SIZE)
static xd_uint8_t xd_task_shell_stack[TASK_SHELL_STACK_SIZE];
struct xd_task shell_task;

struct Signal shell_sig;



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
    int temp = get_char();
    if(temp != -1)
    // if (serialReceive(&debugSerial, (uint8_t *)data, 1, 0) == 1)
    {
        *data = temp;
        return 0;
    }
    else
    {
        return -1;
    }

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
	signal_init(&shell_task , 0 , &shell_sig);
    xd_task_startup(&shell_task);
}






// void xd_task_shell_entry(void* parameter)
// {
// 	while(1)
// 	{
// 			/*shell task start*/
// 			xd_printf("shell function\n\r");
// 		  xd_printf("==>");
// 			/*shell task end*/
//       xd_task_suspend(&shell);
// 			xd_scheduler();
// 	}
// }

// void xd_task_shell_init(void)
// {
// 	xd_task_init( 10,
// 				&shell_task,
// 				shellTask,
// 				&shell,
// 				&xd_task_shell_stack[0],
// 				sizeof(xd_task_shell_stack),
// 				0
//         );

// 	// shell_task.current_priority = shell.init_priority;
// 	// shell_task.number_mask = 1 << (shell.current_priority);
// 	// shell_task.stat = XD_TASK_SUSPEND;
// 	signal_init(&shell_task , 0 , &shell_sig);
// 	// xd_task_suspend(&shell_task);

//     xd_task_startup(&shell_task);
// }


