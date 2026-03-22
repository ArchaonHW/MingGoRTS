#include "MingPersonalManager.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
#include "MingAIUIManager.h"
#include "Save/MingSaveGameManager.h"
#include "MingPerformanceManager.h"
#include "MingMemoryOptimizer.h"
#include "Network/MingNetworkManager.h"
#include "Network/MingLobbySystem.h"
#include "MingLocalizationManager.h"
#include "MingUniversityGuideManager.h"

UMingPersonalManager::UMingPersonalManager()
    : bIsInitialized(false)
    , CurrentLevel(1)
    , CurrentExperience(0)
    , ExperienceToNextLevel(100)
{
}

void UMingPersonalManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 初始化關係和聲望管理器
    RelationshipManager = NewObject<UMingRelationshipManager>(this);
    if (RelationshipManager)
    {
        RelationshipManager->Initialize();
    }

    // 初始化音頻關係管理器
    AudioRelationshipManager = NewObject<UMingAudioRelationshipManager>(this);
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->InitializeAudioSystem(nullptr);
    }

    // 初始化AI UI管理器
    AIUIManager = NewObject<UMingAIUIManager>(this);
    if (AIUIManager)
    {
        AIUIManager->Initialize();
    }

    // 初始化保存遊戲管理器
    SaveGameManager = NewObject<UMingSaveGameManager>(this);
    if (SaveGameManager)
    {
        SaveGameManager->Initialize(this);
    }

    // 初始化網絡管理器
    NetworkManager = NewObject<UMingNetworkManager>(this);
    if (NetworkManager)
    {
        NetworkManager->InitializeNetwork();
    }

    // 初始化大廳系統
    LobbySystem = NewObject<UMingLobbySystem>(this);
    if (LobbySystem)
    {
        LobbySystem->InitializeLobbySystem();
    }

    // 初始化本地化系統
    LocalizationManager = NewObject<UMingLocalizationManager>(this);
    if (LocalizationManager)
    {
        LocalizationManager->InitializeLocalization(this);
    }

    // 初始化性能優化管理器
    PerformanceManager = NewObject<UMingPerformanceManager>(this);
    if (PerformanceManager)
    {
        PerformanceManager->Initialize();
    }

    // 初始化內存優化器
    MemoryOptimizer = NewObject<UMingMemoryOptimizer>(this);
    if (MemoryOptimizer)
    {
        MemoryOptimizer->Initialize();
    }

    // 初始化高校引導管理器（延遲創建，按需啟動）
    // UniversityGuideManager 將在 StartUniversityGuide 時創建

    SetupEventSubscriptions();
    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("MingPersonalManager initialized - All systems ready"));
}

void UMingPersonalManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 關閉高校引導管理器
    if (UniversityGuideManager)
    {
        UniversityGuideManager->Shutdown();
        UniversityGuideManager = nullptr;
    }

    // 關閉本地化系統
    if (LocalizationManager)
    {
        LocalizationManager->Shutdown();
        LocalizationManager = nullptr;
    }

    // 關閉性能優化管理器
    if (PerformanceManager)
    {
        PerformanceManager->Shutdown();
        PerformanceManager = nullptr;
    }

    // 關閉內存優化器
    if (MemoryOptimizer)
    {
        MemoryOptimizer->Shutdown();
        MemoryOptimizer = nullptr;
    }

    // 關閉大廳系統
    if (LobbySystem)
    {
        LobbySystem->ShutdownLobbySystem();
        LobbySystem = nullptr;
    }

    // 關閉網絡管理器
    if (NetworkManager)
    {
        NetworkManager->ShutdownNetwork();
        NetworkManager = nullptr;
    }

    // 關閉保存遊戲管理器
    if (SaveGameManager)
    {
        SaveGameManager->Shutdown();
        SaveGameManager = nullptr;
    }

    // 關閉AI UI管理器
    if (AIUIManager)
    {
        AIUIManager->Shutdown();
        AIUIManager = nullptr;
    }

    // 關閉音頻關係管理器
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->ShutdownAudioSystem();
        AudioRelationshipManager = nullptr;
    }

    // 關閉關係和聲望管理器
    if (RelationshipManager)
    {
        RelationshipManager->Shutdown();
        RelationshipManager = nullptr;
    }

    CleanupEventSubscriptions();
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("MingPersonalManager shutdown"));
}

void UMingPersonalManager::SetupEventSubscriptions()
{
    // 設置事件訂閱
    UE_LOG(LogTemp, Log, TEXT("Event subscriptions setup"));
}

void UMingPersonalManager::CleanupEventSubscriptions()
{
    // 清理事件訂閱
    UE_LOG(LogTemp, Log, TEXT("Event subscriptions cleaned up"));
}

// 角色成長接口
void UMingPersonalManager::AddExperience(int32 Amount)
{
    CurrentExperience += Amount;
    OnExperienceGained(Amount);

    // 檢查升級
    while (CurrentExperience >= ExperienceToNextLevel)
    {
        CurrentExperience -= ExperienceToNextLevel;
        LevelUp();
    }
}

void UMingPersonalManager::LevelUp()
{
    CurrentLevel++;
    ExperienceToNextLevel = FMath::FloorToInt(ExperienceToNextLevel * 1.5f);
    OnLevelUp();
}

void UMingPersonalManager::OnExperienceGained(int32 Amount)
{
    UE_LOG(LogTemp, Log, TEXT("Gained %d experience"), Amount);
}

void UMingPersonalManager::OnLevelUp()
{
    UE_LOG(LogTemp, Log, TEXT("Leveled up to %d"), CurrentLevel);
}

// 敘事事件
void UMingPersonalManager::TriggerDialogue(const FString& DialogueId)
{
    UE_LOG(LogTemp, Log, TEXT("Triggering dialogue: %s"), *DialogueId);
}

// 關係和聲望系統接口
void UMingPersonalManager::UpdateCharacterRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason)
{
    if (RelationshipManager)
    {
        FRelationshipUpdateRequest Request;
        Request.CharacterID = CharacterID;
        Request.RelationshipChange = ChangeAmount;
        Request.UpdateReason = Reason;
        RelationshipManager->UpdateRelationship(Request);
    }
}

void UMingPersonalManager::UpdateRegionReputation(const FString& RegionID, float ChangeAmount, const FString& Reason)
{
    if (RelationshipManager)
    {
        FReputationUpdateRequest Request;
        Request.RegionID = RegionID;
        Request.ReputationChange = ChangeAmount;
        Request.UpdateReason = Reason;
        RelationshipManager->UpdateReputation(Request);
    }
}

FRelationshipData UMingPersonalManager::GetCharacterRelationship(const FString& CharacterID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->GetRelationshipData(CharacterID);
    }
    return FRelationshipData();
}

FReputationData UMingPersonalManager::GetRegionReputation(const FString& RegionID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->GetReputationData(RegionID);
    }
    return FReputationData();
}

TArray<FString> UMingPersonalManager::GetAvailableDialogueOptions(const FString& CharacterID) const
{
    TArray<FString> Options;
    if (RelationshipManager)
    {
        FRelationshipData Data = RelationshipManager->GetRelationshipData(CharacterID);
        // 根據關係值返回不同的對話選項
        if (Data.RelationshipValue >= 50.0f)
        {
            Options.Add(TEXT("friendly_greeting"));
            Options.Add(TEXT("trade_offer"));
            Options.Add(TEXT("quest_request"));
        }
        else if (Data.RelationshipValue >= 0.0f)
        {
            Options.Add(TEXT("neutral_greeting"));
            Options.Add(TEXT("basic_trade"));
        }
        else
        {
            Options.Add(TEXT("hostile_greeting"));
        }
    }
    return Options;
}

float UMingPersonalManager::GetTradePriceModifier(const FString& CharacterID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->GetTradePriceModifier(CharacterID);
    }
    return 1.0f;
}

bool UMingPersonalManager::CanAcceptQuestByReputation(const FString& QuestID, const FString& RegionID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->CanAcceptQuest(QuestID, RegionID);
    }
    return false;
}

void UMingPersonalManager::OnQuestCompleted(const FString& QuestID, const FString& RegionID, const FString& QuestGiverID)
{
    // 更新聲望
    UpdateRegionReputation(RegionID, 20.0f, FString::Printf(TEXT("Completed quest %s"), *QuestID));
    // 更新關係
    UpdateCharacterRelationship(QuestGiverID, 15.0f, FString::Printf(TEXT("Completed quest %s"), *QuestID));
    // 獲得經驗
    AddExperience(100);

    UE_LOG(LogTemp, Log, TEXT("Quest completed: %s"), *QuestID);
}

void UMingPersonalManager::OnNPCInteraction(const FString& CharacterID, const FString& InteractionType)
{
    // 播放對話音頻
    PlayDialogueAudio(CharacterID, InteractionType);

    UE_LOG(LogTemp, Log, TEXT("NPC interaction: %s - %s"), *CharacterID, *InteractionType);
}

// 音頻系統接口
void UMingPersonalManager::InitializeAudioSystem(UObject* MetaSoundsSystem)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->InitializeAudioSystem(MetaSoundsSystem);
    }
}

void UMingPersonalManager::PlayRelationshipAudio(const FString& CharacterID, const FString& AudioType)
{
    if (AudioRelationshipManager)
    {
        FRelationshipData Data = GetCharacterRelationship(CharacterID);
        ERelationshipAudioType Type = static_cast<ERelationshipAudioType>(FMath::RandRange(0, 4));
        AudioRelationshipManager->PlayRelationshipSound(Type, Data.CurrentType);
    }
}

void UMingPersonalManager::PlayReputationAudio(const FString& RegionID, const FString& AudioType)
{
    if (AudioRelationshipManager)
    {
        FReputationData Data = GetRegionReputation(RegionID);
        EReputationLevel Level = static_cast<EReputationLevel>(FMath::Clamp(FMath::FloorToInt(Data.ReputationValue / 20.0f), 0, 4));
        AudioRelationshipManager->PlayReputationSound(Level);
    }
}

void UMingPersonalManager::PlayDialogueAudio(const FString& CharacterID, const FString& DialogueType)
{
    if (AudioRelationshipManager)
    {
        EDialogueAudioType Type = EDialogueAudioType::Neutral;
        if (DialogueType == TEXT("friendly"))
        {
            Type = EDialogueAudioType::Friendly;
        }
        else if (DialogueType == TEXT("hostile"))
        {
            Type = EDialogueAudioType::Hostile;
        }

        FRelationshipData Data = GetCharacterRelationship(CharacterID);
        AudioRelationshipManager->PlayDialogueSound(Type, Data.CurrentType);
    }
}

void UMingPersonalManager::PlayRepublicEraTheme(const FString& ThemeName, float Intensity)
{
    if (AudioRelationshipManager)
    {
        ERepublicEraAudioTheme Theme = ERepublicEraAudioTheme::BattlefieldGlory;

        if (ThemeName == TEXT("ScholarlyContemplation"))
            Theme = ERepublicEraAudioTheme::ScholarlyContemplation;
        else if (ThemeName == TEXT("RevolutionaryPassion"))
            Theme = ERepublicEraAudioTheme::RevolutionaryPassion;
        else if (ThemeName == TEXT("UrbanTransformation"))
            Theme = ERepublicEraAudioTheme::UrbanTransformation;
        else if (ThemeName == TEXT("RuralSerenity"))
            Theme = ERepublicEraAudioTheme::RuralSerenity;
        else if (ThemeName == TEXT("DiplomaticIntrigue"))
            Theme = ERepublicEraAudioTheme::DiplomaticIntrigue;
        else if (ThemeName == TEXT("HeroicSacrifice"))
            Theme = ERepublicEraAudioTheme::HeroicSacrifice;
        else if (ThemeName == TEXT("CulturalRenaissance"))
            Theme = ERepublicEraAudioTheme::CulturalRenaissance;
        else if (ThemeName == TEXT("IndustrialAwakening"))
            Theme = ERepublicEraAudioTheme::IndustrialAwakening;
        else if (ThemeName == TEXT("NationBuilding"))
            Theme = ERepublicEraAudioTheme::NationBuilding;

        AudioRelationshipManager->PlayRepublicEraTheme(Theme, Intensity);
    }
}

void UMingPersonalManager::SetAudioVolume(float RelationshipVolume, float ReputationVolume, float DialogueVolume)
{
    if (AudioRelationshipManager)
    {
        FAudioVolumeSettings Settings;
        Settings.RelationshipVolume = RelationshipVolume;
        Settings.ReputationVolume = ReputationVolume;
        Settings.DialogueVolume = DialogueVolume;
        AudioRelationshipManager->SetVolumeSettings(Settings);
    }
}

// AI UI系統接口
void UMingPersonalManager::InitializeAIUISystem()
{
    if (AIUIManager)
    {
        AIUIManager->Initialize();
    }
}

void UMingPersonalManager::TrackUserBehavior(const FString& BehaviorType, const FString& Context, float Value)
{
    if (AIUIManager)
    {
        AIUIManager->TrackUserBehavior(BehaviorType, Context, Value);
    }
}

void UMingPersonalManager::OptimizeUIForUser()
{
    if (AIUIManager)
    {
        AIUIManager->OptimizeUIForUser();
    }
}

void UMingPersonalManager::ShowContextualHelp(const FString& Context)
{
    if (AIUIManager)
    {
        AIUIManager->ShowContextualHelp(Context);
    }
}

TArray<FString> UMingPersonalManager::GetAIRecommendations()
{
    TArray<FString> Recommendations;
    if (AIUIManager)
    {
        Recommendations = AIUIManager->GetAIRecommendations();
    }
    return Recommendations;
}

// 保存和載入系統接口
void UMingPersonalManager::InitializeSaveSystem()
{
    if (SaveGameManager)
    {
        SaveGameManager->Initialize(this);
    }
}

EMingSaveGameResult UMingPersonalManager::SaveGame(int32 SlotIndex, const FString& SaveName)
{
    if (SaveGameManager)
    {
        return SaveGameManager->SaveGame(SlotIndex, SaveName);
    }
    return EMingSaveGameResult::Error_SystemNotInitialized;
}

EMingSaveGameResult UMingPersonalManager::QuickSave()
{
    if (SaveGameManager)
    {
        return SaveGameManager->QuickSave();
    }
    return EMingSaveGameResult::Error_SystemNotInitialized;
}

EMingSaveGameResult UMingPersonalManager::LoadGame(int32 SlotIndex)
{
    if (SaveGameManager)
    {
        return SaveGameManager->LoadGame(SlotIndex);
    }
    return EMingSaveGameResult::Error_SystemNotInitialized;
}

EMingSaveGameResult UMingPersonalManager::QuickLoad()
{
    if (SaveGameManager)
    {
        return SaveGameManager->QuickLoad();
    }
    return EMingSaveGameResult::Error_SystemNotInitialized;
}

bool UMingPersonalManager::DeleteSaveGame(int32 SlotIndex)
{
    if (SaveGameManager)
    {
        return SaveGameManager->DeleteSaveGame(SlotIndex);
    }
    return false;
}

void UMingPersonalManager::SetAutoSaveEnabled(bool bEnabled)
{
    if (SaveGameManager)
    {
        SaveGameManager->SetAutoSaveEnabled(bEnabled);
    }
}

bool UMingPersonalManager::IsAutoSaveEnabled() const
{
    if (SaveGameManager)
    {
        return SaveGameManager->IsAutoSaveEnabled();
    }
    return false;
}

UMingSaveGameManager* UMingPersonalManager::GetSaveGameManager() const
{
    return SaveGameManager;
}

// 多人遊戲系統接口
void UMingPersonalManager::InitializeMultiplayerSystem()
{
    if (NetworkManager)
    {
        NetworkManager->InitializeNetwork();
    }

    if (LobbySystem)
    {
        LobbySystem->InitializeLobbySystem();
    }
}

bool UMingPersonalManager::CreateMultiplayerServer(const FMingNetworkConfig& Config)
{
    if (NetworkManager)
    {
        return NetworkManager->CreateServer(Config);
    }
    return false;
}

bool UMingPersonalManager::ConnectToMultiplayerServer(const FString& ServerAddress, int32 Port)
{
    if (NetworkManager)
    {
        return NetworkManager->ConnectToServer(ServerAddress, Port);
    }
    return false;
}

void UMingPersonalManager::DisconnectFromMultiplayerServer()
{
    if (NetworkManager)
    {
        NetworkManager->DisconnectFromServer();
    }
}

bool UMingPersonalManager::IsMultiplayerConnected() const
{
    if (NetworkManager)
    {
        return NetworkManager->IsConnected();
    }
    return false;
}

bool UMingPersonalManager::IsMultiplayerHost() const
{
    if (NetworkManager)
    {
        return NetworkManager->IsHost();
    }
    return false;
}

int32 UMingPersonalManager::GetLocalPlayerID() const
{
    if (NetworkManager)
    {
        return NetworkManager->GetLocalPlayerID();
    }
    return -1;
}

void UMingPersonalManager::SetPlayerReady(bool bReady)
{
    if (LobbySystem)
    {
        LobbySystem->SetPlayerReady(bReady);
    }
}

void UMingPersonalManager::StartMultiplayerGame()
{
    if (LobbySystem)
    {
        LobbySystem->StartGame();
    }
}

void UMingPersonalManager::SendMultiplayerChatMessage(const FString& Message)
{
    if (LobbySystem)
    {
        LobbySystem->SendChatMessage(Message);
    }
}

UMingNetworkManager* UMingPersonalManager::GetNetworkManager() const
{
    return NetworkManager;
}

UMingLobbySystem* UMingPersonalManager::GetLobbySystem() const
{
    return LobbySystem;
}

// 本地化系統接口
void UMingPersonalManager::InitializeLocalizationSystem()
{
    if (!LocalizationManager)
    {
        LocalizationManager = NewObject<UMingLocalizationManager>(this);
    }

    if (LocalizationManager)
    {
        LocalizationManager->InitializeLocalization(this);
        UE_LOG(LogTemp, Log, TEXT("Localization system initialized"));
    }
}

bool UMingPersonalManager::SetGameLanguage(EMingLanguage NewLanguage)
{
    if (!LocalizationManager)
    {
        InitializeLocalizationSystem();
    }

    if (LocalizationManager)
    {
        bool bSuccess = LocalizationManager->SetLanguage(NewLanguage);
        if (bSuccess)
        {
            UE_LOG(LogTemp, Log, TEXT("Game language changed to: %s"), *LocalizationManager->GetCurrentLanguageName());
        }
        return bSuccess;
    }

    return false;
}

void UMingPersonalManager::SetGameRegion(EMingRegion NewRegion)
{
    if (!LocalizationManager)
    {
        InitializeLocalizationSystem();
    }

    if (LocalizationManager)
    {
        LocalizationManager->SetRegion(NewRegion);
        UE_LOG(LogTemp, Log, TEXT("Game region set to: %s"), *LocalizationManager->GetRegionName(NewRegion));
    }
}

EMingLanguage UMingPersonalManager::GetCurrentGameLanguage() const
{
    if (LocalizationManager)
    {
        return LocalizationManager->GetCurrentLanguage();
    }
    return EMingLanguage::ChineseTraditional;
}

EMingRegion UMingPersonalManager::GetCurrentGameRegion() const
{
    if (LocalizationManager)
    {
        return LocalizationManager->GetCurrentRegion();
    }
    return EMingRegion::Taiwan;
}

FString UMingPersonalManager::GetLocalizedString(const FString& Key, const FString& Namespace) const
{
    if (LocalizationManager)
    {
        return LocalizationManager->GetLocalizedString(Key, Namespace);
    }
    return Key;
}

bool UMingPersonalManager::DownloadLanguagePack(EMingLanguage Language)
{
    if (!LocalizationManager)
    {
        InitializeLocalizationSystem();
    }

    if (LocalizationManager)
    {
        return LocalizationManager->DownloadLanguagePack(Language);
    }

    return false;
}

TArray<FLanguagePackInfo> UMingPersonalManager::GetAvailableLanguagePacks() const
{
    TArray<FLanguagePackInfo> Packs;

    if (LocalizationManager)
    {
        TArray<EMingLanguage> Languages = LocalizationManager->GetSupportedLanguages();
        for (EMingLanguage Language : Languages)
        {
            Packs.Add(LocalizationManager->GetLanguagePackInfo(Language));
        }
    }

    return Packs;
}

UMingLocalizationManager* UMingPersonalManager::GetLocalizationManager() const
{
    return LocalizationManager;
}

// 高校引導系統接口
void UMingPersonalManager::StartUniversityGuide()
{
    if (!UniversityGuideManager)
    {
        UniversityGuideManager = NewObject<UMingUniversityGuideManager>(this);
        UniversityGuideManager->Initialize();
    }

    if (UniversityGuideManager)
    {
        // 設置默認引導配置
        FUniversityGuideConfig GuideConfig;
        GuideConfig.ContentType = EMingUniversityContentType::GameTutorial;
        GuideConfig.UniversityName = TEXT("民國時期策略學院");
        GuideConfig.Department = TEXT("戰略與歷史");
        GuideConfig.TargetAudience = TEXT("新玩家");
        GuideConfig.Duration = TEXT("10分鐘");
        GuideConfig.Language = TEXT("繁體中文");
        GuideConfig.CulturalContext = TEXT("RepublicanEra");
        GuideConfig.bInteractive = true;
        GuideConfig.bIncludeHistoricalContent = true;
        GuideConfig.GuideDescription = TEXT("歡迎來到MingGoRTS！本引導將幫助您了解遊戲的基本玩法和歷史背景。");
        
        UniversityGuideManager->SetGuideConfig(GuideConfig);
        UniversityGuideManager->StartUniversityGuide();
        
        UE_LOG(LogTemp, Log, TEXT("University Guide started"));
    }
}

void UMingPersonalManager::StopUniversityGuide()
{
    if (UniversityGuideManager)
    {
        UniversityGuideManager->StopUniversityGuide();
        UE_LOG(LogTemp, Log, TEXT("University Guide stopped"));
    }
}

UMingUniversityGuideManager* UMingPersonalManager::GetUniversityGuideManager() const
{
    return UniversityGuideManager;
}

// 性能優化系統接口
void UMingPersonalManager::InitializePerformanceSystem()
{
    if (!PerformanceManager)
    {
        PerformanceManager = NewObject<UMingPerformanceManager>(this);
        PerformanceManager->Initialize();
    }

    if (!MemoryOptimizer)
    {
        MemoryOptimizer = NewObject<UMingMemoryOptimizer>(this);
        MemoryOptimizer->Initialize();
    }

    UE_LOG(LogTemp, Log, TEXT("Performance system initialized"));
}

void UMingPersonalManager::SetPerformanceLevel(EMingPerformanceLevel NewLevel)
{
    if (PerformanceManager)
    {
        PerformanceManager->SetPerformanceLevel(NewLevel);
    }
}

EMingPerformanceLevel UMingPersonalManager::GetCurrentPerformanceLevel() const
{
    if (PerformanceManager)
    {
        return PerformanceManager->GetCurrentPerformanceLevel();
    }
    return EMingPerformanceLevel::Medium;
}

void UMingPersonalManager::OptimizePerformance()
{
    if (PerformanceManager)
    {
        PerformanceManager->RunAutoOptimization();
    }
}

void UMingPersonalManager::SetTargetFrameRate(float TargetFPS)
{
    if (PerformanceManager)
    {
        PerformanceManager->SetTargetFrameRate(TargetFPS);
    }
}

FMingPerformanceMetrics UMingPersonalManager::GetPerformanceMetrics() const
{
    if (PerformanceManager)
    {
        return PerformanceManager->GetCurrentMetrics();
    }
    return FMingPerformanceMetrics();
}

void UMingPersonalManager::StartPerformanceMonitoring()
{
    if (PerformanceManager)
    {
        PerformanceManager->StartPerformanceMonitoring();
    }
}

void UMingPersonalManager::StopPerformanceMonitoring()
{
    if (PerformanceManager)
    {
        PerformanceManager->StopPerformanceMonitoring();
    }
}

void UMingPersonalManager::OptimizeMemory()
{
    if (MemoryOptimizer)
    {
        MemoryOptimizer->QuickOptimize();
    }
}

void UMingPersonalManager::SetMemoryBudget(int32 MaxMemoryMB)
{
    if (MemoryOptimizer)
    {
        MemoryOptimizer->SetMemoryBudget(MaxMemoryMB);
    }
}

float UMingPersonalManager::GetMemoryUsagePercent() const
{
    if (MemoryOptimizer)
    {
        return MemoryOptimizer->GetMemoryUsagePercent();
    }
    return 0.0f;
}

UMingPerformanceManager* UMingPersonalManager::GetPerformanceManager() const
{
    return PerformanceManager;
}

UMingMemoryOptimizer* UMingPersonalManager::GetMemoryOptimizer() const
{
    return MemoryOptimizer;
}
