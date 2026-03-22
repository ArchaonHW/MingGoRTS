#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingCombatAIManager.h"
#include "MingBaseCombatAI.generated.h"

// AI ?Ä??USTRUCT(BlueprintType)
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

// ?∞Ë?Ê±∫Á?
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

// ?Æ‰??Ω‰ª§
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
 * ?∫Á??∞È¨• AI È°? * ?Ä?âÂÖ∑È´?AI È°ûÂ??ÑÂü∫È°? */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MINGTACTICAL_API UMingBaseCombatAI : public UObject
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingBaseCombatAI();

    // ?ùÂ???AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void Initialize(class AController* Controller);

    // ?¥Êñ∞ AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void UpdateAI(float DeltaTime);

    // Ë£Ω‰??∞Ë?Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context);

    // ?∑Ë??Æ‰??Ω‰ª§
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void ExecuteUnitCommand(class AActor* Unit, const FMingUnitCommand& Command);

    // Ë©ï‰º∞?∞Â†¥Â±Ä??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual float AssessBattlefieldSituation(const FMingAIContext& Context);

    // Ë®≠ÁΩÆ AI ??∫¶
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void SetDifficulty(EMingAIDifficulty Difficulty);

    // ?≤Â? AI ?Ä??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIState GetAIState() const { return CurrentAIState; }

    // ?çÁΩÆ AI ?Ä??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void ResetAIState();

protected:
    // AI ?ßÂà∂??    UPROPERTY(BlueprintReadOnly)
    class AController* AIController;

    // ?∂Â? AI ?Ä??    UPROPERTY(BlueprintReadOnly)
    FMingAIState CurrentAIState;

    // ?∂Â???∫¶
    UPROPERTY(BlueprintReadOnly)
    EMingAIDifficulty CurrentDifficulty;

    // ??∫¶Ë®≠ÁΩÆ
    UPROPERTY(BlueprintReadOnly)
    FMingAIDifficultySettings DifficultySettings;

    // ?∞Ë??èÂ•Ω
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTacticalStyle TacticalStyle;

    // ?ªÊ??ßÁ?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Aggressiveness;

    // Ë¨πÊ??ßÁ?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cautiousness;

    // ?îË™ø?ΩÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CoordinationSkill;

    // ?ÜÊ?Â®ÅË?
    virtual TArray<class AActor*> AnalyzeThreats(const TArray<class AActor*>& EnemyUnits);

    // Ë≠òÂà•Ê©üÊ?
    virtual TArray<FVector> IdentifyOpportunities(const FMingAIContext& Context);

    // Ë©ï‰º∞?Æ‰??Ä??    virtual float AssessUnitStatus(class AActor* Unit);

    // Ë¶èÂ?Ê©üÂ?Ë∑ØÂ?
    virtual TArray<FVector> PlanMovementPath(class AActor* Unit, const FVector& Destination);

    // Ë®àÁ??ªÊ??™Â?Á¥?    virtual float CalculateAttackPriority(class AActor* Target);

    // Ê±∫Â??ØÂê¶?§ÈÄÄ
    virtual bool ShouldRetreat(const FMingAIContext& Context);

    // Ê±∫Â??ØÂê¶?≤Á¶¶
    virtual bool ShouldDefend(const FMingAIContext& Context);

    // Ê±∫Â??ØÂê¶?ªÊ?
    virtual bool ShouldAttack(const FMingAIContext& Context);

    // ?¥Êñ∞Â£´Ê∞£
    virtual void UpdateMorale(const FMingAIContext& Context);

    // ?¥Êñ∞?ùË???    virtual void UpdateCohesion(const TArray<class AActor*>& Units);

private:
    // ?ÄÂæåÊõ¥?∞Ê???    float LastUpdateTime;

    // Ê±∫Á??∑Âçª?ÇÈ?
    float DecisionCooldown;

    // ?∂Â?Ê±∫Á?
    FMingTacticalDecision CurrentDecision;
};
