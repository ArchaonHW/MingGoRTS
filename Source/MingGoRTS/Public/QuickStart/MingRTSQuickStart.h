#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationSystemManager.h"
#include "MingRTSQuickStart.generated.h"

/**
 * MingGoRTS ?ªå?ç·¨è­¯ç³»çµ±å¿«é€Ÿå??? * ç«‹å³?‹å?ä½¿ç”¨?ªå?ç·¨è­¯ç³»çµ±?„ç¤ºä¾? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSQuickStart : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSQuickStart();

    // ç«‹å³?‹å?ä½¿ç”¨ - ä¸€?µå??•å„ª?–ç³»çµ?    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void StartOptimizationNow();

    // ?²å?ç³»çµ±?€??    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    FString GetSystemStatus();

    // å¿«é€Ÿæ€§èƒ½æª¢æŸ¥
    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void QuickPerformanceCheck();

    // ç«‹å³?ªå??¶å??…ç›®
    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    void OptimizeCurrentProject();

    // ?²å??ªå?çµæ?
    UFUNCTION(BlueprintCallable, Category = "Quick Start")
    FString GetOptimizationResults();

protected:
    // ?ªå?ç³»çµ±ç®¡ç???    UPROPERTY(BlueprintReadOnly, Category = "Quick Start")
    TObjectPtr<UMingRTSOptimizationSystemManager> OptimizationManager;

    // ç³»çµ±?€??    UPROPERTY(BlueprintReadOnly, Category = "Quick Start")
    bool bSystemStarted;

    UPROPERTY(BlueprintReadOnly, Category = "Quick Start")
    bool bOptimizationRunning;

private:
    // ?å??–å¿«?Ÿå???    void InitializeQuickStart();
    
    // é¡¯ç¤ºçµæ?
    void DisplayResults(const FString& Results);
};
