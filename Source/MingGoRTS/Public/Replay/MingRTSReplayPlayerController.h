#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayPlayerController.generated.h"

class UMingRTSReplaySystem;

/**
 * ??放??放??制X- ??制??放??放??UI??交?? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSReplayPlayerController : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayPlayerController();
    
    // ???X    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void InitializePlayerController(UMingRTSReplaySystem* ReplaySystem};
    
    // ??放??制
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void Play();
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void Pause();
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void Stop();
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void StepForward();
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void StepBackward();
    
    // ??度??制
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void SetPlaybackSpeed(float Speed};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetPlaybackSpeed() const;
    
    // ????軸控X    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void JumpToTime(float TimeInSeconds};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void JumpToPercentage(float Percentage};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetCurrentTime() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetTotalDuration() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetCurrentPercentage() const;
    
    // ??籤系統
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    void AddBookmark(float Time, const FText& Label};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    void RemoveBookmark(float Time};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    TArray<float> GetBookmarks() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    void JumpToBookmark(int32 BookmarkIndex};
    
    // 事件標??
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Events")
    TArray<FString> GetEventMarkers() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Events")
    void JumpToNextEvent();
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Events")
    void JumpToPreviousEvent();
    
    // 視?X?制
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    void SetFreeCameraMode(bool bFreeCamera};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    bool IsFreeCameraMode() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    void FollowPlayer(const FString& PlayerID};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    void FollowUnit(int32 UnitID};
    
    // 顯示設置
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowUI(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowEventMarkers(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowResourceGraph(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowAPMGraph(bool bVisible};
    
    // 導出??能
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Export")
    void ExportCurrentFrameAsImage(const FString& FilePath};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Export")
    void ExportTimeRangeAsVideo(float StartTime, float EndTime, const FString& FilePath};
    
    // 事件委??
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaybackStarted};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaybackPaused};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlaybackStopped};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, float, NewTime};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedChanged, float, NewSpeed};
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayer|Events")
    FOnPlaybackStarted OnPlaybackStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayer|Events")
    FOnPlaybackPaused OnPlaybackPaused;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayer|Events")
    FOnPlaybackStopped OnPlaybackStopped;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayer|Events")
    FOnTimeChanged OnTimeChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayer|Events")
    FOnSpeedChanged OnSpeedChanged;
    
private:
    UPROPERTY()
    TObjectPtr<UMingRTSReplaySystem> ReplaySystem;
    
    UPROPERTY()
    TArray<float> Bookmarks;
    
    UPROPERTY()
    TMap<float, FText> BookmarkLabels;
    
    UPROPERTY()
    bool bFreeCameraMode;
    
    UPROPERTY()
    bool bShowUI;
    
    UPROPERTY()
    bool bShowEventMarkers;
    
    UPROPERTY()
    float CurrentPlaybackSpeed;
    
    void BindToReplaySystem();
    void UnbindFromReplaySystem();
    void OnReplayTimeChanged(float NewTime};
    void OnReplayFinished();
};

