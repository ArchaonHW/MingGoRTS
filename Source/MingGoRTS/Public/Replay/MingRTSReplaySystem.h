#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplaySystem.generated.h"

/**
 * ?ûÊîæ‰∫ã‰ª∂È°ûÂ?
 */
UENUM(BlueprintType)
enum class EReplayEventType : uint8
{
    None                    UMETA(DisplayName = "??),
    UnitSpawn               UMETA(DisplayName = "?Æ‰??üÊ?"),
    UnitMove                UMETA(DisplayName = "?Æ‰?ÁßªÂ?"),
    UnitAttack              UMETA(DisplayName = "?Æ‰??ªÊ?"),
    UnitDeath               UMETA(DisplayName = "?Æ‰?Ê≠ª‰∫°"),
    BuildingConstruct       UMETA(DisplayName = "Âª∫Á?Âª∫ÈÄ?),
    BuildingDestroy         UMETA(DisplayName = "Âª∫Á??ßÊ?"),
    ResourceGather          UMETA(DisplayName = "Ë≥áÊ??∂È?"),
    TechnologyResearch      UMETA(DisplayName = "ÁßëÊ??îÁ©∂"),
    CommandIssued           UMETA(DisplayName = "?Ω‰ª§‰∏ãÈ?"),
    CameraMove              UMETA(DisplayName = "Ë¶ñË?ÁßªÂ?"),
    GamePause               UMETA(DisplayName = "?äÊà≤?´Â?"),
    GameResume              UMETA(DisplayName = "?äÊà≤?¢Âæ©"),
    ChatMessage             UMETA(DisplayName = "?äÂ§©Ê∂àÊÅØ"),
    Victory                 UMETA(DisplayName = "?ùÂà©"),
    Defeat                  UMETA(DisplayName = "Â§±Ê?"),
    Custom                  UMETA(DisplayName = "?™Â?Áæ?)
};

/**
 * ?ûÊîæ?Ä?? */
UENUM(BlueprintType)
enum class EReplayState : uint8
{
    Idle                    UMETA(DisplayName = "?íÁΩÆ"),
    Recording               UMETA(DisplayName = "?ÑË£Ω‰∏?),
    Playing                 UMETA(DisplayName = "?≠Êîæ‰∏?),
    Paused                  UMETA(DisplayName = "?´Â?"),
    Finished                UMETA(DisplayName = "ÂÆåÊ?"),
    Error                   UMETA(DisplayName = "?ØË™§")
};

/**
 * ?ûÊîæÂπÄ?∏Ê?
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
 * ?ûÊîæ‰∫ã‰ª∂
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
 * ?ûÊîæ?ÉÊï∏?? */
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
 * ?ûÊîæÁ≥ªÁµ± - Ë®òÈ??åÂ??æÈ??≤Â?Â±Ä
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSReplaySystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplaySystem(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Replay")
    void InitializeReplaySystem(};
    
    // ?ãÂ??ÑË£Ω
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StartRecording(const FString& MatchID, const FString& MapName};
    
    // ?úÊ≠¢?ÑË£Ω
    UFUNCTION(BlueprintCallable, Category = "Replay")
    FReplayMetadata StopRecording(};
    
    // ?ãÂ??ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool StartPlayback(const FString& ReplayID};
    
    // ?úÊ≠¢?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void StopPlayback(};
    
    // ?´Â?/?¢Âæ©?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void PausePlayback(};
    
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void ResumePlayback(};
    
    // Ë®≠ÁΩÆ?ûÊîæ?üÂ∫¶
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void SetPlaybackSpeed(float Speed};
    
    // Ë∑≥Ë??∞Ê?ÂÆöÊ???    UFUNCTION(BlueprintCallable, Category = "Replay")
    void SeekToTime(float TimeInSeconds};
    
    // Ë∑≥Ë??∞Ê?ÂÆöÂ?
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void SeekToFrame(int32 FrameNumber};
    
    // Ë®òÈ?‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void RecordEvent(EReplayEventType Type, const FString& PlayerID, const FVector& Location, 
                     const TArray<uint8>& Data, const FString& Description};
    
    // Ë®òÈ?ÂπÄ?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Replay")
    void RecordFrame(const TArray<uint8>& GameStateData};
    
    // ?≤Â??ûÊîæ?óË°®
    UFUNCTION(BlueprintCallable, Category = "Replay")
    TArray<FReplayMetadata> GetReplayList() const;
    
    // ?™Èô§?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool DeleteReplay(const FString& ReplayID};
    
    // Â∞éÂá∫?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "Replay")
    bool ExportReplay(const FString& ReplayID, const FString& FilePath};
    
    // Â∞éÂÖ•?ûÊîæ
    UFUNCTION(BlueprintCallable, Category = "Replay")
    FReplayMetadata ImportReplay(const FString& FilePath};
    
    // ?≤Â??∂Â??ûÊîæ?Ä??    UFUNCTION(BlueprintCallable, Category = "Replay")
    EReplayState GetReplayState() const;
    
    // ?≤Â??∂Â??ûÊîæ?ÇÈ?
    UFUNCTION(BlueprintCallable, Category = "Replay")
    float GetCurrentPlaybackTime() const;
    
    // ?≤Â??∂Â??ûÊîæÂπÄ
    UFUNCTION(BlueprintCallable, Category = "Replay")
    int32 GetCurrentPlaybackFrame() const;
    
    // ?≤Â??ûÊîæÁ∏ΩÊ???    UFUNCTION(BlueprintCallable, Category = "Replay")
    float GetTotalDuration() const;
    
    // ‰∫ã‰ª∂ÂßîË?
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

