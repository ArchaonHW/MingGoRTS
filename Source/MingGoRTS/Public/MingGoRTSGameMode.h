#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSGameMode.generated.h"

UCLASS()
class MINGGORTS_API AMingGoRTSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSGameMode();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Game")
    void SpawnInitialUnits();

    UFUNCTION(BlueprintCallable, Category = "Game")
    class AMingGoRTSCamera* GetRTSCamera() const { return RTSCamera; }

private:
    UPROPERTY()
    class AMingGoRTSCamera* RTSCamera;

    UPROPERTY()
    TArray<class AMingGoRTSUnit*> GameUnits;
};
