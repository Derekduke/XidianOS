#include <xd_k.h>

#define TASK_SHELL_STACK_SIZE	512

ALIGN(XD_ALIGN_SIZE)
static xd_uint8_t xd_task_shell_stack[TASK_SHELL_STACK_SIZE];
struct xd_task shell;

struct Signal shell_sig;

void xd_task_shell_entry(void* parameter)
{
	while(1)
	{
			/*shell task start*/
			xd_printf("shell function\n\r");
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
	//xd_task_resume(&shell);
	xd_task_suspend(&shell);
}

