
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <queue>
#include <set>
#include <limits>
#include <string>
#include <chrono>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 点云数据结构
struct Point3D {
    double x, y, z;
    int treeId;  // 所属树木ID，-1表示未分配
    bool isGround;  // 是否为地面点
    bool isTreeTop;  // 是否为树顶点
    
    Point3D(double x = 0, double y = 0, double z = 0) 
        : x(x), y(y), z(z), treeId(-1), isGround(false), isTreeTop(false) {}
    
    double distance(const Point3D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    
    double distance2D(const Point3D& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

// 单木分割类
class SingleTreeSegmentation {
private:
    std::vector<Point3D> points;
    std::vector<Point3D> treeTops;  // 检测到的树顶点
    double cellSize;  // 栅格大小（用于CHM生成）
    double minTreeHeight;  // 最小树高阈值
    double maxCrownRadius;  // 最大树冠半径
    double growthDistance;  // 区域生长距离阈值
    double growthAngle;  // 区域生长角度阈值（度）
    
public:
    SingleTreeSegmentation() 
        : cellSize(0.5), minTreeHeight(2.0), maxCrownRadius(15.0), 
          growthDistance(1.0), growthAngle(30.0) {}
    
    // 设置参数
    void setCellSize(double size) { cellSize = size; }
    void setMinTreeHeight(double height) { minTreeHeight = height; }
    void setMaxCrownRadius(double radius) { maxCrownRadius = radius; }
    void setGrowthDistance(double dist) { growthDistance = dist; }
    void setGrowthAngle(double angle) { growthAngle = angle; }
    
    // 读取点云数据（简单格式：每行 x y z）
    bool loadPointCloud(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return false;
        }
        
        points.clear();
        double x, y, z;
        while (file >> x >> y >> z) {
            points.push_back(Point3D(x, y, z));
        }
        
        file.close();
        std::cout << "成功加载 " << points.size() << " 个点" << std::endl;
        return true;
    }
    
    // 地面点滤除（简化版：基于高度阈值）
    void filterGroundPoints() {
        if (points.empty()) return;
        
        // 找到最低点作为地面参考
        double minZ = points[0].z;
        for (const auto& p : points) {
            if (p.z < minZ) minZ = p.z;
        }
        
        // 标记地面点（高度低于阈值）
        double groundThreshold = minZ + 0.5;  // 地面点高度阈值
        for (auto& p : points) {
            p.isGround = (p.z <= groundThreshold);
        }
        
        int groundCount = 0;
        for (const auto& p : points) {
            if (p.isGround) groundCount++;
        }
        std::cout << "地面点数量: " << groundCount << std::endl;
    }
    
    // 生成归一化数字表面模型（nDSM）
    std::vector<std::vector<double>> generateNDSM() {
        if (points.empty()) return {};
        
        // 计算点云范围
        double minX = points[0].x, maxX = points[0].x;
        double minY = points[0].y, maxY = points[0].y;
        for (const auto& p : points) {
            if (p.isGround) continue;  // 跳过地面点
            minX = std::min(minX, p.x);
            maxX = std::max(maxX, p.x);
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }
        
        // 计算栅格尺寸
        int cols = static_cast<int>((maxX - minX) / cellSize) + 1;
        int rows = static_cast<int>((maxY - minY) / cellSize) + 1;
        
        // 初始化nDSM（存储每个栅格的最大高度）
        std::vector<std::vector<double>> nDSM(rows, std::vector<double>(cols, 0.0));
        std::vector<std::vector<int>> count(rows, std::vector<int>(cols, 0));
        
        // 填充nDSM
        for (const auto& p : points) {
            if (p.isGround) continue;
            
            int col = static_cast<int>((p.x - minX) / cellSize);
            int row = static_cast<int>((p.y - minY) / cellSize);
            
            if (row >= 0 && row < rows && col >= 0 && col < cols) {
                if (p.z > nDSM[row][col]) {
                    nDSM[row][col] = p.z;
                }
                count[row][col]++;
            }
        }
        
        std::cout << "nDSM尺寸: " << rows << " x " << cols << std::endl;
        return nDSM;
    }
    
    // 树顶点检测（在nDSM中检测局部最大值）
    void detectTreeTops() {
        treeTops.clear();
        
        // 生成nDSM
        auto nDSM = generateNDSM();
        if (nDSM.empty()) return;
        
        // 计算点云范围
        double minX = points[0].x, maxX = points[0].x;
        double minY = points[0].y, maxY = points[0].y;
        for (const auto& p : points) {
            if (p.isGround) continue;
            minX = std::min(minX, p.x);
            maxX = std::max(maxX, p.x);
            minY = std::min(minY, p.y);
            maxY = std::max(maxY, p.y);
        }
        
        int rows = nDSM.size();
        int cols = nDSM[0].size();
        
        // 检测局部最大值（树顶点）
        int windowSize = 3;  // 搜索窗口大小
        double minHeight = minTreeHeight;
        
        for (int i = windowSize; i < rows - windowSize; i++) {
            for (int j = windowSize; j < cols - windowSize; j++) {
                if (nDSM[i][j] < minHeight) continue;
                
                // 检查是否为局部最大值
                bool isLocalMax = true;
                double centerHeight = nDSM[i][j];
                
                for (int di = -windowSize; di <= windowSize && isLocalMax; di++) {
                    for (int dj = -windowSize; dj <= windowSize && isLocalMax; dj++) {
                        if (di == 0 && dj == 0) continue;
                        if (nDSM[i + di][j + dj] >= centerHeight) {
                            isLocalMax = false;
                        }
                    }
                }
                
                if (isLocalMax) {
                    // 找到对应的点云中的最高点
                    double cellX = minX + j * cellSize;
                    double cellY = minY + i * cellSize;
                    double cellZ = centerHeight;
                    
                    // 在该栅格内找到最接近的点
                    Point3D* bestPoint = nullptr;
                    double minDist = std::numeric_limits<double>::max();
                    
                    for (auto& p : points) {
                        if (p.isGround) continue;
                        double dist = std::sqrt((p.x - cellX) * (p.x - cellX) + 
                                               (p.y - cellY) * (p.y - cellY));
                        if (dist < cellSize && dist < minDist) {
                            minDist = dist;
                            bestPoint = &p;
                        }
                    }
                    
                    if (bestPoint && !bestPoint->isTreeTop) {
                        bestPoint->isTreeTop = true;
                        treeTops.push_back(*bestPoint);
                    }
                }
            }
        }
        
        std::cout << "检测到 " << treeTops.size() << " 个树顶点" << std::endl;
    }
    
    // 区域生长分割（改进版：使用多轮迭代同时生长）
    void regionGrowingSegmentation() {
        if (treeTops.empty()) {
            std::cerr << "错误: 未检测到树顶点" << std::endl;
            return;
        }
        
        // 重置所有点的treeId
        for (auto& p : points) {
            if (!p.isGround) {
                p.treeId = -1;
            }
        }
        
        // 第一阶段：基于距离的初始分配（将每个点分配给最近的树顶点）
        std::cout << "第一阶段: 基于距离的初始分配..." << std::endl;
        int initialAssignments = 0;
        for (auto& p : points) {
            if (p.isGround) continue;
            
            int nearestTreeId = -1;
            double minDist2D = std::numeric_limits<double>::max();
            
            // 找到最近的树顶点
            for (size_t i = 0; i < treeTops.size(); i++) {
                double dist2D = p.distance2D(treeTops[i]);
                if (dist2D < minDist2D) {
                    minDist2D = dist2D;
                    nearestTreeId = static_cast<int>(i);
                }
            }
            
            // 如果距离在树冠半径内，则分配
            if (nearestTreeId != -1 && minDist2D <= maxCrownRadius) {
                p.treeId = nearestTreeId;
                initialAssignments++;
            }
        }
        std::cout << "初始分配: " << initialAssignments << " 个点" << std::endl;
        
        // 第二阶段：多轮区域生长优化（所有树同时生长）
        std::cout << "\n第二阶段: 多轮区域生长优化..." << std::endl;
        bool changed = true;
        int iteration = 0;
        const int maxIterations = 20;  // 最多迭代20轮
        
        while (changed && iteration < maxIterations) {
            changed = false;
            iteration++;
            
            // 为每棵树创建生长队列
            std::vector<std::queue<Point3D*>> treeQueues(treeTops.size());
            
            // 将所有已分配的点加入对应树的队列
            for (auto& p : points) {
                if (p.isGround) continue;
                if (p.treeId >= 0 && p.treeId < static_cast<int>(treeTops.size())) {
                    treeQueues[p.treeId].push(&p);
                }
            }
            
            // 每棵树进行一轮生长
            std::vector<int> growthCount(treeTops.size(), 0);
            for (size_t treeIdx = 0; treeIdx < treeTops.size(); treeIdx++) {
                // 处理当前树的所有点（一轮生长）
                std::set<Point3D*> processedThisRound;
                
                while (!treeQueues[treeIdx].empty()) {
                    Point3D* current = treeQueues[treeIdx].front();
                    treeQueues[treeIdx].pop();
                    
                    // 避免重复处理
                    if (processedThisRound.find(current) != processedThisRound.end()) {
                        continue;
                    }
                    processedThisRound.insert(current);
                    
                    // 搜索所有未分配的点，找到满足生长条件的点
                    for (auto& neighbor : points) {
                        if (neighbor.isGround) continue;
                        if (neighbor.treeId != -1) continue;  // 已分配给其他树
                        
                        // 计算3D距离
                        double dist3D = current->distance(neighbor);
                        
                        // 主要判断：3D距离必须在生长距离内
                        if (dist3D > growthDistance) continue;
                        
                        // 辅助判断：2D距离不能超过树冠半径
                        double dist2D = current->distance2D(neighbor);
                        if (dist2D > maxCrownRadius) continue;
                        
                        // 检查角度：允许向下和水平方向生长
                        double heightDiff = neighbor.z - current->z;
                        if (dist2D > 0.01) {
                            double angle = std::atan2(heightDiff, dist2D) * 180.0 / M_PI;
                            if (angle > growthAngle) continue;  // 只限制向上生长
                        }
                        
                        // 满足条件，分配给当前树
                        neighbor.treeId = static_cast<int>(treeIdx);
                        growthCount[treeIdx]++;
                        changed = true;
                    }
                }
            }
            
            if (changed) {
                int totalGrowth = 0;
                for (size_t i = 0; i < treeTops.size(); i++) {
                    if (growthCount[i] > 0) {
                        totalGrowth += growthCount[i];
                    }
                }
                std::cout << "  迭代 " << iteration << ": 新增 " << totalGrowth << " 个点" << std::endl;
            }
        }
        
        // 第三阶段：将剩余未分配的点分配给最近的已分配点
        std::cout << "\n第三阶段: 分配剩余未分配点..." << std::endl;
        int reassignedCount = 0;
        for (auto& p : points) {
            if (p.isGround) continue;
            if (p.treeId != -1) continue;  // 已分配
            
            // 找到最近的已分配点
            Point3D* nearestAssigned = nullptr;
            int nearestTreeId = -1;
            double minDist3D = std::numeric_limits<double>::max();
            
            for (auto& candidate : points) {
                if (candidate.isGround) continue;
                if (candidate.treeId == -1) continue;  // 未分配的点
                
                double dist3D = p.distance(candidate);
                double dist2D = p.distance2D(candidate);
                
                // 必须在树冠半径内，且3D距离最小
                if (dist2D <= maxCrownRadius * 1.5 && dist3D < minDist3D) {
                    minDist3D = dist3D;
                    nearestAssigned = &candidate;
                    nearestTreeId = candidate.treeId;
                }
            }
            
            // 如果找到最近的已分配点，则分配
            if (nearestAssigned != nullptr) {
                p.treeId = nearestTreeId;
                reassignedCount++;
            }
        }
        
        // 第四阶段：对于仍然未分配的点，分配给最近的树顶点（放宽距离限制）
        for (auto& p : points) {
            if (p.isGround) continue;
            if (p.treeId != -1) continue;  // 已分配
            
            // 找到最近的树顶点
            int nearestTreeId = -1;
            double minDist2D = std::numeric_limits<double>::max();
            
            for (size_t i = 0; i < treeTops.size(); i++) {
                double dist2D = p.distance2D(treeTops[i]);
                if (dist2D < minDist2D) {
                    minDist2D = dist2D;
                    nearestTreeId = static_cast<int>(i);
                }
            }
            
            // 放宽距离限制到3倍树冠半径
            if (nearestTreeId != -1 && minDist2D <= maxCrownRadius * 3.0) {
                p.treeId = nearestTreeId;
                reassignedCount++;
            }
        }
        
        std::cout << "重新分配点数: " << reassignedCount << std::endl;
        
        // 统计分割结果
        std::vector<int> treePointCount(treeTops.size(), 0);
        int unassigned = 0;
        
        for (const auto& p : points) {
            if (p.isGround) continue;
            if (p.treeId >= 0 && p.treeId < static_cast<int>(treeTops.size())) {
                treePointCount[p.treeId]++;
            } else {
                unassigned++;
            }
        }
        
        std::cout << "\n分割完成:" << std::endl;
        for (size_t i = 0; i < treeTops.size(); i++) {
            std::cout << "  树木 " << i << ": " << treePointCount[i] << " 个点" << std::endl;
        }
        std::cout << "未分配点数: " << unassigned << std::endl;
    }
    
    // 保存分割结果
    bool saveResults(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法创建输出文件: " << filename << std::endl;
            return false;
        }
        
        // 保存格式: x y z treeId
        for (const auto& p : points) {
            if (!p.isGround) {
                file << p.x << " " << p.y << " " << p.z << " " << p.treeId << std::endl;
            }
        }
        
        file.close();
        std::cout << "结果已保存到: " << filename << std::endl;
        return true;
    }
    
    // 保存树顶点
    bool saveTreeTops(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法创建输出文件: " << filename << std::endl;
            return false;
        }
        
        for (size_t i = 0; i < treeTops.size(); i++) {
            file << treeTops[i].x << " " << treeTops[i].y << " " 
                 << treeTops[i].z << " " << i << std::endl;
        }
        
        file.close();
        std::cout << "树顶点已保存到: " << filename << std::endl;
        return true;
    }
    
    // 执行完整的分割流程
    void executeSegmentation(const std::string& inputFile, 
                            const std::string& outputFile,
                            const std::string& treeTopFile) {
        std::cout << "=== 开始单木分割 ===" << std::endl;
        
        // 1. 加载点云
        if (!loadPointCloud(inputFile)) {
            return;
        }
        
        // 2. 地面点滤除
        std::cout << "\n步骤 1: 地面点滤除" << std::endl;
        filterGroundPoints();
        
        // 3. 树顶点检测
        std::cout << "\n步骤 2: 树顶点检测" << std::endl;
        detectTreeTops();
        
        // 4. 区域生长分割
        std::cout << "\n步骤 3: 区域生长分割" << std::endl;
        regionGrowingSegmentation();
        
        // 5. 保存结果
        std::cout << "\n步骤 4: 保存结果" << std::endl;
        saveResults(outputFile);
        saveTreeTops(treeTopFile);
        
        std::cout << "\n=== 分割完成 ===" << std::endl;
    }
};

int main() {
    // 1. 记录开始时间（时间点类型：time_point）
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "单木分割程序 by Yubo" << std::endl;
    std::cout << "================================================" << std::endl;
    
    SingleTreeSegmentation seg;
    
    // 设置参数（可根据实际数据调整）
    // 注意：growthDistance应该根据点云密度设置，通常为平均点间距的1.5-2倍
    seg.setCellSize(0.5);          // 栅格大小（米）- 用于CHM生成
    seg.setMinTreeHeight(2.0);     // 最小树高（米）- 用于树顶点检测
    seg.setMaxCrownRadius(15.0);   // 最大树冠半径（米）- 限制单棵树的最大范围
    seg.setGrowthDistance(1.5);    // 区域生长距离阈值（米）- 主要判断条件，应基于点云密度
    seg.setGrowthAngle(60.0);      // 区域生长角度阈值（度）- 限制向上生长，允许向下和水平
    
    // 执行分割
    // 注意：请将 "input.txt" 替换为实际的点云数据文件路径
    // 输入文件格式：每行包含 x y z（空格分隔）
    std::string inputFile = "E:\\0512\\Plot1n.txt";
    std::string outputFile = "output.txt";
    std::string treeTopFile = "tree_tops.txt";
    
    seg.executeSegmentation(inputFile, outputFile, treeTopFile);
    
    // 如果文件不存在，创建示例数据
    std::ifstream testFile(inputFile);
    if (!testFile.good()) {
        std::cout << "\n注意: 输入文件不存在，正在创建示例数据文件..." << std::endl;
        std::ofstream sampleFile(inputFile);
        
        // 创建示例点云数据（模拟几棵树）
        for (int tree = 0; tree < 3; tree++) {
            double centerX = tree * 20.0;
            double centerY = tree * 20.0;
            double baseZ = 0.0;
            
            // 生成树冠点云
            for (int i = 0; i < 100; i++) {
                double angle = (i * 2.0 * M_PI) / 100.0;
                double radius = (i % 20) * 0.5;
                double x = centerX + radius * std::cos(angle);
                double y = centerY + radius * std::sin(angle);
                double z = baseZ + 5.0 + (i % 10) * 0.3;
                sampleFile << x << " " << y << " " << z << std::endl;
            }
        }
        
        // 添加地面点
        for (int i = 0; i < 50; i++) {
            sampleFile << (i * 2.0) << " " << (i * 2.0) << " 0.0" << std::endl;
        }
        
        sampleFile.close();
        std::cout << "示例数据已创建，请重新运行程序" << std::endl;
    } else {
        testFile.close();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_s = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "运行时间：" << duration_s << " 秒\n";
    return 0;
}
