#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSHistoricalRoleplay.h"
#include "MingGoRTSHistoricalSimulation.generated.h"

UENUM(BlueprintType)
enum class ESimulationType : uint8
{
    Political         UMETA(DisplayName = "?øÊ≤ªÊ®°Êì¨"),
    Military          UMETA(DisplayName = "Ëªç‰?Ê®°Êì¨"),
    Economic          UMETA(DisplayName = "Á∂ìÊ?Ê®°Êì¨"),
    Social            UMETA(DisplayName = "Á§æÊ?Ê®°Êì¨"),
    Cultural          UMETA(DisplayName = "?áÂ?Ê®°Êì¨"),
    International     UMETA(DisplayName = "?ãÈ?Ê®°Êì¨")
};

UENUM(BlueprintType)
enum class ESimulationPhase : uint8
{
    Setup             UMETA(DisplayName = "Ë®≠ÁΩÆ?éÊÆµ"),
    Execution         UMETA(DisplayName = "?∑Ë??éÊÆµ"),
    Analysis          UMETA(DisplayName = "?ÜÊ??éÊÆµ"),
    Results           UMETA(DisplayName = "ÁµêÊ??éÊÆµ")
};

USTRUCT(BlueprintType)
struct FSimulationParameter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Parameter")
    FString ParameterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Parameter")
    float Value;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Parameter")
    float MinValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Parameter")
    float MaxValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Parameter")
    FString Unit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Parameter")
    FString Description;

    FSimulationParameter()
    {
        ParameterName = TEXT(""};
        Value = 0.0f;
        MinValue = 0.0f;
        MaxValue = 100.0f;
        Unit = TEXT(""};
        Description = TEXT(""};
    }
};

UENUM(BlueprintType)
enum class EConsequenceOperation : uint8
{
    Add         UMETA(DisplayName = "?†Ê?"),
    Subtract    UMETA(DisplayName = "Ê∏õÊ?"),
    Multiply    UMETA(DisplayName = "‰πòÊ?"),
    Divide      UMETA(DisplayName = "?§Ê?"),
    Set         UMETA(DisplayName = "Ë®≠ÁΩÆ"),
    Reset       UMETA(DisplayName = "?çÁΩÆ")
};

USTRUCT(BlueprintType)
struct FMingEventConsequence
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Consequence")
    FString TargetParameter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Consequence")
    EConsequenceOperation Operation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Consequence")
    float ValueChange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Consequence")
    bool bIsPercentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Consequence")
    float DelaySeconds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Consequence")
    TArray<FString> ConditionParameters;

    FMingEventConsequence()
    {
        TargetParameter = TEXT(""};
        Operation = EConsequenceOperation::Add;
        ValueChange = 0.0f;
        bIsPercentage = false;
        DelaySeconds = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSimulationEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Event")
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Event")
    FString EventName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Event")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Event")
    float TriggerProbability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Event")
    TArray<FSimulationParameter> RequiredParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Event")
    TArray<FMingEventConsequence> Consequences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Event")
    FString TimeDelay;

    FSimulationEvent()
    {
        EventID = TEXT(""};
        EventName = TEXT(""};
        Description = TEXT(""};
        TriggerProbability = 0.0f;
        TimeDelay = TEXT(""};
    }
};

USTRUCT(BlueprintType)
struct FHistoricalSimulation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    FString SimulationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    ESimulationType SimulationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    FString HistoricalPeriod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    FString GeographicScope;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    TArray<FSimulationParameter> InitialParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    TArray<FSimulationParameter> VariableParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    TArray<FSimulationEvent> PossibleEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    TArray<FString> SuccessConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    TArray<FString> FailureConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    FString HistoricalBaseline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    float SimulationDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    int32 MaxIterations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    bool bIsRealTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Simulation")
    bool bCanIntervene;

    FHistoricalSimulation()
    {
        SimulationID = TEXT(""};
        Title = TEXT(""};
        Description = TEXT(""};
        SimulationType = ESimulationType::Political;
        HistoricalPeriod = TEXT(""};
        GeographicScope = TEXT(""};
        HistoricalBaseline = TEXT(""};
        SimulationDuration = 0.0f;
        MaxIterations = 100;
        bIsRealTime = false;
        bCanIntervene = false;
    }
};

USTRUCT(BlueprintType)
struct FSimulationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    FString ResultID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    FString SimulationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    float FinalScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    TArray<FSimulationParameter> FinalParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    TArray<FString> TriggeredEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    TArray<FString> KeyDecisions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    FString OutcomeDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    float HistoricalAccuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    FString PlayerImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Result")
    FDateTime EndTime;

    FSimulationResult()
    {
        ResultID = TEXT(""};
        SimulationID = TEXT(""};
        bSuccess = false;
        FinalScore = 0.0f;
        OutcomeDescription = TEXT(""};
        HistoricalAccuracy = 0.0f;
        PlayerImpact = TEXT(""};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSimulationStarted, const FString&, SimulationID, ESimulationType, Type};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSimulationEventTriggered, const FString&, SimulationID, const FString&, EventID, const FString&, EventName};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSimulationCompleted, const FString&, SimulationID, const FSimulationResult&, Result};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalSimulation : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalSimulation(};

    // Ê®°Êì¨Á≥ªÁµ±‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationStarted OnSimulationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationEventTriggered OnSimulationEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationCompleted OnSimulationCompleted;

    // ?ùÂ??ñÊ®°?¨Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    void InitializeSimulationSystem(};

    // ?≤Â??Ä?âÊ®°??    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FHistoricalSimulation> GetAllSimulations() const;

    // ?≤Â?Ê®°Êì¨
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FHistoricalSimulation GetSimulation(const FString& SimulationID) const;

    // ?ãÂ?Ê®°Êì¨
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StartSimulation(const FString& SimulationID, const TMap<FString, float>& PlayerDecisions};

    // ?´Â?Ê®°Êì¨
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool PauseSimulation(const FString& SimulationID};

    // ÁπºÁ?Ê®°Êì¨
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool ResumeSimulation(const FString& SimulationID};

    // ?úÊ≠¢Ê®°Êì¨
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StopSimulation(const FString& SimulationID};

    // Âπ≤È?Ê®°Êì¨
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool InterveneInSimulation(const FString& SimulationID, const FString& ParameterName, float NewValue};

    // ?≤Â?Ê®°Êì¨?Ä??    UFUNCTION(BlueprintPure, Category = "Simulation System")
    ESimulationPhase GetSimulationPhase(const FString& SimulationID) const;

    // ?≤Â??∂Â??ÉÊï∏
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FSimulationParameter> GetCurrentParameters(const FString& SimulationID) const;

    // ?≤Â?Ê®°Êì¨?≤Â∫¶
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float GetSimulationProgress(const FString& SimulationID) const;

    // ?≤Â?Ê®°Êì¨ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FSimulationResult GetSimulationResult(const FString& SimulationID) const;

    // ?≤Â?Ê®°Êì¨Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FString> GetSimulationHistory(const FString& SimulationID) const;

    // Ë®àÁ?Ê®°Êì¨?êÊ∏¨
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<float> PredictSimulationOutcome(const FString& SimulationID, const TMap<FString, float>& ParameterChanges) const;

    // ÊØîË?Ê®°Êì¨ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float CompareWithHistoricalBaseline(const FString& SimulationID) const;

    // ?≤Â?Ê®°Êì¨Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TMap<FString, float> GetSimulationStatistics(const FString& SimulationID) const;

    // ‰øùÂ?Ê®°Êì¨?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool SaveSimulationData(const FString& SaveSlotName};

    // ËºâÂÖ•Ê®°Êì¨?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool LoadSimulationData(const FString& SaveSlotName};

protected:
    // ?Ä?âÊ®°??    UPROPERTY()
    TArray<FHistoricalSimulation> AllSimulations;

    // Ê®°Êì¨ID?∞Ê®°?¨Á??†Â?
    UPROPERTY()
    TMap<FString, FHistoricalSimulation> SimulationMap;

    // Ê¥ªÂ?Ê®°Êì¨?Ä??    UPROPERTY()
    TMap<FString, ESimulationPhase> ActiveSimulations;

    // Ê®°Êì¨?ÉÊï∏ - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<FString, TArray<FSimulationParameter>> SimulationParameters;

    // ?ÉÊï∏Âø´Â? - Ê≥®Ê?ÔºöÂ?Â•?TMap ‰∏çÊîØ??UPROPERTY
    TMap<FString, TMap<FString, int32>> ParameterCache;

    // Ê®°Êì¨ÁµêÊ?
    UPROPERTY()
    TMap<FString, FSimulationResult> SimulationResults;

    // Ê®°Êì¨Ê≠∑Âè≤
    UPROPERTY()
    TMap<FString, FStringArrayWrapper> SimulationHistories;

    // ‰∫ã‰ª∂Ëß∏Áôº?≤Ë≠∑ - ?≤Ê≠¢Âæ™Áí∞Ëß∏Áôº
    UPROPERTY()
    TMap<FString, int32> EventTriggerDepth;

    // ?ÄÂ§ß‰?‰ª∂Ëß∏?ºÊ∑±Â∫?(?≤Ê≠¢?°È?Âæ™Áí∞)
    int32 MaxEventTriggerDepth = 10;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ®°?¨Â∫´
    void InitializeSimulationLibrary(};

    // ?µÂª∫?øÊ≤ªÊ®°Êì¨
    void CreatePoliticalSimulations(};

    // ?µÂª∫Ëªç‰?Ê®°Êì¨
    void CreateMilitarySimulations(};

    // ?µÂª∫Á∂ìÊ?Ê®°Êì¨
    void CreateEconomicSimulations(};

    // ?µÂª∫Á§æÊ?Ê®°Êì¨
    void CreateSocialSimulations(};

    // ?µÂª∫?áÂ?Ê®°Êì¨
    void CreateCulturalSimulations(};

    // ?µÂª∫?ãÈ?Ê®°Êì¨
    void CreateInternationalSimulations(};

    // ?∑Ë?Ê®°Êì¨Ê≠•È?
    void ExecuteSimulationStep(const FString& SimulationID};

    // ?ïÁ?Ê®°Êì¨‰∫ã‰ª∂
    void ProcessSimulationEvents(const FString& SimulationID};

    // Ëß∏ÁôºÊ®°Êì¨‰∫ã‰ª∂
    void TriggerSimulationEvent(const FString& SimulationID, const FSimulationEvent& Event};

    // Ë®àÁ??ÉÊï∏ËÆäÂ?
    void CalculateParameterChanges(const FString& SimulationID};

    // Ê™¢Êü•?êÂ?Ê¢ù‰ª∂
    bool CheckSuccessConditions(const FString& SimulationID};

    // Ê™¢Êü•Â§±Ê?Ê¢ù‰ª∂
    bool CheckFailureConditions(const FString& SimulationID};

    // ?üÊ?Ê®°Êì¨ÁµêÊ?
    void GenerateSimulationResult(const FString& SimulationID};

    // Ë®àÁ?Ê≠∑Âè≤Ê∫ñÁ¢∫??    float CalculateHistoricalAccuracy(const FString& SimulationID};

    // Ë®àÁ??©ÂÆ∂ÂΩ±Èüø
    float CalculatePlayerImpact(const FString& SimulationID};

    // ?¥Êñ∞Ê®°Êì¨?≤Â∫¶
    void UpdateSimulationProgress(const FString& SimulationID};

    // Ë®òÈ?Ê®°Êì¨‰∫ã‰ª∂
    void RecordSimulationEvent(const FString& SimulationID, const FString& EventName};

    // ?≤Â??ÉÊï∏??(?™Â?Âæå‰Ωø?®Âø´??
    float GetParameterValue(const FString& SimulationID, const FString& ParameterName) const;

    // Ë®≠ÁΩÆ?ÉÊï∏??(?™Â?ÂæåÊõ¥?∞Âø´??
    void SetParameterValue(const FString& SimulationID, const FString& ParameterName, float Value};

    // ÊßãÂª∫?ÉÊï∏Âø´Â?
    void BuildParameterCache(const FString& SimulationID};

    // ?üÊ?Ê®°Êì¨ID
    FString GenerateSimulationID(const FString& BaseName, ESimulationType Type) const;

    // ?≤Â?Ê®°Êì¨È°ûÂ??çÁ∂¥
    FString GetSimulationTypePrefix(ESimulationType Type) const;

    // È©óË?Ê®°Êì¨?ÉÊï∏
    bool ValidateSimulationParameters(const FString& SimulationID, const TMap<FString, float>& Parameters) const;

    // Ë®àÁ?‰∫ã‰ª∂Ëß∏ÁôºÊ¶ÇÁ?
    float CalculateEventTriggerProbability(const FString& SimulationID, const FSimulationEvent& Event) const;

    // ?âÁî®‰∫ã‰ª∂ÂæåÊ?
    void ApplyEventConsequences(const FString& SimulationID, const FSimulationEvent& Event};

    // ?üÊ?Ê®°Êì¨?±Â?
    FString GenerateSimulationReport(const FString& SimulationID) const;
};

