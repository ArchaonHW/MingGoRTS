#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSTelemetrySystem.generated.h"

/**
 * ??測事件類??
 */
UENUM(BlueprintType)
enum class ETelemetryEventType : uint8
{
    SessionStart          UMETA(DisplayName = "??話????"),
    SessionEnd            UMETA(DisplayName = "??話結??"),
    MatchStart            UMETA(DisplayName = "對?X???"),
    MatchEnd              UMETA(DisplayName = "對??結??"),
    PlayerAction          UMETA(DisplayName = "??家????"),
    UIInteraction         UMETA(DisplayName = "UI交??"),
    Error                 UMETA(DisplayName = "??誤"),
    Performance           UMETA(DisplayName = "??能????"),
    FeatureUsage          UMETA(DisplayName = "??能使用"),
    Crash                 UMETA(DisplayName = "崩潰")
};

/**
 * ??測事件
 */
USTRUCT(BlueprintType)
struct FTelemetryEvent
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ETelemetryEventType EventType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Timestamp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SessionID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> Parameters;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Value;
    
    FTelemetryEvent()
        : EventType(ETelemetryEventType::PlayerAction)
        , Value(0.0f)
    {}
};

/**
 * ??能????
 */
USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FrameTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsageMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CPULoad;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GPULoad;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DrawCalls;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriangleCount;
    
    FPerformanceMetrics()
        : FPS(0.0f)
        , FrameTime(0.0f)
        , MemoryUsageMB(0.0f)
        , CPULoad(0.0f)
        , GPULoad(0.0f)
        , DrawCalls(0)
        , TriangleCount(0)
    {}
};

/**
 * ??測??話
 */
USTRUCT(BlueprintType)
struct FTelemetrySession
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SessionID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StartTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GameVersion;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Platform;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DeviceModel;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalEvents;
    
    FTelemetrySession()
        : TotalEvents(0)
    {}
};

/**
 * ??測系統 - ???X??X??X???測數X */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSTelemetrySystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSTelemetrySystem();
    
    // ???X    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void InitializeTelemetrySystem();
    
    // ??用/禁用??測
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void SetTelemetryEnabled(bool bEnabled};
    
    // ???X?話
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void StartSession(const FString& PlayerID};
    
    // 結?X?話
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void EndSession();
    
    // 記??事件
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordEvent(ETelemetryEventType Type, const FString& EventName, const TMap<FString, FString>& Parameters, float Value = 0.0f};
    
    // 記?X?家????
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordPlayerAction(const FString& ActionName, const FString& Target, float Duration};
    
    // 記??UI交??
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordUIInteraction(const FString& ElementName, const FString& InteractionType};
    
    // 記?X?能????
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordPerformanceMetrics(const FPerformanceMetrics& Metrics};
    
    // 記?X?誤
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordError(const FString& ErrorType, const FString& ErrorMessage, const FString& StackTrace};
    
    // 記??崩潰
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordCrash(const FString& CrashType, const FString& CrashLog};
    
    // 記?X?能使用
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordFeatureUsage(const FString& FeatureName, int32 UsageCount};
    
    // ???X?送????    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void FlushEvents();
    
    // ???X??X?話ID
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    FString GetCurrentSessionID() const;
    
    // ????緩??事件????
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    int32 GetCachedEventCount() const;
    
    // 設置??次大??
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void SetBatchSize(int32 Size};
    
    // 設置??送?X    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void SetFlushInterval(float Seconds};
    
    // 導出???X???
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    bool ExportRawData(const FString& FilePath) const;
    
    // 清除緩??
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void ClearCache();
    
    // 事件委??
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTelemetryEnabledChanged, bool, bEnabled};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventsFlushed, int32, EventCount};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnErrorRecorded, const FString&, ErrorType};
    
    UPROPERTY(BlueprintAssignable, Category = "Telemetry|Events")
    FOnTelemetryEnabledChanged OnTelemetryEnabledChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Telemetry|Events")
    FOnEventsFlushed OnEventsFlushed;
    
    UPROPERTY(BlueprintAssignable, Category = "Telemetry|Events")
    FOnErrorRecorded OnErrorRecorded;
    
private:
    UPROPERTY()
    bool bTelemetryEnabled;
    
    UPROPERTY()
    FString CurrentSessionID;
    
    UPROPERTY()
    FTelemetrySession CurrentSession;
    
    UPROPERTY()
    TArray<FTelemetryEvent> EventCache;
    
    UPROPERTY()
    int32 BatchSize;
    
    UPROPERTY()
    float FlushInterval;
    
    FTimerHandle FlushTimerHandle;
    
    void InitializeFlushTimer();
    void OnFlushTimerTick();
    void SendEventsToServer(const TArray<FTelemetryEvent>& Events);
    void SaveEventsToLocal(const TArray<FTelemetryEvent>& Events);
    FString GenerateEventID();
    FString GenerateSessionID();
    FString GetCurrentTimestamp();
};

