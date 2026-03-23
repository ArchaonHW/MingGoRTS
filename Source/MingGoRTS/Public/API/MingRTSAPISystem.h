#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCompilationAPI.h"
#include "MingRTSLogAPI.h"
#include "MingRTSDebugAPI.h"
#include "MingRTSOptimizationAPI.h"
#include "MingRTSSmartPanelAPI.h"
#include "MingRTSCodeStandardsAPI.h"
#include "MingRTSBMADSystem.h"
#include "MingRTSCodeReviewAPI.h"
#include "MingRTSAPISystem.generated.h"

UENUM(BlueprintType)
enum class EAPIType : uint8
{
    Compilation,
    Log,
    Debug,
    Optimization,
    SmartPanel,
    CodeStandards,
    BMAD,
    CodeReview
};

UENUM(BlueprintType)
enum class EAPIInteractionType : uint8
{
    Request,
    Response,
    Event,
    Data,
    Control
};

USTRUCT(BlueprintType)
struct FAPIInteraction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EAPIType SourceAPI;

    UPROPERTY(BlueprintReadOnly)
    EAPIType TargetAPI;

    UPROPERTY(BlueprintReadOnly)
    EAPIInteractionType InteractionType;

    UPROPERTY(BlueprintReadOnly)
    FString Action;

    UPROPERTY(BlueprintReadOnly)
    FString Data;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    bool bSuccessful;

    UPROPERTY(BlueprintReadOnly)
    float ResponseTime;

    FAPIInteraction()
    {
        Timestamp = FDateTime::Now();
        bSuccessful = false;
        ResponseTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FAPICoordinationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly)
    FString CoordinationId;

    UPROPERTY(BlueprintReadOnly)
    TArray<FAPIInteraction> Interactions;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Results;

    UPROPERTY(BlueprintReadOnly)
    float TotalTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;

    FAPICoordinationResult()
    {
        bSuccess = false;
        TotalTime = 0.0f;
    }
};

/**
 * 統一API系統 - 協調所有API的互動和自我學習
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAPISystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAPISystem();

    // API實例管理
    UFUNCTION(BlueprintCallable, Category = "API System")
    void InitializeAPISystem();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSCompilationAPI* GetCompilationAPI();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSLogAPI* GetLogAPI();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSDebugAPI* GetDebugAPI();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSOptimizationAPI* GetOptimizationAPI();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSSmartPanelAPI* GetSmartPanelAPI();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSCodeStandardsAPI* GetCodeStandardsAPI();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSBMADSystem* GetBMADSystem();

    UFUNCTION(BlueprintCallable, Category = "API System")
    UMingRTSCodeReviewAPI* GetCodeReviewAPI();

    // API協調
    UFUNCTION(BlueprintCallable, Category = "API System|Coordination")
    FAPICoordinationResult CoordinateAPIs(const TArray<EAPIType>& APIs, const FString& Task);

    UFUNCTION(BlueprintCallable, Category = "API System|Coordination")
    bool ExecuteAPIWorkflow(const TArray<FAPIInteraction>& Workflow);

    UFUNCTION(BlueprintCallable, Category = "API System|Coordination")
    void RegisterAPIInteraction(const FAPIInteraction& Interaction);

    // 智慧協調
    UFUNCTION(BlueprintCallable, Category = "API System|Intelligence")
    TArray<FAPIInteraction> SuggestOptimalWorkflow(const FString& Goal);

    UFUNCTION(BlueprintCallable, Category = "API System|Intelligence")
    FAPICoordinationResult ExecuteIntelligentTask(const FString& Task);

    UFUNCTION(BlueprintCallable, Category = "API System|Intelligence")
    void LearnFromCoordinationResult(const FAPICoordinationResult& Result);

    // 系統監控
    UFUNCTION(BlueprintCallable, Category = "API System|Monitoring")
    TArray<FAPIInteraction> GetAPIInteractions(const FDateTime& StartTime, const FDateTime& EndTime);

    UFUNCTION(BlueprintCallable, Category = "API System|Monitoring")
    TMap<EAPIType, float> GetAPIPerformanceMetrics();

    UFUNCTION(BlueprintCallable, Category = "API System|Monitoring")
    TArray<FString> GetSystemHealthStatus();

    // 自我學習
    UFUNCTION(BlueprintCallable, Category = "API System|Learning")
    void UpdateInteractionPatterns(const TArray<FAPIInteraction>& Interactions);

    UFUNCTION(BlueprintCallable, Category = "API System|Learning")
    void OptimizeAPICommunication();

    UFUNCTION(BlueprintCallable, Category = "API System|Learning")
    void AdaptToSystemChanges();

    // 預測分析
    UFUNCTION(BlueprintCallable, Category = "API System|Prediction")
    TArray<FAPIInteraction> PredictInteractions(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "API System|Prediction")
    TArray<FString> PredictSystemNeeds();

    UFUNCTION(BlueprintCallable, Category = "API System|Prediction")
    void EnablePredictiveMode(bool bEnabled);

    // 自動化
    UFUNCTION(BlueprintCallable, Category = "API System|Automation")
    void CreateAutomationWorkflow(const FString& WorkflowName, const TArray<FAPIInteraction>& Workflow);

    UFUNCTION(BlueprintCallable, Category = "API System|Automation")
    void ExecuteAutomationWorkflow(const FString& WorkflowName);

    UFUNCTION(BlueprintCallable, Category = "API System|Automation")
    void EnableAutoCoordination(bool bEnabled);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAPIInteraction, const FAPIInteraction&, Interaction);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoordinationCompleted, const FAPICoordinationResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemHealthChanged, const TArray<FString>&, HealthStatus);

    UPROPERTY(BlueprintAssignable)
    FOnAPIInteraction OnAPIInteraction;

    UPROPERTY(BlueprintAssignable)
    FOnCoordinationCompleted OnCoordinationCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnSystemHealthChanged OnSystemHealthChanged;

private:
    // API實例
    UPROPERTY()
    TObjectPtr<UMingRTSCompilationAPI> CompilationAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSLogAPI> LogAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSDebugAPI> DebugAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSOptimizationAPI> OptimizationAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSSmartPanelAPI> SmartPanelAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSCodeStandardsAPI> CodeStandardsAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSBMADSystem> BMADSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSCodeReviewAPI> CodeReviewAPI;

    // 協調數據
    UPROPERTY()
    TArray<FAPIInteraction> InteractionHistory;

    UPROPERTY()
    TMap<FString, TArray<FAPIInteraction>> AutomationWorkflows;

    UPROPERTY()
    TMap<EAPIType, float> APIMetrics;

    UPROPERTY()
    bool bAutoCoordinationEnabled;

    UPROPERTY()
    bool bPredictiveModeEnabled;

    // 內部方法
    void InitializeAPIInstances();
    void SetupAPIConnections();
    void SaveSystemData();
    void LoadSystemData();
    void ProcessInteraction(const FAPIInteraction& Interaction);
    TArray<FAPIInteraction> OptimizeWorkflow(const TArray<FAPIInteraction>& BaseWorkflow);
    bool ValidateWorkflow(const TArray<FAPIInteraction>& Workflow);
    void UpdateSystemMetrics();
};
