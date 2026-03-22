#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MingRTSAIController.generated.h"

class AMingGoRTSUnit;
class UMingRTSUnitManager;
class UMingRTSCombatSystem;

UENUM(BlueprintType)
enum class ERTSAIBehavior : uint8
{
    Idle,           // ??X
    Follow,         // ???H
    Gather,         // ????
    Build,          // ??y
    Repair,         // ??_
    Explore         // ????
};

UENUM(BlueprintType)
enum class ERTSAITargetPriority : uint8
{
    None,           // ?L???
    Low,            // ?C?u????
    Medium,         // ???u????
    High,           // ???u????
    Critical        // ?????u????
};

UENUM(BlueprintType)
enum class ERTSAIState : uint8
{
    Thinking,       // ????
    Planning,        // ?p????
    Executing,      // ????
    Waiting,        // ?????
    Reacting        // ??????
};

USTRUCT(BlueprintType)
struct FRTSAITarget
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Target")
    AActor* TargetActor;

    UPROPERTY(BlueprintReadOnly, Category = "AI Target")
    FVector TargetLocation;

    UPROPERTY(BlueprintReadOnly, Category = "AI Target")
    ERTSAITargetPriority Priority;

    UPROPERTY(BlueprintReadOnly, Category = "AI Target")
    float ThreatLevel;

    UPROPERTY(BlueprintReadOnly, Category = "AI Target")
    float Distance;

    UPROPERTY(BlueprintReadOnly, Category = "AI Target")
    bool bIsValid;

    UPROPERTY(BlueprintReadOnly, Category = "AI Target")
    FDateTime LastSeen;

    FRTSAITarget()
    {
        TargetActor = nullptr;
        TargetLocation = FVector::ZeroVector;
        Priority = ERTSAITargetPriority::None;
        ThreatLevel = 0.0f;
        Distance = 0.0f;
        bIsValid = false;
    }
};

USTRUCT(BlueprintType)
struct FRTSAIPatrolPath
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Patrol")
    TArray<FVector> PatrolPoints;

    UPROPERTY(BlueprintReadOnly, Category = "AI Patrol")
    int32 CurrentPointIndex;

    UPROPERTY(BlueprintReadOnly, Category = "AI Patrol")
    bool bLoop;

    UPROPERTY(BlueprintReadOnly, Category = "AI Patrol")
    bool bReverse;

    FRTSAIPatrolPath()
    {
        CurrentPointIndex = 0;
        bLoop = true;
        bReverse = false;
    }
};

USTRUCT(BlueprintType)
struct FRTSAIDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Decision")
    ERTSAIBehavior Behavior;

    UPROPERTY(BlueprintReadOnly, Category = "AI Decision")
    FRTSAITarget Target;

    UPROPERTY(BlueprintReadOnly, Category = "AI Decision")
    float Confidence;

    UPROPERTY(BlueprintReadOnly, Category = "AI Decision")
    FString Reason;

    UPROPERTY(BlueprintReadOnly, Category = "AI Decision")
    float ExecutionTime;

    FRTSAIDecision()
    {
        Behavior = ERTSAIBehavior::Idle;
        Confidence = 0.0f;
        ExecutionTime = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIBehaviorChanged, ERTSAIBehavior, OldBehavior, ERTSAIBehavior, NewBehavior};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAITargetChanged, const FRTSAITarget&, OldTarget, const FRTSAITarget&, NewTarget};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAIDecisionMade, const FRTSAIDecision&, Decision, ERTSAIState, AIState, float, Confidence};

/**
 * RTS AIX??X * ??XAI??X?MX */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API AMingRTSAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMingRTSAIController();

    
    void InitializeAI(UMingRTSUnitManager* InUnitManager, UMingRTSCombatSystem* InCombatSystem};

    // AI??X??
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetBehavior(ERTSAIBehavior NewBehavior};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    ERTSAIBehavior GetCurrentBehavior() const { return CurrentBehavior; }

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void ExecuteBehavior(float DeltaTime};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void UpdateAI(float DeltaTime};

    // X??X
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetTarget(const FRTSAITarget& NewTarget) {};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    FRTSAITarget GetCurrentTarget() const { return CurrentTarget; }

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void ClearTarget();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    bool HasValidTarget() const;

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void UpdateTarget();

    // X???t??
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void ScanForEnemies(float ScanRadius};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void ScanForAllies(float ScanRadius};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void ScanForResources(float ScanRadius};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    TArray<FRTSAITarget> GetDetectedEnemies() const;

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    TArray<FRTSAITarget> GetDetectedAllies() const;

    // ?MX?t??
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    FRTSAIDecision MakeDecision();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void EvaluateThreats();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    float CalculateThreatLevel(AActor* Target) const;

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    ERTSAITargetPriority GetTargetPriority(AActor* Target) const;

    // ??X?t??
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetPatrolPath(const FRTSAIPatrolPath& Path) {};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void AddPatrolPoint(const FVector& Point) {};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void ClearPatrolPath();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    FVector GetNextPatrolPoint();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    bool IsAtPatrolPoint() const;

    // X??AI
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void EngageTarget(AActor* Target};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void DisengageTarget();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    bool ShouldEngage(AActor* Target) const;

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    bool ShouldRetreat() const;

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void FindBestAttackPosition();

    // ??XAI
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void MoveToTarget();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void MoveToPosition(const FVector& Position) {};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    bool IsMoving() const;

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void StopMovement();

    
    ERTSAIState GetAIState() const { return AIState; }

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetAIState(ERTSAIState NewState};

    // AI?]?m
    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetAggressiveness(float Value};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetReactionTime(float Value};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetDecisionFrequency(float Value};

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void SetPerceptionRange(float Value};

    
    void ImproveAIBehavior();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void OptimizeDecisionMaking();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void EnhanceTargetSelection();

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void AdaptToPlayerBehavior();

    
    FString GetCurrentBehaviorName() const;

    UFUNCTION(BlueprintPure, Category = "RTS AI Controller")
    FString GetCurrentStateName() const;

    UFUNCTION(BlueprintPure, Category = "RTS AI Controller")
    bool IsAIActive() const;

    UFUNCTION(BlueprintCallable, Category = "RTS AI Controller")
    void EnableDebugMode(bool bEnable};

    // ???
    UPROPERTY(BlueprintAssignable, Category = "RTS AI Events")
    FOnAIBehaviorChanged OnAIBehaviorChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS AI Events")
    FOnAITargetChanged OnAITargetChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS AI Events")
    FOnAIDecisionMade OnAIDecisionMade;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ?t?£V??
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    UPROPERTY()
    TObjectPtr<UMingRTSCombatSystem> CombatSystem;

    
    ERTSAIBehavior CurrentBehavior;

    UPROPERTY()
    ERTSAIState AIState;

    UPROPERTY()
    FRTSAITarget CurrentTarget;

    UPROPERTY()
    FRTSAIPatrolPath PatrolPath;

    UPROPERTY()
    TArray<FRTSAITarget> DetectedEnemies;

    UPROPERTY()
    TArray<FRTSAITarget> DetectedAllies;

    // AI?]?m
    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    float Aggressiveness = 0.5f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    float ReactionTime = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    float DecisionFrequency = 2.0f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    float PerceptionRange = 500.0f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    float AttackRange = 200.0f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    float RetreatThreshold = 0.3f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    bool bIsAIActive = true;

    UPROPERTY(BlueprintReadWrite, Category = "AI Settings")
    bool bDebugMode = false;

    
    float LearningRate = 0.1f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Improvement")
    float AdaptationSpeed = 0.5f;

    UPROPERTY(BlueprintReadWrite, Category = "AI Improvement")
    int32 MemorySize = 100;

    UPROPERTY(BlueprintReadWrite, Category = "AI Improvement")
    bool bEnableLearning = true;

    UPROPERTY(BlueprintReadWrite, Category = "AI Improvement")
    bool bEnableAdaptation = true;

    // X????X
    UPROPERTY()
    float LastDecisionTime;

    UPROPERTY()
    float LastScanTime;

    UPROPERTY()
    float CurrentDecisionTime;

    UPROPERTY()
    FVector LastKnownTargetLocation;

    
    TArray<FRTSAIDecision> DecisionHistory;

    UPROPERTY()
    TMap<FString, float> BehaviorSuccessRates;

    UPROPERTY()
    TMap<AActor*, float> TargetThreatHistory;

    // X??X??
    void ExecuteIdleBehavior(float DeltaTime};
    void ExecutePatrolBehavior(float DeltaTime};
    void ExecuteGuardBehavior(float DeltaTime};
    void ExecuteAttackBehavior(float DeltaTime};
    void ExecuteRetreatBehavior(float DeltaTime};
    void ExecuteFollowBehavior(float DeltaTime};
    void ExecuteGatherBehavior(float DeltaTime};
    void ExecuteBuildBehavior(float DeltaTime};
    void ExecuteRepairBehavior(float DeltaTime};
    void ExecuteExploreBehavior(float DeltaTime};

    void ProcessDecision(const FRTSAIDecision& Decision) {};
    void UpdatePerception(float DeltaTime};
    void MakeAIDecision();
    bool ShouldMakeDecision() const;
    FRTSAITarget FindBestTarget() const;
    void LogAIDebug(const FString& Message) const;

    // AIX?i??X??X    void UpdateDecisionHistory(const FRTSAIDecision& Decision) {};
    void AnalyzeBehaviorPatterns();
    void AdaptBehaviorBasedOnHistory();
    void OptimizeTargetSelection();
    void LearnFromExperience();
    float CalculateBehaviorSuccess(const FString& BehaviorName) const;

private:
    // ???UX??
    void NotifyBehaviorChanged(ERTSAIBehavior OldBehavior, ERTSAIBehavior NewBehavior};
    void NotifyTargetChanged(const FRTSAITarget& OldTarget, const FRTSAITarget& NewTarget) {};
    void NotifyDecisionMade(const FRTSAIDecision& Decision, ERTSAIState AIState, float Confidence};
};

