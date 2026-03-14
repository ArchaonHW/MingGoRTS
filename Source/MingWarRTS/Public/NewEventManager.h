#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewEventManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGWARRTS_API UNewEventManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UNewEventManager();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
