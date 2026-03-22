#include "MingHistoricalDecisionManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

UMingHistoricalDecisionManager::UMingHistoricalDecisionManager()
{
    bInitialized = false;
    DecisionDatabase.Empty();
    DecisionHistory.Empty();
    ActiveDecisions.Empty();
    PlayerAttributes.Empty();
    HistoricalPath.Empty();
}

bool UMingHistoricalDecisionManager::InitializeDecisionSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("歷史決策系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化歷史決策系統..."));

    // 初始化玩家屬性
    PlayerAttributes = {
        {TEXT("聲望"), 50.0f},
        {TEXT("軍事實力"), 30.0f},
        {TEXT("政治影響力"), 40.0f},
        {TEXT("經濟實力"), 35.0f},
        {TEXT("民眾支持度"), 45.0f}
    };

    // 載入預設決策數據
    LoadDefaultDecisions();

    // 載入已保存的決策數據
    LoadDecisionData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("歷史決策系統初始化完成，共載入 %d 個決策"), DecisionDatabase.Num());
    
    return true;
}

bool UMingHistoricalDecisionManager::RegisterHistoricalDecision(const FMingHistoricalDecision& Decision)
{
    if (!ValidateDecisionData(Decision))
    {
        UE_LOG(LogTemp, Error, TEXT("決策數據驗證失敗: %s"), *Decision.DecisionTitle);
        return false;
    }

    if (DecisionDatabase.Contains(Decision.DecisionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("決策ID %d 已存在，將覆蓋"), Decision.DecisionID);
    }

    DecisionDatabase.Add(Decision.DecisionID, Decision);
    
    UE_LOG(LogTemp, Log, TEXT("註冊歷史決策: %s (ID: %d)"), *Decision.DecisionTitle, Decision.DecisionID);
    
    return true;
}

bool UMingHistoricalDecisionManager::CheckDecisionTriggerConditions(int32 DecisionID)
{
    if (!DecisionDatabase.Contains(DecisionID))
    {
        UE_LOG(LogTemp, Error, TEXT("決策ID %d 不存在"), DecisionID);
        return false;
    }

    const FMingHistoricalDecision& Decision = DecisionDatabase[DecisionID];

    // 檢查決策是否已經完成
    if (Decision.bDecisionMade)
    {
        return false;
    }

    // 檢查觸發條件
    for (const FString& Condition : Decision.TriggerConditions)
    {
        // 簡單的條件檢查邏輯
        if (Condition.Contains(TEXT("年份")))
        {
            // 提取年份條件
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(">"));
            if (Parts.Num() == 2)
            {
                int32 RequiredYear = FCString::Atoi(*Parts[1]);
                // 這裡需要獲取當前遊戲年份，暫時使用1920
                int32 CurrentYear = 1920;
                if (CurrentYear < RequiredYear)
                {
                    return false;
                }
            }
        }
        else if (Condition.Contains(TEXT("聲望")))
        {
            // 檢查聲望條件
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(">"));
            if (Parts.Num() == 2)
            {
                float RequiredReputation = FCString::Atof(*Parts[1]);
                float CurrentReputation = PlayerAttributes.FindRef(TEXT("聲望"));
                if (CurrentReputation < RequiredReputation)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

bool UMingHistoricalDecisionManager::PresentDecisionToPlayer(int32 DecisionID)
{
    if (!CheckDecisionTriggerConditions(DecisionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("決策 %d 觸發條件不滿足"), DecisionID);
        return false;
    }

    const FMingHistoricalDecision& Decision = DecisionDatabase[DecisionID];
    
    // 獲取可用選項
    TArray<FMingDecisionOption> AvailableOptions = GetAvailableOptions(DecisionID);
    
    if (AvailableOptions.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("決策 %d 沒有可用選項"), DecisionID);
        return false;
    }

    // 添加到活躍決策列表
    if (!ActiveDecisions.Contains(DecisionID))
    {
        ActiveDecisions.Add(DecisionID);
    }

    // 觸發決策呈現事件
    OnDecisionPresented.Broadcast(DecisionID, Decision, AvailableOptions);

    UE_LOG(LogTemp, Log, TEXT("呈現決策給玩家: %s"), *Decision.DecisionTitle);
    
    return true;
}

bool UMingHistoricalDecisionManager::ProcessPlayerDecision(int32 DecisionID, int32 ChosenOptionID)
{
    if (!DecisionDatabase.Contains(DecisionID))
    {
        UE_LOG(LogTemp, Error, TEXT("決策ID %d 不存在"), DecisionID);
        return false;
    }

    FMingHistoricalDecision& Decision = DecisionDatabase[DecisionID];
    
    // 驗證選項ID
    bool bValidOption = false;
    for (const FMingDecisionOption& Option : Decision.Options)
    {
        if (Option.OptionID == ChosenOptionID)
        {
            bValidOption = true;
            break;
        }
    }

    if (!bValidOption)
    {
        UE_LOG(LogTemp, Error, TEXT("無效的選項ID: %d"), ChosenOptionID);
        return false;
    }

    // 標記決策已製作
    Decision.bDecisionMade = true;
    Decision.ChosenOptionID = ChosenOptionID;
    Decision.DecisionTimestamp = FDateTime::Now();

    // 計算決策影響
    FMingDecisionResult Result = CalculateDecisionImpact(DecisionID, ChosenOptionID);

    // 觸發決策製作事件
    OnDecisionMade.Broadcast(DecisionID, ChosenOptionID);

    // 處理決策結果
    UpdatePlayerAttributes(Result);
    UpdateHistoricalPath(Result);
    UnlockNewDecisions(Result);
    TriggerFollowUpEvents(Result);
    RecordDecisionResult(Result);

    // 從活躍決策中移除
    ActiveDecisions.Remove(DecisionID);

    // 觸發決策完成事件
    OnDecisionCompleted.Broadcast(Result);

    UE_LOG(LogTemp, Log, TEXT("處理玩家決策完成: %s -> 選項 %d"), *Decision.DecisionTitle, ChosenOptionID);
    
    return true;
}

FMingHistoricalDecision UMingHistoricalDecisionManager::GetDecisionInfo(int32 DecisionID) const
{
    if (DecisionDatabase.Contains(DecisionID))
    {
        return DecisionDatabase[DecisionID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("決策ID %d 不存在，返回空決策"), DecisionID);
    return FMingHistoricalDecision();
}

TArray<FMingDecisionOption> UMingHistoricalDecisionManager::GetAvailableOptions(int32 DecisionID) const
{
    TArray<FMingDecisionOption> AvailableOptions;
    
    if (!DecisionDatabase.Contains(DecisionID))
    {
        return AvailableOptions;
    }

    const FMingHistoricalDecision& Decision = DecisionDatabase[DecisionID];
    
    for (const FMingDecisionOption& Option : Decision.Options)
    {
        // 檢查選項前置條件
        bool bMeetsPrerequisites = true;
        for (int32 PrereqID : Option.Prerequisites)
        {
            if (!DecisionHistory.ContainsByPredicate([PrereqID](const FMingDecisionResult& Result)
            {
                return Result.DecisionID == PrereqID;
            }))
            {
                bMeetsPrerequisites = false;
                break;
            }
        }

        if (bMeetsPrerequisites)
        {
            AvailableOptions.Add(Option);
        }
    }

    return AvailableOptions;
}

TArray<FMingDecisionResult> UMingHistoricalDecisionManager::GetDecisionHistory() const
{
    return DecisionHistory;
}

TArray<int32> UMingHistoricalDecisionManager::GetAvailableDecisionsForYear(int32 CurrentYear)
{
    TArray<int32> AvailableDecisions;
    
    for (const TPair<int32, FMingHistoricalDecision>& Pair : DecisionDatabase)
    {
        int32 DecisionID = Pair.Key;
        const FMingHistoricalDecision& Decision = Pair.Value;
        
        if (Decision.Year == CurrentYear && !Decision.bDecisionMade)
        {
            if (CheckDecisionTriggerConditions(DecisionID))
            {
                AvailableDecisions.Add(DecisionID);
            }
        }
    }

    return AvailableDecisions;
}

FMingDecisionResult UMingHistoricalDecisionManager::CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID)
{
    FMingDecisionResult Result;
    Result.DecisionID = DecisionID;
    Result.ChosenOptionID = ChosenOptionID;
    Result.DecisionTime = FDateTime::Now();

    if (!DecisionDatabase.Contains(DecisionID))
    {
        UE_LOG(LogTemp, Error, TEXT("計算決策影響時決策ID %d 不存在"), DecisionID);
        return Result;
    }

    const FMingHistoricalDecision& Decision = DecisionDatabase[DecisionID];
    
    // 找到選擇的選項
    for (const FMingDecisionOption& Option : Decision.Options)
    {
        if (Option.OptionID == ChosenOptionID)
        {
            // 計算屬性影響
            Result.AffectedAttributes = Option.Consequences;
            
            // 添加歷史路徑變化
            FString PathChange = FString::Printf(TEXT("在%d年做出決策: %s - %s"), 
                Decision.Year, *Decision.DecisionTitle, *Option.OptionTitle);
            Result.HistoricalPathChanges.Add(PathChange);
            
            // 設定解鎖的決策（基於選項權重）
            if (Option.OptionWeight > 0.8f)
            {
                // 高權重選項可能解鎖重要決策
                Result.UnlockedDecisions.Add(DecisionID + 100); // 示例ID
            }
            
            // 觸發後續事件
            if (Decision.Importance == EMingDecisionImportance::Critical)
            {
                Result.TriggeredEvents.Add(TEXT("觸發重大歷史事件"));
            }
            
            break;
        }
    }

    return Result;
}

bool UMingHistoricalDecisionManager::SaveDecisionData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("HistoricalDecisions.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("決策數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個決策歷史記錄"), DecisionHistory.Num());
    
    return true;
}

bool UMingHistoricalDecisionManager::LoadDecisionData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("HistoricalDecisions.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入決策數據"), *LoadPath);
    
    return true;
}

void UMingHistoricalDecisionManager::ClearAllDecisions()
{
    DecisionDatabase.Empty();
    DecisionHistory.Empty();
    ActiveDecisions.Empty();
    PlayerAttributes.Empty();
    HistoricalPath.Empty();
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("所有決策數據已清除"));
}

TMap<EMingDecisionType, int32> UMingHistoricalDecisionManager::GetDecisionStatistics() const
{
    TMap<EMingDecisionType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingDecisionType::Political, 0);
    Statistics.Add(EMingDecisionType::Military, 0);
    Statistics.Add(EMingDecisionType::Economic, 0);
    Statistics.Add(EMingDecisionType::Diplomatic, 0);
    Statistics.Add(EMingDecisionType::Personal, 0);
    
    // 統計已完成的決策
    for (const FMingDecisionResult& Result : DecisionHistory)
    {
        if (DecisionDatabase.Contains(Result.DecisionID))
        {
            const FMingHistoricalDecision& Decision = DecisionDatabase[Result.DecisionID];
            int32& Count = Statistics[Decision.DecisionType];
            Count++;
        }
    }
    
    return Statistics;
}

void UMingHistoricalDecisionManager::LoadDefaultDecisions()
{
    // 創建預設的歷史決策
    
    // 決策1: 選擇效忠對象 (1920年)
    FMingHistoricalDecision Decision1;
    Decision1.DecisionID = 1;
    Decision1.DecisionTitle = TEXT("選擇效忠對象");
    Decision1.DecisionDescription = TEXT("1920年，中國政局動盪，您需要選擇效忠的軍閥勢力");
    Decision1.HistoricalContext = TEXT("1920年直皖戰爭後，北洋政府分裂為多個軍閥派系，包括直系、皖系、奉系等。您的選擇將影響後續的政治格局。");
    Decision1.DecisionType = EMingDecisionType::Political;
    Decision1.Importance = EMingDecisionImportance::Critical;
    Decision1.Year = 1920;
    Decision1.TriggerConditions.Add(TEXT("年份>=1920"));
    
    // 添加選項
    FMingDecisionOption Option1;
    Option1.OptionID = 1;
    Option1.OptionTitle = TEXT("效忠直系軍閥");
    Option1.OptionDescription = TEXT("加入吳佩孚、曹錕領導的直系軍閥");
    Option1.OptionWeight = 1.0f;
    Option1.Consequences = {
        {TEXT("政治影響力"), 20.0f},
        {TEXT("軍事實力"), 15.0f},
        {TEXT("聲望"), 10.0f}
    };
    Decision1.Options.Add(Option1);
    
    FMingDecisionOption Option2;
    Option2.OptionID = 2;
    Option2.OptionTitle = TEXT("效忠皖系軍閥");
    Option2.OptionDescription = TEXT("支持段祺瑞領導的皖系軍閥");
    Option2.OptionWeight = 0.8f;
    Option2.Consequences = {
        {TEXT("政治影響力"), 15.0f},
        {TEXT("軍事實力"), 10.0f},
        {TEXT("聲望"), 5.0f}
    };
    Decision1.Options.Add(Option2);
    
    FMingDecisionOption Option3;
    Option3.OptionID = 3;
    Option3.OptionTitle = TEXT("保持中立");
    Option3.OptionDescription = TEXT("不選擇任何派系，保持獨立");
    Option3.OptionWeight = 0.6f;
    Option3.Consequences = {
        {TEXT("政治影響力"), -5.0f},
        {TEXT("軍事實力"), 0.0f},
        {TEXT("聲望"), 15.0f}
    };
    Decision1.Options.Add(Option3);
    
    RegisterHistoricalDecision(Decision1);
    
    // 決策2: 經濟政策 (1921年)
    FMingHistoricalDecision Decision2;
    Decision2.DecisionID = 2;
    Decision2.DecisionTitle = TEXT("經濟發展政策");
    Decision2.DecisionDescription = TEXT("選擇您控制地區的經濟發展方向");
    Decision2.HistoricalContext = TEXT("1921年，中國面臨經濟重建的挑戰。您需要決定如何發展地方經濟，以支持您的軍事和政治目標。");
    Decision2.DecisionType = EMingDecisionType::Economic;
    Decision2.Importance = EMingDecisionImportance::Major;
    Decision2.Year = 1921;
    Decision2.TriggerConditions.Add(TEXT("年份>=1921"));
    Decision2.TriggerConditions.Add(TEXT("聲望>30"));
    
    // 添加選項
    FMingDecisionOption Option4;
    Option4.OptionID = 4;
    Option4.OptionTitle = TEXT("發展工業");
    Option4.OptionDescription = TEXT("優先發展重工業和軍事工業");
    Option4.OptionWeight = 0.9f;
    Option4.Consequences = {
        {TEXT("經濟實力"), 25.0f},
        {TEXT("軍事實力"), 20.0f},
        {TEXT("民眾支持度"), -10.0f}
    };
    Decision2.Options.Add(Option4);
    
    FMingDecisionOption Option5;
    Option5.OptionID = 5;
    Option5.OptionTitle = TEXT("發展農業");
    Option5.OptionDescription = TEXT("優先發展農業和輕工業");
    Option5.OptionWeight = 0.8f;
    Option5.Consequences = {
        {TEXT("經濟實力"), 15.0f},
        {TEXT("軍事實力"), 5.0f},
        {TEXT("民眾支持度"), 20.0f}
    };
    Decision2.Options.Add(Option5);
    
    RegisterHistoricalDecision(Decision2);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設歷史決策"), DecisionDatabase.Num());
}

bool UMingHistoricalDecisionManager::ValidateDecisionData(const FMingHistoricalDecision& Decision) const
{
    if (Decision.DecisionID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("決策ID必須大於0"));
        return false;
    }
    
    if (Decision.DecisionTitle.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("決策標題不能為空"));
        return false;
    }
    
    if (Decision.Options.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("決策必須至少有一個選項"));
        return false;
    }
    
    if (Decision.Year < 1912 || Decision.Year > 1949)
    {
        UE_LOG(LogTemp, Error, TEXT("決策年份必須在1912-1949之間"));
        return false;
    }
    
    return true;
}

void UMingHistoricalDecisionManager::UpdatePlayerAttributes(const FMingDecisionResult& Result)
{
    for (const TPair<FString, float>& Pair : Result.AffectedAttributes)
    {
        const FString& AttributeName = Pair.Key;
        float AttributeChange = Pair.Value;
        
        if (PlayerAttributes.Contains(AttributeName))
        {
            float& CurrentValue = PlayerAttributes[AttributeName];
            CurrentValue += AttributeChange;
            
            // 限制屬性範圍在0-100之間
            CurrentValue = FMath::Clamp(CurrentValue, 0.0f, 100.0f);
            
            UE_LOG(LogTemp, Log, TEXT("玩家屬性更新: %s %.1f -> %.1f"), 
                *AttributeName, CurrentValue - AttributeChange, CurrentValue);
        }
    }
}

void UMingHistoricalDecisionManager::UpdateHistoricalPath(const FMingDecisionResult& Result)
{
    for (const FString& PathChange : Result.HistoricalPathChanges)
    {
        HistoricalPath.Add(PathChange);
        UE_LOG(LogTemp, Log, TEXT("歷史路徑更新: %s"), *PathChange);
    }
}

void UMingHistoricalDecisionManager::UnlockNewDecisions(const FMingDecisionResult& Result)
{
    for (int32 DecisionID : Result.UnlockedDecisions)
    {
        if (DecisionDatabase.Contains(DecisionID))
        {
            UE_LOG(LogTemp, Log, TEXT("解鎖新決策: ID %d"), DecisionID);
            // 這裡可以觸發決策解鎖事件
        }
    }
}

void UMingHistoricalDecisionManager::TriggerFollowUpEvents(const FMingDecisionResult& Result)
{
    for (const FString& Event : Result.TriggeredEvents)
    {
        UE_LOG(LogTemp, Log, TEXT("觸發後續事件: %s"), *Event);
        // 這裡可以實作事件觸發邏輯
    }
}

void UMingHistoricalDecisionManager::RecordDecisionResult(const FMingDecisionResult& Result)
{
    DecisionHistory.Add(Result);
    
    UE_LOG(LogTemp, Log, TEXT("記錄決策結果: 決策ID %d, 選項 %d"), 
        Result.DecisionID, Result.ChosenOptionID);
}
