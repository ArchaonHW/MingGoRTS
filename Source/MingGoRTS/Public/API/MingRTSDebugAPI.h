#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDebugAPI.generated.h"

UENUM(BlueprintType)
enum class EDebugMode : uint8
{
    Step,
    Breakpoint,
    Trace,
    Memory,
    Performance,
    Network,
    AI
};

UENUM(BlueprintType)
enum class EBreakpointType : uint8
{
    CodeLine,
    Function,
    Memory,
    Condition,
    Exception
};

USTRUCT(BlueprintType)
struct FDebugBreakpoint
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber;

    UPROPERTY(BlueprintReadOnly)
    EBreakpointType Type;

    UPROPERTY(BlueprintReadOnly)
    FString Condition;

    UPROPERTY(BlueprintReadOnly)
    bool bEnabled;

    UPROPERTY(BlueprintReadOnly)
    int32 HitCount;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    FDebugBreakpoint()
    {
        LineNumber = 0;
        Type = EBreakpointType::CodeLine;
        bEnabled = true;
        HitCount = 0;
    }
};

USTRUCT(BlueprintType)
struct FDebugSession
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SessionId;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly)
    EDebugMode Mode;

    UPROPERTY(BlueprintReadOnly)
    TArray<FDebugBreakpoint> Breakpoints;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CallStack;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Variables;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> WatchExpressions;

    FDebugSession()
    {
        StartTime = FDateTime::Now();
        Mode = EDebugMode::Step;
    }
};

USTRUCT(BlueprintType)
struct FDebugAnalysisResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bHasErrors;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ErrorDescriptions;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> WarningDescriptions;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PerformanceIssues;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> VariableStates;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly)
    float AnalysisTime;

    FDebugAnalysisResult()
    {
        bHasErrors = false;
        AnalysisTime = 0.0f;
    }
};

/**
 * 智慧除錯API - 具備自我學習能力的除錯系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSDebugAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSDebugAPI();

    // 核心除錯功能
    UFUNCTION(BlueprintCallable, Category = "Debug API")
    FDebugSession StartDebugSession(EDebugMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Debug API")
    void StopDebugSession(const FString& SessionId);

    UFUNCTION(BlueprintCallable, Category = "Debug API")
    bool SetBreakpoint(const FDebugBreakpoint& Breakpoint);

    UFUNCTION(BlueprintCallable, Category = "Debug API")
    bool RemoveBreakpoint(const FString& FilePath, int32 LineNumber);

    UFUNCTION(BlueprintCallable, Category = "Debug API")
    void StepOver();

    UFUNCTION(BlueprintCallable, Category = "Debug API")
    void StepInto();

    UFUNCTION(BlueprintCallable, Category = "Debug API")
    void Continue();

    // 變數監控
    UFUNCTION(BlueprintCallable, Category = "Debug API|Variables")
    TMap<FString, FString> GetLocalVariables();

    UFUNCTION(BlueprintCallable, Category = "Debug API|Variables")
    TMap<FString, FString> GetGlobalVariables();

    UFUNCTION(BlueprintCallable, Category = "Debug API|Variables")
    void AddWatchExpression(const FString& Expression);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Variables")
    FString EvaluateExpression(const FString& Expression);

    // 記憶體除錯
    UFUNCTION(BlueprintCallable, Category = "Debug API|Memory")
    TArray<uint8> GetMemoryRegion(uint64 Address, int32 Size);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Memory")
    bool SetMemoryRegion(uint64 Address, const TArray<uint8>& Data);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Memory")
    void TrackMemoryLeaks();

    UFUNCTION(BlueprintCallable, Category = "Debug API|Memory")
    TArray<FString> GetMemoryStatistics();

    // 性能除錯
    UFUNCTION(BlueprintCallable, Category = "Debug API|Performance")
    void StartPerformanceProfile(const FString& OperationName);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Performance")
    void StopPerformanceProfile(const FString& OperationName);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Performance")
    TMap<FString, float> GetPerformanceMetrics();

    // 自我學習功能
    UFUNCTION(BlueprintCallable, Category = "Debug API|Learning")
    void LearnFromBugPattern(const FString& BugPattern, const FString& Solution);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Learning")
    void UpdateBreakpointEfficiency(const FString& FilePath, int32 LineNumber, float Efficiency);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Learning")
    TArray<FString> GetSmartBreakpointSuggestions(const FString& FilePath);

    // 智慧分析
    UFUNCTION(BlueprintCallable, Category = "Debug API|Analysis")
    FDebugAnalysisResult AnalyzeCurrentState();

    UFUNCTION(BlueprintCallable, Category = "Debug API|Analysis")
    FDebugAnalysisResult AnalyzeErrorPattern(const TArray<FString>& ErrorLogs);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Analysis")
    TArray<FString> PredictPotentialIssues();

    // 跨API調用
    UFUNCTION(BlueprintCallable, Category = "Debug API|Integration")
    void RequestLogAnalysis(const FString& SessionId);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Integration")
    void RequestCompilationFix(const FDebugAnalysisResult& Analysis);

    UFUNCTION(BlueprintCallable, Category = "Debug API|Integration")
    void RequestPerformanceOptimization(const TMap<FString, float>& Metrics);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugSessionStarted, const FDebugSession&, Session);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBreakpointHit, const FDebugBreakpoint&, Breakpoint);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVariableChanged, const FString&, VariableName, const FString&, NewValue);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDebugAnalysisCompleted, const FDebugAnalysisResult&, Analysis);

    UPROPERTY(BlueprintAssignable)
    FOnDebugSessionStarted OnDebugSessionStarted;

    UPROPERTY(BlueprintAssignable)
    FOnBreakpointHit OnBreakpointHit;

    UPROPERTY(BlueprintAssignable)
    FOnVariableChanged OnVariableChanged;

    UPROPERTY(BlueprintAssignable)
    FOnDebugAnalysisCompleted OnDebugAnalysisCompleted;

private:
    // 學習數據
    UPROPERTY()
    TArray<FDebugSession> DebugSessions;

    UPROPERTY()
    TArray<FDebugBreakpoint> Breakpoints;

    UPROPERTY()
    TMap<FString, FString> BugPatterns;

    UPROPERTY()
    TMap<FString, float> BreakpointEfficiencies;

    UPROPERTY()
    TMap<FString, FString> CurrentVariables;

    // 內部方法
    void InitializeDebugDatabase();
    void SaveDebugData();
    void LoadDebugData();
    FString AnalyzeCallStack(const TArray<FString>& CallStack);
    bool IsConditionalBreakpoint(const FDebugBreakpoint& Breakpoint);
    void UpdateLearningFromSession(const FDebugSession& Session);
};
