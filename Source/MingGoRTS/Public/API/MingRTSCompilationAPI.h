#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCompilationAPI.generated.h"

UENUM(BlueprintType)
enum class EBuildConfiguration : uint8
{
    Development,
    Debug,
    Shipping,
    Test,
    Editor
};

UENUM(BlueprintType)
enum class EBuildPlatform : uint8
{
    Win64,
    Win32,
    Android,
    IOS,
    Linux,
    Mac
};

UENUM(BlueprintType)
enum class EBuildStatus : uint8
{
    Idle,
    Preparing,
    Compiling,
    Linking,
    Succeeded,
    Failed,
    Cancelled
};

USTRUCT(BlueprintType)
struct FBuildOptions
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EBuildConfiguration Configuration = EBuildConfiguration::Development;

    UPROPERTY(BlueprintReadWrite)
    EBuildPlatform Platform = EBuildPlatform::Win64;

    UPROPERTY(BlueprintReadWrite)
    FString TargetName = TEXT("MingGoRTSEditor");

    UPROPERTY(BlueprintReadWrite)
    FString ProjectPath;

    UPROPERTY(BlueprintReadWrite)
    FString EnginePath;

    UPROPERTY(BlueprintReadWrite)
    bool bCleanBuild = false;

    UPROPERTY(BlueprintReadWrite)
    bool bIncrementalBuild = true;

    UPROPERTY(BlueprintReadWrite)
    bool bNoHotReload = true;

    UPROPERTY(BlueprintReadWrite)
    bool bWaitMutex = true;

    UPROPERTY(BlueprintReadWrite)
    bool bVerbose = false;

    UPROPERTY(BlueprintReadWrite)
    int32 MaxParallelJobs = 0;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> AdditionalFlags;
};

USTRUCT(BlueprintType)
struct FBuildError
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber = 0;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorCode;

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    FString Severity; // Error, Warning, Info
};

USTRUCT(BlueprintType)
struct FBuildResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EBuildStatus Status = EBuildStatus::Idle;

    UPROPERTY(BlueprintReadOnly)
    int32 ExitCode = 0;

    UPROPERTY(BlueprintReadOnly)
    FString OutputLog;

    UPROPERTY(BlueprintReadOnly)
    TArray<FBuildError> Errors;

    UPROPERTY(BlueprintReadOnly)
    TArray<FBuildError> Warnings;

    UPROPERTY(BlueprintReadOnly)
    float BuildTimeSeconds = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    int32 FilesCompiled = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalFiles = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 MemoryUsedMB = 0;
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

    // 項目編譯功能
    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    FBuildResult BuildProject(const FBuildOptions& Options);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    FBuildResult BuildProjectAsync(const FBuildOptions& Options);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    bool CancelBuild();

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    bool CleanBuildCache(bool bCleanIntermediate = true, bool bCleanDerivedData = true, bool bCleanSaved = true);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    bool GenerateProjectFiles();

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    bool ValidateBuildEnvironment(FString& OutErrorMessage);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    TArray<EBuildConfiguration> GetAvailableConfigurations() const;

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    TArray<EBuildPlatform> GetAvailablePlatforms() const;

    UFUNCTION(BlueprintPure, Category = "Compilation API|Build")
    bool IsBuilding() const { return bIsBuilding; }

    UFUNCTION(BlueprintPure, Category = "Compilation API|Build")
    FBuildResult GetLastBuildResult() const { return LastBuildResult; }

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    static FString GetBuildStatusString(EBuildStatus Status);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    static FString GetConfigurationString(EBuildConfiguration Config);

    UFUNCTION(BlueprintCallable, Category = "Compilation API|Build")
    static FString GetPlatformString(EBuildPlatform Platform);

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
    // 編譯狀態
    UPROPERTY()
    bool bIsBuilding = false;

    UPROPERTY()
    bool bShouldCancel = false;

    UPROPERTY()
    FBuildResult LastBuildResult;

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

    // 構建相關內部方法
    FString BuildUBTCommand(const FBuildOptions& Options);
    FBuildResult ExecuteBuildCommand(const FString& Command);
    TArray<FBuildError> ParseBuildErrors(const FString& LogOutput);
    int CalculateBuildProgress(const FString& LogLine, int CurrentProgress) const;
    bool DeleteDirectory(const FString& DirPath);
};
