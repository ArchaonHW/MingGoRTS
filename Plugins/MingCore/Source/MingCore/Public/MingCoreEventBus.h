#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MingCoreEventBus.generated.h"

// ‰∫ã‰ª∂?™Â?Á¥öÊ???(?®Á?ÂÆöÁæ©‰ª•ÊîØ??UHT)
UENUM(BlueprintType)
enum class EEventPriority : uint8
{
    Critical = 0,  // ?Æ‰??∏Ê??ÅÊîª?äÊ?‰ª?    High = 1,      // ?∞Ë?ÁßªÂ??ÅÁ??ãË???    Normal = 2,    // Ë≥áÊ??¥Êñ∞?ÅUI?∑Êñ∞
    Low = 3        // Áµ±Ë??∏Ê??ÅÊó•Ë™åË???};

class MINGCORE_API IMingCoreEventBus
{
public:
    virtual ~IMingCoreEventBus() {}
    
    // ‰∫ã‰ª∂?ºÂ??•Âè£
    template<typename EventType>
    static void PublishEvent(const EventType& Event);
    
    // ‰∫ã‰ª∂Ë®ÇÈñ±?•Âè£
    template<typename EventType>
    static void Subscribe(class UObject* Listener, TFunction<void(const EventType&)> Callback);
    
    // ‰∫ã‰ª∂?ñÊ?Ë®ÇÈñ±
    template<typename EventType>
    static void Unsubscribe(class UObject* Listener);
    
    // ?πË??Ü‰?‰ª∂ÁôºÂ∏ÉÔ??ßËÉΩ?™Â?Ôº?    static void PublishBatchEvents(const TArray<struct FMingCoreEvent>& Events);
};

// ?∫Á?‰∫ã‰ª∂ÁµêÊ?
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

// ?Æ‰??∏Ê?‰∫ã‰ª∂
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

// ?Æ‰?ÁßªÂ?‰∫ã‰ª∂
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

// Ë≥áÊ??¥Êñ∞‰∫ã‰ª∂
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
