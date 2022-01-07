#include <xd_ai.h>

#define ai_init     (ai->init)
#define ai_input    (ai->get_input)
#define ai_run      (ai->run)
#define ai_output   (ai->get_output)
#define ai_get_info (ai->get_info)
#define ai_config   (ai->config)

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