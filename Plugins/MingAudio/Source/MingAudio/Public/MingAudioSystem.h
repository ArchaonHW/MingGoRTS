#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundCue.h"
#include "MingAudioSystem.generated.h"

UENUM(BlueprintType)
enum class EMingAudioType : uint8
{
    Music UMETA(DisplayName = "Music"),
    SFX UMETA(DisplayName = "SFX"),
    Voice UMETA(DisplayName = "Voice"),
    Ambient UMETA(DisplayName = "Ambient"),
    UI UMETA(DisplayName = "UI")
};

UENUM(BlueprintType)
enum class EMingMusicTheme : uint8
{
    MainMenu UMETA(DisplayName = "Main Menu"),
    CampaignMap UMETA(DisplayName = "Campaign Map"),
    Battle UMETA(DisplayName = "Battle"),
    Victory UMETA(DisplayName = "Victory"),
    Defeat UMETA(DisplayName = "Defeat"),
    Historical UMETA(DisplayName = "Historical"),
    Republic UMETA(DisplayName = "Republic"),
    Imperial UMETA(DisplayName = "Imperial"),
    Tense UMETA(DisplayName = "Tense"),
    Peaceful UMETA(DisplayName = "Peaceful")
};

USTRUCT(BlueprintType)
struct FMingAudioSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MasterVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MusicVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SFXVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VoiceVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AmbientVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UIVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableSFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableVoice;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableAmbient;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableUI;

    FMingAudioSettings()
        : MasterVolume(1.0f)
        , MusicVolume(0.8f)
        , SFXVolume(0.9f)
        , VoiceVolume(1.0f)
        , AmbientVolume(0.6f)
        , UIVolume(0.7f)
        , bEnableMusic(true)
        , bEnableSFX(true)
        , bEnableVoice(true)
        , bEnableAmbient(true)
        , bEnableUI(true)
    {}
};

USTRUCT(BlueprintType)
struct FMingAudioTrack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TrackID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TrackName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAudioType AudioType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingMusicTheme MusicTheme;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase* AudioAsset;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bLoop;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAutoPlay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeInTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FadeOutTime;

    FMingAudioTrack()
        : AudioType(EMingAudioType::Music)
        , MusicTheme(EMingMusicTheme::MainMenu)
        , AudioAsset(nullptr)
        , Duration(0.0f)
        , bLoop(false)
        , bAutoPlay(false)
        , FadeInTime(1.0f)
        , FadeOutTime(1.0f)
    {}
};

/**
 * Audio System for MingGoRTS
 * Manages music, sound effects, voice, and ambient audio
 */
UCLASS(ClassGroup = (Audio), Blueprintable)
class MINGAUDIO_API UMingAudioSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAudioSystem();

    // Audio Management
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void InitializeAudio();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void ShutdownAudio();

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void UpdateAudioSettings(const FMingAudioSettings& NewSettings};

    UFUNCTION(BlueprintPure, Category = "Audio")
    FMingAudioSettings GetCurrentSettings() const;

    // Music Control
    UFUNCTION(BlueprintCallable, Category = "Music")
    void PlayMusic(EMingMusicTheme Theme};

    UFUNCTION(BlueprintCallable, Category = "Music")
    void PlayMusicTrack(const FString& TrackID};

    UFUNCTION(BlueprintCallable, Category = "Music")
    void StopMusic();

    UFUNCTION(BlueprintCallable, Category = "Music")
    void PauseMusic();

    UFUNCTION(BlueprintCallable, Category = "Music")
    void ResumeMusic();

    UFUNCTION(BlueprintCallable, Category = "Music")
    void FadeMusic(float TargetVolume, float Duration};

    UFUNCTION(BlueprintCallable, Category = "Music")
    void CrossFadeMusic(EMingMusicTheme NewTheme, float Duration};

    UFUNCTION(BlueprintPure, Category = "Music")
    bool IsMusicPlaying() const;

    UFUNCTION(BlueprintPure, Category = "Music")
    EMingMusicTheme GetCurrentMusicTheme() const;

    // Sound Effects
    UFUNCTION(BlueprintCallable, Category = "SFX")
    void PlaySFX(const FString& SFXID};

    UFUNCTION(BlueprintCallable, Category = "SFX")
    void PlaySFXAtLocation(const FString& SFXID, FVector Location};

    UFUNCTION(BlueprintCallable, Category = "SFX")
    void PlaySFX2D(const FString& SFXID};

    UFUNCTION(BlueprintCallable, Category = "SFX")
    void StopSFX(const FString& SFXID};

    UFUNCTION(BlueprintCallable, Category = "SFX")
    void StopAllSFX();

    // Voice Audio
    UFUNCTION(BlueprintCallable, Category = "Voice")
    void PlayVoice(const FString& VoiceID};

    UFUNCTION(BlueprintCallable, Category = "Voice")
    void PlayVoiceLine(const FString& CharacterID, const FString& LineID};

    UFUNCTION(BlueprintCallable, Category = "Voice")
    void StopVoice();

    UFUNCTION(BlueprintCallable, Category = "Voice")
    void SetVoiceVolume(float Volume};

    // Ambient Audio
    UFUNCTION(BlueprintCallable, Category = "Ambient")
    void PlayAmbient(const FString& AmbientID};

    UFUNCTION(BlueprintCallable, Category = "Ambient")
    void StopAmbient();

    UFUNCTION(BlueprintCallable, Category = "Ambient")
    void SetAmbientVolume(float Volume};

    // UI Audio
    UFUNCTION(BlueprintCallable, Category = "UI")
    void PlayUISelect();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void PlayUIClick();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void PlayUIHover();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void PlayUIError();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void PlayUISuccess();

    // Audio Track Management
    UFUNCTION(BlueprintCallable, Category = "Tracks")
    void RegisterAudioTrack(const FMingAudioTrack& Track};

    UFUNCTION(BlueprintCallable, Category = "Tracks")
    void UnregisterAudioTrack(const FString& TrackID};

    UFUNCTION(BlueprintPure, Category = "Tracks")
    FMingAudioTrack GetAudioTrack(const FString& TrackID) const;

    UFUNCTION(BlueprintPure, Category = "Tracks")
    TArray<FMingAudioTrack> GetTracksByType(EMingAudioType AudioType) const;

    UFUNCTION(BlueprintPure, Category = "Tracks")
    TArray<FMingAudioTrack> GetTracksByTheme(EMingMusicTheme Theme) const;

    // Volume Control
    UFUNCTION(BlueprintCallable, Category = "Volume")
    void SetMasterVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Volume")
    void SetMusicVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Volume")
    void SetSFXVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Volume")
    void SetVoiceVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Volume")
    void SetAmbientVolume(float Volume};

    UFUNCTION(BlueprintCallable, Category = "Volume")
    void SetUIVolume(float Volume};

    UFUNCTION(BlueprintPure, Category = "Volume")
    float GetMasterVolume() const;

    UFUNCTION(BlueprintPure, Category = "Volume")
    float GetMusicVolume() const;

    UFUNCTION(BlueprintPure, Category = "Volume")
    float GetSFXVolume() const;

    UFUNCTION(BlueprintPure, Category = "Volume")
    float GetVoiceVolume() const;

    UFUNCTION(BlueprintPure, Category = "Volume")
    float GetAmbientVolume() const;

    UFUNCTION(BlueprintPure, Category = "Volume")
    float GetUIVolume() const;

    // Audio States
    UFUNCTION(BlueprintCallable, Category = "States")
    void SetAudioState(EMingAudioType AudioType, bool bEnabled};

    UFUNCTION(BlueprintPure, Category = "States")
    bool IsAudioTypeEnabled(EMingAudioType AudioType) const;

    UFUNCTION(BlueprintCallable, Category = "States")
    void MuteAllAudio();

    UFUNCTION(BlueprintCallable, Category = "States")
    void UnmuteAllAudio();

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMusicStarted, EMingMusicTheme, Theme, const FString&, TrackID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMusicStopped, const FString&, TrackID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSFXPlayed, const FString&, SFXID, const FVector&, Location};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVoicePlayed, const FString&, CharacterID, const FString&, LineID};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMusicStarted OnMusicStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMusicStopped OnMusicStopped;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSFXPlayed OnSFXPlayed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVoicePlayed OnVoicePlayed;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetAudioTypeName(EMingAudioType AudioType};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetMusicThemeName(EMingMusicTheme Theme};

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveAudioSettings() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadAudioSettings(const FString& JsonString};

protected:
    UPROPERTY()
    FMingAudioSettings AudioSettings;

    UPROPERTY()
    TMap<FString, FMingAudioTrack> AudioTracks;

    UPROPERTY()
    EMingMusicTheme CurrentMusicTheme;

    UPROPERTY()
    FString CurrentMusicTrack;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeDefaultTracks();
    void SetupMusicTracks();
    void SetupSFXTracks();
    void SetupVoiceTracks();
    void SetupAmbientTracks();
    void SetupUITracks();

    // Audio Playback
    void PlayAudioTrackInternal(const FString& TrackID, FVector Location = FVector::ZeroVector);
    void StopAudioTrackInternal(const FString& TrackID);
    void ApplyVolumeSettings();

    // Helpers
    FMingAudioTrack* FindAudioTrack(const FString& TrackID);
    TArray<FMingAudioTrack*> GetTracksByTypeInternal(EMingAudioType AudioType);
    TArray<FMingAudioTrack*> GetTracksByThemeInternal(EMingMusicTheme Theme);
    float GetEffectiveVolume(EMingAudioType AudioType) const;
};

