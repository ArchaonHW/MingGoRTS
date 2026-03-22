#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSOptimizationCompiler.generated.h"

UENUM(BlueprintType)
enum class EOptimizationLevel : uint8
{
    Debug, UMETA(DisplayName = "Debug"),
    Development, UMETA(DisplayName = "Development"),
    Release, UMETA(DisplayName = "Release"),
    Performance, UMETA(DisplayName = "Performance")
};

UENUM(BlueprintType)
enum class ECompilationPhase : uint8
{
    PreProcessing, UMETA(DisplayName = "Pre Processing"),
    Compilation, UMETA(DisplayName = "Compilation"),
    Optimization, UMETA(DisplayName = "Optimization"),
    Linking, UMETA(DisplayName = "Linking"),
    PostProcessing, UMETA(DisplayName = "Post Processing")
};

UENUM(BlueprintType)
enum class EOptimizationType : uint8
{
    Speed, UMETA(DisplayName = "Speed"),
    Memory, UMETA(DisplayName = "Memory"),
    Size, UMETA(DisplayName = "Size"),
    Power, UMETA(DisplayName = "Power"),
    Balanced, UMETA(DisplayName = "Balanced")
};

/**
 * MingGoRTS Optimization Compiler
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationCompiler : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationCompiler();

    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    void InitializeCompiler();

    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    bool SetOptimizationLevel(EOptimizationLevel Level);

    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    bool CompileProject();

    UFUNCTION(BlueprintPure, Category = "Optimization Compiler")
    ECompilationPhase GetCurrentPhase() const;

    UFUNCTION(BlueprintPure, Category = "Optimization Compiler")
    EOptimizationLevel GetOptimizationLevel() const;

protected:
    UPROPERTY()
    EOptimizationLevel CurrentOptimizationLevel;

    UPROPERTY()
    ECompilationPhase CurrentPhase;

    void InitializeCompilationPhases();
};
