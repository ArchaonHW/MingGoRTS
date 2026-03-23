#include "MingGoRTSFilmRenderManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Widget/WidgetSystemLibrary.h"

UMingGoRTSFilmRenderManager::UMingGoRTSFilmRenderManager()
    : CurrentPlaybackState(EFilmPlaybackState::Stopped)
    , CurrentFrameIndex(0)
    , FrameRate(24.0f)
    , FrameTimer(0.0f)
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // ?å??–æ’­?¾è??‚å™¨
    PlaybackTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSFilmRenderManager::OnPlaybackTick);
}

void UMingGoRTSFilmRenderManager::BeginPlay()
{
    Super::BeginPlay();
    
    // å¦‚æ?gäº†è‡ª?•æ’­?¾ï??‡é?å§‹æ’­??
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
        UE_LOG(LogTemp, Warning, TEXT("No film Frames to play"));
        return;
    }

    if (CurrentPlaybackState == EFilmPlaybackState::playing)
    {
        UE_LOG(LogTemp, Log, TEXT("Film is already playing"));
        return;
    }

    CurrentPlaybackState = EFilmPlaybackState::playing;
    
    // ?Ÿå??­æ”¾è¨ˆæ???
    if (!PlaybackTickerHandle.IsValid())
    {
        PlaybackTickerHandle = FTicker::GetCoreTicker().AddTicker(PlaybackTicker, 1.0f / FrameRate);
    }

    NotifyPlaybackStateChanged(EFilmPlaybackState::playing);
    
    UE_LOG(LogTemp, Log, TEXT("Started film playback with %d Frames"), FilmFrames.Num());
}

void UMingGoRTSFilmRenderManager::PauseFilm()
{
    if (CurrentPlaybackState != EFilmPlaybackState::playing)
    {
        UE_LOG(LogTemp, Warning, TEXT("Film is not playing, cannot Pause"));
        return;
    }

    CurrentPlaybackState = EFilmPlaybackState::Paused;
    
    // ?œæ­¢?­æ”¾è¨ˆæ???
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
    
    // ?œæ­¢?­æ”¾è¨ˆæ???
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
    if (FrameIndex < 0  FrameIndex >= FilmFrames.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Frame index: %d"), FrameIndex);
        return;
    }

    CurrentFrameIndex = FrameIndex;
    FrameTimer = 0.0f;
    
    NotifyFrameChanged();
    
    UE_LOG(LogTemp, Log, TEXT("Seeked to Frame %d"), FrameIndex);
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
    
    UE_LOG(LogTemp, Log, TEXT("Set %d film Frames"), FilmFrames.Num());
}

void UMingGoRTSFilmRenderManager::AddFrame(UTexture2D* NewFrame)
{
    if (NewFrame)
    {
        FilmFrames.Add(NewFrame);
        UE_LOG(LogTemp, Log, TEXT("Added new film Frame. Total Frames: %d"), FilmFrames.Num());
    }
}

void UMingGoRTSFilmRenderManager::ClearFrames()
{
    StopFilm();
    FilmFrames.Empty();
    CurrentFrameIndex = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Cleared all film Frames"));
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
    FrameRate = Settings.PlaybackSpeed * 24.0f; // ?ºç?å¹€?‡ç‚º24fps
    
    UE_LOG(LogTemp, Log, TEXT("Updated playback Settings. Speed: %.2f, Loop: %s"), 
        Settings.PlaybackSpeed, Settings.bLoop ?ºr TEXT("true") : TEXT("false"));
}

void UMingGoRTSFilmRenderManager::SetFrameRate(float NewFrameRate)
{
    FrameRate = FMath::Max(1.0f, NewFrameRate);
    
    UE_LOG(LogTemp, Log, TEXT("Set Frame rate to %.2f fps"), FrameRate);
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
        UE_LOG(LogTemp, Warning, TEXT("Invalid render Target"));
        return;
    }

    UTexture2D* CurrentFrame = GetCurrentFrameTexture();
    if (!CurrentFrame)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Current Frame to render"));
        return;
    }

    // ?™è£¡?¯ä»¥æ·»å?å¯¦é?Næ¸²æ??è¼¯
    // ä¾‹å?i??SceneCaptureComponent ?–å…¶ä»–æ¸²?“æ–¹æ³?
    
    UE_LOG(LogTemp, Log, TEXT("Rendered Frame %d to render Target"), CurrentFrameIndex);
}

void UMingGoRTSFilmRenderManager::UpdatePlayback(float DeltaTime)
{
    if (CurrentPlaybackState != EFilmPlaybackState::playing)
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
            UE_LOG(LogTemp, Log, TEXT("Loopine film playback"));
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
    
    // é¡¯ç¤º?¶å?å¹€ä¿¡æ¯ï¼ˆç”¨?¼èª¿è©¦ï?
    if (GEngine && CurrentPlaybackState == EFilmPlaybackState::playing)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, 
            FString::Printf(TEXT("Frame: %d/%d"), CurrentFrameIndex + 1, FilmFrames.Num()));
    }
}

void UMingGoRTSFilmRenderManager::NotifyPlaybackStateChanged(EFilmPlaybackState NewState)
{
    OnFilmPlaybackStateChanged.Broadcast(NewState, CurrentFrameIndex);
    
    // é¡¯ç¤º?­æ”¾?€?‹ï??¨æ–¼èª¿è©¦ï¼?
    if (GEngine)
    {
        FString StateString;
        switch (NewState)
        {
        case EFilmPlaybackState::playing:
            StateString = TEXT("playing");
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
    if (CurrentPlaybackState != EFilmPlaybackState::playing)
    {
        return false;
    }

    AdvanceToNextFrame();
    return CurrentPlaybackState == EFilmPlaybackState::playing;
}
