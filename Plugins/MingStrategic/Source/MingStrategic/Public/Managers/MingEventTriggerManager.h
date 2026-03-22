#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerManager.generated.h"

/**
 * 觸發?�管?�器
 * 管�X�?��?件觸?�器?�註?�、更?��?觸發
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerManager : public UObject
{
    GENERATED_BODY()

public:
    UMingEventTriggerManager();

    // ?��X
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Initialize();

    // ?��?
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Shutdown();

    // 每�X�新
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Tick(float DeltaTime};

    // 註�?觸發X
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void RegisterTrigger(UMingEventTrigger* Trigger};

    // 註銷觸發X
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void UnregisterTrigger(const FString& TriggerId};

    // ?��?觸發
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    bool ManualTrigger(const FString& TriggerId};

    // ?��?觸發X
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    UMingEventTrigger* GetTrigger(const FString& TriggerId) const;

    // ?��X�?�觸?�器
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    TArray<UMingEventTrigger*> GetAllTriggers() const;

    // ?��X��?類�X�觸?�器
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    TArray<UMingEventTrigger*> GetTriggersByType(EEventTriggerType Type) const;

    // ?�用/禁用觸發X
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void SetTriggerEnabled(const FString& TriggerId, bool bEnabled};

    // ?�置觸發X
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void ResetTrigger(const FString& TriggerId};

    // ?�置?�?�觸?�器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void ResetAllTriggers();

    // ?��?/?�復?�?�觸?�器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void SetAllTriggersPaused(bool bPaused};

    // ?�建觸發X(工�X��?)
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager", meta = (DeterminesOutputType = "TriggerClass"))
    UMingEventTrigger* CreateTrigger(TSubclassOf<UMingEventTrigger> TriggerClass, const FString& TriggerId, const FString& EventId};

    // ?��?事件委�?
    UPROPERTY(BlueprintAssignable, Category = "Trigger Manager|Events")
    FOnEventTriggered OnAnyTriggerFired;

    // ?��X��?實�?
    static UMingEventTriggerManager* Get();

private:
    // 註�X�觸?�器?�表
    UPROPERTY()
    TArray<TObjectPtr<UMingEventTrigger>> Triggers;

    // 觸發?�ID?�索引�X��?
    TMap<FString, int32> TriggerIdMap;

    // ?�否已�?始�?
    bool bInitialized;

    // ?�否?��?
    bool bPaused;

    // ?��?實�?
    static UMingEventTriggerManager* Instance;

    // ?�部?��?
    void ProcessTriggerTick(float DeltaTime);
    void OnTriggerFired(const FString& EventId);
    void CleanupInvalidTriggers();
};

