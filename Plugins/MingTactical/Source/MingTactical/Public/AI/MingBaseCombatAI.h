#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingCombatAIManager.h"
#include "MingBaseCombatAI.generated.h"

// AI ?�XUSTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bIsInCombat;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRetreating;

    UPROPERTY(BlueprintReadOnly)
    bool bIsDefending;

    UPROPERTY(BlueprintReadOnly)
    bool bIsAttacking;

    UPROPERTY(BlueprintReadOnly)
    float Morale;

    UPROPERTY(BlueprintReadOnly)
    float Cohesion;

    UPROPERTY(BlueprintReadOnly)
    FVector CurrentPosition;

    UPROPERTY(BlueprintReadOnly)
    FVector TargetPosition;

    UPROPERTY(BlueprintReadOnly)
    float LastDecisionTime;
};

// ?��?決�?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingTacticalDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation;

    UPROPERTY(BlueprintReadOnly)
    TArray<class AActor*> TargetUnits;

    UPROPERTY(BlueprintReadOnly)
    float Priority;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    bool bRequiresCoordination;
};

// ?��X�令
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingUnitCommand
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CommandType;

    UPROPERTY(BlueprintReadOnly)
    FVector TargetLocation;

    UPROPERTY(BlueprintReadOnly)
    class AActor* TargetUnit;

    UPROPERTY(BlueprintReadOnly)
    float CommandPriority;

    UPROPERTY(BlueprintReadOnly)
    bool bIsImmediate;
};

/**
 * ?��X�鬥 AI �? * ?�?�具�?AI 類�X�基�? */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MINGTACTICAL_API UMingBaseCombatAI : public UObject
{
    GENERATED_BODY()

public:
    // 建�?�?    UMingBaseCombatAI(};

    // ?��XAI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void Initialize(class AController* Controller};

    // ?�新 AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void UpdateAI(float DeltaTime};

    // 製�X��?決�?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context};

    // ?��X��X�令
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void ExecuteUnitCommand(class AActor* Unit, const FMingUnitCommand& Command};

    // 評估?�場局X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual float AssessBattlefieldSituation(const FMingAIContext& Context};

    // 設置 AI X��
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void SetDifficulty(EMingAIDifficulty Difficulty};

    // ?��? AI ?�X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIState GetAIState() const { return CurrentAIState; }

    // ?�置 AI ?�X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void ResetAIState(};

protected:
    // AI ?�制X    UPROPERTY(BlueprintReadOnly)
    class AController* AIController;

    // ?��? AI ?�X    UPROPERTY(BlueprintReadOnly)
    FMingAIState CurrentAIState;

    // ?��X��
    UPROPERTY(BlueprintReadOnly)
    EMingAIDifficulty CurrentDifficulty;

    // X��設置
    UPROPERTY(BlueprintReadOnly)
    FMingAIDifficultySettings DifficultySettings;

    // ?��X�好
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTacticalStyle TacticalStyle;

    // ?��X��?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Aggressiveness;

    // 謹�X��?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cautiousness;

    // ?�調?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CoordinationSkill;

    // ?��?威�?
    virtual TArray<class AActor*> AnalyzeThreats(const TArray<class AActor*>& EnemyUnits};

    // 識別機�?
    virtual TArray<FVector> IdentifyOpportunities(const FMingAIContext& Context};

    // 評估?��X�X    virtual float AssessUnitStatus(class AActor* Unit};

    // 規�?機�?路�?
    virtual TArray<FVector> PlanMovementPath(class AActor* Unit, const FVector& Destination};

    // 計�X��X��?�?    virtual float CalculateAttackPriority(class AActor* Target};

    // 決�X�否?�退
    virtual bool ShouldRetreat(const FMingAIContext& Context};

    // 決�X�否?�禦
    virtual bool ShouldDefend(const FMingAIContext& Context};

    // 決�X�否?��?
    virtual bool ShouldAttack(const FMingAIContext& Context};

    // ?�新士氣
    virtual void UpdateMorale(const FMingAIContext& Context};

    // ?�新?��X    virtual void UpdateCohesion(const TArray<class AActor*>& Units};

private:
    // ?�後更?��X    float LastUpdateTime;

    // 決�X�卻?��?
    float DecisionCooldown;

    // ?��?決�?
    FMingTacticalDecision CurrentDecision;
};

