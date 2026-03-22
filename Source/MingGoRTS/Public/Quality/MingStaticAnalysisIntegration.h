// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Static Analysis Integration System - B3-3
// Provides static code analysis and automated issue detection

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingStaticAnalysisIntegration.generated.h"

UENUM(BlueprintType)
enum class EAnalysisIssueSeverity : uint8
{
    Info = 0, UMETA(DisplayName = "Info"),
    Warning, UMETA(DisplayName = "Warning"),
    Error, UMETA(DisplayName = "Error"),
    Critical, UMETA(DisplayName = "Critical"),
    Security, UMETA(DisplayName = "Security Issue"),
    Performance, UMETA(DisplayName = "Performance Issue"),
    Style, UMETA(DisplayName = "Style Issue"),
    Portability, UMETA(DisplayName = "Portability Issue")
};

UENUM(BlueprintType)
enum class EAnalysisRuleCategory : uint8
{
    CodeQuality = 0, UMETA(DisplayName = "Code Quality"),
    Security, UMETA(DisplayName = "Security"),
    Performance, UMETA(DisplayName = "Performance"),
    Correctness, UMETA(DisplayName = "Correctness"),
    Maintainability, UMETA(DisplayName = "Maintainability"),
    NamingConvention, UMETA(DisplayName = "Naming Convention"),
    Documentation, UMETA(DisplayName = "Documentation"),
    MemoryManagement, UMETA(DisplayName = "Memory Management"),
    Concurrency, UMETA(DisplayName = "Concurrency"),
    BestPractices, UMETA(DisplayName = "Best Practices")
};

UENUM(BlueprintType)
enum class EAnalysisStatus : uint8
{
    Idle = 0, UMETA(DisplayName = "Idle"),
    Running, UMETA(DisplayName = "Running"),
    Completed, UMETA(DisplayName = "Completed"),
    Failed, UMETA(DisplayName = "Failed"),
    Cancelled UMETA(DisplayName = "Cancelled")
};

USTRUCT(BlueprintType)
struct FAnalysisIssue
{
    GENERATED_BODY()

    UPROPERTY()
    FName IssueID;

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    EAnalysisIssueSeverity Severity;

    UPROPERTY()
    EAnalysisRuleCategory Category;

    UPROPERTY()
    FString FilePath;

    UPROPERTY()
    int32 LineNumber;

    UPROPERTY()
    int32 ColumnNumber;

    UPROPERTY()
    FString RuleID;

    UPROPERTY()
    FString RuleName;

    UPROPERTY()
    TArray<FString> SuggestedFixes;

    UPROPERTY()
    FString AffectedCode;

    UPROPERTY()
    bool bAutoFixable;

    UPROPERTY()
    uint32 Timestamp;

    UPROPERTY()
    FString AssignedTo;

    UPROPERTY()
    EAnalysisStatus Status;

    FAnalysisIssue()
        : IssueID(NAME_None)
        , Severity(EAnalysisIssueSeverity::Info)
        , Category(EAnalysisRuleCategory::CodeQuality)
        , LineNumber(0)
        , ColumnNumber(0)
        , bAutoFixable(false)
        , Timestamp(0)
        , Status(EAnalysisStatus::Idle)
    {}
};

USTRUCT(BlueprintType)
struct FAnalysisRule
{
    GENERATED_BODY()

    UPROPERTY()
    FName RuleID;

    UPROPERTY()
    FString RuleName;

    UPROPERTY()
    EAnalysisRuleCategory Category;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    EAnalysisIssueSeverity DefaultSeverity;

    UPROPERTY()
    bool bEnabled;

    UPROPERTY()
    TArray<FString> ExcludedPaths;

    UPROPERTY()
    TMap<FString, FString> Parameters;

    UPROPERTY()
    FString HelpURL;

    FAnalysisRule()
        : RuleID(NAME_None)
        , Category(EAnalysisRuleCategory::CodeQuality)
        , DefaultSeverity(EAnalysisIssueSeverity::Warning)
        , bEnabled(true)
    {}
};

USTRUCT(BlueprintType)
struct FAnalysisReport
{
    GENERATED_BODY()

    UPROPERTY()
    FString ReportID;

    UPROPERTY()
    uint32 StartTime;

    UPROPERTY()
    uint32 EndTime;

    UPROPERTY()
    TArray<FAnalysisIssue> Issues;

    UPROPERTY()
    TMap<EAnalysisIssueSeverity, int32> IssuesBySeverity;

    UPROPERTY()
    TMap<EAnalysisRuleCategory, int32> IssuesByCategory;

    UPROPERTY()
    int32 TotalFilesAnalyzed;

    UPROPERTY()
    int32 TotalLinesAnalyzed;

    UPROPERTY()
    float AnalysisDuration;

    UPROPERTY()
    TArray<FString> FilesWithMostIssues;

    UPROPERTY()
    TArray<FString> RulesTriggered;

    FAnalysisReport()
        : StartTime(0)
        , EndTime(0)
        , TotalFilesAnalyzed(0)
        , TotalLinesAnalyzed(0)
        , AnalysisDuration(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FAnalysisConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    TArray<EAnalysisRuleCategory> EnabledCategories;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    TArray<FString> IncludedPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    TArray<FString> ExcludedPaths;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    bool bAutoFixEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    bool bFailOnError;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    int32 MaxIssuesPerFile;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    float TimeoutSeconds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    bool bIncrementalAnalysis;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    bool bGenerateReports;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Analysis")
    TArray<FString> CustomRulesPaths;

    FAnalysisConfig()
        : bAutoFixEnabled(false)
        , bFailOnError(true)
        , MaxIssuesPerFile(100)
        , TimeoutSeconds(300.0f)
        , bIncrementalAnalysis(true)
        , bGenerateReports(true)
    {}
};

USTRUCT(BlueprintType)
struct FQualityGate
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    FString GateName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    int32 MaxCriticalIssues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    int32 MaxErrorIssues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    int32 MaxWarningIssues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    float MinCodeCoverage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    float MaxComplexity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    float MaxDuplication;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quality Gate")
    bool bBlockBuildOnFailure;

    FQualityGate()
        : MaxCriticalIssues(0)
        , MaxErrorIssues(10)
        , MaxWarningIssues(100)
        , MinCodeCoverage(70.0f)
        , MaxComplexity(15.0f)
        , MaxDuplication(5.0f)
        , bBlockBuildOnFailure(true)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIssueDetected, FAnalysisIssue, Issue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnalysisStarted, FString, TargetPath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnalysisCompleted, FAnalysisReport, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQualityGateFailed, FQualityGate, Gate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAutoFixApplied, FAnalysisIssue, Issue, bool, Success);

/**
 * Static Analysis Integration System
 * Provides static code analysis and automated issue detection
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingStaticAnalysisIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingStaticAnalysisIntegration();

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void InitializeIntegration(const FAnalysisConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void ShutdownIntegration();

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    FAnalysisReport RunAnalysis(const FString& Path);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    FAnalysisReport RunIncrementalAnalysis();

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    bool RunAnalysisAsync(const FString& Path);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void CancelAnalysis();

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void EnableRule(const FName& RuleID, bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void SetRuleSeverity(const FName& RuleID, EAnalysisIssueSeverity Severity);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    TArray<FAnalysisRule> GetAvailableRules() const;

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    TArray<FAnalysisIssue> GetAllIssues() const;

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    TArray<FAnalysisIssue> GetIssuesBySeverity(EAnalysisIssueSeverity Severity) const;

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    TArray<FAnalysisIssue> GetIssuesByCategory(EAnalysisRuleCategory Category) const;

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    TArray<FAnalysisIssue> GetIssuesForFile(const FString& FilePath) const;

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    bool ApplyAutoFix(const FAnalysisIssue& Issue);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    int32 ApplyAllAutoFixes();

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    bool CheckQualityGate(const FQualityGate& Gate);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void SetQualityGate(const FQualityGate& Gate);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void ExportReport(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void ImportIssues(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void SuppressIssue(const FName& IssueID, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void AssignIssue(const FName& IssueID, const FString& Assignee);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void MarkIssueAsResolved(const FName& IssueID);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void SchedulePeriodicAnalysis(float IntervalHours);

    UFUNCTION(BlueprintCallable, Category = "Static Analysis")
    void CancelScheduledAnalysis();

    UFUNCTION(BlueprintPure, Category = "Static Analysis")
    EAnalysisStatus GetAnalysisStatus() const { return CurrentStatus; }

    UFUNCTION(BlueprintPure, Category = "Static Analysis")
    int32 GetTotalIssuesCount() const;

    UFUNCTION(BlueprintPure, Category = "Static Analysis")
    int32 GetAutoFixableCount() const;

    UPROPERTY(BlueprintAssignable, Category = "Static Analysis Events")
    FOnIssueDetected OnIssueDetected;

    UPROPERTY(BlueprintAssignable, Category = "Static Analysis Events")
    FOnAnalysisStarted OnAnalysisStarted;

    UPROPERTY(BlueprintAssignable, Category = "Static Analysis Events")
    FOnAnalysisCompleted OnAnalysisCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Static Analysis Events")
    FOnQualityGateFailed OnQualityGateFailed;

    UPROPERTY(BlueprintAssignable, Category = "Static Analysis Events")
    FOnAutoFixApplied OnAutoFixApplied;

protected:
    UPROPERTY()
    FAnalysisConfig Config;

    UPROPERTY()
    TArray<FAnalysisRule> Rules;

    UPROPERTY()
    TArray<FAnalysisIssue> DetectedIssues;

    UPROPERTY()
    EAnalysisStatus CurrentStatus;

    UPROPERTY()
    FQualityGate ActiveQualityGate;

    UPROPERTY()
    FTimerHandle PeriodicAnalysisTimer;

    UPROPERTY()
    FTimerHandle AnalysisProgressTimer;

    void InitializeDefaultRules();
    void StartAnalysis(const FString& Path);
    void RunAnalysisInternal(const FString& Path);
    void CompleteAnalysis();
    void FailAnalysis(const FString& Reason);
    void DetectIssuesInFile(const FString& FilePath);
    void ApplyAnalysisRules(const FString& FileContent, const FString& FilePath);
    FAnalysisIssue CreateIssue(const FAnalysisRule& Rule, const FString& FilePath, int32 Line, int32 Column);
    bool TryAutoFix(const FAnalysisIssue& Issue);
    void UpdateIssueStatus(const FName& IssueID, EAnalysisStatus NewStatus);
    void NotifyIssueDetected(const FAnalysisIssue& Issue);
    void CheckQualityGateConditions();
    bool IsPathExcluded(const FString& Path) const;
    bool IsRuleEnabled(const FName& RuleID) const;
    TArray<FString> GetFilesToAnalyze(const FString& Path) const;
    void ExportIssuesToJSON(const FString& FilePath);
    void ExportIssuesToXML(const FString& FilePath);
    void LogAnalysisActivity(const FString& Activity);
    static UMingStaticAnalysisIntegration* Get(UObject* WorldContextObject);
};
