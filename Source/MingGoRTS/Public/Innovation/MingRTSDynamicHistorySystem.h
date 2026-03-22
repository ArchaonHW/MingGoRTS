#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicHistorySystem.generated.h"

UENUM(BlueprintType)
enum class EHistoricalEventType : uint8
{
    Political,       // ?øÊ≤ª‰∫ã‰ª∂
    Military,        // Ëªç‰?‰∫ã‰ª∂
    Economic,        // Á∂ìÊ?‰∫ã‰ª∂
    Cultural,        // ?áÂ?‰∫ã‰ª∂
    Social,          // Á§æÊ?‰∫ã‰ª∂
    Technological,   // ?ÄË°ì‰?‰ª?    Natural,         // ?™ÁÑ∂‰∫ã‰ª∂
    Personal         // ?ã‰∫∫‰∫ã‰ª∂
};

UENUM(BlueprintType)
enum class EHistoricalImpact : uint8
{
    Local,           // ?¨Âú∞ÂΩ±Èüø
    Regional,        // ?∞Â?ÂΩ±Èüø
    National,        // ?®Â?ÂΩ±Èüø
    International,   // ?ãÈ?ÂΩ±Èüø
    Global           // ?®Á?ÂΩ±Èüø
};

UENUM(BlueprintType)
enum class EHistoricalSignificance : uint8
{
    Trivial,         // ?êÁ?
    Minor,           // Ê¨°Ë?
    Moderate,        // ‰∏≠Á?
    Major,           // ‰∏ªË?
    Critical,        // ?úÈçµ
    Legendary        // ?≥Â?
};

USTRUCT(BlueprintType)
struct FHistoricalEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHistoricalEventType EventType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHistoricalImpact EventImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EHistoricalSignificance Significance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EventDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> KeyFigures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Consequences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RelatedEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> WorldStateChanges;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPlayerInfluenced;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerAction;

    FHistoricalEvent()
    {
        EventID = TEXT("");
        EventName = TEXT("");
        Description = TEXT("");
        EventType = EHistoricalEventType::Political;
        EventImpact = EHistoricalImpact::Local;
        Significance = EHistoricalSignificance::Moderate;
        Location = TEXT("");
        bPlayerInfluenced = false;
        PlayerAction = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FHistoricalTimeline
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TimelineID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TimelineName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FHistoricalEvent> Events;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> WorldState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ActiveFactions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CurrentEra;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentYear;

    FHistoricalTimeline()
    {
        TimelineID = TEXT("");
        TimelineName = TEXT("");
        CurrentEra = TEXT("");
        CurrentYear = 1911;
    }
};

USTRUCT(BlueprintType)
struct FHistoricalBranch
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BranchID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BranchName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DivergencePoint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FHistoricalEvent> BranchEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> BranchWorldState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Probability;

    FHistoricalBranch()
    {
        BranchID = TEXT("");
        BranchName = TEXT("");
        DivergencePoint = TEXT("");
        Probability = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHistoricalEvent, const FHistoricalEvent&, Event, bool, bPlayerInfluenced);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineBranch, const FString&, BranchID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEraChange, const FString&, NewEra);

/**
 * ?ïÊ?Ê≠∑Âè≤ÊºîÈÄ≤Á≥ªÁµ?- ?ïÊ??üÊ?Ê≠∑Âè≤‰∫ã‰ª∂?åÂ??ÖÁ≥ªÁµ? * ?ê‰??∫Êñº?©ÂÆ∂Ë°åÁÇ∫?ÑÂ??ãÊ≠∑?≤Ê??≤Â?Â§öÈ??ÇÈ?Á∑öÊîØ?? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSDynamicHistorySystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSDynamicHistorySystem();

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void InitializeDynamicHistorySystem();

    // ?µÂª∫Ê≠∑Âè≤?ÇÈ?Á∑?    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalTimeline CreateTimeline(const FString& TimelineID, const FString& TimelineName, int32 StartYear);

    // ?üÊ?Ê≠∑Âè≤‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalEvent GenerateHistoricalEvent(const FString& TimelineID, EHistoricalEventType EventType, const FString& Context);

    // ?ïÁ??©ÂÆ∂Ë°åÁÇ∫ÂΩ±Èüø
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void ProcessPlayerAction(const FString& PlayerID, const FString& Action, const FString& Context);

    // ?µÂª∫Ê≠∑Âè≤?ÜÊîØ
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalBranch CreateHistoricalBranch(const FString& TimelineID, const FString& DivergenceEvent);

    // ÊºîÈÄ≤Ê??ìÁ?
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void AdvanceTimeline(const FString& TimelineID, int32 Years);

    // ?≤Â??∂Â??ÇÈ?Á∑?    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalTimeline GetCurrentTimeline(const FString& TimelineID) const;

    // ?≤Â?Ê≠∑Âè≤‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TArray<FHistoricalEvent> GetHistoricalEvents(const FString& TimelineID, EHistoricalEventType EventType) const;

    // ?≤Â?ÊΩõÂú®‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TArray<FHistoricalEvent> GetPotentialEvents(const FString& TimelineID, const FString& Context) const;

    // Ë®àÁ?Ê≠∑Âè≤ÂΩ±Èüø
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    float CalculateHistoricalImpact(const FHistoricalEvent& Event) const;

    // ?üÊ??áÊ??ßÂÆπ
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FString GenerateNarrativeContent(const FHistoricalEvent& Event) const;

    // ?êÊ∏¨?™‰?‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TArray<FHistoricalEvent> PredictFutureEvents(const FString& TimelineID, int32 YearsAhead) const;

    // Ë®≠ÁΩÆÊ≠∑Âè≤?ÉÊï∏
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void SetHistoricalParameters(float EventGenerationRate, float PlayerInfluenceWeight, float BranchProbability);

    // ?≤Â?Ê≠∑Âè≤Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TMap<FString, float> GetHistoricalStatistics(const FString& TimelineID) const;

    // ?çÁΩÆ?ÇÈ?Á∑?    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void ResetTimeline(const FString& TimelineID);

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnHistoricalEvent OnHistoricalEvent;

    UPROPERTY(BlueprintAssignable)
    FOnTimelineBranch OnTimelineBranch;

    UPROPERTY(BlueprintAssignable)
    FOnEraChange OnEraChange;

protected:
    // Ê≠∑Âè≤?ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    float EventGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    float PlayerInfluenceWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    float BranchProbability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    int32 MaxEventsPerYear;

    // ?ÇÈ?Á∑öÂ???    UPROPERTY(BlueprintReadOnly, Category = "History Data")
    TMap<FString, FHistoricalTimeline> Timelines;

    // Ê≠∑Âè≤?ÜÊîØÂ≠òÂÑ≤
    UPROPERTY(BlueprintReadOnly, Category = "History Data")
    TMap<FString, TArray<FHistoricalBranch>> TimelineBranches;

    // ‰∫ã‰ª∂Ê®°Êùø
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Templates")
    TArray<FHistoricalEvent> EventTemplates;

    // ?Ç‰ª£ÂÆöÁæ©
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Data")
    TMap<int32, FString> EraDefinitions;

    // ‰∏ñÁ??Ä??    UPROPERTY(BlueprintReadOnly, Category = "History Data")
    TMap<FString, TMap<FString, float>> WorldStates;

private:
    // ?ßÈÉ®‰∫ã‰ª∂?üÊ??èËºØ
    FHistoricalEvent GeneratePoliticalEvent(const FString& Context);
    FHistoricalEvent GenerateMilitaryEvent(const FString& Context);
    FHistoricalEvent GenerateEconomicEvent(const FString& Context);
    FHistoricalEvent GenerateCulturalEvent(const FString& Context);
    FHistoricalEvent GenerateSocialEvent(const FString& Context);
    FHistoricalEvent GenerateTechnologicalEvent(const FString& Context);
    FHistoricalEvent GenerateNaturalEvent(const FString& Context);
    FHistoricalEvent GeneratePersonalEvent(const FString& Context);
    
    // Ê≠∑Âè≤ÊºîÈÄ≤È?Ëº?    void UpdateWorldState(const FString& TimelineID, const FHistoricalEvent& Event);
    void CheckEraProgression(const FString& TimelineID);
    void EvaluateBranchConditions(const FString& TimelineID);
    
    // ËºîÂä©?ΩÊï∏
    FString GenerateEventID() const;
    float CalculateEventProbability(const FHistoricalEvent& Event, const FString& TimelineID) const;
    FString GetCurrentEra(int32 Year) const;
    TArray<FString> GetActiveFactions(const FString& TimelineID) const;
    void RecordHistoricalEvent(const FString& TimelineID, const FHistoricalEvent& Event);
    
    // ?áÊ??üÊ?
    FString GenerateEventNarrative(const FHistoricalEvent& Event) const;
    FString GenerateConsequenceNarrative(const TArray<FString>& Consequences) const;
    FString GenerateCharacterNarrative(const TArray<FString>& KeyFigures) const;
};
