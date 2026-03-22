# MingRTSProceduralMap 系统文档

## 概述

MingRTSProceduralMap 是一个程序生成地图系统，使用分形噪声和地形算法动态生成RTS游戏地图，支持多种地形类型、气候区域和战略要素。

## 核心功能

### 1. 地形类型
系统支持14种地形类型：
- **Plains**: 平原 - 适合大规模战斗
- **Hills**: 丘陵 - 提供战术高地优势
- **Mountains**: 山地 - 难以通过的天然屏障
- **Forest**: 森林 - 提供掩护和伏击机会
- **Desert**: 沙漠 - 资源稀缺的开阔地带
- **Swamp**: 沼泽 - 减缓移动速度的危险地形
- **Water**: 水域 - 需要海军或桥梁才能通过
- **River**: 河流 - 天然的防御线
- **Lake**: 湖泊 - 战略资源点
- **Beach**: 海滩 - 登陆战的关键位置
- **Cliff**: 悬崖 - 不可逾越的地形障碍
- **Canyon**: 峡谷 - 狭窄的战略通道
- **Plateau**: 高原 - 视野开阔的战术要地
- **Valley**: 山谷 - 资源丰富的封闭区域

### 2. 地图尺寸
支持6种标准尺寸：
- **Tiny**: 64x64 - 快速对战
- **Small**: 128x128 - 小型遭遇战
- **Medium**: 256x256 - 标准对战
- **Large**: 512x512 - 大规模战役
- **Huge**: 768x768 - 史诗级战斗
- **Gigantic**: 1024x1024 - 巨型沙盒

### 3. 气候类型
- **Temperate**: 温带 - 平衡的资源分布
- **Tropical**: 热带 - 丰富的植被和水源
- **Arid**: 干旱 - 稀缺的资源，需要争夺
- **Polar**: 极地 - 极端环境，特殊挑战
- **Continental**: 大陆性 - 四季变化
- **Mediterranean**: 地中海 - 独特的资源模式

### 4. 资源分布
- **均匀分布**: 资源均匀散布
- **随机分布**: 完全随机的资源位置
- **聚类分布**: 资源成组出现
- **边缘分布**: 资源集中在地图边缘
- **中心分布**: 资源集中在地图中心
- **带状分布**: 资源形成带状区域
- **网络分布**: 资源沿道路网络分布

## API 参考

### 系统初始化

```cpp
// 初始化地图系统
void InitializeMapSystem();

// 关闭地图系统
void ShutdownMapSystem();
```

### 地图生成

```cpp
// 生成地图（完整参数）
FProceduralMap GenerateMap(int32 SizeX, int32 SizeY, EMapClimate Climate,
    EResourceDistribution ResourceDist, uint32 Seed);

// 生成地图（使用预设配置）
FProceduralMap GenerateMap(const FMapGenerationConfig& Config);

// 生成地图（简化版）
FProceduralMap GenerateQuickMap(int32 SizeX, int32 SizeY, uint32 Seed);

// 异步生成地图
void GenerateMapAsync(const FMapGenerationConfig& Config, 
    FOnMapGenerationCompleted Callback);
```

### 高度图操作

```cpp
// 生成高度图
TArray<float> GenerateHeightMap(int32 SizeX, int32 SizeY, uint32 Seed);

// 应用侵蚀效果
void ApplyErosion(TArray<float>& HeightMap, int32 Iterations, float Strength);

// 平滑高度图
void SmoothHeightMap(TArray<float>& HeightMap, int32 Iterations);

// 添加山脉
void AddMountains(TArray<float>& HeightMap, int32 Count, float HeightScale);

// 添加山谷
void AddValleys(TArray<float>& HeightMap, int32 Count, float DepthScale);
```

### 地形分类

```cpp
// 分类地形格子
ETerrainType ClassifyTerrain(float Height, float Moisture, float Temperature);

// 获取地形类型
ETerrainType GetTerrainAt(const FProceduralMap& Map, int32 X, int32 Y);

// 设置地形类型
void SetTerrainAt(FProceduralMap& Map, int32 X, int32 Y, ETerrainType Type);

// 平滑地形边界
void SmoothTerrainBoundaries(FProceduralMap& Map, int32 Iterations);
```

### 气候应用

```cpp
// 应用气候
void ApplyClimate(FProceduralMap& Map, EMapClimate Climate);

// 生成河流
void GenerateRivers(FProceduralMap& Map, int32 RiverCount);

// 生成湖泊
void GenerateLakes(FProceduralMap& Map, int32 LakeCount);

// 应用风化效果
void ApplyWeathering(FProceduralMap& Map, EWeatheringType Type, float Intensity);
```

### 资源放置

```cpp
// 放置资源节点
void PlaceResources(FProceduralMap& Map, EResourceDistribution Distribution);

// 放置特定资源
void PlaceResource(FProceduralMap& Map, EResourceType Type, int32 X, int32 Y, int32 Amount);

// 平衡资源分布
void BalanceResources(FProceduralMap& Map, float BalanceFactor);

// 验证资源平衡
bool ValidateResourceBalance(const FProceduralMap& Map, float Tolerance);
```

### 区域分析

```cpp
// 检测区域
TArray<FMapRegion> DetectRegions(const FProceduralMap& Map);

// 计算区域大小
int32 CalculateRegionSize(const FProceduralMap& Map, int32 StartX, int32 StartY);

// 获取区域地形
ETerrainType GetDominantTerrain(const FMapRegion& Region);

// 合并小区域
void MergeSmallRegions(FProceduralMap& Map, int32 MinRegionSize);
```

### 路径查找

```cpp
// 查找路径
TArray<FIntPoint> FindPath(const FProceduralMap& Map, FIntPoint Start, FIntPoint End,
    EPathfindingMode Mode);

// 检查路径可行性
bool IsPathValid(const FProceduralMap& Map, const TArray<FIntPoint>& Path);

// 计算路径代价
float CalculatePathCost(const FProceduralMap& Map, const TArray<FIntPoint>& Path);

// 寻路回调
FOnPathFound OnPathFound;
```

### 战略分析

```cpp
// 识别战略点
TArray<FStrategicPoint> IdentifyStrategicPoints(const FProceduralMap& Map);

// 评估地图平衡性
FMapBalanceMetrics EvaluateMapBalance(const FProceduralMap& Map);

// 识别瓶颈点
TArray<FIntPoint> IdentifyBottlenecks(const FProceduralMap& Map, float Threshold);

// 生成热力图
TArray<float> GenerateHeatmap(const FProceduralMap& Map, EHeatmapType Type);
```

### 地图操作

```cpp
// 保存地图
void SaveMap(const FProceduralMap& Map, const FString& FilePath);

// 加载地图
FProceduralMap LoadMap(const FString& FilePath);

// 导出高度图
void ExportHeightmap(const FProceduralMap& Map, const FString& FilePath);

// 导入高度图
FProceduralMap ImportHeightmap(const FString& FilePath);

// 重置地图
void ResetMap(FProceduralMap& Map);
```

### 预设模板

```cpp
// 获取预设模板
TArray<FMapPresetTemplate> GetAvailablePresets();

// 应用预设模板
FProceduralMap ApplyPreset(const FMapPresetTemplate& Preset);

// 创建自定义预设
FMapPresetTemplate CreateCustomPreset(const FString& Name, 
    const FMapGenerationConfig& Config);
```

## 数据结构

### FProceduralMap
```cpp
struct FProceduralMap
{
    int32 SizeX;                        // 地图宽度
    int32 SizeY;                        // 地图高度
    uint32 Seed;                        // 随机种子
    EMapClimate Climate;                // 气候类型
    EResourceDistribution ResourceDist; // 资源分布模式
    TArray<ETerrainType> TerrainGrid;   // 地形网格
    TArray<float> HeightMap;            // 高度图
    TArray<FResourceNode> Resources;    // 资源节点
    TArray<FIntPoint> Rivers;           // 河流路径
    TArray<FIntPoint> Roads;            // 道路网络
    TArray<FMapRegion> Regions;         // 区域列表
    TArray<FStrategicPoint> StrategicPoints; // 战略点
    FMapBalanceMetrics BalanceMetrics;  // 平衡性指标
    FString MapName;                    // 地图名称
    FString Description;                // 地图描述
};
```

### FMapGenerationConfig
```cpp
struct FMapGenerationConfig
{
    int32 SizeX;                        // 宽度
    int32 SizeY;                        // 高度
    uint32 Seed;                        // 随机种子(0 = 随机)
    EMapClimate Climate;                // 气候
    EResourceDistribution ResourceDist; // 资源分布
    int32 ResourceDensity;              // 资源密度 (1-10)
    float WaterLevel;                   // 水位高度 (0-1)
    float MountainThreshold;            // 山地阈值 (0-1)
    float ErosionIterations;            // 侵蚀迭代次数
    int32 RiverCount;                   // 河流数量
    bool bGenerateRoads;                // 是否生成道路
    bool bGenerateStrategicPoints;      // 是否生成战略点
    bool bBalanceTeams;                 // 是否平衡队伍位置
    int32 PlayerCount;                  // 玩家数量
};
```

### FResourceNode
```cpp
struct FResourceNode
{
    int32 X;                            // X坐标
    int32 Y;                            // Y坐标
    EResourceType Type;                 // 资源类型
    int32 Amount;                       // 资源数量
    float Accessibility;                // 可访问性 (0-1)
    int32 RegionID;                     // 所属区域ID
};
```

### FMapRegion
```cpp
struct FMapRegion
{
    int32 RegionID;                     // 区域ID
    ETerrainType DominantTerrain;       // 主导地形
    TArray<FIntPoint> Cells;            // 包含的格子
    int32 ResourceCount;                // 资源数量
    float AverageHeight;                // 平均高度
    float StrategicValue;               // 战略价值
    bool bIsAccessible;                 // 是否可访问
};
```

### FStrategicPoint
```cpp
struct FStrategicPoint
{
    FIntPoint Location;                 // 位置
    EStrategicPointType Type;         // 类型
    float Importance;                   // 重要程度 (0-1)
    TArray<FIntPoint> ControlZone;      // 控制区域
    float DefenseBonus;                 // 防御加成
    float ResourceBonus;                // 资源加成
    FString Description;                // 描述
};
```

### FMapBalanceMetrics
```cpp
struct FMapBalanceMetrics
{
    float SymmetryScore;                  // 对称性分数 (0-1)
    float ResourceBalance;              // 资源平衡 (0-1)
    float TerrainVariety;               // 地形多样性 (0-1)
    float StrategicBalance;             // 战略平衡 (0-1)
    float AccessibilityScore;           // 可访问性分数 (0-1)
    float OverallBalance;               // 总体平衡 (0-1)
    TMap<int32, float> TeamScores;      // 各队伍分数
    bool bIsTournamentReady;            // 是否适合比赛
};
```

### FMapPresetTemplate
```cpp
struct FMapPresetTemplate
{
    FString TemplateName;               // 模板名称
    FString Description;                // 描述
    FMapGenerationConfig Config;        // 配置
    EMapTemplateCategory Category;      // 类别
    int32 RecommendedPlayerCount;       // 推荐玩家数
    float AveragePlayTime;              // 平均游戏时长(分钟)
    bool bIsOfficial;                   // 是否官方模板
};
```

## 预设模板

### 内置模板

#### 平原战场 (Plains Warfare)
```cpp
FMapGenerationConfig PlainsConfig;
PlainsConfig.SizeX = 256;
PlainsConfig.SizeY = 256;
PlainsConfig.Climate = EMapClimate::Temperate;
PlainsConfig.ResourceDist = EResourceDistribution::Uniform;
PlainsConfig.WaterLevel = 0.2f;
PlainsConfig.MountainThreshold = 0.7f;
```
特征：开阔地形，适合大规模机动战

#### 山地要塞 (Mountain Fortress)
```cpp
FMapGenerationConfig MountainConfig;
MountainConfig.SizeX = 256;
MountainConfig.SizeY = 256;
MountainConfig.Climate = EMapClimate::Continental;
MountainConfig.MountainThreshold = 0.4f;
MountainConfig.ErosionIterations = 3;
```
特征：多山地，适合防守战术

#### 岛屿争霸 (Island Dominance)
```cpp
FMapGenerationConfig IslandConfig;
IslandConfig.SizeX = 256;
IslandConfig.SizeY = 256;
IslandConfig.WaterLevel = 0.6f;
IslandConfig.Climate = EMapClimate::Tropical;
```
特征：岛屿分散，强调海军和两栖作战

#### 沙漠风暴 (Desert Storm)
```cpp
FMapGenerationConfig DesertConfig;
DesertConfig.SizeX = 256;
DesertConfig.SizeY = 256;
DesertConfig.Climate = EMapClimate::Arid;
DesertConfig.ResourceDist = EResourceDistribution::Clustered;
```
特征：资源稀缺，争夺绿洲和水井

#### 三角洲 (River Delta)
```cpp
FMapGenerationConfig DeltaConfig;
DeltaConfig.SizeX = 256;
DeltaConfig.SizeY = 256;
DeltaConfig.Climate = EMapClimate::Mediterranean;
DeltaConfig.RiverCount = 8;
DeltaConfig.WaterLevel = 0.3f;
```
特征：河网密布，控制桥梁至关重要

## 事件委托

```cpp
// 地图生成完成
FOnMapGenerationCompleted OnMapGenerationCompleted;

// 地图生成进度
FOnMapGenerationProgress OnMapGenerationProgress;

// 路径找到
FOnPathFound OnPathFound;

// 战略点识别
FOnStrategicPointsIdentified OnStrategicPointsIdentified;
```

## 使用示例

### 初始化系统
```cpp
UMingRTSProceduralMap* MapSystem = NewObject<UMingRTSProceduralMap>();
MapSystem->InitializeMapSystem();
```

### 生成简单地图
```cpp
// 生成 256x256 的温带地图
FProceduralMap Map = MapSystem->GenerateQuickMap(256, 256, 12345);

UE_LOG(LogTemp, Log, TEXT("Generated map: %dx%d"), Map.SizeX, Map.SizeY);
```

### 使用完整配置
```cpp
FMapGenerationConfig Config;
Config.SizeX = 512;
Config.SizeY = 512;
Config.Seed = 0;  // 随机种子
Config.Climate = EMapClimate::Temperate;
Config.ResourceDist = EResourceDistribution::Balanced;
Config.ResourceDensity = 7;
Config.WaterLevel = 0.25f;
Config.MountainThreshold = 0.6f;
Config.ErosionIterations = 2;
Config.RiverCount = 5;
Config.bGenerateRoads = true;
Config.bGenerateStrategicPoints = true;
Config.bBalanceTeams = true;
Config.PlayerCount = 4;

FProceduralMap Map = MapSystem->GenerateMap(Config);
```

### 异步生成
```cpp
void AMyGameMode::GenerateMapAsync()
{
    FMapGenerationConfig Config;
    // ... 配置参数
    
    MapSystem->OnMapGenerationCompleted.AddDynamic(this, &AMyGameMode::OnMapReady);
    MapSystem->GenerateMapAsync(Config);
}

void AMyGameMode::OnMapReady(const FProceduralMap& GeneratedMap)
{
    UE_LOG(LogTemp, Log, TEXT("Map generation complete!"));
    CurrentMap = GeneratedMap;
    LoadMapIntoGame();
}
```

### 路径查找
```cpp
FIntPoint Start(50, 50);
FIntPoint End(200, 200);

TArray<FIntPoint> Path = MapSystem->FindPath(
    CurrentMap, Start, End, EPathfindingMode::Balanced);

if (Path.Num() > 0)
{
    float Cost = MapSystem->CalculatePathCost(CurrentMap, Path);
    UE_LOG(LogTemp, Log, TEXT("Path found! Length: %d, Cost: %.2f"), 
        Path.Num(), Cost);
}
```

### 战略分析
```cpp
// 识别战略点
TArray<FStrategicPoint> Points = MapSystem->IdentifyStrategicPoints(CurrentMap);

for (const FStrategicPoint& Point : Points)
{
    UE_LOG(LogTemp, Log, TEXT("Strategic Point at (%d, %d): %s (Importance: %.2f)"),
        Point.Location.X, Point.Location.Y,
        *Point.Description, Point.Importance);
}

// 评估平衡性
FMapBalanceMetrics Metrics = MapSystem->EvaluateMapBalance(CurrentMap);
UE_LOG(LogTemp, Log, TEXT("Map Balance: %.2f"), Metrics.OverallBalance);
UE_LOG(LogTemp, Log, TEXT("Tournament Ready: %s"), 
    Metrics.bIsTournamentReady ? TEXT("Yes") : TEXT("No"));
```

### 保存和加载
```cpp
// 保存地图
MapSystem->SaveMap(CurrentMap, TEXT("Maps/MyCustomMap.map"));

// 加载地图
FProceduralMap LoadedMap = MapSystem->LoadMap(TEXT("Maps/MyCustomMap.map"));
```

## 性能考虑

### 生成时间参考
| 地图尺寸 | 生成时间 | 内存使用 |
|---------|---------|---------|
| 64x64   | <1秒    | ~1 MB   |
| 256x256 | 1-2秒   | ~5 MB   |
| 512x512 | 3-5秒   | ~20 MB  |
| 1024x1024 | 10-15秒 | ~80 MB |

### 优化建议
- 使用异步生成避免阻塞主线程
- 对于大型地图，考虑分块生成
- 缓存常用地图配置
- 使用LOD系统显示远处地形

## 最佳实践

1. **种子管理**: 记录好的地图种子以便重现
2. **平衡性测试**: 始终验证地图平衡性指标
3. **多样性与可玩性**: 确保地形多样性但避免过度随机
4. **资源分布**: 资源应该鼓励探索和争夺
5. **战略深度**: 确保有多个战略点可供争夺
6. **路径网络**: 确保主要区域之间有可通行的路径

## 调试工具

```cpp
// 生成高度图可视化
void ExportHeightmapForDebug(const FProceduralMap& Map);

// 生成地形类型热力图
void ExportTerrainHeatmap(const FProceduralMap& Map);

// 生成资源分布图
void ExportResourceMap(const FProceduralMap& Map);

// 生成战略点标记图
void ExportStrategicPointsMap(const FProceduralMap& Map);
```

## 扩展性

系统支持以下扩展：
- 自定义地形类型
- 新的气候效果
- 特殊地图特性（如火山、冰川）
- 自定义资源类型
- 新的战略点类型
- 第三方噪声算法集成
