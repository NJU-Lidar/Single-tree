# Git配置和上传指南

## 当前状态

✅ Git仓库已初始化
✅ 所有文件已添加到暂存区
⏳ 需要配置Git用户信息才能提交

## 步骤1: 配置Git用户信息

请运行以下命令（替换为您的信息）：

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

例如：
```bash
git config --global user.name "Yubo Wang"
git config --global user.email "yubo@example.com"
```

## 步骤2: 提交代码

配置完成后，运行：

```bash
git commit -m "Initial commit: Single-tree segmentation with CMake support"
```

## 步骤3: 在GitHub上创建仓库

1. 登录 https://github.com
2. 点击右上角 "+" > "New repository"
3. 仓库名称: `Single-tree`
4. 描述: "Single-tree segmentation from LiDAR point cloud data"
5. 选择 Public 或 Private
6. **不要**勾选任何初始化选项（README、.gitignore、LICENSE）
7. 点击 "Create repository"

## 步骤4: 连接并推送

```bash
# 添加远程仓库（替换yourusername为您的GitHub用户名）
git remote add origin https://github.com/yourusername/Single-tree.git

# 设置主分支
git branch -M main

# 推送到GitHub
git push -u origin main
```

## 如果遇到身份验证问题

GitHub现在要求使用Personal Access Token：

1. 访问: https://github.com/settings/tokens
2. 点击 "Generate new token" > "Generate new token (classic)"
3. 选择权限: 至少勾选 `repo`
4. 生成token并复制
5. 推送时，用户名输入您的GitHub用户名，密码输入token

## 快速命令（复制粘贴）

```bash
# 1. 配置Git（替换为您的信息）
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"

# 2. 提交
git commit -m "Initial commit: Single-tree segmentation with CMake support"

# 3. 连接GitHub（替换yourusername）
git remote add origin https://github.com/yourusername/Single-tree.git
git branch -M main
git push -u origin main
```

## 已准备的文件

以下文件已准备好上传：
- ✅ 源代码 (Single-tree.cpp)
- ✅ CMake配置 (CMakeLists.txt)
- ✅ 所有文档文件
- ✅ 构建脚本
- ✅ 配置文件

编译输出和临时文件已被.gitignore排除，不会上传。

