/*------------------------------
 * Author: spaceman
 * date: 2021/3/1
 * version: first version
 *------------------------------*/

#ifndef __CPUPORT_H_
#define __CPUPORT_H_

#include <xd_k.h>

#if               /* ARMCC */ (  (defined ( __CC_ARM ) && defined ( __TARGET_FPU_VFP ))    \
                  /* Clang */ || (defined ( __CLANG_ARM ) && defined ( __VFP_FP__ ) && !defined(__SOFTFP__)) \
                  /* IAR */   || (defined ( __ICCARM__ ) && defined ( __ARMVFP__ ))        \
                  /* GNU */   || (defined ( __GNUC__ ) && defined ( __VFP_FP__ ) && !defined(__SOFTFP__)) )
#define USE_FPU   1
#else
#define USE_FPU   0
#endif

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
#if USE_FPU
    xd_uint32_t flag;
#endif /* USE_FPU */

    /* r4 ~ r11 register */
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

struct exception_stack_frame_fpu
{
    xd_uint32_t r0;
    xd_uint32_t r1;
    xd_uint32_t r2;
    xd_uint32_t r3;
    xd_uint32_t r12;
    xd_uint32_t lr;
    xd_uint32_t pc;
    xd_uint32_t psr;

#if USE_FPU
    /* FPU register */
    xd_uint32_t S0;
    xd_uint32_t S1;
    xd_uint32_t S2;
    xd_uint32_t S3;
    xd_uint32_t S4;
    xd_uint32_t S5;
    xd_uint32_t S6;
    xd_uint32_t S7;
    xd_uint32_t S8;
    xd_uint32_t S9;
    xd_uint32_t S10;
    xd_uint32_t S11;
    xd_uint32_t S12;
    xd_uint32_t S13;
    xd_uint32_t S14;
    xd_uint32_t S15;
    xd_uint32_t FPSCR;
    xd_uint32_t NO_NAME;
#endif
};

struct stack_frame_fpu
{
    xd_uint32_t flag;

    /* r4 ~ r11 register */
    xd_uint32_t r4;
    xd_uint32_t r5;
    xd_uint32_t r6;
    xd_uint32_t r7;
    xd_uint32_t r8;
    xd_uint32_t r9;
    xd_uint32_t r10;
    xd_uint32_t r11;

#if USE_FPU
    /* FPU register s16 ~ s31 */
    xd_uint32_t s16;
    xd_uint32_t s17;
    xd_uint32_t s18;
    xd_uint32_t s19;
    xd_uint32_t s20;
    xd_uint32_t s21;
    xd_uint32_t s22;
    xd_uint32_t s23;
    xd_uint32_t s24;
    xd_uint32_t s25;
    xd_uint32_t s26;
    xd_uint32_t s27;
    xd_uint32_t s28;
    xd_uint32_t s29;
    xd_uint32_t s30;
    xd_uint32_t s31;
#endif

    struct exception_stack_frame_fpu exception_stack_frame;
};





xd_uint8_t* xd_stack_init(void* entry,
                            void* parameter,
                            xd_uint8_t* stack_addr);

#endif
