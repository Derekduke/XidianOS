/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#ifndef __CPUPORT_H_
#define __CPUPORT_H_

#include <xd_k.h>

struct exception_stack_frame
{
    xd_uint32_t r0;
    xd_uint32_t r1;
    xd_uint32_t r2;
    xd_uint32_t r3;
    xd_uint32_t r12;
    xd_uint32_t lr;
    xd_uint32_t pc;
    xd_uint32_t psr;
};

struct stack_frame
{
    xd_uint32_t r4;
    xd_uint32_t r5;
    xd_uint32_t r6;
    xd_uint32_t r7;
    xd_uint32_t r8;
    xd_uint32_t r9;
    xd_uint32_t r10;
    xd_uint32_t r11;

    struct exception_stack_frame exception_stack_frame;
};

xd_uint8_t* xd_stack_init(void* entry,
                            void* parameter,
                            xd_uint8_t* stack_addr);

#endif
