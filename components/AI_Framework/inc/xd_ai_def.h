#ifndef __XD_AI_DEF_H_
#define __XD_AI_DEF_H_

#include <xd_k.h>

#define XD_AI_FLAG_INITED   0x01
#define XD_AI_FLAG_LOADED   0X02
#define XD_AI_FLAG_RUN      0x04
#define XD_AI_FLAG_OUTPUT   0x08

#define ALLOC_WORK_BUFFER_FLAG      0X01
#define ALLOC_INPUT_BUFFER_FLAG     0X02
#define ALLOC_OUTPUT_BUFFER_FLAG    0X04
#define ALLOC_DEFAULT_BUFFER_FLAG   0xffff

#define XD_AI_T(h)                  ((xd_ai_t)(h))
#define XD_AI_INFO(h)               (((xd_ai_t)(h))->info)

#define XD_AI_LOG_ENABLE
/* log level & ref rt_ai_log.h */
#define AI_LOG_LEVEL 2
/* RTAK obj name lenght */
#define XD_AI_NAME_MAX 16
/* RTAK suport num of neural model input/output */
#define XD_AI_IO_MAX 10
/* enable print runtime performance (now suport run time cost only) */
#define ENABLE_PERFORMANCE 0
/* use struct rt_ai_record */
#define XD_AI_USE_RECORD 1

#define XD_AI_STATICIAL_TYPE(_type) (XD_AI_CLASS_STATIC | _type)
#define XD_AI_IS_STATICIAL(_type)   (XD_AI_CLASS_STATIC & _type)
#define XD_AI_MASK_STATICIAL(_type) ((~XD_AI_CLASS_STATIC) & _type)

/*
 * define object type info for the number of rt_ai_core_container items.
 */
enum xd_ai_obj_type
{
    XD_AI_CLASS_HANDLE = 0, /**< The type is a rt_ai. */
    XD_AI_CLASS_UNKNOWN, /**< The object is unknown. */
    XD_AI_CLASS_STATIC = 0X80, /** XD_AI STATICAL TYPE */
    XD_AI_CLASS_STATIC_HANDLE   = XD_AI_STATICIAL_TYPE(XD_AI_CLASS_HANDLE),
    XD_AI_CLASS_STATIC_UNKNOWN  = XD_AI_STATICIAL_TYPE(XD_AI_CLASS_UNKNOWN), /**< The type is a statical unknow. */
};

/**
 * Base structure of Kernel object
 */
typedef struct xd_ai_core *xd_ai_core_t; /**< Type for kernel objects. */
struct xd_ai_core
{
    char name[XD_AI_NAME_MAX]; /**< name of kernel object */
    xd_uint8_t type;           /**< type of kernel object */
    xd_uint8_t flag;           /**< flag of kernel object */
    xd_list_t  list;            /**< list node of kernel object */
};

typedef struct xd_ai_info *xd_ai_info_t;
struct xd_ai_info
{
    xd_uint32_t input_n;
    xd_uint32_t output_n;
    xd_uint32_t input_n_stack[XD_AI_IO_MAX];
    xd_uint32_t output_n_stack[XD_AI_IO_MAX];
    xd_uint32_t work_buffer_size;
    xd_uint16_t flag;
};

typedef struct xd_ai *xd_ai_t;
struct xd_ai
{
    struct xd_ai_core   parent;
    xd_uint16_t      flag; /**< ai flag */
    xd_uint16_t      mem_flag;
    xd_uint8_t      *workbuffer;           /** runtime buf */
    xd_uint8_t      *input[XD_AI_IO_MAX];  /* num of input */
    xd_uint8_t      *output[XD_AI_IO_MAX]; /* num of output */
    struct xd_ai_info   info;
    void                *cfg; /* platform config */

#ifdef RT_USING_AI_OPS
    const struct rt_ai_ops *ops;
#else
    /* common ai interface 
     *@return 0:OK; other:fail
    */
    int (*init)         (xd_ai_t ai, xd_uint8_t* buf);
    int (*get_input)    (xd_ai_t ai, xd_uint32_t index);
    int (*run)          (xd_ai_t ai, void* arg);
    int (*get_output)   (xd_ai_t ai, void* index);
    int (*get_info)     (xd_ai_t ai, xd_uint8_t *buf);
    int (*config)       (xd_ai_t ai, int cmd, xd_uint8_t *args);

#endif
    /* callback when run done */
    void (*done_callback_user)(void *arg);
    void *arg;
};

#endif