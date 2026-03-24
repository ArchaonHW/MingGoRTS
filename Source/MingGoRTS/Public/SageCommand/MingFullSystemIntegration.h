// MingFullSystemIntegration.h
// 至聖者指揮學系統 - 全系統集成框架
// Generated: 2026-03-24 08:28:52
// Task: SAGE-039

#pragma once
#ifndef __MING_INGFULLSYSTEMING_INTEGRATION_H__
#define __MING_INGFULLSYSTEMING_INTEGRATION_H__

#include "MingGoRTS.h"
#include "MingFullSystemIntegration.generated.h"

/**
 * 全系統集成框架
 * Priority: Critical
 * Estimated Hours: 20
 */
UCLASS()
class MINGGORTS_API MingFullSystemIntegration : public UObject
{
    GENERATED_BODY()

public:
    MingFullSystemIntegration();
    
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

#endif // __MING_INGFULLSYSTEMING_INTEGRATION_H__