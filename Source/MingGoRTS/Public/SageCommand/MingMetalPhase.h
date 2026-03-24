// MingMetalPhase.h
// 至聖者指揮學系統 - 金階段實現
// Generated: 2026-03-24 08:28:51
// Task: SAGE-024

#pragma once
#ifndef __MING_INGMING_ETALPHASE_H__
#define __MING_INGMING_ETALPHASE_H__

#include "MingGoRTS.h"
#include "MingMetalPhase.generated.h"

/**
 * 金階段實現
 * Priority: Low
 * Estimated Hours: 8
 */
UCLASS()
class MINGGORTS_API MingMetalPhase : public UObject
{
    GENERATED_BODY()

public:
    MingMetalPhase();
    
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

#endif // __MING_INGMING_ETALPHASE_H__