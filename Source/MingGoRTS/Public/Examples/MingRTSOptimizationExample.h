#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationExample.generated.h"

/**
 * ????�sĶ�t�ΨϥΥ�??
 * �t�ܦp??�ϥ�MingGoRTS??�u??�sĶ�t?? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationExample : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationExample();

    // ????�ϥΥ�??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BasicUsageExample();

    // �ֳt�u??��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void QuickOptimizationExample();

    // �`��????��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void DeepOptimizationExample();

    // ????�q??�m��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void CustomConfigurationExample();

    // ��?X??��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void RealTimeMonitoringExample();

    // ??��????��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void PerformanceAnalysisExample();

    // ��?X???��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void AlgorithmOptimizationExample();

    // ???X???��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BatchProcessingExample();

    // ????�u??�y??��??
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void IntegratedWorkflowExample();

    // ????�ϥ�????
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    FString GetUsageGuide();

protected:
    // ????�t�κ�?X
    UPROPERTY(BlueprintReadOnly, Category = "Optimization Example")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

private:
    // ???X?��??
    void InitializeExample();
    
    // �t�ܻ��U????
    void LogExampleResult(const FString& ExampleName, const FString& Result);
    void ShowConfigurationDetails(const FOptimizationConfiguration& Config);
    void DisplayPerformanceReport(const FString& Report);
};
