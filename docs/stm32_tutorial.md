
# 开发板 

## 芯片 STM32F103C8T6

STM32是ST公司基于ARM Cortex-M内核开发的32位微控制器

- 系列：主流系列STM32F1
- 内核：ARM Cortex-M3
- 主频 72M
- 爱普生 32768hz 晶振
- ROM 64K （Flash）
- RAM: 20k
- TQFP48 封装

## CMSIS-DAP 仿真器


# dev software

## Install Keil5

- 安装Keil5 MDK: 安装 Arm compiler: ARM_Compiler_5.06u7
- 安装器件支持包
- 软件注册
- 安装STLINK驱动
- 安装USB转串口驱动
- STM32F10x_StdPeriph_Lib_V3.5.0 标准库/固件库

## Debug method
 - 串口调试： 通过串口通信，将调试信息发送到电脑端，电脑使用串口助手显示调试信息
 - 显示屏调试
 - Keil调试模式

## Keil Debug

- compile 
- start debug session
- 



# 项目工程结构

- 建立工程文件夹，Keil中新建工程，选择型号 STM32F103C8
- 工程文件夹里建立Start、Library、User等文件夹，复制固件库里面的文件到工程文件夹
  - start: STM32F10x 启动文件： STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm + Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x + Libraries\CMSIS\CM3\CoreSupport
  - libraty: STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\STM32F10x_StdPeriph_Driver\src + Libraries\STM32F10x_StdPeriph_Driver\inc
  - user:  STM32F10x_StdPeriph_Lib_V3.5.0\Project\STM32F10x_StdPeriph_Template

- 工程里对应建立Start、Library、User等同名称的分组 group，然后将文件夹内的文件添加到工程分组里
  - start: startup_stm32f10x_md.s (MD表示: STM32F101/102/103 中容量产品 ),  *.c , *.h
- 工程选项，C/C++，Include Paths内声明所有包含头文件的文件夹
- 工程选项，C/C++，Define内定义 USE_STDPERIPH_DRIVER
- 工程选项，Debug，下拉列表选择对应调试器 ST-Lin/CMISS-DAP debugger，Settings，Flash Download里勾选Reset and Run

[Note: ARM compiler v5.0](https://blog.csdn.net/weixin_44807874/article/details/128627528)
the new Keil MDK-ARM 5.37+ default use ARM compiler v6.0+, not support ARM compiler v5.0, we need install ARM compiler v5.06: ARMCompiler_506_Windows_x86_b960.rar 
instal ARM compiler v5.06 to C:\Keil_v5\ARM\ARM_Compiler_5.06u7
setting Keil MDK-ARM 5.37+ compilers: Project mange > Folders/Extenstions > add another Arm compiler Version to list
choose ARM compiler v5
- 工程选项，Target->  ARM compiler -> use defalut compiler version v5


# Teams


# GPIO（General Purpose Input Output/IO）

每个GPI/O端口有

- 两个32位配置寄存器(GPIOx_CRL，GPIOx_CRH)
- 两个32位数据寄存器(GPIOx_IDR和GPIOx_ODR）
- 一个32位置位/复位寄存器(GPIOx_BSRR)
- 一个16位复位寄存器(GPIOx_BRR)
- 一个32位锁定寄存器(GPIOx_LCKR)。

## GPIO 输出

- 推挽输出
- 开漏输出
  上拉电阻： 上拉电阻和单片机内的内阻并联之后减少阻抗，从而提高输出端的电压，驱动能力增强

- 复用 推挽输出
- 复用 开漏输出

## GPIO 输入

- 输入浮空 
- 输入上拉 
- 输入下拉 
- 模拟输入



# 中断系统

中断：在主程序运行过程中，出现了特定的中断触发条件（中断源），使得CPU暂停当前正在运行的程序，转而去处理中断程序，处理完成后又返回原来被暂停的位置继续运行

中断优先级：当有多个中断源同时申请中断时，CPU会根据中断源的轻重缓急进行裁决，优先响应更加紧急的中断源

中断嵌套：当一个中断程序正在运行时，又有新的更高优先级的中断源申请中断，CPU再次暂停当前中断程序，转而去处理新的中断程序，处理完成后依次进行返回




## EXTI ： （Extern Interrupt）外部中断

EXTI可以监测指定GPIO口的电平信号，当其指定的GPIO口产生电平变化时，EXTI将立即向NVIC发出中断申请，经过NVIC裁决后即可中断CPU主程序，使CPU执行EXTI对应的中断程序
- 支持的触发方式：上升沿/下降沿/双边沿/软件触发
- 支持的GPIO口：所有GPIO口，但相同的Pin不能同时触发中断
- 通道数：16个GPIO_Pin，外加PVD输出、RTC闹钟、USB唤醒、以太网唤醒
- 触发响应方式：中断响应/事件响应

### EXTI基本结构

GPICO -> AFIO 中断引脚选择 ->EXTI 边沿检测及控制 > NVIC

- 68个可屏蔽中断通道，包含EXTI、TIM、ADC、USART、SPI、I2C、RTC等多个外设
- AFIO ： 在STM32中，AFIO主要完成两个任务：复用功能引脚重映射、中断引脚选择
- NVIC：嵌套中断向量控制器， 使用 NVIC 统一管理中断，每个中断通道都拥有16个可编程的优先等级，可对优先级进行分组，进一步设置抢占优先级和响应优先级


## Timer Interrupt 定时中断



# FlyMcu程序烧录软件
## 原理
-  Bootloader: 串口USRAT1 下载程序进行更新
-  Boot0=0 时 启动模式为 主闪存存储器， Boot0=1，Boot1=0时 为系统存储器 

## Steps
- 串口下载程序： objects/*.hex
- boot0 跳线设置为1
- USART1不断接收数据，刷新到主闪存 


# STLINK Utility

