# Zynq Uart

- 这里的uart是用来zynq与上位机或其他单片机（如esp32）进行通信，使用zynq的ps的uart1通信，使用ps uart0进行printf的串口打印调试。
- 如果想每次只收发一个字节使用XUartPs_SendByte()和XUartPs_RecvByte()；如果想每次收发多个字节（一个数组）使用XUartPs_Recv()和XUartPs_Send()，代码在注释中，按需选择。
- uart1的EMIO引脚自行约束，注意与单片机的TX和RX相对连接。
- **需要注意的是，Vivado SDK有bug，内置的串口调试窗口不能实现发送消息，建议使用另外的串口调试软件（如串口调试助手）；除此之外，Program FPGA调试会经常出现问题，但代码是完全没问题的，需要固化到板子上就不会有问题了。**