// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MingRTSBaseAIController.generated.h"

// AI行为状态
UENUM(BlueprintType)
enum class ERTSAIState: uint8 {
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
enum class ERTSAIBehaviorType: uint8 {
    Aggressive      UMETA(DisplayName = "Aggressive"),
    Defensive       UMETA(DisplayName = "Defensive"),
    Passive         UMETA(DisplayName = "Passive"),
    Scout           UMETA(DisplayName = "Scout"),
    Support         UMETA(DisplayName = "Support"),
    Patrol          UMETA(DisplayName = "Patrol")
};

UCLASS(ClassGroup = (AI), meta = (DisplayName = "RTS Base AI Controller"))
class MINGGORTS_API AMingRTSBaseAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMingRTSBaseAIController(const FObjectInitializer& ObjectInitializer);

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;
    virtual void Tick(float DeltaTime) override;

    // AI初始化
    UFUNCTION(BlueprintCallable, Category = "RTS|AI")
    void InitializeAIController(ERTSAIBehaviorType BehaviorType);

    // 基础移动
    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Movement")
    void MoveToLocation(const FVector& TargetLocation, float AcceptanceRadius = 50.0f);

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Movement")
    void MoveToActor(AActor* TargetActor, float AcceptanceRadius = 50.0f);

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Movement")
    void StopMovement();

    // 攻击行为
    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Combat")
    void SetAttackTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Combat")
    void ClearAttackTarget();

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Combat")
    void AttackCurrentTarget();

    UFUNCTION(BlueprintPure, Category = "RTS|AI|Combat")
    bool HasValidAttackTarget() const;

    // 感知系统
    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Perception")
    void ScanForEnemies(float ScanRadius);

    UFUNCTION(BlueprintPure, Category = "RTS|AI|Perception")
    TArray<AActor*> GetDetectedEnemies() const;

    UFUNCTION(BlueprintPure, Category = "RTS|AI|Perception")
    AActor* GetNearestEnemy() const;

    // 行为控制
    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Behavior")
    void SetAIState(ERTSAIState NewState);

    UFUNCTION(BlueprintPure, Category = "RTS|AI|Behavior")
    ERTSAIState GetCurrentAIState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Behavior")
    void SetAIBehaviorType(ERTSAIBehaviorType NewBehavior);

    UFUNCTION(BlueprintPure, Category = "RTS|AI|Behavior")
    ERTSAIBehaviorType GetAIBehaviorType() const { return BehaviorType; }

    // 巡逻
    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Patrol")
    void SetPatrolPoints(const TArray<FVector>& PatrolPoints);

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Patrol")
    void StartPatrol();

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Patrol")
    void StopPatrol();

    // 防守位置
    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Guard")
    void SetGuardLocation(const FVector& Location, float GuardRadius);

    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Guard")
    void StartGuarding();

    // 撤退
    UFUNCTION(BlueprintCallable, Category = "RTS|AI|Flee")
    void FleeFromLocation(const FVector& ThreatLocation);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float DetectionRadius = 1000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float AttackRange = 200.0f;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float FleeHealthThreshold = 0.3f;

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
    TArray<FVector> PatrolWaypoints;

    UPROPERTY()
    int32 CurrentPatrolIndex = 0;

    UPROPERTY()
    FVector GuardCenter;

    UPROPERTY()
    float GuardRadius = 500.0f;

    UPROPERTY()
    float TimeSinceLastUpdate = 0.0f;

    // 内部状态机
    void UpdateAIState(float DeltaTime);
    void ProcessAggressiveBehavior(float DeltaTime);
    void ProcessDefensiveBehavior(float DeltaTime);
    void ProcessPassiveBehavior(float DeltaTime);
    void ProcessScoutBehavior(float DeltaTime);
    void ProcessSupportBehavior(float DeltaTime);
    void ProcessPatrolBehavior(float DeltaTime);

    // 辅助函数
    bool IsEnemyInAttackRange() const;
    bool ShouldFlee() const;
    FVector FindFleeLocation() const;
    void MoveToNextPatrolPoint();
    void ReturnToGuardPosition();

    // 导航
    bool FindPathToLocation(const FVector& TargetLocation);

public:
    // 事件委托
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIStateChanged, ERTSAIState, NewState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDetected, AActor*, Enemy);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachedDestination);

    UPROPERTY(BlueprintAssignable, Category = "RTS|AI|Events")
    FOnAIStateChanged OnAIStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS|AI|Events")
    FOnEnemyDetected OnEnemyDetected;

    UPROPERTY(BlueprintAssignable, Category = "RTS|AI|Events")
    FOnReachedDestination OnReachedDestination;
};
