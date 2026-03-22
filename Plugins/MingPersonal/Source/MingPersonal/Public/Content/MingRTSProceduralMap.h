#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSProceduralMap.generated.h"

// 地形類型
UENUM(BlueprintType)
enum class ETerrainType : uint8
{
    Plains              UMETA(DisplayName = "平原"),
    Hills               UMETA(DisplayName = "丘陵"),
    Mountains           UMETA(DisplayName = "山地"),
    Forest              UMETA(DisplayName = "森林"),
    Desert              UMETA(DisplayName = "沙漠"),
    Swamp               UMETA(DisplayName = "沼澤"),
    Water               UMETA(DisplayName = "水域"),
    River               UMETA(DisplayName = "河流"),
    Coast               UMETA(DisplayName = "海岸"),
    Canyon              UMETA(DisplayName = "峽谷"),
    Plateau             UMETA(DisplayName = "高原"),
    Volcanic            UMETA(DisplayName = "火山"),
    Snow                UMETA(DisplayName = "雪地"),
    Jungle              UMETA(DisplayName = "叢林")
};

// 資源類型
UENUM(BlueprintType)
enum class EMapResourceType : uint8
{
    Wood                UMETA(DisplayName = "木材"),
    Stone               UMETA(DisplayName = "石料"),
    Iron                UMETA(DisplayName = "鐵礦"),
    Gold                UMETA(DisplayName = "金礦"),
    Food                UMETA(DisplayName = "食物"),
    Water               UMETA(DisplayName = "水源"),
    Oil                 UMETA(DisplayName = "石油"),
    Crystal             UMETA(DisplayName = "水晶"),
    RareEarth           UMETA(DisplayName = "稀土"),
    None                UMETA(DisplayName = "無")
};

// 地圖尺寸
UENUM(BlueprintType)
enum class EMapSize : uint8
{
    Tiny                UMETA(DisplayName = "微小 - 64x64"),
    Small               UMETA(DisplayName = "小型 - 128x128"),
    Medium              UMETA(DisplayName = "中型 - 256x256"),
    Large               UMETA(DisplayName = "大型 - 512x512"),
    Huge                UMETA(DisplayName = "巨大 - 1024x1024"),
    Custom              UMETA(DisplayName = "自定義")
};

// 地圖風格
UENUM(BlueprintType)
enum class EMapStyle : uint8
{
    Historical          UMETA(DisplayName = "歷史風格"),
    Fantasy             UMETA(DisplayName = "奇幻風格"),
    Realistic           UMETA(DisplayName = "寫實風格"),
    Stylized            UMETA(DisplayName = "風格化"),
    LowPoly             UMETA(DisplayName = "低多邊形"),
    Isometric           UMETA(DisplayName = "等角視角")
};

// 氣候類型
UENUM(BlueprintType)
enum class EClimateType : uint8
{
    Temperate           UMETA(DisplayName = "溫帶"),
    Tropical            UMETA(DisplayName = "熱帶"),
    Arid                UMETA(DisplayName = "乾旱"),
    Polar               UMETA(DisplayName = "極地"),
    Continental         UMETA(DisplayName = "大陸性"),
    Mediterranean       UMETA(DisplayName = "地中海"),
    Monsoon             UMETA(DisplayName = "季風")
};

// 地形單元格數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMapCellData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 X;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Y;
    
    UPROPERTY(BlueprintReadOnly)
    ETerrainType TerrainType;
    
    UPROPERTY(BlueprintReadOnly)
    float Height;
    
    UPROPERTY(BlueprintReadOnly)
    float Moisture;
    
    UPROPERTY(BlueprintReadOnly)
    float Temperature;
    
    UPROPERTY(BlueprintReadOnly)
    EMapResourceType ResourceType;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceAmount;
    
    UPROPERTY(BlueprintReadOnly)
    bool bBuildable;
    
    UPROPERTY(BlueprintReadOnly)
    bool bWalkable;
    
    UPROPERTY(BlueprintReadOnly)
    float MovementCost;
    
    UPROPERTY(BlueprintReadOnly)
    FString BiomeName;
    
    FMapCellData()
        : X(0)
        , Y(0)
        , TerrainType(ETerrainType::Plains)
        , Height(0.0f)
        , Moisture(0.5f)
        , Temperature(0.5f)
        , ResourceType(EMapResourceType::None)
        , ResourceAmount(0.0f)
        , bBuildable(true)
        , bWalkable(true)
        , MovementCost(1.0f)
    {}
};

// 資源節點
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FResourceNode
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString NodeID;
    
    UPROPERTY(BlueprintReadOnly)
    EMapResourceType ResourceType;
    
    UPROPERTY(BlueprintReadOnly)
    FVector2D Location;
    
    UPROPERTY(BlueprintReadOnly)
    float Amount;
    
    UPROPERTY(BlueprintReadOnly)
    float Quality;
    
    UPROPERTY(BlueprintReadOnly)
    float ExtractionRate;
    
    UPROPERTY(BlueprintReadOnly)
    float Radius;
    
    FResourceNode()
        : ResourceType(EMapResourceType::None)
        , Amount(100.0f)
        , Quality(1.0f)
        , ExtractionRate(1.0f)
        , Radius(50.0f)
    {}
};

// 地圖區域
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMapRegion
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString RegionID;
    
    UPROPERTY(BlueprintReadOnly)
    FString RegionName;
    
    UPROPERTY(BlueprintReadOnly)
    ETerrainType DominantTerrain;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FVector2D> BoundaryPoints;
    
    UPROPERTY(BlueprintReadOnly)
    FVector2D Center;
    
    UPROPERTY(BlueprintReadOnly)
    float Area;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<EMapResourceType> AvailableResources;
    
    UPROPERTY(BlueprintReadOnly)
    int32 CellCount;
    
    UPROPERTY(BlueprintReadOnly)
    FBox2D BoundingBox;
    
    FMapRegion()
        : DominantTerrain(ETerrainType::Plains)
        , Center(FVector2D::ZeroVector)
        , Area(0.0f)
        , CellCount(0)
    {}
};

// 河流數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FRiverData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString RiverID;
    
    UPROPERTY(BlueprintReadOnly)
    FString RiverName;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FVector2D> Path;
    
    UPROPERTY(BlueprintReadOnly)
    float Width;
    
    UPROPERTY(BlueprintReadOnly)
    float Length;
    
    UPROPERTY(BlueprintReadOnly)
    FVector2D Source;
    
    UPROPERTY(BlueprintReadOnly)
    FVector2D Mouth;
    
    UPROPERTY(BlueprintReadOnly)
    int32 TributaryCount;
    
    FRiverData()
        : Width(10.0f)
        , Length(0.0f)
        , TributaryCount(0)
    {}
};

// 地圖生成配置
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMapGenerationConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMapSize MapSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMapStyle MapStyle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EClimateType Climate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CustomWidth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CustomHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Seed;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainRoughness;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WaterLevel;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceDensity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RegionCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RiverCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGenerateRoads;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGenerateSettlements;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseErosion;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ErosionIterations;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MountainHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ForestDensity;
    
    FMapGenerationConfig()
        : MapSize(EMapSize::Medium)
        , MapStyle(EMapStyle::Historical)
        , Climate(EClimateType::Temperate)
        , CustomWidth(256)
        , CustomHeight(256)
        , Seed(0)
        , TerrainRoughness(0.5f)
        , WaterLevel(0.3f)
        , ResourceDensity(0.5f)
        , RegionCount(4)
        , RiverCount(3)
        , bGenerateRoads(true)
        , bGenerateSettlements(true)
        , bUseErosion(true)
        , ErosionIterations(100)
        , MountainHeight(1000.0f)
        , ForestDensity(0.4f)
    {}
};

// 生成的地圖數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FProceduralMapData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString MapID;
    
    UPROPERTY(BlueprintReadOnly)
    FString MapName;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Width;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Height;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FMapCellData> Cells;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FResourceNode> ResourceNodes;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FMapRegion> Regions;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FRiverData> Rivers;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FVector2D> SettlementLocations;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<TArray<FVector2D>> RoadNetworks;
    
    UPROPERTY(BlueprintReadOnly)
    float MinHeight;
    
    UPROPERTY(BlueprintReadOnly)
    float MaxHeight;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalLandArea;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalWaterArea;
    
    UPROPERTY(BlueprintReadOnly)
    FMapGenerationConfig GenerationConfig;
    
    UPROPERTY(BlueprintReadOnly)
    float GenerationTime;
    
    FProceduralMapData()
        : Width(0)
        , Height(0)
        , MinHeight(0.0f)
        , MaxHeight(0.0f)
        , TotalLandArea(0.0f)
        , TotalWaterArea(0.0f)
        , GenerationTime(0.0f)
    {}
};

// 地圖分析結果
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMapAnalysisResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    TMap<ETerrainType, float> TerrainDistribution;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<EMapResourceType, int32> ResourceCounts;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageHeight;
    
    UPROPERTY(BlueprintReadOnly)
    float HeightVariance;
    
    UPROPERTY(BlueprintReadOnly)
    float StrategicValue;
    
    UPROPERTY(BlueprintReadOnly)
    float BalanceScore;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> StrategicPoints;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ChokePoints;
    
    FMapAnalysisResult()
        : AverageHeight(0.0f)
        , HeightVariance(0.0f)
        , StrategicValue(0.5f)
        , BalanceScore(0.5f)
    {}
};

// 委托聲明
declare_dynamic_multicast_delegate_one_param(FOnMapGenerationStarted, FString, MapID);
declare_dynamic_multicast_delegate_two_params(FOnMapGenerationProgress, FString, MapID, float, Progress);
declare_dynamic_multicast_delegate_two_params(FOnMapGenerationCompleted, FString, MapID, FProceduralMapData, MapData);
declare_dynamic_multicast_delegate_one_param(FOnMapGenerationFailed, FString, ErrorMessage);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingRTSProceduralMap : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSProceduralMap();
    
    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Map|Generation")
    void InitializeProceduralMapSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Map|Generation")
    void ShutdownProceduralMapSystem();
    
    // 地圖生成
    UFUNCTION(BlueprintCallable, Category = "Map|Generation")
    FProceduralMapData GenerateMap(const FMapGenerationConfig& Config);
    
    UFUNCTION(BlueprintCallable, Category = "Map|Generation")
    void GenerateMapAsync(const FMapGenerationConfig& Config);
    
    UFUNCTION(BlueprintCallable, Category = "Map|Generation")
    void CancelGeneration();
    
    // 地圖操作
    UFUNCTION(BlueprintPure, Category = "Map|Data")
    FMapCellData GetCell(int32 X, int32 Y) const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Data")
    FMapCellData GetCellAtLocation(FVector2D Location) const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Data")
    void SetCellTerrain(int32 X, int32 Y, ETerrainType NewTerrain);
    
    UFUNCTION(BlueprintPure, Category = "Map|Data")
    TArray<FMapCellData> GetCellsInRadius(FVector2D Center, float Radius) const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Data")
    TArray<FMapCellData> GetCellsInRegion(const FString& RegionID) const;
    
    // 資源管理
    UFUNCTION(BlueprintCallable, Category = "Map|Resources")
    void AddResourceNode(EMapResourceType Type, FVector2D Location, float Amount, float Quality);
    
    UFUNCTION(BlueprintCallable, Category = "Map|Resources")
    void RemoveResourceNode(const FString& NodeID);
    
    UFUNCTION(BlueprintPure, Category = "Map|Resources")
    TArray<FResourceNode> GetResourceNodesInRadius(FVector2D Center, float Radius) const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Resources")
    FResourceNode GetNearestResourceNode(FVector2D Location, EMapResourceType Type) const;
    
    // 區域管理
    UFUNCTION(BlueprintPure, Category = "Map|Regions")
    FMapRegion GetRegion(const FString& RegionID) const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Regions")
    FString GetRegionAtLocation(FVector2D Location) const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Regions")
    void DefineRegion(const TArray<FVector2D>& Boundary, const FString& RegionName);
    
    // 路徑查找
    UFUNCTION(BlueprintCallable, Category = "Map|Pathfinding")
    TArray<FVector2D> FindPath(FVector2D Start, FVector2D End) const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Pathfinding")
    TArray<FVector2D> FindPathAvoidingTerrain(FVector2D Start, FVector2D End, TArray<ETerrainType> AvoidedTerrains) const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Pathfinding")
    bool IsPathClear(FVector2D Start, FVector2D End) const;
    
    // 分析與驗證
    UFUNCTION(BlueprintCallable, Category = "Map|Analysis")
    FMapAnalysisResult AnalyzeMap() const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Analysis")
    float CalculateMapBalance() const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Analysis")
    TArray<FVector2D> FindStrategicPoints() const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Analysis")
    TArray<FVector2D> FindChokePoints() const;
    
    UFUNCTION(BlueprintPure, Category = "Map|Analysis")
    bool ValidateMap() const;
    
    // 地圖存儲與加載
    UFUNCTION(BlueprintCallable, Category = "Map|Storage")
    bool SaveMap(const FString& FilePath) const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Storage")
    bool LoadMap(const FString& FilePath);
    
    UFUNCTION(BlueprintCallable, Category = "Map|Storage")
    bool ExportToTexture(const FString& FilePath, EMapResourceType ResourceToVisualize) const;
    
    // 地圖修改
    UFUNCTION(BlueprintCallable, Category = "Map|Modification")
    void ApplyHeightmap(const TArray<float>& HeightData);
    
    UFUNCTION(BlueprintCallable, Category = "Map|Modification")
    void SmoothTerrain(int32 Iterations);
    
    UFUNCTION(BlueprintCallable, Category = "Map|Modification")
    void AddNoise(float Amplitude, float Frequency);
    
    UFUNCTION(BlueprintCallable, Category = "Map|Modification")
    void ErodeTerrain(int32 Iterations);
    
    // 預設模板
    UFUNCTION(BlueprintCallable, Category = "Map|Templates")
    FMapGenerationConfig GetPlainsTemplate() const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Templates")
    FMapGenerationConfig GetMountainTemplate() const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Templates")
    FMapGenerationConfig GetIslandTemplate() const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Templates")
    FMapGenerationConfig GetDesertTemplate() const;
    
    UFUNCTION(BlueprintCallable, Category = "Map|Templates")
    FMapGenerationConfig GetRiverDeltaTemplate() const;
    
    // 委托
    UPROPERTY(BlueprintAssignable, Category = "Map|Events")
    FOnMapGenerationStarted OnMapGenerationStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Map|Events")
    FOnMapGenerationProgress OnMapGenerationProgress;
    
    UPROPERTY(BlueprintAssignable, Category = "Map|Events")
    FOnMapGenerationCompleted OnMapGenerationCompleted;
    
    UPROPERTY(BlueprintAssignable, Category = "Map|Events")
    FOnMapGenerationFailed OnMapGenerationFailed;
    
protected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    bool bIsGenerating;
    
    UPROPERTY()
    FProceduralMapData CurrentMap;
    
    UPROPERTY()
    TMap<FString, FProceduralMapData> SavedMaps;
    
    // 生成步驟
    void GenerateHeightmap();
    void ApplyClimateEffects();
    void GenerateRivers();
    void PlaceResources();
    void DefineRegions();
    void GenerateRoads();
    void PlaceSettlements();
    void FinalizeMap();
    
    // 噪聲函數
    float PerlinNoise(float X, float Y) const;
    float FractalBrownianMotion(float X, float Y, int32 Octaves) const;
    float RidgedMultifractal(float X, float Y, int32 Octaves) const;
    
    // 輔助函數
    int32 GetCellIndex(int32 X, int32 Y) const;
    bool IsValidCell(int32 X, int32 Y) const;
    float GetDistance(FVector2D A, FVector2D B) const;
    ETerrainType DetermineTerrainFromHeight(float Height, float Moisture, float Temperature) const;
    float GetMovementCostForTerrain(ETerrainType Terrain) const;
    
    // 河流生成
    TArray<FVector2D> FindRiverPath(FVector2D Start) const;
    float CalculateDrainage(int32 X, int32 Y) const;
    
    // 路徑查找 (A*)
    float Heuristic(FVector2D A, FVector2D B) const;
    TArray<FVector2D> ReconstructPath(TMap<FVector2D, FVector2D>& CameFrom, FVector2D Current) const;
    
    // 資源分佈
    FVector2D FindOptimalResourceLocation(EMapResourceType Type) const;
    bool IsValidResourceLocation(FVector2D Location, EMapResourceType Type) const;
    
    // 隨機數生成
    float RandomFloat() const;
    int32 RandomInt(int32 Max) const;
    FVector2D RandomPoint() const;
    
    UPROPERTY()
    int32 CurrentSeed;
    
    UPROPERTY()
    FMapGenerationConfig CurrentConfig;
};
