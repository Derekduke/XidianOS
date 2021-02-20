/*------------------------------
 * Author: derekduke
 * date: 2021/2/19
 * version: first version
 *------------------------------*/

#include <xd_k.h>

xd_uint32_t xd_interrupt_from_task;
xd_uint32_t xd_interrupt_to_task;
xd_uint32_t xd_task_switch_interrupt_flag;

xd_uint8_t* xd_stack_init(void* entry,
                                void* parameter,
                                xd_uint8_t* stack_addr)
{
	struct stack_frame* stack_frame;
	xd_uint8_t* stk;
	unsigned long i;
	stk = stack_addr + sizeof(xd_uint32_t);
	stk = (xd_uint8_t*)ALIGN_DOWN((xd_uint32_t)stk , 8);
	stk -= sizeof(struct stack_frame);
	stack_frame = (struct stack_frame*)stk;
	
	for(i=0 ; i<sizeof(struct stack_frame)/sizeof(xd_uint32_t); i++)
	{
		((xd_uint32_t*)stack_frame)[i] = 0xdeadbeef;
	}
		
	stack_frame->exception_stack_frame.r0 = (unsigned long)parameter;
	stack_frame->exception_stack_frame.r1 = 0;
	stack_frame->exception_stack_frame.r2 = 0;
	stack_frame->exception_stack_frame.r3 = 0;
	stack_frame->exception_stack_frame.r12 = 0;
	stack_frame->exception_stack_frame.lr = 0;
	stack_frame->exception_stack_frame.pc = (unsigned long)entry;
	stack_frame->exception_stack_frame.psr = 0x01000000L;
		
	return stk;
}
