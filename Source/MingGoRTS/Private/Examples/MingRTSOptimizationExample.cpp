#include "MingRTSOptimizationExample.h"
#include "Engine/Engine.h"
#include "Misc/Paths.h"

UMingRTSOptimizationExample::UMingRTSOptimizationExample()
{
}

void UMingRTSOptimizationExample::BasicUsageExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 基礎使用示例 ==="));
    
    // 初始化示例
    InitializeExample();
    
    // 檢查系統是否準備就緒
    if (!OptimizationManager->IsSystemReady())
    {
        LogExampleResult(TEXT("基礎使用"), TEXT("系統未準備就緒"));
        return;
    }
    
    // 獲取當前配置
    FOptimizationConfiguration CurrentConfig = OptimizationManager->GetCurrentConfiguration();
    ShowConfigurationDetails(CurrentConfig);
    
    // 執行快速優化
    OptimizationManager->QuickOptimize();
    
    // 獲取性能概覽
    TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();
    
    FString Result = FString::Printf(TEXT("基礎使用完成 - 系統狀態: 正常, 性能指標: %d個"), PerformanceOverview.Num());
    LogExampleResult(TEXT("基礎使用"), Result);
}

void UMingRTSOptimizationExample::QuickOptimizationExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 快速優化示例 ==="));
    
    InitializeExample();
    
    // 設置快速優化配置
    FOptimizationConfiguration QuickConfig;
    QuickConfig.Mode = EOptimizationMode::Development;
    QuickConfig.bEnableParallelCompilation = true;
    QuickConfig.bEnablePerformanceProfiling = false;
    QuickConfig.bEnableAlgorithmOptimization = false;
    QuickConfig.MaxCompilationThreads = 2;
    QuickConfig.PerformanceMonitoringInterval = 2.0f;
    
    OptimizationManager->SetOptimizationConfiguration(QuickConfig);
    
    // 執行快速優化
    FString ProjectPath = FPaths::ProjectDir();
    OptimizationManager->CompileProject(ProjectPath);
    
    // 獲取優化報告
    FString Report = OptimizationManager->GetOptimizationReport();
    
    LogExampleResult(TEXT("快速優化"), TEXT("快速優化完成 - 編譯速度優化"));
    UE_LOG(LogTemp, Log, TEXT("快速優化報告:\n%s"), *Report);
}

void UMingRTSOptimizationExample::DeepOptimizationExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 深度優化示例 ==="));
    
    InitializeExample();
    
    // 設置深度優化配置
    FOptimizationConfiguration DeepConfig;
    DeepConfig.Mode = EOptimizationMode::Performance;
    DeepConfig.bEnableParallelCompilation = true;
    DeepConfig.bEnablePerformanceProfiling = true;
    DeepConfig.bEnableAlgorithmOptimization = true;
    DeepConfig.MaxCompilationThreads = FPlatformMisc::NumberOfCores();
    DeepConfig.PerformanceMonitoringInterval = 0.5f;
    DeepConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
    DeepConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
    DeepConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Parallelization);
    DeepConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::CacheOptimization);
    
    OptimizationManager->SetOptimizationConfiguration(DeepConfig);
    
    // 執行深度優化
    OptimizationManager->DeepOptimize();
    
    // 獲取詳細報告
    FString Report = OptimizationManager->GetOptimizationReport();
    DisplayPerformanceReport(Report);
    
    LogExampleResult(TEXT("深度優化"), TEXT("深度優化完成 - 全面性能提升"));
}

void UMingRTSOptimizationExample::CustomConfigurationExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 自定義配置示例 ==="));
    
    InitializeExample();
    
    // 創建自定義配置
    FOptimizationConfiguration CustomConfig;
    CustomConfig.Mode = EOptimizationMode::Testing;
    CustomConfig.bEnableParallelCompilation = true;
    CustomConfig.bEnablePerformanceProfiling = true;
    CustomConfig.bEnableAlgorithmOptimization = true;
    CustomConfig.MaxCompilationThreads = 4;
    CustomConfig.PerformanceMonitoringInterval = 1.0f;
    
    // 添加特定的算法優化技術
    CustomConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
    CustomConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Memoization);
    CustomConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Heuristic);
    
    // 應用配置
    OptimizationManager->SetOptimizationConfiguration(CustomConfig);
    
    // 驗證配置
    FOptimizationConfiguration AppliedConfig = OptimizationManager->GetCurrentConfiguration();
    
    FString Result = FString::Printf(TEXT("自定義配置應用成功 - 模式: %d, 線程: %d, 優化技術: %d個"), 
                                   (int32)AppliedConfig.Mode, 
                                   AppliedConfig.MaxCompilationThreads,
                                   AppliedConfig.AlgorithmOptimizationTechniques.Num());
    
    LogExampleResult(TEXT("自定義配置"), Result);
}

void UMingRTSOptimizationExample::RealTimeMonitoringExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 實時監控示例 ==="));
    
    InitializeExample();
    
    // 設置監控配置
    FOptimizationConfiguration MonitorConfig;
    MonitorConfig.Mode = EOptimizationMode::Development;
    MonitorConfig.bEnablePerformanceProfiling = true;
    MonitorConfig.PerformanceMonitoringInterval = 0.5f; // 0.5秒間隔
    
    OptimizationManager->SetOptimizationConfiguration(MonitorConfig);
    
    // 開始實時監控
    OptimizationManager->StartRealTimeMonitoring();
    
    // 模擬一些工作
    UE_LOG(LogTemp, Log, TEXT("模擬項目工作..."));
    for (int32 i = 0; i < 10; i++)
    {
        UE_LOG(LogTemp, Log, TEXT("工作中... %d/10"), i + 1);
        FPlatformProcess::Sleep(0.5f); // 等待0.5秒
        
        // 檢查性能狀態
        TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();
        if (PerformanceOverview.Num() > 0)
        {
            UE_LOG(LogTemp, Log, TEXT("當前性能狀態: %d個指標監控中"), PerformanceOverview.Num());
        }
    }
    
    // 停止實時監控
    OptimizationManager->StopRealTimeMonitoring();
    
    LogExampleResult(TEXT("實時監控"), TEXT("實時監控完成 - 持續監控5秒"));
}

void UMingRTSOptimizationExample::PerformanceAnalysisExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 性能分析示例 ==="));
    
    InitializeExample();
    
    // 設置性能分析配置
    FOptimizationConfiguration AnalysisConfig;
    AnalysisConfig.Mode = EOptimizationMode::Release;
    AnalysisConfig.bEnablePerformanceProfiling = true;
    AnalysisConfig.PerformanceMonitoringInterval = 0.1f;
    
    OptimizationManager->SetOptimizationConfiguration(AnalysisConfig);
    
    // 執行性能分析
    FString ProjectPath = FPaths::ProjectDir();
    OptimizationManager->AnalyzeProjectPerformance(ProjectPath);
    
    // 獲取性能概覽
    TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();
    
    // 顯示性能結果
    UE_LOG(LogTemp, Log, TEXT("性能分析結果:"));
    for (const auto& Pair : PerformanceOverview)
    {
        FString TypeName = GetProfilingTypeName(Pair.Key);
        FString LevelName = GetPerformanceLevelName(Pair.Value);
        UE_LOG(LogTemp, Log, TEXT("  %s: %s"), *TypeName, *LevelName);
    }
    
    LogExampleResult(TEXT("性能分析"), FString::Printf(TEXT("性能分析完成 - 分析了%d個性能指標"), PerformanceOverview.Num()));
}

void UMingRTSOptimizationExample::AlgorithmOptimizationExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 算法優化示例 ==="));
    
    InitializeExample();
    
    // 設置算法優化配置
    FOptimizationConfiguration AlgorithmConfig;
    AlgorithmConfig.Mode = EOptimizationMode::Performance;
    AlgorithmConfig.bEnableAlgorithmOptimization = true;
    AlgorithmConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
    AlgorithmConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
    AlgorithmConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Parallelization);
    AlgorithmConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Memoization);
    
    OptimizationManager->SetOptimizationConfiguration(AlgorithmConfig);
    
    // 執行算法優化
    FString ProjectPath = FPaths::ProjectDir();
    OptimizationManager->OptimizeProjectAlgorithms(ProjectPath);
    
    // 獲取優化報告
    FString Report = OptimizationManager->GetOptimizationReport();
    
    LogExampleResult(TEXT("算法優化"), TEXT("算法優化完成 - 應用多種優化技術"));
    UE_LOG(LogTemp, Log, TEXT("算法優化報告摘要:\n%s"), *Report.Left(500)); // 顯示前500字符
}

void UMingRTSOptimizationExample::BatchProcessingExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 批量處理示例 ==="));
    
    InitializeExample();
    
    // 設置批量處理配置
    FOptimizationConfiguration BatchConfig;
    BatchConfig.Mode = EOptimizationMode::Release;
    BatchConfig.bEnableParallelCompilation = true;
    BatchConfig.bEnablePerformanceProfiling = true;
    BatchConfig.bEnableAlgorithmOptimization = true;
    BatchConfig.MaxCompilationThreads = FPlatformMisc::NumberOfCores();
    
    OptimizationManager->SetOptimizationConfiguration(BatchConfig);
    
    // 執行批量優化
    FString ProjectPath = FPaths::ProjectDir();
    OptimizationManager->OptimizeProject(ProjectPath);
    
    // 獲取完整報告
    FString Report = OptimizationManager->GetOptimizationReport();
    DisplayPerformanceReport(Report);
    
    LogExampleResult(TEXT("批量處理"), TEXT("批量處理完成 - 並行編譯、性能分析、算法優化"));
}

void UMingRTSOptimizationExample::IntegratedWorkflowExample()
{
    UE_LOG(LogTemp, Log, TEXT("=== 集成工作流程示例 ==="));
    
    InitializeExample();
    
    // 步驟1: 開發階段快速編譯
    UE_LOG(LogTemp, Log, TEXT("步驟1: 開發階段快速編譯"));
    FOptimizationConfiguration DevConfig;
    DevConfig.Mode = EOptimizationMode::Development;
    DevConfig.bEnableParallelCompilation = true;
    DevConfig.bEnablePerformanceProfiling = false;
    OptimizationManager->SetOptimizationConfiguration(DevConfig);
    OptimizationManager->QuickOptimize();
    
    // 步驟2: 測試階段性能分析
    UE_LOG(LogTemp, Log, TEXT("步驟2: 測試階段性能分析"));
    FOptimizationConfiguration TestConfig;
    TestConfig.Mode = EOptimizationMode::Testing;
    TestConfig.bEnablePerformanceProfiling = true;
    TestConfig.PerformanceMonitoringInterval = 1.0f;
    OptimizationManager->SetOptimizationConfiguration(TestConfig);
    OptimizationManager->AnalyzeProjectPerformance(FPaths::ProjectDir());
    
    // 步驟3: 發布階段深度優化
    UE_LOG(LogTemp, Log, TEXT("步驟3: 發布階段深度優化"));
    OptimizationManager->DeepOptimize();
    
    // 步驟4: 最終報告
    FString FinalReport = OptimizationManager->GetOptimizationReport();
    DisplayPerformanceReport(FinalReport);
    
    LogExampleResult(TEXT("集成工作流程"), TEXT("完整開發流程優化完成 - 從開發到發布"));
}

FString UMingRTSOptimizationExample::GetUsageGuide()
{
    FString Guide = TEXT("=== MingGoRTS 優化編譯系統使用指南 ===\n\n");
    
    Guide += TEXT("1. 基礎使用:\n");
    Guide += TEXT("   - 初始化系統: InitializeOptimizationSystem()\n");
    Guide += TEXT("   - 檢查系統狀態: IsSystemReady()\n");
    Guide += TEXT("   - 快速優化: QuickOptimize()\n\n");
    
    Guide += TEXT("2. 配置選項:\n");
    Guide += TEXT("   - 開發模式: 快速編譯，基礎優化\n");
    Guide += TEXT("   - 測試模式: 平衡性能和編譯速度\n");
    Guide += TEXT("   - 發布模式: 最大優化\n");
    Guide += TEXT("   - 性能模式: 極致性能優化\n\n");
    
    Guide += TEXT("3. 主要功能:\n");
    Guide += TEXT("   - 批量編譯: CompileProject()\n");
    Guide += TEXT("   - 性能分析: AnalyzeProjectPerformance()\n");
    Guide += TEXT("   - 算法優化: OptimizeProjectAlgorithms()\n");
    Guide += TEXT("   - 實時監控: StartRealTimeMonitoring()\n\n");
    
    Guide += TEXT("4. 優化技術:\n");
    Guide += TEXT("   - 時間複雜度優化\n");
    Guide += TEXT("   - 空間複雜度優化\n");
    Guide += TEXT("   - 緩存優化\n");
    Guide += TEXT("   - 並行化優化\n");
    Guide += TEXT("   - 記憶化優化\n\n");
    
    Guide += TEXT("5. 最佳實踐:\n");
    Guide += TEXT("   - 開發階段使用快速優化\n");
    Guide += TEXT("   - 測試階段啟用性能分析\n");
    Guide += TEXT("   - 發布前進行深度優化\n");
    Guide += TEXT("   - 持續監控性能指標\n\n");
    
    Guide += TEXT("6. 性能指標:\n");
    Guide += TEXT("   - CPU使用率\n");
    Guide += TEXT("   - 內存使用量\n");
    Guide += TEXT("   - 編譯時間\n");
    Guide += TEXT("   - 算法複雜度\n\n");
    
    Guide += TEXT("=== 使用指南結束 ===");
    
    return Guide;
}

// 私有方法實現

void UMingRTSOptimizationExample::InitializeExample()
{
    if (!OptimizationManager.IsValid())
    {
        OptimizationManager = NewObject<UMingRTSOptimizationSystemManager>();
        OptimizationManager->InitializeOptimizationSystem();
    }
}

void UMingRTSOptimizationExample::LogExampleResult(const FString& ExampleName, const FString& Result)
{
    UE_LOG(LogTemp, Log, TEXT("示例結果 [%s]: %s"), *ExampleName, *Result);
}

void UMingRTSOptimizationExample::ShowConfigurationDetails(const FOptimizationConfiguration& Config)
{
    UE_LOG(LogTemp, Log, TEXT("當前配置詳情:"));
    UE_LOG(LogTemp, Log, TEXT("  模式: %d"), (int32)Config.Mode);
    UE_LOG(LogTemp, Log, TEXT("  並行編譯: %s"), Config.bEnableParallelCompilation ? TEXT("啟用") : TEXT("禁用"));
    UE_LOG(LogTemp, Log, TEXT("  性能分析: %s"), Config.bEnablePerformanceProfiling ? TEXT("啟用") : TEXT("禁用"));
    UE_LOG(LogTemp, Log, TEXT("  算法優化: %s"), Config.bEnableAlgorithmOptimization ? TEXT("啟用") : TEXT("禁用"));
    UE_LOG(LogTemp, Log, TEXT("  最大線程數: %d"), Config.MaxCompilationThreads);
    UE_LOG(LogTemp, Log, TEXT("  監控間隔: %.1f秒"), Config.PerformanceMonitoringInterval);
}

void UMingRTSOptimizationExample::DisplayPerformanceReport(const FString& Report)
{
    UE_LOG(LogTemp, Log, TEXT("性能報告:"));
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
}

// 輔助方法

FString UMingRTSOptimizationExample::GetProfilingTypeName(EProfilingType ProfilingType) const
{
    switch (ProfilingType)
    {
        case EProfilingType::CPU: return TEXT("CPU");
        case EProfilingType::Memory: return TEXT("Memory");
        case EProfilingType::GPU: return TEXT("GPU");
        case EProfilingType::Network: return TEXT("Network");
        case EProfilingType::Disk: return TEXT("Disk");
        case EProfilingType::Rendering: return TEXT("Rendering");
        case EProfilingType::Audio: return TEXT("Audio");
        case EProfilingType::Physics: return TEXT("Physics");
        default: return TEXT("Unknown");
    }
}

FString UMingRTSOptimizationExample::GetPerformanceLevelName(EPerformanceLevel Level) const
{
    switch (Level)
    {
        case EPerformanceLevel::Excellent: return TEXT("Excellent");
        case EPerformanceLevel::Good: return TEXT("Good");
        case EPerformanceLevel::Average: return TEXT("Average");
        case EPerformanceLevel::Poor: return TEXT("Poor");
        case EPerformanceLevel::Critical: return TEXT("Critical");
        default: return TEXT("Unknown");
    }
}
