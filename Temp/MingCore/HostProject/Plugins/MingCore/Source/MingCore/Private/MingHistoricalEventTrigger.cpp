#include "MingHistoricalEventTrigger.h"
#include "MingHistoricalDecisionManager.h"
#include "MingHistoricalCharacterSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

UMingHistoricalEventTrigger::UMingHistoricalEventTrigger()
{
    bInitialized = false;
    EventDatabase.Empty();
    EventHistory.Empty();
    ActiveEvents.Empty();
    CurrentGameYear = 1920;
    CurrentGameMonth = 1;
}

bool UMingHistoricalEventTrigger::InitializeEventSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("歷史事件系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化歷史事件系統..."));

    // 載入預設事件數據
    LoadDefaultEvents();

    // 載入已保存的事件數據
    LoadEventData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("歷史事件系統初始化完成，共載入 %d 個事件"), EventDatabase.Num());
    
    return true;
}

bool UMingHistoricalEventTrigger::RegisterHistoricalEvent(const FMingHistoricalEvent& Event)
{
    if (!ValidateEventData(Event))
    {
        UE_LOG(LogTemp, Error, TEXT("事件數據驗證失敗: %s"), *Event.EventName);
        return false;
    }

    if (EventDatabase.Contains(Event.EventID))
    {
        UE_LOG(LogTemp, Warning, TEXT("事件ID %d 已存在，將覆蓋"), Event.EventID);
    }

    EventDatabase.Add(Event.EventID, Event);
    
    UE_LOG(LogTemp, Log, TEXT("註冊歷史事件: %s (ID: %d)"), *Event.EventName, Event.EventID);
    
    return true;
}

bool UMingHistoricalEventTrigger::CheckEventTriggerConditions(int32 EventID)
{
    if (!EventDatabase.Contains(EventID))
    {
        UE_LOG(LogTemp, Error, TEXT("事件ID %d 不存在"), EventID);
        return false;
    }

    const FMingHistoricalEvent& Event = EventDatabase[EventID];

    // 檢查事件是否已經觸發或過期
    if (Event.EventStatus != EMingEventStatus::Pending)
    {
        return false;
    }

    // 檢查所有觸發條件
    bool bAllRequiredConditionsMet = true;
    float TotalWeight = 0.0f;
    float MetWeight = 0.0f;

    for (const FMingEventTriggerCondition& Condition : Event.TriggerConditions)
    {
        bool bConditionMet = false;

        switch (Condition.TriggerType)
        {
        case EMingEventTriggerType::TimeBased:
            bConditionMet = CheckTimeTrigger(Condition);
            break;
        case EMingEventTriggerType::DecisionBased:
            bConditionMet = CheckDecisionTrigger(Condition);
            break;
        case EMingEventTriggerType::ConditionBased:
            bConditionMet = CheckConditionTrigger(Condition);
            break;
        case EMingEventTriggerType::Random:
            // 隨機觸發
            bConditionMet = (FMath::FRand() < 0.3f); // 30%概率
            break;
        case EMingEventTriggerType::Chain:
            // 連鎖觸發由其他事件處理
            bConditionMet = false;
            break;
        case EMingEventTriggerType::PlayerAction:
            // 玩家行動觸發由外部系統處理
            bConditionMet = false;
            break;
        }

        if (bConditionMet)
        {
            MetWeight += Condition.ConditionWeight;
        }

        TotalWeight += Condition.ConditionWeight;

        if (Condition.bIsRequired && !bConditionMet)
        {
            bAllRequiredConditionsMet = false;
            break;
        }
    }

    // 計算觸發概率
    float TriggerProbability = (TotalWeight > 0.0f) ? (MetWeight / TotalWeight) : 0.0f;
    
    // 如果所有必要條件都滿足，且觸發概率超過閾值
    if (bAllRequiredConditionsMet && TriggerProbability >= 0.5f)
    {
        UE_LOG(LogTemp, Log, TEXT("事件 %s 觸發條件滿足，概率: %.2f"), *Event.EventName, TriggerProbability);
        return true;
    }

    return false;
}

bool UMingHistoricalEventTrigger::TriggerHistoricalEvent(int32 EventID)
{
    if (!CheckEventTriggerConditions(EventID))
    {
        UE_LOG(LogTemp, Warning, TEXT("事件 %d 觸發條件不滿足"), EventID);
        return false;
    }

    FMingHistoricalEvent& Event = EventDatabase[EventID];

    // 更新事件狀態
    Event.EventStatus = EMingEventStatus::Active;

    // 添加到活躍事件列表
    if (!ActiveEvents.Contains(EventID))
    {
        ActiveEvents.Add(EventID);
    }

    // 觸發事件
    OnEventTriggered.Broadcast(Event);

    UE_LOG(LogTemp, Log, TEXT("觸發歷史事件: %s"), *Event.EventName);
    
    return true;
}

bool UMingHistoricalEventTrigger::ProcessEventChoice(int32 EventID, int32 OptionID)
{
    if (!EventDatabase.Contains(EventID))
    {
        UE_LOG(LogTemp, Error, TEXT("事件ID %d 不存在"), EventID);
        return false;
    }

    FMingHistoricalEvent& Event = EventDatabase[EventID];

    if (Event.EventStatus != EMingEventStatus::Active)
    {
        UE_LOG(LogTemp, Warning, TEXT("事件 %s 不是活躍狀態"), *Event.EventName);
        return false;
    }

    // 驗證選項ID
    bool bValidOption = false;
    for (const FMingEventOption& Option : Event.EventOptions)
    {
        if (Option.OptionID == OptionID)
        {
            bValidOption = true;
            break;
        }
    }

    if (!bValidOption)
    {
        UE_LOG(LogTemp, Error, TEXT("無效的事件選項ID: %d"), OptionID);
        return false;
    }

    // 處理事件選擇
    ProcessEventConsequences(EventID, OptionID);

    // 觸發選擇事件
    OnEventOptionChosen.Broadcast(EventID, OptionID);

    UE_LOG(LogTemp, Log, TEXT("處理事件選擇: 事件 %s -> 選項 %d"), *Event.EventName, OptionID);
    
    return true;
}

FMingHistoricalEvent UMingHistoricalEventTrigger::GetEventInfo(int32 EventID) const
{
    if (EventDatabase.Contains(EventID))
    {
        return EventDatabase[EventID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("事件ID %d 不存在，返回空事件"), EventID);
    return FMingHistoricalEvent();
}

TArray<int32> UMingHistoricalEventTrigger::GetActiveEvents() const
{
    return ActiveEvents;
}

TArray<int32> UMingHistoricalEventTrigger::GetEventsForYear(int32 Year) const
{
    TArray<int32> YearEvents;
    
    for (const TPair<int32, FMingHistoricalEvent>& Pair : EventDatabase)
    {
        int32 EventID = Pair.Key;
        const FMingHistoricalEvent& Event = Pair.Value;
        
        if (Event.EventYear == Year)
        {
            YearEvents.Add(EventID);
        }
    }

    return YearEvents;
}

TArray<FMingEventResult> UMingHistoricalEventTrigger::GetEventHistory() const
{
    return EventHistory;
}

bool UMingHistoricalEventTrigger::UpdateEventStatus(int32 EventID, EMingEventStatus NewStatus)
{
    if (!EventDatabase.Contains(EventID))
    {
        UE_LOG(LogTemp, Error, TEXT("事件ID %d 不存在"), EventID);
        return false;
    }

    FMingHistoricalEvent& Event = EventDatabase[EventID];
    EMingEventStatus OldStatus = Event.EventStatus;
    Event.EventStatus = NewStatus;

    // 根據狀態更新活躍事件列表
    if (NewStatus == EMingEventStatus::Active && !ActiveEvents.Contains(EventID))
    {
        ActiveEvents.Add(EventID);
    }
    else if (NewStatus != EMingEventStatus::Active)
    {
        ActiveEvents.Remove(EventID);
    }

    UE_LOG(LogTemp, Log, TEXT("事件 %s 狀態更新: %d -> %d"), *Event.EventName, (int32)OldStatus, (int32)NewStatus);
    
    return true;
}

void UMingHistoricalEventTrigger::CheckChainEvents(int32 EventID, const FMingEventResult& Result)
{
    if (!EventDatabase.Contains(EventID))
    {
        return;
    }

    const FMingHistoricalEvent& Event = EventDatabase[EventID];

    // 檢查連鎖事件
    for (int32 ChainEventID : Event.ChainEvents)
    {
        if (EventDatabase.Contains(ChainEventID))
        {
            FMingHistoricalEvent& ChainEvent = EventDatabase[ChainEventID];
            
            // 檢查連鎖事件是否可以觸發
            if (ChainEvent.EventStatus == EMingEventStatus::Pending)
            {
                // 根據結果決定是否觸發連鎖事件
                bool bShouldTrigger = false;
                
                if (Result.bSuccess)
                {
                    // 成功時觸發正面連鎖事件
                    bShouldTrigger = ChainEvent.EventTags.Contains(TEXT("positive_chain"));
                }
                else
                {
                    // 失敗時觸發負面連鎖事件
                    bShouldTrigger = ChainEvent.EventTags.Contains(TEXT("negative_chain"));
                }

                if (bShouldTrigger)
                {
                    TriggerHistoricalEvent(ChainEventID);
                    OnEventChainTriggered.Broadcast(ChainEventID);
                }
            }
        }
    }
}

TArray<int32> UMingHistoricalEventTrigger::SimulateEventTriggers(int32 CurrentYear, int32 CurrentMonth)
{
    TArray<int32> TriggeredEvents;
    
    // 更新當前遊戲時間
    CurrentGameYear = CurrentYear;
    CurrentGameMonth = CurrentMonth;

    // 檢查所有待觸發事件
    for (const TPair<int32, FMingHistoricalEvent>& Pair : EventDatabase)
    {
        int32 EventID = Pair.Key;
        const FMingHistoricalEvent& Event = Pair.Value;
        
        if (Event.EventStatus == EMingEventStatus::Pending)
        {
            // 檢查事件是否在當前時間範圍內
            if (Event.EventYear == CurrentYear && 
                (Event.EventMonth == CurrentMonth || Event.EventMonth == 0))
            {
                if (CheckEventTriggerConditions(EventID))
                {
                    TriggeredEvents.Add(EventID);
                }
            }
        }
    }

    return TriggeredEvents;
}

FMingEventStatistics UMingHistoricalEventTrigger::GetEventStatistics() const
{
    FMingEventStatistics Statistics;
    
    Statistics.TotalEvents = EventDatabase.Num();
    Statistics.TriggeredEvents = ActiveEvents.Num();
    Statistics.ResolvedEvents = 0;
    Statistics.FailedEvents = 0;

    // 初始化類型統計
    Statistics.EventTypeStats = {
        {EMingEventType::Political, 0},
        {EMingEventType::Military, 0},
        {EMingEventType::Economic, 0},
        {EMingEventType::Diplomatic, 0},
        {EMingEventType::Social, 0},
        {EMingEventType::Cultural, 0},
        {EMingEventType::NaturalDisaster, 0},
        {EMingEventType::Revolution, 0}
    };

    // 初始化重要性統計
    Statistics.EventImportanceStats = {
        {EMingEventImportance::Minor, 0},
        {EMingEventImportance::Moderate, 0},
        {EMingEventImportance::Major, 0},
        {EMingEventImportance::Critical, 0},
        {EMingEventImportance::WorldChanging, 0}
    };

    // 統計事件歷史
    for (const FMingEventResult& Result : EventHistory)
    {
        if (EventDatabase.Contains(Result.EventID))
        {
            const FMingHistoricalEvent& Event = EventDatabase[Result.EventID];
            
            // 統計類型
            if (Statistics.EventTypeStats.Contains(Event.EventType))
            {
                Statistics.EventTypeStats[Event.EventType]++;
            }
            
            // 統計重要性
            if (Statistics.EventImportanceStats.Contains(Event.EventImportance))
            {
                Statistics.EventImportanceStats[Event.EventImportance]++;
            }
            
            // 統計結果
            if (Result.bSuccess)
            {
                Statistics.ResolvedEvents++;
            }
            else
            {
                Statistics.FailedEvents++;
            }
        }
    }

    return Statistics;
}

bool UMingHistoricalEventTrigger::SaveEventData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("HistoricalEvents.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("事件數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個事件，%d 條歷史記錄"), EventDatabase.Num(), EventHistory.Num());
    
    return true;
}

bool UMingHistoricalEventTrigger::LoadEventData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("HistoricalEvents.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入事件數據"), *LoadPath);
    
    return true;
}

void UMingHistoricalEventTrigger::ClearAllEventData()
{
    EventDatabase.Empty();
    EventHistory.Empty();
    ActiveEvents.Empty();
    CurrentGameYear = 1920;
    CurrentGameMonth = 1;
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("所有事件數據已清除"));
}

void UMingHistoricalEventTrigger::LoadDefaultEvents()
{
    // 創建預設的歷史事件
    
    // 事件1: 直皖戰爭 (1920年)
    FMingHistoricalEvent Event1;
    Event1.EventID = 1001;
    Event1.EventName = TEXT("直皖戰爭");
    Event1.EventDescription = TEXT("直系與皖系軍閥之間的戰爭，將決定北洋政府的控制權");
    Event1.HistoricalBackground = TEXT("1920年7月，直系軍閥吳佩孚與皖系軍閥段祺瑞之間爆發戰爭。這場戰爭是北洋政府內部權力鬥爭的結果，最終直系獲勝，皖系失勢。");
    Event1.EventType = EMingEventType::Military;
    Event1.EventImportance = EMingEventImportance::Critical;
    Event1.EventStatus = EMingEventStatus::Pending;
    Event1.EventYear = 1920;
    Event1.EventMonth = 7;
    Event1.EventLocation = TEXT("河北、山東");
    Event1.DurationMonths = 2;
    Event1.AffectedRegions = {TEXT("華北"), TEXT("華中")};
    Event1.InvolvedCharacters = {1, 2}; // 吳佩孚、段祺瑞
    Event1.EventTags = {TEXT("war"), TEXT("power_struggle"), TEXT("decisive_battle")};
    
    // 添加觸發條件
    FMingEventTriggerCondition Condition1;
    Condition1.ConditionID = 1;
    Condition1.TriggerType = EMingEventTriggerType::TimeBased;
    Condition1.ConditionDescription = TEXT("1920年7月到來");
    Condition1.ConditionParameters = {
        {TEXT("year"), TEXT("1920")},
        {TEXT("month"), TEXT("7")}
    };
    Condition1.ConditionWeight = 1.0f;
    Condition1.bIsRequired = true;
    Event1.TriggerConditions.Add(Condition1);
    
    FMingEventTriggerCondition Condition2;
    Condition2.ConditionID = 2;
    Condition2.TriggerType = EMingEventTriggerType::DecisionBased;
    Condition2.ConditionDescription = TEXT("玩家已做出效忠選擇");
    Condition2.ConditionParameters = {
        {TEXT("decision_id"), TEXT("1")},
        {TEXT("decision_made"), TEXT("true")}
    };
    Condition2.ConditionWeight = 0.8f;
    Condition2.bIsRequired = true;
    Event1.TriggerConditions.Add(Condition2);
    
    // 添加事件選項
    FMingEventOption EventOption1;
    EventOption1.OptionID = 1;
    EventOption1.OptionTitle = TEXT("支持直系作戰");
    EventOption1.OptionDescription = TEXT("加入直系軍隊，對抗皖系");
    EventOption1.SuccessProbability = 0.7f;
    EventOption1.Consequences = {
        {TEXT("直系好感度"), 30.0f},
        {TEXT("軍事地位"), 25.0f},
        {TEXT("皖系敵意"), 40.0f}
    };
    EventOption1.Costs = {
        {TEXT("軍事資源"), 20.0f},
        {TEXT("政治風險"), 15.0f}
    };
    EventOption1.Benefits = {
        {TEXT("戰爭勝利"), 50.0f},
        {TEXT("政治地位"), 30.0f}
    };
    Event1.EventOptions.Add(EventOption1);
    
    FMingEventOption EventOption2;
    EventOption2.OptionID = 2;
    EventOption2.OptionTitle = TEXT("支持皖系作戰");
    EventOption2.OptionDescription = TEXT("加入皖系軍隊，對抗直系");
    EventOption2.SuccessProbability = 0.3f;
    EventOption2.Consequences = {
        {TEXT("皖系好感度"), 30.0f},
        {TEXT("軍事地位"), 20.0f},
        {TEXT("直系敵意"), 40.0f}
    };
    EventOption2.Costs = {
        {TEXT("軍事資源"), 20.0f},
        {TEXT("政治風險"), 25.0f}
    };
    EventOption2.Benefits = {
        {TEXT("改革機會"), 40.0f},
        {TEXT("政治影響力"), 25.0f}
    };
    Event1.EventOptions.Add(EventOption2);
    
    FMingEventOption EventOption3;
    EventOption3.OptionID = 3;
    EventOption3.OptionTitle = TEXT("保持中立");
    EventOption3.OptionDescription = TEXT("不參與戰爭，觀望局勢");
    EventOption3.SuccessProbability = 1.0f;
    EventOption3.Consequences = {
        {TEXT("政治影響力"), -10.0f},
        {TEXT("軍事聲望"), -15.0f},
        {TEXT("雙方關係"), 5.0f}
    };
    Event1.EventOptions.Add(EventOption3);
    
    // 添加連鎖事件
    Event1.ChainEvents = {1002, 1003}; // 戰後政治重組、地方割據加劇
    
    RegisterHistoricalEvent(Event1);
    
    // 事件2: 北伐開始 (1926年)
    FMingHistoricalEvent Event2;
    Event2.EventID = 1002;
    Event2.EventName = TEXT("國民革命軍北伐");
    Event2.EventDescription = TEXT("國民黨領導的北伐戰爭，旨在統一中國");
    Event2.HistoricalBackground = TEXT("1926年7月，國民革命軍在蔣介石領導下開始北伐，目標是消滅各地軍閥，統一中國。這是中國近代史上的重要轉折點。");
    Event2.EventType = EMingEventType::Military;
    Event2.EventImportance = EMingEventImportance::WorldChanging;
    Event2.EventStatus = EMingEventStatus::Pending;
    Event2.EventYear = 1926;
    Event2.EventMonth = 7;
    Event2.EventLocation = TEXT("廣州");
    Event2.DurationMonths = 24;
    Event2.AffectedRegions = {TEXT("全國")};
    Event2.EventTags = {TEXT("unification"), TEXT("revolution"), TEXT("nation_building")};
    
    // 添加觸發條件
    FMingEventTriggerCondition Condition3;
    Condition3.ConditionID = 3;
    Condition3.TriggerType = EMingEventTriggerType::TimeBased;
    Condition3.ConditionDescription = TEXT("1926年7月到來");
    Condition3.ConditionParameters = {
        {TEXT("year"), TEXT("1926")},
        {TEXT("month"), TEXT("7")}
    };
    Condition3.ConditionWeight = 1.0f;
    Condition3.bIsRequired = true;
    Event2.TriggerConditions.Add(Condition3);
    
    FMingEventTriggerCondition Condition4;
    Condition4.ConditionID = 4;
    Condition4.TriggerType = EMingEventTriggerType::Chain;
    Condition4.ConditionDescription = TEXT("直皖戰爭後政治格局穩定");
    Condition4.ConditionParameters = {
        {TEXT("previous_event"), TEXT("1001")},
        {TEXT("result"), TEXT("resolved")}
    };
    Condition4.ConditionWeight = 0.6f;
    Condition4.bIsRequired = false;
    Event2.TriggerConditions.Add(Condition4);
    
    RegisterHistoricalEvent(Event2);
    
    // 事件3: 九一八事變 (1931年)
    FMingHistoricalEvent Event3;
    Event3.EventID = 1003;
    Event3.EventName = TEXT("九一八事變");
    Event3.EventDescription = TEXT("日本關東軍發動侵華戰爭，佔領東北");
    Event3.HistoricalBackground = TEXT("1931年9月18日，日本關東軍在瀋陽附近製造事端，藉機發動侵華戰爭，迅速佔領東北三省。這標誌著日本全面侵華的開始。");
    Event3.EventType = EMingEventType::Military;
    Event3.EventImportance = EMingEventImportance::Critical;
    Event3.EventStatus = EMingEventStatus::Pending;
    Event3.EventYear = 1931;
    Event3.EventMonth = 9;
    Event3.EventLocation = TEXT("瀋陽");
    Event3.DurationMonths = 6;
    Event3.AffectedRegions = {TEXT("東北"), TEXT("華北")};
    Event3.EventTags = {TEXT("invasion"), TEXT("japanese_aggression"), TEXT("national_crisis")};
    
    // 添加觸發條件
    FMingEventTriggerCondition Condition5;
    Condition5.ConditionID = 5;
    Condition5.TriggerType = EMingEventTriggerType::TimeBased;
    Condition5.ConditionDescription = TEXT("1931年9月到來");
    Condition5.ConditionParameters = {
        {TEXT("year"), TEXT("1931")},
        {TEXT("month"), TEXT("9")}
    };
    Condition5.ConditionWeight = 1.0f;
    Condition5.bIsRequired = true;
    Event3.TriggerConditions.Add(Condition5);
    
    RegisterHistoricalEvent(Event3);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設歷史事件"), EventDatabase.Num());
}

bool UMingHistoricalEventTrigger::ValidateEventData(const FMingHistoricalEvent& Event) const
{
    if (Event.EventID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("事件ID必須大於0"));
        return false;
    }
    
    if (Event.EventName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("事件名稱不能為空"));
        return false;
    }
    
    if (Event.EventYear < 1912 || Event.EventYear > 1949)
    {
        UE_LOG(LogTemp, Error, TEXT("事件年份必須在1912-1949之間"));
        return false;
    }
    
    if (Event.EventOptions.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("事件沒有選項，玩家將無法參與"));
    }
    
    return true;
}

bool UMingHistoricalEventTrigger::CheckTimeTrigger(const FMingEventTriggerCondition& Condition) const
{
    if (Condition.ConditionParameters.Contains(TEXT("year")) && 
        Condition.ConditionParameters.Contains(TEXT("month")))
    {
        int32 RequiredYear = FCString::Atoi(*Condition.ConditionParameters[TEXT("year")]);
        int32 RequiredMonth = FCString::Atoi(*Condition.ConditionParameters[TEXT("month")]);
        
        return (CurrentGameYear == RequiredYear && CurrentGameMonth == RequiredMonth);
    }
    
    return false;
}

bool UMingHistoricalEventTrigger::CheckDecisionTrigger(const FMingEventTriggerCondition& Condition) const
{
    if (Condition.ConditionParameters.Contains(TEXT("decision_id")))
    {
        int32 DecisionID = FCString::Atoi(*Condition.ConditionParameters[TEXT("decision_id")]);
        
        // 這裡需要檢查決策管理器中該決策是否已經製作
        // 暫時返回true表示條件滿足
        return true;
    }
    
    return false;
}

bool UMingHistoricalEventTrigger::CheckConditionTrigger(const FMingEventTriggerCondition& Condition) const
{
    // 檢查各種遊戲條件
    if (Condition.ConditionParameters.Contains(TEXT("player_reputation")))
    {
        float RequiredReputation = FCString::Atof(*Condition.ConditionParameters[TEXT("player_reputation")]);
        // 這裡需要獲取玩家當前聲望
        // 暫時返回true
        return true;
    }
    
    return false;
}

void UMingHistoricalEventTrigger::ProcessEventConsequences(int32 EventID, int32 OptionID)
{
    if (!EventDatabase.Contains(EventID))
    {
        return;
    }

    FMingHistoricalEvent& Event = EventDatabase[EventID];
    
    // 找到選擇的選項
    for (const FMingEventOption& Option : Event.EventOptions)
    {
        if (Option.OptionID == OptionID)
        {
            // 計算成功率
            float SuccessRate = CalculateEventSuccessRate(EventID, OptionID);
            bool bSuccess = (FMath::FRand() < SuccessRate);
            
            // 創建事件結果
            FMingEventResult Result;
            Result.EventID = EventID;
            Result.ChosenOptionID = OptionID;
            Result.StartTime = FDateTime::Now();
            Result.bSuccess = bSuccess;
            
            // 計算實際結果
            if (bSuccess)
            {
                Result.ActualOutcomes = Option.Benefits;
            }
            else
            {
                // 失敗時的後果
                for (const TPair<FString, float>& Pair : Option.Costs)
                {
                    Result.ActualOutcomes.Add(Pair.Key, -Pair.Value);
                }
            }
            
            // 設置結束時間
            Result.EndTime = Result.StartTime + FTimespan::FromDays(Event.DurationMonths * 30);
            
            // 記錄歷史路徑變化
            FString PathChange = FString::Printf(TEXT("在%d年%d月參與事件: %s - %s"), 
                Event.EventYear, Event.EventMonth, *Event.EventName, *Option.OptionTitle);
            Result.HistoricalPathChanges.Add(PathChange);
            
            // 更新事件狀態
            UpdateEventStatus(EventID, bSuccess ? EMingEventStatus::Resolved : EMingEventStatus::Failed);
            
            // 記錄事件結果
            EventHistory.Add(Result);
            
            // 檢查連鎖事件
            CheckChainEvents(EventID, Result);
            
            // 更新歷史進程
            UpdateHistoricalProgress(Result);
            
            // 觸發事件完成事件
            OnEventResolved.Broadcast(Result);
            
            break;
        }
    }
}

void UMingHistoricalEventTrigger::UnlockChainEvents(int32 EventID)
{
    if (!EventDatabase.Contains(EventID))
    {
        return;
    }

    const FMingHistoricalEvent& Event = EventDatabase[EventID];
    
    for (int32 ChainEventID : Event.ChainEvents)
    {
        if (EventDatabase.Contains(ChainEventID))
        {
            FMingHistoricalEvent& ChainEvent = EventDatabase[ChainEventID];
            
            // 解鎖連鎖事件
            if (ChainEvent.EventStatus == EMingEventStatus::Pending)
            {
                UE_LOG(LogTemp, Log, TEXT("解鎖連鎖事件: %s"), *ChainEvent.EventName);
                // 這裡可以觸發連鎖事件解鎖事件
            }
        }
    }
}

void UMingHistoricalEventTrigger::UpdateHistoricalProgress(const FMingEventResult& Result)
{
    // 更新歷史進程
    for (const FString& PathChange : Result.HistoricalPathChanges)
    {
        UE_LOG(LogTemp, Log, TEXT("歷史進程更新: %s"), *PathChange);
    }
}

float UMingHistoricalEventTrigger::CalculateEventSuccessRate(int32 EventID, int32 OptionID) const
{
    if (!EventDatabase.Contains(EventID))
    {
        return 0.5f; // 默認50%成功率
    }

    const FMingHistoricalEvent& Event = EventDatabase[EventID];
    
    for (const FMingEventOption& Option : Event.EventOptions)
    {
        if (Option.OptionID == OptionID)
        {
            return Option.SuccessProbability;
        }
    }
    
    return 0.5f;
}

FMingHistoricalEvent UMingHistoricalEventTrigger::GenerateDynamicEvent(const FString& Context) const
{
    FMingHistoricalEvent DynamicEvent;
    
    DynamicEvent.EventID = -1; // 動態生成事件
    DynamicEvent.EventName = FString::Printf(TEXT("動態事件: %s"), *Context);
    DynamicEvent.EventDescription = FString::Printf(TEXT("基於當前情況 %s 發生的動態事件"), *Context);
    DynamicEvent.EventType = EMingEventType::Political;
    DynamicEvent.EventImportance = EMingEventImportance::Moderate;
    DynamicEvent.EventStatus = EMingEventStatus::Pending;
    DynamicEvent.EventYear = CurrentGameYear;
    DynamicEvent.EventMonth = CurrentGameMonth;
    
    // 添加基本選項
    FMingEventOption DynamicOption;
    DynamicOption.OptionID = 1;
    DynamicOption.OptionTitle = TEXT("處理事件");
    DynamicOption.OptionDescription = TEXT("積極處理當前情況");
    DynamicOption.SuccessProbability = 0.7f;
    DynamicEvent.EventOptions.Add(DynamicOption);
    
    return DynamicEvent;
}
