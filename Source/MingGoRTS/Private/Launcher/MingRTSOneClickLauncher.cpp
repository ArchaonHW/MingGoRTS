#include "MingRTSOneClickLauncher.h"
#include "Engine/Engine.h"
#include "HAL/PlatformMisc.h"
#include "Misc/Paths.h"
#include "Kismet/KismetSystemLibrary.h"

UMingRTSOneClickLauncher::UMingRTSOneClickLauncher()
{
    bSystemLaunched = false;
    bOptimizationActive = false;
    LaunchTime = FDateTime::Now();
}

UMingRTSOneClickLauncher* UMingRTSOneClickLauncher::LaunchOptimizationSystem()
{
    UE_LOG(LogTemp, Log, TEXT("🚀 MingGoRTS 一鍵啟動優化編譯系統"));
    
    // 創建啟動器實例
    UMingRTSOneClickLauncher* Launcher = NewObject<UMingRTSOneClickLauncher>();
    
    // 初始化並快速啟動
    Launcher->InitializeLauncher();
    Launcher->QuickLaunchAndOptimize();
    
    UE_LOG(LogTemp, Log, TEXT("✅ 一鍵啟動完成！系統已準備就緒"));
    
    return Launcher;
}

void UMingRTSOneClickLauncher::QuickLaunchAndOptimize()
{
    UE_LOG(LogTemp, Log, TEXT("⚡ 快速啟動並優化..."));
    
    if (bSystemLaunched)
    {
        UE_LOG(LogTemp, Warning, TEXT("系統已經啟動"));
        return;
    }

    // 初始化啟動器
    InitializeLauncher();

    // 配置開發模式（平衡性能和速度）
    ConfigureForMode(EOptimizationMode::Development);

    // 開始優化
    StartOptimization();

    // 顯示啟動成功信息
    ShowLaunchSuccess();

    bSystemLaunched = true;
    LaunchTime = FDateTime::Now();

    UE_LOG(LogTemp, Log, TEXT("🎯 快速啟動完成！系統正在運行"));
}

void UMingRTSOneClickLauncher::SmartLaunch()
{
    UE_LOG(LogTemp, Log, TEXT("🎯 智能啟動..."));
    
    if (bSystemLaunched)
    {
        UE_LOG(LogTemp, Warning, TEXT("系統已經啟動"));
        return;
    }

    // 初始化啟動器
    InitializeLauncher();

    // 根據系統資源智能選擇模式
    EOptimizationMode SmartMode = DetermineBestMode();
    ConfigureForMode(SmartMode);

    // 開始優化
    StartOptimization();

    // 顯示系統信息
    DisplaySystemInfo();

    bSystemLaunched = true;
    LaunchTime = FDateTime::Now();

    UE_LOG(LogTemp, Log, TEXT("🧠 智能啟動完成！使用模式: %s"), *GetLaunchModeName(SmartMode));
}

void UMingRTSOneClickLauncher::ExtremeLaunch()
{
    UE_LOG(LogTemp, Log, TEXT("🔥 極致啟動..."));
    
    if (bSystemLaunched)
    {
        UE_LOG(LogTemp, Warning, TEXT("系統已經啟動，切換到極致模式"));
    }

    // 初始化啟動器
    InitializeLauncher();

    // 配置極致性能模式
    ConfigureForMode(EOptimizationMode::Performance);

    // 開始優化
    StartOptimization();

    bSystemLaunched = true;
    LaunchTime = FDateTime::Now();

    UE_LOG(LogTemp, Log, TEXT("🔥 極致啟動完成！系統以最高性能運行"));
}

FString UMingRTSOneClickLauncher::GetLaunchStatus()
{
    FString Status = TEXT("🚀 MingGoRTS 一鍵啟動系統狀態\n\n");
    
    Status += FString::Printf(TEXT("系統狀態: %s\n"), bSystemLaunched ? TEXT("🟢 已啟動") : TEXT("🔴 未啟動"));
    Status += FString::Printf(TEXT("優化狀態: %s\n"), bOptimizationActive ? TEXT("🔄 運行中") : TEXT("⏸️ 空閒"));
    
    if (bSystemLaunched)
    {
        FTimespan ElapsedTime = FDateTime::Now() - LaunchTime;
        Status += FString::Printf(TEXT("運行時間: %s\n"), *ElapsedTime.ToString());
        
        if (OptimizationManager.IsValid())
        {
            Status += FString::Printf(TEXT("系統就緒: %s\n"), OptimizationManager->IsSystemReady() ? TEXT("✅ 就緒") : TEXT("❌ 未就緒"));
            
            // 獲取性能概覽
            TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();
            Status += FString::Printf(TEXT("監控指標: %d個\n"), PerformanceOverview.Num());
            
            Status += TEXT("\n📊 實時性能:\n");
            for (const auto& Pair : PerformanceOverview)
            {
                FString TypeName = GetProfilingTypeName(Pair.Key);
                FString LevelName = GetPerformanceLevelName(Pair.Value);
                FString Emoji = GetPerformanceEmoji(Pair.Value);
                Status += FString::Printf(TEXT("  %s %s: %s\n"), *Emoji, *TypeName, *LevelName);
            }
        }
    }
    
    // 系統健康度
    float Health = GetSystemHealth();
    Status += FString::Printf(TEXT("\n💚 系統健康度: %.1f%% %s\n"), Health * 100.0f, *GetSystemHealthStatus(Health));
    
    return Status;
}

FString UMingRTSOneClickLauncher::GetPerformanceReport()
{
    if (!bSystemLaunched || !OptimizationManager.IsValid())
    {
        return TEXT("❌ 請先啟動系統");
    }

    FString Report = TEXT("🎯 MingGoRTS 性能報告\n\n");

    // 獲取優化報告
    FString OptimizationReport = OptimizationManager->GetOptimizationReport();
    Report += OptimizationReport;

    // 添加啟動器特定信息
    Report += FString::Printf(TEXT("\n🚀 啟動器信息:\n"));
    Report += FString::Printf(TEXT("  啟動時間: %s\n"), *LaunchTime.ToString());
    
    FTimespan ElapsedTime = FDateTime::Now() - LaunchTime;
    Report += FString::Printf(TEXT("  運行時長: %s\n"), *ElapsedTime.ToString());
    Report += FString::Printf(TEXT("  系統健康: %.1f%%\n"), GetSystemHealth() * 100.0f);

    return Report;
}

void UMingRTSOneClickLauncher::RestartSystem()
{
    UE_LOG(LogTemp, Log, TEXT("🔄 重新啟動系統..."));
    
    // 停止當前系統
    StopSystem();
    
    // 等待一秒
    FPlatformProcess::Sleep(1.0f);
    
    // 重新啟動
    QuickLaunchAndOptimize();
    
    UE_LOG(LogTemp, Log, TEXT("✅ 系統重新啟動完成"));
}

void UMingRTSOneClickLauncher::StopSystem()
{
    UE_LOG(LogTemp, Log, TEXT("⏹️ 停止優化系統..."));
    
    if (OptimizationManager.IsValid())
    {
        OptimizationManager->StopRealTimeMonitoring();
    }
    
    bOptimizationActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("✅ 系統已停止"));
}

float UMingRTSOneClickLauncher::GetSystemHealth()
{
    if (!bSystemLaunched || !OptimizationManager.IsValid())
    {
        return 0.0f;
    }

    // 獲取性能概覽
    TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();
    
    if (PerformanceOverview.Num() == 0)
    {
        return 0.0f;
    }

    // 計算健康度
    float TotalHealth = 0.0f;
    for (const auto& Pair : PerformanceOverview)
    {
        float HealthScore = 0.0f;
        switch (Pair.Value)
        {
            case EPerformanceLevel::Excellent: HealthScore = 1.0f; break;
            case EPerformanceLevel::Good: HealthScore = 0.8f; break;
            case EPerformanceLevel::Average: HealthScore = 0.6f; break;
            case EPerformanceLevel::Poor: HealthScore = 0.4f; break;
            case EPerformanceLevel::Critical: HealthScore = 0.2f; break;
            default: HealthScore = 0.0f; break;
        }
        TotalHealth += HealthScore;
    }

    return TotalHealth / PerformanceOverview.Num();
}

void UMingRTSOneClickLauncher::OneClickPerformanceCheck()
{
    UE_LOG(LogTemp, Log, TEXT("🎯 一鍵性能檢查..."));
    
    if (!bSystemLaunched)
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 系統未啟動，請先啟動系統"));
        return;
    }

    if (!OptimizationManager.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 優化管理器無效"));
        return;
    }

    // 獲取當前性能狀態
    TMap<EProfilingType, EPerformanceLevel> PerformanceOverview = OptimizationManager->GetPerformanceOverview();

    UE_LOG(LogTemp, Log, TEXT("📊 性能檢查結果:"));
    
    int32 ExcellentCount = 0;
    int32 GoodCount = 0;
    int32 PoorCount = 0;
    int32 CriticalCount = 0;
    
    for (const auto& Pair : PerformanceOverview)
    {
        FString TypeName = GetProfilingTypeName(Pair.Key);
        FString LevelName = GetPerformanceLevelName(Pair.Value);
        FString Emoji = GetPerformanceEmoji(Pair.Value);
        
        UE_LOG(LogTemp, Log, TEXT("  %s %s: %s"), *Emoji, *TypeName, *LevelName);
        
        switch (Pair.Value)
        {
            case EPerformanceLevel::Excellent: ExcellentCount++; break;
            case EPerformanceLevel::Good: GoodCount++; break;
            case EPerformanceLevel::Average: break; // 中等不計入警告
            case EPerformanceLevel::Poor: PoorCount++; break;
            case EPerformanceLevel::Critical: CriticalCount++; break;
        }
    }

    // 總結和建議
    float Health = GetSystemHealth();
    UE_LOG(LogTemp, Log, TEXT("📈 系統健康度: %.1f%%"), Health * 100.0f);
    
    if (Health >= 0.8f)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ 系統性能優秀！運行狀態良好。"));
    }
    else if (Health >= 0.6f)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠️ 系統性能一般，建議進行優化。"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 系統性能較差，建議立即優化！"));
    }

    // 顯示性能報告
    ShowPerformanceResults();
}

// 私有方法實現

void UMingRTSOneClickLauncher::InitializeLauncher()
{
    UE_LOG(LogTemp, Log, TEXT("初始化一鍵啟動器..."));
    
    // 創建優化系統管理器
    if (!OptimizationManager.IsValid())
    {
        OptimizationManager = NewObject<UMingRTSOptimizationSystemManager>();
        OptimizationManager->InitializeOptimizationSystem();
    }
}

void UMingRTSOneClickLauncher::ConfigureForMode(EOptimizationMode Mode)
{
    if (!OptimizationManager.IsValid())
    {
        return;
    }

    FOptimizationConfiguration Config;
    Config.Mode = Mode;
    Config.bEnableParallelCompilation = true;
    Config.bEnablePerformanceProfiling = true;
    Config.bEnableAlgorithmOptimization = true;
    
    // 根據模式調整參數
    switch (Mode)
    {
        case EOptimizationMode::Development:
            Config.MaxCompilationThreads = FPlatformMisc::NumberOfCores() / 2;
            Config.PerformanceMonitoringInterval = 1.0f;
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
            break;
            
        case EOptimizationMode::Testing:
            Config.MaxCompilationThreads = FPlatformMisc::NumberOfCores() * 3 / 4;
            Config.PerformanceMonitoringInterval = 0.5f;
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::CacheOptimization);
            break;
            
        case EOptimizationMode::Release:
            Config.MaxCompilationThreads = FPlatformMisc::NumberOfCores();
            Config.PerformanceMonitoringInterval = 0.5f;
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::CacheOptimization);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Parallelization);
            break;
            
        case EOptimizationMode::Performance:
            Config.MaxCompilationThreads = FPlatformMisc::NumberOfCores();
            Config.PerformanceMonitoringInterval = 0.1f;
            // 添加所有優化技術
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::TimeComplexity);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::SpaceComplexity);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::CacheOptimization);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Parallelization);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Memoization);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Approximation);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Heuristic);
            Config.AlgorithmOptimizationTechniques.Add(EOptimizationTechnique::Hybrid);
            break;
    }

    OptimizationManager->SetOptimizationConfiguration(Config);
    
    UE_LOG(LogTemp, Log, TEXT("配置完成 - 模式: %s, 線程: %d, 監控: %.1f秒"), 
           *GetLaunchModeName(Mode), Config.MaxCompilationThreads, Config.PerformanceMonitoringInterval);
}

void UMingRTSOneClickLauncher::StartOptimization()
{
    if (!OptimizationManager.IsValid())
    {
        return;
    }

    bOptimizationActive = true;
    
    // 開始實時監控
    OptimizationManager->StartRealTimeMonitoring();
    
    // 優化當前項目
    FString ProjectPath = FPaths::ProjectDir();
    OptimizationManager->OptimizeProject(ProjectPath);
    
    UE_LOG(LogTemp, Log, TEXT("🚀 優化已開始"));
}

bool UMingRTSOneClickLauncher::IsSystemHealthy()
{
    return GetSystemHealth() >= 0.6f;
}

void UMingRTSOneClickLauncher::UpdateLaunchStatus()
{
    // 更新啟動狀態
    if (OptimizationManager.IsValid())
    {
        bSystemLaunched = OptimizationManager->IsSystemReady();
    }
}

void UMingRTSOneClickLauncher::ShowLaunchSuccess()
{
    UE_LOG(LogTemp, Log, TEXT(""));
    UE_LOG(LogTemp, Log, TEXT("🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉"));
    UE_LOG(LogTemp, Log, TEXT("🚀 MingGoRTS 優化編譯系統啟動成功！"));
    UE_LOG(LogTemp, Log, TEXT("✅ 系統狀態: 活躍"));
    UE_LOG(LogTemp, Log, TEXT("⚡ 優化模式: 自動配置"));
    UE_LOG(LogTemp, Log, TEXT("🔧 並行處理: 已啟用"));
    UE_LOG(LogTemp, Log, TEXT("📊 性能監控: 已啟動"));
    UE_LOG(LogTemp, Log, TEXT("🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯"));
    UE_LOG(LogTemp, Log, TEXT(""));
}

void UMingRTSOneClickLauncher::ShowPerformanceResults()
{
    if (!OptimizationManager.IsValid())
    {
        return;
    }

    FString Report = OptimizationManager->GetOptimizationReport();
    UE_LOG(LogTemp, Log, TEXT("📊 性能報告:"));
    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
}

void UMingRTSOneClickLauncher::DisplaySystemInfo()
{
    UE_LOG(LogTemp, Log, TEXT("🖥️ 系統信息:"));
    UE_LOG(LogTemp, Log, TEXT("  CPU核心: %d"), FPlatformMisc::NumberOfCores());
    UE_LOG(LogTemp, Log, TEXT("  系統內存: %.1f GB"), FPlatformMemory::GetStats().TotalPhysical / (1024.0f * 1024.0f * 1024.0f));
    UE_LOG(LogTemp, Log, TEXT("  項目路徑: %s"), *FPaths::ProjectDir());
    
    if (OptimizationManager.IsValid())
    {
        FOptimizationConfiguration Config = OptimizationManager->GetCurrentConfiguration();
        UE_LOG(LogTemp, Log, TEXT("  智能選擇模式: %s"), *GetLaunchModeName(Config.Mode));
        UE_LOG(LogTemp, Log, TEXT("  配置線程數: %d"), Config.MaxCompilationThreads);
    }
}

EOptimizationMode UMingRTSOneClickLauncher::DetermineBestMode()
{
    // 根據系統資源智能選擇最佳模式
    int32 Cores = FPlatformMisc::NumberOfCores();
    float MemoryGB = FPlatformMemory::GetStats().TotalPhysical / (1024.0f * 1024.0f * 1024.0f);
    
    if (Cores >= 8 && MemoryGB >= 16.0f)
    {
        return EOptimizationMode::Performance; // 高性能系統
    }
    else if (Cores >= 4 && MemoryGB >= 8.0f)
    {
        return EOptimizationMode::Release; // 中等性能系統
    }
    else
    {
        return EOptimizationMode::Development; // 低性能系統
    }
}

FString UMingRTSOneClickLauncher::GetLaunchModeName(EOptimizationMode Mode)
{
    switch (Mode)
    {
        case EOptimizationMode::Development: return TEXT("開發模式");
        case EOptimizationMode::Testing: return TEXT("測試模式");
        case EOptimizationMode::Release: return TEXT("發布模式");
        case EOptimizationMode::Performance: return TEXT("性能模式");
        default: return TEXT("未知模式");
    }
}

FString UMingRTSOneClickLauncher::GetSystemHealthStatus(float Health)
{
    if (Health >= 0.8f) return TEXT("🟢 優秀");
    if (Health >= 0.6f) return TEXT("🟡 良好");
    if (Health >= 0.4f) return TEXT("🟠 一般");
    if (Health >= 0.2f) return TEXT("🔴 較差");
    return TEXT("💀 臨界");
}

FString UMingRTSOneClickLauncher::GenerateLaunchReport()
{
    FString Report = TEXT("🚀 一鍵啟動報告\n\n");
    Report += FString::Printf(TEXT("啟動時間: %s\n"), *LaunchTime.ToString());
    Report += FString::Printf(TEXT("系統狀態: %s\n"), bSystemLaunched ? TEXT("已啟動") : TEXT("未啟動"));
    Report += FString::Printf(TEXT("健康度: %.1f%%\n"), GetSystemHealth() * 100.0f);
    return Report;
}

// 輔助方法

FString UMingRTSOneClickLauncher::GetProfilingTypeName(EProfilingType ProfilingType) const
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

FString UMingRTSOneClickLauncher::GetPerformanceLevelName(EPerformanceLevel Level) const
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

FString UMingRTSOneClickLauncher::GetPerformanceEmoji(EPerformanceLevel Level) const
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
