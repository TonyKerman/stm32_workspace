# 安装
## 1. 安装`Embedded IDE`插件
## 2. 安装`.net运行时 
```bash
sudo apt-get update && \
  sudo apt-get install -y aspnetcore-runtime-6.0
```
## 3. 安装工具链
```bash
sudo apt-get install openocd -y
```
下载 (gcc-arm-none-eabi)[https://developer.arm.com/downloads/-/gnu-rm]
```bash
#依赖libncurses-dev安装
sudo apt install libncurses-dev 
sudo ln -s /usr/lib/x86_64-linux-gnu/libncurses.so.6 /usr/lib/x86_64-linux-gnu/libncurses.so.5
sudo ln -s /usr/lib/x86_64-linux-gnu/libtinfo.so.6 /usr/lib/x86_64-linux-gnu/libtinfo.so.5

#gcc-arm-none-eabi解压安装                                                                                                                                                                                                                                                                                              
sudo tar xvf gcc-arm-none-eabi-xxx-x86_64-linux.tar.bz2 ~/
sudo ln -s ~/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-gcc /usr/bin/arm-none-eabi-gcc 
sudo ln -s ~/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-g++ /usr/bin/arm-none-eabi-g++
sudo ln -s ~/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-gdb /usr/bin/arm-none-eabi-gdb
sudo ln -s ~/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-size /usr/bin/arm-none-eabi-size
sudo ln -s ~/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-as /usr/bin/arm-none-eabi-as
sudo ln -s ~/gcc-arm-none-eabi-your-version/bin/arm-none-eabi-objcopy /usr/bin/arm-none-eabi-objcopy
```
##

# 导出eide模板

# clangd 插件
## 安装插件
## 配置clangd

在项目根目录中添加`compile_flags.txt`
### 方法1（存疑）
```
# 项目内的include 路径:参考Makefile:C includes
-I
Core/Inc
-I
Drivers/STM32F4xx_HAL_Driver/Inc 
-I
Drivers/STM32F4xx_HAL_Driver/Inc/Legacy 
-I
Drivers/CMSIS/Device/ST/STM32F4xx/Include 
-I
Drivers/CMSIS/Include 
-I
Middlewares/ST/ARM/DSP/Inc 
-I
Middlewares/Third_Party/FreeRTOS/Source/include 
-I
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 
-I
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
-I
UC
-I
mavlink_message
-I
mavlink_message/message_define
# 标准库的include路径,使用命令查询:arm-none-eabi-gcc -xc -E -v -
# 非stm32的标准库：echo | gcc -E -Wp,-v -
-I
/usr/lib/gcc/arm-none-eabi/10.3.1/include
-I
/usr/lib/gcc/arm-none-eabi/10.3.1/include-fixed
-I
/usr/lib/gcc/arm-none-eabi/10.3.1/../../../arm-none-eabi/include
# Macros,参考Makefile:C defines
-DUSE_HAL_DRIVER
-DSTM32F427xx
-DARM_MATH_CM4
-D__CC_ARM_
-D__FPU_PRESENT=1U
-D_TARGET_FPU_VFP

# Compiler options
-Og
-Wall
-fdata-sections
-ffunction-sections
```
其中：

### 方法2
1. 按`Shift+ctrl+p`选择`clangd:open config file`,编辑`.clang`文件
```
CompileFlags:
    Add:
        - "--include-directory=/home/tony/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/include"
        - "--include-directory=/home/tony/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/include-fixed"
        - "--include-directory=/home/tony/gcc-arm-none-eabi-10.3-2021.10/bin/../lib/gcc/arm-none-eabi/10.3.1/../../../../arm-none-eabi/include"

```
其中添加的是标准库的include路径地址，获取方法是：
* stm32使用命令查询:`arm-none-eabi-gcc -xc -E -v -`
* 非stm32的标准库：`echo | gcc -E -Wp,-v -`
2. 将`build/Debug/compile_commands.json`移动到`build`下
或者为项目增加一个构建后任务，在编译结束后自动复制 compiler_commands.json 到 build 下
* eide->构建配置->构建器选项->用户任务->构建后任务
```bash
bash -c "cp build/${ConfigName}/compile_commands.json build/compile_commands.json"
```
3. 重启vscode