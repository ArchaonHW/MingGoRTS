#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingSpatialPartitionComponent.generated.h"

class AMingTacticalUnit;

/**
 * Á©∫È?Á∂≤Ê†º?ÆÂ?ÁµêÊ?
 */
USTRUCT()
struct FMingComponentSpatialGridCell
{
    GENERATED_BODY()

    // Á∂≤Ê†º‰ΩçÁΩÆ
    UPROPERTY()
    FIntVector GridPosition;

    // ?ÖÂê´?ÑÂñÆ‰Ω?    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> Units;

    // Ê¥ªË?Ê®ôË? (?ØÂê¶?ÄË¶ÅÊõ¥??
    UPROPERTY()
    bool bIsActive;

    // ?ÄÂæåÊõ¥?∞Ê???    UPROPERTY()
    float LastUpdateTime;

    // ?¥Êñ∞?™Â?Á¥?    UPROPERTY()
    float UpdatePriority;

    FSpatialGridCell()
        : GridPosition(FIntVector::ZeroValue)
        , bIsActive(false)
        , LastUpdateTime(0.0f)
        , UpdatePriority(0.0f)
    {}
};

/**
 * Á©∫È??ÜÂ?ÁµÑ‰ª∂
 * Â∞áÊà∞?¥Â??ÜÁÇ∫Á∂≤Ê†ºÔºåÂØ¶?æÂ?Â°äÊõ¥?∞Â?Ë¶ñÈ??îÈô§
 */
UCLASS(ClassGroup=(Performance), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingSpatialPartitionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingSpatialPartitionComponent(};

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * Ë®ªÂ??Æ‰??∞Á©∫?ìÂ?Â°äÁ≥ªÁµ?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void RegisterUnit(AMingTacticalUnit* Unit};

    /**
     * ÂæûÁ©∫?ìÂ?Â°äÁ≥ªÁµ±Áßª?§ÂñÆ‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UnregisterUnit(AMingTacticalUnit* Unit};

    /**
     * ?¥Êñ∞?Æ‰?‰ΩçÁΩÆ (ÁßªÂ??∞Êñ∞Á∂≤Ê†º)
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void UpdateUnitPosition(AMingTacticalUnit* Unit};

    /**
     * ?≤Â??áÂ?‰ΩçÁΩÆ?®Â??ÑÂñÆ‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInRadius(const FVector& Center, float Radius) const;

    /**
     * ?≤Â??áÂ?Á∂≤Ê†º‰∏≠Á??Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<AMingTacticalUnit*> GetUnitsInCell(const FIntVector& GridPosition) const;

    /**
     * ?≤Â??∏Ê?Ë¶ñÈ??ßÁ?Á∂≤Ê†º
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    TArray<FIntVector> GetVisibleGridCells() const;

    /**
     * Ë®≠ÁΩÆÁ∂≤Ê†ºÂ§ßÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void SetCellSize(float NewCellSize};

    /**
     * Ë®≠ÁΩÆ?¥Êñ∞ÁØÑÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void SetUpdateRadius(float NewRadius};

    /**
     * ?≤Â??ßËÉΩÁµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetTotalGridCellCount() const { return GridCells.Num(}; }

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetActiveGridCellCount() const;

    UFUNCTION(BlueprintPure, Category = "Performance")
    int32 GetRegisteredUnitCount() const { return RegisteredUnits.Num(}; }

    /**
     * ?≤Â?‰∏ñÁ?‰ΩçÁΩÆÂ∞çÊ??ÑÁ∂≤?ºÂ?Ê®?     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FIntVector WorldToGrid(const FVector& WorldPosition) const;

    /**
     * ?≤Â?Á∂≤Ê†º‰∏≠Â??Ñ‰??å‰?ÁΩ?     */
    UFUNCTION(BlueprintPure, Category = "Spatial Partition")
    FVector GridToWorld(const FIntVector& GridPosition) const;

    /**
     * Âº∑Âà∂?¥Êñ∞?Ä?âÁ∂≤??     */
    UFUNCTION(BlueprintCallable, Category = "Spatial Partition")
    void ForceUpdateAllCells(};

    /**
     * ?≤Â?Á∂≤Ê†ºË™øË©¶Ë≥áË?
     */
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void GetDebugGridInfo(TArray<FVector>& CellCenters, TArray<int32>& CellUnitCounts) const;

protected:
    // Ë®ªÂ??ÑÂñÆ‰Ω?    UPROPERTY()
    TArray<TObjectPtr<AMingTacticalUnit>> RegisteredUnits;

    // ?Æ‰??∞Á∂≤?ºÁ??†Â?
    TMap<AMingTacticalUnit*, FIntVector> UnitToCellMap;

    // Á∂≤Ê†º?∏Ê?
    UPROPERTY()
    TMap<FIntVector, FSpatialGridCell> GridCells;

    // Á∂≤Ê†ºÂ§ßÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Partition")
    float CellSize;

    // ?¥Êñ∞?äÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spatial Partition")
    float UpdateRadius;

    // ?∏Ê?‰ΩçÁΩÆ
    FVector CameraLocation;

    // ?∏Ê?Ë¶ñÈ?
    FRotator CameraRotation;

    // ?¥Êñ∞Ë®àÊ???    float UpdateTimer;
    
    // ?¥Êñ∞?ªÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    float UpdateFrequency;

    // ?ÄÂ§ßÊ?ÂπÄ?¥Êñ∞Á∂≤Ê†º??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Performance")
    int32 MaxCellsPerFrame;

    // ?∂Â??¥Êñ∞Á¥¢Â? (?®Êñº?ÜÂ??¥Êñ∞)
    int32 CurrentUpdateIndex;

    // ?¥Êñ∞?∏Ê?Ë≥áË?
    void UpdateCameraInfo(};

    // Á¢∫‰?Á∂≤Ê†ºÂ≠òÂú®
    FSpatialGridCell& GetOrCreateCell(const FIntVector& GridPosition};

    // ÂæûÁ∂≤?ºÁßª?§ÂñÆ‰Ω?    void RemoveUnitFromCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition};

    // Â∞áÂñÆ‰ΩçÊ∑ª?†Âà∞Á∂≤Ê†º
    void AddUnitToCell(AMingTacticalUnit* Unit, const FIntVector& CellPosition};

    // ?¥Êñ∞Á∂≤Ê†ºÊ¥ªË??Ä??    void UpdateCellActivity(};

    // ?¥Êñ∞Ê¥ªË?Á∂≤Ê†º‰∏≠Á??Æ‰?
    void UpdateActiveCells(float DeltaTime};

    // ?∑Ë?Ë¶ñÈ??îÈô§
    void PerformFrustumCulling(};

    // Ê™¢Êü•Á∂≤Ê†º?ØÂê¶?®Ë??êÂÖß
    bool IsCellInFrustum(const FIntVector& CellPosition) const;

    // ?≤Â?Ë¶ñÈ????ãË?Èª?    void GetFrustumCorners(TArray<FVector>& OutCorners) const;

    // Ë®àÁ?Á∂≤Ê†º?ÑAABB
    FBox GetCellBounds(const FIntVector& CellPosition) const;

    // Ë∑ùÈõ¢?™Â?Á¥öË?ÁÆ?    float CalculateCellPriority(const FIntVector& CellPosition) const;

    // ?πÈ??¥Êñ∞?Æ‰?
    void BatchUpdateUnits(const TArray<AMingTacticalUnit*>& Units, float DeltaTime};

    // ‰ºëÁ?Á∂≤Ê†º‰∏≠Á??Æ‰?
    void SleepUnitsInCell(const FIntVector& CellPosition};

    // ?öÈ?Á∂≤Ê†º‰∏≠Á??Æ‰?
    void WakeUnitsInCell(const FIntVector& CellPosition};
};

