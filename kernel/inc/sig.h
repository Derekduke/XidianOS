#ifndef __SIG_H_
#define __SIG_H_

#include <xd_def.h>
#define SIGNAL_MAX_NUMBER 128

struct Signal
{
    xd_uint32_t value;
    xd_uint8_t update;
    xd_uint8_t used;
    struct xd_task* task;
};
typedef struct Signal* signal_def;

void signal_init(struct xd_task* task , xd_uint32_t value , signal_def sig);
xd_uint8_t signal_handler(void);

#endif
