#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFogOfWarManager.generated.h"

// 視??類??
UENUM(BlueprintType)
enum class EMingVisionType : uint8
{
    Normal,            // ????視??
    Enhanced,          // 增強視??
    NightVision,        // 夜??
    Thermal,           // ???X    Satellite,         // 衛??視??
    Aerial             // 空中????
};

// 迷霧等??
UENUM(BlueprintType)
enum class EMingFogLevel : uint8
{
    Clear,             // 清晰
    Light,             // 輕霧
    Medium,            // 中霧
    Heavy,             // 濃霧
    Complete           // 完全迷霧
};

// ??形類??
UENUM(BlueprintType)
enum class EMingTerrainType : uint8
{
    Open,              // ???X?形
    Forest,            // 森??
    Mountain,          // 山地
    Urban,             // ????
    Desert,            // 沙??
    Water,             // 水??
    Swamp,             // 沼澤
    Snow               // ??地
};

// ???X?XUENUM(BlueprintType)
enum class EMingScoutingStatus : uint8
{
    Idle,              // 空??
    Scouting,          // ??????    Observing,         // 觀察中
    Reporting,          // ??????    Hidden,            // ??蔽
    Detected           // 被發X};

// 視?X???
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingVisionData
{
    GENERATED_BODY()

    // ????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // 視?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionRadius;

    // 視??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingVisionType VisionType;

    // 視??角度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionAngle;

    // 視?X??X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionDuration;

    // ??否????續?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPersistent;

    // 視?X?新????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionUpdateInterval;

    // ???X??X?表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> VisibleUnits;

    // 已偵察?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> ScoutedAreas;

    // ??後更???X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// 迷霧網格????
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingFogGridCell
{
    GENERATED_BODY()

    // 網格座??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FIntPoint GridPosition;

    // 迷霧等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingFogLevel FogLevel;

    // ??否被偵察??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsScouted;

    // ???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutedTime;

    // ???X???ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // ??形類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTerrainType TerrainType;

    // ??形??蔽X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TerrainOcclusion;

    // 建?X?遮????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StructureOcclusion;

    // ??後可見?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastVisibleTime;

    FMingFogGridCell()
    {
        GridPosition = FIntPoint(0, 0};
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

// ???X???
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingScoutingReport
{
    GENERATED_BODY()

    // ????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ReportID;

    // ???X???ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ScoutingUnitID;

    // ???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime ScoutingTime;

    // ????位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ScoutingLocation;

    // ????範??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ScoutingRadius;

    // ??現??單??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> DiscoveredUnits;

    // ??現??建築物
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> DiscoveredStructures;

    // ??形信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> TerrainInfo;

    // 威??評估
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatAssessment;

    // ????詳細??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReportDetail;

    // ???X??X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
        ThreatAssessment = TEXT(""};
        ReportDetail = 0.0f;
        ReportReliability = 1.0f;
    }
};

// ??爭迷霧事件委??
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFogOfWarUpdated, const FVector&, Location, EMingFogLevel, NewFogLevel};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVisionUpdated, int32, UnitID, const TArray<int32>&, NewVisibleUnits};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoutingCompleted, const FMingScoutingReport&, Report};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitSpotted, int32, SpottingUnitID, int32, SpottedUnitID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitLostSight, int32, LosingUnitID, int32, LostUnitID};

/**
 * ??爭迷霧管?X * 負責管?X?爭迷霧???X??X???系統
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingFogOfWarManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFogOfWarManager();

    // ???X?戰??迷??系??    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool InitializeFogOfWarSystem();

    // 註?X???視??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool RegisterUnitVision(int32 UnitID, const FMingVisionData& VisionData};

    // ??新????視??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool UpdateUnitVision(int32 UnitID, const FVector& NewPosition};

    // ???X???視??
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    FMingVisionData GetUnitVision(int32 UnitID) const;

    // ???X??X???
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<int32> GetVisibleUnits(int32 UnitID) const;

    // 檢查???X??X    UFUNCTION(BlueprintPure, Category = "Fog of War")
    bool IsUnitVisible(int32 ViewerID, int32 TargetID) const;

    // ??新??爭迷霧
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void UpdateFogOfWar();

    // ????迷霧等??
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    EMingFogLevel GetFogLevel(const FVector& Location) const;

    // 設置??形類??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetTerrainType(const FVector& Location, EMingTerrainType TerrainType};

    // 添??建?X?遮X    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool AddStructureOcclusion(const FVector& Location, float OcclusionValue};

    // ???X???任??
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool ExecuteScoutingMission(int32 UnitID, const FVector& TargetLocation, float Radius};

    // ???X??X???
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingScoutingReport> GetScoutingReports() const;

    // 設置???X?X    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SetScoutingStatus(int32 UnitID, EMingScoutingStatus Status};

    // 計??視??範??
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FVector> CalculateVisionArea(int32 UnitID) const;

    // ????迷霧網格
    UFUNCTION(BlueprintPure, Category = "Fog of War")
    TArray<FMingFogGridCell> GetFogGrid() const;

    // 保??迷霧????
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool SaveFogOfWarData();

    // 載入迷霧????
    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    bool LoadFogOfWarData();

    // 清除????迷??數X    UFUNCTION(BlueprintCallable, Category = "Fog of War")
    void ClearAllFogOfWarData();

    // 事件委??
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

    // ????視?X?????    UPROPERTY()
    TMap<int32, FMingVisionData> UnitVisionDatabase;

    // 迷霧網格
    UPROPERTY()
    TArray<FMingFogGridCell> FogGrid;

    // ??形????
    UPROPERTY()
    TMap<FIntPoint, EMingTerrainType> TerrainGrid;

    // 建?X?遮??數X    UPROPERTY()
    TMap<FIntPoint, float> StructureOcclusionGrid;

    // ???X??X?????    UPROPERTY()
    TArray<FMingScoutingReport> ScoutingReports;

    // ????任?X???
    UPROPERTY()
    TArray<int32> ActiveScoutingUnits;

    // 網格設置
    UPROPERTY()
    int32 GridSizeX;

    UPROPERTY()
    int32 GridSizeY;

    UPROPERTY()
    float CellSize;

    // ??否已??始??
    UPROPERTY()
    bool bInitialized;

private:
    // ???X?迷??網X    void InitializeFogGrid();

    // ??新????視??
    void UpdateUnitVisionInternal(int32 UnitID};

    // 計??視?X???網格
    TArray<FIntPoint> CalculateVisibleGridCells(int32 UnitID};

    // ??新迷霧網格
    void UpdateFogGridCell(int32 GridX, int32 GridY, EMingFogLevel NewFogLevel, bool bIsScouted, int32 ScoutingUnitID};

    // 檢查??形??蔽
    bool CheckTerrainOcclusion(const FVector& From, const FVector& To};

    // 檢查建?X?遮X    bool CheckStructureOcclusion(const FVector& From, const FVector& To};

    // 計??視????    TArray<FVector> CalculateVisionLines(int32 UnitID};

    // ???X???任??
    void ProcessScoutingMission(int32 UnitID, float DeltaTime};

    // ???X??X???
    FMingScoutingReport GenerateScoutingReport(int32 UnitID, const FVector& Location, float Radius};

    // ??新??形信息
    void UpdateTerrainInfo(const FVector& Location};

    // 世??座??轉?X?網??座??    FIntPoint WorldToGrid(const FVector& WorldPosition) const;

    // 網格座??轉?X??X?座??    FVector GridToWorld(const FIntPoint& GridPosition) const;

    // 檢查網格座?X?否????
    bool IsValidGridPosition(const FIntPoint& GridPosition) const;

    // ????網格索??
    int32 GetGridIndex(int32 GridX, int32 GridY) const;

    // 計?X??X??X    float CalculateDistance(const FVector& Point1, const FVector& Point2) const;

    // 檢查視?X??X???
    void CheckUnitsInVision(int32 UnitID};

    // ???X??X?現/??失
    void ProcessUnitDiscovery(int32 ViewerID, int32 DiscoveredUnitID};
    void ProcessUnitLossOfSight(int32 ViewerID, int32 LostUnitID};

    // 驗??視?X???
    bool ValidateVisionData(const FMingVisionData& VisionData) const;

    // 清除???X?偵察信X    void CleanExpiredScoutingInfo();

    // ??新迷霧渲??
    void UpdateFogRendering();
};

