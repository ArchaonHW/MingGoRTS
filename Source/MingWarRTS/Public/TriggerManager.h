#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
class UIdeaManager;
class UNewEventManager;
#include "NewEventManager.h"
#include "NewEventManager.h"
#include "TriggerManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGWARRTS_API UTriggerManager : public UActorComponent
{
 GENERATED_BODY()

public: 
 UTriggerManager();

 void InitializeTriggers(UIdeaManager* InIdeaManager, UNewEventManager* InEventManager);

private:
 void HandleIdeaUnlocked(FName IdeaID);

 UPROPERTY()
 UIdeaManager* IdeaManagerRef;

 UPROPERTY()
 UNewEventManager* EventManagerRef;
};
