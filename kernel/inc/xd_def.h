/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#ifndef _XD_DEF_H_
#define _XD_DEF_H_

/*config*/
#define XD_ALIGN_SIZE 4
#define XD_TASK_PRIORITY_MAX 32
#define XD_TICK_PER_SECOND 100

/*type*/
typedef signed char         xd_int8_t;
typedef signed short        xd_int16_t;
typedef signed long         xd_int32_t;
typedef unsigned char       xd_uint8_t;
typedef unsigned short      xd_uint16_t;
typedef unsigned long       xd_uint32_t;

#define XD_TRUE             1
#define XD_FALSE            0
#define XD_EOK              0
#define XD_ERROR            1
#define XD_NULL             0

#define xd_inline           static __inline
	
#ifdef __CC_ARM
#define cc_inline static __inline
#define ALIGN(n)    __attribute__((aligned(n)))
#define ALIGN_DOWN(addr, size)  ((addr)&(~((size)-1)))
#endif

#define XD_TASK_INIT        0x00
#define XD_TASK_READY	      0x01
#define XD_TASK_SUSPEND     0x02
#define XD_TASK_RUNNING     0x03
#define XD_TASK_CLOSE	      0x04

struct xd_task_node
{
    struct xd_task_node* next;
    struct xd_task_node* prev;
};
typedef struct xd_task_node xd_list_t;

#endif
