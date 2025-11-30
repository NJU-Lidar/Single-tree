# CMake使用指南

## 快速开始

### 使用构建脚本（最简单）

**Windows:**
```bash
build.bat
```

**Linux/macOS:**
```bash
chmod +x build.sh
./build.sh
```

### 手动使用CMake

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目
cmake ..

# 3. 编译
cmake --build .
```

## 详细说明

### 配置选项

#### 指定构建类型

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

#### 指定安装路径

```bash
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
```

#### 指定编译器

```bash
cmake -DCMAKE_CXX_COMPILER=g++ ..
cmake -DCMAKE_CXX_COMPILER=clang++ ..
```

### Windows特定选项

#### 选择Visual Studio版本

```bash
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake -G "Visual Studio 16 2019" -A x64 ..
```

#### 选择MinGW

```bash
cmake -G "MinGW Makefiles" ..
```

### 安装

```bash
cmake --build . --target install
```

## 常见问题

**Q: CMake找不到编译器？**
A: 确保编译器已安装并在PATH中，或使用 `-DCMAKE_CXX_COMPILER` 指定。

**Q: 如何查看所有可用的生成器？**
A: 运行 `cmake --help`。

**Q: 如何清理构建？**
A: 删除 `build` 目录或运行 `cmake --build . --target clean`。

## 更多信息

详细构建说明请参考 [BUILD.md](BUILD.md)。

