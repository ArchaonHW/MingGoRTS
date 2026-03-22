#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingCombatAIManager.h"
#include "MingBaseCombatAI.generated.h"

// AI 狀態
USTRUCT(BlueprintType)
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

// 戰術決策
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

// 單位命令
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
 * 基礎戰鬥 AI 類
 * 所有具體 AI 類型的基類
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class MINGTACTICAL_API UMingBaseCombatAI : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingBaseCombatAI();

    // 初始化 AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void Initialize(class AController* Controller);

    // 更新 AI
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void UpdateAI(float DeltaTime);

    // 製作戰術決策
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context);

    // 執行單位命令
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void ExecuteUnitCommand(class AActor* Unit, const FMingUnitCommand& Command);

    // 評估戰場局勢
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual float AssessBattlefieldSituation(const FMingAIContext& Context);

    // 設置 AI 難度
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void SetDifficulty(EMingAIDifficulty Difficulty);

    // 獲取 AI 狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    FMingAIState GetAIState() const { return CurrentAIState; }

    // 重置 AI 狀態
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    virtual void ResetAIState();

protected:
    // AI 控制器
    UPROPERTY(BlueprintReadOnly)
    class AController* AIController;

    // 當前 AI 狀態
    UPROPERTY(BlueprintReadOnly)
    FMingAIState CurrentAIState;

    // 當前難度
    UPROPERTY(BlueprintReadOnly)
    EMingAIDifficulty CurrentDifficulty;

    // 難度設置
    UPROPERTY(BlueprintReadOnly)
    FMingAIDifficultySettings DifficultySettings;

    // 戰術偏好
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingTacticalStyle TacticalStyle;

    // 攻擊性等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Aggressiveness;

    // 謹慎性等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cautiousness;

    // 協調能力
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CoordinationSkill;

    // 分析威脅
    virtual TArray<class AActor*> AnalyzeThreats(const TArray<class AActor*>& EnemyUnits);

    // 識別機會
    virtual TArray<FVector> IdentifyOpportunities(const FMingAIContext& Context);

    // 評估單位狀態
    virtual float AssessUnitStatus(class AActor* Unit);

    // 規劃機動路徑
    virtual TArray<FVector> PlanMovementPath(class AActor* Unit, const FVector& Destination);

    // 計算攻擊優先級
    virtual float CalculateAttackPriority(class AActor* Target);

    // 決定是否撤退
    virtual bool ShouldRetreat(const FMingAIContext& Context);

    // 決定是否防禦
    virtual bool ShouldDefend(const FMingAIContext& Context);

    // 決定是否攻擊
    virtual bool ShouldAttack(const FMingAIContext& Context);

    // 更新士氣
    virtual void UpdateMorale(const FMingAIContext& Context);

    // 更新凝聚力
    virtual void UpdateCohesion(const TArray<class AActor*>& Units);

private:
    // 最後更新時間
    float LastUpdateTime;

    // 決策冷卻時間
    float DecisionCooldown;

    // 當前決策
    FMingTacticalDecision CurrentDecision;
};
