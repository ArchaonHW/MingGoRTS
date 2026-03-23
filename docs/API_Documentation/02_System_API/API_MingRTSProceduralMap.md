# MingRTSProceduralMap 系統文檔 / MingRTSProceduralMap System Documentation

## 概述 / Overview

MingRTSProceduralMap 是一個程序生成地圖系統，使用分形噪聲和地形算法動態生成RTS遊戲地圖，支持多種地形類型、氣候區域和戰略要素。

MingRTSProceduralMap is a procedural map generation system that uses fractal noise and terrain algorithms to dynamically generate RTS game maps, supporting multiple terrain types, climate regions, and strategic elements.

## 核心功能 / Core Features

### 1. 地形類型 / Terrain Types
系統支持14種地形類型 / System supports 14 terrain types：
- **Plains**: 平原 / Plains - 適合大規模戰鬥 / Suitable for large-scale battles
- **Hills**: 丘陵 / Hills - 提供戰術高地優勢 / Provides tactical high ground advantage
- **Mountains**: 山地 / Mountains - 難以通過的天然屏障 / Natural barriers difficult to traverse
- **Forest**: 森林 / Forest - 提供掩護和伏擊機會 / Provides cover and ambush opportunities
- **Desert**: 沙漠 / Desert - 資源稀缺的開闊地帶 / Open areas with scarce resources
- **Swamp**: 沼澤 / Swamp - 減緩移動速度的危險地形 / Dangerous terrain slowing movement
- **Water**: 水域 / Water - 需要海軍或橋樑才能通過 / Requires navy or bridges to cross
- **River**: 河流 / River - 天然的防禦線 / Natural defensive lines
- **Lake**: 湖泊 / Lake - 戰略資源點 / Strategic resource points
- **Beach**: 海灘 / Beach - 登陸戰的關鍵位置 / Key positions for landing battles
- **Cliff**: 懸崖 / Cliff - 不可逾越的地形障礙 / Impassable terrain obstacles
- **Canyon**: 峽谷 / Canyon - 狹窄的戰略通道 / Narrow strategic passages
- **Plateau**: 高原 / Plateau - 視野開闊的戰術要地 / Tactical high ground with open views
- **Valley**: 山谷 / Valley - 資源豐富的封閉區域 / Enclosed areas rich in resources

### 2. 地圖尺寸 / Map Sizes
支持6種標準尺寸 / Supports 6 standard sizes：
- **Tiny**: 64x64 - 快速對戰 / Quick matches
- **Small**: 128x128 - 小型遭遇戰 / Small skirmishes
- **Medium**: 256x256 - 標準對戰 / Standard matches
- **Large**: 512x512 - 大規模戰役 / Large-scale campaigns
- **Huge**: 768x768 - 史詩級戰鬥 / Epic battles
- **Gigantic**: 1024x1024 - 巨型沙盒 / Giant sandbox

### 3. 氣候類型 / Climate Types
- **Temperate**: 溫帶 / Temperate - 平衡的資源分布 / Balanced resource distribution
- **Tropical**: 熱帶 / Tropical - 豐富的植被和水源 / Rich vegetation and water sources
- **Arid**: 乾旱 / Arid - 稀缺的資源，需要爭奪 / Scarce resources requiring competition
- **Polar**: 極地 / Polar - 極端環境，特殊挑戰 / Extreme environment with special challenges
- **Continental**: 大陸性 / Continental - 四季變化 / Four seasons variation
- **Mediterranean**: 地中海 / Mediterranean - 獨特的資源模式 / Unique resource patterns

### 4. 資源分布 / Resource Distribution
- **均勻分布**: 資源均勻散佈 / Uniform - Resources evenly distributed
- **隨機分布**: 完全隨機的資源位置 / Random - Completely random resource positions
- **聚類分布**: 資源成組出現 / Clustered - Resources appear in groups
- **邊緣分布**: 資源集中在地圖邊緣 / Edge - Resources concentrated at map edges
- **中心分布**: 資源集中在地圖中心 / Center - Resources concentrated at map center
- **帶狀分布**: 資源形成帶狀區域 / Band - Resources form band-shaped regions
- **網絡分布**: 資源沿道路網絡分布 / Network - Resources distributed along road networks

## API 參考 / API Reference

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
