#include <xd_backend_plugin.h>

#ifdef RT_AI_USE_microinfer

static int _microinfer_init(xd_ai_t ai, xd_uint8_t *static_buf)
{
    microinfer_set_buf(static_buff , sizeof(static_buff)/sizeof(xd_uint8_t));
    ai->arg = microinfer_model_create();
    return 0;
}

static int _microinfer_run(xd_ai_t ai , void *arg)
{
    model_run((microinfer_model_t*)ai->arg);
    return 0;
}

static int _microinfer_get_ouput(xd_ai_t ai, xd_uint32_t* index)
{
    microinfer_output((microinfer_model_t*)ai->arg, label);
    return 0;
}

static int _microinfer_get_info(xd_ai_t ai, xd_uint8_t *buf)
{
    return 0;
}

static int _microinfer_config(xd_ai_t ai, int cmd, xd_uint8_t *args)
{
    return 0;
}

int microinfer_backend_init(void *microinfer_s)
{
    XD_AI_T(microinfer_s)->init =  _microinfer_init;
    XD_AI_T(microinfer_s)->run =  _microinfer_run;
    XD_AI_T(microinfer_s)->get_output = _microinfer_get_ouput;
    XD_AI_T(microinfer_s)->get_info = _microinfer_get_info;
    XD_AI_T(microinfer_s)->config =  _microinfer_config;
    XD_AI_T(microinfer_s)->mem_flag = ALLOC_INPUT_BUFFER_FLAG;

    return 0;
}

#endif //RT_AI_USE_imx6ull