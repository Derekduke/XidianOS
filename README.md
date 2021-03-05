<!--
 * @Author: spaceman
 * @Date: 2021-03-01 17:08:09
 * @LastEditTime: 2021-03-05 18:10:19
 * @LastEditors: spaceman
 * @Description:
 * @FilePath: \XidianOS\README.md
-->
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

### 基于STM32H750

![WeAct_stm32h750](./pic/WeAct_stm32h750.jpg)

打开XidianOS->board->WEACT_STM32H750/keil下的工程，编译下载程序至开发板中,可以看到LED灯一直在闪烁，再观察串口助手（波特率15200），可以运行shell命令：

![lettershell](pic/lettershell.png)

## 内核功能
初步计划支持以下功能：
- [x] 信号量
- [ ] 互斥锁
- [ ] 消息队列
- [ ] 邮箱
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