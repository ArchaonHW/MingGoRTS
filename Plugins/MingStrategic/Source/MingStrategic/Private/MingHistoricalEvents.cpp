#include "MingHistoricalEvents.h"
#include "Engine/World.h"

UMingHistoricalEvents::UMingHistoricalEvents()
{
    WorldContext = GetWorld();
    InitializeHistoricalEvents();
}

void UMingHistoricalEvents::InitializeHistoricalEvents()
{
    HistoricalEvents.Empty();
    EventChoices.Empty();
    PlayerInfluence.Empty();
    PlayerEventChoices.Empty();
    HistoricalNotes.Empty();
    TriggeredEvents.Empty();
    
    Setup1911RevolutionEvents();
    SetupNorthernExpeditionEvents();
    SetupSecondSinoJapaneseEvents();
    SetupCivilWarEvents();
    
    UE_LOG(LogTemp, Log, TEXT("Historical events system initialized with %d events"), HistoricalEvents.Num());
}

void UMingHistoricalEvents::Setup1911RevolutionEvents()
{
    // Wuchang Uprising Event
    FMingHistoricalEvent WuchangEvent;
    WuchangEvent.EventID = TEXT("WUCHANG_UPRISING");
    WuchangEvent.EventName = TEXT("武昌起義");
    WuchangEvent.Description = TEXT("1911年10月10日，武昌新軍起義，標誌著辛亥革命的開始");
    WuchangEvent.HistoricalContext = TEXT("武昌起義是由革命黨人孫武、蔣翊武等人策劃的武裝起義。起義成功後，各地紛紛響應，最終導致清朝的覆滅。");
    WuchangEvent.Era = EMingHistoricalEra::LateQing;
    WuchangEvent.EventType = EMingHistoricalEventType::Military;
    WuchangEvent.Year = 1911;
    WuchangEvent.Month = 10;
    WuchangEvent.Day = 10;
    WuchangEvent.Location = FVector(114.3f, 30.6f, 0.0f); // Wuhan coordinates
    WuchangEvent.KeyFigures.Add(TEXT("孫中山"));
    WuchangEvent.KeyFigures.Add(TEXT("黃興"));
    WuchangEvent.KeyFigures.Add(TEXT("蔣翊武"));
    WuchangEvent.Consequences.Add(TEXT("清朝統治動搖"));
    WuchangEvent.Consequences.Add(TEXT("各地紛紛響應起義"));
    WuchangEvent.Consequences.Add(TEXT("共和思想傳播"));
    WuchangEvent.TriggerConditions.Add(TEXT("MissionComplete('REV_WUCHANG')"));
    WuchangEvent.bIsMajorEvent = true;
    WuchangEvent.bIsPlayerInfluencable = true;
    
    HistoricalEvents.Add(WuchangEvent.EventID, WuchangEvent);
    
    // Setup choices for Wuchang Uprising
    TArray<FMingEventChoice> WuchangChoices;
    
    FMingEventChoice Choice1;
    Choice1.ChoiceID = TEXT("HISTORICAL_PATH");
    Choice1.ChoiceText = TEXT("遵循歷史路線：按照史實進行武昌起義");
    Choice1.Description = TEXT("按照歷史記載的方式進行起義，確保歷史準確性");
    Choice1.Outcomes.Add(TEXT("SetCondition('HistoricalAccuracy', +0.2)"));
    Choice1.Outcomes.Add(TEXT("TriggerEvent('NANJING_ESTABLISHMENT')"));
    Choice1.InfluenceWeight = 1.0f;
    Choice1.bIsHistorical = true;
    
    FMingEventChoice Choice2;
    Choice2.ChoiceID = TEXT("EARLY_REVOLUTION");
    Choice2.ChoiceText = TEXT("提前革命：在原定時間前發動起義");
    Choice2.Description = TEXT("提前發動起義，可能會打亂清軍的部署，但也可能準備不足");
    Choice2.Outcomes.Add(TEXT("SetCondition('RevolutionSpeed', +0.3)"));
    Choice2.Outcomes.Add(TEXT("SetCondition('HistoricalAccuracy', -0.1)"));
    Choice2.InfluenceWeight = 0.8f;
    
    FMingEventChoice Choice3;
    Choice3.ChoiceID = TEXT("BROAD_ALLIANCE");
    Choice3.ChoiceText = TEXT("擴大聯盟：聯合更多地方勢力");
    Choice3.Description = TEXT("不僅依靠新軍，還要聯合地方士紳和商會");
    Choice3.Outcomes.Add(TEXT("SetCondition('SupportBase', +0.4)"));
    Choice3.Outcomes.Add(TEXT("SetCondition('RevolutionComplexity', +0.2)"));
    Choice2.InfluenceWeight = 0.9f;
    
    WuchangChoices.Add(Choice1);
    WuchangChoices.Add(Choice2);
    WuchangChoices.Add(Choice3);
    
    EventChoices.Add(WuchangEvent.EventID, WuchangChoices);
    
    // Add historical notes
    TArray<FString> WuchangNotes;
    WuchangNotes.Add(TEXT("武昌起義又稱武昌首義，是辛亥革命的起點"));
    WuchangNotes.Add(TEXT("起義成功後，湖北軍政府成立，黎元洪被推舉為都督"));
    WuchangNotes.Add(TEXT("武昌起義的成功激發了全國的革命熱潮"));
    HistoricalNotes.Add(WuchangEvent.EventID, WuchangNotes);
    
    // Establishment of Republic of China
    FMingHistoricalEvent RepublicEvent;
    RepublicEvent.EventID = TEXT("REPUBLIC_ESTABLISHMENT");
    RepublicEvent.EventName = TEXT("中華民國成立");
    RepublicEvent.Description = TEXT("1912年1月1日，中華民國正式成立，孫中山就任臨時大總統");
    RepublicEvent.HistoricalContext = TEXT("武昌起義成功後，各省代表在南京集會，選舉孫中山為臨時大總統，宣告中華民國成立。");
    RepublicEvent.Era = EMingHistoricalEra::Republic;
    RepublicEvent.EventType = EMingHistoricalEventType::Political;
    RepublicEvent.Year = 1912;
    RepublicEvent.Month = 1;
    RepublicEvent.Day = 1;
    RepublicEvent.Location = FVector(118.8f, 32.1f, 0.0f); // Nanjing coordinates
    RepublicEvent.KeyFigures.Add(TEXT("孫中山"));
    RepublicEvent.KeyFigures.Add(TEXT("黃興"));
    RepublicEvent.KeyFigures.Add(TEXT("宋教仁"));
    RepublicEvent.Consequences.Add(TEXT("清朝統治正式結束"));
    RepublicEvent.Consequences.Add(TEXT("亞洲第一個共和國建立"));
    RepublicEvent.Consequences.Add(TEXT("民主思想傳播"));
    RepublicEvent.TriggerConditions.Add(TEXT("EventComplete('WUCHANG_UPRISING')"));
    RepublicEvent.bIsMajorEvent = true;
    RepublicEvent.bIsPlayerInfluencable = false;
    
    HistoricalEvents.Add(RepublicEvent.EventID, RepublicEvent);
}

void UMingHistoricalEvents::SetupNorthernExpeditionEvents()
{
    // Northern Expedition Decision
    FMingHistoricalEvent NorthernExpeditionEvent;
    NorthernExpeditionEvent.EventID = TEXT("NORTHERN_EXPEDITION_START");
    NorthernExpeditionEvent.EventName = TEXT("北伐開始");
    NorthernExpeditionEvent.Description = TEXT("1926年7月，國民革命軍開始北伐，目標是統一中國");
    NorthernExpeditionEvent.HistoricalContext = TEXT("在蘇聯援助下，國民黨決定發動北伐，消滅北洋軍閥，統一中國。蔣介石擔任北伐軍總司令。");
    NorthernExpeditionEvent.Era = EMingHistoricalEra::NanjingDecade;
    NorthernExpeditionEvent.EventType = EMingHistoricalEventType::Military;
    NorthernExpeditionEvent.Year = 1926;
    NorthernExpeditionEvent.Month = 7;
    NorthernExpeditionEvent.Day = 9;
    NorthernExpeditionEvent.Location = FVector(113.3f, 23.1f, 0.0f); // Guangzhou coordinates
    NorthernExpeditionEvent.KeyFigures.Add(TEXT("蔣介石"));
    NorthernExpeditionEvent.KeyFigures.Add(TEXT("汪精衛"));
    NorthernExpeditionEvent.KeyFigures.Add(TEXT("周恩來"));
    NorthernExpeditionEvent.Consequences.Add(TEXT("軍閥勢力削弱"));
    NorthernExpeditionEvent.Consequences.Add(TEXT("國民政府威望提升"));
    NorthernExpeditionEvent.Consequences.Add(TEXT("國共合作加強"));
    NorthernExpeditionEvent.TriggerConditions.Add(TEXT("MissionComplete('NORTH_GUANGZHOU')"));
    NorthernExpeditionEvent.bIsMajorEvent = true;
    NorthernExpeditionEvent.bIsPlayerInfluencable = true;
    
    HistoricalEvents.Add(NorthernExpeditionEvent.EventID, NorthernExpeditionEvent);
    
    // Setup choices for Northern Expedition
    TArray<FMingEventChoice> ExpeditionChoices;
    
    FMingEventChoice Choice1;
    Choice1.ChoiceID = TEXT("THREE_ROUTE_STRATEGY");
    Choice1.ChoiceText = TEXT("三路並進：按照歷史的三路北伐策略");
    Choice1.Description = TEXT("東路從廣州，中路從湖南，西路從四川，三路並進");
    Choice1.Outcomes.Add(TEXT("SetCondition('NorthernStrategy', 'Historical')"));
    Choice1.Outcomes.Add(TEXT("SetCondition('HistoricalAccuracy', +0.15)"));
    Choice1.InfluenceWeight = 1.0f;
    Choice1.bIsHistorical = true;
    
    FMingEventChoice Choice2;
    Choice2.ChoiceID = TEXT("CENTRAL_FOCUS");
    Choice2.ChoiceText = TEXT("中路突破：集中兵力主攻中路");
    Choice2.Description = TEXT("放棄兩翼，集中主力從湖南直取武漢");
    Choice2.Outcomes.Add(TEXT("SetCondition('NorthernStrategy', 'Central')"));
    Choice2.Outcomes.Add(TEXT("SetCondition('CampaignSpeed', +0.2)"));
    Choice2.Outcomes.Add(TEXT("SetCondition('RiskLevel', +0.3)"));
    Choice2.InfluenceWeight = 0.8f;
    
    FMingEventChoice Choice3;
    Choice3.ChoiceID = TEXT("DIPLOMATIC_FIRST");
    Choice3.ChoiceText = TEXT("外交先行：先聯合部分軍閥");
    Choice3.Description = TEXT("先通過外交手段聯合部分軍閥，減少軍事阻力");
    Choice3.Outcomes.Add(TEXT("SetCondition('NorthernStrategy', 'Diplomatic')"));
    Choice3.Outcomes.Add(TEXT("SetCondition('AllianceCount', +2)"));
    Choice2.Outcomes.Add(TEXT("SetCondition('CampaignDuration', +0.4)"));
    Choice3.InfluenceWeight = 0.7f;
    
    ExpeditionChoices.Add(Choice1);
    ExpeditionChoices.Add(Choice2);
    ExpeditionChoices.Add(Choice3);
    
    EventChoices.Add(NorthernExpeditionEvent.EventID, ExpeditionChoices);
}

void UMingHistoricalEvents::SetupSecondSinoJapaneseEvents()
{
    // Marco Polo Bridge Incident
    FMingHistoricalEvent MarcoPoloEvent;
    MarcoPoloEvent.EventID = TEXT("MARCO_POLO_BRIDGE");
    MarcoPoloEvent.EventName = TEXT("盧溝橋事變");
    MarcoPoloEvent.Description = TEXT("1937年7月7日，日軍在盧溝橋挑釁，標誌著全面抗戰的開始");
    MarcoPoloEvent.HistoricalContext = TEXT("日軍以一名士兵失踪為藉口，要求進入宛平城搜查，遭到中國守軍拒絕，雙方發生衝突。");
    MarcoPoloEvent.Era = EMingHistoricalEra::SecondSinoJapanese;
    MarcoPoloEvent.EventType = EMingHistoricalEventType::Military;
    MarcoPoloEvent.Year = 1937;
    MarcoPoloEvent.Month = 7;
    MarcoPoloEvent.Day = 7;
    MarcoPoloEvent.Location = FVector(116.2f, 39.8f, 0.0f); // Beijing coordinates
    MarcoPoloEvent.KeyFigures.Add(TEXT("宋哲元"));
    MarcoPoloEvent.KeyFigures.Add(TEXT("馮治安"));
    MarcoPoloEvent.KeyFigures.Add(TEXT("佟麟閣"));
    MarcoPoloEvent.Consequences.Add(TEXT("全面抗戰爆發"));
    MarcoPoloEvent.Consequences.Add(TEXT("國共第二次合作"));
    MarcoPoloEvent.Consequences.Add(TEXT("民族意識高漲"));
    MarcoPoloEvent.TriggerConditions.Add(TEXT("TimeProgress(1937, 7, 7)"));
    MarcoPoloEvent.bIsMajorEvent = true;
    MarcoPoloEvent.bIsPlayerInfluencable = false;
    
    HistoricalEvents.Add(MarcoPoloEvent.EventID, MarcoPoloEvent);
    
    // Nanjing Massacre
    FMingHistoricalEvent NanjingMassacreEvent;
    NanjingMassacreEvent.EventID = TEXT("NANJING_MASSACRE");
    NanjingMassacreEvent.EventName = TEXT("南京大屠殺");
    NanjingMassacreEvent.Description = TEXT("1937年12月，日軍佔領南京後進行了大規模屠殺");
    NanjingMassacreEvent.HistoricalContext = TEXT("日軍佔領南京後，在六週內殺害了超過30萬中國平民和戰俘，是人類歷史上的慘劇。");
    NanjingMassacreEvent.Era = EMingHistoricalEra::SecondSinoJapanese;
    NanjingMassacreEvent.EventType = EMingHistoricalEventType::Social;
    NanjingMassacreEvent.Year = 1937;
    NanjingMassacreEvent.Month = 12;
    NanjingMassacreEvent.Day = 13;
    NanjingMassacreEvent.Location = FVector(118.8f, 32.1f, 0.0f); // Nanjing coordinates
    NanjingMassacreEvent.KeyFigures.Add(TEXT("唐生智"));
    NanjingMassacreEvent.KeyFigures.Add(TEXT("松井石根"));
    NanjingMassacreEvent.Consequences.Add(TEXT("國際社會譴責"));
    NanjingMassacreEvent.Consequences.Add(TEXT("抗戰決心加強"));
    NanjingMassacreEvent.Consequences.Add(TEXT("中日關係惡化"));
    NanjingMassacreEvent.TriggerConditions.Add(TEXT("EventComplete('MARCO_POLO_BRIDGE')"));
    NanjingMassacreEvent.TriggerConditions.Add(TEXT("CityFall('Nanjing')"));
    NanjingMassacreEvent.bIsMajorEvent = true;
    NanjingMassacreEvent.bIsPlayerInfluencable = false;
    
    HistoricalEvents.Add(NanjingMassacreEvent.EventID, NanjingMassacreEvent);
}

void UMingHistoricalEvents::SetupCivilWarEvents()
{
    // Chongqing Negotiations
    FMingHistoricalEvent ChongqingEvent;
    ChongqingEvent.EventID = TEXT("CHONGQING_NEGOTIATIONS");
    ChongqingEvent.EventName = TEXT("重慶談判");
    ChongqingEvent.Description = TEXT("1945年，毛澤東赴重慶與蔣介石進行和平談判");
    ChongqingEvent.HistoricalContext = TEXT("抗戰勝利後，國共兩黨就中國前途進行談判，簽訂《雙十協定》，但最終未能避免內戰。");
    ChongqingEvent.Era = EMingHistoricalEra::CivilWar;
    ChongqingEvent.EventType = EMingHistoricalEventType::Diplomatic;
    ChongqingEvent.Year = 1945;
    ChongqingEvent.Month = 8;
    ChongqingEvent.Day = 28;
    ChongqingEvent.Location = FVector(106.6f, 29.5f, 0.0f); // Chongqing coordinates
    ChongqingEvent.KeyFigures.Add(TEXT("毛澤東"));
    ChongqingEvent.KeyFigures.Add(TEXT("蔣介石"));
    ChongqingEvent.KeyFigures.Add(TEXT("周恩來"));
    ChongqingEvent.Consequences.Add(TEXT("《雙十協定》簽訂"));
    ChongqingEvent.Consequences.Add(TEXT("和平希望短暫"));
    ChongqingEvent.Consequences.Add(TEXT("內戰不可避免"));
    ChongqingEvent.TriggerConditions.Add(TEXT("EventComplete('JAPAN_SURRENDER')"));
    ChongqingEvent.bIsMajorEvent = true;
    ChongqingEvent.bIsPlayerInfluencable = true;
    
    HistoricalEvents.Add(ChongqingEvent.EventID, ChongqingEvent);
    
    // Setup choices for Chongqing Negotiations
    TArray<FMingEventChoice> ChongqingChoices;
    
    FMingEventChoice Choice1;
    Choice1.ChoiceID = TEXT("HISTORICAL_AGREEMENT");
    Choice1.ChoiceText = TEXT("歷史協議：按照史實簽訂《雙十協定》");
    Choice1.Description = TEXT("達成表面和平，但實際分歧依然存在");
    Choice1.Outcomes.Add(TEXT("SetCondition('PeaceDuration', 'Historical')"));
    Choice1.Outcomes.Add(TEXT("SetCondition('HistoricalAccuracy', +0.2)"));
    Choice1.InfluenceWeight = 1.0f;
    Choice1.bIsHistorical = true;
    
    FMingEventChoice Choice2;
    Choice2.ChoiceID = TEXT("GENUINE_COALITION");
    Choice2.ChoiceText = TEXT("真正聯合：建立聯合政府");
    Choice2.Description = TEXT("嘗試建立真正的聯合政府，共享政權");
    Choice2.Outcomes.Add(TEXT("SetCondition('GovernmentType', 'Coalition')"));
    Choice2.Outcomes.Add(TEXT("SetCondition('CivilWarAvoided', true)"));
    Choice2.Outcomes.Add(TEXT("SetCondition('HistoricalAccuracy', -0.5)"));
    Choice2.InfluenceWeight = 0.6f;
    
    FMingEventChoice Choice3;
    Choice3.ChoiceID = TEXT("IMMEDIATE_CONFLICT");
    Choice3.ChoiceText = TEXT("立即衝突：拒絕談判，直接內戰");
    Choice3.Description = TEXT("拒絕和平談判，立即發動內戰");
    Choice3.Outcomes.Add(TEXT("SetCondition('CivilWarStart', 'Early')"));
    Choice3.Outcomes.Add(TEXT("SetCondition('InternationalSupport', -0.3)"));
    Choice3.InfluenceWeight = 0.4f;
    
    ChongqingChoices.Add(Choice1);
    ChongqingChoices.Add(Choice2);
    ChongqingChoices.Add(Choice3);
    
    EventChoices.Add(ChongqingEvent.EventID, ChongqingChoices);
}

void UMingHistoricalEvents::TriggerEvent(const FString& EventID)
{
    FMingHistoricalEvent* Event = FindEvent(EventID);
    if (!Event)
    {
        UE_LOG(LogTemp, Warning, TEXT("Historical event not found: %s"), *EventID);
        return;
    }
    
    if (Event->bHasBeenTriggered)
    {
        UE_LOG(LogTemp, Warning, TEXT("Event already triggered: %s"), *EventID);
        return;
    }
    
    if (!AreEventConditionsMet(*Event))
    {
        UE_LOG(LogTemp, Warning, TEXT("Event conditions not met: %s"), *EventID);
        return;
    }
    
    TriggerEventInternal(*Event);
}

void UMingHistoricalEvents::TriggerEventByConditions()
{
    for (auto& Pair : HistoricalEvents)
    {
        FMingHistoricalEvent& Event = Pair.Value;
        
        if (!Event.bHasBeenTriggered && AreEventConditionsMet(Event))
        {
            TriggerEventInternal(Event);
        }
    }
}

void UMingHistoricalEvents::CompleteEvent(const FString& EventID, const FString& ChoiceID)
{
    FMingHistoricalEvent* Event = FindEvent(EventID);
    if (!Event)
    {
        return;
    }
    
    Event->bHasBeenTriggered = true;
    Event->TriggerTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    TriggeredEvents.Add(EventID);
    
    // Process choice consequences
    ProcessEventConsequences(EventID, ChoiceID);
    
    // Record player decision
    RecordPlayerDecision(EventID, ChoiceID);
    
    // Check for dependent events
    CheckEventDependencies(EventID);
    
    UE_LOG(LogTemp, Log, TEXT("Historical event completed: %s with choice %s"), *EventID, *ChoiceID);
}

TArray<FMingHistoricalEvent> UMingHistoricalEvents::GetEventsByEra(EMingHistoricalEra Era) const
{
    TArray<FMingHistoricalEvent> Result;
    for (const auto& Pair : HistoricalEvents)
    {
        if (Pair.Value.Era == Era)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingHistoricalEvent> UMingHistoricalEvents::GetAvailableEvents() const
{
    TArray<FMingHistoricalEvent> Result;
    for (const auto& Pair : HistoricalEvents)
    {
        const FMingHistoricalEvent& Event = Pair.Value;
        
        if (!Event.bHasBeenTriggered && AreEventConditionsMet(Event))
        {
            Result.Add(Event);
        }
    }
    return Result;
}

TArray<FMingHistoricalEvent> UMingHistoricalEvents::GetTriggeredEvents() const
{
    TArray<FMingHistoricalEvent> Result;
    for (const auto& Pair : HistoricalEvents)
    {
        if (Pair.Value.bHasBeenTriggered)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FMingHistoricalEvent UMingHistoricalEvents::GetEvent(const FString& EventID) const
{
    const FMingHistoricalEvent* Event = HistoricalEvents.Find(EventID);
    return Event ? *Event : FMingHistoricalEvent();
}

void UMingHistoricalEvents::PresentEventChoices(const FString& EventID)
{
    TArray<FMingEventChoice> Choices = GetEventChoices(EventID);
    
    if (Choices.Num() > 0)
    {
        OnHistoricalEventTriggered.Broadcast(GetEvent(EventID), Choices);
    }
}

void UMingHistoricalEvents::SelectEventChoice(const FString& EventID, const FString& ChoiceID)
{
    TArray<FMingEventChoice>* Choices = EventChoices.Find(EventID);
    if (!Choices)
    {
        return;
    }
    
    // Find and process the choice
    for (const FMingEventChoice& Choice : *Choices)
    {
        if (Choice.ChoiceID == ChoiceID)
        {
            CompleteEvent(EventID, ChoiceID);
            OnEventChoiceSelected.Broadcast(EventID, ChoiceID, TEXT("Choice processed"));
            break;
        }
    }
}

TArray<FMingEventChoice> UMingHistoricalEvents::GetEventChoices(const FString& EventID) const
{
    const TArray<FMingEventChoice>* Choices = EventChoices.Find(EventID);
    return Choices ? *Choices : TArray<FMingEventChoice>();
}

void UMingHistoricalEvents::ShowHistoricalBackground(const FString& EventID)
{
    FMingHistoricalEvent Event = GetEvent(EventID);
    if (!Event.EventID.IsEmpty())
    {
        OnHistoricalContextShown.Broadcast(EventID, Event.HistoricalContext);
    }
}

void UMingHistoricalEvents::AddHistoricalNote(const FString& EventID, const FString& Note)
{
    if (!HistoricalNotes.Contains(EventID))
    {
        HistoricalNotes.Add(EventID, TArray<FString>());
    }
    
    HistoricalNotes[EventID].Add(Note);
}

TArray<FString> UMingHistoricalEvents::GetHistoricalNotes(const FString& EventID) const
{
    const TArray<FString>* Notes = HistoricalNotes.Find(EventID);
    return Notes ? *Notes : TArray<FString>();
}

void UMingHistoricalEvents::SetPlayerInfluence(const FString& EventID, float Influence)
{
    PlayerInfluence.Add(EventID, FMath::Clamp(Influence, 0.0f, 1.0f));
}

float UMingHistoricalEvents::GetPlayerInfluence(const FString& EventID) const
{
    const float* Influence = PlayerInfluence.Find(EventID);
    return Influence ? *Influence : 0.0f;
}

void UMingHistoricalEvents::ModifyEventOutcome(const FString& EventID, const FString& Modification)
{
    UE_LOG(LogTemp, Log, TEXT("Modifying event outcome: %s - %s"), *EventID, *Modification);
}

TMap<FString, FString> UMingHistoricalEvents::GetPlayerChoices() const
{
    return PlayerEventChoices;
}

TArray<FString> UMingHistoricalEvents::GetEventChain(const FString& EventID) const
{
    TArray<FString> Chain;
    
    // Find events that depend on this event
    for (const auto& Pair : HistoricalEvents)
    {
        const FMingHistoricalEvent& Event = Pair.Value;
        
        for (const FString& Condition : Event.TriggerConditions)
        {
            if (Condition.Contains(EventID))
            {
                Chain.Add(Event.EventID);
            }
        }
    }
    
    return Chain;
}

bool UMingHistoricalEvents::HasEventOccurred(const FString& EventID) const
{
    return TriggeredEvents.Contains(EventID);
}

float UMingHistoricalEvents::GetHistoricalAccuracy() const
{
    // Calculate based on player choices vs historical choices
    int32 TotalEvents = 0;
    int32 HistoricalChoices = 0;
    
    for (const auto& Pair : EventChoices)
    {
        const FString& EventID = Pair.Key;
        const TArray<FMingEventChoice>& Choices = Pair.Value;
        
        if (HasEventOccurred(EventID))
        {
            TotalEvents++;
            
            const FString* PlayerChoice = PlayerEventChoices.Find(EventID);
            if (PlayerChoice)
            {
                // Check if player made historical choice
                for (const FMingEventChoice& Choice : Choices)
                {
                    if (Choice.ChoiceID == *PlayerChoice && Choice.bIsHistorical)
                    {
                        HistoricalChoices++;
                        break;
                    }
                }
            }
        }
    }
    
    return TotalEvents > 0 ? (float)HistoricalChoices / TotalEvents : 0.0f;
}

FString UMingHistoricalEvents::GetEraName(EMingHistoricalEra Era)
{
    switch (Era)
    {
    case EMingHistoricalEra::LateQing: return TEXT("晚清");
    case EMingHistoricalEra::Republic: return TEXT("民國");
    case EMingHistoricalEra::Warlord: return TEXT("軍閥");
    case EMingHistoricalEra::NanjingDecade: return TEXT("南京十年");
    case EMingHistoricalEra::SecondSinoJapanese: return TEXT("抗戰");
    case EMingHistoricalEra::CivilWar: return TEXT("內戰");
    default: return TEXT("未知");
    }
}

FString UMingHistoricalEvents::GetEventTypeName(EMingHistoricalEventType EventType)
{
    switch (EventType)
    {
    case EMingHistoricalEventType::Political: return TEXT("政治");
    case EMingHistoricalEventType::Military: return TEXT("軍事");
    case EMingHistoricalEventType::Economic: return TEXT("經濟");
    case EMingHistoricalEventType::Social: return TEXT("社會");
    case EMingHistoricalEventType::Cultural: return TEXT("文化");
    case EMingHistoricalEventType::Diplomatic: return TEXT("外交");
    default: return TEXT("未知");
    }
}

FString UMingHistoricalEvents::SaveHistoricalData() const
{
    FString Result = TEXT("{\n");
    Result += TEXT("  \"triggered_events\": [");
    
    bool First = true;
    for (const FString& EventID : TriggeredEvents)
    {
        if (!First) Result += TEXT(",");
        Result += FString::Printf(TEXT("\"%s\""), *EventID);
        First = false;
    }
    
    Result += TEXT("],\n");
    Result += TEXT("  \"player_choices\": {\n");
    
    First = true;
    for (const auto& Pair : PlayerEventChoices)
    {
        if (!First) Result += TEXT(",\n");
        Result += FString::Printf(TEXT("    \"%s\": \"%s\""), *Pair.Key, *Pair.Value);
        First = false;
    }
    
    Result += TEXT("\n  },\n");
    Result += FString::Printf(TEXT("  \"historical_accuracy\": %.2f\n"), GetHistoricalAccuracy());
    Result += TEXT("}\n");
    
    return Result;
}

void UMingHistoricalEvents::LoadHistoricalData(const FString& JsonString)
{
    // Parse JSON and restore historical data
    UE_LOG(LogTemp, Log, TEXT("Loading historical data"));
}

FMingHistoricalEvent* UMingHistoricalEvents::FindEvent(const FString& EventID)
{
    return HistoricalEvents.Find(EventID);
}

bool UMingHistoricalEvents::AreEventConditionsMet(const FMingHistoricalEvent& Event) const
{
    // This would check trigger conditions against game state
    // For now, always return true for simplicity
    return true;
}

void UMingHistoricalEvents::TriggerEventInternal(const FMingHistoricalEvent& Event)
{
    UE_LOG(LogTemp, Log, TEXT("Historical event triggered: %s"), *Event.EventName);
    
    // Present choices if player can influence
    if (Event.bIsPlayerInfluencable)
    {
        PresentEventChoices(Event.EventID);
    }
    else
    {
        // Auto-complete with historical choice
        CompleteEvent(Event.EventID, TEXT("HISTORICAL_AUTO"));
    }
}

void UMingHistoricalEvents::ProcessEventConsequences(const FString& EventID, const FString& ChoiceID)
{
    const TArray<FMingEventChoice>* Choices = EventChoices.Find(EventID);
    if (!Choices)
    {
        return;
    }
    
    for (const FMingEventChoice& Choice : *Choices)
    {
        if (Choice.ChoiceID == ChoiceID)
        {
            // Process outcomes
            for (const FString& Outcome : Choice.Outcomes)
            {
                // This would process the outcome
                UE_LOG(LogTemp, Verbose, TEXT("Processing outcome: %s"), *Outcome);
            }
            break;
        }
    }
}

void UMingHistoricalEvents::UpdateHistoricalAccuracy()
{
    // This would update the historical accuracy metric
}

void UMingHistoricalEvents::CheckEventDependencies(const FString& EventID)
{
    // Check if any events can now be triggered
    TriggerEventByConditions();
}

void UMingHistoricalEvents::RecordPlayerDecision(const FString& EventID, const FString& ChoiceID)
{
    PlayerEventChoices.Add(EventID, ChoiceID);
    UE_LOG(LogTemp, Verbose, TEXT("Recorded player decision: %s -> %s"), *EventID, *ChoiceID);
}
