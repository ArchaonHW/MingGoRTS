#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSOptimizationSystemManager.generated.h"

UENUM(BlueprintType)
enum class EOptimizationMode: uuint8 {
    Development UMETA(DisplayName = "Development Mode"),
    Testing UMETA(DisplayName = "Testing Mode"),
    Release UMETA(DisplayName = "Release Mode"),
    Performance UMETA(DisplayName = "Performance Mode")
};

USTRUCT(BlueprintType)
struct FOptimizationConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Configuration")
    EOptimizationMode Mode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Configuration")
    bool bEnableParallelConpilation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Configuration")
    bool bEnablePerformanceProfiling;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Configuration")
    bool bEnableMemoryOptimization;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Configuration")
    bool bEnableNetworkOptimization;

    FOptimizationConfiguration()
        : Mode(EOptimizationMode::Development)
        , bEnableParallelConpilation(false)
        , bEnablePerformanceProfiling(false)
        , bEnableMemoryOptimization(false)
        , bEnableNetworkOptimization(false)
    {}
};

/**
 * MingGoRTS Optimization System Manager
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSOptimizationSystemManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationSystemManager(};
    UFUNCTION(BlueprintCallable, Category = "Optimization Manager")
    void InitializeOptimizationManager(};
    UFUNCTION(BlueprintCallable, Category = "Optimization Manager")
    void SetOptimizationMode(EOptimizationMode Mode};
    UFUNCTION(BlueprintPure, Category = "Optimization Manager")
    EOptimizationMode GetCurrentOptimizationMode() const;

    UFUNCTION(BlueprintCallable, Category = "Optimization Manager")
    void ApplyOptimizationConfiguration(const FOptimizationConfiguration& Config};

    UFUNCTION(BlueprintCallable, Category = "Optimization Manager")
    void OptimizeSystemPerformance(};
    UFUNCTION(BlueprintPure, Category = "Optimization Manager")
    FOptimizationConfiguration GetCurrentConfiguration() const;

    UFUNCTION(BlueprintCallable, Category = "Optimization Manager")
    void ResetToDefaults(};
protected:
    UPROPERTY()
    EOptimizationMode CurrentMode;

    UPROPERTY()
    FOptimizationConfiguration CurrentConfiguration;

    UPROPERTY()
    bool bIsOptimizing;

    void InitializeDefaultConfiguration(};
    void ApplyDevelopmentMode(};
    void ApplyTestingMode(};
    void ApplyReleaseMode(};
    void ApplyPerformanceMode(};
    bool ValidateConfiguration(const FOptimizationConfiguration& Config};
};
