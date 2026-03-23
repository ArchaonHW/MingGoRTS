// Copyright (c) 2026 MingGoRTS. All rights reserved.
// MingRTSCodingAPI.h - 統合程式開發 API 系統
// 整合專案中所有程式語言功能，統一分配給對應的 Coding API

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSCppReferenceIntegration.h"
#include "MingRTSCodingAPI.generated.h"

// ============================================================================
// SECTION 1: Coding API 類型枚舉
// ============================================================================

UENUM(BlueprintType)
enum class ECodingAPIDomain : uint8
{
    // 代碼生成與模板
    CodeGeneration      UMETA(DisplayName = "代碼生成"),
    TemplateManagement  UMETA(DisplayName = "模板管理"),
    CodeTemplates       UMETA(DisplayName = "代碼模板"),

    // 代碼分析與審查
    CodeAnalysis        UMETA(DisplayName = "代碼分析"),
    CodeReview          UMETA(DisplayName = "代碼審查"),
    StaticAnalysis      UMETA(DisplayName = "靜態分析"),
    QualityCheck        UMETA(DisplayName = "品質檢查"),

    // 除錯與優化
    Debugging           UMETA(DisplayName = "除錯"),
    Performance         UMETA(DisplayName = "性能優化"),
    Profiling           UMETA(DisplayName = "性能分析"),
    Optimization        UMETA(DisplayName = "優化"),

    // 編譯與構建
    Compilation         UMETA(DisplayName = "編譯"),
    BuildSystem         UMETA(DisplayName = "構建系統"),
    Linking             UMETA(DisplayName = "連結"),

    // 版本控制
    VersionControl      UMETA(DisplayName = "版本控制"),
    GitIntegration      UMETA(DisplayName = "Git整合"),
    BranchManagement    UMETA(DisplayName = "分支管理"),

    // 規範與標準
    CodingStandards     UMETA(DisplayName = "編碼規範"),
    NamingConventions   UMETA(DisplayName = "命名規範"),
    Documentation         UMETA(DisplayName = "文檔規範"),

    // AI 輔助開發
    AICodeGeneration    UMETA(DisplayName = "AI代碼生成"),
    SmartRefactoring    UMETA(DisplayName = "智能重構"),
    AutoCompletion      UMETA(DisplayName = "自動完成"),
    CodePrediction      UMETA(DisplayName = "代碼預測"),

    // 腳本整合
    ScriptIntegration   UMETA(DisplayName = "腳本整合"),
    PowerShellAPI       UMETA(DisplayName = "PowerShell API"),
    PythonIntegration   UMETA(DisplayName = "Python整合"),

    // 安全與測試
    SecurityCheck       UMETA(DisplayName = "安全檢查"),
    Testing             UMETA(DisplayName = "測試"),
    UnitTesting         UMETA(DisplayName = "單元測試"),

    // 學習系統
    LearningSystem      UMETA(DisplayName = "學習系統"),
    UE5Learning         UMETA(DisplayName = "UE5學習"),
    KnowledgeBase       UMETA(DisplayName = "知識庫"),

    // 多智能體協作
    MultiAgent          UMETA(DisplayName = "多智能體"),
    BMADSystem          UMETA(DisplayName = "BMAD系統"),
    AgentCollaboration  UMETA(DisplayName = "智能體協作"),

    // 高級功能
    AdvancedFeatures    UMETA(DisplayName = "高級功能"),
    DataMining          UMETA(DisplayName = "數據挖掘"),
    NLP                 UMETA(DisplayName = "自然語言處理"),
    PatternRecognition  UMETA(DisplayName = "模式識別")
};

UENUM(BlueprintType)
enum class ESupportedLanguage : uint8
{
    CPP             UMETA(DisplayName = "C++"),
    Blueprint       UMETA(DisplayName = "Blueprint"),
    Python          UMETA(DisplayName = "Python"),
    JavaScript      UMETA(DisplayName = "JavaScript"),
    PowerShell      UMETA(DisplayName = "PowerShell"),
    ShaderLanguage  UMETA(DisplayName = "Shader"),
    INI             UMETA(DisplayName = "INI/Config"),
    JSON            UMETA(DisplayName = "JSON"),
    XML             UMETA(DisplayName = "XML"),
    Markdown        UMETA(DisplayName = "Markdown")
};

UENUM(BlueprintType)
enum class ECodingTaskType : uint8
{
    FeatureDevelopment  UMETA(DisplayName = "功能開發"),
    BugFix              UMETA(DisplayName = "錯誤修復"),
    Refactoring         UMETA(DisplayName = "重構"),
    Optimization        UMETA(DisplayName = "優化"),
    Documentation       UMETA(DisplayName = "文檔"),
    Testing             UMETA(DisplayName = "測試"),
    Integration         UMETA(DisplayName = "整合"),
    Deployment          UMETA(DisplayName = "部署"),
    Maintenance         UMETA(DisplayName = "維護"),
    Research            UMETA(DisplayName = "研究")
};

// ============================================================================
// SECTION 2: Coding API 功能註冊結構
// ============================================================================

USTRUCT(BlueprintType)
struct FCodingAPIFunction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    FString FunctionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    FString FunctionSignature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    ECodingAPIDomain Domain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    ESupportedLanguage Language;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    TArray<FString> Parameters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    FString ReturnType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    bool bIsBlueprintCallable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    FString SourceFile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Function")
    int32 LineNumber;

    FCodingAPIFunction()
    {
        bIsBlueprintCallable = true;
        LineNumber = 0;
    }
};

USTRUCT(BlueprintType)
struct FCodingAPIModule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Module")
    FString ModuleName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Module")
    FString ModuleDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Module")
    ECodingAPIDomain Domain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Module")
    TArray<FCodingAPIFunction> Functions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Module")
    TMap<FString, FString> Metadata;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Module")
    bool bIsActive;

    FCodingAPIModule()
    {
        bIsActive = true;
    }
};

USTRUCT(BlueprintType)
struct FCodingAPIFunctionMapping
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function Mapping")
    ECodingAPIDomain Domain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function Mapping")
    TArray<FString> FunctionNames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function Mapping")
    FString TargetAPIClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function Mapping")
    FString IncludeHeader;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Function Mapping")
    FString UsageExample;

    FCodingAPIFunctionMapping()
    {
    }
};

USTRUCT(BlueprintType)
struct FCodingAPITaskAssignment
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Assignment")
    ECodingTaskType TaskType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Assignment")
    TArray<ECodingAPIDomain> RelevantDomains;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Assignment")
    TArray<FString> SuggestedAPIs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task Assignment")
    FString WorkflowDescription;

    FCodingAPITaskAssignment()
    {
    }
};

// ============================================================================
// SECTION 3: Coding API 執行結果
// ============================================================================

USTRUCT(BlueprintType)
struct FCodingAPIResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Result")
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Result")
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Result")
    ECodingAPIDomain ExecutedDomain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Result")
    TMap<FString, FString> OutputData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Result")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Result")
    FDateTime Timestamp;

    FCodingAPIResult()
    {
        bSuccess = false;
        ExecutionTime = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FCodingAPIMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Metrics")
    int32 TotalFunctions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Metrics")
    int32 ActiveDomains;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Metrics")
    TMap<ECodingAPIDomain, int32> FunctionsPerDomain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Metrics")
    float AverageResponseTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "API Metrics")
    float SuccessRate;

    FCodingAPIMetrics()
    {
        TotalFunctions = 0;
        ActiveDomains = 0;
        AverageResponseTime = 0.0f;
        SuccessRate = 1.0f;
    }
};

// ============================================================================
// SECTION 4: 委派定義
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCodingAPIDomainRegistered, ECodingAPIDomain, Domain, const FString&, ModuleName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCodingAPIFunctionCalled, ECodingAPIDomain, Domain, const FString&, FunctionName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCodingAPITaskCompleted, const FCodingAPIResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCodingAPIMetricsUpdated, const FCodingAPIMetrics&, Metrics);

// ============================================================================
// SECTION 5: Coding API 核心類
// ============================================================================

/**
 * MingRTS Coding API
 * 統合管理所有程式開發相關功能的 API 系統
 * 整合專案中所有程式語言功能並分配給對應的 API
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Coding|Core")
class MINGGORTS_API UMingRTSCodingAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCodingAPI();

    // ==================== 初始化與配置 ====================

    UFUNCTION(BlueprintCallable, Category = "Coding API|Initialization")
    bool InitializeCodingAPI();

    UFUNCTION(BlueprintCallable, Category = "Coding API|Initialization")
    void ShutdownCodingAPI();

    UFUNCTION(BlueprintCallable, Category = "Coding API|Configuration")
    void RegisterAPIModule(const FCodingAPIModule& Module);

    UFUNCTION(BlueprintCallable, Category = "Coding API|Configuration")
    void UnregisterAPIModule(ECodingAPIDomain Domain);

    UFUNCTION(BlueprintCallable, Category = "Coding API|Configuration")
    bool IsDomainActive(ECodingAPIDomain Domain) const;

    // ==================== 功能發現與查詢 ====================

    UFUNCTION(BlueprintCallable, Category = "Coding API|Discovery")
    TArray<FCodingAPIModule> GetAllModules() const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Discovery")
    FCodingAPIModule GetModuleByDomain(ECodingAPIDomain Domain) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Discovery")
    TArray<FCodingAPIFunction> GetFunctionsByDomain(ECodingAPIDomain Domain) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Discovery")
    TArray<FCodingAPIFunction> GetFunctionsByLanguage(ESupportedLanguage Language) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Discovery")
    TArray<FCodingAPIFunction> SearchFunctions(const FString& SearchQuery) const;

    UFUNCTION(BlueprintPure, Category = "Coding API|Discovery")
    bool HasFunctionInDomain(const FString& FunctionName, ECodingAPIDomain Domain) const;

    // ==================== 任務分配與工作流 ====================

    UFUNCTION(BlueprintCallable, Category = "Coding API|Task Management")
    TArray<ECodingAPIDomain> GetDomainsForTask(ECodingTaskType TaskType) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Task Management")
    FString GetRecommendedWorkflow(ECodingTaskType TaskType) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Task Management")
    TArray<FString> GetSuggestedAPIsForTask(ECodingTaskType TaskType, ESupportedLanguage Language) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Task Management")
    FCodingAPITaskAssignment CreateTaskAssignment(ECodingTaskType TaskType, const TMap<FString, FString>& Requirements);

    // ==================== 跨域 API 調用 ====================

    UFUNCTION(BlueprintCallable, Category = "Coding API|Cross-Domain")
    FCodingAPIResult ExecuteCrossDomain(ECodingAPIDomain SourceDomain, ECodingAPIDomain TargetDomain, const TMap<FString, FString>& Parameters);

    UFUNCTION(BlueprintCallable, Category = "Coding API|Cross-Domain")
    TArray<FCodingAPIResult> ExecutePipeline(const TArray<ECodingAPIDomain>& Pipeline, const TMap<FString, FString>& InitialParameters);

    UFUNCTION(BlueprintCallable, Category = "Coding API|Cross-Domain")
    bool CanDomainsIntegrate(ECodingAPIDomain DomainA, ECodingAPIDomain DomainB) const;

    // ==================== API 功能調用入口 ====================

    // 代碼生成入口
    UFUNCTION(BlueprintCallable, Category = "Coding API|Code Generation")
    FCodingAPIResult GenerateCode(ECodingAPIDomain Domain, const FString& Requirement, ESupportedLanguage Language);

    // 代碼分析入口
    UFUNCTION(BlueprintCallable, Category = "Coding API|Code Analysis")
    FCodingAPIResult AnalyzeCode(const FString& Code, ECodingAPIDomain AnalysisType);

    // 除錯入口
    UFUNCTION(BlueprintCallable, Category = "Coding API|Debugging")
    FCodingAPIResult DebugCode(const FString& Code, const FString& ErrorMessage);

    // 編譯入口
    UFUNCTION(BlueprintCallable, Category = "Coding API|Compilation")
    FCodingAPIResult CompileProject(const FString& ProjectPath, const TMap<FString, FString>& Options);

    // 版本控制入口
    UFUNCTION(BlueprintCallable, Category = "Coding API|Version Control")
    FCodingAPIResult ExecuteVersionControl(const FString& Command, const TMap<FString, FString>& Parameters);

    // ==================== 智能推薦系統 ====================

    UFUNCTION(BlueprintCallable, Category = "Coding API|Intelligence")
    TArray<FCodingAPIFunction> GetRecommendedFunctions(const FString& Context) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Intelligence")
    ECodingAPIDomain SuggestBestDomain(const FString& TaskDescription) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Intelligence")
    FString GenerateAPICodeExample(ECodingAPIDomain Domain, const FString& FunctionName) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Intelligence")
    void LearnFromUsage(const FString& FunctionName, bool bWasSuccessful);

    // ==================== 報告與統計 ====================

    UFUNCTION(BlueprintCallable, Category = "Coding API|Reporting")
    FCodingAPIMetrics GetAPIMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Reporting")
    FString GenerateAPIReport() const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Reporting")
    bool ExportAPIDocumentation(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "Coding API|Reporting")
    TMap<ECodingAPIDomain, int32> GetUsageStatistics() const;

    // ==================== 事件 ====================

    UPROPERTY(BlueprintAssignable, Category = "Coding API Events")
    FOnCodingAPIDomainRegistered OnDomainRegistered;

    UPROPERTY(BlueprintAssignable, Category = "Coding API Events")
    FOnCodingAPIFunctionCalled OnFunctionCalled;

    UPROPERTY(BlueprintAssignable, Category = "Coding API Events")
    FOnCodingAPITaskCompleted OnTaskCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Coding API Events")
    FOnCodingAPIMetricsUpdated OnMetricsUpdated;

protected:
    // ==================== 內部方法 ====================

    virtual void BeginDestroy() override;

    void InitializeDefaultModules();
    void RegisterAllExistingAPIs();
    void BuildFunctionRegistry();
    void UpdateMetrics();

    // 子系統引用獲取
    UObject* GetSubSystem(ECodingAPIDomain Domain) const;
    void RegisterSubSystem(ECodingAPIDomain Domain, UObject* System);

    // 智能匹配
    float CalculateDomainRelevance(ECodingAPIDomain Domain, const FString& TaskDescription) const;
    TArray<FCodingAPIFunction> FindBestMatches(const FString& Query, int32 MaxResults = 5) const;

    // 工作流執行
    FCodingAPIResult ExecuteInDomain(ECodingAPIDomain Domain, const FString& Operation, const TMap<FString, FString>& Params);
    bool ValidateDomainIntegration(ECodingAPIDomain Source, ECodingAPIDomain Target) const;

    // ==================== C++ Reference 整合 ====================
    
    UFUNCTION(BlueprintCallable, Category = "Coding API|CppReference")
    bool LoadCppReferenceDocumentation();
    
    UFUNCTION(BlueprintCallable, Category = "Coding API|CppReference")
    FString GenerateCodeWithReference(ECodingAPIDomain Domain, const FString& Topic, const FString& Context);
    
    UFUNCTION(BlueprintCallable, Category = "Coding API|CppReference")
    TArray<FString> GetCppReferenceTopics(ECodingTaskType TaskType) const;

private:
    // ==================== 成員變量 ====================

    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    TObjectPtr<UMingRTSCppReferenceIntegration> CppReferenceIntegration;

    UPROPERTY()
    TMap<ECodingAPIDomain, FCodingAPIModule> RegisteredModules;

    UPROPERTY()
    TMap<ECodingAPIDomain, UObject*> DomainSubSystems;

    UPROPERTY()
    TArray<FCodingAPIFunction> FunctionRegistry;

    UPROPERTY()
    TMap<ECodingTaskType, FCodingAPITaskAssignment> TaskWorkflows;

    UPROPERTY()
    FCodingAPIMetrics CurrentMetrics;

    UPROPERTY()
    TMap<FString, int32> FunctionUsageCount;

    UPROPERTY()
    TMap<FString, float> FunctionSuccessRate;

    // 日誌與追蹤
    void LogAPICall(ECodingAPIDomain Domain, const FString& Function, const FCodingAPIResult& Result);
    void TrackMetrics(ECodingAPIDomain Domain, float ExecutionTime, bool bSuccess);
};

// ============================================================================
// SECTION 6: Coding API 功能分配表（靜態映射）
// ============================================================================

/*
 * 功能分配總覽：
 * 
 * ECodingAPIDomain::CodeGeneration
 *   - 來源: MingRTSPotatoAICodeGeneratorAPI
 *   - 功能: AI代碼生成、模板選擇、質量檢查
 * 
 * ECodingAPIDomain::CodeAnalysis
 *   - 來源: MingRTSCodeReviewAPI, MingRTSCodeStandardsAPI
 *   - 功能: 代碼審查、規範檢查、靜態分析
 * 
 * ECodingAPIDomain::Debugging
 *   - 來源: MingRTSDebugAPI
 *   - 功能: 斷點管理、變量監控、性能分析
 * 
 * ECodingAPIDomain::Compilation
 *   - 來源: MingRTSCompilationAPI
 *   - 功能: 編譯、優化、錯誤修復
 * 
 * ECodingAPIDomain::VersionControl
 *   - 來源: MingRTSAPISystem (Version Control)
 *   - 功能: Git操作、分支管理、提交追蹤
 * 
 * ECodingAPIDomain::AICodeGeneration
 *   - 來源: MingRTSPotatoAIEnhancedAPI
 *   - 功能: 智能代碼生成、需求分析
 * 
 * ECodingAPIDomain::MultiAgent
 *   - 來源: MingRTSBMADSystem
 *   - 功能: 多智能體協作、角色管理
 * 
 * ECodingAPIDomain::ScriptIntegration
 *   - 來源: MingRTSScriptIntegrationAPI
 *   - 功能: PowerShell/Python 腳本整合
 * 
 * ECodingAPIDomain::AdvancedFeatures
 *   - 來源: MingRTSAdvancedAPIFeatures
 *   - 功能: 數據挖掘、模式識別、安全分析
 * 
 * ECodingAPIDomain::LearningSystem
 *   - 來源: MingRTSUE5LearningAPI
 *   - 功能: UE5學習、文檔生成、知識累積
 */

// ============================================================================
// SECTION 7: 快速使用宏與輔助函數
// ============================================================================

#define GET_CODING_API() (UMingRTSCodingAPI::GetCodingAPI())
#define CALL_CODING_API(Domain, Operation, Params) (UMingRTSCodingAPI::GetCodingAPI()->ExecuteInDomain(Domain, Operation, Params))

namespace MingCodingAPI
{
    // 輔助函數命名空間
    MINGGORTS_API FString DomainToString(ECodingAPIDomain Domain);
    MINGGORTS_API ECodingAPIDomain StringToDomain(const FString& DomainString);
    MINGGORTS_API FString LanguageToString(ESupportedLanguage Language);
    MINGGORTS_API ESupportedLanguage StringToLanguage(const FString& LanguageString);
    MINGGORTS_API FString TaskTypeToString(ECodingTaskType TaskType);
    MINGGORTS_API ECodingTaskType StringToTaskType(const FString& TaskString);
}
