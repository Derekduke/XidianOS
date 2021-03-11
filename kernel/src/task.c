/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#include <xd_k.h>

extern xd_list_t xd_task_priority_table[XD_TASK_PRIORITY_MAX];
extern struct xd_task* xd_current_task;
extern xd_uint32_t xd_task_ready_priority_group;
extern struct priority_list xd_task_priority_list[XD_TASK_PRIORITY_MAX];

xd_uint8_t xd_task_init(xd_uint8_t pid,
						struct xd_task* task,
                        void (*entry)(void* parameter),
                        void *parameter,
                        void *stack_start,
                        xd_uint32_t stack_size,
						xd_uint8_t priority,
						xd_uint32_t tick
						)
{
    xd_list_init(&(task->tlist));
    task->pid = pid;
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
	task->init_tick = tick;
	task->remaining_tick = tick;

	xd_timer_init(&(task->task_timer),xd_task_timeout , task , 0);
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
	xd_task_init( 31,
				&idle,
				xd_task_idle_entry,
				XD_NULL,
				&xd_task_stack[0],
				sizeof(xd_task_stack),
				XD_TASK_PRIORITY_MAX-1,
				0
                );
	xd_task_startup(&idle);
}

xd_uint8_t xd_task_startup(struct xd_task* task)
{
	task->current_priority = task->init_priority;
	task->number_mask = 1 << (task->current_priority);
	task->stat = XD_TASK_SUSPEND;
	xd_task_resume(task);
	return XD_EOK;
}

xd_uint8_t xd_task_suspend(struct xd_task* task)
{
	register xd_uint32_t level;
	level = xd_interrupt_disable();
	if((task->stat == XD_TASK_RUNNING) || (task->stat == XD_TASK_READY))
	{
		update_priority_list(task , TASK_SUSPEND);
	}
	task->stat = XD_TASK_SUSPEND;
	xd_scheduler_remove_task(task);
	if(task->init_tick > 0) xd_timer_stop(&(task->task_timer));
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
	task->stat = XD_TASK_READY;
	xd_list_insert_before(&(xd_task_priority_table[task->current_priority]) , &(task->tlist));
	update_priority_list(task , TASK_RESUME);
	xd_interrupt_enable(level);
	return XD_EOK;
}

void xd_task_delay(xd_uint32_t tick)
{
	register xd_uint32_t temp;
	struct xd_task* task;
	temp = xd_interrupt_disable();
	task = xd_current_task;
	xd_task_suspend(task);
	xd_timer_set(&(task->task_timer) , &tick);
	xd_timer_start(&(task->task_timer));
	task->stat = XD_TASK_SUSPEND;
	xd_interrupt_enable(temp);
	xd_scheduler();
}

struct xd_task* get_current_task()
{
	return xd_current_task;
}

void xd_task_yield(void)
{
    register xd_uint32_t level;
    struct xd_task* task;
    level = xd_interrupt_disable();
    task = get_current_task();
    if((task->stat == XD_TASK_RUNNING)&&(task->tlist.next != task->tlist.prev))
    {
        xd_list_remove(&(task->tlist));
        xd_list_insert_before(&(xd_task_priority_table[task->current_priority]) , &(task->tlist));  
		task->stat = XD_TASK_READY;
	}
    xd_interrupt_enable(level);
	xd_scheduler();
}

void xd_task_timeout(void* parameter)
{
    struct xd_task* task;
    task = (struct xd_task*)parameter;
    xd_scheduler_insert_task(task);
    xd_scheduler();
}
