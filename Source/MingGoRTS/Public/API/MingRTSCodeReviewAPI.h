// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Code Review API - 版控前代碼審查系統

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSCodeReviewAPI.generated.h"

UENUM(BlueprintType)
enum class ECodeReviewSeverity : uint8
{
    Info,           // 信息級別
    Warning,        // 警告級別
    Error,          // 錯誤級別
    Critical        // 嚴重級別
};

UENUM(BlueprintType)
enum class ECodeReviewCategory : uint8
{
    Syntax,         // 語法檢查
    Style,          // 代碼風格
    Performance,    // 性能問題
    Security,       // 安全問題
    Maintainability, // 可維護性
    Documentation,  // 文檔規範
    Testing,        // 測試覆蓋
    Dependencies    // 依賴管理
};

UENUM(BlueprintType)
enum class ECodeReviewResult : uint8
{
    Pass,           // 通過審查
    Warning,        // 警告通過
    Fail,           // 審查失敗
    Critical        // 嚴重問題
};

USTRUCT(BlueprintType)
struct FCodeReviewIssue
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString IssueID;

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber;

    UPROPERTY(BlueprintReadOnly)
    int32 ColumnNumber;

    UPROPERTY(BlueprintReadOnly)
    FString IssueType;

    UPROPERTY(BlueprintReadOnly)
    ECodeReviewCategory Category;

    UPROPERTY(BlueprintReadOnly)
    ECodeReviewSeverity Severity;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FString Suggestion;

    UPROPERTY(BlueprintReadOnly)
    FString RuleReference;

    UPROPERTY(BlueprintReadOnly)
    bool bAutoFixable;

    UPROPERTY(BlueprintReadOnly)
    FString AutoFixCommand;

    FCodeReviewIssue()
    {
        IssueID = TEXT("");
        FilePath = TEXT("");
        LineNumber = 0;
        ColumnNumber = 0;
        IssueType = TEXT("");
        Category = ECodeReviewCategory::Syntax;
        Severity = ECodeReviewSeverity::Info;
        Description = TEXT("");
        Suggestion = TEXT("");
        RuleReference = TEXT("");
        bAutoFixable = false;
        AutoFixCommand = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FCodeReviewMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalIssues;

    UPROPERTY(BlueprintReadOnly)
    int32 CriticalIssues;

    UPROPERTY(BlueprintReadOnly)
    int32 ErrorIssues;

    UPROPERTY(BlueprintReadOnly)
    int32 WarningIssues;

    UPROPERTY(BlueprintReadOnly)
    int32 InfoIssues;

    UPROPERTY(BlueprintReadOnly)
    float CodeQualityScore;

    UPROPERTY(BlueprintReadOnly)
    float MaintainabilityScore;

    UPROPERTY(BlueprintReadOnly)
    float PerformanceScore;

    UPROPERTY(BlueprintReadOnly)
    float SecurityScore;

    UPROPERTY(BlueprintReadOnly)
    float DocumentationScore;

    UPROPERTY(BlueprintReadOnly)
    float TestCoverageScore;

    FCodeReviewMetrics()
    {
        TotalIssues = 0;
        CriticalIssues = 0;
        ErrorIssues = 0;
        WarningIssues = 0;
        InfoIssues = 0;
        CodeQualityScore = 100.0f;
        MaintainabilityScore = 100.0f;
        PerformanceScore = 100.0f;
        SecurityScore = 100.0f;
        DocumentationScore = 100.0f;
        TestCoverageScore = 100.0f;
    }
};

USTRUCT(BlueprintType)
struct FCodeReviewReport
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ReportID;

    UPROPERTY(BlueprintReadOnly)
    FDateTime ReviewTime;

    UPROPERTY(BlueprintReadOnly)
    FString Reviewer;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ReviewedFiles;

    UPROPERTY(BlueprintReadOnly)
    TArray<FCodeReviewIssue> Issues;

    UPROPERTY(BlueprintReadOnly)
    FCodeReviewMetrics Metrics;

    UPROPERTY(BlueprintReadOnly)
    ECodeReviewResult OverallResult;

    UPROPERTY(BlueprintReadOnly)
    FString Summary;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly)
    bool bCanCommit;

    FCodeReviewReport()
    {
        ReportID = TEXT("");
        ReviewTime = FDateTime::Now();
        Reviewer = TEXT("CodeReviewAPI");
        ReviewedFiles.Empty();
        Issues.Empty();
        OverallResult = ECodeReviewResult::Pass;
        Summary = TEXT("");
        Recommendations.Empty();
        bCanCommit = true;
    }
};

USTRUCT(BlueprintType)
struct FCodeReviewRule
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RuleID;

    UPROPERTY(BlueprintReadOnly)
    FString RuleName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    ECodeReviewCategory Category;

    UPROPERTY(BlueprintReadOnly)
    ECodeReviewSeverity DefaultSeverity;

    UPROPERTY(BlueprintReadOnly)
    bool bEnabled;

    UPROPERTY(BlueprintReadOnly)
    FString Pattern;

    UPROPERTY(BlueprintReadOnly)
    FString SuggestionTemplate;

    FCodeReviewRule()
    {
        RuleID = TEXT("");
        RuleName = TEXT("");
        Description = TEXT("");
        Category = ECodeReviewCategory::Syntax;
        DefaultSeverity = ECodeReviewSeverity::Warning;
        bEnabled = true;
        Pattern = TEXT("");
        SuggestionTemplate = TEXT("");
    }
};

/**
 * Code Review API - 版控前代碼審查系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCodeReviewAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCodeReviewAPI();

    /// 初始化代碼審查系統
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    void InitializeCodeReviewSystem();

    /// 執行完整代碼審查
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    FCodeReviewReport PerformCodeReview(const TArray<FString>& Files);

    /// 檢查單個文件
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    FCodeReviewReport ReviewSingleFile(const FString& FilePath);

    /// 檢查代碼語法
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckSyntax(const FString& FilePath);

    /// 檢查代碼風格
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckCodeStyle(const FString& FilePath);

    /// 檢查性能問題
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckPerformance(const FString& FilePath);

    /// 檢查安全問題
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckSecurity(const FString& FilePath);

    /// 檢查可維護性
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckMaintainability(const FString& FilePath);

    /// 檢查文檔規範
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckDocumentation(const FString& FilePath);

    /// 檢查測試覆蓋
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckTestCoverage(const FString& FilePath);

    /// 檢查依賴管理
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewIssue> CheckDependencies(const FString& FilePath);

    /// 自動修復問題
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    bool AutoFixIssue(const FString& FilePath, const FString& IssueID);

    /// 批量自動修復
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FString> AutoFixAllIssues(const FString& FilePath);

    /// 生成審查報告
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    FString GenerateReviewReport(const FCodeReviewReport& Report);

    /// 驗證提交資格
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    bool ValidateCommitReadiness(const FCodeReviewReport& Report);

    /// 獲取審查規則
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    TArray<FCodeReviewRule> GetReviewRules();

    /// 更新審查規則
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    bool UpdateReviewRule(const FCodeReviewRule& Rule);

    /// 設置審查閾值
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    void SetReviewThresholds(float CriticalThreshold, float ErrorThreshold, float WarningThreshold);

    /// 獲取審查統計
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    FCodeReviewMetrics GetReviewMetrics();

    /// 導出審查結果
    UFUNCTION(BlueprintCallable, Category = "Code Review API")
    FString ExportReviewResults(const FCodeReviewReport& Report);

    /// 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReviewStarted, const FString&, FilePath);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIssueFound, const FString&, FilePath, const FCodeReviewIssue&, Issue);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReviewCompleted, const FString&, FilePath, const FCodeReviewReport&, Report);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommitValidated, bool, bCanCommit);

    UPROPERTY(BlueprintAssignable)
    FOnReviewStarted OnReviewStarted;

    UPROPERTY(BlueprintAssignable)
    FOnIssueFound OnIssueFound;

    UPROPERTY(BlueprintAssignable)
    FOnReviewCompleted OnReviewCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnCommitValidated OnCommitValidated;

private:
    /// 審查規則
    UPROPERTY()
    TArray<FCodeReviewRule> ReviewRules;

    /// 審查閾值
    UPROPERTY()
    float CriticalThreshold;

    UPROPERTY()
    float ErrorThreshold;

    UPROPERTY()
    float WarningThreshold;

    /// 系統配置
    UPROPERTY()
    bool bAutoFixEnabled;

    UPROPERTY()
    bool bStrictMode;

    UPROPERTY()
    int32 MaxIssuesPerFile;

    /// 內部方法
    void InitializeDefaultRules();
    FCodeReviewIssue AnalyzeLine(const FString& FilePath, const FString& Line, int32 LineNumber);
    bool MatchesPattern(const FString& Text, const FString& Pattern);
    FString GenerateIssueID();
    float CalculateQualityScore(const TArray<FCodeReviewIssue>& Issues);
    ECodeReviewResult DetermineOverallResult(const FCodeReviewMetrics& Metrics);
    FString GenerateSuggestion(const FCodeReviewRule& Rule, const FString& Context);
    bool ApplyAutoFix(const FString& FilePath, const FCodeReviewIssue& Issue);
    void UpdateMetrics(FCodeReviewMetrics& Metrics, const TArray<FCodeReviewIssue>& Issues);
};
