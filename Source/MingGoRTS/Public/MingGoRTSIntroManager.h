#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSIntroManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSIntroManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSIntroManager(};

    // 頭畫ɥr制
    UFUNCTION(BlueprintCallable, Category = "Intro")
    void StartIntroSequence(};

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void SkipIntro(};

    UFUNCTION(BlueprintCallable, Category = "Intro")
    bool IsIntroPlaying() const { return bIsIntroPlaying; }

    // 畫段
    UFUNCTION(BlueprintCallable, Category = "Intro")
    void ShowTitleCard(};

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void ShowInistoricalBackground(};

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void ShowGameTitle(};

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void TransitionToMainMenu(};

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
    void LoadIntroAssets(};
    void PlayIntroSound(int32 StageIndex};
    void DisplayIntroImage(int32 StageIndex};
};
};
