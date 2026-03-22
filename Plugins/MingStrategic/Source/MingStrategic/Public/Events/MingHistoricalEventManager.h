#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Events/MingEventTrigger.h"
#include "MingHistoricalEventManager.generated.h"

/**
 * Ê∞ëÂ??ÇÊ??éÊÆµ
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
 * Ê≠∑Âè≤‰∫ã‰ª∂È°ûÂ?
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
 * ‰∫ã‰ª∂ÂΩ±ÈüøÁØÑÂ?
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
 * Ê∞ëÂ?Ê≠∑Âè≤‰∫ã‰ª∂ÂÆöÁæ© (Event Manager ?àÊú¨)
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
 * ‰∫ã‰ª∂Ëß∏Áôº‰∏ä‰??? */
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
 * ‰∫ã‰ª∂?∑Ë?ÁµêÊ?
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
 * Ê∞ëÂ?Ê≠∑Âè≤‰∫ã‰ª∂ÁÆ°Á??? * ÁÆ°Á??Ä?âÊ??ãÊ??üÁ?Ê≠∑Âè≤‰∫ã‰ª∂?åÂ??ãËß∏?? */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalEventManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEventManager(};

    /**
     * ?ùÂ??ñ‰?‰ª∂ÁÆ°?ÜÂô®
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Initialize(};

    /**
     * ?úÈ?‰∫ã‰ª∂ÁÆ°Á???     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Shutdown(};

    /**
     * Ë®ªÂ?Ê≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool RegisterHistoricalEvent(const FMingManagerEvent& Event};

    /**
     * ?ñÊ?Ë®ªÂ?Ê≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool UnregisterHistoricalEvent(const FString& EventID};

    /**
     * Ê™¢Êü•‰∫ã‰ª∂Ëß∏ÁôºÊ¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool CheckEventTriggerConditions(const FString& EventID, const FEventTriggerContext& Context};

    /**
     * Ëß∏ÁôºÊ≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    FEventExecutionResult TriggerHistoricalEvent(const FString& EventID, const FEventTriggerContext& Context};

    /**
     * ?≤Â?Ê≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FMingManagerEvent GetHistoricalEvent(const FString& EventID) const;

    /**
     * ?≤Â??Ä?âÊ≠∑?≤‰?‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingManagerEvent> GetAllHistoricalEvents() const;

    /**
     * ?≤Â??áÂ??ÇÊ??Ñ‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventsByEra(ERepublicEra Era) const;

    /**
     * ?≤Â??áÂ?È°ûÂ??Ñ‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventsByType(EHistoricalEventType EventType) const;

    /**
     * ?≤Â??úÈçµ‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetCriticalEvents() const;

    /**
     * ?≤Â??ØËß∏?º‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FString> GetTriggerableEvents(const FEventTriggerContext& Context) const;

    /**
     * ?≤Â?‰∫ã‰ª∂??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetEventChain(const FString& EventID) const;

    /**
     * ?¥Êñ∞?äÊà≤‰∏ä‰???     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void UpdateGameContext(const FEventTriggerContext& Context};

    /**
     * ?≤Â??∂Â??äÊà≤‰∏ä‰???     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FEventTriggerContext GetCurrentGameContext() const;

    /**
     * Ë®≠ÁΩÆ?äÊà≤?ÇÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetGameTime(float GameTime};

    /**
     * Ë®≠ÁΩÆÊ∞ëÂ??ÇÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetRepublicEra(ERepublicEra Era};

    /**
     * Ê∑ªÂ??©ÂÆ∂Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue};

    /**
     * ?≤Â?‰∫ã‰ª∂Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TMap<FString, int32> GetEventStatistics() const;

    /**
     * ?çÁΩÆ?Ä?â‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void ResetAllEvents(};

    /**
     * Âº∑Âà∂Ëß∏Áôº‰∫ã‰ª∂ (?®ÊñºÊ∏¨Ë©¶)
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    FEventExecutionResult ForceTriggerEvent(const FString& EventID};

    /**
     * ?≤Â??®Ëñ¶‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingManagerEvent> GetRecommendedEvents(const FEventTriggerContext& Context) const;

protected:
    // Â∑≤Ë®ª?äÁ?Ê≠∑Âè≤‰∫ã‰ª∂
    UPROPERTY()
    TArray<FMingManagerEvent> RegisteredEvents;

    // ‰∫ã‰ª∂ID?∞‰?‰ª∂Á??†Â?
    UPROPERTY()
    TMap<FString, FMingManagerEvent> EventMap;

    // ?∂Â??äÊà≤‰∏ä‰???    UPROPERTY()
    FEventTriggerContext CurrentContext;

    // ‰∫ã‰ª∂Áµ±Ë?
    UPROPERTY()
    TMap<FString, int32> EventStatistics;

    // Â∑≤Ëß∏?ºÁ?‰∫ã‰ª∂
    UPROPERTY()
    TArray<FString> TriggeredEvents;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ??ãÊ≠∑?≤‰?‰ª∂Â∫´
    void InitializeHistoricalEventLibrary(};

    // ?µÂª∫?©Ê??±Â??ã‰?‰ª?    void CreateEarlyRepublicEvents(};

    // ?µÂª∫?ó‰∫¨?ÅÂπ¥‰∫ã‰ª∂
    void CreateNanjingDecadeEvents(};

    // ?µÂª∫?óÊà∞‰∫ã‰ª∂
    void CreateWarOfResistanceEvents(};

    // ?µÂª∫?ßÊà∞‰∫ã‰ª∂
    void CreateCivilWarEvents(};

    // Ê™¢Êü•?ÇÈ?Ê¢ù‰ª∂
    bool CheckTimeCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // Ê™¢Êü•Ê±∫Á?Ê¢ù‰ª∂
    bool CheckDecisionCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // Ê™¢Êü•Ë≥áÊ?Ê¢ù‰ª∂
    bool CheckResourceCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // Ê™¢Êü•Âª∫Á?Ê¢ù‰ª∂
    bool CheckBuildingCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // Ê™¢Êü•Ëªç‰?Ê¢ù‰ª∂
    bool CheckMilitaryCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // Ê™¢Êü•Â§ñ‰∫§Ê¢ù‰ª∂
    bool CheckDiplomaticCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // ?∑Ë?‰∫ã‰ª∂ÂæåÊ?
    bool ExecuteEventConsequence(const FString& Consequence, const FString& EventID};

    // ?âÁî®‰∫ã‰ª∂ÂΩ±Èüø
    void ApplyEventImpact(const FMingManagerEvent& Event};

    // ?¥Êñ∞‰∫ã‰ª∂Áµ±Ë?
    void UpdateEventStatistics(const FString& EventID};

    // È©óË?‰∫ã‰ª∂‰æùË≥¥
    bool ValidateEventDependencies(const FString& EventID) const;

    // ?≤Â?‰∫ã‰ª∂ÂΩ±Èüø?ÜÊï∏
    float CalculateEventImpact(const FMingManagerEvent& Event) const;

    // Ë®òÈ?‰∫ã‰ª∂?•Ë?
    void LogEvent(const FString& EventID, const FString& Message};

    // ?üÊ?‰∫ã‰ª∂?±Â?
    FString GenerateEventReport(const FString& EventID, const FEventExecutionResult& Result) const;

    // Ëß??Ê¢ù‰ª∂Â≠óÁ¨¶‰∏?    TArray<FString> ParseConditionString(const FString& Condition) const;

    // ÊØîË??∏ÂÄ?    bool CompareValues(float Value1, const FString& Operator, float Value2) const;

    // ?≤Â?‰∏ä‰??áÂÄ?    float GetContextValue(const FString& Key, const FEventTriggerContext& Context) const;

    // Ë®≠ÁΩÆ‰∏ä‰??áÂÄ?    void SetContextValue(const FString& Key, float Value, FEventTriggerContext& Context};
};

