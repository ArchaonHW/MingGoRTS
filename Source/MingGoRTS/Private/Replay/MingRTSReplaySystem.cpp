#include "Replay/MingRTSReplaySystem.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSReplaySystem::UMingRTSReplaySystem()
    : CurrentState(EReplayState::Idle)
    , CurrentPlaybackTime(0.0f)
    , PlaybackSpeed(1.0f)
    , CurrentFrameIndex(0)
{
}

void UMingRTSReplaySystem::InitializeReplaySystem()
{
    UE_LOG(LogTemp, Log, TEXT("Replay System Initialized"));
    
    // 確保回放目錄存在
    FString ReplayDir = GetReplayDirectory();
    IFileManager::Get().MakeDirectory(*ReplayDir, true);
}

void UMingRTSReplaySystem::StartRecording(const FString& MatchID, const FString& MapName)
{
    if (CurrentState == EReplayState::Recording)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already recording"));
        return;
    }
    
    // 清除之前的數據
    RecordedFrames.Empty();
    RecordedEvents.Empty();
    CurrentFrameIndex = 0;
    
    // 設置回放ID
    CurrentReplayID = GenerateReplayID();
    
    // 初始化元數據
    CurrentMetadata.ReplayID = CurrentReplayID;
    CurrentMetadata.MatchID = MatchID;
    CurrentMetadata.MapName = MapName;
    CurrentMetadata.GameVersion = TEXT("1.0.0");
    CurrentMetadata.CreationDate = FDateTime::Now().ToString();
    CurrentMetadata.Duration = 0.0f;
    CurrentMetadata.TotalFrames = 0;
    CurrentMetadata.TotalEvents = 0;
    
    CurrentState = EReplayState::Recording;
    
    // 啟動錄制定時器 (30fps)
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(RecordingTimerHandle, this, &UMingRTSReplaySystem::RecordingTick, 1.0f / 30.0f, true);
    }
    
    OnReplayStarted.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("Started recording replay: %s"), *CurrentReplayID);
}

FReplayMetadata UMingRTSReplaySystem::StopRecording()
{
    if (CurrentState != EReplayState::Recording)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not recording"));
        return FReplayMetadata();
    }
    
    // 停止錄制定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(RecordingTimerHandle);
    }
    
    // 更新元數據
    CurrentMetadata.Duration = CurrentPlaybackTime;
    CurrentMetadata.TotalFrames = RecordedFrames.Num();
    CurrentMetadata.TotalEvents = RecordedEvents.Num();
    
    // 保存到文件
    SaveReplayToFile(CurrentReplayID);
    
    CurrentState = EReplayState::Idle;
    
    OnReplayStopped.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("Stopped recording replay: %s, Duration: %.1fs, Frames: %d, Events: %d"),
        *CurrentReplayID, CurrentMetadata.Duration, CurrentMetadata.TotalFrames, CurrentMetadata.TotalEvents);
    
    return CurrentMetadata;
}

bool UMingRTSReplaySystem::StartPlayback(const FString& ReplayID)
{
    if (CurrentState == EReplayState::Recording)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot playback while recording"));
        return false;
    }
    
    if (CurrentState == EReplayState::Playing)
    {
        StopPlayback();
    }
    
    // 載入回放文件
    if (!LoadReplayFromFile(ReplayID))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load replay: %s"), *ReplayID);
        return false;
    }
    
    CurrentReplayID = ReplayID;
    CurrentPlaybackTime = 0.0f;
    CurrentFrameIndex = 0;
    PlaybackSpeed = 1.0f;
    
    CurrentState = EReplayState::Playing;
    
    // 啟動回放循環
    StartPlaybackLoop();
    
    UE_LOG(LogTemp, Log, TEXT("Started playback of replay: %s"), *ReplayID);
    return true;
}

void UMingRTSReplaySystem::StopPlayback()
{
    if (CurrentState != EReplayState::Playing && CurrentState != EReplayState::Paused)
    {
        return;
    }
    
    StopPlaybackLoop();
    
    CurrentState = EReplayState::Idle;
    CurrentPlaybackTime = 0.0f;
    CurrentFrameIndex = 0;
    
    OnReplayStopped.Broadcast();
    UE_LOG(LogTemp, Log, TEXT("Stopped playback"));
}

void UMingRTSReplaySystem::PausePlayback()
{
    if (CurrentState == EReplayState::Playing)
    {
        CurrentState = EReplayState::Paused;
        StopPlaybackLoop();
        OnReplayPaused.Broadcast();
        UE_LOG(LogTemp, Log, TEXT("Playback paused at %.1fs"), CurrentPlaybackTime);
    }
}

void UMingRTSReplaySystem::ResumePlayback()
{
    if (CurrentState == EReplayState::Paused)
    {
        CurrentState = EReplayState::Playing;
        StartPlaybackLoop();
        UE_LOG(LogTemp, Log, TEXT("Playback resumed"));
    }
}

void UMingRTSReplaySystem::SetPlaybackSpeed(float Speed)
{
    PlaybackSpeed = FMath::Clamp(Speed, 0.1f, 10.0f);
    
    // 如果正在播放，重新啟動定時器以應用新速度
    if (CurrentState == EReplayState::Playing)
    {
        StopPlaybackLoop();
        StartPlaybackLoop();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Playback speed set to %.1fx"), PlaybackSpeed);
}

void UMingRTSReplaySystem::SeekToTime(float TimeInSeconds)
{
    if (CurrentState != EReplayState::Playing && CurrentState != EReplayState::Paused)
    {
        return;
    }
    
    TimeInSeconds = FMath::Clamp(TimeInSeconds, 0.0f, CurrentMetadata.Duration);
    CurrentPlaybackTime = TimeInSeconds;
    
    // 找到對應的幀索引
    CurrentFrameIndex = 0;
    for (int32 i = 0; i < RecordedFrames.Num(); ++i)
    {
        if (RecordedFrames[i].Timestamp >= TimeInSeconds)
        {
            CurrentFrameIndex = i;
            break;
        }
    }
    
    OnPlaybackTimeChanged.Broadcast(CurrentPlaybackTime);
}

void UMingRTSReplaySystem::SeekToFrame(int32 FrameNumber)
{
    if (CurrentState != EReplayState::Playing && CurrentState != EReplayState::Paused)
    {
        return;
    }
    
    if (RecordedFrames.IsValidIndex(FrameNumber))
    {
        CurrentFrameIndex = FrameNumber;
        CurrentPlaybackTime = RecordedFrames[FrameNumber].Timestamp;
        OnPlaybackTimeChanged.Broadcast(CurrentPlaybackTime);
    }
}

void UMingRTSReplaySystem::RecordEvent(EReplayEventType Type, const FString& PlayerID, 
    const FVector& Location, const TArray<uint8>& Data, const FString& Description)
{
    if (CurrentState != EReplayState::Recording)
    {
        return;
    }
    
    FReplayEvent Event;
    Event.EventID = FName(*FString::Printf(TEXT("EVENT_%d_%s"), RecordedEvents.Num(), *FDateTime::Now().ToString()));
    Event.EventType = Type;
    Event.Timestamp = CurrentPlaybackTime;
    Event.FrameNumber = CurrentFrameIndex;
    Event.PlayerID = PlayerID;
    Event.EventData = Data;
    Event.Location = Location;
    Event.Description = Description;
    
    RecordedEvents.Add(Event);
    
    // 廣播事件給監聽器
    OnReplayEvent.Broadcast(Type, Event);
}

void UMingRTSReplaySystem::RecordFrame(const TArray<uint8>& GameStateData)
{
    if (CurrentState != EReplayState::Recording)
    {
        return;
    }
    
    FReplayFrame Frame;
    Frame.FrameNumber = CurrentFrameIndex++;
    Frame.Timestamp = CurrentPlaybackTime;
    Frame.GameStateData = GameStateData;
    
    // 找到當前幀相關的事件
    for (const FReplayEvent& Event : RecordedEvents)
    {
        if (Event.FrameNumber == Frame.FrameNumber)
        {
            Frame.EventIDs.Add(Event.EventID.ToString());
        }
    }
    
    RecordedFrames.Add(Frame);
}

TArray<FReplayMetadata> UMingRTSReplaySystem::GetReplayList() const
{
    TArray<FReplayMetadata> ReplayList;
    
    FString ReplayDir = GetReplayDirectory();
    TArray<FString> Files;
    IFileManager::Get().FindFiles(Files, *(ReplayDir + TEXT("/*")), true, false);
    
    for (const FString& File : Files)
    {
        // 這裡應該從文件中讀取元數據
        // 簡化處理，假設文件名即為回放ID
        FReplayMetadata Metadata;
        Metadata.ReplayID = FPaths::GetBaseFilename(File);
        ReplayList.Add(Metadata);
    }
    
    return ReplayList;
}

bool UMingRTSReplaySystem::DeleteReplay(const FString& ReplayID)
{
    FString FilePath = GetReplayDirectory() / ReplayID + TEXT(".replay");
    
    if (IFileManager::Get().Delete(*FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("Deleted replay: %s"), *ReplayID);
        return true;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Failed to delete replay: %s"), *ReplayID);
    return false;
}

bool UMingRTSReplaySystem::ExportReplay(const FString& ReplayID, const FString& FilePath)
{
    FString SourcePath = GetReplayDirectory() / ReplayID + TEXT(".replay");
    
    if (IFileManager::Get().Copy(*FilePath, *SourcePath, true, true))
    {
        UE_LOG(LogTemp, Log, TEXT("Exported replay %s to %s"), *ReplayID, *FilePath);
        return true;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Failed to export replay %s"), *ReplayID);
    return false;
}

FReplayMetadata UMingRTSReplaySystem::ImportReplay(const FString& FilePath)
{
    FReplayMetadata Metadata;
    
    if (!IFileManager::Get().FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Replay file not found: %s"), *FilePath);
        return Metadata;
    }
    
    // 生成新的回放ID
    FString NewReplayID = GenerateReplayID();
    FString DestPath = GetReplayDirectory() / NewReplayID + TEXT(".replay");
    
    if (IFileManager::Get().Copy(*DestPath, *FilePath, true, true))
    {
        // 載入元數據
        LoadReplayFromFile(NewReplayID);
        Metadata = CurrentMetadata;
        UE_LOG(LogTemp, Log, TEXT("Imported replay as %s"), *NewReplayID);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to import replay"));
    }
    
    return Metadata;
}

EReplayState UMingRTSReplaySystem::GetReplayState() const
{
    return CurrentState;
}

float UMingRTSReplaySystem::GetCurrentPlaybackTime() const
{
    return CurrentPlaybackTime;
}

int32 UMingRTSReplaySystem::GetCurrentPlaybackFrame() const
{
    return CurrentFrameIndex;
}

float UMingRTSReplaySystem::GetTotalDuration() const
{
    return CurrentMetadata.Duration;
}

void UMingRTSReplaySystem::StartPlaybackLoop()
{
    if (UWorld* World = GetWorld())
    {
        float TickInterval = (1.0f / 30.0f) / PlaybackSpeed;
        World->GetTimerManager().SetTimer(PlaybackTimerHandle, this, &UMingRTSReplaySystem::PlaybackTick, TickInterval, true);
    }
}

void UMingRTSReplaySystem::StopPlaybackLoop()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(PlaybackTimerHandle);
    }
}

void UMingRTSReplaySystem::PlaybackTick()
{
    if (CurrentFrameIndex >= RecordedFrames.Num())
    {
        // 回放完成
        StopPlayback();
        OnPlaybackFinished.Broadcast();
        return;
    }
    
    // 處理當前幀
    const FReplayFrame& Frame = RecordedFrames[CurrentFrameIndex];
    ProcessFrame(Frame);
    
    // 更新時間
    CurrentPlaybackTime = Frame.Timestamp;
    OnPlaybackTimeChanged.Broadcast(CurrentPlaybackTime);
    
    // 處理該幀的事件
    for (const FString& EventID : Frame.EventIDs)
    {
        for (const FReplayEvent& Event : RecordedEvents)
        {
            if (Event.EventID.ToString() == EventID)
            {
                ProcessEvent(Event);
                break;
            }
        }
    }
    
    CurrentFrameIndex++;
}

void UMingRTSReplaySystem::RecordingTick()
{
    if (CurrentState != EReplayState::Recording)
    {
        return;
    }
    
    // 更新錄製時間
    CurrentPlaybackTime += 1.0f / 30.0f;
}

FString UMingRTSReplaySystem::GenerateReplayID()
{
    return FString::Printf(TEXT("REPLAY_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
}

FString UMingRTSReplaySystem::GetReplayDirectory() const
{
    return FPaths::ProjectSavedDir() / TEXT("Replays");
}

bool UMingRTSReplaySystem::SaveReplayToFile(const FString& ReplayID)
{
    FString FilePath = GetReplayDirectory() / ReplayID + TEXT(".replay");
    
    // 這裡應該實現序列化邏輯
    // 簡化處理，僅作為示例
    
    FString JsonData;
    JsonData += FString::Printf(TEXT("{\n"));
    JsonData += FString::Printf(TEXT("  \"ReplayID\": \"%s\",\n"), *CurrentMetadata.ReplayID);
    JsonData += FString::Printf(TEXT("  \"MatchID\": \"%s\",\n"), *CurrentMetadata.MatchID);
    JsonData += FString::Printf(TEXT("  \"MapName\": \"%s\",\n"), *CurrentMetadata.MapName);
    JsonData += FString::Printf(TEXT("  \"GameVersion\": \"%s\",\n"), *CurrentMetadata.GameVersion);
    JsonData += FString::Printf(TEXT("  \"CreationDate\": \"%s\",\n"), *CurrentMetadata.CreationDate);
    JsonData += FString::Printf(TEXT("  \"Duration\": %.2f,\n"), CurrentMetadata.Duration);
    JsonData += FString::Printf(TEXT("  \"TotalFrames\": %d,\n"), CurrentMetadata.TotalFrames);
    JsonData += FString::Printf(TEXT("  \"TotalEvents\": %d\n"), CurrentMetadata.TotalEvents);
    JsonData += FString::Printf(TEXT("}\n"));
    
    return FFileHelper::SaveStringToFile(JsonData, *FilePath);
}

bool UMingRTSReplaySystem::LoadReplayFromFile(const FString& ReplayID)
{
    FString FilePath = GetReplayDirectory() / ReplayID + TEXT(".replay");
    
    if (!IFileManager::Get().FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Replay file not found: %s"), *FilePath);
        return false;
    }
    
    // 這裡應該實現反序列化邏輯
    // 簡化處理，僅作為示例
    
    FString JsonData;
    if (!FFileHelper::LoadFileToString(JsonData, *FilePath))
    {
        return false;
    }
    
    // 重置數據
    RecordedFrames.Empty();
    RecordedEvents.Empty();
    
    // 解析JSON並填充數據
    // 這裡應該使用JSON解析器
    
    CurrentMetadata.ReplayID = ReplayID;
    
    UE_LOG(LogTemp, Log, TEXT("Loaded replay from file: %s"), *FilePath);
    return true;
}

void UMingRTSReplaySystem::ProcessFrame(const FReplayFrame& Frame)
{
    // 在這裡處理回放幀數據
    // 例如：重建遊戲狀態、更新單位位置等
    
    UE_LOG(LogTemp, Verbose, TEXT("Processing frame %d at %.2fs"), Frame.FrameNumber, Frame.Timestamp);
}

void UMingRTSReplaySystem::ProcessEvent(const FReplayEvent& Event)
{
    // 在這裡處理回放事件
    // 例如：播放特效、顯示提示等
    
    OnReplayEvent.Broadcast(Event.EventType, Event);
    
    UE_LOG(LogTemp, Verbose, TEXT("Processing event %s of type %d at %.2fs"), 
        *Event.EventID.ToString(), (int32)Event.EventType, Event.Timestamp);
}
