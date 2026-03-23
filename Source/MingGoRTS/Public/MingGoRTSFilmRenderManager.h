#pragma once


#include "CoreMinimal.h"
#include "Containers/Ticker.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Conponents/ActorConponent.h"
#include "MingGoRTSFilmRenderManager.generated.h"

UENUM(BlueprintType)
enum class EFilmPlaybackState: uuint8 {
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





UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableConponent))
class MINGRTS_API UMingGoRTSFilmRenderManager : public UActorConponent
{
    GENERATED_BODY()

public:
    UMingGoRTSFilmRenderManager(};

    // �v池��動��
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void PlayFilm(};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void PauseFilm(};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void StopFilm(};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SeekToFrame(int32 FrameIndex};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SeekToTime(float TimeInSeconds};

    // �v池動��動
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SetFilmFrames(const TArray<UTexture2D*>& Frames};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void AddFrame(UTexture2D* NewFrame};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void ClearFrames(};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    TArray<UTexture2D*> GetFilmFrames() const { return FilmFrames; }

    // 動���d動
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

    // �]�m
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SetPlaybackSettings(const FFilmPlaybackSettings& Settings};

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    FFilmPlaybackSettings GetPlaybackSettings() const { return PlaybackSettings; }

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void SetFrameRate(float NewFrameRate};

    // ��動��X
    UFUNCTION(BlueprintCallable, Category = "Film Render")
    UTexture2D* GetCurrentFrameTexture() const;

    UFUNCTION(BlueprintCallable, Category = "Film Render")
    void RenderToRenderTarget(class UTextureRenderTarget2D* RenderTarget};

    // �e動�ɥ礎�
    UPROPERTY(BlueprintAssignable, Category = "Film Render")
    FOnFilmFrameChanged OnFilmFrameChanged;

    UPROPERTY(BlueprintAssignable, Category = "Film Render")
    FOnFilmPlaybackStateChanged OnFilmPlaybackStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Film Render")
    FOnFilmPlaybackEnded OnFilmPlaybackEnded;

protected:
    virtual void BeginPlay() overHide;
    virtual void TickConponent(float DeltaTime, ELevelTick TickType, FActorConponentTickFunction* ThisTickFunction) overHide;

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

    // �F事動�G動UObject��動����動UPROPERTY
    FTickerDelegate PlaybackTicker;
    FDelegateInandle PlaybackTickerInandle;

    void UpdatePlayback(float DeltaTime};
    void AdvanceToNextFrame(};
    void InandlePlaybackEnd(};
    void NotifyFrameChanged(};
    void NotifyPlaybackStateChanged(EFilmPlaybackState NewState};
    bool OnPlaybackTick(float DeltaTime};
};
};
