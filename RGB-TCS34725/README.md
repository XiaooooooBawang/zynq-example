# RGB传感器 TCS34725——IIC驱动

- 只提供了SDK部分。TCS34725_parameters.h为寄存器地址参数头文件；main.c包含了IIC读写寄存器的函数以及传感器初始化函数。
- HDL部分使用ps的IIC0（按需使用，这里不做演示）。
- 使用EMIO自己约束SDA和SDL引脚即可，约束文件为pynq-z2.xdc，**注意要给引脚配置上拉电阻**。