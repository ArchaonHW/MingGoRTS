#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEventTrigger.generated.h"

// �ƥ���X
UENUM(BlueprintType)
enum class EMingEventType : uint8
{
    Political,         // X�v�ƥ�
    Military,          // �xX�ƥ�
    Economic,          // �gX�ƥ�
    Diplomatic,         // �~��ƥ�
    Social,             // ��X�ƥ�
    Cultural,          // X�ƥ�
    NaturalDisaster,    // X�MX�`
    Revolution         // X�R�ƥ�
};

// �ƥ�Ĳ�o��X
UENUM(BlueprintType)
enum class EMingEventTriggerType : uint8
{
    TimeBased,          // XĲ�o
    DecisionBased,      // �MXĲ�o
    ConditionBased,     // ����Ĳ�o
    Random,             // XĲ�o
    Chain,              // XĲ�o
    PlayerAction        // X�a��XĲ�o
};

// �ƥ�XUENUM(BlueprintType)
enum class EMingEventImportance : uint8
{
    Minor,              // ��X�ƥ�
    Moderate,           // ��X�ƥ�
    Major,              // X�ƥ�
    Critical,           // X��ƥ�
    WorldChanging,  // X�@X};

// �ƥ�XUENUM(BlueprintType)
enum class EMingEventStatus : uint8
{
    Pending,            // ��XĲ�o
    Active,             // ��X    Resolved,           // �w��X    Failed,             // ��X
    Expired,  // �wX};

/**
 * �ƥ�Ĳ�o����X
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

// �ƥ�Ĳ�o����
USTRUCT(BlueprintType)
struct FINGCORE_API FMingCoreEventTriggerCondition
{
    GENERATED_BODY()

    // ����ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConditionID;

    // ������X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventTriggerType TriggerType;

    // ����X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionDescription;

    // ����X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> ConditionParameters;

    // �����vX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConditionWeight;

    // X�_X�nX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    FMingEventTriggerCondition()
    {
        ConditionID = -1;
        TriggerType = EMingEventTriggerType::ConditionBased;
        ConditionDescription = TEXT(""};
        ConditionParameters.Empty(};
        ConditionWeight = 1.0f;
        bIsRequired = true;
    }
};

// �ƥ�X
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventOption
{
    GENERATED_BODY()

    // XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionTitle;

    // X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    // X�m����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Prerequisites;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessProbability;

    // X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Costs;

    // X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Benefits;

    FMingEventOption()
    {
        OptionID = -1;
        OptionTitle = TEXT(""};
        OptionDescription = TEXT(""};
        Consequences.Empty(};
        Prerequisites.Empty(};
        SuccessProbability = 1.0f;
        Costs.Empty(};
        Benefits.Empty(};
    }
};

// ���v�ƥ�X
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalEvent
{
    GENERATED_BODY()

    // �ƥ�ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventID;

    // �ƥ�X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventName;

    // �ƥ�X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventDescription;

    // ���vX��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // �ƥ���X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventType EventType;

    // �ƥ�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventImportance EventImportance;

    // �ƥ�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEventStatus EventStatus;

    // X�~��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventYear;

    // X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventMonth;

    // X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventLocation;

    // Ĳ�o����X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingCoreEventTriggerCondition> TriggerConditions;

    // �ƥ�X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingEventOption> EventOptions;

    // �ƥ�X�]X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DurationMonths;

    // �v�T�dX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffectedRegions;

    // X�H��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> InvolvedCharacters;

    // X�ƥ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ChainEvents;

    // �ƥ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EventTags;

    FMingHistoricalEvent()
    {
        EventID = -1;
        EventName = TEXT(""};
        EventDescription = TEXT(""};
        HistoricalBackground = TEXT(""};
        EventType = EMingEventType::Political;
        EventImportance = EMingEventImportance::Moderate;
        EventStatus = EMingEventStatus::Pending;
        EventYear = 1920;
        EventMonth = 1;
        EventLocation = TEXT(""};
        TriggerConditions.Empty(};
        EventOptions.Empty(};
        DurationMonths = 1;
        AffectedRegions.Empty(};
        InvolvedCharacters.Empty(};
        ChainEvents.Empty(};
        EventTags.Empty(};
    }
};

// �ƥ�X
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventResult
{
    GENERATED_BODY()

    // �ƥ�ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EventID;

    // X��XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // �ƥ�X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    // �ƥ�X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;

    // �ƥ�X�_
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    // ��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ActualOutcomes;

    // �v�TX�MX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> AffectedDecisions;

    // �v�TX�HX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> AffectedCharacters;

    // X�s�ƥ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedEvents;

    // ���v��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> HistoricalPathChanges;

    FMingEventResult()
    {
        EventID = -1;
        ChosenOptionID = -1;
        StartTime = FDateTime::Now(};
        EndTime = FDateTime::Now(};
        bSuccess = false;
        ActualOutcomes.Empty(};
        AffectedDecisions.Empty(};
        AffectedCharacters.Empty(};
        UnlockedEvents.Empty(};
        HistoricalPathChanges.Empty(};
    }
};

// �ƥ��X
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEventStatistics
{
    GENERATED_BODY()

    // �`X���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalEvents;

    // �wĲX���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggeredEvents;

    // �w�ѨMX���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResolvedEvents;

    // ��X�ƥ�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FailedEvents;

    // X���X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingEventType, int32> EventTypeStats;

    // X�n��X���X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingEventImportance, int32> EventImportanceStats;

    FMingEventStatistics()
    {
        TotalEvents = 0;
        TriggeredEvents = 0;
        ResolvedEvents = 0;
        FailedEvents = 0;
        EventTypeStats.Empty(};
        EventImportanceStats.Empty(};
    }
};

// �ƥ�Ĳ�o�eX
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventTriggered, const FMingHistoricalEvent&, Event};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEventOptionChosen, int32, EventID, int32, OptionID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventResolved, const FMingEventResult&, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventChainTriggered, int32, ChainEventID};

/**
 * ���v�ƥ�Ĳ�oX * �t�d��XĲX��X */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalEventTrigger : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEventTrigger(};

    // X��tX    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool InitializeEventSystem(};

    // ��X���v�ƥ�
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event) {};

    // �ˬd�ƥ�Ĳ�o����
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool CheckEventTriggerConditions(int32 EventID};

    // Ĳ�o���v�ƥ�
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool TriggerHistoricalEvent(int32 EventID};

    // X�ƥ�X
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool ProcessEventChoice(int32 EventID, int32 OptionID};

    // X�ƥ�H��
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    FMingHistoricalEvent GetEventInfo(int32 EventID) const;

    // X��X�ƥ�
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<int32> GetActiveEvents() const;

    // X�~��X    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<int32> GetEventsForYear(int32 Year) const;

    // X�ƥ���v
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    TArray<FMingEventResult> GetEventHistory() const;

    // X�s�ƥ�X    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool UpdateEventStatus(int32 EventID, EMingEventStatus NewStatus};

    // �ˬdX�ƥ�
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    void CheckChainEvents(int32 EventID, const FMingEventResult& Result) {};

    // �����ƥ�Ĳ�o
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    TArray<int32> SimulateEventTriggers(int32 CurrentYear, int32 CurrentMonth};

    // X�ƥ��X
    UFUNCTION(BlueprintPure, Category = "Historical Event")
    FMingEventStatistics GetEventStatistics() const;

    // �OX�ƥ�X
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool SaveEventData(};

    // ���J�ƥ�X
    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    bool LoadEventData(};

    // �M��X���X    UFUNCTION(BlueprintCallable, Category = "Historical Event")
    void ClearAllEventData(};

    // �ƥ�eX
    UPROPERTY(BlueprintAssignable)
    FOnEventTriggered OnEventTriggered;

    UPROPERTY(BlueprintAssignable)
    FOnEventOptionChosen OnEventOptionChosen;

    UPROPERTY(BlueprintAssignable)
    FOnEventResolved OnEventResolved;

    UPROPERTY(BlueprintAssignable)
    FOnEventChainTriggered OnEventChainTriggered;

protected:
    // �ƥ�X    UPROPERTY()
    TMap<int32, FMingHistoricalEvent> EventDatabase;

    // �ƥ���v�OX
    UPROPERTY()
    TArray<FMingEventResult> EventHistory;

    // ��X�ƥ�X��
    UPROPERTY()
    TArray<int32> ActiveEvents;

    // X��X
    UPROPERTY()
    int32 CurrentGameYear;

    UPROPERTY()
    int32 CurrentGameMonth;

    // X�_�wX�lX
    UPROPERTY()
    bool bInitialized;

private:
    // ���JX�]�ƥ�X
    void LoadDefaultEvents(};

    // ��X�ƥ�X
    bool ValidateEventData(const FMingHistoricalEvent& Event) const;

    // �ˬdXĲ�o����
    bool CheckTimeTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // �ˬd�MXĲ�o����
    bool CheckDecisionTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // �ˬd����Ĳ�o
    bool CheckConditionTrigger(const FMingCoreEventTriggerCondition& Condition) const;

    // X�ƥ��X
    void ProcessEventConsequences(int32 EventID, int32 OptionID};

    // X�ƥ�
    void UnlockChainEvents(int32 EventID};

    // X�s���vX
    void UpdateHistoricalProgress(const FMingEventResult& Result) {};

    // �p��ƥ󦨥\�v
    float CalculateEventSuccessRate(int32 EventID, int32 OptionID) const;

    // �ͦ��ʺA�ƥ�
    FMingHistoricalEvent GenerateDynamicEvent(const FString& Context) const;
};

