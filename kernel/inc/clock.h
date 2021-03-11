/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#ifndef __CLOCK_H_
#define __CLOCK_H_

#include <xd_def.h>

struct timer
{
    xd_list_t node[XD_TIMER_NUMBER]; 
    void (*timeout_func)(void* parameter);
    void *parameter;
    xd_uint32_t init_tick;
    xd_uint32_t timeout_tick;
};

typedef struct timer* timer_t;

xd_uint32_t xd_tick_get(void);
void xd_tick_increase(void);
void xd_system_timer_init(void);
void xd_timer_init(timer_t timer,
                            void (*timeout)(void* parameter),
                            void* parameter,
                            xd_uint32_t time);
void xd_timer_set(timer_t timer , xd_uint32_t* tick);
void xd_timer_start(timer_t timer);
void xd_timer_stop(timer_t timer);
void xd_timer_check(void);
void ms_delay(xd_uint32_t time);

#endif
