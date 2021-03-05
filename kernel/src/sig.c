#include <xd_k.h>
#include <sig.h>

signal_def signal_table[SIGNAL_MAX_NUMBER] = {0};

void signal_init(struct xd_task* task , xd_uint32_t value , signal_def sig)
{
    sig->task = task;
    sig->value = value;
    sig->update = 0;
    sig->used = 1;
    signal_table[0] = sig;
}

xd_uint8_t signal_handler()
{
    signal_def sig = signal_table[0];
    xd_uint32_t level;
    level = xd_interrupt_disable();
    if(sig && sig->task && (sig->task->stat == XD_TASK_SUSPEND) && (sig->update>0))
    {
        sig->update = 0;
        xd_task_resume(sig->task);
        xd_interrupt_enable(level);
        xd_scheduler();
        return 1;
    }
    else
    {
        xd_interrupt_enable(level);
        return 0;
    }
}

