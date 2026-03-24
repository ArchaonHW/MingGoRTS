// MingEstablishNationStrategy.h
// 至聖者指揮學系統 - 立國策略(正)
// Generated: 2026-03-24 08:28:51
// Task: SAGE-013

#pragma once
#ifndef __MING_INGESTABLISHNATIONSTRATEGY_H__
#define __MING_INGESTABLISHNATIONSTRATEGY_H__

#include "MingGoRTS.h"
#include "MingEstablishNationStrategy.generated.h"

/**
 * 立國策略(正)
 * Priority: Medium
 * Estimated Hours: 12
 */
UCLASS()
class MINGGORTS_API MingEstablishNationStrategy : public UObject
{
    GENERATED_BODY()

public:
    MingEstablishNationStrategy();
    
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

#endif // __MING_INGESTABLISHNATIONSTRATEGY_H__