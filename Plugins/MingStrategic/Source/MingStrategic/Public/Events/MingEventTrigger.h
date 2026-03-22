#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEventTrigger.generated.h"

/**
 * 觸發器狀態枚舉
 */
UENUM(BlueprintType)
enum class EEventTriggerState : uint8
{
    Inactive            UMETA(DisplayName = "Inactive"),
    Active              UMETA(DisplayName = "Active"),
    Triggered           UMETA(DisplayName = "Triggered"),
    Cooldown            UMETA(DisplayName = "Cooldown")
};

/**
 * 觸發器類型枚舉
 */
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
 * 事件觸發結果
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
 * 事件觸發器基類
 * 所有觸發器的基礎接口
 */
UCLASS(Abstract, Blueprintable)
class MINGSTRATEGIC_API UMingEventTrigger : public UObject
{
    GENERATED_BODY()

public:
    UMingEventTrigger();

    // 初始化觸發器
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Initialize();

    // 關閉觸發器
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Shutdown();

    // 每幀更新
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Tick(float DeltaTime);

    // 檢查是否可以觸發
    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    virtual bool CanTrigger() const;

    // 執行觸發
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual bool Trigger();

    // 重置觸發器
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    virtual void Reset();

    // 啟用/禁用
    UFUNCTION(BlueprintCallable, Category = "Event Trigger")
    void SetEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    bool IsEnabled() const { return bEnabled; }

    // 獲取觸發器類型
    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    EEventTriggerType GetTriggerType() const { return TriggerType; }

    // 獲取當前狀態
    UFUNCTION(BlueprintPure, Category = "Event Trigger")
    EEventTriggerState GetCurrentState() const { return CurrentState; }

    // 事件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    FString EventId;

    // 觸發器ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    FString TriggerId;

    // 優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    int32 Priority;

    // 是否一次性觸發
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    bool bOneShot;

    // 冷卻時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    float CooldownTime;

    // 最大觸發次數 (-1 表示無限制)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event Trigger")
    int32 MaxTriggerCount;

    // 觸發事件委託
    UPROPERTY(BlueprintAssignable, Category = "Event Trigger|Events")
    FOnEventTriggered OnTriggered;

protected:
    // 觸發器類型
    UPROPERTY()
    EEventTriggerType TriggerType;

    // 當前狀態
    UPROPERTY()
    EEventTriggerState CurrentState;

    // 是否啟用
    UPROPERTY()
    bool bEnabled;

    // 當前冷卻時間
    UPROPERTY()
    float CurrentCooldown;

    // 觸發次數
    UPROPERTY()
    int32 TriggerCount;

    // 更新冷卻
    void UpdateCooldown(float DeltaTime);

    // 設置狀態
    void SetState(EEventTriggerState NewState);

    // 內部觸發邏輯 (子類實現)
    virtual bool PerformTrigger();

    // 檢查觸發條件 (子類實現)
    virtual bool CheckTriggerCondition() const;
};
