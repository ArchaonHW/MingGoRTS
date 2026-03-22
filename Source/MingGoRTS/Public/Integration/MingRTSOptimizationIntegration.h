#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationIntegration.generated.h"

/**
 * 摧毀�sĶ�t��摧毀?? * �N�u??�sĶ�t��?X?��MingGoRTS�D?X?��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSOptimizationIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationIntegration();

    // ??��故事選項X???�t��
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void IntegrateOptimizationSystem();

    // ??��故事重要性?�uX
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void StartGameOptimization();

    // ??��目標數量���uX
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

    // 摧毀�t��??X
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    bool IsOptimizationSystemActive() const;

    // ???X?���??
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    FString GetGamePerformanceStats();

protected:
    // 摧毀�t�κ�?X
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
    
    // ??��目標數量
    void OptimizeGameLoop();
    void OptimizeRendering();
    void OptimizeAI();
    void OptimizeAudio();
    void OptimizePhysics();
    
    // ??��X??
    void MonitorGamePerformance();
    void HandlePerformanceWarnings(const FString& Component, const FString& Warning, float Severity);
    
    // 目標數量
    void AutoOptimizeBasedOnPerformance();
    void AdjustQualitySettings();
};


