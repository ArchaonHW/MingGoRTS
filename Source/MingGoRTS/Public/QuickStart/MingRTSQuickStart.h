#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSQuickStart.generated.h"

/**
 * MingGoRTS ?��?編譯系統快速�X * 立即?��?使用?��?編譯系統?�示�? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSQuickStart : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSQuickStart(};

    // 立即?��?使用 - 一?��X�優?�系�?    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void StartOptimizationNow(};

    // ?��?系統?�X    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    FString GetSystemStatus(};

    // 快速性能檢查
    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void QuickPerformanceCheck(};

    // 立即?��X��X�目
    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void OptimizeCurrentProject(};

    // ?��X��?結�?
    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    FString GetOptimizationResults(};

protected:
    // ?��?系統管�X    UPROPERTY(BlueprintReadOnly, Category = "Quick Start")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // 系統?�X    UPROPERTY(BlueprintReadOnly, Category = "Quick Start")
    bool bSystemStarted;

    UPROPERTY(BlueprintReadOnly, Category = "Quick Start")
    bool bOptimizationRunning;

private:
    // ?��X�快?��X    void InitializeQuickStart(};
    
    // 顯示結�?
    void DisplayResults(const FString& Results};
};

