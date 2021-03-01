#include <xd_k.h>
#include <string.h>

#define TASK_SHELL_STACK_SIZE	512

ALIGN(XD_ALIGN_SIZE)
static xd_uint8_t xd_task_shell_stack[TASK_SHELL_STACK_SIZE];
struct xd_task shell;

struct Signal shell_sig;
extern uint8_t DataBuff[128];

void xd_show_logo()
{
xd_printf("\n\r");
xd_printf(" __   ___     _ _              ____   _____ \n\r"); 
xd_printf(" \\ \\ / (_)   | (_)            / __ \\ / ____|\n\r");
xd_printf("  \\ V / _  __| |_  __ _ _ __ | |  | | (___  \n\r");
xd_printf("   > < | |/ _` | |/ _` | '_ \\| |  | |\\___ \\ \n\r");
xd_printf("  / . \\| | (_| | | (_| | | | | |__| |____) |\n\r");
xd_printf(" /_/ \\_\\_|\\__,_|_|\\__,_|_| |_|\\____/|_____/ \n\r");
xd_printf("\n\r");
}

const SHELL_CommandTypeDef shellCommandList[]=
{

    {(xd_uint8_t*)"ls" , ls_func , (xd_uint8_t*)"list file system"},
    {(xd_uint8_t*)"ps" , ps_func , (xd_uint8_t*)"show current tasks"},
    {(xd_uint8_t*)"help" , help_func , (xd_uint8_t*)"list commands"}
};

void xd_task_shell_entry(void* parameter)
{
	while(1)
	{
		/*shell task start*/
		//xd_printf("shell function\n\r");
		shell_command();
		xd_printf("\n\r");
		xd_printf("==>");
		/*shell task end*/
    	xd_task_suspend(&shell);
		xd_scheduler();
	}
}
void xd_task_shell_init(void)
{
	xd_task_init( 10,
				&shell,
				xd_task_shell_entry,
				XD_NULL,
				&xd_task_shell_stack[0],
				sizeof(xd_task_shell_stack),
				0
        );									
	shell.current_priority = shell.init_priority;
	shell.number_mask = 1 << (shell.current_priority);
	shell.stat = XD_TASK_SUSPEND;
	signal_init(&shell , 0 , &shell_sig);
	xd_show_logo();
	//xd_task_resume(&shell);
	xd_task_suspend(&shell);
}

void shell_command()
{
	xd_uint8_t i=0;
	char s[128];
	while(DataBuff[i] != '\r')
	{
		s[i] = DataBuff[i];
		//xd_printf("\n\rs[%d] = %c\n\r" , i , DataBuff[i]);
		i++;
	}
	s[i] = '\0';
	//xd_printf("\n\rs = %s\n\r" , s);
	if(strcmp(s , "help") == 0)
	{
		help_func();
	}
}

void ls_func()
{

}

void ps_func()
{

}

void help_func()
{
	uint8_t count = sizeof(shellCommandList)/sizeof(SHELL_CommandTypeDef);
	xd_printf("\n\rCommands number: %d\n\r" , count);
    for(xd_uint8_t i=0 ; i<count; i++)
    {
        xd_printf("\n\r%s  |   %s\n\r" , shellCommandList[i].name , shellCommandList[i].desc);
    }
}
