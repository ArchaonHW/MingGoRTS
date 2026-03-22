#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationIntegration.generated.h"

/**
 * ????編譯系統?????? * 將優??編譯系統?X?到MingGoRTS主?X?中
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationIntegration();

    // ??戲???X??X??X???系統
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void IntegrateOptimizationSystem();

    // ??戲???X??X?優X    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void StartGameOptimization();

    // ??戲???X???續優X    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void RuntimeOptimization();

    // ??卡???X?優X    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void LevelTransitionOptimization();

    // 保?X?優X    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void SaveGameOptimization();

    // ???X?優X    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    void LoadGameOptimization();

    // ????系統??X    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    bool IsOptimizationSystemActive() const;

    // ???X?能統??
    UFUNCTION(BlueprintCallable, Category = "Optimization Integration")
    FString GetGamePerformanceStats();

protected:
    // ????系統管?X    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // 系統??X    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    bool bSystemIntegrated;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Integration")
    bool bOptimizationActive;

private:
    // ???X?輯
    void SetupGameSpecificOptimizations();
    void ConfigureGameOptimizationParameters();
    void IntegrateWithGameSystems();
    
    // ??戲???X???
    void OptimizeGameLoop();
    void OptimizeRendering();
    void OptimizeAI();
    void OptimizeAudio();
    void OptimizePhysics();
    
    // ??能X??
    void MonitorGamePerformance();
    void HandlePerformanceWarnings(const FString& Component, const FString& Warning, float Severity};
    
    // ???X???
    void AutoOptimizeBasedOnPerformance();
    void AdjustQualitySettings();
};

