#pragma once

#include "CoreMinimal.h"
#include "MingMemoryOptimization.h"
#include "MingPerformanceOptimization.h"
#include "MingOptimizedRTS.generated.h"

// ============================================================================
// RTS 單位優化
// ============================================================================

USTRUCT(BlueprintType)
struct FMingOptimizedUnitData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 UnitId;
    
    UPROPERTY(BlueprintReadOnly)
    int32 UnitType;
    
    // 壓縮位置
    UPROPERTY(BlueprintReadOnly)
    FMingCompressedVector Location;
    
    UPROPERTY(BlueprintReadOnly)
    FMingCompressedRotator Rotation;
    
    // 打包健康值 (0-255)
    UPROPERTY(BlueprintReadOnly)
    uint8 Health_Packed;
    
    // 打包能量值
    UPROPERTY(BlueprintReadOnly)
    uint8 Energy_Packed;
    
    // 打包等級 (0-255)
    UPROPERTY(BlueprintReadOnly)
    uint8 Level;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 Experience_Packed;
    
    // 狀態標誌 (位元封裝)
    UPROPERTY(BlueprintReadOnly)
    uint8 StatusFlags;
    
    UPROPERTY(BlueprintReadOnly)
    int32 OwnerPlayerId;
    
    FMingOptimizedUnitData()
        : UnitId(0)
        , UnitType(0)
        , Health_Packed(255)
        , Energy_Packed(255)
        , Level(1)
        , Experience_Packed(0)
        , StatusFlags(0)
        , OwnerPlayerId(0)
    {}
    
    float GetHealth() const { return Health_Packed / 255.0f; }
    void SetHealth(float Value) { Health_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetEnergy() const { return Energy_Packed / 255.0f; }
    void SetEnergy(float Value) { Energy_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetExperience() const { return Experience_Packed / 255.0f; }
    void SetExperience(float Value) { Experience_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    bool IsAlive() const { return (StatusFlags & 0x01) != 0; }
    void SetAlive(bool bAlive) { if (bAlive) StatusFlags |= 0x01; else StatusFlags &= ~0x01; }
    
    bool IsSelected() const { return (StatusFlags & 0x02) != 0; }
    void SetSelected(bool bSelected) { if (bSelected) StatusFlags |= 0x02; else StatusFlags &= ~0x02; }
    
    bool IsMoving() const { return (StatusFlags & 0x04) != 0; }
    void SetMoving(bool bMoving) { if (bMoving) StatusFlags |= 0x04; else StatusFlags &= ~0x04; }
};

// ============================================================================
// RTS 單位管理器
// ============================================================================

UCLASS()
class MINGGORTS_API UMingOptimizedUnitManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    void Initialize(int32 MaxUnits);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    int32 SpawnUnit(int32 UnitType, FVector Location, int32 OwnerPlayerId);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    void DestroyUnit(int32 UnitId);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    void UpdateUnitPosition(int32 UnitId, FVector NewLocation, FRotator NewRotation);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    void UpdateUnitHealth(int32 UnitId, float Health);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    FMingOptimizedUnitData* GetUnitData(int32 UnitId);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    TArray<int32> GetUnitsInRadius(FVector Center, float Radius) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    TArray<int32> GetUnitsByPlayer(int32 PlayerId) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    void BatchUpdatePositions(const TArray<int32>& UnitIds, const TArray<FVector>& NewLocations);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    int32 GetActiveUnitCount() const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Units")
    void ProcessUnitAI(float DeltaTime);

private:
    struct FUnitSlot
    {
        FMingOptimizedUnitData Data;
        bool bInUse;
        float LastUpdateTime;
    };
    
    TMingFixedArray<FUnitSlot, 512> UnitSlots;
    TMingFixedArray<int32, 512> AvailableUnitIds;
    TUniformGridSpatialPartition<int32> SpatialPartition;
    
    int32 ActiveUnitCount;
    
    void ProcessUnitBehavior(int32 UnitId, float DeltaTime);
    void UpdateSpatialPartition(int32 UnitId);
};

// ============================================================================
// RTS 資源管理優化
// ============================================================================

USTRUCT(BlueprintType)
struct FMingOptimizedResourceNode
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 NodeId;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ResourceType;
    
    UPROPERTY(BlueprintReadOnly)
    FMingCompressedVector Location;
    
    // 打包資源量
    UPROPERTY(BlueprintReadOnly)
    uint16 Amount_Packed;
    
    // 最大資源量
    UPROPERTY(BlueprintReadOnly)
    uint16 MaxAmount;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 DepletionLevel;  // 0-255
    
    FMingOptimizedResourceNode()
        : NodeId(0)
        , ResourceType(0)
        , Amount_Packed(1000)
        , MaxAmount(1000)
        , DepletionLevel(0)
    {}
    
    int32 GetAmount() const { return Amount_Packed; }
    void SetAmount(int32 Value) { Amount_Packed = FMath::Clamp(Value, 0, 65535); }
    
    float GetDepletionRatio() const { return DepletionLevel / 255.0f; }
};

UCLASS()
class MINGGORTS_API UMingOptimizedResourceManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RTS|Resources")
    void InitializeResourceSystem();
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Resources")
    void RegisterResourceNode(int32 NodeId, int32 ResourceType, FVector Location, int32 InitialAmount);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Resources")
    int32 HarvestResource(int32 NodeId, int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Resources")
    TArray<int32> FindNearestResourceNodes(FVector Location, int32 ResourceType, int32 MaxResults) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Resources")
    FMingOptimizedResourceNode* GetResourceNode(int32 NodeId);

private:
    TMingFixedArray<FMingOptimizedResourceNode, 256> ResourceNodes;
    TUniformGridSpatialPartition<int32> ResourceSpatialGrid;
};

// ============================================================================
// RTS 路徑尋找優化
// ============================================================================

/**
 * 流場 (Flow Field) 路徑尋找
 * 用於大量單位移動的高效路徑尋找
 */
UCLASS()
class MINGGORTS_API UMingFlowFieldPathfinder : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RTS|Pathfinding")
    void Initialize(int32 GridWidth, int32 GridHeight, float CellSize);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Pathfinding")
    void GenerateFlowField(FVector TargetLocation);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Pathfinding")
    FVector GetFlowDirection(FVector Location) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Pathfinding")
    bool IsValidLocation(FVector Location) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Pathfinding")
    void SetObstacle(FVector Location, bool bIsObstacle);

private:
    struct FFlowFieldCell
    {
        uint16 Cost;
        uint16 BestCost;
        FVector FlowDirection;
        bool bIsObstacle;
    };
    
    TArray<FFlowFieldCell> Grid;
    int32 GridWidth;
    int32 GridHeight;
    float CellSize;
    FVector GridOrigin;
    
    int32 GetCellIndex(FVector Location) const;
    void ComputeIntegrationField(FVector TargetLocation);
    void ComputeFlowField();
};

// ============================================================================
// RTS 視野系統優化
// ============================================================================

UCLASS()
class MINGGORTS_API UMingOptimizedFogOfWar : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RTS|FogOfWar")
    void Initialize(int32 MapWidth, int32 MapHeight);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|FogOfWar")
    void UpdateVisibility(int32 PlayerId, FVector Location, float SightRadius);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|FogOfWar")
    void ClearVisibility(int32 PlayerId);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|FogOfWar")
    bool IsLocationVisible(int32 PlayerId, FVector Location) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|FogOfWar")
    uint8 GetVisibilityLevel(int32 PlayerId, FVector Location) const;

private:
    struct FFogOfWarData
    {
        TArray<uint8> VisibilityGrid;  // 0 = 未探索, 1-255 = 可見度
        int32 GridWidth;
        int32 GridHeight;
    };
    
    TMap<int32, FFogOfWarData> PlayerFogData;
    
    int32 WorldToGridX(FVector Location) const;
    int32 WorldToGridY(FVector Location) const;
    void UpdateVisibilityCircle(FFogOfWarData& FogData, int32 CenterX, int32 CenterY, int32 Radius, uint8 Visibility);
};

// ============================================================================
// RTS 戰鬥系統優化
// ============================================================================

USTRUCT(BlueprintType)
struct FMingOptimizedCombatEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 AttackerId;
    
    UPROPERTY(BlueprintReadOnly)
    int32 DefenderId;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 Damage_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 AttackType;
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsCritical;
    
    FMingOptimizedCombatEvent()
        : AttackerId(0)
        , DefenderId(0)
        , Damage_Packed(0)
        , AttackType(0)
        , Timestamp(0.0f)
        , bIsCritical(false)
    {}
    
    int32 GetDamage() const { return Damage_Packed; }
    void SetDamage(int32 Value) { Damage_Packed = FMath::Clamp(Value, 0, 255); }
};

UCLASS()
class MINGGORTS_API UMingOptimizedCombatManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RTS|Combat")
    void Initialize();
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Combat")
    void ProcessCombatEvent(const FMingOptimizedCombatEvent& Event);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Combat")
    void BatchProcessCombat(const TArray<FMingOptimizedCombatEvent>& Events);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Combat")
    TArray<int32> FindTargetsInRange(int32 AttackerId, float AttackRange) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Combat")
    int32 SelectBestTarget(int32 AttackerId, const TArray<int32>& PotentialTargets) const;

private:
    TMingFixedArray<FMingOptimizedCombatEvent, 128> PendingCombatEvents;
    
    void ApplyDamage(int32 UnitId, int32 Damage);
    float CalculateThreatLevel(int32 UnitId) const;
};

// ============================================================================
// RTS 建築系統優化
// ============================================================================

USTRUCT(BlueprintType)
struct FMingOptimizedBuildingData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 BuildingId;
    
    UPROPERTY(BlueprintReadOnly)
    int32 BuildingType;
    
    UPROPERTY(BlueprintReadOnly)
    FMingCompressedVector Location;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 Health_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 ConstructionProgress_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    uint8 StatusFlags;
    
    UPROPERTY(BlueprintReadOnly)
    int32 OwnerPlayerId;
    
    FMingOptimizedBuildingData()
        : BuildingId(0)
        , BuildingType(0)
        , Health_Packed(255)
        , ConstructionProgress_Packed(255)
        , StatusFlags(0)
        , OwnerPlayerId(0)
    {}
    
    float GetHealth() const { return Health_Packed / 255.0f; }
    void SetHealth(float Value) { Health_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetConstructionProgress() const { return ConstructionProgress_Packed / 255.0f; }
    void SetConstructionProgress(float Value) { ConstructionProgress_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
};

UCLASS()
class MINGGORTS_API UMingOptimizedBuildingManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RTS|Buildings")
    void Initialize(int32 MaxBuildings);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Buildings")
    int32 PlaceBuilding(int32 BuildingType, FVector Location, int32 OwnerPlayerId);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Buildings")
    void UpdateConstructionProgress(int32 BuildingId, float ProgressDelta);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Buildings")
    void CompleteBuilding(int32 BuildingId);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Buildings")
    void DestroyBuilding(int32 BuildingId);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Buildings")
    bool CanPlaceBuildingAt(int32 BuildingType, FVector Location) const;
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Buildings")
    TArray<int32> GetBuildingsByPlayer(int32 PlayerId) const;

private:
    TMingFixedArray<FMingOptimizedBuildingData, 128> Buildings;
    TUniformGridSpatialPartition<int32> BuildingSpatialGrid;
};

// ============================================================================
// RTS 整體優化管理器
// ============================================================================

UCLASS()
class MINGGORTS_API UMingRTSOptimizationManager : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "RTS|Optimization")
    void InitializeRTSOptimization();
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Optimization")
    void Update(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Optimization")
    void EnableLODSystem(bool bEnable);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Optimization")
    void SetVisibilityCullingDistance(float Distance);
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Optimization")
    void PerformMemoryCleanup();
    
    UFUNCTION(BlueprintCallable, Category = "RTS|Optimization")
    FString GetOptimizationReport() const;

private:
    UPROPERTY()
    UMingOptimizedUnitManager* UnitManager;
    
    UPROPERTY()
    UMingOptimizedResourceManager* ResourceManager;
    
    UPROPERTY()
    UMingFlowFieldPathfinder* Pathfinder;
    
    UPROPERTY()
    UMingOptimizedFogOfWar* FogOfWar;
    
    UPROPERTY()
    UMingOptimizedCombatManager* CombatManager;
    
    UPROPERTY()
    UMingOptimizedBuildingManager* BuildingManager;
    
    float LODDistance;
    bool bLODEnabled;
    
    void UpdateUnitLOD();
    void UpdateBuildingLOD();
    void CullDistantObjects();
};
