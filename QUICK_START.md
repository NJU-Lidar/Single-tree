# 快速开始指南

## 5分钟快速上手

### 1. 编译项目

**使用CMake（推荐，跨平台）：**

**Windows:**
```bash
build.bat
```

**Linux/macOS:**
```bash
chmod +x build.sh
./build.sh
```

**或者手动使用CMake：**
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

**其他方法：**

**Windows (Visual Studio):**
1. 打开 `Single-tree.sln`
2. 按 F7 编译

**Linux/macOS (直接编译):**
```bash
g++ -std=c++11 -O2 Single-tree/Single-tree.cpp -o single_tree_segmentation
```

### 2. 准备数据

将您的点云数据保存为文本文件，格式为每行一个点的XYZ坐标：
```
x1 y1 z1
x2 y2 z2
x3 y3 z3
```

或者使用示例数据：`Single-tree/data/input.txt`

### 3. 运行程序

**使用CMake构建后：**

**Windows:**
```bash
build\bin\Release\single_tree_segmentation.exe
```

**Linux/macOS:**
```bash
./build/bin/single_tree_segmentation
```

**使用Visual Studio构建后：**
```bash
cd x64\Release
Single-tree.exe
```

**直接编译后：**
```bash
./single_tree_segmentation
```

### 4. 查看结果

结果文件将保存在 `output/` 文件夹：
- `segmented_points.txt`: 分割后的点云（每行：x y z treeId）
- `tree_tops.txt`: 检测到的树顶点（每行：x y z treeId）

## 自定义参数

在 `main()` 函数中修改参数：

```cpp
seg.setCellSize(0.5);          // 栅格大小（米）
seg.setMinTreeHeight(2.0);     // 最小树高（米）
seg.setMaxCrownRadius(15.0);   // 最大树冠半径（米）
seg.setGrowthDistance(1.5);    // 区域生长距离（米）
seg.setGrowthAngle(60.0);      // 区域生长角度（度）
```

## 常见问题

**Q: 程序运行很慢？**  
A: 对于大型点云（>100万点），考虑增加 `growthDistance` 或优化算法。

**Q: 分割结果不理想？**  
A: 尝试调整参数，特别是 `growthDistance` 和 `maxCrownRadius`。

**Q: 如何可视化结果？**  
A: 可以使用CloudCompare、MeshLab等软件打开输出文件。

## 下一步

- 阅读 [README.md](README.md) 了解详细文档
- 查看 [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) 了解项目结构
- 参考 [CONTRIBUTING.md](CONTRIBUTING.md) 参与贡献

