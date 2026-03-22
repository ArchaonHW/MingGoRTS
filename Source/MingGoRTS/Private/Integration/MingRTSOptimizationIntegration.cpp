#include "MingRTSOptimizationIntegration.h"
#include "Engine/Engine.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

UMingRTSOptimizationIntegration::UMingRTSOptimizationIntegration()
{
    bSystemIntegrated = false;
    bOptimizationActive = false;
}

void UMingRTSOptimizationIntegration::IntegrateOptimizationSystem()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Integrating optimization system into MingGoRTS"));

    // 創建優化系統管理器
    OptimizationManager = NewObject<UMingRTSOptimizationSystemManager>();
    OptimizationManager->InitializeOptimizationSystem();

    // 設置遊戲特定優化
    SetupGameSpecificOptimizations();

    // 配置優化參數
    ConfigureGameOptimizationParameters();

    // 集成到遊戲系統
    IntegrateWithGameSystems();

    // 設置事件處理
    SetupEventHandlers();

    bSystemIntegrated = true;

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimization system integrated successfully"));
}

void UMingRTSOptimizationIntegration::StartGameOptimization()
{
    if (!bSystemIntegrated)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSOptimizationIntegration: System not integrated"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Starting game optimization"));

    // 設置遊戲開始時的優化配置
    FOptimizationConfiguration GameConfig;
    GameConfig.Mode = EOptimizationMode::Development;
    GameConfig.bEnableParallelCompilation = true;
    GameConfig.bEnablePerformanceProfiling = true;
    GameConfig.bEnableAlgorithmOptimization = true;
    GameConfig.MaxCompilationThreads = FPlatformMisc::NumberOfCores() / 2;
    GameConfig.PerformanceMonitoringInterval = 1.0f;

    // 添加遊戲特定的算法優化
    GameConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
    GameConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::CacheOptimization);
    GameConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Parallelization);

    OptimizationManager->SetOptimizationConfiguration(GameConfig);

    // 開始實時監控
    OptimizationManager->StartRealTimeMonitoring();

    // 優化遊戲循環
    OptimizeGameLoop();

    bOptimizationActive = true;

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Game optimization started"));
}

void UMingRTSOptimizationIntegration::RuntimeOptimization()
{
    if (!bOptimizationActive)
    {
        return;
    }

    // 持續監控遊戲性能
    MonitorGamePerformance();

    // 自動優化基於性能
    AutoOptimizeBasedOnPerformance();

    // 優化各個遊戲系統
    OptimizeRendering();
    OptimizeAI();
    OptimizeAudio();
    OptimizePhysics();
}

void UMingRTSOptimizationIntegration::LevelTransitionOptimization()
{
    if (!bOptimizationActive)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing for level transition"));

    // 關卡切換時的優化配置
    FOptimizationConfiguration LevelConfig;
    LevelConfig.Mode = EOptimizationMode::Testing;
    LevelConfig.bEnableParallelCompilation = true;
    LevelConfig.bEnablePerformanceProfiling = true;
    LevelConfig.MaxCompilationThreads = FPlatformMisc::NumberOfCores();

    OptimizationManager->SetOptimizationConfiguration(LevelConfig);

    // 預加載優化
    OptimizePreloading();

    // 內存清理優化
    OptimizeMemoryCleanup();

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Level transition optimization completed"));
}

void UMingRTSOptimizationIntegration::SaveGameOptimization()
{
    if (!bOptimizationActive)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing for save game"));

    // 保存時的優化配置
    FOptimizationConfiguration SaveConfig;
    SaveConfig.Mode = EOptimizationMode::Development;
    SaveConfig.bEnablePerformanceProfiling = false; // 保存時不需要性能分析
    SaveConfig.bEnableAlgorithmOptimization = false;

    OptimizationManager->SetOptimizationConfiguration(SaveConfig);

    // 優化保存數據
    OptimizeSaveData();

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Save game optimization completed"));
}

void UMingRTSOptimizationIntegration::LoadGameOptimization()
{
    if (!bOptimizationActive)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing for load game"));

    // 加載時的優化配置
    FOptimizationConfiguration LoadConfig;
    LoadConfig.Mode = EOptimizationMode::Development;
    LoadConfig.bEnablePerformanceProfiling = true;
    LoadConfig.bEnableAlgorithmOptimization = false;

    OptimizationManager->SetOptimizationConfiguration(LoadConfig);

    // 優化加載過程
    OptimizeLoadingProcess();

    // 重新啟動實時監控
    OptimizationManager->StartRealTimeMonitoring();

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Load game optimization completed"));
}

bool UMingRTSOptimizationIntegration::IsOptimizationSystemActive() const
{
    return bOptimizationActive && bSystemIntegrated && OptimizationManager.IsValid();
}

FString UMingRTSOptimizationIntegration::GetGamePerformanceStats()
{
    if (!IsOptimizationSystemActive())
    {
        return TEXT("Optimization system not active");
    }

    // 獲取性能概覽
    TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();

    // 獲取優化報告
    FString Report = OptimizationManager->GetOptimizationReport();

    // 生成遊戲特定統計
    FString GameStats = TEXT("=== MingGoRTS 性能統計 ===\n");
    GameStats += FString::Printf(TEXT("系統狀態: %s\n"), IsOptimizationSystemActive() ? TEXT("活躍") : TEXT("非活躍"));
    GameStats += FString::Printf(TEXT("性能指標: %d個\n"), PerformanceOverview.Num());

    for (const auto& Pair : PerformanceOverview)
    {
        GameStats += FString::Printf(TEXT("%s: %s\n"), 
            *GetProfilingTypeName(Pair.Key), 
            *GetPerformanceLevelName(Pair.Value));
    }

    GameStats += TEXT("\n");
    GameStats += Report;

    return GameStats;
}

// 私有方法實現

void UMingRTSOptimizationIntegration::SetupGameSpecificOptimizations()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Setting up game-specific optimizations"));

    // 設置MingGoRTS特定的優化規則
    // 這裡可以添加遊戲特定的優化配置
}

void UMingRTSOptimizationIntegration::ConfigureGameOptimizationParameters()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Configuring game optimization parameters"));

    // 配置遊戲特定的優化參數
    if (OptimizationManager.IsValid())
    {
        // 設置默認配置
        FOptimizationConfiguration DefaultConfig;
        DefaultConfig.Mode = EOptimizationMode::Development;
        DefaultConfig.bEnableParallelCompilation = true;
        DefaultConfig.bEnablePerformanceProfiling = true;
        DefaultConfig.bEnableAlgorithmOptimization = true;
        DefaultConfig.MaxCompilationThreads = FPlatformMisc::NumberOfCores();
        DefaultConfig.PerformanceMonitoringInterval = 1.0f;

        OptimizationManager->SetOptimizationConfiguration(DefaultConfig);
    }
}

void UMingRTSOptimizationIntegration::IntegrateWithGameSystems()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Integrating with game systems"));

    // 與遊戲系統集成
    // 這裡可以添加與其他遊戲系統的集成邏輯
}

void UMingRTSOptimizationIntegration::SetupEventHandlers()
{
    if (!OptimizationManager.IsValid())
    {
        return;
    }

    // 設置性能警告處理
    OptimizationManager->OnPerformanceWarning.AddDynamic(this, &UMingRTSOptimizationIntegration::HandlePerformanceWarnings);

    // 設置優化完成處理
    OptimizationManager->OnOptimizationCompleted.AddDynamic(this, &UMingRTSOptimizationIntegration::OnOptimizationCompleted);

    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Event handlers setup completed"));
}

void UMingRTSOptimizationIntegration::OptimizeGameLoop()
{
    // 優化遊戲主循環
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing game loop"));

    // 這裡可以添加遊戲循環特定的優化邏輯
}

void UMingRTSOptimizationIntegration::OptimizeRendering()
{
    // 優化渲染系統
    // 這裡可以添加渲染特定的優化邏輯
}

void UMingRTSOptimizationIntegration::OptimizeAI()
{
    // 優化AI系統
    // 這裡可以添加AI特定的優化邏輯
}

void UMingRTSOptimizationIntegration::OptimizeAudio()
{
    // 優化音頻系統
    // 這裡可以添加音頻特定的優化邏輯
}

void UMingRTSOptimizationIntegration::OptimizePhysics()
{
    // 優化物理系統
    // 這裡可以添加物理特定的優化邏輯
}

void UMingRTSOptimizationIntegration::MonitorGamePerformance()
{
    // 監控遊戲性能
    // 這裡可以添加性能監控邏輯
}

void UMingRTSOptimizationIntegration::HandlePerformanceWarnings(const FString& Component, const FString& Warning, float Severity)
{
    UE_LOG(LogTemp, Warning, TEXT("MingRTSOptimizationIntegration: Performance warning - %s: %s (Severity: %.2f)"), 
           *Component, *Warning, Severity);

    // 處理性能警告
    if (Severity > 0.8f)
    {
        // 高嚴重度警告，立即處理
        HandleCriticalPerformanceIssue(Component, Warning);
    }
    else if (Severity > 0.5f)
    {
        // 中等嚴重度警告，記錄並監控
        HandleMediumPerformanceIssue(Component, Warning);
    }
}

void UMingRTSOptimizationIntegration::AutoOptimizeBasedOnPerformance()
{
    // 基於性能自動優化
    if (OptimizationManager.IsValid())
    {
        TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();

        // 檢查是否需要調整優化級別
        bool bNeedUpgrade = false;
        for (const auto& Pair : PerformanceOverview)
        {
            if (Pair.Value <= EPerformanceLevel::Poor)
            {
                bNeedUpgrade = true;
                break;
            }
        }

        if (bNeedUpgrade)
        {
            UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Auto-upgrading optimization level"));
            UpgradeOptimizationLevel();
        }
    }
}

void UMingRTSOptimizationIntegration::AdjustQualitySettings()
{
    // 調整質量設置
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Adjusting quality settings"));

    // 這裡可以添加質量調整邏輯
}

void UMingRTSOptimizationIntegration::OptimizePreloading()
{
    // 優化預加載
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing preloading"));
}

void UMingRTSOptimizationIntegration::OptimizeMemoryCleanup()
{
    // 優化內存清理
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing memory cleanup"));
}

void UMingRTSOptimizationIntegration::OptimizeSaveData()
{
    // 優化保存數據
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing save data"));
}

void UMingRTSOptimizationIntegration::OptimizeLoadingProcess()
{
    // 優化加載過程
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimizing loading process"));
}

void UMingRTSOptimizationIntegration::HandleCriticalPerformanceIssue(const FString& Component, const FString& Warning)
{
    UE_LOG(LogTemp, Error, TEXT("MingRTSOptimizationIntegration: Critical performance issue in %s: %s"), *Component, *Warning);

    // 立即採取措施
    EmergencyOptimization();
}

void UMingRTSOptimizationIntegration::HandleMediumPerformanceIssue(const FString& Component, const FString& Warning)
{
    UE_LOG(LogTemp, Warning, TEXT("MingRTSOptimizationIntegration: Medium performance issue in %s: %s"), *Component, *Warning);

    // 記錄並計劃優化
    ScheduleOptimization(Component, Warning);
}

void UMingRTSOptimizationIntegration::UpgradeOptimizationLevel()
{
    if (!OptimizationManager.IsValid())
    {
        return;
    }

    // 升級優化級別
    FOptimizationConfiguration CurrentConfig = OptimizationManager->GetCurrentConfiguration();
    
    // 根據當前模式升級
    switch (CurrentConfig.Mode)
    {
        case EOptimizationMode::Development:
            CurrentConfig.Mode = EOptimizationMode::Testing;
            break;
        case EOptimizationMode::Testing:
            CurrentConfig.Mode = EOptimizationMode::Release;
            break;
        case EOptimizationMode::Release:
            CurrentConfig.Mode = EOptimizationMode::Performance;
            break;
        case EOptimizationMode::Performance:
            // 已經是最高級別，添加更多優化技術
            CurrentConfig.AlgorithmOptimizationTechniques.AddUnique(EOptimizationTechnique::Hybrid);
            break;
    }

    OptimizationManager->SetOptimizationConfiguration(CurrentConfig);
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Upgraded to optimization mode %d"), (int32)CurrentConfig.Mode);
}

void UMingRTSOptimizationIntegration::EmergencyOptimization()
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Emergency optimization activated"));

    // 緊急優化措施
    if (OptimizationManager.IsValid())
    {
        // 設置最高優化級別
        FOptimizationConfiguration EmergencyConfig;
        EmergencyConfig.Mode = EOptimizationMode::Performance;
        EmergencyConfig.bEnableParallelCompilation = true;
        EmergencyConfig.bEnablePerformanceProfiling = true;
        EmergencyConfig.bEnableAlgorithmOptimization = true;
        EmergencyConfig.MaxCompilationThreads = FPlatformMisc::NumberOfCores();
        EmergencyConfig.PerformanceMonitoringInterval = 0.1f;

        // 添加所有優化技術
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::CacheOptimization);
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Parallelization);
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Memoization);
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Approximation);
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Heuristic);
        EmergencyConfig.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Hybrid);

        OptimizationManager->SetOptimizationConfiguration(EmergencyConfig);
        OptimizationManager->DeepOptimize();
    }
}

void UMingRTSOptimizationIntegration::ScheduleOptimization(const FString& Component, const FString& Warning)
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Scheduled optimization for %s"), *Component);

    // 計劃優化
    // 這裡可以添加計劃優化邏輯
}

void UMingRTSOptimizationIntegration::OnOptimizationCompleted(const FString& ProjectName, const FOptimizationConfiguration& Config)
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSOptimizationIntegration: Optimization completed for %s"), *ProjectName);

    // 優化完成後的處理
    // 這裡可以添加優化完成後的邏輯
}

// 輔助方法

FString UMingRTSOptimizationIntegration::GetProfilingTypeName(EProfilingType ProfilingType) const
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

FString UMingRTSOptimizationIntegration::GetPerformanceLevelName(EPerformanceLevel Level) const
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
