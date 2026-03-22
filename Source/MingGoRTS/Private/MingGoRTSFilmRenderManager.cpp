#include "MingGoRTSFilmRenderManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetSystemLibrary.h"

UMingGoRTSFilmRenderManager::UMingGoRTSFilmRenderManager()
    : CurrentPlaybackState(EFilmPlaybackState::Stopped)
    , CurrentFrameIndex(0)
    , FrameRate(24.0f)
    , FrameTimer(0.0f)
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化播放計時器
    PlaybackTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSFilmRenderManager::OnPlaybackTick);
}

void UMingGoRTSFilmRenderManager::BeginPlay()
{
    Super::BeginPlay();
    
    // 如果設置了自動播放，則開始播放
    if (PlaybackSettings.bAutoPlay && FilmFrames.Num() > 0)
    {
        PlayFilm();
    }
}

void UMingGoRTSFilmRenderManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    UpdatePlayback(DeltaTime);
}

void UMingGoRTSFilmRenderManager::PlayFilm()
{
    if (FilmFrames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No film frames to play"));
        return;
    }

    if (CurrentPlaybackState == EFilmPlaybackState::Playing)
    {
        UE_LOG(LogTemp, Log, TEXT("Film is already playing"));
        return;
    }

    CurrentPlaybackState = EFilmPlaybackState::Playing;
    
    // 啟動播放計時器
    if (!PlaybackTickerHandle.IsValid())
    {
        PlaybackTickerHandle = FTicker::GetCoreTicker().AddTicker(PlaybackTicker, 1.0f / FrameRate);
    }

    NotifyPlaybackStateChanged(EFilmPlaybackState::Playing);
    
    UE_LOG(LogTemp, Log, TEXT("Started film playback with %d frames"), FilmFrames.Num());
}

void UMingGoRTSFilmRenderManager::PauseFilm()
{
    if (CurrentPlaybackState != EFilmPlaybackState::Playing)
    {
        UE_LOG(LogTemp, Warning, TEXT("Film is not playing, cannot pause"));
        return;
    }

    CurrentPlaybackState = EFilmPlaybackState::Paused;
    
    // 停止播放計時器
    if (PlaybackTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(PlaybackTickerHandle);
        PlaybackTickerHandle.Reset();
    }

    NotifyPlaybackStateChanged(EFilmPlaybackState::Paused);
    
    UE_LOG(LogTemp, Log, TEXT("Paused film playback"));
}

void UMingGoRTSFilmRenderManager::StopFilm()
{
    if (CurrentPlaybackState == EFilmPlaybackState::Stopped)
    {
        UE_LOG(LogTemp, Log, TEXT("Film is already stopped"));
        return;
    }

    CurrentPlaybackState = EFilmPlaybackState::Stopped;
    CurrentFrameIndex = 0;
    FrameTimer = 0.0f;
    
    // 停止播放計時器
    if (PlaybackTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(PlaybackTickerHandle);
        PlaybackTickerHandle.Reset();
    }

    NotifyPlaybackStateChanged(EFilmPlaybackState::Stopped);
    NotifyFrameChanged();
    
    UE_LOG(LogTemp, Log, TEXT("Stopped film playback"));
}

void UMingGoRTSFilmRenderManager::SeekToFrame(int32 FrameIndex)
{
    if (FrameIndex < 0 || FrameIndex >= FilmFrames.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid frame index: %d"), FrameIndex);
        return;
    }

    CurrentFrameIndex = FrameIndex;
    FrameTimer = 0.0f;
    
    NotifyFrameChanged();
    
    UE_LOG(LogTemp, Log, TEXT("Seeked to frame %d"), FrameIndex);
}

void UMingGoRTSFilmRenderManager::SeekToTime(float TimeInSeconds)
{
    int32 TargetFrame = FMath::RoundToInt(TimeInSeconds * FrameRate);
    SeekToFrame(TargetFrame);
}

void UMingGoRTSFilmRenderManager::SetFilmFrames(const TArray<UTexture2D*>& Frames)
{
    FilmFrames = Frames;
    CurrentFrameIndex = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Set %d film frames"), FilmFrames.Num());
}

void UMingGoRTSFilmRenderManager::AddFrame(UTexture2D* NewFrame)
{
    if (NewFrame)
    {
        FilmFrames.Add(NewFrame);
        UE_LOG(LogTemp, Log, TEXT("Added new film frame. Total frames: %d"), FilmFrames.Num());
    }
}

void UMingGoRTSFilmRenderManager::ClearFrames()
{
    StopFilm();
    FilmFrames.Empty();
    CurrentFrameIndex = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all film frames"));
}

float UMingGoRTSFilmRenderManager::GetCurrentTime() const
{
    return CurrentFrameIndex / FrameRate;
}

float UMingGoRTSFilmRenderManager::GetTotalDuration() const
{
    return FilmFrames.Num() / FrameRate;
}

void UMingGoRTSFilmRenderManager::SetPlaybackSettings(const FFilmPlaybackSettings& Settings)
{
    PlaybackSettings = Settings;
    FrameRate = Settings.PlaybackSpeed * 24.0f; // 基礎幀率為24fps
    
    UE_LOG(LogTemp, Log, TEXT("Updated playback settings. Speed: %.2f, Loop: %s"), 
        Settings.PlaybackSpeed, Settings.bLoop ? TEXT("true") : TEXT("false"));
}

void UMingGoRTSFilmRenderManager::SetFrameRate(float NewFrameRate)
{
    FrameRate = FMath::Max(1.0f, NewFrameRate);
    
    UE_LOG(LogTemp, Log, TEXT("Set frame rate to %.2f fps"), FrameRate);
}

UTexture2D* UMingGoRTSFilmRenderManager::GetCurrentFrameTexture() const
{
    if (CurrentFrameIndex >= 0 && CurrentFrameIndex < FilmFrames.Num())
    {
        return FilmFrames[CurrentFrameIndex];
    }
    
    return nullptr;
}

void UMingGoRTSFilmRenderManager::RenderToRenderTarget(UTextureRenderTarget2D* RenderTarget)
{
    if (!RenderTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid render target"));
        return;
    }

    UTexture2D* CurrentFrame = GetCurrentFrameTexture();
    if (!CurrentFrame)
    {
        UE_LOG(LogTemp, Warning, TEXT("No current frame to render"));
        return;
    }

    // 這裡可以添加實際的渲染邏輯
    // 例如使用 SceneCaptureComponent 或其他渲染方法
    
    UE_LOG(LogTemp, Log, TEXT("Rendered frame %d to render target"), CurrentFrameIndex);
}

void UMingGoRTSFilmRenderManager::UpdatePlayback(float DeltaTime)
{
    if (CurrentPlaybackState != EFilmPlaybackState::Playing)
    {
        return;
    }

    FrameTimer += DeltaTime * PlaybackSettings.PlaybackSpeed;
    
    if (FrameTimer >= 1.0f / FrameRate)
    {
        AdvanceToNextFrame();
        FrameTimer = 0.0f;
    }
}

void UMingGoRTSFilmRenderManager::AdvanceToNextFrame()
{
    CurrentFrameIndex++;
    
    if (CurrentFrameIndex >= FilmFrames.Num())
    {
        if (PlaybackSettings.bLoop)
        {
            CurrentFrameIndex = 0;
            UE_LOG(LogTemp, Log, TEXT("Looping film playback"));
        }
        else
        {
            HandlePlaybackEnd();
            return;
        }
    }
    
    NotifyFrameChanged();
}

void UMingGoRTSFilmRenderManager::HandlePlaybackEnd()
{
    StopFilm();
    OnFilmPlaybackEnded.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("Film playback ended"));
}

void UMingGoRTSFilmRenderManager::NotifyFrameChanged()
{
    OnFilmFrameChanged.Broadcast(CurrentFrameIndex);
    
    // 顯示當前幀信息（用於調試）
    if (GEngine && CurrentPlaybackState == EFilmPlaybackState::Playing)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, 
            FString::Printf(TEXT("Frame: %d/%d"), CurrentFrameIndex + 1, FilmFrames.Num()));
    }
}

void UMingGoRTSFilmRenderManager::NotifyPlaybackStateChanged(EFilmPlaybackState NewState)
{
    OnFilmPlaybackStateChanged.Broadcast(NewState, CurrentFrameIndex);
    
    // 顯示播放狀態（用於調試）
    if (GEngine)
    {
        FString StateString;
        switch (NewState)
        {
        case EFilmPlaybackState::Playing:
            StateString = TEXT("Playing");
            break;
        case EFilmPlaybackState::Paused:
            StateString = TEXT("Paused");
            break;
        case EFilmPlaybackState::Stopped:
            StateString = TEXT("Stopped");
            break;
        case EFilmPlaybackState::Seeking:
            StateString = TEXT("Seeking");
            break;
        }
        
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
            FString::Printf(TEXT("Film State: %s"), *StateString));
    }
}

bool UMingGoRTSFilmRenderManager::OnPlaybackTick(float DeltaTime)
{
    if (CurrentPlaybackState != EFilmPlaybackState::Playing)
    {
        return false;
    }

    AdvanceToNextFrame();
    return CurrentPlaybackState == EFilmPlaybackState::Playing;
}
