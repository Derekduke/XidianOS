#ifndef __SEM_H_
#define __SEM_H_

#include <xd_def.h>

struct semaphore
{
    char name[10];
    xd_int32_t value;
    xd_list_t sem_wait_task;
};
typedef struct semaphore* xd_sem_t;

xd_sem_t xd_sem_create(char* name , xd_int32_t value);
void xd_sem_init(xd_sem_t sem , char* name , xd_int32_t value);
void xd_sem_delete(xd_sem_t sem);
void xd_sem_take(xd_sem_t sem , struct xd_task* task);
void xd_sem_release(xd_sem_t sem);

#endif
