#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerManager.generated.h"

/**
 * 觸發器管理器
 * 管理所有事件觸發器的註冊、更新和觸發
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerManager : public UObject
{
    GENERATED_BODY()

public:
    UMingEventTriggerManager();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Initialize();

    // 關閉
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Shutdown();

    // 每幀更新
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Tick(float DeltaTime);

    // 註冊觸發器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void RegisterTrigger(UMingEventTrigger* Trigger);

    // 註銷觸發器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void UnregisterTrigger(const FString& TriggerId);

    // 手動觸發
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    bool ManualTrigger(const FString& TriggerId);

    // 獲取觸發器
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    UMingEventTrigger* GetTrigger(const FString& TriggerId) const;

    // 獲取所有觸發器
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    TArray<UMingEventTrigger*> GetAllTriggers() const;

    // 獲取特定類型的觸發器
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    TArray<UMingEventTrigger*> GetTriggersByType(EEventTriggerType Type) const;

    // 啟用/禁用觸發器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void SetTriggerEnabled(const FString& TriggerId, bool bEnabled);

    // 重置觸發器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void ResetTrigger(const FString& TriggerId);

    // 重置所有觸發器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void ResetAllTriggers();

    // 暫停/恢復所有觸發器
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void SetAllTriggersPaused(bool bPaused);

    // 創建觸發器 (工廠方法)
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager", meta = (DeterminesOutputType = "TriggerClass"))
    UMingEventTrigger* CreateTrigger(TSubclassOf<UMingEventTrigger> TriggerClass, const FString& TriggerId, const FString& EventId);

    // 全局事件委託
    UPROPERTY(BlueprintAssignable, Category = "Trigger Manager|Events")
    FOnEventTriggered OnAnyTriggerFired;

    // 靜態獲取實例
    static UMingEventTriggerManager* Get();

private:
    // 註冊的觸發器列表
    UPROPERTY()
    TArray<TObjectPtr<UMingEventTrigger>> Triggers;

    // 觸發器ID到索引的映射
    TMap<FString, int32> TriggerIdMap;

    // 是否已初始化
    bool bInitialized;

    // 是否暫停
    bool bPaused;

    // 單例實例
    static UMingEventTriggerManager* Instance;

    // 內部方法
    void ProcessTriggerTick(float DeltaTime);
    void OnTriggerFired(const FString& EventId);
    void CleanupInvalidTriggers();
};
