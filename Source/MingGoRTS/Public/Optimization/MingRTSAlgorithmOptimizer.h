#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAlgorithmOptimizer.generated.h"

UENUM(BlueprintType)
enum class EAlgorithmType : uint8
{
    Sorting,         // ?íÂ?ÁÆóÊ?
    Searching,       // ?úÁ¥¢ÁÆóÊ?
    Pathfinding,     // Ë∑ØÂ?Ë¶èÂ?
    MachineLearning, // Ê©üÂô®Â≠∏Á?
    DataCompression, // ?∏Ê?Â£ìÁ∏Æ
    Cryptography,    // ?†Â?ÁÆóÊ?
    Graph,           // ?ñÁ?Ê≥?    DynamicProgramming, // ?ïÊ?Ë¶èÂ?
    Greedy,          // Ë≤™Â?ÁÆóÊ?
    DivideAndConquer // ?ÜÊ≤ªÁÆóÊ?
};

UENUM(BlueprintType)
enum class EOptimizationTechnique : uint8
{
    TimeComplexity,   // ?ÇÈ?Ë§áÈ?Â∫¶ÂÑ™??    SpaceComplexity,  // Á©∫È?Ë§áÈ?Â∫¶ÂÑ™??    CacheOptimization, // Á∑©Â??™Â?
    Parallelization,  // ‰∏¶Ë???    Memoization,      // Ë®òÊÜ∂??    Approximation,    // Ëøë‰ººÁÆóÊ?
    Heuristic,        // ?üÁôºÂº?    Hybrid           // Ê∑∑Â?ÁÆóÊ?
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
        AlgorithmName = TEXT("");
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
        OriginalAlgorithm = TEXT("");
        OptimizedAlgorithm = TEXT("");
        Technique = EOptimizationTechnique::TimeComplexity;
        TimeImprovement = 0.0f;
        SpaceImprovement = 0.0f;
        OverallImprovement = 0.0f;
        bSuccess = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAlgorithmOptimized, const FString&, AlgorithmName, const FOptimizationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOptimizationCompleted, const FString&, AlgorithmName, EOptimizationTechnique, Technique, float, Improvement);

/**
 * ÁÆóÊ??™Â??®Á≥ªÁµ?- ?∫ËÉΩÁÆóÊ??ÜÊ??åÂÑ™?ñÁ≥ªÁµ? * ?ê‰?ÁÆóÊ??ßËÉΩ?ÜÊ??ÅËá™?ïÂÑ™?ñÂ?Âª∫Ë≠∞?üËÉΩ
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSAlgorithmOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAlgorithmOptimizer();

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void InitializeAlgorithmOptimizer();

    // ?ÜÊ?ÁÆóÊ??ßËÉΩ
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FAlgorithmProfile AnalyzeAlgorithm(const FString& AlgorithmCode, EAlgorithmType AlgorithmType);

    // ?™Â?ÁÆóÊ?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FOptimizationResult OptimizeAlgorithm(const FString& AlgorithmCode, EOptimizationTechnique Technique);

    // ?πÈ??™Â?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TArray<FOptimizationResult> BatchOptimizeAlgorithms(const TArray<FString>& AlgorithmCodes, const TArray<EOptimizationTechnique>& Techniques);

    // ÊØîË?ÁÆóÊ??ßËÉΩ
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<FString, float> CompareAlgorithms(const TArray<FString>& AlgorithmCodes);

    // ?üÊ??™Â?Âª∫Ë≠∞
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TArray<FString> GenerateOptimizationSuggestions(const FString& AlgorithmCode);

    // ?™Â??∏Ê??Ä‰Ω≥Á?Ê≥?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString SelectBestAlgorithm(const TArray<FString>& AlgorithmCodes, EAlgorithmType AlgorithmType);

    // Ë®àÁ?Ë§áÈ?Â∫?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<FString, float> CalculateComplexity(const FString& AlgorithmCode);

    // ?™Â??∏Ê?ÁµêÊ?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString OptimizeDataStructure(const FString& AlgorithmCode);

    // ?âÁî®Ë®òÊÜ∂??    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString ApplyMemoization(const FString& AlgorithmCode);

    // ‰∏¶Ë??ñÁ?Ê≥?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString ParallelizeAlgorithm(const FString& AlgorithmCode);

    // ?âÁî®?üÁôºÂº?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    FString ApplyHeuristics(const FString& AlgorithmCode);

    // ?≤Â?ÁÆóÊ?Â∫?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<EAlgorithmType, TArray<FString>> GetAlgorithmLibrary() const;

    // Ê∑ªÂ??™Â?Áæ©Á?Ê≥?    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void AddCustomAlgorithm(EAlgorithmType AlgorithmType, const FString& AlgorithmCode);

    // Ë®≠ÁΩÆ?™Â??ÉÊï∏
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    void SetOptimizationParameters(float TimeWeight, float SpaceWeight, float AccuracyWeight);

    // ?≤Â??™Â?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Algorithm Optimizer")
    TMap<EAlgorithmType, FAlgorithmProfile> GetOptimizationStatistics() const;

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnAlgorithmOptimized OnAlgorithmOptimized;

    UPROPERTY(BlueprintAssignable)
    FOnOptimizationCompleted OnOptimizationCompleted;

protected:
    // ?™Â??ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    float TimeWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    float SpaceWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    float AccuracyWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Parameters")
    bool bEnableAdvancedOptimizations;

    // ÁÆóÊ?Â∫?    UPROPERTY(BlueprintReadOnly, Category = "Algorithm Data")
    TMap<EAlgorithmType, TArray<FString>> AlgorithmLibrary;

    // ?™Â?Ê≠∑Âè≤
    UPROPERTY(BlueprintReadOnly, Category = "Algorithm Data")
    TMap<FString, FAlgorithmProfile> AlgorithmProfiles;

    // ?™Â?Áµ±Ë?
    UPROPERTY(BlueprintReadOnly, Category = "Algorithm Data")
    TMap<EAlgorithmType, FAlgorithmProfile> OptimizationStatistics;

private:
    // ?ßÈÉ®?™Â?ÁÆóÊ?
    FOptimizationResult OptimizeTimeComplexity(const FString& AlgorithmCode);
    FOptimizationResult OptimizeSpaceComplexity(const FString& AlgorithmCode);
    FOptimizationResult OptimizeCachePerformance(const FString& AlgorithmCode);
    FOptimizationResult OptimizeForParallelization(const FString& AlgorithmCode);
    FOptimizationResult ApplyMemoizationOptimization(const FString& AlgorithmCode);
    FOptimizationResult ApplyApproximationOptimization(const FString& AlgorithmCode);
    FOptimizationResult ApplyHeuristicOptimization(const FString& AlgorithmCode);
    FOptimizationResult ApplyHybridOptimization(const FString& AlgorithmCode);
    
    // ÁÆóÊ??ÜÊ?
    float AnalyzeTimeComplexity(const FString& AlgorithmCode);
    float AnalyzeSpaceComplexity(const FString& AlgorithmCode);
    float AnalyzeCacheEfficiency(const FString& AlgorithmCode);
    float AnalyzeParallelizability(const FString& AlgorithmCode);
    
    // Ë§áÈ?Â∫¶Ë?ÁÆ?    float CalculateTimeComplexityInternal(const FString& AlgorithmCode);
    float CalculateSpaceComplexityInternal(const FString& AlgorithmCode);
    float CalculateBigOComplexity(const FString& AlgorithmCode);
    
    // ?™Â??ÄË°?    FString OptimizeLoops(const FString& AlgorithmCode);
    FString OptimizeRecursion(const FString& AlgorithmCode);
    FString OptimizeDataAccess(const FString& AlgorithmCode);
    FString OptimizeConditionalStatements(const FString& AlgorithmCode);
    
    // ËºîÂä©?ΩÊï∏
    void UpdateOptimizationStatistics(const FString& AlgorithmName, const FAlgorithmProfile& Profile);
    bool IsAlgorithmOptimizable(const FString& AlgorithmCode, EOptimizationTechnique Technique);
    FString GenerateOptimizedCode(const FString& OriginalCode, const TArray<FString>& Optimizations);
    float CalculatePerformanceGain(const FAlgorithmProfile& Original, const FAlgorithmProfile& Optimized);
    
    // ÁÆóÊ?Â∫´ÁÆ°??    void InitializeAlgorithmLibrary();
    void LoadStandardAlgorithms();
    void AddSortingAlgorithms();
    void AddSearchingAlgorithms();
    void AddPathfindingAlgorithms();
    void AddMachineLearningAlgorithms();
};
