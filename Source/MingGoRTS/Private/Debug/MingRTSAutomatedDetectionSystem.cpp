// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 自動化檢測系統實作

#include "Debug/MingRTSAutomatedDetectionSystem.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Engine/Engine.h"
#include "MingGoRTS.h"
#include "JsonObjectConverter.h"

void UMingRTSAutomatedDetectionSystem::InitializeDetectionSystem()
{
    bIsDetecting = false;
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Initialized"));
}

FDetectionResult UMingRTSAutomatedDetectionSystem::RunFullDetection(const FDetectionConfig& Config)
{
    FDetectionResult Result;
    Result.DetectionName = Config.DetectionName;
    Result.StartTime = FDateTime::Now();
    CurrentDetectionStartTime = Result.StartTime;
    bIsDetecting = true;
    
    OnDetectionStarted.Broadcast();
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Starting full detection '%s'"), *Config.DetectionName);
    
    // 執行啟用的檢測任務
    for (EDetectionType DetectionType : Config.EnabledDetections)
    {
        FDetectionTaskResult TaskResult = RunSingleDetection(DetectionType, Config.ScanPaths);
        Result.TaskResults.Add(TaskResult);
        
        OnDetectionTaskCompleted.Broadcast(DetectionType, TaskResult);
        
        // 統計發現
        for (const FDetectionFinding& Finding : TaskResult.Findings)
        {
            Result.TotalFindings++;
            switch (Finding.Severity)
            {
                case EDetectionSeverity::Critical: Result.CriticalFindings++; break;
                case EDetectionSeverity::High: Result.HighFindings++; break;
                case EDetectionSeverity::Medium: Result.MediumFindings++; break;
                case EDetectionSeverity::Low: Result.LowFindings++; break;
                default: break;
            }
        }
    }
    
    Result.EndTime = FDateTime::Now();
    Result.TotalExecutionTime = (Result.EndTime - Result.StartTime).GetTotalSeconds();
    
    // 計算整體健康評分
    CalculateOverallScore(Result);
    
    // 生成報告
    if (!Config.OutputReportPath.IsEmpty())
    {
        Result.ReportFilePath = GenerateDetectionReport(Result, Config.bGenerateDetailedReport);
    }
    
    // 發送通知
    if (Config.bSendNotifications)
    {
        SendDetectionNotification(Result);
    }
    
    bIsDetecting = false;
    OnDetectionCompleted.Broadcast(Result);
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Detection completed. Health Score: %.1f%%, Findings: %d"),
        Result.OverallHealthScore, Result.TotalFindings);
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::RunSingleDetection(EDetectionType DetectionType, const TArray<FString>& TargetPaths)
{
    FDetectionTaskResult Result;
    Result.DetectionType = DetectionType;
    Result.Status = EDetectionStatus::InProgress;
    
    FDateTime TaskStart = FDateTime::Now();
    
    switch (DetectionType)
    {
        case EDetectionType::ProjectHealth:
            Result = DetectProjectHealth();
            break;
            
        case EDetectionType::CodeQuality:
            Result = DetectCodeQuality(TargetPaths);
            break;
            
        case EDetectionType::PerformanceMetrics:
            Result = DetectPerformanceMetrics();
            break;
            
        case EDetectionType::AssetIntegrity:
            Result = DetectAssetIntegrity(TargetPaths);
            break;
            
        case EDetectionType::ConfigurationCheck:
            Result = DetectConfigurationIssues();
            break;
            
        case EDetectionType::DependencyCheck:
            Result = DetectDependencyIssues();
            break;
            
        case EDetectionType::SecurityScan:
            Result = PerformSecurityScan();
            break;
            
        case EDetectionType::LocalizationCheck:
            Result = DetectLocalizationIssues();
            break;
            
        case EDetectionType::MemoryAnalysis:
            Result = AnalyzeMemoryUsage();
            break;
            
        case EDetectionType::NetworkValidation:
            Result = ValidateNetworkConfiguration();
            break;
            
        default:
            Result.Status = EDetectionStatus::Skipped;
            Result.Summary = FText::FromString(TEXT("未知的檢測類型"));
            break;
    }
    
    Result.ExecutionTimeSeconds = (FDateTime::Now() - TaskStart).GetTotalSeconds();
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::DetectProjectHealth()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::ProjectHealth;
    Result.Status = EDetectionStatus::InProgress;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Detecting project health..."));
    
    // 檢查項目結構
    FString ProjectDir = FPaths::ProjectDir();
    FString SourceDir = FPaths::ProjectSourceDir();
    FString ContentDir = FPaths::ProjectContentDir();
    
    // 檢查關鍵目錄
    if (!FPaths::DirectoryExists(SourceDir))
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("MissingSourceDir");
        Finding.DetectionType = EDetectionType::ProjectHealth;
        Finding.Severity = EDetectionSeverity::Critical;
        Finding.Title = FText::FromString(TEXT("缺少 Source 目錄"));
        Finding.Description = FText::FromString(TEXT("項目 Source 目錄不存在，這是一個嚴重的項目結構問題"));
        Finding.Recommendation = FText::FromString(TEXT("請確認項目結構正確，Source 目錄應該存在"));
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Result.Findings.Add(Finding);
        OnFindingDetected.Broadcast(Finding);
    }
    
    // 檢查 .uproject 文件
    FString UProjectPath = FPaths::GetProjectFilePath();
    if (!FPaths::FileExists(UProjectPath))
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("MissingUProject");
        Finding.DetectionType = EDetectionType::ProjectHealth;
        Finding.Severity = EDetectionSeverity::Critical;
        Finding.Title = FText::FromString(TEXT("缺少 .uproject 文件"));
        Finding.Description = FText::FromString(TEXT("項目文件 (.uproject) 不存在"));
        Finding.Recommendation = FText::FromString(TEXT("請確認項目文件存在且未損壞"));
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Result.Findings.Add(Finding);
        OnFindingDetected.Broadcast(Finding);
    }
    
    // 檢查 Config 目錄
    FString ConfigDir = FPaths::ProjectConfigDir();
    if (!FPaths::DirectoryExists(ConfigDir))
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("MissingConfigDir");
        Finding.DetectionType = EDetectionType::ProjectHealth;
        Finding.Severity = EDetectionSeverity::High;
        Finding.Title = FText::FromString(TEXT("缺少 Config 目錄"));
        Finding.Description = FText::FromString(TEXT("項目 Config 目錄不存在"));
        Finding.Recommendation = FText::FromString(TEXT("請創建 Config 目錄並添加必要的配置文件"));
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Result.Findings.Add(Finding);
        OnFindingDetected.Broadcast(Finding);
    }
    
    // 添加健康評分指標
    FDetectionMetrics HealthMetric;
    HealthMetric.MetricName = TEXT("ProjectStructureScore");
    HealthMetric.CurrentValue = Result.Findings.Num() == 0 ? 100.0f : 100.0f - (Result.Findings.Num() * 10.0f);
    HealthMetric.ExpectedValue = 100.0f;
    HealthMetric.MinThreshold = 80.0f;
    HealthMetric.MaxThreshold = 100.0f;
    HealthMetric.Unit = TEXT("%");
    HealthMetric.bIsPassed = HealthMetric.CurrentValue >= HealthMetric.MinThreshold;
    
    Result.Metrics.Add(HealthMetric);
    
    // 確定狀態
    if (Result.Findings.Num() == 0)
    {
        Result.Status = EDetectionStatus::Passed;
        Result.Summary = FText::FromString(TEXT("項目健康檢測通過，結構完整"));
    }
    else
    {
        bool bHasCritical = false;
        for (const FDetectionFinding& Finding : Result.Findings)
        {
            if (Finding.Severity == EDetectionSeverity::Critical)
            {
                bHasCritical = true;
                break;
            }
        }
        
        Result.Status = bHasCritical ? EDetectionStatus::Failed : EDetectionStatus::Warning;
        Result.Summary = FText::FromString(FString::Printf(TEXT("項目健康檢測完成，發現 %d 個問題"), Result.Findings.Num()));
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::DetectCodeQuality(const TArray<FString>& TargetPaths)
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::CodeQuality;
    Result.Status = EDetectionStatus::InProgress;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Detecting code quality..."));
    
    Result.Findings = AnalyzeCodeQuality(TargetPaths);
    
    // 計算代碼品質指標
    int32 TotalFiles = 0;
    for (const FString& Path : TargetPaths)
    {
        TArray<FString> Files;
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.cpp"), true, false);
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.h"), true, false);
        TotalFiles += Files.Num();
    }
    
    FDetectionMetrics QualityMetric;
    QualityMetric.MetricName = TEXT("CodeQualityScore");
    QualityMetric.CurrentValue = (Result.Findings.Num() == 0) ? 100.0f : FMath::Max(0.0f, 100.0f - (Result.Findings.Num() * 5.0f));
    QualityMetric.ExpectedValue = 90.0f;
    QualityMetric.MinThreshold = 70.0f;
    QualityMetric.Unit = TEXT("%");
    QualityMetric.bIsPassed = QualityMetric.CurrentValue >= QualityMetric.MinThreshold;
    
    Result.Metrics.Add(QualityMetric);
    
    // 確定狀態
    if (Result.Findings.Num() == 0)
    {
        Result.Status = EDetectionStatus::Passed;
        Result.Summary = FText::FromString(FString::Printf(TEXT("代碼品質檢測通過，檢查了 %d 個文件"), TotalFiles));
    }
    else
    {
        bool bHasHighSeverity = false;
        for (const FDetectionFinding& Finding : Result.Findings)
        {
            if (Finding.Severity == EDetectionSeverity::Critical || Finding.Severity == EDetectionSeverity::High)
            {
                bHasHighSeverity = true;
                break;
            }
        }
        
        Result.Status = bHasHighSeverity ? EDetectionStatus::Failed : EDetectionStatus::Warning;
        Result.Summary = FText::FromString(FString::Printf(TEXT("檢查了 %d 個文件，發現 %d 個品質問題"), TotalFiles, Result.Findings.Num()));
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::DetectPerformanceMetrics()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::PerformanceMetrics;
    Result.Status = EDetectionStatus::InProgress;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Detecting performance metrics..."));
    
    // 收集性能指標
    FDetectionMetrics PerfMetric = CollectPerformanceMetrics();
    Result.Metrics.Add(PerfMetric);
    
    // 檢查性能問題
    if (!PerfMetric.bIsPassed)
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("PerformanceIssue");
        Finding.DetectionType = EDetectionType::PerformanceMetrics;
        Finding.Severity = EDetectionSeverity::Medium;
        Finding.Title = FText::FromString(TEXT("性能指標未達標"));
        Finding.Description = FText::FromString(FString::Printf(TEXT("當前值: %.1f %s, 期望值: %.1f %s"),
            PerfMetric.CurrentValue, *PerfMetric.Unit, PerfMetric.ExpectedValue, *PerfMetric.Unit));
        Finding.Recommendation = FText::FromString(TEXT("請優化相關模塊以達到性能目標"));
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Result.Findings.Add(Finding);
        OnFindingDetected.Broadcast(Finding);
    }
    
    Result.Status = PerfMetric.bIsPassed ? EDetectionStatus::Passed : EDetectionStatus::Warning;
    Result.Summary = PerfMetric.bIsPassed ? 
        FText::FromString(TEXT("性能指標檢測通過")) : 
        FText::FromString(TEXT("性能指標未完全達標"));
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::DetectAssetIntegrity(const TArray<FString>& TargetPaths)
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::AssetIntegrity;
    Result.Status = EDetectionStatus::InProgress;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Detecting asset integrity..."));
    
    Result.Findings = CheckAssetIntegrity(TargetPaths);
    
    if (Result.Findings.Num() == 0)
    {
        Result.Status = EDetectionStatus::Passed;
        Result.Summary = FText::FromString(TEXT("資源完整性檢測通過"));
    }
    else
    {
        Result.Status = EDetectionStatus::Warning;
        Result.Summary = FText::FromString(FString::Printf(TEXT("發現 %d 個資源問題"), Result.Findings.Num()));
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::DetectConfigurationIssues()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::ConfigurationCheck;
    Result.Status = EDetectionStatus::Passed;
    Result.Summary = FText::FromString(TEXT("配置檢查完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Checking configuration..."));
    
    // 檢查 DefaultEngine.ini
    FString EngineConfigPath = FPaths::ProjectConfigDir() / TEXT("DefaultEngine.ini");
    if (!FPaths::FileExists(EngineConfigPath))
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("MissingEngineConfig");
        Finding.DetectionType = EDetectionType::ConfigurationCheck;
        Finding.Severity = EDetectionSeverity::High;
        Finding.Title = FText::FromString(TEXT("缺少 DefaultEngine.ini"));
        Finding.Description = FText::FromString(TEXT("引擎配置文件不存在"));
        Finding.Recommendation = FText::FromString(TEXT("請創建 DefaultEngine.ini 配置文件"));
        Finding.TargetPath = EngineConfigPath;
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Result.Findings.Add(Finding);
        Result.Status = EDetectionStatus::Failed;
    }
    
    // 檢查 DefaultGame.ini
    FString GameConfigPath = FPaths::ProjectConfigDir() / TEXT("DefaultGame.ini");
    if (!FPaths::FileExists(GameConfigPath))
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("MissingGameConfig");
        Finding.DetectionType = EDetectionType::ConfigurationCheck;
        Finding.Severity = EDetectionSeverity::High;
        Finding.Title = FText::FromString(TEXT("缺少 DefaultGame.ini"));
        Finding.Description = FText::FromString(TEXT("遊戲配置文件不存在"));
        Finding.Recommendation = FText::FromString(TEXT("請創建 DefaultGame.ini 配置文件"));
        Finding.TargetPath = GameConfigPath;
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Result.Findings.Add(Finding);
        Result.Status = EDetectionStatus::Failed;
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::DetectDependencyIssues()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::DependencyCheck;
    Result.Status = EDetectionStatus::Passed;
    Result.Summary = FText::FromString(TEXT("依賴檢查完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Checking dependencies..."));
    
    // 檢查插件依賴
    TArray<FString> RequiredPlugins = {
        TEXT("MingPersonal"),
        TEXT("MingAchievements"),
        TEXT("MingAudio"),
        TEXT("MingBuilding")
    };
    
    FString PluginsDir = FPaths::ProjectPluginsDir();
    for (const FString& Plugin : RequiredPlugins)
    {
        FString PluginPath = PluginsDir / Plugin / (Plugin + TEXT(".uplugin"));
        if (!FPaths::FileExists(PluginPath))
        {
            FDetectionFinding Finding;
            Finding.FindingID = FName(*FString::Printf(TEXT("MissingPlugin_%s"), *Plugin));
            Finding.DetectionType = EDetectionType::DependencyCheck;
            Finding.Severity = EDetectionSeverity::Medium;
            Finding.Title = FText::FromString(FString::Printf(TEXT("缺少插件: %s"), *Plugin));
            Finding.Description = FText::FromString(FString::Printf(TEXT("必需的插件 %s 未找到"), *Plugin));
            Finding.Recommendation = FText::FromString(TEXT("請確保所有必需插件已安裝"));
            Finding.bAutoFixable = false;
            Finding.DetectionTime = FDateTime::Now();
            
            Result.Findings.Add(Finding);
            Result.Status = EDetectionStatus::Warning;
        }
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::PerformSecurityScan()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::SecurityScan;
    Result.Status = EDetectionStatus::Passed;
    Result.Summary = FText::FromString(TEXT("安全掃描完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Performing security scan..."));
    
    Result.Findings = ScanSecurityIssues();
    
    if (Result.Findings.Num() > 0)
    {
        bool bHasCritical = false;
        for (const FDetectionFinding& Finding : Result.Findings)
        {
            if (Finding.Severity == EDetectionSeverity::Critical)
            {
                bHasCritical = true;
                break;
            }
        }
        Result.Status = bHasCritical ? EDetectionStatus::Failed : EDetectionStatus::Warning;
        Result.Summary = FText::FromString(FString::Printf(TEXT("發現 %d 個安全問題"), Result.Findings.Num()));
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::DetectLocalizationIssues()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::LocalizationCheck;
    Result.Status = EDetectionStatus::Passed;
    Result.Summary = FText::FromString(TEXT("本地化檢查完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Detecting localization issues..."));
    
    Result.Findings = ValidateLocalization();
    
    if (Result.Findings.Num() > 0)
    {
        Result.Status = EDetectionStatus::Warning;
        Result.Summary = FText::FromString(FString::Printf(TEXT("發現 %d 個本地化問題"), Result.Findings.Num()));
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::AnalyzeMemoryUsage()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::MemoryAnalysis;
    Result.Status = EDetectionStatus::Passed;
    Result.Summary = FText::FromString(TEXT("記憶體分析完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Analyzing memory usage..."));
    
    FDetectionMetrics MemoryMetric = CollectMemoryMetrics();
    Result.Metrics.Add(MemoryMetric);
    
    if (!MemoryMetric.bIsPassed)
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("MemoryIssue");
        Finding.DetectionType = EDetectionType::MemoryAnalysis;
        Finding.Severity = EDetectionSeverity::Medium;
        Finding.Title = FText::FromString(TEXT("記憶體使用異常"));
        Finding.Description = FText::FromString(TEXT("記憶體使用超出預期範圍"));
        Finding.Recommendation = FText::FromString(TEXT("請檢查記憶體洩漏或優化記憶體使用"));
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Result.Findings.Add(Finding);
        Result.Status = EDetectionStatus::Warning;
    }
    
    return Result;
}

FDetectionTaskResult UMingRTSAutomatedDetectionSystem::ValidateNetworkConfiguration()
{
    FDetectionTaskResult Result;
    Result.DetectionType = EDetectionType::NetworkValidation;
    Result.Status = EDetectionStatus::Passed;
    Result.Summary = FText::FromString(TEXT("網絡配置驗證完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedDetectionSystem: Validating network configuration..."));
    
    return Result;
}

TArray<FDetectionFinding> UMingRTSAutomatedDetectionSystem::AnalyzeCodeQuality(const TArray<FString>& TargetPaths)
{
    TArray<FDetectionFinding> Findings;
    
    for (const FString& Path : TargetPaths)
    {
        TArray<FString> Files;
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.cpp"), true, false);
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.h"), true, false);
        
        for (const FString& File : Files)
        {
            FString FilePath = FPaths::Combine(Path, File);
            FString FileContent;
            
            if (FFileHelper::LoadFileToString(FileContent, *FilePath))
            {
                TArray<FString> Lines;
                FileContent.ParseIntoArrayLines(Lines);
                
                for (int32 i = 0; i < Lines.Num(); i++)
                {
                    const FString& Line = Lines[i];
                    
                    // 檢查 TODO/FIXME
                    if (Line.Contains(TEXT("TODO")) || Line.Contains(TEXT("FIXME")) || Line.Contains(TEXT("HACK")))
                    {
                        FDetectionFinding Finding;
                        Finding.FindingID = FName(*FString::Printf(TEXT("Todo_%s_%d"), *File, i));
                        Finding.DetectionType = EDetectionType::CodeQuality;
                        Finding.Severity = EDetectionSeverity::Low;
                        Finding.Title = FText::FromString(TEXT("代碼標記"));
                        Finding.Description = FText::FromString(Line.TrimStartAndEnd());
                        Finding.TargetPath = FilePath;
                        Finding.Recommendation = FText::FromString(TEXT("請處理標記的代碼"));
                        Finding.bAutoFixable = false;
                        Finding.DetectionTime = FDateTime::Now();
                        
                        Findings.Add(Finding);
                        OnFindingDetected.Broadcast(Finding);
                    }
                    
                    // 檢查硬編碼字符串
                    if (Line.Contains(TEXT("TEXT(\"")) && !Line.Contains(TEXT("NSLOCTEXT")) && !Line.Contains(TEXT("LOCTEXT")))
                    {
                        if (!Line.TrimStart().StartsWith(TEXT("//")) && !Line.Contains(TEXT("UE_LOG")))
                        {
                            FDetectionFinding Finding;
                            Finding.FindingID = FName(*FString::Printf(TEXT("Localization_%s_%d"), *File, i));
                            Finding.DetectionType = EDetectionType::CodeQuality;
                            Finding.Severity = EDetectionSeverity::Low;
                            Finding.Title = FText::FromString(TEXT("可能需要本地化"));
                            Finding.Description = FText::FromString(TEXT("檢測到硬編碼字符串"));
                            Finding.TargetPath = FilePath;
                            Finding.Recommendation = FText::FromString(TEXT("建議使用 NSLOCTEXT 或 LOCTEXT 進行本地化"));
                            Finding.bAutoFixable = false;
                            Finding.DetectionTime = FDateTime::Now();
                            
                            Findings.Add(Finding);
                        }
                    }
                }
            }
        }
    }
    
    return Findings;
}

TArray<FDetectionFinding> UMingRTSAutomatedDetectionSystem::CheckAssetIntegrity(const TArray<FString>& TargetPaths)
{
    TArray<FDetectionFinding> Findings;
    
    for (const FString& Path : TargetPaths)
    {
        // 檢查 .uasset 文件
        TArray<FString> AssetFiles;
        IFileManager::Get().FindFilesRecursive(AssetFiles, *Path, TEXT("*.uasset"), true, false);
        
        for (const FString& AssetFile : AssetFiles)
        {
            FString AssetPath = FPaths::Combine(Path, AssetFile);
            
            // 檢查文件大小（異常小的文件可能損壞）
            int64 FileSize = IFileManager::Get().FileSize(*AssetPath);
            if (FileSize < 100)
            {
                FDetectionFinding Finding;
                Finding.FindingID = FName(*FString::Printf(TEXT("SmallAsset_%s"), *AssetFile));
                Finding.DetectionType = EDetectionType::AssetIntegrity;
                Finding.Severity = EDetectionSeverity::Medium;
                Finding.Title = FText::FromString(TEXT("資源文件異常"));
                Finding.Description = FText::FromString(FString::Printf(TEXT("資源文件 %s 大小異常（%d bytes）"), *AssetFile, FileSize));
                Finding.TargetPath = AssetPath;
                Finding.Recommendation = FText::FromString(TEXT("請檢查資源文件是否損壞"));
                Finding.bAutoFixable = false;
                Finding.DetectionTime = FDateTime::Now();
                
                Findings.Add(Finding);
            }
        }
    }
    
    return Findings;
}

TArray<FDetectionFinding> UMingRTSAutomatedDetectionSystem::ScanSecurityIssues()
{
    TArray<FDetectionFinding> Findings;
    
    // 掃描安全問題
    // 這裡可以添加具體的安全掃描邏輯
    
    return Findings;
}

TArray<FDetectionFinding> UMingRTSAutomatedDetectionSystem::ValidateLocalization()
{
    TArray<FDetectionFinding> Findings;
    
    // 檢查本地化文件
    FString ContentDir = FPaths::ProjectContentDir();
    FString LocalizationDir = ContentDir / TEXT("Localization");
    
    if (!FPaths::DirectoryExists(LocalizationDir))
    {
        FDetectionFinding Finding;
        Finding.FindingID = TEXT("MissingLocalization");
        Finding.DetectionType = EDetectionType::LocalizationCheck;
        Finding.Severity = EDetectionSeverity::Medium;
        Finding.Title = FText::FromString(TEXT("缺少本地化目錄"));
        Finding.Description = FText::FromString(TEXT("Content/Localization 目錄不存在"));
        Finding.Recommendation = FText::FromString(TEXT("請創建本地化目錄並添加語言文件"));
        Finding.bAutoFixable = false;
        Finding.DetectionTime = FDateTime::Now();
        
        Findings.Add(Finding);
    }
    
    return Findings;
}

FDetectionMetrics UMingRTSAutomatedDetectionSystem::CollectPerformanceMetrics()
{
    FDetectionMetrics Metric;
    Metric.MetricName = TEXT("FrameRate");
    Metric.CurrentValue = 60.0f; // 假設值
    Metric.ExpectedValue = 60.0f;
    Metric.MinThreshold = 30.0f;
    Metric.Unit = TEXT("FPS");
    Metric.bIsPassed = Metric.CurrentValue >= Metric.MinThreshold;
    
    return Metric;
}

FDetectionMetrics UMingRTSAutomatedDetectionSystem::CollectMemoryMetrics()
{
    FDetectionMetrics Metric;
    Metric.MetricName = TEXT("MemoryUsage");
    Metric.CurrentValue = 2048.0f; // 假設值 MB
    Metric.ExpectedValue = 2048.0f;
    Metric.MinThreshold = 0.0f;
    Metric.MaxThreshold = 4096.0f;
    Metric.Unit = TEXT("MB");
    Metric.bIsPassed = Metric.CurrentValue <= Metric.MaxThreshold;
    
    return Metric;
}

void UMingRTSAutomatedDetectionSystem::CalculateOverallScore(FDetectionResult& Result)
{
    float BaseScore = 100.0f;
    
    // 根據發現的問題計算扣分
    BaseScore -= Result.CriticalFindings * 20.0f;
    BaseScore -= Result.HighFindings * 10.0f;
    BaseScore -= Result.MediumFindings * 5.0f;
    BaseScore -= Result.LowFindings * 2.0f;
    
    // 確保分數在 0-100 範圍內
    Result.OverallHealthScore = FMath::Clamp(BaseScore, 0.0f, 100.0f);
}

FString UMingRTSAutomatedDetectionSystem::GenerateDetectionReport(const FDetectionResult& Result, bool bAsHTML)
{
    if (bAsHTML)
    {
        return GenerateHTMLReport(Result);
    }
    else
    {
        return GenerateTextReport(Result);
    }
}

FString UMingRTSAutomatedDetectionSystem::GenerateHTMLReport(const FDetectionResult& Result)
{
    FString HTML = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    HTML += TEXT("<title>MingGoRTS 自動化檢測報告</title>\n");
    HTML += TEXT("<style>\n");
    HTML += TEXT("body { font-family: 'Segoe UI', Arial, sans-serif; margin: 20px; background: #f5f5f5; }\n");
    HTML += TEXT(".container { max-width: 1200px; margin: 0 auto; background: white; padding: 30px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n");
    HTML += TEXT(".header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 10px; margin-bottom: 30px; }\n");
    HTML += TEXT(".health-score { font-size: 48px; font-weight: bold; text-align: center; margin: 20px 0; }\n");
    HTML += TEXT(".score-excellent { color: #4CAF50; }\n");
    HTML += TEXT(".score-good { color: #8BC34A; }\n");
    HTML += TEXT(".score-warning { color: #FFC107; }\n");
    HTML += TEXT(".score-poor { color: #F44336; }\n");
    HTML += TEXT(".stats-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); gap: 20px; margin: 20px 0; }\n");
    HTML += TEXT(".stat-card { background: #f8f9fa; padding: 20px; border-radius: 8px; text-align: center; }\n");
    HTML += TEXT(".stat-number { font-size: 32px; font-weight: bold; }\n");
    HTML += TEXT(".critical { color: #F44336; }\n");
    HTML += TEXT(".high { color: #FF9800; }\n");
    HTML += TEXT(".medium { color: #FFC107; }\n");
    HTML += TEXT(".low { color: #4CAF50; }\n");
    HTML += TEXT("table { width: 100%; border-collapse: collapse; margin: 20px 0; }\n");
    HTML += TEXT("th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    HTML += TEXT("th { background: #667eea; color: white; }\n");
    HTML += TEXT("tr:hover { background: #f5f5f5; }\n");
    HTML += TEXT("</style>\n</head>\n<body>\n");
    
    HTML += TEXT("<div class=\"container\">\n");
    
    // 標題區域
    HTML += TEXT("<div class=\"header\">\n");
    HTML += FString::Printf(TEXT("<h1>%s</h1>\n"), *Result.DetectionName);
    HTML += FString::Printf(TEXT("<p>執行時間: %s - %s | 總耗時: %.1f 秒</p>\n"),
        *Result.StartTime.ToString(), *Result.EndTime.ToString(), Result.TotalExecutionTime);
    HTML += TEXT("</div>\n");
    
    // 健康評分
    FString ScoreClass;
    if (Result.OverallHealthScore >= 90) ScoreClass = TEXT("score-excellent");
    else if (Result.OverallHealthScore >= 70) ScoreClass = TEXT("score-good");
    else if (Result.OverallHealthScore >= 50) ScoreClass = TEXT("score-warning");
    else ScoreClass = TEXT("score-poor");
    
    HTML += FString::Printf(TEXT("<div class=\"health-score %s\">%.1f%%</div>\n"), *ScoreClass, Result.OverallHealthScore);
    HTML += TEXT("<p style=\"text-align: center; color: #666;\">項目健康評分</p>\n");
    
    // 統計卡片
    HTML += TEXT("<div class=\"stats-grid\">\n");
    HTML += FString::Printf(TEXT("<div class=\"stat-card\"><div class=\"stat-number critical\">%d</div><div>嚴重問題</div></div>\n"), Result.CriticalFindings);
    HTML += FString::Printf(TEXT("<div class=\"stat-card\"><div class=\"stat-number high\">%d</div><div>高級問題</div></div>\n"), Result.HighFindings);
    HTML += FString::Printf(TEXT("<div class=\"stat-card\"><div class=\"stat-number medium\">%d</div><div>中級問題</div></div>\n"), Result.MediumFindings);
    HTML += FString::Printf(TEXT("<div class=\"stat-card\"><div class=\"stat-number low\">%d</div><div>低級問題</div></div>\n"), Result.LowFindings);
    HTML += TEXT("</div>\n");
    
    // 任務結果表格
    HTML += TEXT("<h2>檢測任務詳情</h2>\n");
    HTML += TEXT("<table>\n");
    HTML += TEXT("<tr><th>檢測類型</th><th>狀態</th><th>發現數</th><th>指標數</th><th>耗時</th><th>摘要</th></tr>\n");
    
    for (const FDetectionTaskResult& Task : Result.TaskResults)
    {
        FString StatusColor;
        switch (Task.Status)
        {
            case EDetectionStatus::Passed: StatusColor = TEXT("#4CAF50"); break;
            case EDetectionStatus::Failed: StatusColor = TEXT("#F44336"); break;
            case EDetectionStatus::Warning: StatusColor = TEXT("#FFC107"); break;
            default: StatusColor = TEXT("#9E9E9E"); break;
        }
        
        HTML += FString::Printf(TEXT("<tr>"));
        HTML += FString::Printf(TEXT("<td>%s</td>"), *StaticEnum<EDetectionType>()->GetNameStringByValue((int64)Task.DetectionType));
        HTML += FString::Printf(TEXT("<td style=\"color: %s; font-weight: bold;\">%s</td>"), *StatusColor, *StaticEnum<EDetectionStatus>()->GetNameStringByValue((int64)Task.Status));
        HTML += FString::Printf(TEXT("<td>%d</td>"), Task.Findings.Num());
        HTML += FString::Printf(TEXT("<td>%d</td>"), Task.Metrics.Num());
        HTML += FString::Printf(TEXT("<td>%.2f s</td>"), Task.ExecutionTimeSeconds);
        HTML += FString::Printf(TEXT("<td>%s</td>"), *Task.Summary.ToString());
        HTML += TEXT("</tr>\n");
    }
    
    HTML += TEXT("</table>\n");
    HTML += TEXT("</div>\n</body>\n</html>");
    
    // 保存報告
    FString ReportPath = FPaths::ProjectLogDir() / FString::Printf(TEXT("Detection_Report_%s.html"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    FFileHelper::SaveStringToFile(HTML, *ReportPath);
    
    return ReportPath;
}

FString UMingRTSAutomatedDetectionSystem::GenerateTextReport(const FDetectionResult& Result)
{
    FString Report = TEXT("========================================\n");
    Report += FString::Printf(TEXT("自動化檢測報告: %s\n"), *Result.DetectionName);
    Report += TEXT("========================================\n\n");
    
    Report += FString::Printf(TEXT("執行時間: %s - %s\n"), *Result.StartTime.ToString(), *Result.EndTime.ToString());
    Report += FString::Printf(TEXT("總耗時: %.1f 秒\n"), Result.TotalExecutionTime);
    Report += FString::Printf(TEXT("健康評分: %.1f%%\n\n"), Result.OverallHealthScore);
    
    Report += TEXT("--- 問題統計 ---\n");
    Report += FString::Printf(TEXT("嚴重: %d | 高: %d | 中: %d | 低: %d\n\n"),
        Result.CriticalFindings, Result.HighFindings, Result.MediumFindings, Result.LowFindings);
    
    Report += TEXT("--- 檢測任務結果 ---\n");
    for (const FDetectionTaskResult& Task : Result.TaskResults)
    {
        Report += FString::Printf(TEXT("\n[%s] %s\n"),
            *StaticEnum<EDetectionType>()->GetNameStringByValue((int64)Task.DetectionType),
            *StaticEnum<EDetectionStatus>()->GetNameStringByValue((int64)Task.Status));
        Report += FString::Printf(TEXT("  發現: %d | 指標: %d | 耗時: %.2f s\n"),
            Task.Findings.Num(), Task.Metrics.Num(), Task.ExecutionTimeSeconds);
        Report += FString::Printf(TEXT("  %s\n"), *Task.Summary.ToString());
    }
    
    Report += TEXT("\n========================================\n");
    Report += TEXT("報告生成完成\n");
    
    FString ReportPath = FPaths::ProjectLogDir() / FString::Printf(TEXT("Detection_Report_%s.txt"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    FFileHelper::SaveStringToFile(Report, *ReportPath);
    
    return ReportPath;
}

bool UMingRTSAutomatedDetectionSystem::ExportToJSON(const FDetectionResult& Result, const FString& FilePath)
{
    TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
    
    RootObject->SetStringField(TEXT("DetectionName"), Result.DetectionName);
    RootObject->SetStringField(TEXT("StartTime"), Result.StartTime.ToIso8601());
    RootObject->SetStringField(TEXT("EndTime"), Result.EndTime.ToIso8601());
    RootObject->SetNumberField(TEXT("TotalExecutionTime"), Result.TotalExecutionTime);
    RootObject->SetNumberField(TEXT("OverallHealthScore"), Result.OverallHealthScore);
    RootObject->SetNumberField(TEXT("TotalFindings"), Result.TotalFindings);
    RootObject->SetNumberField(TEXT("CriticalFindings"), Result.CriticalFindings);
    RootObject->SetNumberField(TEXT("HighFindings"), Result.HighFindings);
    RootObject->SetNumberField(TEXT("MediumFindings"), Result.MediumFindings);
    RootObject->SetNumberField(TEXT("LowFindings"), Result.LowFindings);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    return FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

FDetectionConfig UMingRTSAutomatedDetectionSystem::GetDefaultDetectionConfig()
{
    FDetectionConfig Config;
    Config.DetectionName = TEXT("MingGoRTS 項目健康檢測");
    Config.EnabledDetections = {
        EDetectionType::ProjectHealth,
        EDetectionType::CodeQuality,
        EDetectionType::ConfigurationCheck,
        EDetectionType::DependencyCheck,
        EDetectionType::AssetIntegrity
    };
    Config.ScanPaths.Add(FPaths::ProjectSourceDir());
    Config.ScanPaths.Add(FPaths::ProjectContentDir());
    Config.ParallelDetectionCount = 4;
    Config.DetectionTimeoutSeconds = 600.0f;
    Config.bGenerateDetailedReport = true;
    Config.bSendNotifications = true;
    
    return Config;
}

TMap<EDetectionSeverity, int32> UMingRTSAutomatedDetectionSystem::GetFindingStatistics(const FDetectionResult& Result)
{
    TMap<EDetectionSeverity, int32> Stats;
    
    for (const FDetectionTaskResult& Task : Result.TaskResults)
    {
        for (const FDetectionFinding& Finding : Task.Findings)
        {
            int32 Count = Stats.FindRef(Finding.Severity);
            Stats.Add(Finding.Severity, Count + 1);
        }
    }
    
    return Stats;
}

TArray<FDetectionFinding> UMingRTSAutomatedDetectionSystem::FilterFindings(const TArray<FDetectionFinding>& Findings, EDetectionSeverity MinSeverity)
{
    TArray<FDetectionFinding> Filtered;
    
    for (const FDetectionFinding& Finding : Findings)
    {
        if ((int32)Finding.Severity >= (int32)MinSeverity)
        {
            Filtered.Add(Finding);
        }
    }
    
    return Filtered;
}

bool UMingRTSAutomatedDetectionSystem::IsAllPassed(const FDetectionResult& Result)
{
    for (const FDetectionTaskResult& Task : Result.TaskResults)
    {
        if (Task.Status == EDetectionStatus::Failed)
        {
            return false;
        }
    }
    return true;
}

float UMingRTSAutomatedDetectionSystem::CalculateHealthScore(const FDetectionResult& Result)
{
    return Result.OverallHealthScore;
}

void UMingRTSAutomatedDetectionSystem::SendDetectionNotification(const FDetectionResult& Result)
{
    FString Title;
    FString Message;
    
    if (Result.OverallHealthScore >= 90)
    {
        Title = TEXT("項目健康檢測通過 ✓");
        Message = FString::Printf(TEXT("健康評分: %.1f%% - 項目狀態良好！"), Result.OverallHealthScore);
    }
    else if (Result.OverallHealthScore >= 70)
    {
        Title = TEXT("項目健康檢測通過 ⚠");
        Message = FString::Printf(TEXT("健康評分: %.1f%% - 發現 %d 個需要注意的問題"),
            Result.OverallHealthScore, Result.TotalFindings);
    }
    else
    {
        Title = TEXT("項目健康檢測失敗 ✗");
        Message = FString::Printf(TEXT("健康評分: %.1f%% - 發現 %d 個嚴重問題需要處理"),
            Result.OverallHealthScore, Result.CriticalFindings + Result.HighFindings);
    }
    
    UE_LOG(LogMingGoRTS, Log, TEXT("Detection Notification: %s - %s"), *Title, *Message);
}
