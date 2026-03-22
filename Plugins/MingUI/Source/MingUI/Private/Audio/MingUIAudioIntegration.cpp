#include "Audio/MingUIAudioIntegration.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MingAudio/Public/MingMetaSoundsSystem.h"

UMingUIAudioIntegration::UMingUIAudioIntegration()
    : MasterVolume(1.0f)
    , bIsMuted(false)
    , bIsInitialized(false)
    , MaxConcurrentSounds(10)
    , bDebugLogging(false)
{
}

void UMingUIAudioIntegration::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }
    
    // Initialize default sound type volumes
    for (int32 i = 0; i < static_cast<int32>(EMingUISoundType::ErrorCannotInteract) + 1; i++)
    {
        EMingUISoundType SoundType = static_cast<EMingUISoundType>(i);
        SoundTypeVolumes.Add(SoundType, 1.0f);
    }
    
    // Load Republic Era sounds
    LoadRepublicEraSounds();
    
    bIsInitialized = true;
    LogSoundEvent(TEXT("Initialized"), EMingUISoundType::ButtonClick);
}

void UMingUIAudioIntegration::PlayUISound(EMingUISoundType SoundType)
{
    PlayUISoundWithVolume(SoundType, 1.0f);
}

void UMingUIAudioIntegration::PlayUISoundWithVolume(EMingUISoundType SoundType, float Volume)
{
    if (bIsMuted || !bIsInitialized)
    {
        return;
    }
    
    PlaySoundInternal(SoundType, Volume);
}

void UMingUIAudioIntegration::StopUISound(EMingUISoundType SoundType)
{
    StopSoundInternal(SoundType, 0.0f);
}

void UMingUIAudioIntegration::StopAllUISounds()
{
    TArray<EMingUISoundType> SoundsToStop;
    ActiveSounds.GetKeys(SoundsToStop);
    
    for (EMingUISoundType SoundType : SoundsToStop)
    {
        StopUISound(SoundType);
    }
}

void UMingUIAudioIntegration::FadeOutUISound(EMingUISoundType SoundType, float FadeOutDuration)
{
    StopSoundInternal(SoundType, FadeOutDuration);
}

void UMingUIAudioIntegration::RegisterSoundMapping(EMingUISoundType SoundType, USoundCue* SoundCue)
{
    FMingUISoundMapping Mapping;
    Mapping.SoundType = SoundType;
    Mapping.SoundCue = SoundCue;
    Mapping.VolumeMultiplier = 1.0f;
    
    SoundMappings.Add(SoundType, Mapping);
}

void UMingUIAudioIntegration::RegisterSoundMappingStruct(const FMingUISoundMapping& Mapping)
{
    SoundMappings.Add(Mapping.SoundType, Mapping);
}

USoundCue* UMingUIAudioIntegration::GetSoundCue(EMingUISoundType SoundType) const
{
    if (SoundMappings.Contains(SoundType))
    {
        return SoundMappings[SoundType].SoundCue;
    }
    return nullptr;
}

bool UMingUIAudioIntegration::HasSoundMapping(EMingUISoundType SoundType) const
{
    return SoundMappings.Contains(SoundType) && SoundMappings[SoundType].SoundCue != nullptr;
}

void UMingUIAudioIntegration::SetUISoundVolume(float Volume)
{
    MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    OnVolumeChanged.Broadcast(MasterVolume);
    
    // Update all active sounds
    for (auto& Pair : ActiveSounds)
    {
        if (Pair.Value)
        {
            float TypeVolume = GetVolumeForSoundType(Pair.Key);
            Pair.Value->SetVolumeMultiplier(MasterVolume * TypeVolume);
        }
    }
}

void UMingUIAudioIntegration::SetSoundTypeVolume(EMingUISoundType SoundType, float Volume)
{
    SoundTypeVolumes.Add(SoundType, FMath::Clamp(Volume, 0.0f, 1.0f));
    
    // Update if currently playing
    if (ActiveSounds.Contains(SoundType) && ActiveSounds[SoundType])
    {
        ActiveSounds[SoundType]->SetVolumeMultiplier(MasterVolume * SoundTypeVolumes[SoundType]);
    }
}

float UMingUIAudioIntegration::GetSoundTypeVolume(EMingUISoundType SoundType) const
{
    if (SoundTypeVolumes.Contains(SoundType))
    {
        return SoundTypeVolumes[SoundType];
    }
    return 1.0f;
}

void UMingUIAudioIntegration::MuteAll(bool bMute)
{
    bIsMuted = bMute;
    
    if (bMute)
    {
        StopAllUISounds();
    }
}

void UMingUIAudioIntegration::PlayPanelOpenSound()
{
    PlayUISound(EMingUISoundType::PanelOpen);
}

void UMingUIAudioIntegration::PlayPanelCloseSound()
{
    PlayUISound(EMingUISoundType::PanelClose);
}

void UMingUIAudioIntegration::PlayButtonHoverSound()
{
    PlayUISound(EMingUISoundType::ButtonHover);
}

void UMingUIAudioIntegration::PlayButtonClickSound()
{
    PlayUISound(EMingUISoundType::ButtonClick);
}

void UMingUIAudioIntegration::PlayNotificationSound(EMingUISoundType NotificationType)
{
    PlayUISound(NotificationType);
}

void UMingUIAudioIntegration::PlayRelationshipSound(float DeltaValue)
{
    if (DeltaValue > 0)
    {
        PlayUISound(EMingUISoundType::RelationshipIncrease);
    }
    else if (DeltaValue < 0)
    {
        PlayUISound(EMingUISoundType::RelationshipDecrease);
    }
    
    // Check for level up
    // This would be more complex in actual implementation
}

void UMingUIAudioIntegration::PlayReputationSound(float DeltaValue)
{
    if (DeltaValue > 0)
    {
        PlayUISound(EMingUISoundType::ReputationIncrease);
    }
    else if (DeltaValue < 0)
    {
        PlayUISound(EMingUISoundType::ReputationDecrease);
    }
}

void UMingUIAudioIntegration::PlayQuestSound(int32 QuestEventType)
{
    EMingUISoundType SoundType;
    
    switch (QuestEventType)
    {
    case 0: // Accepted
        SoundType = EMingUISoundType::QuestAccepted;
        break;
    case 1: // Completed
        SoundType = EMingUISoundType::QuestCompleted;
        break;
    case 2: // Failed
        SoundType = EMingUISoundType::QuestFailed;
        break;
    case 3: // Updated
        SoundType = EMingUISoundType::QuestUpdated;
        break;
    case 4: // Objective Complete
        SoundType = EMingUISoundType::QuestObjectiveComplete;
        break;
    default:
        SoundType = EMingUISoundType::QuestUpdated;
        break;
    }
    
    PlayUISound(SoundType);
}

void UMingUIAudioIntegration::PlayDialogueSound(EMingUISoundType DialogueSoundType)
{
    PlayUISound(DialogueSoundType);
}

void UMingUIAudioIntegration::StartAmbientSound(EMingUISoundType AmbientType)
{
    PlayUISound(AmbientType);
}

void UMingUIAudioIntegration::StopAmbientSound(EMingUISoundType AmbientType)
{
    StopUISound(AmbientType);
}

void UMingUIAudioIntegration::CrossfadeAmbient(EMingUISoundType FromAmbient, EMingUISoundType ToAmbient, float FadeDuration)
{
    FadeOutUISound(FromAmbient, FadeDuration);
    
    // Play new ambient with fade in
    if (SoundMappings.Contains(ToAmbient))
    {
        FMingUISoundMapping Mapping = SoundMappings[ToAmbient];
        Mapping.FadeInTime = FadeDuration;
        RegisterSoundMappingStruct(Mapping);
        PlayUISound(ToAmbient);
    }
}

void UMingUIAudioIntegration::PlayRelationshipChangeFeedback(float OldValue, float NewValue)
{
    float Delta = NewValue - OldValue;
    
    if (FMath::Abs(Delta) < 0.1f)
    {
        return; // Ignore tiny changes
    }
    
    PlayRelationshipSound(Delta);
    
    // Play level up sound if threshold crossed
    if (OldValue < 80.0f && NewValue >= 80.0f)
    {
        PlayUISound(EMingUISoundType::RelationshipLevelUp);
    }
}

void UMingUIAudioIntegration::PlayReputationChangeFeedback(FName RegionID, float OldValue, float NewValue)
{
    float Delta = NewValue - OldValue;
    
    if (FMath::Abs(Delta) < 0.1f)
    {
        return;
    }
    
    PlayReputationSound(Delta);
    
    // Check for rank up (assuming thresholds at 10, 20, 30...)
    int32 OldRank = FMath::FloorToInt(OldValue / 10.0f);
    int32 NewRank = FMath::FloorToInt(NewValue / 10.0f);
    
    if (NewRank > OldRank)
    {
        PlayUISound(EMingUISoundType::RankUp);
    }
}

void UMingUIAudioIntegration::PlayQuestUpdateFeedback(FName QuestID, int32 UpdateType)
{
    PlayQuestSound(UpdateType);
}

void UMingUIAudioIntegration::PlayErrorFeedback(int32 ErrorType)
{
    EMingUISoundType SoundType = EMingUISoundType::ErrorGeneric;
    
    switch (ErrorType)
    {
    case 0: // Requirement not met
        SoundType = EMingUISoundType::ErrorRequirementNotMet;
        break;
    case 1: // Cannot interact
        SoundType = EMingUISoundType::ErrorCannotInteract;
        break;
    default:
        SoundType = EMingUISoundType::ErrorGeneric;
        break;
    }
    
    PlayUISound(SoundType);
}

void UMingUIAudioIntegration::SetAudioTheme(const FString& ThemeName)
{
    CurrentTheme = ThemeName;
    
    // Reload sounds with new theme
    if (ThemeName == TEXT("RepublicEra"))
    {
        LoadRepublicEraSounds();
    }
}

void UMingUIAudioIntegration::LoadRepublicEraSounds()
{
    // Load Republic Era themed sounds
    // This would load specific sound cues for the 1912-1949 Chinese Republican era style
    
    // Example mappings (paths would be actual project paths)
    // RegisterSoundMapping(EMingUISoundType::PanelOpen, LoadObject<USoundCue>(nullptr, TEXT("/Game/Audio/UI/RepublicEra/PanelOpen_Cue.PanelOpen_Cue")));
    // RegisterSoundMapping(EMingUISoundType::ButtonClick, LoadObject<USoundCue>(nullptr, TEXT("/Game/Audio/UI/RepublicEra/ButtonClick_Cue.ButtonClick_Cue")));
    // etc.
    
    LogSoundEvent(TEXT("Loaded Republic Era sounds"), EMingUISoundType::ButtonClick);
}

void UMingUIAudioIntegration::ApplyCulturalAudioStyle(FName RegionID)
{
    // Apply region-specific audio variations
    // Different regions might have slightly different UI sound flavors
}

void UMingUIAudioIntegration::SetMetaSoundsSystem(UMingMetaSoundsSystem* MetaSounds)
{
    MetaSoundsSystem = MetaSounds;
}

void UMingUIAudioIntegration::SetMaxConcurrentSounds(int32 MaxSounds)
{
    MaxConcurrentSounds = FMath::Max(1, MaxSounds);
    CleanupFinishedSounds();
}

void UMingUIAudioIntegration::EnableDebugLogging(bool bEnable)
{
    bDebugLogging = bEnable;
}

FString UMingUIAudioIntegration::GetDebugInfo() const
{
    FString DebugInfo;
    DebugInfo += FString::Printf(TEXT("UI Audio Integration - %s\n"), bIsInitialized ? TEXT("Initialized") : TEXT("Not Initialized"));
    DebugInfo += FString::Printf(TEXT("Master Volume: %.2f\n"), MasterVolume);
    DebugInfo += FString::Printf(TEXT("Muted: %s\n"), bIsMuted ? TEXT("Yes") : TEXT("No"));
    DebugInfo += FString::Printf(TEXT("Active Sounds: %d / %d\n"), GetActiveSoundCount(), MaxConcurrentSounds);
    DebugInfo += FString::Printf(TEXT("Sound Mappings: %d\n"), SoundMappings.Num());
    DebugInfo += FString::Printf(TEXT("Current Theme: %s\n"), *CurrentTheme);
    
    if (MetaSoundsSystem.IsValid())
    {
        DebugInfo += TEXT("MetaSounds System: Connected\n");
    }
    else
    {
        DebugInfo += TEXT("MetaSounds System: Not Connected\n");
    }
    
    return DebugInfo;
}

// Internal Functions

UAudioComponent* UMingUIAudioIntegration::PlaySoundInternal(EMingUISoundType SoundType, float VolumeMultiplier)
{
    if (!HasSoundMapping(SoundType))
    {
        LogSoundEvent(TEXT("No mapping for sound"), SoundType);
        return nullptr;
    }
    
    // Cleanup finished sounds first
    CleanupFinishedSounds();
    
    // Check concurrent sound limit
    if (ActiveSounds.Num() >= MaxConcurrentSounds)
    {
        // Stop oldest sound
        TArray<EMingUISoundType> SoundTypes;
        ActiveSounds.GetKeys(SoundTypes);
        if (SoundTypes.Num() > 0)
        {
            StopUISound(SoundTypes[0]);
        }
    }
    
    USoundCue* SoundCue = GetSoundCue(SoundType);
    if (!SoundCue)
    {
        return nullptr;
    }
    
    float FinalVolume = MasterVolume * GetVolumeForSoundType(SoundType) * VolumeMultiplier;
    
    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }
    
    // Play sound
    UAudioComponent* AudioComponent = UGameplayStatics::SpawnSound2D(World, SoundCue, FinalVolume);
    
    if (AudioComponent)
    {
        ActiveSounds.Add(SoundType, AudioComponent);
        OnSoundPlayed.Broadcast(SoundType, FinalVolume);
        LogSoundEvent(TEXT("Played"), SoundType);
    }
    
    return AudioComponent;
}

void UMingUIAudioIntegration::StopSoundInternal(EMingUISoundType SoundType, float FadeOutTime)
{
    if (!ActiveSounds.Contains(SoundType))
    {
        return;
    }
    
    UAudioComponent* AudioComponent = ActiveSounds[SoundType];
    if (AudioComponent)
    {
        if (FadeOutTime > 0.0f)
        {
            AudioComponent->FadeOut(FadeOutTime, 0.0f);
        }
        else
        {
            AudioComponent->Stop();
        }
    }
    
    ActiveSounds.Remove(SoundType);
    OnSoundStopped.Broadcast(SoundType);
    LogSoundEvent(TEXT("Stopped"), SoundType);
}

void UMingUIAudioIntegration::CleanupFinishedSounds()
{
    TArray<EMingUISoundType> SoundsToRemove;
    
    for (auto& Pair : ActiveSounds)
    {
        if (!Pair.Value || !Pair.Value->IsPlaying())
        {
            SoundsToRemove.Add(Pair.Key);
        }
    }
    
    for (EMingUISoundType SoundType : SoundsToRemove)
    {
        ActiveSounds.Remove(SoundType);
    }
}

float UMingUIAudioIntegration::GetVolumeForSoundType(EMingUISoundType SoundType) const
{
    return GetSoundTypeVolume(SoundType);
}

FString UMingUIAudioIntegration::GetSoundTypeName(EMingUISoundType SoundType) const
{
    return GetSoundTypeDisplayName(SoundType);
}

void UMingUIAudioIntegration::LogSoundEvent(const FString& Event, EMingUISoundType SoundType)
{
    if (bDebugLogging)
    {
        UE_LOG(LogTemp, Log, TEXT("UI Audio: %s - %s"), *Event, *GetSoundTypeName(SoundType));
    }
}

// Static Utilities

FString UMingUIAudioIntegration::GetSoundTypeDisplayName(EMingUISoundType SoundType)
{
    switch (SoundType)
    {
    case EMingUISoundType::PanelOpen: return TEXT("Panel Open");
    case EMingUISoundType::PanelClose: return TEXT("Panel Close");
    case EMingUISoundType::ButtonHover: return TEXT("Button Hover");
    case EMingUISoundType::ButtonClick: return TEXT("Button Click");
    case EMingUISoundType::NotificationDefault: return TEXT("Notification Default");
    case EMingUISoundType::NotificationRelationship: return TEXT("Notification Relationship");
    case EMingUISoundType::NotificationReputation: return TEXT("Notification Reputation");
    case EMingUISoundType::NotificationQuest: return TEXT("Notification Quest");
    case EMingUISoundType::RelationshipIncrease: return TEXT("Relationship Increase");
    case EMingUISoundType::RelationshipDecrease: return TEXT("Relationship Decrease");
    case EMingUISoundType::RelationshipLevelUp: return TEXT("Relationship Level Up");
    case EMingUISoundType::ReputationIncrease: return TEXT("Reputation Increase");
    case EMingUISoundType::ReputationDecrease: return TEXT("Reputation Decrease");
    case EMingUISoundType::RankUp: return TEXT("Rank Up");
    case EMingUISoundType::QuestAccepted: return TEXT("Quest Accepted");
    case EMingUISoundType::QuestCompleted: return TEXT("Quest Completed");
    case EMingUISoundType::QuestFailed: return TEXT("Quest Failed");
    case EMingUISoundType::QuestUpdated: return TEXT("Quest Updated");
    case EMingUISoundType::DialogueStart: return TEXT("Dialogue Start");
    case EMingUISoundType::DialogueEnd: return TEXT("Dialogue End");
    case EMingUISoundType::DialogueAdvance: return TEXT("Dialogue Advance");
    case EMingUISoundType::DialogueOptionSelect: return TEXT("Dialogue Option Select");
    case EMingUISoundType::ErrorGeneric: return TEXT("Error Generic");
    default: return TEXT("Unknown");
    }
}

TArray<EMingUISoundType> UMingUIAudioIntegration::GetAllNotificationSoundTypes()
{
    return {
        EMingUISoundType::NotificationDefault,
        EMingUISoundType::NotificationRelationship,
        EMingUISoundType::NotificationReputation,
        EMingUISoundType::NotificationQuest,
        EMingUISoundType::NotificationWarning,
        EMingUISoundType::NotificationSuccess
    };
}

TArray<EMingUISoundType> UMingUIAudioIntegration::GetAllRelationshipSoundTypes()
{
    return {
        EMingUISoundType::RelationshipIncrease,
        EMingUISoundType::RelationshipDecrease,
        EMingUISoundType::RelationshipLevelUp
    };
}
