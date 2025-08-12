# LuTool

一个C++工具库，提供字符串处理、文件操作和日志记录功能。

## 功能特性

### 日志系统 (Logger)
支持格式化字符串的日志记录系统，提供同步和异步两种日志方式。

#### 基本使用

```cpp
#include "Source/IO/Logger.h"

// 设置日志文件
LuTool::Logger::getInstance().setOutputFile("app.log");

// 设置日志级别
LuTool::Logger::getInstance().setLogLevel(LuTool::LogLevel::LOG_DEBUG);

// 使用格式化字符串记录日志
int edgeCount = 100;
int faceCount = 50;
int shellCount = 10;

// 同步日志
LOG_INFO("实体内容统计 - 边: {}, 面: {}, 壳体: {}", edgeCount, faceCount, shellCount);
LOG_DEBUG("调试信息 - 当前处理进度: {:.2f}%", 75.5);
LOG_WARN("警告信息 - 发现 {} 个重复项", 5);
LOG_ERROR("错误信息 - 文件 {} 不存在", "config.xml");

// 异步日志
LOG_ASYN_INFO("异步日志 - 用户 {} 登录成功", "admin");
LOG_ASYN_DEBUG("异步调试 - 内存使用: {} MB", 128.5);
LOG_ASYN_WARN("异步警告 - 连接超时，重试第 {} 次", 3);
LOG_ASYN_ERROR("异步错误 - 数据库连接失败: {}", "Connection refused");
```

#### 日志级别
- `LOG_DEBUG`: 调试信息
- `LOG_INFO`: 一般信息
- `LOG_WARNING`: 警告信息
- `LOG_ERROR`: 错误信息
- `LOG_FATAL`: 致命错误

#### 格式化支持
Logger支持多种格式化方式，按优先级自动选择：

1. **fmt库** (推荐): 如果系统中安装了fmt库，将使用fmt::format进行格式化
2. **C++20 std::format**: 如果编译器支持C++20，将使用std::format
3. **回退方案**: 如果以上都不支持，将使用简单的字符串输出

#### 格式化语法
- `{}`: 默认格式化
- `{:.2f}`: 浮点数格式化，保留2位小数
- `{:<10}`: 左对齐，宽度10
- `{:>10}`: 右对齐，宽度10
- `{:^10}`: 居中对齐，宽度10

#### 日志宏
- `LOG_DEBUG(format, ...)`: 同步调试日志
- `LOG_INFO(format, ...)`: 同步信息日志
- `LOG_WARN(format, ...)`: 同步警告日志
- `LOG_ERROR(format, ...)`: 同步错误日志
- `LOG_FATAL(format, ...)`: 同步致命错误日志

- `LOG_ASYN_DEBUG(format, ...)`: 异步调试日志
- `LOG_ASYN_INFO(format, ...)`: 异步信息日志
- `LOG_ASYN_WARN(format, ...)`: 异步警告日志
- `LOG_ASYN_ERROR(format, ...)`: 异步错误日志
- `LOG_ASYN_FATAL(format, ...)`: 异步致命错误日志

#### Bool类型支持
Logger完全支持bool类型的直接输出，自动格式化为"true"/"false"：

```cpp
bool isEnabled = true;
bool isVisible = false;

// 直接使用现有的日志宏，bool值会自动格式化为"true"/"false"
LOG_INFO("功能状态 - 启用: {}, 可见: {}", isEnabled, isVisible);
LOG_DEBUG("调试信息 - 启用状态: {}", isEnabled);
LOG_WARN("警告信息 - 错误状态: {}", hasError);
```

**注意**: 通过fmt库的formatter特化，bool值会自动格式化为"true"或"false"字符串，而不是默认的数字1/0。

### 字符串工具 (StrTool)
提供字符串处理功能。

### 文件工具 (FileTool)
提供文件操作功能。

## 构建要求

- CMake 3.15+
- C++17 编译器
- fmt 库 (可选，推荐安装以获得最佳格式化支持)

## 构建步骤

```bash
mkdir build
cd build
cmake ..
make
```

## 安装fmt库 (可选但推荐)

### Windows (使用vcpkg)
```powershell
# 运行安装脚本
.\install_fmt.ps1

# 或者手动安装
vcpkg install fmt:x64-windows
vcpkg integrate install
```

### Linux/macOS
```bash
# 运行安装脚本
./install_fmt.sh

# 或者手动安装
# Ubuntu/Debian
sudo apt-get install libfmt-dev

# macOS
brew install fmt
```

## 依赖项

- fmt: 用于字符串格式化 (可选，https://github.com/fmtlib/fmt)
- lz4: 用于数据压缩

## 示例

运行示例程序：
```bash
cd build
./bin/Sample
```

查看生成的日志文件 `Authorization.log` 来了解日志输出格式。

## 测试

运行测试程序：
```bash
# 编译测试程序
g++ -std=c++17 -I. test_logger.cpp Source/IO/Logger.cpp -lpthread -o test_logger

# 运行测试
./test_logger
```

查看生成的 `test_format.log` 文件验证格式化功能。

### c++基本库

`Source\IO\FileTool.h`：文件操作工具类

`Source\Str\StrTool.h`：字符串操作工具类

