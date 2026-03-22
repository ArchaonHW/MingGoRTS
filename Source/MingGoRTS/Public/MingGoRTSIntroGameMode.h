#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSIntroGameMode.generated.h"

UCLASS()
class MINGRTS_API AMingGoRTSIntroGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSIntroGameMode();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Intro")
    void StartIntroSequence();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void SkipIntro();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void LoadMainMenu();

    UFUNCTION(BlueprintCallable, Category = "Intro")
    void LoadFirstLevel();

private:
    UPROPERTY()
    class UMingGoRTSIntroManager* IntroManager;

    UPROPERTY()
    class UObject* IntroWidget; // ?�為 UObject*

    UPROPERTY()
    bool bIntroCompleted;

    void SetupIntroManager();
    void SetupIntroWidget();
    void HandleIntroCompleted();
};

