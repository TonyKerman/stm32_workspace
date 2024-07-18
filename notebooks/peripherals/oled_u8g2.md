# u8g2 多功能显示屏库

U8g2 是一个用于嵌入式设备的单色图形库。U8g2支持单色OLED和LCD，并支持如SSD1306等多种类型的OLED驱动。

[https://github.com/olikraus/u8g2](https://github.com/olikraus/u8g2)

## 移植步骤

下载U8g2的源码，因为STM32主要是使用C语言编程，所以只需关注源码中的C源码部分，即csrc文件夹下的文件。

### 精简c源码

U8g2支持多种显示驱动的屏幕，因为源码中也包含了各个驱动对应的文件，为了减小整个工程的代码体积，在移植U8g2时，可以删除一些无用的文件。

#### 去掉无用的驱动文件

这些驱动文件通常是u8x8_d_xxx.c，xxx包括驱动的型号和屏幕分辨率。ssd1306驱动芯片的OLED，使用u8x8_ssd1306_128x64_noname.c这个文件，其它的屏幕驱动和分辨率的文件可以删掉。

#### 精简u8g2_d_setup.c

由于我的OLED是IIC接口，只留一个本次要用到的u8g2_Setup_ssd1306_i2c_128x64_noname_f就好（如果是SPI接口，需要使用u8g2_Setup_ssd1306_128x64_noname_f这个函数），其它的可以删掉或注释掉。

#### 精简u8g2_d_memory.c

由于用到的u8g2_Setup_ssd1306_i2c_128x64_noname_f函数中，只调用了u8g2_m_16_8_f这个函数，所以留下这个函数，其它的函数一定要删掉或注释掉，否则编译时很可能会提示内存不足！！！

### 编写移植函数

#### GPIO初始化

用#define 对OLED用到的IIC/SPI接口进行GPIO的初始化配置

#### u8x8_gpio_and_delay

这个函数也需要自己写，主要的修改包括：

赋予U8g2相应的延时函数，比如下面的HAL_Delay_us
为U8g2提供IIC接口的高低电平调用:
U8X8_MSG_GPIO_I2C_CLOCK：IIC的SCL
U8X8_MSG_GPIO_I2C_DATA：IIC的SDA

注意：其中的HAL_Delay_us非HAL库自带函数，需要采用定时器自己编写该函数。

#### u8g2Init

U8g2的初始化，需要调用下面这个u8g2_Setup_ssd1306_128x64_noname_f函数，该函数的4个参数含义：

u8g2：传入的U8g2结构体
U8G2_R0：默认使用U8G2_R0即可（用于配置屏幕是否要旋转）
u8x8_byte_sw_i2c：使用软件IIC驱动，该函数由U8g2源码提供
u8x8_gpio_and_delay：就是上面我们写的配置函数

在main.c中加入以下代码：

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay);  // 初始化 u8g2 结构体
    u8g2_InitDisplay(u8g2); // 根据所选的芯片进行初始化工作，初始化完成后，显示器处于关闭状态
    u8g2_SetPowerSave(u8g2, 0); // 打开显示器
    u8g2_ClearBuffer(u8g2);