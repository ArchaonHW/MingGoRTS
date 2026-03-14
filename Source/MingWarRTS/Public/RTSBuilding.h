#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RTSBuilding.generated.h"

UCLASS()
class MINGWARRTS_API ARTSBuilding : public AActor
{
    GENERATED_BODY()

public:
    ARTSBuilding();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
};