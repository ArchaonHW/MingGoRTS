// Copyright (c) 2026 MingGoRTS. All rights reserved.
// MingRTSCodingAPIFunctionMappings.cpp - 功能映射實現

#include "API/MingRTSCodingAPIFunctionMappings.h"

UMingRTSCodingAPIMappings::UMingRTSCodingAPIMappings()
{
}

void UMingRTSCodingAPIMappings::InitializeMappings()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Coding API Function Mappings..."));

    // 構建所有域的映射
    BuildCodeGenerationMappings();
    BuildAICodeGenerationMappings();
    BuildCodeAnalysisMappings();
    BuildDebuggingMappings();
    BuildCompilationMappings();
    BuildVersionControlMappings();
    BuildMultiAgentMappings();
    BuildScriptIntegrationMappings();
    BuildAdvancedFeaturesMappings();
    BuildLearningSystemMappings();

    UE_LOG(LogTemp, Log, TEXT("Coding API Function Mappings initialized successfully"));
}

void UMingRTSCodingAPIMappings::BuildCodeGenerationMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::CodeGeneration);

    // 核心生成功能
    DomainMap.Add(TEXT("GenerateUE5Class"), CodeGenerationMap.GenerateUE5Class);
    DomainMap.Add(TEXT("GenerateUE5Function"), CodeGenerationMap.GenerateUE5Function);
    DomainMap.Add(TEXT("GenerateUE5System"), CodeGenerationMap.GenerateUE5System);
    DomainMap.Add(TEXT("GenerateUE5Gameplay"), CodeGenerationMap.GenerateUE5Gameplay);
    DomainMap.Add(TEXT("GenerateGenericCode"), CodeGenerationMap.GenerateGenericCode);

    // 模板功能
    DomainMap.Add(TEXT("SelectCodeTemplate"), CodeGenerationMap.SelectCodeTemplate);
    DomainMap.Add(TEXT("GetAvailableTemplates"), CodeGenerationMap.GetAvailableTemplates);

    // 質量功能
    DomainMap.Add(TEXT("CheckCodeQuality"), CodeGenerationMap.CheckCodeQuality);
    DomainMap.Add(TEXT("CalculateQualityScore"), CodeGenerationMap.CalculateQualityScore);

    // 智能命名
    DomainMap.Add(TEXT("GenerateClassName"), CodeGenerationMap.GenerateClassName);
    DomainMap.Add(TEXT("GenerateFunctionName"), CodeGenerationMap.GenerateFunctionName);
    DomainMap.Add(TEXT("GenerateSystemName"), CodeGenerationMap.GenerateSystemName);
    DomainMap.Add(TEXT("GenerateActorName"), CodeGenerationMap.GenerateActorName);

    // 批量與匯出
    DomainMap.Add(TEXT("BatchGenerateCode"), CodeGenerationMap.BatchGenerateCode);
    DomainMap.Add(TEXT("ExportCodeToFile"), CodeGenerationMap.ExportCodeToFile);
    DomainMap.Add(TEXT("ExportToClipboard"), CodeGenerationMap.ExportToClipboard);

    // 建立反向映射
    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::CodeGeneration);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d CodeGeneration mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildAICodeGenerationMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::AICodeGeneration);

    DomainMap.Add(TEXT("IntelligentGenerate"), AICodeGenerationMap.IntelligentGenerate);
    DomainMap.Add(TEXT("ContextAwareGenerate"), AICodeGenerationMap.ContextAwareGenerate);
    DomainMap.Add(TEXT("AnalyzeRequirements"), AICodeGenerationMap.AnalyzeRequirements);
    DomainMap.Add(TEXT("ExtractCodeIntent"), AICodeGenerationMap.ExtractCodeIntent);
    DomainMap.Add(TEXT("GenerateWithProvider"), AICodeGenerationMap.GenerateWithProvider);
    DomainMap.Add(TEXT("CompareProviders"), AICodeGenerationMap.CompareProviders);
    DomainMap.Add(TEXT("SuggestOptimizations"), AICodeGenerationMap.SuggestOptimizations);
    DomainMap.Add(TEXT("RefactorSuggestions"), AICodeGenerationMap.RefactorSuggestions);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::AICodeGeneration);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d AICodeGeneration mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildCodeAnalysisMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::CodeAnalysis);

    // 審查功能
    DomainMap.Add(TEXT("ReviewCode"), CodeAnalysisMap.ReviewCode);
    DomainMap.Add(TEXT("ReviewFile"), CodeAnalysisMap.ReviewFile);
    DomainMap.Add(TEXT("ReviewProject"), CodeAnalysisMap.ReviewProject);

    // 規範檢查
    DomainMap.Add(TEXT("CheckNamingConventions"), CodeAnalysisMap.CheckNamingConventions);
    DomainMap.Add(TEXT("CheckFormatting"), CodeAnalysisMap.CheckFormatting);
    DomainMap.Add(TEXT("CheckDocumentation"), CodeAnalysisMap.CheckDocumentation);
    DomainMap.Add(TEXT("CheckArchitecture"), CodeAnalysisMap.CheckArchitecture);

    // 問題檢測
    DomainMap.Add(TEXT("FindIssues"), CodeAnalysisMap.FindIssues);
    DomainMap.Add(TEXT("AutoFixIssues"), CodeAnalysisMap.AutoFixIssues);
    DomainMap.Add(TEXT("GenerateFixSuggestions"), CodeAnalysisMap.GenerateFixSuggestions);

    // 品質指標
    DomainMap.Add(TEXT("CalculateQualityMetrics"), CodeAnalysisMap.CalculateQualityMetrics);
    DomainMap.Add(TEXT("GenerateQualityReport"), CodeAnalysisMap.GenerateQualityReport);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::CodeAnalysis);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d CodeAnalysis mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildDebuggingMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::Debugging);

    // 會話管理
    DomainMap.Add(TEXT("StartDebugSession"), DebuggingMap.StartDebugSession);
    DomainMap.Add(TEXT("StopDebugSession"), DebuggingMap.StopDebugSession);

    // 斷點管理
    DomainMap.Add(TEXT("SetBreakpoint"), DebuggingMap.SetBreakpoint);
    DomainMap.Add(TEXT("RemoveBreakpoint"), DebuggingMap.RemoveBreakpoint);
    DomainMap.Add(TEXT("GetSmartBreakpointSuggestions"), DebuggingMap.GetSmartBreakpointSuggestions);

    // 執行控制
    DomainMap.Add(TEXT("StepOver"), DebuggingMap.StepOver);
    DomainMap.Add(TEXT("StepInto"), DebuggingMap.StepInto);
    DomainMap.Add(TEXT("Continue"), DebuggingMap.Continue);

    // 變數監控
    DomainMap.Add(TEXT("GetLocalVariables"), DebuggingMap.GetLocalVariables);
    DomainMap.Add(TEXT("GetGlobalVariables"), DebuggingMap.GetGlobalVariables);
    DomainMap.Add(TEXT("AddWatchExpression"), DebuggingMap.AddWatchExpression);
    DomainMap.Add(TEXT("EvaluateExpression"), DebuggingMap.EvaluateExpression);

    // 記憶體除錯
    DomainMap.Add(TEXT("GetMemoryRegion"), DebuggingMap.GetMemoryRegion);
    DomainMap.Add(TEXT("TrackMemoryLeaks"), DebuggingMap.TrackMemoryLeaks);
    DomainMap.Add(TEXT("GetMemoryStatistics"), DebuggingMap.GetMemoryStatistics);

    // 性能除錯
    DomainMap.Add(TEXT("StartPerformanceProfile"), DebuggingMap.StartPerformanceProfile);
    DomainMap.Add(TEXT("StopPerformanceProfile"), DebuggingMap.StopPerformanceProfile);
    DomainMap.Add(TEXT("GetPerformanceMetrics"), DebuggingMap.GetPerformanceMetrics);

    // 自我學習
    DomainMap.Add(TEXT("LearnFromBugPattern"), DebuggingMap.LearnFromBugPattern);
    DomainMap.Add(TEXT("UpdateBreakpointEfficiency"), DebuggingMap.UpdateBreakpointEfficiency);

    // 智能分析
    DomainMap.Add(TEXT("AnalyzeCurrentState"), DebuggingMap.AnalyzeCurrentState);
    DomainMap.Add(TEXT("AnalyzeErrorPattern"), DebuggingMap.AnalyzeErrorPattern);
    DomainMap.Add(TEXT("PredictPotentialIssues"), DebuggingMap.PredictPotentialIssues);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::Debugging);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d Debugging mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildCompilationMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::Compilation);

    // 核心編譯
    DomainMap.Add(TEXT("CompileProject"), CompilationMap.CompileProject);
    DomainMap.Add(TEXT("CompileFile"), CompilationMap.CompileFile);
    DomainMap.Add(TEXT("ValidateSyntax"), CompilationMap.ValidateSyntax);

    // 自我學習
    DomainMap.Add(TEXT("LearnFromCompilationResult"), CompilationMap.LearnFromCompilationResult);
    DomainMap.Add(TEXT("UpdateErrorPatterns"), CompilationMap.UpdateErrorPatterns);
    DomainMap.Add(TEXT("GetSmartSuggestions"), CompilationMap.GetSmartSuggestions);

    // 智能優化
    DomainMap.Add(TEXT("OptimizeCompilation"), CompilationMap.OptimizeCompilation);
    DomainMap.Add(TEXT("UpdateOptimizationStrategies"), CompilationMap.UpdateOptimizationStrategies);

    // 跨 API 調用
    DomainMap.Add(TEXT("RequestLogAnalysis"), CompilationMap.RequestLogAnalysis);
    DomainMap.Add(TEXT("RequestDebuggingSession"), CompilationMap.RequestDebuggingSession);
    DomainMap.Add(TEXT("RequestPerformanceAnalysis"), CompilationMap.RequestPerformanceAnalysis);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::Compilation);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d Compilation mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildVersionControlMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::VersionControl);

    // Git 操作
    DomainMap.Add(TEXT("GitCommit"), VersionControlMap.GitCommit);
    DomainMap.Add(TEXT("GitPush"), VersionControlMap.GitPush);
    DomainMap.Add(TEXT("GitPull"), VersionControlMap.GitPull);
    DomainMap.Add(TEXT("GitBranch"), VersionControlMap.GitBranch);
    DomainMap.Add(TEXT("GitMerge"), VersionControlMap.GitMerge);

    // 分支管理
    DomainMap.Add(TEXT("CreateBranch"), VersionControlMap.CreateBranch);
    DomainMap.Add(TEXT("SwitchBranch"), VersionControlMap.SwitchBranch);
    DomainMap.Add(TEXT("DeleteBranch"), VersionControlMap.DeleteBranch);
    DomainMap.Add(TEXT("ListBranches"), VersionControlMap.ListBranches);

    // 提交管理
    DomainMap.Add(TEXT("StageFiles"), VersionControlMap.StageFiles);
    DomainMap.Add(TEXT("UnstageFiles"), VersionControlMap.UnstageFiles);
    DomainMap.Add(TEXT("GetCommitHistory"), VersionControlMap.GetCommitHistory);
    DomainMap.Add(TEXT("RevertCommit"), VersionControlMap.RevertCommit);

    // 差異分析
    DomainMap.Add(TEXT("ShowDiff"), VersionControlMap.ShowDiff);
    DomainMap.Add(TEXT("CompareBranches"), VersionControlMap.CompareBranches);
    DomainMap.Add(TEXT("GeneratePatch"), VersionControlMap.GeneratePatch);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::VersionControl);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d VersionControl mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildMultiAgentMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::MultiAgent);

    // 智能體管理
    DomainMap.Add(TEXT("CreateAgent"), MultiAgentMap.CreateAgent);
    DomainMap.Add(TEXT("RemoveAgent"), MultiAgentMap.RemoveAgent);
    DomainMap.Add(TEXT("ConfigureAgent"), MultiAgentMap.ConfigureAgent);
    DomainMap.Add(TEXT("GetAgentProfile"), MultiAgentMap.GetAgentProfile);

    // 討論管理
    DomainMap.Add(TEXT("StartDiscussion"), MultiAgentMap.StartDiscussion);
    DomainMap.Add(TEXT("AddAgentToDiscussion"), MultiAgentMap.AddAgentToDiscussion);
    DomainMap.Add(TEXT("PostAgentMessage"), MultiAgentMap.PostAgentMessage);
    DomainMap.Add(TEXT("GetDiscussionSummary"), MultiAgentMap.GetDiscussionSummary);

    // 決策流程
    DomainMap.Add(TEXT("ProposeSolution"), MultiAgentMap.ProposeSolution);
    DomainMap.Add(TEXT("VoteOnSolution"), MultiAgentMap.VoteOnSolution);
    DomainMap.Add(TEXT("ReachConsensus"), MultiAgentMap.ReachConsensus);
    DomainMap.Add(TEXT("GenerateFinalDecision"), MultiAgentMap.GenerateFinalDecision);

    // 角色特定
    DomainMap.Add(TEXT("GetArchitectAnalysis"), MultiAgentMap.GetArchitectAnalysis);
    DomainMap.Add(TEXT("GetDeveloperImplementation"), MultiAgentMap.GetDeveloperImplementation);
    DomainMap.Add(TEXT("GetTesterFeedback"), MultiAgentMap.GetTesterFeedback);
    DomainMap.Add(TEXT("GetSecurityReview"), MultiAgentMap.GetSecurityReview);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::MultiAgent);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d MultiAgent mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildScriptIntegrationMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::ScriptIntegration);

    // PowerShell
    DomainMap.Add(TEXT("ExecutePowerShell"), ScriptIntegrationMap.ExecutePowerShell);
    DomainMap.Add(TEXT("ExecutePowerShellScript"), ScriptIntegrationMap.ExecutePowerShellScript);
    DomainMap.Add(TEXT("GetPowerShellOutput"), ScriptIntegrationMap.GetPowerShellOutput);

    // Python
    DomainMap.Add(TEXT("ExecutePython"), ScriptIntegrationMap.ExecutePython);
    DomainMap.Add(TEXT("ExecutePythonScript"), ScriptIntegrationMap.ExecutePythonScript);
    DomainMap.Add(TEXT("GetPythonOutput"), ScriptIntegrationMap.GetPythonOutput);

    // 腳本管理
    DomainMap.Add(TEXT("LoadScript"), ScriptIntegrationMap.LoadScript);
    DomainMap.Add(TEXT("SaveScript"), ScriptIntegrationMap.SaveScript);
    DomainMap.Add(TEXT("ListAvailableScripts"), ScriptIntegrationMap.ListAvailableScripts);
    DomainMap.Add(TEXT("ScheduleScriptExecution"), ScriptIntegrationMap.ScheduleScriptExecution);

    // 結果處理
    DomainMap.Add(TEXT("ParseScriptOutput"), ScriptIntegrationMap.ParseScriptOutput);
    DomainMap.Add(TEXT("ConvertToUE5Types"), ScriptIntegrationMap.ConvertToUE5Types);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::ScriptIntegration);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d ScriptIntegration mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildAdvancedFeaturesMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::AdvancedFeatures);

    // NLP
    DomainMap.Add(TEXT("ProcessNaturalLanguage"), AdvancedFeaturesMap.ProcessNaturalLanguage);
    DomainMap.Add(TEXT("ExtractCodeEntities"), AdvancedFeaturesMap.ExtractCodeEntities);
    DomainMap.Add(TEXT("AnalyzeCodeSentiment"), AdvancedFeaturesMap.AnalyzeCodeSentiment);

    // 數據挖掘
    DomainMap.Add(TEXT("PerformPatternRecognition"), AdvancedFeaturesMap.PerformPatternRecognition);
    DomainMap.Add(TEXT("AnalyzeTrends"), AdvancedFeaturesMap.AnalyzeTrends);
    DomainMap.Add(TEXT("PerformClustering"), AdvancedFeaturesMap.PerformClustering);
    DomainMap.Add(TEXT("PerformClassification"), AdvancedFeaturesMap.PerformClassification);

    // 安全分析
    DomainMap.Add(TEXT("AnalyzeSecurityThreats"), AdvancedFeaturesMap.AnalyzeSecurityThreats);
    DomainMap.Add(TEXT("DetectVulnerabilities"), AdvancedFeaturesMap.DetectVulnerabilities);
    DomainMap.Add(TEXT("GenerateSecurityReport"), AdvancedFeaturesMap.GenerateSecurityReport);

    // 預測維護
    DomainMap.Add(TEXT("PredictIssues"), AdvancedFeaturesMap.PredictIssues);
    DomainMap.Add(TEXT("AnalyzeSystemHealth"), AdvancedFeaturesMap.AnalyzeSystemHealth);
    DomainMap.Add(TEXT("GenerateMaintenanceSchedule"), AdvancedFeaturesMap.GenerateMaintenanceSchedule);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::AdvancedFeatures);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d AdvancedFeatures mappings"), DomainMap.Num());
}

void UMingRTSCodingAPIMappings::BuildLearningSystemMappings()
{
    TMap<FString, FString>& DomainMap = FunctionMappings.FindOrAdd(ECodingAPIDomain::LearningSystem);

    // 學習管理
    DomainMap.Add(TEXT("StartLearningSession"), LearningSystemMap.StartLearningSession);
    DomainMap.Add(TEXT("StopLearningSession"), LearningSystemMap.StopLearningSession);
    DomainMap.Add(TEXT("GetLearningProgress"), LearningSystemMap.GetLearningProgress);

    // 文檔生成
    DomainMap.Add(TEXT("GenerateDocumentation"), LearningSystemMap.GenerateDocumentation);
    DomainMap.Add(TEXT("GenerateAPIDocs"), LearningSystemMap.GenerateAPIDocs);
    DomainMap.Add(TEXT("GenerateTutorials"), LearningSystemMap.GenerateTutorials);

    // 知識累積
    DomainMap.Add(TEXT("LearnFromCode"), LearningSystemMap.LearnFromCode);
    DomainMap.Add(TEXT("LearnFromErrors"), LearningSystemMap.LearnFromErrors);
    DomainMap.Add(TEXT("BuildKnowledgeBase"), LearningSystemMap.BuildKnowledgeBase);
    DomainMap.Add(TEXT("QueryKnowledgeBase"), LearningSystemMap.QueryKnowledgeBase);

    // 技能評估
    DomainMap.Add(TEXT("AssessCodingSkills"), LearningSystemMap.AssessCodingSkills);
    DomainMap.Add(TEXT("GenerateLearningPath"), LearningSystemMap.GenerateLearningPath);
    DomainMap.Add(TEXT("RecommendResources"), LearningSystemMap.RecommendResources);

    for (const auto& Pair : DomainMap)
    {
        ReverseMappings.Add(Pair.Key, ECodingAPIDomain::LearningSystem);
    }

    UE_LOG(LogTemp, Verbose, TEXT("Built %d LearningSystem mappings"), DomainMap.Num());
}

// ============================================================================
// 查詢方法
// ============================================================================

FString UMingRTSCodingAPIMappings::GetFunctionMapping(ECodingAPIDomain Domain, const FString& FunctionName) const
{
    if (const TMap<FString, FString>* DomainMap = FunctionMappings.Find(Domain))
    {
        if (const FString* Mapping = DomainMap->Find(FunctionName))
        {
            return *Mapping;
        }
    }
    return FString();
}

TArray<FString> UMingRTSCodingAPIMappings::GetAllFunctionsInDomain(ECodingAPIDomain Domain) const
{
    TArray<FString> Functions;

    if (const TMap<FString, FString>* DomainMap = FunctionMappings.Find(Domain))
    {
        DomainMap->GenerateKeyArray(Functions);
    }

    return Functions;
}

ECodingAPIDomain UMingRTSCodingAPIMappings::GetDomainForFunction(const FString& FunctionName) const
{
    if (const ECodingAPIDomain* Domain = ReverseMappings.Find(FunctionName))
    {
        return *Domain;
    }
    return ECodingAPIDomain::CodeGeneration;
}

bool UMingRTSCodingAPIMappings::IsFunctionAvailable(const FString& FunctionName) const
{
    return ReverseMappings.Contains(FunctionName);
}

TMap<ECodingAPIDomain, TArray<FString>> UMingRTSCodingAPIMappings::GetAllMappings() const
{
    TMap<ECodingAPIDomain, TArray<FString>> AllMappings;

    for (const auto& Pair : FunctionMappings)
    {
        TArray<FString> Functions;
        Pair.Value.GenerateKeyArray(Functions);
        AllMappings.Add(Pair.Key, Functions);
    }

    return AllMappings;
}

int32 UMingRTSCodingAPIMappings::GetTotalFunctionCount() const
{
    int32 Total = 0;
    for (const auto& Pair : FunctionMappings)
    {
        Total += Pair.Value.Num();
    }
    return Total;
}

TMap<ECodingAPIDomain, int32> UMingRTSCodingAPIMappings::GetFunctionCountPerDomain() const
{
    TMap<ECodingAPIDomain, int32> Counts;

    for (const auto& Pair : FunctionMappings)
    {
        Counts.Add(Pair.Key, Pair.Value.Num());
    }

    return Counts;
}

// ============================================================================
// 報告生成
// ============================================================================

FString UMingRTSCodingAPIMappings::GenerateMappingReport() const
{
    FString Report = TEXT("============================================\n");
    Report += TEXT("MingRTS Coding API 功能映射報告\n");
    Report += TEXT("============================================\n\n");

    Report += FString::Printf(TEXT("總功能數: %d\n"), GetTotalFunctionCount());
    Report += FString::Printf(TEXT("域數量: %d\n\n"), FunctionMappings.Num());

    Report += TEXT("各域功能分布:\n");
    TMap<ECodingAPIDomain, int32> Counts = GetFunctionCountPerDomain();
    for (const auto& Pair : Counts)
    {
        Report += FString::Printf(TEXT("  - %s: %d 個功能\n"),
            *MingCodingAPI::DomainToString(Pair.Key),
            Pair.Value);
    }

    Report += TEXT("\n詳細功能列表:\n");
    for (const auto& Pair : FunctionMappings)
    {
        Report += FString::Printf(TEXT("\n【%s】\n"), *MingCodingAPI::DomainToString(Pair.Key));

        TArray<FString> Functions;
        Pair.Value.GenerateKeyArray(Functions);
        for (const auto& Func : Functions)
        {
            Report += FString::Printf(TEXT("  - %s\n"), *Func);
        }
    }

    return Report;
}

bool UMingRTSCodingAPIMappings::ExportMappingsToJSON(const FString& FilePath) const
{
    // JSON 格式的映射匯出
    FString JSON = TEXT("{\n");
    JSON += TEXT("  \"coding_api_mappings\": {\n");

    bool FirstDomain = true;
    for (const auto& DomainPair : FunctionMappings)
    {
        if (!FirstDomain)
        {
            JSON += TEXT(",\n");
        }
        FirstDomain = false;

        JSON += FString::Printf(TEXT("    \"%s\": {\n"),
            *MingCodingAPI::DomainToString(DomainPair.Key));

        bool FirstFunc = true;
        for (const auto& FuncPair : DomainPair.Value)
        {
            if (!FirstFunc)
            {
                JSON += TEXT(",\n");
            }
            FirstFunc = false;

            JSON += FString::Printf(TEXT("      \"%s\": \"%s\""),
                *FuncPair.Key,
                *FuncPair.Value);
        }

        JSON += TEXT("\n    }");
    }

    JSON += TEXT("\n  }\n");
    JSON += TEXT("}\n");

    // 實際文件寫入需要平台相關實現
    UE_LOG(LogTemp, Log, TEXT("JSON would be exported to: %s"), *FilePath);
    UE_LOG(LogTemp, Log, TEXT("JSON length: %d characters"), JSON.Len());

    return true;
}

// ============================================================================
// 驗證
// ============================================================================

bool UMingRTSCodingAPIMappings::ValidateMappings() const
{
    bool bValid = true;

    // 檢查是否有空映射
    for (const auto& Pair : FunctionMappings)
    {
        if (Pair.Value.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("Domain %s has no function mappings"),
                *MingCodingAPI::DomainToString(Pair.Key));
            bValid = false;
        }
    }

    // 檢查反向映射一致性
    for (const auto& Pair : ReverseMappings)
    {
        ECodingAPIDomain ExpectedDomain = Pair.Value;
        if (const TMap<FString, FString>* DomainMap = FunctionMappings.Find(ExpectedDomain))
        {
            if (!DomainMap->Contains(Pair.Key))
            {
                UE_LOG(LogTemp, Warning, TEXT("Inconsistent mapping for function: %s"), *Pair.Key);
                bValid = false;
            }
        }
    }

    return bValid;
}

TArray<FString> UMingRTSCodingAPIMappings::FindUnmappedFunctions() const
{
    TArray<FString> Unmapped;

    // 這裡可以擴展為掃描源代碼中未映射的功能
    // 目前返回空數組作為佔位符

    return Unmapped;
}
