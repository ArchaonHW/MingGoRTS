#include "MingRTSDynamicHistorySystem.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSDynamicHistorySystem::UMingRTSDynamicHistorySystem()
{
    EventGenerationRate = 0.8f;
    PlayerInfluenceWeight = 1.2f;
    BranchProbability = 0.15f;
    MaxEventsPerYear = 5;
}

void UMingRTSDynamicHistorySystem::InitializeDynamicHistorySystem()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Initializing dynamic history system..."));

    // 初始化時代定義
    EraDefinitions.Empty();
    EraDefinitions.Add(1911, TEXT("辛亥革命"));
    EraDefinitions.Add(1912, TEXT("民國初建"));
    EraDefinitions.Add(1916, TEXT("軍閥割據"));
    EraDefinitions.Add(1927, TEXT("北伐統一"));
    EraDefinitions.Add(1937, TEXT("抗戰爆發"));
    EraDefinitions.Add(1945, TEXT("抗戰勝利"));
    EraDefinitions.Add(1949, TEXT("國共內戰"));

    // 初始化事件模板
    InitializeEventTemplates();

    UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Dynamic history system initialized successfully"));
}

FHistoricalTimeline UMingRTSDynamicHistorySystem::CreateTimeline(const FString& TimelineID, const FString& TimelineName, int32 StartYear)
{
    FHistoricalTimeline NewTimeline;
    NewTimeline.TimelineID = TimelineID;
    NewTimeline.TimelineName = TimelineName;
    NewTimeline.CurrentYear = StartYear;
    NewTimeline.CurrentEra = GetCurrentEra(StartYear);

    // 初始化世界狀態
    TMap<FString, float> InitialWorldState;
    InitialWorldState.Add(TEXT("PoliticalStability"), 0.5f);
    InitialWorldState.Add(TEXT("MilitaryStrength"), 0.6f);
    InitialWorldState.Add(TEXT("EconomicProsperity"), 0.4f);
    InitialWorldState.Add(TEXT("CulturalDevelopment"), 0.5f);
    InitialWorldState.Add(TEXT("SocialHarmony"), 0.3f);
    InitialWorldState.Add(TEXT("TechnologicalProgress"), 0.4f);
    InitialWorldState.Add(TEXT("NaturalResources"), 0.7f);
    InitialWorldState.Add(TEXT("PlayerInfluence"), 0.0f);

    NewTimeline.WorldState = InitialWorldState;
    NewTimeline.ActiveFactions = GetActiveFactions(TimelineID);

    Timelines.Add(TimelineID, NewTimeline);
    WorldStates.Add(TimelineID, InitialWorldState);

    // 初始化分支數組
    TArray<FHistoricalBranch> EmptyBranches;
    TimelineBranches.Add(TimelineID, EmptyBranches);

    UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Created timeline %s starting in year %d"), *TimelineName, StartYear);
    return NewTimeline;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GenerateHistoricalEvent(const FString& TimelineID, EHistoricalEventType EventType, const FString& Context)
{
    FHistoricalEvent NewEvent;

    switch (EventType)
    {
        case EHistoricalEventType::Political:
            NewEvent = GeneratePoliticalEvent(Context);
            break;
        case EHistoricalEventType::Military:
            NewEvent = GenerateMilitaryEvent(Context);
            break;
        case EHistoricalEventType::Economic:
            NewEvent = GenerateEconomicEvent(Context);
            break;
        case EHistoricalEventType::Cultural:
            NewEvent = GenerateCulturalEvent(Context);
            break;
        case EHistoricalEventType::Social:
            NewEvent = GenerateSocialEvent(Context);
            break;
        case EHistoricalEventType::Technological:
            NewEvent = GenerateTechnologicalEvent(Context);
            break;
        case EHistoricalEventType::Natural:
            NewEvent = GenerateNaturalEvent(Context);
            break;
        case EHistoricalEventType::Personal:
            NewEvent = GeneratePersonalEvent(Context);
            break;
    }

    // 設置基本屬性
    NewEvent.EventID = GenerateEventID();
    NewEvent.EventType = EventType;
    NewEvent.Significance = DetermineEventSignificance(NewEvent);
    NewEvent.EventImpact = DetermineEventImpact(NewEvent);

    // 獲取當前時間線信息
    if (Timelines.Contains(TimelineID))
    {
        const FHistoricalTimeline& Timeline = Timelines[TimelineID];
        NewEvent.EventDate = FDateTime(Timeline.CurrentYear, 1, 1);
        NewEvent.Location = DetermineEventLocation(NewEvent, Timeline);
        NewEvent.KeyFigures = DetermineKeyFigures(NewEvent, Timeline);
    }

    return NewEvent;
}

void UMingRTSDynamicHistorySystem::ProcessPlayerAction(const FString& PlayerID, const FString& Action, const FString& Context)
{
    // 根據玩家行為生成相應的歷史事件
    EHistoricalEventType EventType = DetermineEventTypeFromAction(Action);
    
    FHistoricalEvent PlayerEvent = GenerateHistoricalEvent(TEXT("MainTimeline"), EventType, Context);
    PlayerEvent.bPlayerInfluenced = true;
    PlayerEvent.PlayerAction = Action;

    // 增強玩家影響事件的顯著性
    PlayerEvent.Significance = (EHistoricalSignificance)FMath::Min((int32)PlayerEvent.Significance + 1, (int32)EHistoricalSignificance::Legendary);
    
    // 記錄事件
    RecordHistoricalEvent(TEXT("MainTimeline"), PlayerEvent);

    // 更新世界狀態
    UpdateWorldState(TEXT("MainTimeline"), PlayerEvent);

    // 觸發事件
    OnHistoricalEvent.Broadcast(PlayerEvent, true);

    UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Player action processed - %s"), *Action);
}

FHistoricalBranch UMingRTSDynamicHistorySystem::CreateHistoricalBranch(const FString& TimelineID, const FString& DivergenceEvent)
{
    FHistoricalBranch NewBranch;
    NewBranch.BranchID = FString::Printf(TEXT("Branch_%s_%d"), *TimelineID, FMath::RandRange(1000, 9999));
    NewBranch.BranchName = FString::Printf(TEXT("分支時間線 - %s"), *DivergenceEvent);
    NewBranch.DivergencePoint = DivergenceEvent;
    NewBranch.Probability = FMath::FRandRange(0.1f, 0.9f);

    // 複製當前世界狀態
    if (WorldStates.Contains(TimelineID))
    {
        NewBranch.BranchWorldState = WorldStates[TimelineID];
    }

    // 添加分支到時間線
    if (TimelineBranches.Contains(TimelineID))
    {
        TimelineBranches[TimelineID].Add(NewBranch);
    }

    OnTimelineBranch.Broadcast(NewBranch.BranchID);

    UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Created historical branch %s"), *NewBranch.BranchName);
    return NewBranch;
}

void UMingRTSDynamicHistorySystem::AdvanceTimeline(const FString& TimelineID, int32 Years)
{
    if (!Timelines.Contains(TimelineID))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSDynamicHistorySystem: Timeline %s not found"), *TimelineID);
        return;
    }

    FHistoricalTimeline& Timeline = Timelines[TimelineID];
    
    for (int32 Year = 1; Year <= Years; Year++)
    {
        Timeline.CurrentYear++;
        
        // 檢查時代變更
        CheckEraProgression(TimelineID);
        
        // 生成年度事件
        GenerateYearlyEvents(TimelineID);
        
        // 評估分支條件
        EvaluateBranchConditions(TimelineID);
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Timeline %s advanced by %d years to %d"), 
           *TimelineID, Years, Timeline.CurrentYear);
}

FHistoricalTimeline UMingRTSDynamicHistorySystem::GetCurrentTimeline(const FString& TimelineID) const
{
    return Timelines.FindRef(TimelineID);
}

TArray<FHistoricalEvent> UMingRTSDynamicHistorySystem::GetHistoricalEvents(const FString& TimelineID, EHistoricalEventType EventType) const
{
    TArray<FHistoricalEvent> FilteredEvents;
    
    if (Timelines.Contains(TimelineID))
    {
        const FHistoricalTimeline& Timeline = Timelines[TimelineID];
        
        for (const FHistoricalEvent& Event : Timeline.Events)
        {
            if (Event.EventType == EventType)
            {
                FilteredEvents.Add(Event);
            }
        }
    }
    
    return FilteredEvents;
}

TArray<FHistoricalEvent> UMingRTSDynamicHistorySystem::GetPotentialEvents(const FString& TimelineID, const FString& Context) const
{
    TArray<FHistoricalEvent> PotentialEvents;
    
    // 基於當前世界狀態生成潛在事件
    if (WorldStates.Contains(TimelineID))
    {
        const TMap<FString, float>& CurrentWorldState = WorldStates[TimelineID];
        
        // 生成各類型的潛在事件
        for (int32 i = 0; i < 8; i++)
        {
            EHistoricalEventType EventType = (EHistoricalEventType)i;
            FHistoricalEvent PotentialEvent = GenerateHistoricalEvent(TimelineID, EventType, Context);
            
            // 計算事件概率
            float Probability = CalculateEventProbability(PotentialEvent, TimelineID);
            
            if (Probability > 0.3f) // 只包含概率較高的事件
            {
                PotentialEvents.Add(PotentialEvent);
            }
        }
    }
    
    return PotentialEvents;
}

float UMingRTSDynamicHistorySystem::CalculateHistoricalImpact(const FHistoricalEvent& Event) const
{
    float Impact = 0.0f;
    
    // 基於事件顯著性計算影響
    switch (Event.Significance)
    {
        case EHistoricalSignificance::Trivial:
            Impact = 0.1f;
            break;
        case EHistoricalSignificance::Minor:
            Impact = 0.3f;
            break;
        case EHistoricalSignificance::Moderate:
            Impact = 0.5f;
            break;
        case EHistoricalSignificance::Major:
            Impact = 0.7f;
            break;
        case EHistoricalSignificance::Critical:
            Impact = 0.9f;
            break;
        case EHistoricalSignificance::Legendary:
            Impact = 1.0f;
            break;
    }
    
    // 基於事件影響範圍調整
    switch (Event.EventImpact)
    {
        case EHistoricalImpact::Local:
            Impact *= 0.5f;
            break;
        case EHistoricalImpact::Regional:
            Impact *= 0.7f;
            break;
        case EHistoricalImpact::National:
            Impact *= 1.0f;
            break;
        case EHistoricalImpact::International:
            Impact *= 1.3f;
            break;
        case EHistoricalImpact::Global:
            Impact *= 1.5f;
            break;
    }
    
    // 玩家影響加成
    if (Event.bPlayerInfluenced)
    {
        Impact *= PlayerInfluenceWeight;
    }
    
    return FMath::Clamp(Impact, 0.0f, 1.0f);
}

FString UMingRTSDynamicHistorySystem::GenerateNarrativeContent(const FHistoricalEvent& Event) const
{
    FString Narrative = GenerateEventNarrative(Event);
    
    if (Event.Consequences.Num() > 0)
    {
        Narrative += TEXT("\n\n") + GenerateConsequenceNarrative(Event.Consequences);
    }
    
    if (Event.KeyFigures.Num() > 0)
    {
        Narrative += TEXT("\n\n") + GenerateCharacterNarrative(Event.KeyFigures);
    }
    
    return Narrative;
}

TArray<FHistoricalEvent> UMingRTSDynamicHistorySystem::PredictFutureEvents(const FString& TimelineID, int32 YearsAhead) const
{
    TArray<FHistoricalEvent> PredictedEvents;
    
    if (!Timelines.Contains(TimelineID))
    {
        return PredictedEvents;
    }
    
    const FHistoricalTimeline& Timeline = Timelines[TimelineID];
    int32 FutureYear = Timeline.CurrentYear + YearsAhead;
    
    // 基於歷史趨勢預測未來事件
    for (int32 Year = Timeline.CurrentYear + 1; Year <= FutureYear; Year++)
    {
        // 預測每年度的主要事件
        EHistoricalEventType PredictedEventType = PredictEventTypeForYear(Year, Timeline);
        
        FHistoricalEvent PredictedEvent = GenerateHistoricalEvent(TimelineID, PredictedEventType, TEXT("FuturePrediction"));
        PredictedEvent.EventDate = FDateTime(Year, 1, 1);
        
        PredictedEvents.Add(PredictedEvent);
    }
    
    return PredictedEvents;
}

void UMingRTSDynamicHistorySystem::SetHistoricalParameters(float EventGenerationRate, float PlayerInfluenceWeight, float BranchProbability)
{
    this->EventGenerationRate = FMath::Clamp(EventGenerationRate, 0.0f, 1.0f);
    this->PlayerInfluenceWeight = FMath::Clamp(PlayerInfluenceWeight, 0.5f, 2.0f);
    this->BranchProbability = FMath::Clamp(BranchProbability, 0.0f, 1.0f);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Historical parameters updated"));
}

TMap<FString, float> UMingRTSDynamicHistorySystem::GetHistoricalStatistics(const FString& TimelineID) const
{
    TMap<FString, float> Statistics;
    
    if (!Timelines.Contains(TimelineID))
    {
        return Statistics;
    }
    
    const FHistoricalTimeline& Timeline = Timelines[TimelineID];
    
    // 統計事件類型分佈
    TMap<EHistoricalEventType, int32> EventTypeCounts;
    for (const FHistoricalEvent& Event : Timeline.Events)
    {
        EventTypeCounts.FindOrAdd(Event.EventType)++;
    }
    
    Statistics.Add(TEXT("TotalEvents"), Timeline.Events.Num());
    Statistics.Add(TEXT("PoliticalEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Political));
    Statistics.Add(TEXT("MilitaryEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Military));
    Statistics.Add(TEXT("EconomicEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Economic));
    Statistics.Add(TEXT("CulturalEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Cultural));
    Statistics.Add(TEXT("SocialEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Social));
    Statistics.Add(TEXT("TechnologicalEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Technological));
    Statistics.Add(TEXT("NaturalEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Natural));
    Statistics.Add(TEXT("PersonalEvents"), EventTypeCounts.FindRef(EHistoricalEventType::Personal));
    
    // 統計玩家影響
    int32 PlayerInfluencedEvents = 0;
    for (const FHistoricalEvent& Event : Timeline.Events)
    {
        if (Event.bPlayerInfluenced)
        {
            PlayerInfluencedEvents++;
        }
    }
    
    if (Timeline.Events.Num() > 0)
    {
        Statistics.Add(TEXT("PlayerInfluenceRatio"), (float)PlayerInfluencedEvents / Timeline.Events.Num());
    }
    
    // 統計分支數量
    if (TimelineBranches.Contains(TimelineID))
    {
        Statistics.Add(TEXT("BranchCount"), TimelineBranches[TimelineID].Num());
    }
    
    return Statistics;
}

void UMingRTSDynamicHistorySystem::ResetTimeline(const FString& TimelineID)
{
    if (Timelines.Contains(TimelineID))
    {
        FHistoricalTimeline& Timeline = Timelines[TimelineID];
        Timeline.Events.Empty();
        Timeline.CurrentYear = 1911;
        Timeline.CurrentEra = GetCurrentEra(Timeline.CurrentYear);
        
        // 重置世界狀態
        if (WorldStates.Contains(TimelineID))
        {
            TMap<FString, float>& WorldState = WorldStates[TimelineID];
            WorldState.Empty();
            WorldState.Add(TEXT("PoliticalStability"), 0.5f);
            WorldState.Add(TEXT("MilitaryStrength"), 0.6f);
            WorldState.Add(TEXT("EconomicProsperity"), 0.4f);
            WorldState.Add(TEXT("CulturalDevelopment"), 0.5f);
            WorldState.Add(TEXT("SocialHarmony"), 0.3f);
            WorldState.Add(TEXT("TechnologicalProgress"), 0.4f);
            WorldState.Add(TEXT("NaturalResources"), 0.7f);
            WorldState.Add(TEXT("PlayerInfluence"), 0.0f);
        }
        
        // 清空分支
        if (TimelineBranches.Contains(TimelineID))
        {
            TimelineBranches[TimelineID].Empty();
        }
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Timeline %s reset"), *TimelineID);
    }
}

// 私有方法實現

void UMingRTSDynamicHistorySystem::InitializeEventTemplates()
{
    EventTemplates.Empty();
    
    // 政治事件模板
    FHistoricalEvent PoliticalTemplate;
    PoliticalTemplate.EventType = EHistoricalEventType::Political;
    PoliticalTemplate.Description = TEXT("政治變革事件");
    EventTemplates.Add(PoliticalTemplate);
    
    // 軍事事件模板
    FHistoricalEvent MilitaryTemplate;
    MilitaryTemplate.EventType = EHistoricalEventType::Military;
    MilitaryTemplate.Description = TEXT("軍事衝突事件");
    EventTemplates.Add(MilitaryTemplate);
    
    // 經濟事件模板
    FHistoricalEvent EconomicTemplate;
    EconomicTemplate.EventType = EHistoricalEventType::Economic;
    EconomicTemplate.Description = TEXT("經濟變動事件");
    EventTemplates.Add(EconomicTemplate);
    
    // 文化事件模板
    FHistoricalEvent CulturalTemplate;
    CulturalTemplate.EventType = EHistoricalEventType::Cultural;
    CulturalTemplate.Description = TEXT("文化發展事件");
    EventTemplates.Add(CulturalTemplate);
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GeneratePoliticalEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("政治變革");
    Event.Description = TEXT("一場重要的政治變革正在發生");
    Event.EventType = EHistoricalEventType::Political;
    Event.EventImpact = EHistoricalImpact::National;
    Event.Significance = EHistoricalSignificance::Major;
    
    // 添加政治相關的後果
    Event.Consequences.Add(TEXT("政治格局重新洗牌"));
    Event.Consequences.Add(TEXT("權力平衡發生變化"));
    Event.Consequences.Add(TEXT("政策方向可能轉變"));
    
    return Event;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GenerateMilitaryEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("軍事衝突");
    Event.Description = TEXT("軍事衝突爆發，影響地區穩定");
    Event.EventType = EHistoricalEventType::Military;
    Event.EventImpact = EHistoricalImpact::Regional;
    Event.Significance = EHistoricalSignificance::Major;
    
    Event.Consequences.Add(TEXT("軍事力量對抗"));
    Event.Consequences.Add(TEXT("平民生活受影響"));
    Event.Consequences.Add(TEXT("地區安全局勢緊張"));
    
    return Event;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GenerateEconomicEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("經濟變動");
    Event.Description = TEXT("經濟狀況發生重大變化");
    Event.EventType = EHistoricalEventType::Economic;
    Event.EventImpact = EHistoricalImpact::National;
    Event.Significance = EHistoricalSignificance::Moderate;
    
    Event.Consequences.Add(TEXT("市場波動"));
    Event.Consequences.Add(TEXT("就業狀況變化"));
    Event.Consequences.Add(TEXT("貿易關係調整"));
    
    return Event;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GenerateCulturalEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("文化發展");
    Event.Description = TEXT("文化領域出現新的發展");
    Event.EventType = EHistoricalEventType::Cultural;
    Event.EventImpact = EHistoricalImpact::Regional;
    Event.Significance = EHistoricalSignificance::Minor;
    
    Event.Consequences.Add(TEXT("文化傳播"));
    Event.Consequences.Add(TEXT("思想交流"));
    Event.Consequences.Add(TEXT("藝術發展"));
    
    return Event;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GenerateSocialEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("社會變遷");
    Event.Description = TEXT("社會結構和關係發生變化");
    Event.EventType = EHistoricalEventType::Social;
    Event.EventImpact = EHistoricalImpact::Local;
    Event.Significance = EHistoricalSignificance::Moderate;
    
    Event.Consequences.Add(TEXT("社會秩序變化"));
    Event.Consequences.Add(TEXT("人際關係調整"));
    Event.Consequences.Add(TEXT("生活方式改變"));
    
    return Event;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GenerateTechnologicalEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("技術進步");
    Event.Description = TEXT("新技術的出現和應用");
    Event.EventType = EHistoricalEventType::Technological;
    Event.EventImpact = EHistoricalImpact::National;
    Event.Significance = EHistoricalSignificance::Minor;
    
    Event.Consequences.Add(TEXT("生產力提升"));
    Event.Consequences.Add(TEXT("生活方式改變"));
    Event.Consequences.Add(TEXT("經濟結構調整"));
    
    return Event;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GenerateNaturalEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("自然現象");
    Event.Description = TEXT("自然界的重大變化");
    Event.EventType = EHistoricalEventType::Natural;
    Event.EventImpact = EHistoricalImpact::Regional;
    Event.Significance = EHistoricalSignificance::Moderate;
    
    Event.Consequences.Add(TEXT("環境影響"));
    Event.Consequences.Add(TEXT("資源變化"));
    Event.Consequences.Add(TEXT("生活條件改變"));
    
    return Event;
}

FHistoricalEvent UMingRTSDynamicHistorySystem::GeneratePersonalEvent(const FString& Context)
{
    FHistoricalEvent Event;
    Event.EventName = TEXT("個人事件");
    Event.Description = TEXT("個人層面的重要事件");
    Event.EventType = EHistoricalEventType::Personal;
    Event.EventImpact = EHistoricalImpact::Local;
    Event.Significance = EHistoricalSignificance::Minor;
    
    Event.Consequences.Add(TEXT("個人命運改變"));
    Event.Consequences.Add(TEXT("家庭關係變化"));
    Event.Consequences.Add(TEXT("社會地位調整"));
    
    return Event;
}

void UMingRTSDynamicHistorySystem::UpdateWorldState(const FString& TimelineID, const FHistoricalEvent& Event)
{
    if (!WorldStates.Contains(TimelineID))
    {
        return;
    }
    
    TMap<FString, float>& WorldState = WorldStates[TimelineID];
    float Impact = CalculateHistoricalImpact(Event);
    
    // 根據事件類型更新世界狀態
    switch (Event.EventType)
    {
        case EHistoricalEventType::Political:
            WorldState.Add(TEXT("PoliticalStability"), FMath::Clamp(WorldState.FindRef(TEXT("PoliticalStability")) + Impact * 0.1f, 0.0f, 1.0f));
            break;
        case EHistoricalEventType::Military:
            WorldState.Add(TEXT("MilitaryStrength"), FMath::Clamp(WorldState.FindRef(TEXT("MilitaryStrength")) + Impact * 0.1f, 0.0f, 1.0f));
            break;
        case EHistoricalEventType::Economic:
            WorldState.Add(TEXT("EconomicProsperity"), FMath::Clamp(WorldState.FindRef(TEXT("EconomicProsperity")) + Impact * 0.1f, 0.0f, 1.0f));
            break;
        case EHistoricalEventType::Cultural:
            WorldState.Add(TEXT("CulturalDevelopment"), FMath::Clamp(WorldState.FindRef(TEXT("CulturalDevelopment")) + Impact * 0.1f, 0.0f, 1.0f));
            break;
        case EHistoricalEventType::Social:
            WorldState.Add(TEXT("SocialHarmony"), FMath::Clamp(WorldState.FindRef(TEXT("SocialHarmony")) + Impact * 0.1f, 0.0f, 1.0f));
            break;
        case EHistoricalEventType::Technological:
            WorldState.Add(TEXT("TechnologicalProgress"), FMath::Clamp(WorldState.FindRef(TEXT("TechnologicalProgress")) + Impact * 0.1f, 0.0f, 1.0f));
            break;
        case EHistoricalEventType::Natural:
            WorldState.Add(TEXT("NaturalResources"), FMath::Clamp(WorldState.FindRef(TEXT("NaturalResources")) + Impact * 0.1f, 0.0f, 1.0f));
            break;
    }
    
    // 玩家影響更新
    if (Event.bPlayerInfluenced)
    {
        WorldState.Add(TEXT("PlayerInfluence"), FMath::Clamp(WorldState.FindRef(TEXT("PlayerInfluence")) + Impact * 0.05f, 0.0f, 1.0f));
    }
}

void UMingRTSDynamicHistorySystem::CheckEraProgression(const FString& TimelineID)
{
    if (!Timelines.Contains(TimelineID))
    {
        return;
    }
    
    FHistoricalTimeline& Timeline = Timelines[TimelineID];
    FString NewEra = GetCurrentEra(Timeline.CurrentYear);
    
    if (NewEra != Timeline.CurrentEra)
    {
        FString OldEra = Timeline.CurrentEra;
        Timeline.CurrentEra = NewEra;
        
        OnEraChange.Broadcast(NewEra);
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSDynamicHistorySystem: Era changed from %s to %s"), *OldEra, *NewEra);
    }
}

void UMingRTSDynamicHistorySystem::EvaluateBranchConditions(const FString& TimelineID)
{
    if (!Timelines.Contains(TimelineID) || !WorldStates.Contains(TimelineID))
    {
        return;
    }
    
    const FHistoricalTimeline& Timeline = Timelines[TimelineID];
    const TMap<FString, float>& WorldState = WorldStates[TimelineID];
    
    // 檢查是否滿足分支條件
    bool ShouldBranch = false;
    FString BranchReason;
    
    // 基於世界狀態決定是否分支
    if (WorldState.FindRef(TEXT("PlayerInfluence")) > 0.7f)
    {
        ShouldBranch = true;
        BranchReason = TEXT("玩家影響力過高");
    }
    else if (WorldState.FindRef(TEXT("PoliticalStability")) < 0.2f)
    {
        ShouldBranch = true;
        BranchReason = TEXT("政治不穩定");
    }
    else if (Timeline.Events.Num() > 50)
    {
        ShouldBranch = true;
        BranchReason = TEXT("歷史事件過多");
    }
    
    if (ShouldBranch && FMath::FRand() < BranchProbability)
    {
        CreateHistoricalBranch(TimelineID, BranchReason);
    }
}

FString UMingRTSDynamicHistorySystem::GenerateEventID() const
{
    return FString::Printf(TEXT("Event_%d_%d"), FDateTime::Now().GetTicks(), FMath::RandRange(1000, 9999));
}

float UMingRTSDynamicHistorySystem::CalculateEventProbability(const FHistoricalEvent& Event, const FString& TimelineID) const
{
    float BaseProbability = EventGenerationRate;
    
    // 根據世界狀態調整概率
    if (WorldStates.Contains(TimelineID))
    {
        const TMap<FString, float>& WorldState = WorldStates[TimelineID];
        
        switch (Event.EventType)
        {
            case EHistoricalEventType::Political:
                BaseProbability *= (1.0f - WorldState.FindRef(TEXT("PoliticalStability")));
                break;
            case EHistoricalEventType::Military:
                BaseProbability *= (1.0f - WorldState.FindRef(TEXT("MilitaryStrength")));
                break;
            case EHistoricalEventType::Economic:
                BaseProbability *= (1.0f - WorldState.FindRef(TEXT("EconomicProsperity")));
                break;
            case EHistoricalEventType::Cultural:
                BaseProbability *= WorldState.FindRef(TEXT("CulturalDevelopment"));
                break;
            case EHistoricalEventType::Social:
                BaseProbability *= (1.0f - WorldState.FindRef(TEXT("SocialHarmony")));
                break;
            case EHistoricalEventType::Technological:
                BaseProbability *= WorldState.FindRef(TEXT("TechnologicalProgress"));
                break;
            case EHistoricalEventType::Natural:
                BaseProbability *= (1.0f - WorldState.FindRef(TEXT("NaturalResources")));
                break;
        }
    }
    
    return FMath::Clamp(BaseProbability, 0.0f, 1.0f);
}

FString UMingRTSDynamicHistorySystem::GetCurrentEra(int32 Year) const
{
    for (const auto& EraPair : EraDefinitions)
    {
        if (Year >= EraPair.Key)
        {
            return EraPair.Value;
        }
    }
    
    return TEXT("未知時代");
}

TArray<FString> UMingRTSDynamicHistorySystem::GetActiveFactions(const FString& TimelineID) const
{
    TArray<FString> Factions;
    Factions.Add(TEXT("北洋政府"));
    Factions.Add(TEXT("國民政府"));
    Factions.Add(TEXT("中國共產黨"));
    Factions.Add(TEXT("各地軍閥"));
    Factions.Add(TEXT("民間勢力"));
    
    return Factions;
}

void UMingRTSDynamicHistorySystem::RecordHistoricalEvent(const FString& TimelineID, const FHistoricalEvent& Event)
{
    if (Timelines.Contains(TimelineID))
    {
        FHistoricalTimeline& Timeline = Timelines[TimelineID];
        Timeline.Events.Add(Event);
        
        // 限制事件數量
        if (Timeline.Events.Num() > 1000)
        {
            Timeline.Events.RemoveAt(0);
        }
    }
}

FString UMingRTSDynamicHistorySystem::GenerateEventNarrative(const FHistoricalEvent& Event) const
{
    FString Narrative = FString::Printf(TEXT("在%s，%s發生了重大事件：%s。"), 
        *Event.EventDate.ToString(), *Event.Location, *Event.EventName);
    
    Narrative += TEXT("\n\n") + Event.Description;
    
    return Narrative;
}

FString UMingRTSDynamicHistorySystem::GenerateConsequenceNarrative(const TArray<FString>& Consequences) const
{
    FString Narrative = TEXT("此次事件的後果包括：");
    
    for (const FString& Consequence : Consequences)
    {
        Narrative += TEXT("\n• ") + Consequence;
    }
    
    return Narrative;
}

FString UMingRTSDynamicHistorySystem::GenerateCharacterNarrative(const TArray<FString>& KeyFigures) const
{
    FString Narrative = TEXT("關鍵人物涉及：");
    
    for (const FString& Figure : KeyFigures)
    {
        Narrative += TEXT("\n• ") + Figure;
    }
    
    return Narrative;
}

EHistoricalSignificance UMingRTSDynamicHistorySystem::DetermineEventSignificance(const FHistoricalEvent& Event) const
{
    // 基於事件類型和影響範圍確定顯著性
    if (Event.EventImpact == EHistoricalImpact::Global)
    {
        return EHistoricalSignificance::Legendary;
    }
    else if (Event.EventImpact == EHistoricalImpact::International)
    {
        return EHistoricalSignificance::Critical;
    }
    else if (Event.EventImpact == EHistoricalImpact::National)
    {
        return EHistoricalSignificance::Major;
    }
    else if (Event.EventImpact == EHistoricalImpact::Regional)
    {
        return EHistoricalSignificance::Moderate;
    }
    else
    {
        return EHistoricalSignificance::Minor;
    }
}

EHistoricalImpact UMingRTSDynamicHistorySystem::DetermineEventImpact(const FHistoricalEvent& Event) const
{
    // 基於事件類型確定影響範圍
    switch (Event.EventType)
    {
        case EHistoricalEventType::Political:
            return EHistoricalImpact::National;
        case EHistoricalEventType::Military:
            return EHistoricalImpact::Regional;
        case EHistoricalEventType::Economic:
            return EHistoricalImpact::National;
        case EHistoricalEventType::Cultural:
            return EHistoricalImpact::Regional;
        case EHistoricalEventType::Social:
            return EHistoricalImpact::Local;
        case EHistoricalEventType::Technological:
            return EHistoricalImpact::National;
        case EHistoricalEventType::Natural:
            return EHistoricalImpact::Regional;
        case EHistoricalEventType::Personal:
            return EHistoricalImpact::Local;
        default:
            return EHistoricalImpact::Local;
    }
}

FString UMingRTSDynamicHistorySystem::DetermineEventLocation(const FHistoricalEvent& Event, const FHistoricalTimeline& Timeline) const
{
    // 基於事件類型和活躍勢力確定地點
    TArray<FString> PossibleLocations;
    
    switch (Event.EventType)
    {
        case EHistoricalEventType::Political:
            PossibleLocations.Add(TEXT("北京"));
            PossibleLocations.Add(TEXT("南京"));
            PossibleLocations.Add(TEXT("廣州"));
            break;
        case EHistoricalEventType::Military:
            PossibleLocations.Add(TEXT("戰場"));
            PossibleLocations.Add(TEXT("邊境"));
            PossibleLocations.Add(TEXT("要塞"));
            break;
        case EHistoricalEventType::Economic:
            PossibleLocations.Add(TEXT("上海"));
            PossibleLocations.Add(TEXT("天津"));
            PossibleLocations.Add(TEXT("武漢"));
            break;
        case EHistoricalEventType::Cultural:
            PossibleLocations.Add(TEXT("北平"));
            PossibleLocations.Add(TEXT("南京"));
            PossibleLocations.Add(TEXT("杭州"));
            break;
        default:
            PossibleLocations.Add(TEXT("未知地點"));
            break;
    }
    
    if (PossibleLocations.Num() > 0)
    {
        return PossibleLocations[FMath::RandRange(0, PossibleLocations.Num() - 1)];
    }
    
    return TEXT("未知地點");
}

TArray<FString> UMingRTSDynamicHistorySystem::DetermineKeyFigures(const FHistoricalEvent& Event, const FHistoricalTimeline& Timeline) const
{
    TArray<FString> KeyFigures;
    
    // 基於事件類型和活躍勢力確定關鍵人物
    switch (Event.EventType)
    {
        case EHistoricalEventType::Political:
            KeyFigures.Add(TEXT("政治領袖"));
            KeyFigures.Add(TEXT("政府官員"));
            break;
        case EHistoricalEventType::Military:
            KeyFigures.Add(TEXT("軍事指揮官"));
            KeyFigures.Add(TEXT("士兵"));
            break;
        case EHistoricalEventType::Economic:
            KeyFigures.Add(TEXT("商人"));
            KeyFigures.Add(TEXT("銀行家"));
            break;
        case EHistoricalEventType::Cultural:
            KeyFigures.Add(TEXT("學者"));
            KeyFigures.Add(TEXT("藝術家"));
            break;
        default:
            KeyFigures.Add(TEXT("相關人士"));
            break;
    }
    
    return KeyFigures;
}

EHistoricalEventType UMingRTSDynamicHistorySystem::DetermineEventTypeFromAction(const FString& Action) const
{
    // 基於玩家行為確定事件類型
    if (Action.Contains(TEXT("戰鬥")) || Action.Contains(TEXT("攻擊")))
    {
        return EHistoricalEventType::Military;
    }
    else if (Action.Contains(TEXT("交易")) || Action.Contains(TEXT("經濟")))
    {
        return EHistoricalEventType::Economic;
    }
    else if (Action.Contains(TEXT("政治")) || Action.Contains(TEXT("政策")))
    {
        return EHistoricalEventType::Political;
    }
    else if (Action.Contains(TEXT("文化")) || Action.Contains(TEXT("教育")))
    {
        return EHistoricalEventType::Cultural;
    }
    else if (Action.Contains(TEXT("社交")) || Action.Contains(TEXT("關係")))
    {
        return EHistoricalEventType::Social;
    }
    else if (Action.Contains(TEXT("技術")) || Action.Contains(TEXT("研發")))
    {
        return EHistoricalEventType::Technological;
    }
    else
    {
        return EHistoricalEventType::Personal;
    }
}

void UMingRTSDynamicHistorySystem::GenerateYearlyEvents(const FString& TimelineID)
{
    if (!Timelines.Contains(TimelineID))
    {
        return;
    }
    
    const FHistoricalTimeline& Timeline = Timelines[TimelineID];
    int32 EventsGenerated = 0;
    
    // 生成年度事件
    while (EventsGenerated < MaxEventsPerYear && FMath::FRand() < EventGenerationRate)
    {
        EHistoricalEventType EventType = (EHistoricalEventType)FMath::RandRange(0, 7);
        FString Context = FString::Printf(TEXT("Year_%d"), Timeline.CurrentYear);
        
        FHistoricalEvent NewEvent = GenerateHistoricalEvent(TimelineID, EventType, Context);
        
        // 計算事件概率
        float Probability = CalculateEventProbability(NewEvent, TimelineID);
        
        if (FMath::FRand() < Probability)
        {
            RecordHistoricalEvent(TimelineID, NewEvent);
            UpdateWorldState(TimelineID, NewEvent);
            OnHistoricalEvent.Broadcast(NewEvent, false);
            
            EventsGenerated++;
        }
    }
}

EHistoricalEventType UMingRTSDynamicHistorySystem::PredictEventTypeForYear(int32 Year, const FHistoricalTimeline& Timeline) const
{
    // 基於歷史趨勢預測事件類型
    FString Era = GetCurrentEra(Year);
    
    if (Era.Contains(TEXT("革命")))
    {
        return EHistoricalEventType::Political;
    }
    else if (Era.Contains(TEXT("軍閥")))
    {
        return EHistoricalEventType::Military;
    }
    else if (Era.Contains(TEXT("抗戰")))
    {
        return EHistoricalEventType::Military;
    }
    else if (Era.Contains(TEXT("內戰")))
    {
        return EHistoricalEventType::Military;
    }
    else
    {
        return EHistoricalEventType::Political;
    }
}
