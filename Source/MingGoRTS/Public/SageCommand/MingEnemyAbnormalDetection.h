// MingEnemyAbnormalDetection.h
// 至聖者指揮學系統 - 敵轉異常徵象
// Generated: 2026-03-24 08:28:52
// Task: SAGE-030

#pragma once
#ifndef __MING_INGENEMING_YABNORMING_ALDETECTION_H__
#define __MING_INGENEMING_YABNORMING_ALDETECTION_H__

#include "MingGoRTS.h"
#include "MingEnemyAbnormalDetection.generated.h"

/**
 * 敵轉異常徵象
 * Priority: Medium
 * Estimated Hours: 8
 */
UCLASS()
class MINGGORTS_API MingEnemyAbnormalDetection : public UObject
{
    GENERATED_BODY()

public:
    MingEnemyAbnormalDetection();
    
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

#endif // __MING_INGENEMING_YABNORMING_ALDETECTION_H__