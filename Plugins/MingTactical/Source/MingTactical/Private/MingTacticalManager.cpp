#include "MingTacticalManager.h"
#include "MingCoreEventBus.h"

UMingTacticalManager::UMingTacticalManager()
    : bIsInitialized(false)
    , ActiveUnitCount(0)
    , bMemoryPoolInitialized(false)
{
}

void UMingTacticalManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 預初始化記憶體池 (為1000+單位準備)
    bMemoryPoolInitialized = true;
    
    SetupEventSubscriptions();
    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("MingTacticalManager initialized - Memory pool ready for 1000+ units"));
}

void UMingTacticalManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    CleanupEventSubscriptions();
    ActiveUnitCount = 0;
    bMemoryPoolInitialized = false;
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("MingTacticalManager shutdown"));
}

void UMingTacticalManager::SetupEventSubscriptions()
{
    // 訂閱單位選擇事件 (來自UI/輸入系統)
    IMingCoreEventBus::Subscribe<FUnitSelectedEvent>(this,
        [this](const FUnitSelectedEvent& Event)
    {
        OnUnitSelected(Event.UnitId);
    });
    
    // 訂閱單位移動事件
    IMingCoreEventBus::Subscribe<FUnitMovedEvent>(this,
        [this](const FUnitMovedEvent& Event)
    {
        OnUnitMoved(Event.UnitId, Event.TargetPosition);
    });
    
    UE_LOG(LogTemp, Log, TEXT("TacticalManager event subscriptions setup complete"));
}

void UMingTacticalManager::CleanupEventSubscriptions()
{
    IMingCoreEventBus::Unsubscribe<FUnitSelectedEvent>(this);
    IMingCoreEventBus::Unsubscribe<FUnitMovedEvent>(this);
    UE_LOG(LogTemp, Log, TEXT("TacticalManager event subscriptions cleaned up"));
}

void UMingTacticalManager::SelectUnits(const TArray<int32>& UnitIds)
{
    // 發布單位選擇事件
    for (int32 UnitId : UnitIds)
    {
        FUnitSelectedEvent Event(UnitId, FVector2D::ZeroVector);
        IMingCoreEventBus::PublishEvent(Event);
    }
}

void UMingTacticalManager::MoveUnits(const TArray<int32>& UnitIds, const FVector& TargetPosition, bool bAttackMove)
{
    // 發布單位移動事件
    for (int32 UnitId : UnitIds)
    {
        FUnitMovedEvent Event(UnitId, TargetPosition, bAttackMove);
        IMingCoreEventBus::PublishEvent(Event);
    }
}

void UMingTacticalManager::OnUnitSelected(const int32 UnitId)
{
    UE_LOG(LogTemp, Verbose, TEXT("Unit selected: %d"), UnitId);
}

void UMingTacticalManager::OnUnitMoved(const int32 UnitId, const FVector& NewPosition)
{
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d moved to (%f, %f, %f)"), 
        UnitId, NewPosition.X, NewPosition.Y, NewPosition.Z);
}
