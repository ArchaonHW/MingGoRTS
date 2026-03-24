// MingEstablishPeopleStrategy.h
// 至聖者指揮學系統 - 立人策略(正)
// Generated: 2026-03-24 08:28:51
// Task: SAGE-015

#pragma once
#ifndef __MING_INGESTABLISHPEOPLESTRATEGY_H__
#define __MING_INGESTABLISHPEOPLESTRATEGY_H__

#include "MingGoRTS.h"
#include "MingEstablishPeopleStrategy.generated.h"

/**
 * 立人策略(正)
 * Priority: Medium
 * Estimated Hours: 12
 */
UCLASS()
class MINGGORTS_API MingEstablishPeopleStrategy : public UObject
{
    GENERATED_BODY()

public:
    MingEstablishPeopleStrategy();
    
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

#endif // __MING_INGESTABLISHPEOPLESTRATEGY_H__