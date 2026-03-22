#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSpatialPartition.generated.h"

class AMingTacticalUnit;

/**
 * 空�?網格?��? | Spatial Grid Cell
 * 存儲位於該網?�內?�單�?| Store units located within this grid cell
 */
USTRUCT()
struct MINGTACTICAL_API FSpatialGridCell
{
    GENERATED_BODY()

    // 网格?��?
    UPROPERTY()
    FIntVector GridCoord;

    // ?�含?��?�?    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> Units;

    // ?�?�更?�时X    float LastUpdateTime = 0.0f;

    // ?�围X    FBox Bounds;

    FMingSpatialGridCell() = default;
    explicit FMingSpatialGridCell(const FIntVector& InCoord) : GridCoord(InCoord) {}

    void AddUnit(AMingTacticalUnit* Unit);
    void RemoveUnit(AMingTacticalUnit* Unit);
    bool Contains(AMingTacticalUnit* Unit) const;
    void Clear();
    int32 GetUnitCount() const { return Units.Num(); }
};

/**
 * 空�X�詢結�? | Spatial Query Result
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FSpatialQueryResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<AMingTacticalUnit*> FoundUnits;

    UPROPERTY(BlueprintReadOnly)
    int32 CellsChecked = 0;

    UPROPERTY(BlueprintReadOnly)
    float QueryTimeMs = 0.0f;

    void AddUnit(AMingTacticalUnit* Unit);
};

/**
 * 空�X��?系統 | Spatial Partitioning System
 * 
 * ?��?大�?模單位場?��?空�X�詢?�能�?| Optimize spatial query performance for large-scale unit scenes:
 * - ?��?位置追蹤 | Unit position tracking
 * - 範�X�詢 | Range queries
 * - ?�近鄰?�詢 | Nearest neighbor queries
 * - 視�X�詢 | Line of sight queries
 * 
 * ?�用X1000+ ?��X�大規模?�鬥?�景 | Suitable for 1000+ unit large-scale combat scenarios
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingSpatialPartition : public UObject
{
    GENERATED_BODY()

public:
    UMingSpatialPartition();

    /**
     * ?��X�空?��X�系�?     * @param WorldBounds 世�?边�?
     * @param CellSize 网格?��?大�?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void Initialize(const FBox& WorldBounds, float CellSize = 1000.0f);

    /**
     * ?�闭系�?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void Shutdown();

    /**
     * 注�X��X�空?��X     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void RegisterUnit(AMingTacticalUnit* Unit);

    /**
     * 从空?��X�注?�?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * ?�新?��?位置
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UpdateUnitPosition(AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * ?�围?�询 - ?��X��X�围?��X�?��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult QueryUnitsInRadius(const FVector& Center, float Radius, int32 TeamId = -1);

    /**
     * 框查�?- ?��X��X�围?��X��X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult QueryUnitsInBox(const FBox& Box, int32 TeamId = -1);

    /**
     * ?��X�近�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    AMingTacticalUnit* GetNearestUnit(const FVector& Location, float MaxRadius, int32 TeamId = -1);

    /**
     * ?��X�近�X�方?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    AMingTacticalUnit* GetNearestEnemyUnit(AMingTacticalUnit* SourceUnit, float MaxRadius);

    /**
     * ?��X�围?��X�方?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetEnemyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius);

    /**
     * ?��X�围?��X�方?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetFriendlyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius);

    /**
     * 视线?�询 - 检测两个�?置间?�否?��?位阻X     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    bool HasLineOfSight(const FVector& Start, const FVector& End, float CheckRadius = 50.0f};

    /**
     * ?��?视线?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetUnitsInLineOfSight(const FVector& Start, const FVector& End, float Width};

    /**
     * ?��X��?网格?��X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInCell(const FIntVector& GridCoord};

    /**
     * ?��X��X�?��?网格?��?
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector GetUnitGridCoord(AMingTacticalUnit* Unit) const;

    /**
     * ?��?世�?位置对�X��X��X     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector WorldToGrid(const FVector& WorldLocation) const;

    /**
     * ?��?网格?��X��X�中心�?�?     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FVector GridToWorld(const FIntVector& GridCoord) const;

    /**
     * ?��?系�?统计信息
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetTotalCellCount() const { return GridCells.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetOccupiedCellCount() const;

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetTotalUnitCount() const { return TrackedUnits.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    float GetAverageUnitsPerCell() const;

    /**
     * 调�?绘制
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Debug")
    void DrawDebugVisualization(bool bDrawGrid = true, bool bDrawUnits = false};

    /**
     * 验�?系�?完整X     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Debug")
    bool ValidateIntegrity(};

    /**
     * 清�X��?引用
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void CleanupInvalidReferences(};

    // ==== ?��X��? ====

    /**
     * ?��?注�X��?
     */
    void BatchRegisterUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?��?注�X��?
     */
    void BatchUnregisterUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?��X�新位置
     */
    void BatchUpdatePositions(const TMap<AMingTacticalUnit*, FVector>& PositionUpdates};

private:
    // 网格?��X��?
    UPROPERTY()
    TMap<FIntVector, FSpatialGridCell> GridCells;

    // ?��X��X��X��X��?
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, FIntVector> UnitToGridMap;

    // 被追踪�X��?
    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> TrackedUnits;

    // ?�置
    float CellSize;
    FBox WorldBounds;
    bool bIsInitialized;

    // 统计
    int32 QueryCount;
    float TotalQueryTime;

    // ?��X��?建�X��X    FSpatialGridCell* GetOrCreateCell(const FIntVector& Coord};
    FSpatialGridCell* GetCell(const FIntVector& Coord};

    // ?��X�邻网格?��?
    TArray<FIntVector> GetNeighboringCells(const FIntVector& Center, int32 Radius = 1};

    // ?��X�询?�?�?��X��X    TArray<FIntVector> GetCellsInRadius(const FVector& Center, float Radius};
    TArray<FIntVector> GetCellsInBox(const FBox& Box};

    // 清�?空�X    void CleanupEmptyCells(};

    // 验�X��X�否?�边?��?
    bool IsValidGridCoord(const FIntVector& Coord) const;

    // ?�新统计
    void RecordQueryTime(float TimeMs};
};

