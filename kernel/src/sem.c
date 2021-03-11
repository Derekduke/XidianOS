#include <xd_k.h>
#include <string.h>
#include <stdlib.h>

xd_sem_t xd_sem_create(char* name , xd_int32_t value)
{
    xd_sem_t sem = (xd_sem_t)malloc(sizeof(struct semaphore));
    strcpy(sem->name , name);
    sem->value = value;
		xd_list_init(&(sem->sem_wait_task));	
    return sem;
}

void xd_sem_init(xd_sem_t sem , char* name , xd_int32_t value)
{
    strcpy(sem->name , name);
    sem->value = value;  
		xd_list_init(&(sem->sem_wait_task));	
}

void xd_sem_delete(xd_sem_t sem)
{
    free(sem);
}

void xd_sem_take(xd_sem_t sem , struct xd_task* task)
{
    while(sem->value <= 0)
    {
        xd_task_suspend(task);
        xd_list_insert_before(&(sem->sem_wait_task) , &(task->tlist));
				task->stat = XD_TASK_SUSPEND;
        xd_scheduler();
    }
		sem->value --;
}

void xd_sem_release(xd_sem_t sem)
{   
    sem->value++;
		struct xd_task* resume_task = xd_list_entry(sem->sem_wait_task.next , struct xd_task , tlist);
		xd_list_remove(sem->sem_wait_task.next);
    xd_task_resume(resume_task);
    xd_scheduler();
}
