#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFogOfWarManager.generated.h"

// 視野類型
UENUM(BlueprintType)
enum class EMingVisionType : uint8
{
    Normal,            // 正常視野
    Enhanced,          // 增強視野
    NightVision,        // 夜視
    Thermal,           // 熱成像
    Satellite,         // 衛星視野
    Aerial             // 空中偵察
};

// 迷霧等級
UENUM(BlueprintType)
enum class EMingFogLevel : uint8
{
    Clear,             // 清晰
    Light,             // 輕霧
    Medium,            // 中霧
    Heavy,             // 濃霧
    Complete           // 完全迷霧
};

// 地形類型
UENUM(BlueprintType)
enum class EMingTerrainType : uint8
{
    Open,              // 開闊地形
    Forest,            // 森林
    Mountain,          // 山地
    Urban,             // 城市
    Desert,            // 沙漠
    Water,             // 水域
    Swamp,             // 沼澤
    Snow               // 雪地
};

// 偵察狀態
UENUM(BlueprintType)
enum class EMingScoutingStatus : uint8
{
    Idle,              // 空閒
    Scouting,          // 偵察中
    Observing,         // 觀察中
    Reporting,          // 報告中
    Hidden,            // 隱蔽
    Detected           // 被發現
};

// 視野數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingVisionData
{
    GENERATED_BODY()

    // 單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // 視野半徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionRadius;

    // 視野類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingVisionType VisionType;

    // 視野角度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionAngle;

    // 視野持續時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionDuration;

    // 是否為持續視野
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPersistent;

    // 視野更新間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionUpdateInterval;

    // 可見單位列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> VisibleUnits;

    // 已偵察區域
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> ScoutedAreas;

    // 最後更新時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdateTime;

    FMingVisionData()
    {
        UnitID = -1;
        VisionRadius = 0.0f;
        VisionType = EMingVisionType::Normal;
        VisionAngle = 360.0f;
        VisionDuration = 0.0f;
        bIsPersistent = false;
        VisionUpdateInterval = 0.1f;
        VisibleUnits.Empty();
        ScoutedAreas.Empty();
        LastUpdateTime = FDateTime::Now();
    }
};

// 迷霧網格數據
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFogGridCell
{
    GENERATED_BODY()

    // 網格座標
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint GridPosition;

    // 迷霧等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFogLevel FogLevel;

    // 是否被偵察過
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsScouted;

    // 偵察時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutedTime;

    // 偵察單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // 地形類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTerrainType TerrainType;

    // 地形遮蔽值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainOcclusion;

    // 建築物遮蔽值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StructureOcclusion;

    // 最後可見時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastVisibleTime;

    FMingFogGridCell()
    {
        GridPosition = FIntPoint(0, 0);
        FogLevel = EMingFogLevel::Complete;
        bIsScouted = false;
        ScoutedTime = FDateTime::MinValue();
        ScoutingUnitID = -1;
        TerrainType = EMingTerrainType::Open;
        TerrainOcclusion = 0.0f;
        StructureOcclusion = 0.0f;
        LastVisibleTime = FDateTime::MinValue();
    }
};

// 偵察報告
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingScoutingReport
{
    GENERATED_BODY()

    // 報告ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ReportID;

    // 偵察單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // 偵察時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutingTime;

    // 偵察位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ScoutingLocation;

    // 偵察範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoutingRadius;

    // 發現的單位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> DiscoveredUnits;

    // 發現的建築物
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> DiscoveredStructures;

    // 地形信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> TerrainInfo;

    // 威脅評估
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatAssessment;

    // 報告詳細度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReportDetail;

    // 報告可靠性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReportReliability;

    FMingScoutingReport()
    {
        ReportID = -1;
        ScoutingUnitID = -1;
        ScoutingTime = FDateTime::Now();
        ScoutingLocation = FVector::ZeroVector;
        ScoutingRadius = 0.0f;
        DiscoveredUnits.Empty();
        DiscoveredStructures.Empty();
        TerrainInfo.Empty();
        ThreatAssessment = TEXT("");
        ReportDetail = 0.0f;
        ReportReliability = 1.0f;
    }
};

// 戰爭迷霧事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFogOfWarUpdated, const FVector&, Location, EMingFogLevel, NewFogLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVisionUpdated, int32, UnitID, const TArray<int32>&, NewVisibleUnits);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoutingCompleted, const FMingScoutingReport&, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSpotted, int32, SpottingUnitID, int32, SpottedUnitID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitLostSight, int32, LosingUnitID, int32, LostUnitID);

/**
 * 戰爭迷霧管理器
 * 負責管理戰爭迷霧、視野和偵察系統
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFogOfWarManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFogOfWarManager();

    // 初始化戰爭迷霧系統
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool InitializeFogOfWarSystem();

    // 註冊單位視野
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool RegisterUnitVision(int32 UnitID, const FMingVisionData& VisionData);

    // 更新單位視野
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool UpdateUnitVision(int32 UnitID, const FVector& NewPosition);

    // 獲取單位視野
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    FMingVisionData GetUnitVision(int32 UnitID) const;

    // 獲取可見單位
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<int32> GetVisibleUnits(int32 UnitID) const;

    // 檢查單位可見性
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // 更新戰爭迷霧
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void UpdateFogOfWar();

    // 獲取迷霧等級
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    EMingFogLevel GetFogLevel(const FVector& Location) const;

    // 設置地形類型
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetTerrainType(const FVector& Location, EMingTerrainType TerrainType);

    // 添加建築物遮蔽
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool AddStructureOcclusion(const FVector& Location, float OcclusionValue);

    // 執行偵察任務
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool ExecuteScoutingMission(int32 UnitID, const FVector& TargetLocation, float Radius);

    // 獲取偵察報告
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingScoutingReport> GetScoutingReports() const;

    // 設置偵察狀態
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetScoutingStatus(int32 UnitID, EMingScoutingStatus Status);

    // 計算視野範圍
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FVector> CalculateVisionArea(int32 UnitID) const;

    // 獲取迷霧網格
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingFogGridCell> GetFogGrid() const;

    // 保存迷霧數據
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SaveFogOfWarData();

    // 載入迷霧數據
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool LoadFogOfWarData();

    // 清除所有迷霧數據
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void ClearAllFogOfWarData();

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnFogOfWarUpdated OnFogOfWarUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnVisionUpdated OnVisionUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnScoutingCompleted OnScoutingCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnUnitSpotted OnUnitSpotted;

    UPROPERTY(BlueprintAssignable)
    FOnUnitLostSight OnUnitLostSight;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // 單位視野數據庫
    UPROPERTY()
    TMap<int32, FMingVisionData> UnitVisionDatabase;

    // 迷霧網格
    UPROPERTY()
    TArray<FMingFogGridCell> FogGrid;

    // 地形數據
    UPROPERTY()
    TMap<FIntPoint, EMingTerrainType> TerrainGrid;

    // 建築物遮蔽數據
    UPROPERTY()
    TMap<FIntPoint, float> StructureOcclusionGrid;

    // 偵察報告數據庫
    UPROPERTY()
    TArray<FMingScoutingReport> ScoutingReports;

    // 偵察任務隊列
    UPROPERTY()
    TArray<int32> ActiveScoutingUnits;

    // 網格設置
    UPROPERTY()
    int32 GridSizeX;

    UPROPERTY()
    int32 GridSizeY;

    UPROPERTY()
    float CellSize;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 初始化迷霧網格
    void InitializeFogGrid();

    // 更新單位視野
    void UpdateUnitVisionInternal(int32 UnitID);

    // 計算視野內的網格
    TArray<FIntPoint> CalculateVisibleGridCells(int32 UnitID);

    // 更新迷霧網格
    void UpdateFogGridCell(int32 GridX, int32 GridY, EMingFogLevel NewFogLevel, bool bIsScouted, int32 ScoutingUnitID);

    // 檢查地形遮蔽
    bool CheckTerrainOcclusion(const FVector& From, const FVector& To);

    // 檢查建築物遮蔽
    bool CheckStructureOcclusion(const FVector& From, const FVector& To);

    // 計算視野線
    TArray<FVector> CalculateVisionLines(int32 UnitID);

    // 處理偵察任務
    void ProcessScoutingMission(int32 UnitID, float DeltaTime);

    // 生成偵察報告
    FMingScoutingReport GenerateScoutingReport(int32 UnitID, const FVector& Location, float Radius);

    // 更新地形信息
    void UpdateTerrainInfo(const FVector& Location);

    // 世界座標轉換為網格座標
    FIntPoint WorldToGrid(const FVector& WorldPosition) const;

    // 網格座標轉換為世界座標
    FVector GridToWorld(const FIntPoint& GridPosition) const;

    // 檢查網格座標是否有效
    bool IsValidGridPosition(const FIntPoint& GridPosition) const;

    // 獲取網格索引
    int32 GetGridIndex(int32 GridX, int32 GridY) const;

    // 計算兩點間距離
    float CalculateDistance(const FVector& Point1, const FVector& Point2) const;

    // 檢查視野內的單位
    void CheckUnitsInVision(int32 UnitID);

    // 處理單位發現/遺失
    void ProcessUnitDiscovery(int32 ViewerID, int32 DiscoveredUnitID);
    void ProcessUnitLossOfSight(int32 ViewerID, int32 LostUnitID);

    // 驗證視野數據
    bool ValidateVisionData(const FMingVisionData& VisionData) const;

    // 清除過期的偵察信息
    void CleanExpiredScoutingInfo();

    // 更新迷霧渲染
    void UpdateFogRendering();
};
