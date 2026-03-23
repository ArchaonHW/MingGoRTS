// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingSixStrategiesSystem.h"
#include "SageCommand/MingStrategyAuthority.h"
#include "SageCommand/MingMilitaryAuthority.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingSixStrategiesSystem::UMingSixStrategiesSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化系統參數
    DefaultEffectDecayRate = 0.1f;
    MaxSynergyBonus = 0.5f;
    WuXingCoordinationMultiplier = 1.2f;
    
    // 初始化狀態
    bSystemInitialized = false;
}

void UMingSixStrategiesSystem::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化系統
    InitializeSixStrategiesSystem();
    
    // 啟動策略更新定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            StrategyUpdateTimer,
            this,
            &UMingSixStrategiesSystem::UpdateStrategyExecutions,
            STRATEGY_UPDATE_INTERVAL,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("六策系統初始化完成"));
}

void UMingSixStrategiesSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bSystemInitialized)
    {
        return;
    }
    
    // 更新效果衰減
    UpdateEffectDecay(DeltaTime);
}

bool UMingSixStrategiesSystem::InitializeSixStrategiesSystem()
{
    if (bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("六策系統已經初始化"));
        return true;
    }
    
    UE_LOG(LogTemp, Log, TEXT("開始初始化六策系統"));
    
    // 初始化五行映射
    InitializeWuXingMappings();
    
    // 清空活動策略和效果
    ActiveStrategies.Empty();
    CurrentEffects.Empty();
    ActiveCombinations.Empty();
    
    bSystemInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 六策系統初始化完成"));
    UE_LOG(LogTemp, Log, TEXT("  五行映射數量: %d"), WuXingMappings.Num());
    
    return true;
}

void UMingSixStrategiesSystem::ResetSixStrategiesSystem()
{
    UE_LOG(LogTemp, Log, TEXT("重置六策系統"));
    
    // 停止所有活動策略
    for (auto& StrategyPair : ActiveStrategies)
    {
        StopStrategyExecution(StrategyPair.Key);
    }
    
    // 清空所有效果
    CurrentEffects.Empty();
    
    // 清空活動組合
    ActiveCombinations.Empty();
    
    bSystemInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("六策系統已重置"));
}

bool UMingSixStrategiesSystem::ExecuteStrategy(ESixStrategyType StrategyType, const FStrategyExecutionContext& Context)
{
    if (!bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("六策系統未初始化，無法執行策略"));
        return false;
    }
    
    // 檢查是否已經在執行
    if (ActiveStrategies.Contains(StrategyType))
    {
        UE_LOG(LogTemp, Warning, TEXT("策略 %d 已經在執行中"), (int32)StrategyType);
        return false;
    }
    
    // 檢查並發策略限制
    if (ActiveStrategies.Num() >= MAX_CONCURRENT_STRATEGIES)
    {
        UE_LOG(LogTemp, Warning, TEXT("已達到最大並發策略數量限制"));
        return false;
    }
    
    // 檢查策略衝突
    for (auto& ActiveStrategy : ActiveStrategies)
    {
        if (CheckStrategyConflict(StrategyType, ActiveStrategy.Key))
        {
            UE_LOG(LogTemp, Warning, TEXT("策略 %d 與活動策略 %d 衝突"), 
                   (int32)StrategyType, (int32)ActiveStrategy.Key);
            return false;
        }
    }
    
    // 創建執行上下文副本
    FStrategyExecutionContext ExecutionContext = Context;
    ExecutionContext.StrategyType = StrategyType;
    ExecutionContext.StartTime = FDateTime::Now();
    ExecutionContext.ExecutionProgress = 0.0f;
    
    // 計算五行加成
    float WuXingBonus = GetWuXingBonus(StrategyType, ExecutionContext.CurrentPhase);
    ExecutionContext.EffectIntensity *= WuXingBonus;
    
    // 添加到活動策略
    ActiveStrategies.Add(StrategyType, ExecutionContext);
    
    // 觸發執行開始事件
    OnStrategyExecutionStarted.Broadcast(StrategyType, ExecutionContext);
    
    // 應用初始效果
    ApplyInitialStrategyEffects(StrategyType, ExecutionContext);
    
    UE_LOG(LogTemp, Log, TEXT("策略 %d 開始執行，五行加成: %.2f"), 
           (int32)StrategyType, WuXingBonus);
    
    return true;
}

bool UMingSixStrategiesSystem::StopStrategyExecution(ESixStrategyType StrategyType)
{
    if (!ActiveStrategies.Contains(StrategyType))
    {
        UE_LOG(LogTemp, Warning, TEXT("策略 %d 未在執行中"), (int32)StrategyType);
        return false;
    }
    
    // 移除策略
    ActiveStrategies.Remove(StrategyType);
    
    // 觸發完成事件
    OnStrategyExecutionCompleted.Broadcast(StrategyType, false);
    
    UE_LOG(LogTemp, Log, TEXT("策略 %d 執行已停止"), (int32)StrategyType);
    
    return true;
}

EStrategyExecutionStatus UMingSixStrategiesSystem::GetStrategyExecutionStatus(ESixStrategyType StrategyType) const
{
    if (!ActiveStrategies.Contains(StrategyType))
    {
        return EStrategyExecutionStatus::None;
    }
    
    const FStrategyExecutionContext& Context = ActiveStrategies[StrategyType];
    
    if (Context.ExecutionProgress >= 1.0f)
    {
        return EStrategyExecutionStatus::Completed;
    }
    else if (Context.ExecutionProgress > 0.0f)
    {
        return EStrategyExecutionStatus::Executing;
    }
    else
    {
        return EStrategyExecutionStatus::Preparing;
    }
}

TArray<ESixStrategyType> UMingSixStrategiesSystem::GetActiveStrategies() const
{
    TArray<ESixStrategyType> ActiveStrategyList;
    
    for (const auto& StrategyPair : ActiveStrategies)
    {
        ActiveStrategyList.Add(StrategyPair.Key);
    }
    
    return ActiveStrategyList;
}

void UMingSixStrategiesSystem::UpdateStrategyProgress(ESixStrategyType StrategyType, float DeltaProgress)
{
    if (!ActiveStrategies.Contains(StrategyType))
    {
        return;
    }
    
    FStrategyExecutionContext& Context = ActiveStrategies[StrategyType];
    Context.ExecutionProgress = FMath::Clamp(Context.ExecutionProgress + DeltaProgress, 0.0f, 1.0f);
    
    // 檢查是否完成
    if (Context.ExecutionProgress >= 1.0f)
    {
        HandleStrategyCompletion(StrategyType, true);
    }
}

bool UMingSixStrategiesSystem::ApplyStrategyEffect(EStrategyEffectType EffectType, float Value, const TArray<FString>& Targets)
{
    FStrategyEffectDetails EffectDetails;
    EffectDetails.EffectType = EffectType;
    EffectDetails.BaseValue = Value;
    EffectDetails.CurrentValue = Value;
    EffectDetails.AffectedTargets = Targets;
    EffectDetails.StartTime = FDateTime::Now();
    
    // 根據效果類型設置屬性
    switch (EffectType)
    {
    case EStrategyEffectType::MoraleBoost:
        EffectDetails.Duration = 300.0f; // 5分鐘
        EffectDetails.DecayRate = 0.05f;
        break;
        
    case EStrategyEffectType::EconomicGrowth:
        EffectDetails.Duration = 600.0f; // 10分鐘
        EffectDetails.DecayRate = 0.02f;
        break;
        
    case EStrategyEffectType::MilitaryAdvantage:
        EffectDetails.Duration = 180.0f; // 3分鐘
        EffectDetails.DecayRate = 0.1f;
        break;
        
    default:
        EffectDetails.Duration = 120.0f; // 2分鐘
        EffectDetails.DecayRate = 0.08f;
        break;
    }
    
    CurrentEffects.Add(EffectDetails);
    
    // 觸發效果應用事件
    OnStrategyEffectApplied.Broadcast(EffectType, Value, Targets);
    
    UE_LOG(LogTemp, Log, TEXT("應用策略效果: %d, 數值: %.2f, 目標數量: %d"), 
           (int32)EffectType, Value, Targets.Num());
    
    return true;
}

TArray<FStrategyEffectDetails> UMingSixStrategiesSystem::GetCurrentEffects() const
{
    return CurrentEffects;
}

float UMingSixStrategiesSystem::GetWuXingBonus(ESixStrategyType StrategyType, EFiveElementPhase CurrentPhase) const
{
    for (const FWuXingStrategyMapping& Mapping : WuXingMappings)
    {
        if (Mapping.ElementPhase == CurrentPhase && 
            Mapping.SupportedStrategies.Contains(StrategyType))
        {
            return Mapping.ElementBonus;
        }
    }
    
    return 1.0f; // 無加成
}

EWuXingRelationship UMingSixStrategiesSystem::CheckWuXingRelationship(EFiveElementPhase Phase1, EFiveElementPhase Phase2) const
{
    // 五行相生相剋關係
    // 木生火，火生土，土生金，金生水，水生木
    // 木剋土，土剋水，水剋火，火剋金，金剋木
    
    if (Phase1 == EFiveElementPhase::Wood && Phase2 == EFiveElementPhase::Fire) return EWuXingRelationship::Generating;
    if (Phase1 == EFiveElementPhase::Fire && Phase2 == EFiveElementPhase::Earth) return EWuXingRelationship::Generating;
    if (Phase1 == EFiveElementPhase::Earth && Phase2 == EFiveElementPhase::Metal) return EWuXingRelationship::Generating;
    if (Phase1 == EFiveElementPhase::Metal && Phase2 == EFiveElementPhase::Water) return EWuXingRelationship::Generating;
    if (Phase1 == EFiveElementPhase::Water && Phase2 == EFiveElementPhase::Wood) return EWuXingRelationship::Generating;
    
    if (Phase1 == EFiveElementPhase::Wood && Phase2 == EFiveElementPhase::Earth) return EWuXingRelationship::Overcoming;
    if (Phase1 == EFiveElementPhase::Earth && Phase2 == EFiveElementPhase::Water) return EWuXingRelationship::Overcoming;
    if (Phase1 == EFiveElementPhase::Water && Phase2 == EFiveElementPhase::Fire) return EWuXingRelationship::Overcoming;
    if (Phase1 == EFiveElementPhase::Fire && Phase2 == EFiveElementPhase::Metal) return EWuXingRelationship::Overcoming;
    if (Phase1 == EFiveElementPhase::Metal && Phase2 == EFiveElementPhase::Wood) return EWuXingRelationship::Overcoming;
    
    // 反向關係
    if (Phase2 == EFiveElementPhase::Wood && Phase1 == EFiveElementPhase::Fire) return EWuXingRelationship::Generated;
    if (Phase2 == EFiveElementPhase::Fire && Phase1 == EFiveElementPhase::Earth) return EWuXingRelationship::Generated;
    if (Phase2 == EFiveElementPhase::Earth && Phase1 == EFiveElementPhase::Metal) return EWuXingRelationship::Generated;
    if (Phase2 == EFiveElementPhase::Metal && Phase1 == EFiveElementPhase::Water) return EWuXingRelationship::Generated;
    if (Phase2 == EFiveElementPhase::Water && Phase1 == EFiveElementPhase::Wood) return EWuXingRelationship::Generated;
    
    if (Phase2 == EFiveElementPhase::Wood && Phase1 == EFiveElementPhase::Earth) return EWuXingRelationship::Overcome;
    if (Phase2 == EFiveElementPhase::Earth && Phase1 == EFiveElementPhase::Water) return EWuXingRelationship::Overcome;
    if (Phase2 == EFiveElementPhase::Water && Phase1 == EFiveElementPhase::Fire) return EWuXingRelationship::Overcome;
    if (Phase2 == EFiveElementPhase::Fire && Phase1 == EFiveElementPhase::Metal) return EWuXingRelationship::Overcome;
    if (Phase2 == EFiveElementPhase::Metal && Phase1 == EFiveElementPhase::Wood) return EWuXingRelationship::Overcome;
    
    return EWuXingRelationship::None;
}

FStrategyCombination UMingSixStrategiesSystem::CreateStrategyCombination(const TArray<ESixStrategyType>& Strategies)
{
    FStrategyCombination Combination;
    Combination.Strategies = Strategies;
    
    // 計算協效加成
    Combination.SynergyBonus = CalculateSynergyBonus(Strategies);
    
    // 計算風險係數
    Combination.RiskMultiplier = CalculateRiskMultiplier(Strategies);
    
    // 確定最佳五行階段
    Combination.OptimalPhase = DetermineOptimalPhase(Strategies);
    
    // 確定組合效果
    Combination.CombinedEffects = DetermineCombinedEffects(Strategies);
    
    return Combination;
}

float UMingSixStrategiesSystem::CalculateSynergyBonus(const TArray<ESixStrategyType>& Strategies) const
{
    if (Strategies.Num() < 2)
    {
        return 0.0f;
    }
    
    float Synergy = 0.0f;
    
    // 檢查策略間的協效關係
    for (int32 i = 0; i < Strategies.Num(); ++i)
    {
        for (int32 j = i + 1; j < Strategies.Num(); ++j)
        {
            Synergy += CalculateStrategySynergy(Strategies[i], Strategies[j]);
        }
    }
    
    return FMath::Clamp(Synergy, 0.0f, MaxSynergyBonus);
}

bool UMingSixStrategiesSystem::CheckStrategyConflict(EStrategyType Strategy1, ESixStrategyType Strategy2) const
{
    // 檢查正逆策略衝突
    bool IsStrategy1Righteous = IsRighteousStrategy(Strategy1);
    bool IsStrategy2Righteous = IsRighteousStrategy(Strategy2);
    
    // 正逆策略不能同時執行
    if (IsStrategy1Righteous != IsStrategy2Righteous)
    {
        return true;
    }
    
    // 檢查特定策略衝突
    return CheckSpecificStrategyConflict(Strategy1, Strategy2);
}

// ============================================================================
// 內部方法實現
// ============================================================================

void UMingSixStrategiesSystem::InitializeWuXingMappings()
{
    WuXingMappings.Empty();
    
    // 木階段 - 立名正當性
    {
        FWuXingStrategyMapping Mapping;
        Mapping.ElementPhase = EFiveElementPhase::Wood;
        Mapping.ElementBonus = 1.3f;
        Mapping.SupportedStrategies.Add(ESixStrategyType::EstablishNation);
        Mapping.SupportedStrategies.Add(ESixStrategyType::EstablishPeople);
        WuXingMappings.Add(Mapping);
    }
    
    // 火階段 - 造勢加速
    {
        FWuXingStrategyMapping Mapping;
        Mapping.ElementPhase = EFiveElementPhase::Fire;
        Mapping.ElementBonus = 1.4f;
        Mapping.SupportedStrategies.Add(ESixStrategyType::EstablishSystem);
        Mapping.SupportedStrategies.Add(ESixStrategyType::BreakSituation);
        WuXingMappings.Add(Mapping);
    }
    
    // 土階段 - 收權止亂
    {
        FWuXingStrategyMapping Mapping;
        Mapping.ElementPhase = EFiveElementPhase::Earth;
        Mapping.ElementBonus = 1.2f;
        Mapping.SupportedStrategies.Add(ESixStrategyType::EstablishSystem);
        Mapping.SupportedStrategies.Add(ESixStrategyType::BreakStructure);
        WuXingMappings.Add(Mapping);
    }
    
    // 金階段 - 裁斷清除
    {
        FWuXingStrategyMapping Mapping;
        Mapping.ElementPhase = EFiveElementPhase::Metal;
        Mapping.ElementBonus = 1.5f;
        Mapping.SupportedStrategies.Add(ESixStrategyType::BreakSituation);
        Mapping.SupportedStrategies.Add(ESixStrategyType::BreakStructure);
        WuXingMappings.Add(Mapping);
    }
    
    // 水階段 - 留白不動
    {
        FWuXingStrategyMapping Mapping;
        Mapping.ElementPhase = EFiveElementPhase::Water;
        Mapping.ElementBonus = 1.1f;
        Mapping.SupportedStrategies.Add(ESixStrategyType::BreakNotPeople);
        Mapping.SupportedStrategies.Add(ESixStrategyType::EstablishNation);
        WuXingMappings.Add(Mapping);
    }
}

void UMingSixStrategiesSystem::UpdateStrategyExecutions(float DeltaTime)
{
    TArray<ESixStrategyType> CompletedStrategies;
    
    for (auto& StrategyPair : ActiveStrategies)
    {
        ESixStrategyType StrategyType = StrategyPair.Key;
        FStrategyExecutionContext& Context = StrategyPair.Value;
        
        // 更新執行進度
        float ProgressIncrement = DeltaTime / 60.0f; // 假設每個策略需要60秒完成
        UpdateStrategyProgress(StrategyType, ProgressIncrement);
        
        // 檢查是否完成
        if (Context.ExecutionProgress >= 1.0f)
        {
            CompletedStrategies.Add(StrategyType);
        }
    }
    
    // 處理完成的策略
    for (ESixStrategyType StrategyType : CompletedStrategies)
    {
        HandleStrategyCompletion(StrategyType, true);
    }
}

void UMingSixStrategiesSystem::HandleStrategyCompletion(ESixStrategyType StrategyType, bool bSuccess)
{
    if (!ActiveStrategies.Contains(StrategyType))
    {
        return;
    }
    
    const FStrategyExecutionContext& Context = ActiveStrategies[StrategyType];
    
    // 應用完成效果
    if (bSuccess)
    {
        ApplyCompletionEffects(StrategyType, Context);
    }
    
    // 移除活動策略
    ActiveStrategies.Remove(StrategyType);
    
    // 觸發完成事件
    OnStrategyExecutionCompleted.Broadcast(StrategyType, bSuccess);
    
    UE_LOG(LogTemp, Log, TEXT("策略 %d 執行%s"), 
           (int32)StrategyType, bSuccess ? TEXT("完成") : TEXT("失敗"));
}

void UMingSixStrategiesSystem::UpdateEffectDecay(float DeltaTime)
{
    for (int32 i = CurrentEffects.Num() - 1; i >= 0; --i)
    {
        FStrategyEffectDetails& Effect = CurrentEffects[i];
        
        if (!Effect.bIsPermanent)
        {
            // 衰減效果
            Effect.CurrentValue *= (1.0f - Effect.DecayRate * DeltaTime);
            
            // 檢查是否歸零
            if (Effect.CurrentValue <= 0.01f)
            {
                CurrentEffects.RemoveAt(i);
            }
        }
    }
}

bool UMingSixStrategiesSystem::IsRighteousStrategy(ESixStrategyType StrategyType) const
{
    return (StrategyType == ESixStrategyType::EstablishNation ||
            StrategyType == ESixStrategyType::EstablishSystem ||
            StrategyType == ESixStrategyType::EstablishPeople);
}

float UMingSixStrategiesSystem::CalculateStrategySynergy(ESixStrategyType Strategy1, ESixStrategyType Strategy2) const
{
    // 策略協效計算邏輯
    float Synergy = 0.0f;
    
    // 正策之間的協效
    if (IsRighteousStrategy(Strategy1) && IsRighteousStrategy(Strategy2))
    {
        Synergy += 0.2f;
    }
    
    // 逆策之間的協效
    if (!IsRighteousStrategy(Strategy1) && !IsRighteousStrategy(Strategy2))
    {
        Synergy += 0.15f;
    }
    
    // 特定組合協效
    if ((Strategy1 == ESixStrategyType::EstablishNation && Strategy2 == ESixStrategyType::EstablishPeople) ||
        (Strategy2 == ESixStrategyType::EstablishNation && Strategy1 == ESixStrategyType::EstablishPeople))
    {
        Synergy += 0.1f;
    }
    
    return Synergy;
}
