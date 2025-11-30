# Single-Tree Segmentation

基于LiDAR点云数据的单木分割算法实现

## 项目简介

本项目实现了基于2012年Li等人发表在Photogrammetric Engineering & Remote Sensing (PERS)期刊上的单木分割方法。该算法能够从LiDAR点云数据中自动识别和分割出单个树木，适用于森林资源调查、生态监测等应用场景。

## 主要功能

- **点云数据加载**: 支持标准XYZ格式的点云数据
- **地面点滤除**: 自动分离地面点和非地面点
- **树顶点检测**: 基于归一化数字表面模型(nDSM)的局部最大值检测
- **区域生长分割**: 多轮迭代的区域生长算法，确保准确分割
- **结果输出**: 输出分割后的点云数据和树顶点位置

## 编译要求

- **编译器**: 支持C++11或更高版本的编译器
  - Windows: Visual Studio 2015或更高版本
  - Linux: GCC 4.8+ 或 Clang 3.3+
  - macOS: Xcode 8.0+
- **标准库**: 仅使用C++标准库，无需额外依赖

## 编译方法

### 使用CMake（推荐）

CMake支持跨平台构建，适用于Windows、Linux和macOS。

#### 基本步骤

```bash
# 1. 创建构建目录
mkdir build
cd build

# 2. 配置项目
cmake ..

# 3. 编译
cmake --build .
```

#### Windows (Visual Studio)

```bash
mkdir build
cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

#### Linux/macOS

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

编译后的可执行文件位于 `build/bin/` 目录。

详细构建说明请参考 [BUILD.md](BUILD.md)。

### 其他编译方法

#### Windows (Visual Studio项目文件)

1. 打开 `Single-tree.sln`
2. 选择配置（Debug或Release）和平台（x64或Win32）
3. 生成解决方案（F7）

#### 直接使用编译器

**Linux/macOS:**
```bash
g++ -std=c++11 -O2 Single-tree/Single-tree.cpp -o single_tree_segmentation
```

## 使用方法

### 基本用法

```cpp
#include "Single-tree.cpp"

SingleTreeSegmentation seg;

// 设置参数
seg.setCellSize(0.5);          // 栅格大小（米）
seg.setMinTreeHeight(2.0);     // 最小树高（米）
seg.setMaxCrownRadius(15.0);   // 最大树冠半径（米）
seg.setGrowthDistance(1.5);    // 区域生长距离阈值（米）
seg.setGrowthAngle(60.0);      // 区域生长角度阈值（度）

// 执行分割
seg.executeSegmentation(
    "data/input.txt",           // 输入文件
    "output/segmented_points.txt",  // 输出文件
    "output/tree_tops.txt"      // 树顶点文件
);
```

### 输入文件格式

输入文件应为纯文本格式，每行包含一个点的三维坐标（空格分隔）：

```
x1 y1 z1
x2 y2 z2
x3 y3 z3
...
```

### 输出文件格式

**分割结果文件** (`segmented_points.txt`):
```
x y z treeId
```

**树顶点文件** (`tree_tops.txt`):
```
x y z treeId
```

## 参数说明

| 参数 | 说明 | 默认值 | 建议范围 |
|------|------|--------|----------|
| `cellSize` | 栅格大小，用于生成nDSM | 0.5 m | 0.3 - 1.0 m |
| `minTreeHeight` | 最小树高阈值，低于此高度的点不视为树顶点 | 2.0 m | 1.5 - 3.0 m |
| `maxCrownRadius` | 最大树冠半径，限制单棵树的最大范围 | 15.0 m | 10.0 - 20.0 m |
| `growthDistance` | 区域生长距离阈值，应基于点云密度设置 | 1.5 m | 1.0 - 3.0 m |
| `growthAngle` | 区域生长角度阈值，限制向上生长 | 60.0° | 45.0 - 75.0° |

### 参数调整建议

- **点云密度较低**: 增加 `growthDistance` (2.0-3.0 m)
- **点云密度较高**: 减小 `growthDistance` (1.0-1.5 m)
- **树冠较大**: 增加 `maxCrownRadius` (20.0-25.0 m)
- **树冠较小**: 减小 `maxCrownRadius` (10.0-15.0 m)

## 项目结构

```
Single-tree/
├── CMakeLists.txt          # CMake配置文件
├── Single-tree/
│   ├── Single-tree.cpp     # 主程序源代码
│   ├── data/              # 数据文件夹
│   │   └── input.txt      # 示例输入数据
│   └── output/            # 输出文件夹
│       ├── segmented_points.txt # 分割结果
│       └── tree_tops.txt  # 树顶点位置
├── Single-tree.sln        # Visual Studio解决方案文件（可选）
├── Single-tree.vcxproj     # Visual Studio项目文件（可选）
├── README.md              # 项目说明文档
├── BUILD.md               # 详细构建指南
├── LICENSE                # 许可证文件
├── .gitignore            # Git忽略文件
├── build.sh              # Linux/macOS构建脚本
└── build.bat             # Windows构建脚本
```

## 算法原理

### 1. 地面点滤除
使用基于高度阈值的方法分离地面点和非地面点。地面点高度阈值 = 最低点高度 + 0.5米。

### 2. 树顶点检测
1. 生成归一化数字表面模型(nDSM)
2. 在nDSM中检测局部最大值
3. 将局部最大值对应的点标记为树顶点

### 3. 区域生长分割
采用多阶段分割策略：
- **第一阶段**: 基于距离的初始分配，将每个点分配给最近的树顶点
- **第二阶段**: 多轮迭代的区域生长，所有树同时扩展
- **第三阶段**: 将剩余未分配点分配给最近的已分配点
- **第四阶段**: 最终分配，放宽距离限制

## 示例数据

项目包含示例数据文件，位于 `data/` 目录。您可以使用自己的点云数据替换。

## 贡献

欢迎提交Issue和Pull Request！

## 许可证

本项目采用 MIT 许可证。详见 [LICENSE](LICENSE) 文件。

## 参考文献

Li, W., Guo, Q., Jakubowski, M. K., & Kelly, M. (2012). A new method for segmenting individual trees from the lidar point cloud. *Photogrammetric Engineering & Remote Sensing*, 78(1), 75-84.

## 作者

Yubo Wang

## 致谢

感谢Li等人提出的优秀算法方法。
