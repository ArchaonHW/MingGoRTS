#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSHistoricalRoleplay.h"
#include "MingGoRTSHistoricalSimulation.generated.h"

UENUM(BlueprintType)
enum class ESimulationType : uint8
{
    Political         UMETA(DisplayName = "政治模擬"),
    Military          UMETA(DisplayName = "軍事模擬"),
    Economic          UMETA(DisplayName = "經濟模擬"),
    Social            UMETA(DisplayName = "社會模擬"),
    Cultural          UMETA(DisplayName = "文化模擬"),
    International     UMETA(DisplayName = "國際模擬")
};

UENUM(BlueprintType)
enum class ESimulationPhase : uint8
{
    Setup             UMETA(DisplayName = "設置階段"),
    Execution         UMETA(DisplayName = "執行階段"),
    Analysis          UMETA(DisplayName = "分析階段"),
    Results           UMETA(DisplayName = "結果階段")
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
        ParameterName = TEXT("");
        Value = 0.0f;
        MinValue = 0.0f;
        MaxValue = 100.0f;
        Unit = TEXT("");
        Description = TEXT("");
    }
};

UENUM(BlueprintType)
enum class EConsequenceOperation : uint8
{
    Add         UMETA(DisplayName = "加法"),
    Subtract    UMETA(DisplayName = "減法"),
    Multiply    UMETA(DisplayName = "乘法"),
    Divide      UMETA(DisplayName = "除法"),
    Set         UMETA(DisplayName = "設置"),
    Reset       UMETA(DisplayName = "重置")
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
        TargetParameter = TEXT("");
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
        EventID = TEXT("");
        EventName = TEXT("");
        Description = TEXT("");
        TriggerProbability = 0.0f;
        TimeDelay = TEXT("");
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
        SimulationID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        SimulationType = ESimulationType::Political;
        HistoricalPeriod = TEXT("");
        GeographicScope = TEXT("");
        HistoricalBaseline = TEXT("");
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
        ResultID = TEXT("");
        SimulationID = TEXT("");
        bSuccess = false;
        FinalScore = 0.0f;
        OutcomeDescription = TEXT("");
        HistoricalAccuracy = 0.0f;
        PlayerImpact = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSimulationStarted, const FString&, SimulationID, ESimulationType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSimulationEventTriggered, const FString&, SimulationID, const FString&, EventID, const FString&, EventName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSimulationCompleted, const FString&, SimulationID, const FSimulationResult&, Result);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalSimulation : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalSimulation();

    // 模擬系統事件
    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationStarted OnSimulationStarted;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationEventTriggered OnSimulationEventTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Simulation System")
    FOnSimulationCompleted OnSimulationCompleted;

    // 初始化模擬系統
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    void InitializeSimulationSystem();

    // 獲取所有模擬
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FHistoricalSimulation> GetAllSimulations() const;

    // 獲取模擬
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FHistoricalSimulation GetSimulation(const FString& SimulationID) const;

    // 開始模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StartSimulation(const FString& SimulationID, const TMap<FString, float>& PlayerDecisions);

    // 暫停模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool PauseSimulation(const FString& SimulationID);

    // 繼續模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool ResumeSimulation(const FString& SimulationID);

    // 停止模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool StopSimulation(const FString& SimulationID);

    // 干預模擬
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool InterveneInSimulation(const FString& SimulationID, const FString& ParameterName, float NewValue);

    // 獲取模擬狀態
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    ESimulationPhase GetSimulationPhase(const FString& SimulationID) const;

    // 獲取當前參數
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FSimulationParameter> GetCurrentParameters(const FString& SimulationID) const;

    // 獲取模擬進度
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float GetSimulationProgress(const FString& SimulationID) const;

    // 獲取模擬結果
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    FSimulationResult GetSimulationResult(const FString& SimulationID) const;

    // 獲取模擬歷史
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<FString> GetSimulationHistory(const FString& SimulationID) const;

    // 計算模擬預測
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TArray<float> PredictSimulationOutcome(const FString& SimulationID, const TMap<FString, float>& ParameterChanges) const;

    // 比較模擬結果
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    float CompareWithHistoricalBaseline(const FString& SimulationID) const;

    // 獲取模擬統計
    UFUNCTION(BlueprintPure, Category = "Simulation System")
    TMap<FString, float> GetSimulationStatistics(const FString& SimulationID) const;

    // 保存模擬數據
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool SaveSimulationData(const FString& SaveSlotName);

    // 載入模擬數據
    UFUNCTION(BlueprintCallable, Category = "Simulation System")
    bool LoadSimulationData(const FString& SaveSlotName);

protected:
    // 所有模擬
    UPROPERTY()
    TArray<FHistoricalSimulation> AllSimulations;

    // 模擬ID到模擬的映射
    UPROPERTY()
    TMap<FString, FHistoricalSimulation> SimulationMap;

    // 活動模擬狀態
    UPROPERTY()
    TMap<FString, ESimulationPhase> ActiveSimulations;

    // 模擬參數 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<FSimulationParameter>> SimulationParameters;

    // 參數快取 - 注意：嵌套 TMap 不支持 UPROPERTY
    TMap<FString, TMap<FString, int32>> ParameterCache;

    // 模擬結果
    UPROPERTY()
    TMap<FString, FSimulationResult> SimulationResults;

    // 模擬歷史
    UPROPERTY()
    TMap<FString, FStringArrayWrapper> SimulationHistories;

    // 事件觸發防護 - 防止循環觸發
    UPROPERTY()
    TMap<FString, int32> EventTriggerDepth;

    // 最大事件觸發深度 (防止無限循環)
    int32 MaxEventTriggerDepth = 10;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化模擬庫
    void InitializeSimulationLibrary();

    // 創建政治模擬
    void CreatePoliticalSimulations();

    // 創建軍事模擬
    void CreateMilitarySimulations();

    // 創建經濟模擬
    void CreateEconomicSimulations();

    // 創建社會模擬
    void CreateSocialSimulations();

    // 創建文化模擬
    void CreateCulturalSimulations();

    // 創建國際模擬
    void CreateInternationalSimulations();

    // 執行模擬步驟
    void ExecuteSimulationStep(const FString& SimulationID);

    // 處理模擬事件
    void ProcessSimulationEvents(const FString& SimulationID);

    // 觸發模擬事件
    void TriggerSimulationEvent(const FString& SimulationID, const FSimulationEvent& Event);

    // 計算參數變化
    void CalculateParameterChanges(const FString& SimulationID);

    // 檢查成功條件
    bool CheckSuccessConditions(const FString& SimulationID);

    // 檢查失敗條件
    bool CheckFailureConditions(const FString& SimulationID);

    // 生成模擬結果
    void GenerateSimulationResult(const FString& SimulationID);

    // 計算歷史準確性
    float CalculateHistoricalAccuracy(const FString& SimulationID);

    // 計算玩家影響
    float CalculatePlayerImpact(const FString& SimulationID);

    // 更新模擬進度
    void UpdateSimulationProgress(const FString& SimulationID);

    // 記錄模擬事件
    void RecordSimulationEvent(const FString& SimulationID, const FString& EventName);

    // 獲取參數值 (優化後使用快取)
    float GetParameterValue(const FString& SimulationID, const FString& ParameterName) const;

    // 設置參數值 (優化後更新快取)
    void SetParameterValue(const FString& SimulationID, const FString& ParameterName, float Value);

    // 構建參數快取
    void BuildParameterCache(const FString& SimulationID);

    // 生成模擬ID
    FString GenerateSimulationID(const FString& BaseName, ESimulationType Type) const;

    // 獲取模擬類型前綴
    FString GetSimulationTypePrefix(ESimulationType Type) const;

    // 驗證模擬參數
    bool ValidateSimulationParameters(const FString& SimulationID, const TMap<FString, float>& Parameters) const;

    // 計算事件觸發概率
    float CalculateEventTriggerProbability(const FString& SimulationID, const FSimulationEvent& Event) const;

    // 應用事件後果
    void ApplyEventConsequences(const FString& SimulationID, const FSimulationEvent& Event);

    // 生成模擬報告
    FString GenerateSimulationReport(const FString& SimulationID) const;
};
