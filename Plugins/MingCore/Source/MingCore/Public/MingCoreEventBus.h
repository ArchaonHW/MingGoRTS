#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingCoreEventBus.generated.h"

// 故事重要性(X?w?q?H??XUHT)
UENUM(BlueprintType)
enum class EEventPriority: uint8 {
    Critical = 0,   // 摧毀?? - ?t摧毀???@
    High = 1,       // ???u摧毀 - ???n???M?V??
    Normal = 2,     // ???q?u摧毀 - 摧毀s?BUI??s
    Low = 3         // ?C?u摧毀 - ??p???B??x?O??
};

class MINGCORE_API IMingCoreEventBus
{
public:
    virtual ~IMingCoreEventBus() {}
    
    // ???X?f
    template<typename EventType>
    static void PublishEvent(const EventType& Event) {};
    
    // ???q?\X?f
    template<typename EventType>
    static void Subscribe(class UObject* Listener, TFunction<void(const EventType&)> Callback);
    
    // ???X?q?\
    template<typename EventType>
    static void Unsubscribe(class UObject* Listener);    
    // X??o??X??X
    static void PublishBatchEvents(const TArray<struct FMingCoreEvent>& Events);
};

// X???X
USTRUCT(BlueprintType)
struct MINGCORE_API FMingCoreEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString EventId;
    
    UPROPERTY(BlueprintReadOnly)
    EEventPriority Priority;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
    
    UPROPERTY(BlueprintReadOnly)
    UObject* Source;
    
    FMingCoreEvent()
        : Priority(EEventPriority::Normal)
        , Timestamp(FDateTime::Now())
        , Source(nullptr)
    {
        EventId = FGuid::NewGuid().ToString();
    }
};

// X???
USTRUCT(BlueprintType)
struct MINGCORE_API FUnitSelectedEvent : public FMingCoreEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 UnitId;
    
    UPROPERTY(BlueprintReadOnly)
    FVector2D SelectionPosition;
    
    FUnitSelectedEvent(int32 InUnitId, FVector2D InPosition)
        : UnitId(InUnitId), SelectionPosition(InPosition)
    {
        Priority = EEventPriority::Critical;
    }
};

// X??X???
USTRUCT(BlueprintType)
struct MINGCORE_API FUnitMovedEvent : public FMingCoreEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    int32 UnitId;
    
    UPROPERTY(BlueprintReadOnly)
    FVector TargetPosition;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsAttackMove;
    
    FUnitMovedEvent(int32 InUnitId, FVector InTargetPos, bool bInAttackMove = false)
        : UnitId(InUnitId), TargetPosition(InTargetPos), bIsAttackMove(bInAttackMove)
    {
        Priority = EEventPriority::High;
    }
};

// ??X?s???
USTRUCT(BlueprintType)
struct MINGCORE_API FResourceUpdateEvent : public FMingCoreEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString ResourceType;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Amount;
    
    UPROPERTY(BlueprintReadOnly)
    int32 NewTotal;
    
    FResourceUpdateEvent(const FString& InType, int32 InAmount, int32 InNewTotal)
        : ResourceType(InType), Amount(InAmount), NewTotal(InNewTotal)
    {
        Priority = EEventPriority::Normal;
    }
};

