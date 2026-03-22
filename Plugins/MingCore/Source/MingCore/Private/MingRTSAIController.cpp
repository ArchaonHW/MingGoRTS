#include "MingRTSAIController.h"
#include "MingRTSUnitManager.h"
#include "MingRTSCombatSystem.h"
#include "MingGoRTSUnit.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"

AMingRTSAIController::AMingRTSAIController()
    : CurrentBehavior(ERTSAIBehavior::Idle)
    , AIState(ERTSAIState::Thinking)
    , Aggressiveness(0.5f)
    , ReactionTime(1.0f)
    , DecisionFrequency(2.0f)
    , PerceptionRange(500.0f)
    , AttackRange(200.0f)
    , RetreatThreshold(0.3f)
    , bIsAIActive(true)
    , bDebugMode(false)
    , LastDecisionTime(0.0f)
    , LastScanTime(0.0f)
    , CurrentDecisionTime(0.0f)
    , LastKnownTargetLocation(FVector::ZeroVector)
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMingRTSAIController::BeginPlay()
{
    Super::BeginPlay();
    
    // 獲取控制的單位
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (ControlledUnit)
    {
        UE_LOG(LogTemp, Log, TEXT("RTS AI Controller started for unit"));
    }
}

void AMingRTSAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bIsAIActive)
    {
        return;
    }
    
    UpdateAI(DeltaTime);
}

void AMingRTSAIController::InitializeAI(UMingRTSUnitManager* InUnitManager, UMingRTSCombatSystem* InCombatSystem)
{
    UnitManager = InUnitManager;
    CombatSystem = InCombatSystem;
    
    if (UnitManager && CombatSystem)
    {
        UE_LOG(LogTemp, Log, TEXT("RTS AI Controller initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RTS AI Controller initialization failed"));
    }
}

void AMingRTSAIController::SetBehavior(ERTSAIBehavior NewBehavior)
{
    if (CurrentBehavior != NewBehavior)
    {
        ERTSAIBehavior OldBehavior = CurrentBehavior;
        CurrentBehavior = NewBehavior;
        
        SetAIState(ERTSAIState::Planning);
        
        NotifyBehaviorChanged(OldBehavior, NewBehavior);
        
        LogAIDebug(FString::Printf(TEXT("Behavior changed to: %s"), *GetCurrentBehaviorName()));
    }
}

void AMingRTSAIController::ExecuteBehavior(float DeltaTime)
{
    switch (CurrentBehavior)
    {
        case ERTSAIBehavior::Idle:
            ExecuteIdleBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Patrol:
            ExecutePatrolBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Guard:
            ExecuteGuardBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Attack:
            ExecuteAttackBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Retreat:
            ExecuteRetreatBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Follow:
            ExecuteFollowBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Gather:
            ExecuteGatherBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Build:
            ExecuteBuildBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Repair:
            ExecuteRepairBehavior(DeltaTime);
            break;
        case ERTSAIBehavior::Explore:
            ExecuteExploreBehavior(DeltaTime);
            break;
    }
}

void AMingRTSAIController::UpdateAI(float DeltaTime)
{
    if (!bIsAIActive)
    {
        return;
    }

    // 更新感知
    UpdatePerception(DeltaTime);
    
    // 檢查是否需要做決策
    if (ShouldMakeDecision())
    {
        MakeAIDecision();
    }
    
    // 執行當前行為
    ExecuteBehavior(DeltaTime);
    
    // 更新目標
    UpdateTarget();
}

void AMingRTSAIController::SetTarget(const FRTSAITarget& NewTarget)
{
    FRTSAITarget OldTarget = CurrentTarget;
    CurrentTarget = NewTarget;
    
    NotifyTargetChanged(OldTarget, NewTarget);
    
    LogAIDebug(FString::Printf(TEXT("Target set: %s"), NewTarget.TargetActor ? *NewTarget.TargetActor->GetName() : TEXT("None")));
}

FRTSAITarget AMingRTSAIController::GetCurrentTarget() const
{
    return CurrentTarget;
}

void AMingRTSAIController::ClearTarget()
{
    FRTSAITarget EmptyTarget;
    SetTarget(EmptyTarget);
}

bool AMingRTSAIController::HasValidTarget() const
{
    return CurrentTarget.bIsValid && CurrentTarget.TargetActor != nullptr;
}

void AMingRTSAIController::UpdateTarget()
{
    if (!HasValidTarget())
    {
        return;
    }

    // 檢查目標是否仍然有效
    if (!CurrentTarget.TargetActor || CurrentTarget.TargetActor->IsPendingKill())
    {
        ClearTarget();
        return;
    }

    // 更新目標信息
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (ControlledUnit)
    {
        CurrentTarget.Distance = FVector::Dist(ControlledUnit->GetActorLocation(), CurrentTarget.TargetActor->GetActorLocation());
        CurrentTarget.ThreatLevel = CalculateThreatLevel(CurrentTarget.TargetActor);
        CurrentTarget.Priority = GetTargetPriority(CurrentTarget.TargetActor);
        CurrentTarget.LastSeen = FDateTime::Now();
    }
}

void AMingRTSAIController::ScanForEnemies(float ScanRadius)
{
    if (!UnitManager)
    {
        return;
    }

    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (!ControlledUnit)
    {
        return;
    }

    DetectedEnemies.Empty();
    
    TArray<AMingGoRTSUnit*> AllUnits = UnitManager->GetAllUnits();
    FVector UnitLocation = ControlledUnit->GetActorLocation();
    
    for (AMingGoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != ControlledUnit)
        {
            float Distance = FVector::Dist(UnitLocation, Unit->GetActorLocation());
            if (Distance <= ScanRadius)
            {
                FRTSAITarget EnemyTarget;
                EnemyTarget.TargetActor = Unit;
                EnemyTarget.TargetLocation = Unit->GetActorLocation();
                EnemyTarget.Distance = Distance;
                EnemyTarget.ThreatLevel = CalculateThreatLevel(Unit);
                EnemyTarget.Priority = GetTargetPriority(Unit);
                EnemyTarget.bIsValid = true;
                EnemyTarget.LastSeen = FDateTime::Now();
                
                DetectedEnemies.Add(EnemyTarget);
            }
        }
    }
    
    LogAIDebug(FString::Printf(TEXT("Scanned for enemies: %d detected"), DetectedEnemies.Num()));
}

void AMingRTSAIController::ScanForAllies(float ScanRadius)
{
    if (!UnitManager)
    {
        return;
    }

    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (!ControlledUnit)
    {
        return;
    }

    DetectedAllies.Empty();
    
    TArray<AMingGoRTSUnit*> AllUnits = UnitManager->GetAllUnits();
    FVector UnitLocation = ControlledUnit->GetActorLocation();
    
    for (AMingGoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != ControlledUnit)
        {
            // 這裡應該檢查是否為友軍
            // 暫時將所有其他單位視為敵人
            float Distance = FVector::Dist(UnitLocation, Unit->GetActorLocation());
            if (Distance <= ScanRadius)
            {
                FRTSAITarget AllyTarget;
                AllyTarget.TargetActor = Unit;
                AllyTarget.TargetLocation = Unit->GetActorLocation();
                AllyTarget.Distance = Distance;
                AllyTarget.ThreatLevel = 0.0f; // 友軍威脅為0
                AllyTarget.Priority = ERTSAITargetPriority::Low;
                AllyTarget.bIsValid = true;
                AllyTarget.LastSeen = FDateTime::Now();
                
                DetectedAllies.Add(AllyTarget);
            }
        }
    }
    
    LogAIDebug(FString::Printf(TEXT("Scanned for allies: %d detected"), DetectedAllies.Num()));
}

void AMingRTSAIController::ScanForResources(float ScanRadius)
{
    // 資源掃描實現
    LogAIDebug(TEXT("Scanning for resources"));
}

TArray<FRTSAITarget> AMingRTSAIController::GetDetectedEnemies() const
{
    return DetectedEnemies;
}

TArray<FRTSAITarget> AMingRTSAIController::GetDetectedAllies() const
{
    return DetectedAllies;
}

FRTSAIDecision AMingRTSAIController::MakeDecision()
{
    FRTSAIDecision Decision;
    
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (!ControlledUnit)
    {
        return Decision;
    }

    // 評估威脅
    EvaluateThreats();
    
    // 根據情況做決策
    if (DetectedEnemies.Num() > 0)
    {
        if (ShouldRetreat())
        {
            Decision.Behavior = ERTSAIBehavior::Retreat;
            Decision.Confidence = 0.8f;
            Decision.Reason = TEXT("Retreating due to overwhelming threat");
        }
        else
        {
            Decision.Behavior = ERTSAIBehavior::Attack;
            Decision.Target = FindBestTarget();
            Decision.Confidence = 0.7f;
            Decision.Reason = TEXT("Engaging enemy target");
        }
    }
    else if (PatrolPath.PatrolPoints.Num() > 0)
    {
        Decision.Behavior = ERTSAIBehavior::Patrol;
        Decision.Confidence = 0.6f;
        Decision.Reason = TEXT("Patrolling along path");
    }
    else
    {
        Decision.Behavior = ERTSAIBehavior::Idle;
        Decision.Confidence = 0.9f;
        Decision.Reason = TEXT("No threats detected, remaining idle");
    }
    
    // 處理決策
    ProcessDecision(Decision);
    
    return Decision;
}

void AMingRTSAIController::EvaluateThreats()
{
    // 掃描敵人
    ScanForEnemies(PerceptionRange);
    
    // 評估威脅等級
    for (FRTSAITarget& Enemy : DetectedEnemies)
    {
        Enemy.ThreatLevel = CalculateThreatLevel(Enemy.TargetActor);
        Enemy.Priority = GetTargetPriority(Enemy.TargetActor);
    }
    
    // 按威脅等級排序
    DetectedEnemies.Sort([this](const FRTSAITarget& A, const FRTSAITarget& B)
    {
        return A.ThreatLevel > B.ThreatLevel;
    });
}

float AMingRTSAIController::CalculateThreatLevel(AActor* Target) const
{
    if (!Target)
    {
        return 0.0f;
    }

    AMingGoRTSUnit* TargetUnit = Cast<AMingGoRTSUnit>(Target);
    if (!TargetUnit)
    {
        return 0.0f;
    }

    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (!ControlledUnit)
    {
        return 0.0f;
    }

    float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), TargetUnit->GetActorLocation());
    float ThreatLevel = 0.0f;
    
    // 距離威脅（距離越近威脅越大）
    float DistanceThreat = FMath::Clamp(1.0f - (Distance / PerceptionRange), 0.0f, 1.0f);
    
    // 單位類型威脅（這裡可以根據單位類型調整）
    float UnitTypeThreat = 0.5f;
    
    // 數量威脅（可以根據周圍友軍數量調整）
    float QuantityThreat = 0.3f;
    
    ThreatLevel = (DistanceThreat * 0.5f) + (UnitTypeThreat * 0.3f) + (QuantityThreat * 0.2f);
    
    return ThreatLevel;
}

ERTSAITargetPriority AMingRTSAIController::GetTargetPriority(AActor* Target) const
{
    if (!Target)
    {
        return ERTSAITargetPriority::None;
    }

    float ThreatLevel = CalculateThreatLevel(Target);
    
    if (ThreatLevel >= 0.8f)
    {
        return ERTSAITargetPriority::Critical;
    }
    else if (ThreatLevel >= 0.6f)
    {
        return ERTSAITargetPriority::High;
    }
    else if (ThreatLevel >= 0.4f)
    {
        return ERTSAITargetPriority::Medium;
    }
    else if (ThreatLevel >= 0.2f)
    {
        return ERTSAITargetPriority::Low;
    }
    else
    {
        return ERTSAITargetPriority::None;
    }
}

void AMingRTSAIController::SetPatrolPath(const FRTSAIPatrolPath& Path)
{
    PatrolPath = Path;
    PatrolPath.CurrentPointIndex = 0;
    
    LogAIDebug(FString::Printf(TEXT("Set patrol path with %d points"), Path.PatrolPoints.Num()));
}

void AMingRTSAIController::AddPatrolPoint(const FVector& Point)
{
    PatrolPath.PatrolPoints.Add(Point);
    
    LogAIDebug(FString::Printf(TEXT("Added patrol point: %s"), *Point.ToString()));
}

void AMingRTSAIController::ClearPatrolPath()
{
    PatrolPath.PatrolPoints.Empty();
    PatrolPath.CurrentPointIndex = 0;
    
    LogAIDebug(TEXT("Cleared patrol path"));
}

FVector AMingRTSAIController::GetNextPatrolPoint()
{
    if (PatrolPath.PatrolPoints.Num() == 0)
    {
        return GetPawn()->GetActorLocation();
    }
    
    FVector CurrentPoint = PatrolPath.PatrolPoints[PatrolPath.CurrentPointIndex];
    
    // 檢查是否到達當前巡邏點
    if (IsAtPatrolPoint())
    {
        // 移動到下一個點
        if (PatrolPath.bReverse)
        {
            PatrolPath.CurrentPointIndex--;
            if (PatrolPath.CurrentPointIndex < 0)
            {
                PatrolPath.CurrentPointIndex = PatrolPath.bLoop ? PatrolPath.PatrolPoints.Num() - 1 : 0;
            }
        }
        else
        {
            PatrolPath.CurrentPointIndex++;
            if (PatrolPath.CurrentPointIndex >= PatrolPath.PatrolPoints.Num())
            {
                PatrolPath.CurrentPointIndex = PatrolPath.bLoop ? 0 : PatrolPath.PatrolPoints.Num() - 1;
            }
        }
    }
    
    return PatrolPath.PatrolPoints[PatrolPath.CurrentPointIndex];
}

bool AMingRTSAIController::IsAtPatrolPoint() const
{
    if (PatrolPath.PatrolPoints.Num() == 0)
    {
        return true;
    }
    
    FVector CurrentPoint = PatrolPath.PatrolPoints[PatrolPath.CurrentPointIndex];
    FVector UnitLocation = GetPawn()->GetActorLocation();
    
    return FVector::Dist(CurrentPoint, UnitLocation) < 50.0f;
}

void AMingRTSAIController::EngageTarget(AActor* Target)
{
    if (!Target)
    {
        return;
    }

    FRTSAITarget NewTarget;
    NewTarget.TargetActor = Target;
    NewTarget.TargetLocation = Target->GetActorLocation();
    NewTarget.bIsValid = true;
    NewTarget.LastSeen = FDateTime::Now();
    
    SetTarget(NewTarget);
    SetBehavior(ERTSAIBehavior::Attack);
    
    LogAIDebug(FString::Printf(TEXT("Engaging target: %s"), *Target->GetName()));
}

void AMingRTSAIController::DisengageTarget()
{
    ClearTarget();
    SetBehavior(ERTSAIBehavior::Idle);
    
    LogAIDebug(TEXT("Disengaging target"));
}

bool AMingRTSAIController::ShouldEngage(AActor* Target) const
{
    if (!Target)
    {
        return false;
    }

    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (!ControlledUnit)
    {
        return false;
    }

    float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), Target->GetActorLocation());
    float ThreatLevel = CalculateThreatLevel(Target);
    
    // 根據攻擊性和威脅等級決定是否攻擊
    return Distance <= AttackRange && ThreatLevel >= (0.3f * Aggressiveness);
}

bool AMingRTSAIController::ShouldRetreat() const
{
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (!ControlledUnit)
    {
        return false;
    }

    // 檢查生命值
    float HealthPercentage = 1.0f; // 這裡應該從單位獲取實際生命值
    
    // 檢查威脅等級
    float TotalThreat = 0.0f;
    for (const FRTSAITarget& Enemy : DetectedEnemies)
    {
        TotalThreat += Enemy.ThreatLevel;
    }
    
    // 如果生命值低或威脅過高，則撤退
    return HealthPercentage <= RetreatThreshold || TotalThreat >= (1.5f * (1.0f - Aggressiveness));
}

void AMingRTSAIController::FindBestAttackPosition()
{
    if (!HasValidTarget())
    {
        return;
    }

    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (!ControlledUnit)
    {
        return;
    }

    FVector TargetLocation = CurrentTarget.TargetActor->GetActorLocation();
    FVector UnitLocation = ControlledUnit->GetActorLocation();
    
    // 計算最佳攻擊位置（在攻擊範圍內）
    FVector Direction = (TargetLocation - UnitLocation).GetSafeNormal();
    FVector BestPosition = TargetLocation - Direction * (AttackRange * 0.8f);
    
    // 移動到最佳位置
    MoveToPosition(BestPosition);
    
    LogAIDebug(FString::Printf(TEXT("Moving to best attack position: %s"), *BestPosition.ToString()));
}

void AMingRTSAIController::MoveToTarget()
{
    if (!HasValidTarget())
    {
        return;
    }

    MoveToPosition(CurrentTarget.TargetLocation);
}

void AMingRTSAIController::MoveToPosition(const FVector& Position)
{
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (ControlledUnit)
    {
        ControlledUnit->MoveToLocation(Position);
        LogAIDebug(FString::Printf(TEXT("Moving to position: %s"), *Position.ToString()));
    }
}

bool AMingRTSAIController::IsMoving() const
{
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (ControlledUnit)
    {
        return UnitManager ? UnitManager->IsUnitMoving(ControlledUnit) : false;
    }
    return false;
}

void AMingRTSAIController::StopMovement()
{
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (ControlledUnit && UnitManager)
    {
        UnitManager->StopUnitMovement(ControlledUnit);
    }
}

void AMingRTSAIController::SetAIState(ERTSAIState NewState)
{
    AIState = NewState;
    
    LogAIDebug(FString::Printf(TEXT("AI State changed to: %s"), *GetCurrentStateName()));
}

void AMingRTSAIController::SetAggressiveness(float Value)
{
    Aggressiveness = FMath::Clamp(Value, 0.0f, 1.0f);
}

void AMingRTSAIController::SetReactionTime(float Value)
{
    ReactionTime = FMath::Max(0.1f, Value);
}

void AMingRTSAIController::SetDecisionFrequency(float Value)
{
    DecisionFrequency = FMath::Max(0.1f, Value);
}

void AMingRTSAIController::SetPerceptionRange(float Value)
{
    PerceptionRange = FMath::Max(100.0f, Value);
}

FString AMingRTSAIController::GetCurrentBehaviorName() const
{
    switch (CurrentBehavior)
    {
        case ERTSAIBehavior::Idle: return TEXT("Idle");
        case ERTSAIBehavior::Patrol: return TEXT("Patrol");
        case ERTSAIBehavior::Guard: return TEXT("Guard");
        case ERTSAIBehavior::Attack: return TEXT("Attack");
        case ERTSAIBehavior::Retreat: return TEXT("Retreat");
        case ERTSAIBehavior::Follow: return TEXT("Follow");
        case ERTSAIBehavior::Gather: return TEXT("Gather");
        case ERTSAIBehavior::Build: return TEXT("Build");
        case ERTSAIBehavior::Repair: return TEXT("Repair");
        case ERTSAIBehavior::Explore: return TEXT("Explore");
        default: return TEXT("Unknown");
    }
}

FString AMingRTSAIController::GetCurrentStateName() const
{
    switch (AIState)
    {
        case ERTSAIState::Thinking: return TEXT("Thinking");
        case ERTSAIState::Planning: return TEXT("Planning");
        case ERTSAIState::Executing: return TEXT("Executing");
        case ERTSAIState::Waiting: return TEXT("Waiting");
        case ERTSAIState::Reacting: return TEXT("Reacting");
        default: return TEXT("Unknown");
    }
}

bool AMingRTSAIController::IsAIActive() const
{
    return bIsAIActive;
}

void AMingRTSAIController::EnableDebugMode(bool bEnable)
{
    bDebugMode = bEnable;
    
    UE_LOG(LogTemp, Warning, TEXT("AI Debug Mode %s"), bEnable ? TEXT("Enabled") : TEXT("Disabled"));
}

void AMingRTSAIController::ExecuteIdleBehavior(float DeltaTime)
{
    // 空閒行為：掃描周圍環境
    ScanForEnemies(PerceptionRange);
    
    // 如果發現威脅，切換到攻擊行為
    if (DetectedEnemies.Num() > 0 && ShouldEngage(DetectedEnemies[0].TargetActor))
    {
        EngageTarget(DetectedEnemies[0].TargetActor);
    }
}

void AMingRTSAIController::ExecutePatrolBehavior(float DeltaTime)
{
    // 巡邏行為：沿著巡邏路徑移動
    FVector NextPoint = GetNextPatrolPoint();
    MoveToPosition(NextPoint);
    
    // 掃描威脅
    ScanForEnemies(PerceptionRange);
    
    // 如果發現威脅，切換到攻擊行為
    if (DetectedEnemies.Num() > 0 && ShouldEngage(DetectedEnemies[0].TargetActor))
    {
        EngageTarget(DetectedEnemies[0].TargetActor);
    }
}

void AMingRTSAIController::ExecuteGuardBehavior(float DeltaTime)
{
    // 守衛行為：保護特定位置
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (ControlledUnit)
    {
        FVector GuardPosition = ControlledUnit->GetActorLocation();
        
        // 掃描威脅
        ScanForEnemies(PerceptionRange);
        
        // 如果發現威脅，攻擊
        if (DetectedEnemies.Num() > 0)
        {
            EngageTarget(DetectedEnemies[0].TargetActor);
        }
        else
        {
            // 返回守衛位置
            if (FVector::Dist(ControlledUnit->GetActorLocation(), GuardPosition) > 100.0f)
            {
                MoveToPosition(GuardPosition);
            }
        }
    }
}

void AMingRTSAIController::ExecuteAttackBehavior(float DeltaTime)
{
    // 攻擊行為：追擊並攻擊目標
    if (HasValidTarget())
    {
        MoveToTarget();
        
        // 檢查是否在攻擊範圍內
        if (CurrentTarget.Distance <= AttackRange)
        {
            // 停止移動並攻擊
            StopMovement();
            
            if (CombatSystem)
            {
                AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
                AMingGoRTSUnit* TargetUnit = Cast<AMingGoRTSUnit>(CurrentTarget.TargetActor);
                
                if (ControlledUnit && TargetUnit)
                {
                    CombatSystem->StartCombat(ControlledUnit, TargetUnit);
                }
            }
        }
    }
    else
    {
        // 沒有目標，切換到空閒行為
        SetBehavior(ERTSAIBehavior::Idle);
    }
}

void AMingRTSAIController::ExecuteRetreatBehavior(float DeltaTime)
{
    // 撤退行為：移動到安全位置
    AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
    if (ControlledUnit)
    {
        FVector RetreatDirection = FVector::ZeroVector;
        
        // 計算撤退方向（遠離威脅）
        for (const FRTSAITarget& Enemy : DetectedEnemies)
        {
            FVector AwayFromEnemy = (ControlledUnit->GetActorLocation() - Enemy.TargetLocation).GetSafeNormal();
            RetreatDirection += AwayFromEnemy;
        }
        
        if (RetreatDirection.SizeSquared() > 0.0f)
        {
            RetreatDirection = RetreatDirection.GetSafeNormal();
            FVector RetreatPosition = ControlledUnit->GetActorLocation() + RetreatDirection * 300.0f;
            MoveToPosition(RetreatPosition);
        }
        
        // 如果威脅消失，切換到空閒行為
        if (DetectedEnemies.Num() == 0)
        {
            SetBehavior(ERTSAIBehavior::Idle);
        }
    }
}

void AMingRTSAIController::ExecuteFollowBehavior(float DeltaTime)
{
    // 跟隨行為：跟隨目標單位
    if (HasValidTarget())
    {
        FVector FollowPosition = CurrentTarget.TargetLocation;
        MoveToPosition(FollowPosition);
    }
    else
    {
        SetBehavior(ERTSAIBehavior::Idle);
    }
}

void AMingRTSAIController::ExecuteGatherBehavior(float DeltaTime)
{
    // 採集行為：採集資源
    LogAIDebug(TEXT("Executing gather behavior"));
}

void AMingRTSAIController::ExecuteBuildBehavior(float DeltaTime)
{
    // 建造行為：建造建築
    LogAIDebug(TEXT("Executing build behavior"));
}

void AMingRTSAIController::ExecuteRepairBehavior(float DeltaTime)
{
    // 修復行為：修復建築或單位
    LogAIDebug(TEXT("Executing repair behavior"));
}

void AMingRTSAIController::ExecuteExploreBehavior(float DeltaTime)
{
    // 探索行為：探索未知區域
    LogAIDebug(TEXT("Executing explore behavior"));
}

void AMingRTSAIController::ProcessDecision(const FRTSAIDecision& Decision)
{
    SetBehavior(Decision.Behavior);
    SetAIState(ERTSAIState::Executing);
    
    if (Decision.Target.TargetActor)
    {
        SetTarget(Decision.Target);
    }
    
    NotifyDecisionMade(Decision, AIState, Decision.Confidence);
    
    LogAIDebug(FString::Printf(TEXT("Decision made: %s - %s"), *Decision.Reason, *GetCurrentBehaviorName()));
}

void AMingRTSAIController::UpdatePerception(float DeltaTime)
{
    LastScanTime += DeltaTime;
    
    // 定期掃描環境
    if (LastScanTime >= 1.0f) // 每秒掃描一次
    {
        ScanForEnemies(PerceptionRange);
        LastScanTime = 0.0f;
    }
}

void AMingRTSAIController::MakeAIDecision()
{
    FRTSAIDecision Decision = MakeDecision();
    LastDecisionTime = GetWorld()->GetTimeSeconds();
}

bool AMingRTSAIController::ShouldMakeDecision() const
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    return (CurrentTime - LastDecisionTime) >= DecisionFrequency;
}

FRTSAITarget AMingRTSAIController::FindBestTarget() const
{
    if (DetectedEnemies.Num() > 0)
    {
        return DetectedEnemies[0]; // 返回威脅最高的敵人
    }
    
    FRTSAITarget EmptyTarget;
    return EmptyTarget;
}

void AMingRTSAIController::LogAIDebug(const FString& Message) const
{
    if (bDebugMode)
    {
        AMingGoRTSUnit* ControlledUnit = Cast<AMingGoRTSUnit>(GetPawn());
        FString UnitName = ControlledUnit ? ControlledUnit->GetName() : TEXT("Unknown");
        UE_LOG(LogTemp, Warning, TEXT("AI [%s]: %s"), *UnitName, *Message);
    }
}

void AMingRTSAIController::NotifyBehaviorChanged(ERTSAIBehavior OldBehavior, ERTSAIBehavior NewBehavior)
{
    OnAIBehaviorChanged.Broadcast(OldBehavior, NewBehavior);
}

void AMingRTSAIController::NotifyTargetChanged(const FRTSAITarget& OldTarget, const FRTSAITarget& NewTarget)
{
    OnAITargetChanged.Broadcast(OldTarget, NewTarget);
}

void AMingRTSAIController::NotifyDecisionMade(const FRTSAIDecision& Decision, ERTSAIState AIState, float Confidence)
{
    OnAIDecisionMade.Broadcast(Decision, AIState, Confidence);
}

// AI行為改進功能
void AMingRTSAIController::ImproveAIBehavior()
{
    if (!bEnableLearning)
    {
        return;
    }

    LogAIDebug("Improving AI behavior...");
    
    // 分析行為模式
    AnalyzeBehaviorPatterns();
    
    // 從經驗中學習
    LearnFromExperience();
    
    // 適應行為
    AdaptBehaviorBasedOnHistory();
    
    LogAIDebug("AI behavior improved");
}

void AMingRTSAIController::OptimizeDecisionMaking()
{
    LogAIDebug("Optimizing decision making...");
    
    // 調整決策頻率基於性能
    if (DecisionHistory.Num() > 0)
    {
        float AverageSuccessRate = 0.0f;
        for (const auto& SuccessRate : BehaviorSuccessRates)
        {
            AverageSuccessRate += SuccessRate.Value;
        }
        AverageSuccessRate /= BehaviorSuccessRates.Num();
        
        // 根據成功率調整決策頻率
        if (AverageSuccessRate < 0.3f)
        {
            DecisionFrequency = FMath::Min(DecisionFrequency * 1.2f, 5.0f);
        }
        else if (AverageSuccessRate > 0.8f)
        {
            DecisionFrequency = FMath::Max(DecisionFrequency * 0.9f, 0.5f);
        }
    }
    
    LogAIDebug("Decision making optimized");
}

void AMingRTSAIController::EnhanceTargetSelection()
{
    LogAIDebug("Enhancing target selection...");
    
    // 優化目標選擇算法
    OptimizeTargetSelection();
    
    LogAIDebug("Target selection enhanced");
}

void AMingRTSAIController::AdaptToPlayerBehavior()
{
    if (!bEnableAdaptation)
    {
        return;
    }

    LogAIDebug("Adapting to player behavior...");
    
    // 根據玩家行為調整AI策略
    if (Aggressiveness > 0.7f)
    {
        // 玩家激進，AI更謹慎
        RetreatThreshold = FMath::Max(RetreatThreshold * 1.1f, 0.5f);
        PerceptionRange = FMath::Max(PerceptionRange * 1.2f, 800.0f);
    }
    else if (Aggressiveness < 0.3f)
    {
        // 玩家被動，AI更主動
        RetreatThreshold = FMath::Min(RetreatThreshold * 0.9f, 0.2f);
        AttackRange = FMath::Max(AttackRange * 1.1f, 300.0f);
    }
    
    LogAIDebug("Adapted to player behavior");
}

void AMingRTSAIController::UpdateDecisionHistory(const FRTSAIDecision& Decision)
{
    // 添加到決策歷史
    DecisionHistory.Add(Decision);
    
    // 限制歷史大小
    if (DecisionHistory.Num() > MemorySize)
    {
        DecisionHistory.RemoveAt(0);
    }
    
    // 更新行為成功率
    FString BehaviorName = GetCurrentBehaviorName();
    float CurrentSuccess = BehaviorSuccessRates.FindRef(BehaviorName);
    float NewSuccess = (CurrentSuccess * 0.8f) + (Decision.Confidence * 0.2f);
    BehaviorSuccessRates.Add(BehaviorName, NewSuccess);
}

void AMingRTSAIController::AnalyzeBehaviorPatterns()
{
    if (DecisionHistory.Num() < 10)
    {
        return;
    }
    
    // 分析最近的決策模式
    TArray<FRTSAIDecision> RecentDecisions;
    for (int32 i = DecisionHistory.Num() - FMath::Min(10, DecisionHistory.Num()); i < DecisionHistory.Num(); i++)
    {
        RecentDecisions.Add(DecisionHistory[i]);
    }
    
    // 計算平均成功率
    float AverageSuccess = 0.0f;
    for (const FRTSAIDecision& Decision : RecentDecisions)
    {
        AverageSuccess += Decision.Confidence;
    }
    AverageSuccess /= RecentDecisions.Num();
    
    // 根據成功率調整行為
    if (AverageSuccess < 0.4f)
    {
        // 成功率低，增加謹慎性
        Aggressiveness = FMath::Max(Aggressiveness * 0.9f, 0.1f);
        ReactionTime = FMath::Min(ReactionTime * 1.1f, 2.0f);
    }
    else if (AverageSuccess > 0.7f)
    {
        // 成功率高，可以更激進
        Aggressiveness = FMath::Min(Aggressiveness * 1.1f, 0.9f);
        ReactionTime = FMath::Max(ReactionTime * 0.9f, 0.5f);
    }
}

void AMingRTSAIController::AdaptBehaviorBasedOnHistory()
{
    // 根據歷史數據適應當前行為
    FString CurrentBehaviorName = GetCurrentBehaviorName();
    float BehaviorSuccess = CalculateBehaviorSuccess(CurrentBehaviorName);
    
    if (BehaviorSuccess < 0.3f)
    {
        // 當前行為成功率低，考慮切換
        LogAIDebug("Current behavior has low success rate, considering change");
        
        // 可以在這裡實施行為切換邏輯
        if (CurrentBehavior == ERTSAIBehavior::Attack)
        {
            SetAIState(ERTSAIState::Thinking);
            CurrentBehavior = ERTSAIBehavior::Guard;
        }
    }
}

void AMingRTSAIController::OptimizeTargetSelection()
{
    // 優化目標選擇邏輯
    if (DetectedEnemies.Num() > 1)
    {
        // 按威脅等級重新排序
        DetectedEnemies.Sort([](const FRTSAITarget& A, const FRTSAITarget& B)
        {
            return A.ThreatLevel > B.ThreatLevel;
        });
        
        // 考慮歷史威脅數據
        for (FRTSAITarget& Target : DetectedEnemies)
        {
            if (Target.TargetActor)
            {
                float HistoricalThreat = TargetThreatHistory.FindRef(Target.TargetActor);
                Target.ThreatLevel = (Target.ThreatLevel * 0.7f) + (HistoricalThreat * 0.3f);
                
                // 更新歷史數據
                TargetThreatHistory.Add(Target.TargetActor, Target.ThreatLevel);
            }
        }
    }
}

void AMingRTSAIController::LearnFromExperience()
{
    // 從經驗中學習
    if (DecisionHistory.Num() < 5)
    {
        return;
    }
    
    // 分析最近的成功和失敗決策
    int32 SuccessCount = 0;
    int32 FailureCount = 0;
    
    for (int32 i = DecisionHistory.Num() - FMath::Min(5, DecisionHistory.Num()); i < DecisionHistory.Num(); i++)
    {
        if (DecisionHistory[i].Confidence > 0.6f)
        {
            SuccessCount++;
        }
        else
        {
            FailureCount++;
        }
    }
    
    // 根據成功/失敗比例調整學習率
    if (FailureCount > SuccessCount)
    {
        LearningRate = FMath::Min(LearningRate * 1.2f, 0.5f);
    }
    else if (SuccessCount > FailureCount)
    {
        LearningRate = FMath::Max(LearningRate * 0.9f, 0.05f);
    }
}

float AMingRTSAIController::CalculateBehaviorSuccess(const FString& BehaviorName) const
{
    return BehaviorSuccessRates.FindRef(BehaviorName);
}
