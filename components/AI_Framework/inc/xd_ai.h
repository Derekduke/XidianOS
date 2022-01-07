#ifndef _XD_AI_H
#define _XD_AI_H
#include <xd_k.h>

#include <xd_ai_def.h>

xd_uint32_t xd_ai_register(xd_ai_t ai, const char *name, xd_uint16_t flags,int (*call)(void *arg), void *arg);
xd_uint32_t xd_ai_init(xd_ai_t ai, xd_ai_buffer_t* work_buf);
xd_uint32_t xd_ai_run(xd_ai_t ai , void *arg);
xd_ai_t  xd_ai_find(const char *name);
xd_uint32_t xd_ai_output(xd_ai_t ai , void *arg);
xd_uint32_t xd_ai_config(xd_ai_t ai, int cmd, xd_ai_buffer_t *arg);

#endif