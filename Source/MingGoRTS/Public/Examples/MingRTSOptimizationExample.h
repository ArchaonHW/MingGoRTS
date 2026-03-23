#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSOptimizationExample.generated.h"

/**
 * �sĶ�t�ΨϥΥ�動
 * �t�ܦp動�ϥ�MingGoRTS動�u動�sĶ�t動 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSOptimizationExample : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationExample(};

    // �ϥΥ�動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BasicUsageExample(};

    // �ֳt�u動��動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void QuickOptimizationExample(};

    // �F事����動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void DeepOptimizationExample(};

    // �q動�m��動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void CustomConfigurationExample(};

    // ��池ɥr��動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void RealTimeMonitoringExample(};

    // 動����動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void PerformanceAnalysisExample(};

    // ��池動��動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void AlgorithmOptimizationExample(};

    // 目標數量��動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void BatchProcessingExample(};

    // �u動�y動��動
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    void IntegratedɥrorkflowExample(};

    // �ϥ�
    UFUNCTION(BlueprintCallable, Category = "Optimization Example")
    FString GetUsageGuide(};

protected:
    // �t�κ�務
    UPROPERTY(BlueprintReadOnly, Category = "Optimization Example")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

private:
    // 動池��動
    void InitializeExample(};
    
    // �t�ܻ��U
    void LogExampleResult(const FString& ExampleName, const FString& Result};
    void ShowConfigurationDetails(const FOptimizationConfiguration& Config};
    void DisplayPerformanceReport(const FString& Report};
};
