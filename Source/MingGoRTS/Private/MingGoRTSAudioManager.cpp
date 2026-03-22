#include "MingGoRTSAudioManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWaveProcedural.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

UMingGoRTSAudioManager::UMingGoRTSAudioManager()
    : CurrentMixingState(EAudioMixingState::Idle)
    , MixedAudioResult(nullptr)
    , MixedAudioComponent(nullptr)
    , bIsMixing(false)
    , MixingProgress(0.0f)
{
    // 初始化混音計時器
    MixingTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSAudioManager::OnMixingTick);
}

void UMingGoRTSAudioManager::AddAudioTrack(const FAudioTrack& Track)
{
    if (Track.SoundWave && !Track.TrackName.IsEmpty())
    {
        AudioTracks.Add(Track);
        UE_LOG(LogTemp, Log, TEXT("Added audio track: %s"), *Track.TrackName);
    }
}

void UMingGoRTSAudioManager::RemoveAudioTrack(const FString& TrackName)
{
    for (int32 i = 0; i < AudioTracks.Num(); ++i)
    {
        if (AudioTracks[i].TrackName == TrackName)
        {
            AudioTracks.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Removed audio track: %s"), *TrackName);
            break;
        }
    }
}

void UMingGoRTSAudioManager::ClearAllTracks()
{
    AudioTracks.Empty();
    StopMixedAudio();
    UE_LOG(LogTemp, Log, TEXT("Cleared all audio tracks"));
}

FAudioTrack* UMingGoRTSAudioManager::GetAudioTrack(const FString& TrackName)
{
    for (FAudioTrack& Track : AudioTracks)
    {
        if (Track.TrackName == TrackName)
        {
            return &Track;
        }
    }
    return nullptr;
}

void UMingGoRTSAudioManager::SetTrackVolume(const FString& TrackName, float Volume)
{
    if (FAudioTrack* Track = GetAudioTrack(TrackName))
    {
        Track->Volume = FMath::Clamp(Volume, 0.0f, 2.0f);
        UpdateTrackVolumes();
        UE_LOG(LogTemp, Log, TEXT("Set track %s volume to %.2f"), *TrackName, Volume);
    }
}

void UMingGoRTSAudioManager::SetTrackPitch(const FString& TrackName, float Pitch)
{
    if (FAudioTrack* Track = GetAudioTrack(TrackName))
    {
        Track->Pitch = FMath::Clamp(Pitch, 0.1f, 3.0f);
        UE_LOG(LogTemp, Log, TEXT("Set track %s pitch to %.2f"), *TrackName, Pitch);
    }
}

void UMingGoRTSAudioManager::MuteTrack(const FString& TrackName, bool bMuted)
{
    if (FAudioTrack* Track = GetAudioTrack(TrackName))
    {
        Track->bMuted = bMuted;
        UpdateTrackVolumes();
        UE_LOG(LogTemp, Log, TEXT("%s track %s"), bMuted ? TEXT("Muted") : TEXT("Unmuted"), *TrackName);
    }
}

void UMingGoRTSAudioManager::SetTrackLoop(const FString& TrackName, bool bLoop)
{
    if (FAudioTrack* Track = GetAudioTrack(TrackName))
    {
        Track->bLoop = bLoop;
        UE_LOG(LogTemp, Log, TEXT("Set track %s loop to %s"), *TrackName, bLoop ? TEXT("true") : TEXT("false"));
    }
}

void UMingGoRTSAudioManager::StartAudioMixing()
{
    if (AudioTracks.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No audio tracks to mix"));
        return;
    }

    if (CurrentMixingState == EAudioMixingState::Mixing)
    {
        UE_LOG(LogTemp, Warning, TEXT("Audio mixing already in progress"));
        return;
    }

    bIsMixing = true;
    CurrentMixingState = EAudioMixingState::Mixing;
    MixingProgress = 0.0f;

    // 啟動混音計時器
    if (!MixingTickerHandle.IsValid())
    {
        MixingTickerHandle = FTicker::GetCoreTicker().AddTicker(MixingTicker, 0.1f);
    }

    UE_LOG(LogTemp, Log, TEXT("Started audio mixing with %d tracks"), AudioTracks.Num());
}

void UMingGoRTSAudioManager::StopAudioMixing()
{
    bIsMixing = false;
    CurrentMixingState = EAudioMixingState::Idle;

    // 停止混音計時器
    if (MixingTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(MixingTickerHandle);
        MixingTickerHandle.Reset();
    }

    UE_LOG(LogTemp, Log, TEXT("Stopped audio mixing"));
}

void UMingGoRTSAudioManager::SetMixSettings(const FAudioMixSettings& Settings)
{
    MixSettings = Settings;
    ApplyMixSettings();
    UE_LOG(LogTemp, Log, TEXT("Updated audio mix settings"));
}

void UMingGoRTSAudioManager::PlayMixedAudio()
{
    if (!MixedAudioResult)
    {
        UE_LOG(LogTemp, Warning, TEXT("No mixed audio to play"));
        return;
    }

    if (!MixedAudioComponent)
    {
        InitializeMixedAudioComponent();
    }

    if (MixedAudioComponent)
    {
        MixedAudioComponent->SetSound(MixedAudioResult);
        MixedAudioComponent->SetVolumeMultiplier(MixSettings.MasterVolume);
        MixedAudioComponent->Play();
        
        UE_LOG(LogTemp, Log, TEXT("Playing mixed audio"));
    }
}

void UMingGoRTSAudioManager::StopMixedAudio()
{
    if (MixedAudioComponent && MixedAudioComponent->IsPlaying())
    {
        MixedAudioComponent->Stop();
        UE_LOG(LogTemp, Log, TEXT("Stopped mixed audio playback"));
    }
}

void UMingGoRTSAudioManager::PauseMixedAudio()
{
    if (MixedAudioComponent && MixedAudioComponent->IsPlaying())
    {
        MixedAudioComponent->Pause();
        UE_LOG(LogTemp, Log, TEXT("Paused mixed audio playback"));
    }
}

bool UMingGoRTSAudioManager::IsMixedAudioPlaying() const
{
    return MixedAudioComponent && MixedAudioComponent->IsPlaying();
}

void UMingGoRTSAudioManager::PlayMusicTrack(USoundWave* Music, float Volume)
{
    if (!Music)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid music track"));
        return;
    }

    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        UAudioComponent* AudioComponent = UGameplayStatics::CreateSound2D(World, Music, Volume * MixSettings.MusicVolume);
        if (AudioComponent)
        {
            AudioComponent->Play();
            UE_LOG(LogTemp, Log, TEXT("Playing music track"));
        }
    }
}

void UMingGoRTSAudioManager::PlaySFXTrack(USoundWave* SFX, const FVector& Location)
{
    if (!SFX)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid SFX track"));
        return;
    }

    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        UGameplayStatics::PlaySoundAtLocation(World, SFX, Location, MixSettings.SFXVolume);
        UE_LOG(LogTemp, Log, TEXT("Playing SFX track at location"));
    }
}

void UMingGoRTSAudioManager::PlayVoiceTrack(USoundWave* Voice, float Volume)
{
    if (!Voice)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid voice track"));
        return;
    }

    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        UAudioComponent* AudioComponent = UGameplayStatics::CreateSound2D(World, Voice, Volume * MixSettings.VoiceVolume);
        if (AudioComponent)
        {
            AudioComponent->Play();
            UE_LOG(LogTemp, Log, TEXT("Playing voice track"));
        }
    }
}

void UMingGoRTSAudioManager::ExportMixedAudio(const FString& FilePath)
{
    if (!MixedAudioResult)
    {
        UE_LOG(LogTemp, Warning, TEXT("No mixed audio to export"));
        return;
    }

    // 這裡需要實際的音頻導出邏輯
    // 簡化版本：只是記錄文件路徑
    UE_LOG(LogTemp, Log, TEXT("Exporting mixed audio to: %s"), *FilePath);
    
    // 實際實作需要將音頻數據保存為WAV或MP3文件
}

float UMingGoRTSAudioManager::GetAudioDuration(USoundWave* SoundWave) const
{
    if (!SoundWave)
    {
        return 0.0f;
    }

    return SoundWave->GetDuration();
}

void UMingGoRTSAudioManager::AnalyzeAudio(USoundWave* SoundWave, float& OutRMS, float& OutPeak)
{
    OutRMS = 0.0f;
    OutPeak = 0.0f;

    if (!SoundWave)
    {
        return;
    }

    // 這裡需要實際的音頻分析邏輯
    // 簡化版本：返回默認值
    OutRMS = 0.707f; // RMS of sine wave
    OutPeak = 1.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Analyzed audio - RMS: %.3f, Peak: %.3f"), OutRMS, OutPeak);
}

void UMingGoRTSAudioManager::ProcessAudioMixing()
{
    // 混音進度更新
    MixingProgress += 0.1f;
    
    if (MixingProgress >= 1.0f)
    {
        RenderMixedAudio();
        MixingProgress = 1.0f;
        bIsMixing = false;
        CurrentMixingState = EAudioMixingState::Completed;
        
        // 停止混音計時器
        if (MixingTickerHandle.IsValid())
        {
            FTicker::GetCoreTicker().RemoveTicker(MixingTickerHandle);
            MixingTickerHandle.Reset();
        }
        
        NotifyMixCompleted(MixedAudioResult);
    }
    
    NotifyMixProgress(MixingProgress, TEXT("Mixing audio tracks"));
}

bool UMingGoRTSAudioManager::OnMixingTick(float DeltaTime)
{
    if (!bIsMixing)
    {
        return false;
    }

    ProcessAudioMixing();
    return bIsMixing;
}

void UMingGoRTSAudioManager::ApplyMixSettings()
{
    if (MixedAudioComponent)
    {
        MixedAudioComponent->SetVolumeMultiplier(MixSettings.MasterVolume);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applied mix settings - Master Volume: %.2f"), MixSettings.MasterVolume);
}

void UMingGoRTSAudioManager::RenderMixedAudio()
{
    // 創建混合後的音頻波形
    MixedAudioResult = CreateMixedSoundWave();
    
    if (MixedAudioResult)
    {
        UE_LOG(LogTemp, Log, TEXT("Rendered mixed audio successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to render mixed audio"));
    }
}

USoundWave* UMingGoRTSAudioManager::CreateMixedSoundWave()
{
    // 創建程序化音頻波形
    USoundWaveProcedural* SoundWave = NewObject<USoundWaveProcedural>();
    
    if (SoundWave)
    {
        // 計算混合後的音頻參數
        float MaxDuration = 0.0f;
        for (const FAudioTrack& Track : AudioTracks)
        {
            if (Track.SoundWave && !Track.bMuted)
            {
                float TrackDuration = GetAudioDuration(Track.SoundWave);
                MaxDuration = FMath::Max(MaxDuration, TrackDuration);
            }
        }
        
        // 設置音頻參數
        SoundWave->SetSampleRate(44100);
        SoundWave->NumChannels = 2;
        SoundWave->Duration = MaxDuration;
        SoundWave->bLooping = false;
        
        UE_LOG(LogTemp, Log, TEXT("Created mixed sound wave with duration: %.2f seconds"), MaxDuration);
    }
    
    return SoundWave;
}

void UMingGoRTSAudioManager::NotifyMixProgress(float Progress, const FString& Operation)
{
    OnAudioMixProgress.Broadcast(Progress, Operation);
    
    // 顯示混音進度（用於調試）
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, 
            FString::Printf(TEXT("Mixing: %s - %.1f%%"), *Operation, Progress * 100.0f));
    }
}

void UMingGoRTSAudioManager::NotifyMixCompleted(USoundWave* Result)
{
    OnAudioMixCompleted.Broadcast(Result);
    
    // 顯示混音完成（用於調試）
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
            TEXT("Audio mixing completed"));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Audio mixing completed successfully"));
}

void UMingGoRTSAudioManager::InitializeMixedAudioComponent()
{
    if (UWorld* World = GEngine->GetCurrentPlayWorld())
    {
        MixedAudioComponent = NewObject<UAudioComponent>(World);
        if (MixedAudioComponent)
        {
            MixedAudioComponent->RegisterComponent();
            MixedAudioComponent->AttachToComponent(World->GetWorldSettings(), FAttachmentTransformRules::KeepRelativeTransform);
            
            UE_LOG(LogTemp, Log, TEXT("Initialized mixed audio component"));
        }
    }
}

void UMingGoRTSAudioManager::UpdateTrackVolumes()
{
    // 這裡可以實時更新各音軌的音量
    // 實際實作需要重新計算混音
    UE_LOG(LogTemp, Log, TEXT("Updated track volumes"));
}
