#include "Events/MingEventBlueprintLibrary.h"
#include "Engine/Engine.h"

UMingHistoricalEventManager* UMingEventBlueprintLibrary::GetHistoricalEventManager()
{
    // 這裡應該從遊戲實例或單例獲取歷史事件管理器
    // 簡化實作
    static UMingHistoricalEventManager* ManagerInstance = nullptr;
    if (!ManagerInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            ManagerInstance = NewObject<UMingHistoricalEventManager>(World);
            ManagerInstance->Initialize();
        }
    }
    
    return ManagerInstance;
}

void UMingEventBlueprintLibrary::InitializeHistoricalEventSystem()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (Manager)
    {
        Manager->Initialize();
        UE_LOG(LogTemp, Log, TEXT("Historical event system initialized from Blueprint"));
    }
}

bool UMingEventBlueprintLibrary::RegisterHistoricalEvent(const FMingHistoricalEvent& Event)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        UE_LOG(LogTemp, Error, TEXT("Historical event manager not available"));
        return false;
    }
    
    return Manager->RegisterHistoricalEvent(Event);
}

bool UMingEventBlueprintLibrary::UnregisterHistoricalEvent(const FString& EventID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->UnregisterHistoricalEvent(EventID);
}

bool UMingEventBlueprintLibrary::CheckEventTriggerConditions(const FString& EventID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return false;
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    return Manager->CheckEventTriggerConditions(EventID, Context);
}

bool UMingEventBlueprintLibrary::TriggerHistoricalEvent(const FString& EventID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        UE_LOG(LogTemp, Error, TEXT("Historical event manager not available"));
        return false;
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    FEventExecutionResult Result = Manager->TriggerHistoricalEvent(EventID, Context);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Historical event triggered successfully: %s"), *EventID);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to trigger historical event: %s - %s"), 
            *EventID, *Result.ErrorMessage);
        return false;
    }
}

FMingHistoricalEvent UMingEventBlueprintLibrary::GetHistoricalEvent(const FString& EventID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return FMingHistoricalEvent();
    }
    
    return Manager->GetHistoricalEvent(EventID);
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetAllHistoricalEvents()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalEvent>();
    }
    
    return Manager->GetAllHistoricalEvents();
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetEventsByEra(ERepublicEra Era)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalEvent>();
    }
    
    return Manager->GetEventsByEra(Era);
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetEventsByType(EHistoricalEventType EventType)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalEvent>();
    }
    
    return Manager->GetEventsByType(EventType);
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetCriticalEvents()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalEvent>();
    }
    
    return Manager->GetCriticalEvents();
}

TArray<FString> UMingEventBlueprintLibrary::GetTriggerableEvents()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FString>();
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    return Manager->GetTriggerableEvents(Context);
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetEventChain(const FString& EventID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalEvent>();
    }
    
    return Manager->GetEventChain(EventID);
}

void UMingEventBlueprintLibrary::SetGameTime(float GameTime)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (Manager)
    {
        Manager->SetGameTime(GameTime);
    }
}

float UMingEventBlueprintLibrary::GetGameTime()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return 0.0f;
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    return Context.CurrentGameTime;
}

void UMingEventBlueprintLibrary::SetRepublicEra(ERepublicEra Era)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (Manager)
    {
        Manager->SetRepublicEra(Era);
    }
}

ERepublicEra UMingEventBlueprintLibrary::GetCurrentRepublicEra()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return ERepublicEra::EarlyRepublic;
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    return Context.CurrentEra;
}

int32 UMingEventBlueprintLibrary::GetCurrentYear()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return 1912;
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    return Context.CurrentYear;
}

void UMingEventBlueprintLibrary::AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (Manager)
    {
        Manager->AddPlayerDecision(DecisionID, DecisionValue);
    }
}

FString UMingEventBlueprintLibrary::GetPlayerDecision(const FString& DecisionID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TEXT("");
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    FString* DecisionValue = Context.PlayerDecisions.Find(DecisionID);
    return DecisionValue ? *DecisionValue : TEXT("");
}

TMap<FString, int32> UMingEventBlueprintLibrary::GetEventStatistics()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TMap<FString, int32>();
    }
    
    return Manager->GetEventStatistics();
}

void UMingEventBlueprintLibrary::ResetAllEvents()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (Manager)
    {
        Manager->ResetAllEvents();
        UE_LOG(LogTemp, Log, TEXT("All historical events reset from Blueprint"));
    }
}

bool UMingEventBlueprintLibrary::ForceTriggerEvent(const FString& EventID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return false;
    }
    
    FEventExecutionResult Result = Manager->ForceTriggerEvent(EventID);
    return Result.bSuccess;
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetRecommendedEvents()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalEvent>();
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    return Manager->GetRecommendedEvents(Context);
}

FString UMingEventBlueprintLibrary::GetEventDescription(const FString& EventID)
{
    FMingHistoricalEvent Event = GetHistoricalEvent(EventID);
    return Event.EventDescription;
}

FString UMingEventBlueprintLibrary::GetEraName(ERepublicEra Era)
{
    switch (Era)
    {
    case ERepublicEra::EarlyRepublic:
        return TEXT("早期共和國 (1912-1928)");
    case ERepublicEra::NanjingDecade:
        return TEXT("南京十年 (1928-1937)");
    case ERepublicEra::WarOfResistance:
        return TEXT("抗戰時期 (1937-1945)");
    case ERepublicEra::CivilWar:
        return TEXT("內戰時期 (1945-1949)");
    default:
        return TEXT("未知時期");
    }
}

FString UMingEventBlueprintLibrary::GetEventTypeName(EHistoricalEventType EventType)
{
    switch (EventType)
    {
    case EHistoricalEventType::Political:
        return TEXT("政治");
    case EHistoricalEventType::Military:
        return TEXT("軍事");
    case EHistoricalEventType::Economic:
        return TEXT("經濟");
    case EHistoricalEventType::Social:
        return TEXT("社會");
    case EHistoricalEventType::Cultural:
        return TEXT("文化");
    case EHistoricalEventType::Diplomatic:
        return TEXT("外交");
    case EHistoricalEventType::Revolutionary:
        return TEXT("革命");
    default:
        return TEXT("未知類型");
    }
}

FString UMingEventBlueprintLibrary::GetImpactScopeName(EEventImpactScope ImpactScope)
{
    switch (ImpactScope)
    {
    case EEventImpactScope::Local:
        return TEXT("本地");
    case EEventImpactScope::Regional:
        return TEXT("區域");
    case EEventImpactScope::National:
        return TEXT("全國");
    case EEventImpactScope::International:
        return TEXT("國際");
    default:
        return TEXT("未知範圍");
    }
}

FMingHistoricalEvent UMingEventBlueprintLibrary::CreateHistoricalEvent(
    const FString& EventID,
    const FString& EventName,
    const FString& EventDescription,
    ERepublicEra Era,
    int32 HistoricalYear,
    EHistoricalEventType EventType,
    EEventImpactScope ImpactScope,
    bool bIsCriticalEvent)
{
    FMingHistoricalEvent Event;
    Event.EventID = EventID;
    Event.EventName = EventName;
    Event.EventDescription = EventDescription;
    Event.Era = Era;
    Event.HistoricalYear = HistoricalYear;
    Event.EventType = EventType;
    Event.ImpactScope = ImpactScope;
    Event.bIsCriticalEvent = bIsCriticalEvent;
    
    return Event;
}

FMingHistoricalEvent UMingEventBlueprintLibrary::AddTriggerCondition(
    const FMingHistoricalEvent& Event,
    const FString& Condition)
{
    FMingHistoricalEvent ModifiedEvent = Event;
    ModifiedEvent.TriggerConditions.Add(Condition);
    return ModifiedEvent;
}

FMingHistoricalEvent UMingEventBlueprintLibrary::AddEventConsequence(
    const FMingHistoricalEvent& Event,
    const FString& Consequence)
{
    FMingHistoricalEvent ModifiedEvent = Event;
    ModifiedEvent.EventConsequences.Add(Consequence);
    return ModifiedEvent;
}

FMingHistoricalEvent UMingEventBlueprintLibrary::AddRelatedFigure(
    const FMingHistoricalEvent& Event,
    const FString& FigureName)
{
    FMingHistoricalEvent ModifiedEvent = Event;
    ModifiedEvent.RelatedFigures.Add(FigureName);
    return ModifiedEvent;
}

FMingHistoricalEvent UMingEventBlueprintLibrary::AddRelatedLocation(
    const FMingHistoricalEvent& Event,
    const FString& LocationName)
{
    FMingHistoricalEvent ModifiedEvent = Event;
    ModifiedEvent.RelatedLocations.Add(LocationName);
    return ModifiedEvent;
}

FMingHistoricalEvent UMingEventBlueprintLibrary::SetPrerequisiteEvent(
    const FMingHistoricalEvent& Event,
    const FString& PrerequisiteEventID)
{
    FMingHistoricalEvent ModifiedEvent = Event;
    ModifiedEvent.PrerequisiteEvents.Add(PrerequisiteEventID);
    return ModifiedEvent;
}

FMingHistoricalEvent UMingEventBlueprintLibrary::SetFollowUpEvent(
    const FMingHistoricalEvent& Event,
    const FString& FollowUpEventID)
{
    FMingHistoricalEvent ModifiedEvent = Event;
    ModifiedEvent.FollowUpEvents.Add(FollowUpEventID);
    return ModifiedEvent;
}

TArray<bool> UMingEventBlueprintLibrary::BatchTriggerEvents(const TArray<FString>& EventIDs)
{
    TArray<bool> Results;
    
    for (const FString& EventID : EventIDs)
    {
        Results.Add(TriggerHistoricalEvent(EventID));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch triggered %d events"), Results.Num());
    return Results;
}

TArray<bool> UMingEventBlueprintLibrary::BatchCheckEventConditions(const TArray<FString>& EventIDs)
{
    TArray<bool> Results;
    
    for (const FString& EventID : EventIDs)
    {
        Results.Add(CheckEventTriggerConditions(EventID));
    }
    
    return Results;
}

bool UMingEventBlueprintLibrary::IsHistoricalEventSystemInitialized()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    return Manager != nullptr;
}

FString UMingEventBlueprintLibrary::GetHistoricalEventSystemVersion()
{
    return TEXT("1.0.0");
}

int32 UMingEventBlueprintLibrary::GetTriggeredEventCount()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return 0;
    }
    
    TMap<FString, int32> Stats = Manager->GetEventStatistics();
    int32 TriggeredCount = 0;
    
    for (const auto& Stat : Stats)
    {
        if (Stat.Value > 0)
        {
            TriggeredCount++;
        }
    }
    
    return TriggeredCount;
}

int32 UMingEventBlueprintLibrary::GetRegisteredEventCount()
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return 0;
    }
    
    return Manager->GetAllHistoricalEvents().Num();
}

float UMingEventBlueprintLibrary::GetEventTriggerRate()
{
    int32 TotalEvents = GetRegisteredEventCount();
    int32 TriggeredEvents = GetTriggeredEventCount();
    
    if (TotalEvents == 0)
    {
        return 0.0f;
    }
    
    return (float)TriggeredEvents / TotalEvents;
}

float UMingEventBlueprintLibrary::GetHistoricalAccuracyScore()
{
    // 簡化實作：基於觸發的關鍵事件比例
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return 0.0f;
    }
    
    TArray<FMingHistoricalEvent> CriticalEvents = Manager->GetCriticalEvents();
    int32 TriggeredCriticalEvents = 0;
    
    for (const FMingHistoricalEvent& Event : CriticalEvents)
    {
        TMap<FString, int32> Stats = Manager->GetEventStatistics();
        if (Stats.Contains(Event.EventID) && Stats[Event.EventID] > 0)
        {
            TriggeredCriticalEvents++;
        }
    }
    
    if (CriticalEvents.Num() == 0)
    {
        return 0.0f;
    }
    
    return (float)TriggeredCriticalEvents / CriticalEvents.Num();
}

TMap<FString, float> UMingEventBlueprintLibrary::GetEventImpactAnalysis()
{
    TMap<FString, float> ImpactAnalysis;
    
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return ImpactAnalysis;
    }
    
    TArray<FMingHistoricalEvent> AllEvents = Manager->GetAllHistoricalEvents();
    
    for (const FMingHistoricalEvent& Event : AllEvents)
    {
        float Impact = 0.0f;
        
        // 基於優先級
        Impact += Event.Priority * 0.01f;
        
        // 基於影響範圍
        switch (Event.ImpactScope)
        {
        case EEventImpactScope::Local:
            Impact += 0.2f;
            break;
        case EEventImpactScope::Regional:
            Impact += 0.4f;
            break;
        case EEventImpactScope::National:
            Impact += 0.6f;
            break;
        case EEventImpactScope::International:
            Impact += 0.8f;
            break;
        }
        
        // 關鍵事件額外加分
        if (Event.bIsCriticalEvent)
        {
            Impact += 0.3f;
        }
        
        ImpactAnalysis.Add(Event.EventID, FMath::Clamp(Impact, 0.0f, 1.0f));
    }
    
    return ImpactAnalysis;
}

TMap<FString, float> UMingEventBlueprintLibrary::GetPlayerDecisionImpact()
{
    TMap<FString, float> DecisionImpact;
    
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return DecisionImpact;
    }
    
    FEventTriggerContext Context = Manager->GetCurrentGameContext();
    
    // 簡化實作：基於決策數量計算影響
    for (const auto& Decision : Context.PlayerDecisions)
    {
        float Impact = 0.5f; // 基礎影響值
        DecisionImpact.Add(Decision.Key, Impact);
    }
    
    return DecisionImpact;
}

float UMingEventBlueprintLibrary::GetEraProgress()
{
    ERepublicEra CurrentEra = GetCurrentRepublicEra();
    int32 CurrentYear = GetCurrentYear();
    
    switch (CurrentEra)
    {
    case ERepublicEra::EarlyRepublic:
        return (float)(CurrentYear - 1912) / (1928 - 1912);
    case ERepublicEra::NanjingDecade:
        return (float)(CurrentYear - 1928) / (1937 - 1928);
    case ERepublicEra::WarOfResistance:
        return (float)(CurrentYear - 1937) / (1945 - 1937);
    case ERepublicEra::CivilWar:
        return (float)(CurrentYear - 1945) / (1949 - 1945);
    default:
        return 0.0f;
    }
}

float UMingEventBlueprintLibrary::GetHistoricalProgress()
{
    int32 CurrentYear = GetCurrentYear();
    
    // 1912-1949 總共37年
    return (float)(CurrentYear - 1912) / 37.0f;
}

FMingHistoricalEvent UMingEventBlueprintLibrary::GetNextMajorEvent()
{
    TArray<FMingHistoricalEvent> RecommendedEvents = GetRecommendedEvents();
    
    if (RecommendedEvents.Num() > 0)
    {
        return RecommendedEvents[0];
    }
    
    return FMingHistoricalEvent();
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetPossibleEventBranches(const FString& EventID)
{
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalEvent>();
    }
    
    FMingHistoricalEvent Event = Manager->GetHistoricalEvent(EventID);
    TArray<FMingHistoricalEvent> Branches;
    
    // 獲取後續事件
    for (const FString& FollowUpEventID : Event.FollowUpEvents)
    {
        FMingHistoricalEvent FollowUpEvent = Manager->GetHistoricalEvent(FollowUpEventID);
        if (!FollowUpEvent.EventID.IsEmpty())
        {
            Branches.Add(FollowUpEvent);
        }
    }
    
    return Branches;
}

TArray<FString> UMingEventBlueprintLibrary::GetEventRecommendations()
{
    TArray<FString> Recommendations;
    TArray<FMingHistoricalEvent> RecommendedEvents = GetRecommendedEvents();
    
    for (const FMingHistoricalEvent& Event : RecommendedEvents)
    {
        Recommendations.Add(Event.EventID);
    }
    
    return Recommendations;
}

TArray<FString> UMingEventBlueprintLibrary::SimulateEventTriggers(float SimulationTime)
{
    TArray<FString> SimulatedEvents;
    
    // 保存當前時間
    float OriginalTime = GetGameTime();
    
    // 模擬時間推進
    SetGameTime(OriginalTime + SimulationTime);
    
    // 檢查可觸發事件
    TArray<FString> TriggerableEvents = GetTriggerableEvents();
    
    // 模擬觸發
    for (const FString& EventID : TriggerableEvents)
    {
        if (CheckEventTriggerConditions(EventID))
        {
            SimulatedEvents.Add(EventID);
        }
    }
    
    // 恢復原始時間
    SetGameTime(OriginalTime);
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d event triggers in %.1f time units"), 
        SimulatedEvents.Num(), SimulationTime);
    
    return SimulatedEvents;
}

TArray<FMingHistoricalEvent> UMingEventBlueprintLibrary::GetEventTimeline(ERepublicEra Era)
{
    return GetEventsByEra(Era);
}

TMap<FString, TArray<FString>> UMingEventBlueprintLibrary::GetEventRelationships()
{
    TMap<FString, TArray<FString>> Relationships;
    
    UMingHistoricalEventManager* Manager = GetHistoricalEventManager();
    if (!Manager)
    {
        return Relationships;
    }
    
    TArray<FMingHistoricalEvent> AllEvents = Manager->GetAllHistoricalEvents();
    
    for (const FMingHistoricalEvent& Event : AllEvents)
    {
        TArray<FString> RelatedEvents;
        
        // 添加前置事件
        for (const FString& PrerequisiteEvent : Event.PrerequisiteEvents)
        {
            RelatedEvents.Add(PrerequisiteEvent);
        }
        
        // 添加後續事件
        for (const FString& FollowUpEvent : Event.FollowUpEvents)
        {
            RelatedEvents.Add(FollowUpEvent);
        }
        
        Relationships.Add(Event.EventID, RelatedEvents);
    }
    
    return Relationships;
}

bool UMingEventBlueprintLibrary::ExportEventData(const FString& FilePath)
{
    // 簡化實作：記錄導出操作
    UE_LOG(LogTemp, Log, TEXT("Event data would be exported to: %s"), *FilePath);
    return true;
}

bool UMingEventBlueprintLibrary::ImportEventData(const FString& FilePath)
{
    // 簡化實作：記錄導入操作
    UE_LOG(LogTemp, Log, TEXT("Event data would be imported from: %s"), *FilePath);
    return true;
}
