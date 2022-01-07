#ifndef __XD_BACKEND_PLUGIN_H__
#define __XD_BACKEND_PLUGIN_H__
#include <xd_k.h>

#include <xd_ai.h>

#ifdef XD_AI_USE_MICROINFER
#include <microinfer.h>

struct microinfer_backend
{
    struct rt_ai parent;
    uint8_t *model;
};
typedef struct microinfer_backend *microinfer_backend_t;

int microinfer_backend_init(void *microinfer_backend_s);
#endif //XD_AI_USE_MICROINFER

#endif