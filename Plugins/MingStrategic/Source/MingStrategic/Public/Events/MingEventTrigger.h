#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEventTrigger.generated.h"

/**
 * Ëß∏Áôº?®Á??ãÊ??? */
UENUM(BlueprintType)
enum class EEventTriggerState : uint8
{
    Inactive            UMETA(DisplayName = "Inactive"),
    Active              UMETA(DisplayName = "Active"),
    Triggered           UMETA(DisplayName = "Triggered"),
    Cooldown            UMETA(DisplayName = "Cooldown")
};

/**
 * Ëß∏Áôº?®È??ãÊ??? */
UENUM(BlueprintType)
enum class EEventTriggerType : uint8
{
    Base                UMETA(DisplayName = "Base"),
    Time                UMETA(DisplayName = "Time"),
    Condition           UMETA(DisplayName = "Condition"),
    Random              UMETA(DisplayName = "Random"),
    Composite           UMETA(DisplayName = "Composite")
};

/**
 * ‰∫ã‰ª∂Ëß∏ÁôºÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FEventTriggerResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bTriggered;
    
    UPROPERTY(BlueprintReadOnly)
    FString EventId;
    
    UPROPERTY(BlueprintReadOnly)
    FString TriggerData;
    
    FEventTriggerResult()
        : bTriggered(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventTriggered, const FString&, EventId);

/**
 * ‰∫ã‰ª∂Ëß∏Áôº?®Âü∫È°? * ?Ä?âËß∏?ºÂô®?ÑÂü∫Á§éÊé•?? */
UCLASS(Abstract, Blueprintable)
class MINGSTRATEGIC_API UMingEventTrigger : public UObject
{
    GENERATED_BODY()

public:
    UMingEventTrigger();

    // ?ùÂ??ñËß∏?ºÂô®
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Initialize();

    // ?úÈ?Ëß∏Áôº??    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Shutdown();

    // ÊØèÂ??¥Êñ∞
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Tick(float DeltaTime);

    // Ê™¢Êü•?ØÂê¶?Ø‰ª•Ëß∏Áôº
    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    virtual bool CanTrigger() const;

    // ?∑Ë?Ëß∏Áôº
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual bool Trigger();

    // ?çÁΩÆËß∏Áôº??    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Reset();

    // ?üÁî®/Á¶ÅÁî®
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    void SetEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    bool IsEnabled() const { return bEnabled; }

    // ?≤Â?Ëß∏Áôº?®È???    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    EEventTriggerType GetTriggerType() const { return TriggerType; }

    // ?≤Â??∂Â??Ä??    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    EEventTriggerState GetCurrentState() const { return CurrentState; }

    // ‰∫ã‰ª∂ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    FString EventId;

    // Ëß∏Áôº?®ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    FString TriggerId;

    // ?™Â?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    int32 Priority;

    // ?ØÂê¶‰∏ÄÊ¨°ÊÄßËß∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    bool bOneShot;

    // ?∑Âçª?ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    float CooldownTime;

    // ?ÄÂ§ßËß∏?ºÊ¨°??(-1 Ë°®Á§∫?°È???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    int32 MaxTriggerCount;

    // Ëß∏Áôº‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable, Category = "Event Trigger|Events")
    FOnEventTriggered OnTriggered;

protected:
    // Ëß∏Áôº?®È???    UPROPERTY()
    EEventTriggerType TriggerType;

    // ?∂Â??Ä??    UPROPERTY()
    EEventTriggerState CurrentState;

    // ?ØÂê¶?üÁî®
    UPROPERTY()
    bool bEnabled;

    // ?∂Â??∑Âçª?ÇÈ?
    UPROPERTY()
    float CurrentCooldown;

    // Ëß∏ÁôºÊ¨°Êï∏
    UPROPERTY()
    int32 TriggerCount;

    // ?¥Êñ∞?∑Âçª
    void UpdateCooldown(float DeltaTime);

    // Ë®≠ÁΩÆ?Ä??    void SetState(EEventTriggerState NewState);

    // ?ßÈÉ®Ëß∏Áôº?èËºØ (Â≠êÈ?ÂØ¶Áèæ)
    virtual bool PerformTrigger();

    // Ê™¢Êü•Ëß∏ÁôºÊ¢ù‰ª∂ (Â≠êÈ?ÂØ¶Áèæ)
    virtual bool CheckTriggerCondition() const;
};
