# pwm-generator pwm发生器

- HDL文件用于顶层设计。本来是用AXI4自定义一个ip核的，但经过我自己的努力尝试，都未能解决AXI总线的接口报错问题，遂将其作为封装好的RTL使用。
- SDK文件用于SDK开发，提供了头文件和代码示例。

# 使用方式

1. 新建工程
2. Create Block Design，（我起名为example），添加zynq作为PS
3. 将HDL中的3个文件导入自己的Design Source中，然后将其点击拖动到右边的block design中会自动添加为RTL模块，再点击自动连线就得到下图的结果。

![image-20230510232154927](http://picture.xiaoooooobawang.xyz/image-20230510232154927.png)

4. Generate Output Product，最好使用Global能避免很多报错

![image-20230510232221013](http://picture.xiaoooooobawang.xyz/image-20230510232221013.png)

![image-20230510231738796](http://picture.xiaoooooobawang.xyz/image-20230510231738796.png)

6. Create HDL Wrapper 

![image-20230510232356986](http://picture.xiaoooooobawang.xyz/image-20230510232356986.png)

7. 将example_wrapper set as top

![image-20230510232508594](http://picture.xiaoooooobawang.xyz/image-20230510232508594.png)

8. 最终得到文件结构如下图

   ![image-20230510234106888](http://picture.xiaoooooobawang.xyz/image-20230510234106888.png)

9. 根据自己的需求约束引脚，生成bitstream，导出hardware，并勾选include bitstream，启动SDK就可开始SDK开发

![image-20230510234256191](http://picture.xiaoooooobawang.xyz/image-20230510234256191.png)

![image-20230510234312747](http://picture.xiaoooooobawang.xyz/image-20230510234312747.png)

