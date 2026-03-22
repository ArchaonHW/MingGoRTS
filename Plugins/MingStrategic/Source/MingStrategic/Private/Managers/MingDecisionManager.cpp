#include "Managers/MingDecisionManager.h"
#include "MingCoreEventBus.h"

// 靜態實例初始化
UMingDecisionManager* UMingDecisionManager::Instance = nullptr;

UMingDecisionManager::UMingDecisionManager()
    : CurrentGameTime(0.0f)
    , bInitialized(false)
{
}

void UMingDecisionManager::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    RegisteredDecisions.Empty();
    ActiveDecisions.Empty();
    DecisionHistory.Empty();
    CompletedDecisionIds.Empty();
    CurrentGameTime = 0.0f;
    
    Instance = this;
    bInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("DecisionManager initialized"));
}

void UMingDecisionManager::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    Instance = nullptr;
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("DecisionManager shutdown"));
}

void UMingDecisionManager::RegisterDecision(const FMingDecision& Decision)
{
    // 檢查是否已註冊
    for (const auto& Registered : RegisteredDecisions)
    {
        if (Registered.DecisionId == Decision.DecisionId)
        {
            UE_LOG(LogTemp, Warning, TEXT("Decision %s already registered"), *Decision.DecisionId);
            return;
        }
    }
    
    RegisteredDecisions.Add(Decision);
    
    UE_LOG(LogTemp, Log, TEXT("Decision registered: %s (%s)"),
        *Decision.DecisionId, *Decision.DecisionTitle.ToString());
}

bool UMingDecisionManager::TriggerDecision(const FString& DecisionId)
{
    if (!bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("DecisionManager not initialized"));
        return false;
    }
    
    // 檢查決策是否已做過
    if (CompletedDecisionIds.Contains(DecisionId))
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision %s already completed"), *DecisionId);
        return false;
    }
    
    // 查找註冊的決策
    FMingDecision* Decision = FindRegisteredDecision(DecisionId);
    if (!Decision)
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision %s not found"), *DecisionId);
        return false;
    }
    
    // 添加到活躍決策列表
    ActiveDecisions.Add(*Decision);
    
    // 發布決策觸發事件
    OnDecisionTriggered.Broadcast(*Decision);
    
    UE_LOG(LogTemp, Log, TEXT("Decision triggered: %s"), *DecisionId);
    
    return true;
}

bool UMingDecisionManager::ExecuteDecisionOption(const FString& DecisionId, const FString& OptionId)
{
    if (!bInitialized)
    {
        return false;
    }
    
    // 查找活躍決策
    FMingDecision* ActiveDecision = FindActiveDecision(DecisionId);
    if (!ActiveDecision)
    {
        UE_LOG(LogTemp, Warning, TEXT("Active decision %s not found"), *DecisionId);
        return false;
    }
    
    // 查找選項
    FMingDecisionOption* SelectedOption = nullptr;
    for (auto& Option : ActiveDecision->Options)
    {
        if (Option.OptionId == OptionId)
        {
            SelectedOption = &Option;
            break;
        }
    }
    
    if (!SelectedOption)
    {
        UE_LOG(LogTemp, Warning, TEXT("Option %s not found in decision %s"), *OptionId, *DecisionId);
        return false;
    }
    
    // 應用效果
    ApplyDecisionEffects(SelectedOption->Effects);
    
    // 記錄決策
    RecordDecision(DecisionId, OptionId, ActiveDecision->DecisionTitle, SelectedOption->OptionTitle);
    
    // 從活躍列表移除
    for (int32 i = 0; i < ActiveDecisions.Num(); ++i)
    {
        if (ActiveDecisions[i].DecisionId == DecisionId)
        {
            ActiveDecisions.RemoveAt(i);
            break;
        }
    }
    
    // 標記為已完成
    CompletedDecisionIds.Add(DecisionId);
    
    // 發布決策完成事件
    OnDecisionMade.Broadcast(DecisionId, OptionId);
    
    UE_LOG(LogTemp, Log, TEXT("Decision %s executed with option %s"), *DecisionId, *OptionId);
    
    // 處理後續決策
    if (!SelectedOption->NextDecisionId.IsEmpty())
    {
        TriggerDecision(SelectedOption->NextDecisionId);
    }
    
    return true;
}

bool UMingDecisionManager::HasDecisionBeenMade(const FString& DecisionId) const
{
    return CompletedDecisionIds.Contains(DecisionId);
}

FMingDecisionRecord UMingDecisionManager::GetDecisionRecord(const FString& DecisionId) const
{
    for (const auto& Record : DecisionHistory)
    {
        if (Record.DecisionId == DecisionId)
        {
            return Record;
        }
    }
    
    return FMingDecisionRecord();
}

void UMingDecisionManager::UpdateGameTime(float InCurrentGameTime)
{
    CurrentGameTime = InCurrentGameTime;
    
    // 處理時間觸發的決策
    ProcessTimeBasedDecisions();
    
    // 檢查過期的決策
    for (int32 i = ActiveDecisions.Num() - 1; i >= 0; --i)
    {
        if (ActiveDecisions[i].IsExpired(CurrentGameTime) && !ActiveDecisions[i].bCanPostpone)
        {
            // 過期且不能延後，自動選擇默認選項或取消
            UE_LOG(LogTemp, Warning, TEXT("Decision %s expired and removed"), *ActiveDecisions[i].DecisionId);
            ActiveDecisions.RemoveAt(i);
        }
    }
}

void UMingDecisionManager::CheckConditionBasedDecisions()
{
    for (const auto& Decision : RegisteredDecisions)
    {
        if (Decision.TriggerType == EDecisionTriggerType::ConditionBased)
        {
            if (!CompletedDecisionIds.Contains(Decision.DecisionId))
            {
                if (CheckTriggerCondition(Decision))
                {
                    TriggerDecision(Decision.DecisionId);
                }
            }
        }
    }
}

void UMingDecisionManager::ProcessTimeBasedDecisions()
{
    for (const auto& Decision : RegisteredDecisions)
    {
        if (Decision.TriggerType == EDecisionTriggerType::TimeBased)
        {
            if (!CompletedDecisionIds.Contains(Decision.DecisionId))
            {
                // 檢查時間條件
                // TODO: 解析 TriggerCondition 中的時間要求
                // 如果時間條件滿足，觸發決策
            }
        }
    }
}

void UMingDecisionManager::ProcessEventBasedDecisions(const FString& EventName)
{
    for (const auto& Decision : RegisteredDecisions)
    {
        if (Decision.TriggerType == EDecisionTriggerType::EventBased)
        {
            if (!CompletedDecisionIds.Contains(Decision.DecisionId))
            {
                // 檢查事件條件
                if (Decision.TriggerCondition.Contains(EventName))
                {
                    TriggerDecision(Decision.DecisionId);
                }
            }
        }
    }
}

bool UMingDecisionManager::CheckTriggerCondition(const FMingDecision& Decision) const
{
    // TODO: 實現條件檢查邏輯
    // 解析 TriggerCondition 並檢查遊戲狀態
    return false;
}

void UMingDecisionManager::ApplyDecisionEffects(const TArray<FMingDecisionEffect>& Effects)
{
    for (const auto& Effect : Effects)
    {
        switch (Effect.EffectType)
        {
        case EDecisionEffectType::ResourceChange:
            // TODO: 調用資源管理器修改資源
            UE_LOG(LogTemp, Log, TEXT("Resource change: %s = %f"), *Effect.TargetKey, Effect.EffectValue);
            break;
            
        case EDecisionEffectType::FactionRelation:
            // TODO: 修改派系關係
            UE_LOG(LogTemp, Log, TEXT("Faction relation change: %s = %f"), *Effect.TargetKey, Effect.EffectValue);
            break;
            
        case EDecisionEffectType::CharacterAttribute:
            // TODO: 修改角色屬性
            UE_LOG(LogTemp, Log, TEXT("Character attribute change: %s = %f"), *Effect.TargetKey, Effect.EffectValue);
            break;
            
        case EDecisionEffectType::UnlockContent:
            // TODO: 解鎖內容
            UE_LOG(LogTemp, Log, TEXT("Unlock content: %s"), *Effect.TargetKey);
            break;
            
        case EDecisionEffectType::TriggerEvent:
            // 觸發事件
            UE_LOG(LogTemp, Log, TEXT("Trigger event: %s"), *Effect.TargetKey);
            break;
            
        case EDecisionEffectType::Custom:
            // 自定義效果
            UE_LOG(LogTemp, Log, TEXT("Custom effect: %s = %f"), *Effect.TargetKey, Effect.EffectValue);
            break;
        }
    }
}

void UMingDecisionManager::RecordDecision(const FString& DecisionId, const FString& OptionId, 
    const FText& DecisionTitle, const FText& OptionTitle)
{
    FMingDecisionRecord Record;
    Record.DecisionId = DecisionId;
    Record.SelectedOptionId = OptionId;
    Record.DecisionTime = CurrentGameTime;
    Record.DecisionTitle = DecisionTitle;
    Record.SelectedOptionTitle = OptionTitle;
    
    // TODO: 格式化遊戲內日期
    Record.InGameDate = FString::Printf(TEXT("Year %d"), FMath::FloorToInt(CurrentGameTime / 365.0f) + 1924);
    
    DecisionHistory.Add(Record);
}

FMingDecision* UMingDecisionManager::FindRegisteredDecision(const FString& DecisionId)
{
    for (auto& Decision : RegisteredDecisions)
    {
        if (Decision.DecisionId == DecisionId)
        {
            return &Decision;
        }
    }
    return nullptr;
}

FMingDecision* UMingDecisionManager::FindActiveDecision(const FString& DecisionId)
{
    for (auto& Decision : ActiveDecisions)
    {
        if (Decision.DecisionId == DecisionId)
        {
            return &Decision;
        }
    }
    return nullptr;
}

UMingDecisionManager* UMingDecisionManager::Get()
{
    return Instance;
}
