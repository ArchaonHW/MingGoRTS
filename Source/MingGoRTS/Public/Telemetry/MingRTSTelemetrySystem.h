#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSTelemetrySystem.generated.h"

/**
 * ?ôÊ∏¨‰∫ã‰ª∂È°ûÂ?
 */
UENUM(BlueprintType)
enum class ETelemetryEventType : uint8
{
    SessionStart          UMETA(DisplayName = "?ÉË©±?ãÂ?"),
    SessionEnd            UMETA(DisplayName = "?ÉË©±ÁµêÊ?"),
    MatchStart            UMETA(DisplayName = "Â∞çÂ??ãÂ?"),
    MatchEnd              UMETA(DisplayName = "Â∞çÂ?ÁµêÊ?"),
    PlayerAction          UMETA(DisplayName = "?©ÂÆ∂?ç‰?"),
    UIInteraction         UMETA(DisplayName = "UI‰∫§‰?"),
    Error                 UMETA(DisplayName = "?ØË™§"),
    Performance           UMETA(DisplayName = "?ßËÉΩ?∏Ê?"),
    FeatureUsage          UMETA(DisplayName = "?üËÉΩ‰ΩøÁî®"),
    Crash                 UMETA(DisplayName = "Â¥©ÊΩ∞")
};

/**
 * ?ôÊ∏¨‰∫ã‰ª∂
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
 * ?ßËÉΩ?áÊ?
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
 * ?ôÊ∏¨?ÉË©±
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
 * ?ôÊ∏¨Á≥ªÁµ± - ?∂È??åÂ??êÈ??≤È?Ê∏¨Êï∏?? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSTelemetrySystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSTelemetrySystem(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void InitializeTelemetrySystem(};
    
    // ?üÁî®/Á¶ÅÁî®?ôÊ∏¨
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void SetTelemetryEnabled(bool bEnabled};
    
    // ?ãÂ??ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void StartSession(const FString& PlayerID};
    
    // ÁµêÊ??ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void EndSession(};
    
    // Ë®òÈ?‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordEvent(ETelemetryEventType Type, const FString& EventName, const TMap<FString, FString>& Parameters, float Value = 0.0f};
    
    // Ë®òÈ??©ÂÆ∂?ç‰?
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordPlayerAction(const FString& ActionName, const FString& Target, float Duration};
    
    // Ë®òÈ?UI‰∫§‰?
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordUIInteraction(const FString& ElementName, const FString& InteractionType};
    
    // Ë®òÈ??ßËÉΩ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordPerformanceMetrics(const FPerformanceMetrics& Metrics};
    
    // Ë®òÈ??ØË™§
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordError(const FString& ErrorType, const FString& ErrorMessage, const FString& StackTrace};
    
    // Ë®òÈ?Â¥©ÊΩ∞
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordCrash(const FString& CrashType, const FString& CrashLog};
    
    // Ë®òÈ??üËÉΩ‰ΩøÁî®
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void RecordFeatureUsage(const FString& FeatureName, int32 UsageCount};
    
    // ?πÈ??ºÈÄÅ‰?‰ª?    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void FlushEvents(};
    
    // ?≤Â??∂Â??ÉË©±ID
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    FString GetCurrentSessionID() const;
    
    // ?≤Â?Á∑©Â?‰∫ã‰ª∂?∏È?
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    int32 GetCachedEventCount() const;
    
    // Ë®≠ÁΩÆ?πÊ¨°Â§ßÂ?
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void SetBatchSize(int32 Size};
    
    // Ë®≠ÁΩÆ?ºÈÄÅÈ???    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void SetFlushInterval(float Seconds};
    
    // Â∞éÂá∫?üÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    bool ExportRawData(const FString& FilePath) const;
    
    // Ê∏ÖÈô§Á∑©Â?
    UFUNCTION(BlueprintCallable, Category = "Telemetry")
    void ClearCache(};
    
    // ‰∫ã‰ª∂ÂßîË?
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
    
    void InitializeFlushTimer(};
    void OnFlushTimerTick(};
    void SendEventsToServer(const TArray<FTelemetryEvent>& Events};
    void SaveEventsToLocal(const TArray<FTelemetryEvent>& Events};
    FString GenerateEventID(};
    FString GenerateSessionID(};
    FString GetCurrentTimestamp(};
};

