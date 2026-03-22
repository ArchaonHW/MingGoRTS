#include "MingStrategicManager.h"
#include "MingCoreEventBus.h"

UMingStrategicManager::UMingStrategicManager()
    : bIsInitialized(false)
{
}

void UMingStrategicManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    SetupEventSubscriptions();
    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("MingStrategicManager initialized"));
}

void UMingStrategicManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    CleanupEventSubscriptions();
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("MingStrategicManager shutdown"));
}

void UMingStrategicManager::SetupEventSubscriptions()
{
    // 訂閱資源更新事件 (來自建築層)
    IMingCoreEventBus::Subscribe<FResourceUpdateEvent>(this, 
        [this](const FResourceUpdateEvent& Event)
    {
        UE_LOG(LogTemp, Verbose, TEXT("Strategic layer received resource update: %s = %d"),
            *Event.ResourceType, Event.NewTotal);
    });
    
    UE_LOG(LogTemp, Log, TEXT("StrategicManager event subscriptions setup complete"));
}

void UMingStrategicManager::CleanupEventSubscriptions()
{
    IMingCoreEventBus::Unsubscribe<FResourceUpdateEvent>(this);
    UE_LOG(LogTemp, Log, TEXT("StrategicManager event subscriptions cleaned up"));
}

void UMingStrategicManager::OnStrategicDecisionReceived(const FString& DecisionId, const FString& Choice)
{
    UE_LOG(LogTemp, Log, TEXT("Strategic decision received: %s = %s"), *DecisionId, *Choice);
    // 處理戰略決策邏輯
}

void UMingStrategicManager::OnDiplomaticEventReceived(const FString& EventType, const FString& TargetFaction)
{
    UE_LOG(LogTemp, Log, TEXT("Diplomatic event: %s with %s"), *EventType, *TargetFaction);
    // 處理外交事件邏輯
}
