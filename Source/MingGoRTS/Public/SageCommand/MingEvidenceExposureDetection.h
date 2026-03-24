// MingEvidenceExposureDetection.h
// 至聖者指揮學系統 - 事過露跡徵象
// Generated: 2026-03-24 08:28:52
// Task: SAGE-031

#pragma once
#ifndef __MING_INGEVIDENCEEXPOSUREDETECTION_H__
#define __MING_INGEVIDENCEEXPOSUREDETECTION_H__

#include "MingGoRTS.h"
#include "MingEvidenceExposureDetection.generated.h"

/**
 * 事過露跡徵象
 * Priority: Medium
 * Estimated Hours: 8
 */
UCLASS()
class MINGGORTS_API MingEvidenceExposureDetection : public UObject
{
    GENERATED_BODY()

public:
    MingEvidenceExposureDetection();
    
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

#endif // __MING_INGEVIDENCEEXPOSUREDETECTION_H__