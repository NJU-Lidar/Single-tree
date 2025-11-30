# 构建指南

## 使用CMake构建

### 前置要求

- CMake 3.10 或更高版本
- C++11 兼容的编译器
  - Windows: Visual Studio 2015+ 或 MinGW
  - Linux: GCC 4.8+ 或 Clang 3.3+
  - macOS: Xcode 8.0+

### 基本构建步骤

#### 1. 创建构建目录

```bash
mkdir build
cd build
```

#### 2. 配置项目

**Windows (Visual Studio):**
```bash
cmake ..
```

或者指定生成器：
```bash
cmake -G "Visual Studio 16 2019" -A x64 ..
```

**Linux/macOS:**
```bash
cmake ..
```

或者指定构建类型：
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### 3. 编译

**Windows (Visual Studio):**
```bash
cmake --build . --config Release
```

**Linux/macOS:**
```bash
cmake --build .
```

或者使用make：
```bash
make
```

### 高级选项

#### 指定构建类型

```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### 指定安装路径

```bash
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install ..
```

#### 指定C++编译器

```bash
cmake -DCMAKE_CXX_COMPILER=g++ ..
cmake -DCMAKE_CXX_COMPILER=clang++ ..
```

### 安装

编译完成后，可以安装到系统：

```bash
cmake --build . --target install
```

### 运行

编译完成后，可执行文件位于：

- Windows: `build/bin/Release/single_tree_segmentation.exe`
- Linux/macOS: `build/bin/single_tree_segmentation`

运行示例：

```bash
# Windows
build\bin\Release\single_tree_segmentation.exe

# Linux/macOS
./build/bin/single_tree_segmentation
```

### 清理构建

```bash
# 删除构建目录
rm -rf build

# 或者在build目录中
cmake --build . --target clean
```

### 常见问题

**Q: CMake找不到编译器？**
A: 确保编译器已安装并在PATH中，或使用 `-DCMAKE_CXX_COMPILER` 指定编译器路径。

**Q: 在Windows上如何选择Visual Studio版本？**
A: 使用 `-G` 选项指定生成器，例如：
```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
```

**Q: 如何查看所有可用的生成器？**
A: 运行 `cmake --help` 查看帮助信息。

### 跨平台构建

#### Windows (MinGW)

```bash
cmake -G "MinGW Makefiles" ..
cmake --build .
```

#### macOS

```bash
cmake -G "Unix Makefiles" ..
cmake --build .
```

#### Linux

```bash
cmake -G "Unix Makefiles" ..
cmake --build .
```

