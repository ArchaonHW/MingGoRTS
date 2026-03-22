#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSpatialPartition.generated.h"

class AMingTacticalUnit;

/**
 * 空間網格單元 | Spatial Grid Cell
 * 存儲位於該網格內的單位 | Store units located within this grid cell
 */
USTRUCT()
struct MINGTACTICAL_API FSpatialGridCell
{
    GENERATED_BODY()

    // 网格坐标
    UPROPERTY()
    FIntVector GridCoord;

    // 包含的单位
    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> Units;

    // 最后更新时间
    float LastUpdateTime = 0.0f;

    // 包围盒
    FBox Bounds;

    FMingSpatialGridCell() = default;
    explicit FMingSpatialGridCell(const FIntVector& InCoord) : GridCoord(InCoord) {}

    void AddUnit(AMingTacticalUnit* Unit);
    void RemoveUnit(AMingTacticalUnit* Unit);
    bool Contains(AMingTacticalUnit* Unit) const;
    void Clear();
    int32 GetUnitCount() const { return Units.Num(); }
};

/**
 * 空間查詢結果 | Spatial Query Result
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
 * 空間分區系統 | Spatial Partitioning System
 * 
 * 優化大規模單位場景的空間查詢性能： | Optimize spatial query performance for large-scale unit scenes:
 * - 單位位置追蹤 | Unit position tracking
 * - 範圍查詢 | Range queries
 * - 最近鄰查詢 | Nearest neighbor queries
 * - 視線查詢 | Line of sight queries
 * 
 * 適用於 1000+ 單位的大規模戰鬥場景 | Suitable for 1000+ unit large-scale combat scenarios
 */
UCLASS(ClassGroup = (Optimization), Blueprintable)
class MINGTACTICAL_API UMingSpatialPartition : public UObject
{
    GENERATED_BODY()

public:
    UMingSpatialPartition();

    /**
     * 初始化空间分区系统
     * @param WorldBounds 世界边界
     * @param CellSize 网格单元大小
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void Initialize(const FBox& WorldBounds, float CellSize = 1000.0f);

    /**
     * 关闭系统
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void Shutdown();

    /**
     * 注册单位到空间分区
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void RegisterUnit(AMingTacticalUnit* Unit);

    /**
     * 从空间分区注销单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * 更新单位位置
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UpdateUnitPosition(AMingTacticalUnit* Unit, const FVector& NewLocation);

    /**
     * 范围查询 - 获取指定范围内的所有单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult QueryUnitsInRadius(const FVector& Center, float Radius, int32 TeamId = -1);

    /**
     * 框查询 - 获取指定包围盒内的所有单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult QueryUnitsInBox(const FBox& Box, int32 TeamId = -1);

    /**
     * 获取最近的单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    AMingTacticalUnit* GetNearestUnit(const FVector& Location, float MaxRadius, int32 TeamId = -1);

    /**
     * 获取最近的敌方单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    AMingTacticalUnit* GetNearestEnemyUnit(AMingTacticalUnit* SourceUnit, float MaxRadius);

    /**
     * 获取范围内的敌方单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetEnemyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius);

    /**
     * 获取范围内的友方单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetFriendlyUnitsInRadius(AMingTacticalUnit* SourceUnit, float Radius);

    /**
     * 视线查询 - 检测两个位置间是否有单位阻挡
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    bool HasLineOfSight(const FVector& Start, const FVector& End, float CheckRadius = 50.0f);

    /**
     * 获取视线内的单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Queries")
    FSpatialQueryResult GetUnitsInLineOfSight(const FVector& Start, const FVector& End, float Width);

    /**
     * 获取指定网格单元内的单位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInCell(const FIntVector& GridCoord);

    /**
     * 获取单位所在的网格坐标
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector GetUnitGridCoord(AMingTacticalUnit* Unit) const;

    /**
     * 获取世界位置对应的网格坐标
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector WorldToGrid(const FVector& WorldLocation) const;

    /**
     * 获取网格坐标的世界中心位置
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FVector GridToWorld(const FIntVector& GridCoord) const;

    /**
     * 获取系统统计信息
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetTotalCellCount() const { return GridCells.Num(); }

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetOccupiedCellCount() const;

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    int32 GetTotalUnitCount() const { return TrackedUnits.Num(); }

    UFUNCTION(BlueprintPure, Category = "Spatial Partition|Stats")
    float GetAverageUnitsPerCell() const;

    /**
     * 调试绘制
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Debug")
    void DrawDebugVisualization(bool bDrawGrid = true, bool bDrawUnits = false);

    /**
     * 验证系统完整性
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition|Debug")
    bool ValidateIntegrity();

    /**
     * 清理无效引用
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void CleanupInvalidReferences();

    // ==== 批量操作 ====

    /**
     * 批量注册单位
     */
    void BatchRegisterUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 批量注销单位
     */
    void BatchUnregisterUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 批量更新位置
     */
    void BatchUpdatePositions(const TMap<AMingTacticalUnit*, FVector>& PositionUpdates);

private:
    // 网格单元映射
    UPROPERTY()
    TMap<FIntVector, FSpatialGridCell> GridCells;

    // 单位到网格坐标的映射
    UPROPERTY()
    TMap<TWeakObjectPtr<AMingTacticalUnit>, FIntVector> UnitToGridMap;

    // 被追踪的单位
    UPROPERTY()
    TArray<TWeakObjectPtr<AMingTacticalUnit>> TrackedUnits;

    // 配置
    float CellSize;
    FBox WorldBounds;
    bool bIsInitialized;

    // 统计
    int32 QueryCount;
    float TotalQueryTime;

    // 获取或创建网格单元
    FSpatialGridCell* GetOrCreateCell(const FIntVector& Coord);
    FSpatialGridCell* GetCell(const FIntVector& Coord);

    // 获取相邻网格坐标
    TArray<FIntVector> GetNeighboringCells(const FIntVector& Center, int32 Radius = 1);

    // 获取查询所需的网格范围
    TArray<FIntVector> GetCellsInRadius(const FVector& Center, float Radius);
    TArray<FIntVector> GetCellsInBox(const FBox& Box);

    // 清理空单元
    void CleanupEmptyCells();

    // 验证坐标是否在边界内
    bool IsValidGridCoord(const FIntVector& Coord) const;

    // 更新统计
    void RecordQueryTime(float TimeMs);
};
