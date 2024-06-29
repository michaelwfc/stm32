# 红外遥控

红外遥控是利用红外光进行通信的设备，由红外LED将调制后的信号发出，由专用的红外接收头进行解调输出

- 通信方式：单工，异步
- 红外LED波长：940nm
- 通信协议标准：NEC标准

# 基本发送与接收

- 空闲状态：红外LED不亮，接收头输出高电平
- 发送低电平：红外LED以38KHz频率闪烁发光，接收头输出低电平
- 发送高电平：红外LED不亮，接收头输出高电平


# 红外接收头

- VCC
- GND
- OUTPUT: PA8

## NCE 编码
### Start : 9ms + 4.5ms
### Data 

- Address (8 bit)
- ^Address (8 bit)
- Command (8 bit)
- ^Command (8 bit)

logical 0 : 560 us(Low) + 560 us(High)  
logical 1 : 560 us(Low) + 1690 us(High)  
低位在前，高位在后
  
### Repeat: 9ms + 2.25ms