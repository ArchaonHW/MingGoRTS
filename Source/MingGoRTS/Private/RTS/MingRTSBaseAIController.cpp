// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS/MingRTSBaseAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "DrawDebugHelpers.h"

AMingRTSBaseAIController::AMingRTSBaseAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 设置更新频率
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.5f;
    
    CurrentState = ERTSAIState::Idle;
    BehaviorType = ERTSAIBehaviorType::Passive;
    CurrentAttackTarget = nullptr;
    CurrentPatrolIndex = 0;
}

void AMingRTSBaseAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    
    InitializeAIController(BehaviorType);
    UE_LOG(LogTemp, Log, TEXT("AI Controller possessed %s"), *InPawn->GetName());
}

void AMingRTSBaseAIController::OnUnPossess()
{
    Super::OnUnPossess();
    
    CurrentAttackTarget = nullptr;
    DetectedEnemies.Empty();
    UE_LOG(LogTemp, Log, TEXT("AI Controller unpossessed"));
}

void AMingRTSBaseAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    TimeSinceLastUpdate += DeltaTime;
    
    if (TimeSinceLastUpdate >= UpdateInterval)
    {
        UpdateAIState(TimeSinceLastUpdate);
        TimeSinceLastUpdate = 0.0f;
    }
}

void AMingRTSBaseAIController::InitializeAIController(ERTSAIBehaviorType InBehaviorType)
{
    BehaviorType = InBehaviorType;
    CurrentState = ERTSAIState::Idle;
    
    UE_LOG(LogTemp, Log, TEXT("AI Controller initialized with behavior: %s"),
        *UEnum::GetValueAsString(BehaviorType));
}

void AMingRTSBaseAIController::MoveToLocation(const FVector& TargetLocation, float AcceptanceRadius)
{
    if (!GetPawn())
    {
        return;
    }
    
    UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, TargetLocation);
    SetAIState(ERTSAIState::Moving);
    
    UE_LOG(LogTemp, Log, TEXT("AI moving to location: %s"), *TargetLocation.ToString());
}

void AMingRTSBaseAIController::MoveToActor(AActor* TargetActor, float AcceptanceRadius)
{
    if (!GetPawn() || !TargetActor)
    {
        return;
    }
    
    UAIBlueprintHelperLibrary::SimpleMoveToActor(this, TargetActor, AcceptanceRadius);
    SetAIState(ERTSAIState::Moving);
}

void AMingRTSBaseAIController::StopMovement()
{
    StopMovement();
    SetAIState(ERTSAIState::Idle);
}

void AMingRTSBaseAIController::SetAttackTarget(AActor* Target)
{
    if (Target && Target != GetPawn())
    {
        CurrentAttackTarget = Target;
        SetAIState(ERTSAIState::Attacking);
        
        UE_LOG(LogTemp, Log, TEXT("AI set attack target: %s"), *Target->GetName());
    }
}

void AMingRTSBaseAIController::ClearAttackTarget()
{
    CurrentAttackTarget = nullptr;
    SetAIState(ERTSAIState::Idle);
}

void AMingRTSBaseAIController::AttackCurrentTarget()
{
    if (!CurrentAttackTarget || !IsEnemyInAttackRange())
    {
        // 目标不在范围内，移动靠近
        if (CurrentAttackTarget)
        {
            MoveToActor(CurrentAttackTarget, AttackRange * 0.8f);
        }
        return;
    }
    
    // 执行攻击逻辑
    // 这里应该调用战斗系统的攻击函数
    UE_LOG(LogTemp, Log, TEXT("AI attacking target: %s"), *CurrentAttackTarget->GetName());
}

bool AMingRTSBaseAIController::HasValidAttackTarget() const
{
    return CurrentAttackTarget != nullptr && IsValid(CurrentAttackTarget);
}

void AMingRTSBaseAIController::ScanForEnemies(float ScanRadius)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        return;
    }
    
    DetectedEnemies.Empty();
    
    FVector Location = ControlledPawn->GetActorLocation();
    UWorld* World = GetWorld();
    
    if (!World)
    {
        return;
    }
    
    // 绘制扫描范围调试
    DrawDebugSphere(World, Location, ScanRadius, 32, FColor::Yellow, false, 1.0f);
    
    // 查找范围内的所有Actor
    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(ScanRadius);
    
    World->OverlapMultiByChannel(Overlaps, Location, FQuat::Identity, ECC_Pawn, Sphere);
    
    for (const FOverlapResult& Overlap : Overlaps)
    {
        AActor* Actor = Overlap.GetActor();
        if (Actor && Actor != ControlledPawn)
        {
            // 检查是否为敌人（这里简单判断，实际应该根据阵营等判断）
            if (Actor->ActorHasTag(FName("Enemy")))
            {
                DetectedEnemies.Add(Actor);
                OnEnemyDetected.Broadcast(Actor);
            }
        }
    }
    
    // 如果有新敌人，根据行为类型决定反应
    if (DetectedEnemies.Num() > 0)
    {
        switch (BehaviorType)
        {
        case ERTSAIBehaviorType::Aggressive:
            SetAttackTarget(GetNearestEnemy());
            break;
        case ERTSAIBehaviorType::Defensive:
            // 防守型AI只在被攻击时反击
            break;
        case ERTSAIBehaviorType::Scout:
            // 侦察型AI发现敌人后撤退
            if (DetectedEnemies.Num() > 0)
            {
                FleeFromLocation(DetectedEnemies[0]->GetActorLocation());
            }
            break;
        default:
            break;
        }
    }
}

TArray<AActor*> AMingRTSBaseAIController::GetDetectedEnemies() const
{
    return DetectedEnemies;
}

AActor* AMingRTSBaseAIController::GetNearestEnemy() const
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn || DetectedEnemies.Num() == 0)
    {
        return nullptr;
    }
    
    AActor* Nearest = nullptr;
    float MinDistance = FLT_MAX;
    FVector MyLocation = ControlledPawn->GetActorLocation();
    
    for (AActor* Enemy : DetectedEnemies)
    {
        if (Enemy)
        {
            float Distance = FVector::Distance(MyLocation, Enemy->GetActorLocation());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                Nearest = Enemy;
            }
        }
    }
    
    return Nearest;
}

void AMingRTSBaseAIController::SetAIState(ERTSAIState NewState)
{
    if (CurrentState != NewState)
    {
        ERTSAIState OldState = CurrentState;
        CurrentState = NewState;
        
        OnAIStateChanged.Broadcast(NewState);
        
        UE_LOG(LogTemp, Log, TEXT("AI state changed from %s to %s"),
            *UEnum::GetValueAsString(OldState),
            *UEnum::GetValueAsString(NewState));
    }
}

void AMingRTSBaseAIController::SetAIBehaviorType(ERTSAIBehaviorType NewBehavior)
{
    BehaviorType = NewBehavior;
    UE_LOG(LogTemp, Log, TEXT("AI behavior changed to: %s"),
        *UEnum::GetValueAsString(BehaviorType));
}

void AMingRTSBaseAIController::SetPatrolPoints(const TArray<FVector>& PatrolPoints)
{
    PatrolWaypoints = PatrolPoints;
    CurrentPatrolIndex = 0;
}

void AMingRTSBaseAIController::StartPatrol()
{
    if (PatrolWaypoints.Num() > 0)
    {
        SetAIState(ERTSAIState::Patrolling);
        MoveToNextPatrolPoint();
    }
}

void AMingRTSBaseAIController::StopPatrol()
{
    StopMovement();
    SetAIState(ERTSAIState::Idle);
}

void AMingRTSBaseAIController::SetGuardLocation(const FVector& Location, float InGuardRadius)
{
    GuardCenter = Location;
    GuardRadius = InGuardRadius;
}

void AMingRTSBaseAIController::StartGuarding()
{
    SetAIState(ERTSAIState::Guarding);
    
    // 移动到守卫位置
    MoveToLocation(GuardCenter, 100.0f);
}

void AMingRTSBaseAIController::FleeFromLocation(const FVector& ThreatLocation)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        return;
    }
    
    FVector FleeLocation = FindFleeLocation();
    if (!FleeLocation.IsZero())
    {
        MoveToLocation(FleeLocation);
        SetAIState(ERTSAIState::Fleeing);
    }
}

void AMingRTSBaseAIController::UpdateAIState(float DeltaTime)
{
    // 扫描敌人
    ScanForEnemies(DetectionRadius);
    
    // 根据当前行为类型处理
    switch (BehaviorType)
    {
    case ERTSAIBehaviorType::Aggressive:
        ProcessAggressiveBehavior(DeltaTime);
        break;
    case ERTSAIBehaviorType::Defensive:
        ProcessDefensiveBehavior(DeltaTime);
        break;
    case ERTSAIBehaviorType::Passive:
        ProcessPassiveBehavior(DeltaTime);
        break;
    case ERTSAIBehaviorType::Scout:
        ProcessScoutBehavior(DeltaTime);
        break;
    case ERTSAIBehaviorType::Support:
        ProcessSupportBehavior(DeltaTime);
        break;
    case ERTSAIBehaviorType::Patrol:
        ProcessPatrolBehavior(DeltaTime);
        break;
    default:
        break;
    }
}

void AMingRTSBaseAIController::ProcessAggressiveBehavior(float DeltaTime)
{
    // 侵略型AI: 主动寻找并攻击敌人
    if (HasValidAttackTarget())
    {
        AttackCurrentTarget();
    }
    else if (GetNearestEnemy())
    {
        SetAttackTarget(GetNearestEnemy());
    }
}

void AMingRTSBaseAIController::ProcessDefensiveBehavior(float DeltaTime)
{
    // 防守型AI: 保持位置，只在敌人靠近时攻击
    switch (CurrentState)
    {
    case ERTSAIState::Guarding:
        // 检查是否有敌人进入守卫范围
        if (GetNearestEnemy())
        {
            AActor* Enemy = GetNearestEnemy();
            float Distance = FVector::Distance(GetPawn()->GetActorLocation(), Enemy->GetActorLocation());
            
            if (Distance <= GuardRadius)
            {
                SetAttackTarget(Enemy);
            }
        }
        break;
    case ERTSAIState::Attacking:
        // 攻击后返回守卫位置
        if (!HasValidAttackTarget())
        {
            ReturnToGuardPosition();
        }
        break;
    default:
        break;
    }
}

void AMingRTSBaseAIController::ProcessPassiveBehavior(float DeltaTime)
{
    // 被动型AI: 不主动攻击，只在被攻击时逃跑
    if (ShouldFlee())
    {
        FVector ThreatLocation = GetPawn()->GetActorLocation();
        if (CurrentAttackTarget)
        {
            ThreatLocation = CurrentAttackTarget->GetActorLocation();
        }
        FleeFromLocation(ThreatLocation);
    }
}

void AMingRTSBaseAIController::ProcessScoutBehavior(float DeltaTime)
{
    // 侦察型AI: 持续巡逻，发现敌人后标记并撤退
    if (CurrentState != ERTSAIState::Fleeing && DetectedEnemies.Num() > 0)
    {
        // 标记敌人位置
        UE_LOG(LogTemp, Log, TEXT("Scout detected %d enemies"), DetectedEnemies.Num());
        
        // 撤退
        if (AActor* NearestEnemy = GetNearestEnemy())
        {
            FleeFromLocation(NearestEnemy->GetActorLocation());
        }
    }
    else if (CurrentState == ERTSAIState::Idle)
    {
        // 继续巡逻
        StartPatrol();
    }
}

void AMingRTSBaseAIController::ProcessSupportBehavior(float DeltaTime)
{
    // 支援型AI: 跟随友军单位并提供支援
    // 这里可以实现治疗、增益等逻辑
}

void AMingRTSBaseAIController::ProcessPatrolBehavior(float DeltaTime)
{
    // 巡逻型AI: 在巡逻点之间移动
    if (CurrentState == ERTSAIState::Idle && PatrolWaypoints.Num() > 0)
    {
        MoveToNextPatrolPoint();
    }
}

bool AMingRTSBaseAIController::IsEnemyInAttackRange() const
{
    if (!CurrentAttackTarget || !GetPawn())
    {
        return false;
    }
    
    float Distance = FVector::Distance(GetPawn()->GetActorLocation(), CurrentAttackTarget->GetActorLocation());
    return Distance <= AttackRange;
}

bool AMingRTSBaseAIController::ShouldFlee() const
{
    // 检查是否应该逃跑（血量过低等）
    // 这里可以集成战斗系统查询血量
    return false;
}

FVector AMingRTSBaseAIController::FindFleeLocation() const
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        return FVector::ZeroVector;
    }
    
    FVector CurrentLocation = ControlledPawn->GetActorLocation();
    
    // 简单实现：向随机方向逃跑
    FVector FleeDirection = FMath::VRand();
    FleeDirection.Z = 0.0f;
    FleeDirection.Normalize();
    
    FVector FleeLocation = CurrentLocation + FleeDirection * 1000.0f;
    
    // 使用导航系统寻找有效位置
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (NavSystem)
    {
        FNavLocation NavLocation;
        if (NavSystem->GetRandomPointInNavigableRadius(FleeLocation, 500.0f, NavLocation))
        {
            return NavLocation.Location;
        }
    }
    
    return FleeLocation;
}

void AMingRTSBaseAIController::MoveToNextPatrolPoint()
{
    if (PatrolWaypoints.Num() == 0)
    {
        return;
    }
    
    FVector NextPoint = PatrolWaypoints[CurrentPatrolIndex];
    MoveToLocation(NextPoint);
    
    CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolWaypoints.Num();
}

void AMingRTSBaseAIController::ReturnToGuardPosition()
{
    MoveToLocation(GuardCenter, 100.0f);
    SetAIState(ERTSAIState::Guarding);
}

bool AMingRTSBaseAIController::FindPathToLocation(const FVector& TargetLocation)
{
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
        return false;
    }
    
    // 这里可以添加更复杂的路径查找逻辑
    return true;
}
