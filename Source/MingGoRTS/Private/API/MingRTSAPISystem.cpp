#include "MingRTSAPISystem.h"
#include "MingRTSCompilationAPI.h"
#include "MingRTSLogAPI.h"
#include "MingRTSDebugAPI.h"
#include "MingRTSOptimizationAPI.h"
#include "MingRTSSmartPanelAPI.h"
#include "MingRTSCodeStandardsAPI.h"
#include "MingRTSBMADSystem.h"
#include "MingRTSCodeReviewAPI.h"
#include "MingRTSUE5LearningAPI.h"
#include "MingRTSPotatoAICodeGeneratorAPI.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"

UMingRTSAPISystem::UMingRTSAPISystem()
{
    bAutoCoordinationEnabled = true;
    bPredictiveModeEnabled = false;
}

void UMingRTSAPISystem::InitializeAPISystem()
{
    InitializeAPIInstances();
    SetupAPIConnections();
    
    // 記錄系統初始化
    if (LogAPI)
    {
        LogAPI->LogMessage(TEXT("API System initialized"), ELogLevel::Info, ELogCategory::General);
    }
}

void UMingRTSAPISystem::InitializeAPIInstances()
{
    // 創建API實例
    CompilationAPI = NewObject<UMingRTSCompilationAPI>();
    LogAPI = NewObject<UMingRTSLogAPI>();
    DebugAPI = NewObject<UMingRTSDebugAPI>();
    OptimizationAPI = NewObject<UMingRTSOptimizationAPI>();
    SmartPanelAPI = NewObject<UMingRTSSmartPanelAPI>();
    CodeStandardsAPI = NewObject<UMingRTSCodeStandardsAPI>();
    BMADSystem = NewObject<UMingRTSBMADSystem>();
    CodeReviewAPI = NewObject<UMingRTSCodeReviewAPI>();
    UE5LearningAPI = NewObject<UMingRTSUE5LearningAPI>();
    PotatoAICodeGeneratorAPI = NewObject<UMingRTSPotatoAICodeGeneratorAPI>();
    
    // 初始化各個API
    if (CompilationAPI) CompilationAPI->InitializeCompilationSystem();
    if (LogAPI) LogAPI->InitializeLogging();
    if (DebugAPI) DebugAPI->InitializeDebugSystem();
    if (OptimizationAPI) OptimizationAPI->InitializeOptimization();
    if (SmartPanelAPI) SmartPanelAPI->InitializeSmartPanel();
    if (CodeStandardsAPI) CodeStandardsAPI->InitializeCodeStandards();
    if (BMADSystem) BMADSystem->InitializeBMADSystem();
    if (CodeReviewAPI) CodeReviewAPI->InitializeCodeReviewSystem();
    if (UE5LearningAPI) UE5LearningAPI->Initialize(FUE5LearningConfig());
    if (PotatoAICodeGeneratorAPI) PotatoAICodeGeneratorAPI->Initialize();
}

void UMingRTSAPISystem::SetupAPIConnections()
{
    // 設置API間的事件連接
    if (CompilationAPI && LogAPI)
    {
        CompilationAPI->OnCompilationCompleted.AddDynamic(this, &UMingRTSAPISystem::OnCompilationCompleted);
    }
    
    if (DebugAPI && LogAPI)
    {
        DebugAPI->OnDebugAnalysisCompleted.AddDynamic(this, &UMingRTSAPISystem::OnDebugAnalysisCompleted);
    }
    
    if (OptimizationAPI && LogAPI)
    {
        OptimizationAPI->OnOptimizationCompleted.AddDynamic(this, &UMingRTSAPISystem::OnOptimizationCompleted);
    }
}

UMingRTSCompilationAPI* UMingRTSAPISystem::GetCompilationAPI()
{
    return CompilationAPI;
}

UMingRTSLogAPI* UMingRTSAPISystem::GetLogAPI()
{
    return LogAPI;
}

UMingRTSDebugAPI* UMingRTSAPISystem::GetDebugAPI()
{
    return DebugAPI;
}

UMingRTSOptimizationAPI* UMingRTSAPISystem::GetOptimizationAPI()
{
    return OptimizationAPI;
}

UMingRTSSmartPanelAPI* UMingRTSAPISystem::GetSmartPanelAPI()
{
    return SmartPanelAPI;
}

UMingRTSCodeStandardsAPI* UMingRTSAPISystem::GetCodeStandardsAPI()
{
    return CodeStandardsAPI;
}

UMingRTSBMADSystem* UMingRTSAPISystem::GetBMADSystem()
{
    return BMADSystem;
}

UMingRTSCodeReviewAPI* UMingRTSAPISystem::GetCodeReviewAPI()
{
    return CodeReviewAPI;
}

UMingRTSUE5LearningAPI* UMingRTSAPISystem::GetUE5LearningAPI()
{
    return UE5LearningAPI;
}

UMingRTSPotatoAICodeGeneratorAPI* UMingRTSAPISystem::GetPotatoAICodeGeneratorAPI()
{
    return PotatoAICodeGeneratorAPI;
}

FAPICoordinationResult UMingRTSAPISystem::CoordinateAPIs(const TArray<EAPIType>& APIs, const FString& Task)
{
    FAPICoordinationResult Result;
    Result.CoordinationId = FGuid::NewGuid().ToString();
    Result.TotalTime = 0.0f;
    
    FDateTime StartTime = FDateTime::Now();
    
    // 根據任務類型確定執行順序
    TArray<FAPIInteraction> Workflow = SuggestOptimalWorkflow(Task);
    
    // 執行工作流
    bool bSuccess = ExecuteAPIWorkflow(Workflow);
    
    FDateTime EndTime = FDateTime::Now();
    Result.TotalTime = (EndTime - StartTime).GetTotalSeconds();
    Result.bSuccess = bSuccess;
    
    // 記錄協調結果
    RegisterAPIInteraction({
        EAPIType::SmartPanel, // 假設由智慧面板觸發
        APIs.Num() > 0 ? APIs[0] : EAPIType::Compilation,
        EAPIInteractionType::Control,
        Task,
        Result.CoordinationId,
        FDateTime::Now(),
        bSuccess,
        Result.TotalTime
    });
    
    // 學習從協調結果
    LearnFromCoordinationResult(Result);
    
    // 觸發事件
    OnCoordinationCompleted.Broadcast(Result);
    
    return Result;
}

bool UMingRTSAPISystem::ExecuteAPIWorkflow(const TArray<FAPIInteraction>& Workflow)
{
    for (const FAPIInteraction& Interaction : Workflow)
    {
        bool bSuccess = ProcessInteraction(Interaction);
        
        if (!bSuccess)
        {
            if (LogAPI)
            {
                LogAPI->LogMessage(FString::Printf(TEXT("API interaction failed: %s"), *Interaction.Action), 
                    ELogLevel::Error, ELogCategory::General);
            }
            return false;
        }
    }
    
    return true;
}

void UMingRTSAPISystem::RegisterAPIInteraction(const FAPIInteraction& Interaction)
{
    InteractionHistory.Add(Interaction);
    
    // 更新API性能指標
    if (APIMetrics.Contains(Interaction.TargetAPI))
    {
        APIMetrics[Interaction.TargetAPI] = (APIMetrics[Interaction.TargetAPI] + Interaction.ResponseTime) / 2.0f;
    }
    else
    {
        APIMetrics.Add(Interaction.TargetAPI, Interaction.ResponseTime);
    }
    
    // 觸發事件
    OnAPIInteraction.Broadcast(Interaction);
}

TArray<FAPIInteraction> UMingRTSAPISystem::SuggestOptimalWorkflow(const FString& Goal)
{
    TArray<FAPIInteraction> Workflow;
    
    // 基於歷史數據和學習結果建議最佳工作流
    if (Goal.Contains(TEXT("learn")) || Goal.Contains(TEXT("documentation")) || Goal.Contains(TEXT("ue5")))
    {
        Workflow.Add({
            EAPIType::SmartPanel, EAPIType::UE5Learning, EAPIInteractionType::Request,
            TEXT("start_learning"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
        
        Workflow.Add({
            EAPIType::UE5Learning, EAPIType::Log, EAPIInteractionType::Data,
            TEXT("document_progress"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
    }
    else if (Goal.Contains(TEXT("generate")) || Goal.Contains(TEXT("code")) || Goal.Contains(TEXT("class")))
    {
        Workflow.Add({
            EAPIType::SmartPanel, EAPIType::PotatoAICodeGen, EAPIInteractionType::Request,
            TEXT("generate_code"), Goal, FDateTime::Now(), true, 0.0f
        });
        
        Workflow.Add({
            EAPIType::PotatoAICodeGen, EAPIType::CodeStandards, EAPIInteractionType::Request,
            TEXT("check_quality"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
    }
    else if (Goal.Contains(TEXT("compile")) || Goal.Contains(TEXT("build")))
    {
        // 編譯工作流
        Workflow.Add({
            EAPIType::SmartPanel, EAPIType::Compilation, EAPIInteractionType::Request,
            TEXT("compile_project"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
        
        // 如果編譯失敗，請求日誌分析
        Workflow.Add({
            EAPIType::Compilation, EAPIType::Log, EAPIInteractionType::Request,
            TEXT("analyze_compilation_logs"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
        
        // 如果有錯誤，請求除錯會話
        Workflow.Add({
            EAPIType::Log, EAPIType::Debug, EAPIInteractionType::Request,
            TEXT("start_debug_session"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
    }
    else if (Goal.Contains(TEXT("optimize")) || Goal.Contains(TEXT("performance")))
    {
        // 優化工作流
        Workflow.Add({
            EAPIType::SmartPanel, EAPIType::Optimization, EAPIInteractionType::Request,
            TEXT("analyze_performance"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
        
        Workflow.Add({
            EAPIType::Optimization, EAPIType::Log, EAPIInteractionType::Request,
            TEXT("get_performance_logs"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
        
        Workflow.Add({
            EAPIType::Log, EAPIType::SmartPanel, EAPIInteractionType::Data,
            TEXT("update_performance_metrics"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
    }
    else if (Goal.Contains(TEXT("debug")) || Goal.Contains(TEXT("error")))
    {
        // 除錯工作流
        Workflow.Add({
            EAPIType::SmartPanel, EAPIType::Log, EAPIInteractionType::Request,
            TEXT("get_error_logs"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
        
        Workflow.Add({
            EAPIType::Log, EAPIType::Debug, EAPIInteractionType::Request,
            TEXT("analyze_errors"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
        
        Workflow.Add({
            EAPIType::Debug, EAPIType::Compilation, EAPIInteractionType::Request,
            TEXT("fix_compilation_errors"), TEXT(""), FDateTime::Now(), true, 0.0f
        });
    }
    
    return OptimizeWorkflow(Workflow);
}

FAPICoordinationResult UMingRTSAPISystem::ExecuteIntelligentTask(const FString& Task)
{
    // 使用AI決定最佳API組合
    TArray<EAPIType> RequiredAPIs;
    
    if (Task.Contains(TEXT("compile")) || Task.Contains(TEXT("build")))
    {
        RequiredAPIs.Add(EAPIType::Compilation);
    }
    if (Task.Contains(TEXT("log")) || Task.Contains(TEXT("analyze")))
    {
        RequiredAPIs.Add(EAPIType::Log);
    }
    if (Task.Contains(TEXT("debug")) || Task.Contains(TEXT("error")))
    {
        RequiredAPIs.Add(EAPIType::Debug);
    }
    if (Task.Contains(TEXT("optimize")) || Task.Contains(TEXT("performance")))
    {
        RequiredAPIs.Add(EAPIType::Optimization);
    }
    if (RequiredAPIs.Num() == 0)
    {
        RequiredAPIs.Add(EAPIType::SmartPanel); // 預設使用智慧面板
    }
    
    return CoordinateAPIs(RequiredAPIs, Task);
}

void UMingRTSAPISystem::LearnFromCoordinationResult(const FAPICoordinationResult& Result)
{
    // 分析協調結果並更新學習數據
    if (Result.bSuccess)
    {
        // 記錄成功的協調模式
        for (const FAPIInteraction& Interaction : Result.Interactions)
        {
            UpdateInteractionPatterns({Interaction});
        }
    }
    else
    {
        // 分析失敗原因並調整策略
        if (LogAPI)
        {
            LogAPI->LogMessage(FString::Printf(TEXT("Coordination failed: %s"), *Result.CoordinationId), 
                ELogLevel::Warning, ELogCategory::General);
        }
    }
    
    // 更新系統指標
    UpdateSystemMetrics();
}

void UMingRTSAPISystem::UpdateInteractionPatterns(const TArray<FAPIInteraction>& Interactions)
{
    // 更新交互模式學習
    for (const FAPIInteraction& Interaction : Interactions)
    {
        // 這裡可以實現更複雜的學習算法
        // 例如：機器學習、神經網絡等
    }
}

void UMingRTSAPISystem::OptimizeAPICommunication()
{
    // 基於歷史數據優化API通信
    if (InteractionHistory.Num() > 100)
    {
        // 分析最常見的交互模式
        TMap<FString, int32> InteractionCounts;
        
        for (const FAPIInteraction& Interaction : InteractionHistory)
        {
            FString Key = FString::Printf(TEXT("%s_%s"), 
                *UEnum::GetValueAsString(Interaction.SourceAPI),
                *UEnum::GetValueAsString(Interaction.TargetAPI));
            
            InteractionCounts.FindOrAdd(Key)++;
        }
        
        // 找出最有效的交互模式
        FString BestPattern;
        int32 MaxCount = 0;
        
        for (const auto& Pair : InteractionCounts)
        {
            if (Pair.Value > MaxCount)
            {
                MaxCount = Pair.Value;
                BestPattern = Pair.Key;
            }
        }
        
        if (LogAPI)
        {
            LogAPI->LogMessage(FString::Printf(TEXT("Optimal interaction pattern: %s (count: %d)"), 
                *BestPattern, MaxCount), ELogLevel::Info, ELogCategory::General);
        }
    }
}

TArray<FAPIInteraction> UMingRTSAPISystem::OptimizeWorkflow(const TArray<FAPIInteraction>& BaseWorkflow)
{
    // 基於歷史性能數據優化工作流
    TArray<FAPIInteraction> OptimizedWorkflow = BaseWorkflow;
    
    // 根據API性能指標重新排序
    OptimizedWorkflow.Sort([this](const FAPIInteraction& A, const FAPIInteraction& B)
    {
        float MetricA = APIMetrics.FindRef(A.TargetAPI);
        float MetricB = APIMetrics.FindRef(B.TargetAPI);
        return MetricA < MetricB; // 較小的響應時間優先
    });
    
    return OptimizedWorkflow;
}

bool UMingRTSAPISystem::ProcessInteraction(const FAPIInteraction& Interaction)
{
    FDateTime StartTime = FDateTime::Now();
    bool bSuccess = false;
    
    switch (Interaction.TargetAPI)
    {
        case EAPIType::Compilation:
            if (CompilationAPI)
            {
                if (Interaction.Action == TEXT("compile_project"))
                {
                    FCompilationContext Context;
                    FCompilationResult Result = CompilationAPI->CompileProject(Context);
                    bSuccess = Result.bSuccess;
                }
            }
            break;
            
        case EAPIType::Log:
            if (LogAPI)
            {
                if (Interaction.Action == TEXT("analyze_compilation_logs"))
                {
                    FLogFilter Filter;
                    FLogAnalysisResult Analysis = LogAPI->AnalyzeLogs(Filter);
                    bSuccess = true;
                }
            }
            break;
            
        case EAPIType::Debug:
            if (DebugAPI)
            {
                if (Interaction.Action == TEXT("start_debug_session"))
                {
                    FDebugSession Session = DebugAPI->StartDebugSession(EDebugMode::Step);
                    bSuccess = !Session.SessionId.IsEmpty();
                }
            }
            break;
            
        case EAPIType::Optimization:
            if (OptimizationAPI)
            {
                if (Interaction.Action == TEXT("analyze_performance"))
                {
                    FOptimizationProfile Profile;
                    FOptimizationResult Result = OptimizationAPI->OptimizePerformance(Profile);
                    bSuccess = Result.bSuccess;
                }
            }
            break;
            
        case EAPIType::SmartPanel:
            if (SmartPanelAPI)
            {
                if (Interaction.Action == TEXT("update_performance_metrics"))
                {
                    // 更新智慧面板的性能指標
                    bSuccess = true;
                }
            }
            break;
            
        case EAPIType::CodeStandards:
            if (CodeStandardsAPI)
            {
                if (Interaction.Action == TEXT("analyze_code"))
                {
                    FCodeStandardProfile Profile;
                    FCodeAnalysisResult Result = CodeStandardsAPI->AnalyzeCode(Interaction.Data, Profile);
                    bSuccess = Result.OverallQuality != ECodeQuality::Poor;
                }
                else if (Interaction.Action == TEXT("apply_standards"))
                {
                    FCodeStandardProfile Profile;
                    bSuccess = CodeStandardsAPI->ApplyCodeStandards(Interaction.Data, Profile);
                }
            }
            break;
            
        case EAPIType::UE5Learning:
            if (UE5LearningAPI)
            {
                if (Interaction.Action == TEXT("start_learning"))
                {
                    bSuccess = UE5LearningAPI->StartLearning(EUE5LearningMode::Comprehensive);
                }
                else if (Interaction.Action == TEXT("fetch_documentation"))
                {
                    EUE5DocSection Section = static_cast<EUE5DocSection>(FCString::Atoi(*Interaction.Data));
                    FUE5DocumentationContent Content = UE5LearningAPI->FetchDocumentation(Section);
                    bSuccess = !Content.Title.IsEmpty();
                }
            }
            break;
            
        case EAPIType::PotatoAICodeGen:
            if (PotatoAICodeGeneratorAPI)
            {
                if (Interaction.Action == TEXT("generate_code"))
                {
                    ECodeTemplateType Template = ECodeTemplateType::UE5_Class;
                    FGeneratedCodeResult Result = PotatoAICodeGeneratorAPI->GenerateCode(Interaction.Data, ECodeGenerationLanguage::CPP, Template);
                    bSuccess = Result.QualityScore > 70;
                }
                else if (Interaction.Action == TEXT("generate_class"))
                {
                    FCodeGenerationOptions Options;
                    FGeneratedCodeResult Result = PotatoAICodeGeneratorAPI->GenerateUE5Class(Interaction.Data, Options);
                    bSuccess = !Result.CodeContent.IsEmpty();
                }
            }
            break;
    }
    
    FDateTime EndTime = FDateTime::Now();
    float ResponseTime = (EndTime - StartTime).GetTotalSeconds();
    
    // 記錄交互結果
    FAPIInteraction ResultInteraction = Interaction;
    ResultInteraction.ResponseTime = ResponseTime;
    ResultInteraction.bSuccessful = bSuccess;
    RegisterAPIInteraction(ResultInteraction);
    
    return bSuccess;
}

void UMingRTSAPISystem::UpdateSystemMetrics()
{
    // 更新系統健康狀態
    TArray<FString> HealthStatus;
    
    for (const auto& Metric : APIMetrics)
    {
        FString Status = FString::Printf(TEXT("%s: %.2fms"), 
            *UEnum::GetValueAsString(Metric.Key), Metric.Value);
        HealthStatus.Add(Status);
    }
    
    OnSystemHealthChanged.Broadcast(HealthStatus);
}

// 事件處理函數
UFUNCTION()
void UMingRTSAPISystem::OnCompilationCompleted(const FCompilationResult& Result)
{
    if (!Result.bSuccess)
    {
        // 編譯失敗，自動觸發日誌分析
        if (LogAPI)
        {
            LogAPI->RequestCompilationAnalysis({Result.ErrorMessage});
        }
        
        // 觸發除錯會話
        if (DebugAPI)
        {
            DebugAPI->RequestCompilationFix({Result});
        }
    }
}

UFUNCTION()
void UMingRTSAPISystem::OnDebugAnalysisCompleted(const FDebugAnalysisResult& Analysis)
{
    if (Analysis.bHasErrors)
    {
        // 有錯誤，請求優化
        if (OptimizationAPI)
        {
            TMap<FString, float> Metrics;
            OptimizationAPI->RequestPerformanceOptimization(Metrics);
        }
    }
}

UFUNCTION()
void UMingRTSAPISystem::OnOptimizationCompleted(const FOptimizationResult& Result)
{
    // 優化完成，更新智慧面板
    if (SmartPanelAPI && Result.bSuccess)
    {
        // 更新面板數據
        FPanelData Data;
        Data.DataSource = TEXT("Optimization");
        Data.Value = FString::Printf(TEXT("%.2f%%"), Result.PerformanceImprovement);
        Data.Status = TEXT("Completed");
        
        SmartPanelAPI->UpdatePanelData(TEXT("OptimizationPanel"), TEXT("PerformanceImprovement"), Data);
    }
}
