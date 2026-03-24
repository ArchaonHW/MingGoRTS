// MingBreakStructureStrategy.h
// 至聖者指揮學系統 - 破結構策略(逆)
// Generated: 2026-03-24 08:28:51
// Task: SAGE-017

#pragma once
#ifndef __MING_INGBREAKSTRUCTURESTRATEGY_H__
#define __MING_INGBREAKSTRUCTURESTRATEGY_H__

#include "MingGoRTS.h"
#include "MingBreakStructureStrategy.generated.h"

/**
 * 破結構策略(逆)
 * Priority: Medium
 * Estimated Hours: 16
 */
UCLASS()
class MINGGORTS_API MingBreakStructureStrategy : public UObject
{
    GENERATED_BODY()

public:
    MingBreakStructureStrategy();
    
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

#endif // __MING_INGBREAKSTRUCTURESTRATEGY_H__