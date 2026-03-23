#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSReplayPlayerController.generated.h"

class UMingRTSReplaySystem;

/**
 * 動放動放動制X- 動制動放動放動UI動交動 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableConponent))
class MINGRTS_API UMingRTSReplayPlayerController : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSReplayPlayerController(};
    
    // 動務
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void InitializePlayerController(UMingRTSReplaySystem* ReplaySystem};
    
    // 動放動制
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
    
    // 動d動制
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void SetPlaybackSpeed(float Speed};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetPlaybackSpeed() const;
    
    // 軸控X
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void JumpToTime(float TimeInSeconds};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    void JumpToPercentage(float Percentage};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetCurrentTime() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetTotalDuration() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayer")
    float GetCurrentPercentage() const;
    
    // 動籤系統
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerBookmarks")
    void AddBookmark(float Time, const FText& Label};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerBookmarks")
    void RemoveBookmark(float Time};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerBookmarks")
    TArray<float> GetBookmarks() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerBookmarks")
    void JumpToBookmark(int32 BookmarkIndex};
    
    // 事件標動
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerEvents")
    TArray<FString> GetEventMarkers() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerEvents")
    void JumpToNextEvent(};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerEvents")
    void JumpToPreviousEvent(};
    
    // 視池制
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerCamera")
    void SetFreeCameraMode(bool bFreeCamera};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerCamera")
    bool IsFreeCameraMode() const;
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerCamera")
    void FollowPlayer(const FString& PlayerID};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerCamera")
    void FollowUnit(int32 UnitID};
    
    // 顯示m
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerDisplay")
    void ShowUI(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerDisplay")
    void ShowEventMarkers(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerDisplay")
    void ShowResourceGraph(bool bVisible};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerDisplay")
    void ShowAPMGraph(bool bVisible};
    
    // 導動能
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerExport")
    void ExportCurrentFrameAsImage(const FString& FilePath};
    
    UFUNCTION(BlueprintCallable, Category = "ReplayPlayerExport")
    void ExportTimeRangeAsVideo(float StartTime, float EndTime, const FString& FilePath};
    
    // 事件委動
    
    
    
    
    
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayerEvents")
    FOnPlaybackStarted OnPlaybackStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayerEvents")
    FOnPlaybackPaused OnPlaybackPaused;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayerEvents")
    FOnPlaybackStopped OnPlaybackStopped;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayerEvents")
    FOnTimeChanged OnTimeChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "ReplayPlayerEvents")
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
};


