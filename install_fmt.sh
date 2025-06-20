#!/bin/bash

# 安装fmt库的bash脚本
# 适用于Linux和macOS系统

echo "正在安装fmt库..."

# 检查操作系统
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    if command -v apt-get &> /dev/null; then
        # Ubuntu/Debian
        sudo apt-get update
        sudo apt-get install -y libfmt-dev
    elif command -v yum &> /dev/null; then
        # CentOS/RHEL
        sudo yum install -y fmt-devel
    elif command -v pacman &> /dev/null; then
        # Arch Linux
        sudo pacman -S fmt
    else
        echo "未找到支持的包管理器，请手动安装fmt库"
        exit 1
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    if command -v brew &> /dev/null; then
        brew install fmt
    else
        echo "请先安装Homebrew: https://brew.sh/"
        exit 1
    fi
else
    echo "不支持的操作系统: $OSTYPE"
    exit 1
fi

echo "fmt库安装完成！"
echo "现在可以构建LuTool项目了。" 