// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Base Combat AI System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBaseCombatAI.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
    Idle, UMETA(DisplayName = "Idle"),
    Attacking, UMETA(DisplayName = "Attacking"),
    Defending, UMETA(DisplayName = "Defending"),
    Retreating, UMETA(DisplayName = "Retreating"),
    Pursuing, UMETA(DisplayName = "Pursuing")
};

UENUM(BlueprintType)
enum class ECombatRole : uint8
{
    Frontline, UMETA(DisplayName = "Frontline"),
    Support, UMETA(DisplayName = "Support"),
    Ranged, UMETA(DisplayName = "Ranged"),
    Flanker, UMETA(DisplayName = "Flanker"),
    Commander, UMETA(DisplayName = "Commander")
};

USTRUCT(BlueprintType)
struct FMingAIContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Context")
    FVector CurrentPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AI Context")
    FVector TargetPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AI Context")
    float HealthPercentage;

    UPROPERTY(BlueprintReadOnly, Category = "AI Context")
    int32 AllEnemiesCount;

    UPROPERTY(BlueprintReadOnly, Category = "AI Context")
    int32 NearbyAlliesCount;

    FMingAIContext()
        : CurrentPosition(FVector::ZeroVector)
        , TargetPosition(FVector::ZeroVector)
        , HealthPercentage(1.0f)
        , AllEnemiesCount(0)
        , NearbyAlliesCount(0)
    {}
};

USTRUCT(BlueprintType)
struct FMingTacticalDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Tactical Decision")
    ECombatState RecommendedState;

    UPROPERTY(BlueprintReadOnly, Category = "Tactical Decision")
    FVector RecommendedPosition;

    UPROPERTY(BlueprintReadOnly, Category = "Tactical Decision")
    AActor* RecommendedTarget;

    UPROPERTY(BlueprintReadOnly, Category = "Tactical Decision")
    float Confidence;

    FMingTacticalDecision()
        : RecommendedState(ECombatState::Idle)
        , RecommendedPosition(FVector::ZeroVector)
        , RecommendedTarget(nullptr)
        , Confidence(0.0f)
    {}
};

/**
 * MingGoRTS Base Combat AI
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingBaseCombatAI : public UObject
{
    GENERATED_BODY()

public:
    UMingBaseCombatAI();

    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    virtual void Initialize(class AController* Controller);

    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    virtual void UpdateAI(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context);

    UFUNCTION(BlueprintPure, Category = "Combat AI")
    ECombatState GetCurrentCombatState() const;

    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetCombatRole(ECombatRole Role);

    UFUNCTION(BlueprintPure, Category = "Combat AI")
    ECombatRole GetCombatRole() const;

protected:
    UPROPERTY()
    class AController* OwningController;

    UPROPERTY()
    ECombatState CurrentCombatState;

    UPROPERTY()
    ECombatRole CurrentCombatRole;

    UPROPERTY()
    FMingAIContext CurrentContext;

    virtual void AnalyzeSituation();
    virtual void ExecuteDecision(const FMingTacticalDecision& Decision);
    virtual bool ShouldAttack(const FMingAIContext& Context);
    virtual bool ShouldDefend(const FMingAIContext& Context);
    virtual bool ShouldRetreat(const FMingAIContext& Context);
};
