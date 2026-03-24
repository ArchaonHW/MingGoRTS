// MingBreakWithoutHarmStrategy.h
// 至聖者指揮學系統 - 不破人策略(逆)
// Generated: 2026-03-24 08:28:51
// Task: SAGE-018

#pragma once
#ifndef __MING_INGBREAKWITHOUTHARMING_STRATEGY_H__
#define __MING_INGBREAKWITHOUTHARMING_STRATEGY_H__

#include "MingGoRTS.h"
#include "MingBreakWithoutHarmStrategy.generated.h"

/**
 * 不破人策略(逆)
 * Priority: Medium
 * Estimated Hours: 16
 */
UCLASS()
class MINGGORTS_API MingBreakWithoutHarmStrategy : public UObject
{
    GENERATED_BODY()

public:
    MingBreakWithoutHarmStrategy();
    
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

#endif // __MING_INGBREAKWITHOUTHARMING_STRATEGY_H__