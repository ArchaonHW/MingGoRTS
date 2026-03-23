#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCompilationAPI.generated.h"

UENUM(BlueprintType)
enum class ECompilationPhase : uint8
{
    Preprocessing,
    Parsing,
    CodeGeneration,
    Linking,
    Optimization,
    Validation
};

UENUM(BlueprintType)
enum class ECompilationErrorType : uint8
{
    SyntaxError,
    LinkerError,
    RuntimeError,
    Warning,
    DeprecationWarning,
    PerformanceIssue
};

USTRUCT(BlueprintType)
struct FCompilationResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    ECompilationErrorType ErrorType;

    UPROPERTY(BlueprintReadOnly)
    int32 ErrorCode;

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber;

    UPROPERTY(BlueprintReadOnly)
    float CompilationTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Suggestions;

    FCompilationResult()
    {
        bSuccess = false;
        ErrorType = ECompilationErrorType::SyntaxError;
        ErrorCode = 0;
        LineNumber = 0;
        CompilationTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FCompilationContext
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ProjectPath;

    UPROPERTY(BlueprintReadOnly)
    FString TargetConfiguration;

    UPROPERTY(BlueprintReadOnly)
    ECompilationPhase CurrentPhase;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EnvironmentVariables;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> IncludePaths;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> LibraryPaths;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CompilerFlags;

    FCompilationContext()
    {
        CurrentPhase = ECompilationPhase::Preprocessing;
    }
};

/**
 * 智慧編譯API - 具備自我學習能力的編譯系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCompilationAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCompilationAPI();

    // 核心編譯功能
    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    FCompilationResult CompileProject(const FCompilationContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    FCompilationResult CompileFile(const FString& FilePath, const FCompilationContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Compilation API")
    bool ValidateSyntax(const FString& SourceCode, TArray<FString>& OutErrors);

    // 自我學習功能
    UFUNCTION(BlueprintCallable, Category = "Compilation API|Learning")
    void LearnFromCompilationResult(const FCompilationResult& Result, const FString& OriginalCode);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Learning")
    void UpdateErrorPatterns(const ECompilationErrorType ErrorType, const FString& Pattern);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Learning")
    TArray<FString> GetSmartSuggestions(const FCompilationResult& Result);

    // 智慧優化
    UFUNCTION(BlueprintCallable, Category = "Compilation API|Optimization")
    FCompilationResult OptimizeCompilation(const FCompilationContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Optimization")
    void UpdateOptimizationStrategies(const FString& Strategy, float SuccessRate);

    // 跨API調用
    UFUNCTION(BlueprintCallable, Category = "Compilation API|Integration")
    void RequestLogAnalysis(const FString& CompilationLog);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Integration")
    void RequestDebuggingSession(const FCompilationResult& ErrorResult);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Integration")
    void RequestPerformanceAnalysis(const FString& ProjectPath);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompilationStarted, const FCompilationContext&, Context);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompilationProgress, ECompilationPhase, Phase, float, Progress);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompilationCompleted, const FCompilationResult&, Result);

    UPROPERTY(BlueprintAssignable)
    FOnCompilationStarted OnCompilationStarted;

    UPROPERTY(BlueprintAssignable)
    FOnCompilationProgress OnCompilationProgress;

    UPROPERTY(BlueprintAssignable)
    FOnCompilationCompleted OnCompilationCompleted;

private:
    // 學習數據
    UPROPERTY()
    TMap<ECompilationErrorType, TArray<FString>> ErrorPatterns;

    UPROPERTY()
    TMap<FString, float> OptimizationStrategies;

    UPROPERTY()
    TArray<FCompilationResult> HistoricalResults;

    // 內部方法
    void InitializeLearningDatabase();
    void SaveLearningData();
    void LoadLearningData();
    FString AnalyzeErrorPattern(const FCompilationResult& Result);
    bool ApplyLearnedFix(FString& SourceCode, const FString& ErrorPattern);
};
