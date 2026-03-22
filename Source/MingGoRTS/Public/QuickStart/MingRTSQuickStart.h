// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Quick Start System for New Players

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSQuickStart.generated.h"

UENUM(BlueprintType)
enum class EQuickStartStep : uint8
{
    Welcome, UMETA(DisplayName = "Welcome"),
    Tutorial, UMETA(DisplayName = "Tutorial"),
    FirstMission, UMETA(DisplayName = "First Mission"),
    BasicControls, UMETA(DisplayName = "Basic Controls"),
    ResourceManagement, UMETA(DisplayName = "Resource Management"),
    UnitControl, UMETA(DisplayName = "Unit Control"),
    BuildingConstruction, UMETA(DisplayName = "Building Construction"),
    CombatBasics, UMETA(DisplayName = "Combat Basics"),
    AdvancedFeatures, UMETA(DisplayName = "Advanced Features"),
    Completed, UMETA(DisplayName = "Completed")
};

USTRUCT(BlueprintType)
struct FQuickStartProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Quick Start Progress")
    EQuickStartStep CurrentStep;

    UPROPERTY(BlueprintReadOnly, Category = "Quick Start Progress")
    float ProgressPercentage;

    UPROPERTY(BlueprintReadOnly, Category = "Quick Start Progress")
    int32 StepsCompleted;

    UPROPERTY(BlueprintReadOnly, Category = "Quick Start Progress")
    int32 TotalSteps;

    UPROPERTY(BlueprintReadOnly, Category = "Quick Start Progress")
    bool bIsCompleted;

    FQuickStartProgress()
        : CurrentStep(EQuickStartStep::Welcome)
        , ProgressPercentage(0.0f)
        , StepsCompleted(0)
        , TotalSteps(10)
        , bIsCompleted(false)
    {}
};

/**
 * MingGoRTS Quick Start System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSQuickStart : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSQuickStart();

    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void StartQuickStart();

    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void StopQuickStart();

    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void NextStep();

    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void PreviousStep();

    UFUNCTION(BlueprintPure, Category = "Quick Start")
    EQuickStartStep GetCurrentStep() const;

    UFUNCTION(BlueprintPure, Category = "Quick Start")
    FQuickStartProgress GetProgress() const;

    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void SetStep(EQuickStartStep Step);

    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    bool IsQuickStartActive() const;

protected:
    UPROPERTY()
    EQuickStartStep CurrentStep;

    UPROPERTY()
    FQuickStartProgress Progress;

    UPROPERTY()
    bool bSystemStarted;

    UPROPERTY()
    bool bOptimizationRunning;

private:
    void InitializeQuickStart();
    void DisplayResults(const FString& Results);
};
