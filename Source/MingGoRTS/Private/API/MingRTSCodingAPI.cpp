// Copyright (c) 2026 MingGoRTS. All rights reserved.
// MingRTSCodingAPI.cpp - 統合程式開發 API 實現

#include "API/MingRTSCodingAPI.h"
#include "API/MingRTSAPIOrchestrator.h"
#include "API/MingRTSPotatoAICodeGeneratorAPI.h"
#include "API/MingRTSCodeReviewAPI.h"
#include "API/MingRTSCodeStandardsAPI.h"
#include "API/MingRTSDebugAPI.h"
#include "API/MingRTSCompilationAPI.h"
#include "API/MingRTSBMADSystem.h"
#include "Engine/Engine.h"

// ============================================================================
// 靜態單例實例
// ============================================================================

namespace
{
    UMingRTSCodingAPI* GCodingAPIInstance = nullptr;
}

UMingRTSCodingAPI* UMingRTSCodingAPI::GetCodingAPI()
{
    if (!GCodingAPIInstance)
    {
        GCodingAPIInstance = NewObject<UMingRTSCodingAPI>();
        GCodingAPIInstance->AddToRoot();
    }
    return GCodingAPIInstance;
}

// ============================================================================
// 構造與析構
// ============================================================================

UMingRTSCodingAPI::UMingRTSCodingAPI()
    : bIsInitialized(false)
{
}

void UMingRTSCodingAPI::BeginDestroy()
{
    if (bIsInitialized)
    {
        ShutdownCodingAPI();
    }
    Super::BeginDestroy();
}

// ============================================================================
// 初始化與配置
// ============================================================================

bool UMingRTSCodingAPI::InitializeCodingAPI()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("CodingAPI already initialized"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing MingRTS Coding API..."));

    // 初始化預設模組
    InitializeDefaultModules();

    // 註冊所有現有 API
    RegisterAllExistingAPIs();

    // 構建功能註冊表
    BuildFunctionRegistry();

    // 更新指標
    UpdateMetrics();

    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("Coding API initialized successfully with %d modules"), RegisteredModules.Num());
    return true;
}

void UMingRTSCodingAPI::ShutdownCodingAPI()
{
    if (!bIsInitialized)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Shutting down Coding API..."));

    // 清除所有註冊
    RegisteredModules.Empty();
    DomainSubSystems.Empty();
    FunctionRegistry.Empty();

    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("Coding API shutdown complete"));
}

void UMingRTSCodingAPI::RegisterAPIModule(const FCodingAPIModule& Module)
{
    if (RegisteredModules.Contains(Module.Domain))
    {
        UE_LOG(LogTemp, Warning, TEXT("Module for domain %d already registered, updating..."), static_cast<int32>(Module.Domain));
    }

    RegisteredModules.Add(Module.Domain, Module);

    // 將功能添加到全局註冊表
    for (const auto& Function : Module.Functions)
    {
        FunctionRegistry.Add(Function);
    }

    // 觸發事件
    OnDomainRegistered.Broadcast(Module.Domain, Module.ModuleName);

    UE_LOG(LogTemp, Log, TEXT("Registered API module: %s with %d functions"), *Module.ModuleName, Module.Functions.Num());
}

void UMingRTSCodingAPI::UnregisterAPIModule(ECodingAPIDomain Domain)
{
    if (RegisteredModules.Contains(Domain))
    {
        const FString ModuleName = RegisteredModules[Domain].ModuleName;
        RegisteredModules.Remove(Domain);
        UE_LOG(LogTemp, Log, TEXT("Unregistered API module: %s"), *ModuleName);
    }
}

bool UMingRTSCodingAPI::IsDomainActive(ECodingAPIDomain Domain) const
{
    if (const FCodingAPIModule* Module = RegisteredModules.Find(Domain))
    {
        return Module->bIsActive;
    }
    return false;
}

// ============================================================================
// 功能發現與查詢
// ============================================================================

TArray<FCodingAPIModule> UMingRTSCodingAPI::GetAllModules() const
{
    TArray<FCodingAPIModule> Result;
    RegisteredModules.GenerateValueArray(Result);
    return Result;
}

FCodingAPIModule UMingRTSCodingAPI::GetModuleByDomain(ECodingAPIDomain Domain) const
{
    if (const FCodingAPIModule* Module = RegisteredModules.Find(Domain))
    {
        return *Module;
    }
    return FCodingAPIModule();
}

TArray<FCodingAPIFunction> UMingRTSCodingAPI::GetFunctionsByDomain(ECodingAPIDomain Domain) const
{
    TArray<FCodingAPIFunction> Result;

    for (const auto& Function : FunctionRegistry)
    {
        if (Function.Domain == Domain)
        {
            Result.Add(Function);
        }
    }

    return Result;
}

TArray<FCodingAPIFunction> UMingRTSCodingAPI::GetFunctionsByLanguage(ESupportedLanguage Language) const
{
    TArray<FCodingAPIFunction> Result;

    for (const auto& Function : FunctionRegistry)
    {
        if (Function.Language == Language)
        {
            Result.Add(Function);
        }
    }

    return Result;
}

TArray<FCodingAPIFunction> UMingRTSCodingAPI::SearchFunctions(const FString& SearchQuery) const
{
    TArray<FCodingAPIFunction> Result;
    FString LowerQuery = SearchQuery.ToLower();

    for (const auto& Function : FunctionRegistry)
    {
        if (Function.FunctionName.ToLower().Contains(LowerQuery) ||
            Function.Description.ToLower().Contains(LowerQuery) ||
            Function.FunctionSignature.ToLower().Contains(LowerQuery))
        {
            Result.Add(Function);
        }
    }

    return Result;
}

bool UMingRTSCodingAPI::HasFunctionInDomain(const FString& FunctionName, ECodingAPIDomain Domain) const
{
    for (const auto& Function : FunctionRegistry)
    {
        if (Function.FunctionName == FunctionName && Function.Domain == Domain)
        {
            return true;
        }
    }
    return false;
}

// ============================================================================
// 任務分配與工作流
// ============================================================================

TArray<ECodingAPIDomain> UMingRTSCodingAPI::GetDomainsForTask(ECodingTaskType TaskType) const
{
    TArray<ECodingAPIDomain> Domains;

    switch (TaskType)
    {
    case ECodingTaskType::FeatureDevelopment:
        Domains.Add(ECodingAPIDomain::CodeGeneration);
        Domains.Add(ECodingAPIDomain::AICodeGeneration);
        Domains.Add(ECodingAPIDomain::CodeAnalysis);
        break;

    case ECodingTaskType::BugFix:
        Domains.Add(ECodingAPIDomain::Debugging);
        Domains.Add(ECodingAPIDomain::CodeAnalysis);
        Domains.Add(ECodingAPIDomain::Compilation);
        break;

    case ECodingTaskType::Refactoring:
        Domains.Add(ECodingAPIDomain::SmartRefactoring);
        Domains.Add(ECodingAPIDomain::CodeAnalysis);
        Domains.Add(ECodingAPIDomain::CodeReview);
        break;

    case ECodingTaskType::Optimization:
        Domains.Add(ECodingAPIDomain::Performance);
        Domains.Add(ECodingAPIDomain::Profiling);
        Domains.Add(ECodingAPIDomain::Optimization);
        break;

    case ECodingTaskType::Documentation:
        Domains.Add(ECodingAPIDomain::Documentation);
        Domains.Add(ECodingAPIDomain::LearningSystem);
        break;

    case ECodingTaskType::Testing:
        Domains.Add(ECodingAPIDomain::Testing);
        Domains.Add(ECodingAPIDomain::UnitTesting);
        Domains.Add(ECodingAPIDomain::CodeAnalysis);
        break;

    case ECodingTaskType::Integration:
        Domains.Add(ECodingAPIDomain::Compilation);
        Domains.Add(ECodingAPIDomain::ScriptIntegration);
        Domains.Add(ECodingAPIDomain::VersionControl);
        break;

    case ECodingTaskType::Deployment:
        Domains.Add(ECodingAPIDomain::BuildSystem);
        Domains.Add(ECodingAPIDomain::VersionControl);
        break;

    case ECodingTaskType::Maintenance:
        Domains.Add(ECodingAPIDomain::CodeAnalysis);
        Domains.Add(ECodingAPIDomain::SecurityCheck);
        Domains.Add(ECodingAPIDomain::Performance);
        break;

    case ECodingTaskType::Research:
        Domains.Add(ECodingAPIDomain::AdvancedFeatures);
        Domains.Add(ECodingAPIDomain::LearningSystem);
        Domains.Add(ECodingAPIDomain::MultiAgent);
        break;

    default:
        break;
    }

    return Domains;
}

FString UMingRTSCodingAPI::GetRecommendedWorkflow(ECodingTaskType TaskType) const
{
    switch (TaskType)
    {
    case ECodingTaskType::FeatureDevelopment:
        return TEXT("1. AI代碼生成 (AICodeGeneration) -> 2. 代碼審查 (CodeReview) -> 3. 編譯測試 (Compilation) -> 4. 版本控制 (VersionControl)");

    case ECodingTaskType::BugFix:
        return TEXT("1. 除錯分析 (Debugging) -> 2. 代碼審查 (CodeReview) -> 3. 修復驗證 (Compilation)");

    case ECodingTaskType::Refactoring:
        return TEXT("1. 靜態分析 (StaticAnalysis) -> 2. 智能重構 (SmartRefactoring) -> 3. 回歸測試 (Testing)");

    case ECodingTaskType::Optimization:
        return TEXT("1. 性能分析 (Profiling) -> 2. 優化建議 (Optimization) -> 3. 驗證測試 (Testing)");

    default:
        return TEXT("使用通用工作流: 分析 -> 實現 -> 測試 -> 提交");
    }
}

TArray<FString> UMingRTSCodingAPI::GetSuggestedAPIsForTask(ECodingTaskType TaskType, ESupportedLanguage Language) const
{
    TArray<FString> Suggestions;
    TArray<ECodingAPIDomain> Domains = GetDomainsForTask(TaskType);

    for (const auto& Domain : Domains)
    {
        if (const FCodingAPIModule* Module = RegisteredModules.Find(Domain))
        {
            Suggestions.Add(FString::Printf(TEXT("%s (%s)"), *Module->ModuleName, *MingCodingAPI::DomainToString(Domain)));
        }
    }

    return Suggestions;
}

FCodingAPITaskAssignment UMingRTSCodingAPI::CreateTaskAssignment(ECodingTaskType TaskType, const TMap<FString, FString>& Requirements)
{
    FCodingAPITaskAssignment Assignment;
    Assignment.TaskType = TaskType;
    Assignment.RelevantDomains = GetDomainsForTask(TaskType);
    Assignment.SuggestedAPIs = GetSuggestedAPIsForTask(TaskType, ESupportedLanguage::CPP);
    Assignment.WorkflowDescription = GetRecommendedWorkflow(TaskType);

    return Assignment;
}

// ============================================================================
// 跨域 API 調用
// ============================================================================

FCodingAPIResult UMingRTSCodingAPI::ExecuteCrossDomain(ECodingAPIDomain SourceDomain, ECodingAPIDomain TargetDomain, const TMap<FString, FString>& Parameters)
{
    FCodingAPIResult Result;
    Result.ExecutedDomain = TargetDomain;
    Result.Timestamp = FDateTime::Now();

    if (!CanDomainsIntegrate(SourceDomain, TargetDomain))
    {
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("Domains %d and %d cannot integrate"), static_cast<int32>(SourceDomain), static_cast<int32>(TargetDomain));
        return Result;
    }

    // 執行目標域操作
    Result = ExecuteInDomain(TargetDomain, TEXT("cross_domain_execute"), Parameters);

    // 觸發事件
    OnFunctionCalled.Broadcast(SourceDomain, TEXT("cross_domain_call"));
    OnTaskCompleted.Broadcast(Result);

    return Result;
}

TArray<FCodingAPIResult> UMingRTSCodingAPI::ExecutePipeline(const TArray<ECodingAPIDomain>& Pipeline, const TMap<FString, FString>& InitialParameters)
{
    TArray<FCodingAPIResult> Results;
    TMap<FString, FString> CurrentParams = InitialParameters;

    for (const auto& Domain : Pipeline)
    {
        FCodingAPIResult Result = ExecuteInDomain(Domain, TEXT("pipeline_execute"), CurrentParams);
        Results.Add(Result);

        if (!Result.bSuccess)
        {
            UE_LOG(LogTemp, Warning, TEXT("Pipeline failed at domain %d"), static_cast<int32>(Domain));
            break;
        }

        // 將輸出傳遞到下一步
        for (const auto& Output : Result.OutputData)
        {
            CurrentParams.Add(Output.Key, Output.Value);
        }
    }

    return Results;
}

bool UMingRTSCodingAPI::CanDomainsIntegrate(ECodingAPIDomain DomainA, ECodingAPIDomain DomainB) const
{
    // 定義可以整合的域對
    static TMap<ECodingAPIDomain, TArray<ECodingAPIDomain>> IntegrationMap;

    if (IntegrationMap.IsEmpty())
    {
        // 代碼生成可以與分析、審查整合
        IntegrationMap.Add(ECodingAPIDomain::CodeGeneration, { ECodingAPIDomain::CodeAnalysis, ECodingAPIDomain::CodeReview });
        IntegrationMap.Add(ECodingAPIDomain::AICodeGeneration, { ECodingAPIDomain::CodeReview, ECodingAPIDomain::SmartRefactoring });

        // 除錯可以與編譯、分析整合
        IntegrationMap.Add(ECodingAPIDomain::Debugging, { ECodingAPIDomain::Compilation, ECodingAPIDomain::CodeAnalysis });

        // 編譯可以與除錯、優化整合
        IntegrationMap.Add(ECodingAPIDomain::Compilation, { ECodingAPIDomain::Debugging, ECodingAPIDomain::Optimization });

        // 版本控制可以與所有域整合
        IntegrationMap.Add(ECodingAPIDomain::VersionControl, {
            ECodingAPIDomain::CodeGeneration, ECodingAPIDomain::CodeAnalysis, ECodingAPIDomain::Debugging,
            ECodingAPIDomain::Compilation, ECodingAPIDomain::Testing
        });

        // 測試可以與分析、編譯整合
        IntegrationMap.Add(ECodingAPIDomain::Testing, { ECodingAPIDomain::CodeAnalysis, ECodingAPIDomain::Compilation });

        // 多智能體可以與所有創造性任務整合
        IntegrationMap.Add(ECodingAPIDomain::MultiAgent, {
            ECodingAPIDomain::AICodeGeneration, ECodingAPIDomain::CodeGeneration, ECodingAPIDomain::LearningSystem
        });
    }

    if (const TArray<ECodingAPIDomain>* CompatibleDomains = IntegrationMap.Find(DomainA))
    {
        return CompatibleDomains->Contains(DomainB);
    }

    return false;
}

// ============================================================================
// API 功能調用入口
// ============================================================================

FCodingAPIResult UMingRTSCodingAPI::GenerateCode(ECodingAPIDomain Domain, const FString& Requirement, ESupportedLanguage Language)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("requirement"), Requirement);
    Params.Add(TEXT("language"), MingCodingAPI::LanguageToString(Language));

    return ExecuteInDomain(Domain, TEXT("generate"), Params);
}

FCodingAPIResult UMingRTSCodingAPI::AnalyzeCode(const FString& Code, ECodingAPIDomain AnalysisType)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("code"), Code);

    return ExecuteInDomain(AnalysisType, TEXT("analyze"), Params);
}

FCodingAPIResult UMingRTSCodingAPI::DebugCode(const FString& Code, const FString& ErrorMessage)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("code"), Code);
    Params.Add(TEXT("error"), ErrorMessage);

    return ExecuteInDomain(ECodingAPIDomain::Debugging, TEXT("debug"), Params);
}

FCodingAPIResult UMingRTSCodingAPI::CompileProject(const FString& ProjectPath, const TMap<FString, FString>& Options)
{
    TMap<FString, FString> Params = Options;
    Params.Add(TEXT("project_path"), ProjectPath);

    return ExecuteInDomain(ECodingAPIDomain::Compilation, TEXT("compile"), Params);
}

FCodingAPIResult UMingRTSCodingAPI::ExecuteVersionControl(const FString& Command, const TMap<FString, FString>& Parameters)
{
    TMap<FString, FString> Params = Parameters;
    Params.Add(TEXT("command"), Command);

    return ExecuteInDomain(ECodingAPIDomain::VersionControl, TEXT("execute"), Params);
}

// ============================================================================
// 智能推薦系統
// ============================================================================

TArray<FCodingAPIFunction> UMingRTSCodingAPI::GetRecommendedFunctions(const FString& Context) const
{
    // 基於上下文智能推薦功能
    TArray<FCodingAPIFunction> Recommendations;

    FString LowerContext = Context.ToLower();

    // 關鍵詞匹配
    if (LowerContext.Contains(TEXT("generate")) || LowerContext.Contains(TEXT("create")))
    {
        Recommendations.Append(GetFunctionsByDomain(ECodingAPIDomain::CodeGeneration));
        Recommendations.Append(GetFunctionsByDomain(ECodingAPIDomain::AICodeGeneration));
    }

    if (LowerContext.Contains(TEXT("debug")) || LowerContext.Contains(TEXT("error")) || LowerContext.Contains(TEXT("fix")))
    {
        Recommendations.Append(GetFunctionsByDomain(ECodingAPIDomain::Debugging));
    }

    if (LowerContext.Contains(TEXT("compile")) || LowerContext.Contains(TEXT("build")))
    {
        Recommendations.Append(GetFunctionsByDomain(ECodingAPIDomain::Compilation));
    }

    if (LowerContext.Contains(TEXT("review")) || LowerContext.Contains(TEXT("analyze")) || LowerContext.Contains(TEXT("check")))
    {
        Recommendations.Append(GetFunctionsByDomain(ECodingAPIDomain::CodeReview));
        Recommendations.Append(GetFunctionsByDomain(ECodingAPIDomain::CodeAnalysis));
    }

    return Recommendations;
}

ECodingAPIDomain UMingRTSCodingAPI::SuggestBestDomain(const FString& TaskDescription) const
{
    float BestScore = 0.0f;
    ECodingAPIDomain BestDomain = ECodingAPIDomain::CodeGeneration;

    for (const auto& Module : RegisteredModules)
    {
        float Score = CalculateDomainRelevance(Module.Key, TaskDescription);
        if (Score > BestScore)
        {
            BestScore = Score;
            BestDomain = Module.Key;
        }
    }

    return BestDomain;
}

FString UMingRTSCodingAPI::GenerateAPICodeExample(ECodingAPIDomain Domain, const FString& FunctionName) const
{
    // 生成 API 使用示例代碼
    FString Example = FString::Printf(
        TEXT("// 使用 %s API\n")
        TEXT("UMingRTSCodingAPI* API = UMingRTSCodingAPI::GetCodingAPI();\n")
        TEXT("API->InitializeCodingAPI();\n\n")
        TEXT("// 調用 %s 功能\n")
        TEXT("FCodingAPIResult Result = API->ExecuteInDomain(%s, TEXT(\"%s\"), Parameters);\n\n")
        TEXT("if (Result.bSuccess)\n")
        TEXT("{\n")
        TEXT("    // 處理結果\n")
        TEXT("    FString Output = Result.Message;\n")
        TEXT("}\n"),
        *MingCodingAPI::DomainToString(Domain),
        *FunctionName,
        *MingCodingAPI::DomainToString(Domain),
        *FunctionName
    );

    return Example;
}

void UMingRTSCodingAPI::LearnFromUsage(const FString& FunctionName, bool bWasSuccessful)
{
    // 更新功能使用統計
    int32& Count = FunctionUsageCount.FindOrAdd(FunctionName);
    Count++;

    // 更新成功率
    float& Rate = FunctionSuccessRate.FindOrAdd(FunctionName);
    Rate = (Rate * (Count - 1) + (bWasSuccessful ? 1.0f : 0.0f)) / Count;
}

// ============================================================================
// 報告與統計
// ============================================================================

FCodingAPIMetrics UMingRTSCodingAPI::GetAPIMetrics() const
{
    return CurrentMetrics;
}

FString UMingRTSCodingAPI::GenerateAPIReport() const
{
    FString Report = TEXT("============================================\n");
    Report += TEXT("MingRTS Coding API 報告\n");
    Report += TEXT("============================================\n\n");

    Report += FString::Printf(TEXT("總功能數: %d\n"), CurrentMetrics.TotalFunctions);
    Report += FString::Printf(TEXT("活躍域數: %d\n"), CurrentMetrics.ActiveDomains);
    Report += FString::Printf(TEXT("平均響應時間: %.2f ms\n"), CurrentMetrics.AverageResponseTime);
    Report += FString::Printf(TEXT("成功率: %.1f%%\n\n"), CurrentMetrics.SuccessRate * 100.0f);

    Report += TEXT("各域功能分布:\n");
    for (const auto& Pair : CurrentMetrics.FunctionsPerDomain)
    {
        Report += FString::Printf(TEXT("  - %s: %d 個功能\n"),
            *MingCodingAPI::DomainToString(Pair.Key),
            Pair.Value);
    }

    Report += TEXT("\n已註冊模組:\n");
    for (const auto& Module : RegisteredModules)
    {
        Report += FString::Printf(TEXT("  - %s (%s): %d 個功能\n"),
            *Module.Value.ModuleName,
            *MingCodingAPI::DomainToString(Module.Key),
            Module.Value.Functions.Num());
    }

    return Report;
}

bool UMingRTSCodingAPI::ExportAPIDocumentation(const FString& FilePath) const
{
    // 生成 API 文檔並導出
    FString Documentation = GenerateAPIReport();

    // 添加詳細功能文檔
    Documentation += TEXT("\n\n============================================\n");
    Documentation += TEXT("詳細功能文檔\n");
    Documentation += TEXT("============================================\n\n");

    for (const auto& Function : FunctionRegistry)
    {
        Documentation += FString::Printf(
            TEXT("功能: %s\n")
            TEXT("  域: %s\n")
            TEXT("  語言: %s\n")
            TEXT("  簽名: %s\n")
            TEXT("  描述: %s\n\n"),
            *Function.FunctionName,
            *MingCodingAPI::DomainToString(Function.Domain),
            *MingCodingAPI::LanguageToString(Function.Language),
            *Function.FunctionSignature,
            *Function.Description
        );
    }

    // 寫入文件
    // 注意：實際文件寫入需要平台相關代碼
    UE_LOG(LogTemp, Log, TEXT("API Documentation would be exported to: %s"), *FilePath);
    UE_LOG(LogTemp, Log, TEXT("Documentation length: %d characters"), Documentation.Len());

    return true;
}

TMap<ECodingAPIDomain, int32> UMingRTSCodingAPI::GetUsageStatistics() const
{
    TMap<ECodingAPIDomain, int32> Stats;

    for (const auto& Pair : FunctionUsageCount)
    {
        // 解析功能名稱以確定域
        for (const auto& Function : FunctionRegistry)
        {
            if (Function.FunctionName == Pair.Key)
            {
                int32& Count = Stats.FindOrAdd(Function.Domain);
                Count += Pair.Value;
                break;
            }
        }
    }

    return Stats;
}

// ============================================================================
// 內部方法
// ============================================================================

void UMingRTSCodingAPI::InitializeDefaultModules()
{
    // 創建預設模組配置
    {
        FCodingAPIModule Module;
        Module.ModuleName = TEXT("Code Generation System");
        Module.Domain = ECodingAPIDomain::CodeGeneration;
        Module.ModuleDescription = TEXT("AI-powered code generation and template management");
        Module.bIsActive = true;
        RegisteredModules.Add(Module.Domain, Module);
    }

    {
        FCodingAPIModule Module;
        Module.ModuleName = TEXT("Code Analysis System");
        Module.Domain = ECodingAPIDomain::CodeAnalysis;
        Module.ModuleDescription = TEXT("Static analysis and code quality checking");
        Module.bIsActive = true;
        RegisteredModules.Add(Module.Domain, Module);
    }

    {
        FCodingAPIModule Module;
        Module.ModuleName = TEXT("Debug System");
        Module.Domain = ECodingAPIDomain::Debugging;
        Module.ModuleDescription = TEXT("Intelligent debugging with self-learning");
        Module.bIsActive = true;
        RegisteredModules.Add(Module.Domain, Module);
    }

    {
        FCodingAPIModule Module;
        Module.ModuleName = TEXT("Compilation System");
        Module.Domain = ECodingAPIDomain::Compilation;
        Module.ModuleDescription = TEXT("Smart compilation with learning capabilities");
        Module.bIsActive = true;
        RegisteredModules.Add(Module.Domain, Module);
    }

    {
        FCodingAPIModule Module;
        Module.ModuleName = TEXT("Version Control System");
        Module.Domain = ECodingAPIDomain::VersionControl;
        Module.ModuleDescription = TEXT("Git integration and branch management");
        Module.bIsActive = true;
        RegisteredModules.Add(Module.Domain, Module);
    }

    {
        FCodingAPIModule Module;
        Module.ModuleName = TEXT("Multi-Agent System");
        Module.Domain = ECodingAPIDomain::MultiAgent;
        Module.ModuleDescription = TEXT("Brain Multiple Agent Discussion System");
        Module.bIsActive = true;
        RegisteredModules.Add(Module.Domain, Module);
    }
}

void UMingRTSCodingAPI::RegisterAllExistingAPIs()
{
    // 註冊 Potato AI Code Generator API
    if (UObject* PotatoAI = NewObject<UMingRTSPotatoAICodeGeneratorAPI>())
    {
        RegisterSubSystem(ECodingAPIDomain::CodeGeneration, PotatoAI);
        RegisterSubSystem(ECodingAPIDomain::AICodeGeneration, PotatoAI);
    }

    // 註冊 Debug API
    if (UObject* DebugAPI = NewObject<UMingRTSDebugAPI>())
    {
        RegisterSubSystem(ECodingAPIDomain::Debugging, DebugAPI);
    }

    // 註冊 Compilation API
    if (UObject* CompileAPI = NewObject<UMingRTSCompilationAPI>())
    {
        RegisterSubSystem(ECodingAPIDomain::Compilation, CompileAPI);
    }

    // 註冊 Code Review API
    if (UObject* ReviewAPI = NewObject<UMingRTSCodeReviewAPI>())
    {
        RegisterSubSystem(ECodingAPIDomain::CodeReview, ReviewAPI);
        RegisterSubSystem(ECodingAPIDomain::CodeAnalysis, ReviewAPI);
    }

    // 註冊 BMAD System
    if (UObject* BMAD = NewObject<UMingRTSBMADSystem>())
    {
        RegisterSubSystem(ECodingAPIDomain::MultiAgent, BMAD);
        RegisterSubSystem(ECodingAPIDomain::BMADSystem, BMAD);
    }
}

void UMingRTSCodingAPI::BuildFunctionRegistry()
{
    // 從所有註冊的模組收集功能
    FunctionRegistry.Empty();

    for (auto& Module : RegisteredModules)
    {
        // 這裡會解析每個模組的功能
        // 實際實現需要掃描源代碼或使用反射
        UE_LOG(LogTemp, Verbose, TEXT("Building function registry for %s"), *Module.Value.ModuleName);
    }
}

void UMingRTSCodingAPI::UpdateMetrics()
{
    CurrentMetrics.TotalFunctions = FunctionRegistry.Num();
    CurrentMetrics.ActiveDomains = RegisteredModules.Num();

    CurrentMetrics.FunctionsPerDomain.Empty();
    for (const auto& Function : FunctionRegistry)
    {
        int32& Count = CurrentMetrics.FunctionsPerDomain.FindOrAdd(Function.Domain);
        Count++;
    }
}

UObject* UMingRTSCodingAPI::GetSubSystem(ECodingAPIDomain Domain) const
{
    if (UObject* const* System = DomainSubSystems.Find(Domain))
    {
        return *System;
    }
    return nullptr;
}

void UMingRTSCodingAPI::RegisterSubSystem(ECodingAPIDomain Domain, UObject* System)
{
    if (System)
    {
        DomainSubSystems.Add(Domain, System);
        UE_LOG(LogTemp, Log, TEXT("Registered subsystem for domain %d"), static_cast<int32>(Domain));
    }
}

float UMingRTSCodingAPI::CalculateDomainRelevance(ECodingAPIDomain Domain, const FString& TaskDescription) const
{
    FString LowerDesc = TaskDescription.ToLower();
    float Score = 0.0f;

    // 基於關鍵詞匹配計算相關性
    switch (Domain)
    {
    case ECodingAPIDomain::CodeGeneration:
        if (LowerDesc.Contains(TEXT("generate"))) Score += 1.0f;
        if (LowerDesc.Contains(TEXT("create"))) Score += 0.8f;
        if (LowerDesc.Contains(TEXT("template"))) Score += 0.7f;
        break;

    case ECodingAPIDomain::CodeAnalysis:
        if (LowerDesc.Contains(TEXT("analyze"))) Score += 1.0f;
        if (LowerDesc.Contains(TEXT("review"))) Score += 0.9f;
        if (LowerDesc.Contains(TEXT("check"))) Score += 0.7f;
        break;

    case ECodingAPIDomain::Debugging:
        if (LowerDesc.Contains(TEXT("debug"))) Score += 1.0f;
        if (LowerDesc.Contains(TEXT("error"))) Score += 0.9f;
        if (LowerDesc.Contains(TEXT("fix"))) Score += 0.8f;
        break;

    case ECodingAPIDomain::Compilation:
        if (LowerDesc.Contains(TEXT("compile"))) Score += 1.0f;
        if (LowerDesc.Contains(TEXT("build"))) Score += 0.9f;
        break;

    case ECodingAPIDomain::VersionControl:
        if (LowerDesc.Contains(TEXT("git"))) Score += 1.0f;
        if (LowerDesc.Contains(TEXT("commit"))) Score += 0.9f;
        if (LowerDesc.Contains(TEXT("branch"))) Score += 0.8f;
        break;

    default:
        break;
    }

    return Score;
}

FCodingAPIResult UMingRTSCodingAPI::ExecuteInDomain(ECodingAPIDomain Domain, const FString& Operation, const TMap<FString, FString>& Params)
{
    FCodingAPIResult Result;
    Result.ExecutedDomain = Domain;
    Result.Timestamp = FDateTime::Now();

    double StartTime = FPlatformTime::Seconds();

    UObject* System = GetSubSystem(Domain);
    if (!System)
    {
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("No system registered for domain %d"), static_cast<int32>(Domain));
        return Result;
    }

    // 這裡會調用具體子系統的方法
    // 實際實現需要通過反射或直接調用
    Result.bSuccess = true;
    Result.Message = FString::Printf(TEXT("Executed %s in domain %s"), *Operation, *MingCodingAPI::DomainToString(Domain));

    // 記錄執行時間
    Result.ExecutionTime = (FPlatformTime::Seconds() - StartTime) * 1000.0f;

    // 記錄日誌
    LogAPICall(Domain, Operation, Result);

    return Result;
}

void UMingRTSCodingAPI::LogAPICall(ECodingAPIDomain Domain, const FString& Function, const FCodingAPIResult& Result)
{
    UE_LOG(LogTemp, Log, TEXT("[CodingAPI] %s.%s: %s (%.2f ms)"),
        *MingCodingAPI::DomainToString(Domain),
        *Function,
        Result.bSuccess ? TEXT("Success") : TEXT("Failed"),
        Result.ExecutionTime);
}

void UMingRTSCodingAPI::TrackMetrics(ECodingAPIDomain Domain, float ExecutionTime, bool bSuccess)
{
    // 更新平均響應時間
    CurrentMetrics.AverageResponseTime = (CurrentMetrics.AverageResponseTime * 0.9f) + (ExecutionTime * 0.1f);

    // 更新成功率
    static int32 SuccessCount = 0;
    static int32 TotalCount = 0;

    TotalCount++;
    if (bSuccess) SuccessCount++;

    CurrentMetrics.SuccessRate = (float)SuccessCount / (float)TotalCount;

    // 觸發事件
    OnMetricsUpdated.Broadcast(CurrentMetrics);
}

// ============================================================================
// 輔助函數命名空間實現
// ============================================================================

FString MingCodingAPI::DomainToString(ECodingAPIDomain Domain)
{
    switch (Domain)
    {
    case ECodingAPIDomain::CodeGeneration: return TEXT("CodeGeneration");
    case ECodingAPIDomain::CodeAnalysis: return TEXT("CodeAnalysis");
    case ECodingAPIDomain::Debugging: return TEXT("Debugging");
    case ECodingAPIDomain::Compilation: return TEXT("Compilation");
    case ECodingAPIDomain::VersionControl: return TEXT("VersionControl");
    case ECodingAPIDomain::AICodeGeneration: return TEXT("AICodeGeneration");
    case ECodingAPIDomain::MultiAgent: return TEXT("MultiAgent");
    case ECodingAPIDomain::CodeReview: return TEXT("CodeReview");
    case ECodingAPIDomain::Performance: return TEXT("Performance");
    case ECodingAPIDomain::Testing: return TEXT("Testing");
    case ECodingAPIDomain::LearningSystem: return TEXT("LearningSystem");
    default: return TEXT("Unknown");
    }
}

ECodingAPIDomain MingCodingAPI::StringToDomain(const FString& DomainString)
{
    if (DomainString == TEXT("CodeGeneration")) return ECodingAPIDomain::CodeGeneration;
    if (DomainString == TEXT("CodeAnalysis")) return ECodingAPIDomain::CodeAnalysis;
    if (DomainString == TEXT("Debugging")) return ECodingAPIDomain::Debugging;
    if (DomainString == TEXT("Compilation")) return ECodingAPIDomain::Compilation;
    if (DomainString == TEXT("VersionControl")) return ECodingAPIDomain::VersionControl;
    if (DomainString == TEXT("AICodeGeneration")) return ECodingAPIDomain::AICodeGeneration;
    if (DomainString == TEXT("MultiAgent")) return ECodingAPIDomain::MultiAgent;
    if (DomainString == TEXT("CodeReview")) return ECodingAPIDomain::CodeReview;
    if (DomainString == TEXT("Performance")) return ECodingAPIDomain::Performance;
    if (DomainString == TEXT("Testing")) return ECodingAPIDomain::Testing;
    if (DomainString == TEXT("LearningSystem")) return ECodingAPIDomain::LearningSystem;
    return ECodingAPIDomain::CodeGeneration;
}

FString MingCodingAPI::LanguageToString(ESupportedLanguage Language)
{
    switch (Language)
    {
    case ESupportedLanguage::CPP: return TEXT("C++");
    case ESupportedLanguage::Blueprint: return TEXT("Blueprint");
    case ESupportedLanguage::Python: return TEXT("Python");
    case ESupportedLanguage::JavaScript: return TEXT("JavaScript");
    case ESupportedLanguage::PowerShell: return TEXT("PowerShell");
    case ESupportedLanguage::ShaderLanguage: return TEXT("Shader");
    case ESupportedLanguage::INI: return TEXT("INI");
    case ESupportedLanguage::JSON: return TEXT("JSON");
    case ESupportedLanguage::XML: return TEXT("XML");
    case ESupportedLanguage::Markdown: return TEXT("Markdown");
    default: return TEXT("Unknown");
    }
}

ESupportedLanguage MingCodingAPI::StringToLanguage(const FString& LanguageString)
{
    if (LanguageString == TEXT("C++")) return ESupportedLanguage::CPP;
    if (LanguageString == TEXT("Blueprint")) return ESupportedLanguage::Blueprint;
    if (LanguageString == TEXT("Python")) return ESupportedLanguage::Python;
    if (LanguageString == TEXT("JavaScript")) return ESupportedLanguage::JavaScript;
    if (LanguageString == TEXT("PowerShell")) return ESupportedLanguage::PowerShell;
    if (LanguageString == TEXT("Shader")) return ESupportedLanguage::ShaderLanguage;
    if (LanguageString == TEXT("INI")) return ESupportedLanguage::INI;
    if (LanguageString == TEXT("JSON")) return ESupportedLanguage::JSON;
    if (LanguageString == TEXT("XML")) return ESupportedLanguage::XML;
    if (LanguageString == TEXT("Markdown")) return ESupportedLanguage::Markdown;
    return ESupportedLanguage::CPP;
}

FString MingCodingAPI::TaskTypeToString(ECodingTaskType TaskType)
{
    switch (TaskType)
    {
    case ECodingTaskType::FeatureDevelopment: return TEXT("FeatureDevelopment");
    case ECodingTaskType::BugFix: return TEXT("BugFix");
    case ECodingTaskType::Refactoring: return TEXT("Refactoring");
    case ECodingTaskType::Optimization: return TEXT("Optimization");
    case ECodingTaskType::Documentation: return TEXT("Documentation");
    case ECodingTaskType::Testing: return TEXT("Testing");
    case ECodingTaskType::Integration: return TEXT("Integration");
    case ECodingTaskType::Deployment: return TEXT("Deployment");
    case ECodingTaskType::Maintenance: return TEXT("Maintenance");
    case ECodingTaskType::Research: return TEXT("Research");
    default: return TEXT("Unknown");
    }
}

ECodingTaskType MingCodingAPI::StringToTaskType(const FString& TaskString)
{
    if (TaskString == TEXT("FeatureDevelopment")) return ECodingTaskType::FeatureDevelopment;
    if (TaskString == TEXT("BugFix")) return ECodingTaskType::BugFix;
    if (TaskString == TEXT("Refactoring")) return ECodingTaskType::Refactoring;
    if (TaskString == TEXT("Optimization")) return ECodingTaskType::Optimization;
    if (TaskString == TEXT("Documentation")) return ECodingTaskType::Documentation;
    if (TaskString == TEXT("Testing")) return ECodingTaskType::Testing;
    if (TaskString == TEXT("Integration")) return ECodingTaskType::Integration;
    if (TaskString == TEXT("Deployment")) return ECodingTaskType::Deployment;
    if (TaskString == TEXT("Maintenance")) return ECodingTaskType::Maintenance;
    if (TaskString == TEXT("Research")) return ECodingTaskType::Research;
    return ECodingTaskType::FeatureDevelopment;
}

// ============================================================================
// C++ Reference 整合實現
// ============================================================================

bool UMingRTSCodingAPI::LoadCppReferenceDocumentation()
{
    if (!CppReferenceIntegration)
    {
        CppReferenceIntegration = NewObject<UMingRTSCppReferenceIntegration>();
    }
    
    bool bSuccess = CppReferenceIntegration->LoadCppReferenceDocs();
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully loaded C++ Reference documentation"));
        
        // 添加 CppReference 相關功能到註冊表
        TArray<FCppReferenceCategory> Categories = CppReferenceIntegration->GetAllCategories();
        for (const auto& Category : Categories)
        {
            UE_LOG(LogTemp, Verbose, TEXT("Loaded category: %s with %d topics"), 
                *Category.ChineseTitle, Category.Topics.Num());
        }
    }
    
    return bSuccess;
}

FString UMingRTSCodingAPI::GenerateCodeWithReference(ECodingAPIDomain Domain, const FString& Topic, const FString& Context)
{
    if (!CppReferenceIntegration)
    {
        LoadCppReferenceDocumentation();
    }
    
    // 基於 C++ Reference 文檔生成代碼
    FString CategoryName;
    
    // 根據 Domain 映射到對應的 C++ Reference 類別
    switch (Domain)
    {
    case ECodingAPIDomain::CodeGeneration:
    case ECodingAPIDomain::AICodeGeneration:
        CategoryName = TEXT("language");
        break;
    case ECodingAPIDomain::CodeAnalysis:
        CategoryName = TEXT("container");
        break;
    case ECodingAPIDomain::Performance:
    case ECodingAPIDomain::Optimization:
        CategoryName = TEXT("algorithm");
        break;
    case ECodingAPIDomain::Concurrency:
        CategoryName = TEXT("thread");
        break;
    default:
        CategoryName = TEXT("utility");
    }
    
    // 生成結合 C++ Reference 的代碼模板
    FString GeneratedCode = CppReferenceIntegration->GenerateCodeFromReference(CategoryName, Topic);
    
    // 添加上下文特定的註釋
    FString Result = FString::Printf(
        TEXT("// 基於 C++ Reference (%s) 生成\n")
        TEXT("// 上下文: %s\n\n")
        TEXT("%s"),
        *CategoryName,
        *Context,
        *GeneratedCode
    );
    
    return Result;
}

TArray<FString> UMingRTSCodingAPI::GetCppReferenceTopics(ECodingTaskType TaskType) const
{
    if (!CppReferenceIntegration)
    {
        return TArray<FString>();
    }
    
    return CppReferenceIntegration->GetRelevantTopics(TaskType);
}
