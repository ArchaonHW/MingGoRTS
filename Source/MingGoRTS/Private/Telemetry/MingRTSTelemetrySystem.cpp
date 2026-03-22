#include "Telemetry/MingRTSTelemetrySystem.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

UMingRTSTelemetrySystem::UMingRTSTelemetrySystem()
    : bTelemetryEnabled(true)
    , BatchSize(100)
    , FlushInterval(30.0f)
{
}

void UMingRTSTelemetrySystem::InitializeTelemetrySystem()
{
    StartSession(TEXT("Anonymous"));
    InitializeFlushTimer();
    UE_LOG(LogTemp, Log, TEXT("Telemetry System Initialized"));
}

void UMingRTSTelemetrySystem::SetTelemetryEnabled(bool bEnabled)
{
    if (bTelemetryEnabled != bEnabled)
    {
        bTelemetryEnabled = bEnabled;
        OnTelemetryEnabledChanged.Broadcast(bEnabled);
        
        if (bEnabled)
        {
            InitializeFlushTimer();
        }
        else
        {
            if (UWorld* World = GetWorld())
            {
                World->GetTimerManager().ClearTimer(FlushTimerHandle);
            }
            FlushEvents();
        }
    }
}

void UMingRTSTelemetrySystem::StartSession(const FString& PlayerID)
{
    CurrentSessionID = GenerateSessionID();
    CurrentSession.SessionID = CurrentSessionID;
    CurrentSession.PlayerID = PlayerID;
    CurrentSession.StartTime = GetCurrentTimestamp();
    CurrentSession.GameVersion = TEXT("1.0.0");
    CurrentSession.Platform = FPlatformProperties::PlatformName();
    CurrentSession.DeviceModel = TEXT("Unknown");
    CurrentSession.TotalEvents = 0;
    
    // 記錄會話開始事件
    TMap<FString, FString> Params;
    Params.Add(TEXT("platform"), CurrentSession.Platform);
    Params.Add(TEXT("version"), CurrentSession.GameVersion);
    RecordEvent(ETelemetryEventType::SessionStart, TEXT("session_start"), Params);
    
    UE_LOG(LogTemp, Log, TEXT("Telemetry session started: %s for player %s"), *CurrentSessionID, *PlayerID);
}

void UMingRTSTelemetrySystem::EndSession()
{
    if (!CurrentSessionID.IsEmpty())
    {
        // 記錄會話結束事件
        TMap<FString, FString> Params;
        Params.Add(TEXT("duration"), FString::Printf(TEXT("%.0f"), 
            FDateTime::Now().ToUnixTimestamp() - FDateTime::Parse(CurrentSession.StartTime).ToUnixTimestamp()));
        Params.Add(TEXT("total_events"), FString::FromInt(CurrentSession.TotalEvents));
        RecordEvent(ETelemetryEventType::SessionEnd, TEXT("session_end"), Params);
        
        // 發送剩餘事件
        FlushEvents();
        
        CurrentSession.EndTime = GetCurrentTimestamp();
        
        UE_LOG(LogTemp, Log, TEXT("Telemetry session ended: %s, Total events: %d"), 
            *CurrentSessionID, CurrentSession.TotalEvents);
        
        CurrentSessionID.Empty();
    }
}

void UMingRTSTelemetrySystem::RecordEvent(ETelemetryEventType Type, const FString& EventName, 
    const TMap<FString, FString>& Parameters, float Value)
{
    if (!bTelemetryEnabled)
    {
        return;
    }
    
    FTelemetryEvent Event;
    Event.EventID = GenerateEventID();
    Event.EventType = Type;
    Event.Timestamp = GetCurrentTimestamp();
    Event.SessionID = CurrentSessionID;
    Event.PlayerID = CurrentSession.PlayerID;
    Event.EventName = EventName;
    Event.Parameters = Parameters;
    Event.Value = Value;
    
    EventCache.Add(Event);
    CurrentSession.TotalEvents++;
    
    // 如果達到批次大小，立即發送
    if (EventCache.Num() >= BatchSize)
    {
        FlushEvents();
    }
}

void UMingRTSTelemetrySystem::RecordPlayerAction(const FString& ActionName, const FString& Target, float Duration)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("target"), Target);
    Params.Add(TEXT("duration"), FString::Printf(TEXT("%.3f"), Duration));
    RecordEvent(ETelemetryEventType::PlayerAction, ActionName, Params, Duration);
}

void UMingRTSTelemetrySystem::RecordUIInteraction(const FString& ElementName, const FString& InteractionType)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("element"), ElementName);
    Params.Add(TEXT("interaction"), InteractionType);
    RecordEvent(ETelemetryEventType::UIInteraction, TEXT("ui_interaction"), Params);
}

void UMingRTSTelemetrySystem::RecordPerformanceMetrics(const FPerformanceMetrics& Metrics)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("fps"), FString::Printf(TEXT("%.1f"), Metrics.FPS));
    Params.Add(TEXT("frame_time"), FString::Printf(TEXT("%.3f"), Metrics.FrameTime));
    Params.Add(TEXT("memory_mb"), FString::Printf(TEXT("%.1f"), Metrics.MemoryUsageMB));
    Params.Add(TEXT("cpu_load"), FString::Printf(TEXT("%.1f"), Metrics.CPULoad));
    Params.Add(TEXT("gpu_load"), FString::Printf(TEXT("%.1f"), Metrics.GPULoad));
    Params.Add(TEXT("draw_calls"), FString::FromInt(Metrics.DrawCalls));
    Params.Add(TEXT("triangles"), FString::FromInt(Metrics.TriangleCount));
    
    RecordEvent(ETelemetryEventType::Performance, TEXT("performance_metrics"), Params, Metrics.FPS);
}

void UMingRTSTelemetrySystem::RecordError(const FString& ErrorType, const FString& ErrorMessage, const FString& StackTrace)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("error_type"), ErrorType);
    Params.Add(TEXT("message"), ErrorMessage);
    Params.Add(TEXT("stack_trace"), StackTrace);
    
    RecordEvent(ETelemetryEventType::Error, TEXT("error"), Params);
    OnErrorRecorded.Broadcast(ErrorType);
    
    // 錯誤立即發送
    FlushEvents();
    
    UE_LOG(LogTemp, Error, TEXT("Telemetry error recorded: %s - %s"), *ErrorType, *ErrorMessage);
}

void UMingRTSTelemetrySystem::RecordCrash(const FString& CrashType, const FString& CrashLog)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("crash_type"), CrashType);
    Params.Add(TEXT("crash_log"), CrashLog);
    
    RecordEvent(ETelemetryEventType::Crash, TEXT("crash"), Params);
    
    // 崩潰事件立即保存到本地
    FlushEvents();
    
    UE_LOG(LogTemp, Error, TEXT("Telemetry crash recorded: %s"), *CrashType);
}

void UMingRTSTelemetrySystem::RecordFeatureUsage(const FString& FeatureName, int32 UsageCount)
{
    TMap<FString, FString> Params;
    Params.Add(TEXT("feature"), FeatureName);
    Params.Add(TEXT("count"), FString::FromInt(UsageCount));
    
    RecordEvent(ETelemetryEventType::FeatureUsage, TEXT("feature_usage"), Params, (float)UsageCount);
}

void UMingRTSTelemetrySystem::FlushEvents()
{
    if (EventCache.Num() == 0)
    {
        return;
    }
    
    int32 FlushedCount = EventCache.Num();
    
    // 發送到服務器
    SendEventsToServer(EventCache);
    
    // 保存到本地備份
    SaveEventsToLocal(EventCache);
    
    EventCache.Empty();
    
    OnEventsFlushed.Broadcast(FlushedCount);
    
    UE_LOG(LogTemp, Log, TEXT("Telemetry events flushed: %d"), FlushedCount);
}

FString UMingRTSTelemetrySystem::GetCurrentSessionID() const
{
    return CurrentSessionID;
}

int32 UMingRTSTelemetrySystem::GetCachedEventCount() const
{
    return EventCache.Num();
}

void UMingRTSTelemetrySystem::SetBatchSize(int32 Size)
{
    BatchSize = FMath::Max(1, Size);
}

void UMingRTSTelemetrySystem::SetFlushInterval(float Seconds)
{
    FlushInterval = FMath::Max(1.0f, Seconds);
    InitializeFlushTimer();
}

bool UMingRTSTelemetrySystem::ExportRawData(const FString& FilePath) const
{
    FString JsonData = TEXT("[\n");
    
    for (int32 i = 0; i < EventCache.Num(); ++i)
    {
        const FTelemetryEvent& Event = EventCache[i];
        JsonData += FString::Printf(TEXT("  {\n"));
        JsonData += FString::Printf(TEXT("    \"event_id\": \"%s\",\n"), *Event.EventID);
        JsonData += FString::Printf(TEXT("    \"event_type\": %d,\n"), (int32)Event.EventType);
        JsonData += FString::Printf(TEXT("    \"timestamp\": \"%s\",\n"), *Event.Timestamp);
        JsonData += FString::Printf(TEXT("    \"session_id\": \"%s\",\n"), *Event.SessionID);
        JsonData += FString::Printf(TEXT("    \"event_name\": \"%s\",\n"), *Event.EventName);
        JsonData += FString::Printf(TEXT("    \"value\": %.3f\n"), Event.Value);
        JsonData += FString::Printf(TEXT("  }"));
        
        if (i < EventCache.Num() - 1)
        {
            JsonData += TEXT(",\n");
        }
        else
        {
            JsonData += TEXT("\n");
        }
    }
    
    JsonData += TEXT("]\n");
    
    return FFileHelper::SaveStringToFile(JsonData, *FilePath);
}

void UMingRTSTelemetrySystem::ClearCache()
{
    int32 ClearedCount = EventCache.Num();
    EventCache.Empty();
    UE_LOG(LogTemp, Log, TEXT("Telemetry cache cleared: %d events"), ClearedCount);
}

void UMingRTSTelemetrySystem::InitializeFlushTimer()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FlushTimerHandle);
        
        if (bTelemetryEnabled)
        {
            World->GetTimerManager().SetTimer(FlushTimerHandle, this, 
                &UMingRTSTelemetrySystem::OnFlushTimerTick, FlushInterval, true);
        }
    }
}

void UMingRTSTelemetrySystem::OnFlushTimerTick()
{
    FlushEvents();
}

void UMingRTSTelemetrySystem::SendEventsToServer(const TArray<FTelemetryEvent>& Events)
{
    // 這裡應該實現HTTP請求發送到遙測服務器
    // 簡化處理，僅記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Sending %d events to telemetry server"), Events.Num());
}

void UMingRTSTelemetrySystem::SaveEventsToLocal(const TArray<FTelemetryEvent>& Events)
{
    if (Events.Num() == 0)
    {
        return;
    }
    
    FString LogDir = FPaths::ProjectSavedDir() / TEXT("Telemetry");
    IFileManager::Get().MakeDirectory(*LogDir, true);
    
    FString FileName = FString::Printf(TEXT("%s/telemetry_%s.log"), *LogDir, *FDateTime::Now().ToString(TEXT("%Y%m%d")));
    
    FString LogEntry;
    for (const FTelemetryEvent& Event : Events)
    {
        LogEntry += FString::Printf(TEXT("[%s] %s | %s | %.3f\n"), 
            *Event.Timestamp, *Event.EventName, *Event.PlayerID, Event.Value);
    }
    
    FFileHelper::SaveStringToFile(LogEntry, *FileName, FFileHelper::EEncodingOptions::AutoDetect, 
        &IFileManager::Get(), FILEWRITE_Append);
}

FString UMingRTSTelemetrySystem::GenerateEventID()
{
    return FString::Printf(TEXT("EVT_%s_%d"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")), FMath::Rand());
}

FString UMingRTSTelemetrySystem::GenerateSessionID()
{
    return FString::Printf(TEXT("SESS_%s_%d"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")), FMath::Rand());
}

FString UMingRTSTelemetrySystem::GetCurrentTimestamp()
{
    return FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S.%s"));
}
