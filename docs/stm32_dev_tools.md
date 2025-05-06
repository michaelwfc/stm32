# dev software
[STM32入门教程-2023版-软件安装](https://www.bilibili.com/video/BV1th411z7sn/?spm_id_from=333.788.videopod.episodes&vd_source=b3d4057adb36b9b243dc8d7a6fc41295&p=3)
## Install Keil5

1. 安装 Keil5 MDK : 
  install MDK524a.EXE on C:\Keil_v5

2. 安装器件支持包
  A. offline Install
  1. open Keil_v5.exe
  2. Keil.STM32F1xx_DFP.2.2.0.pack for STM32F103C8T6

  B.  online install
  1. open Keil_v5.exe
  2. pack install 

3. 软件注册
  1. open
  2. File > License Management >copy  Computer-ID
  3. open keygen_new2032.exe to generate License ID code
  4. cope key to Keil_v5 adn add LLC

4. 安装 STLINK 驱动
   install C:\Keil_v5\ARM\STLink\USBDriver\dpinst_amd64.exe

5. 安装USB转串口驱动
   install CH341SER.EXE from \STM32入门教程资料\工具软件\USB转串口CH340驱动

6. STM32F10x_StdPeriph_Lib_V3.5.0 标准库/固件库

7. install Arm compiler
  安装 Arm compiler from ARMCompiler_506_Windows_x86_b960: ARM_Compiler_5.06u7
  [Note: ARM compiler v5.0](https://blog.csdn.net/weixin_44807874/article/details/128627528)
the new Keil MDK-ARM 5.37+ default use ARM compiler v6.0+, not support ARM compiler v5.0, we need install ARM compiler v5.06: ARMCompiler_506_Windows_x86_b960.rar 
instal ARM compiler v5.06 to C:\Keil_v5\ARM\ARM_Compiler_5.06u7
setting Keil MDK-ARM 5.37+ compilers: Project mange > Folders/Extenstions > add another Arm compiler Version to list
choose ARM compiler v5
   
  


# 项目工程结构

- 建立工程文件夹，Keil中新建工程，选择型号 STM32F103C8
- 工程文件夹里建立Start、Library、User等文件夹，复制固件库里面的文件到工程文件夹
  - Start: STM32F10x 启动文件： STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm + Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x + Libraries\CMSIS\CM3\CoreSupport
  - Libraty: STM32F10x_StdPeriph_Lib_V3.5.0\Libraries\STM32F10x_StdPeriph_Driver\src + Libraries\STM32F10x_StdPeriph_Driver\inc
  - User:  STM32F10x_StdPeriph_Lib_V3.5.0\Project\STM32F10x_StdPeriph_Template

- 工程里对应建立Start、Library、User等同名称的分组 group，然后将文件夹内的文件添加到工程分组里
  - Start: startup_stm32f10x_md.s (MD表示: STM32F101/102/103 中容量产品 ),  *.c , *.h
- 工程选项，C/C++，Include Paths内声明所有包含头文件的文件夹
- 工程选项，C/C++，Define内定义 USE_STDPERIPH_DRIVER
- 工程选项，Debug，下拉列表选择对应调试器 ST-Lin/CMISS-DAP debugger，Settings，Debug: Max clock选择 50kHZ(频率过大可能导致调试连接频繁错误), Flash Download里勾选Reset and Run

- 工程选项，Target->  ARM compiler -> use defalut compiler version v5


# Debug

## Debug method

- 串口调试： 通过串口通信，将调试信息发送到电脑端，电脑使用串口助手显示调试信息
- 显示屏调试
- Keil调试模式

##  使用 DAP 仿真器作为开发板 调试器（代替 STLINK）
   Note:  DAP插上不亮，下载烧录时才亮的哦
   DAP仿真器 下载烧录 使用方法。
    1. 亲先看操作视频   
    2. 图片是 DAP仿真器连线图    
    3. 亲先看看这个视频 ，按照视频先操作一下(https://www.bilibili.com/video/BV19N411j7kA/?spm_id_from=333.999.0.0)
    
   
   第一步. 单独把DAP仿真器插到电脑USB接口上

    第二步.打开keil软件程序(随便选择一个程序）

    第三步  按照下面图片里面的5步骤操作一下，看keil软件是否可以识别到 "CMSIS-DAP"   

## Keil Debug

- compile 
- start debug session



# FlyMcu程序烧录软件

## 原理

-  Bootloader: 串口USRAT1 下载程序进行更新
-  Boot0=0 时 启动模式为 主闪存存储器， Boot0=1，Boot1=0时 为系统存储器 

## Steps

- 串口下载程序： objects/*.hex
- boot0 跳线设置为1
- USART1不断接收数据，刷新到主闪存 


# STLINK Utility


