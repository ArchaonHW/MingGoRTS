#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSHistoricalRoleplay.h"
#include "MingGoRTSHistoricalSimulation.generated.h"

UENUM(BlueprintType)
enum class ESimulationType: uint8 {
    Political         UMETA(DisplayName = "??�v����"),
    Military          UMETA(DisplayName = "�x??����"),
    Economic          UMETA(DisplayName = "�g??����"),
    Social            UMETA(DisplayName = "��??����"),
    Cultural          UMETA(DisplayName = "????����"),
    International     UMETA(DisplayName = "????����")
};

UENUM(BlueprintType)
enum class ESimulationPhase: uint8 {
    Setup             UMETA(DisplayName = "�]�m??�q"),
    Execution         UMETA(DisplayName = "???X?�q"),
    Analysis          UMETA(DisplayName = "???X?�q"),
    Results           UMETA(DisplayName = "��?X?�q")
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
);

UENUM(BlueprintType)
enum class EConsequenceOperation: uint8 {
    Add         UMETA(DisplayName = "????"),
    Subtract    UMETA(DisplayName = "��??"),
    Multiply    UMETA(DisplayName = "��??"),
    Divide      UMETA(DisplayName = "????"),
    Set         UMETA(DisplayName = "�]�m"),
    Reset       UMETA(DisplayName = "??�m")
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
);

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
);

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
);

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
);





UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalSimulation : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalSimulation();

    // �����t�Ψƥ�
    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationStarted OnSimulationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationEventTriggered OnSimulationEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationCompleted OnSimulationCompleted;

    // ???X?��??�t??
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    void InitializeSimulationSystem();

    // ???X???��X
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FHistoricalSimulation> GetAllSimulations() const;

    // ????����
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FHistoricalSimulation GetSimulation(const FString& SimulationID) const;

    // ????����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StartSimulation(const FString& SimulationID, const TMap<FString, float>& PlayerDecisions);

    // ????����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool PauseSimulation(const FString& SimulationID);

    // �~??����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool ResumeSimulation(const FString& SimulationID);

    // ??�����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StopSimulation(const FString& SimulationID);

    // �z??����
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool InterveneInSimulation(const FString& SimulationID, const FString& ParameterName, float NewValue);

    // ????����??X
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    ESimulationPhase GetSimulationPhase(const FString& SimulationID) const;

    // ???X??X?��
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FSimulationParameter> GetCurrentParameters(const FString& SimulationID) const;

    // ????����??��
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float GetSimulationProgress(const FString& SimulationID) const;

    // ????������??
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FSimulationResult GetSimulationResult(const FString& SimulationID) const;

    // ????�������v
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FString> GetSimulationHistory(const FString& SimulationID) const;

    // �p??����??��
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<float> PredictSimulationOutcome(const FString& SimulationID, const TMap<FString, float>& ParameterChanges) const;

    // ��??������??
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float CompareWithHistoricalBaseline(const FString& SimulationID) const;

    // ????������??
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TMap<FString, float> GetSimulationStatistics(const FString& SimulationID) const;

    // �O??����????
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool SaveSimulationData(const FString& SaveSlotName);

    // ���J����????
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool LoadSimulationData(const FString& SaveSlotName);

protected:
    // ????��X
    UPROPERTY()
    TArray<FHistoricalSimulation> AllSimulations;

    // ����ID??��???X???
    UPROPERTY()
    TMap<FString, FHistoricalSimulation> SimulationMap;

    // ��??����??X
    UPROPERTY()
    TMap<FString, ESimulationPhase> ActiveSimulations;

    // ����??�� - �`??�GTMap<TArray> ����XUPROPERTY
    TMap<FString, TArray<FSimulationParameter>> SimulationParameters;

    // ??�Ƨ�?? - �`??�G????TMap ����XUPROPERTY
    TMap<FString, TMap<FString, int32>> ParameterCache;

    // ������??
    UPROPERTY()
    TMap<FString, FSimulationResult> SimulationResults;

    // �������v
    UPROPERTY()
    TMap<FString, FStringArrayWrapper> SimulationHistories;

    // �ƥ�Ĳ�o??�@ - ??��`��Ĳ�o
    UPROPERTY()
    TMap<FString, int32> EventTriggerDepth;

    // ??�j??��Ĳ??�`??(??��????�`��)
    int32 MaxEventTriggerDepth = 10;

    // ??�_�w??�l??
    bool bIsInitialized;

    // ???X?��??�w
    void InitializeSimulationLibrary();

    // ??��??�v����
    void CreatePoliticalSimulations();

    // ??�حx??����
    void CreateMilitarySimulations();

    // ??�ظg??����
    void CreateEconomicSimulations();

    // ??�ت�??����
    void CreateSocialSimulations();

    // ??��????����
    void CreateCulturalSimulations();

    // ??��????����
    void CreateInternationalSimulations();

    // ????�����B??
    void ExecuteSimulationStep(const FString& SimulationID);

    // ????�����ƥ�
    void ProcessSimulationEvents(const FString& SimulationID);

    // Ĳ�o�����ƥ�
    void TriggerSimulationEvent(const FString& SimulationID, const FSimulationEvent& Event);

    // �p?X?����??
    void CalculateParameterChanges(const FString& SimulationID);

    // �ˬd????����
    bool CheckSuccessConditions(const FString& SimulationID);

    // �ˬd��??����
    bool CheckFailureConditions(const FString& SimulationID);

    // ????������??
    void GenerateSimulationResult(const FString& SimulationID);

    // �p??���v�ǽTX
    float CalculateHistoricalAccuracy(const FString& SimulationID);

    // �p?X?�a�v�T
    float CalculatePlayerImpact(const FString& SimulationID);

    // ??�s����??��
    void UpdateSimulationProgress(const FString& SimulationID);

    // �O??�����ƥ�
    void RecordSimulationEvent(const FString& SimulationID, const FString& EventName);

    // ???X?��X(????���??��X
    float GetParameterValue(const FString& SimulationID, const FString& ParameterName) const;

    // �]�m??��X(????���??��X
    void SetParameterValue(const FString& SimulationID, const FString& ParameterName, float Value);

    // �c��??�Ƨ�??
    void BuildParameterCache(const FString& SimulationID);

    // ????����ID
    FString GenerateSimulationID(const FString& BaseName, ESimulationType Type) const;

    // ????������?X?��
    FString GetSimulationTypePrefix(ESimulationType Type) const;

    // ��??����??��
    bool ValidateSimulationParameters(const FString& SimulationID, const TMap<FString, float>& Parameters) const;

    // �p??�ƥ�Ĳ�o��??
    float CalculateEventTriggerProbability(const FString& SimulationID, const FSimulationEvent& Event) const;

    // ??�Ψƥ��??
    void ApplyEventConsequences(const FString& SimulationID, const FSimulationEvent& Event);

    // ????����????
    FString GenerateSimulationReport(const FString& SimulationID) const;
};

