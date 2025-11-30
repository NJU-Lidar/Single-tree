#!/bin/bash
# CMake构建脚本 (Linux/macOS)

set -e  # 遇到错误立即退出

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Single-Tree Segmentation 构建脚本 ===${NC}"

# 检查CMake是否安装
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}错误: 未找到CMake，请先安装CMake${NC}"
    exit 1
fi

# 创建构建目录
if [ ! -d "build" ]; then
    echo -e "${YELLOW}创建构建目录...${NC}"
    mkdir build
fi

cd build

# 配置项目
echo -e "${YELLOW}配置CMake项目...${NC}"
cmake -DCMAKE_BUILD_TYPE=Release ..

# 编译
echo -e "${YELLOW}开始编译...${NC}"
cmake --build . -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo -e "${GREEN}构建完成！${NC}"
echo -e "${GREEN}可执行文件位于: build/bin/single_tree_segmentation${NC}"

