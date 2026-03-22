#include "MingGoRTSGameMode.h"
#include "MingGoRTSCamera.h"
#include "MingGoRTSPlayerController.h"
#include "MingGoRTSUnit.h"
#include "Kismet/GameplayStatics.h"

// 系統頭文件
#include "RepublicEraFactions/MingRTSFactionManager.h"
#include "History/MingRTSDynamicHistorySystem.h"
#include "AI/MingRTSSelfLearningSystem.h"
#include "AI/MingRTSAILearningIntegration.h"
#include "AI/MingAutoSceneGenerator.h"
#include "AI/MingRTSGameAssetGenerator.h"
#include "Localization/MingRTSLocalizationEnhancedSystem.h"
#include "Performance/MingRTSPerformanceEnhancedSystem.h"
#include "MingPersonalManager.h"
#include "UI/MingRTSUIEnhancedSystem.h"
#include "Audio/MingRTSAudioEnhancedSystem.h"
#include "Save/MingSaveGameManager.h"
#include "UI/MingMainGameUI.h"

AMingGoRTSGameMode::AMingGoRTSGameMode()
{
    // 設置預設的類別
    DefaultPawnClass = AMingGoRTSCamera::StaticClass();
    PlayerControllerClass = AMingGoRTSPlayerController::StaticClass();
    GameStateClass = nullptr;
    
    RTSCamera = nullptr;
    bSystemsInitialized = false;
    LastPerformanceUpdate = 0.0f;
}

void AMingGoRTSGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS GameMode Starting - Initializing Systems..."));
    
    // 初始化所有系統
    if (InitializeAllSystems())
    {
        UE_LOG(LogTemp, Log, TEXT("All systems initialized successfully"));
        
        // 獲取 RTS 相機引用
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            RTSCamera = Cast<AMingGoRTSCamera>(PC->GetPawn());
        }
        
        // 生成初始單位
        SpawnInitialUnits();
        
        UE_LOG(LogTemp, Log, TEXT("MingGoRTS GameMode Started Successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initialize all systems"));
        for (const FString& Error : InitializationErrors)
        {
            UE_LOG(LogTemp, Error, TEXT("Initialization Error: %s"), *Error);
        }
    }
}

bool AMingGoRTSGameMode::InitializeAllSystems()
{
    if (bSystemsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Systems already initialized"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("Starting system initialization..."));
    InitializationErrors.Empty();

    // 按順序初始化系統
    bool bSuccess = true;
    
    // 1. 初始化核心系統
    bSuccess &= InitializeCoreSystems();
    
    // 2. 初始化AI系統
    bSuccess &= InitializeAISystems();
    
    // 3. 初始化內容系統
    bSuccess &= InitializeContentSystems();
    
    // 4. 初始化用戶界面系統
    bSuccess &= InitializeUserInterfaceSystems();
    
    // 5. 設置系統通信
    bSuccess &= SetupSystemCommunication();
    
    // 6. 設置事件綁定
    bSuccess &= SetupEventBindings();

    bSystemsInitialized = bSuccess;
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("All systems initialized successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("System initialization failed with %d errors"), InitializationErrors.Num());
    }

    return bSuccess;
}

bool AMingGoRTSGameMode::InitializeCoreSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Core Systems..."));
    
    bool bSuccess = true;

    // 初始化勢力管理器
    FactionManager = NewObject<UMingRTSFactionManager>(this);
    if (FactionManager)
    {
        FactionManager->InitializeFactionManager();
        UE_LOG(LogTemp, Log, TEXT("Faction Manager initialized"));
    }
    else
    {
        LogSystemError(TEXT("FactionManager"), TEXT("Failed to create FactionManager"));
        bSuccess = false;
    }

    // 初始化個人管理器
    PersonalManager = NewObject<UMingPersonalManager>(this);
    if (PersonalManager)
    {
        PersonalManager->InitializePersonalManager();
        UE_LOG(LogTemp, Log, TEXT("Personal Manager initialized"));
    }
    else
    {
        LogSystemError(TEXT("PersonalManager"), TEXT("Failed to create PersonalManager"));
        bSuccess = false;
    }

    // 初始化保存管理器
    SaveGameManager = NewObject<UMingSaveGameManager>(this);
    if (SaveGameManager)
    {
        SaveGameManager->InitializeSaveGameManager();
        UE_LOG(LogTemp, Log, TEXT("Save Game Manager initialized"));
    }
    else
    {
        LogSystemError(TEXT("SaveGameManager"), TEXT("Failed to create SaveGameManager"));
        bSuccess = false;
    }

    return bSuccess;
}

bool AMingGoRTSGameMode::InitializeAISystems()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing AI Systems..."));
    
    bool bSuccess = true;

    // 初始化自我學習系統
    LearningSystem = NewObject<UMingRTSSelfLearningSystem>(this);
    if (LearningSystem)
    {
        LearningSystem->InitializeSelfLearningSystem();
        UE_LOG(LogTemp, Log, TEXT("Self Learning System initialized"));
    }
    else
    {
        LogSystemError(TEXT("LearningSystem"), TEXT("Failed to create LearningSystem"));
        bSuccess = false;
    }

    // 初始化AI學習集成
    if (LearningSystem)
    {
        AIIntegration = NewObject<UMingRTSAILearningIntegration>(this);
        if (AIIntegration)
        {
            AIIntegration->InitializeAILearning(LearningSystem);
            UE_LOG(LogTemp, Log, TEXT("AI Learning Integration initialized"));
        }
        else
        {
            LogSystemError(TEXT("AIIntegration"), TEXT("Failed to create AIIntegration"));
            bSuccess = false;
        }
    }

    return bSuccess;
}

bool AMingGoRTSGameMode::InitializeContentSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Content Systems..."));
    
    bool bSuccess = true;

    // 初始化歷史系統
    HistorySystem = NewObject<UMingRTSDynamicHistorySystem>(this);
    if (HistorySystem)
    {
        HistorySystem->InitializeDynamicHistorySystem();
        UE_LOG(LogTemp, Log, TEXT("Dynamic History System initialized"));
    }
    else
    {
        LogSystemError(TEXT("HistorySystem"), TEXT("Failed to create HistorySystem"));
        bSuccess = false;
    }

    // 初始化場景生成器
    SceneGenerator = NewObject<UMingAutoSceneGenerator>(this);
    if (SceneGenerator)
    {
        SceneGenerator->InitializeAutoSceneGenerator();
        UE_LOG(LogTemp, Log, TEXT("Auto Scene Generator initialized"));
    }
    else
    {
        LogSystemError(TEXT("SceneGenerator"), TEXT("Failed to create SceneGenerator"));
        bSuccess = false;
    }

    // 初始化資產生成器
    AssetGenerator = NewObject<UMingRTSGameAssetGenerator>(this);
    if (AssetGenerator)
    {
        AssetGenerator->InitializeGameAssetGenerator();
        UE_LOG(LogTemp, Log, TEXT("Game Asset Generator initialized"));
    }
    else
    {
        LogSystemError(TEXT("AssetGenerator"), TEXT("Failed to create AssetGenerator"));
        bSuccess = false;
    }

    // 初始化本地化系統
    LocalizationSystem = NewObject<UMingRTSLocalizationEnhancedSystem>(this);
    if (LocalizationSystem)
    {
        LocalizationSystem->InitializeLocalizationSystem();
        UE_LOG(LogTemp, Log, TEXT("Localization System initialized"));
    }
    else
    {
        LogSystemError(TEXT("LocalizationSystem"), TEXT("Failed to create LocalizationSystem"));
        bSuccess = false;
    }

    return bSuccess;
}

bool AMingGoRTSGameMode::InitializeUserInterfaceSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing UI Systems..."));
    
    bool bSuccess = true;

    // 初始化UI系統
    UISystem = NewObject<UMingRTSUIEnhancedSystem>(this);
    if (UISystem)
    {
        UISystem->InitializeUIEnhancedSystem();
        UE_LOG(LogTemp, Log, TEXT("UI Enhanced System initialized"));
    }
    else
    {
        LogSystemError(TEXT("UISystem"), TEXT("Failed to create UISystem"));
        bSuccess = false;
    }

    // 初始化音頻系統
    AudioSystem = NewObject<UMingRTSAudioEnhancedSystem>(this);
    if (AudioSystem)
    {
        AudioSystem->InitializeAudioEnhancedSystem();
        UE_LOG(LogTemp, Log, TEXT("Audio Enhanced System initialized"));
    }
    else
    {
        LogSystemError(TEXT("AudioSystem"), TEXT("Failed to create AudioSystem"));
        bSuccess = false;
    }

    // 初始化性能系統
    PerformanceSystem = NewObject<UMingRTSPerformanceEnhancedSystem>(this);
    if (PerformanceSystem)
    {
        PerformanceSystem->InitializePerformanceEnhancedSystem();
        UE_LOG(LogTemp, Log, TEXT("Performance Enhanced System initialized"));
    }
    else
    {
        LogSystemError(TEXT("PerformanceSystem"), TEXT("Failed to create PerformanceSystem"));
        bSuccess = false;
    }

    // 創建主UI
    MainGameUI = CreateWidget<UMingMainGameUI>(GetWorld(), UMingMainGameUI::StaticClass());
    if (MainGameUI)
    {
        MainGameUI->AddToViewport();
        UE_LOG(LogTemp, Log, TEXT("Main Game UI created and added to viewport"));
    }
    else
    {
        LogSystemError(TEXT("MainGameUI"), TEXT("Failed to create MainGameUI"));
        bSuccess = false;
    }

    return bSuccess;
}

bool AMingGoRTSGameMode::SetupSystemCommunication()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up System Communication..."));
    
    SetupSystemCommunicationChannels();
    
    // 這裡可以設置系統間的通信接口
    // 例如：學習系統與AI系統的通信，歷史系統與勢力系統的通信等
    
    return true;
}

bool AMingGoRTSGameMode::SetupEventBindings()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up Event Bindings..."));
    
    // 綁定系統間事件
    if (HistorySystem && LearningSystem)
    {
        // 歷史事件觸發學習
        HistorySystem->OnHistoricalEventGenerated.AddDynamic(
            LearningSystem, &UMingRTSSelfLearningSystem::OnHistoricalEventOccurred);
    }
    
    if (FactionManager && HistorySystem)
    {
        // 勢力變化觸發歷史事件
        FactionManager->OnFactionRelationshipChanged.AddDynamic(
            HistorySystem, &UMingRTSDynamicHistorySystem::OnFactionDiplomacyChanged);
    }
    
    return true;
}

void AMingGoRTSGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bSystemsInitialized && bEnablePerformanceMonitoring)
    {
        UpdatePerformanceMetrics(DeltaTime);
    }
}

void AMingGoRTSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS GameMode Ending - Cleaning up systems..."));
    CleanupAllSystems();
}

bool AMingGoRTSGameMode::AreAllSystemsInitialized() const
{
    return bSystemsInitialized;
}

FString AMingGoRTSGameMode::GetSystemPerformanceReport() const
{
    FString Report = TEXT("=== MingGoRTS System Performance Report ===\n");
    
    for (const auto& Metric : SystemPerformanceMetrics)
    {
        Report += FString::Printf(TEXT("%s: %.2f ms\n"), *Metric.Key, Metric.Value);
    }
    
    return Report;
}

void AMingGoRTSGameMode::SpawnInitialUnits()
{
    // 生成一些測試單位
    for (int32 i = 0; i < 5; i++)
    {
        FVector SpawnLocation = FVector(i * 200.0f, 0.0f, 100.0f);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        
        if (AMingGoRTSUnit* NewUnit = GetWorld()->SpawnActor<AMingGoRTSUnit>(AMingGoRTSUnit::StaticClass(), SpawnLocation, SpawnRotation))
        {
            GameUnits.Add(NewUnit);
            UE_LOG(LogTemp, Log, TEXT("Spawned unit at location: %s"), *SpawnLocation.ToString());
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Spawned %d initial units"), GameUnits.Num());
}

void AMingGoRTSGameMode::CleanupAllSystems()
{
    UE_LOG(LogTemp, Log, TEXT("Cleaning up all systems..."));
    
    // 清理所有系統
    if (MainGameUI)
    {
        MainGameUI->RemoveFromParent();
        MainGameUI = nullptr;
    }
    
    // 其他系統會由UE的垃圾回收機制自動清理
    bSystemsInitialized = false;
}

void AMingGoRTSGameMode::UpdatePerformanceMetrics(float DeltaTime)
{
    LastPerformanceUpdate += DeltaTime;
    
    if (LastPerformanceUpdate >= PerformanceUpdateInterval)
    {
        // 更新性能指標
        if (PerformanceSystem)
        {
            // 這裡可以從性能系統獲取實時數據
            SystemPerformanceMetrics.Add(TEXT("FrameTime"), DeltaTime * 1000.0f);
        }
        
        LastPerformanceUpdate = 0.0f;
    }
}

void AMingGoRTSGameMode::SetupSystemCommunicationChannels()
{
    UE_LOG(LogTemp, Log, TEXT("Setting up communication channels between systems..."));
    
    // 實現系統間通信邏輯
}

void AMingGoRTSGameMode::BroadcastSystemEvent(const FString& EventName, UObject* EventData)
{
    UE_LOG(LogTemp, Log, TEXT("Broadcasting system event: %s"), *EventName);
    
    // 實現事件廣播邏輯
}

void AMingGoRTSGameMode::LogSystemError(const FString& SystemName, const FString& ErrorMessage)
{
    FString FullError = FString::Printf(TEXT("[%s] %s"), *SystemName, *ErrorMessage);
    InitializationErrors.Add(FullError);
    UE_LOG(LogTemp, Error, TEXT("System Error: %s"), *FullError);
}

void AMingGoRTSGameMode::HandleSystemInitializationFailure(const FString& SystemName)
{
    UE_LOG(LogTemp, Error, TEXT("System initialization failed: %s"), *SystemName);
    
    // 實現錯誤處理邏輯，可能包括：
    // 1. 嘗試重新初始化
    // 2. 降級到備用系統
    // 3. 通知用戶
}
