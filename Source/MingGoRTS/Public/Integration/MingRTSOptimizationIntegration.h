#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationIntegration.generated.h"

/**
 * ?ªå?ç·¨è­¯ç³»çµ±?†æ?é¡? * å°‡å„ª?–ç·¨è­¯ç³»çµ±é??åˆ°MingGoRTSä¸»é??²ä¸­
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationIntegration();

    // ?Šæˆ²?å??–æ??†æ??ªå?ç³»çµ±
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void IntegrateOptimizationSystem();

    // ?Šæˆ²?‹å??‚å??•å„ª??    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void StartGameOptimization();

    // ?Šæˆ²?‹è??‚æ?çºŒå„ª??    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void RuntimeOptimization();

    // ?œå¡?‡æ??‚å„ª??    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void LevelTransitionOptimization();

    // ä¿å??‚å„ª??    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void SaveGameOptimization();

    // ? è??‚å„ª??    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void LoadGameOptimization();

    // ?²å?ç³»çµ±?€??    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    bool IsOptimizationSystemActive() const;

    // ?²å??§èƒ½çµ±è?
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    FString GetGamePerformanceStats();

protected:
    // ?ªå?ç³»çµ±ç®¡ç???    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // ç³»çµ±?€??    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    bool bSystemIntegrated;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    bool bOptimizationActive;

private:
    // ?†æ??è¼¯
    void SetupGameSpecificOptimizations();
    void ConfigureGameOptimizationParameters();
    void IntegrateWithGameSystems();
    
    // ?Šæˆ²?¹å??ªå?
    void OptimizeGameLoop();
    void OptimizeRendering();
    void OptimizeAI();
    void OptimizeAudio();
    void OptimizePhysics();
    
    // ?§èƒ½??§
    void MonitorGamePerformance();
    void HandlePerformanceWarnings(const FString& Component, const FString& Warning, float Severity);
    
    // ?ªå??ªå?
    void AutoOptimizeBasedOnPerformance();
    void AdjustQualitySettings();
};
