#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEventTrigger.generated.h"

// 事件類型枚舉
UENUM(BlueprintType)
enum class EMingEventType : uint8
{
    Political,         // 政治事件
    Military,          // 軍事事件
    Economic,          // 經濟事件
    Diplomatic,         // 外交事件
    Social,             // 社會事件
    Cultural,          // 文化事件
    NaturalDisaster,    // 自然災害
    Revolution         // 革命事件
};

// 事件觸發類型
UENUM(BlueprintType)
enum class EMingEventTriggerType : uint8
{
    TimeBased,          // 時間觸發
    DecisionBased,      // 決策觸發
    ConditionBased,     // 條件觸發
    Random,             // 隨機觸發
    Chain,              // 連鎖觸發
    PlayerAction        // 玩家行動觸發
};

// 事件重要性級別
UENUM(BlueprintType)
enum class EMingEventImportance : uint8
{
    Minor,              // 次要事件
    Moderate,           // 中等事件
    Major,              // 重要事件
    Critical,           // 關鍵事件
    WorldChanging       // 改變世界的事件
};

// 事件狀態
UENUM(BlueprintType)
enum class EMingEventStatus : uint8
{
    Pending,            // 等待觸發
    Active,             // 活躍中
    Resolved,           // 已解決
    Failed,             // 失敗
    Expired             // 已過期
};

/**
 * 事件觸發條件結構
 */
USTRUCT(BlueprintType)
struct FMingEventTriggerCondition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventTriggerType TriggerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredYear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredMonth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ConditionParameters;

    FMingEventTriggerCondition()
        : TriggerType(EMingEventTriggerType::TimeBased)
        , RequiredYear(1912)
        , RequiredMonth(1)
    {}
};

// 事件觸發條件
USTRUCT(BlueprintType)
struct FINGCORE_API FMingCoreEventTriggerCondition
{
    GENERATED_BODY()

    // 條件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConditionID;

    // 條件類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventTriggerType TriggerType;

    // 條件描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionDescription;

    // 條件參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ConditionParameters;

    // 條件權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConditionWeight;

    // 是否為必要條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    FMingEventTriggerCondition()
    {
        ConditionID = -1;
        TriggerType = EMingEventTriggerType::ConditionBased;
        ConditionDescription = TEXT("");
        ConditionParameters.Empty();
        ConditionWeight = 1.0f;
        bIsRequired = true;
    }
};

// 事件選項
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventOption
{
    GENERATED_BODY()

    // 選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // 選項標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionTitle;

    // 選項描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // 選項後果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    // 選項前置條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Prerequisites;

    // 選項成功概率
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessProbability;

    // 選項成本
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Costs;

    // 選項收益
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Benefits;

    FMingEventOption()
    {
        OptionID = -1;
        OptionTitle = TEXT("");
        OptionDescription = TEXT("");
        Consequences.Empty();
        Prerequisites.Empty();
        SuccessProbability = 1.0f;
        Costs.Empty();
        Benefits.Empty();
    }
};

// 歷史事件數據
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalEvent
{
    GENERATED_BODY()

    // 事件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventID;

    // 事件名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventName;

    // 事件描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventDescription;

    // 歷史背景
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // 事件類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventType EventType;

    // 事件重要性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventImportance EventImportance;

    // 事件狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventStatus EventStatus;

    // 發生年份
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventYear;

    // 發生月份
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventMonth;

    // 發生地點
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventLocation;

    // 觸發條件列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingCoreEventTriggerCondition> TriggerConditions;

    // 事件選項列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingEventOption> EventOptions;

    // 事件持續時間（月）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DurationMonths;

    // 影響範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffectedRegions;

    // 參與人物
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> InvolvedCharacters;

    // 連鎖事件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ChainEvents;

    // 事件標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EventTags;

    FMingHistoricalEvent()
    {
        EventID = -1;
        EventName = TEXT("");
        EventDescription = TEXT("");
        HistoricalBackground = TEXT("");
        EventType = EMingEventType::Political;
        EventImportance = EMingEventImportance::Moderate;
        EventStatus = EMingEventStatus::Pending;
        EventYear = 1920;
        EventMonth = 1;
        EventLocation = TEXT("");
        TriggerConditions.Empty();
        EventOptions.Empty();
        DurationMonths = 1;
        AffectedRegions.Empty();
        InvolvedCharacters.Empty();
        ChainEvents.Empty();
        EventTags.Empty();
    }
};

// 事件結果
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventResult
{
    GENERATED_BODY()

    // 事件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventID;

    // 選擇的選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 事件開始時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    // 事件結束時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;

    // 事件成功與否
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    // 實際結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ActualOutcomes;

    // 影響的決策
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> AffectedDecisions;

    // 影響的人物
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> AffectedCharacters;

    // 解鎖的新事件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedEvents;

    // 歷史路徑變化
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> HistoricalPathChanges;

    FMingEventResult()
    {
        EventID = -1;
        ChosenOptionID = -1;
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
        bSuccess = false;
        ActualOutcomes.Empty();
        AffectedDecisions.Empty();
        AffectedCharacters.Empty();
        UnlockedEvents.Empty();
        HistoricalPathChanges.Empty();
    }
};

// 事件統計數據
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventStatistics
{
    GENERATED_BODY()

    // 總事件數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalEvents;

    // 已觸發事件數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggeredEvents;

    // 已解決事件數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResolvedEvents;

    // 失敗事件數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FailedEvents;

    // 各類型事件統計
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingEventType, int32> EventTypeStats;

    // 各重要性事件統計
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingEventImportance, int32> EventImportanceStats;

    FMingEventStatistics()
    {
        TotalEvents = 0;
        TriggeredEvents = 0;
        ResolvedEvents = 0;
        FailedEvents = 0;
        EventTypeStats.Empty();
        EventImportanceStats.Empty();
    }
};

// 事件觸發委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventTriggered, const FMingHistoricalEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventOptionChosen, int32, EventID, int32, OptionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventResolved, const FMingEventResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventChainTriggered, int32, ChainEventID);

/**
 * 歷史事件觸發器
 * 負責管理和觸發動態歷史事件
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalEventTrigger : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEventTrigger();

    // 初始化事件系統
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool InitializeEventSystem();

    // 註冊歷史事件
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event);

    // 檢查事件觸發條件
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool CheckEventTriggerConditions(int32 EventID);

    // 觸發歷史事件
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool TriggerHistoricalEvent(int32 EventID);

    // 處理事件選擇
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool ProcessEventChoice(int32 EventID, int32 OptionID);

    // 獲取事件信息
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    FMingHistoricalEvent GetEventInfo(int32 EventID) const;

    // 獲取當前活躍事件
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<int32> GetActiveEvents() const;

    // 獲取指定年份的事件
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<int32> GetEventsForYear(int32 Year) const;

    // 獲取事件歷史
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<FMingEventResult> GetEventHistory() const;

    // 更新事件狀態
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool UpdateEventStatus(int32 EventID, EMingEventStatus NewStatus);

    // 檢查連鎖事件
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    void CheckChainEvents(int32 EventID, const FMingEventResult& Result);

    // 模擬事件觸發
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    TArray<int32> SimulateEventTriggers(int32 CurrentYear, int32 CurrentMonth);

    // 獲取事件統計
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    FMingEventStatistics GetEventStatistics() const;

    // 保存事件數據
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool SaveEventData();

    // 載入事件數據
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool LoadEventData();

    // 清除所有事件數據
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    void ClearAllEventData();

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnEventTriggered OnEventTriggered;

    UPROPERTY(BlueprintAssignable)
    FOnEventOptionChosen OnEventOptionChosen;

    UPROPERTY(BlueprintAssignable)
    FOnEventResolved OnEventResolved;

    UPROPERTY(BlueprintAssignable)
    FOnEventChainTriggered OnEventChainTriggered;

protected:
    // 事件數據庫
    UPROPERTY()
    TMap<int32, FMingHistoricalEvent> EventDatabase;

    // 事件歷史記錄
    UPROPERTY()
    TArray<FMingEventResult> EventHistory;

    // 活躍事件列表
    UPROPERTY()
    TArray<int32> ActiveEvents;

    // 當前遊戲時間
    UPROPERTY()
    int32 CurrentGameYear;

    UPROPERTY()
    int32 CurrentGameMonth;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設事件數據
    void LoadDefaultEvents();

    // 驗證事件數據
    bool ValidateEventData(const FMingHistoricalEvent& Event) const;

    // 檢查時間觸發條件
    bool CheckTimeTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // 檢查決策觸發條件
    bool CheckDecisionTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // 檢查條件觸發
    bool CheckConditionTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // 處理事件後果
    void ProcessEventConsequences(int32 EventID, int32 OptionID);

    // 解鎖連鎖事件
    void UnlockChainEvents(int32 EventID);

    // 更新歷史進程
    void UpdateHistoricalProgress(const FMingEventResult& Result);

    // 計算事件成功率
    float CalculateEventSuccessRate(int32 EventID, int32 OptionID) const;

    // 生成動態事件
    FMingHistoricalEvent GenerateDynamicEvent(const FString& Context) const;
};
