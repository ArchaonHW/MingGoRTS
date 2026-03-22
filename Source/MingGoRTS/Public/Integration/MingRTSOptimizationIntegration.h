#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationIntegration.generated.h"

/**
 * ????�sĶ�t��?????? * �N�u??�sĶ�t��?X?��MingGoRTS�D?X?��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationIntegration();

    // ??��???X??X??X???�t��
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void IntegrateOptimizationSystem();

    // ??��???X??X?�uX
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void StartGameOptimization();

    // ??��???X???���uX
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void RuntimeOptimization();

    // ??�d???X?�uX
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void LevelTransitionOptimization();

    // �O?X?�uX
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void SaveGameOptimization();

    // ???X?�uX
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void LoadGameOptimization();

    // ????�t��??X
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    bool IsOptimizationSystemActive() const;

    // ???X?���??
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    FString GetGamePerformanceStats();

protected:
    // ????�t�κ�?X
    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // �t��??X
    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    bool bSystemIntegrated;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    bool bOptimizationActive;

private:
    // ???X?��
    void SetupGameSpecificOptimizations();
    void ConfigureGameOptimizationParameters();
    void IntegrateWithGameSystems();
    
    // ??��???X???
    void OptimizeGameLoop();
    void OptimizeRendering();
    void OptimizeAI();
    void OptimizeAudio();
    void OptimizePhysics();
    
    // ??��X??
    void MonitorGamePerformance();
    void HandlePerformanceWarnings(const FString& Component, const FString& Warning, float Severity);
    
    // ???X???
    void AutoOptimizeBasedOnPerformance();
    void AdjustQualitySettings();
};


