/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#include <xd_k.h>

static xd_uint32_t xd_tick = 0;
extern xd_list_t xd_task_priority_table[XD_TASK_PRIORITY_MAX];
extern xd_uint32_t xd_task_ready_priority_group;

void xd_tick_increase(void)
{
    xd_uint32_t i;
    struct xd_task* task;
    xd_tick++;
		xd_uint8_t res = signal_handler();
    for(i=0 ; i<XD_TASK_PRIORITY_MAX ; i++)
    {
	    if(i==1 || i==2)
			{
            task = xd_list_entry(xd_task_priority_table[i].next,
                                struct xd_task,
                                tlist);
            if(task->remaining_tick > 0)
            {
                task->remaining_tick --;
						}
						else if(task->remaining_tick == 0)
						{
							xd_task_ready_priority_group |= task->number_mask;
							//xd_task_resume(task);
						}
        
			}				
    }
		if(res == 1) return ;
    xd_scheduler();
}
