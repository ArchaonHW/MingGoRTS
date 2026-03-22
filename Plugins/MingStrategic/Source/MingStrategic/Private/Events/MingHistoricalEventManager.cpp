#include "Events/MingHistoricalEventManager.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingHistoricalEventManager::UMingHistoricalEventManager()
    : bIsInitialized(false)
{
    // 預分配容量
    RegisteredEvents.Reserve(100);
    EventStatistics.Reserve(50);
    TriggeredEvents.Reserve(200);
}

void UMingHistoricalEventManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 初始化遊戲上下文
    CurrentContext = FEventTriggerContext();
    
    // 初始化歷史事件庫
    InitializeHistoricalEventLibrary();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingHistoricalEventManager initialized"));
    UE_LOG(LogTemp, Log, TEXT("Loaded %d historical events"), RegisteredEvents.Num());
}

void UMingHistoricalEventManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    RegisteredEvents.Empty();
    EventMap.Empty();
    EventStatistics.Empty();
    TriggeredEvents.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingHistoricalEventManager shutdown"));
}

bool UMingHistoricalEventManager::RegisterHistoricalEvent(const FMingHistoricalEvent& Event)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("EventManager not initialized"));
        return false;
    }

    if (Event.EventID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Event ID cannot be empty"));
        return false;
    }

    // 檢查是否已存在
    if (EventMap.Contains(Event.EventID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Event %s already registered"), *Event.EventID);
        return false;
    }

    // 添加到註冊列表
    RegisteredEvents.Add(Event);
    EventMap.Add(Event.EventID, Event);
    
    UE_LOG(LogTemp, Log, TEXT("Historical event registered: %s - %s"), 
        *Event.EventID, *Event.EventName);
    
    return true;
}

bool UMingHistoricalEventManager::UnregisterHistoricalEvent(const FString& EventID)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (!EventMap.Contains(EventID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Event %s not found"), *EventID);
        return false;
    }

    // 從註冊列表移除
    for (int32 i = 0; i < RegisteredEvents.Num(); ++i)
    {
        if (RegisteredEvents[i].EventID == EventID)
        {
            RegisteredEvents.RemoveAt(i);
            break;
        }
    }
    
    EventMap.Remove(EventID);
    
    UE_LOG(LogTemp, Log, TEXT("Historical event unregistered: %s"), *EventID);
    
    return true;
}

bool UMingHistoricalEventManager::CheckEventTriggerConditions(const FString& EventID, const FEventTriggerContext& Context)
{
    if (!bIsInitialized)
    {
        return false;
    }

    const FMingHistoricalEvent* Event = EventMap.Find(EventID);
    if (!Event)
    {
        UE_LOG(LogTemp, Error, TEXT("Event %s not found"), *EventID);
        return false;
    }

    // 檢查是否已經觸發過
    if (TriggeredEvents.Contains(EventID) && Event->bIsCriticalEvent)
    {
        return false;
    }

    // 檢查前置事件
    for (const FString& PrerequisiteEvent : Event->PrerequisiteEvents)
    {
        if (!TriggeredEvents.Contains(PrerequisiteEvent))
        {
            UE_LOG(LogTemp, Verbose, TEXT("Event %s prerequisite %s not triggered"), 
                *EventID, *PrerequisiteEvent);
            return false;
        }
    }

    // 檢查所有觸發條件
    for (const FString& Condition : Event->TriggerConditions)
    {
        if (!EvaluateCondition(Condition, Context))
        {
            UE_LOG(LogTemp, Verbose, TEXT("Event %s condition not met: %s"), *EventID, *Condition);
            return false;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Event %s conditions satisfied"), *EventID);
    return true;
}

FEventExecutionResult UMingHistoricalEventManager::TriggerHistoricalEvent(const FString& EventID, const FEventTriggerContext& Context)
{
    FEventExecutionResult Result;
    Result.EventID = EventID;
    Result.ExecutionTime = Context.CurrentGameTime;

    if (!bIsInitialized)
    {
        Result.ErrorMessage = TEXT("EventManager not initialized");
        return Result;
    }

    const FMingHistoricalEvent* Event = EventMap.Find(EventID);
    if (!Event)
    {
        Result.ErrorMessage = FString::Printf(TEXT("Event %s not found"), *EventID);
        return Result;
    }

    // 檢查觸發條件
    if (!CheckEventTriggerConditions(EventID, Context))
    {
        Result.ErrorMessage = TEXT("Trigger conditions not satisfied");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Triggering historical event: %s - %s"), 
        *EventID, *Event->EventName);

    // 執行事件後果
    for (const FString& Consequence : Event->EventConsequences)
    {
        if (ExecuteEventConsequence(Consequence, EventID))
        {
            Result.ExecutedConsequences.Add(Consequence);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to execute consequence: %s"), *Consequence);
        }
    }

    // 應用事件影響
    ApplyEventImpact(*Event);

    // 添加到已觸發列表
    TriggeredEvents.Add(EventID);

    // 更新統計
    UpdateEventStatistics(EventID);

    Result.bSuccess = true;

    // 記錄事件日誌
    LogEvent(EventID, FString::Printf(TEXT("Event triggered successfully: %s"), *Event->EventName));

    UE_LOG(LogTemp, Log, TEXT("Historical event triggered successfully: %s"), *EventID);
    
    return Result;
}

FMingHistoricalEvent UMingHistoricalEventManager::GetHistoricalEvent(const FString& EventID) const
{
    const FMingHistoricalEvent* Event = EventMap.Find(EventID);
    return Event ? *Event : FMingHistoricalEvent();
}

TArray<FMingHistoricalEvent> UMingHistoricalEventManager::GetAllHistoricalEvents() const
{
    return RegisteredEvents;
}

TArray<FMingHistoricalEvent> UMingHistoricalEventManager::GetEventsByEra(ERepublicEra Era) const
{
    TArray<FMingHistoricalEvent> Result;
    
    for (const FMingHistoricalEvent& Event : RegisteredEvents)
    {
        if (Event.Era == Era)
        {
            Result.Add(Event);
        }
    }
    
    return Result;
}

TArray<FMingHistoricalEvent> UMingHistoricalEventManager::GetEventsByType(EHistoricalEventType EventType) const
{
    TArray<FMingHistoricalEvent> Result;
    
    for (const FMingHistoricalEvent& Event : RegisteredEvents)
    {
        if (Event.EventType == EventType)
        {
            Result.Add(Event);
        }
    }
    
    return Result;
}

TArray<FMingHistoricalEvent> UMingHistoricalEventManager::GetCriticalEvents() const
{
    TArray<FMingHistoricalEvent> Result;
    
    for (const FMingHistoricalEvent& Event : RegisteredEvents)
    {
        if (Event.bIsCriticalEvent)
        {
            Result.Add(Event);
        }
    }
    
    return Result;
}

TArray<FString> UMingHistoricalEventManager::GetTriggerableEvents(const FEventTriggerContext& Context) const
{
    TArray<FString> TriggerableEvents;
    
    for (const FMingHistoricalEvent& Event : RegisteredEvents)
    {
        if (CheckEventTriggerConditions(Event.EventID, Context))
        {
            TriggerableEvents.Add(Event.EventID);
        }
    }
    
    return TriggerableEvents;
}

TArray<FMingHistoricalEvent> UMingHistoricalEventManager::GetEventChain(const FString& EventID) const
{
    TArray<FMingHistoricalEvent> EventChain;
    
    const FMingHistoricalEvent* Event = EventMap.Find(EventID);
    if (!Event)
    {
        return EventChain;
    }

    // 添加當前事件
    EventChain.Add(*Event);
    
    // 遞歸添加後續事件
    TArray<FString> ProcessedEvents;
    TArray<FString> EventsToProcess = Event->FollowUpEvents;
    
    while (EventsToProcess.Num() > 0)
    {
        FString NextEventID = EventsToProcess[0];
        EventsToProcess.RemoveAt(0);
        
        if (ProcessedEvents.Contains(NextEventID))
        {
            continue;
        }
        
        const FMingHistoricalEvent* NextEvent = EventMap.Find(NextEventID);
        if (NextEvent)
        {
            EventChain.Add(*NextEvent);
            ProcessedEvents.Add(NextEventID);
            
            // 添加後續事件到處理隊列
            for (const FString& FollowUpEvent : NextEvent->FollowUpEvents)
            {
                if (!ProcessedEvents.Contains(FollowUpEvent))
                {
                    EventsToProcess.Add(FollowUpEvent);
                }
            }
        }
    }
    
    return EventChain;
}

void UMingHistoricalEventManager::UpdateGameContext(const FEventTriggerContext& Context)
{
    CurrentContext = Context;
    UE_LOG(LogTemp, Verbose, TEXT("Game context updated - Era: %d, Year: %d"), 
        static_cast<int32>(Context.CurrentEra), Context.CurrentYear);
}

FEventTriggerContext UMingHistoricalEventManager::GetCurrentGameContext() const
{
    return CurrentContext;
}

void UMingHistoricalEventManager::SetGameTime(float GameTime)
{
    CurrentContext.CurrentGameTime = GameTime;
    
    // 根據遊戲時間更新年份和時期
    int32 GameYears = FMath::FloorToInt(GameTime / (365.0f * 24.0f * 60.0f * 60.0f)); // 假設1年 = 365天
    
    CurrentContext.CurrentYear = 1912 + GameYears;
    
    if (CurrentContext.CurrentYear <= 1928)
    {
        CurrentContext.CurrentEra = ERepublicEra::EarlyRepublic;
    }
    else if (CurrentContext.CurrentYear <= 1937)
    {
        CurrentContext.CurrentEra = ERepublicEra::NanjingDecade;
    }
    else if (CurrentContext.CurrentYear <= 1945)
    {
        CurrentContext.CurrentEra = ERepublicEra::WarOfResistance;
    }
    else
    {
        CurrentContext.CurrentEra = ERepublicEra::CivilWar;
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Game time updated: %.1f, Year: %d, Era: %d"), 
        GameTime, CurrentContext.CurrentYear, static_cast<int32>(CurrentContext.CurrentEra));
}

void UMingHistoricalEventManager::SetRepublicEra(ERepublicEra Era)
{
    CurrentContext.CurrentEra = Era;
    
    // 根據時期設置對應年份
    switch (Era)
    {
    case ERepublicEra::EarlyRepublic:
        CurrentContext.CurrentYear = 1912;
        break;
    case ERepublicEra::NanjingDecade:
        CurrentContext.CurrentYear = 1928;
        break;
    case ERepublicEra::WarOfResistance:
        CurrentContext.CurrentYear = 1937;
        break;
    case ERepublicEra::CivilWar:
        CurrentContext.CurrentYear = 1945;
        break;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Republic era set to: %d, Year: %d"), 
        static_cast<int32>(Era), CurrentContext.CurrentYear);
}

void UMingHistoricalEventManager::AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue)
{
    CurrentContext.PlayerDecisions.Add(DecisionID, DecisionValue);
    UE_LOG(LogTemp, Verbose, TEXT("Player decision added: %s = %s"), *DecisionID, *DecisionValue);
}

TMap<FString, int32> UMingHistoricalEventManager::GetEventStatistics() const
{
    return EventStatistics;
}

void UMingHistoricalEventManager::ResetAllEvents()
{
    TriggeredEvents.Empty();
    EventStatistics.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("All events reset"));
}

FEventExecutionResult UMingHistoricalEventManager::ForceTriggerEvent(const FString& EventID)
{
    return TriggerHistoricalEvent(EventID, CurrentContext);
}

TArray<FMingHistoricalEvent> UMingHistoricalEventManager::GetRecommendedEvents(const FEventTriggerContext& Context) const
{
    TArray<FMingHistoricalEvent> RecommendedEvents;
    
    for (const FMingHistoricalEvent& Event : RegisteredEvents)
    {
        // 跳過已觸發的關鍵事件
        if (Event.bIsCriticalEvent && TriggeredEvents.Contains(Event.EventID))
        {
            continue;
        }
        
        // 計算事件影響分數
        float ImpactScore = CalculateEventImpact(Event);
        
        // 檢查基本條件
        bool bBasicConditionsMet = true;
        for (const FString& Condition : Event.TriggerConditions)
        {
            if (!EvaluateBasicCondition(Condition, Context))
            {
                bBasicConditionsMet = false;
                break;
            }
        }
        
        if (bBasicConditionsMet && ImpactScore > 0.5f)
        {
            RecommendedEvents.Add(Event);
        }
    }
    
    // 按影響分數排序
    RecommendedEvents.Sort([this](const FMingHistoricalEvent& A, const FMingHistoricalEvent& B)
    {
        return CalculateEventImpact(A) > CalculateEventImpact(B);
    });
    
    return RecommendedEvents;
}

// === 內部方法實現 ===

void UMingHistoricalEventManager::InitializeHistoricalEventLibrary()
{
    CreateEarlyRepublicEvents();
    CreateNanjingDecadeEvents();
    CreateWarOfResistanceEvents();
    CreateCivilWarEvents();
}

void UMingHistoricalEventManager::CreateEarlyRepublicEvents()
{
    // 辛亥革命成功事件
    FMingHistoricalEvent XinhaiRevolution;
    XinhaiRevolution.EventID = TEXT("xinhai_revolution_1912");
    XinhaiRevolution.EventName = TEXT("辛亥革命成功");
    XinhaiRevolution.EventDescription = TEXT("1912年，孫中山領導的辛亥革命成功，中華民國成立");
    XinhaiRevolution.Era = ERepublicEra::EarlyRepublic;
    XinhaiRevolution.HistoricalYear = 1912;
    XinhaiRevolution.EventType = EHistoricalEventType::Revolutionary;
    XinhaiRevolution.ImpactScope = EEventImpactScope::National;
    XinhaiRevolution.Priority = 100;
    XinhaiRevolution.bIsCriticalEvent = true;
    XinhaiRevolution.TriggerConditions.Add(TEXT("game_time >= 0"));
    XinhaiRevolution.EventConsequences.Add(TEXT("set_republic_era:early_republic"));
    XinhaiRevolution.EventConsequences.Add(TEXT("add_national_prestige:+50"));
    XinhaiRevolution.RelatedFigures.Add(TEXT("sun_yat_sen"));
    XinhaiRevolution.RelatedLocations.Add(TEXT("nanjing"));
    XinhaiRevolution.EventTags.Add(TEXT("founding"));
    XinhaiRevolution.EventTags.Add(TEXT("revolution"));
    
    RegisterHistoricalEvent(XinhaiRevolution);

    // 二次革命事件
    FMingHistoricalEvent SecondRevolution;
    SecondRevolution.EventID = TEXT("second_revolution_1913");
    SecondRevolution.EventName = TEXT("二次革命");
    SecondRevolution.EventDescription = TEXT("1913年，孫中山發動二次革命，反對袁世凱獨裁");
    SecondRevolution.Era = ERepublicEra::EarlyRepublic;
    SecondRevolution.HistoricalYear = 1913;
    SecondRevolution.EventType = EHistoricalEventType::Political;
    SecondRevolution.ImpactScope = EEventImpactScope::National;
    SecondRevolution.Priority = 80;
    SecondRevolution.bIsCriticalEvent = true;
    SecondRevolution.PrerequisiteEvents.Add(TEXT("xinhai_revolution_1912"));
    SecondRevolution.TriggerConditions.Add(TEXT("game_time >= 8760")); // 1年後
    SecondRevolution.TriggerConditions.Add(TEXT("player_decision:oppose_yuan_shikai"));
    SecondRevolution.EventConsequences.Add(TEXT("trigger_civil_unrest"));
    SecondRevolution.EventConsequences.Add(TEXT("add_military_tension:+30"));
    SecondRevolution.RelatedFigures.Add(TEXT("sun_yat_sen"));
    SecondRevolution.RelatedFigures.Add(TEXT("yuan_shikai"));
    SecondRevolution.EventTags.Add(TEXT("conflict"));
    SecondRevolution.EventTags.Add(TEXT("political"));
    
    RegisterHistoricalEvent(SecondRevolution);
}

void UMingHistoricalEventManager::CreateNanjingDecadeEvents()
{
    // 北伐統一事件
    FMingHistoricalEvent NorthernExpedition;
    NorthernExpedition.EventID = TEXT("northern_expedition_1926");
    NorthernExpedition.EventName = TEXT("北伐統一");
    NorthernExpedition.EventDescription = TEXT("1926年，國民革命軍開始北伐，統一中國");
    NorthernExpedition.Era = ERepublicEra::NanjingDecade;
    NorthernExpedition.HistoricalYear = 1926;
    NorthernExpedition.EventType = EHistoricalEventType::Military;
    NorthernExpedition.ImpactScope = EEventImpactScope::National;
    NorthernExpedition.Priority = 90;
    NorthernExpedition.bIsCriticalEvent = true;
    NorthernExpedition.TriggerConditions.Add(TEXT("game_time >= 499320")); // 1926年
    NorthernExpedition.TriggerConditions.Add(TEXT("military_strength >= 50"));
    NorthernExpedition.EventConsequences.Add(TEXT("set_republic_era:nanjing_decade"));
    NorthernExpedition.EventConsequences.Add(TEXT("unify_china"));
    NorthernExpedition.EventConsequences.Add(TEXT("add_national_prestige:+100"));
    NorthernExpedition.RelatedFigures.Add(TEXT("chiang_kai_shek"));
    NorthernExpedition.EventTags.Add(TEXT("unification"));
    NorthernExpedition.EventTags.Add(TEXT("military"));
    
    RegisterHistoricalEvent(NorthernExpedition);

    // 九一八事變事件
    FMingHistoricalEvent MukdenIncident;
    MukdenIncident.EventID = TEXT("mukden_incident_1931");
    MukdenIncident.EventName = TEXT("九一八事變");
    MukdenIncident.EventDescription = TEXT("1931年，日本發動九一八事變，侵占中國東北");
    MukdenIncident.Era = ERepublicEra::NanjingDecade;
    MukdenIncident.HistoricalYear = 1931;
    MukdenIncident.EventType = EHistoricalEventType::Military;
    MukdenIncident.ImpactScope = EEventImpactScope::International;
    MukdenIncident.Priority = 95;
    MukdenIncident.bIsCriticalEvent = true;
    MukdenIncident.TriggerConditions.Add(TEXT("game_time >= 622080")); // 1931年
    MukdenIncident.TriggerConditions.Add(TEXT("japan_aggression >= 30"));
    MukdenIncident.EventConsequences.Add(TEXT("lose_territory:manchuria"));
    MukdenIncident.EventConsequences.Add(TEXT("add_japanese_threat:+50"));
    MukdenIncident.EventConsequences.Add(TEXT("trigger_anti_japanese_sentiment"));
    MukdenIncident.RelatedLocations.Add(TEXT("manchuria"));
    MukdenIncident.EventTags.Add(TEXT("invasion"));
    MukdenIncident.EventTags.Add(TEXT("conflict"));
    
    RegisterHistoricalEvent(MukdenIncident);
}

void UMingHistoricalEventManager::CreateWarOfResistanceEvents()
{
    // 七七事變事件
    FMingHistoricalEvent MarcoPoloBridge;
    MarcoPoloBridge.EventID = TEXT("marco_polo_bridge_1937");
    MarcoPoloBridge.EventName = TEXT("七七事變");
    MarcoPoloBridge.EventDescription = TEXT("1937年7月7日，盧溝橋事變爆發，全面抗戰開始");
    MarcoPoloBridge.Era = ERepublicEra::WarOfResistance;
    MarcoPoloBridge.HistoricalYear = 1937;
    MarcoPoloBridge.EventType = EHistoricalEventType::Military;
    MarcoPoloBridge.ImpactScope = EEventImpactScope::International;
    MarcoPoloBridge.Priority = 100;
    MarcoPoloBridge.bIsCriticalEvent = true;
    MarcoPoloBridge.PrerequisiteEvents.Add(TEXT("mukden_incident_1931"));
    MarcoPoloBridge.TriggerConditions.Add(TEXT("game_time >= 788928")); // 1937年
    MarcoPoloBridge.TriggerConditions.Add(TEXT("japan_aggression >= 70"));
    MarcoPoloBridge.EventConsequences.Add(TEXT("set_republic_era:war_of_resistance"));
    MarcoPoloBridge.EventConsequences.Add(TEXT("start_total_war"));
    MarcoPoloBridge.EventConsequences.Add(TEXT("unite_all_factions"));
    MarcoPoloBridge.EventConsequences.Add(TEXT("add_national_unity:+80"));
    MarcoPoloBridge.RelatedLocations.Add(TEXT("beijing"));
    MarcoPoloBridge.RelatedLocations.Add(TEXT("marco_polo_bridge"));
    MarcoPoloBridge.EventTags.Add(TEXT("war"));
    MarcoPoloBridge.EventTags.Add(TEXT("resistance"));
    
    RegisterHistoricalEvent(MarcoPoloBridge);

    // 南京大屠殺事件
    FMingHistoricalEvent NanjingMassacre;
    NanjingMassacre.EventID = TEXT("nanjing_massacre_1937");
    NanjingMassacre.EventName = TEXT("南京大屠殺");
    NanjingMassacre.EventDescription = TEXT("1937年12月，日軍在南京進行大屠殺");
    NanjingMassacre.Era = ERepublicEra::WarOfResistance;
    NanjingMassacre.HistoricalYear = 1937;
    NanjingMassacre.EventType = EHistoricalEventType::Social;
    NanjingMassacre.ImpactScope = EEventImpactScope::National;
    NanjingMassacre.Priority = 85;
    NanjingMassacre.bIsCriticalEvent = true;
    NanjingMassacre.PrerequisiteEvents.Add(TEXT("marco_polo_bridge_1937"));
    NanjingMassacre.TriggerConditions.Add(TEXT("game_time >= 795168")); // 1937年12月
    NanjingMassacre.TriggerConditions.Add(TEXT("nanjing_occupied"));
    NanjingMassacre.EventConsequences.Add(TEXT("add_national_trauma:+100"));
    NanjingMassacre.EventConsequences.Add(TEXT("increase_resistance_determination:+50"));
    NanjingMassacre.EventConsequences.Add(TEXT("international_condemnation"));
    NanjingMassacre.RelatedLocations.Add(TEXT("nanjing"));
    NanjingMassacre.EventTags.Add(TEXT("tragedy"));
    NanjingMassacre.EventTags.Add(TEXT("atrocity"));
    
    RegisterHistoricalEvent(NanjingMassacre);
}

void UMingHistoricalEventManager::CreateCivilWarEvents()
{
    // 重慶談判事件
    FMingHistoricalEvent ChongqingNegotiations;
    ChongqingNegotiations.EventID = TEXT("chongqing_negotiations_1945");
    ChongqingNegotiations.EventName = TEXT("重慶談判");
    ChongqingNegotiations.EventDescription = TEXT("1945年，國共兩黨在重慶進行和平談判");
    ChongqingNegotiations.Era = ERepublicEra::CivilWar;
    ChongqingNegotiations.HistoricalYear = 1945;
    ChongqingNegotiations.EventType = EHistoricalEventType::Political;
    ChongqingNegotiations.ImpactScope = EEventImpactScope::National;
    ChongqingNegotiations.Priority = 80;
    ChongqingNegotiations.bIsCriticalEvent = true;
    ChongqingNegotiations.PrerequisiteEvents.Add(TEXT("marco_polo_bridge_1937"));
    ChongqingNegotiations.TriggerConditions.Add(TEXT("game_time >= 1054080")); // 1945年
    ChongqingNegotiations.TriggerConditions.Add(TEXT("japan_defeated"));
    ChongqingNegotiations.EventConsequences.Add(TEXT("set_republic_era:civil_war"));
    ChongqingNegotiations.EventConsequences.Add(TEXT("start_peace_negotiations"));
    ChongqingNegotiations.EventConsequences.Add(TEXT("increase_political_tension:+40"));
    ChongqingNegotiations.RelatedFigures.Add(TEXT("chiang_kai_shek"));
    ChongqingNegotiations.RelatedFigures.Add(TEXT("mao_zedong"));
    ChongqingNegotiations.RelatedLocations.Add(TEXT("chongqing"));
    ChongqingNegotiations.EventTags.Add(TEXT("negotiation"));
    ChongqingNegotiations.EventTags.Add(TEXT("political"));
    
    RegisterHistoricalEvent(ChongqingNegotiations);
}

// === 條件評估方法 ===

bool UMingHistoricalEventManager::EvaluateCondition(const FString& Condition, const FEventTriggerContext& Context) const
{
    TArray<FString> Parts = ParseConditionString(Condition);
    
    if (Parts.Num() < 3)
    {
        return false;
    }
    
    FString Key = Parts[0];
    FString Operator = Parts[1];
    FString Value = Parts[2];
    
    if (Key == TEXT("game_time"))
    {
        return CompareValues(Context.CurrentGameTime, Operator, FCString::Atof(*Value));
    }
    else if (Key == TEXT("player_decision"))
    {
        FString* DecisionValue = Context.PlayerDecisions.Find(Value);
        return DecisionValue != nullptr;
    }
    else if (Key == TEXT("military_strength"))
    {
        float MilitaryStrength = GetContextValue(TEXT("military_strength"), Context);
        return CompareValues(MilitaryStrength, Operator, FCString::Atof(*Value));
    }
    
    return false;
}

bool UMingHistoricalEventManager::ExecuteEventConsequence(const FString& Consequence, const FString& EventID)
{
    TArray<FString> Parts = ParseConditionString(Consequence);
    
    if (Parts.Num() < 2)
    {
        return false;
    }
    
    FString Action = Parts[0];
    FString Parameter = Parts[1];
    
    if (Action == TEXT("set_republic_era"))
    {
        if (Parameter == TEXT("early_republic"))
        {
            SetRepublicEra(ERepublicEra::EarlyRepublic);
        }
        else if (Parameter == TEXT("nanjing_decade"))
        {
            SetRepublicEra(ERepublicEra::NanjingDecade);
        }
        else if (Parameter == TEXT("war_of_resistance"))
        {
            SetRepublicEra(ERepublicEra::WarOfResistance);
        }
        else if (Parameter == TEXT("civil_war"))
        {
            SetRepublicEra(ERepublicEra::CivilWar);
        }
        return true;
    }
    else if (Action == TEXT("add_national_prestige"))
    {
        // 這裡應該調用遊戲系統來添加聲望值
        UE_LOG(LogTemp, Log, TEXT("Added national prestige: %s"), *Parameter);
        return true;
    }
    
    return false;
}

void UMingHistoricalEventManager::ApplyEventImpact(const FMingHistoricalEvent& Event)
{
    // 根據事件影響範圍應用影響
    switch (Event.ImpactScope)
    {
    case EEventImpactScope::Local:
        UE_LOG(LogTemp, Log, TEXT("Applied local impact for event: %s"), *Event.EventID);
        break;
    case EEventImpactScope::Regional:
        UE_LOG(LogTemp, Log, TEXT("Applied regional impact for event: %s"), *Event.EventID);
        break;
    case EEventImpactScope::National:
        UE_LOG(LogTemp, Log, TEXT("Applied national impact for event: %s"), *Event.EventID);
        break;
    case EEventImpactScope::International:
        UE_LOG(LogTemp, Log, TEXT("Applied international impact for event: %s"), *Event.EventID);
        break;
    }
}

void UMingHistoricalEventManager::UpdateEventStatistics(const FString& EventID)
{
    int32* Count = EventStatistics.Find(EventID);
    if (Count)
    {
        (*Count)++;
    }
    else
    {
        EventStatistics.Add(EventID, 1);
    }
}

float UMingHistoricalEventManager::CalculateEventImpact(const FMingHistoricalEvent& Event) const
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
    
    return FMath::Clamp(Impact, 0.0f, 1.0f);
}

void UMingHistoricalEventManager::LogEvent(const FString& EventID, const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[Event %s] %s"), *EventID, *Message);
}

bool UMingHistoricalEventManager::CompareValues(float Value1, const FString& Operator, float Value2) const
{
    if (Operator == TEXT(">"))
        return Value1 > Value2;
    else if (Operator == TEXT("<"))
        return Value1 < Value2;
    else if (Operator == TEXT("=="))
        return FMath::IsNearlyEqual(Value1, Value2);
    else if (Operator == TEXT(">="))
        return Value1 >= Value2;
    else if (Operator == TEXT("<="))
        return Value1 <= Value2;
    
    return false;
}

TArray<FString> UMingHistoricalEventManager::ParseConditionString(const FString& Condition) const
{
    TArray<FString> Parts;
    Condition.ParseIntoArray(Parts, TEXT(" "));
    return Parts;
}

float UMingHistoricalEventManager::GetContextValue(const FString& Key, const FEventTriggerContext& Context) const
{
    // 簡化實作，實際應該從遊戲系統獲取
    if (Key == TEXT("military_strength"))
    {
        return 50.0f; // 示例值
    }
    else if (Key == TEXT("japan_aggression"))
    {
        return 30.0f; // 示例值
    }
    
    return 0.0f;
}

bool UMingHistoricalEventManager::EvaluateBasicCondition(const FString& Condition, const FEventTriggerContext& Context) const
{
    return EvaluateCondition(Condition, Context);
}
