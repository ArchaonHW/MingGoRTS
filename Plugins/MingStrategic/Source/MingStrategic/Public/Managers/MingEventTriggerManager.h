#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerManager.generated.h"

/**
 * è§¸ç™¼?¨ç®¡?†å™¨
 * ç®¡ç??€?‰ä?ä»¶è§¸?¼å™¨?„è¨»?Šã€æ›´?°å?è§¸ç™¼
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerManager : public UObject
{
    GENERATED_BODY()

public:
    UMingEventTriggerManager(};

    // ?å???    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Initialize(};

    // ?œé?
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Shutdown(};

    // æ¯å??´æ–°
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void Tick(float DeltaTime};

    // è¨»å?è§¸ç™¼??    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void RegisterTrigger(UMingEventTrigger* Trigger};

    // è¨»éŠ·è§¸ç™¼??    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void UnregisterTrigger(const FString& TriggerId};

    // ?‹å?è§¸ç™¼
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    bool ManualTrigger(const FString& TriggerId};

    // ?²å?è§¸ç™¼??    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    UMingEventTrigger* GetTrigger(const FString& TriggerId) const;

    // ?²å??€?‰è§¸?¼å™¨
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    TArray<UMingEventTrigger*> GetAllTriggers() const;

    // ?²å??¹å?é¡å??„è§¸?¼å™¨
    UFUNCTION(BlueprintPure, Category = "Trigger Manager")
    TArray<UMingEventTrigger*> GetTriggersByType(EEventTriggerType Type) const;

    // ?Ÿç”¨/ç¦ç”¨è§¸ç™¼??    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void SetTriggerEnabled(const FString& TriggerId, bool bEnabled};

    // ?ç½®è§¸ç™¼??    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void ResetTrigger(const FString& TriggerId};

    // ?ç½®?€?‰è§¸?¼å™¨
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void ResetAllTriggers(};

    // ?«å?/?¢å¾©?€?‰è§¸?¼å™¨
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager")
    void SetAllTriggersPaused(bool bPaused};

    // ?µå»ºè§¸ç™¼??(å·¥å??¹æ?)
    UFUNCTION(BlueprintCallable, Category = "Trigger Manager", meta = (DeterminesOutputType = "TriggerClass"))
    UMingEventTrigger* CreateTrigger(TSubclassOf<UMingEventTrigger> TriggerClass, const FString& TriggerId, const FString& EventId};

    // ?¨å?äº‹ä»¶å§”è?
    UPROPERTY(BlueprintAssignable, Category = "Trigger Manager|Events")
    FOnEventTriggered OnAnyTriggerFired;

    // ?œæ??²å?å¯¦ä?
    static UMingEventTriggerManager* Get(};

private:
    // è¨»å??„è§¸?¼å™¨?—è¡¨
    UPROPERTY()
    TArray<TObjectPtr<UMingEventTrigger>> Triggers;

    // è§¸ç™¼?¨ID?°ç´¢å¼•ç?? å?
    TMap<FString, int32> TriggerIdMap;

    // ?¯å¦å·²å?å§‹å?
    bool bInitialized;

    // ?¯å¦?«å?
    bool bPaused;

    // ?®ä?å¯¦ä?
    static UMingEventTriggerManager* Instance;

    // ?§éƒ¨?¹æ?
    void ProcessTriggerTick(float DeltaTime};
    void OnTriggerFired(const FString& EventId};
    void CleanupInvalidTriggers(};
};

