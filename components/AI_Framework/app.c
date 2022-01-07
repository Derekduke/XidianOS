#include <xd_ai.h>

//这里以microinfer运行mnist为参考示例
#define XD_AI_USE_MICROINFER 1
#define XD_AI_MODEL_NAME "mnist"

#include <xd_backend_plugin.h>

static int8_t microinfer_input_data[1960];
static int8_t microinfer_output_data[10];
static uint8_t static_buff[1024*20];

static struct microinfer_backend xd_microinfer_backend_model;

static int xd_ai_network_model_init(){
    xd_ai_register(XD_AI_T(&xd_microinfer_backend_model), XD_AI_MODEL_NAME , microinfer_backend_init, &xd_microinfer_backend_model);
    xd_kprintf("ai register success\n");
    return 0;
}
const char codeLib[] = "@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.   ";
void print_img(int8_t * buf)
{
    for(int y = 0; y < 28; y++) 
	{
        for (int x = 0; x < 28; x++) 
		{
            int index =  69 / 127.0 * (127 - buf[y*28+x]); 
			if(index > 69) index =69;
			if(index < 0) index = 0;
            printf("%c",codeLib[index]);
			printf("%c",codeLib[index]);
        }
        printf("\n");
    }
}

int ai_app(void)
{
    uint32_t predic_label;
	print_img((int8_t*)&img[index][0]);
	memcpy(microinfer_input_data, (int8_t*)&img[index][0], 784);

    xd_ai_network_model_init(); //模型注册
    xd_uint32_t result = XD_EOK;
    
    model = xd_ai_find(XD_AI_MODEL_NAME); //模型查找
    if(model == XD_NULL){
        return -1;
    }
    xd_kprintf("find ai model success\n");
    result = rt_ai_init(model , &static_buff); //模型运行初始化
    if(result != 0){
        return -1;
    }
    rt_kprintf("init ai model success\n"); //模型运行
    result = rt_ai_run(model , NULL);
    if (result != 0) {
        rt_kprintf("ai model run err\r\n");
        return -1;
    }
    result = rt_ai_output(model , &predic_label); //模型输出
    rt_kprintf("run ai model success\n");
}