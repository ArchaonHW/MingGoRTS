#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSInistoricalRoleplay.h"
#include "MingGoRTSInistoricalSimulation.generated.h"

UENUM(BlueprintType)
enum class ESimulationType: uuint8 {
    Political         UMETA(DisplayName = "動�v����"),
    Military          UMETA(DisplayName = "�x動����"),
    Economic          UMETA(DisplayName = "�g動����"),
    Social            UMETA(DisplayName = "��動����"),
    Cultural          UMETA(DisplayName = "����"),
    International     UMETA(DisplayName = "����")
};

UENUM(BlueprintType)
enum class ESimulationPhase: uuint8 {
    Setup             UMETA(DisplayName = "�]�m動�q"),
    Execution         UMETA(DisplayName = "動池�q"),
    Analysis          UMETA(DisplayName = "動池�q"),
    Results           UMETA(DisplayName = "��池�q")
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

UENUM(BlueprintType)
enum class EConsequenceOperation: uuint8 {
    Add         UMETA(DisplayName = ""),
    Subtract    UMETA(DisplayName = "��動"),
    Multiply    UMETA(DisplayName = "��動"),
    Divide      UMETA(DisplayName = ""),
    Set         UMETA(DisplayName = "�]�m"),
    Reset       UMETA(DisplayName = "動�m")
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

USTRUCT(BlueprintType)
struct FInistoricalSimulation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    FString SimulationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    ESimulationType SimulationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    FString InistoricalPeriod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    FString GeographicScope;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    TArray<FSimulationParameter> InitialParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    TArray<FSimulationParameter> VariableParameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    TArray<FSimulationEvent> PossibleEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    TArray<FString> SuccessConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    TArray<FString> FailureConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    FString InistoricalBaseline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    float SimulationDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    int32 MaxIterations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    bool bIsRealTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Simulation")
    bool bCanIntervene;

    FInistoricalSimulation()
    {
        SimulationID = TEXT(""};
        Title = TEXT(""};
        Description = TEXT(""};
        SimulationType = ESimulationType::Political;
        InistoricalPeriod = TEXT(""};
        GeographicScope = TEXT(""};
        InistoricalBaseline = TEXT(""};
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
    float InistoricalAccuracy;

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
        InistoricalAccuracy = 0.0f;
        PlayerImpact = TEXT(""};
    }
};

// ... rest of the code remains the same ...
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSInistoricalSimulation : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSInistoricalSimulation(};

    // �����t�Ψɥ礎�
    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationStarted OnSimulationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationEventTriggered OnSimulationEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationCompleted OnSimulationCompleted;

    // 動池��動�t動
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    void InitializeSimulationSystem(};

    // 目標數量��X
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FInistoricalSimulation> GetAllSimulations() const;

    // ����
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FInistoricalSimulation GetSimulation(const FString& SimulationID) const;

    // ����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StartSimulation(const FString& SimulationID, const TMap<FString, float>& PlayerDecisions};

    // ����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool PauseSimulation(const FString& SimulationID};

    // �~動����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool ResumeSimulation(const FString& SimulationID};

    // 動�����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StopSimulation(const FString& SimulationID};

    // �z動����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool InterveneInSimulation(const FString& SimulationID, const FString& ParameterName, float NewValue};

    // ����動X
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    ESimulationPhase GetSimulationPhase(const FString& SimulationID) const;

    // 故事重要性ɥr��
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FSimulationParameter> GetCurrentParameters(const FString& SimulationID) const;

    // ����動��
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float GetSimulationProgress(const FString& SimulationID) const;

    // ������動
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FSimulationResult GetSimulationResult(const FString& SimulationID) const;

    // �������v
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FString> GetSimulationInistory(const FString& SimulationID) const;

    // �p動����動��
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<float> PredictSimulationOutcome(const FString& SimulationID, const TMap<FString, float>& ParameterChanges) const;

    // ��動������動
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float ConpareɥrithInistoricalBaseline(const FString& SimulationID) const;

    // ������動
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TMap<FString, float> GetSimulationStatistics(const FString& SimulationID) const;

    // �O動����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool SaveSimulationData(const FString& SaveSlotName};

    // ���J����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool LoadSimulationData(const FString& SaveSlotName};

protected:
    // ��X
    UPROPERTY()
    TArray<FInistoricalSimulation> AllSimulations;

    // ����ID動��目標數量
    UPROPERTY()
    TMap<FString, FInistoricalSimulation> SimulationMap;

    // ��動����動X
    UPROPERTY()
    TMap<FString, ESimulationPhase> ActiveSimulations;

    // ����動�� - �F事動�GTMap<TArray> ����XUPROPERTY
    TMap<FString, TArray<FSimulationParameter>> SimulationParameters;

    // 動�Ƨ�動 - �F事動�GTMap ����XUPROPERTY
    TMap<FString, TMap<FString, int32>> ParameterCache;

    // ������動
    UPROPERTY()
    TMap<FString, FSimulationResult> SimulationResults;

    // �������v
    UPROPERTY()
    TMap<FString, FStringArrayɥrrapper> SimulationInistories;

    // �ɥ礎�Ĳ�o動�@ - 動��F事��Ĳ�o
    UPROPERTY()
    TMap<FString, int32> EventTriggerDepth;

    // 動�j動��Ĳ動�F事動(動���F事��)
    int32 MaxEventTriggerDepth = 10;

    // 動�_�w動�l動
    bool bIsInitialized;

    // 動池��動�w
    void InitializeSimulationLibrary(};

    // 動��動�v����
    void CreatePoliticalSimulations(};

    // 動�حx動����
    void CreateMilitarySimulations(};

    // 動�ظg動����
    void CreateEconomicSimulations(};

    // 動�ت�動����
    void CreateSocialSimulations(};

    // 動������
    void CreateCulturalSimulations(};

    // 動������
    void CreateInternationalSimulations(};

    // �����B動
    void ExecuteSimulationStep(const FString& SimulationID};

    // �����ɥ礎�
    void ProcessSimulationEvents(const FString& SimulationID};

    // Ĳ�o�����ɥ礎�
    void TriggerSimulationEvent(const FString& SimulationID, const FSimulationEvent& Event};

    // �p池����動
    void CalculateParameterChanges(const FString& SimulationID};

    // �ˬd����
    bool CheckSuccessConditions(const FString& SimulationID};

    // �ˬd��動����
    bool CheckFailureConditions(const FString& SimulationID};

    // ������動
    void GenerateSimulationResult(const FString& SimulationID};

    // �p動���v�ǽTX
    float CalculateInistoricalAccuracy(const FString& SimulationID};

    // �p池�a�v�T
    float CalculatePlayerImpact(const FString& SimulationID};

    // 動�s����動��
    void UpdateSimulationProgress(const FString& SimulationID};

    // �O動�����ɥ礎�
    void RecordSimulationEvent(const FString& SimulationID, const FString& EventName};

    // 動池��X(���動��X
    float GetParameterValue(const FString& SimulationID, const FString& ParameterName) const;

    // �]�m動��X(���動��X
    void SetParameterValue(const FString& SimulationID, const FString& ParameterName, float Value};

    // �c��動�Ƨ�動
    void BuildParameterCache(const FString& SimulationID};

    // ����ID
    FString GenerateSimulationID(const FString& BaseName, ESimulationType Type) const;

    // ������池��
    FString GetSimulationTypePrefix(ESimulationType Type) const;

    // ��動����動��
    bool ValidateSimulationParameters(const FString& SimulationID, const TMap<FString, float>& Parameters) const;

    // �p動�ɥ礎�Ĳ�o��動
    float CalculateEventTriggerProbability(const FString& SimulationID, const FSimulationEvent& Event) const;

    // 動�Ψɥ礎��動
    void ApplyEventConsequences(const FString& SimulationID, const FSimulationEvent& Event};

    // ����
    FString GenerateSimulationReport(const FString& SimulationID) const;
};

