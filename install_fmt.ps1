# 安装fmt库的PowerShell脚本
# 适用于Windows系统

Write-Host "正在安装fmt库..." -ForegroundColor Green

# 检查是否已安装vcpkg
if (-not (Get-Command vcpkg -ErrorAction SilentlyContinue)) {
    Write-Host "未找到vcpkg，正在安装..." -ForegroundColor Yellow
    
    # 克隆vcpkg仓库
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    
    # 运行bootstrap脚本
    .\bootstrap-vcpkg.bat
    
    # 将vcpkg添加到PATH
    $env:PATH += ";$PWD"
    
    cd ..
}

# 安装fmt库
Write-Host "正在安装fmt库..." -ForegroundColor Green
vcpkg install fmt:x64-windows

# 集成到Visual Studio
Write-Host "正在集成到Visual Studio..." -ForegroundColor Green
vcpkg integrate install

Write-Host "fmt库安装完成！" -ForegroundColor Green
Write-Host "现在可以构建LuTool项目了。" -ForegroundColor Green 