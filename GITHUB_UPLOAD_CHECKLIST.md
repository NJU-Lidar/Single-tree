# GitHub上传文件清单

## 必须上传的文件

### 源代码
- [x] `Single-tree/Single-tree.cpp` - 主程序源代码

### CMake配置文件
- [x] `CMakeLists.txt` - CMake主配置文件

### 项目文件（可选，保留以便Visual Studio用户使用）
- [x] `Single-tree.sln` - Visual Studio解决方案文件
- [x] `Single-tree/Single-tree.vcxproj` - Visual Studio项目文件
- [x] `Single-tree/Single-tree.vcxproj.filters` - Visual Studio过滤器文件

### 文档文件
- [x] `README.md` - 项目主文档
- [x] `BUILD.md` - 构建指南
- [x] `QUICK_START.md` - 快速开始指南
- [x] `CONTRIBUTING.md` - 贡献指南
- [x] `PROJECT_STRUCTURE.md` - 项目结构说明
- [x] `LICENSE` - MIT许可证

### 构建脚本
- [x] `build.sh` - Linux/macOS构建脚本
- [x] `build.bat` - Windows构建脚本

### 配置文件
- [x] `.gitignore` - Git忽略文件配置

### 示例数据（可选）
- [ ] `Single-tree/data/input.txt` - 示例输入数据（如果文件太大，建议不上传或使用Git LFS）

### 占位文件
- [x] `Single-tree/output/.gitkeep` - 保持output目录结构

## 不应上传的文件

以下文件已在`.gitignore`中配置，不会上传：

### 编译输出
- `build/` - CMake构建目录
- `x64/` - Visual Studio编译输出
- `*.exe`, `*.dll`, `*.obj`, `*.pdb` - 编译生成的文件

### Visual Studio用户文件
- `*.vcxproj.user` - 用户特定配置
- `.vs/` - Visual Studio缓存

### 输出文件
- `Single-tree/output/*.txt` - 程序运行生成的输出文件
- `Single-tree/output.txt` - 临时输出文件
- `Single-tree/tree_tops.txt` - 临时输出文件

### 其他
- `*.LiPrj` - LiDAR项目文件
- `.DS_Store`, `Thumbs.db` - 系统文件

## 可选文件（根据情况决定）

### 论文PDF（如果允许）
- [ ] `Single-tree/2012-Li-etal-PERS.pdf` - 参考论文（如果文件较大，建议不上传或使用Git LFS）

### 大型数据文件
如果`Single-tree/data/input.txt`文件很大（>10MB），建议：
1. 不上传，在README中说明如何准备数据
2. 使用Git LFS（Large File Storage）
3. 上传到其他位置（如云存储），在README中提供链接

## 上传前检查清单

- [ ] 确认所有源代码文件已包含
- [ ] 确认所有文档文件已包含
- [ ] 确认`.gitignore`配置正确
- [ ] 检查是否有敏感信息（如API密钥、密码等）
- [ ] 确认LICENSE文件存在
- [ ] 确认README.md内容完整
- [ ] 检查文件大小，大文件考虑使用Git LFS

## 快速上传命令

```bash
# 1. 初始化Git仓库（如果还没有）
git init

# 2. 添加所有文件（.gitignore会自动排除不需要的文件）
git add .

# 3. 检查将要提交的文件
git status

# 4. 提交
git commit -m "Initial commit: Single-tree segmentation with CMake support"

# 5. 在GitHub上创建仓库后，添加远程仓库
git remote add origin https://github.com/yourusername/Single-tree.git

# 6. 推送到GitHub
git push -u origin main
```

## 注意事项

1. **大文件处理**: 如果数据文件很大，建议不包含在仓库中，或使用Git LFS
2. **敏感信息**: 确保代码中没有硬编码的路径、密钥等敏感信息
3. **文件大小**: GitHub建议单个文件不超过100MB，仓库总大小不超过1GB
4. **许可证**: 确保LICENSE文件存在且内容正确

