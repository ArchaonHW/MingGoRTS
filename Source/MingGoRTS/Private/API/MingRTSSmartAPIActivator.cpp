#include "MingRTSSmartAPIActivator.h"
#include "MingRTSCompilationAPI.h"
#include "MingRTSLogAPI.h"
#include "MingRTSDebugAPI.h"
#include "MingRTSOptimizationAPI.h"
#include "MingRTSSmartPanelAPI.h"
#include "MingRTSCodeStandardsAPI.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"

UMingRTSSmartAPIActivator::UMingRTSSmartAPIActivator()
{
    bSystemEnabled = false;
    bIntelligentModeEnabled = true;
    bAutoLearningEnabled = true;
    bPredictiveFeaturesEnabled = true;
    CoordinationMode = TEXT("Intelligent");
}

void UMingRTSSmartAPIActivator::EnableSmartAPISystem()
{
    if (bSystemEnabled)
    {
        return; // 已經啟用
    }

    // 創建並初始化API系統
    APISystem = NewObject<UMingRTSAPISystem>();
    APISystem->InitializeAPISystem();

    // 配置智慧功能
    InitializeSmartFeatures();
    SetupIntelligentWorkflows();
    StartBackgroundMonitoring();

    bSystemEnabled = true;

    // 觸發事件
    OnSmartSystemEnabled.Broadcast(true);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
            TEXT("🚀 Smart API System Enabled Successfully!"));
    }
}

void UMingRTSSmartAPIActivator::DisableSmartAPISystem()
{
    if (!bSystemEnabled)
    {
        return; // 已經禁用
    }

    // 清理API系統
    if (APISystem)
    {
        APISystem = nullptr;
    }

    bSystemEnabled = false;

    // 觸發事件
    OnSmartSystemEnabled.Broadcast(false);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
            TEXT("🛑 Smart API System Disabled"));
    }
}

bool UMingRTSSmartAPIActivator::IsSmartAPISystemEnabled()
{
    return bSystemEnabled;
}

void UMingRTSSmartAPIActivator::ConfigureIntelligentMode(bool bEnabled)
{
    bIntelligentModeEnabled = bEnabled;
    
    if (APISystem)
    {
        // 配置所有API的智慧模式
        if (APISystem->GetCompilationAPI())
        {
            // 配置編譯API智慧模式
        }
        if (APISystem->GetLogAPI())
        {
            // 配置日誌API智慧模式
        }
        if (APISystem->GetDebugAPI())
        {
            // 配置除錯API智慧模式
        }
        if (APISystem->GetOptimizationAPI())
        {
            APISystem->GetOptimizationAPI()->EnablePredictiveMode(bEnabled);
        }
        if (APISystem->GetSmartPanelAPI())
        {
            APISystem->GetSmartPanelAPI()->EnablePredictiveMode(TEXT("MainPanel"), bEnabled);
        }
        if (APISystem->GetCodeStandardsAPI())
        {
            APISystem->GetCodeStandardsAPI()->EnableIntelligentMode(bEnabled);
        }
    }

    OnIntelligentActionCompleted.Broadcast(TEXT("ConfigureIntelligentMode"));
}

void UMingRTSSmartAPIActivator::SetAutoLearning(bool bEnabled)
{
    bAutoLearningEnabled = bEnabled;
    
    if (APISystem)
    {
        // 配置所有API的自動學習
        if (APISystem->GetOptimizationAPI())
        {
            APISystem->GetOptimizationAPI()->EnableAutoOptimization(bEnabled);
        }
        if (APISystem->GetSmartPanelAPI())
        {
            // 配置智慧面板自動學習
        }
        if (APISystem->GetCodeStandardsAPI())
        {
            APISystem->GetCodeStandardsAPI()->EnableAutoFix(bEnabled);
        }
    }

    OnIntelligentActionCompleted.Broadcast(TEXT("SetAutoLearning"));
}

void UMingRTSSmartAPIActivator::EnablePredictiveFeatures(bool bEnabled)
{
    bPredictiveFeaturesEnabled = bEnabled;
    
    if (APISystem)
    {
        APISystem->EnablePredictiveMode(bEnabled);
    }

    OnIntelligentActionCompleted.Broadcast(TEXT("EnablePredictiveFeatures"));
}

void UMingRTSSmartAPIActivator::SetCoordinationMode(const FString& Mode)
{
    CoordinationMode = Mode;
    
    if (APISystem)
    {
        APISystem->EnableAutoCoordination(Mode == TEXT("Automatic"));
    }

    OnIntelligentActionCompleted.Broadcast(TEXT("SetCoordinationMode"));
}

void UMingRTSSmartAPIActivator::QuickCompileAndOptimize()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 執行智慧編譯和優化工作流
    FAPICoordinationResult Result = APISystem->ExecuteIntelligentTask(TEXT("compile_and_optimize"));
    
    if (Result.bSuccess)
    {
        OnIntelligentActionCompleted.Broadcast(TEXT("QuickCompileAndOptimize"));
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                TEXT("✅ Compile and Optimize Completed Successfully!"));
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
                TEXT("❌ Compile and Optimize Failed"));
        }
    }
}

void UMingRTSSmartAPIActivator::QuickDebugAndFix()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 執行智慧除錯和修復工作流
    FAPICoordinationResult Result = APISystem->ExecuteIntelligentTask(TEXT("debug_and_fix"));
    
    if (Result.bSuccess)
    {
        OnIntelligentActionCompleted.Broadcast(TEXT("QuickDebugAndFix"));
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                TEXT("🔧 Debug and Fix Completed Successfully!"));
        }
    }
}

void UMingRTSSmartAPIActivator::QuickAnalyzeAndImprove()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 執行智慧分析和改進工作流
    FAPICoordinationResult Result = APISystem->ExecuteIntelligentTask(TEXT("analyze_and_improve"));
    
    if (Result.bSuccess)
    {
        OnIntelligentActionCompleted.Broadcast(TEXT("QuickAnalyzeAndImprove"));
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                TEXT("📊 Analyze and Improve Completed Successfully!"));
        }
    }
}

void UMingRTSSmartAPIActivator::QuickHealthCheck()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 執行系統健康檢查
    TArray<FString> HealthStatus = GetSystemStatus();
    
    OnSystemHealthUpdated.Broadcast(HealthStatus);
    OnIntelligentActionCompleted.Broadcast(TEXT("QuickHealthCheck"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, 
            TEXT("🏥 System Health Check Completed"));
    }
}

void UMingRTSSmartAPIActivator::StartIntelligentCompilation()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 啟動智慧編譯工作流
    TArray<EAPIType> APIs = {EAPIType::CodeStandards, EAPIType::Compilation, EAPIType::Log};
    FAPICoordinationResult Result = APISystem->CoordinateAPIs(APIs, TEXT("intelligent_compilation"));
    
    OnIntelligentActionCompleted.Broadcast(TEXT("StartIntelligentCompilation"));
}

void UMingRTSSmartAPIActivator::StartPerformanceOptimization()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 啟動性能優化工作流
    TArray<EAPIType> APIs = {EAPIType::Optimization, EAPIType::Log, EAPIType::SmartPanel};
    FAPICoordinationResult Result = APISystem->CoordinateAPIs(APIs, TEXT("performance_optimization"));
    
    OnIntelligentActionCompleted.Broadcast(TEXT("StartPerformanceOptimization"));
}

void UMingRTSSmartAPIActivator::StartCodeQualityImprovement()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 啟動代碼品質改進工作流
    TArray<EAPIType> APIs = {EAPIType::CodeStandards, EAPIType::Compilation, EAPIType::Debug};
    FAPICoordinationResult Result = APISystem->CoordinateAPIs(APIs, TEXT("code_quality_improvement"));
    
    OnIntelligentActionCompleted.Broadcast(TEXT("StartCodeQualityImprovement"));
}

void UMingRTSSmartAPIActivator::StartSystemMonitoring()
{
    if (!bSystemEnabled || !APISystem)
    {
        return;
    }

    // 啟動系統監控工作流
    TArray<EAPIType> APIs = {EAPIType::Log, EAPIType::Optimization, EAPIType::SmartPanel};
    FAPICoordinationResult Result = APISystem->CoordinateAPIs(APIs, TEXT("system_monitoring"));
    
    OnIntelligentActionCompleted.Broadcast(TEXT("StartSystemMonitoring"));
}

TArray<FString> UMingRTSSmartAPIActivator::GetSystemStatus()
{
    TArray<FString> Status;
    
    if (!bSystemEnabled)
    {
        Status.Add(TEXT("Smart API System: Disabled"));
        return Status;
    }

    Status.Add(TEXT("Smart API System: Enabled"));
    
    if (bIntelligentModeEnabled)
    {
        Status.Add(TEXT("Intelligent Mode: Active"));
    }
    
    if (bAutoLearningEnabled)
    {
        Status.Add(TEXT("Auto Learning: Active"));
    }
    
    if (bPredictiveFeaturesEnabled)
    {
        Status.Add(TEXT("Predictive Features: Active"));
    }
    
    Status.Add(FString::Printf(TEXT("Coordination Mode: %s"), *CoordinationMode));

    // 獲取API健康狀態
    if (APISystem)
    {
        TArray<FString> APIHealth = APISystem->GetSystemHealthStatus();
        Status.Append(APIHealth);
    }

    return Status;
}

TMap<FString, float> UMingRTSSmartAPIActivator::GetPerformanceMetrics()
{
    TMap<FString, float> Metrics;
    
    if (APISystem)
    {
        Metrics = APISystem->GetAPIPerformanceMetrics();
    }
    
    return Metrics;
}

TArray<FString> UMingRTSSmartAPIActivator::GetActiveFeatures()
{
    TArray<FString> Features;
    
    if (bSystemEnabled)
    {
        Features.Add(TEXT("Smart API System"));
    }
    
    if (bIntelligentModeEnabled)
    {
        Features.Add(TEXT("Intelligent Mode"));
    }
    
    if (bAutoLearningEnabled)
    {
        Features.Add(TEXT("Auto Learning"));
    }
    
    if (bPredictiveFeaturesEnabled)
    {
        Features.Add(TEXT("Predictive Features"));
    }
    
    return Features;
}

void UMingRTSSmartAPIActivator::InitializeSmartFeatures()
{
    // 初始化所有智慧功能
    ConfigureIntelligentMode(bIntelligentModeEnabled);
    SetAutoLearning(bAutoLearningEnabled);
    EnablePredictiveFeatures(bPredictiveFeaturesEnabled);
    SetCoordinationMode(CoordinationMode);
}

void UMingRTSSmartAPIActivator::SetupIntelligentWorkflows()
{
    // 設置智慧工作流
    ConfigureAPICoordination();
    EnableCrossAPICommunication();
    InitializeLearningSystems();
    StartPredictiveAnalysis();
}

void UMingRTSSmartAPIActivator::StartBackgroundMonitoring()
{
    // 啟動後台監控
    if (APISystem)
    {
        // 配置自動協調
        APISystem->EnableAutoCoordination(true);
    }
}

void UMingRTSSmartAPIActivator::ConfigureAPICoordination()
{
    // 配置API協調參數
    if (APISystem)
    {
        // 設置自動協調為啟用狀態
        APISystem->EnableAutoCoordination(true);
    }
}

void UMingRTSSmartAPIActivator::EnableCrossAPICommunication()
{
    // 啟用跨API通信
    if (APISystem)
    {
        // 設置API間的事件連接和通信
    }
}

void UMingRTSSmartAPIActivator::InitializeLearningSystems()
{
    // 初始化學習系統
    if (APISystem)
    {
        // 配置所有API的學習功能
        if (APISystem->GetOptimizationAPI())
        {
            APISystem->GetOptimizationAPI()->EnableAutoOptimization(bAutoLearningEnabled);
        }
        
        if (APISystem->GetCodeStandardsAPI())
        {
            APISystem->GetCodeStandardsAPI()->EnableAutoFix(bAutoLearningEnabled);
        }
    }
}

void UMingRTSSmartAPIActivator::StartPredictiveAnalysis()
{
    // 啟動預測分析
    if (APISystem && bPredictiveFeaturesEnabled)
    {
        APISystem->EnablePredictiveMode(true);
    }
}
