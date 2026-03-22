#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEventTrigger.generated.h"

// ‰∫ã‰ª∂È°ûÂ??öË?
UENUM(BlueprintType)
enum class EMingEventType : uint8
{
    Political,         // ?øÊ≤ª‰∫ã‰ª∂
    Military,          // Ëªç‰?‰∫ã‰ª∂
    Economic,          // Á∂ìÊ?‰∫ã‰ª∂
    Diplomatic,         // Â§ñ‰∫§‰∫ã‰ª∂
    Social,             // Á§æÊ?‰∫ã‰ª∂
    Cultural,          // ?áÂ?‰∫ã‰ª∂
    NaturalDisaster,    // ?™ÁÑ∂?ΩÂÆ≥
    Revolution         // ?©ÂëΩ‰∫ã‰ª∂
};

// ‰∫ã‰ª∂Ëß∏ÁôºÈ°ûÂ?
UENUM(BlueprintType)
enum class EMingEventTriggerType : uint8
{
    TimeBased,          // ?ÇÈ?Ëß∏Áôº
    DecisionBased,      // Ê±∫Á?Ëß∏Áôº
    ConditionBased,     // Ê¢ù‰ª∂Ëß∏Áôº
    Random,             // ?®Ê?Ëß∏Áôº
    Chain,              // ???Ëß∏Áôº
    PlayerAction        // ?©ÂÆ∂Ë°åÂ?Ëß∏Áôº
};

// ‰∫ã‰ª∂?çË??ßÁ???UENUM(BlueprintType)
enum class EMingEventImportance : uint8
{
    Minor,              // Ê¨°Ë?‰∫ã‰ª∂
    Moderate,           // ‰∏≠Á?‰∫ã‰ª∂
    Major,              // ?çË?‰∫ã‰ª∂
    Critical,           // ?úÈçµ‰∫ã‰ª∂
    WorldChanging       // ?πË?‰∏ñÁ??Ñ‰?‰ª?};

// ‰∫ã‰ª∂?Ä??UENUM(BlueprintType)
enum class EMingEventStatus : uint8
{
    Pending,            // Á≠âÂ?Ëß∏Áôº
    Active,             // Ê¥ªË?‰∏?    Resolved,           // Â∑≤Ëß£Ê±?    Failed,             // Â§±Ê?
    Expired             // Â∑≤È???};

/**
 * ‰∫ã‰ª∂Ëß∏ÁôºÊ¢ù‰ª∂ÁµêÊ?
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

// ‰∫ã‰ª∂Ëß∏ÁôºÊ¢ù‰ª∂
USTRUCT(BlueprintType)
struct FINGCORE_API FMingCoreEventTriggerCondition
{
    GENERATED_BODY()

    // Ê¢ù‰ª∂ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConditionID;

    // Ê¢ù‰ª∂È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventTriggerType TriggerType;

    // Ê¢ù‰ª∂?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionDescription;

    // Ê¢ù‰ª∂?ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ConditionParameters;

    // Ê¢ù‰ª∂Ê¨äÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConditionWeight;

    // ?ØÂê¶?∫Â?Ë¶ÅÊ?‰ª?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ‰∫ã‰ª∂?∏È?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventOption
{
    GENERATED_BODY()

    // ?∏È?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // ?∏È?Ê®ôÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionTitle;

    // ?∏È??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // ?∏È?ÂæåÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    // ?∏È??çÁΩÆÊ¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Prerequisites;

    // ?∏È??êÂ?Ê¶ÇÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessProbability;

    // ?∏È??êÊú¨
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Costs;

    // ?∏È??∂Á?
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

// Ê≠∑Âè≤‰∫ã‰ª∂?∏Ê?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalEvent
{
    GENERATED_BODY()

    // ‰∫ã‰ª∂ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventID;

    // ‰∫ã‰ª∂?çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventName;

    // ‰∫ã‰ª∂?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventDescription;

    // Ê≠∑Âè≤?åÊôØ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // ‰∫ã‰ª∂È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventType EventType;

    // ‰∫ã‰ª∂?çË???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventImportance EventImportance;

    // ‰∫ã‰ª∂?Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventStatus EventStatus;

    // ?ºÁ?Âπ¥‰ªΩ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventYear;

    // ?ºÁ??à‰ªΩ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventMonth;

    // ?ºÁ??∞È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventLocation;

    // Ëß∏ÁôºÊ¢ù‰ª∂?óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingCoreEventTriggerCondition> TriggerConditions;

    // ‰∫ã‰ª∂?∏È??óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingEventOption> EventOptions;

    // ‰∫ã‰ª∂?ÅÁ??ÇÈ?ÔºàÊ?Ôº?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DurationMonths;

    // ÂΩ±ÈüøÁØÑÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffectedRegions;

    // ?ÉË?‰∫∫Áâ©
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> InvolvedCharacters;

    // ???‰∫ã‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ChainEvents;

    // ‰∫ã‰ª∂Ê®ôÁ±§
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

// ‰∫ã‰ª∂ÁµêÊ?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventResult
{
    GENERATED_BODY()

    // ‰∫ã‰ª∂ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventID;

    // ?∏Ê??ÑÈÅ∏?ÖID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // ‰∫ã‰ª∂?ãÂ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    // ‰∫ã‰ª∂ÁµêÊ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;

    // ‰∫ã‰ª∂?êÂ??áÂê¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    // ÂØ¶È?ÁµêÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ActualOutcomes;

    // ÂΩ±Èüø?ÑÊ±∫Á≠?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> AffectedDecisions;

    // ÂΩ±Èüø?Ñ‰∫∫??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> AffectedCharacters;

    // Ëß???ÑÊñ∞‰∫ã‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedEvents;

    // Ê≠∑Âè≤Ë∑ØÂ?ËÆäÂ?
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

// ‰∫ã‰ª∂Áµ±Ë??∏Ê?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventStatistics
{
    GENERATED_BODY()

    // Á∏Ω‰?‰ª∂Êï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalEvents;

    // Â∑≤Ëß∏?º‰?‰ª∂Êï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggeredEvents;

    // Â∑≤Ëß£Ê±∫‰?‰ª∂Êï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResolvedEvents;

    // Â§±Ê?‰∫ã‰ª∂??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FailedEvents;

    // ?ÑÈ??ã‰?‰ª∂Áµ±Ë®?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingEventType, int32> EventTypeStats;

    // ?ÑÈ?Ë¶ÅÊÄß‰?‰ª∂Áµ±Ë®?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// ‰∫ã‰ª∂Ëß∏ÁôºÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventTriggered, const FMingHistoricalEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventOptionChosen, int32, EventID, int32, OptionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventResolved, const FMingEventResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventChainTriggered, int32, ChainEventID);

/**
 * Ê≠∑Âè≤‰∫ã‰ª∂Ëß∏Áôº?? * Ë≤†Ë≤¨ÁÆ°Á??åËß∏?ºÂ??ãÊ≠∑?≤‰?‰ª? */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalEventTrigger : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEventTrigger();

    // ?ùÂ??ñ‰?‰ª∂Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool InitializeEventSystem();

    // Ë®ªÂ?Ê≠∑Âè≤‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event);

    // Ê™¢Êü•‰∫ã‰ª∂Ëß∏ÁôºÊ¢ù‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool CheckEventTriggerConditions(int32 EventID);

    // Ëß∏ÁôºÊ≠∑Âè≤‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool TriggerHistoricalEvent(int32 EventID);

    // ?ïÁ?‰∫ã‰ª∂?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool ProcessEventChoice(int32 EventID, int32 OptionID);

    // ?≤Â?‰∫ã‰ª∂‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    FMingHistoricalEvent GetEventInfo(int32 EventID) const;

    // ?≤Â??∂Â?Ê¥ªË?‰∫ã‰ª∂
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<int32> GetActiveEvents() const;

    // ?≤Â??áÂ?Âπ¥‰ªΩ?Ñ‰?‰ª?    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<int32> GetEventsForYear(int32 Year) const;

    // ?≤Â?‰∫ã‰ª∂Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<FMingEventResult> GetEventHistory() const;

    // ?¥Êñ∞‰∫ã‰ª∂?Ä??    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool UpdateEventStatus(int32 EventID, EMingEventStatus NewStatus);

    // Ê™¢Êü•???‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    void CheckChainEvents(int32 EventID, const FMingEventResult& Result);

    // Ê®°Êì¨‰∫ã‰ª∂Ëß∏Áôº
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    TArray<int32> SimulateEventTriggers(int32 CurrentYear, int32 CurrentMonth);

    // ?≤Â?‰∫ã‰ª∂Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    FMingEventStatistics GetEventStatistics() const;

    // ‰øùÂ?‰∫ã‰ª∂?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool SaveEventData();

    // ËºâÂÖ•‰∫ã‰ª∂?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool LoadEventData();

    // Ê∏ÖÈô§?Ä?â‰?‰ª∂Êï∏??    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    void ClearAllEventData();

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable)
    FOnEventTriggered OnEventTriggered;

    UPROPERTY(BlueprintAssignable)
    FOnEventOptionChosen OnEventOptionChosen;

    UPROPERTY(BlueprintAssignable)
    FOnEventResolved OnEventResolved;

    UPROPERTY(BlueprintAssignable)
    FOnEventChainTriggered OnEventChainTriggered;

protected:
    // ‰∫ã‰ª∂?∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingHistoricalEvent> EventDatabase;

    // ‰∫ã‰ª∂Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FMingEventResult> EventHistory;

    // Ê¥ªË?‰∫ã‰ª∂?óË°®
    UPROPERTY()
    TArray<int32> ActiveEvents;

    // ?∂Â??äÊà≤?ÇÈ?
    UPROPERTY()
    int32 CurrentGameYear;

    UPROPERTY()
    int32 CurrentGameMonth;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠‰∫ã‰ª∂?∏Ê?
    void LoadDefaultEvents();

    // È©óË?‰∫ã‰ª∂?∏Ê?
    bool ValidateEventData(const FMingHistoricalEvent& Event) const;

    // Ê™¢Êü•?ÇÈ?Ëß∏ÁôºÊ¢ù‰ª∂
    bool CheckTimeTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // Ê™¢Êü•Ê±∫Á?Ëß∏ÁôºÊ¢ù‰ª∂
    bool CheckDecisionTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // Ê™¢Êü•Ê¢ù‰ª∂Ëß∏Áôº
    bool CheckConditionTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // ?ïÁ?‰∫ã‰ª∂ÂæåÊ?
    void ProcessEventConsequences(int32 EventID, int32 OptionID);

    // Ëß?????‰∫ã‰ª∂
    void UnlockChainEvents(int32 EventID);

    // ?¥Êñ∞Ê≠∑Âè≤?≤Á?
    void UpdateHistoricalProgress(const FMingEventResult& Result);

    // Ë®àÁ?‰∫ã‰ª∂?êÂ???    float CalculateEventSuccessRate(int32 EventID, int32 OptionID) const;

    // ?üÊ??ïÊ?‰∫ã‰ª∂
    FMingHistoricalEvent GenerateDynamicEvent(const FString& Context) const;
};
