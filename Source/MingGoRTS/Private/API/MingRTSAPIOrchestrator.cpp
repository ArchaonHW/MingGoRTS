// MingGoRTS API Orchestrator Implementation
// 整合所有 PowerShell 轉換後的 C++ API 系統實作

#include "MingRTSAPIOrchestrator.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingRTSAPIOrchestrator::UMingRTSAPIOrchestrator()
{
    PrimaryComponentTick.bCanEverTick = true;
    bAutoExecuteOnStart = false;
    DefaultExecutionMode = TEXT("Sequential");
    HealthCheckInterval = 60.0f;
    bEnablePerformanceMonitoring = true;
    TimeSinceLastHealthCheck = 0.0f;
    
    // 初始化子系統參考
    ScriptIntegrationAPI = nullptr;
    PotatoAIAPI = nullptr;
    UE5LearningAPI = nullptr;
    
    // 初始化系統狀態
    SystemInitializedStatus.Add(EIntegratedAPIType::ScriptIntegration, false);
    SystemInitializedStatus.Add(EIntegratedAPIType::PotatoAI, false);
    SystemInitializedStatus.Add(EIntegratedAPIType::UE5Learning, false);
    SystemInitializedStatus.Add(EIntegratedAPIType::VersionControl, false);
    SystemInitializedStatus.Add(EIntegratedAPIType::Documentation, false);
}

void UMingRTSAPIOrchestrator::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化協調器
    bool bInitSuccess = InitializeOrchestrator();
    OnOrchestratorInitialized.Broadcast(bInitSuccess);
    
    // 如果設定自動執行，則在啟動時執行
    if (bAutoExecuteOnStart && bInitSuccess)
    {
        TMap<FString, FString> EmptyParams;
        ExecuteAllSystems(EmptyParams);
    }
}

void UMingRTSAPIOrchestrator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 清理子系統
    ScriptIntegrationAPI = nullptr;
    PotatoAIAPI = nullptr;
    UE5LearningAPI = nullptr;
    
    Super::EndPlay(EndPlayReason);
}

void UMingRTSAPIOrchestrator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 定期健康檢查
    if (bEnablePerformanceMonitoring)
    {
        TimeSinceLastHealthCheck += DeltaTime;
        if (TimeSinceLastHealthCheck >= HealthCheckInterval)
        {
            PerformHealthCheck();
            TimeSinceLastHealthCheck = 0.0f;
        }
    }
    
    UpdatePerformanceMetrics();
}

bool UMingRTSAPIOrchestrator::InitializeOrchestrator()
{
    UE_LOG(LogTemp, Log, TEXT("🎯 初始化 MingGoRTS API Orchestrator..."));
    
    bool bSuccess = InitializeSubSystems();
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ API Orchestrator 初始化成功"));
        LogOrchestratorEvent(TEXT("Initialization"), TEXT("All subsystems initialized successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ API Orchestrator 初始化部分失敗"));
        LogOrchestratorEvent(TEXT("Initialization"), TEXT("Some subsystems failed to initialize"));
    }
    
    return bSuccess;
}

bool UMingRTSAPIOrchestrator::InitializeSubSystems()
{
    bool bAllSuccess = true;
    
    // 初始化腳本整合系統
    InitializeScriptIntegration();
    if (!SystemInitializedStatus[EIntegratedAPIType::ScriptIntegration])
    {
        bAllSuccess = false;
    }
    
    // 初始化 Potato AI 系統
    InitializePotatoAI();
    if (!SystemInitializedStatus[EIntegratedAPIType::PotatoAI])
    {
        bAllSuccess = false;
    }
    
    // 初始化 UE5 學習系統
    InitializeUE5Learning();
    if (!SystemInitializedStatus[EIntegratedAPIType::UE5Learning])
    {
        bAllSuccess = false;
    }
    
    // 初始化自動化開發系統
    InitializeAutoDevelopment();
    if (!SystemInitializedStatus[EIntegratedAPIType::AutoDevelopment])
    {
        bAllSuccess = false;
    }
    
    // 初始化版本控制系統
    InitializeVersionControl();
    if (!SystemInitializedStatus[EIntegratedAPIType::VersionControl])
    {
        bAllSuccess = false;
    }
    
    return bAllSuccess;
}

void UMingRTSAPIOrchestrator::InitializeScriptIntegration()
{
    if (GetOwner())
    {
        ScriptIntegrationAPI = NewObject<UMingRTSScriptIntegrationAPI>(GetOwner());
        if (ScriptIntegrationAPI)
        {
            ScriptIntegrationAPI->RegisterComponent();
            bool bInit = ScriptIntegrationAPI->InitializeScriptIntegration();
            SystemInitializedStatus[EIntegratedAPIType::ScriptIntegration] = bInit;
            
            UE_LOG(LogTemp, Log, TEXT("📝 腳本整合系統: %s"), bInit ? TEXT("✅ 就緒") : TEXT("❌ 失敗"));
        }
    }
}

void UMingRTSAPIOrchestrator::InitializePotatoAI()
{
    if (GetOwner())
    {
        PotatoAIAPI = NewObject<UMingRTSPotatoAIEnhancedAPI>(GetOwner());
        if (PotatoAIAPI)
        {
            PotatoAIAPI->RegisterComponent();
            bool bInit = PotatoAIAPI->InitializeEnhancedPotatoAI();
            SystemInitializedStatus[EIntegratedAPIType::PotatoAI] = bInit;
            
            UE_LOG(LogTemp, Log, TEXT("🥔 Potato AI 系統: %s"), bInit ? TEXT("✅ 就緒") : TEXT("❌ 失敗"));
        }
    }
}

void UMingRTSAPIOrchestrator::InitializeUE5Learning()
{
    if (GetOwner())
    {
        UE5LearningAPI = NewObject<UMingRTSUE5LearningAPI>(GetOwner());
        if (UE5LearningAPI)
        {
            UE5LearningAPI->RegisterComponent();
            bool bInit = UE5LearningAPI->InitializeUE5Learning();
            SystemInitializedStatus[EIntegratedAPIType::UE5Learning] = bInit;
            
            UE_LOG(LogTemp, Log, TEXT("📚 UE5 學習系統: %s"), bInit ? TEXT("✅ 就緒") : TEXT("❌ 失敗"));
        }
    }
}

void UMingRTSAPIOrchestrator::InitializeAutoDevelopment()
{
    if (GetOwner())
    {
        AutoDevelopmentAPI = NewObject<UMingRTSAutoDevelopmentAPI>(GetOwner());
        if (AutoDevelopmentAPI)
        {
            AutoDevelopmentAPI->RegisterComponent();
            bool bInit = AutoDevelopmentAPI->InitializeAutoDevelopment();
            SystemInitializedStatus[EIntegratedAPIType::AutoDevelopment] = bInit;
            
            UE_LOG(LogTemp, Log, TEXT("🔧 自動化開發系統: %s"), bInit ? TEXT("✅ 就緒") : TEXT("❌ 失敗"));
        }
    }
}

void UMingRTSAPIOrchestrator::InitializeVersionControl()
{
    if (GetOwner())
    {
        VersionControlAPI = NewObject<UMingRTSVersionControlAPI>(GetOwner());
        if (VersionControlAPI)
        {
            VersionControlAPI->RegisterComponent();
            bool bInit = VersionControlAPI->InitializeVersionControl();
            SystemInitializedStatus[EIntegratedAPIType::VersionControl] = bInit;
            
            UE_LOG(LogTemp, Log, TEXT("📁 版本控制系統: %s"), bInit ? TEXT("✅ 就緒") : TEXT("❌ 失敗"));
        }
    }
}

FAPIOrchestratorResult UMingRTSAPIOrchestrator::ExecuteSystem(EIntegratedAPIType SystemType, const TMap<FString, FString>& Parameters)
{
    FAPIOrchestratorResult Result;
    
    UE_LOG(LogTemp, Log, TEXT("🎯 執行整合系統: %d"), (int32)SystemType);
    
    switch (SystemType)
    {
        case EIntegratedAPIType::ScriptIntegration:
            Result = ExecuteScriptIntegration(Parameters);
            break;
            
        case EIntegratedAPIType::PotatoAI:
        {
            FString Requirement = Parameters.Contains(TEXT("Requirement")) ? 
                Parameters[TEXT("Requirement")] : TEXT("Generate UE5 class");
            Result = ExecutePotatoAIGeneration(Requirement, Parameters);
            break;
        }
            
        case EIntegratedAPIType::UE5Learning:
        {
            FString LearningMode = Parameters.Contains(TEXT("Mode")) ? 
                Parameters[TEXT("Mode")] : TEXT("Comprehensive");
            Result = ExecuteUE5Learning(LearningMode, Parameters);
            break;
        }
            
        case EIntegratedAPIType::AutoDevelopment:
            Result = ExecuteAutoDevelopment(EAutoDevelopmentMode::Full, EAutoDevelopmentTarget::Development);
            break;
            
        case EIntegratedAPIType::VersionControl:
        {
            FString Operation = Parameters.Contains(TEXT("Operation")) ? 
                Parameters[TEXT("Operation")] : TEXT("Status");
            Result = ExecuteVersionControl(Operation, Parameters);
            break;
        }
    }
    
    FDateTime EndTime = FDateTime::Now();
    Result.ExecutionTime = (EndTime - FDateTime::Now()).GetTotalSeconds();
    Result.Timestamp = FDateTime::Now();
    
    // 廣播系統執行事件
    OnSystemExecuted.Broadcast(SystemType, Result.Message);
    
    UE_LOG(LogTemp, Log, TEXT("✅ 系統執行完成: %s (%.2f 秒)"), *Result.Message, Result.ExecutionTime);
    
    return Result;
}

FAPIOrchestratorResult UMingRTSAPIOrchestrator::ExecuteAllSystems(const TMap<FString, FString>& Parameters)
{
    FDateTime StartTime = FDateTime::Now();
    FAPIOrchestratorResult Result;
    Result.bSuccess = true;
    
    UE_LOG(LogTemp, Log, TEXT("🎯 開始執行所有整合系統..."));
    
    // 依序執行所有系統
    TArray<EIntegratedAPIType> SystemsToExecute = {
        EIntegratedAPIType::ScriptIntegration,
        EIntegratedAPIType::PotatoAI,
        EIntegratedAPIType::UE5Learning,
        EIntegratedAPIType::AutoDevelopment,
        EIntegratedAPIType::VersionControl
    };
    
    for (EIntegratedAPIType SystemType : SystemsToExecute)
    {
        if (SystemInitializedStatus[SystemType])
        {
            FAPIOrchestratorResult SystemResult = ExecuteSystem(SystemType, Parameters);
            Result.SystemResults.Add(SystemType, SystemResult.Message);
            
            if (!SystemResult.bSuccess)
            {
                Result.bSuccess = false;
            }
        }
    }
    
    FDateTime EndTime = FDateTime::Now();
    Result.ExecutionTime = (EndTime - StartTime).GetTotalSeconds();
    Result.Timestamp = FDateTime::Now();
    Result.Message = Result.bSuccess ? TEXT("所有系統執行成功") : TEXT("部分系統執行失敗");
    
    UE_LOG(LogTemp, Log, TEXT("✅ 所有系統執行完成 (%.2f 秒)"), Result.ExecutionTime);
    
    return Result;
}

FAPIOrchestratorResult UMingRTSAPIOrchestrator::ExecuteScriptIntegration(const TMap<FString, FString>& Parameters)
{
    FAPIOrchestratorResult Result;
    
    if (!ScriptIntegrationAPI || !SystemInitializedStatus[EIntegratedAPIType::ScriptIntegration])
    {
        Result.bSuccess = false;
        Result.Message = TEXT("腳本整合系統未初始化");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("📝 執行腳本整合..."));
    
    // 取得執行模式
    EScriptCategory Category = EScriptCategory::All;
    if (Parameters.Contains(TEXT("Category")))
    {
        FString CategoryStr = Parameters[TEXT("Category")];
        if (CategoryStr == TEXT("Build")) Category = EScriptCategory::Build;
        else if (CategoryStr == TEXT("AI")) Category = EScriptCategory::AI;
        else if (CategoryStr == TEXT("API")) Category = EScriptCategory::API;
    }
    
    bool bDryRun = Parameters.Contains(TEXT("DryRun")) ? 
        Parameters[TEXT("DryRun")] == TEXT("true") : false;
    
    // 執行腳本
    FIntegrationSession Session = ScriptIntegrationAPI->ExecuteScriptsSequential(Category, bDryRun);
    
    Result.bSuccess = (Session.FailureCount == 0);
    Result.Message = FString::Printf(TEXT("腳本整合完成: %d 成功, %d 失敗"), 
        Session.SuccessCount, Session.FailureCount);
    
    // 生成報告
    if (Parameters.Contains(TEXT("GenerateReport")) && Parameters[TEXT("GenerateReport")] == TEXT("true"))
    {
        FString ReportPath = FPaths::ProjectDir() + TEXT("Reports/ScriptIntegration_") + 
            FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss")) + TEXT(".html");
        ScriptIntegrationAPI->GenerateIntegrationReport(Session, ReportPath);
    }
    
    return Result;
}

FAPIOrchestratorResult UMingRTSAPIOrchestrator::ExecutePotatoAIGeneration(const FString& Requirement, const TMap<FString, FString>& Parameters)
{
    FAPIOrchestratorResult Result;
    
    if (!PotatoAIAPI || !SystemInitializedStatus[EIntegratedAPIType::PotatoAI])
    {
        Result.bSuccess = false;
        Result.Message = TEXT("Potato AI 系統未初始化");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("🥔 執行 Potato AI 程式碼生成: %s"), *Requirement);
    
    // 設定選項
    FPotatoAIEnhancedOptions Options;
    
    if (Parameters.Contains(TEXT("Complexity")))
    {
        FString ComplexityStr = Parameters[TEXT("Complexity")];
        if (ComplexityStr == TEXT("Simple")) Options.Complexity = EPotatoAICodeComplexity::Simple;
        else if (ComplexityStr == TEXT("Complex")) Options.Complexity = EPotatoAICodeComplexity::Complex;
    }
    
    Options.bIncludeTests = !Parameters.Contains(TEXT("SkipTests"));
    Options.bIncludeDocumentation = !Parameters.Contains(TEXT("SkipDocs"));
    
    // 執行生成
    FPotatoAIEnhancedCodeGenerationResult GenerationResult = PotatoAIAPI->GenerateEnhancedCode(Requirement, Options);
    
    Result.bSuccess = GenerationResult.bGenerationSuccessful;
    Result.Message = GenerationResult.bGenerationSuccessful ? 
        FString::Printf(TEXT("程式碼生成成功: %s (品質評分: %d)"), 
            *GenerationResult.ClassName, GenerationResult.QualityAssessment.OverallScore) :
        FString::Printf(TEXT("程式碼生成失敗: %s"), *GenerationResult.ErrorMessage);
    
    return Result;
}

FAPIOrchestratorResult UMingRTSAPIOrchestrator::ExecuteUE5Learning(const FString& LearningMode, const TMap<FString, FString>& Parameters)
{
    FAPIOrchestratorResult Result;
    
    if (!UE5LearningAPI || !SystemInitializedStatus[EIntegratedAPIType::UE5Learning])
    {
        Result.bSuccess = false;
        Result.Message = TEXT("UE5 學習系統未初始化");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("📚 執行 UE5 學習系統: %s"), *LearningMode);
    
    // 執行學習
    FUE5LearningResult LearningResult = UE5LearningAPI->StartLearning(LearningMode);
    
    Result.bSuccess = LearningResult.bLearningCompleted;
    Result.Message = FString::Printf(TEXT("UE5 學習完成: %d 個概念已學習"), 
        LearningResult.LearnedConcepts.Num());
    
    return Result;
}

FAPIOrchestratorHealth UMingRTSAPIOrchestrator::CheckSystemHealth()
{
    PerformHealthCheck();
    return CurrentHealth;
}

void UMingRTSAPIOrchestrator::PerformHealthCheck()
{
    // 檢查各子系統健康狀況
    CurrentHealth.bScriptIntegrationHealthy = ScriptIntegrationAPI != nullptr && 
        SystemInitializedStatus[EIntegratedAPIType::ScriptIntegration];
    
    CurrentHealth.bPotatoAIHealthy = PotatoAIAPI != nullptr && 
        SystemInitializedStatus[EIntegratedAPIType::PotatoAI];
    
    CurrentHealth.bUE5LearningHealthy = UE5LearningAPI != nullptr && 
        SystemInitializedStatus[EIntegratedAPIType::UE5Learning];
    
    CurrentHealth.bAutoDevelopmentHealthy = AutoDevelopmentAPI != nullptr && 
        SystemInitializedStatus[EIntegratedAPIType::AutoDevelopment];
    
    CurrentHealth.bVersionControlHealthy = VersionControlAPI != nullptr && 
        SystemInitializedStatus[EIntegratedAPIType::VersionControl];
    CurrentHealth.bDocumentationHealthy = true; // 預設為 true
    
    // 計算整體健康分數
    CurrentHealth.OverallHealthScore = CalculateHealthScore();
    
    // 生成健康報告
    CurrentHealth.HealthReport = GenerateHealthReport();
    
    // 廣播健康更新事件
    OnOrchestratorHealthUpdated.Broadcast(CurrentHealth);
    
    UE_LOG(LogTemp, Log, TEXT("🏥 系統健康檢查: %d/100"), CurrentHealth.OverallHealthScore);
}

int32 UMingRTSAPIOrchestrator::CalculateHealthScore()
{
    int32 Score = 0;
    int32 MaxScore = 6; // 6 個系統
    
    if (CurrentHealth.bScriptIntegrationHealthy) Score++;
    if (CurrentHealth.bPotatoAIHealthy) Score++;
    if (CurrentHealth.bUE5LearningHealthy) Score++;
    if (CurrentHealth.bAutoDevelopmentHealthy) Score++;
    if (CurrentHealth.bVersionControlHealthy) Score++;
    
    return (Score * 100) / MaxScore;
}

FString UMingRTSAPIOrchestrator::GenerateHealthReport()
{
    FString Report;
    
    Report += TEXT("## MingGoRTS API Orchestrator 健康報告\n\n");
    Report += FString::Printf(TEXT("**檢查時間:** %s\n\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("**整體健康分數:** %d/100\n\n"), CurrentHealth.OverallHealthScore);
    
    Report += TEXT("### 子系統狀態\n\n");
    Report += FString::Printf(TEXT("- **腳本整合系統:** %s\n"), 
        CurrentHealth.bScriptIntegrationHealthy ? TEXT("✅ 健康") : TEXT("❌ 異常"));
    Report += FString::Printf(TEXT("- **Potato AI 系統:** %s\n"), 
        CurrentHealth.bPotatoAIHealthy ? TEXT("✅ 健康") : TEXT("❌ 異常"));
    Report += FString::Printf(TEXT("- **UE5 學習系統:** %s\n"), 
        CurrentHealth.bUE5LearningHealthy ? TEXT("✅ 健康") : TEXT("❌ 異常"));
    Report += FString::Printf(TEXT("- **自動化開發系統:** %s\n"), 
        CurrentHealth.bAutoDevelopmentHealthy ? TEXT("✅ 健康") : TEXT("❌ 異常"));
    Report += FString::Printf(TEXT("- **版本控制系統:** %s\n\n"), 
        CurrentHealth.bVersionControlHealthy ? TEXT("✅ 健康") : TEXT("❌ 異常"));
    
    if (CurrentHealth.OverallHealthScore < 80)
    {
        Report += TEXT("### ⚠️ 建議\n\n");
        Report += TEXT("部分系統未正常運作，建議檢查:\n");
        if (!CurrentHealth.bAutoDevelopmentHealthy) Report += TEXT("- 自動化開發系統初始化\n");
        if (!CurrentHealth.bVersionControlHealthy) Report += TEXT("- 版本控制系統初始化\n");
        if (!CurrentHealth.bPotatoAIHealthy) Report += TEXT("- Potato AI 系統配置\n");
        if (!CurrentHealth.bUE5LearningHealthy) Report += TEXT("- UE5 學習系統連線\n");
    }
    
    return Report;
}

bool UMingRTSAPIOrchestrator::IsSystemInitialized(EIntegratedAPIType SystemType)
{
    return SystemInitializedStatus.Contains(SystemType) ? SystemInitializedStatus[SystemType] : false;
}

TArray<EIntegratedAPIType> UMingRTSAPIOrchestrator::GetInitializedSystems()
{
    TArray<EIntegratedAPIType> InitializedSystems;
    
    for (const auto& Pair : SystemInitializedStatus)
    {
        if (Pair.Value)
        {
            InitializedSystems.Add(Pair.Key);
        }
    }
    
    return InitializedSystems;
}

bool UMingRTSAPIOrchestrator::GenerateOrchestratorReport(const FString& ReportPath)
{
    FString ReportContent;
    
    ReportContent += TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    ReportContent += TEXT("<title>MingGoRTS API Orchestrator 報告</title>\n");
    ReportContent += TEXT("<style>\n");
    ReportContent += TEXT("body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    ReportContent += TEXT(".header { background-color: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
    ReportContent += TEXT(".section { background-color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".healthy { color: #27ae60; }\n");
    ReportContent += TEXT(".unhealthy { color: #e74c3c; }\n");
    ReportContent += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 10px; }\n");
    ReportContent += TEXT("th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    ReportContent += TEXT("th { background-color: #f8f9fa; }\n");
    ReportContent += TEXT("</style>\n");
    ReportContent += TEXT("</head>\n<body>\n");
    
    // 標題
    ReportContent += TEXT("<div class=\"header\">\n");
    ReportContent += TEXT("<h1>🎯 MingGoRTS API Orchestrator 報告</h1>\n");
    ReportContent += FString::Printf(TEXT("<p>生成時間: %s</p>\n"), *FDateTime::Now().ToString());
    ReportContent += FString::Printf(TEXT("<p>整體健康狀態: %d/100</p>\n"), CurrentHealth.OverallHealthScore);
    ReportContent += TEXT("</div>\n");
    
    // 系統狀態
    ReportContent += TEXT("<div class=\"section\">\n");
    ReportContent += TEXT("<h2>子系統狀態</h2>\n");
    ReportContent += TEXT("<table>\n");
    ReportContent += TEXT("<tr><th>系統</th><th>狀態</th><th>初始化</th></tr>\n");
    
    for (const auto& Pair : SystemInitializedStatus)
    {
        FString SystemName = GetSystemStatusString(Pair.Key);
        FString StatusClass = Pair.Value ? TEXT("healthy") : TEXT("unhealthy");
        FString StatusText = Pair.Value ? TEXT("✅ 已初始化") : TEXT("❌ 未初始化");
        
        ReportContent += FString::Printf(TEXT("<tr><td>%s</td><td class=\"%s\">%s</td><td>%s</td></tr>\n"),
            *SystemName, *StatusClass, *StatusText, *StatusText);
    }
    
    ReportContent += TEXT("</table>\n");
    ReportContent += TEXT("</div>\n");
    
    // 健康報告
    ReportContent += TEXT("<div class=\"section\">\n");
    ReportContent += TEXT("<h2>健康報告</h2>\n");
    ReportContent += TEXT("<pre>\n");
    ReportContent += CurrentHealth.HealthReport;
    ReportContent += TEXT("</pre>\n");
    ReportContent += TEXT("</div>\n");
    
    ReportContent += TEXT("</body>\n</html>");
    
    // 確保目錄存在
    FString ReportDir = FPaths::GetPath(ReportPath);
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*ReportDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*ReportDir);
    }
    
    return FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
}

UMingRTSScriptIntegrationAPI* UMingRTSAPIOrchestrator::GetScriptIntegrationAPI()
{
    return ScriptIntegrationAPI;
}

UMingRTSPotatoAIEnhancedAPI* UMingRTSAPIOrchestrator::GetPotatoAIAPI()
{
    return PotatoAIAPI;
}

UMingRTSUE5LearningAPI* UMingRTSAPIOrchestrator::GetUE5LearningAPI()
{
    return UE5LearningAPI;
}

UMingRTSAutoDevelopmentAPI* UMingRTSAPIOrchestrator::GetAutoDevelopmentAPI()
{
    return AutoDevelopmentAPI;
}

UMingRTSVersionControlAPI* UMingRTSAPIOrchestrator::GetVersionControlAPI()
{
    return VersionControlAPI;
}

FString UMingRTSAPIOrchestrator::GetSystemStatusString(EIntegratedAPIType SystemType)
{
    switch (SystemType)
    {
        case EIntegratedAPIType::ScriptIntegration: return TEXT("腳本整合系統");
        case EIntegratedAPIType::PotatoAI: return TEXT("Potato AI 系統");
        case EIntegratedAPIType::UE5Learning: return TEXT("UE5 學習系統");
        case EIntegratedAPIType::AutoDevelopment: return TEXT("自動化開發系統");
        case EIntegratedAPIType::VersionControl: return TEXT("版本控制系統");
        case EIntegratedAPIType::Documentation: return TEXT("文件系統");
        default: return TEXT("未知系統");
    }
}

FAPIOrchestratorResult UMingRTSAPIOrchestrator::ExecuteAutoDevelopment(EAutoDevelopmentMode Mode, EAutoDevelopmentTarget Target)
{
    FAPIOrchestratorResult Result;
    
    if (!AutoDevelopmentAPI || !SystemInitializedStatus[EIntegratedAPIType::AutoDevelopment])
    {
        Result.bSuccess = false;
        Result.Message = TEXT("自動化開發系統未初始化");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("🔧 執行自動化開發流程..."));
    
    FAutoDevelopmentResult DevResult = AutoDevelopmentAPI->ExecuteFullAutomation(Mode, Target);
    
    Result.bSuccess = DevResult.bOverallSuccess;
    Result.Message = FString::Printf(TEXT("自動化開發完成: %s (耗時 %.1f 秒)"), 
        DevResult.bOverallSuccess ? TEXT("成功") : TEXT("部分失敗"), 
        DevResult.ExecutionTime);
    
    return Result;
}

FAPIOrchestratorResult UMingRTSAPIOrchestrator::ExecuteVersionControl(const FString& Operation, const TMap<FString, FString>& Parameters)
{
    FAPIOrchestratorResult Result;
    
    if (!VersionControlAPI || !SystemInitializedStatus[EIntegratedAPIType::VersionControl])
    {
        Result.bSuccess = false;
        Result.Message = TEXT("版本控制系統未初始化");
        return Result;
    }
    
    UE_LOG(LogTemp, Log, TEXT("📁 執行版本控制操作: %s"), *Operation);
    
    if (Operation == TEXT("Status"))
    {
        FVCStateInfo State = VersionControlAPI->GetVCStatus(true);
        Result.bSuccess = true;
        Result.Message = FString::Printf(TEXT("當前分支: %s, 未提交更改: %d"), 
            *State.CurrentBranch, State.UncommittedCount);
    }
    else if (Operation == TEXT("Commit"))
    {
        FString Message = Parameters.Contains(TEXT("Message")) ? Parameters[TEXT("Message")] : TEXT("Auto commit");
        FVCCommitInfo Commit = VersionControlAPI->CreateCommit(Message);
        Result.bSuccess = !Commit.Hash.IsEmpty();
        Result.Message = FString::Printf(TEXT("提交創建: %s"), *Commit.ShortHash);
    }
    else
    {
        Result.bSuccess = false;
        Result.Message = FString::Printf(TEXT("未知操作: %s"), *Operation);
    }
    
    return Result;
}

void UMingRTSAPIOrchestrator::LogOrchestratorEvent(const FString& Event, const FString& Details)
{
    FString LogEntry = FString::Printf(TEXT("[%s] [Orchestrator] %s: %s"),
        *FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss")), *Event, *Details);
    
    UE_LOG(LogTemp, Log, TEXT("%s"), *LogEntry);
    
    // 也可以寫入檔案
    FString LogFilePath = FPaths::ProjectDir() + TEXT("Logs/Orchestrator_") + 
        FDateTime::Now().ToString(TEXT("yyyyMMdd")) + TEXT(".log");
    
    FString ExistingContent;
    FFileHelper::LoadFileToString(ExistingContent, *LogFilePath);
    ExistingContent += LogEntry + TEXT("\n");
    FFileHelper::SaveStringToFile(ExistingContent, *LogFilePath);
}

void UMingRTSAPIOrchestrator::UpdatePerformanceMetrics()
{
    if (!bEnablePerformanceMonitoring)
    {
        return;
    }
    
    // 更新效能指標
    // 這裡可以收集各種效能數據
}
