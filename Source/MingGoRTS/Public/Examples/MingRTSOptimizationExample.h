#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationExample.generated.h"

/**
 * ????編譯系統使用示??
 * 演示如??使用MingGoRTS??優??編譯系?? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationExample : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationExample(};

    // ????使用示??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BasicUsageExample(};

    // 快速優??示??    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void QuickOptimizationExample(};

    // 深度????示??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void DeepOptimizationExample(};

    // ????義??置示??    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void CustomConfigurationExample(};

    // 實?X??示??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void RealTimeMonitoringExample(};

    // ??能????示??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void PerformanceAnalysisExample(};

    // 算?X???示??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void AlgorithmOptimizationExample(};

    // ???X???示??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BatchProcessingExample(};

    // ????工??流??示??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void IntegratedWorkflowExample(};

    // ????使用????
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    FString GetUsageGuide(};

protected:
    // ????系統管?X    UPROPERTY(BlueprintReadOnly, Category = "Optimization Example")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

private:
    // ???X?示??    void InitializeExample(};
    
    // 演示輔助????
    void LogExampleResult(const FString& ExampleName, const FString& Result};
    void ShowConfigurationDetails(const FOptimizationConfiguration& Config};
    void DisplayPerformanceReport(const FString& Report};
};