#include "Replay/MingRTSReplayPlayerController.h"
#include "Replay/MingRTSReplaySystem.h"
#include "Engine/World.h"

UMingRTSReplayPlayerController::UMingRTSReplayPlayerController()
    : bFreeCameraMode(false)
    , bShowUI(true)
    , bShowEventMarkers(true)
    , CurrentPlaybackSpeed(1.0f)
{
}

void UMingRTSReplayPlayerController::InitializePlayerController(UMingRTSReplaySystem* InReplaySystem)
{
    if (InReplaySystem)
    {
        UnbindFromReplaySystem();
        ReplaySystem = InReplaySystem;
        BindToReplaySystem();
    }
}

void UMingRTSReplayPlayerController::BindToReplaySystem()
{
    if (ReplaySystem)
    {
        ReplaySystem->OnPlaybackTimeChanged.AddDynamic(this, &UMingRTSReplayPlayerController::OnReplayTimeChanged);
        ReplaySystem->OnPlaybackFinished.AddDynamic(this, &UMingRTSReplayPlayerController::OnReplayFinished);
    }
}

void UMingRTSReplayPlayerController::UnbindFromReplaySystem()
{
    if (ReplaySystem)
    {
        ReplaySystem->OnPlaybackTimeChanged.RemoveDynamic(this, &UMingRTSReplayPlayerController::OnReplayTimeChanged);
        ReplaySystem->OnPlaybackFinished.RemoveDynamic(this, &UMingRTSReplayPlayerController::OnReplayFinished);
    }
}

void UMingRTSReplayPlayerController::Play()
{
    if (ReplaySystem)
    {
        ReplaySystem->ResumePlayback();
        OnPlaybackStarted.Broadcast();
    }
}

void UMingRTSReplayPlayerController::Pause()
{
    if (ReplaySystem)
    {
        ReplaySystem->PausePlayback();
        OnPlaybackPaused.Broadcast();
    }
}

void UMingRTSReplayPlayerController::Stop()
{
    if (ReplaySystem)
    {
        ReplaySystem->StopPlayback();
        OnPlaybackStopped.Broadcast();
    }
}

void UMingRTSReplayPlayerController::StepForward()
{
    if (ReplaySystem)
    {
        int32 CurrentFrame = ReplaySystem->GetCurrentPlaybackFrame();
        ReplaySystem->SeekToFrame(CurrentFrame + 1);
    }
}

void UMingRTSReplayPlayerController::StepBackward()
{
    if (ReplaySystem)
    {
        int32 CurrentFrame = ReplaySystem->GetCurrentPlaybackFrame();
        if (CurrentFrame > 0)
        {
            ReplaySystem->SeekToFrame(CurrentFrame - 1);
        }
    }
}

void UMingRTSReplayPlayerController::SetPlaybackSpeed(float Speed)
{
    CurrentPlaybackSpeed = FMath::Clamp(Speed, 0.1f, 10.0f);
    
    if (ReplaySystem)
    {
        ReplaySystem->SetPlaybackSpeed(CurrentPlaybackSpeed);
    }
    
    OnSpeedChanged.Broadcast(CurrentPlaybackSpeed);
}

float UMingRTSReplayPlayerController::GetPlaybackSpeed() const
{
    return CurrentPlaybackSpeed;
}

void UMingRTSReplayPlayerController::JumpToTime(float TimeInSeconds)
{
    if (ReplaySystem)
    {
        ReplaySystem->SeekToTime(TimeInSeconds);
    }
}

void UMingRTSReplayPlayerController::JumpToPercentage(float Percentage)
{
    if (ReplaySystem)
    {
        float TotalDuration = ReplaySystem->GetTotalDuration();
        float TargetTime = TotalDuration * FMath::Clamp(Percentage, 0.0f, 1.0f);
        ReplaySystem->SeekToTime(TargetTime);
    }
}

float UMingRTSReplayPlayerController::GetCurrentTime() const
{
    if (ReplaySystem)
    {
        return ReplaySystem->GetCurrentPlaybackTime();
    }
    return 0.0f;
}

float UMingRTSReplayPlayerController::GetTotalDuration() const
{
    if (ReplaySystem)
    {
        return ReplaySystem->GetTotalDuration();
    }
    return 0.0f;
}

float UMingRTSReplayPlayerController::GetCurrentPercentage() const
{
    float Total = GetTotalDuration();
    if (Total > 0.0f)
    {
        return GetCurrentTime() / Total;
    }
    return 0.0f;
}

void UMingRTSReplayPlayerController::AddBookmark(float Time, const FText& Label)
{
    if (!Bookmarks.Contains(Time))
    {
        Bookmarks.Add(Time);
        Bookmarks.Sort();
        BookmarkLabels.Add(Time, Label);
    }
}

void UMingRTSReplayPlayerController::RemoveBookmark(float Time)
{
    Bookmarks.Remove(Time);
    BookmarkLabels.Remove(Time);
}

TArray<float> UMingRTSReplayPlayerController::GetBookmarks() const
{
    return Bookmarks;
}

void UMingRTSReplayPlayerController::JumpToBookmark(int32 BookmarkIndex)
{
    if (Bookmarks.IsValidIndex(BookmarkIndex))
    {
        JumpToTime(Bookmarks[BookmarkIndex]);
    }
}

TArray<FString> UMingRTSReplayPlayerController::GetEventMarkers() const
{
    TArray<FString> Markers;
    // 這裡應該從回放系統獲取事件標記
    return Markers;
}

void UMingRTSReplayPlayerController::JumpToNextEvent()
{
    float CurrentTime = GetCurrentTime();
    TArray<FString> Events = GetEventMarkers();
    
    // 找到下一個事件的時間
    for (const FString& Event : Events)
    {
        // 解析事件時間並跳轉
        // 這裡需要實現具體的邏輯
    }
}

void UMingRTSReplayPlayerController::JumpToPreviousEvent()
{
    float CurrentTime = GetCurrentTime();
    TArray<FString> Events = GetEventMarkers();
    
    // 找到上一個事件的時間
    // 這裡需要實現具體的邏輯
}

void UMingRTSReplayPlayerController::SetFreeCameraMode(bool bFreeCamera)
{
    bFreeCameraMode = bFreeCamera;
    UE_LOG(LogTemp, Log, TEXT("Free camera mode: %s"), bFreeCamera ? TEXT("Enabled") : TEXT("Disabled"));
}

bool UMingRTSReplayPlayerController::IsFreeCameraMode() const
{
    return bFreeCameraMode;
}

void UMingRTSReplayPlayerController::FollowPlayer(const FString& PlayerID)
{
    bFreeCameraMode = false;
    UE_LOG(LogTemp, Log, TEXT("Following player: %s"), *PlayerID);
}

void UMingRTSReplayPlayerController::FollowUnit(int32 UnitID)
{
    bFreeCameraMode = false;
    UE_LOG(LogTemp, Log, TEXT("Following unit: %d"), UnitID);
}

void UMingRTSReplayPlayerController::ShowUI(bool bVisible)
{
    bShowUI = bVisible;
    UE_LOG(LogTemp, Log, TEXT("Replay UI: %s"), bVisible ? TEXT("Visible") : TEXT("Hidden"));
}

void UMingRTSReplayPlayerController::ShowEventMarkers(bool bVisible)
{
    bShowEventMarkers = bVisible;
    UE_LOG(LogTemp, Log, TEXT("Event markers: %s"), bVisible ? TEXT("Visible") : TEXT("Hidden"));
}

void UMingRTSReplayPlayerController::ShowResourceGraph(bool bVisible)
{
    UE_LOG(LogTemp, Log, TEXT("Resource graph: %s"), bVisible ? TEXT("Visible") : TEXT("Hidden"));
}

void UMingRTSReplayPlayerController::ShowAPMGraph(bool bVisible)
{
    UE_LOG(LogTemp, Log, TEXT("APM graph: %s"), bVisible ? TEXT("Visible") : TEXT("Hidden"));
}

void UMingRTSReplayPlayerController::ExportCurrentFrameAsImage(const FString& FilePath)
{
    UE_LOG(LogTemp, Log, TEXT("Exporting current frame to: %s"), *FilePath);
    // 這裡應該實現截圖邏輯
}

void UMingRTSReplayPlayerController::ExportTimeRangeAsVideo(float StartTime, float EndTime, const FString& FilePath)
{
    UE_LOG(LogTemp, Log, TEXT("Exporting video from %.1fs to %.1fs to: %s"), StartTime, EndTime, *FilePath);
    // 這裡應該實現視頻導出邏輯
}

void UMingRTSReplayPlayerController::OnReplayTimeChanged(float NewTime)
{
    OnTimeChanged.Broadcast(NewTime);
}

void UMingRTSReplayPlayerController::OnReplayFinished()
{
    OnPlaybackStopped.Broadcast();
}
