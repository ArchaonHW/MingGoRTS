#include "MingRTSAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "MingRTSUnitManager.h"
#include "MingRTSCombatSystem.h"

UMingRTSAIController::UMingRTSAIController()
{
    CurrentWorld = nullptr;
    ControlledUnit = nullptr;
    CurrentState = EAIState::Idle;
    CurrentTarget = nullptr;
    CurrentPatrolIndex = 0;
    DefensivePosition = FVector::ZeroVector;
    bAIEnabled = true;
    AIDifficulty = 0.5f;
    DecisionInterval = 0.5f;
    LastDecisionTime = 0.0f;
    ReactionTimer = 0.0f;
    MemoryDuration = 30.0f;
    LearningRate = 0.1f;
    AdaptabilityFactor = 0.05f;
}

void UMingRTSAIController::InitializeAIController(UWorld* World, AActor* ControlledUnitActor)
{
    if (!World || !ControlledUnitActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSAIController: Invalid world or unit provided"));
        return;
    }

    CurrentWorld = World;
    ControlledUnit = ControlledUnitActor;
    CurrentState = EAIState::Idle;
    CurrentTarget = nullptr;
    CurrentPatrolIndex = 0;
    DefensivePosition = ControlledUnit->GetActorLocation();
    
    // 根據AI類型設置默認參數
    switch (AIParameters.AIType)
    {
        case EAIType::Aggressive:
            AIParameters.AggressionLevel = 0.8f;
            AIParameters.DefensivePriority = 0.3f;
            AIParameters.RiskAssessment = 0.2f;
            break;
        case EAIType::Defensive:
            AIParameters.AggressionLevel = 0.2f;
            AIParameters.DefensivePriority = 0.8f;
            AIParameters.RiskAssessment = 0.8f;
            break;
        case EAIType::Balanced:
            AIParameters.AggressionLevel = 0.5f;
            AIParameters.DefensivePriority = 0.5f;
            AIParameters.RiskAssessment = 0.5f;
            break;
        case EAIType::Cowardly:
            AIParameters.AggressionLevel = 0.1f;
            AIParameters.DefensivePriority = 0.9f;
            AIParameters.RiskAssessment = 0.9f;
            break;
        case EAIType::Tactical:
            AIParameters.TacticalThinking = 0.9f;
            AIParameters.DecisionMakingSpeed = 0.3f;
            AIParameters.TeamworkAbility = 0.8f;
            break;
        case EAIType::Berserker:
            AIParameters.AggressionLevel = 1.0f;
            AIParameters.DefensivePriority = 0.0f;
            AIParameters.RiskAssessment = 0.0f;
            break;
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIController initialized for unit: %s"), *ControlledUnitActor->GetName());
}

void UMingRTSAIController::UpdateAIController(float DeltaTime)
{
    if (!CurrentWorld || !ControlledUnit || !bAIEnabled)
    {
        return;
    }

    // 更新反應計時器
    if (ReactionTimer > 0.0f)
    {
        ReactionTimer -= DeltaTime;
    }

    // 更新AI記憶
    UpdateAIMemory();

    // 評估威脅
    EvaluateThreats();

    // 戰術分析
    AnalyzeTacticalSituation();

    // 處理AI狀態
    ProcessAIState(DeltaTime);

    // 學習和適應
    LearnAndAdapt();

    // 處理協作
    ProcessTeamwork();

    // 定期做出決策
    float CurrentTime = CurrentWorld->GetTimeSeconds();
    if (CurrentTime - LastDecisionTime >= DecisionInterval)
    {
        MakeAIDecision();
        LastDecisionTime = CurrentTime;
    }
}

void UMingRTSAIController::SetAIParameters(const FAIParameters& Parameters)
{
    AIParameters = Parameters;
    
    // 根據新參數調整決策間隔
    DecisionInterval = FMath::Lerp(1.0f, 0.1f, AIParameters.DecisionMakingSpeed);
}

void UMingRTSAIController::SetAIState(EAIState NewState)
{
    if (CurrentState != NewState)
    {
        EAIState OldState = CurrentState;
        CurrentState = NewState;
        
        // 重置反應計時器
        ReactionTimer = AIParameters.ReactionTime;
        
        // 觸發狀態變化事件
        OnAIStateChanged.Broadcast(NewState);
        
        UE_LOG(LogTemp, Log, TEXT("AI state changed from %d to %d for unit: %s"), 
            (int32)OldState, (int32)NewState, *ControlledUnit->GetName());
    }
}

void UMingRTSAIController::HandleThreatDetection(AActor* Threat)
{
    if (!Threat || Threat == ControlledUnit)
    {
        return;
    }

    // 添加到威脅列表
    if (!Threats.Contains(Threat))
    {
        Threats.Add(Threat);
        
        // 記錄到AI記憶
        AIMemory.Add(Threat, CurrentWorld->GetTimeSeconds());
        
        // 根據AI類型反應
        if (AIParameters.AIType == EAIType::Aggressive || AIParameters.AIType == EAIType::Berserker)
        {
            SetAIState(EAIState::Attack);
            SetAttackTarget(Threat);
        }
        else if (AIParameters.AIType == EAIType::Defensive)
        {
            SetAIState(EAIState::Defend);
        }
        else if (AIParameters.AIType == EAIType::Cowardly)
        {
            SetAIState(EAIState::Flee);
        }
    }
}

void UMingRTSAIController::HandleDamageReceived(AActor* DamageSource, float Damage)
{
    if (!DamageSource || DamageSource == ControlledUnit)
    {
        return;
    }

    // 添加到敵人列表
    if (!Enemies.Contains(DamageSource))
    {
        Enemies.Add(DamageSource);
        AIMemory.Add(DamageSource, CurrentWorld->GetTimeSeconds());
    }

    // 根據傷害程度和AI類型反應
    float ThreatLevel = AssessThreatLevel(DamageSource);
    
    if (ThreatLevel > 0.7f || AIParameters.AIType == EAIType::Berserker)
    {
        SetAIState(EAIState::Attack);
        SetAttackTarget(DamageSource);
    }
    else if (AIParameters.AIType == EAIType::Cowardly)
    {
        SetAIState(EAIState::Flee);
    }
    else if (AIParameters.AIType == EAIType::Defensive)
    {
        SetAIState(EAIState::Defend);
    }

    UE_LOG(LogTemp, Log, TEXT("AI received damage from %s: %.1f, Threat level: %.2f"), 
        *DamageSource->GetName(), Damage, ThreatLevel);
}

void UMingRTSAIController::HandleTargetLost(AActor* LostTarget)
{
    if (LostTarget == CurrentTarget)
    {
        CurrentTarget = nullptr;
        
        // 根據AI類型選擇新行為
        if (AIParameters.AIType == EAIType::Aggressive || AIParameters.AIType == EAIType::Berserker)
        {
            // 尋找新的目標
            AActor* NewTarget = SelectBestTarget();
            if (NewTarget)
            {
                SetAttackTarget(NewTarget);
            }
            else
            {
                SetAIState(EAIState::Patrol);
            }
        }
        else
        {
            SetAIState(EAIState::Idle);
        }
    }
}

void UMingRTSAIController::SetPatrolPath(const TArray<FVector>& Waypoints)
{
    PatrolPath = Waypoints;
    CurrentPatrolIndex = 0;
    
    if (PatrolPath.Num() > 0 && CurrentState == EAIState::Idle)
    {
        SetAIState(EAIState::Patrol);
    }
}

void UMingRTSAIController::SetDefensivePosition(FVector Position)
{
    DefensivePosition = Position;
    
    if (CurrentState == EAIState::Idle || CurrentState == EAIState::Patrol)
    {
        SetAIState(EAIState::Defend);
    }
}

void UMingRTSAIController::SetAttackTarget(AActor* Target)
{
    if (Target && Target != ControlledUnit)
    {
        CurrentTarget = Target;
        
        // 添加到記憶
        AIMemory.Add(Target, CurrentWorld->GetTimeSeconds());
        
        // 觸發目標獲取事件
        OnAITargetAcquired.Broadcast(ControlledUnit, Target);
        
        UE_LOG(LogTemp, Log, TEXT("AI acquired target: %s"), *Target->GetName());
    }
}

FAIDecision UMingRTSAIController::GetCurrentDecision() const
{
    return CurrentDecision;
}

void UMingRTSAIController::ExecuteDecision(const FAIDecision& Decision)
{
    CurrentDecision = Decision;
    
    // 執行決策
    switch (Decision.RecommendedState)
    {
        case EAIState::Attack:
            if (Decision.TargetActor)
            {
                SetAttackTarget(Decision.TargetActor);
            }
            else if (Decision.TargetPosition != FVector::ZeroVector)
            {
                // 移動到目標位置
                UMingRTSUnitManager* UnitManager = GetUnitManager();
                if (UnitManager)
                {
                    TArray<AActor*> Units;
                    Units.Add(ControlledUnit);
                    UnitManager->MoveSelectedUnits(Decision.TargetPosition);
                }
            }
            break;
            
        case EAIState::Patrol:
            if (PatrolPath.Num() > 0)
            {
                SetAIState(EAIState::Patrol);
            }
            break;
            
        case EAIState::Defend:
            SetDefensivePosition(Decision.TargetPosition);
            break;
            
        case EAIState::Retreat:
        case EAIState::Flee:
            {
                TArray<FVector> EscapePath = CalculateEscapePath(nullptr);
                if (EscapePath.Num() > 0)
                {
                    UMingRTSUnitManager* UnitManager = GetUnitManager();
                    if (UnitManager)
                    {
                        TArray<AActor*> Units;
                        Units.Add(ControlledUnit);
                        UnitManager->MoveSelectedUnits(EscapePath[0]);
                    }
                }
            }
            break;
            
        default:
            break;
    }
    
    // 觸發決策事件
    OnAIDecisionMade.Broadcast(Decision);
}

FVector UMingRTSAIController::CalculateTacticalPosition(AActor* Target, EAIState State) const
{
    if (!Target)
    {
        return ControlledUnit->GetActorLocation();
    }

    FVector TargetLocation = Target->GetActorLocation();
    FVector CurrentLocation = ControlledUnit->GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

    switch (State)
    {
        case EAIState::Attack:
            {
                // 計算最佳攻擊位置
                return GetBestAttackPosition(Target);
            }
            
        case EAIState::Defend:
            {
                // 計算防禦位置
                float DefenseDistance = 200.0f;
                return CurrentLocation + (-Direction) * DefenseDistance;
            }
            
        case EAIState::Retreat:
            {
                // 計算撤退位置
                float RetreatDistance = 500.0f;
                return CurrentLocation + (-Direction) * RetreatDistance;
            }
            
        default:
            return CurrentLocation;
    }
}

float UMingRTSAIController::AssessThreatLevel(AActor* Threat) const
{
    if (!Threat)
    {
        return 0.0f;
    }

    float ThreatLevel = 0.0f;
    
    // 距離威脅
    float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), Threat->GetActorLocation());
    float DistanceThreat = FMath::Clamp(1.0f - (Distance / 1000.0f), 0.0f, 1.0f);
    
    // 戰鬥力威脅
    UMingRTSCombatSystem* CombatSystem = GetCombatSystem();
    if (CombatSystem)
    {
        FCombatStats ThreatStats = CombatSystem->GetUnitStats(Threat);
        FCombatStats MyStats = CombatSystem->GetUnitStats(ControlledUnit);
        
        float CombatPowerThreat = FMath::Clamp(ThreatStats.AttackPower / MyStats.DefensePower, 0.0f, 2.0f);
        ThreatLevel = (DistanceThreat + CombatPowerThreat) / 2.0f;
    }
    else
    {
        ThreatLevel = DistanceThreat;
    }
    
    return FMath::Clamp(ThreatLevel, 0.0f, 1.0f);
}

TArray<FVector> UMingRTSAIController::CalculateEscapePath(AActor* Threat) const
{
    TArray<FVector> EscapePath;
    
    if (!ControlledUnit)
    {
        return EscapePath;
    }

    FVector CurrentLocation = ControlledUnit->GetActorLocation();
    FVector ThreatLocation = Threat ? Threat->GetActorLocation() : CurrentLocation;
    FVector EscapeDirection = (CurrentLocation - ThreatLocation).GetSafeNormal();
    
    // 生成多個逃跑路徑點
    for (int32 i = 1; i <= 3; i++)
    {
        FVector EscapePoint = CurrentLocation + EscapeDirection * (200.0f * i);
        
        // 添加一些隨機偏移
        EscapePoint.X += FMath::RandRange(-50.0f, 50.0f);
        EscapePoint.Y += FMath::RandRange(-50.0f, 50.0f);
        
        EscapePath.Add(EscapePoint);
    }
    
    return EscapePath;
}

bool UMingRTSAIController::ShouldRetreat() const
{
    // 檢查生命值
    UMingRTSCombatSystem* CombatSystem = GetCombatSystem();
    if (CombatSystem)
    {
        FCombatStats MyStats = CombatSystem->GetUnitStats(ControlledUnit);
        float HealthPercentage = MyStats.CurrentHealth / MyStats.MaxHealth;
        
        // 根據AI類型和生命值決定是否撤退
        float RetreatThreshold = 0.3f; // 默認30%生命值撤退
        
        switch (AIParameters.AIType)
        {
            case EAIType::Berserker:
                RetreatThreshold = 0.1f;
                break;
            case EAIType::Aggressive:
                RetreatThreshold = 0.2f;
                break;
            case EAIType::Balanced:
                RetreatThreshold = 0.3f;
                break;
            case EAIType::Defensive:
                RetreatThreshold = 0.4f;
                break;
            case EAIType::Cowardly:
                RetreatThreshold = 0.6f;
                break;
            case EAIType::Tactical:
                RetreatThreshold = 0.35f;
                break;
        }
        
        return HealthPercentage < RetreatThreshold;
    }
    
    return false;
}

bool UMingRTSAIController::ShouldChase(AActor* Target) const
{
    if (!Target)
    {
        return false;
    }

    // 根據AI類型決定是否追擊
    switch (AIParameters.AIType)
    {
        case EAIType::Berserker:
        case EAIType::Aggressive:
            return true;
        case EAIType::Balanced:
            return AssessThreatLevel(Target) < 0.5f;
        case EAIType::Tactical:
            return EvaluateTacticalAdvantage(Target) > 0.6f;
        case EAIType::Defensive:
        case EAIType::Cowardly:
            return false;
        default:
            return false;
    }
}

FVector UMingRTSAIController::GetBestAttackPosition(AActor* Target) const
{
    if (!Target)
    {
        return ControlledUnit->GetActorLocation();
    }

    FVector TargetLocation = Target->GetActorLocation();
    FVector CurrentLocation = ControlledUnit->GetActorLocation();
    
    // 獲取戰鬥系統以確定攻擊範圍
    UMingRTSCombatSystem* CombatSystem = GetCombatSystem();
    float AttackRange = 100.0f; // 默認攻擊範圍
    
    if (CombatSystem)
    {
        FCombatStats MyStats = CombatSystem->GetUnitStats(ControlledUnit);
        AttackRange = MyStats.AttackRange;
    }
    
    // 計算最佳攻擊位置
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    FVector BestPosition = TargetLocation - Direction * (AttackRange * 0.8f);
    
    // 添加一些戰術偏移
    if (AIParameters.TacticalThinking > 0.5f)
    {
        // 嘗試側翼攻擊
        FVector SideDirection = FVector(Direction.Y, -Direction.X, 0).GetSafeNormal();
        BestPosition += SideDirection * FMath::RandRange(-50.0f, 50.0f);
    }
    
    return BestPosition;
}

void UMingRTSAIController::SetAIDifficulty(float Difficulty)
{
    AIDifficulty = FMath::Clamp(Difficulty, 0.0f, 1.0f);
    
    // 根據難度調整AI參數
    AIParameters.DecisionMakingSpeed = FMath::Lerp(0.3f, 0.9f, Difficulty);
    AIParameters.TacticalThinking = FMath::Lerp(0.3f, 0.8f, Difficulty);
    AIParameters.ReactionTime = FMath::Lerp(1.0f, 0.2f, Difficulty);
    AIParameters.RiskAssessment = FMath::Lerp(0.8f, 0.3f, Difficulty);
    
    // 調整決策間隔
    DecisionInterval = FMath::Lerp(1.0f, 0.2f, Difficulty);
}

void UMingRTSAIController::MakeAIDecision()
{
    if (ReactionTimer > 0.0f)
    {
        return; // 仍在反應中
    }

    FAIDecision Decision;
    Decision.Priority = 0.0f;
    
    // 評估當前情況
    if (CurrentTarget && IsUnitAlive(CurrentTarget))
    {
        float ThreatLevel = AssessThreatLevel(CurrentTarget);
        float TacticalAdvantage = EvaluateTacticalAdvantage(CurrentTarget);
        
        if (ShouldRetreat())
        {
            Decision.RecommendedState = EAIState::Flee;
            Decision.TargetPosition = CalculateEscapePath(CurrentTarget)[0];
            Decision.Priority = 0.9f;
            Decision.Confidence = 0.8f;
            Decision.Reasoning = TEXT("Low health, retreating");
        }
        else if (ThreatLevel > 0.7f && TacticalAdvantage < 0.3f)
        {
            Decision.RecommendedState = EAIState::Defend;
            Decision.TargetPosition = DefensivePosition;
            Decision.Priority = 0.7f;
            Decision.Confidence = 0.6f;
            Decision.Reasoning = TEXT("High threat, defending");
        }
        else
        {
            Decision.RecommendedState = EAIState::Attack;
            Decision.TargetActor = CurrentTarget;
            Decision.TargetPosition = GetBestAttackPosition(CurrentTarget);
            Decision.Priority = 0.8f;
            Decision.Confidence = 0.7f;
            Decision.Reasoning = TEXT("Engaging target");
        }
    }
    else
    {
        // 沒有當前目標，尋找新目標或巡邏
        AActor* BestTarget = SelectBestTarget();
        if (BestTarget)
        {
            Decision.RecommendedState = EAIState::Attack;
            Decision.TargetActor = BestTarget;
            Decision.TargetPosition = GetBestAttackPosition(BestTarget);
            Decision.Priority = 0.6f;
            Decision.Confidence = 0.5f;
            Decision.Reasoning = TEXT("Acquired new target");
        }
        else if (PatrolPath.Num() > 0)
        {
            Decision.RecommendedState = EAIState::Patrol;
            Decision.TargetPosition = PatrolPath[CurrentPatrolIndex];
            Decision.Priority = 0.3f;
            Decision.Confidence = 0.4f;
            Decision.Reasoning = TEXT("Patrolling");
        }
        else
        {
            Decision.RecommendedState = EAIState::Idle;
            Decision.TargetPosition = ControlledUnit->GetActorLocation();
            Decision.Priority = 0.1f;
            Decision.Confidence = 0.3f;
            Decision.Reasoning = TEXT("No threats detected");
        }
    }
    
    // 執行決策
    ExecuteDecision(Decision);
}

void UMingRTSAIController::ProcessAIState(float DeltaTime)
{
    switch (CurrentState)
    {
        case EAIState::Idle:
            ProcessIdleState(DeltaTime);
            break;
        case EAIState::Patrol:
            ProcessPatrolState(DeltaTime);
            break;
        case EAIState::Attack:
            ProcessAttackState(DeltaTime);
            break;
        case EAIState::Defend:
            ProcessDefendState(DeltaTime);
            break;
        case EAIState::Retreat:
            ProcessRetreatState(DeltaTime);
            break;
        case EAIState::Chase:
            ProcessChaseState(DeltaTime);
            break;
        case EAIState::Flee:
            ProcessFleeState(DeltaTime);
            break;
    }
}

void UMingRTSAIController::EvaluateThreats()
{
    // 清理過期威脅
    for (int32 i = Threats.Num() - 1; i >= 0; --i)
    {
        AActor* Threat = Threats[i];
        if (!Threat || !IsUnitAlive(Threat))
        {
            Threats.RemoveAt(i);
        }
    }
    
    // 更新威脅等級
    UpdateThreatLevels();
}

void UMingRTSAIController::AnalyzeTacticalSituation()
{
    // 戰術分析邏輯
    // 評估地形優勢、團隊協作、包圍機會等
}

void UMingRTSAIController::UpdateAIMemory()
{
    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    // 清理過期記憶
    CleanupExpiredMemory();
    
    // 更新記憶強度
    for (TPair<TObjectPtr<AActor>, float>& MemoryPair : AIMemory)
    {
        float Age = CurrentTime - MemoryPair.Value;
        if (Age > MemoryDuration)
        {
            AIMemory.Remove(MemoryPair.Key);
        }
    }
}

void UMingRTSAIController::LearnAndAdapt()
{
    // 學習戰鬥模式
    LearnCombatPatterns();
    
    // 適應AI參數
    AdaptAIParameters();
}

void UMingRTSAIController::ProcessTeamwork()
{
    // 處理團隊協作邏輯
    if (AIParameters.TeamworkAbility > 0.5f)
    {
        // 評估團隊協調
        float TeamCoordination = EvaluateTeamCoordination();
        
        // 根據協調程度調整行為
        if (TeamCoordination > 0.7f)
        {
            // 支援盟友
            SupportAllies();
        }
    }
}

void UMingRTSAIController::ProcessIdleState(float DeltaTime)
{
    // 閒置狀態處理
    // 尋找威脅或目標
    AActor* BestTarget = SelectBestTarget();
    if (BestTarget)
    {
        SetAttackTarget(BestTarget);
    }
    else if (PatrolPath.Num() > 0)
    {
        SetAIState(EAIState::Patrol);
    }
}

void UMingRTSAIController::ProcessPatrolState(float DeltaTime)
{
    // 巡邏狀態處理
    if (PatrolPath.Num() == 0)
    {
        SetAIState(EAIState::Idle);
        return;
    }
    
    // 檢查是否到達當前巡邏點
    FVector CurrentPosition = ControlledUnit->GetActorLocation();
    FVector TargetPosition = PatrolPath[CurrentPatrolIndex];
    float Distance = FVector::Dist(CurrentPosition, TargetPosition);
    
    if (Distance < 50.0f)
    {
        // 到達巡邏點，移動到下一個
        CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPath.Num();
    }
    
    // 移動到巡邏點
    UMingRTSUnitManager* UnitManager = GetUnitManager();
    if (UnitManager)
    {
        TArray<AActor*> Units;
        Units.Add(ControlledUnit);
        UnitManager->MoveSelectedUnits(TargetPosition);
    }
    
    // 檢查威脅
    AActor* Threat = GetNearestThreat();
    if (Threat)
    {
        HandleThreatDetection(Threat);
    }
}

void UMingRTSAIController::ProcessAttackState(float DeltaTime)
{
    // 攻擊狀態處理
    if (!CurrentTarget || !IsUnitAlive(CurrentTarget))
    {
        HandleTargetLost(CurrentTarget);
        return;
    }
    
    // 檢查是否在攻擊範圍內
    UMingRTSCombatSystem* CombatSystem = GetCombatSystem();
    if (CombatSystem && !CombatSystem->IsInRange(ControlledUnit, CurrentTarget))
    {
        // 移動到攻擊位置
        FVector AttackPosition = GetBestAttackPosition(CurrentTarget);
        UMingRTSUnitManager* UnitManager = GetUnitManager();
        if (UnitManager)
        {
            TArray<AActor*> Units;
            Units.Add(ControlledUnit);
            UnitManager->MoveSelectedUnits(AttackPosition);
        }
    }
    else
    {
        // 在攻擊範圍內，執行攻擊
        CombatSystem->StartAttack(ControlledUnit, CurrentTarget);
    }
}

void UMingRTSAIController::ProcessDefendState(float DeltaTime)
{
    // 防禦狀態處理
    FVector CurrentPosition = ControlledUnit->GetActorLocation();
    float Distance = FVector::Dist(CurrentPosition, DefensivePosition);
    
    if (Distance > 100.0f)
    {
        // 移動到防禦位置
        UMingRTSUnitManager* UnitManager = GetUnitManager();
        if (UnitManager)
        {
            TArray<AActor*> Units;
            Units.Add(ControlledUnit);
            UnitManager->MoveSelectedUnits(DefensivePosition);
        }
    }
    
    // 檢查威脅
    AActor* Threat = GetNearestThreat();
    if (Threat && FVector::Dist(CurrentPosition, Threat->GetActorLocation()) < 300.0f)
    {
        SetAIState(EAIState::Attack);
        SetAttackTarget(Threat);
    }
}

void UMingRTSAIController::ProcessRetreatState(float DeltaTime)
{
    // 撤退狀態處理
    if (Threats.Num() > 0)
    {
        AActor* PrimaryThreat = Threats[0];
        TArray<FVector> EscapePath = CalculateEscapePath(PrimaryThreat);
        
        if (EscapePath.Num() > 0)
        {
            UMingRTSUnitManager* UnitManager = GetUnitManager();
            if (UnitManager)
            {
                TArray<AActor*> Units;
                Units.Add(ControlledUnit);
                UnitManager->MoveSelectedUnits(EscapePath[0]);
            }
        }
    }
    else
    {
        // 沒有威脅，切換到防禦狀態
        SetAIState(EAIState::Defend);
    }
}

void UMingRTSAIController::ProcessChaseState(float DeltaTime)
{
    // 追擊狀態處理
    if (!CurrentTarget || !IsUnitAlive(CurrentTarget))
    {
        HandleTargetLost(CurrentTarget);
        return;
    }
    
    // 追擊目標
    FVector TargetPosition = CurrentTarget->GetActorLocation();
    UMingRTSUnitManager* UnitManager = GetUnitManager();
    if (UnitManager)
    {
        TArray<AActor*> Units;
        Units.Add(ControlledUnit);
        UnitManager->MoveSelectedUnits(TargetPosition);
    }
    
    // 檢查是否應該停止追擊
    if (!ShouldChase(CurrentTarget))
    {
        SetAIState(EAIState::Idle);
    }
}

void UMingRTSAIController::ProcessFleeState(float DeltaTime)
{
    // 逃跑狀態處理
    ProcessRetreatState(DeltaTime);
}

float UMingRTSAIController::EvaluateTacticalAdvantage(AActor* Target) const
{
    if (!Target)
    {
        return 0.0f;
    }

    float Advantage = 0.5f; // 默認均勢
    
    // 戰鬥力比較
    UMingRTSCombatSystem* CombatSystem = GetCombatSystem();
    if (CombatSystem)
    {
        FCombatStats MyStats = CombatSystem->GetUnitStats(ControlledUnit);
        FCombatStats TargetStats = CombatSystem->GetUnitStats(Target);
        
        float CombatPowerRatio = MyStats.AttackPower / TargetStats.DefensePower;
        Advantage = FMath::Clamp(CombatPowerRatio, 0.0f, 1.0f);
    }
    
    // 數量優勢
    int32 NearbyAllies = GetNearbyAllies().Num();
    int32 NearbyEnemies = GetNearbyEnemies().Num();
    
    if (NearbyEnemies > 0)
    {
        float NumericalAdvantage = (float)NearbyAllies / (float)NearbyEnemies;
        Advantage = (Advantage + NumericalAdvantage) / 2.0f;
    }
    
    return FMath::Clamp(Advantage, 0.0f, 1.0f);
}

float UMingRTSAIController::CalculateRiskScore(AActor* Target) const
{
    if (!Target)
    {
        return 0.0f;
    }

    float RiskScore = 0.0f;
    
    // 威脅等級
    float ThreatLevel = AssessThreatLevel(Target);
    RiskScore += ThreatLevel * 0.4f;
    
    // 戰術劣勢
    float TacticalDisadvantage = 1.0f - EvaluateTacticalAdvantage(Target);
    RiskScore += TacticalDisadvantage * 0.3f;
    
    // 距離風險
    float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), Target->GetActorLocation());
    float DistanceRisk = FMath::Clamp(1.0f - (Distance / 1000.0f), 0.0f, 1.0f);
    RiskScore += DistanceRisk * 0.3f;
    
    return FMath::Clamp(RiskScore, 0.0f, 1.0f);
}

AActor* UMingRTSAIController::SelectBestTarget() const
{
    AActor* BestTarget = nullptr;
    float BestScore = 0.0f;
    
    // 評估所有潛在目標
    for (AActor* PotentialTarget : Enemies)
    {
        if (!IsUnitAlive(PotentialTarget))
        {
            continue;
        }
        
        float Score = 0.0f;
        
        // 威脅等級
        float ThreatLevel = AssessThreatLevel(PotentialTarget);
        Score += ThreatLevel * 0.3f;
        
        // 戰術優勢
        float TacticalAdvantage = EvaluateTacticalAdvantage(PotentialTarget);
        Score += TacticalAdvantage * 0.4f;
        
        // 距離因素
        float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), PotentialTarget->GetActorLocation());
        float DistanceScore = FMath::Clamp(1.0f - (Distance / 500.0f), 0.0f, 1.0f);
        Score += DistanceScore * 0.3f;
        
        if (Score > BestScore)
        {
            BestScore = Score;
            BestTarget = PotentialTarget;
        }
    }
    
    return BestTarget;
}

void UMingRTSAIController::UpdateThreatLevels()
{
    // 更新威脅等級邏輯
    // 可以根據時間、距離、行為等調整威脅等級
}

void UMingRTSAIController::CleanupExpiredMemory()
{
    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    for (auto It = AIMemory.CreateIterator(); It; ++It)
    {
        float MemoryAge = CurrentTime - It->Value;
        if (MemoryAge > MemoryDuration)
        {
            It.RemoveCurrent();
        }
    }
}

void UMingRTSAIController::LearnCombatPatterns()
{
    // 學習戰鬥模式邏輯
    // 可以分析戰鬥結果，調整AI參數
}

void UMingRTSAIController::AdaptAIParameters()
{
    // 適應AI參數邏輯
    // 根據戰鬥結果和環境調整AI行為
}

bool UMingRTSAIController::HasLineOfSight(AActor* Target) const
{
    if (!CurrentWorld || !ControlledUnit || !Target)
    {
        return false;
    }
    
    FVector Start = ControlledUnit->GetActorLocation();
    FVector End = Target->GetActorLocation();
    
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(ControlledUnit);
    QueryParams.AddIgnoredActor(Target);
    
    return !CurrentWorld->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
}

FVector UMingRTSAIController::FindCoverPosition(AActor* Threat) const
{
    if (!Threat)
    {
        return ControlledUnit->GetActorLocation();
    }
    
    FVector CurrentPosition = ControlledUnit->GetActorLocation();
    FVector ThreatPosition = Threat->GetActorLocation();
    FVector AwayFromThreat = (CurrentPosition - ThreatPosition).GetSafeNormal();
    
    // 尋找掩護位置
    FVector CoverPosition = CurrentPosition + AwayFromThreat * 200.0f;
    
    return CoverPosition;
}

float UMingRTSAIController::EvaluateTeamCoordination() const
{
    // 評估團隊協調邏輯
    float Coordination = 0.0f;
    
    // 檢查盟友數量和位置
    int32 NearbyAllies = GetNearbyAllies().Num();
    Coordination += FMath::Clamp((float)NearbyAllies / 5.0f, 0.0f, 1.0f) * 0.5f;
    
    // 檢查團隊協作能力
    Coordination += AIParameters.TeamworkAbility * 0.5f;
    
    return FMath::Clamp(Coordination, 0.0f, 1.0f);
}

void UMingRTSAIController::SupportAllies()
{
    // 支援盟友邏輯
    for (AActor* Ally : Allies)
    {
        if (!IsUnitAlive(Ally))
        {
            continue;
        }
        
        // 檢查盟友是否需要幫助
        UMingRTSCombatSystem* CombatSystem = GetCombatSystem();
        if (CombatSystem)
        {
            FCombatStats AllyStats = CombatSystem->GetUnitStats(Ally);
            float HealthPercentage = AllyStats.CurrentHealth / AllyStats.MaxHealth;
            
            if (HealthPercentage < 0.5f)
            {
                // 移動到盟友附近提供支援
                FVector AllyPosition = Ally->GetActorLocation();
                FVector SupportPosition = AllyPosition + (ControlledUnit->GetActorLocation() - AllyPosition).GetSafeNormal() * 100.0f;
                
                UMingRTSUnitManager* UnitManager = GetUnitManager();
                if (UnitManager)
                {
                    TArray<AActor*> Units;
                    Units.Add(ControlledUnit);
                    UnitManager->MoveSelectedUnits(SupportPosition);
                }
                
                break; // 一次只支援一個盟友
            }
        }
    }
}

TArray<AActor*> UMingRTSAIController::GetNearbyAllies() const
{
    TArray<AActor*> NearbyAllies;
    
    for (AActor* Ally : Allies)
    {
        if (!IsUnitAlive(Ally))
        {
            continue;
        }
        
        float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), Ally->GetActorLocation());
        if (Distance <= 500.0f) // 500單位範圍內的盟友
        {
            NearbyAllies.Add(Ally);
        }
    }
    
    return NearbyAllies;
}

TArray<AActor*> UMingRTSAIController::GetNearbyEnemies() const
{
    TArray<AActor*> NearbyEnemies;
    
    for (AActor* Enemy : Enemies)
    {
        if (!IsUnitAlive(Enemy))
        {
            continue;
        }
        
        float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), Enemy->GetActorLocation());
        if (Distance <= 500.0f) // 500單位範圍內的敵人
        {
            NearbyEnemies.Add(Enemy);
        }
    }
    
    return NearbyEnemies;
}

AActor* UMingRTSAIController::GetNearestThreat() const
{
    AActor* NearestThreat = nullptr;
    float NearestDistance = 1000.0f;
    
    for (AActor* Threat : Threats)
    {
        if (!IsUnitAlive(Threat))
        {
            continue;
        }
        
        float Distance = FVector::Dist(ControlledUnit->GetActorLocation(), Threat->GetActorLocation());
        if (Distance < NearestDistance)
        {
            NearestDistance = Distance;
            NearestThreat = Threat;
        }
    }
    
    return NearestThreat;
}

bool UMingRTSAIController::IsUnitAlive(AActor* Unit) const
{
    if (!Unit)
    {
        return false;
    }
    
    UMingRTSCombatSystem* CombatSystem = GetCombatSystem();
    if (CombatSystem)
    {
        return CombatSystem->IsUnitAlive(Unit);
    }
    
    return !Unit->IsPendingKill();
}

UMingRTSUnitManager* UMingRTSAIController::GetUnitManager() const
{
    // 這裡需要從遊戲模式或其他地方獲取單位管理器
    // 簡化實現，返回nullptr
    return nullptr;
}

UMingRTSCombatSystem* UMingRTSAIController::GetCombatSystem() const
{
    // 這裡需要從遊戲模式或其他地方獲取戰鬥系統
    // 簡化實現，返回nullptr
    return nullptr;
}
