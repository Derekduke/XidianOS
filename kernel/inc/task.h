/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#ifndef __TASK_H_
#define __TASK_H_

#include <xd_def.h>

struct xd_task
{
    void* sp;
    void* entry;
    void* parameter;
    void* stack_addr;

    xd_uint32_t stack_size;

    xd_uint8_t num;
    xd_uint8_t stat;
    xd_uint8_t error;
    xd_uint8_t current_priority;
    xd_uint8_t init_priority;
    xd_uint32_t number_mask;

    xd_list_t tlist;
    xd_uint32_t remaining_tick;
};

xd_uint8_t xd_task_init(xd_uint8_t num,
						struct xd_task* task,
                        void (*entry)(void* parameter),
                        void *parameter,
                        void *stack_start,
                        xd_uint32_t stack_size,
						xd_uint8_t priority
						);
void xd_task_idle_entry(void* parameter);
void xd_task_idle_init(void);
xd_uint8_t xd_task_startup(struct xd_task* task);
xd_uint8_t xd_task_startup(struct xd_task* task);
xd_uint8_t xd_task_resume(struct xd_task* task);
xd_uint8_t xd_task_suspend(struct xd_task* task);
void xd_task_delay(xd_uint32_t tick);
												
#endif
