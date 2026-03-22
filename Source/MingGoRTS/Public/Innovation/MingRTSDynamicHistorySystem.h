#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicHistorySystem.generated.h"

UENUM(BlueprintType)
enum class EHistoricalEventType : uint8
{
    Political,       // ?�治事件
    Military,        // 軍�?事件
    Economic,        // 經�?事件
    Cultural,        // ?��?事件
    Social,          // 社�?事件
    Technological,   // ?�術�?�?    Natural,         // ?�然事件
    Personal         // ?�人事件
};

UENUM(BlueprintType)
enum class EHistoricalImpact : uint8
{
    Local,           // ?�地影響
    Regional,        // ?��?影響
    National,        // ?��?影響
    International,   // ?��?影響
    Global           // ?��?影響
};

UENUM(BlueprintType)
enum class EHistoricalSignificance : uint8
{
    Trivial,         // ?��?
    Minor,           // 次�?
    Moderate,        // 中�?
    Major,           // 主�?
    Critical,        // ?�鍵
    Legendary        // ?��?
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
        EventID = TEXT(""};
        EventName = TEXT(""};
        Description = TEXT(""};
        EventType = EHistoricalEventType::Political;
        EventImpact = EHistoricalImpact::Local;
        Significance = EHistoricalSignificance::Moderate;
        Location = TEXT(""};
        bPlayerInfluenced = false;
        PlayerAction = TEXT(""};
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
        TimelineID = TEXT(""};
        TimelineName = TEXT(""};
        CurrentEra = TEXT(""};
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
        BranchID = TEXT(""};
        BranchName = TEXT(""};
        DivergencePoint = TEXT(""};
        Probability = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHistoricalEvent, const FHistoricalEvent&, Event, bool, bPlayerInfluenced};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimelineBranch, const FString&, BranchID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEraChange, const FString&, NewEra};

/**
 * ?��?歷史演進系�?- ?��X��?歷史事件?��X�系�? * ?��X�於?�家行為?��X�歷?��X��?多�X��?線支X */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSDynamicHistorySystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSDynamicHistorySystem(};

    // 系統?��X    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void InitializeDynamicHistorySystem(};

    // ?�建歷史?��?�?    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalTimeline CreateTimeline(const FString& TimelineID, const FString& TimelineName, int32 StartYear};

    // ?��?歷史事件
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalEvent GenerateHistoricalEvent(const FString& TimelineID, EHistoricalEventType EventType, const FString& Context};

    // ?��X�家行為影響
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void ProcessPlayerAction(const FString& PlayerID, const FString& Action, const FString& Context};

    // ?�建歷史?�支
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalBranch CreateHistoricalBranch(const FString& TimelineID, const FString& DivergenceEvent};

    // 演進�X��?
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void AdvanceTimeline(const FString& TimelineID, int32 Years};

    // ?��X��X��?�?    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FHistoricalTimeline GetCurrentTimeline(const FString& TimelineID) const;

    // ?��?歷史事件
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TArray<FHistoricalEvent> GetHistoricalEvents(const FString& TimelineID, EHistoricalEventType EventType) const;

    // ?��?潛在事件
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TArray<FHistoricalEvent> GetPotentialEvents(const FString& TimelineID, const FString& Context) const;

    // 計�?歷史影響
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    float CalculateHistoricalImpact(const FHistoricalEvent& Event) const;

    // ?��X��X�容
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    FString GenerateNarrativeContent(const FHistoricalEvent& Event) const;

    // ?�測?��?事件
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TArray<FHistoricalEvent> PredictFutureEvents(const FString& TimelineID, int32 YearsAhead) const;

    // 設置歷史?�數
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void SetHistoricalParameters(float EventGenerationRate, float PlayerInfluenceWeight, float BranchProbability};

    // ?��?歷史統�?
    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    TMap<FString, float> GetHistoricalStatistics(const FString& TimelineID) const;

    // ?�置?��?�?    UFUNCTION(BlueprintCallable, Category = "Dynamic History")
    void ResetTimeline(const FString& TimelineID};

    // 事件委�?
    UPROPERTY(BlueprintAssignable)
    FOnHistoricalEvent OnHistoricalEvent;

    UPROPERTY(BlueprintAssignable)
    FOnTimelineBranch OnTimelineBranch;

    UPROPERTY(BlueprintAssignable)
    FOnEraChange OnEraChange;

protected:
    // 歷史?�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    float EventGenerationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    float PlayerInfluenceWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    float BranchProbability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Parameters")
    int32 MaxEventsPerYear;

    // ?��?線�X    UPROPERTY(BlueprintReadOnly, Category = "History Data")
    TMap<FString, FHistoricalTimeline> Timelines;

    // 歷史?�支存儲
    UPROPERTY(BlueprintReadOnly, Category = "History Data")
    TMap<FString, TArray<FHistoricalBranch>> TimelineBranches;

    // 事件模板
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Templates")
    TArray<FHistoricalEvent> EventTemplates;

    // ?�代定義
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "History Data")
    TMap<int32, FString> EraDefinitions;

    // 世�X�X    UPROPERTY(BlueprintReadOnly, Category = "History Data")
    TMap<FString, TMap<FString, float>> WorldStates;

private:
    // ?�部事件?��X�輯
    FHistoricalEvent GeneratePoliticalEvent(const FString& Context};
    FHistoricalEvent GenerateMilitaryEvent(const FString& Context};
    FHistoricalEvent GenerateEconomicEvent(const FString& Context};
    FHistoricalEvent GenerateCulturalEvent(const FString& Context};
    FHistoricalEvent GenerateSocialEvent(const FString& Context};
    FHistoricalEvent GenerateTechnologicalEvent(const FString& Context};
    FHistoricalEvent GenerateNaturalEvent(const FString& Context};
    FHistoricalEvent GeneratePersonalEvent(const FString& Context};
    
    // 歷史演進�?�?    void UpdateWorldState(const FString& TimelineID, const FHistoricalEvent& Event};
    void CheckEraProgression(const FString& TimelineID};
    void EvaluateBranchConditions(const FString& TimelineID};
    
    // 輔助?�數
    FString GenerateEventID() const;
    float CalculateEventProbability(const FHistoricalEvent& Event, const FString& TimelineID) const;
    FString GetCurrentEra(int32 Year) const;
    TArray<FString> GetActiveFactions(const FString& TimelineID) const;
    void RecordHistoricalEvent(const FString& TimelineID, const FHistoricalEvent& Event};
    
    // ?��X��?
    FString GenerateEventNarrative(const FHistoricalEvent& Event) const;
    FString GenerateConsequenceNarrative(const TArray<FString>& Consequences) const;
    FString GenerateCharacterNarrative(const TArray<FString>& KeyFigures) const;
};

