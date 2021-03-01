/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#include <xd_k.h>

extern xd_list_t xd_task_priority_table[XD_TASK_PRIORITY_MAX];
extern struct xd_task* xd_current_task;
extern xd_uint32_t xd_task_ready_priority_group;

xd_uint8_t xd_task_init(xd_uint8_t num,
						struct xd_task* task,
                        void (*entry)(void* parameter),
                        void *parameter,
                        void *stack_start,
                        xd_uint32_t stack_size,
						xd_uint8_t priority
						)
{
    xd_list_init(&(task->tlist));
    task->num = num;
		task->entry = (void*)entry;
		task->parameter = parameter;
		task->stack_addr = stack_start;
		task->stack_size = stack_size;
		task->sp = 
		(void*)xd_stack_init(task->entry,
						task->parameter,
		                (void*)((char*)task->stack_addr + task->stack_size - 4));
		task->init_priority = priority;
		task->current_priority = priority;
		task->number_mask = 0;
		task->error = XD_EOK;
		task->stat = XD_TASK_INIT;
		return XD_EOK;
}

#define IDLE_THREAD_STACK_SIZE	512

ALIGN(XD_ALIGN_SIZE)
static xd_uint8_t xd_task_stack[IDLE_THREAD_STACK_SIZE];
struct xd_task idle;
xd_uint32_t xd_idletask_ctr = 0;

void xd_task_idle_entry(void* parameter)
{
	while(1)
	{
			/*idle task*/
		xd_idletask_ctr ++;
	}
}

void xd_task_idle_init(void)
{
	xd_task_init( 0,
				&idle,
				xd_task_idle_entry,
				XD_NULL,
				&xd_task_stack[0],
				sizeof(xd_task_stack),
				XD_TASK_PRIORITY_MAX-1
                );									
	xd_task_startup(&idle);
}

xd_uint8_t xd_task_startup(struct xd_task* task)
{
	task->current_priority = task->init_priority;
	task->number_mask = 1 << (task->current_priority);
	task->stat = XD_TASK_SUSPEND;
	xd_task_resume(task);
	//if(xd_current_task != XD_NULL)
	//{
//		xd_scheduler();
	//}
	return XD_EOK;
}

xd_uint8_t xd_task_suspend(struct xd_task* task)
{
	register xd_uint32_t level;
	level = xd_interrupt_disable();
	if((task->stat == XD_TASK_RUNNING) || (task->stat == XD_TASK_READY))
	{
	xd_list_remove(&(task->tlist));
	xd_task_ready_priority_group &= ~(task->number_mask);
	//xd_printf("priority group = %x\n" , xd_task_ready_priority_group);
	task->stat = XD_TASK_SUSPEND;
	}
	xd_interrupt_enable(level);
	return XD_EOK;
}

xd_uint8_t xd_task_resume(struct xd_task* task)
{
	register xd_uint32_t level;
	if(task->stat != XD_TASK_SUSPEND)
	{
		return XD_ERROR;
	}
	level = xd_interrupt_disable();
	//xd_list_remove(&(task->tlist));
	
	xd_scheduler_insert_task(task);
	xd_interrupt_enable(level);
	return XD_EOK;
}    

void xd_task_delay(xd_uint32_t tick)
{
	register xd_uint32_t temp;
	struct xd_task* task;
	
	temp = xd_interrupt_disable();
	task = xd_current_task;
	task->remaining_tick = tick;
	task->stat = XD_TASK_SUSPEND;
	xd_task_ready_priority_group &= ~(task->number_mask);
	xd_interrupt_enable(temp);
	xd_scheduler();
}
