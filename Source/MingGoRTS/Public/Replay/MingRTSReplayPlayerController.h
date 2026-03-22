#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayPlayerController.generated.h"

class UMingRTSReplaySystem;

/**
 * ?ûÊîæ?≠Êîæ?ßÂà∂??- ?ßÂà∂?ûÊîæ?≠Êîæ?ÑUI?å‰∫§‰∫? */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSReplayPlayerController : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayPlayerController(};
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void InitializePlayerController(UMingRTSReplaySystem* ReplaySystem};
    
    // ?≠Êîæ?ßÂà∂
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void Play(};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void Pause(};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void Stop(};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void StepForward(};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void StepBackward(};
    
    // ?üÂ∫¶?ßÂà∂
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void SetPlaybackSpeed(float Speed};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetPlaybackSpeed() const;
    
    // ?ÇÈ?Ëª∏Êéß??    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void JumpToTime(float TimeInSeconds};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void JumpToPercentage(float Percentage};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetCurrentTime() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetTotalDuration() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetCurrentPercentage() const;
    
    // ?∏Á±§Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    void AddBookmark(float Time, const FText& Label};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    void RemoveBookmark(float Time};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    TArray<float> GetBookmarks() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Bookmarks")
    void JumpToBookmark(int32 BookmarkIndex};
    
    // ‰∫ã‰ª∂Ê®ôË?
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Events")
    TArray<FString> GetEventMarkers() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Events")
    void JumpToNextEvent(};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Events")
    void JumpToPreviousEvent(};
    
    // Ë¶ñË??ßÂà∂
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    void SetFreeCameraMode(bool bFreeCamera};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    bool IsFreeCameraMode() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    void FollowPlayer(const FString& PlayerID};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Camera")
    void FollowUnit(int32 UnitID};
    
    // È°ØÁ§∫Ë®≠ÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowUI(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowEventMarkers(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowResourceGraph(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Display")
    void ShowAPMGraph(bool bVisible};
    
    // Â∞éÂá∫?üËÉΩ
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Export")
    void ExportCurrentFrameAsImage(const FString& FilePath};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer|Export")
    void ExportTimeRangeAsVideo(float StartTime, float EndTime, const FString& FilePath};
    
    // ‰∫ã‰ª∂ÂßîË?
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
    
    void BindToReplaySystem(};
    void UnbindFromReplaySystem(};
    void OnReplayTimeChanged(float NewTime};
    void OnReplayFinished(};
};

