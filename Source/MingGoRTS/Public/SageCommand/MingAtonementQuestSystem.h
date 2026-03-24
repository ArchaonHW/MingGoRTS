// MingAtonementQuestSystem.h
// 至聖者指揮學系統 - 贖罪任務系統
// Generated: 2026-03-24 08:28:52
// Task: SAGE-032

#pragma once
#ifndef __MING_INGATONEMING_ENTQUESTSYSTEMING__H__
#define __MING_INGATONEMING_ENTQUESTSYSTEMING__H__

#include "MingGoRTS.h"
#include "MingAtonementQuestSystem.generated.h"

/**
 * 贖罪任務系統
 * Priority: Medium
 * Estimated Hours: 12
 */
UCLASS()
class MINGGORTS_API MingAtonementQuestSystem : public UObject
{
    GENERATED_BODY()

public:
    MingAtonementQuestSystem();
    
    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual void Initialize();

    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual bool Execute();

    UFUNCTION(BlueprintCallable, Category = "SageCommand")
    virtual void Shutdown();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bInitialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bActive = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    float DefaultValue = 1.0f;

    virtual void OnInitialize();
    virtual void OnExecute();
    virtual void OnShutdown();

    UFUNCTION(BlueprintPure, Category = "Utility")
    bool IsReady() const { return bInitialized && bActive; }
};

#endif // __MING_INGATONEMING_ENTQUESTSYSTEMING__H__