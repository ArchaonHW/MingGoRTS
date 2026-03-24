// MingAIDifficultyCalibration.h
// 至聖者指揮學系統 - AI難度校準
// Generated: 2026-03-24 08:28:52
// Task: SAGE-042

#pragma once
#ifndef __MING_INGAIDIFFICULTYCALIBRATION_H__
#define __MING_INGAIDIFFICULTYCALIBRATION_H__

#include "MingGoRTS.h"
#include "MingAIDifficultyCalibration.generated.h"

/**
 * AI難度校準
 * Priority: High
 * Estimated Hours: 12
 */
UCLASS()
class MINGGORTS_API MingAIDifficultyCalibration : public UObject
{
    GENERATED_BODY()

public:
    MingAIDifficultyCalibration();
    
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

#endif // __MING_INGAIDIFFICULTYCALIBRATION_H__