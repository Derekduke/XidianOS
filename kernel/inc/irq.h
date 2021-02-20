/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#ifndef __IRQ_H_
#define __IRQ_H_

#include <xd_k.h>

xd_uint32_t xd_interrupt_disable(void);
void xd_interrupt_enable(xd_uint32_t level);
void xd_interrupt_enter(void);
void xd_interrupt_leave(void);

#endif
