#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Events/MingEventTrigger.h"
#include "MingHistoricalEventManager.generated.h"

/**
 * 民�X��X�段
 */
UENUM(BlueprintType)
enum class ERepublicEra: uint8 {
    EarlyRepublic      UMETA(DisplayName = "Early Republic (1912-1928)"),
    NanjingDecade      UMETA(DisplayName = "Nanjing Decade (1928-1937)"),
    WarOfResistance    UMETA(DisplayName = "War of Resistance (1937-1945)"),
    CivilWar           UMETA(DisplayName = "Civil War (1945-1949)")
};

/**
 * 歷史事件類�?
 */
UENUM(BlueprintType)
enum class EHistoricalEventType: uint8 {
    Political           UMETA(DisplayName = "Political"),
    Military            UMETA(DisplayName = "Military"),
    Economic            UMETA(DisplayName = "Economic"),
    Social              UMETA(DisplayName = "Social"),
    Cultural            UMETA(DisplayName = "Cultural"),
    Diplomatic          UMETA(DisplayName = "Diplomatic"),
    Revolutionary      UMETA(DisplayName = "Revolutionary")
};

/**
 * 事件影響範�?
 */
UENUM(BlueprintType)
enum class EEventImpactScope: uint8 {
    Local               UMETA(DisplayName = "Local"),
    Regional            UMETA(DisplayName = "Regional"),
    National            UMETA(DisplayName = "National"),
    International       UMETA(DisplayName = "International")
};

/**
 * 民�?歷史事件定義 (Event Manager ?�本)
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
 * 事件觸發上�X */
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
 * 事件?��?結�?
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
 * 民�?歷史事件管�X * 管�X�?��X��X��?歷史事件?��X�觸X */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalEventManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEventManager();

    /**
     * ?��X��?件管?�器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Initialize();

    /**
     * ?��?事件管�X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Shutdown();

    /**
     * 註�?歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool RegisterHistoricalEvent(const FMingManagerEvent& Event);

    /**
     * ?��?註�?歷史事件
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
     * ?��?歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FMingManagerEvent GetHistoricalEvent(const FString& EventID) const;

    /**
     * ?��X�?�歷?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingManagerEvent> GetAllHistoricalEvents() const;

    /**
     * ?��X��X��X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventsByEra(ERepublicEra Era) const;

    /**
     * ?��X��?類�X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventsByType(EHistoricalEventType EventType) const;

    /**
     * ?��X�鍵事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetCriticalEvents() const;

    /**
     * ?��X�觸?��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FString> GetTriggerableEvents(const FEventTriggerContext& Context) const;

    /**
     * ?��?事件X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventChain(const FString& EventID) const;

    /**
     * ?�新?�戲上�X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void UpdateGameContext(const FEventTriggerContext& Context);

    /**
     * ?��X��X�戲上�X     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FEventTriggerContext GetCurrentGameContext() const;

    /**
     * 設置?�戲?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetGameTime(float GameTime);

    /**
     * 設置民�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetRepublicEra(ERepublicEra Era);

    /**
     * 添�X�家決�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue);

    /**
     * ?��?事件統�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TMap<FString, int32> GetEventStatistics() const;

    /**
     * ?�置?�?��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void ResetAllEvents();

    /**
     * 強制觸發事件 (?�於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    FEventExecutionResult ForceTriggerEvent(const FString& EventID);

    /**
     * ?��X�薦事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetRecommendedEvents(const FEventTriggerContext& Context) const;

protected:
    // 已註?��?歷史事件
    UPROPERTY()
    TArray<FMingManagerEvent> RegisteredEvents;

    // 事件ID?��?件�X��?
    UPROPERTY()
    TMap<FString, FMingManagerEvent> EventMap;

    // ?��X�戲上�X
    UPROPERTY()
    FEventTriggerContext CurrentContext;

    // 事件統�?
    UPROPERTY()
    TMap<FString, int32> EventStatistics;

    // 已觸?��?事件
    UPROPERTY()
    TArray<FString> TriggeredEvents;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?��X��X�歷?��?件庫
    void InitializeHistoricalEventLibrary();

    // ?�建?��X��X��?�?
    void CreateEarlyRepublicEvents();

    // ?�建?�京?�年事件
    void CreateNanjingDecadeEvents();

    // ?�建?�戰事件
    void CreateWarOfResistanceEvents();

    // ?�建?�戰事件
    void CreateCivilWarEvents();

    // 檢查?��?條件
    bool CheckTimeCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查決�?條件
    bool CheckDecisionCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查資�?條件
    bool CheckResourceCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查建�?條件
    bool CheckBuildingCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查軍�?條件
    bool CheckMilitaryCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查外交條件
    bool CheckDiplomaticCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // ?��?事件後�?
    bool ExecuteEventConsequence(const FString& Consequence, const FString& EventID);

    // ?�用事件影響
    void ApplyEventImpact(const FMingManagerEvent& Event);

    // ?�新事件統�?
    void UpdateEventStatistics(const FString& EventID);

    // 驗�?事件依賴
    bool ValidateEventDependencies(const FString& EventID) const;

    // ?��?事件影響?�數
    float CalculateEventImpact(const FMingManagerEvent& Event) const;

    // 記�?事件?��?
    void LogEvent(const FString& EventID, const FString& Message);

    // ?��?事件?��?
    FString GenerateEventReport(const FString& EventID, const FEventExecutionResult& Result) const;

    // �X條件字符�?
    TArray<FString> ParseConditionString(const FString& Condition) const;

    // 比�X��?
    bool CompareValues(float Value1, const FString& Operator, float Value2) const;

    // ?��?上�X��?
    float GetContextValue(const FString& Key, const FEventTriggerContext& Context) const;

    // 設置上�X��?
    void SetContextValue(const FString& Key, float Value, FEventTriggerContext& Context);
};
