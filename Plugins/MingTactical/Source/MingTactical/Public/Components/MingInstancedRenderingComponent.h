#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMeshInstanceData.h"
#include "MingInstancedRenderingComponent.generated.h"

/**
 * 渲染批次結構
 */
USTRUCT()
struct FRenderBatch
{
    GENERATED_BODY()

    // 實例數據
    TArray<FMatrix> InstanceTransforms;
    TArray<FLinearColor> InstanceColors;
    
    // 可見性標記
    TArray<bool> InstanceVisibility;
    
    // 批次ID
    int32 BatchId;
    
    // 實例數量
    int32 InstanceCount;
    
    FRenderBatch()
        : BatchId(-1)
        , InstanceCount(0)
    {}
};

/**
 * LOD級別枚舉
 */
UENUM(BlueprintType)
enum class ELODLevel : uint8
{
    LOD0    UMETA(DisplayName = "LOD 0 - Full Detail"),      // 完整細節
    LOD1    UMETA(DisplayName = "LOD 1 - High Detail"),       // 高細節
    LOD2    UMETA(DisplayName = "LOD 2 - Medium Detail"),   // 中等細節
    LOD3    UMETA(DisplayName = "LOD 3 - Low Detail"),       // 低細節
    LOD4    UMETA(DisplayName = "LOD 4 - Lowest Detail"),    // 最低細節
    Culled  UMETA(DisplayName = "Culled")                     // 完全剔除
};

class AMingTacticalUnit;

/**
 * 實例化渲染組件
 * 用於高效渲染大量相同類型的單位
 */
UCLASS(ClassGroup=(Performance), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingInstancedRenderingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingInstancedRenderingComponent();

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * 註冊單位到實例化渲染系統
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void RegisterUnit(AMingTacticalUnit* Unit);

    /**
     * 從實例化渲染系統移除單位
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * 更新單位變換矩陣
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void UpdateUnitTransform(AMingTacticalUnit* Unit, const FTransform& NewTransform);

    /**
     * 更新單位可見性
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible);

    /**
     * 計算LOD級別
     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    ELODLevel CalculateLODLevel(AMingTacticalUnit* Unit) const;

    /**
     * 設置LOD距離閾值
     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void SetLODDistanceThresholds(const TArray<float>& Thresholds);

    /**
     * 獲取性能統計
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetRegisteredUnitCount() const { return RegisteredUnits.Num(); }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetVisibleInstanceCount() const { return VisibleInstanceCount; }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetCulledInstanceCount() const { return CulledInstanceCount; }

    /**
     * 強制更新所有實例
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void ForceUpdateAllInstances();

    /**
     * 設置更新頻率
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetUpdateFrequency(float Frequency);

protected:
    // 註冊的單位
    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> RegisteredUnits;

    // 單位到批次索引的映射
    TMap<AMingTacticalUnit*, int32> UnitToBatchIndex;

    // 渲染批次
    UPROPERTY()
    TArray<FRenderBatch> RenderBatches;

    // LOD距離閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    TArray<float> LODDistanceThresholds;

    // 更新計時器
    float UpdateTimer;
    
    // 更新頻率 (秒)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float UpdateFrequency;

    // 性能統計
    int32 VisibleInstanceCount;
    int32 CulledInstanceCount;
    int32 LastUpdatedInstanceCount;

    // 相機位置 (用於LOD計算)
    FVector CameraLocation;

    // 初始化LOD閾值
    void InitializeLODThresholds();

    // 創建或獲取批次
    int32 GetOrCreateBatchForUnit(AMingTacticalUnit* Unit);

    // 更新批次實例數據
    void UpdateBatchInstanceData(int32 BatchIndex);

    // 執行LOD計算
    void PerformLODUpdate();

    // 更新實例可見性
    void UpdateInstanceVisibility();

    // 獲取相機位置
    void UpdateCameraLocation();

    // 批次合併 (優化小批次)
    void MergeSmallBatches();

    // 批次分割 (避免大批次)
    void SplitLargeBatches();

    // 最大批次大小
    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MaxBatchSize;

    // 最小批次大小 (低於此值考慮合併)
    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MinBatchSize;

    // 是否啟用LOD
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    bool bEnableLOD;

    // 是否啟用動態批次調整
    UPROPERTY(EditAnywhere, Category = "Performance")
    bool bEnableDynamicBatching;
};
