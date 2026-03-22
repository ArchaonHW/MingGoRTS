#include "MingPersonalManager.h"
#include "MingCoreEventBus.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"

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
