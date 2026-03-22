#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSOptimizationCompiler.generated.h"

UENUM(BlueprintType)
enum class EOptimizationLevel : uint8
{
    Debug,          // ?§ÈåØÁ¥öÂà•
    Development,    // ?ãÁôºÁ¥öÂà•
    Release,        // ?ºÂ?Á¥öÂà•
    Performance     // ?ßËÉΩÁ¥öÂà•
};

UENUM(BlueprintType)
enum class ECompilationPhase : uint8
{
    PreProcessing,   // ?êË???    Compilation,     // Á∑®Ë≠Ø
    Optimization,    // ?™Â?
    Linking,         // ??é•
    PostProcessing   // ÂæåË???};

UENUM(BlueprintType)
enum class EOptimizationType : uint8
{
    Speed,           // ?üÂ∫¶?™Â?
    Memory,          // ?ßÂ??™Â?
    Size,            // Â§ßÂ??™Â?
    Power,           // ?üËÄóÂÑ™??    Balanced         // Âπ≥Ë°°?™Â?
};

USTRUCT(BlueprintType)
struct FCompilationTask
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SourceFile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OutputFile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOptimizationLevel OptimizationLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> CompilerFlags;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsParallel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;

    FCompilationTask()
    {
        TaskID = TEXT("");
        SourceFile = TEXT("");
        OutputFile = TEXT("");
        OptimizationLevel = EOptimizationLevel::Development;
        Priority = 1.0f;
        bIsParallel = true;
    }
};

USTRUCT(BlueprintType)
struct FOptimizationMetrics
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CompilationTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CPUUsage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CodeSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptimizedLines;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PerformanceGain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Warnings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Errors;

    FOptimizationMetrics()
    {
        CompilationTime = 0.0f;
        MemoryUsage = 0.0f;
        CPUUsage = 0.0f;
        CodeSize = 0;
        OptimizedLines = 0;
        PerformanceGain = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FOptimizationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TaskID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOptimizationMetrics Metrics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptimizedCode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AppliedOptimizations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> PerformanceImprovements;

    FOptimizationResult()
    {
        TaskID = TEXT("");
        bSuccess = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompilationStarted, const FString&, TaskID, const FCompilationTask&, Task);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompilationCompleted, const FString&, TaskID, const FOptimizationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnOptimizationApplied, const FString&, TaskID, const FString&, OptimizationType, float, PerformanceGain);

/**
 * ?™Â?Á∑®Ë≠Ø?®Á≥ªÁµ?- ?πÈ?Á∑®Ë≠Ø?ÅÈô§?Ø„ÄÅÂÑ™?ñÂ??àÁ??êÂ?Á≥ªÁµ±
 * ?ê‰??∫ËÉΩÁ∑®Ë≠Ø?™Â??Å‰∏¶Ë°åË??ÜÂ??ßËÉΩ?ÜÊ??üËÉΩ
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationCompiler : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationCompiler();

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    void InitializeOptimizationCompiler();

    // ?πÈ?Á∑®Ë≠Ø‰ªªÂ?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    TArray<FOptimizationResult> BatchCompile(const TArray<FCompilationTask>& Tasks);

    // ‰∏¶Ë?Á∑®Ë≠Ø
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    void ParallelCompile(const TArray<FCompilationTask>& Tasks);

    // ?∫ËÉΩ?™Â?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    FOptimizationResult OptimizeCode(const FString& SourceCode, EOptimizationType OptimizationType);

    // ?§ÈåØ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    TArray<FString> DebugAnalyze(const FString& SourceCode, EOptimizationLevel DebugLevel);

    // ?ßËÉΩ?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    FOptimizationMetrics AnalyzePerformance(const FString& SourceCode);

    // ‰ª?¢º?çÊ?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    FString RefactorCode(const FString& SourceCode, const TArray<FString>& RefactoringRules);

    // ?ßÂ??™Â?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    FString OptimizeMemory(const FString& SourceCode);

    // ?üÂ∫¶?™Â?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    FString OptimizeSpeed(const FString& SourceCode);

    // ÁÆóÊ??™Â?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    FString OptimizeAlgorithms(const FString& SourceCode);

    // ‰æùË≥¥?ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    TArray<FString> AnalyzeDependencies(const TArray<FCompilationTask>& Tasks);

    // Á∑®Ë≠Ø?™Â?Âª∫Ë≠∞
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    TArray<FString> GetOptimizationSuggestions(const FString& SourceCode);

    // Ë®≠ÁΩÆ?™Â??ÉÊï∏
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    void SetOptimizationParameters(int32 MaxThreads, float MemoryLimit, EOptimizationLevel DefaultLevel);

    // ?≤Â?Á∑®Ë≠ØÁµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    TMap<FString, FOptimizationMetrics> GetCompilationStatistics() const;

    // Ê∏ÖÁ?Á∑®Ë≠ØÁ∑©Â?
    UFUNCTION(BlueprintCallable, Category = "Optimization Compiler")
    void ClearCompilationCache();

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnCompilationStarted OnCompilationStarted;

    UPROPERTY(BlueprintAssignable)
    FOnCompilationCompleted OnCompilationCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnOptimizationApplied OnOptimizationApplied;

protected:
    // Á∑®Ë≠Ø?ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compilation Parameters")
    int32 MaxThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compilation Parameters")
    float MemoryLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compilation Parameters")
    EOptimizationLevel DefaultOptimizationLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compilation Parameters")
    bool bEnableParallelCompilation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compilation Parameters")
    bool bEnableAdvancedOptimizations;

    // Á∑®Ë≠Ø‰ªªÂ??äÂ?
    UPROPERTY(BlueprintReadOnly, Category = "Compilation Data")
    TArray<FCompilationTask> CompilationQueue;

    // Á∑®Ë≠ØÁµêÊ?Â≠òÂÑ≤
    UPROPERTY(BlueprintReadOnly, Category = "Compilation Data")
    TMap<FString, FOptimizationResult> CompilationResults;

    // Á∑®Ë≠ØÁµ±Ë?
    UPROPERTY(BlueprintReadOnly, Category = "Compilation Data")
    TMap<FString, FOptimizationMetrics> CompilationStatistics;

    // ?™Â?Ë¶èÂ?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization Rules")
    TArray<FString> OptimizationRules;

private:
    // ?ßÈÉ®Á∑®Ë≠Ø?èËºØ
    FOptimizationResult CompileTask(const FCompilationTask& Task);
    void ProcessCompilationQueue();
    void ApplyOptimizations(FString& SourceCode, EOptimizationType OptimizationType);
    
    // ?™Â?ÁÆóÊ?
    FString ApplyLoopOptimizations(const FString& SourceCode);
    FString ApplyMemoryOptimizations(const FString& SourceCode);
    FString ApplyAlgorithmOptimizations(const FString& SourceCode);
    FString ApplyParallelOptimizations(const FString& SourceCode);
    
    // ?§ÈåØÁÆóÊ?
    TArray<FString> PerformStaticAnalysis(const FString& SourceCode);
    TArray<FString> PerformDynamicAnalysis(const FString& SourceCode);
    TArray<FString> PerformMemoryLeakDetection(const FString& SourceCode);
    
    // ?ßËÉΩ?ÜÊ?
    float CalculateComplexity(const FString& SourceCode);
    float EstimateMemoryUsage(const FString& SourceCode);
    float EstimateExecutionTime(const FString& SourceCode);
    
    // ËºîÂä©?ΩÊï∏
    void UpdateCompilationStatistics(const FString& TaskID, const FOptimizationMetrics& Metrics);
    FString GenerateOptimizedCode(const FString& SourceCode, const TArray<FString>& Optimizations);
    bool IsOptimizationApplicable(const FString& SourceCode, const FString& Optimization);
    
    // ‰∏¶Ë??ïÁ?
    void DistributeTasks(const TArray<FCompilationTask>& Tasks);
    TArray<FCompilationTask> GetParallelizableTasks(const TArray<FCompilationTask>& Tasks);
    void MergeCompilationResults(const TArray<FOptimizationResult>& Results);
};
