#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingSpatialPartitionComponent.generated.h"

class AMingTacticalUnit;

/**
 * 空間網格單元結構
 */
USTRUCT()
struct FSpatialGridCell
{
    GENERATED_BODY()

    // 網格位置
    UPROPERTY()
    FIntVector GridPosition;

    // 包含的單位
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> Units;

    // 活躍標記 (是否需要更新)
    UPROPERTY()
    bool bIsActive;

    // 最後更新時間
    UPROPERTY()
    float LastUpdateTime;

    // 更新優先級
    UPROPERTY()
    float UpdatePriority;

    FSpatialGridCell()
        : GridPosition(FIntVector::ZeroValue)
        , bIsActive(false)
        , LastUpdateTime(0.0f)
        , UpdatePriority(0.0f)
    {}
};

/**
 * 空間分塊組件
 * 將戰場劃分為網格，實現分塊更新和視錐剔除
 */
UCLASS(ClassGroup=(Performance), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingSpatialPartitionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingSpatialPartitionComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * 註冊單位到空間分塊系統
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void RegisterUnit(AMingTacticalUnit* Unit);

    /**
     * 從空間分塊系統移除單位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * 更新單位位置 (移動到新網格)
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UpdateUnitPosition(AMingTacticalUnit* Unit);

    /**
     * 獲取指定位置周圍的單位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInRadius(const FVector& Center, float Radius) const;

    /**
     * 獲取指定網格中的單位
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInCell(const FIntVector& GridPosition) const;

    /**
     * 獲取相機視錐內的網格
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<FIntVector> GetVisibleGridCells() const;

    /**
     * 設置網格大小
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void SetCellSize(float NewCellSize);

    /**
     * 設置更新範圍
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void SetUpdateRadius(float NewRadius);

    /**
     * 獲取性能統計
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetTotalGridCellCount() const { return GridCells.Num(); }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetActiveGridCellCount() const;

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetRegisteredUnitCount() const { return RegisteredUnits.Num(); }

    /**
     * 獲取世界位置對應的網格坐標
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector WorldToGrid(const FVector& WorldPosition) const;

    /**
     * 獲取網格中心的世界位置
     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FVector GridToWorld(const FIntVector& GridPosition) const;

    /**
     * 強制更新所有網格
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void ForceUpdateAllCells();

    /**
     * 獲取網格調試資訊
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts) const;

protected:
    // 註冊的單位
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> RegisteredUnits;

    // 單位到網格的映射
    TMap<AMingTacticalUnit*, FIntVector> UnitToCellMap;

    // 網格數據
    UPROPERTY()
    TMap<FIntVector, FSpatialGridCell> GridCells;

    // 網格大小
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Partition")
    float CellSize;

    // 更新半徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Partition")
    float UpdateRadius;

    // 相機位置
    FVector CameraLocation;

    // 相機視錐
    FRotator CameraRotation;

    // 更新計時器
    float UpdateTimer;
    
    // 更新頻率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float UpdateFrequency;

    // 最大每幀更新網格數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    int32 MaxCellsPerFrame;

    // 當前更新索引 (用於分幀更新)
    int32 CurrentUpdateIndex;

    // 更新相機資訊
    void UpdateCameraInfo();

    // 確保網格存在
    FSpatialGridCell& GetOrCreateCell(const FIntVector& GridPosition);

    // 從網格移除單位
    void RemoveUnitFromCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition);

    // 將單位添加到網格
    void AddUnitToCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition);

    // 更新網格活躍狀態
    void UpdateCellActivity();

    // 更新活躍網格中的單位
    void UpdateActiveCells(float DeltaTime);

    // 執行視錐剔除
    void PerformFrustumCulling();

    // 檢查網格是否在視錐內
    bool IsCellInFrustum(const FIntVector& CellPosition) const;

    // 獲取視錐的8個角點
    void GetFrustumCorners(TArray<FVector>& OutCorners) const;

    // 計算網格的AABB
    FBox GetCellBounds(const FIntVector& CellPosition) const;

    // 距離優先級計算
    float CalculateCellPriority(const FIntVector& CellPosition) const;

    // 批量更新單位
    void BatchUpdateUnits(const TArray<AMingTacticalUnit*>& Units, float DeltaTime);

    // 休眠網格中的單位
    void SleepUnitsInCell(const FIntVector& CellPosition);

    // 喚醒網格中的單位
    void WakeUnitsInCell(const FIntVector& CellPosition);
};
