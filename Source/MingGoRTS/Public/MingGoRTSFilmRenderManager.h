#pragma once

#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Components/ActorComponent.h"
#include "MingGoRTSFilmRenderManager.generated.h"

UENUM(BlueprintType)
enum class EFilmPlaybackState : uint8
{
    Stopped,
    Playing,
    Paused,
    Seeking
};

USTRUCT(BlueprintType)
struct FFilmPlaybackSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback")
    float PlaybackSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback")
    bool bLoop = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback")
    bool bAutoPlay = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback")
    float FadeInDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Playback")
    float FadeOutDuration = 1.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFilmFrameChanged, int32, CurrentFrame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFilmPlaybackStateChanged, EFilmPlaybackState, NewState, int32, CurrentFrame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFilmPlaybackEnded);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingGoRTSFilmRenderManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingGoRTSFilmRenderManager();

    // 影片播放控制
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void PlayFilm();

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void PauseFilm();

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void StopFilm();

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SeekToFrame(int32 FrameIndex);

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SeekToTime(float TimeInSeconds);

    // 影片數據管理
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SetFilmFrames(const TArray<UTexture2D*>& Frames);

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void AddFrame(UTexture2D* NewFrame);

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void ClearFrames();

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    TArray<UTexture2D*> GetFilmFrames() const { return FilmFrames; }

    // 播放狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    EFilmPlaybackState GetPlaybackState() const { return CurrentPlaybackState; }

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    int32 GetCurrentFrame() const { return CurrentFrameIndex; }

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    float GetCurrentTime() const;

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    float GetTotalDuration() const;

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    bool IsPlaying() const { return CurrentPlaybackState == EFilmPlaybackState::Playing; }

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    bool IsPaused() const { return CurrentPlaybackState == EFilmPlaybackState::Paused; }

    // 設置
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SetPlaybackSettings(const FFilmPlaybackSettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    FFilmPlaybackSettings GetPlaybackSettings() const { return PlaybackSettings; }

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SetFrameRate(float NewFrameRate);

    // 渲染輸出
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    UTexture2D* GetCurrentFrameTexture() const;

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void RenderToRenderTarget(class UTextureRenderTarget2D* RenderTarget);

    // 委托事件
    UPROPERTY(BlueprintAssignable, Category = "Film Render")
    FOnFilmFrameChanged OnFilmFrameChanged;

    UPROPERTY(BlueprintAssignable, Category = "Film Render")
    FOnFilmPlaybackStateChanged OnFilmPlaybackStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Film Render")
    FOnFilmPlaybackEnded OnFilmPlaybackEnded;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    TArray<UTexture2D*> FilmFrames;

    UPROPERTY()
    EFilmPlaybackState CurrentPlaybackState;

    UPROPERTY()
    int32 CurrentFrameIndex;

    UPROPERTY()
    float FrameRate;

    UPROPERTY()
    float FrameTimer;

    UPROPERTY()
    FFilmPlaybackSettings PlaybackSettings;

    UPROPERTY()
    FTickerDelegate PlaybackTicker;

    UPROPERTY()
    FDelegateHandle PlaybackTickerHandle;

    void UpdatePlayback(float DeltaTime);
    void AdvanceToNextFrame();
    void HandlePlaybackEnd();
    void NotifyFrameChanged();
    void NotifyPlaybackStateChanged(EFilmPlaybackState NewState);
    bool OnPlaybackTick(float DeltaTime);
};
