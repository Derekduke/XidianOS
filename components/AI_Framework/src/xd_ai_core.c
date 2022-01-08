#include <xd_ai_core.h>

struct xd_ai_core_information
{
    enum xd_ai_obj_type type; /**< object class type */
    xd_list_t object_list;    /**< object list */
    xd_uint32_t object_size;    /**< object size */
};

#define _XD_AI_CONTAINER_LIST_INIT(c)  { &(xd_ai_core_container[c].object_list), &(xd_ai_core_container[c].object_list) }
static struct xd_ai_core_information xd_ai_core_container[XD_AI_CLASS_UNKNOWN] =
{
    /* initialize object container - xd_ai_handle */
    {XD_AI_CLASS_HANDLE, _XD_AI_CONTAINER_LIST_INIT(XD_AI_CLASS_HANDLE), sizeof(struct xd_ai)},
};

struct xd_ai_core_information *xd_ai_core_get_information(enum xd_ai_obj_type type)
{
    int index;
    for (index = 0; index < XD_AI_CLASS_UNKNOWN; index++)
    {
        if (XD_AI_MASK_STATICIAL(xd_ai_core_container[index].type) == XD_AI_MASK_STATICIAL(type))
        {
            return &xd_ai_core_container[index];
        }
    }
    return XD_NULL;
}

xd_ai_core_t xd_ai_core_find(const char *name, xd_uint8_t type)
{
    struct xd_ai_core *object = XD_NULL;
    struct xd_task_node *node = XD_NULL;
    struct xd_ai_core_information *information = XD_NULL;

    information = xd_ai_core_get_information((enum xd_ai_obj_type)type);

    /* parameter check */
    if ((name == XD_NULL) || (information == XD_NULL)) return XD_NULL;

    /* try to find object */
    xd_list_for_each(node, &(information->object_list))
    {
        object = xd_list_entry(node, struct xd_ai_core, list);
        if (xd_strncmp(object->name, name, XD_AI_NAME_MAX) == 0)
        {
            return object;
        }
    }
    return XD_NULL;
}

xd_ai_core_t xd_ai_core_register(struct xd_ai_core *object, enum xd_ai_obj_type type, const char *name)
{
    struct xd_ai_core_information *information;
    /* get object information */
    information = xd_ai_core_get_information(type);
    if(information == XD_NULL) return XD_NULL;
    /* try to find object */
    if(xd_ai_core_find(name,type)){
        return XD_NULL;
    }    
    object->type |= type;
    /* copy name */
    xd_strncpy(object->name, name, XD_AI_NAME_MAX);
    /* insert object into information object list */
    xd_list_insert_after(&(information->object_list), &(object->list));
    return object;
}

void xd_ai_core_detach(xd_ai_core_t object)
{
    /* object check */
   if(object == XD_NULL) return;
    /* remove from old list */
    xd_list_remove(&(object->list));
}
