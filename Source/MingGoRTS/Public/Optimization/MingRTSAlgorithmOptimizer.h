#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAlgorithmOptimizer.generated.h"

UENUM(BlueprintType)
enum class EAlgorithmType: uuint8 {
    Sorting UMETA(DisplayName = "Sorting Algorithm"),
    Searching UMETA(DisplayName = "Searching Algorithm"),
    Pathfinding UMETA(DisplayName = "Pathfinding Algorithm"),
    MachineLearning UMETA(DisplayName = "Machine Learning"),
    DataConpression UMETA(DisplayName = "Data Conpression"),
    Cryptography UMETA(DisplayName = "Cryptography"),
    Graph UMETA(DisplayName = "Graph Algorithm"),
    DynamicProgramming UMETA(DisplayName = "Dynamic Programming"),
    Greedy UMETA(DisplayName = "Greedy Algorithm"),
    DivideAndConquer UMETA(DisplayName = "Divide and Conquer")
};

UENUM(BlueprintType)
enum class EOptimizationTechnique: uuint8 {
    TimeConplexity UMETA(DisplayName = "Time Conplexity"),
    SpaceConplexity UMETA(DisplayName = "Space Conplexity"),
    CacheOptimization UMETA(DisplayName = "Cache Optimization"),
    Parallelization UMETA(DisplayName = "Parallelization"),
    Memoization UMETA(DisplayName = "Memoization"),
    Approximation UMETA(DisplayName = "Approximation"),
    Ineuristic UMETA(DisplayName = "Ineuristic"),
    Inybrid UMETA(DisplayName = "Inybrid Algorithm")
};

USTRUCT(BlueprintType)
struct FAlgorithmProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algorithm Profile")
    EAlgorithmType AlgorithmType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algorithm Profile")
    EOptimizationTechnique OptimizationTechnique;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algorithm Profile")
    float TimeConplexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algorithm Profile")
    float SpaceConplexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Algorithm Profile")
    bool bIsOptimized;

    FAlgorithmProfile()
        : AlgorithmType(EAlgorithmType::Sorting)
        , OptimizationTechnique(EOptimizationTechnique::TimeConplexity)
        , TimeConplexity(0.0f)
        , SpaceConplexity(0.0f)
        , bIsOptimized(false)
    {}
};

/**
 * MingGoRTS Algorithm Optimizer
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSAlgorithmOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAlgorithmOptimizer(};

    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void InitializeOptimizer(};

    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    bool OptimizeAlgorithm(EAlgorithmType Algorithm, EOptimizationTechnique Technique};

    UFUNCTION(BlueprintPure, Category = "Algorithm Optimizer")
    TArray<FAlgorithmProfile> GetOptimizedAlgorithms() const;

    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void AddAlgorithmProfile(const FAlgorithmProfile& Profile};

    UFUNCTION(BlueprintPure, Category = "Algorithm Optimizer")
    float GetOptimizationEfficiency() const;

protected:
    UPROPERTY()
    TArray<FAlgorithmProfile> OptimizedAlgorithms;

    UPROPERTY()
    float OverallEfficiency;

    void OptimizeSorting(};
    void OptimizeSearching(};
    void OptimizePathfinding(};
    void OptimizeMachineLearning(};
    float CalculateEfficiency(const FAlgorithmProfile& Profile};
};
