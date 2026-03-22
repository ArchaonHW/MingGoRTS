#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSHistoricalRoleplay.h"
#include "MingGoRTSHistoricalSimulation.generated.h"

UENUM(BlueprintType)
enum class ESimulationType : uint8
{
    Political         UMETA(DisplayName = "?�治模擬"),
    Military          UMETA(DisplayName = "軍�?模擬"),
    Economic          UMETA(DisplayName = "經�?模擬"),
    Social            UMETA(DisplayName = "社�?模擬"),
    Cultural          UMETA(DisplayName = "?��?模擬"),
    International     UMETA(DisplayName = "?��?模擬")
};

UENUM(BlueprintType)
enum class ESimulationPhase : uint8
{
    Setup             UMETA(DisplayName = "設置?�段"),
    Execution         UMETA(DisplayName = "?��X�段"),
    Analysis          UMETA(DisplayName = "?��X�段"),
    Results           UMETA(DisplayName = "結�X�段")
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
    Add         UMETA(DisplayName = "?��?"),
    Subtract    UMETA(DisplayName = "減�?"),
    Multiply    UMETA(DisplayName = "乘�?"),
    Divide      UMETA(DisplayName = "?��?"),
    Set         UMETA(DisplayName = "設置"),
    Reset       UMETA(DisplayName = "?�置")
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

    // 模擬系統事件
    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationStarted OnSimulationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationEventTriggered OnSimulationEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationCompleted OnSimulationCompleted;

    // ?��X�模?�系�?    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    void InitializeSimulationSystem(};

    // ?��X�?�模X    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FHistoricalSimulation> GetAllSimulations() const;

    // ?��?模擬
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FHistoricalSimulation GetSimulation(const FString& SimulationID) const;

    // ?��?模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StartSimulation(const FString& SimulationID, const TMap<FString, float>& PlayerDecisions};

    // ?��?模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool PauseSimulation(const FString& SimulationID};

    // 繼�?模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool ResumeSimulation(const FString& SimulationID};

    // ?�止模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StopSimulation(const FString& SimulationID};

    // 干�?模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool InterveneInSimulation(const FString& SimulationID, const FString& ParameterName, float NewValue};

    // ?��?模擬?�X    UFUNCTION(BlueprintPure, Category = "Simulation System")
    ESimulationPhase GetSimulationPhase(const FString& SimulationID) const;

    // ?��X��X�數
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FSimulationParameter> GetCurrentParameters(const FString& SimulationID) const;

    // ?��?模擬?�度
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float GetSimulationProgress(const FString& SimulationID) const;

    // ?��?模擬結�?
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FSimulationResult GetSimulationResult(const FString& SimulationID) const;

    // ?��?模擬歷史
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FString> GetSimulationHistory(const FString& SimulationID) const;

    // 計�?模擬?�測
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<float> PredictSimulationOutcome(const FString& SimulationID, const TMap<FString, float>& ParameterChanges) const;

    // 比�?模擬結�?
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float CompareWithHistoricalBaseline(const FString& SimulationID) const;

    // ?��?模擬統�?
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TMap<FString, float> GetSimulationStatistics(const FString& SimulationID) const;

    // 保�?模擬?��?
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool SaveSimulationData(const FString& SaveSlotName};

    // 載入模擬?��?
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool LoadSimulationData(const FString& SaveSlotName};

protected:
    // ?�?�模X    UPROPERTY()
    TArray<FHistoricalSimulation> AllSimulations;

    // 模擬ID?�模?��X��?
    UPROPERTY()
    TMap<FString, FHistoricalSimulation> SimulationMap;

    // 活�?模擬?�X    UPROPERTY()
    TMap<FString, ESimulationPhase> ActiveSimulations;

    // 模擬?�數 - 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<FString, TArray<FSimulationParameter>> SimulationParameters;

    // ?�數快�? - 注�?：�?�?TMap 不支XUPROPERTY
    TMap<FString, TMap<FString, int32>> ParameterCache;

    // 模擬結�?
    UPROPERTY()
    TMap<FString, FSimulationResult> SimulationResults;

    // 模擬歷史
    UPROPERTY()
    TMap<FString, FStringArrayWrapper> SimulationHistories;

    // 事件觸發?�護 - ?�止循環觸發
    UPROPERTY()
    TMap<FString, int32> EventTriggerDepth;

    // ?�大�?件觸?�深�?(?�止?��?循環)
    int32 MaxEventTriggerDepth = 10;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?��X�模?�庫
    void InitializeSimulationLibrary(};

    // ?�建?�治模擬
    void CreatePoliticalSimulations(};

    // ?�建軍�?模擬
    void CreateMilitarySimulations(};

    // ?�建經�?模擬
    void CreateEconomicSimulations(};

    // ?�建社�?模擬
    void CreateSocialSimulations(};

    // ?�建?��?模擬
    void CreateCulturalSimulations(};

    // ?�建?��?模擬
    void CreateInternationalSimulations(};

    // ?��?模擬步�?
    void ExecuteSimulationStep(const FString& SimulationID};

    // ?��?模擬事件
    void ProcessSimulationEvents(const FString& SimulationID};

    // 觸發模擬事件
    void TriggerSimulationEvent(const FString& SimulationID, const FSimulationEvent& Event};

    // 計�X�數變�?
    void CalculateParameterChanges(const FString& SimulationID};

    // 檢查?��?條件
    bool CheckSuccessConditions(const FString& SimulationID};

    // 檢查失�?條件
    bool CheckFailureConditions(const FString& SimulationID};

    // ?��?模擬結�?
    void GenerateSimulationResult(const FString& SimulationID};

    // 計�?歷史準確X    float CalculateHistoricalAccuracy(const FString& SimulationID};

    // 計�X�家影響
    float CalculatePlayerImpact(const FString& SimulationID};

    // ?�新模擬?�度
    void UpdateSimulationProgress(const FString& SimulationID};

    // 記�?模擬事件
    void RecordSimulationEvent(const FString& SimulationID, const FString& EventName};

    // ?��X�數X(?��?後使?�快X
    float GetParameterValue(const FString& SimulationID, const FString& ParameterName) const;

    // 設置?�數X(?��?後更?�快X
    void SetParameterValue(const FString& SimulationID, const FString& ParameterName, float Value};

    // 構建?�數快�?
    void BuildParameterCache(const FString& SimulationID};

    // ?��?模擬ID
    FString GenerateSimulationID(const FString& BaseName, ESimulationType Type) const;

    // ?��?模擬類�X�綴
    FString GetSimulationTypePrefix(ESimulationType Type) const;

    // 驗�?模擬?�數
    bool ValidateSimulationParameters(const FString& SimulationID, const TMap<FString, float>& Parameters) const;

    // 計�?事件觸發概�?
    float CalculateEventTriggerProbability(const FString& SimulationID, const FSimulationEvent& Event) const;

    // ?�用事件後�?
    void ApplyEventConsequences(const FString& SimulationID, const FSimulationEvent& Event};

    // ?��?模擬?��?
    FString GenerateSimulationReport(const FString& SimulationID) const;
};

