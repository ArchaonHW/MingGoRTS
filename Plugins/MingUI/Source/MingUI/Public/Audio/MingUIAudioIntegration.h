#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingUIAudioIntegration.generated.h"

UENUM(BlueprintType)
enum class EMingUISoundType : uint8 {
    // Panel Sounds
    PanelOpen UMETA(DisplayName = "Panel Open"),
    PanelClose UMETA(DisplayName = "Panel Close"),
    PanelTransition UMETA(DisplayName = "Panel Transition"),
    
    // Button Sounds
    ButtonHover UMETA(DisplayName = "Button Hover"),
    ButtonClick UMETA(DisplayName = "Button Click"),
    ButtonBack UMETA(DisplayName = "Button Back"),
    
    // Notification Sounds
    NotificationDefault UMETA(DisplayName = "Notification Default"),
    NotificationRelationship UMETA(DisplayName = "Notification Relationship"),
    NotificationReputation UMETA(DisplayName = "Notification Reputation"),
    NotificationQuest UMETA(DisplayName = "Notification Quest"),
    NotificationWarning UMETA(DisplayName = "Notification Warning"),
    NotificationSuccess UMETA(DisplayName = "Notification Success"),
    
    // Relationship Sounds
    RelationshipIncrease UMETA(DisplayName = "Relationship Increase"),
    RelationshipDecrease UMETA(DisplayName = "Relationship Decrease"),
    RelationshipLevelUp UMETA(DisplayName = "Relationship Level Up"),
    
    // Reputation Sounds
    ReputationIncrease UMETA(DisplayName = "Reputation Increase"),
    ReputationDecrease UMETA(DisplayName = "Reputation Decrease"),
    RankUp UMETA(DisplayName = "Rank Up"),
    
    // Quest Sounds
    QuestAccepted UMETA(DisplayName = "Quest Accepted"),
    QuestCompleted UMETA(DisplayName = "Quest Completed"),
    QuestFailed UMETA(DisplayName = "Quest Failed"),
    QuestUpdated UMETA(DisplayName = "Quest Updated"),
    QuestObjectiveComplete UMETA(DisplayName = "Quest Objective Complete"),
    
    // Dialogue Sounds
    DialogueStart UMETA(DisplayName = "Dialogue Start"),
    DialogueEnd UMETA(DisplayName = "Dialogue End"),
    DialogueAdvance UMETA(DisplayName = "Dialogue Advance"),
    DialogueOptionHover UMETA(DisplayName = "Dialogue Option Hover"),
    DialogueOptionSelect UMETA(DisplayName = "Dialogue Option Select"),
    DialogueTyping UMETA(DisplayName = "Dialogue Typing"),
    
    // Ambient Sounds
    AmbientMenu UMETA(DisplayName = "Ambient Menu"),
    AmbientPanel UMETA(DisplayName = "Ambient Panel"),
    
    // Error Sounds
    ErrorGeneric UMETA(DisplayName = "Error Generic"),
    ErrorRequirementNotMet UMETA(DisplayName = "Error Requirement Not Met"),
    ErrorCannotInteract UMETA(DisplayName = "Error Cannot Interact")
};

USTRUCT(BlueprintType)
struct FMingUISoundMapping
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingUISoundType SoundType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundCue* SoundCue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VolumeMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PitchMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bLoop;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeInTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeOutTime;

    FMingUISoundMapping()
        : SoundType(EMingUISoundType::ButtonClick)
        , SoundCue(nullptr)
        , VolumeMultiplier(1.0f)
        , PitchMultiplier(1.0f)
        , bLoop(false)
        , FadeInTime(0.0f)
        , FadeOutTime(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSoundPlayed, EMingUISoundType, SoundType, float, Volume};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundStopped, EMingUISoundType, SoundType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVolumeChanged, float, NewVolume};

/**
 * UI Audio Integration System
 * Manages all UI sounds and audio feedback
 */
UCLASS(ClassGroup = (Audio, UI), Blueprintable)
class MINGUI_API UMingUIAudioIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingUIAudioIntegration(};

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void Initialize(};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    bool IsInitialized() const { return bIsInitialized; }

    // Sound Playback
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayUISound(EMingUISoundType SoundType};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayUISoundWithVolume(EMingUISoundType SoundType, float Volume};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void StopUISound(EMingUISoundType SoundType};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void StopAllUISounds(};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void FadeOutUISound(EMingUISoundType SoundType, float FadeOutDuration};

    // Sound Mapping
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void RegisterSoundMapping(EMingUISoundType SoundType, USoundCue* SoundCue};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void RegisterSoundMappingStruct(const FMingUISoundMapping& Mapping};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    USoundCue* GetSoundCue(EMingUISoundType SoundType) const;

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    bool HasSoundMapping(EMingUISoundType SoundType) const;

    // Volume Control
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void SetUISoundVolume(float Volume};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    float GetUISoundVolume() const { return MasterVolume; }

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void SetSoundTypeVolume(EMingUISoundType SoundType, float Volume};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    float GetSoundTypeVolume(EMingUISoundType SoundType) const;

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void MuteAll(bool bMute};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    bool IsMuted() const { return bIsMuted; }

    // Specific Sound Functions
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayPanelOpenSound(};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayPanelCloseSound(};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayButtonHoverSound(};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayButtonClickSound(};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayNotificationSound(EMingUISoundType NotificationType};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayRelationshipSound(float DeltaValue};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayReputationSound(float DeltaValue};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayQuestSound(int32 QuestEventType};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayDialogueSound(EMingUISoundType DialogueSoundType};

    // Ambient Sounds
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void StartAmbientSound(EMingUISoundType AmbientType};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void StopAmbientSound(EMingUISoundType AmbientType};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void CrossfadeAmbient(EMingUISoundType FromAmbient, EMingUISoundType ToAmbient, float FadeDuration};

    // Audio Feedback for UI Events
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayRelationshipChangeFeedback(float OldValue, float NewValue};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayReputationChangeFeedback(FName RegionID, float OldValue, float NewValue};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayQuestUpdateFeedback(FName QuestID, int32 UpdateType};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void PlayErrorFeedback(int32 ErrorType};

    // Audio Themes (Republic Era Style)
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void SetAudioTheme(const FString& ThemeName};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    FString GetCurrentAudioTheme() const { return CurrentTheme; }

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void LoadRepublicEraSounds(};

    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void ApplyCulturalAudioStyle(FName RegionID};

    // Integration with MetaSounds
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void SetMetaSoundsSystem(class UMingMetaSoundsSystem* MetaSounds};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    UMingMetaSoundsSystem* GetMetaSoundsSystem() const { return MetaSoundsSystem; }

    // Sound Pool Management
    UFUNCTION(BlueprintCallable, Category = "UI Audio")
    void SetMaxConcurrentSounds(int32 MaxSounds};

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    int32 GetMaxConcurrentSounds() const { return MaxConcurrentSounds; }

    UFUNCTION(BlueprintPure, Category = "UI Audio")
    int32 GetActiveSoundCount() const { return ActiveSounds.Num(); }

    // Debugging
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void EnableDebugLogging(bool bEnable};

    UFUNCTION(BlueprintPure, Category = "Debug")
    FString GetDebugInfo() const;

    // Event Delegates
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSoundPlayed OnSoundPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSoundStopped OnSoundStopped;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVolumeChanged OnVolumeChanged;

protected:
    // Sound Mappings
    UPROPERTY()
    TMap<EMingUISoundType, FMingUISoundMapping> SoundMappings;

    UPROPERTY()
    TMap<EMingUISoundType, float> SoundTypeVolumes;

    UPROPERTY()
    TMap<EMingUISoundType, UAudioComponent*> ActiveSounds;

    UPROPERTY()
    float MasterVolume;

    UPROPERTY()
    bool bIsMuted;

    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    int32 MaxConcurrentSounds;

    UPROPERTY()
    FString CurrentTheme;

    UPROPERTY()
    TWeakObjectPtr<UMingMetaSoundsSystem> MetaSoundsSystem;

    UPROPERTY()
    bool bDebugLogging;

    // Internal Functions
    UAudioComponent* PlaySoundInternal(EMingUISoundType SoundType, float VolumeMultiplier};
    void StopSoundInternal(EMingUISoundType SoundType, float FadeOutTime};
    void CleanupFinishedSounds(};
    float GetVolumeForSoundType(EMingUISoundType SoundType) const;
    FString GetSoundTypeName(EMingUISoundType SoundType) const;
    void LogSoundEvent(const FString& Event, EMingUISoundType SoundType};

public:
    // Static utility functions
    UFUNCTION(BlueprintPure, Category = "UI Audio", meta = (StaticFunction))
    static FString GetSoundTypeDisplayName(EMingUISoundType SoundType};

    UFUNCTION(BlueprintPure, Category = "UI Audio", meta = (StaticFunction))
    static TArray<EMingUISoundType> GetAllNotificationSoundTypes(};

    UFUNCTION(BlueprintPure, Category = "UI Audio", meta = (StaticFunction))
    static TArray<EMingUISoundType> GetAllRelationshipSoundTypes(};
};

