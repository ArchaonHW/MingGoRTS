#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Optimization/MingRTSOptimizationCompiler.h"
#include "Optimization/MingRTSPerformanceProfiler.h"
#include "Optimization/MingRTSAlgorithmOptimizer.h"
#include "MingRTSOptimizationSystemManager.generated.h"

UENUM(BlueprintType)
enum class EOptimizationMode : uint8
{
    Development,    // ?ãÁôºÊ®°Â? - Âø´ÈÄüÁ∑®Ë≠ØÔ??∫Á??™Â?
    Testing,        // Ê∏¨Ë©¶Ê®°Â? - Âπ≥Ë°°?ßËÉΩ?åÁ∑®Ë≠ØÈÄüÂ∫¶
    Release,        // ?ºÂ?Ê®°Â? - ?ÄÂ§ßÂÑ™??    Performance     // ?ßËÉΩÊ®°Â? - Ê•µËá¥?ßËÉΩ?™Â?
};

USTRUCT(BlueprintType)
struct FOptimizationConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOptimizationMode Mode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableParallelCompilation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnablePerformanceProfiling;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableAlgorithmOptimization;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxCompilationThreads;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PerformanceMonitoringInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EOptimizationTechnique> AlgorithmOptimizationTechniques;

    FOptimizationConfiguration()
    {
        Mode = EOptimizationMode::Development;
        bEnableParallelCompilation = true;
        bEnablePerformanceProfiling = true;
        bEnableAlgorithmOptimization = true;
        MaxCompilationThreads = 4;
        PerformanceMonitoringInterval = 1.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOptimizationCompleted, const FString&, ProjectName, const FOptimizationConfiguration&, Config);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPerformanceWarning, const FString&, Component, const FString&, Warning, float, Severity);

/**
 * ?™Â?Á≥ªÁµ±ÁÆ°Á???- Áµ±‰?ÁÆ°Á??Ä?âÂÑ™?ñÁ∑®Ë≠ØÁ≥ªÁµ? * ?ê‰?Á∞°Â??ÑAPI?åËá™?ïÂ?Â∑•‰?ÊµÅÁ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationSystemManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationSystemManager();

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void InitializeOptimizationSystem();

    // Ë®≠ÁΩÆ?™Â??çÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void SetOptimizationConfiguration(const FOptimizationConfiguration& Config);

    // ?™Â??¥ÂÄãÈ???    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void OptimizeProject(const FString& ProjectPath);

    // ?πÈ?Á∑®Ë≠Ø?ÖÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void CompileProject(const FString& ProjectPath);

    // ?ßËÉΩ?ÜÊ??ÖÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void AnalyzeProjectPerformance(const FString& ProjectPath);

    // ÁÆóÊ??™Â??ÖÁõÆ
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void OptimizeProjectAlgorithms(const FString& ProjectPath);

    // ?≤Â??™Â??±Â?
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    FString GetOptimizationReport();

    // ?≤Â??ßËÉΩÊ¶ÇË¶Ω
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    TMap<EProfilingType, EPerformanceLevel> GetPerformanceOverview();

    // ?ãÂ?ÂØ¶Ê???éß
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void StartRealTimeMonitoring();

    // ?úÊ≠¢ÂØ¶Ê???éß
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void StopRealTimeMonitoring();

    // Âø´ÈÄüÂÑ™??    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void QuickOptimize();

    // Ê∑±Â∫¶?™Â?
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void DeepOptimize();

    // ?≤Â?Á≥ªÁµ±?Ä??    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    bool IsSystemReady() const;

    // ?≤Â??∂Â??çÁΩÆ
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    FOptimizationConfiguration GetCurrentConfiguration() const;

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnOptimizationCompleted OnOptimizationCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnPerformanceWarning OnPerformanceWarning;

protected:
    // ?™Â?ÁµÑ‰ª∂
    UPROPERTY(BlueprintReadOnly, Category = "Optimization Components")
    TObjectPtr<UMingRTSOptimizationCompiler> OptimizationCompiler;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Components")
    TObjectPtr<UMingRTSPerformanceProfiler> PerformanceProfiler;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Components")
    TObjectPtr<UMingRTSAlgorithmOptimizer> AlgorithmOptimizer;

    // Á≥ªÁµ±?çÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Configuration")
    FOptimizationConfiguration CurrentConfiguration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Configuration")
    bool bSystemInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Configuration")
    bool bRealTimeMonitoringActive;

private:
    // ?ßÈÉ®Â∑•‰?ÊµÅÁ?
    void InitializeComponents();
    void SetupEventHandlers();
    void ApplyConfigurationToComponents();
    
    // ?ÖÁõÆ?ÜÊ?
    TArray<FString> AnalyzeProjectStructure(const FString& ProjectPath);
    TArray<FCompilationTask> CreateCompilationTasks(const TArray<FString>& SourceFiles);
    TArray<FString> ExtractAlgorithmsFromProject(const FString& ProjectPath);
    
    // ?™Â?Â∑•‰?ÊµÅÁ?
    void ExecuteCompilationOptimization(const TArray<FCompilationTask>& Tasks);
    void ExecutePerformanceAnalysis(const FString& ProjectPath);
    void ExecuteAlgorithmOptimization(const TArray<FString>& Algorithms);
    
    // ?±Â??üÊ?
    FString GenerateComprehensiveReport();
    void UpdateOptimizationStatistics();
    
    // ËºîÂä©?ΩÊï∏
    EOptimizationLevel GetOptimizationLevelFromMode(EOptimizationMode Mode);
    TArray<EOptimizationTechnique> GetOptimizationTechniquesFromMode(EOptimizationMode Mode);
    void LogOptimizationProgress(const FString& Message);
};
