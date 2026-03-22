#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingSpatialPartitionComponent.generated.h"

class AMingTacticalUnit;

/**
 * 空�?網格?��?結�?
 */
USTRUCT()
struct FMingComponentSpatialGridCell
{
    GENERATED_BODY()

    // 網格位置
    UPROPERTY()
    FIntVector GridPosition;

    // ?�含?�單�?    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> Units;

    // 活�?標�? (?�否?�要更X
    UPROPERTY()
    bool bIsActive;

    // ?�後更?��X    UPROPERTY()
    float LastUpdateTime;

    // ?�新?��?�?    UPROPERTY()
    float UpdatePriority;

    FSpatialGridCell()
        : GridPosition(FIntVector::ZeroValue)
        , bIsActive(false)
        , LastUpdateTime(0.0f)
        , UpdatePriority(0.0f)
    {}
};

/**
 * 空�X��?組件
 * 將戰?��X�為網格，實?��?塊更?��?視�X�除
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
     * 註�X��X�空?��?塊系�?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void RegisterUnit(AMingTacticalUnit* Unit);

    /**
     * 從空?��?塊系統移?�單�?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * ?�新?��?位置 (移�X�新網格)
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UpdateUnitPosition(AMingTacticalUnit* Unit);

    /**
     * ?��X��?位置?��X�單�?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInRadius(const FVector& Center, float Radius) const;

    /**
     * ?��X��?網格中�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInCell(const FIntVector& GridPosition) const;

    /**
     * ?��X��?視�X��?網格
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<FIntVector> GetVisibleGridCells() const;

    /**
     * 設置網格大�?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void SetCellSize(float NewCellSize);

    /**
     * 設置?�新範�?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void SetUpdateRadius(float NewRadius);

    /**
     * ?��X�能統�?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetTotalGridCellCount() const { return GridCells.Num(); }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetActiveGridCellCount() const;

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetRegisteredUnitCount() const { return RegisteredUnits.Num(); }

    /**
     * ?��?世�?位置對�X�網?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector WorldToGrid(const FVector& WorldPosition) const;

    /**
     * ?��?網格中�X��X��?�?     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FVector GridToWorld(const FIntVector& GridPosition) const;

    /**
     * 強制?�新?�?�網X     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void ForceUpdateAllCells();

    /**
     * ?��?網格調試資�?
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts) const;

protected:
    // 註�X�單�?    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> RegisteredUnits;

    // ?��X�網?��X��?
    TMap<AMingTacticalUnit*, FIntVector> UnitToCellMap;

    // 網格?��?
    UPROPERTY()
    TMap<FIntVector, FSpatialGridCell> GridCells;

    // 網格大�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Partition")
    float CellSize;

    // ?�新?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Partition")
    float UpdateRadius;

    // ?��?位置
    FVector CameraLocation;

    // ?��?視�?
    FRotator CameraRotation;

    // ?�新計�X    float UpdateTimer;
    
    // ?�新?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float UpdateFrequency;

    // ?�大�?幀?�新網格X    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    int32 MaxCellsPerFrame;

    // ?��X�新索�? (?�於?��X�新)
    int32 CurrentUpdateIndex;

    // ?�新?��?資�?
    void UpdateCameraInfo();

    // 確�?網格存在
    FSpatialGridCell& GetOrCreateCell(const FIntVector& GridPosition);

    // 從網?�移?�單�?    void RemoveUnitFromCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition);

    // 將單位添?�到網格
    void AddUnitToCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition);

    // ?�新網格活�X�X    void UpdateCellActivity();

    // ?�新活�?網格中�X��?
    void UpdateActiveCells(float DeltaTime);

    // ?��?視�X�除
    void PerformFrustumCulling();

    // 檢查網格?�否?��X�內
    bool IsCellInFrustum(const FIntVector& CellPosition) const;

    // ?��?視�X��?�?    void GetFrustumCorners(TArray<FVector>& OutCorners) const;

    // 計�?網格?�AABB
    FBox GetCellBounds(const FIntVector& CellPosition) const;

    // 距離?��?級�?�?    float CalculateCellPriority(const FIntVector& CellPosition) const;

    // ?��X�新?��?
    void BatchUpdateUnits(const TArray<AMingTacticalUnit*>& Units, float DeltaTime);

    // 休�?網格中�X��?
    void SleepUnitsInCell(const FIntVector& CellPosition);

    // ?��?網格中�X��?
    void WakeUnitsInCell(const FIntVector& CellPosition);
};

