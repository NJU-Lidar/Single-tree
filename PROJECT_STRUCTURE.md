# 项目结构说明

## 目录结构

```
Single-tree/
├── Single-tree.cpp          # 主程序源代码
├── Single-tree.sln          # Visual Studio解决方案文件（Windows）
├── Single-tree.vcxproj      # Visual Studio项目文件（Windows）
├── Single-tree.vcxproj.filters  # Visual Studio过滤器文件
├── README.md                # 项目说明文档（必读）
├── LICENSE                  # MIT许可证文件
├── CONTRIBUTING.md          # 贡献指南
├── PROJECT_STRUCTURE.md     # 本文件：项目结构说明
├── .gitignore              # Git忽略文件配置
│
├── data/                   # 数据文件夹
│   └── input.txt          # 示例输入数据（XYZ格式）
│
└── output/                # 输出文件夹（自动生成）
    ├── segmented_points.txt # 分割结果（格式：x y z treeId）
    └── tree_tops.txt       # 树顶点位置（格式：x y z treeId）
```

## 文件说明

### 源代码文件

- **Single-tree.cpp**: 包含完整的单木分割算法实现
  - `Point3D`: 三维点数据结构
  - `SingleTreeSegmentation`: 单木分割主类

### 配置文件

- **.gitignore**: 指定Git版本控制忽略的文件和文件夹
- **Single-tree.sln**: Visual Studio解决方案文件
- **Single-tree.vcxproj**: Visual Studio项目配置文件

### 文档文件

- **README.md**: 项目主要文档，包含使用说明、参数配置等
- **LICENSE**: MIT许可证
- **CONTRIBUTING.md**: 贡献者指南
- **PROJECT_STRUCTURE.md**: 本文件

### 数据文件

- **data/input.txt**: 示例点云数据，格式为每行一个点的XYZ坐标

### 输出文件

- **output/segmented_points.txt**: 分割后的点云数据，每行包含点的坐标和所属树木ID
- **output/tree_tops.txt**: 检测到的树顶点位置

## 编译输出

编译后的可执行文件和中间文件位于：
- Windows: `x64/Debug/` 或 `x64/Release/`
- Linux/macOS: 当前目录下的可执行文件

这些文件已被 `.gitignore` 排除，不会提交到版本库。

## 注意事项

1. **data/** 和 **output/** 文件夹需要手动创建（如果不存在）
2. 输出文件夹中的文件会在运行程序时自动生成
3. 建议将大型点云数据文件放在 `data/` 文件夹外，或使用 `.gitignore` 排除
