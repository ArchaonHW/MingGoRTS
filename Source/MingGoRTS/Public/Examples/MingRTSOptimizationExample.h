#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationExample.generated.h"

/**
 * 摧毀�sĶ�t�ΨϥΥ�??
 * �t�ܦp??�ϥ�MingGoRTS??�u??�sĶ�t?? */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSOptimizationExample : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationExample();

    // 摧毀�ϥΥ�??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BasicUsageExample();

    // �ֳt�u??��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void QuickOptimizationExample();

    // �`��摧毀��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void DeepOptimizationExample();

    // 摧毀�q??�m��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void CustomConfigurationExample();

    // ��?X??��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void RealTimeMonitoringExample();

    // ??��摧毀��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void PerformanceAnalysisExample();

    // ��?X???��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void AlgorithmOptimizationExample();

    // 目標數量��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BatchProcessingExample();

    // 摧毀�u??�y??��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void IntegratedWorkflowExample();

    // 摧毀�ϥ�摧毀
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    FString GetUsageGuide();

protected:
    // 摧毀�t�κ�?X
    UPROPERTY(BlueprintReadOnly, Category = "Optimization Example")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

private:
    // ???X?��??
    void InitializeExample();
    
    // �t�ܻ��U摧毀
    void LogExampleResult(const FString& ExampleName, const FString& Result);
    void ShowConfigurationDetails(const FOptimizationConfiguration& Config);
    void DisplayPerformanceReport(const FString& Report);
};
