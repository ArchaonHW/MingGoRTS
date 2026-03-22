#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSIntroManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSIntroManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSIntroManager();

    // ?‹é ­?•ç•«?§åˆ¶
    UFUNCTION(BlueprintCallable, Category = "Intro")
    void StartIntroSequence();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void SkipIntro();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    bool IsIntroPlaying() const { return bIsIntroPlaying; }

    // ?•ç•«?Žæ®µ
    UFUNCTION(BlueprintCallable, Category = "Intro")
    void ShowTitleCard();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void ShowHistoricalBackground();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void ShowGameTitle();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void TransitionToMainMenu();

protected:
    UPROPERTY()
    bool bIsIntroPlaying;

    UPROPERTY()
    float IntroTimer;

    UPROPERTY()
    int32 CurrentIntroStage;

    UPROPERTY()
    TArray<class UTexture2D*> IntroTextures;

    UPROPERTY()
    TArray<class USoundBase*> IntroSounds;

private:
    void LoadIntroAssets();
    void PlayIntroSound(int32 StageIndex};
    void DisplayIntroImage(int32 StageIndex};
};

