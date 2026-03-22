// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Unit Controller for Tactical Units

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MingUnitController.generated.h"

class AMingTacticalUnit;
class UMingTacticalAI;

UENUM(BlueprintType)
enum class EUnitControlMode: uint8 {
    Manual, UMETA(DisplayName = "Manual Control"),
    AI, UMETA(DisplayName = "AI Control"),
    Mixed, UMETA(DisplayName = "Mixed Control")
};

UENUM(BlueprintType)
enum class EUnitStance: uint8 {
    Aggressive, UMETA(DisplayName = "Aggressive"),
    Defensive, UMETA(DisplayName = "Defensive"),
    Hold, UMETA(DisplayName = "Hold Position"),
    Patrol, UMETA(DisplayName = "Patrol")
};

/**
 * MingGoRTS Unit Controller
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingUnitController : public APlayerController
{
    GENERATED_BODY()

public:
    AMingUnitController();

    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void SetControlMode(EUnitControlMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void SetUnitStance(EUnitStance Stance);

    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void SelectUnits(const TArray<AMingTacticalUnit*>& Units);

    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void MoveUnitsTo(const TArray<AMingTacticalUnit*>& Units, const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void AttackTarget(const TArray<AMingTacticalUnit*>& Units, AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Unit Controller")
    void SetPatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& Path);

    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    TArray<AMingTacticalUnit*> GetSelectedUnits() const;

    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    EUnitControlMode GetControlMode() const;

    UFUNCTION(BlueprintPure, Category = "Unit Controller")
    EUnitStance GetUnitStance() const;

protected:
    UPROPERTY()
    EUnitControlMode CurrentControlMode;

    UPROPERTY()
    EUnitStance CurrentStance;

    UPROPERTY()
    TArray<AMingTacticalUnit*> SelectedUnits;

    UPROPERTY()
    UMingTacticalAI* TacticalAI;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    void HandleUnitSelection();
    void HandleMovementCommand();
    void HandleAttackCommand();
    void UpdateAI();
    void UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions);
};
