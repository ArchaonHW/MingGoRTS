#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSIntelligentAPIEnhancements.h"
#include "MingRTSAdvancedAPIFeatures.h"
#include "MingRTSAPISystem.h"
#include "MingRTSSmartAPIActivator.h"
#include "MingRTSIntelligentScriptIntegration.generated.h"

// ========== 腳本整合枚舉 ==========

UENUM(BlueprintType)
enum class EScriptCategory : uint8
{
    Build UMETA(DisplayName = "構建腳本"),
    Automation UMETA(DisplayName = "自動化腳本"),
    AI UMETA(DisplayName = "AI腳本"),
    Optimization UMETA(DisplayName = "優化腳本"),
    Debug UMETA(DisplayName = "除錯腳本"),
    Deployment UMETA(DisplayName = "部署腳本"),
    Testing UMETA(DisplayName = "測試腳本"),
    Maintenance UMETA(DisplayName = "維護腳本")
};

UENUM(BlueprintType)
enum class EScriptExecutionMode : uint8
{
    Immediate UMETA(DisplayName = "立即執行"),
    Scheduled UMETA(DisplayName = "計劃執行"),
    EventDriven UMETA(DisplayName = "事件驅動"),
    Conditional UMETA(DisplayName = "條件執行"),
    Parallel UMETA(DisplayName = "並行執行")
};

UENUM(BlueprintType)
enum class EIntegrationStatus : uint8
{
    Disconnected UMETA(DisplayName = "未連接"),
    Connected UMETA(DisplayName = "已連接"),
    Synchronizing UMETA(DisplayName = "同步中"),
    Active UMETA(DisplayName = "活動中"),
    Error UMETA(DisplayName = "錯誤")
};

// ========== 腳本整合數據結構 ==========

USTRUCT(BlueprintType)
struct FScriptFunctionality
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FunctionalityID;

    UPROPERTY(BlueprintReadOnly)
    FString FunctionalityName;

    UPROPERTY(BlueprintReadOnly)
    FString ScriptPath;

    UPROPERTY(BlueprintReadOnly)
    EScriptCategory Category;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Parameters;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> DefaultValues;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    float AverageExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    int32 ExecutionCount;

    FScriptFunctionality()
    {
        Category = EScriptCategory::Automation;
        AverageExecutionTime = 0.0f;
        SuccessRate = 1.0f;
        ExecutionCount = 0;
    }
};

USTRUCT(BlueprintType)
struct FScriptExecutionContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ContextID;

    UPROPERTY(BlueprintReadOnly)
    EScriptExecutionMode ExecutionMode;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Parameters;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Dependencies;

    UPROPERTY(BlueprintReadOnly)
    FString TriggerCondition;

    UPROPERTY(BlueprintReadOnly)
    FDateTime ScheduledTime;

    UPROPERTY(BlueprintReadOnly)
    int32 Priority;

    UPROPERTY(BlueprintReadOnly)
    bool bRequiresConfirmation;

    FScriptExecutionContext()
    {
        ExecutionMode = EScriptExecutionMode::Immediate;
        Priority = 5;
        bRequiresConfirmation = false;
    }
};

USTRUCT(BlueprintType)
struct FScriptExecutionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ExecutionID;

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly)
    FString Output;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> GeneratedFiles;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CompletedAt;

    FScriptExecutionResult()
    {
        bSuccess = false;
        ExecutionTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FScriptIntelligenceProfile
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ProfileID;

    UPROPERTY(BlueprintReadOnly)
    TArray<EScriptCategory> PreferredCategories;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> ExecutionPatterns;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> SuccessRates;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> FrequentlyUsedScripts;

    UPROPERTY(BlueprintReadOnly)
    float LearningProgress;

    FScriptIntelligenceProfile()
    {
        LearningProgress = 0.0f;
    }
};

// ========== 智慧腳本整合類 ==========

/**
 * 智慧腳本整合系統 - 將現有PowerShell腳本功能與智慧API系統整合
 * 
 * 整合功能：
 * - 自動化開發系統 (AutoDevelopmentSystem.ps1)
 * - 自學習編譯系統 (SelfLearningCompile.ps1)
 * - 智能推薦生成器 (IntelligentRecommendationGenerator.ps1)
 * - 自動修復系統 (FixGarbledText.ps1, FixSyntaxErrors.ps1等)
 * - 持續整合系統 (ContinuousIntegration.ps1)
 * - 風險監控系統 (RiskMonitoring.ps1)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSIntelligentScriptIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSIntelligentScriptIntegration();

    // ========== 系統初始化與連接 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|System")
    void InitializeScriptIntegration();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|System")
    void ConnectToIntelligentAPI(UMingRTSIntelligentAPIEnhancements* IntelligenceAPI);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|System")
    void ConnectToAdvancedAPI(UMingRTSAdvancedAPIFeatures* AdvancedAPI);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|System")
    void ConnectToAPISystem(UMingRTSAPISystem* APISystem);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|System")
    EIntegrationStatus GetIntegrationStatus() const;

    // ========== 腳本功能註冊與管理 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Management")
    void RegisterScriptFunctionality(const FScriptFunctionality& Functionality);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Management")
    void UnregisterScriptFunctionality(const FString& FunctionalityID);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Management")
    TArray<FScriptFunctionality> GetRegisteredScripts() const;

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Management")
    TArray<FScriptFunctionality> GetScriptsByCategory(EScriptCategory Category) const;

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Management")
    FScriptFunctionality GetScriptByID(const FString& FunctionalityID) const;

    // ========== 智慧腳本執行 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Execution")
    FScriptExecutionResult ExecuteScript(const FString& FunctionalityID, const FScriptExecutionContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Execution")
    FScriptExecutionResult ExecuteScriptWithIntelligence(const FString& FunctionalityID, const FScriptExecutionContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Execution")
    void ExecuteScriptAsync(const FString& FunctionalityID, const FScriptExecutionContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Execution")
    void ScheduleScriptExecution(const FString& FunctionalityID, const FDateTime& ScheduleTime);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Execution")
    void CancelScheduledExecution(const FString& ExecutionID);

    // ========== 自動化開發系統整合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Development")
    FScriptExecutionResult RunFullDevelopmentCycle(const FString& Target, bool bSkipBuild = false, bool bSkipTest = false, bool bSkipDeploy = false);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Development")
    FScriptExecutionResult ExecuteCodeQualityCheck();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Development")
    FScriptExecutionResult RunEnvironmentCheck();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Development")
    void StartContinuousDevelopmentMode();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Development")
    void StopContinuousDevelopmentMode();

    // ========== 自學習編譯系統整合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Self Learning")
    FScriptExecutionResult ExecuteSelfLearningCompile(const FString& LearningMode = TEXT("Advanced"), const FString& Strategy = TEXT("Adaptive"));

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Self Learning")
    FScriptExecutionResult AnalyzeCompilationErrors(const FString& LogContent);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Self Learning")
    TArray<FString> SuggestCompilationFixes(const TArray<FString>& ErrorCodes);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Self Learning")
    void UpdateCompilationLearningData(const FScriptExecutionResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Self Learning")
    float GetCompilationLearningProgress() const;

    // ========== 智能修復系統整合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Healing")
    FScriptExecutionResult FixGarbledTextInFiles(const TArray<FString>& FilePaths);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Healing")
    FScriptExecutionResult FixSyntaxErrors(const FString& TargetDirectory);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Healing")
    FScriptExecutionResult FixEncodingIssues(const TArray<FString>& FilePaths);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Healing")
    void EnableAutoHealingMode(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Auto Healing")
    TArray<FString> DetectFileIssues(const FString& DirectoryPath);

    // ========== 智能推薦系統整合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Recommendations")
    TArray<FString> GenerateIntelligentRecommendations(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Recommendations")
    TArray<FString> SuggestOptimizationStrategies(const TArray<FOptimizationMetric>& Metrics);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Recommendations")
    FString GenerateAssetBlueprintBridge(const FString& AssetName, EAssetType AssetType);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Recommendations")
    void ApplyRecommendedFixes(const TArray<FString>& RecommendationIDs);

    // ========== 持續整合系統整合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|CI/CD")
    FScriptExecutionResult RunContinuousIntegration();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|CI/CD")
    FScriptExecutionResult DeployAssets(const FString& TargetEnvironment);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|CI/CD")
    void SetupCIEnvironment();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|CI/CD")
    TArray<FString> GetCIDeploymentStatus();

    // ========== 風險監控系統整合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Risk Monitoring")
    TArray<FString> MonitorSystemRisks();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Risk Monitoring")
    FSecurityThreat DetectSecurityThreats();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Risk Monitoring")
    void EnableRealTimeRiskMonitoring(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Risk Monitoring")
    TArray<FString> GetRiskAssessmentReport();

    // ========== 智慧協調與工作流程 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Coordination")
    FAPICoordinationResult CoordinateScriptsWithAPIs(const TArray<FString>& ScriptIDs, const TArray<EAPIType>& APIs, const FString& Task);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Coordination")
    void CreateIntelligentWorkflow(const FString& WorkflowName, const TArray<FString>& ScriptSequence);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Coordination")
    void ExecuteIntelligentWorkflow(const FString& WorkflowName);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Coordination")
    TArray<FString> SuggestOptimalScriptSequence(const FString& Goal);

    // ========== 預測性腳本執行 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Prediction")
    TArray<FString> PredictRequiredScripts(const FString& UpcomingTask);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Prediction")
    FScriptExecutionContext PredictOptimalExecutionContext(const FString& FunctionalityID);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Prediction")
    float PredictScriptSuccessRate(const FString& FunctionalityID, const FScriptExecutionContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Prediction")
    void EnablePredictiveScriptExecution(bool bEnabled);

    // ========== 學習與適應 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Learning")
    void LearnFromScriptExecution(const FScriptExecutionResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Learning")
    void UpdateScriptIntelligenceProfile(const FScriptIntelligenceProfile& Profile);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Learning")
    FScriptIntelligenceProfile GetCurrentIntelligenceProfile() const;

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Learning")
    void EnableContinuousLearning(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Learning")
    void AdaptScriptParameters(const FString& FunctionalityID);

    // ========== 自然語言整合 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|NLP")
    FScriptExecutionResult ExecuteScriptFromNaturalLanguage(const FString& NaturalLanguageCommand);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|NLP")
    FString GenerateScriptDescription(const FString& FunctionalityID);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|NLP")
    TArray<FString> FindScriptsByDescription(const FString& DescriptionQuery);

    // ========== 監控與報告 ==========
    UFUNCTION(BlueprintCallable, Category = "Script Integration|Monitoring")
    TArray<FScriptExecutionResult> GetExecutionHistory(const FDateTime& StartTime, const FDateTime& EndTime);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Monitoring")
    TMap<EScriptCategory, float> GetScriptCategoryPerformance();

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Monitoring")
    void GenerateIntegrationReport(const FString& ReportPath);

    UFUNCTION(BlueprintCallable, Category = "Script Integration|Monitoring")
    TArray<FString> GetSystemHealthStatus();

    // ========== 事件委託 ==========
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScriptExecutionCompleted, const FString&, FunctionalityID, const FScriptExecutionResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntelligentFixApplied, const FString&, FixDescription);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningDataUpdated, const FScriptIntelligenceProfile&, Profile);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorkflowStepCompleted, const FString&, StepDescription);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPredictiveExecutionTriggered, const FString&, PredictedScriptID, const FScriptExecutionContext&, PredictedContext);

    UPROPERTY(BlueprintAssignable)
    FOnScriptExecutionCompleted OnScriptExecutionCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnIntelligentFixApplied OnIntelligentFixApplied;

    UPROPERTY(BlueprintAssignable)
    FOnLearningDataUpdated OnLearningDataUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnWorkflowStepCompleted OnWorkflowStepCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnPredictiveExecutionTriggered OnPredictiveExecutionTriggered;

private:
    // ========== API連接 ==========
    UPROPERTY()
    TObjectPtr<UMingRTSIntelligentAPIEnhancements> IntelligentAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSAdvancedAPIFeatures> AdvancedAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSAPISystem> APISystem;

    // ========== 腳本註冊表 ==========
    UPROPERTY()
    TMap<FString, FScriptFunctionality> RegisteredScripts;

    UPROPERTY()
    TMap<EScriptCategory, TArray<FString>> ScriptsByCategory;

    // ========== 執行狀態 ==========
    UPROPERTY()
    TMap<FString, FScriptExecutionResult> ExecutionHistory;

    UPROPERTY()
    TArray<FString> PendingExecutions;

    UPROPERTY()
    TMap<FString, FDateTime> ScheduledExecutions;

    // ========== 智慧配置 ==========
    UPROPERTY()
    FScriptIntelligenceProfile CurrentProfile;

    UPROPERTY()
    bool bContinuousLearningEnabled;

    UPROPERTY()
    bool bPredictiveExecutionEnabled;

    UPROPERTY()
    bool bAutoHealingEnabled;

    UPROPERTY()
    bool bContinuousDevelopmentMode;

    // ========== 工作流程 ==========
    UPROPERTY()
    TMap<FString, TArray<FString>> IntelligentWorkflows;

    UPROPERTY()
    FString ActiveWorkflow;

    // ========== 內部方法 ==========
    void InitializeDefaultScripts();
    void RegisterBuildScripts();
    void RegisterAutomationScripts();
    void RegisterAIScripts();
    void RegisterOptimizationScripts();
    void RegisterDebugScripts();
    void RegisterDeploymentScripts();
    void RegisterTestingScripts();
    void RegisterMaintenanceScripts();

    FString ExecutePowerShellScript(const FString& ScriptPath, const TMap<FString, FString>& Parameters);
    bool ValidateScriptPath(const FString& ScriptPath);
    FString BuildScriptCommand(const FString& ScriptPath, const TMap<FString, FString>& Parameters);
    
    void ProcessExecutionResult(const FString& FunctionalityID, const FScriptExecutionResult& Result);
    void UpdateLearningData(const FString& FunctionalityID, bool bSuccess, float ExecutionTime);
    void TriggerPredictiveExecution();
    
    TArray<FString> AnalyzeExecutionPatterns();
    void OptimizeScriptParameters(const FString& FunctionalityID);
    FString GenerateScriptRecommendation(const FString& Context);
    
    void SyncWithIntelligentAPI();
    void SyncWithAdvancedAPI();
    void HandleIntelligentAPIInsight(const FInsightData& Insight);
    void HandleAdvancedAPIAnomaly(const FString& AnomalyDescription);
    
    void SaveIntegrationData();
    void LoadIntegrationData();
    
    FString GenerateExecutionID();
    FString GetProjectRootPath() const;
    FString GetScriptDirectoryPath() const;
};
