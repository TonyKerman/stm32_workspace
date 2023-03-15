#ZS_040 (HC-06)
两个蓝牙模块配对连接后可以作无线串口
##接线
###蓝牙A:
和usb转ttl连接,把usb转ttl跳线连到3.3V

        5V  <---------> 5V
    TTL TXD <---------> RXD   HC-06
        RXD <---------> TXD
        GND <---------> GND
###蓝牙B:
和STM32 UART1连接

##AT设置:
连接串口后,打开串口监视器进行设置
A
1.调节波特率9600,无结束符
2.连接输入 AT 回复OK
3.AT+BUAD4
4.AT+PINxxxx(四位密码)
5.AT+ROLE=M

B 相同,除了5 AT+ROLE=S

##指令

#### 设置/查询设备名称
    
    发送：AT+NAMEname
    响应：OKname
#### 设置模块角色

    发送：AT+ROLE=M（设置为主模式Master）

    响应：OK+ROLE：M

    发送：AT+ROLE=S（设置为主模式Slave）

    响应：OK+ROLE：S
#### 设置/查询-配对码

    发送：AT+PINxxx

    响应：OKsetpin
#### 设置／查询串口参数

    发送：AT+BAUD（1、2、3、4）

    响应：OK