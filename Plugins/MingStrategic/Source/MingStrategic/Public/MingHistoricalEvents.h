#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEvents.generated.h"

UENUM(BlueprintType)
enum class EMingHistoricalEra : uint8
{
    LateQing UMETA(DisplayName = "Late Qing Dynasty"),
    Republic UMETA(DisplayName = "Republic Era"),
    Warlord UMETA(DisplayName = "Warlord Era"),
    NanjingDecade UMETA(DisplayName = "Nanjing Decade"),
    SecondSinoJapanese UMETA(DisplayName = "Second Sino-Japanese War"),
    CivilWar UMETA(DisplayName = "Chinese Civil War")
};

UENUM(BlueprintType)
enum class EMingHistoricalEventType : uint8
{
    Political UMETA(DisplayName = "Political Event"),
    Military UMETA(DisplayName = "Military Event"),
    Economic UMETA(DisplayName = "Economic Event"),
    Social UMETA(DisplayName = "Social Event"),
    Cultural UMETA(DisplayName = "Cultural Event"),
    Diplomatic UMETA(DisplayName = "Diplomatic Event")
};

USTRUCT(BlueprintType)
struct FMingHistoricalEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString HistoricalContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingHistoricalEra Era;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingHistoricalEventType EventType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Year;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Month;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Day;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> KeyFigures;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Consequences;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> TriggerConditions;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsMajorEvent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsPlayerInfluencable;

    UPROPERTY(BlueprintReadOnly)
    bool bHasBeenTriggered;

    UPROPERTY(BlueprintReadOnly)
    float TriggerTime;

    FMingHistoricalEvent()
        : Era(EMingHistoricalEra::Republic)
        , EventType(EMingHistoricalEventType::Political)
        , Year(1911)
        , Month(10)
        , Day(10)
        , Location(FVector::ZeroVector)
        , bIsMajorEvent(false)
        , bIsPlayerInfluencable(true)
        , bHasBeenTriggered(false)
        , TriggerTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingEventChoice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChoiceText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Requirements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Outcomes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InfluenceWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHistorical;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAvailable;

    FMingEventChoice()
        : InfluenceWeight(1.0f)
        , bIsHistorical(false)
        , bIsAvailable(true)
    {}
};

/**
 * Historical Events System for MingGoRTS
 * Manages historically accurate events and player choices
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGSTRATEGIC_API UMingHistoricalEvents : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEvents();

    // Event Management
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void InitializeHistoricalEvents();

    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void TriggerEvent(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void TriggerEventByConditions();

    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void CompleteEvent(const FString& EventID, const FString& ChoiceID);

    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetEventsByEra(EMingHistoricalEra Era) const;

    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetAvailableEvents() const;

    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetTriggeredEvents() const;

    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FMingHistoricalEvent GetEvent(const FString& EventID) const;

    // Event Choices
    UFUNCTION(BlueprintCallable, Category = "Event Choices")
    void PresentEventChoices(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "Event Choices")
    void SelectEventChoice(const FString& EventID, const FString& ChoiceID);

    UFUNCTION(BlueprintPure, Category = "Event Choices")
    TArray<FMingEventChoice> GetEventChoices(const FString& EventID) const;

    // Historical Context
    UFUNCTION(BlueprintCallable, Category = "Historical Context")
    void ShowHistoricalBackground(const FString& EventID);

    UFUNCTION(BlueprintCallable, Category = "Historical Context")
    void AddHistoricalNote(const FString& EventID, const FString& Note);

    UFUNCTION(BlueprintPure, Category = "Historical Context")
    TArray<FString> GetHistoricalNotes(const FString& EventID) const;

    // Player Influence
    UFUNCTION(BlueprintCallable, Category = "Player Influence")
    void SetPlayerInfluence(const FString& EventID, float Influence);

    UFUNCTION(BlueprintPure, Category = "Player Influence")
    float GetPlayerInfluence(const FString& EventID) const;

    UFUNCTION(BlueprintCallable, Category = "Player Influence")
    void ModifyEventOutcome(const FString& EventID, const FString& Modification);

    // Event Tracking
    UFUNCTION(BlueprintPure, Category = "Event Tracking")
    TMap<FString, FString> GetPlayerChoices() const;

    UFUNCTION(BlueprintPure, Category = "Event Tracking")
    TArray<FString> GetEventChain(const FString& EventID) const;

    UFUNCTION(BlueprintPure, Category = "Event Tracking")
    bool HasEventOccurred(const FString& EventID) const;

    UFUNCTION(BlueprintPure, Category = "Event Tracking")
    float GetHistoricalAccuracy() const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHistoricalEventTriggered, const FMingHistoricalEvent&, Event, const TArray<FMingEventChoice>&, Choices);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEventChoiceSelected, const FString&, EventID, const FString&, ChoiceID, const FString&, Outcome);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHistoricalContextShown, const FString&, EventID, const FString&, Context);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHistoricalEventTriggered OnHistoricalEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEventChoiceSelected OnEventChoiceSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHistoricalContextShown OnHistoricalContextShown;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetEraName(EMingHistoricalEra Era);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetEventTypeName(EMingHistoricalEventType EventType);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveHistoricalData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadHistoricalData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingHistoricalEvent> HistoricalEvents;

    // 注意：TArray 不能直接作為 TMap 的 UPROPERTY 值類型
    // TMap<FString, TArray<FMingEventChoice>> EventChoices;

    UPROPERTY()
    TMap<FString, float> PlayerInfluence;

    UPROPERTY()
    TMap<FString, FString> PlayerEventChoices;

    // 注意：TArray 不能直接作為 TMap 的 UPROPERTY 值類型
    // TMap<FString, TArray<FString>> HistoricalNotes;

    UPROPERTY()
    TSet<FString> TriggeredEvents;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Historical Events Setup
    void Setup1911RevolutionEvents();
    void SetupNorthernExpeditionEvents();
    void SetupSecondSinoJapaneseEvents();
    void SetupCivilWarEvents();

    // Internal Functions
    void ProcessEventConsequences(const FString& EventID, const FString& ChoiceID);
    void UpdateHistoricalAccuracy();
    void CheckEventDependencies(const FString& EventID);
    void RecordPlayerDecision(const FString& EventID, const FString& ChoiceID);

    // Helpers
    FMingHistoricalEvent* FindEvent(const FString& EventID);
    bool AreEventConditionsMet(const FMingHistoricalEvent& Event) const;
    void TriggerEventInternal(const FMingHistoricalEvent& Event);
};
