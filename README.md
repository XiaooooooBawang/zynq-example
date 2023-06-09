# zynq-example
Zynq 常用（PWM、UART、I2C、传感器等等）示例——西电 通院教改 “片上微控制系统原理与项目设计”课程特供

（该repo的一些README.md里的图片在GitHub里看不见，可以clone或者download来看）

## 动机

众所周知，通院教改喜欢课程上有所“创新”，该课程也不例外，只发pynq-z2的板子让我们用SoC做实验和项目设计，连stm32都不发。网上（国内国外）关于zynq的资料要比stm32少非常多，因为很少人会用FPGA去做控制，都是拿stm32做的，像我这种穷人又不可能买块stm32，只能硬着头皮自己写各种驱动。技术积累较少的同学直接接触SoC是有些难度的，故将我自己写的代码push上来，给这一“小众领域”提供一些资料。

## Tips

- pynq-z2可以理解成可用python开发的zynq 7020，但电子领域的开发主流都是用C/C++来做的，如果把精力用在python开发上就很没有性价比了，所以我们大可不必理会“p”，只需关注zynq即可。网上关于zynq的资料要比pynq丰富很多，所以我们在找资料的时候搜zynq7000就能满足我们的需求，在pynq上是完全适用的。
- 该课程其实分成了2门课——”数电“和“项目设计”。在做数电的课程实验时，绝大部分只需使用PL部分（写Verilog）；而在做项目设计的任务时我非常建议用SoC做（即用PS部分），使用SDK的C语言开发会极大降低开发难度，相应的开发流程在这一篇blog中讲得很清楚，[博客点这里](https://blog.csdn.net/qq_45467083/article/details/121384967)。
- 当我们在开发zynq的时候可以借鉴stm32或者arduino的示例代码，具体的流程基本不变，只是API不一样，我们将他们的API“翻译”成zynq的API即可。
- 老师能提供的技术支持约等于零，不如多看数据手册，多看网上的示例，多问班上的大佬。

