#include <xd_ai.h>
#include <xd_ai_core.h>

#define ai_init     (ai->init)
#define ai_input    (ai->get_input)
#define ai_run      (ai->run)
#define ai_output   (ai->get_output)
#define ai_get_info (ai->get_info)
#define ai_config   (ai->config)

xd_ai_t xd_ai_find(const char *name)
{
    if(name == XD_NULL) return XD_NULL;
    xd_ai_core_t core = XD_NULL;
    core = xd_ai_core_find(name, XD_AI_CLASS_HANDLE);
    if(core == XD_NULL){
        xd_kprintf("xd_ai_core_find return NULL %s,%d!\n",__FILE__,__LINE__);
        return XD_NULL;
    }
    return (xd_ai_t) core;
}

xd_uint32_t rt_ai_register(xd_ai_t ai, const char *name, int (*call)(void *arg), void *arg)
{
    if(ai == XD_NULL) return XD_ERROR;
    xd_kprintf("register model %s\n", name);
    if(xd_ai_core_register(&(ai->parent),XD_AI_CLASS_STATIC_HANDLE,name) == XD_NULL){
       xd_kprintf("rt_ai_core_register err!%s,%d\n",__FILE__,__LINE__);
    }
    return XD_EOK;
}

xd_uint32_t xd_ai_init(xd_ai_t ai, xd_uint8_t *work_buf)
{
    xd_uint32_t result = XD_EOK;
    ai->flag = 0;
    /* get ai_init handler */
    ai->workbuffer = work_buf;
    if (ai_init == XD_NULL)
    {
        xd_kprintf("ai init interface is None!\n");
        return XD_ERROR;
    }
    result = ai_init(ai, work_buf);
    if (result !=XD_EOK)
    {
        xd_kprintf("ai init interface return a err!\n", result);
        return result;
    }
    ai->flag |= XD_AI_FLAG_INITED;
    return XD_EOK;
}

xd_uint32_t xd_ai_run(xd_ai_t ai , void *arg)
{
    xd_uint32_t result = XD_EOK;
    /* if ai is not initialized, initialize it. */
    if (!(ai->flag & XD_AI_FLAG_INITED))
    {
        xd_kprintf("ai uninitialize!\n");
        return XD_ERROR;
    }
    /* call ai_close interface */
    if (ai_run ==  XD_NULL)
    {
        xd_kprintf("ai run interface is None!\n");
        return XD_ERROR;
    }
    result = ai_run(ai , arg);
    /* set open flag */
    if (result != XD_EOK)
    {
       xd_kprintf("ai run interface return a err!\n");
        return result;
    }
    ai->flag |= XD_AI_FLAG_RUN;
    ai->flag &= ~XD_AI_FLAG_OUTPUT;
    return XD_EOK;
}

xd_uint32_t xd_ai_output(xd_ai_t ai , void *arg)
{
    xd_uint32_t result = XD_EOK;
    /* if ai is not initialized, initialize it. */
    if (!(ai->flag & XD_AI_FLAG_INITED))
    {
        xd_kprintf("ai uninitialize!\n");
        return XD_ERROR;
    }
    /* call ai_close interface */
    if (ai_run ==  XD_NULL)
    {
        xd_kprintf("ai run interface is None!\n");
        return XD_ERROR;
    }
    result = ai_output(ai, arg);
    /* set open flag */
    if (result != XD_EOK)
    {
       xd_kprintf("ai run interface return a err!\n");
        return result;
    }
    ai->flag |= XD_AI_FLAG_RUN;
    ai->flag &= ~XD_AI_FLAG_OUTPUT;
    return XD_EOK;
}