#include "Tutorial/MingRTSAdaptiveGuide.h"
#include "TimerManager.h"
#include "Engine/World.h"

UMingRTSAdaptiveGuide::UMingRTSAdaptiveGuide()
    : bGuideEnabled(true)
    , EvaluationInterval(5.0f)
{
}

void UMingRTSAdaptiveGuide::InitializeAdaptiveGuide()
{
    LoadGuideTips();
    StartEvaluationTimer();
    
    UE_LOG(LogTemp, Log, TEXT("Adaptive Guide System Initialized with %d tips"), GuideTips.Num());
}

void UMingRTSAdaptiveGuide::UpdatePlayerMetrics(const FPlayerBehaviorMetrics& Metrics)
{
    CurrentMetrics = Metrics;
}

void UMingRTSAdaptiveGuide::TriggerContextualTip(FName TipID)
{
    FAdaptiveGuideTip* Tip = GuideTips.FindByPredicate([TipID](const FAdaptiveGuideTip& T) {
        return T.TipID == TipID;
    });
    
    if (Tip && ShouldShowTip(*Tip))
    {
        ShowTip(*Tip);
    }
}

void UMingRTSAdaptiveGuide::EvaluateAndShowTips()
{
    if (!bGuideEnabled)
    {
        return;
    }
    
    // 根據當前狀態評估哪些提示應該顯示
    for (const FAdaptiveGuideTip& Tip : GuideTips)
    {
        if (DisabledTips.Contains(Tip.TipID))
        {
            continue;
        }
        
        bool bShouldShow = false;
        
        // 根據觸發條件檢查
        if (Tip.TriggerCondition == TEXT("IdleWorkers"))
        {
            bShouldShow = CheckIdleWorkersCondition();
        }
        else if (Tip.TriggerCondition == TEXT("UnspentResources"))
        {
            bShouldShow = CheckUnspentResourcesCondition();
        }
        else if (Tip.TriggerCondition == TEXT("NoScouting"))
        {
            bShouldShow = CheckNoScoutingCondition();
        }
        else if (Tip.TriggerCondition == TEXT("UnderAttack"))
        {
            bShouldShow = CheckUnderAttackCondition();
        }
        else if (Tip.TriggerCondition == TEXT("IdleProduction"))
        {
            bShouldShow = CheckIdleProductionCondition();
        }
        else if (Tip.TriggerCondition == TEXT("LowAPM"))
        {
            bShouldShow = CheckLowAPMCondition();
        }
        else if (Tip.TriggerCondition == TEXT("ArmyDisparity"))
        {
            bShouldShow = CheckArmySizeDisparityCondition();
        }
        else if (Tip.TriggerCondition == TEXT("TechAvailable"))
        {
            bShouldShow = CheckTechAvailableCondition();
        }
        else if (Tip.TriggerCondition == TEXT("PopulationCap"))
        {
            bShouldShow = CheckPopulationCapCondition();
        }
        
        if (bShouldShow && ShouldShowTip(Tip))
        {
            ShowTip(Tip);
            // 一次只顯示一個提示
            break;
        }
    }
}

void UMingRTSAdaptiveGuide::DisableTip(FName TipID)
{
    if (!DisabledTips.Contains(TipID))
    {
        DisabledTips.Add(TipID);
    }
}

void UMingRTSAdaptiveGuide::EnableTip(FName TipID)
{
    DisabledTips.Remove(TipID);
}

void UMingRTSAdaptiveGuide::SetGuideEnabled(bool bEnabled)
{
    bGuideEnabled = bEnabled;
    
    if (bEnabled)
    {
        StartEvaluationTimer();
    }
    else
    {
        StopEvaluationTimer();
    }
}

TArray<FName> UMingRTSAdaptiveGuide::GetRecentTips(float WithinSeconds) const
{
    TArray<FName> RecentTips;
    float CurrentTime = GetWorld()->GetTimeSeconds();
    
    for (const auto& Pair : LastShownTime)
    {
        if (CurrentTime - Pair.Value <= WithinSeconds)
        {
            RecentTips.Add(Pair.Key);
        }
    }
    
    return RecentTips;
}

void UMingRTSAdaptiveGuide::ShowManualTip(const FText& Title, const FText& Message, EGuideTipType Type)
{
    FAdaptiveGuideTip ManualTip;
    ManualTip.TipID = FName(*FString::Printf(TEXT("Manual_%f"), GetWorld()->GetTimeSeconds()));
    ManualTip.TipType = Type;
    ManualTip.Title = Title;
    ManualTip.Message = Message;
    ManualTip.bCanBeDisabled = false;
    
    ShowTip(ManualTip);
}

void UMingRTSAdaptiveGuide::LoadGuideTips()
{
    GuideTips.Empty();
    
    // 閒置工人提示
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("IdleWorkers"));
        Tip.TipType = EGuideTipType::Warning;
        Tip.Title = FText::FromString(TEXT("閒置工人"));
        Tip.Message = FText::FromString(TEXT("您有 %d 個閒置工人。選擇他們並右鍵點擊資源點讓他們開始工作。"));
        Tip.TriggerCondition = TEXT("IdleWorkers");
        Tip.CooldownSeconds = 120.0f;
        Tip.Keywords.Add(TEXT("工人"));
        Tip.Keywords.Add(TEXT("資源"));
        GuideTips.Add(Tip);
    }
    
    // 資源過剩提示
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("UnspentResources"));
        Tip.TipType = EGuideTipType::Suggestion;
        Tip.Title = FText::FromString(TEXT("資源充裕"));
        Tip.Message = FText::FromString(TEXT("您的資源已超過 %d。可以考慮擴充軍隊、升級科技或建造更多生產設施。"));
        Tip.TriggerCondition = TEXT("UnspentResources");
        Tip.CooldownSeconds = 180.0f;
        Tip.Keywords.Add(TEXT("資源"));
        Tip.Keywords.Add(TEXT("經濟"));
        GuideTips.Add(Tip);
    }
    
    // 偵察提示
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("NoScouting"));
        Tip.TipType = EGuideTipType::Suggestion;
        Tip.Title = FText::FromString(TEXT("情報不足"));
        Tip.Message = FText::FromString(TEXT("您已經有一段時間沒有偵察敵方了。信息是戰爭的關鍵！"));
        Tip.TriggerCondition = TEXT("NoScouting");
        Tip.CooldownSeconds = 300.0f;
        Tip.Keywords.Add(TEXT("偵察"));
        Tip.Keywords.Add(TEXT("情報"));
        GuideTips.Add(Tip);
    }
    
    // 被攻擊提示
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("UnderAttack"));
        Tip.TipType = EGuideTipType::Alert;
        Tip.Title = FText::FromString(TEXT("遭受攻擊！"));
        Tip.Message = FText::FromString(TEXT("偵察到敵方正在進攻您的基地！建議召回部隊防守。"));
        Tip.TriggerCondition = TEXT("UnderAttack");
        Tip.CooldownSeconds = 60.0f;
        Tip.bCanBeDisabled = false;
        Tip.Keywords.Add(TEXT("戰鬥"));
        Tip.Keywords.Add(TEXT("防守"));
        GuideTips.Add(Tip);
    }
    
    // 閒置生產設施
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("IdleProduction"));
        Tip.TipType = EGuideTipType::Info;
        Tip.Title = FText::FromString(TEXT("生產設施閒置"));
        Tip.Message = FText::FromString(TEXT("您有生產設施處於閒置狀態。持續生產單位是取得優勢的關鍵！"));
        Tip.TriggerCondition = TEXT("IdleProduction");
        Tip.CooldownSeconds = 150.0f;
        Tip.Keywords.Add(TEXT("生產"));
        Tip.Keywords.Add(TEXT("部隊"));
        GuideTips.Add(Tip);
    }
    
    // 人口上限提示
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("PopulationCap"));
        Tip.TipType = EGuideTipType::Warning;
        Tip.Title = FText::FromString(TEXT("人口達上限"));
        Tip.Message = FText::FromString(TEXT("您的人口已達上限。建造更多房屋或升級人口上限以繼續生產單位。"));
        Tip.TriggerCondition = TEXT("PopulationCap");
        Tip.CooldownSeconds = 200.0f;
        Tip.Keywords.Add(TEXT("人口"));
        Tip.Keywords.Add(TEXT("建築"));
        GuideTips.Add(Tip);
    }
    
    // 科技可升級
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("TechAvailable"));
        Tip.TipType = EGuideTipType::Suggestion;
        Tip.Title = FText::FromString(TEXT("科技可升級"));
        Tip.Message = FText::FromString(TEXT("新的科技研究已完成或可以開始。點擊科技建築查看可用升級。"));
        Tip.TriggerCondition = TEXT("TechAvailable");
        Tip.CooldownSeconds = 240.0f;
        Tip.Keywords.Add(TEXT("科技"));
        Tip.Keywords.Add(TEXT("升級"));
        GuideTips.Add(Tip);
    }
    
    // 兵力劣勢
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("ArmyDisparity"));
        Tip.TipType = EGuideTipType::Warning;
        Tip.Title = FText::FromString(TEXT("兵力劣勢"));
        Tip.Message = FText::FromString(TEXT("偵察顯示敵方軍隊規模明顯大於您。建議採取防守姿態並加速生產。"));
        Tip.TriggerCondition = TEXT("ArmyDisparity");
        Tip.CooldownSeconds = 180.0f;
        Tip.Keywords.Add(TEXT("軍隊"));
        Tip.Keywords.Add(TEXT("戰鬥"));
        GuideTips.Add(Tip);
    }
    
    // 新手APM提示
    {
        FAdaptiveGuideTip Tip;
        Tip.TipID = FName(TEXT("LowAPM"));
        Tip.TipType = EGuideTipType::Tutorial;
        Tip.Title = FText::FromString(TEXT("提升操作速度"));
        Tip.Message = FText::FromString(TEXT("嘗試使用快捷鍵和編隊來提高操作效率。多線操作是RTS的精髓！"));
        Tip.TriggerCondition = TEXT("LowAPM");
        Tip.CooldownSeconds = 600.0f;
        Tip.RelatedTutorialStage = TEXT("AdvancedTactics");
        Tip.Keywords.Add(TEXT("操作"));
        Tip.Keywords.Add(TEXT("快捷鍵"));
        GuideTips.Add(Tip);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Loaded %d adaptive guide tips"), GuideTips.Num());
}

void UMingRTSAdaptiveGuide::StartEvaluationTimer()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(EvaluationTimerHandle, this, &UMingRTSAdaptiveGuide::EvaluateAndShowTips, EvaluationInterval, true);
    }
}

void UMingRTSAdaptiveGuide::StopEvaluationTimer()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(EvaluationTimerHandle);
    }
}

bool UMingRTSAdaptiveGuide::ShouldShowTip(const FAdaptiveGuideTip& Tip) const
{
    // 檢查冷卻時間
    const float* LastTime = LastShownTime.Find(Tip.TipID);
    if (LastTime)
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        if (CurrentTime - *LastTime < Tip.CooldownSeconds)
        {
            return false;
        }
    }
    
    // 檢查是否已禁用
    if (DisabledTips.Contains(Tip.TipID))
    {
        return false;
    }
    
    return true;
}

void UMingRTSAdaptiveGuide::ShowTip(const FAdaptiveGuideTip& Tip)
{
    // 記錄顯示時間
    LastShownTime.Add(Tip.TipID, GetWorld()->GetTimeSeconds());
    
    // 廣播事件
    OnGuideTipShown.Broadcast(Tip);
    OnGuideTipTriggered.Broadcast(Tip.TipID, Tip.TipType);
    
    UE_LOG(LogTemp, Log, TEXT("Showing guide tip: %s"), *Tip.Title.ToString());
}

// 條件檢查實現
bool UMingRTSAdaptiveGuide::CheckIdleWorkersCondition() const
{
    return CurrentMetrics.IdleWorkerCount >= 5;
}

bool UMingRTSAdaptiveGuide::CheckUnspentResourcesCondition() const
{
    return CurrentMetrics.UnspentResources >= 1000;
}

bool UMingRTSAdaptiveGuide::CheckNoScoutingCondition() const
{
    return CurrentMetrics.TimeSinceLastScout >= 180.0f; // 3分鐘
}

bool UMingRTSAdaptiveGuide::CheckUnderAttackCondition() const
{
    return CurrentMetrics.bIsUnderAttack;
}

bool UMingRTSAdaptiveGuide::CheckIdleProductionCondition() const
{
    return CurrentMetrics.bHasIdleProduction;
}

bool UMingRTSAdaptiveGuide::CheckLowAPMCondition() const
{
    // 對新手玩家的低APM提示
    return CurrentMetrics.APM > 0 && CurrentMetrics.APM < 30.0f;
}

bool UMingRTSAdaptiveGuide::CheckArmySizeDisparityCondition() const
{
    // 敵方軍隊明顯大於我方
    return CurrentMetrics.EnemyArmySize > CurrentMetrics.ArmySize * 1.5f;
}

bool UMingRTSAdaptiveGuide::CheckTechAvailableCondition() const
{
    // 這個條件需要額外的遊戲狀態檢查
    // 簡化實現
    return false;
}

bool UMingRTSAdaptiveGuide::CheckPopulationCapCondition() const
{
    return CurrentMetrics.UnusedPopulation <= 0;
}
