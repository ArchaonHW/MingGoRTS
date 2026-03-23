// Copyright (c) 2026 MingGoRTS. All rights reserved.
// MingRTSCodingAPIFunctionMappings.h
// 功能到 API 的詳細映射定義

#pragma once

#include "CoreMinimal.h"
#include "MingRTSCodingAPI.h"

/**
 * ============================================================================
 * MingRTS Coding API 功能映射總覽
 * ============================================================================
 * 
 * 本文件定義了專案中所有程式語言功能如何分配到對應的 Coding API。
 * 
 * 映射結構:
 * - ECodingAPIDomain: API 域分類
 * - SourceAPI: 原始 API 類
 * - FunctionName: 功能名稱
 * - TargetDomain: 目標域分配
 * - Language: 支援語言
 * - UsagePattern: 使用模式
 */

// ============================================================================
// 域 1: 代碼生成 (CodeGeneration)
// 來源: MingRTSPotatoAICodeGeneratorAPI
// ============================================================================

USTRUCT(BlueprintType)
struct FCodeGenerationMappings
{
    GENERATED_BODY()

    // 核心代碼生成功能
    UPROPERTY()
    FString GenerateUE5Class = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5Class");
    
    UPROPERTY()
    FString GenerateUE5Function = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5Function");
    
    UPROPERTY()
    FString GenerateUE5System = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5System");
    
    UPROPERTY()
    FString GenerateUE5Gameplay = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateUE5Gameplay");
    
    UPROPERTY()
    FString GenerateGenericCode = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateGenericCode");

    // 模板選擇
    UPROPERTY()
    FString SelectCodeTemplate = TEXT("UMingRTSPotatoAICodeGeneratorAPI::SelectCodeTemplate");
    
    UPROPERTY()
    FString GetAvailableTemplates = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GetAvailableTemplates");

    // 質量檢查
    UPROPERTY()
    FString CheckCodeQuality = TEXT("UMingRTSPotatoAICodeGeneratorAPI::CheckCodeQuality");
    
    UPROPERTY()
    FString CalculateQualityScore = TEXT("UMingRTSPotatoAICodeGeneratorAPI::CalculateQualityScore");

    // 智能命名
    UPROPERTY()
    FString GenerateClassName = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateClassName");
    
    UPROPERTY()
    FString GenerateFunctionName = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateFunctionName");
    
    UPROPERTY()
    FString GenerateSystemName = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateSystemName");
    
    UPROPERTY()
    FString GenerateActorName = TEXT("UMingRTSPotatoAICodeGeneratorAPI::GenerateActorName");

    // 批量生成
    UPROPERTY()
    FString BatchGenerateCode = TEXT("UMingRTSPotatoAICodeGeneratorAPI::BatchGenerateCode");

    // 匯出功能
    UPROPERTY()
    FString ExportCodeToFile = TEXT("UMingRTSPotatoAICodeGeneratorAPI::ExportCodeToFile");
    
    UPROPERTY()
    FString ExportToClipboard = TEXT("UMingRTSPotatoAICodeGeneratorAPI::ExportToClipboard");
};

// ============================================================================
// 域 2: AI 代碼生成 (AICodeGeneration)
// 來源: MingRTSPotatoAIEnhancedAPI
// ============================================================================

USTRUCT(BlueprintType)
struct FAICodeGenerationMappings
{
    GENERATED_BODY()

    // 智能代碼生成
    UPROPERTY()
    FString IntelligentGenerate = TEXT("UMingRTSPotatoAIEnhancedAPI::IntelligentGenerate");
    
    UPROPERTY()
    FString ContextAwareGenerate = TEXT("UMingRTSPotatoAIEnhancedAPI::ContextAwareGenerate");

    // 需求分析
    UPROPERTY()
    FString AnalyzeRequirements = TEXT("UMingRTSPotatoAIEnhancedAPI::AnalyzeRequirements");
    
    UPROPERTY()
    FString ExtractCodeIntent = TEXT("UMingRTSPotatoAIEnhancedAPI::ExtractCodeIntent");

    // 多供應商支援
    UPROPERTY()
    FString GenerateWithProvider = TEXT("UMingRTSPotatoAIEnhancedAPI::GenerateWithProvider");
    
    UPROPERTY()
    FString CompareProviders = TEXT("UMingRTSPotatoAIEnhancedAPI::CompareProviders");

    // 代碼優化建議
    UPROPERTY()
    FString SuggestOptimizations = TEXT("UMingRTSPotatoAIEnhancedAPI::SuggestOptimizations");
    
    UPROPERTY()
    FString RefactorSuggestions = TEXT("UMingRTSPotatoAIEnhancedAPI::RefactorSuggestions");
};

// ============================================================================
// 域 3: 代碼分析 (CodeAnalysis)
// 來源: MingRTSCodeReviewAPI, MingRTSCodeStandardsAPI
// ============================================================================

USTRUCT(BlueprintType)
struct FCodeAnalysisMappings
{
    GENERATED_BODY()

    // 代碼審查功能 (來自 CodeReviewAPI)
    UPROPERTY()
    FString ReviewCode = TEXT("UMingRTSCodeReviewAPI::ReviewCode");
    
    UPROPERTY()
    FString ReviewFile = TEXT("UMingRTSCodeReviewAPI::ReviewFile");
    
    UPROPERTY()
    FString ReviewProject = TEXT("UMingRTSCodeReviewAPI::ReviewProject");

    // 規範檢查 (來自 CodeStandardsAPI)
    UPROPERTY()
    FString CheckNamingConventions = TEXT("UMingRTSCodeStandardsAPI::CheckNamingConventions");
    
    UPROPERTY()
    FString CheckFormatting = TEXT("UMingRTSCodeStandardsAPI::CheckFormatting");
    
    UPROPERTY()
    FString CheckDocumentation = TEXT("UMingRTSCodeStandardsAPI::CheckDocumentation");
    
    UPROPERTY()
    FString CheckArchitecture = TEXT("UMingRTSCodeStandardsAPI::CheckArchitecture");

    // 問題檢測
    UPROPERTY()
    FString FindIssues = TEXT("UMingRTSCodeReviewAPI::FindIssues");
    
    UPROPERTY()
    FString AutoFixIssues = TEXT("UMingRTSCodeReviewAPI::AutoFixIssues");
    
    UPROPERTY()
    FString GenerateFixSuggestions = TEXT("UMingRTSCodeReviewAPI::GenerateFixSuggestions");

    // 品質指標
    UPROPERTY()
    FString CalculateQualityMetrics = TEXT("UMingRTSCodeReviewAPI::CalculateQualityMetrics");
    
    UPROPERTY()
    FString GenerateQualityReport = TEXT("UMingRTSCodeReviewAPI::GenerateQualityReport");
};

// ============================================================================
// 域 4: 除錯 (Debugging)
// 來源: MingRTSDebugAPI
// ============================================================================

USTRUCT(BlueprintType)
struct FDebuggingMappings
{
    GENERATED_BODY()

    // 除錯會話管理
    UPROPERTY()
    FString StartDebugSession = TEXT("UMingRTSDebugAPI::StartDebugSession");
    
    UPROPERTY()
    FString StopDebugSession = TEXT("UMingRTSDebugAPI::StopDebugSession");

    // 斷點管理
    UPROPERTY()
    FString SetBreakpoint = TEXT("UMingRTSDebugAPI::SetBreakpoint");
    
    UPROPERTY()
    FString RemoveBreakpoint = TEXT("UMingRTSDebugAPI::RemoveBreakpoint");
    
    UPROPERTY()
    FString GetSmartBreakpointSuggestions = TEXT("UMingRTSDebugAPI::GetSmartBreakpointSuggestions");

    // 執行控制
    UPROPERTY()
    FString StepOver = TEXT("UMingRTSDebugAPI::StepOver");
    
    UPROPERTY()
    FString StepInto = TEXT("UMingRTSDebugAPI::StepInto");
    
    UPROPERTY()
    FString Continue = TEXT("UMingRTSDebugAPI::Continue");

    // 變數監控
    UPROPERTY()
    FString GetLocalVariables = TEXT("UMingRTSDebugAPI::GetLocalVariables");
    
    UPROPERTY()
    FString GetGlobalVariables = TEXT("UMingRTSDebugAPI::GetGlobalVariables");
    
    UPROPERTY()
    FString AddWatchExpression = TEXT("UMingRTSDebugAPI::AddWatchExpression");
    
    UPROPERTY()
    FString EvaluateExpression = TEXT("UMingRTSDebugAPI::EvaluateExpression");

    // 記憶體除錯
    UPROPERTY()
    FString GetMemoryRegion = TEXT("UMingRTSDebugAPI::GetMemoryRegion");
    
    UPROPERTY()
    FString TrackMemoryLeaks = TEXT("UMingRTSDebugAPI::TrackMemoryLeaks");
    
    UPROPERTY()
    FString GetMemoryStatistics = TEXT("UMingRTSDebugAPI::GetMemoryStatistics");

    // 性能除錯
    UPROPERTY()
    FString StartPerformanceProfile = TEXT("UMingRTSDebugAPI::StartPerformanceProfile");
    
    UPROPERTY()
    FString StopPerformanceProfile = TEXT("UMingRTSDebugAPI::StopPerformanceProfile");
    
    UPROPERTY()
    FString GetPerformanceMetrics = TEXT("UMingRTSDebugAPI::GetPerformanceMetrics");

    // 自我學習
    UPROPERTY()
    FString LearnFromBugPattern = TEXT("UMingRTSDebugAPI::LearnFromBugPattern");
    
    UPROPERTY()
    FString UpdateBreakpointEfficiency = TEXT("UMingRTSDebugAPI::UpdateBreakpointEfficiency");

    // 智能分析
    UPROPERTY()
    FString AnalyzeCurrentState = TEXT("UMingRTSDebugAPI::AnalyzeCurrentState");
    
    UPROPERTY()
    FString AnalyzeErrorPattern = TEXT("UMingRTSDebugAPI::AnalyzeErrorPattern");
    
    UPROPERTY()
    FString PredictPotentialIssues = TEXT("UMingRTSDebugAPI::PredictPotentialIssues");
};

// ============================================================================
// 域 5: 編譯 (Compilation)
// 來源: MingRTSCompilationAPI
// ============================================================================

USTRUCT(BlueprintType)
struct FCompilationMappings
{
    GENERATED_BODY()

    // 核心編譯
    UPROPERTY()
    FString CompileProject = TEXT("UMingRTSCompilationAPI::CompileProject");
    
    UPROPERTY()
    FString CompileFile = TEXT("UMingRTSCompilationAPI::CompileFile");
    
    UPROPERTY()
    FString ValidateSyntax = TEXT("UMingRTSCompilationAPI::ValidateSyntax");

    // 自我學習
    UPROPERTY()
    FString LearnFromCompilationResult = TEXT("UMingRTSCompilationAPI::LearnFromCompilationResult");
    
    UPROPERTY()
    FString UpdateErrorPatterns = TEXT("UMingRTSCompilationAPI::UpdateErrorPatterns");
    
    UPROPERTY()
    FString GetSmartSuggestions = TEXT("UMingRTSCompilationAPI::GetSmartSuggestions");

    // 智能優化
    UPROPERTY()
    FString OptimizeCompilation = TEXT("UMingRTSCompilationAPI::OptimizeCompilation");
    
    UPROPERTY()
    FString UpdateOptimizationStrategies = TEXT("UMingRTSCompilationAPI::UpdateOptimizationStrategies");

    // 跨 API 調用
    UPROPERTY()
    FString RequestLogAnalysis = TEXT("UMingRTSCompilationAPI::RequestLogAnalysis");
    
    UPROPERTY()
    FString RequestDebuggingSession = TEXT("UMingRTSCompilationAPI::RequestDebuggingSession");
    
    UPROPERTY()
    FString RequestPerformanceAnalysis = TEXT("UMingRTSCompilationAPI::RequestPerformanceAnalysis");
};

// ============================================================================
// 域 6: 版本控制 (VersionControl)
// 來源: MingRTSAPISystem
// ============================================================================

USTRUCT(BlueprintType)
struct FVersionControlMappings
{
    GENERATED_BODY()

    // Git 操作
    UPROPERTY()
    FString GitCommit = TEXT("UMingRTSAPISystem::GitCommit");
    
    UPROPERTY()
    FString GitPush = TEXT("UMingRTSAPISystem::GitPush");
    
    UPROPERTY()
    FString GitPull = TEXT("UMingRTSAPISystem::GitPull");
    
    UPROPERTY()
    FString GitBranch = TEXT("UMingRTSAPISystem::GitBranch");
    
    UPROPERTY()
    FString GitMerge = TEXT("UMingRTSAPISystem::GitMerge");

    // 分支管理
    UPROPERTY()
    FString CreateBranch = TEXT("UMingRTSAPISystem::CreateBranch");
    
    UPROPERTY()
    FString SwitchBranch = TEXT("UMingRTSAPISystem::SwitchBranch");
    
    UPROPERTY()
    FString DeleteBranch = TEXT("UMingRTSAPISystem::DeleteBranch");
    
    UPROPERTY()
    FString ListBranches = TEXT("UMingRTSAPISystem::ListBranches");

    // 提交管理
    UPROPERTY()
    FString StageFiles = TEXT("UMingRTSAPISystem::StageFiles");
    
    UPROPERTY()
    FString UnstageFiles = TEXT("UMingRTSAPISystem::UnstageFiles");
    
    UPROPERTY()
    FString GetCommitHistory = TEXT("UMingRTSAPISystem::GetCommitHistory");
    
    UPROPERTY()
    FString RevertCommit = TEXT("UMingRTSAPISystem::RevertCommit");

    // 差異分析
    UPROPERTY()
    FString ShowDiff = TEXT("UMingRTSAPISystem::ShowDiff");
    
    UPROPERTY()
    FString CompareBranches = TEXT("UMingRTSAPISystem::CompareBranches");
    
    UPROPERTY()
    FString GeneratePatch = TEXT("UMingRTSAPISystem::GeneratePatch");
};

// ============================================================================
// 域 7: 多智能體系統 (MultiAgent)
// 來源: MingRTSBMADSystem
// ============================================================================

USTRUCT(BlueprintType)
struct FMultiAgentMappings
{
    GENERATED_BODY()

    // 智能體管理
    UPROPERTY()
    FString CreateAgent = TEXT("UMingRTSBMADSystem::CreateAgent");
    
    UPROPERTY()
    FString RemoveAgent = TEXT("UMingRTSBMADSystem::RemoveAgent");
    
    UPROPERTY()
    FString ConfigureAgent = TEXT("UMingRTSBMADSystem::ConfigureAgent");
    
    UPROPERTY()
    FString GetAgentProfile = TEXT("UMingRTSBMADSystem::GetAgentProfile");

    // 討論管理
    UPROPERTY()
    FString StartDiscussion = TEXT("UMingRTSBMADSystem::StartDiscussion");
    
    UPROPERTY()
    FString AddAgentToDiscussion = TEXT("UMingRTSBMADSystem::AddAgentToDiscussion");
    
    UPROPERTY()
    FString PostAgentMessage = TEXT("UMingRTSBMADSystem::PostAgentMessage");
    
    UPROPERTY()
    FString GetDiscussionSummary = TEXT("UMingRTSBMADSystem::GetDiscussionSummary");

    // 決策流程
    UPROPERTY()
    FString ProposeSolution = TEXT("UMingRTSBMADSystem::ProposeSolution");
    
    UPROPERTY()
    FString VoteOnSolution = TEXT("UMingRTSBMADSystem::VoteOnSolution");
    
    UPROPERTY()
    FString ReachConsensus = TEXT("UMingRTSBMADSystem::ReachConsensus");
    
    UPROPERTY()
    FString GenerateFinalDecision = TEXT("UMingRTSBMADSystem::GenerateFinalDecision");

    // 角色特定功能
    UPROPERTY()
    FString GetArchitectAnalysis = TEXT("UMingRTSBMADSystem::GetArchitectAnalysis");
    
    UPROPERTY()
    FString GetDeveloperImplementation = TEXT("UMingRTSBMADSystem::GetDeveloperImplementation");
    
    UPROPERTY()
    FString GetTesterFeedback = TEXT("UMingRTSBMADSystem::GetTesterFeedback");
    
    UPROPERTY()
    FString GetSecurityReview = TEXT("UMingRTSBMADSystem::GetSecurityReview");
};

// ============================================================================
// 域 8: 腳本整合 (ScriptIntegration)
// 來源: MingRTSScriptIntegrationAPI
// ============================================================================

USTRUCT(BlueprintType)
struct FScriptIntegrationMappings
{
    GENERATED_BODY()

    // PowerShell 整合
    UPROPERTY()
    FString ExecutePowerShell = TEXT("UMingRTSScriptIntegrationAPI::ExecutePowerShell");
    
    UPROPERTY()
    FString ExecutePowerShellScript = TEXT("UMingRTSScriptIntegrationAPI::ExecutePowerShellScript");
    
    UPROPERTY()
    FString GetPowerShellOutput = TEXT("UMingRTSScriptIntegrationAPI::GetPowerShellOutput");

    // Python 整合
    UPROPERTY()
    FString ExecutePython = TEXT("UMingRTSScriptIntegrationAPI::ExecutePython");
    
    UPROPERTY()
    FString ExecutePythonScript = TEXT("UMingRTSScriptIntegrationAPI::ExecutePythonScript");
    
    UPROPERTY()
    FString GetPythonOutput = TEXT("UMingRTSScriptIntegrationAPI::GetPythonOutput");

    // 腳本管理
    UPROPERTY()
    FString LoadScript = TEXT("UMingRTSScriptIntegrationAPI::LoadScript");
    
    UPROPERTY()
    FString SaveScript = TEXT("UMingRTSScriptIntegrationAPI::SaveScript");
    
    UPROPERTY()
    FString ListAvailableScripts = TEXT("UMingRTSScriptIntegrationAPI::ListAvailableScripts");
    
    UPROPERTY()
    FString ScheduleScriptExecution = TEXT("UMingRTSScriptIntegrationAPI::ScheduleScriptExecution");

    // 結果處理
    UPROPERTY()
    FString ParseScriptOutput = TEXT("UMingRTSScriptIntegrationAPI::ParseScriptOutput");
    
    UPROPERTY()
    FString ConvertToUE5Types = TEXT("UMingRTSScriptIntegrationAPI::ConvertToUE5Types");
};

// ============================================================================
// 域 9: 高級功能 (AdvancedFeatures)
// 來源: MingRTSAdvancedAPIFeatures
// ============================================================================

USTRUCT(BlueprintType)
struct FAdvancedFeaturesMappings
{
    GENERATED_BODY()

    // 自然語言處理
    UPROPERTY()
    FString ProcessNaturalLanguage = TEXT("UMingRTSAdvancedAPIFeatures::ProcessNaturalLanguage");
    
    UPROPERTY()
    FString ExtractCodeEntities = TEXT("UMingRTSAdvancedAPIFeatures::ExtractCodeEntities");
    
    UPROPERTY()
    FString AnalyzeCodeSentiment = TEXT("UMingRTSAdvancedAPIFeatures::AnalyzeCodeSentiment");

    // 數據挖掘
    UPROPERTY()
    FString PerformPatternRecognition = TEXT("UMingRTSAdvancedAPIFeatures::PerformPatternRecognition");
    
    UPROPERTY()
    FString AnalyzeTrends = TEXT("UMingRTSAdvancedAPIFeatures::AnalyzeTrends");
    
    UPROPERTY()
    FString PerformClustering = TEXT("UMingRTSAdvancedAPIFeatures::PerformClustering");
    
    UPROPERTY()
    FString PerformClassification = TEXT("UMingRTSAdvancedAPIFeatures::PerformClassification");

    // 安全分析
    UPROPERTY()
    FString AnalyzeSecurityThreats = TEXT("UMingRTSAdvancedAPIFeatures::AnalyzeSecurityThreats");
    
    UPROPERTY()
    FString DetectVulnerabilities = TEXT("UMingRTSAdvancedAPIFeatures::DetectVulnerabilities");
    
    UPROPERTY()
    FString GenerateSecurityReport = TEXT("UMingRTSAdvancedAPIFeatures::GenerateSecurityReport");

    // 預測維護
    UPROPERTY()
    FString PredictIssues = TEXT("UMingRTSAdvancedAPIFeatures::PredictIssues");
    
    UPROPERTY()
    FString AnalyzeSystemHealth = TEXT("UMingRTSAdvancedAPIFeatures::AnalyzeSystemHealth");
    
    UPROPERTY()
    FString GenerateMaintenanceSchedule = TEXT("UMingRTSAdvancedAPIFeatures::GenerateMaintenanceSchedule");
};

// ============================================================================
// 域 10: 學習系統 (LearningSystem)
// 來源: MingRTSUE5LearningAPI
// ============================================================================

USTRUCT(BlueprintType)
struct FLearningSystemMappings
{
    GENERATED_BODY()

    // 學習管理
    UPROPERTY()
    FString StartLearningSession = TEXT("UMingRTSUE5LearningAPI::StartLearningSession");
    
    UPROPERTY()
    FString StopLearningSession = TEXT("UMingRTSUE5LearningAPI::StopLearningSession");
    
    UPROPERTY()
    FString GetLearningProgress = TEXT("UMingRTSUE5LearningAPI::GetLearningProgress");

    // 文檔生成
    UPROPERTY()
    FString GenerateDocumentation = TEXT("UMingRTSUE5LearningAPI::GenerateDocumentation");
    
    UPROPERTY()
    FString GenerateAPIDocs = TEXT("UMingRTSUE5LearningAPI::GenerateAPIDocs");
    
    UPROPERTY()
    FString GenerateTutorials = TEXT("UMingRTSUE5LearningAPI::GenerateTutorials");

    // 知識累積
    UPROPERTY()
    FString LearnFromCode = TEXT("UMingRTSUE5LearningAPI::LearnFromCode");
    
    UPROPERTY()
    FString LearnFromErrors = TEXT("UMingRTSUE5LearningAPI::LearnFromErrors");
    
    UPROPERTY()
    FString BuildKnowledgeBase = TEXT("UMingRTSUE5LearningAPI::BuildKnowledgeBase");
    
    UPROPERTY()
    FString QueryKnowledgeBase = TEXT("UMingRTSUE5LearningAPI::QueryKnowledgeBase");

    // 技能評估
    UPROPERTY()
    FString AssessCodingSkills = TEXT("UMingRTSUE5LearningAPI::AssessCodingSkills");
    
    UPROPERTY()
    FString GenerateLearningPath = TEXT("UMingRTSUE5LearningAPI::GenerateLearningPath");
    
    UPROPERTY()
    FString RecommendResources = TEXT("UMingRTSUE5LearningAPI::RecommendResources");
};

// ============================================================================
// 統合映射管理器
// ============================================================================

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCodingAPIMappings : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCodingAPIMappings();

    // 初始化所有映射
    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    void InitializeMappings();

    // 查詢功能映射
    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    FString GetFunctionMapping(ECodingAPIDomain Domain, const FString& FunctionName) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    TArray<FString> GetAllFunctionsInDomain(ECodingAPIDomain Domain) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    ECodingAPIDomain GetDomainForFunction(const FString& FunctionName) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    bool IsFunctionAvailable(const FString& FunctionName) const;

    // 批量查詢
    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    TMap<ECodingAPIDomain, TArray<FString>> GetAllMappings() const;

    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    int32 GetTotalFunctionCount() const;

    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    TMap<ECodingAPIDomain, int32> GetFunctionCountPerDomain() const;

    // 生成報告
    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    FString GenerateMappingReport() const;

    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    bool ExportMappingsToJSON(const FString& FilePath) const;

    // 驗證
    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    bool ValidateMappings() const;

    UFUNCTION(BlueprintCallable, Category = "Coding API Mappings")
    TArray<FString> FindUnmappedFunctions() const;

private:
    // 映射存儲
    UPROPERTY()
    TMap<ECodingAPIDomain, TMap<FString, FString>> FunctionMappings;

    UPROPERTY()
    TMap<FString, ECodingAPIDomain> ReverseMappings;

    // 各域映射結構
    UPROPERTY()
    FCodeGenerationMappings CodeGenerationMap;

    UPROPERTY()
    FAICodeGenerationMappings AICodeGenerationMap;

    UPROPERTY()
    FCodeAnalysisMappings CodeAnalysisMap;

    UPROPERTY()
    FDebuggingMappings DebuggingMap;

    UPROPERTY()
    FCompilationMappings CompilationMap;

    UPROPERTY()
    FVersionControlMappings VersionControlMap;

    UPROPERTY()
    FMultiAgentMappings MultiAgentMap;

    UPROPERTY()
    FScriptIntegrationMappings ScriptIntegrationMap;

    UPROPERTY()
    FAdvancedFeaturesMappings AdvancedFeaturesMap;

    UPROPERTY()
    FLearningSystemMappings LearningSystemMap;

    // 內部方法
    void BuildCodeGenerationMappings();
    void BuildAICodeGenerationMappings();
    void BuildCodeAnalysisMappings();
    void BuildDebuggingMappings();
    void BuildCompilationMappings();
    void BuildVersionControlMappings();
    void BuildMultiAgentMappings();
    void BuildScriptIntegrationMappings();
    void BuildAdvancedFeaturesMappings();
    void BuildLearningSystemMappings();
};

// ============================================================================
// 快速訪問宏
// ============================================================================

#define GET_CODE_GENERATION_API()           (UMingRTSPotatoAICodeGeneratorAPI::Get())
#define GET_AI_CODE_GENERATION_API()        (UMingRTSPotatoAIEnhancedAPI::Get())
#define GET_CODE_ANALYSIS_API()             (UMingRTSCodeReviewAPI::Get())
#define GET_DEBUGGING_API()                 (UMingRTSDebugAPI::Get())
#define GET_COMPILATION_API()               (UMingRTSCompilationAPI::Get())
#define GET_VERSION_CONTROL_API()           (UMingRTSAPISystem::Get())
#define GET_MULTI_AGENT_API()               (UMingRTSBMADSystem::Get())
#define GET_SCRIPT_INTEGRATION_API()        (UMingRTSScriptIntegrationAPI::Get())
#define GET_ADVANCED_FEATURES_API()         (UMingRTSAdvancedAPIFeatures::Get())
#define GET_LEARNING_SYSTEM_API()           (UMingRTSUE5LearningAPI::Get())

// ============================================================================
// 使用示例註釋
// ============================================================================

/*
 * 使用示例:
 * 
 * 1. 初始化 Coding API:
 *    UMingRTSCodingAPI* CodingAPI = NewObject<UMingRTSCodingAPI>();
 *    CodingAPI->InitializeCodingAPI();
 * 
 * 2. 代碼生成:
 *    FCodingAPIResult Result = CodingAPI->GenerateCode(
 *        ECodingAPIDomain::CodeGeneration,
 *        TEXT("Create a player controller class"),
 *        ESupportedLanguage::CPP
 *    );
 * 
 * 3. 執行特定域功能:
 *    TArray<FCodingAPIFunction> DebugFunctions = 
 *        CodingAPI->GetFunctionsByDomain(ECodingAPIDomain::Debugging);
 * 
 * 4. 跨域整合:
 *    FCodingAPIResult Result = CodingAPI->ExecuteCrossDomain(
 *        ECodingAPIDomain::CodeGeneration,
 *        ECodingAPIDomain::CodeAnalysis,
 *        Parameters
 *    );
 * 
 * 5. 獲取映射資訊:
 *    UMingRTSCodingAPIMappings* Mappings = NewObject<UMingRTSCodingAPIMappings>();
 *    Mappings->InitializeMappings();
 *    FString FunctionPath = Mappings->GetFunctionMapping(
 *        ECodingAPIDomain::CodeGeneration,
 *        TEXT("GenerateUE5Class")
 *    );
 */
