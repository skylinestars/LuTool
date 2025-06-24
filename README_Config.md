# LuTool Config 模式使用指南

## 概述

LuTool 现在支持 CMake Config 模式，这意味着您可以通过 `find_package(LuTool REQUIRED)` 来使用 LuTool 库，CMake 会自动处理所有依赖关系（包括 FMT 库）。

## 特性

- ✅ 支持 Debug 和 Release 配置
- ✅ 自动处理 FMT 依赖
- ✅ 跨项目使用友好
- ✅ 版本兼容性检查
- ✅ 多配置生成器支持（Visual Studio、Xcode 等）

## 构建和安装

### 1. 构建 Debug 版本

```powershell
# 使用提供的脚本
.\build_and_install.ps1 -Config Debug

# 或手动构建
mkdir build_debug
cd build_debug
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
cmake --install . --config Debug
```

### 2. 构建 Release 版本

```powershell
# 使用提供的脚本
.\build_and_install.ps1 -Config Release

# 或手动构建
mkdir build_release
cd build_release
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --config Release
```

## 在其他项目中使用

### 方法 1：使用 find_package（推荐）

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 LuTool 的安装路径
set(CMAKE_PREFIX_PATH "D:/Soft/LuTool;${CMAKE_PREFIX_PATH}")

# 查找 LuTool 包
find_package(LuTool REQUIRED)

# 创建可执行文件
add_executable(MyApp main.cpp)

# 链接 LuTool 库（自动处理所有依赖）
target_link_libraries(MyApp PUBLIC LuTool::LuTool)

# 包含头文件
target_include_directories(MyApp PUBLIC ${LuTool_INCLUDE_DIRS})
```

### 方法 2：使用传统方式（不推荐）

```cmake
# 手动设置路径和依赖
set(LUTOOL_LIB "D:/Soft/LuTool/lib/release/LuTool.lib")
set(LUTOOL_INCLUDE "D:/Soft/LuTool/include")

target_link_libraries(MyApp PUBLIC ${LUTOOL_LIB})
target_include_directories(MyApp PUBLIC ${LUTOOL_INCLUDE})

# 还需要手动处理 FMT 依赖
find_package(fmt REQUIRED)
target_link_libraries(MyApp PUBLIC fmt::fmt)
```

## 配置信息

使用 `find_package(LuTool REQUIRED)` 后，以下变量可用：

- `LuTool_FOUND`: 是否找到 LuTool
- `LuTool_VERSION`: LuTool 版本
- `LuTool_HAS_FMT`: 是否支持 FMT 格式化
- `LuTool_INCLUDE_DIRS`: 头文件目录
- `LuTool_LIBRARIES`: 库文件列表
- `LuTool_BUILD_TYPE`: 构建类型

## 测试 Config 模式

运行提供的测试脚本：

```powershell
.\test_config.ps1
```

这将创建一个测试项目，验证 Config 模式是否正常工作。

## 安装结构

安装后的目录结构：

```
D:/Soft/LuTool/
├── bin/
│   ├── LuTool.dll (Debug)
│   └── LuTool.dll (Release)
├── lib/
│   ├── LuTool.lib (Debug)
│   └── LuTool.lib (Release)
├── include/
│   └── LuTool/
│       ├── Common/
│       ├── IO/
│       └── Str/
└── lib/cmake/LuTool/
    ├── LuToolConfig.cmake
    ├── LuToolConfigVersion.cmake
    └── LuToolTargets.cmake
```

## 优势

1. **自动依赖管理**: 不需要手动处理 FMT 库
2. **版本兼容性**: CMake 自动检查版本兼容性
3. **跨平台**: 支持 Windows、Linux、macOS
4. **多配置**: 支持 Debug、Release 等不同配置
5. **IDE 集成**: 与 Visual Studio、CLion 等 IDE 完美集成

## 注意事项

1. 确保 LuTool 已正确安装到指定路径
2. 如果 LuTool 是用 FMT 编译的，确保 FMT 库在系统中可用
3. 使用 `CMAKE_PREFIX_PATH` 或 `LuTool_DIR` 来指定 LuTool 的安装路径 