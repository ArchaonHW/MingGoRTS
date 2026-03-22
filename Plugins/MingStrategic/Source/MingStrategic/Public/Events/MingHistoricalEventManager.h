#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEvents.h"
#include "Events/MingEventTrigger.h"
#include "MingHistoricalEventManager.generated.h"

/**
 * 民國時期階段
 */
UENUM(BlueprintType)
enum class ERepublicEra : uint8
{
    EarlyRepublic      UMETA(DisplayName = "Early Republic (1912-1928)"),
    NanjingDecade      UMETA(DisplayName = "Nanjing Decade (1928-1937)"),
    WarOfResistance    UMETA(DisplayName = "War of Resistance (1937-1945)"),
    CivilWar           UMETA(DisplayName = "Civil War (1945-1949)")
};

/**
 * 歷史事件類型
 */
UENUM(BlueprintType)
enum class EHistoricalEventType : uint8
{
    Political           UMETA(DisplayName = "Political"),
    Military            UMETA(DisplayName = "Military"),
    Economic            UMETA(DisplayName = "Economic"),
    Social              UMETA(DisplayName = "Social"),
    Cultural            UMETA(DisplayName = "Cultural"),
    Diplomatic          UMETA(DisplayName = "Diplomatic"),
    Revolutionary      UMETA(DisplayName = "Revolutionary")
};

/**
 * 事件影響範圍
 */
UENUM(BlueprintType)
enum class EEventImpactScope : uint8
{
    Local               UMETA(DisplayName = "Local"),
    Regional            UMETA(DisplayName = "Regional"),
    National            UMETA(DisplayName = "National"),
    International       UMETA(DisplayName = "International")
};

/**
 * 民國歷史事件定義
 */
USTRUCT(BlueprintType)
struct FMingEventManagerEvent
{
    GENERATED_BODY()

    // 事件唯一ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString EventID;

    // 事件名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString EventName;

    // 事件描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString EventDescription;

    // 民國時期
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    ERepublicEra Era;

    // 歷史年份
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    int32 HistoricalYear;

    // 事件類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    EHistoricalEventType EventType;

    // 影響範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    EEventImpactScope ImpactScope;

    // 事件優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    int32 Priority;

    // 是否為關鍵歷史事件
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    bool bIsCriticalEvent;

    // 觸發條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> TriggerConditions;

    // 事件後果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> EventConsequences;

    // 相關人物
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> RelatedFigures;

    // 相關地點
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> RelatedLocations;

    // 事件標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> EventTags;

    // 前置事件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> PrerequisiteEvents;

    // 後續事件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> FollowUpEvents;

    FMingHistoricalEvent()
        : EventID(TEXT(""))
        , EventName(TEXT(""))
        , EventDescription(TEXT(""))
        , Era(ERepublicEra::EarlyRepublic)
        , HistoricalYear(1912)
        , EventType(EHistoricalEventType::Political)
        , ImpactScope(EEventImpactScope::National)
        , Priority(0)
        , bIsCriticalEvent(false)
    {}
};

/**
 * 事件觸發上下文
 */
USTRUCT(BlueprintType)
struct FEventTriggerContext
{
    GENERATED_BODY()

    // 當前遊戲時間
    UPROPERTY(BlueprintReadOnly)
    float CurrentGameTime;

    // 當前民國時期
    UPROPERTY(BlueprintReadOnly)
    ERepublicEra CurrentEra;

    // 當前年份
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;

    // 玩家決策記錄
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> PlayerDecisions;

    // 資源狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceStatus;

    // 建築狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> BuildingStatus;

    // 軍事狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> MilitaryStatus;

    // 外交關係
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> DiplomaticRelations;

    FEventTriggerContext()
        : CurrentGameTime(0.0f)
        , CurrentEra(ERepublicEra::EarlyRepublic)
        , CurrentYear(1912)
    {}
};

/**
 * 事件執行結果
 */
USTRUCT(BlueprintType)
struct FEventExecutionResult
{
    GENERATED_BODY()

    // 是否成功執行
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 事件ID
    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    // 執行時間
    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    // 錯誤信息
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    // 已執行的後果
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ExecutedConsequences;

    // 影響的遊戲狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> AffectedStates;

    FEventExecutionResult()
        : bSuccess(false)
        , EventID(TEXT(""))
        , ExecutionTime(0.0f)
        , ErrorMessage(TEXT(""))
    {}
};

/**
 * 民國歷史事件管理器
 * 管理所有民國時期的歷史事件和動態觸發
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalEventManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEventManager();

    /**
     * 初始化事件管理器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Initialize();

    /**
     * 關閉事件管理器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Shutdown();

    /**
     * 註冊歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event);

    /**
     * 取消註冊歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool UnregisterHistoricalEvent(const FString& EventID);

    /**
     * 檢查事件觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool CheckEventTriggerConditions(const FString& EventID, const FEventTriggerContext& Context);

    /**
     * 觸發歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    FEventExecutionResult TriggerHistoricalEvent(const FString& EventID, const FEventTriggerContext& Context);

    /**
     * 獲取歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FMingHistoricalEvent GetHistoricalEvent(const FString& EventID) const;

    /**
     * 獲取所有歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetAllHistoricalEvents() const;

    /**
     * 獲取指定時期的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetEventsByEra(ERepublicEra Era) const;

    /**
     * 獲取指定類型的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetEventsByType(EHistoricalEventType EventType) const;

    /**
     * 獲取關鍵事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetCriticalEvents() const;

    /**
     * 獲取可觸發事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FString> GetTriggerableEvents(const FEventTriggerContext& Context) const;

    /**
     * 獲取事件鏈
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetEventChain(const FString& EventID) const;

    /**
     * 更新遊戲上下文
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void UpdateGameContext(const FEventTriggerContext& Context);

    /**
     * 獲取當前遊戲上下文
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FEventTriggerContext GetCurrentGameContext() const;

    /**
     * 設置遊戲時間
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetGameTime(float GameTime);

    /**
     * 設置民國時期
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetRepublicEra(ERepublicEra Era);

    /**
     * 添加玩家決策
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue);

    /**
     * 獲取事件統計
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TMap<FString, int32> GetEventStatistics() const;

    /**
     * 重置所有事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void ResetAllEvents();

    /**
     * 強制觸發事件 (用於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    FEventExecutionResult ForceTriggerEvent(const FString& EventID);

    /**
     * 獲取推薦事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetRecommendedEvents(const FEventTriggerContext& Context) const;

protected:
    // 已註冊的歷史事件
    UPROPERTY()
    TArray<FMingHistoricalEvent> RegisteredEvents;

    // 事件ID到事件的映射
    UPROPERTY()
    TMap<FString, FMingHistoricalEvent> EventMap;

    // 當前遊戲上下文
    UPROPERTY()
    FEventTriggerContext CurrentContext;

    // 事件統計
    UPROPERTY()
    TMap<FString, int32> EventStatistics;

    // 已觸發的事件
    UPROPERTY()
    TArray<FString> TriggeredEvents;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化民國歷史事件庫
    void InitializeHistoricalEventLibrary();

    // 創建早期共和國事件
    void CreateEarlyRepublicEvents();

    // 創建南京十年事件
    void CreateNanjingDecadeEvents();

    // 創建抗戰事件
    void CreateWarOfResistanceEvents();

    // 創建內戰事件
    void CreateCivilWarEvents();

    // 檢查時間條件
    bool CheckTimeCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查決策條件
    bool CheckDecisionCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查資源條件
    bool CheckResourceCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查建築條件
    bool CheckBuildingCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查軍事條件
    bool CheckMilitaryCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查外交條件
    bool CheckDiplomaticCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 執行事件後果
    bool ExecuteEventConsequence(const FString& Consequence, const FString& EventID);

    // 應用事件影響
    void ApplyEventImpact(const FMingHistoricalEvent& Event);

    // 更新事件統計
    void UpdateEventStatistics(const FString& EventID);

    // 驗證事件依賴
    bool ValidateEventDependencies(const FString& EventID) const;

    // 獲取事件影響分數
    float CalculateEventImpact(const FMingHistoricalEvent& Event) const;

    // 記錄事件日誌
    void LogEvent(const FString& EventID, const FString& Message);

    // 生成事件報告
    FString GenerateEventReport(const FString& EventID, const FEventExecutionResult& Result) const;

    // 解析條件字符串
    TArray<FString> ParseConditionString(const FString& Condition) const;

    // 比較數值
    bool CompareValues(float Value1, const FString& Operator, float Value2) const;

    // 獲取上下文值
    float GetContextValue(const FString& Key, const FEventTriggerContext& Context) const;

    // 設置上下文值
    void SetContextValue(const FString& Key, float Value, FEventTriggerContext& Context);
};
