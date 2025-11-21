# CLion + STM32CubeMX + STM32CubeCLT 开发环境配置指南

## 环境准备
### 所需软件
- **CLion** - JetBrains C/C++ IDE
- **STM32CubeMX** - STM32配置工具
- **STM32CubeCLT** - STM32开发工具链
- **OpenOCD** - 片上调试工具

## 环境配置步骤

### 1. 配置嵌入式开发路径
进入 **设置 > 构建、执行、部署 > 嵌入式开发**，配置各软件安装位置。

### 2. 配置工具链
1. 进入 **设置 > 构建、执行、部署 > 工具链**
2. 点击 **+** 添加 **MinGW** 工具链，命名为 `STM32`
3. 配置以下路径（指向STM32CubeCLT安装目录）：

| 组件 | 路径 |
|------|------|
| **CMake** | `STM32CubeCLT根目录/CMake/bin/cmake.exe` |
| **构建工具** | `STM32CubeCLT根目录/Ninja/bin/Ninja.exe` |
| **C编译器** | `STM32CubeCLT根目录/GNU-tools-for-STM32/bin/arm-none-eabi-gcc.exe` |
| **C++编译器** | `STM32CubeCLT根目录/GNU-tools-for-STM32/bin/arm-none-eabi-g++.exe` |
| **调试器** | `STM32CubeCLT根目录/GNU-tools-for-STM32/bin/arm-none-eabi-gdb.exe` |

**工具集**保持默认设置即可。

## 项目创建流程

### 1. 使用STM32CubeMX创建项目
- 新建包含`.ioc`文件的STM32项目
- **关键步骤**：生成代码时选择 **CMake** 作为工具链

### 2. 在CLion中导入项目
1. 选择 **新建项目 > STM32CubeMX**
2. 确保STM32CubeMX和STM32CubeCLT状态显示为 **已找到**
3. **项目位置**选择刚才用STM32CubeMX创建的项目目录

### 3. 项目配置
- **工具链**：选择前面创建的`STM32`工具链
- **CMake选项**：添加 `-DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake`

## ST-Link调试配置

如果CLion工具栏中没有显示ST-Link选项，按以下步骤配置：

1. 进入 **设置 > 构建、执行、部署 > 调试器 > 调试服务器**
2. 添加新的调试服务器，选择 **ST-Link**
3. 配置路径：
   - **可执行文件**：`STM32CubeCLT根目录\STLink-gdb-server\bin\ST-LINK_gdbserver.exe`
   - **编程器**：`STM32CubeCLT根目录\STM32CubeProgrammer\bin`

## 验证配置
完成上述配置后，在CLion工具栏中选择对应的配置，点击**构建**按钮（小锤子图标），如能成功构建项目，则说明环境配置正确。

---

**提示**：确保所有软件版本兼容，建议使用官方推荐的最新稳定版本。