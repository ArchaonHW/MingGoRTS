#include "Optimization/MingHierarchicalTickSystem.h"
#include "MingCombatAI.h"
#include "Units/MingTacticalUnit.h"

// FTickGroup 实现
void FTickGroup::AddUnit(AMingTacticalUnit* Unit)
{
    if (!Unit) return;
    
    // 检查是否已存在
    for (const auto& WeakUnit : Units)
    {
        if (WeakUnit.Get() == Unit)
        {
            return;
        }
    }
    
    Units.Add(Unit);
}

void FTickGroup::RemoveUnit(AMingTacticalUnit* Unit)
{
    if (!Unit) return;
    
    Units.RemoveAll([Unit](const TWeakObjectPtr<AMingTacticalUnit>& WeakUnit) {
        return WeakUnit.Get() == Unit;
    });
}

void FTickGroup::AddAI(AMingCombatAI* AI)
{
    if (!AI) return;
    
    for (const auto& WeakAI : AIControllers)
    {
        if (WeakAI.Get() == AI)
        {
            return;
        }
    }
    
    AIControllers.Add(AI);
}

void FTickGroup::RemoveAI(AMingCombatAI* AI)
{
    if (!AI) return;
    
    AIControllers.RemoveAll([AI](const TWeakObjectPtr<AMingCombatAI>& WeakAI) {
        return WeakAI.Get() == AI;
    });
}

bool FTickGroup::ShouldTick(float DeltaTime)
{
    TimeSinceLastTick += DeltaTime;
    return TimeSinceLastTick >= TickInterval;
}

void FTickGroup::ProcessTick(float DeltaTime)
{
    if (Units.Num() == 0 && AIControllers.Num() == 0)
    {
        TimeSinceLastTick = 0.0f;
        return;
    }

    // 限制每帧处理的单位数，分摊到多个帧
    int32 UnitsToProcess = FMath::Min(MaxUnitsPerFrame, Units.Num());
    int32 AIsToProcess = FMath::Min(MaxUnitsPerFrame, AIControllers.Num());

    // 处理单位
    for (int32 i = 0; i < UnitsToProcess; ++i)
    {
        int32 Index = (CurrentIndex + i) % FMath::Max(1, Units.Num());
        if (Index >= Units.Num()) continue;

        AMingTacticalUnit* Unit = Units[Index].Get();
        if (Unit && Unit->IsValidLowLevel())
        {
            // 调用单位的 Tick
            Unit->Tick(TickInterval);
        }
    }

    // 处理 AI
    for (int32 i = 0; i < AIsToProcess; ++i)
    {
        int32 Index = (CurrentIndex + i) % FMath::Max(1, AIControllers.Num());
        if (Index >= AIControllers.Num()) continue;

        AMingCombatAI* AI = AIControllers[Index].Get();
        if (AI && AI->IsValidLowLevel())
        {
            AI->Tick(TickInterval);
        }
    }

    // 更新索引
    CurrentIndex += FMath::Max(UnitsToProcess, AIsToProcess);
    if (CurrentIndex >= FMath::Max(Units.Num(), AIControllers.Num()))
    {
        CurrentIndex = 0;
    }

    TimeSinceLastTick = 0.0f;
}

void FTickGroup::CleanupInvalidReferences()
{
    Units.RemoveAll([](const TWeakObjectPtr<AMingTacticalUnit>& WeakUnit) {
        return !WeakUnit.IsValid();
    });
    
    AIControllers.RemoveAll([](const TWeakObjectPtr<AMingCombatAI>& WeakAI) {
        return !WeakAI.IsValid();
    });
}

int32 FTickGroup::GetActiveCount() const
{
    int32 Count = 0;
    for (const auto& WeakUnit : Units)
    {
        if (WeakUnit.IsValid()) Count++;
    }
    for (const auto& WeakAI : AIControllers)
    {
        if (WeakAI.IsValid()) Count++;
    }
    return Count;
}

// UMingHierarchicalTickSystem 实现
UMingHierarchicalTickSystem::UMingHierarchicalTickSystem()
    : bIsInitialized(false)
    , bAutoBalancingEnabled(true)
    , AverageTickTimeMs(0.0f)
    , TickedUnitsLastFrame(0)
    , CurrentTickTimeMs(0.0f)
    , TickHistoryIndex(0)
{
    TickTimeHistory.Init(0.0f, 60); // 60帧的历史记录
}

void UMingHierarchicalTickSystem::Initialize()
{
    if (bIsInitialized) return;

    InitializeTickGroups();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("HierarchicalTickSystem initialized"));
}

void UMingHierarchicalTickSystem::Shutdown()
{
    TickGroups.Empty();
    UnitLevels.Empty();
    AILevels.Empty();
    PromotedUnits.Empty();
    
    bIsInitialized = false;
}

void UMingHierarchicalTickSystem::Tick(float DeltaTime)
{
    if (!bIsInitialized) return;

    double StartTime = FPlatformTime::Seconds();

    // 处理临时升级
    ProcessPromotedUnits(DeltaTime);

    // 处理各个层级的 Tick
    TickedUnitsLastFrame = 0;

    for (auto& Pair : TickGroups)
    {
        FTickGroup& Group = Pair.Value;
        
        if (Group.ShouldTick(DeltaTime))
        {
            Group.ProcessTick(DeltaTime);
            TickedUnitsLastFrame += Group.GetActiveCount();
        }
    }

    // 自动负载均衡
    if (bAutoBalancingEnabled)
    {
        PerformLoadBalancing();
    }

    // 清理无效引用 (每60帧执行一次)
    static int32 CleanupCounter = 0;
    if (++CleanupCounter >= 60)
    {
        CleanupInvalidReferences();
        CleanupCounter = 0;
    }

    // 更新统计
    CurrentTickTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
    UpdateTickStats(CurrentTickTimeMs);
}

void UMingHierarchicalTickSystem::RegisterUnit(AMingTacticalUnit* Unit, ETickLevel InitialLevel)
{
    if (!bIsInitialized || !Unit) return;

    // 如果单位已注册，先注销
    if (UnitLevels.Contains(Unit))
    {
        UnregisterUnit(Unit);
    }

    FTickGroup* Group = GetTickGroup(InitialLevel);
    if (Group)
    {
        Group->AddUnit(Unit);
        UnitLevels.Add(Unit, InitialLevel);
    }
}

void UMingHierarchicalTickSystem::UnregisterUnit(AMingTacticalUnit* Unit)
{
    if (!Unit) return;

    ETickLevel* CurrentLevel = UnitLevels.Find(Unit);
    if (CurrentLevel)
    {
        FTickGroup* Group = GetTickGroup(*CurrentLevel);
        if (Group)
        {
            Group->RemoveUnit(Unit);
        }
        UnitLevels.Remove(Unit);
    }

    PromotedUnits.Remove(Unit);
}

void UMingHierarchicalTickSystem::SetUnitTickLevel(AMingTacticalUnit* Unit, ETickLevel NewLevel)
{
    if (!Unit) return;

    ETickLevel* CurrentLevel = UnitLevels.Find(Unit);
    if (CurrentLevel && *CurrentLevel == NewLevel)
    {
        return; // 已经是该层级
    }

    // 从旧层级移除
    if (CurrentLevel)
    {
        FTickGroup* OldGroup = GetTickGroup(*CurrentLevel);
        if (OldGroup)
        {
            OldGroup->RemoveUnit(Unit);
        }
    }

    // 添加到新层级
    FTickGroup* NewGroup = GetTickGroup(NewLevel);
    if (NewGroup)
    {
        NewGroup->AddUnit(Unit);
        UnitLevels.Add(Unit, NewLevel);
    }
}

ETickLevel UMingHierarchicalTickSystem::GetUnitTickLevel(AMingTacticalUnit* Unit) const
{
    if (!Unit) return ETickLevel::Normal;

    const ETickLevel* Level = UnitLevels.Find(Unit);
    return Level ? *Level : ETickLevel::Normal;
}

void UMingHierarchicalTickSystem::RegisterAI(AMingCombatAI* AI, ETickLevel InitialLevel)
{
    if (!bIsInitialized || !AI) return;

    if (AILevels.Contains(AI))
    {
        UnregisterAI(AI);
    }

    FTickGroup* Group = GetTickGroup(InitialLevel);
    if (Group)
    {
        Group->AddAI(AI);
        AILevels.Add(AI, InitialLevel);
    }
}

void UMingHierarchicalTickSystem::UnregisterAI(AMingCombatAI* AI)
{
    if (!AI) return;

    ETickLevel* CurrentLevel = AILevels.Find(AI);
    if (CurrentLevel)
    {
        FTickGroup* Group = GetTickGroup(*CurrentLevel);
        if (Group)
        {
            Group->RemoveAI(AI);
        }
        AILevels.Remove(AI);
    }
}

void UMingHierarchicalTickSystem::SetAITickLevel(AMingCombatAI* AI, ETickLevel NewLevel)
{
    if (!AI) return;

    ETickLevel* CurrentLevel = AILevels.Find(AI);
    if (CurrentLevel && *CurrentLevel == NewLevel)
    {
        return;
    }

    if (CurrentLevel)
    {
        FTickGroup* OldGroup = GetTickGroup(*CurrentLevel);
        if (OldGroup)
        {
            OldGroup->RemoveAI(AI);
        }
    }

    FTickGroup* NewGroup = GetTickGroup(NewLevel);
    if (NewGroup)
    {
        NewGroup->AddAI(AI);
        AILevels.Add(AI, NewLevel);
    }
}

void UMingHierarchicalTickSystem::PromoteToCritical(AMingTacticalUnit* Unit, float DurationSeconds)
{
    if (!Unit) return;

    SetUnitTickLevel(Unit, ETickLevel::Critical);
    PromotedUnits.Add(Unit, DurationSeconds);
}

void UMingHierarchicalTickSystem::DemoteToBackground(AMingTacticalUnit* Unit)
{
    if (!Unit) return;

    SetUnitTickLevel(Unit, ETickLevel::Background);
    PromotedUnits.Remove(Unit);
}

void UMingHierarchicalTickSystem::AutoAdjustLevelByDistance(AMingTacticalUnit* Unit, float DistanceToCamera)
{
    if (!Unit) return;

    ETickLevel NewLevel;
    
    if (DistanceToCamera < 5000.0f) // 近距离
    {
        NewLevel = ETickLevel::High;
    }
    else if (DistanceToCamera < 15000.0f) // 中距离
    {
        NewLevel = ETickLevel::Normal;
    }
    else if (DistanceToCamera < 30000.0f) // 远距离
    {
        NewLevel = ETickLevel::Low;
    }
    else // 很远
    {
        NewLevel = ETickLevel::VeryLow;
    }

    SetUnitTickLevel(Unit, NewLevel);
}

void UMingHierarchicalTickSystem::AdjustLevelByCombatState(AMingTacticalUnit* Unit, bool bInCombat)
{
    if (!Unit) return;

    ETickLevel CurrentLevel = GetUnitTickLevel(Unit);
    
    if (bInCombat)
    {
        // 战斗中提升到 High
        if (CurrentLevel > ETickLevel::High)
        {
            SetUnitTickLevel(Unit, ETickLevel::High);
        }
    }
    else
    {
        // 非战斗状态降级到 Normal
        if (CurrentLevel < ETickLevel::Normal)
        {
            SetUnitTickLevel(Unit, ETickLevel::Normal);
        }
    }
}

void UMingHierarchicalTickSystem::SetTickInterval(ETickLevel Level, float IntervalSeconds)
{
    FTickGroup* Group = GetTickGroup(Level);
    if (Group)
    {
        Group->TickInterval = FMath::Max(0.001f, IntervalSeconds);
    }
}

void UMingHierarchicalTickSystem::SetMaxUnitsPerFrame(ETickLevel Level, int32 MaxUnits)
{
    FTickGroup* Group = GetTickGroup(Level);
    if (Group)
    {
        Group->MaxUnitsPerFrame = FMath::Max(1, MaxUnits);
    }
}

void UMingHierarchicalTickSystem::SetAutoBalancingEnabled(bool bEnabled)
{
    bAutoBalancingEnabled = bEnabled;
}

int32 UMingHierarchicalTickSystem::GetTotalRegisteredUnits() const
{
    int32 Total = 0;
    for (const auto& Pair : TickGroups)
    {
        Total += Pair.Value.GetActiveCount();
    }
    return Total;
}

int32 UMingHierarchicalTickSystem::GetUnitCountByLevel(ETickLevel Level) const
{
    const FTickGroup* Group = GetTickGroup(Level);
    return Group ? Group->GetActiveCount() : 0;
}

float UMingHierarchicalTickSystem::GetCurrentFPS() const
{
    if (AverageTickTimeMs <= 0.0f) return 60.0f;
    return 1000.0f / AverageTickTimeMs;
}

void UMingHierarchicalTickSystem::PrintDebugInfo()
{
    UE_LOG(LogTemp, Log, TEXT("=== Hierarchical Tick System ==="));
    UE_LOG(LogTemp, Log, TEXT("Total Units: %d"), GetTotalRegisteredUnits());
    UE_LOG(LogTemp, Log, TEXT("Ticked Last Frame: %d"), TickedUnitsLastFrame);
    UE_LOG(LogTemp, Log, TEXT("Avg Tick Time: %.3f ms"), AverageTickTimeMs);
    UE_LOG(LogTemp, Log, TEXT("Effective FPS: %.1f"), GetCurrentFPS());
    
    UE_LOG(LogTemp, Log, TEXT("--- Units by Level ---"));
    for (int32 i = 0; i <= static_cast<int32>(ETickLevel::Paused); ++i)
    {
        ETickLevel Level = static_cast<ETickLevel>(i);
        int32 Count = GetUnitCountByLevel(Level);
        if (Count > 0)
        {
            UE_LOG(LogTemp, Log, TEXT("  %s: %d units"), 
                *UEnum::GetValueAsString(Level), Count);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("================================"));
}

void UMingHierarchicalTickSystem::DrawDebugVisualization()
{
    // 调试可视化可以在这里实现
}

FString UMingHierarchicalTickSystem::GetDebugString() const
{
    return FString::Printf(TEXT("Units: %d | Ticked: %d | Time: %.2fms | FPS: %.1f"),
        GetTotalRegisteredUnits(),
        TickedUnitsLastFrame,
        AverageTickTimeMs,
        GetCurrentFPS());
}

void UMingHierarchicalTickSystem::InitializeTickGroups()
{
    for (int32 i = 0; i <= static_cast<int32>(ETickLevel::Paused); ++i)
    {
        ETickLevel Level = static_cast<ETickLevel>(i);
        FTickGroup Group;
        Group.TickLevel = Level;
        Group.TickInterval = GetDefaultInterval(Level);
        Group.MaxUnitsPerFrame = GetDefaultMaxUnits(Level);
        TickGroups.Add(Level, Group);
    }
}

void UMingHierarchicalTickSystem::ProcessPromotedUnits(float DeltaTime)
{
    TArray<TWeakObjectPtr<AMingTacticalUnit>> UnitsToDemote;
    
    for (auto& Pair : PromotedUnits)
    {
        Pair.Value -= DeltaTime;
        
        if (Pair.Value <= 0.0f)
        {
            UnitsToDemote.Add(Pair.Key);
        }
    }
    
    for (const auto& Unit : UnitsToDemote)
    {
        if (Unit.IsValid())
        {
            SetUnitTickLevel(Unit.Get(), ETickLevel::Normal);
        }
        PromotedUnits.Remove(Unit);
    }
}

void UMingHierarchicalTickSystem::PerformLoadBalancing()
{
    // 根据当前的 Tick 时间调整分配
    if (AverageTickTimeMs > 16.67f) // 低于 60 FPS
    {
        // 将 Normal 层级的部分单位降级到 Low
        FTickGroup* NormalGroup = GetTickGroup(ETickLevel::Normal);
        if (NormalGroup && NormalGroup->Units.Num() > 50)
        {
            // 降级 10% 的单位
            int32 UnitsToDemote = NormalGroup->Units.Num() / 10;
            for (int32 i = 0; i < UnitsToDemote && i < NormalGroup->Units.Num(); ++i)
            {
                AMingTacticalUnit* Unit = NormalGroup->Units[i].Get();
                if (Unit)
                {
                    SetUnitTickLevel(Unit, ETickLevel::Low);
                }
            }
        }
    }
    else if (AverageTickTimeMs < 8.33f) // 高于 120 FPS
    {
        // 可以升级一些单位
        FTickGroup* LowGroup = GetTickGroup(ETickLevel::Low);
        if (LowGroup && LowGroup->Units.Num() > 0)
        {
            // 升级 5% 的单位
            int32 UnitsToPromote = FMath::Max(1, LowGroup->Units.Num() / 20);
            for (int32 i = 0; i < UnitsToPromote && i < LowGroup->Units.Num(); ++i)
            {
                AMingTacticalUnit* Unit = LowGroup->Units[i].Get();
                if (Unit)
                {
                    SetUnitTickLevel(Unit, ETickLevel::Normal);
                }
            }
        }
    }
}

ETickLevel UMingHierarchicalTickSystem::CalculateRecommendedLevel(AMingTacticalUnit* Unit) const
{
    if (!Unit) return ETickLevel::Normal;
    
    // 根据单位状态计算推荐层级
    // 简化实现，实际可能需要更多参数
    return ETickLevel::Normal;
}

void UMingHierarchicalTickSystem::UpdateTickStats(float TickTimeMs)
{
    TickTimeHistory[TickHistoryIndex] = TickTimeMs;
    TickHistoryIndex = (TickHistoryIndex + 1) % TickTimeHistory.Num();
    
    // 计算移动平均
    float Sum = 0.0f;
    for (float Time : TickTimeHistory)
    {
        Sum += Time;
    }
    AverageTickTimeMs = Sum / TickTimeHistory.Num();
}

void UMingHierarchicalTickSystem::CleanupInvalidReferences()
{
    for (auto& Pair : TickGroups)
    {
        Pair.Value.CleanupInvalidReferences();
    }
    
    // 清理映射中的无效项
    TArray<TWeakObjectPtr<AMingTacticalUnit>> InvalidUnits;
    for (const auto& Pair : UnitLevels)
    {
        if (!Pair.Key.IsValid())
        {
            InvalidUnits.Add(Pair.Key);
        }
    }
    for (const auto& Unit : InvalidUnits)
    {
        UnitLevels.Remove(Unit);
    }
    
    TArray<TWeakObjectPtr<AMingCombatAI>> InvalidAIs;
    for (const auto& Pair : AILevels)
    {
        if (!Pair.Key.IsValid())
        {
            InvalidAIs.Add(Pair.Key);
        }
    }
    for (const auto& AI : InvalidAIs)
    {
        AILevels.Remove(AI);
    }
}

FTickGroup* UMingHierarchicalTickSystem::GetTickGroup(ETickLevel Level)
{
    return TickGroups.Find(Level);
}

const FTickGroup* UMingHierarchicalTickSystem::GetTickGroup(ETickLevel Level) const
{
    return TickGroups.Find(Level);
}

float UMingHierarchicalTickSystem::GetDefaultInterval(ETickLevel Level)
{
    switch (Level)
    {
    case ETickLevel::Critical:  return 0.0f;       // 每帧
    case ETickLevel::High:      return 0.0f;       // 每帧
    case ETickLevel::Normal:    return 1.0f / 30.0f; // 30 FPS
    case ETickLevel::Low:       return 1.0f / 15.0f; // 15 FPS
    case ETickLevel::VeryLow:   return 1.0f / 5.0f;  // 5 FPS
    case ETickLevel::Background:return 1.0f / 2.0f;  // 2 FPS
    case ETickLevel::Paused:    return 999.0f;      // 几乎不更新
    default: return 1.0f / 30.0f;
    }
}

int32 UMingHierarchicalTickSystem::GetDefaultMaxUnits(ETickLevel Level)
{
    switch (Level)
    {
    case ETickLevel::Critical:  return 100;   // 最多 100 个 Critical 单位
    case ETickLevel::High:      return 200;   // 最多 200 个 High 优先级单位
    case ETickLevel::Normal:    return 500;   // 每帧处理 500 个
    case ETickLevel::Low:       return 300;   // 每帧处理 300 个
    case ETickLevel::VeryLow:   return 200;   // 每帧处理 200 个
    case ETickLevel::Background:return 100;   // 每帧处理 100 个
    case ETickLevel::Paused:    return 0;     // 不主动处理
    default: return 100;
    }
}
