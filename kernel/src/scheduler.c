/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#include <xd_k.h>

xd_list_t xd_task_priority_table[XD_TASK_PRIORITY_MAX];
xd_uint32_t xd_task_ready_priority_group;
struct priority_list xd_task_priority_list[XD_TASK_PRIORITY_MAX];

struct xd_task* xd_current_task;
xd_uint8_t xd_current_priority;
xd_uint8_t kernel_state;

void xd_system_scheduler_init(void)
{
	register xd_uint32_t offset;
	for(offset=0 ; offset<XD_TASK_PRIORITY_MAX ; offset++)
	{
		xd_list_init(&xd_task_priority_table[offset]);
		xd_task_priority_list[offset].member = 0;
	}
	xd_current_priority = XD_TASK_PRIORITY_MAX - 1;
	xd_current_task = XD_NULL;
	xd_task_ready_priority_group = 0;
}

void xd_system_scheduler_start(void)
{
	register struct xd_task* to_task;
	register xd_uint32_t highest_ready_priority;
	highest_ready_priority = xd_find_task(xd_task_ready_priority_group);
	to_task = xd_list_entry(xd_task_priority_table[highest_ready_priority].next , struct xd_task , tlist);
	to_task->stat = XD_TASK_RUNNING;
	xd_current_task = to_task;

	kernel_state = 1;
	xd_context_switch_to((xd_uint32_t)&(to_task->sp));
}

void xd_scheduler(void)
{
	xd_uint32_t level;
	register xd_uint32_t highest_ready_priority;
	struct xd_task* to_task;
	struct xd_task* from_task;
	level = xd_interrupt_disable();
	highest_ready_priority = xd_find_task(xd_task_ready_priority_group);
	to_task = xd_list_entry(xd_task_priority_table[highest_ready_priority].next , struct xd_task , tlist);
	if(to_task != xd_current_task)
	{
		xd_current_priority = (xd_uint8_t)highest_ready_priority;
		from_task = xd_current_task;
		xd_current_task = to_task;
		xd_current_task->stat = XD_TASK_RUNNING;
		xd_context_switch((xd_uint32_t)&from_task->sp , (xd_uint32_t)&to_task->sp);
		xd_interrupt_enable(level);
	}
	else
	{
		xd_interrupt_enable(level);
	}
}

void xd_scheduler_insert_task(struct xd_task* task)
{
	register xd_uint32_t level;
	level = xd_interrupt_disable();
	task->stat = XD_TASK_READY;
	xd_list_insert_before(&(xd_task_priority_table[task->current_priority]) , &(task->tlist));
	if(xd_task_priority_list[task->current_priority].member == 0)
	{
		xd_task_ready_priority_group |= task->number_mask;
		xd_task_priority_list[task->current_priority].member++;
	}
	xd_interrupt_enable(level);
}

void xd_scheduler_remove_task(struct xd_task* task)
{
	register xd_uint32_t level;
	level = xd_interrupt_disable();
	xd_list_remove(&(task->tlist));
	xd_interrupt_enable(level);
}

xd_uint8_t xd_find_task(xd_uint32_t val)
{
	for(xd_uint8_t i=0 ; i<XD_TASK_PRIORITY_MAX ; i++)
	{
		if(((val>>i)&(0x0001)) == 1)
		{
			return i;
		}
		else
		{
			continue;
		}
	}
	return XD_ERROR;
}

xd_uint8_t kernel_running()
{
	return kernel_state;
}

/*flag=1:suspend，flag=2:resume，flag=3：time_delay*/
void update_priority_list(struct xd_task* task , xd_uint8_t flag)
{
	if(flag == TASK_SUSPEND)
	{
		xd_task_priority_list[task->current_priority].member --;
		if(xd_task_priority_list[task->current_priority].member == 0)
		{
			xd_task_ready_priority_group &= ~(task->number_mask);
		}
	}	
	if(flag == TASK_RESUME)
	{
		xd_task_ready_priority_group |= task->number_mask;
		xd_task_priority_list[task->current_priority].member ++;
	}
	if(flag == TASK_DELAY)
	{
		if(xd_task_priority_list[task->current_priority].member == 1)
			xd_task_ready_priority_group &= ~(task->number_mask);
	}
}
