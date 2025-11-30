# GitHub推送指南

## 本地Git仓库已准备就绪！

您的代码已经提交到本地Git仓库。现在需要连接到GitHub并推送。

## 步骤1: 在GitHub上创建仓库

1. 登录GitHub
2. 点击右上角的 "+" 号，选择 "New repository"
3. 填写仓库信息：
   - Repository name: `Single-tree` (或您喜欢的名称)
   - Description: "Single-tree segmentation from LiDAR point cloud data"
   - 选择 Public 或 Private
   - **不要**勾选 "Initialize this repository with a README"（我们已经有了）
   - **不要**添加 .gitignore 或 license（我们已经有了）
4. 点击 "Create repository"

## 步骤2: 连接本地仓库到GitHub

在命令行中运行以下命令（将 `yourusername` 替换为您的GitHub用户名）：

```bash
git remote add origin https://github.com/yourusername/Single-tree.git
```

或者如果您使用SSH：

```bash
git remote add origin git@github.com:yourusername/Single-tree.git
```

## 步骤3: 推送到GitHub

```bash
# 设置主分支为main（如果还没有）
git branch -M main

# 推送到GitHub
git push -u origin main
```

## 如果遇到问题

### 问题1: 需要身份验证

如果提示需要登录，您可以使用：
- **Personal Access Token (推荐)**: 在GitHub Settings > Developer settings > Personal access tokens 创建token
- **GitHub CLI**: 使用 `gh auth login` 登录

### 问题2: 远程仓库已存在内容

如果GitHub仓库已经初始化了README等文件，需要先拉取：

```bash
git pull origin main --allow-unrelated-histories
git push -u origin main
```

### 问题3: 分支名称不匹配

如果GitHub默认分支是 `master` 而不是 `main`：

```bash
git push -u origin main:master
```

或者将GitHub仓库的默认分支改为 `main`。

## 验证

推送成功后，访问您的GitHub仓库页面，应该能看到所有文件。

## 后续操作

### 添加标签（可选）

```bash
git tag -a v1.0.0 -m "First release: Single-tree segmentation"
git push origin v1.0.0
```

### 创建Release（可选）

1. 在GitHub仓库页面，点击 "Releases"
2. 点击 "Create a new release"
3. 选择标签 v1.0.0
4. 填写标题和描述
5. 发布

## 快速命令总结

```bash
# 1. 添加远程仓库（替换yourusername）
git remote add origin https://github.com/yourusername/Single-tree.git

# 2. 推送
git branch -M main
git push -u origin main
```

完成！您的代码现在已经在GitHub上了。

