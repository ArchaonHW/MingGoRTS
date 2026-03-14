#include "TriggerManager.h"
#include "MingWarRTS.h"
#include "IdeaManager.h"
#include "NewEventManager.h" // Changed from EventManager.h

UTriggerManager::UTriggerManager()
{
 PrimaryComponentTick.bCanEverTick = false;
}

void UTriggerManager::InitializeTriggers(UIdeaManager* InIdeaManager, UNewEventManager* InEventManager)
{
 if (InIdeaManager)
 {
  IdeaManagerRef = InIdeaManager;
  // We are commenting this out for now, because OnIdeaUnlockedEvent no longer exists on the simple IdeaManager
  // IdeaManagerRef->OnIdeaUnlockedEvent.AddUObject(this, &UTriggerManager::HandleIdeaUnlocked);
  UE_LOG(LogTemp, Log, TEXT("TriggerManager initialized."));
 }
 else
 {
  UE_LOG(LogTemp, Error, TEXT("TriggerManager failed to initialize: InIdeaManager is null."));
 }

 if (InEventManager)
 {
  EventManagerRef = InEventManager;
 }
 else
 {
  UE_LOG(LogTemp, Error, TEXT("TriggerManager failed to initialize: InEventManager is null."));
 }
}

void UTriggerManager::HandleIdeaUnlocked(FName IdeaID)
{
 // All logic is commented out for now to ensure a clean compile
// UE_LOG(LogTemp, Log, TEXT("HandleIdeaUnlocked triggered with IdeaID: %s"), *IdeaID.ToString());
 // if (IdeaID == FName("idea_is_03"))
 // {
 //  if (EventManagerRef)
 //  {
 //   UE_LOG(LogTemp, Log, TEXT("Condition met. Triggering event..."));
 //   // EventManagerRef->TriggerEvent(TEXT("event_foreign_advisor"));
 //  }
 // }
}



