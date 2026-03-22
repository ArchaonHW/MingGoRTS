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
    Development,    // ?�發模�? - 快速編譯�X��X��?
    Testing,        // 測試模�? - 平衡?�能?�編譯速度
    Release,        // ?��?模�? - ?�大優X    Performance     // ?�能模�? - 極致?�能?��?
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOptimizationCompleted, const FString&, ProjectName, const FOptimizationConfiguration&, Config};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPerformanceWarning, const FString&, Component, const FString&, Warning, float, Severity};

/**
 * ?��?系統管�X- 統�?管�X�?�優?�編譯系�? * ?��?簡�X�API?�自?��?工�?流�?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSOptimizationSystemManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSOptimizationSystemManager(};

    // 系統?��X    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void InitializeOptimizationSystem(};

    // 設置?��X�置
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void SetOptimizationConfiguration(const FOptimizationConfiguration& Config};

    // ?��X�個�X    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void OptimizeProject(const FString& ProjectPath};

    // ?��?編譯?�目
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void CompileProject(const FString& ProjectPath};

    // ?�能?��X�目
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void AnalyzeProjectPerformance(const FString& ProjectPath};

    // 算�X��X�目
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void OptimizeProjectAlgorithms(const FString& ProjectPath};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    FString GetOptimizationReport(};

    // ?��X�能概覽
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    TMap<EProfilingType, EPerformanceLevel> GetPerformanceOverview(};

    // ?��?實�X��
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void StartRealTimeMonitoring(};

    // ?�止實�X��
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void StopRealTimeMonitoring(};

    // 快速優X    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void QuickOptimize(};

    // 深度?��?
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    void DeepOptimize(};

    // ?��?系統?�X    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    bool IsSystemReady() const;

    // ?��X��X�置
    UFUNCTION(BlueprintCallable, Category = "Optimization System Manager")
    FOptimizationConfiguration GetCurrentConfiguration() const;

    // 事件委�?
    UPROPERTY(BlueprintAssignable)
    FOnOptimizationCompleted OnOptimizationCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnPerformanceWarning OnPerformanceWarning;

protected:
    // ?��?組件
    UPROPERTY(BlueprintReadOnly, Category = "Optimization Components")
    TObjectPtr<UMingRTSOptimizationCompiler> OptimizationCompiler;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Components")
    TObjectPtr<UMingRTSPerformanceProfiler> PerformanceProfiler;

    UPROPERTY(BlueprintReadOnly, Category = "Optimization Components")
    TObjectPtr<UMingRTSAlgorithmOptimizer> AlgorithmOptimizer;

    // 系統?�置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Configuration")
    FOptimizationConfiguration CurrentConfiguration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Configuration")
    bool bSystemInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "System Configuration")
    bool bRealTimeMonitoringActive;

private:
    // ?�部工�?流�?
    void InitializeComponents(};
    void SetupEventHandlers(};
    void ApplyConfigurationToComponents(};
    
    // ?�目?��?
    TArray<FString> AnalyzeProjectStructure(const FString& ProjectPath};
    TArray<FCompilationTask> CreateCompilationTasks(const TArray<FString>& SourceFiles};
    TArray<FString> ExtractAlgorithmsFromProject(const FString& ProjectPath};
    
    // ?��?工�?流�?
    void ExecuteCompilationOptimization(const TArray<FCompilationTask>& Tasks};
    void ExecutePerformanceAnalysis(const FString& ProjectPath};
    void ExecuteAlgorithmOptimization(const TArray<FString>& Algorithms};
    
    // ?��X��?
    FString GenerateComprehensiveReport(};
    void UpdateOptimizationStatistics(};
    
    // 輔助?�數
    EOptimizationLevel GetOptimizationLevelFromMode(EOptimizationMode Mode};
    TArray<EOptimizationTechnique> GetOptimizationTechniquesFromMode(EOptimizationMode Mode};
    void LogOptimizationProgress(const FString& Message};
};

