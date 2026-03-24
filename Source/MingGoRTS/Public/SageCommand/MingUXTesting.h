// MingUXTesting.h
// 至聖者指揮學系統 - 用戶體驗測試
// Generated: 2026-03-24 08:28:52
// Task: SAGE-043

#pragma once
#ifndef __MING_INGUXTESTING_H__
#define __MING_INGUXTESTING_H__

#include "MingGoRTS.h"
#include "MingUXTesting.generated.h"

/**
 * 用戶體驗測試
 * Priority: Medium
 * Estimated Hours: 12
 */
UCLASS()
class MINGGORTS_API MingUXTesting : public UObject
{
    GENERATED_BODY()

public:
    MingUXTesting();
    
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

#endif // __MING_INGUXTESTING_H__