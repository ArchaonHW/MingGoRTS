#include "Examples/MingGoRTSExampleGameMode.h"

// 包含所有系統頭文件
#include "Tutorial/MingRTSTutorialSystem.h"
#include "Security/MingRTSSecurityManager.h"
#include "Replay/MingRTSReplaySystem.h"
#include "Replay/MingRTSReplayManager.h"
#include "Stats/MingRTSAchievementSystem.h"
#include "Stats/MingRTSStatisticsSystem.h"
#include "Spectator/MingRTSSpectatorSystem.h"
#include "Telemetry/MingRTSTelemetrySystem.h"
#include "Mod/MingRTSModSystem.h"
#include "Tournament/MingRTSTournamentSystem.h"
#include "Cloud/MingRTSCloudSystem.h"
#include "XR/MingRTSVRSupportSystem.h"
#include "Multiplayer/MingRTSMultiplayerSupportSystem.h"
#include "Performance/MingRTSPerformanceSystem.h"

AMingGoRTSExampleGameMode::AMingGoRTSExampleGameMode()
{
    // 設置默認值
    PrimaryActorTick.bCanEverTick = true;
}

void AMingGoRTSExampleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    
    UE_LOG(LogTemp, Log, TEXT("=== MingGoRTS Example GameMode Initializing ==="));
    
    // 初始化所有系統
    InitializeAllSystems();
}

void AMingGoRTSExampleGameMode::StartPlay()
{
    Super::StartPlay();
    
    UE_LOG(LogTemp, Log, TEXT("=== MingGoRTS Example GameMode Starting ==="));
    
    // 綁定事件
    BindSystemEvents();
    
    // 設置示例功能
    if (bEnableTutorialSystem && TutorialSystem)
    {
        SetupTutorial();
    }
    
    if (bEnableReplaySystem && ReplaySystem)
    {
        SetupReplayRecording();
    }
    
    if (bEnableTelemetrySystem && TelemetrySystem)
    {
        SetupTelemetry();
    }
    
    if (bEnablePerformanceSystem && PerformanceSystem)
    {
        SetupPerformanceMonitoring();
    }
}

void AMingGoRTSExampleGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogTemp, Log, TEXT("=== MingGoRTS Example GameMode Ending ==="));
    
    // 關閉所有系統
    ShutdownAllSystems();
    
    Super::EndPlay(EndPlayReason);
}

void AMingGoRTSExampleGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 更新性能監控
    if (PerformanceSystem)
    {
        // 每幀記錄性能數據
        FPerformanceMetrics Metrics;
        // 這裡應該填充實際的性能數據
        // TelemetrySystem->RecordPerformanceMetrics(Metrics);
    }
}

void AMingGoRTSExampleGameMode::InitializeAllSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing all MingGoRTS systems..."));
    
    // 1. 性能系統（最先初始化）
    if (bEnablePerformanceSystem)
    {
        PerformanceSystem = NewObject<UMingRTSPerformanceSystem>(this);
        PerformanceSystem->InitializePerformanceSystem();
        UE_LOG(LogTemp, Log, TEXT("✓ Performance System initialized"));
    }
    
    // 2. 安全系統
    if (bEnableSecuritySystem)
    {
        SecuritySystem = NewObject<UMingRTSSecurityManager>(this);
        SecuritySystem->InitializeSecuritySystem();
        SecuritySystem->StartAntiCheatSystem();
        UE_LOG(LogTemp, Log, TEXT("✓ Security System initialized"));
    }
    
    // 3. 教學系統
    if (bEnableTutorialSystem)
    {
        TutorialSystem = NewObject<UMingRTSTutorialSystem>(this);
        TutorialSystem->InitializeTutorialSystem();
        UE_LOG(LogTemp, Log, TEXT("✓ Tutorial System initialized"));
    }
    
    // 4. 回放系統
    if (bEnableReplaySystem)
    {
        ReplaySystem = NewObject<UMingRTSReplaySystem>(this);
        ReplaySystem->InitializeReplaySystem();
        
        ReplayManager = NewObject<UMingRTSReplayManager>(this);
        UE_LOG(LogTemp, Log, TEXT("✓ Replay System initialized"));
    }
    
    // 5. 成就統計系統
    AchievementSystem = NewObject<UMingRTSAchievementSystem>(this);
    AchievementSystem->InitializeAchievementSystem();
    
    StatisticsSystem = NewObject<UMingRTSStatisticsSystem>(this);
    StatisticsSystem->InitializeStatisticsSystem();
    UE_LOG(LogTemp, Log, TEXT("✓ Achievement & Statistics System initialized"));
    
    // 6. 觀戰系統
    SpectatorSystem = NewObject<UMingRTSSpectatorSystem>(this);
    SpectatorSystem->InitializeSpectatorSystem();
    UE_LOG(LogTemp, Log, TEXT("✓ Spectator System initialized"));
    
    // 7. 遙測系統
    if (bEnableTelemetrySystem)
    {
        TelemetrySystem = NewObject<UMingRTSTelemetrySystem>(this);
        TelemetrySystem->InitializeTelemetrySystem();
        UE_LOG(LogTemp, Log, TEXT("✓ Telemetry System initialized"));
    }
    
    // 8. 模組系統
    ModSystem = NewObject<UMingRTSModSystem>(this);
    ModSystem->InitializeModSystem();
    ModSystem->LoadEnabledMods();
    UE_LOG(LogTemp, Log, TEXT("✓ Mod System initialized"));
    
    // 9. 錦標賽系統
    TournamentSystem = NewObject<UMingRTSTournamentSystem>(this);
    TournamentSystem->InitializeTournamentSystem();
    UE_LOG(LogTemp, Log, TEXT("✓ Tournament System initialized"));
    
    // 10. 雲端系統
    CloudSystem = NewObject<UMingRTSCloudSystem>(this);
    CloudSystem->InitializeCloudSystem();
    UE_LOG(LogTemp, Log, TEXT("✓ Cloud System initialized"));
    
    // 11. VR/AR 支持系統
    VRSupportSystem = NewObject<UMingRTSVRSupportSystem>(this);
    VRSupportSystem->InitializeXRSupport();
    UE_LOG(LogTemp, Log, TEXT("✓ VR/AR Support System initialized"));
    
    // 12. 多人遊戲系統
    MultiplayerSystem = NewObject<UMingRTSMultiplayerSupportSystem>(this);
    MultiplayerSystem->InitializeMultiplayerSystem();
    UE_LOG(LogTemp, Log, TEXT("✓ Multiplayer System initialized"));
    
    UE_LOG(LogTemp, Log, TEXT("=== All systems initialized successfully ==="));
}

void AMingGoRTSExampleGameMode::ShutdownAllSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down all MingGoRTS systems..."));
    
    // 停止性能監控
    if (PerformanceSystem)
    {
        PerformanceSystem->StopPerformanceMonitoring();
    }
    
    // 結束遙測會話
    if (TelemetrySystem)
    {
        TelemetrySystem->EndSession();
    }
    
    // 停止回放錄製
    if (ReplaySystem)
    {
        if (ReplaySystem->IsRecording())
        {
            ReplaySystem->StopRecording();
        }
    }
    
    // 斷開多人連接
    if (MultiplayerSystem)
    {
        MultiplayerSystem->Disconnect();
    }
    
    // 斷開雲端
    if (CloudSystem)
    {
        CloudSystem->DisconnectFromCloud();
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== All systems shutdown ==="));
}

void AMingGoRTSExampleGameMode::BindSystemEvents()
{
    UE_LOG(LogTemp, Log, TEXT("Binding system events..."));
    
    // 綁定教學事件
    if (TutorialSystem)
    {
        TutorialSystem->OnTutorialStepCompleted.AddDynamic(this, &AMingGoRTSExampleGameMode::OnTutorialStepCompleted);
        TutorialSystem->OnTutorialCompleted.AddDynamic(this, &AMingGoRTSExampleGameMode::OnTutorialStepCompleted);
    }
    
    // 綁定成就事件
    if (AchievementSystem)
    {
        AchievementSystem->OnAchievementUnlocked.AddDynamic(this, &AMingGoRTSExampleGameMode::OnAchievementUnlocked);
    }
    
    // 綁定錦標賽事件
    if (TournamentSystem)
    {
        TournamentSystem->OnPlayerRegistered.AddDynamic(this, &AMingGoRTSExampleGameMode::OnPlayerRegistered);
    }
    
    // 綁定性能事件
    if (PerformanceSystem)
    {
        PerformanceSystem->OnPerformanceLevelChanged.AddDynamic(this, &AMingGoRTSExampleGameMode::OnPerformanceLevelChanged);
    }
    
    UE_LOG(LogTemp, Log, TEXT("✓ System events bound"));
}

void AMingGoRTSExampleGameMode::SetupTutorial()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up tutorial..."));
    
    // 註冊教學步驟
    TMap<FString, FString> Params1;
    Params1.Add(TEXT("unit_type"), TEXT("infantry"));
    TutorialSystem->RegisterTutorialStep(
        TEXT("TUTORIAL_SELECT_UNIT"),
        TEXT("選擇一個步兵單位"),
        TEXT("SelectUnit"),
        Params1
    );
    
    TMap<FString, FString> Params2;
    Params2.Add(TEXT("min_distance"), TEXT("100"));
    TutorialSystem->RegisterTutorialStep(
        TEXT("TUTORIAL_MOVE_UNIT"),
        TEXT("移動單位到指定位置"),
        TEXT("MoveUnit"),
        Params2
    );
    
    TMap<FString, FString> Params3;
    Params3.Add(TEXT("target_count"), TEXT("1"));
    TutorialSystem->RegisterTutorialStep(
        TEXT("TUTORIAL_ATTACK"),
        TEXT("攻擊敵人"),
        TEXT("KillEnemy"),
        Params3
    );
    
    // 啟動基礎教學序列
    TutorialSystem->StartTutorialSequence(TEXT("TUTORIAL_BASIC"));
}

void AMingGoRTSExampleGameMode::SetupReplayRecording()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up replay recording..."));
    
    // 生成比賽ID
    FString MatchID = FString::Printf(TEXT("MATCH_%d"), FMath::Rand());
    
    // 開始錄製
    ReplaySystem->StartRecording(MatchID);
    
    UE_LOG(LogTemp, Log, TEXT("Replay recording started for match: %s"), *MatchID);
}

void AMingGoRTSExampleGameMode::SetupTelemetry()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up telemetry..."));
    
    // 開始玩家會話
    TelemetrySystem->StartSession(TEXT("Player_001"));
    
    // 記錄遊戲開始事件
    TMap<FString, FString> Params;
    Params.Add(TEXT("map"), TEXT("ExampleMap"));
    Params.Add(TEXT("mode"), TEXT("Skirmish"));
    TelemetrySystem->RecordEvent(ETelemetryEventType::MatchStart, TEXT("game_start"), Params);
}

void AMingGoRTSExampleGameMode::SetupPerformanceMonitoring()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up performance monitoring..."));
    
    // 設置性能預算
    FPerformanceBudget Budget;
    Budget.TargetFPS = 60;
    Budget.MinFPS = 30;
    Budget.MaxMemoryUsageMB = 4096.0f;
    PerformanceSystem->SetPerformanceBudget(Budget);
    
    // 開始監控
    PerformanceSystem->StartPerformanceMonitoring();
    
    // 應用中等質量預設
    PerformanceSystem->ApplyOptimizationPreset(EOptimizationPreset::Medium);
}

// 事件處理函數

void AMingGoRTSExampleGameMode::OnTutorialStepCompleted(const FString& StepID)
{
    UE_LOG(LogTemp, Log, TEXT("Tutorial step completed: %s"), *StepID);
    
    // 解鎖成就
    if (AchievementSystem)
    {
        if (StepID == TEXT("TUTORIAL_BASIC"))
        {
            AchievementSystem->UnlockAchievement(TEXT("ACH_TUTORIAL_COMPLETE"));
        }
    }
}

void AMingGoRTSExampleGameMode::OnAchievementUnlocked(const FString& AchievementID, const FAchievementData& Data)
{
    UE_LOG(LogTemp, Log, TEXT("Achievement unlocked: %s - %s"), *AchievementID, *Data.Name.ToString());
    
    // 記錄遙測事件
    if (TelemetrySystem)
    {
        TMap<FString, FString> Params;
        Params.Add(TEXT("achievement_id"), AchievementID);
        Params.Add(TEXT("achievement_name"), Data.Name.ToString());
        TelemetrySystem->RecordEvent(ETelemetryEventType::FeatureUsage, TEXT("achievement_unlocked"), Params);
    }
}

void AMingGoRTSExampleGameMode::OnPlayerRegistered(const FString& TournamentID, const FString& PlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Player %s registered for tournament %s"), *PlayerID, *TournamentID);
}

void AMingGoRTSExampleGameMode::OnPerformanceLevelChanged(EPerformanceLevel NewLevel)
{
    UE_LOG(LogTemp, Log, TEXT("Performance level changed to: %d"), (int32)NewLevel);
    
    // 如果性能變差，自動優化
    if (NewLevel == EPerformanceLevel::Poor || NewLevel == EPerformanceLevel::Critical)
    {
        if (PerformanceSystem)
        {
            PerformanceSystem->AutoOptimize();
        }
    }
}
