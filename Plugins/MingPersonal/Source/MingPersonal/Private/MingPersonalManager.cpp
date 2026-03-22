#include "MingPersonalManager.h"
#include "MingCoreEventBus.h"

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
{
}

void UMingPersonalManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
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
