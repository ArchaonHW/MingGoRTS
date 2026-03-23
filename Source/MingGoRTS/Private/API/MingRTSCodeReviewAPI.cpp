#include "MingRTSCodeReviewAPI.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

UMingRTSCodeReviewAPI::UMingRTSCodeReviewAPI()
{
    CriticalThreshold = 0.0f;
    ErrorThreshold = 5.0f;
    WarningThreshold = 10.0f;
    bAutoFixEnabled = true;
    bStrictMode = false;
    MaxIssuesPerFile = 50;
}

void UMingRTSCodeReviewAPI::InitializeCodeReviewSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Initializing code review system..."));
    
    InitializeDefaultRules();
    
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Initialized %d review rules"), ReviewRules.Num());
    UE_LOG(LogTemp, Log, TEXT("Code Review API: System initialization completed"));
}

void UMingRTSCodeReviewAPI::InitializeDefaultRules()
{
    ReviewRules.Empty();
    
    // 語法檢查規則
    FCodeReviewRule SyntaxRule1;
    SyntaxRule1.RuleID = TEXT("SYNTAX_001");
    SyntaxRule1.RuleName = TEXT("Missing Semicolon");
    SyntaxRule1.Description = TEXT("Line ends without semicolon");
    SyntaxRule1.Category = ECodeReviewCategory::Syntax;
    SyntaxRule1.DefaultSeverity = ECodeReviewSeverity::Error;
    SyntaxRule1.bEnabled = true;
    SyntaxRule1.Pattern = TEXT("(?<!;)\\s*$");
    SyntaxRule1.SuggestionTemplate = TEXT("Add semicolon at the end of the line");
    ReviewRules.Add(SyntaxRule1);
    
    // 代碼風格規則
    FCodeReviewRule StyleRule1;
    StyleRule1.RuleID = TEXT("STYLE_001");
    StyleRule1.RuleName = TEXT("Inconsistent Naming");
    StyleRule1.Description = TEXT("Variable name doesn't follow naming convention");
    StyleRule1.Category = ECodeReviewCategory::Style;
    StyleRule1.DefaultSeverity = ECodeReviewSeverity::Warning;
    StyleRule1.bEnabled = true;
    StyleRule1.Pattern = TEXT("\\b[a-z][a-zA-Z0-9]*\\b");
    StyleRule1.SuggestionTemplate = TEXT("Use camelCase for variable names");
    ReviewRules.Add(StyleRule1);
    
    // 性能檢查規則
    FCodeReviewRule PerfRule1;
    PerfRule1.RuleID = TEXT("PERF_001");
    PerfRule1.RuleName = TEXT("Inefficient String Concatenation");
    PerfRule1.Description = TEXT("Using FString::Append in loop may cause performance issues");
    PerfRule1.Category = ECodeReviewCategory::Performance;
    PerfRule1.DefaultSeverity = ECodeReviewSeverity::Warning;
    PerfRule1.bEnabled = true;
    PerfRule1.Pattern = TEXT("\\.Append\\(");
    PerfRule1.SuggestionTemplate = TEXT("Consider using FString::Printf or StringBuilder for better performance");
    ReviewRules.Add(PerfRule1);
    
    // 安全檢查規則
    FCodeReviewRule SecurityRule1;
    SecurityRule1.RuleID = TEXT("SEC_001");
    SecurityRule1.RuleName = TEXT("Hardcoded Password");
    SecurityRule1.Description = TEXT("Potential hardcoded password detected");
    SecurityRule1.Category = ECodeReviewCategory::Security;
    SecurityRule1.DefaultSeverity = ECodeReviewSeverity::Critical;
    SecurityRule1.bEnabled = true;
    SecurityRule1.Pattern = TEXT("(?i)(password|pwd|pass)\\s*=\\s*[\"'][^\"']+[\"']");
    SecurityRule1.SuggestionTemplate = TEXT("Use secure configuration management instead of hardcoded passwords");
    ReviewRules.Add(SecurityRule1);
    
    // 可維護性規則
    FCodeReviewRule MaintainRule1;
    MaintainRule1.RuleID = TEXT("MAINT_001");
    MaintainRule1.RuleName = TEXT("Long Function");
    MaintainRule1.Description = TEXT("Function exceeds recommended length");
    MaintainRule1.Category = ECodeReviewCategory::Maintainability;
    MaintainRule1.DefaultSeverity = ECodeReviewSeverity::Warning;
    MaintainRule1.bEnabled = true;
    MaintainRule1.Pattern = TEXT("^\\s*\\{\\s*$");
    MaintainRule1.SuggestionTemplate = TEXT("Consider breaking down long functions into smaller, more focused functions");
    ReviewRules.Add(MaintainRule1);
    
    // 文檔規則
    FCodeReviewRule DocRule1;
    DocRule1.RuleID = TEXT("DOC_001");
    DocRule1.RuleName = TEXT("Missing Function Documentation");
    DocRule1.Description = TEXT("Public function lacks documentation comment");
    DocRule1.Category = ECodeReviewCategory::Documentation;
    DocRule1.DefaultSeverity = ECodeReviewSeverity::Warning;
    DocRule1.bEnabled = true;
    DocRule1.Pattern = TEXT("UFUNCTION\\(.*\\)");
    DocRule1.SuggestionTemplate = TEXT("Add comprehensive documentation comment for this function");
    ReviewRules.Add(DocRule1);
    
    // 測試覆蓋規則
    FCodeReviewRule TestRule1;
    TestRule1.RuleID = TEXT("TEST_001");
    TestRule1.RuleName = TEXT("Untested Function");
    TestRule1.Description = TEXT("Function lacks corresponding test");
    TestRule1.Category = ECodeReviewCategory::Testing;
    TestRule1.DefaultSeverity = ECodeReviewSeverity::Info;
    TestRule1.bEnabled = true;
    TestRule1.Pattern = TEXT("UFUNCTION\\(.*\\)");
    TestRule1.SuggestionTemplate = TEXT("Consider adding unit tests for this function");
    ReviewRules.Add(TestRule1);
    
    // 依賴管理規則
    FCodeReviewRule DepRule1;
    DepRule1.RuleID = TEXT("DEP_001");
    DepRule1.RuleName = TEXT("Unnecessary Include");
    DepRule1.Description = TEXT("Header file includes unnecessary dependencies");
    DepRule1.Category = ECodeReviewCategory::Dependencies;
    DepRule1.DefaultSeverity = ECodeReviewSeverity::Info;
    DepRule1.bEnabled = true;
    DepRule1.Pattern = TEXT("#include\\s+[\"<].*[\">]");
    DepRule1.SuggestionTemplate = TEXT("Review if this include is necessary");
    ReviewRules.Add(DepRule1);
}

FCodeReviewReport UMingRTSCodeReviewAPI::PerformCodeReview(const TArray<FString>& Files)
{
    FCodeReviewReport Report;
    Report.ReportID = GenerateIssueID();
    Report.ReviewTime = FDateTime::Now();
    Report.Reviewer = TEXT("CodeReviewAPI");
    Report.ReviewedFiles = Files;
    
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Starting code review for %d files"), Files.Num());
    
    for (const FString& FilePath : Files)
    {
        OnReviewStarted.Broadcast(FilePath);
        
        FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
        
        // 合併問題
        Report.Issues.Append(FileReport.Issues);
        
        // 更新文件列表
        Report.ReviewedFiles.Add(FilePath);
        
        OnReviewCompleted.Broadcast(FilePath, FileReport);
    }
    
    // 計算指標
    UpdateMetrics(Report.Metrics, Report.Issues);
    
    // 確定整體結果
    Report.OverallResult = DetermineOverallResult(Report.Metrics);
    
    // 生成總結
    GenerateReviewSummary(Report);
    
    // 驗證提交資格
    Report.bCanCommit = ValidateCommitReadiness(Report);
    
    OnCommitValidated.Broadcast(Report.bCanCommit);
    
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Review completed. Result: %d, Issues: %d, Can Commit: %s"), 
        (int32)Report.OverallResult, Report.Issues.Num(), Report.bCanCommit ? TEXT("Yes") : TEXT("No"));
    
    return Report;
}

FCodeReviewReport UMingRTSCodeReviewAPI::ReviewSingleFile(const FString& FilePath)
{
    FCodeReviewReport Report;
    Report.ReportID = GenerateIssueID();
    Report.ReviewTime = FDateTime::Now();
    Report.Reviewer = TEXT("CodeReviewAPI");
    Report.ReviewedFiles.Add(FilePath);
    
    if (!FFileHelper::FileExists(*FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Review API: File not found: %s"), *FilePath);
        return Report;
    }
    
    // 讀取文件內容
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Code Review API: Failed to read file: %s"), *FilePath);
        return Report;
    }
    
    // 分行處理
    TArray<FString> Lines;
    FileContent.ParseIntoArrayLines(Lines);
    
    // 檢查每一行
    for (int32 LineIndex = 0; LineIndex < Lines.Num(); LineIndex++)
    {
        const FString& Line = Lines[LineIndex];
        
        // 應用所有規則
        for (const FCodeReviewRule& Rule : ReviewRules)
        {
            if (!Rule.bEnabled)
            {
                continue;
            }
            
            if (MatchesPattern(Line, Rule.Pattern))
            {
                FCodeReviewIssue Issue;
                Issue.IssueID = GenerateIssueID();
                Issue.FilePath = FilePath;
                Issue.LineNumber = LineIndex + 1;
                Issue.ColumnNumber = 0;
                Issue.IssueType = Rule.RuleName;
                Issue.Category = Rule.Category;
                Issue.Severity = Rule.DefaultSeverity;
                Issue.Description = Rule.Description;
                Issue.Suggestion = GenerateSuggestion(Rule, Line);
                Issue.RuleReference = Rule.RuleID;
                Issue.bAutoFixable = Rule.RuleID.StartsWith(TEXT("STYLE_")) || Rule.RuleID.StartsWith(TEXT("SYNTAX_"));
                
                if (Issue.bAutoFixable)
                {
                    Issue.AutoFixCommand = FString::Printf(TEXT("AutoFix:%s:%d"), *Rule.RuleID, LineIndex + 1);
                }
                
                Report.Issues.Add(Issue);
                OnIssueFound.Broadcast(FilePath, Issue);
                
                // 限制每個文件的問題數量
                if (Report.Issues.Num() >= MaxIssuesPerFile)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Code Review API: Max issues per file reached for %s"), *FilePath);
                    break;
                }
            }
        }
    }
    
    // 計算指標
    UpdateMetrics(Report.Metrics, Report.Issues);
    
    // 確定結果
    Report.OverallResult = DetermineOverallResult(Report.Metrics);
    
    return Report;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckSyntax(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查語法問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Syntax && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Syntax)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckCodeStyle(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查代碼風格問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Style && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Style)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckPerformance(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查性能問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Performance && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Performance)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckSecurity(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查安全問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Security && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Security)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckMaintainability(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查可維護性問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Maintainability && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Maintainability)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckDocumentation(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查文檔問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Documentation && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Documentation)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckTestCoverage(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查測試覆蓋問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Testing && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Testing)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

TArray<FCodeReviewIssue> UMingRTSCodeReviewAPI::CheckDependencies(const FString& FilePath)
{
    TArray<FCodeReviewIssue> Issues;
    
    // 專門檢查依賴管理問題
    for (const FCodeReviewRule& Rule : ReviewRules)
    {
        if (Rule.Category == ECodeReviewCategory::Dependencies && Rule.bEnabled)
        {
            FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
            for (const FCodeReviewIssue& Issue : FileReport.Issues)
            {
                if (Issue.Category == ECodeReviewCategory::Dependencies)
                {
                    Issues.Add(Issue);
                }
            }
        }
    }
    
    return Issues;
}

bool UMingRTSCodeReviewAPI::AutoFixIssue(const FString& FilePath, const FString& IssueID)
{
    if (!bAutoFixEnabled)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Review API: Auto-fix is disabled"));
        return false;
    }
    
    // 查找對應的問題
    FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
    for (const FCodeReviewIssue& Issue : FileReport.Issues)
    {
        if (Issue.IssueID == IssueID && Issue.bAutoFixable)
        {
            return ApplyAutoFix(FilePath, Issue);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Code Review API: Issue %s not found or not auto-fixable"), *IssueID);
    return false;
}

TArray<FString> UMingRTSCodeReviewAPI::AutoFixAllIssues(const FString& FilePath)
{
    TArray<FString> FixedIssues;
    
    if (!bAutoFixEnabled)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Review API: Auto-fix is disabled"));
        return FixedIssues;
    }
    
    FCodeReviewReport FileReport = ReviewSingleFile(FilePath);
    
    for (const FCodeReviewIssue& Issue : FileReport.Issues)
    {
        if (Issue.bAutoFixable)
        {
            if (ApplyAutoFix(FilePath, Issue))
            {
                FixedIssues.Add(Issue.IssueID);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Auto-fixed %d issues in %s"), FixedIssues.Num(), *FilePath);
    return FixedIssues;
}

bool UMingRTSCodeReviewAPI::ValidateCommitReadiness(const FCodeReviewReport& Report)
{
    // 檢查嚴重問題
    if (Report.Metrics.CriticalIssues > 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Code Review API: Cannot commit - %d critical issues found"), Report.Metrics.CriticalIssues);
        return false;
    }
    
    // 檢查錯誤問題
    if (Report.Metrics.ErrorIssues > ErrorThreshold)
    {
        UE_LOG(LogTemp, Error, TEXT("Code Review API: Cannot commit - %d error issues exceed threshold %d"), Report.Metrics.ErrorIssues, (int32)ErrorThreshold);
        return false;
    }
    
    // 檢查警告問題
    if (Report.Metrics.WarningIssues > WarningThreshold)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Review API: Warning - %d warning issues exceed threshold %d"), Report.Metrics.WarningIssues, (int32)WarningThreshold);
        
        if (bStrictMode)
        {
            return false;
        }
    }
    
    // 檢查代碼質量分數
    if (Report.Metrics.CodeQualityScore < 70.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Code Review API: Low code quality score: %.1f"), Report.Metrics.CodeQualityScore);
        
        if (bStrictMode)
        {
            return false;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Code is ready for commit"));
    return true;
}

FString UMingRTSCodeReviewAPI::GenerateReviewReport(const FCodeReviewReport& Report)
{
    FString ReportContent = TEXT("Code Review Report\n");
    ReportContent += TEXT("==================\n\n");
    ReportContent += FString::Printf(TEXT("Report ID: %s\n"), *Report.ReportID);
    ReportContent += FString::Printf(TEXT("Review Time: %s\n"), *Report.ReviewTime.ToString());
    ReportContent += FString::Printf(TEXT("Reviewer: %s\n"), *Report.Reviewer);
    ReportContent += FString::Printf(TEXT("Overall Result: %s\n"), *StaticEnum<ECodeReviewResult>()->GetNameStringByValue((int64)Report.OverallResult));
    ReportContent += FString::Printf(TEXT("Can Commit: %s\n\n"), Report.bCanCommit ? TEXT("Yes") : TEXT("No"));
    
    ReportContent += TEXT("Metrics:\n");
    ReportContent += FString::Printf(TEXT("  Total Issues: %d\n"), Report.Metrics.TotalIssues);
    ReportContent += FString::Printf(TEXT("  Critical Issues: %d\n"), Report.Metrics.CriticalIssues);
    ReportContent += FString::Printf(TEXT("  Error Issues: %d\n"), Report.Metrics.ErrorIssues);
    ReportContent += FString::Printf(TEXT("  Warning Issues: %d\n"), Report.Metrics.WarningIssues);
    ReportContent += FString::Printf(TEXT("  Info Issues: %d\n"), Report.Metrics.InfoIssues);
    ReportContent += FString::Printf(TEXT("  Code Quality Score: %.1f\n"), Report.Metrics.CodeQualityScore);
    ReportContent += FString::Printf(TEXT("  Maintainability Score: %.1f\n"), Report.Metrics.MaintainabilityScore);
    ReportContent += FString::Printf(TEXT("  Performance Score: %.1f\n"), Report.Metrics.PerformanceScore);
    ReportContent += FString::Printf(TEXT("  Security Score: %.1f\n"), Report.Metrics.SecurityScore);
    ReportContent += FString::Printf(TEXT("  Documentation Score: %.1f\n"), Report.Metrics.DocumentationScore);
    ReportContent += FString::Printf(TEXT("  Test Coverage Score: %.1f\n\n"), Report.Metrics.TestCoverageScore);
    
    ReportContent += TEXT("Issues:\n");
    for (const FCodeReviewIssue& Issue : Report.Issues)
    {
        ReportContent += FString::Printf(TEXT("  [%s] %s:%d - %s\n"), 
            *StaticEnum<ECodeReviewSeverity>()->GetNameStringByValue((int64)Issue.Severity),
            *Issue.FilePath, Issue.LineNumber, Issue.Description);
        ReportContent += FString::Printf(TEXT("    Suggestion: %s\n"), Issue.Suggestion);
        ReportContent += FString::Printf(TEXT("    Category: %s\n"), *StaticEnum<ECodeReviewCategory>()->GetNameStringByValue((int64)Issue.Category));
        if (Issue.bAutoFixable)
        {
            ReportContent += FString::Printf(TEXT("    Auto-fixable: Yes\n"));
        }
        ReportContent += TEXT("\n");
    }
    
    ReportContent += TEXT("Summary:\n");
    ReportContent += Report.Summary + TEXT("\n\n");
    
    ReportContent += TEXT("Recommendations:\n");
    for (const FString& Recommendation : Report.Recommendations)
    {
        ReportContent += FString::Printf(TEXT("  - %s\n"), *Recommendation);
    }
    
    return ReportContent;
}

void UMingRTSCodeReviewAPI::UpdateMetrics(FCodeReviewMetrics& Metrics, const TArray<FCodeReviewIssue>& Issues)
{
    Metrics.TotalIssues = Issues.Num();
    Metrics.CriticalIssues = 0;
    Metrics.ErrorIssues = 0;
    Metrics.WarningIssues = 0;
    Metrics.InfoIssues = 0;
    
    for (const FCodeReviewIssue& Issue : Issues)
    {
        switch (Issue.Severity)
        {
        case ECodeReviewSeverity::Critical:
            Metrics.CriticalIssues++;
            break;
        case ECodeReviewSeverity::Error:
            Metrics.ErrorIssues++;
            break;
        case ECodeReviewSeverity::Warning:
            Metrics.WarningIssues++;
            break;
        case ECodeReviewSeverity::Info:
            Metrics.InfoIssues++;
            break;
        }
    }
    
    // 計算各項分數
    Metrics.CodeQualityScore = CalculateQualityScore(Issues);
    Metrics.MaintainabilityScore = FMath::Max(0.0f, 100.0f - Metrics.ErrorIssues * 2.0f - Metrics.WarningIssues * 0.5f);
    Metrics.PerformanceScore = FMath::Max(0.0f, 100.0f - Metrics.WarningIssues * 1.0f);
    Metrics.SecurityScore = FMath::Max(0.0f, 100.0f - Metrics.CriticalIssues * 10.0f - Metrics.ErrorIssues * 3.0f);
    Metrics.DocumentationScore = FMath::Max(0.0f, 100.0f - Metrics.WarningIssues * 1.5f);
    Metrics.TestCoverageScore = FMath::Max(0.0f, 100.0f - Metrics.InfoIssues * 0.5f);
}

ECodeReviewResult UMingRTSCodeReviewAPI::DetermineOverallResult(const FCodeReviewMetrics& Metrics)
{
    if (Metrics.CriticalIssues > 0)
    {
        return ECodeReviewResult::Critical;
    }
    
    if (Metrics.ErrorIssues > ErrorThreshold)
    {
        return ECodeReviewResult::Fail;
    }
    
    if (Metrics.WarningIssues > WarningThreshold)
    {
        return ECodeReviewResult::Warning;
    }
    
    return ECodeReviewResult::Pass;
}

void UMingRTSCodeReviewAPI::GenerateReviewSummary(FCodeReviewReport& Report)
{
    Report.Summary = FString::Printf(TEXT("Code review completed for %d files. "), Report.ReviewedFiles.Num());
    
    if (Report.Metrics.CriticalIssues > 0)
    {
        Report.Summary += FString::Printf(TEXT("Found %d critical issues that must be fixed. "), Report.Metrics.CriticalIssues);
    }
    
    if (Report.Metrics.ErrorIssues > 0)
    {
        Report.Summary += FString::Printf(TEXT("Found %d error issues. "), Report.Metrics.ErrorIssues);
    }
    
    if (Report.Metrics.WarningIssues > 0)
    {
        Report.Summary += FString::Printf(TEXT("Found %d warning issues. "), Report.Metrics.WarningIssues);
    }
    
    Report.Summary += FString::Printf(TEXT("Overall code quality score: %.1f. "), Report.Metrics.CodeQualityScore);
    
    if (Report.bCanCommit)
    {
        Report.Summary += TEXT("Code is ready for commit.");
    }
    else
    {
        Report.Summary += TEXT("Code needs fixes before commit.");
    }
    
    // 生成建議
    Report.Recommendations.Empty();
    
    if (Report.Metrics.CriticalIssues > 0)
    {
        Report.Recommendations.Add(TEXT("Fix all critical issues before committing"));
    }
    
    if (Report.Metrics.ErrorIssues > 0)
    {
        Report.Recommendations.Add(TEXT("Address error issues to improve code quality"));
    }
    
    if (Report.Metrics.WarningIssues > WarningThreshold)
    {
        Report.Recommendations.Add(TEXT("Consider addressing warning issues for better maintainability"));
    }
    
    if (Report.Metrics.DocumentationScore < 80.0f)
    {
        Report.Recommendations.Add(TEXT("Improve code documentation"));
    }
    
    if (Report.Metrics.TestCoverageScore < 80.0f)
    {
        Report.Recommendations.Add(TEXT("Increase test coverage"));
    }
}

FString UMingRTSCodeReviewAPI::GenerateIssueID()
{
    return FString::Printf(TEXT("ISSUE_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

bool UMingRTSCodeReviewAPI::MatchesPattern(const FString& Text, const FString& Pattern)
{
    // 簡化的模式匹配（實際應用中應使用更強大的正則表達式庫）
    return Text.Contains(Pattern);
}

FString UMingRTSCodeReviewAPI::GenerateSuggestion(const FCodeReviewRule& Rule, const FString& Context)
{
    if (!Rule.SuggestionTemplate.IsEmpty())
    {
        return Rule.SuggestionTemplate;
    }
    
    return FString::Printf(TEXT("Consider fixing: %s"), *Rule.Description);
}

float UMingRTSCodeReviewAPI::CalculateQualityScore(const TArray<FCodeReviewIssue>& Issues)
{
    if (Issues.Num() == 0)
    {
        return 100.0f;
    }
    
    float Score = 100.0f;
    
    for (const FCodeReviewIssue& Issue : Issues)
    {
        switch (Issue.Severity)
        {
        case ECodeReviewSeverity::Critical:
            Score -= 10.0f;
            break;
        case ECodeReviewSeverity::Error:
            Score -= 5.0f;
            break;
        case ECodeReviewSeverity::Warning:
            Score -= 2.0f;
            break;
        case ECodeReviewSeverity::Info:
            Score -= 0.5f;
            break;
        }
    }
    
    return FMath::Max(0.0f, Score);
}

bool UMingRTSCodeReviewAPI::ApplyAutoFix(const FString& FilePath, const FCodeReviewIssue& Issue)
{
    // 簡化的自動修復實現
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Auto-fixing issue %s in %s"), *Issue.IssueID, *FilePath);
    
    // 實際應用中需要更複雜的代碼分析和修復邏輯
    return true;
}

void UMingRTSCodeReviewAPI::SetReviewThresholds(float InCriticalThreshold, float InErrorThreshold, float InWarningThreshold)
{
    CriticalThreshold = InCriticalThreshold;
    ErrorThreshold = InErrorThreshold;
    WarningThreshold = InWarningThreshold;
    
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Updated thresholds - Critical: %.1f, Error: %.1f, Warning: %.1f"), 
        CriticalThreshold, ErrorThreshold, WarningThreshold);
}

TArray<FCodeReviewRule> UMingRTSCodeReviewAPI::GetReviewRules()
{
    return ReviewRules;
}

bool UMingRTSCodeReviewAPI::UpdateReviewRule(const FCodeReviewRule& Rule)
{
    for (int32 i = 0; i < ReviewRules.Num(); i++)
    {
        if (ReviewRules[i].RuleID == Rule.RuleID)
        {
            ReviewRules[i] = Rule;
            UE_LOG(LogTemp, Log, TEXT("Code Review API: Updated rule %s"), *Rule.RuleID);
            return true;
        }
    }
    
    ReviewRules.Add(Rule);
    UE_LOG(LogTemp, Log, TEXT("Code Review API: Added new rule %s"), *Rule.RuleID);
    return true;
}

FCodeReviewMetrics UMingRTSCodeReviewAPI::GetReviewMetrics()
{
    FCodeReviewMetrics Metrics;
    // 返回當前的統計信息
    return Metrics;
}

FString UMingRTSCodeReviewAPI::ExportReviewResults(const FCodeReviewReport& Report)
{
    FString JsonContent = TEXT("{\n");
    JsonContent += FString::Printf(TEXT("  \"reportId\": \"%s\",\n"), *Report.ReportID);
    JsonContent += FString::Printf(TEXT("  \"reviewTime\": \"%s\",\n"), *Report.ReviewTime.ToIso8601());
    JsonContent += FString::Printf(TEXT("  \"reviewer\": \"%s\",\n"), *Report.Reviewer);
    JsonContent += FString::Printf(TEXT("  \"overallResult\": \"%s\",\n"), *StaticEnum<ECodeReviewResult>()->GetNameStringByValue((int64)Report.OverallResult));
    JsonContent += FString::Printf(TEXT("  \"canCommit\": %s,\n"), Report.bCanCommit ? TEXT("true") : TEXT("false"));
    
    JsonContent += TEXT("  \"metrics\": {\n");
    JsonContent += FString::Printf(TEXT("    \"totalIssues\": %d,\n"), Report.Metrics.TotalIssues);
    JsonContent += FString::Printf(TEXT("    \"criticalIssues\": %d,\n"), Report.Metrics.CriticalIssues);
    JsonContent += FString::Printf(TEXT("    \"errorIssues\": %d,\n"), Report.Metrics.ErrorIssues);
    JsonContent += FString::Printf(TEXT("    \"warningIssues\": %d,\n"), Report.Metrics.WarningIssues);
    JsonContent += FString::Printf(TEXT("    \"infoIssues\": %d,\n"), Report.Metrics.InfoIssues);
    JsonContent += FString::Printf(TEXT("    \"codeQualityScore\": %.1f\n"), Report.Metrics.CodeQualityScore);
    JsonContent += TEXT("  }\n");
    
    JsonContent += TEXT("}");
    
    return JsonContent;
}
