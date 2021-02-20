/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <xd_def.h>
#include <task.h>

#define xd_container_of(ptr , type , member) \
	((type*)((char*)(ptr)-(unsigned long)(&((type*)0)->member)))

#define xd_list_entry(node , type , member) \
	xd_container_of(node , type , member)
	
void xd_system_scheduler_init(void);
void xd_system_scheduler_start(void);
void xd_scheduler(void);
xd_uint8_t xd_find_task(xd_uint32_t val);
void xd_scheduler_insert_task(struct xd_task* task);
void xd_scheduler_remove_task(struct xd_task* task);	
void xd_context_switch(xd_uint32_t from, xd_uint32_t to);
void xd_context_switch_to(xd_uint32_t to);
xd_uint8_t kernel_running(void);

#endif	
