#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
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
 * 民國歷史事件定義 (Event Manager 版本)
 */
USTRUCT(BlueprintType)
struct FMingManagerEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString EventDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    ERepublicEra Era;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    int32 HistoricalYear;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    EHistoricalEventType EventType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    EEventImpactScope ImpactScope;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    int32 Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    bool bIsCriticalEvent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> TriggerConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> EventConsequences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> RelatedFigures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> RelatedLocations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> EventTags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> PrerequisiteEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> FollowUpEvents;

    FMingManagerEvent()
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

    UPROPERTY(BlueprintReadOnly)
    float CurrentGameTime;

    UPROPERTY(BlueprintReadOnly)
    ERepublicEra CurrentEra;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> PlayerDecisions;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceStatus;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> BuildingStatus;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> MilitaryStatus;

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

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ExecutedConsequences;

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
    bool RegisterHistoricalEvent(const FMingManagerEvent& Event);

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
    FMingManagerEvent GetHistoricalEvent(const FString& EventID) const;

    /**
     * 獲取所有歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingManagerEvent> GetAllHistoricalEvents() const;

    /**
     * 獲取指定時期的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventsByEra(ERepublicEra Era) const;

    /**
     * 獲取指定類型的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventsByType(EHistoricalEventType EventType) const;

    /**
     * 獲取關鍵事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetCriticalEvents() const;

    /**
     * 獲取可觸發事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FString> GetTriggerableEvents(const FEventTriggerContext& Context) const;

    /**
     * 獲取事件鏈
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventChain(const FString& EventID) const;

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
    TArray<FMingManagerEvent> GetRecommendedEvents(const FEventTriggerContext& Context) const;

protected:
    // 已註冊的歷史事件
    UPROPERTY()
    TArray<FMingManagerEvent> RegisteredEvents;

    // 事件ID到事件的映射
    UPROPERTY()
    TMap<FString, FMingManagerEvent> EventMap;

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
    void ApplyEventImpact(const FMingManagerEvent& Event);

    // 更新事件統計
    void UpdateEventStatistics(const FString& EventID);

    // 驗證事件依賴
    bool ValidateEventDependencies(const FString& EventID) const;

    // 獲取事件影響分數
    float CalculateEventImpact(const FMingManagerEvent& Event) const;

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
