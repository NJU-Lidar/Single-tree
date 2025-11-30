@echo off
REM CMake构建脚本 (Windows)

echo === Single-Tree Segmentation 构建脚本 ===

REM 检查CMake是否安装
where cmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo 错误: 未找到CMake，请先安装CMake
    exit /b 1
)

REM 创建构建目录
if not exist "build" (
    echo 创建构建目录...
    mkdir build
)

cd build

REM 配置项目
echo 配置CMake项目...
cmake -G "Visual Studio 17 2022" -A x64 ..

if %ERRORLEVEL% NEQ 0 (
    echo 尝试使用默认生成器...
    cmake ..
)

REM 编译
echo 开始编译...
cmake --build . --config Release

if %ERRORLEVEL% EQU 0 (
    echo 构建完成！
    echo 可执行文件位于: build\bin\Release\single_tree_segmentation.exe
) else (
    echo 构建失败！
    exit /b 1
)

