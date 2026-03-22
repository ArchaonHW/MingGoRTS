#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAlgorithmOptimizer.generated.h"

UENUM(BlueprintType)
enum class EAlgorithmType : uint8
{
    Sorting,         // ?��?算�?
    Searching,       // ?�索算�?
    Pathfinding,     // 路�?規�?
    MachineLearning, // 機器學�?
    DataCompression, // ?��?壓縮
    Cryptography,    // ?��?算�?
    Graph,           // ?��?�?    DynamicProgramming, // ?��?規�?
    Greedy,          // 貪�?算�?
    DivideAndConquer // ?�治算�?
};

UENUM(BlueprintType)
enum class EOptimizationTechnique : uint8
{
    TimeComplexity,   // ?��?複�?度優X    SpaceComplexity,  // 空�?複�?度優X    CacheOptimization, // 緩�X��?
    Parallelization,  // 並�X    Memoization,      // 記憶X    Approximation,    // 近似算�?
    Heuristic,        // ?�發�?    Hybrid           // 混�?算�?
};

USTRUCT(BlueprintType)
struct FAlgorithmProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AlgorithmName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAlgorithmType AlgorithmType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeComplexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpaceComplexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InputSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EOptimizationTechnique> AppliedOptimizations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsOptimized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PerformanceGain;

    FAlgorithmProfile()
    {
        AlgorithmName = TEXT(""};
        AlgorithmType = EAlgorithmType::Sorting;
        TimeComplexity = 0.0f;
        SpaceComplexity = 0.0f;
        InputSize = 0;
        ExecutionTime = 0.0f;
        MemoryUsage = 0.0f;
        bIsOptimized = false;
        PerformanceGain = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FOptimizationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OriginalAlgorithm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptimizedAlgorithm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOptimizationTechnique Technique;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeImprovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpaceImprovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallImprovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> OptimizationSteps;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    FOptimizationResult()
    {
        OriginalAlgorithm = TEXT(""};
        OptimizedAlgorithm = TEXT(""};
        Technique = EOptimizationTechnique::TimeComplexity;
        TimeImprovement = 0.0f;
        SpaceImprovement = 0.0f;
        OverallImprovement = 0.0f;
        bSuccess = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlgorithmOptimized, const FString&, AlgorithmName, const FOptimizationResult&, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOptimizationCompleted, const FString&, AlgorithmName, EOptimizationTechnique, Technique, float, Improvement};

/**
 * 算�X��X�系�?- ?�能算�X��X�優?�系�? * ?��?算�X�能?��X�自?�優?��?建議?�能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAlgorithmOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAlgorithmOptimizer(};

    // 系統?��X    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void InitializeAlgorithmOptimizer(};

    // ?��?算�X�能
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FAlgorithmProfile AnalyzeAlgorithm(const FString& AlgorithmCode, EAlgorithmType AlgorithmType};

    // ?��?算�?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FOptimizationResult OptimizeAlgorithm(const FString& AlgorithmCode, EOptimizationTechnique Technique};

    // ?��X��?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TArray<FOptimizationResult> BatchOptimizeAlgorithms(const TArray<FString>& AlgorithmCodes, const TArray<EOptimizationTechnique>& Techniques};

    // 比�?算�X�能
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<FString, float> CompareAlgorithms(const TArray<FString>& AlgorithmCodes};

    // ?��X��?建議
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TArray<FString> GenerateOptimizationSuggestions(const FString& AlgorithmCode};

    // ?��X��X�佳�?�?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString SelectBestAlgorithm(const TArray<FString>& AlgorithmCodes, EAlgorithmType AlgorithmType};

    // 計�?複�?�?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<FString, float> CalculateComplexity(const FString& AlgorithmCode};

    // ?��X��?結�?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString OptimizeDataStructure(const FString& AlgorithmCode};

    // ?�用記憶X    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString ApplyMemoization(const FString& AlgorithmCode};

    // 並�X��?�?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString ParallelizeAlgorithm(const FString& AlgorithmCode};

    // ?�用?�發�?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString ApplyHeuristics(const FString& AlgorithmCode};

    // ?��?算�?�?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<EAlgorithmType, TArray<FString>> GetAlgorithmLibrary() const;

    // 添�X��?義�?�?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void AddCustomAlgorithm(EAlgorithmType AlgorithmType, const FString& AlgorithmCode};

    // 設置?��X�數
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void SetOptimizationParameters(float TimeWeight, float SpaceWeight, float AccuracyWeight};

    // ?��X��?統�?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<EAlgorithmType, FAlgorithmProfile> GetOptimizationStatistics() const;

    // 事件委�?
    UPROPERTY(BlueprintAssignable)
    FOnAlgorithmOptimized OnAlgorithmOptimized;

    UPROPERTY(BlueprintAssignable)
    FOnOptimizationCompleted OnOptimizationCompleted;

protected:
    // ?��X�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    float TimeWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    float SpaceWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    float AccuracyWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    bool bEnableAdvancedOptimizations;

    // 算�?�?    UPROPERTY(BlueprintReadOnly, Category = "Algorithm Data")
    TMap<EAlgorithmType, TArray<FString>> AlgorithmLibrary;

    // ?��?歷史
    UPROPERTY(BlueprintReadOnly, Category = "Algorithm Data")
    TMap<FString, FAlgorithmProfile> AlgorithmProfiles;

    // ?��?統�?
    UPROPERTY(BlueprintReadOnly, Category = "Algorithm Data")
    TMap<EAlgorithmType, FAlgorithmProfile> OptimizationStatistics;

private:
    // ?�部?��?算�?
    FOptimizationResult OptimizeTimeComplexity(const FString& AlgorithmCode};
    FOptimizationResult OptimizeSpaceComplexity(const FString& AlgorithmCode};
    FOptimizationResult OptimizeCachePerformance(const FString& AlgorithmCode};
    FOptimizationResult OptimizeForParallelization(const FString& AlgorithmCode};
    FOptimizationResult ApplyMemoizationOptimization(const FString& AlgorithmCode};
    FOptimizationResult ApplyApproximationOptimization(const FString& AlgorithmCode};
    FOptimizationResult ApplyHeuristicOptimization(const FString& AlgorithmCode};
    FOptimizationResult ApplyHybridOptimization(const FString& AlgorithmCode};
    
    // 算�X��?
    float AnalyzeTimeComplexity(const FString& AlgorithmCode};
    float AnalyzeSpaceComplexity(const FString& AlgorithmCode};
    float AnalyzeCacheEfficiency(const FString& AlgorithmCode};
    float AnalyzeParallelizability(const FString& AlgorithmCode};
    
    // 複�?度�?�?    float CalculateTimeComplexityInternal(const FString& AlgorithmCode};
    float CalculateSpaceComplexityInternal(const FString& AlgorithmCode};
    float CalculateBigOComplexity(const FString& AlgorithmCode};
    
    // ?��X��?    FString OptimizeLoops(const FString& AlgorithmCode};
    FString OptimizeRecursion(const FString& AlgorithmCode};
    FString OptimizeDataAccess(const FString& AlgorithmCode};
    FString OptimizeConditionalStatements(const FString& AlgorithmCode};
    
    // 輔助?�數
    void UpdateOptimizationStatistics(const FString& AlgorithmName, const FAlgorithmProfile& Profile};
    bool IsAlgorithmOptimizable(const FString& AlgorithmCode, EOptimizationTechnique Technique};
    FString GenerateOptimizedCode(const FString& OriginalCode, const TArray<FString>& Optimizations};
    float CalculatePerformanceGain(const FAlgorithmProfile& Original, const FAlgorithmProfile& Optimized};
    
    // 算�?庫管X    void InitializeAlgorithmLibrary(};
    void LoadStandardAlgorithms(};
    void AddSortingAlgorithms(};
    void AddSearchingAlgorithms(};
    void AddPathfindingAlgorithms(};
    void AddMachineLearningAlgorithms(};
};

