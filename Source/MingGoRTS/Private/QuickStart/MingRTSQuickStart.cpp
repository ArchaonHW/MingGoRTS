#include "MingRTSQuickStart.h"
#include "Engine/Engine.h"
#include "Misc/Paths.h"
#include "Kismet/KismetSystemLibrary.h"

UMingRTSQuickStart::UMingRTSQuickStart()
{
    bSystemStarted = false;
    bOptimizationRunning = false;
}

void UMingRTSQuickStart::StartOptimizationNow()
{
    UE_LOG(LogTemp, Log, TEXT("=== MingGoRTS 優化編譯系統立即啟動 ==="));
    
    if (bSystemStarted)
    {
        UE_LOG(LogTemp, Warning, TEXT("優化系統已經啟動"));
        return;
    }

    // 初始化快速啟動
    InitializeQuickStart();

    // 創建優化系統管理器
    OptimizationManager = NewObject<UMingRTSOptimizationSystemManager>();
    OptimizationManager->InitializeOptimizationSystem();

    // 檢查系統狀態
    if (!OptimizationManager->IsSystemReady())
    {
        UE_LOG(LogTemp, Error, TEXT("優化系統初始化失敗"));
        return;
    }

    // 設置快速啟動配置
    FOptimizationConfiguration QuickConfig;
    QuickConfig.Mode = EOptimizationMode::Development;
    QuickConfig.bEnableParallelCompilation = true;
    QuickConfig.bEnablePerformanceProfiling = true;
    QuickConfig.bEnableAlgorithmOptimization = true;
    QuickConfig.MaxCompilationThreads = FPlatformMisc::NumberOfCores();
    QuickConfig.PerformanceMonitoringInterval = 1.0f;

    // 添加核心優化技術
    QuickConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
    QuickConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
    QuickConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::CacheOptimization);

    OptimizationManager->SetOptimizationConfiguration(QuickConfig);

    // 立即開始優化
    bOptimizationRunning = true;
    OptimizeCurrentProject();

    bSystemStarted = true;

    UE_LOG(LogTemp, Log, TEXT("✅ MingGoRTS 優化編譯系統已成功啟動！"));
    UE_LOG(LogTemp, Log, TEXT("🚀 系統狀態: 活躍"));
    UE_LOG(LogTemp, Log, TEXT("⚡ 優化模式: 開發模式"));
    UE_LOG(LogTemp, Log, TEXT("🔧 並行線程: %d"), QuickConfig.MaxCompilationThreads);
}

FString UMingRTSQuickStart::GetSystemStatus()
{
    if (!bSystemStarted)
    {
        return TEXT("❌ 優化系統未啟動\n請調用 StartOptimizationNow() 來啟動系統");
    }

    FString Status = TEXT("🟢 MingGoRTS 優化編譯系統狀態\n\n");
    
    Status += FString::Printf(TEXT("系統狀態: %s\n"), bSystemStarted ? TEXT("✅ 已啟動") : TEXT("❌ 未啟動"));
    Status += FString::Printf(TEXT("優化狀態: %s\n"), bOptimizationRunning ? TEXT("🔄 運行中") : TEXT("⏸️ 空閒"));
    
    if (OptimizationManager.IsValid())
    {
        Status += FString::Printf(TEXT("系統就緒: %s\n"), OptimizationManager->IsSystemReady() ? TEXT("✅ 就緒") : TEXT("❌ 未就緒"));
        
        // 獲取當前配置
        FOptimizationConfiguration Config = OptimizationManager->GetCurrentConfiguration();
        Status += FString::Printf(TEXT("優化模式: %d\n"), (int32)Config.Mode);
        Status += FString::Printf(TEXT("並行編譯: %s\n"), Config.bEnableParallelCompilation ? TEXT("✅") : TEXT("❌"));
        Status += FString::Printf(TEXT("性能分析: %s\n"), Config.bEnablePerformanceProfiling ? TEXT("✅") : TEXT("❌"));
        Status += FString::Printf(TEXT("算法優化: %s\n"), Config.bEnableAlgorithmOptimization ? TEXT("✅") : TEXT("❌"));
        Status += FString::Printf(TEXT("最大線程: %d\n"), Config.MaxCompilationThreads);
        Status += FString::Printf(TEXT("監控間隔: %.1f秒\n"), Config.PerformanceMonitoringInterval);
        
        // 獲取性能概覽
        TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();
        Status += FString::Printf(TEXT("性能指標: %d個監控中\n"), PerformanceOverview.Num());
        
        Status += TEXT("\n📊 性能概覽:\n");
        for (const auto& Pair : PerformanceOverview)
        {
            FString TypeName = GetProfilingTypeName(Pair.Key);
            FString LevelName = GetPerformanceLevelName(Pair.Value);
            FString Emoji = GetPerformanceEmoji(Pair.Value);
            Status += FString::Printf(TEXT("  %s %s: %s\n"), *Emoji, *TypeName, *LevelName);
        }
    }
    else
    {
        Status += TEXT("❌ 優化管理器未創建");
    }

    return Status;
}

void UMingRTSQuickStart::QuickPerformanceCheck()
{
    if (!bSystemStarted || !OptimizationManager.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("請先啟動優化系統"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("🔍 執行快速性能檢查..."));

    // 開始實時監控
    OptimizationManager->StartRealTimeMonitoring();

    // 獲取當前性能狀態
    TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();

    UE_LOG(LogTemp, Log, TEXT("📊 性能檢查結果:"));
    
    int32 GoodCount = 0;
    int32 PoorCount = 0;
    
    for (const auto& Pair : PerformanceOverview)
    {
        FString TypeName = GetProfilingTypeName(Pair.Key);
        FString LevelName = GetPerformanceLevelName(Pair.Value);
        FString Emoji = GetPerformanceEmoji(Pair.Value);
        
        UE_LOG(LogTemp, Log, TEXT("  %s %s: %s"), *Emoji, *TypeName, *LevelName);
        
        if (Pair.Value >= EPerformanceLevel::Good)
        {
            GoodCount++;
        }
        else if (Pair.Value <= EPerformanceLevel::Poor)
        {
            PoorCount++;
        }
    }

    // 總結
    if (PoorCount == 0)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ 所有性能指標良好！系統運行優秀。"));
    }
    else if (PoorCount <= 2)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ 發現 %d 個性能問題，建議進行優化。"), PoorCount);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 發現 %d 個嚴重性能問題，需要立即優化！"), PoorCount);
    }

    UE_LOG(LogTemp, Log, TEXT("📈 性能評分: %d/%d 指標良好"), GoodCount, PerformanceOverview.Num());
}

void UMingRTSQuickStart::OptimizeCurrentProject()
{
    if (!bSystemStarted || !OptimizationManager.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("請先啟動優化系統"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("🚀 開始優化當前項目..."));

    // 獲取項目路徑
    FString ProjectPath = FPaths::ProjectDir();
    UE_LOG(LogTemp, Log, TEXT("📁 項目路徑: %s"), *ProjectPath);

    // 執行項目優化
    OptimizationManager->OptimizeProject(ProjectPath);

    // 獲取優化報告
    FString Report = OptimizationManager->GetOptimizationReport();
    
    UE_LOG(LogTemp, Log, TEXT("✅ 項目優化完成！"));
    UE_LOG(LogTemp, Log, TEXT("📄 優化報告:"));
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);

    bOptimizationRunning = false;
}

FString UMingRTSQuickStart::GetOptimizationResults()
{
    if (!bSystemStarted || !OptimizationManager.IsValid())
    {
        return TEXT("❌ 請先啟動優化系統");
    }

    FString Results = TEXT("🎯 MingGoRTS 優化結果\n\n");

    // 獲取優化報告
    FString Report = OptimizationManager->GetOptimizationReport();
    Results += Report;

    // 獲取性能統計
    TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();
    
    Results += TEXT("\n📊 最終性能評估:\n");
    for (const auto& Pair : PerformanceOverview)
    {
        FString TypeName = GetProfilingTypeName(Pair.Key);
        FString LevelName = GetPerformanceLevelName(Pair.Value);
        FString Emoji = GetPerformanceEmoji(Pair.Value);
        Results += FString::Printf(TEXT("  %s %s: %s\n"), *Emoji, *TypeName, *LevelName);
    }

    // 計算總體評分
    float AverageScore = CalculateAverageScore(PerformanceOverview);
    Results += FString::Printf(TEXT("\n🏆 總體性能評分: %.1f/100\n"), AverageScore * 100.0f);

    // 給出建議
    Results += GenerateRecommendations(AverageScore, PerformanceOverview);

    return Results;
}

// 私有方法實現

void UMingRTSQuickStart::InitializeQuickStart()
{
    UE_LOG(LogTemp, Log, TEXT("初始化快速啟動系統..."));
}

void UMingRTSQuickStart::DisplayResults(const FString& Results)
{
    UE_LOG(LogTemp, Log, TEXT("優化結果:"));
    UE_LOG(LogTemp, Log, TEXT("%s"), *Results);
}

// 輔助方法

FString UMingRTSQuickStart::GetProfilingTypeName(EProfilingType ProfilingType) const
{
    switch (ProfilingType)
    {
        case EProfilingType::CPU: return TEXT("CPU");
        case EProfilingType::Memory: return TEXT("內存");
        case EProfilingType::GPU: return TEXT("GPU");
        case EProfilingType::Network: return TEXT("網絡");
        case EProfilingType::Disk: return TEXT("磁盤");
        case EProfilingType::Rendering: return TEXT("渲染");
        case EProfilingType::Audio: return TEXT("音頻");
        case EProfilingType::Physics: return TEXT("物理");
        default: return TEXT("未知");
    }
}

FString UMingRTSQuickStart::GetPerformanceLevelName(EPerformanceLevel Level) const
{
    switch (Level)
    {
        case EPerformanceLevel::Excellent: return TEXT("優秀");
        case EPerformanceLevel::Good: return TEXT("良好");
        case EPerformanceLevel::Average: return TEXT("一般");
        case EPerformanceLevel::Poor: return TEXT("較差");
        case EPerformanceLevel::Critical: return TEXT("臨界");
        default: return TEXT("未知");
    }
}

FString UMingRTSQuickStart::GetPerformanceEmoji(EPerformanceLevel Level) const
{
    switch (Level)
    {
        case EPerformanceLevel::Excellent: return TEXT("🟢");
        case EPerformanceLevel::Good: return TEXT("🟡");
        case EPerformanceLevel::Average: return TEXT("🟠");
        case EPerformanceLevel::Poor: return TEXT("🔴");
        case EPerformanceLevel::Critical: return TEXT("💀");
        default: return TEXT("❓");
    }
}

float UMingRTSQuickStart::CalculateAverageScore(const TMap<EProfilingType, EPerformanceLevel>& PerformanceOverview) const
{
    if (PerformanceOverview.Num() == 0)
    {
        return 0.0f;
    }

    float TotalScore = 0.0f;
    for (const auto& Pair : PerformanceOverview)
    {
        float Score = 0.0f;
        switch (Pair.Value)
        {
            case EPerformanceLevel::Excellent: Score = 1.0f; break;
            case EPerformanceLevel::Good: Score = 0.8f; break;
            case EPerformanceLevel::Average: Score = 0.6f; break;
            case EPerformanceLevel::Poor: Score = 0.4f; break;
            case EPerformanceLevel::Critical: Score = 0.2f; break;
            default: Score = 0.0f; break;
        }
        TotalScore += Score;
    }

    return TotalScore / PerformanceOverview.Num();
}

FString UMingRTSQuickStart::GenerateRecommendations(float AverageScore, const TMap<EProfilingType, EPerformanceLevel>& PerformanceOverview) const
{
    FString Recommendations = TEXT("\n💡 優化建議:\n");

    if (AverageScore >= 0.8f)
    {
        Recommendations += TEXT("  ✅ 系統性能優秀，保持當前配置\n");
    }
    else if (AverageScore >= 0.6f)
    {
        Recommendations += TEXT("  ⚠️ 系統性能一般，建議進行以下優化:\n");
        Recommendations += TEXT("     - 考虑增加并行编译线程数\n");
        Recommendations += TEXT("     - 启用更多算法优化技术\n");
    }
    else
    {
        Recommendations += TEXT("  ❌ 系統性能較差，建議立即優化:\n");
        Recommendations += TEXT("     - 切换到性能模式\n");
        Recommendations += TEXT("     - 启用所有优化技术\n");
        Recommendations += TEXT("     - 检查系统资源使用情况\n");
    }

    // 針對特定組件的建議
    for (const auto& Pair : PerformanceOverview)
    {
        if (Pair.Value <= EPerformanceLevel::Poor)
        {
            FString ComponentName = GetProfilingTypeName(Pair.Key);
            Recommendations += FString::Printf(TEXT("  🔧 %s性能需要改善\n"), *ComponentName);
        }
    }

    return Recommendations;
}
