
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

- 安装Keil5 MDK: 安装 Arm compiler: ARM_Compiler_5.06u7
- 安装器件支持包
- 软件注册
- 安装STLINK驱动
- 安装USB转串口驱动
- STM32F10x_StdPeriph_Lib_V3.5.0 标准库/固件库


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



