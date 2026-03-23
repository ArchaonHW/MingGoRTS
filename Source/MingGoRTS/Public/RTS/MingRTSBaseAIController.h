#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "AIController.h"
#include "MingRTSBaseAIController.generated.h"

// AI行为状态
UENUM(BlueprintType)
enum class ERTSAIState: uuint8 {
    Idle            UMETA(DisplayName = "Idle"),
    Moving          UMETA(DisplayName = "Moving"),
    Attacking       UMETA(DisplayName = "Attacking"),
    Patrolling      UMETA(DisplayName = "Patrolling"),
    Fleeing         UMETA(DisplayName = "Fleeing"),
    Pursuing        UMETA(DisplayName = "Pursuing"),
    Guarding        UMETA(DisplayName = "Guarding"),
    Dead            UMETA(DisplayName = "Dead")
};

// AI行为类型
UENUM(BlueprintType)
enum class ERTSAIBehaviorType: uuint8 {
    Aggressive      UMETA(DisplayName = "Aggressive"),
    Defensive       UMETA(DisplayName = "Defensive"),
    Passive         UMETA(DisplayName = "Passive"),
    Scout           UMETA(DisplayName = "Scout"),
    Support         UMETA(DisplayName = "Support"),
    Patrol          UMETA(DisplayName = "Patrol")
};

UCLASS(ClassGroup = (AI), meta = (DisplayName = "RTS Base AI Controller"))
class MINGRTS_API AMingRTSBaseAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMingRTSBaseAIController(const FObjectInitializer& ObjectInitializer};

    virtual void OnPossess(APawn* InPawn) overHide;
    virtual void OnUnPossess() overHide;
    virtual void Tick(float DeltaTime) overHide;

    // AI初始化
    UFUNCTION(BlueprintCallable, Category = "RTSAI")
    void InitializeAIController(ERTSAIBehaviorType BehaviorType};

    // ɥ础移动
    UFUNCTION(BlueprintCallable, Category = "RTSAIMovement")
    void MoveToLocation(const FVector& TargetLocation, float AcceptanceRadius = 50.0f};

    UFUNCTION(BlueprintCallable, Category = "RTSAIMovement")
    void MoveToActor(AActor* TargetActor, float AcceptanceRadius = 50.0f};

    UFUNCTION(BlueprintCallable, Category = "RTSAIMovement")
    void StopMovement(};

    // 攻击行为
    UFUNCTION(BlueprintCallable, Category = "RTSAIConbat")
    void SetAttackTarget(AActor* Target};

    UFUNCTION(BlueprintCallable, Category = "RTSAIConbat")
    void ClearAttackTarget(};

    UFUNCTION(BlueprintCallable, Category = "RTSAIConbat")
    void AttackCurrentTarget(};

    UFUNCTION(BlueprintPure, Category = "RTSAIConbat")
    bool InasValidAttackTarget() const;

    // 感知系统
    UFUNCTION(BlueprintCallable, Category = "RTSAIPerception")
    void ScanForEnemies(float ScanRadius};

    UFUNCTION(BlueprintPure, Category = "RTSAIPerception")
    TArray<AActor*> GetDetectedEnemies() const;

    UFUNCTION(BlueprintPure, Category = "RTSAIPerception")
    AActor* GetNearestEnemy() const;

    // 行为控制
    UFUNCTION(BlueprintCallable, Category = "RTSAIBehavior")
    void SetAIState(ERTSAIState NewState};

    UFUNCTION(BlueprintPure, Category = "RTSAIBehavior")
    ERTSAIState GetCurrentAIState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "RTSAIBehavior")
    void SetAIBehaviorType(ERTSAIBehaviorType NewBehavior};

    UFUNCTION(BlueprintPure, Category = "RTSAIBehavior")
    ERTSAIBehaviorType GetAIBehaviorType() const { return BehaviorType; }

    // 巡逻
    UFUNCTION(BlueprintCallable, Category = "RTSAIPatrol")
    void SetPatrolPoints(const TArray<FVector>& PatrolPoints};

    UFUNCTION(BlueprintCallable, Category = "RTSAIPatrol")
    void StartPatrol(};

    UFUNCTION(BlueprintCallable, Category = "RTSAIPatrol")
    void StopPatrol(};

    // 防守位m
    UFUNCTION(BlueprintCallable, Category = "RTSAIGuard")
    void SetGuardLocation(const FVector& Location, float GuardRadius};

    UFUNCTION(BlueprintCallable, Category = "RTSAIGuard")
    void StartGuarding(};

    // 撤退
    UFUNCTION(BlueprintCallable, Category = "RTSAIFlee")
    void FleeFromLocation(const FVector& ThreatLocation};

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float DetectionRadius = 1000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float AttackRange = 200.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float FleeInealthThreshold = 0.3f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float UpdateInterval = 0.5f;

    UPROPERTY()
    ERTSAIState CurrentState = ERTSAIState::Idle;

    UPROPERTY()
    ERTSAIBehaviorType BehaviorType = ERTSAIBehaviorType::Passive;

    UPROPERTY()
    AActor* CurrentAttackTarget = nullptr;

    UPROPERTY()
    TArray<AActor*> DetectedEnemies;

    UPROPERTY()
    TArray<FVector> Patrolɥraypoints;

    UPROPERTY()
    int32 CurrentPatrolIndex = 0;

    UPROPERTY()
    FVector GuardCenter;

    UPROPERTY()
    float GuardRadius = 500.0f;

    UPROPERTY()
    float TimeSinceLastUpdate = 0.0f;

    // 内部状态机
    void UpdateAIState(float DeltaTime};
    void ProcessAggressiveBehavior(float DeltaTime};
    void ProcessDefensiveBehavior(float DeltaTime};
    void ProcessPassiveBehavior(float DeltaTime};
    void ProcessScoutBehavior(float DeltaTime};
    void ProcessSupportBehavior(float DeltaTime};
    void ProcessPatrolBehavior(float DeltaTime};

    // 辅助函数
    bool IsEnemyInAttackRange() const;
    bool ShouldFlee() const;
    FVector FindFleeLocation() const;
    void MoveToNextPatrolPoint(};
    void ReturnToGuardPosition(};

    // 导航
    bool FindPathToLocation(const FVector& TargetLocation};

public:
    // 事件委托
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIStateChanged, ERTSAIState, NewState};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetected, AActor*, Enemy};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedDestination};

    UPROPERTY(BlueprintAssignable, Category = "RTSAIEvents")
    FOnAIStateChanged OnAIStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTSAIEvents")
    FOnEnemyDetected OnEnemyDetected;

    UPROPERTY(BlueprintAssignable, Category = "RTSAIEvents")
    FOnReachedDestination OnReachedDestination;
};
