#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSHistoricalRoleplay.h"
#include "MingGoRTSHistoricalSimulation.generated.h"

UENUM(BlueprintType)
enum class ESimulationType : uint8
{
    Political         UMETA(DisplayName = "??獀家览"),
    Military          UMETA(DisplayName = "瓁??家览"),
    Economic          UMETA(DisplayName = "竒??家览"),
    Social            UMETA(DisplayName = "??家览"),
    Cultural          UMETA(DisplayName = "????家览"),
    International     UMETA(DisplayName = "????家览")
};

UENUM(BlueprintType)
enum class ESimulationPhase : uint8
{
    Setup             UMETA(DisplayName = "砞竚??琿"),
    Execution         UMETA(DisplayName = "???X?琿"),
    Analysis          UMETA(DisplayName = "???X?琿"),
    Results           UMETA(DisplayName = "挡?X?琿")
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
    Add         UMETA(DisplayName = "????"),
    Subtract    UMETA(DisplayName = "搭??"),
    Multiply    UMETA(DisplayName = "??"),
    Divide      UMETA(DisplayName = "????"),
    Set         UMETA(DisplayName = "砞竚"),
    Reset       UMETA(DisplayName = "??竚")
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
    UMingGoRTSHistoricalSimulation();

    // 家览╰参ㄆン
    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationStarted OnSimulationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationEventTriggered OnSimulationEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationCompleted OnSimulationCompleted;

    // ???X?家??╰??    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    void InitializeSimulationSystem();

    // ???X???家X    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FHistoricalSimulation> GetAllSimulations() const;

    // ????家览
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FHistoricalSimulation GetSimulation(const FString& SimulationID) const;

    // ????家览
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StartSimulation(const FString& SimulationID, const TMap<FString, float>& PlayerDecisions};

    // ????家览
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool PauseSimulation(const FString& SimulationID};

    // 膥??家览
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool ResumeSimulation(const FString& SimulationID};

    // ??ゎ家览
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StopSimulation(const FString& SimulationID};

    // ??家览
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool InterveneInSimulation(const FString& SimulationID, const FString& ParameterName, float NewValue};

    // ????家览??X    UFUNCTION(BlueprintPure, Category = "Simulation System")
    ESimulationPhase GetSimulationPhase(const FString& SimulationID) const;

    // ???X??X?计
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FSimulationParameter> GetCurrentParameters(const FString& SimulationID) const;

    // ????家览??
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float GetSimulationProgress(const FString& SimulationID) const;

    // ????家览挡??
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FSimulationResult GetSimulationResult(const FString& SimulationID) const;

    // ????家览菌
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FString> GetSimulationHistory(const FString& SimulationID) const;

    // 璸??家览??代
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<float> PredictSimulationOutcome(const FString& SimulationID, const TMap<FString, float>& ParameterChanges) const;

    // ゑ??家览挡??
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float CompareWithHistoricalBaseline(const FString& SimulationID) const;

    // ????家览参??
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TMap<FString, float> GetSimulationStatistics(const FString& SimulationID) const;

    // 玂??家览????
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool SaveSimulationData(const FString& SaveSlotName};

    // 更家览????
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool LoadSimulationData(const FString& SaveSlotName};

protected:
    // ????家X    UPROPERTY()
    TArray<FHistoricalSimulation> AllSimulations;

    // 家览ID??家???X???
    UPROPERTY()
    TMap<FString, FHistoricalSimulation> SimulationMap;

    // ??家览??X    UPROPERTY()
    TMap<FString, ESimulationPhase> ActiveSimulations;

    // 家览??计 - 猔??TMap<TArray> ぃやXUPROPERTY
    TMap<FString, TArray<FSimulationParameter>> SimulationParameters;

    // ??计е?? - 猔??????TMap ぃやXUPROPERTY
    TMap<FString, TMap<FString, int32>> ParameterCache;

    // 家览挡??
    UPROPERTY()
    TMap<FString, FSimulationResult> SimulationResults;

    // 家览菌
    UPROPERTY()
    TMap<FString, FStringArrayWrapper> SimulationHistories;

    // ㄆン牟祇??臔 - ??ゎ碻吏牟祇
    UPROPERTY()
    TMap<FString, int32> EventTriggerDepth;

    // ????ン牟??瞏??(??ゎ????碻吏)
    int32 MaxEventTriggerDepth = 10;

    // ????﹍??
    bool bIsInitialized;

    // ???X?家??畐
    void InitializeSimulationLibrary();

    // ????獀家览
    void CreatePoliticalSimulations();

    // ??瓁??家览
    void CreateMilitarySimulations();

    // ??竒??家览
    void CreateEconomicSimulations();

    // ????家览
    void CreateSocialSimulations();

    // ??????家览
    void CreateCulturalSimulations();

    // ??????家览
    void CreateInternationalSimulations();

    // ????家览˙??
    void ExecuteSimulationStep(const FString& SimulationID};

    // ????家览ㄆン
    void ProcessSimulationEvents(const FString& SimulationID};

    // 牟祇家览ㄆン
    void TriggerSimulationEvent(const FString& SimulationID, const FSimulationEvent& Event};

    // 璸?X?计跑??
    void CalculateParameterChanges(const FString& SimulationID};

    // 浪琩????兵ン
    bool CheckSuccessConditions(const FString& SimulationID};

    // 浪琩ア??兵ン
    bool CheckFailureConditions(const FString& SimulationID};

    // ????家览挡??
    void GenerateSimulationResult(const FString& SimulationID};

    // 璸??菌非絋X    float CalculateHistoricalAccuracy(const FString& SimulationID};

    // 璸?X?產紇臫
    float CalculatePlayerImpact(const FString& SimulationID};

    // ??穝家览??
    void UpdateSimulationProgress(const FString& SimulationID};

    // 癘??家览ㄆン
    void RecordSimulationEvent(const FString& SimulationID, const FString& EventName};

    // ???X?计X(????ㄏ??еX
    float GetParameterValue(const FString& SimulationID, const FString& ParameterName) const;

    // 砞竚??计X(??????еX
    void SetParameterValue(const FString& SimulationID, const FString& ParameterName, float Value};

    // 篶??计е??
    void BuildParameterCache(const FString& SimulationID};

    // ????家览ID
    FString GenerateSimulationID(const FString& BaseName, ESimulationType Type) const;

    // ????家览摸?X?后
    FString GetSimulationTypePrefix(ESimulationType Type) const;

    // 喷??家览??计
    bool ValidateSimulationParameters(const FString& SimulationID, const TMap<FString, float>& Parameters) const;

    // 璸??ㄆン牟祇阀??
    float CalculateEventTriggerProbability(const FString& SimulationID, const FSimulationEvent& Event) const;

    // ??ノㄆン??
    void ApplyEventConsequences(const FString& SimulationID, const FSimulationEvent& Event};

    // ????家览????
    FString GenerateSimulationReport(const FString& SimulationID) const;
};

