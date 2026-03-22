#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationExample.generated.h"

/**
 * ?™Â?Á∑®Ë≠ØÁ≥ªÁµ±‰ΩøÁî®Á§∫‰?
 * ÊºîÁ§∫Â¶Ç‰?‰ΩøÁî®MingGoRTS?ÑÂÑ™?ñÁ∑®Ë≠ØÁ≥ªÁµ? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationExample : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationExample(};

    // ?∫Á?‰ΩøÁî®Á§∫‰?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BasicUsageExample(};

    // Âø´ÈÄüÂÑ™?ñÁ§∫‰æ?    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void QuickOptimizationExample(};

    // Ê∑±Â∫¶?™Â?Á§∫‰?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void DeepOptimizationExample(};

    // ?™Â?Áæ©È?ÁΩÆÁ§∫‰æ?    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void CustomConfigurationExample(};

    // ÂØ¶Ê???éßÁ§∫‰?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void RealTimeMonitoringExample(};

    // ?ßËÉΩ?ÜÊ?Á§∫‰?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void PerformanceAnalysisExample(};

    // ÁÆóÊ??™Â?Á§∫‰?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void AlgorithmOptimizationExample(};

    // ?πÈ??ïÁ?Á§∫‰?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BatchProcessingExample(};

    // ?ÜÊ?Â∑•‰?ÊµÅÁ?Á§∫‰?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void IntegratedWorkflowExample(};

    // ?≤Â?‰ΩøÁî®?áÂ?
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    FString GetUsageGuide(};

protected:
    // ?™Â?Á≥ªÁµ±ÁÆ°Á???    UPROPERTY(BlueprintReadOnly, Category = "Optimization Example")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

private:
    // ?ùÂ??ñÁ§∫‰æ?    void InitializeExample(};
    
    // ÊºîÁ§∫ËºîÂä©?πÊ?
    void LogExampleResult(const FString& ExampleName, const FString& Result};
    void ShowConfigurationDetails(const FOptimizationConfiguration& Config};
    void DisplayPerformanceReport(const FString& Report};
};

