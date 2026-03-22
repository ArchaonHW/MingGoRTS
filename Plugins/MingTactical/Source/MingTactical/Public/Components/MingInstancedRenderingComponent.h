#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMeshInstanceData.h"
#include "MingInstancedRenderingComponent.generated.h"

/**
 * 渲�X�次結�?
 */
USTRUCT()
struct FRenderBatch
{
    GENERATED_BODY()

    // 實�X��?
    TArray<FMatrix> InstanceTransforms;
    TArray<FLinearColor> InstanceColors;
    
    // ?��X��?�?    TArray<bool> InstanceVisibility;
    
    // ?�次ID
    int32 BatchId;
    
    // 實�X��?
    int32 InstanceCount;
    
    FRenderBatch()
        : BatchId(-1)
        , InstanceCount(0)
    {}
};

/**
 * LOD級別?��?
 */
UENUM(BlueprintType)
enum class ELODLevel : uint8
{
    LOD0    UMETA(DisplayName = "LOD 0 - Full Detail"),      // 完整細�?
    LOD1    UMETA(DisplayName = "LOD 1 - High Detail"),       // 高細節
    LOD2    UMETA(DisplayName = "LOD 2 - Medium Detail"),   // 中�?細�?
    LOD3    UMETA(DisplayName = "LOD 3 - Low Detail"),       // 低細節
    LOD4    UMETA(DisplayName = "LOD 4 - Lowest Detail"),    // ?�低細節
    Culled  UMETA(DisplayName = "Culled")                     // 完全?�除
};

class AMingTacticalUnit;

/**
 * 實�X�渲?��?�? * ?�於高�?渲�?大�X��?類�X�單�? */
UCLASS(ClassGroup=(Performance), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingInstancedRenderingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingInstancedRenderingComponent(};

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * 註�X��X�實例�?渲�?系統
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void RegisterUnit(AMingTacticalUnit* Unit};

    /**
     * 從實例�?渲�?系統移除?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void UnregisterUnit(AMingTacticalUnit* Unit};

    /**
     * ?�新?��?變�X�陣
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void UpdateUnitTransform(AMingTacticalUnit* Unit, const FTransform& NewTransform};

    /**
     * ?�新?��X��X     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible};

    /**
     * 計�?LOD級別
     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    ELODLevel CalculateLODLevel(AMingTacticalUnit* Unit) const;

    /**
     * 設置LOD距離?��?     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void SetLODDistanceThresholds(const TArray<float>& Thresholds};

    /**
     * ?��X�能統�?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetRegisteredUnitCount() const { return RegisteredUnits.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetVisibleInstanceCount() const { return VisibleInstanceCount; }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetCulledInstanceCount() const { return CulledInstanceCount; }

    /**
     * 強制?�新?�?�實�?     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void ForceUpdateAllInstances(};

    /**
     * 設置?�新?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetUpdateFrequency(float Frequency};

protected:
    // 註�X�單�?    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> RegisteredUnits;

    // ?��X�批次索引�X��?
    TMap<AMingTacticalUnit*, int32> UnitToBatchIndex;

    // 渲�X�次
    UPROPERTY()
    TArray<FRenderBatch> RenderBatches;

    // LOD距離?��?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    TArray<float> LODDistanceThresholds;

    // ?�新計�X    float UpdateTimer;
    
    // ?�新?��? (�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float UpdateFrequency;

    // ?�能統�?
    int32 VisibleInstanceCount;
    int32 CulledInstanceCount;
    int32 LastUpdatedInstanceCount;

    // ?��?位置 (?�於LOD計�?)
    FVector CameraLocation;

    // ?��X�LOD?��?    void InitializeLODThresholds(};

    // ?�建?�獲?�批�?    int32 GetOrCreateBatchForUnit(AMingTacticalUnit* Unit};

    // ?�新?�次實�X��?
    void UpdateBatchInstanceData(int32 BatchIndex};

    // ?��?LOD計�?
    void PerformLODUpdate(};

    // ?�新實�X��X    void UpdateInstanceVisibility(};

    // ?��X��?位置
    void UpdateCameraLocation(};

    // ?�次?�併 (?��?小批�?
    void MergeSmallBatches(};

    // ?�次?�割 (?��?大批�?
    void SplitLargeBatches(};

    // ?�大批次大�?    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MaxBatchSize;

    // ?�小批次大�?(低於此值考慮?�併)
    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MinBatchSize;

    // ?�否?�用LOD
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    bool bEnableLOD;

    // ?�否?�用?��X�次調整
    UPROPERTY(EditAnywhere, Category = "Performance")
    bool bEnableDynamicBatching;
};

