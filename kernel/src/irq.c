/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#include <irq.h>

volatile xd_uint8_t xd_interrupt_nest;

void xd_interrupt_enter(void)
{
    xd_uint32_t level;
    level = xd_interrupt_disable();
    xd_interrupt_nest ++;
    xd_interrupt_enable(level);
}

void xd_interrupt_leave(void)
{
    xd_uint32_t level;
    level = xd_interrupt_disable();
    xd_interrupt_nest --;
    xd_interrupt_enable(level);
}
