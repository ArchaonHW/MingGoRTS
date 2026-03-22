#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEventTrigger.generated.h"

/**
 * 觸發?��X��X */
UENUM(BlueprintType)
enum class EEventTriggerState : uint8
{
    Inactive            UMETA(DisplayName = "Inactive"),
    Active              UMETA(DisplayName = "Active"),
    Triggered           UMETA(DisplayName = "Triggered"),
    Cooldown            UMETA(DisplayName = "Cooldown")
};

/**
 * 觸發?��X��X */
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
 * 事件觸發結�?
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEventTriggered, const FString&, EventId};

/**
 * 事件觸發?�基�? * ?�?�觸?�器?�基礎接X */
UCLASS(Abstract, Blueprintable)
class MINGSTRATEGIC_API UMingEventTrigger : public UObject
{
    GENERATED_BODY()

public:
    UMingEventTrigger(};

    // ?��X�觸?�器
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Initialize(};

    // ?��?觸發X    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Shutdown(};

    // 每�X�新
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Tick(float DeltaTime};

    // 檢查?�否?�以觸發
    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    virtual bool CanTrigger() const;

    // ?��?觸發
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual bool Trigger(};

    // ?�置觸發X    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Reset(};

    // ?�用/禁用
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    void SetEnabled(bool bEnabled};

    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    bool IsEnabled() const { return bEnabled; }

    // ?��?觸發?��X    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    EEventTriggerType GetTriggerType() const { return TriggerType; }

    // ?��X��X�X    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    EEventTriggerState GetCurrentState() const { return CurrentState; }

    // 事件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    FString EventId;

    // 觸發?�ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    FString TriggerId;

    // ?��?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    int32 Priority;

    // ?�否一次性觸X    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    bool bOneShot;

    // ?�卻?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    float CooldownTime;

    // ?�大觸?�次X(-1 表示?��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    int32 MaxTriggerCount;

    // 觸發事件委�?
    UPROPERTY(BlueprintAssignable, Category = "Event Trigger|Events")
    FOnEventTriggered OnTriggered;

protected:
    // 觸發?��X    UPROPERTY()
    EEventTriggerType TriggerType;

    // ?��X�X    UPROPERTY()
    EEventTriggerState CurrentState;

    // ?�否?�用
    UPROPERTY()
    bool bEnabled;

    // ?��X�卻?��?
    UPROPERTY()
    float CurrentCooldown;

    // 觸發次數
    UPROPERTY()
    int32 TriggerCount;

    // ?�新?�卻
    void UpdateCooldown(float DeltaTime};

    // 設置?�X    void SetState(EEventTriggerState NewState};

    // ?�部觸發?�輯 (子�?實現)
    virtual bool PerformTrigger(};

    // 檢查觸發條件 (子�?實現)
    virtual bool CheckTriggerCondition() const;
};

