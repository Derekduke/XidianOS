/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#include <xd_k.h>

static xd_uint32_t xd_tick = 0;
extern xd_list_t xd_task_priority_table[XD_TASK_PRIORITY_MAX];
extern xd_uint32_t xd_task_ready_priority_group;
extern struct priority_list xd_task_priority_list[XD_TASK_PRIORITY_MAX];

static xd_list_t xd_timer_list[XD_TIMER_NUMBER];

xd_uint32_t xd_tick_get()
{
    return xd_tick;
}

void xd_tick_increase(void)
{
    struct xd_task *task;
    xd_tick++;
    task = get_current_task();
    if(task->init_tick > 0)
    {
        task->remaining_tick--;
        if(task->remaining_tick == 0)
        {
            task->remaining_tick = task->init_tick;
						task->stat = XD_TASK_READY;
            xd_task_yield();
        }
    }
    xd_timer_check();
}

void xd_system_timer_init(void)
{
    for(int i=0 ; i<sizeof(xd_timer_list)/sizeof(xd_timer_list[0]); i++)
    {
        xd_list_init(xd_timer_list+i);
    }
}

void xd_timer_init(timer_t timer,
                            void (*timeout)(void* parameter),
                            void* parameter,
                            xd_uint32_t time
                            )
{
    timer->timeout_func = timeout;
    timer->parameter = parameter;
    timer->timeout_tick = 0;
    timer->init_tick = time;
    for(int i=0 ; i< XD_TIMER_NUMBER ; i++)
    {
        xd_list_init(&(timer->node[i]));
    }
}

void xd_timer_set(timer_t timer , xd_uint32_t* tick)
{
    timer->init_tick = *tick;
}

void xd_timer_start(timer_t timer)
{
    unsigned int row_lvl = 0;
    xd_list_t* timer_list;
    register xd_uint32_t level;
    xd_list_t* node_head[XD_TIMER_NUMBER];
    timer->timeout_tick = xd_tick_get() + timer->init_tick;
    level = xd_interrupt_disable();
	timer_list = xd_timer_list;
    node_head[0] = &timer_list[0];
    for(row_lvl=0 ; row_lvl<XD_TIMER_NUMBER ; row_lvl++)
    {
        for(; node_head[row_lvl] != timer_list[row_lvl].prev; node_head[row_lvl]=node_head[row_lvl]->next)
        {
            timer_t t;
            xd_list_t* p = node_head[row_lvl]->next;
            t = xd_list_entry(p , struct timer , node[XD_TIMER_NUMBER-1]);
            if((t->timeout_tick-timer->timeout_tick)==0)
                continue;
            else if((t->timeout_tick-timer->timeout_tick)<XD_TICK_MAX/2)
                break;
        }
        if(row_lvl != XD_TIMER_NUMBER-1)
            node_head[row_lvl+1] = node_head[row_lvl]+1;
    }
    xd_list_insert_after(node_head[XD_TIMER_NUMBER-1] , &(timer->node[XD_TIMER_NUMBER-1]));
    xd_interrupt_enable(level);
}

void xd_timer_stop(timer_t timer)
{
    register xd_uint32_t level;
    level = xd_interrupt_disable();
    for(int i=0 ; i<XD_TIMER_NUMBER ; i++)
    {
        xd_list_remove(&timer->node[i]);
    }
    xd_interrupt_enable(level);
}

void xd_timer_check(void)
{
    struct timer* t;
    xd_uint32_t current_tick = xd_tick_get();
    register xd_uint32_t level;
    level = xd_interrupt_disable();
    while(xd_list_isempty(&xd_timer_list[XD_TIMER_NUMBER-1]) == 1)
    {
        t = xd_list_entry(xd_timer_list[XD_TIMER_NUMBER-1].next , struct timer , node[XD_TIMER_NUMBER-1]);
        if((current_tick - t->timeout_tick) < XD_TICK_MAX/2)
        {
            xd_timer_stop(t);
            t->timeout_func(t->parameter);
            current_tick = xd_tick_get();
        }
		else
		{
			break;
		}
    }
    xd_interrupt_enable(level);
}

