#ifndef __SHELL_H_
#define __SHELL_H_

#include <xd_k.h>

typedef void (*shellFunction)();

typedef struct
{
    xd_uint8_t *name;                                              //shell命令名称
    shellFunction function;                                     //shell命令函数
    xd_uint8_t *desc;                                              //shell命令描述
}SHELL_CommandTypeDef; 

void xd_show_logo(void);
void ls_func(void);
void ps_func(void);
void help_func(void);
void shell_command(void);

void xd_task_shell_entry(void* parameter);
void xd_task_shell_init(void);

#endif
