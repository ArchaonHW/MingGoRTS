#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "MingGoRTSAudioManager.generated.h"

UENUM(BlueprintType)
enum class EAudioMixingState : uint8
{
    Idle,
    Mixing,
    Rendering,
    Completed,
    Failed
};

USTRUCT(BlueprintType)
struct FAudioTrack
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Track")
    USoundWave* SoundWave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Track")
    FString TrackName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Track")
    float Volume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Track")
    float Pitch = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Track")
    float StartTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Track")
    bool bMuted = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Track")
    bool bLoop = false;
};

USTRUCT(BlueprintType)
struct FAudioMixSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float MasterVolume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float MusicVolume = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float SFXVolume = 0.9f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float VoiceVolume = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    bool bEnableReverb = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float ReverbWetness = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    bool bEnableEQ = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float EQLow = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float EQMid = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio Mix")
    float EQHigh = 1.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAudioMixCompleted, class USoundWave*, MixedAudio};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAudioMixProgress, float, Progress, const FString&, CurrentOperation};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSAudioManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSAudioManager();

    // ????管??
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void AddAudioTrack(const FAudioTrack& Track};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void RemoveAudioTrack(const FString& TrackName};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void ClearAllTracks();

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    TArray<FAudioTrack> GetAudioTracks() const { return AudioTracks; }

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    bool GetAudioTrack(const FString& TrackName, FAudioTrack& OutTrack};

    // ???X?制
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void SetTrackVolume(const FString& TrackName, float Volume};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void SetTrackPitch(const FString& TrackName, float Pitch};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void MuteTrack(const FString& TrackName, bool bMuted};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void SetTrackLoop(const FString& TrackName, bool bLoop};

    // 混音??能
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void StartAudioMixing();

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void StopAudioMixing();

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    EAudioMixingState GetMixingState() const { return CurrentMixingState; }

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void SetMixSettings(const FAudioMixSettings& Settings};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    FAudioMixSettings GetMixSettings() const { return MixSettings; }

    // 實??混音
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void PlayMixedAudio();

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void StopMixedAudio();

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void PauseMixedAudio();

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    bool IsMixedAudioPlaying() const;

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void PlayMusicTrack(USoundWave* Music, float Volume = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void PlaySFXTrack(USoundWave* SFX, const FVector& Location = FVector::ZeroVector};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void PlayVoiceTrack(USoundWave* Voice, float Volume = 1.0f};

    // ??頻導出
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void ExportMixedAudio(const FString& FilePath};

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    USoundWave* GetMixedAudioResult() const { return MixedAudioResult; }

    // ??頻????
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    float GetAudioDuration(USoundWave* SoundWave) const;

    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void AnalyzeAudio(USoundWave* SoundWave, float& OutRMS, float& OutPeak};

    // 委??事件
    UPROPERTY(BlueprintAssignable, Category = "Audio Manager")
    FOnAudioMixCompleted OnAudioMixCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Audio Manager")
    FOnAudioMixProgress OnAudioMixProgress;

protected:
    UPROPERTY()
    TArray<FAudioTrack> AudioTracks;

    UPROPERTY()
    EAudioMixingState CurrentMixingState;

    UPROPERTY()
    FAudioMixSettings MixSettings;

    UPROPERTY()
    USoundWave* MixedAudioResult;

    UPROPERTY()
    UAudioComponent* MixedAudioComponent;

    UPROPERTY()
    bool bIsMixing;

    UPROPERTY()
    float MixingProgress;

    // 注??：??UObject類??不能??UPROPERTY
    FTickerDelegate MixingTicker;
    FDelegateHandle MixingTickerHandle;

private:
    void ProcessAudioMixing();
    bool OnMixingTick(float DeltaTime};
    void ApplyMixSettings();
    void RenderMixedAudio();
    USoundWave* CreateMixedSoundWave();
    void NotifyMixProgress(float Progress, const FString& Operation};
    void NotifyMixCompleted(USoundWave* Result};
    void InitializeMixedAudioComponent();
    void UpdateTrackVolumes();
};

