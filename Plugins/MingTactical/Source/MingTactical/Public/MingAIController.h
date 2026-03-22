#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MingTacticalUnit.h"
#include "MingAIController.generated.h"

UENUM(BlueprintType)
enum class EMingAIState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Moving UMETA(DisplayName = "Moving"),
    Attacking UMETA(DisplayName = "Attacking"),
    Fleeing UMETA(DisplayName = "Fleeing"),
    Patrolling UMETA(DisplayName = "Patrolling"),
    Following UMETA(DisplayName = "Following")
};

USTRUCT(BlueprintType)
struct FMingAICommand
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EMingAIState CommandType;

    UPROPERTY(BlueprintReadWrite)
    FVector TargetLocation;

    UPROPERTY(BlueprintReadWrite)
    TWeakObjectPtr<AActor> TargetActor;

    UPROPERTY(BlueprintReadWrite)
    float Priority;

    UPROPERTY(BlueprintReadWrite)
    FDateTime IssueTime;

    FMingAICommand()
        : CommandType(EMingAIState::Idle)
        , TargetLocation(FVector::ZeroVector)
        , Priority(1.0f)
    {
        IssueTime = FDateTime::Now();
    }
};

/**
 * Base AI Controller for MingGoRTS units
 * Handles navigation, combat, and formation behaviors
 */
UCLASS(ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class MINGTACTICAL_API AMingAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMingAIController(const FObjectInitializer& ObjectInitializer};

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;
    virtual void Tick(float DeltaTime) override;

    // Command Interface
    UFUNCTION(BlueprintCallable, Category = "AI|Commands")
    void IssueMoveCommand(const FVector& TargetLocation, bool bAttackMove = false};

    UFUNCTION(BlueprintCallable, Category = "AI|Commands")
    void IssueAttackCommand(AActor* Target};

    UFUNCTION(BlueprintCallable, Category = "AI|Commands")
    void IssueStopCommand();

    UFUNCTION(BlueprintCallable, Category = "AI|Commands")
    void IssuePatrolCommand(const FVector& PatrolPoint};

    UFUNCTION(BlueprintCallable, Category = "AI|Commands")
    void IssueFollowCommand(AActor* Leader, float FollowDistance = 200.0f};

    // Formation System
    UFUNCTION(BlueprintCallable, Category = "AI|Formation")
    void SetFormationPosition(const FVector& FormationOffset};

    UFUNCTION(BlueprintCallable, Category = "AI|Formation")
    void ClearFormationPosition();

    UFUNCTION(BlueprintPure, Category = "AI|Formation")
    bool HasFormationPosition() const;

    // Navigation (overriding parent functions - no UFUNCTION macro needed)
    bool MoveToLocation(const FVector& TargetLocation, float AcceptanceRadius = 50.0f};

    bool MoveToActor(AActor* TargetActor, float AcceptanceRadius = 50.0f};

    void StopMovement();

    // Combat
    UFUNCTION(BlueprintCallable, Category = "AI|Combat")
    AActor* FindNearestEnemy(float SearchRadius = 1000.0f) const;

    UFUNCTION(BlueprintCallable, Category = "AI|Combat")
    AActor* FindWeakestEnemy(float SearchRadius = 1000.0f) const;

    UFUNCTION(BlueprintCallable, Category = "AI|Combat")
    bool CanAttackTarget(AActor* Target) const;

    UFUNCTION(BlueprintCallable, Category = "AI|Combat")
    void SetAttackTarget(AActor* Target};

    UFUNCTION(BlueprintPure, Category = "AI|Combat")
    AActor* GetCurrentAttackTarget() const { return CurrentAttackTarget.Get(); }

    // State
    UFUNCTION(BlueprintPure, Category = "AI|State")
    EMingAIState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintPure, Category = "AI|State")
    bool IsMoving() const { return CurrentState == EMingAIState::Moving; }

    UFUNCTION(BlueprintPure, Category = "AI|State")
    bool IsAttacking() const { return CurrentState == EMingAIState::Attacking; }

    UFUNCTION(BlueprintPure, Category = "AI|State")
    bool IsIdle() const { return CurrentState == EMingAIState::Idle; }

protected:
    // AI State
    // ??制???? - 使用 TObjectPtr ????置聲X    class AMingTacticalUnit;
    UPROPERTY(BlueprintReadWrite, Category = "AI|Control")
    TObjectPtr<AMingTacticalUnit> ControlledUnit;

    UPROPERTY(BlueprintReadOnly, Category = "AI|State")
    FMingAICommand CurrentCommand;

    UPROPERTY(BlueprintReadOnly, Category = "AI|State")
    TArray<FMingAICommand> CommandQueue;

    // Combat
    UPROPERTY(BlueprintReadOnly, Category = "AI|Combat")
    TWeakObjectPtr<AActor> CurrentAttackTarget;

    UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
    float AttackRange;

    UPROPERTY(EditDefaultsOnly, Category = "AI|Combat")
    float AttackCooldown;

    // Formation
    UPROPERTY(BlueprintReadOnly, Category = "AI|Formation")
    FVector FormationOffset;

    UPROPERTY(BlueprintReadOnly, Category = "AI|Formation")
    bool bHasFormationPosition;

    // Navigation
    UPROPERTY(EditDefaultsOnly, Category = "AI|Navigation")
    float MoveAcceptanceRadius;

    UPROPERTY(EditDefaultsOnly, Category = "AI|Navigation")
    float RotationSpeed;

    UPROPERTY(EditDefaultsOnly, Category = "AI|Navigation")
    bool bUsePathfinding;

    // Performance
    UPROPERTY(EditDefaultsOnly, Category = "AI|Performance")
    float UpdateInterval;

    UPROPERTY(BlueprintReadOnly, Category = "AI|Performance")
    float TimeSinceLastUpdate;

    // Internal functions
    virtual void UpdateAI(float DeltaTime};
    virtual void ProcessCurrentCommand(float DeltaTime};
    virtual void UpdateMovement(float DeltaTime};
    virtual void UpdateCombat(float DeltaTime};
    virtual void UpdateFormation(float DeltaTime};

    // State handlers
    virtual void EnterState(EMingAIState NewState};
    virtual void ExitState(EMingAIState OldState};

    // Combat helpers
    virtual void PerformAttack(AActor* Target};
    virtual void FindNewTarget();

    // Navigation helpers
    virtual bool HasReachedDestination() const;
    virtual FVector GetDestination() const;

    // Event handlers
    UFUNCTION()
    void OnTargetDestroyed(AActor* DestroyedActor};

    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result};

private:
    // Cached components (only declare once)
    UPROPERTY()
    TWeakObjectPtr<class UNavigationSystemV1> NavSystem;

    // Path following
    FAIRequestID CurrentMoveRequest;
    FVector CurrentDestination;
    bool bHasActiveMove;

    // Combat timing
    float LastAttackTime;
    float StateEnterTime;
};

