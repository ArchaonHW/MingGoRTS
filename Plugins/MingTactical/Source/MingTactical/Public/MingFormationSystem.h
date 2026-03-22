#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingFormationSystem.generated.h"

UENUM(BlueprintType)
enum class EMingFormationType : uint8
{
    Line UMETA(DisplayName = "Line"),
    Column UMETA(DisplayName = "Column"),
    Wedge UMETA(DisplayName = "Wedge"),
    Square UMETA(DisplayName = "Square"),
    Circle UMETA(DisplayName = "Circle"),
    Scattered UMETA(DisplayName = "Scattered")
};

USTRUCT(BlueprintType)
struct FMingFormationSlot
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 SlotIndex;

    UPROPERTY(BlueprintReadOnly)
    FVector RelativeOffset;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<class AMingTacticalUnit> AssignedUnit;

    UPROPERTY(BlueprintReadOnly)
    bool bIsOccupied;

    FMingFormationSlot()
        : SlotIndex(-1)
        , RelativeOffset(FVector::ZeroVector)
        , bIsOccupied(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingFormationData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMingFormationType FormationType;

    UPROPERTY(BlueprintReadOnly)
    FVector CenterPosition;

    UPROPERTY(BlueprintReadOnly)
    FVector FormationDirection;

    UPROPERTY(BlueprintReadOnly)
    float UnitSpacing;

    UPROPERTY(BlueprintReadOnly)
    TArray<FMingFormationSlot> FormationSlots;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxUnits;

    FMingFormationData()
        : FormationType(EMingFormationType::Line)
        , CenterPosition(FVector::ZeroVector)
        , FormationDirection(FVector::ForwardVector)
        , UnitSpacing(100.0f)
        , MaxUnits(0)
    {}
};

/**
 * Formation System for managing unit formations
 */
UCLASS(ClassGroup = (AI), Blueprintable)
class MINGTACTICAL_API UMingFormationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingFormationSystem();

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void InitializeFormation(EMingFormationType Type, FVector Center, FVector Direction, int32 UnitCount);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void UpdateFormationCenter(FVector NewCenter);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void UpdateFormationDirection(FVector NewDirection);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void AssignUnitToSlot(class AMingTacticalUnit* Unit, int32 SlotIndex);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void RemoveUnitFromFormation(class AMingTacticalUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void DisbandFormation();

    UFUNCTION(BlueprintPure, Category = "Formation")
    FVector GetSlotPosition(int32 SlotIndex) const;

    UFUNCTION(BlueprintPure, Category = "Formation")
    int32 FindBestSlotForUnit(class AMingTacticalUnit* Unit) const;

    UFUNCTION(BlueprintPure, Category = "Formation")
    bool HasAvailableSlots() const;

    UFUNCTION(BlueprintPure, Category = "Formation")
    int32 GetOccupiedSlotCount() const;

    UFUNCTION(BlueprintPure, Category = "Formation")
    const FMingFormationData& GetFormationData() const { return FormationData; }

    // Static formation generators
    UFUNCTION(BlueprintPure, Category = "Formation|Generators", meta = (StaticFunction = "true"))
    static TArray<FVector> GenerateLineFormation(int32 UnitCount, float Spacing);

    UFUNCTION(BlueprintPure, Category = "Formation|Generators", meta = (StaticFunction = "true"))
    static TArray<FVector> GenerateColumnFormation(int32 UnitCount, float Spacing);

    UFUNCTION(BlueprintPure, Category = "Formation|Generators", meta = (StaticFunction = "true"))
    static TArray<FVector> GenerateWedgeFormation(int32 UnitCount, float Spacing);

    UFUNCTION(BlueprintPure, Category = "Formation|Generators", meta = (StaticFunction = "true"))
    static TArray<FVector> GenerateSquareFormation(int32 UnitCount, float Spacing);

    UFUNCTION(BlueprintPure, Category = "Formation|Generators", meta = (StaticFunction = "true"))
    static TArray<FVector> GenerateCircleFormation(int32 UnitCount, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Formation")
    void RegenerateFormation();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Formation")
    FMingFormationData FormationData;

    // Not exposed to Blueprint as TWeakObjectPtr in TMap is not supported
    // 注意：TMap<TWeakObjectPtr> 不被 Blueprint 支持
    UPROPERTY()
    TMap<int32, TWeakObjectPtr<AMingTacticalUnit>> UnitSlotMap;

    void GenerateFormationSlots();
    void UpdateSlotPositions();
    
    UFUNCTION()
    void OnUnitDestroyed(AActor* DestroyedActor);
};
