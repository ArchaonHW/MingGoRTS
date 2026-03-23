#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSLogAPI.generated.h"

UENUM(BlueprintType)
enum class ELogLevel : uint8
{
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

UENUM(BlueprintType)
enum class ELogCategory : uint8
{
    General,
    Compilation,
    Performance,
    Network,
    AI,
    UI,
    Audio,
    Input,
    Memory,
    FileIO
};

USTRUCT(BlueprintType)
struct FLogEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Message;

    UPROPERTY(BlueprintReadOnly)
    ELogLevel Level;

    UPROPERTY(BlueprintReadOnly)
    ELogCategory Category;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString FunctionName;

    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber;

    UPROPERTY(BlueprintReadOnly)
    FString ThreadId;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Metadata;

    FLogEntry()
    {
        Level = ELogLevel::Info;
        Category = ELogCategory::General;
        Timestamp = FDateTime::Now();
        LineNumber = 0;
    }
};

USTRUCT(BlueprintType)
struct FLogAnalysisResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<FLogEntry> CriticalEntries;

    UPROPERTY(BlueprintReadOnly)
    TArray<FLogEntry> ErrorEntries;

    UPROPERTY(BlueprintReadOnly)
    TArray<FLogEntry> WarningEntries;

    UPROPERTY(BlueprintReadOnly)
    TMap<ELogCategory, int32> CategoryCounts;

    UPROPERTY(BlueprintReadOnly)
    TMap<ELogLevel, int32> LevelCounts;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Patterns;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly)
    float AnalysisTime;
};

USTRUCT(BlueprintType)
struct FLogFilter
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ELogLevel MinLevel;

    UPROPERTY(BlueprintReadOnly)
    ELogLevel MaxLevel;

    UPROPERTY(BlueprintReadOnly)
    TArray<ELogCategory> Categories;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly)
    FString SearchPattern;

    FLogFilter()
    {
        MinLevel = ELogLevel::Trace;
        MaxLevel = ELogLevel::Critical;
        StartTime = FDateTime::MinValue();
        EndTime = FDateTime::MaxValue();
    }
};

/**
 * 智慧日誌API - 具備自我學習能力的日誌系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSLogAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSLogAPI();

    // 核心日誌功能
    UFUNCTION(BlueprintCallable, Category = "Log API")
    void LogMessage(const FString& Message, ELogLevel Level = ELogLevel::Info, ELogCategory Category = ELogCategory::General);

    UFUNCTION(BlueprintCallable, Category = "Log API")
    void LogFunction(const FString& FunctionName, const FString& Message, ELogLevel Level = ELogLevel::Debug);

    UFUNCTION(BlueprintCallable, Category = "Log API")
    void LogPerformance(const FString& Operation, float Duration, const TMap<FString, FString>& Metadata = TMap<FString, FString>());

    UFUNCTION(BlueprintCallable, Category = "Log API")
    TArray<FLogEntry> GetFilteredLogs(const FLogFilter& Filter);

    // 日誌分析
    UFUNCTION(BlueprintCallable, Category = "Log API|Analysis")
    FLogAnalysisResult AnalyzeLogs(const FLogFilter& Filter = FLogFilter());

    UFUNCTION(BlueprintCallable, Category = "Log API|Analysis")
    TArray<FString> DetectAnomalies(const TArray<FLogEntry>& Logs);

    UFUNCTION(BlueprintCallable, Category = "Log API|Analysis")
    TArray<FString> GenerateInsights(const FLogAnalysisResult& Analysis);

    // 自我學習功能
    UFUNCTION(BlueprintCallable, Category = "Log API|Learning")
    void LearnFromLogPattern(const FString& Pattern, ELogLevel Level, ELogCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Log API|Learning")
    void UpdateAnomalyDetection(const FString& AnomalyPattern, bool bIsAnomaly);

    UFUNCTION(BlueprintCallable, Category = "Log API|Learning")
    void OptimizeLogLevels(const TMap<ELogCategory, ELogLevel>& OptimalLevels);

    // 智慧日誌管理
    UFUNCTION(BlueprintCallable, Category = "Log API|Management")
    void AutoArchiveOldLogs(int32 DaysToKeep = 30);

    UFUNCTION(BlueprintCallable, Category = "Log API|Management")
    void CompressLogs(const FDateTime& StartDate, const FDateTime& EndDate);

    UFUNCTION(BlueprintCallable, Category = "Log API|Management")
    void ExportLogs(const FString& FilePath, const FLogFilter& Filter);

    // 跨API調用
    UFUNCTION(BlueprintCallable, Category = "Log API|Integration")
    void RequestCompilationAnalysis(const TArray<FString>& CompilationLogs);

    UFUNCTION(BlueprintCallable, Category = "Log API|Integration")
    void RequestPerformanceOptimization(const FLogAnalysisResult& Analysis);

    UFUNCTION(BlueprintCallable, Category = "Log API|Integration")
    void RequestDebuggingSession(const TArray<FLogEntry>& ErrorLogs);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLogEntryAdded, const FLogEntry&, LogEntry, bool, bIsCritical);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLogAnalysisCompleted, const FLogAnalysisResult&, Analysis);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnomalyDetected, const FString&, AnomalyType);

    UPROPERTY(BlueprintAssignable)
    FOnLogEntryAdded OnLogEntryAdded;

    UPROPERTY(BlueprintAssignable)
    FOnLogAnalysisCompleted OnLogAnalysisCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnAnomalyDetected OnAnomalyDetected;

private:
    // 學習數據
    UPROPERTY()
    TArray<FLogEntry> LogEntries;

    UPROPERTY()
    TMap<FString, int32> PatternFrequencies;

    UPROPERTY()
    TMap<FString, bool> AnomalyPatterns;

    UPROPERTY()
    TMap<ELogCategory, ELogLevel> OptimalLogLevels;

    // 內部方法
    void InitializeLogDatabase();
    void SaveLogData();
    void LoadLogData();
    FString FormatLogEntry(const FLogEntry& Entry);
    bool DetectAnomalyPattern(const FLogEntry& Entry);
    void UpdateLearningStatistics(const FLogEntry& Entry);
};
