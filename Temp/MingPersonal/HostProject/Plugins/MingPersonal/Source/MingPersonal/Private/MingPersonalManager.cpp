#include "MingPersonalManager.h"
#include "MingCoreEventBus.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"
#include "MingAIUIManager.h"
#include "MingPersonalUIManager.h"

// 定義角色相關事件
USTRUCT()
struct FCharacterLevelUpEvent : public FMingCoreEvent
{
    GENERATED_BODY()
    
    UPROPERTY()
    int32 NewLevel;
    
    UPROPERTY()
    FString CharacterName;
    
    FCharacterLevelUpEvent(int32 InLevel, const FString& InName)
        : NewLevel(InLevel), CharacterName(InName)
    {
        Priority = EventPriority::Normal;
    }
};

UMingPersonalManager::UMingPersonalManager()
    : bIsInitialized(false)
    , CurrentLevel(1)
    , CurrentExperience(0)
    , ExperienceToNextLevel(100)
    , RelationshipManager(nullptr)
    , AudioRelationshipManager(nullptr)
{
}

void UMingPersonalManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 創建並初始化關係管理器
    RelationshipManager = NewObject<UMingRelationshipManager>(this);
    if (RelationshipManager)
    {
        RelationshipManager->InitializeRelationshipSystem();
    }

    // 創建音頻關係管理器
    AudioRelationshipManager = NewObject<UMingAudioRelationshipManager>(this);
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->InitializeAudioSystem(nullptr);
    }

    // 創建AI UI管理器
    AIUIManager = NewObject<UMingAIUIManager>(this);

    // 創建保存遊戲管理器
    SaveGameManager = NewObject<UMingSaveGameManager>(this);
    if (SaveGameManager)
    {
        SaveGameManager->Initialize();
    }

    SetupEventSubscriptions();
    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("MingPersonalManager initialized - Character system ready"));
}

void UMingPersonalManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    CleanupEventSubscriptions();
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("MingPersonalManager shutdown"));
}

void UMingPersonalManager::SetupEventSubscriptions()
{
    // 訂閱戰鬥勝利事件 (來自戰術層)
    IMingCoreEventBus::Subscribe<FUnitSelectedEvent>(this,
        [this](const FUnitSelectedEvent& Event)
    {
        // 戰鬥勝利獲得經驗
        AddExperience(50);
    });
    
    UE_LOG(LogTemp, Log, TEXT("PersonalManager event subscriptions setup complete"));
}

void UMingPersonalManager::CleanupEventSubscriptions()
{
    IMingCoreEventBus::Unsubscribe<FUnitSelectedEvent>(this);
    UE_LOG(LogTemp, Log, TEXT("PersonalManager event subscriptions cleaned up"));
}

void UMingPersonalManager::AddExperience(int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

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
    ExperienceToNextLevel = CurrentLevel * 100;
    
    OnLevelUp();
    
    // 發布升級事件
    FCharacterLevelUpEvent Event(CurrentLevel, TEXT("Player"));
    IMingCoreEventBus::PublishEvent(Event);
    
    UE_LOG(LogTemp, Log, TEXT("Character leveled up to %d"), CurrentLevel);
}

void UMingPersonalManager::TriggerDialogue(const FString& DialogueId)
{
    UE_LOG(LogTemp, Log, TEXT("Triggering dialogue: %s"), *DialogueId);
    // TODO: 使用事件總線發布對話事件
}

void UMingPersonalManager::OnExperienceGained(int32 Amount)
{
    UE_LOG(LogTemp, Verbose, TEXT("Gained %d experience (Total: %d/%d)"), 
        Amount, CurrentExperience, ExperienceToNextLevel);
}

void UMingPersonalManager::OnLevelUp()
{
    UE_LOG(LogTemp, Log, TEXT("Character level up processed"));
}

// 關係和聲望系統實現
void UMingPersonalManager::UpdateCharacterRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason)
{
    if (RelationshipManager)
    {
        float OldValue = 0.0f;
        if (RelationshipManager->GetAllRelationships().Contains(CharacterID))
        {
            OldValue = RelationshipManager->GetRelationship(CharacterID).RelationshipValue;
        }
        
        RelationshipManager->UpdateRelationship(CharacterID, ChangeAmount, Reason);
        
        // 觸發音頻反饋
        if (AudioRelationshipManager)
        {
            float NewValue = RelationshipManager->GetRelationship(CharacterID).RelationshipValue;
            AudioRelationshipManager->OnRelationshipChanged(CharacterID, OldValue, NewValue, Reason);
        }
    }
}

void UMingPersonalManager::UpdateRegionReputation(const FString& RegionID, float ChangeAmount, const FString& Reason)
{
    if (RelationshipManager)
    {
        float OldValue = 0.0f;
        if (RelationshipManager->GetAllReputations().Contains(RegionID))
        {
            OldValue = RelationshipManager->GetReputation(RegionID).ReputationScore;
        }
        
        RelationshipManager->UpdateReputation(RegionID, ChangeAmount, Reason);
        
        // 觸發音頻反饋
        if (AudioRelationshipManager)
        {
            float NewValue = RelationshipManager->GetReputation(RegionID).ReputationScore;
            AudioRelationshipManager->OnReputationChanged(RegionID, OldValue, NewValue, Reason);
        }
    }
}

FRelationshipData UMingPersonalManager::GetCharacterRelationship(const FString& CharacterID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->GetRelationship(CharacterID);
    }
    return FRelationshipData();
}

FReputationData UMingPersonalManager::GetRegionReputation(const FString& RegionID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->GetReputation(RegionID);
    }
    return FReputationData();
}

TArray<FString> UMingPersonalManager::GetAvailableDialogueOptions(const FString& CharacterID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->GetDialogueOptions(CharacterID);
    }
    return TArray<FString>();
}

float UMingPersonalManager::GetTradePriceModifier(const FString& CharacterID) const
{
    if (RelationshipManager)
    {
        return RelationshipManager->GetPriceModifier(CharacterID);
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
    if (!RelationshipManager)
    {
        return;
    }

    // 任務完成會提升與任務給予者的關係
    UpdateCharacterRelationship(QuestGiverID, 15.0f, FString::Printf(TEXT("完成任務: %s"), *QuestID));
    
    // 任務完成會提升在該地區的聲望
    UpdateRegionReputation(RegionID, 25.0f, FString::Printf(TEXT("完成任務: %s"), *QuestID));
    
    // 獲得經驗值
    AddExperience(100);
    
    UE_LOG(LogTemp, Log, TEXT("Quest completed: %s in %s for %s"), *QuestID, *RegionID, *QuestGiverID);
}

void UMingPersonalManager::OnNPCInteraction(const FString& CharacterID, const FString& InteractionType)
{
    if (!RelationshipManager)
    {
        return;
    }

    float RelationshipChange = 0.0f;
    FString Reason = FString::Printf(TEXT("NPC互動: %s"), *InteractionType);

    // 根據互動類型決定關係變化
    if (InteractionType == TEXT("Friendly"))
    {
        RelationshipChange = 5.0f;
    }
    else if (InteractionType == TEXT("Help"))
    {
        RelationshipChange = 10.0f;
    }
    else if (InteractionType == TEXT("Gift"))
    {
        RelationshipChange = 8.0f;
    }
    else if (InteractionType == TEXT("Trade"))
    {
        RelationshipChange = 2.0f;
    }
    else if (InteractionType == TEXT("Argument"))
    {
        RelationshipChange = -5.0f;
    }
    else if (InteractionType == TEXT("Refuse"))
    {
        RelationshipChange = -3.0f;
    }
    else if (InteractionType == TEXT("Insult"))
    {
        RelationshipChange = -10.0f;
    }

    if (RelationshipChange != 0.0f)
    {
        UpdateCharacterRelationship(CharacterID, RelationshipChange, Reason);
    }
    
    UE_LOG(LogTemp, Log, TEXT("NPC interaction: %s with %s (%.1f change)"), *InteractionType, *CharacterID, RelationshipChange);

    // 觸發音頻反饋
    if (AudioRelationshipManager)
    {
        EDialogueAudioType DialogueType = EDialogueAudioType::Neutral;
        
        if (InteractionType == TEXT("Friendly"))
        {
            DialogueType = EDialogueAudioType::Friendly;
        }
        else if (InteractionType == TEXT("Help"))
        {
            DialogueType = EDialogueAudioType::Persuasion;
        }
        else if (InteractionType == TEXT("Gift"))
        {
            DialogueType = EDialogueAudioType::Friendly;
        }
        else if (InteractionType == TEXT("Trade"))
        {
            DialogueType = EDialogueAudioType::Business;
        }
        else if (InteractionType == TEXT("Argument"))
        {
            DialogueType = EDialogueAudioType::Hostile;
        }
        else if (InteractionType == TEXT("Refuse"))
        {
            DialogueType = EDialogueAudioType::Hostile;
        }
        else if (InteractionType == TEXT("Insult"))
        {
            DialogueType = EDialogueAudioType::Intimidation;
        }
        
        AudioRelationshipManager->OnDialogueStarted(CharacterID, DialogueType);
    }
}

// 音頻系統實現
void UMingPersonalManager::InitializeAudioSystem(UObject* MetaSoundsSystem)
{
    if (AudioRelationshipManager && MetaSoundsSystem)
    {
        AudioRelationshipManager->InitializeAudioRelationshipSystem(Cast<UMingMetaSoundsSystem>(MetaSoundsSystem));
        UE_LOG(LogTemp, Log, TEXT("Audio system initialized in PersonalManager"));
    }
}

void UMingPersonalManager::PlayRelationshipAudio(const FString& CharacterID, const FString& AudioType)
{
    if (AudioRelationshipManager)
    {
        ERelationshipAudioType RelationshipAudioType = ERelationshipAudioType::RelationshipIncrease;
        
        if (AudioType == TEXT("Increase"))
        {
            RelationshipAudioType = ERelationshipAudioType::RelationshipIncrease;
        }
        else if (AudioType == TEXT("Decrease"))
        {
            RelationshipAudioType = ERelationshipAudioType::RelationshipDecrease;
        }
        else if (AudioType == TEXT("Break"))
        {
            RelationshipAudioType = ERelationshipAudioType::RelationshipBreak;
        }
        else if (AudioType == TEXT("NewFriendship"))
        {
            RelationshipAudioType = ERelationshipAudioType::NewFriendship;
        }
        else if (AudioType == TEXT("Betrayal"))
        {
            RelationshipAudioType = ERelationshipAudioType::Betrayal;
        }
        else if (AudioType == TEXT("Reconciliation"))
        {
            RelationshipAudioType = ERelationshipAudioType::Reconciliation;
        }
        
        FRelationshipAudioParams Params;
        Params.AudioType = RelationshipAudioType;
        Params.CharacterID = CharacterID;
        Params.Intensity = 1.0f;
        
        AudioRelationshipManager->PlayRelationshipAudio(Params);
    }
}

void UMingPersonalManager::PlayReputationAudio(const FString& RegionID, const FString& AudioType)
{
    if (AudioRelationshipManager)
    {
        EReputationAudioType ReputationAudioType = EReputationAudioType::ReputationIncrease;
        
        if (AudioType == TEXT("Increase"))
        {
            ReputationAudioType = EReputationAudioType::ReputationIncrease;
        }
        else if (AudioType == TEXT("Decrease"))
        {
            ReputationAudioType = EReputationAudioType::ReputationDecrease;
        }
        else if (AudioType == TEXT("NewLevel"))
        {
            ReputationAudioType = EReputationAudioType::NewReputationLevel;
        }
        else if (AudioType == TEXT("Legendary"))
        {
            ReputationAudioType = EReputationAudioType::LegendaryAchievement;
        }
        else if (AudioType == TEXT("Welcome"))
        {
            ReputationAudioType = EReputationAudioType::RegionWelcome;
        }
        else if (AudioType == TEXT("Hostility"))
        {
            ReputationAudioType = EReputationAudioType::RegionHostility;
        }
        
        FReputationAudioParams Params;
        Params.AudioType = ReputationAudioType;
        Params.RegionID = RegionID;
        
        AudioRelationshipManager->PlayReputationAudio(Params);
    }
}

void UMingPersonalManager::PlayDialogueAudio(const FString& CharacterID, const FString& DialogueType)
{
    if (AudioRelationshipManager)
    {
        EDialogueAudioType AudioDialogueType = EDialogueAudioType::Neutral;
        
        if (DialogueType == TEXT("Friendly"))
        {
            AudioDialogueType = EDialogueAudioType::Friendly;
        }
        else if (DialogueType == TEXT("Hostile"))
        {
            AudioDialogueType = EDialogueAudioType::Hostile;
        }
        else if (DialogueType == TEXT("Neutral"))
        {
            AudioDialogueType = EDialogueAudioType::Neutral;
        }
        else if (DialogueType == TEXT("Romantic"))
        {
            AudioDialogueType = EDialogueAudioType::Romantic;
        }
        else if (DialogueType == TEXT("Business"))
        {
            AudioDialogueType = EDialogueAudioType::Business;
        }
        else if (DialogueType == TEXT("Intimidation"))
        {
            AudioDialogueType = EDialogueAudioType::Intimidation;
        }
        else if (DialogueType == TEXT("Persuasion"))
        {
            AudioDialogueType = EDialogueAudioType::Persuasion;
        }
        else if (DialogueType == TEXT("Request"))
        {
            AudioDialogueType = EDialogueAudioType::Request;
        }
        
        FDialogueAudioParams Params;
        Params.DialogueType = AudioDialogueType;
        Params.CharacterID = CharacterID;
        Params.EmotionalIntensity = 1.0f;
        
        AudioRelationshipManager->PlayDialogueAudio(Params);
    }
}

void UMingPersonalManager::PlayRepublicEraTheme(const FString& ThemeName, float Intensity)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->PlayRepublicEraTheme(ThemeName, Intensity);
    }
}

void UMingPersonalManager::SetAudioVolume(float RelationshipVolume, float ReputationVolume, float DialogueVolume)
{
    if (AudioRelationshipManager)
    {
        AudioRelationshipManager->SetRelationshipAudioVolume(RelationshipVolume);
        AudioRelationshipManager->SetReputationAudioVolume(ReputationVolume);
        AudioRelationshipManager->SetDialogueAudioVolume(DialogueVolume);
    }
}

void UMingPersonalManager::InitializeAIUISystem()
{
    if (AIUIManager && RelationshipManager)
    {
        // 獲取UI管理器實例（這裡需要從遊戲實例獲取）
        UMingPersonalUIManager* UIManager = nullptr; // 實際實現中需要獲取UI管理器
        AIUIManager->InitializeAIUIManager(UIManager, RelationshipManager);
        
        // 開始追蹤用戶行為
        TrackUserBehavior(TEXT("SystemInitialized"), TEXT("AIUI"), 1.0f);
        
        UE_LOG(LogTemp, Log, TEXT("AI UI System initialized"));
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
        AIUIManager->AnalyzeUserBehavior();
        AIUIManager->ApplyAllRecommendedAdaptations();
        
        TrackUserBehavior(TEXT("UIOptimization"), TEXT("PersonalManager"), 1.0f);
    }
}

void UMingPersonalManager::ShowContextualHelp(const FString& Context)
{
    if (AIUIManager)
    {
        AIUIManager->ShowContextualHelp(Context);
        TrackUserBehavior(TEXT("ContextualHelp"), Context, 1.0f);
    }
}

TArray<FString> UMingPersonalManager::GetAIRecommendations()
{
    TArray<FString> Recommendations;
    
    if (AIUIManager)
    {
        // 獲取推薦任務
        TArray<FString> QuestRecommendations = AIUIManager->GetRecommendedQuests();
        Recommendations.Append(QuestRecommendations);
        
        // 獲取建議角色
        TArray<FString> CharacterSuggestions = AIUIManager->GetSuggestedCharacters();
        Recommendations.Append(CharacterSuggestions);
        
        // 添加基於用戶檔案的建議
        FAIUserProfileData Profile = AIUIManager->GetCurrentProfile();
        switch (Profile.ProfileType)
        {
        case EAIUserProfile::NewPlayer:
            Recommendations.Add(TEXT("Focus on building relationships early"));
            break;
        case EAIUserProfile::SocialPlayer:
            Recommendations.Add(TEXT("Visit the relationship panel frequently"));
            break;
        case EAIUserProfile::PowerPlayer:
            Recommendations.Add(TEXT("Optimize your combat strategies"));
            break;
        default:
            break;
        }
    }
    
    return Recommendations;
}

// 保存和載入系統實現
void UMingPersonalManager::InitializeSaveSystem()
{
    if (!SaveGameManager)
    {
        SaveGameManager = NewObject<UMingSaveGameManager>(this);
    }

    if (SaveGameManager)
    {
        SaveGameManager->Initialize();
        UE_LOG(LogTemp, Log, TEXT("Save system initialized"));
    }
}

EMingSaveGameResult UMingPersonalManager::SaveGame(int32 SlotIndex, const FString& SaveName)
{
    if (!SaveGameManager)
    {
        return EMingSaveGameResult::Failed_Unknown;
    }

    EMingSaveGameResult Result = SaveGameManager->SaveGame(SlotIndex, SaveName, false);

    if (Result == EMingSaveGameResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("Game saved to slot %d: %s"), SlotIndex, *SaveName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to save game to slot %d: %d"), SlotIndex, static_cast<int32>(Result));
    }

    return Result;
}

EMingSaveGameResult UMingPersonalManager::QuickSave()
{
    if (!SaveGameManager)
    {
        return EMingSaveGameResult::Failed_Unknown;
    }

    int32 QuickSlot = SaveGameManager->GetQuickSaveSlot();
    EMingSaveGameResult Result = SaveGameManager->QuickSave();

    if (Result == EMingSaveGameResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("Quick save completed to slot %d"), QuickSlot);
    }

    return Result;
}

EMingSaveGameResult UMingPersonalManager::LoadGame(int32 SlotIndex)
{
    if (!SaveGameManager)
    {
        return EMingSaveGameResult::Failed_Unknown;
    }

    if (!SaveGameManager->DoesSaveExist(SlotIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("No save game exists in slot %d"), SlotIndex);
        return EMingSaveGameResult::Failed_CorruptData;
    }

    EMingSaveGameResult Result = SaveGameManager->LoadGame(SlotIndex);

    if (Result == EMingSaveGameResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("Game loaded from slot %d"), SlotIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load game from slot %d: %d"), SlotIndex, static_cast<int32>(Result));
    }

    return Result;
}

EMingSaveGameResult UMingPersonalManager::QuickLoad()
{
    if (!SaveGameManager)
    {
        return EMingSaveGameResult::Failed_Unknown;
    }

    int32 QuickSlot = SaveGameManager->GetQuickSaveSlot();
    return LoadGame(QuickSlot);
}

bool UMingPersonalManager::DeleteSaveGame(int32 SlotIndex)
{
    if (!SaveGameManager)
    {
        return false;
    }

    bool bSuccess = SaveGameManager->DeleteSaveGame(SlotIndex);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Save game deleted from slot %d"), SlotIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to delete save game from slot %d"), SlotIndex);
    }

    return bSuccess;
}

void UMingPersonalManager::SetAutoSaveEnabled(bool bEnabled)
{
    if (SaveGameManager)
    {
        SaveGameManager->SetAutosaveEnabled(bEnabled);
        UE_LOG(LogTemp, Log, TEXT("Auto save %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
    }
}

bool UMingPersonalManager::IsAutoSaveEnabled() const
{
    if (SaveGameManager)
    {
        return SaveGameManager->IsAutosaveEnabled();
    }
    return false;
}

UMingSaveGameManager* UMingPersonalManager::GetSaveGameManager() const
{
    return SaveGameManager;
}

// 多人遊戲系統實現
void UMingPersonalManager::InitializeMultiplayerSystem()
{
    if (!NetworkManager)
    {
        NetworkManager = NewObject<UMingNetworkManager>(this);
    }

    if (NetworkManager)
    {
        NetworkManager->InitializeNetworkManager(this);
        UE_LOG(LogTemp, Log, TEXT("Network system initialized"));
    }

    if (!LobbySystem)
    {
        LobbySystem = NewObject<UMingLobbySystem>(this);
    }

    if (LobbySystem)
    {
        LobbySystem->InitializeLobbySystem(NetworkManager);
        UE_LOG(LogTemp, Log, TEXT("Lobby system initialized"));
    }
}

bool UMingPersonalManager::CreateMultiplayerServer(const FMingNetworkConfig& Config)
{
    if (!NetworkManager)
    {
        InitializeMultiplayerSystem();
    }

    if (NetworkManager)
    {
        bool bSuccess = NetworkManager->CreateServer(Config);
        if (bSuccess)
        {
            // 創建大廳
            if (LobbySystem)
            {
                FLobbyConfig LobbyConfig;
                LobbyConfig.MaxPlayers = Config.MaxPlayers;
                LobbyConfig.GameMode = EMingNetworkGameMode::Cooperative;
                LobbySystem->CreateLobby(LobbyConfig);
            }
        }
        return bSuccess;
    }

    return false;
}

bool UMingPersonalManager::ConnectToMultiplayerServer(const FString& ServerAddress, int32 Port)
{
    if (!NetworkManager)
    {
        InitializeMultiplayerSystem();
    }

    if (NetworkManager)
    {
        return NetworkManager->ConnectToServer(ServerAddress, Port);
    }

    return false;
}

void UMingPersonalManager::DisconnectFromMultiplayerServer()
{
    if (LobbySystem)
    {
        LobbySystem->LeaveLobby();
    }

    if (NetworkManager)
    {
        NetworkManager->DisconnectFromServer();
    }

    UE_LOG(LogTemp, Log, TEXT("Disconnected from multiplayer server"));
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
    return 0;
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
    if (LobbySystem && IsMultiplayerHost())
    {
        if (LobbySystem->CanStartGame())
        {
            LobbySystem->StartGame();
            UE_LOG(LogTemp, Log, TEXT("Multiplayer game started"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot start game - conditions not met"));
        }
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
