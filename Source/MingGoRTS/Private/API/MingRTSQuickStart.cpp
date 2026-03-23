#include "MingRTSQuickStart.h"
#include "Engine/Engine.h"
#include "Misc/DateTime.h"

// 靜態成員初始化
TObjectPtr<UMingRTSSmartAPIActivator> UMingRTSQuickStart::GlobalActivator = nullptr;

UMingRTSQuickStart::UMingRTSQuickStart()
{
}

void UMingRTSQuickStart::EnableSmartAPI()
{
    if (!GlobalActivator.IsValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->EnableSmartAPISystem();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                TEXT("🚀 智慧API系統已啟用！"));
        }
    }
}

void UMingRTSQuickStart::QuickCompile()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->QuickCompileAndOptimize();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, 
                TEXT("⚡ 快速編譯已啟動..."));
        }
    }
}

void UMingRTSQuickStart::QuickDebug()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->QuickDebugAndFix();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, 
                TEXT("🔧 快速除錯已啟動..."));
        }
    }
}

void UMingRTSQuickStart::QuickOptimize()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->StartPerformanceOptimization();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, 
                TEXT("📊 快速優化已啟動..."));
        }
    }
}

void UMingRTSQuickStart::QuickCodeCheck()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->StartCodeQualityImprovement();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, 
                TEXT("🔍 代碼品質檢查已啟動..."));
        }
    }
}

TArray<FString> UMingRTSQuickStart::GetSystemStatus()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        return GlobalActivator->GetSystemStatus();
    }
    
    TArray<FString> EmptyStatus;
    EmptyStatus.Add(TEXT("智慧API系統未初始化"));
    return EmptyStatus;
}

TMap<FString, float> UMingRTSQuickStart::GetPerformanceMetrics()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        return GlobalActivator->GetPerformanceMetrics();
    }
    
    TMap<FString, float> EmptyMetrics;
    EmptyMetrics.Add(TEXT("系統狀態"), -1.0f);
    return EmptyMetrics;
}

void UMingRTSQuickStart::SmartCompileAndOptimize()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->QuickCompileAndOptimize();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
                TEXT("🧠 智慧編譯優化已啟動..."));
        }
    }
}

void UMingRTSQuickStart::SmartDebugAndFix()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->QuickDebugAndFix();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, 
                TEXT("🤖 智慧除錯修復已啟動..."));
        }
    }
}

void UMingRTSQuickStart::SmartAnalyzeAndImprove()
{
    if (!IsGlobalActivatorValid())
    {
        InitializeGlobalActivator();
    }
    
    if (GlobalActivator.IsValid())
    {
        GlobalActivator->QuickAnalyzeAndImprove();
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, 
                TEXT("📈 智慧分析改進已啟動..."));
        }
    }
}

void UMingRTSQuickStart::InitializeGlobalActivator()
{
    if (!GlobalActivator.IsValid())
    {
        GlobalActivator = NewObject<UMingRTSSmartAPIActivator>();
        GlobalActivator->EnableSmartAPISystem();
        
        // 配置智慧功能
        GlobalActivator->ConfigureIntelligentMode(true);
        GlobalActivator->SetAutoLearning(true);
        GlobalActivator->EnablePredictiveFeatures(true);
        GlobalActivator->SetCoordinationMode(TEXT("Intelligent"));
        
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, 
                TEXT("✅ 智慧API系統初始化完成！"));
        }
    }
}

bool UMingRTSQuickStart::IsGlobalActivatorValid()
{
    return GlobalActivator.IsValid();
}
