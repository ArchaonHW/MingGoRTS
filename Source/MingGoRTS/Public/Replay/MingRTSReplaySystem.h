#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplaySystem.generated.h"

/**
 * ??放事件類??
 */
UENUM(BlueprintType)
enum class EReplayEventType : uint8
{
    None                    UMETA(DisplayName = "X),
    UnitSpawn               UMETA(DisplayName = "???X???"),
    UnitMove                UMETA(DisplayName = "????移??"),
    UnitAttack              UMETA(DisplayName = "???X???"),
    UnitDeath               UMETA(DisplayName = "????死亡"),
    BuildingConstruct       UMETA(DisplayName = "建??建??),
    BuildingDestroy         UMETA(DisplayName = "建?X???"),
    ResourceGather          UMETA(DisplayName = "資?X???"),
    TechnologyResearch      UMETA(DisplayName = "科?X?究"),
    CommandIssued           UMETA(DisplayName = "??令下??"),
    CameraMove              UMETA(DisplayName = "視??移??"),
    GamePause               UMETA(DisplayName = "??戲????"),
    GameResume              UMETA(DisplayName = "??戲??復"),
    ChatMessage             UMETA(DisplayName = "??天消息"),
    Victory                 UMETA(DisplayName = "??利"),
    Defeat                  UMETA(DisplayName = "失??"),
    Custom                  UMETA(DisplayName = "??????)
};

/**
 * ??放??X */
UENUM(BlueprintType)
enum class EReplayState : uint8
{
    Idle                    UMETA(DisplayName = "??置"),
    Recording               UMETA(DisplayName = "??製??),
    Playing                 UMETA(DisplayName = "??放??),
    Paused                  UMETA(DisplayName = "????"),
    Finished                UMETA(DisplayName = "完??"),
    Error                   UMETA(DisplayName = "??誤")
};

/**
 * ??放幀????
 */
USTRUCT(BlueprintType)
struct FReplayFrame
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FrameNumber;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<uint8> GameStateData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EventIDs;
    
    FReplayFrame()
        : FrameNumber(0)
        , Timestamp(0.0f)
    {}
};

/**
 * ??放事件
 */
USTRUCT(BlueprintType)
struct FReplayEvent
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName EventID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EReplayEventType EventType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FrameNumber;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<uint8> EventData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;
    
    FReplayEvent()
        : EventType(EReplayEventType::None)
        , Timestamp(0.0f)
        , FrameNumber(0)
    {}
};

/**
 * ??放??數X */
USTRUCT(BlueprintType)
struct FReplayMetadata
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ReplayID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MatchID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ReplayName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GameVersion;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CreationDate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalFrames;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalEvents;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Players;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Winner;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FileSize;
    
    FReplayMetadata()
        : Duration(0.0f)
        , TotalFrames(0)
        , TotalEvents(0)
        , FileSize(0)
    {}
};

/**
 * ??放系統 - 記?X??X??X???局
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSReplaySystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplaySystem(};
    
    // ???X    UFUNCTION(BlueprintCallable, Category = "Replay")
    void InitializeReplaySystem(};
    
    // ???X?製
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StartRecording(const FString& MatchID, const FString& MapName};
    
    // ??止??製
    UFUNCTION(BlueprintCallable, Category = "Replay")
    FReplayMetadata StopRecording(};
    
    // ???X?放
    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool StartPlayback(const FString& ReplayID};
    
    // ??止??放
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StopPlayback(};
    
    // ????/??復??放
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void PausePlayback(};
    
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void ResumePlayback(};
    
    // 設置??放??度
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void SetPlaybackSpeed(float Speed};
    
    // 跳?X???定?X    UFUNCTION(BlueprintCallable, Category = "Replay")
    void SeekToTime(float TimeInSeconds};
    
    // 跳?X???定??
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void SeekToFrame(int32 FrameNumber};
    
    // 記??事件
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void RecordEvent(EReplayEventType Type, const FString& PlayerID, const FVector& Location, 
                     const TArray<uint8>& Data, const FString& Description};
    
    // 記??幀????
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void RecordFrame(const TArray<uint8>& GameStateData};
    
    // ???X?放??表
    UFUNCTION(BlueprintCallable, Category = "Replay")
    TArray<FReplayMetadata> GetReplayList() const;
    
    // ??除??放
    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool DeleteReplay(const FString& ReplayID};
    
    // 導出??放
    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool ExportReplay(const FString& ReplayID, const FString& FilePath};
    
    // 導入??放
    UFUNCTION(BlueprintCallable, Category = "Replay")
    FReplayMetadata ImportReplay(const FString& FilePath};
    
    // ???X??X?放??X    UFUNCTION(BlueprintCallable, Category = "Replay")
    EReplayState GetReplayState() const;
    
    // ???X??X?放????
    UFUNCTION(BlueprintCallable, Category = "Replay")
    float GetCurrentPlaybackTime() const;
    
    // ???X??X?放幀
    UFUNCTION(BlueprintCallable, Category = "Replay")
    int32 GetCurrentPlaybackFrame() const;
    
    // ???X?放總?X    UFUNCTION(BlueprintCallable, Category = "Replay")
    float GetTotalDuration() const;
    
    // 事件委??
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReplayStarted};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReplayStopped};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReplayPaused};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaybackFinished};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlaybackTimeChanged, float, NewTime};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReplayEvent, EReplayEventType, EventType, const FReplayEvent&, Event};
    
    UPROPERTY(BlueprintAssignable, Category = "Replay|Events")
    FOnReplayStarted OnReplayStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Replay|Events")
    FOnReplayStopped OnReplayStopped;
    
    UPROPERTY(BlueprintAssignable, Category = "Replay|Events")
    FOnReplayPaused OnReplayPaused;
    
    UPROPERTY(BlueprintAssignable, Category = "Replay|Events")
    FOnPlaybackFinished OnPlaybackFinished;
    
    UPROPERTY(BlueprintAssignable, Category = "Replay|Events")
    FOnPlaybackTimeChanged OnPlaybackTimeChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Replay|Events")
    FOnReplayEvent OnReplayEvent;
    
private:
    UPROPERTY()
    EReplayState CurrentState;
    
    UPROPERTY()
    FString CurrentReplayID;
    
    UPROPERTY()
    FReplayMetadata CurrentMetadata;
    
    UPROPERTY()
    TArray<FReplayFrame> RecordedFrames;
    
    UPROPERTY()
    TArray<FReplayEvent> RecordedEvents;
    
    UPROPERTY()
    float CurrentPlaybackTime;
    
    UPROPERTY()
    float PlaybackSpeed;
    
    UPROPERTY()
    int32 CurrentFrameIndex;
    
    FTimerHandle PlaybackTimerHandle;
    FTimerHandle RecordingTimerHandle;
    
    void StartPlaybackLoop(};
    void StopPlaybackLoop(};
    void PlaybackTick(};
    void RecordingTick(};
    
    FString GenerateReplayID(};
    FString GetReplayDirectory() const;
    bool SaveReplayToFile(const FString& ReplayID};
    bool LoadReplayFromFile(const FString& ReplayID};
    void ProcessFrame(const FReplayFrame& Frame};
    void ProcessEvent(const FReplayEvent& Event};
};