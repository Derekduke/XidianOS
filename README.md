# XidianOS
A streamlined real-time operating system.

西电OS操作系统是应用于微处理器上的一个小巧玲珑的操作系统，它可以根据具体的应用场景以去定制化功能，具有较强的可拓展性和可移植性。

## 体验 XidianOS
### 基于STM32F103
打开XidianOS->board->ALIENTEK_STM32F103_NANO/keil下的工程

编译下载程序至开发板中,可以看到两个LED灯以不同频率交替闪烁：
![](./pic/stm32f103nano.jpg)

并再观察串口助手（波特率15200），可以运行shell命令：
![](pic/shell.png)

## 内核功能
初步计划支持以下功能：

## 架构支持
初步计划支持以下架构：
- [x] cortex-m3 
- [ ] cortex-m4
- [x] cortex-m7
- [ ] cortex-a (MMU)
## 组件支持
初步计划实现以下组件：
- [x] shell
- [x] letter-shell
- [x] tensorflow
- [ ] lvgl

## 移植指南

### 对于Keil工程
* 使用Cubemx工具，针对目标处理器和外设生成keil工程
* 添加kernel目录下的.c文件及对应arch下的.c和.s文件，并指定编译所需的头文件路径
* 自行实现uart.c中的串口打印函数以及中断处理函数
* 在stm32xxx_it.c中在"void PendSV_Handler(void)"前添加 “__weak” 
* 根据项目需要添加components下的组件