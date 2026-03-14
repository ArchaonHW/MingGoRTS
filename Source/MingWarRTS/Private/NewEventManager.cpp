#include "NewEventManager.h"

UNewEventManager::UNewEventManager()
{
    PrimaryComponentTick.bCanEverTick = true; // Let's enable tick for now
}

void UNewEventManager::BeginPlay()
{
    Super::BeginPlay();
}

void UNewEventManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
