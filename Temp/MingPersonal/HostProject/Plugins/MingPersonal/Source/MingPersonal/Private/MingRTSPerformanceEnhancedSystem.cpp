#include "MingRTSPerformanceEnhancedSystem.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/IConsoleManager.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Engine/GameEngine.h"
#include "Stats/Stats.h"
#include "HAL/IConsoleManager.h"

UMingRTSPerformanceEnhancedSystem::UMingRTSPerformanceEnhancedSystem()
{
    CurrentWorld = nullptr;
    PerformanceUpdateInterval = 0.1f; // 10 FPS
    LastPerformanceUpdateTime = 0.0f;
    MonitoringInterval = 1.0f; // 1 second
    LastMonitoringTime = 0.0f;
    DebugLevel = EDebugLevel::Standard;
    bPerformanceMonitoringEnabled = true;
    bAutoOptimizationEnabled = true;
    bPerformanceAlertsEnabled = true;
    MaxDebugInfoCount = 1000;
    MaxProfileCount = 50;
    
    // 初始化默認警告閾值
    AlertThresholds.Add(EPerformanceCategory::Rendering, 30.0f); // < 30 FPS
    AlertThresholds.Add(EPerformanceCategory::Physics, 16.67f); // < 60 FPS equivalent
    AlertThresholds.Add(EPerformanceCategory::AI, 100.0f); // > 100ms processing time
    AlertThresholds.Add(EPerformanceCategory::Audio, 50.0f); // > 50ms audio latency
    AlertThresholds.Add(EPerformanceCategory::Network, 150.0f); // > 150ms ping
    AlertThresholds.Add(EPerformanceCategory::UI, 16.67f); // < 60 FPS
    AlertThresholds.Add(EPerformanceCategory::Animation, 16.67f); // < 60 FPS
    AlertThresholds.Add(EPerformanceCategory::Memory, 4096.0f); // > 4GB memory usage
}

void UMingRTSPerformanceEnhancedSystem::InitializePerformanceEnhancedSystem(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSPerformanceEnhancedSystem: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    LastPerformanceUpdateTime = World->GetTimeSeconds();
    LastMonitoringTime = World->GetTimeSeconds();
    
    // 清空現有數據
    PerformanceProfiles.Empty();
    DebugInfo.Empty();
    PerformanceAlerts.Empty();
    
    // 添加初始調試信息
    AddDebugMessage(TEXT("Performance"), TEXT("Performance system initialized"), EDebugLevel::Basic);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPerformanceEnhancedSystem initialized successfully"));
}

void UMingRTSPerformanceEnhancedSystem::UpdatePerformanceEnhancedSystem(float DeltaTime)
{
    if (!CurrentWorld)
    {
        return;
    }

    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    // 定期更新性能監控
    if (bPerformanceMonitoringEnabled && CurrentTime - LastPerformanceUpdateTime >= PerformanceUpdateInterval)
    {
        ProcessPerformanceMonitoring(DeltaTime);
        LastPerformanceUpdateTime = CurrentTime;
    }
    
    // 定期監控性能
    if (CurrentTime - LastMonitoringTime >= MonitoringInterval)
    {
        ProcessProfiling(DeltaTime);
        ProcessPerformanceAlerts(DeltaTime);
        LastMonitoringTime = CurrentTime;
    }
    
    // 處理自動優化
    if (bAutoOptimizationEnabled)
    {
        ProcessAutoOptimization(DeltaTime);
    }
    
    // 清理無效數據
    CleanupInvalidPerformanceData();
}

FPerformanceMetrics UMingRTSPerformanceEnhancedSystem::GetCurrentMetrics() const
{
    return CurrentMetrics;
}

FPerformanceMetrics UMingRTSPerformanceEnhancedSystem::GetCategoryMetrics(EPerformanceCategory Category) const
{
    // 簡化實現：返回當前指標
    // 在實際實現中應該返回特定類別的詳細指標
    return CurrentMetrics;
}

FString UMingRTSPerformanceEnhancedSystem::StartProfiling(const FString& ProfileName, EPerformanceLevel TargetLevel, EOptimizationMode Mode)
{
    if (ProfileName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid profile name"));
        return TEXT("");
    }
    
    if (PerformanceProfiles.Num() >= MaxProfileCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum profile count reached"));
        return TEXT("");
    }
    
    // 創建性能分析
    FPerformanceProfile Profile = GetPerformanceProfileTemplate();
    Profile.ProfileID = GenerateUniqueID(TEXT("Profile"));
    Profile.ProfileName = ProfileName;
    Profile.TargetLevel = TargetLevel;
    Profile.OptimizationMode = Mode;
    Profile.bIsActive = true;
    Profile.StartTime = CurrentWorld->GetTimeSeconds();
    
    PerformanceProfiles.Add(Profile.ProfileID, Profile);
    CurrentProfileID = Profile.ProfileID;
    
    // 觸發分析開始事件
    OnProfileStarted.Broadcast(Profile.ProfileID);
    
    UE_LOG(LogTemp, Log, TEXT("Started profiling: %s (%s)"), *ProfileName, *Profile.ProfileID);
    
    return Profile.ProfileID;
}

bool UMingRTSPerformanceEnhancedSystem::StopProfiling(const FString& ProfileID)
{
    FPerformanceProfile* Profile = PerformanceProfiles.Find(ProfileID);
    if (!Profile)
    {
        UE_LOG(LogTemp, Warning, TEXT("Profile not found: %s"), *ProfileID);
        return false;
    }
    
    Profile->bIsActive = false;
    Profile->Duration = CurrentWorld->GetTimeSeconds() - Profile->StartTime;
    
    // 觸發分析完成事件
    OnProfileCompleted.Broadcast(ProfileID);
    
    UE_LOG(LogTemp, Log, TEXT("Stopped profiling: %s"), *ProfileID);
    
    return true;
}

bool UMingRTSPerformanceEnhancedSystem::PauseProfiling(const FString& ProfileID)
{
    FPerformanceProfile* Profile = PerformanceProfiles.Find(ProfileID);
    if (!Profile)
    {
        UE_LOG(LogTemp, Warning, TEXT("Profile not found: %s"), *ProfileID);
        return false;
    }
    
    // 簡化實現：設置為暫停狀態
    UE_LOG(LogTemp, Log, TEXT("Paused profiling: %s"), *ProfileID);
    
    return true;
}

bool UMingRTSPerformanceEnhancedSystem::ResumeProfiling(const FString& ProfileID)
{
    FPerformanceProfile* Profile = PerformanceProfiles.Find(ProfileID);
    if (!Profile)
    {
        UE_LOG(LogTemp, Warning, TEXT("Profile not found: %s"), *ProfileID);
        return false;
    }
    
    // 簡化實現：恢復分析
    UE_LOG(LogTemp, Log, TEXT("Resumed profiling: %s"), *ProfileID);
    
    return true;
}

FPerformanceProfile UMingRTSPerformanceEnhancedSystem::GetProfile(const FString& ProfileID) const
{
    const FPerformanceProfile* Profile = PerformanceProfiles.Find(ProfileID);
    if (Profile)
    {
        return *Profile;
    }
    return FPerformanceProfile();
}

TArray<FPerformanceProfile> UMingRTSPerformanceEnhancedSystem::GetAllProfiles() const
{
    TArray<FPerformanceProfile> AllProfiles;
    
    for (const TPair<FString, FPerformanceProfile>& ProfilePair : PerformanceProfiles)
    {
        AllProfiles.Add(ProfilePair.Value);
    }
    
    return AllProfiles;
}

void UMingRTSPerformanceEnhancedSystem::AutoOptimize(EPerformanceCategory Category, EPerformanceLevel TargetLevel)
{
    UE_LOG(LogTemp, Log, TEXT("Auto optimizing category: %d to level: %d"), (int32)Category, (int32)TargetLevel);
    
    // 根據類別應用相應的優化
    switch (Category)
    {
        case EPerformanceCategory::Rendering:
            OptimizeRenderingPerformance();
            break;
        case EPerformanceCategory::Physics:
            OptimizePhysicsPerformance();
            break;
        case EPerformanceCategory::AI:
            OptimizeAIPerformance();
            break;
        case EPerformanceCategory::Audio:
            OptimizeAudioPerformance();
            break;
        case EPerformanceCategory::Network:
            OptimizeNetworkPerformance();
            break;
        case EPerformanceCategory::UI:
            OptimizeUIPerformance();
            break;
        case EPerformanceCategory::Animation:
            OptimizeAnimationPerformance();
            break;
        case EPerformanceCategory::Memory:
            OptimizeMemoryPerformance();
            break;
        default:
            break;
    }
    
    // 生成優化結果
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = Category;
    Result.OptimizationType = TEXT("Auto Optimization");
    Result.bSuccessful = true;
    Result.Description = FString::Printf(TEXT("Auto optimized %s to %s"), *GetPerformanceCategoryName(Category), *GetPerformanceLevelName(TargetLevel));
    
    // 觸發優化完成事件
    OnOptimizationCompleted.Broadcast(Result);
}

void UMingRTSPerformanceEnhancedSystem::BatchOptimize(const TArray<EPerformanceCategory>& Categories, EPerformanceLevel TargetLevel)
{
    for (EPerformanceCategory Category : Categories)
    {
        AutoOptimize(Category, TargetLevel);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch optimized %d categories"), Categories.Num());
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ManualOptimize(EPerformanceCategory Category, const TMap<FString, FString>& Settings)
{
    UE_LOG(LogTemp, Log, TEXT("Manual optimizing category: %d"), (int32)Category);
    
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = Category;
    Result.OptimizationType = TEXT("Manual Optimization");
    
    // 根據類別應用手動優化
    switch (Category)
    {
        case EPerformanceCategory::Rendering:
            Result = ApplyRenderingOptimization(Settings);
            break;
        case EPerformanceCategory::Physics:
            Result = ApplyPhysicsOptimization(Settings);
            break;
        case EPerformanceCategory::AI:
            Result = ApplyAIOptimization(Settings);
            break;
        case EPerformanceCategory::Audio:
            Result = ApplyAudioOptimization(Settings);
            break;
        case EPerformanceCategory::Network:
            Result = ApplyNetworkOptimization(Settings);
            break;
        case EPerformanceCategory::UI:
            Result = ApplyUIOptimization(Settings);
            break;
        case EPerformanceCategory::Animation:
            Result = ApplyAnimationOptimization(Settings);
            break;
        case EPerformanceCategory::Memory:
            Result = ApplyMemoryOptimization(Settings);
            break;
        default:
            Result.bSuccessful = false;
            Result.Description = TEXT("Unsupported category");
            break;
    }
    
    // 觸發優化完成事件
    OnOptimizationCompleted.Broadcast(Result);
    
    return Result;
}

TArray<FOptimizationResult> UMingRTSPerformanceEnhancedSystem::GetOptimizationSuggestions() const
{
    TArray<FOptimizationResult> Suggestions;
    
    // 分析性能瓶頸並生成建議
    TArray<EPerformanceCategory> Bottlenecks = AnalyzePerformanceBottlenecks();
    
    for (EPerformanceCategory Category : Bottlenecks)
    {
        FOptimizationResult Suggestion = GenerateOptimizationSuggestion(Category);
        Suggestions.Add(Suggestion);
    }
    
    return Suggestions;
}

bool UMingRTSPerformanceEnhancedSystem::ApplyOptimizationSettings(const TMap<FString, FString>& Settings)
{
    UE_LOG(LogTemp, Log, TEXT("Applying %d optimization settings"), Settings.Num());
    
    for (const TPair<FString, FString>& Setting : Settings)
    {
        // 應用設置到控制台變量
        SetConsoleVariable(Setting.Key, Setting.Value);
    }
    
    return true;
}

void UMingRTSPerformanceEnhancedSystem::ResetOptimizationSettings()
{
    UE_LOG(LogTemp, Log, TEXT("Resetting optimization settings"));
    
    // 重置到默認設置
    TMap<FString, FString> DefaultSettings = GetDefaultPerformanceSettings();
    ApplyOptimizationSettings(DefaultSettings);
}

void UMingRTSPerformanceEnhancedSystem::SetDebugLevel(EDebugLevel Level)
{
    DebugLevel = Level;
    UE_LOG(LogTemp, Log, TEXT("Set debug level: %d"), (int32)Level);
}

EDebugLevel UMingRTSPerformanceEnhancedSystem::GetDebugLevel() const
{
    return DebugLevel;
}

void UMingRTSPerformanceEnhancedSystem::AddDebugMessage(const FString& Category, const FString& Message, EDebugLevel Level, const FString& Context)
{
    if (Level < DebugLevel)
    {
        return;
    }
    
    FDebugInfo Info;
    Info.Category = Category;
    Info.Message = Message;
    Info.Level = Level;
    Info.Timestamp = CurrentWorld->GetTimeSeconds();
    Info.Context = Context;
    
    DebugInfo.Add(Info);
    
    // 限制調試信息數量
    if (DebugInfo.Num() > MaxDebugInfoCount)
    {
        DebugInfo.RemoveAt(0);
    }
    
    // 觸發調試消息事件
    OnDebugMessage.Broadcast(Info);
    
    UE_LOG(LogTemp, Log, TEXT("[%s] %s: %s"), *Category, *GetDebugLevelName(Level), *Message);
}

TArray<FDebugInfo> UMingRTSPerformanceEnhancedSystem::GetDebugInfo(const FString& Category) const
{
    if (Category.IsEmpty())
    {
        return DebugInfo;
    }
    
    TArray<FDebugInfo> FilteredInfo;
    for (const FDebugInfo& Info : DebugInfo)
    {
        if (Info.Category == Category)
        {
            FilteredInfo.Add(Info);
        }
    }
    
    return FilteredInfo;
}

void UMingRTSPerformanceEnhancedSystem::ClearDebugInfo(const FString& Category)
{
    if (Category.IsEmpty())
    {
        DebugInfo.Empty();
    }
    else
    {
        for (int32 i = DebugInfo.Num() - 1; i >= 0; i--)
        {
            if (DebugInfo[i].Category == Category)
            {
                DebugInfo.RemoveAt(i);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Cleared debug info for category: %s"), *Category);
}

bool UMingRTSPerformanceEnhancedSystem::ExportPerformanceData(const FString& FilePath, const FString& ProfileID)
{
    FString Content = TEXT("");
    
    if (ProfileID.IsEmpty())
    {
        // 導出所有性能數據
        Content += TEXT("# Performance Data Export\n\n");
        
        for (const TPair<FString, FPerformanceProfile>& ProfilePair : PerformanceProfiles)
        {
            Content += FString::Printf(TEXT("Profile: %s\n"), *ProfilePair.Value.ProfileName);
            Content += FString::Printf(TEXT("ID: %s\n"), *ProfilePair.Key);
            Content += FString::Printf(TEXT("Target Level: %s\n"), *GetPerformanceLevelName(ProfilePair.Value.TargetLevel));
            Content += FString::Printf(TEXT("Duration: %.2f seconds\n"), ProfilePair.Value.Duration);
            Content += FString::Printf(TEXT("Metrics Count: %d\n\n"), ProfilePair.Value.Metrics.Num());
        }
    }
    else
    {
        // 導出特定分析數據
        const FPerformanceProfile* Profile = PerformanceProfiles.Find(ProfileID);
        if (Profile)
        {
            Content += FString::Printf(TEXT("# Profile: %s\n\n"), *Profile->ProfileName);
            Content += FString::Printf(TEXT("ID: %s\n"), *ProfileID);
            Content += FString::Printf(TEXT("Target Level: %s\n"), *GetPerformanceLevelName(Profile->TargetLevel));
            Content += FString::Printf(TEXT("Duration: %.2f seconds\n"), Profile->Duration);
            Content += FString::Printf(TEXT("Metrics Count: %d\n\n"), Profile->Metrics.Num());
            
            for (const FPerformanceMetrics& Metrics : Profile->Metrics)
            {
                Content += FString::Printf(TEXT("Timestamp: %.2f\n"), Metrics.Timestamp);
                Content += FString::Printf(TEXT("Frame Rate: %.2f FPS\n"), Metrics.FrameRate);
                Content += FString::Printf(TEXT("Frame Time: %.2f ms\n"), Metrics.FrameTime);
                Content += FString::Printf(TEXT("CPU Usage: %.2f%%\n"), Metrics.CPUUsage);
                Content += FString::Printf(TEXT("GPU Usage: %.2f%%\n"), Metrics.GPUUsage);
                Content += FString::Printf(TEXT("Memory Usage: %lld MB\n"), Metrics.MemoryUsage / (1024 * 1024));
                Content += FString::Printf(TEXT("Draw Calls: %.0f\n"), Metrics.DrawCalls);
                Content += FString::Printf(TEXT("Triangles: %.0f\n"), Metrics.Triangles);
                Content += TEXT("\n");
            }
        }
    }
    
    return FFileHelper::SaveStringToFile(Content, *FilePath);
}

bool UMingRTSPerformanceEnhancedSystem::ImportPerformanceData(const FString& FilePath)
{
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load performance data file: %s"), *FilePath);
        return false;
    }
    
    // 簡化的導入實現
    UE_LOG(LogTemp, Log, TEXT("Imported performance data from: %s"), *FilePath);
    
    return true;
}

TMap<FString, float> UMingRTSPerformanceEnhancedSystem::GetPerformanceStatistics() const
{
    TMap<FString, float> Statistics;
    
    // 基本統計
    Statistics.Add(TEXT("CurrentFPS"), CurrentMetrics.FrameRate);
    Statistics.Add(TEXT("CurrentFrameTime"), CurrentMetrics.FrameTime);
    Statistics.Add(TEXT("CPUUsage"), CurrentMetrics.CPUUsage);
    Statistics.Add(TEXT("GPUUsage"), CurrentMetrics.GPUUsage);
    Statistics.Add(TEXT("MemoryUsageMB"), (float)CurrentMetrics.MemoryUsage / (1024 * 1024));
    Statistics.Add(TEXT("DrawCalls"), CurrentMetrics.DrawCalls);
    Statistics.Add(TEXT("Triangles"), CurrentMetrics.Triangles);
    Statistics.Add(TEXT("NetworkLatency"), CurrentMetrics.NetworkLatency);
    Statistics.Add(TEXT("ActorCount"), (float)CurrentMetrics.ActorCount);
    Statistics.Add(TEXT("ComponentCount"), (float)CurrentMetrics.ComponentCount);
    
    // 系統統計
    Statistics.Add(TEXT("ActiveProfiles"), PerformanceProfiles.Num());
    Statistics.Add(TEXT("DebugInfoCount"), DebugInfo.Num());
    Statistics.Add(TEXT("PerformanceAlerts"), PerformanceAlerts.Num());
    
    return Statistics;
}

FString UMingRTSPerformanceEnhancedSystem::GeneratePerformanceReport(const FString& ProfileID) const
{
    FString Report = TEXT("# Performance Report\n\n");
    
    if (ProfileID.IsEmpty())
    {
        // 生成當前性能報告
        Report += TEXT("## Current Performance\n\n");
        Report += FString::Printf(TEXT("- **Frame Rate**: %.2f FPS\n"), CurrentMetrics.FrameRate);
        Report += FString::Printf(TEXT("- **Frame Time**: %.2f ms\n"), CurrentMetrics.FrameTime);
        Report += FString::Printf(TEXT("- **CPU Usage**: %.2f%%\n"), CurrentMetrics.CPUUsage);
        Report += FString::Printf(TEXT("- **GPU Usage**: %.2f%%\n"), CurrentMetrics.GPUUsage);
        Report += FString::Printf(TEXT("- **Memory Usage**: %lld MB\n"), CurrentMetrics.MemoryUsage / (1024 * 1024));
        Report += FString::Printf(TEXT("- **Draw Calls**: %.0f\n"), CurrentMetrics.DrawCalls);
        Report += FString::Printf(TEXT("- **Triangles**: %.0f\n"), CurrentMetrics.Triangles);
        Report += FString::Printf(TEXT("- **Network Latency**: %.2f ms\n"), CurrentMetrics.NetworkLatency);
        Report += FString::Printf(TEXT("- **Actor Count**: %d\n"), CurrentMetrics.ActorCount);
        Report += FString::Printf(TEXT("- **Component Count**: %d\n"), CurrentMetrics.ComponentCount);
    }
    else
    {
        // 生成特定分析報告
        const FPerformanceProfile* Profile = PerformanceProfiles.Find(ProfileID);
        if (Profile)
        {
            Report += FString::Printf(TEXT("## Profile: %s\n\n"), *Profile->ProfileName);
            Report += FString::Printf(TEXT("- **ID**: %s\n"), *ProfileID);
            Report += FString::Printf(TEXT("- **Target Level**: %s\n"), *GetPerformanceLevelName(Profile->TargetLevel));
            Report += FString::Printf(TEXT("- **Duration**: %.2f seconds\n"), Profile->Duration);
            Report += FString::Printf(TEXT("- **Metrics Count**: %d\n"), Profile->Metrics.Num());
            
            if (Profile->Metrics.Num() > 0)
            {
                // 計算平均值
                float AvgFPS = 0.0f;
                float AvgFrameTime = 0.0f;
                float AvgCPU = 0.0f;
                float AvgGPU = 0.0f;
                
                for (const FPerformanceMetrics& Metrics : Profile->Metrics)
                {
                    AvgFPS += Metrics.FrameRate;
                    AvgFrameTime += Metrics.FrameTime;
                    AvgCPU += Metrics.CPUUsage;
                    AvgGPU += Metrics.GPUUsage;
                }
                
                int32 Count = Profile->Metrics.Num();
                AvgFPS /= Count;
                AvgFrameTime /= Count;
                AvgCPU /= Count;
                AvgGPU /= Count;
                
                Report += TEXT("\n### Average Metrics\n\n");
                Report += FString::Printf(TEXT("- **Average FPS**: %.2f\n"), AvgFPS);
                Report += FString::Printf(TEXT("- **Average Frame Time**: %.2f ms\n"), AvgFrameTime);
                Report += FString::Printf(TEXT("- **Average CPU Usage**: %.2f%%\n"), AvgCPU);
                Report += FString::Printf(TEXT("- **Average GPU Usage**: %.2f%%\n"), AvgGPU);
            }
        }
    }
    
    // 添加優化建議
    TArray<FOptimizationResult> Suggestions = GetOptimizationSuggestions();
    if (Suggestions.Num() > 0)
    {
        Report += TEXT("\n## Optimization Suggestions\n\n");
        for (const FOptimizationResult& Suggestion : Suggestions)
        {
            Report += FString::Printf(TEXT("- **%s**: %s\n"), *GetPerformanceCategoryName(Suggestion.Category), *Suggestion.Description);
        }
    }
    
    return Report;
}

void UMingRTSPerformanceEnhancedSystem::SetPerformanceAlertThresholds(const TMap<EPerformanceCategory, float>& Thresholds)
{
    AlertThresholds = Thresholds;
    UE_LOG(LogTemp, Log, TEXT("Set %d performance alert thresholds"), Thresholds.Num());
}

TArray<FString> UMingRTSPerformanceEnhancedSystem::GetPerformanceAlerts() const
{
    return PerformanceAlerts;
}

void UMingRTSPerformanceEnhancedSystem::ClearPerformanceAlerts()
{
    PerformanceAlerts.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared all performance alerts"));
}

void UMingRTSPerformanceEnhancedSystem::SetPerformanceMonitoringEnabled(bool bEnabled)
{
    bPerformanceMonitoringEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Set performance monitoring enabled: %s"), bEnabled ? TEXT("true") : TEXT("false"));
}

bool UMingRTSPerformanceEnhancedSystem::IsPerformanceMonitoringEnabled() const
{
    return bPerformanceMonitoringEnabled;
}

void UMingRTSPerformanceEnhancedSystem::SetMonitoringInterval(float Interval)
{
    MonitoringInterval = FMath::Max(Interval, 0.1f);
    UE_LOG(LogTemp, Log, TEXT("Set monitoring interval: %.2f seconds"), MonitoringInterval);
}

float UMingRTSPerformanceEnhancedSystem::GetMonitoringInterval() const
{
    return MonitoringInterval;
}

void UMingRTSPerformanceEnhancedSystem::ProcessPerformanceMonitoring(float DeltaTime)
{
    // 收集當前性能指標
    CurrentMetrics = CollectPerformanceMetrics();
    
    // 記錄性能數據
    RecordPerformanceData(CurrentMetrics);
    
    // 觸發性能指標更新事件
    OnPerformanceMetricsUpdated.Broadcast(CurrentMetrics);
}

void UMingRTSPerformanceEnhancedSystem::ProcessProfiling(float DeltaTime)
{
    // 處理活躍的性能分析
    for (TPair<FString, FPerformanceProfile>& ProfilePair : PerformanceProfiles)
    {
        FPerformanceProfile& Profile = ProfilePair.Value;
        
        if (Profile.bIsActive)
        {
            // 添加當前指標到分析
            Profile.Metrics.Add(CurrentMetrics);
            
            // 限制指標數量
            if (Profile.Metrics.Num() > 1000)
            {
                Profile.Metrics.RemoveAt(0);
            }
        }
    }
}

void UMingRTSPerformanceEnhancedSystem::ProcessAutoOptimization(float DeltaTime)
{
    // 分析性能瓶頸
    TArray<EPerformanceCategory> Bottlenecks = AnalyzePerformanceBottlenecks();
    
    // 自動優化嚴重的性能問題
    for (EPerformanceCategory Category : Bottlenecks)
    {
        float* Threshold = AlertThresholds.Find(Category);
        if (Threshold)
        {
            float CurrentValue = 0.0f;
            
            switch (Category)
            {
                case EPerformanceCategory::Rendering:
                    CurrentValue = CurrentMetrics.FrameRate;
                    if (CurrentValue < *Threshold)
                    {
                        AutoOptimize(Category, EPerformanceLevel::Medium);
                    }
                    break;
                case EPerformanceCategory::Memory:
                    CurrentValue = (float)CurrentMetrics.MemoryUsage / (1024 * 1024);
                    if (CurrentValue > *Threshold)
                    {
                        AutoOptimize(Category, EPerformanceLevel::Medium);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void UMingRTSPerformanceEnhancedSystem::ProcessPerformanceAlerts(float DeltaTime)
{
    if (!bPerformanceAlertsEnabled)
    {
        return;
    }
    
    // 檢查性能警告閾值
    for (const TPair<EPerformanceCategory, float>& Threshold : AlertThresholds)
    {
        float CurrentValue = 0.0f;
        bool bAlert = false;
        FString AlertMessage;
        
        switch (Threshold.Key)
        {
            case EPerformanceCategory::Rendering:
                CurrentValue = CurrentMetrics.FrameRate;
                if (CurrentValue < Threshold.Value)
                {
                    bAlert = true;
                    AlertMessage = FString::Printf(TEXT("Low frame rate: %.2f FPS (threshold: %.2f)"), CurrentValue, Threshold.Value);
                }
                break;
            case EPerformanceCategory::Memory:
                CurrentValue = (float)CurrentMetrics.MemoryUsage / (1024 * 1024);
                if (CurrentValue > Threshold.Value)
                {
                    bAlert = true;
                    AlertMessage = FString::Printf(TEXT("High memory usage: %.2f MB (threshold: %.2f MB)"), CurrentValue, Threshold.Value);
                }
                break;
            case EPerformanceCategory::Network:
                CurrentValue = CurrentMetrics.NetworkLatency;
                if (CurrentValue > Threshold.Value)
                {
                    bAlert = true;
                    AlertMessage = FString::Printf(TEXT("High network latency: %.2f ms (threshold: %.2f ms)"), CurrentValue, Threshold.Value);
                }
                break;
            default:
                break;
        }
        
        if (bAlert)
        {
            // 添加警告
            PerformanceAlerts.Add(AlertMessage);
            
            // 限制警告數量
            if (PerformanceAlerts.Num() > 100)
            {
                PerformanceAlerts.RemoveAt(0);
            }
            
            // 觸發警告事件
            OnPerformanceAlert.Broadcast(AlertMessage);
            
            // 添加調試信息
            AddDebugMessage(TEXT("Performance Alert"), AlertMessage, EDebugLevel::Basic);
        }
    }
}

FPerformanceMetrics UMingRTSPerformanceEnhancedSystem::CollectPerformanceMetrics() const
{
    FPerformanceMetrics Metrics = GetPerformanceMetricsTemplate();
    
    if (CurrentWorld)
    {
        // 獲取引擎實例
        if (UGameEngine* Engine = GetGameEngine())
        {
            // 獲取幀率
            Metrics.FrameRate = Engine->GetAverageFPS();
            Metrics.FrameTime = Metrics.FrameRate > 0.0f ? 1000.0f / Metrics.FrameRate : 0.0f;
            
            // 獲取GPU信息
            if (Engine->GetRenderer())
            {
                // 簡化實現：使用默認值
                Metrics.GPUUsage = 50.0f;
                Metrics.DrawCalls = 1000.0f;
                Metrics.Triangles = 50000.0f;
                Metrics.TextureMemory = 256.0f;
            }
        }
        
        // 獲取內存使用情況
        FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
        Metrics.MemoryUsage = MemoryStats.UsedPhysical;
        Metrics.MemoryAvailable = MemoryStats.AvailablePhysical;
        
        // 獲取Actor和組件數量
        int32 ActorCount = 0;
        int32 ComponentCount = 0;
        
        for (TActorIterator<AActor> It(CurrentWorld); It; ++It)
        {
            ActorCount++;
            ComponentCount += It->GetComponents().Num();
        }
        
        Metrics.ActorCount = ActorCount;
        Metrics.ComponentCount = ComponentCount;
        
        // 獲取網絡信息
        Metrics.NetworkLatency = 50.0f; // 默認值
        Metrics.NetworkBandwidth = 1000.0f; // 默認值
        
        // 獲取CPU使用率
        Metrics.CPUUsage = 60.0f; // 默認值
        
        Metrics.Timestamp = CurrentWorld->GetTimeSeconds();
    }
    
    return Metrics;
}

TArray<EPerformanceCategory> UMingRTSPerformanceEnhancedSystem::AnalyzePerformanceBottlenecks() const
{
    TArray<EPerformanceCategory> Bottlenecks;
    
    // 分析各類別的性能
    if (CurrentMetrics.FrameRate < 30.0f)
    {
        Bottlenecks.Add(EPerformanceCategory::Rendering);
    }
    
    if (CurrentMetrics.MemoryUsage > 4LL * 1024 * 1024 * 1024) // 4GB
    {
        Bottlenecks.Add(EPerformanceCategory::Memory);
    }
    
    if (CurrentMetrics.NetworkLatency > 150.0f)
    {
        Bottlenecks.Add(EPerformanceCategory::Network);
    }
    
    if (CurrentMetrics.ActorCount > 1000)
    {
        Bottlenecks.Add(EPerformanceCategory::AI);
    }
    
    return Bottlenecks;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::GenerateOptimizationSuggestion(EPerformanceCategory Category) const
{
    FOptimizationResult Suggestion = GetOptimizationResultTemplate();
    Suggestion.Category = Category;
    Suggestion.OptimizationType = TEXT("Suggestion");
    
    switch (Category)
    {
        case EPerformanceCategory::Rendering:
            Suggestion.Description = TEXT("Reduce rendering quality or optimize draw calls");
            Suggestion.AppliedChanges.Add(TEXT("r.ScreenPercentage"), TEXT("80"));
            Suggestion.AppliedChanges.Add(TEXT("r.ShadowQuality"), TEXT("1"));
            break;
        case EPerformanceCategory::Memory:
            Suggestion.Description = TEXT("Optimize memory usage and garbage collection");
            Suggestion.AppliedChanges.Add(TEXT("gc.MaxObjectsInConsideration"), TEXT("1000"));
            Suggestion.AppliedChanges.Add(TEXT("gc.MaxObjectsNotConsideredByGC"), TEXT("10000"));
            break;
        case EPerformanceCategory::Network:
            Suggestion.Description = TEXT("Optimize network settings and reduce bandwidth");
            Suggestion.AppliedChanges.Add(TEXT("net.MaxNetUpdateDelta"), TEXT("0.1"));
            Suggestion.AppliedChanges.Add(TEXT("net.ClientMaxTickRate"), TEXT("60"));
            break;
        case EPerformanceCategory::AI:
            Suggestion.Description = TEXT("Optimize AI processing and reduce actor count");
            Suggestion.AppliedChanges.Add(TEXT("ai.MaxAgents"), TEXT("100"));
            Suggestion.AppliedChanges.Add(TEXT("ai.UpdateInterval"), TEXT("0.1"));
            break;
        default:
            Suggestion.Description = TEXT("General optimization recommended");
            break;
    }
    
    return Suggestion;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyRenderingOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::Rendering;
    Result.OptimizationType = TEXT("Rendering Optimization");
    
    // 應用渲染設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied rendering optimization settings");
    
    return Result;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyPhysicsOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::Physics;
    Result.OptimizationType = TEXT("Physics Optimization");
    
    // 應用物理設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied physics optimization settings");
    
    return Result;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyAIOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::AI;
    Result.OptimizationType = TEXT("AI Optimization");
    
    // 應用AI設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied AI optimization settings");
    
    return Result;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyAudioOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::Audio;
    Result.OptimizationType = TEXT("Audio Optimization");
    
    // 應用音頻設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied audio optimization settings");
    
    return Result;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyNetworkOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::Network;
    Result.OptimizationType = TEXT("Network Optimization");
    
    // 應用網絡設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied network optimization settings");
    
    return Result;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyUIOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::UI;
    Result.OptimizationType = TEXT("UI Optimization");
    
    // 應用UI設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied UI optimization settings");
    
    return Result;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyAnimationOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::Animation;
    Result.OptimizationType = TEXT("Animation Optimization");
    
    // 應用動畫設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied animation optimization settings");
    
    return Result;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::ApplyMemoryOptimization(const TMap<FString, FString>& Settings)
{
    FOptimizationResult Result = GetOptimizationResultTemplate();
    Result.Category = EPerformanceCategory::Memory;
    Result.OptimizationType = TEXT("Memory Optimization");
    
    // 應用內存設置
    for (const TPair<FString, FString>& Setting : Settings)
    {
        SetConsoleVariable(Setting.Key, Setting.Value);
        Result.AppliedChanges.Add(Setting.Key, Setting.Value);
    }
    
    Result.bSuccessful = true;
    Result.Description = TEXT("Applied memory optimization settings");
    
    return Result;
}

bool UMingRTSPerformanceEnhancedSystem::ValidateOptimizationResult(const FOptimizationResult& Result) const
{
    return Result.bSuccessful && !Result.Description.IsEmpty();
}

void UMingRTSPerformanceEnhancedSystem::RecordPerformanceData(const FPerformanceMetrics& Metrics)
{
    // 記錄性能數據到當前分析
    if (!CurrentProfileID.IsEmpty())
    {
        FPerformanceProfile* Profile = PerformanceProfiles.Find(CurrentProfileID);
        if (Profile && Profile->bIsActive)
        {
            Profile->Metrics.Add(Metrics);
            
            // 限制指標數量
            if (Profile->Metrics.Num() > 1000)
            {
                Profile->Metrics.RemoveAt(0);
            }
        }
    }
}

TMap<EPerformanceCategory, float> UMingRTSPerformanceEnhancedSystem::AnalyzePerformanceTrends() const
{
    TMap<EPerformanceCategory, float> Trends;
    
    // 分析性能趨勢
    // 簡化實現：返回當前值
    Trends.Add(EPerformanceCategory::Rendering, CurrentMetrics.FrameRate);
    Trends.Add(EPerformanceCategory::Memory, (float)CurrentMetrics.MemoryUsage / (1024 * 1024));
    Trends.Add(EPerformanceCategory::Network, CurrentMetrics.NetworkLatency);
    
    return Trends;
}

float UMingRTSPerformanceEnhancedSystem::PredictPerformanceLoad() const
{
    float Load = 0.0f;
    
    // 基於當前指標預測負載
    Load += (60.0f - CurrentMetrics.FrameRate) / 60.0f * 0.3f; // 幀率負載
    Load += ((float)CurrentMetrics.MemoryUsage / (1024 * 1024 * 1024)) / 8.0f * 0.3f; // 內存負載
    Load += CurrentMetrics.NetworkLatency / 200.0f * 0.2f; // 網絡負載
    Load += (float)CurrentMetrics.ActorCount / 2000.0f * 0.2f; // Actor負載
    
    return FMath::Clamp(Load, 0.0f, 1.0f);
}

void UMingRTSPerformanceEnhancedSystem::BalancePerformanceLoad()
{
    float CurrentLoad = PredictPerformanceLoad();
    
    if (CurrentLoad > 0.8f)
    {
        // 高負載：降低性能設置
        AutoOptimize(EPerformanceCategory::Rendering, EPerformanceLevel::Low);
        AutoOptimize(EPerformanceCategory::Memory, EPerformanceLevel::Low);
        
        UE_LOG(LogTemp, Log, TEXT("Balanced performance load: reduced settings due to high load"));
    }
    else if (CurrentLoad < 0.3f)
    {
        // 低負載：提高性能設置
        AutoOptimize(EPerformanceCategory::Rendering, EPerformanceLevel::High);
        
        UE_LOG(LogTemp, Log, TEXT("Balanced performance load: increased settings due to low load"));
    }
}

FString UMingRTSPerformanceEnhancedSystem::GetPerformanceCategoryName(EPerformanceCategory Category) const
{
    switch (Category)
    {
        case EPerformanceCategory::Rendering: return TEXT("Rendering");
        case EPerformanceCategory::Physics: return TEXT("Physics");
        case EPerformanceCategory::AI: return TEXT("AI");
        case EPerformanceCategory::Audio: return TEXT("Audio");
        case EPerformanceCategory::Network: return TEXT("Network");
        case EPerformanceCategory::UI: return TEXT("UI");
        case EPerformanceCategory::Animation: return TEXT("Animation");
        case EPerformanceCategory::Input: return TEXT("Input");
        case EPerformanceCategory::Memory: return TEXT("Memory");
        case EPerformanceCategory::Storage: return TEXT("Storage");
        default: return TEXT("Custom");
    }
}

FString UMingRTSPerformanceEnhancedSystem::GetPerformanceLevelName(EPerformanceLevel Level) const
{
    switch (Level)
    {
        case EPerformanceLevel::Low: return TEXT("Low");
        case EPerformanceLevel::Medium: return TEXT("Medium");
        case EPerformanceLevel::High: return TEXT("High");
        case EPerformanceLevel::Ultra: return TEXT("Ultra");
        case EPerformanceLevel::Epic: return TEXT("Epic");
        default: return TEXT("Custom");
    }
}

FString UMingRTSPerformanceEnhancedSystem::GetOptimizationModeName(EOptimizationMode Mode) const
{
    switch (Mode)
    {
        case EOptimizationMode::Manual: return TEXT("Manual");
        case EOptimizationMode::Automatic: return TEXT("Automatic");
        case EOptimizationMode::Adaptive: return TEXT("Adaptive");
        case EOptimizationMode::Aggressive: return TEXT("Aggressive");
        case EOptimizationMode::Conservative: return TEXT("Conservative");
        default: return TEXT("Custom");
    }
}

FString UMingRTSPerformanceEnhancedSystem::GetDebugLevelName(EDebugLevel Level) const
{
    switch (Level)
    {
        case EDebugLevel::None: return TEXT("None");
        case EDebugLevel::Basic: return TEXT("Basic");
        case EDebugLevel::Standard: return TEXT("Standard");
        case EDebugLevel::Verbose: return TEXT("Verbose");
        case EDebugLevel::Extreme: return TEXT("Extreme");
        default: return TEXT("Custom");
    }
}

FString UMingRTSPerformanceEnhancedSystem::GenerateUniqueID(const FString& Prefix) const
{
    return FString::Printf(TEXT("%s_%d"), *Prefix, FDateTime::Now().GetTicks());
}

bool UMingRTSPerformanceEnhancedSystem::ValidatePerformanceMetrics(const FPerformanceMetrics& Metrics) const
{
    return Metrics.FrameRate >= 0.0f && Metrics.FrameTime >= 0.0f;
}

bool UMingRTSPerformanceEnhancedSystem::ValidatePerformanceProfile(const FPerformanceProfile& Profile) const
{
    return !Profile.ProfileID.IsEmpty() && !Profile.ProfileName.IsEmpty();
}

void UMingRTSPerformanceEnhancedSystem::HandlePerformanceError(const FString& Operation, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Performance error in %s: %s"), *Operation, *ErrorMessage);
    
    // 添加調試信息
    AddDebugMessage(TEXT("Performance Error"), FString::Printf(TEXT("%s: %s"), *Operation, *ErrorMessage), EDebugLevel::Basic);
}

void UMingRTSPerformanceEnhancedSystem::CleanupInvalidPerformanceData()
{
    // 清理無效的性能分析
    for (auto It = PerformanceProfiles.CreateIterator(); It; ++It)
    {
        if (!ValidatePerformanceProfile(It->Value))
        {
            It.RemoveCurrent();
            UE_LOG(LogTemp, Log, TEXT("Cleaned up invalid performance profile"));
        }
    }
    
    // 清理過期的調試信息
    float CurrentTime = CurrentWorld ? CurrentWorld->GetTimeSeconds() : 0.0f;
    for (int32 i = DebugInfo.Num() - 1; i >= 0; i--)
    {
        if (CurrentTime - DebugInfo[i].Timestamp > 3600.0f) // 1 hour
        {
            DebugInfo.RemoveAt(i);
        }
    }
}

FPerformanceMetrics UMingRTSPerformanceEnhancedSystem::GetPerformanceMetricsTemplate() const
{
    FPerformanceMetrics Template;
    Template.Category = EPerformanceCategory::Rendering;
    Template.FrameRate = 60.0f;
    Template.FrameTime = 16.67f;
    Template.CPUUsage = 50.0f;
    Template.GPUUsage = 50.0f;
    Template.MemoryUsage = 1024 * 1024 * 1024; // 1GB
    Template.MemoryAvailable = 8LL * 1024 * 1024 * 1024; // 8GB
    Template.DrawCalls = 1000.0f;
    Template.Triangles = 50000.0f;
    Template.TextureMemory = 256.0f;
    Template.NetworkLatency = 50.0f;
    Template.NetworkBandwidth = 1000.0f;
    Template.ActorCount = 100;
    Template.ComponentCount = 500;
    Template.Timestamp = 0.0f;
    
    return Template;
}

FPerformanceProfile UMingRTSPerformanceEnhancedSystem::GetPerformanceProfileTemplate() const
{
    FPerformanceProfile Template;
    Template.TargetLevel = EPerformanceLevel::Medium;
    Template.OptimizationMode = EOptimizationMode::Automatic;
    Template.bIsActive = false;
    Template.StartTime = 0.0f;
    Template.Duration = 0.0f;
    
    return Template;
}

FOptimizationResult UMingRTSPerformanceEnhancedSystem::GetOptimizationResultTemplate() const
{
    FOptimizationResult Template;
    Template.Category = EPerformanceCategory::Rendering;
    Template.OptimizationType = TEXT("");
    Template.BeforeValue = 0.0f;
    Template.AfterValue = 0.0f;
    Template.Improvement = 0.0f;
    Template.bSuccessful = false;
    Template.Description = TEXT("");
    
    return Template;
}

void UMingRTSPerformanceEnhancedSystem::RecordPerformanceStatistics()
{
    TMap<FString, float> Stats = GetPerformanceStatistics();
    
    for (const TPair<FString, float>& Stat : Stats)
    {
        UE_LOG(LogTemp, VeryVerbose, TEXT("Performance Stat: %s - %.2f"), *Stat.Key, Stat.Value);
    }
}

void UMingRTSPerformanceEnhancedSystem::HandlePerformanceBottlenecks()
{
    TArray<EPerformanceCategory> Bottlenecks = AnalyzePerformanceBottlenecks();
    
    for (EPerformanceCategory Category : Bottlenecks)
    {
        AddDebugMessage(TEXT("Bottleneck"), FString::Printf(TEXT("Performance bottleneck detected in %s"), *GetPerformanceCategoryName(Category)), EDebugLevel::Basic);
    }
}

void UMingRTSPerformanceEnhancedSystem::OptimizeRenderingPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing rendering performance"));
    
    // 應用渲染優化設置
    SetConsoleVariable(TEXT("r.ScreenPercentage"), TEXT("85"));
    SetConsoleVariable(TEXT("r.ShadowQuality"), TEXT("1"));
    SetConsoleVariable(TEXT("r.PostProcessAAQuality"), TEXT("1"));
}

void UMingRTSPerformanceEnhancedSystem::OptimizePhysicsPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing physics performance"));
    
    // 應用物理優化設置
    SetConsoleVariable(TEXT("p.PhysicsMaxDeltaTime"), TEXT("0.033"));
    SetConsoleVariable(TEXT("p.MaxPhysicsDeltaTime"), TEXT("0.033"));
}

void UMingRTSPerformanceEnhancedSystem::OptimizeAIPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing AI performance"));
    
    // 應用AI優化設置
    SetConsoleVariable(TEXT("ai.MaxAgents"), TEXT("100"));
    SetConsoleVariable(TEXT("ai.UpdateInterval"), TEXT("0.1"));
}

void UMingRTSPerformanceEnhancedSystem::OptimizeAudioPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing audio performance"));
    
    // 應用音頻優化設置
    SetConsoleVariable(TEXT("sg.AudioQuality"), TEXT("1"));
    SetConsoleVariable(TEXT("sg.MaxChannels"), TEXT("64"));
}

void UMingRTSPerformanceEnhancedSystem::OptimizeNetworkPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing network performance"));
    
    // 應用網絡優化設置
    SetConsoleVariable(TEXT("net.MaxNetUpdateDelta"), TEXT("0.1"));
    SetConsoleVariable(TEXT("net.ClientMaxTickRate"), TEXT("60"));
}

void UMingRTSPerformanceEnhancedSystem::OptimizeUIPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing UI performance"));
    
    // 應用UI優化設置
    SetConsoleVariable(TEXT("ui.MaxSlateTickDelta"), TEXT("0.033"));
}

void UMingRTSPerformanceEnhancedSystem::OptimizeAnimationPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing animation performance"));
    
    // 應用動畫優化設置
    SetConsoleVariable(TEXT("a.MaxAnimationUpdateDelta"), TEXT("0.033"));
}

void UMingRTSPerformanceEnhancedSystem::OptimizeMemoryPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing memory performance"));
    
    // 應用內存優化設置
    SetConsoleVariable(TEXT("gc.MaxObjectsInConsideration"), TEXT("1000"));
    SetConsoleVariable(TEXT("gc.MaxObjectsNotConsideredByGC"), TEXT("10000"));
}

TMap<FString, FString> UMingRTSPerformanceEnhancedSystem::GetDefaultPerformanceSettings() const
{
    TMap<FString, FString> DefaultSettings;
    
    // 渲染設置
    DefaultSettings.Add(TEXT("r.ScreenPercentage"), TEXT("100"));
    DefaultSettings.Add(TEXT("r.ShadowQuality"), TEXT("3"));
    DefaultSettings.Add(TEXT("r.PostProcessAAQuality"), TEXT("4"));
    
    // 物理設置
    DefaultSettings.Add(TEXT("p.PhysicsMaxDeltaTime"), TEXT("0.033"));
    DefaultSettings.Add(TEXT("p.MaxPhysicsDeltaTime"), TEXT("0.033"));
    
    // AI設置
    DefaultSettings.Add(TEXT("ai.MaxAgents"), TEXT("200"));
    DefaultSettings.Add(TEXT("ai.UpdateInterval"), TEXT("0.05"));
    
    // 音頻設置
    DefaultSettings.Add(TEXT("sg.AudioQuality"), TEXT("2"));
    DefaultSettings.Add(TEXT("sg.MaxChannels"), TEXT("128"));
    
    // 網絡設置
    DefaultSettings.Add(TEXT("net.MaxNetUpdateDelta"), TEXT("0.016"));
    DefaultSettings.Add(TEXT("net.ClientMaxTickRate"), TEXT("120"));
    
    // UI設置
    DefaultSettings.Add(TEXT("ui.MaxSlateTickDelta"), TEXT("0.016"));
    
    // 動畫設置
    DefaultSettings.Add(TEXT("a.MaxAnimationUpdateDelta"), TEXT("0.016"));
    
    // 內存設置
    DefaultSettings.Add(TEXT("gc.MaxObjectsInConsideration"), TEXT("2000"));
    DefaultSettings.Add(TEXT("gc.MaxObjectsNotConsideredByGC"), TEXT("20000"));
    
    return DefaultSettings;
}

void UMingRTSPerformanceEnhancedSystem::ApplyConsoleCommand(const FString& Command)
{
    if (GEngine)
    {
        GEngine->Exec(CurrentWorld, *Command, *GLog);
    }
}

FString UMingRTSPerformanceEnhancedSystem::GetConsoleVariable(const FString& VariableName) const
{
    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*VariableName))
    {
        return CVar->GetString();
    }
    return TEXT("");
}

void UMingRTSPerformanceEnhancedSystem::SetConsoleVariable(const FString& VariableName, const FString& Value)
{
    if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(*VariableName))
    {
        CVar->Set(*Value);
        UE_LOG(LogTemp, Log, TEXT("Set console variable: %s = %s"), *VariableName, *Value);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Console variable not found: %s"), *VariableName);
    }
}
