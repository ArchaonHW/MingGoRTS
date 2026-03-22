#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/StaticMeshInstanceData.h"
#include "MingInstancedRenderingComponent.generated.h"

/**
 * æ¸²æ??¹æ¬¡çµæ?
 */
USTRUCT()
struct FRenderBatch
{
    GENERATED_BODY()

    // å¯¦ä??¸æ?
    TArray<FMatrix> InstanceTransforms;
    TArray<FLinearColor> InstanceColors;
    
    // ?¯è??§æ?è¨?    TArray<bool> InstanceVisibility;
    
    // ?¹æ¬¡ID
    int32 BatchId;
    
    // å¯¦ä??¸é?
    int32 InstanceCount;
    
    FRenderBatch()
        : BatchId(-1)
        , InstanceCount(0)
    {}
};

/**
 * LODç´šåˆ¥?šè?
 */
UENUM(BlueprintType)
enum class ELODLevel : uint8
{
    LOD0    UMETA(DisplayName = "LOD 0 - Full Detail"),      // å®Œæ•´ç´°ç?
    LOD1    UMETA(DisplayName = "LOD 1 - High Detail"),       // é«˜ç´°ç¯€
    LOD2    UMETA(DisplayName = "LOD 2 - Medium Detail"),   // ä¸­ç?ç´°ç?
    LOD3    UMETA(DisplayName = "LOD 3 - Low Detail"),       // ä½ç´°ç¯€
    LOD4    UMETA(DisplayName = "LOD 4 - Lowest Detail"),    // ?€ä½ç´°ç¯€
    Culled  UMETA(DisplayName = "Culled")                     // å®Œå…¨?”é™¤
};

class AMingTacticalUnit;

/**
 * å¯¦ä??–æ¸²?“ç?ä»? * ?¨æ–¼é«˜æ?æ¸²æ?å¤§é??¸å?é¡å??„å–®ä½? */
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
     * è¨»å??®ä??°å¯¦ä¾‹å?æ¸²æ?ç³»çµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void RegisterUnit(AMingTacticalUnit* Unit);

    /**
     * å¾å¯¦ä¾‹å?æ¸²æ?ç³»çµ±ç§»é™¤?®ä?
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void UnregisterUnit(AMingTacticalUnit* Unit);

    /**
     * ?´æ–°?®ä?è®Šæ??©é™£
     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void UpdateUnitTransform(AMingTacticalUnit* Unit, const FTransform& NewTransform);

    /**
     * ?´æ–°?®ä??¯è???     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void SetUnitVisibility(AMingTacticalUnit* Unit, bool bVisible);

    /**
     * è¨ˆç?LODç´šåˆ¥
     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    ELODLevel CalculateLODLevel(AMingTacticalUnit* Unit) const;

    /**
     * è¨­ç½®LODè·é›¢?¾å€?     */
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void SetLODDistanceThresholds(const TArray<float>& Thresholds);

    /**
     * ?²å??§èƒ½çµ±è?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetRegisteredUnitCount() const { return RegisteredUnits.Num(); }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetVisibleInstanceCount() const { return VisibleInstanceCount; }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetCulledInstanceCount() const { return CulledInstanceCount; }

    /**
     * å¼·åˆ¶?´æ–°?€?‰å¯¦ä¾?     */
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void ForceUpdateAllInstances();

    /**
     * è¨­ç½®?´æ–°?»ç?
     */
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetUpdateFrequency(float Frequency);

protected:
    // è¨»å??„å–®ä½?    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> RegisteredUnits;

    // ?®ä??°æ‰¹æ¬¡ç´¢å¼•ç?? å?
    TMap<AMingTacticalUnit*, int32> UnitToBatchIndex;

    // æ¸²æ??¹æ¬¡
    UPROPERTY()
    TArray<FRenderBatch> RenderBatches;

    // LODè·é›¢?¾å€?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    TArray<float> LODDistanceThresholds;

    // ?´æ–°è¨ˆæ???    float UpdateTimer;
    
    // ?´æ–°?»ç? (ç§?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float UpdateFrequency;

    // ?§èƒ½çµ±è?
    int32 VisibleInstanceCount;
    int32 CulledInstanceCount;
    int32 LastUpdatedInstanceCount;

    // ?¸æ?ä½ç½® (?¨æ–¼LODè¨ˆç?)
    FVector CameraLocation;

    // ?å??–LOD?¾å€?    void InitializeLODThresholds();

    // ?µå»º?–ç²?–æ‰¹æ¬?    int32 GetOrCreateBatchForUnit(AMingTacticalUnit* Unit);

    // ?´æ–°?¹æ¬¡å¯¦ä??¸æ?
    void UpdateBatchInstanceData(int32 BatchIndex);

    // ?·è?LODè¨ˆç?
    void PerformLODUpdate();

    // ?´æ–°å¯¦ä??¯è???    void UpdateInstanceVisibility();

    // ?²å??¸æ?ä½ç½®
    void UpdateCameraLocation();

    // ?¹æ¬¡?ˆä½µ (?ªå?å°æ‰¹æ¬?
    void MergeSmallBatches();

    // ?¹æ¬¡?†å‰² (?¿å?å¤§æ‰¹æ¬?
    void SplitLargeBatches();

    // ?€å¤§æ‰¹æ¬¡å¤§å°?    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MaxBatchSize;

    // ?€å°æ‰¹æ¬¡å¤§å°?(ä½æ–¼æ­¤å€¼è€ƒæ…®?ˆä½µ)
    UPROPERTY(EditAnywhere, Category = "Performance")
    int32 MinBatchSize;

    // ?¯å¦?Ÿç”¨LOD
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LOD")
    bool bEnableLOD;

    // ?¯å¦?Ÿç”¨?•æ??¹æ¬¡èª¿æ•´
    UPROPERTY(EditAnywhere, Category = "Performance")
    bool bEnableDynamicBatching;
};
