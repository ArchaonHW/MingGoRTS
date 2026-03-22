#include "MingAudioRelationshipManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UMingAudioRelationshipManager::UMingAudioRelationshipManager()
    : AudioIntensity(1.0f)
    , RelationshipAudioVolume(1.0f)
    , ReputationAudioVolume(1.0f)
    , DialogueAudioVolume(1.0f)
    , bIsInitialized(false)
{
}

void UMingAudioRelationshipManager::InitializeAudioRelationshipSystem(UMingMetaSoundsSystem* InMetaSoundsSystem)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("AudioRelationshipManager already initialized"));
        return;
    }

    MetaSoundsSystem = InMetaSoundsSystem;
    
    if (!MetaSoundsSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("MetaSoundsSystem is null - AudioRelationshipManager initialization failed"));
        return;
    }

    // 初始化音頻主題
    InitializeRepublicEraThemes();
    InitializeRelationshipAudioThemes();
    InitializeReputationAudioThemes();
    InitializeDialogueAudioThemes();

    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("AudioRelationshipManager initialized successfully"));
}

void UMingAudioRelationshipManager::PlayRelationshipAudio(const FRelationshipAudioParams& Params)
{
    if (!bIsInitialized || !MetaSoundsSystem)
    {
        return;
    }

    USoundBase* RelationshipSound = GenerateRelationshipAudio(Params);
    
    if (RelationshipSound)
    {
        PlayAudio2D(RelationshipSound, RelationshipAudioVolume * Params.Intensity);
        OnRelationshipAudioPlayed.Broadcast(Params.AudioType, RelationshipSound);
        
        UE_LOG(LogTemp, Log, TEXT("Played relationship audio: %s (Intensity: %.2f)"), 
               *UEnum::GetValueAsString(Params.AudioType), Params.Intensity);
    }
}

void UMingAudioRelationshipManager::OnRelationshipChanged(const FString& CharacterID, float OldValue, float NewValue, const FString& Reason)
{
    if (!bIsInitialized)
    {
        return;
    }

    float ChangeAmount = NewValue - OldValue;
    bool bIsSignificantChange = FMath::Abs(ChangeAmount) >= 10.0f;
    
    FRelationshipAudioParams AudioParams;
    AudioParams.RelationshipChange = ChangeAmount;
    AudioParams.bIsSignificantChange = bIsSignificantChange;
    AudioParams.Intensity = CalculateRelationshipIntensity(ChangeAmount, bIsSignificantChange);

    // 根據變化方向和大小決定音頻類型
    if (ChangeAmount > 0)
    {
        if (bIsSignificantChange && ChangeAmount >= 50.0f)
        {
            AudioParams.AudioType = ERelationshipAudioType::NewFriendship;
        }
        else
        {
            AudioParams.AudioType = ERelationshipAudioType::RelationshipIncrease;
        }
    }
    else if (ChangeAmount < 0)
    {
        if (bIsSignificantChange && ChangeAmount <= -50.0f)
        {
            AudioParams.AudioType = ERelationshipAudioType::RelationshipBreak;
        }
        else
        {
            AudioParams.AudioType = ERelationshipAudioType::RelationshipDecrease;
        }
    }

    PlayRelationshipAudio(AudioParams);
}

void UMingAudioRelationshipManager::OnRelationshipTypeChanged(const FString& CharacterID, ERelationshipType OldType, ERelationshipType NewType)
{
    if (!bIsInitialized)
    {
        return;
    }

    FRelationshipAudioParams AudioParams;
    AudioParams.RelationshipType = NewType;
    AudioParams.Intensity = 1.0f;
    AudioParams.bIsSignificantChange = true;

    // 根據關係類型變化決定音頻
    if (NewType == ERelationshipType::Friend || NewType == ERelationshipType::Ally)
    {
        AudioParams.AudioType = ERelationshipAudioType::NewFriendship;
    }
    else if (NewType == ERelationshipType::Enemy && OldType != ERelationshipType::Enemy)
    {
        AudioParams.AudioType = ERelationshipAudioType::RelationshipBreak;
    }
    else if (NewType == ERelationshipType::Neutral && OldType == ERelationshipType::Enemy)
    {
        AudioParams.AudioType = ERelationshipAudioType::Reconciliation;
    }

    PlayRelationshipAudio(AudioParams);
}

void UMingAudioRelationshipManager::PlayReputationAudio(const FReputationAudioParams& Params)
{
    if (!bIsInitialized || !MetaSoundsSystem)
    {
        return;
    }

    USoundBase* ReputationSound = GenerateReputationAudio(Params);
    
    if (ReputationSound)
    {
        PlayAudio2D(ReputationSound, ReputationAudioVolume);
        OnReputationAudioPlayed.Broadcast(Params.AudioType, ReputationSound);
        
        UE_LOG(LogTemp, Log, TEXT("Played reputation audio: %s (Region: %s)"), 
               *UEnum::GetValueAsString(Params.AudioType), *Params.RegionID);
    }
}

void UMingAudioRelationshipManager::OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason)
{
    if (!bIsInitialized)
    {
        return;
    }

    float ChangeAmount = NewValue - OldValue;
    
    FReputationAudioParams AudioParams;
    AudioParams.ReputationChange = ChangeAmount;
    AudioParams.RegionID = RegionID;
    AudioParams.CurrentLevel = CalculateReputationLevel(NewValue);
    AudioParams.PreviousLevel = CalculateReputationLevel(OldValue);

    // 根據聲望變化決定音頻類型
    if (ChangeAmount > 0)
    {
        AudioParams.AudioType = EReputationAudioType::ReputationIncrease;
    }
    else if (ChangeAmount < 0)
    {
        AudioParams.AudioType = EReputationAudioType::ReputationDecrease;
    }

    // 檢查是否達到新的聲望等級
    if (AudioParams.CurrentLevel != AudioParams.PreviousLevel)
    {
        if (AudioParams.CurrentLevel == EReputationLevel::Legendary)
        {
            AudioParams.AudioType = EReputationAudioType::LegendaryAchievement;
        }
        else
        {
            AudioParams.AudioType = EReputationAudioType::NewReputationLevel;
        }
    }

    PlayReputationAudio(AudioParams);
}

void UMingAudioRelationshipManager::OnReputationLevelChanged(const FString& RegionID, EReputationLevel OldLevel, EReputationLevel NewLevel)
{
    if (!bIsInitialized)
    {
        return;
    }

    FReputationAudioParams AudioParams;
    AudioParams.RegionID = RegionID;
    AudioParams.CurrentLevel = NewLevel;
    AudioParams.PreviousLevel = OldLevel;

    if (NewLevel == EReputationLevel::Legendary)
    {
        AudioParams.AudioType = EReputationAudioType::LegendaryAchievement;
    }
    else if (NewLevel > OldLevel)
    {
        AudioParams.AudioType = EReputationAudioType::NewReputationLevel;
    }
    else if (NewLevel < OldLevel)
    {
        AudioParams.AudioType = EReputationAudioType::ReputationDecrease;
    }

    PlayReputationAudio(AudioParams);
}

void UMingAudioRelationshipManager::PlayDialogueAudio(const FDialogueAudioParams& Params)
{
    if (!bIsInitialized || !MetaSoundsSystem)
    {
        return;
    }

    USoundBase* DialogueSound = GenerateDialogueAudio(Params);
    
    if (DialogueSound)
    {
        PlayAudio2D(DialogueSound, DialogueAudioVolume * Params.EmotionalIntensity);
        OnDialogueAudioPlayed.Broadcast(Params.DialogueType, DialogueSound);
        
        UE_LOG(LogTemp, Log, TEXT("Played dialogue audio: %s (Character: %s)"), 
               *UEnum::GetValueAsString(Params.DialogueType), *Params.CharacterID);
    }
}

void UMingAudioRelationshipManager::OnDialogueStarted(const FString& CharacterID, EDialogueAudioType DialogueType)
{
    if (!bIsInitialized)
    {
        return;
    }

    FDialogueAudioParams AudioParams;
    AudioParams.DialogueType = DialogueType;
    AudioParams.CharacterID = CharacterID;
    AudioParams.EmotionalIntensity = 1.0f;
    AudioParams.bIsImportant = false;

    PlayDialogueAudio(AudioParams);
}

void UMingAudioRelationshipManager::OnDialogueEnded(const FString& CharacterID, bool bWasSuccessful)
{
    if (!bIsInitialized)
    {
        return;
    }

    // 播放對話結束音效
    FDialogueAudioParams AudioParams;
    AudioParams.DialogueType = bWasSuccessful ? EDialogueAudioType::Friendly : EDialogueAudioType::Hostile;
    AudioParams.CharacterID = CharacterID;
    AudioParams.EmotionalIntensity = 0.5f; // 較低的強度
    AudioParams.bIsImportant = false;

    PlayDialogueAudio(AudioParams);
}

void UMingAudioRelationshipManager::PlayRepublicEraTheme(const FString& ThemeName, float Intensity)
{
    if (!bIsInitialized || !MetaSoundsSystem)
    {
        return;
    }

    USoundBase* ThemeSound = GenerateRepublicEraAudio(ThemeName, Intensity);
    
    if (ThemeSound)
    {
        PlayAudio2D(ThemeSound, Intensity);
        
        UE_LOG(LogTemp, Log, TEXT("Played Republic Era theme: %s (Intensity: %.2f)"), *ThemeName, Intensity);
    }
}

void UMingAudioRelationshipManager::PlayRegionalAmbient(const FString& RegionID, EReputationLevel ReputationLevel)
{
    if (!bIsInitialized)
    {
        return;
    }

    FString ThemeName = FString::Printf(TEXT("Regional_%s_%s"), *RegionID, *UEnum::GetValueAsString(ReputationLevel));
    float Intensity = CalculateReputationIntensity(0.0f, ReputationLevel);
    
    PlayRepublicEraTheme(ThemeName, Intensity);
}

void UMingAudioRelationshipManager::PlayCharacterTheme(const FString& CharacterID, ERelationshipType RelationshipType)
{
    if (!bIsInitialized)
    {
        return;
    }

    FString ThemeName = FString::Printf(TEXT("Character_%s_%s"), *CharacterID, *UEnum::GetValueAsString(RelationshipType));
    float Intensity = CalculateDialogueIntensity(RelationshipType, 1.0f);
    
    PlayRepublicEraTheme(ThemeName, Intensity);
}

void UMingAudioRelationshipManager::SetAudioIntensity(float Intensity)
{
    AudioIntensity = FMath::Clamp(Intensity, 0.0f, 2.0f);
}

void UMingAudioRelationshipManager::SetRelationshipAudioVolume(float Volume)
{
    RelationshipAudioVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UMingAudioRelationshipManager::SetReputationAudioVolume(float Volume)
{
    ReputationAudioVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UMingAudioRelationshipManager::SetDialogueAudioVolume(float Volume)
{
    DialogueAudioVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

USoundBase* UMingAudioRelationshipManager::GenerateRelationshipAudio(const FRelationshipAudioParams& Params)
{
    if (!MetaSoundsSystem)
    {
        return nullptr;
    }

    FString AudioPath = GetRelationshipAudioPath(Params.AudioType);
    
    // 使用 MetaSounds 系統生成音頻
    // 這裡可以根據參數調整音頻特性
    switch (Params.AudioType)
    {
        case ERelationshipAudioType::RelationshipIncrease:
            return MetaSoundsSystem->GenerateUIClick(true);
        case ERelationshipAudioType::RelationshipDecrease:
            return MetaSoundsSystem->GenerateUIError();
        case ERelationshipAudioType::NewFriendship:
            return MetaSoundsSystem->GenerateUIClick(true);
        case ERelationshipAudioType::RelationshipBreak:
            return MetaSoundsSystem->GenerateUIError();
        case ERelationshipAudioType::Reconciliation:
            return MetaSoundsSystem->GenerateUIClick(true);
        default:
            return MetaSoundsSystem->GenerateUIClick(true);
    }
}

USoundBase* UMingAudioRelationshipManager::GenerateReputationAudio(const FReputationAudioParams& Params)
{
    if (!MetaSoundsSystem)
    {
        return nullptr;
    }

    switch (Params.AudioType)
    {
        case EReputationAudioType::ReputationIncrease:
            return MetaSoundsSystem->GenerateUIClick(true);
        case EReputationAudioType::ReputationDecrease:
            return MetaSoundsSystem->GenerateUIError();
        case EReputationAudioType::NewReputationLevel:
            return MetaSoundsSystem->GenerateUIClick(true);
        case EReputationAudioType::LegendaryAchievement:
            return MetaSoundsSystem->GenerateUIClick(true);
        default:
            return MetaSoundsSystem->GenerateUIClick(true);
    }
}

USoundBase* UMingAudioRelationshipManager::GenerateDialogueAudio(const FDialogueAudioParams& Params)
{
    if (!MetaSoundsSystem)
    {
        return nullptr;
    }

    switch (Params.DialogueType)
    {
        case EDialogueAudioType::Friendly:
            return MetaSoundsSystem->GenerateUIClick(true);
        case EDialogueAudioType::Hostile:
            return MetaSoundsSystem->GenerateUIError();
        case EDialogueAudioType::Neutral:
            return MetaSoundsSystem->GenerateUIHover();
        case EDialogueAudioType::Romantic:
            return MetaSoundsSystem->GenerateUIClick(true);
        case EDialogueAudioType::Business:
            return MetaSoundsSystem->GenerateUIClick(true);
        case EDialogueAudioType::Intimidation:
            return MetaSoundsSystem->GenerateUIError();
        case EDialogueAudioType::Persuasion:
            return MetaSoundsSystem->GenerateUIHover();
        case EDialogueAudioType::Request:
            return MetaSoundsSystem->GenerateUIClick(true);
        default:
            return MetaSoundsSystem->GenerateUIClick(true);
    }
}

USoundBase* UMingAudioRelationshipManager::GenerateRepublicEraAudio(const FString& ThemeName, float Intensity)
{
    if (!MetaSoundsSystem)
    {
        return nullptr;
    }

    // 根據主題名稱生成對應的環境音效
    if (ThemeName.Contains(TEXT("Battle")))
    {
        return MetaSoundsSystem->GenerateBattleAmbient(100, 500.0f);
    }
    else if (ThemeName.Contains(TEXT("Ambient")))
    {
        return MetaSoundsSystem->GenerateWindAmbient(Intensity, true);
    }
    else
    {
        return MetaSoundsSystem->GenerateWindAmbient(Intensity, false);
    }
}

float UMingAudioRelationshipManager::CalculateRelationshipIntensity(float RelationshipChange, bool bIsSignificant) const
{
    float BaseIntensity = FMath::Abs(RelationshipChange) / 100.0f;
    if (bIsSignificant)
    {
        BaseIntensity *= 1.5f;
    }
    return FMath::Clamp(BaseIntensity * AudioIntensity, 0.1f, 2.0f);
}

float UMingAudioRelationshipManager::CalculateReputationIntensity(float ReputationChange, EReputationLevel Level) const
{
    float BaseIntensity = 1.0f;
    
    switch (Level)
    {
        case EReputationLevel::Legendary:
            BaseIntensity = 2.0f;
            break;
        case EReputationLevel::Honored:
            BaseIntensity = 1.5f;
            break;
        case EReputationLevel::Respected:
            BaseIntensity = 1.2f;
            break;
        case EReputationLevel::Neutral:
            BaseIntensity = 1.0f;
            break;
        case EReputationLevel::Disliked:
            BaseIntensity = 0.8f;
            break;
        case EReputationLevel::Unknown:
        default:
            BaseIntensity = 0.5f;
            break;
    }
    
    return FMath::Clamp(BaseIntensity * AudioIntensity, 0.1f, 2.0f);
}

float UMingAudioRelationshipManager::CalculateDialogueIntensity(ERelationshipType RelationshipType, float EmotionalIntensity) const
{
    float BaseIntensity = EmotionalIntensity;
    
    switch (RelationshipType)
    {
        case ERelationshipType::Romantic:
            BaseIntensity *= 1.3f;
            break;
        case ERelationshipType::Enemy:
            BaseIntensity *= 1.2f;
            break;
        case ERelationshipType::Friend:
        case ERelationshipType::Ally:
            BaseIntensity *= 1.1f;
            break;
        default:
            break;
    }
    
    return FMath::Clamp(BaseIntensity * AudioIntensity, 0.1f, 2.0f);
}

void UMingAudioRelationshipManager::PlayAudioAtLocation(USoundBase* Sound, FVector Location, float VolumeMultiplier)
{
    if (Sound && GetWorld())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location, VolumeMultiplier);
    }
}

void UMingAudioRelationshipManager::PlayAudio2D(USoundBase* Sound, float VolumeMultiplier)
{
    if (Sound && GetWorld())
    {
        UGameplayStatics::PlaySound2D(GetWorld(), Sound, VolumeMultiplier);
    }
}

void UMingAudioRelationshipManager::InitializeRepublicEraThemes()
{
    RepublicEraThemes.Add(TEXT("Battle"), TEXT("Republic_Battle_Theme"));
    RepublicEraThemes.Add(TEXT("Peaceful"), TEXT("Republic_Peaceful_Theme"));
    RepublicEraThemes.Add(TEXT("Tension"), TEXT("Republic_Tension_Theme"));
    RepublicEraThemes.Add(TEXT("Victory"), TEXT("Republic_Victory_Theme"));
    RepublicEraThemes.Add(TEXT("Defeat"), TEXT("Republic_Defeat_Theme"));
}

void UMingAudioRelationshipManager::InitializeRelationshipAudioThemes()
{
    RelationshipAudioThemes.Add(ERelationshipAudioType::RelationshipIncrease, TEXT("Relationship_Increase"));
    RelationshipAudioThemes.Add(ERelationshipAudioType::RelationshipDecrease, TEXT("Relationship_Decrease"));
    RelationshipAudioThemes.Add(ERelationshipAudioType::RelationshipBreak, TEXT("Relationship_Break"));
    RelationshipAudioThemes.Add(ERelationshipAudioType::NewFriendship, TEXT("New_Friendship"));
    RelationshipAudioThemes.Add(ERelationshipAudioType::Betrayal, TEXT("Betrayal"));
    RelationshipAudioThemes.Add(ERelationshipAudioType::Reconciliation, TEXT("Reconciliation"));
    RelationshipAudioThemes.Add(ERelationshipAudioType::FirstMeeting, TEXT("First_Meeting"));
    RelationshipAudioThemes.Add(ERelationshipAudioType::Farewell, TEXT("Farewell"));
}

void UMingAudioRelationshipManager::InitializeReputationAudioThemes()
{
    ReputationAudioThemes.Add(EReputationAudioType::ReputationIncrease, TEXT("Reputation_Increase"));
    ReputationAudioThemes.Add(EReputationAudioType::ReputationDecrease, TEXT("Reputation_Decrease"));
    ReputationAudioThemes.Add(EReputationAudioType::NewReputationLevel, TEXT("New_Reputation_Level"));
    ReputationAudioThemes.Add(EReputationAudioType::LegendaryAchievement, TEXT("Legendary_Achievement"));
    ReputationAudioThemes.Add(EReputationAudioType::RegionWelcome, TEXT("Region_Welcome"));
    ReputationAudioThemes.Add(EReputationAudioType::RegionHostility, TEXT("Region_Hostility"));
}

void UMingAudioRelationshipManager::InitializeDialogueAudioThemes()
{
    DialogueAudioThemes.Add(EDialogueAudioType::Friendly, TEXT("Dialogue_Friendly"));
    DialogueAudioThemes.Add(EDialogueAudioType::Hostile, TEXT("Dialogue_Hostile"));
    DialogueAudioThemes.Add(EDialogueAudioType::Neutral, TEXT("Dialogue_Neutral"));
    DialogueAudioThemes.Add(EDialogueAudioType::Romantic, TEXT("Dialogue_Romantic"));
    DialogueAudioThemes.Add(EDialogueAudioType::Business, TEXT("Dialogue_Business"));
    DialogueAudioThemes.Add(EDialogueAudioType::Intimidation, TEXT("Dialogue_Intimidation"));
    DialogueAudioThemes.Add(EDialogueAudioType::Persuasion, TEXT("Dialogue_Persuasion"));
    DialogueAudioThemes.Add(EDialogueAudioType::Request, TEXT("Dialogue_Request"));
}

FString UMingAudioRelationshipManager::GetRelationshipAudioPath(ERelationshipAudioType AudioType) const
{
    if (const FString* Theme = RelationshipAudioThemes.Find(AudioType))
    {
        return FString::Printf(TEXT("/Game/Audio/Relationship/%s.%s"), *Theme, *Theme);
    }
    return TEXT("/Game/Audio/Relationship/Default_Relationship.Default_Relationship");
}

FString UMingAudioRelationshipManager::GetReputationAudioPath(EReputationAudioType AudioType) const
{
    if (const FString* Theme = ReputationAudioThemes.Find(AudioType))
    {
        return FString::Printf(TEXT("/Game/Audio/Reputation/%s.%s"), *Theme, *Theme);
    }
    return TEXT("/Game/Audio/Reputation/Default_Reputation.Default_Reputation");
}

FString UMingAudioRelationshipManager::GetDialogueAudioPath(EDialogueAudioType AudioType) const
{
    if (const FString* Theme = DialogueAudioThemes.Find(AudioType))
    {
        return FString::Printf(TEXT("/Game/Audio/Dialogue/%s.%s"), *Theme, *Theme);
    }
    return TEXT("/Game/Audio/Dialogue/Default_Dialogue.Default_Dialogue");
}

FString UMingAudioRelationshipManager::GetRepublicEraThemePath(const FString& ThemeName) const
{
    if (const FString* Theme = RepublicEraThemes.Find(ThemeName))
    {
        return FString::Printf(TEXT("/Game/Audio/RepublicEra/%s.%s"), *Theme, *Theme);
    }
    return TEXT("/Game/Audio/RepublicEra/Default_Theme.Default_Theme");
}

EReputationLevel UMingAudioRelationshipManager::CalculateReputationLevel(float ReputationScore) const
{
    if (ReputationScore >= 800.0f)
    {
        return EReputationLevel::Legendary;
    }
    else if (ReputationScore >= 600.0f)
    {
        return EReputationLevel::Honored;
    }
    else if (ReputationScore >= 400.0f)
    {
        return EReputationLevel::Respected;
    }
    else if (ReputationScore >= 200.0f)
    {
        return EReputationLevel::Neutral;
    }
    else if (ReputationScore >= 50.0f)
    {
        return EReputationLevel::Disliked;
    }
    else
    {
        return EReputationLevel::Unknown;
    }
}
