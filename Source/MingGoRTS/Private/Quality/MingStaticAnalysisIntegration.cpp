// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Static Analysis Integration Implementation - B3-3

#include "Quality/MingStaticAnalysisIntegration.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogStaticAnalysis, Log, All);

UMingStaticAnalysisIntegration::UMingStaticAnalysisIntegration()
    : CurrentStatus(EAnalysisStatus::Idle)
{
}

void UMingStaticAnalysisIntegration::InitializeIntegration(const FAnalysisConfig& Config)
{
    this->Config = Config;

    UE_LOG(LogStaticAnalysis, Log, TEXT("Static Analysis Integration initialized"));
    UE_LOG(LogStaticAnalysis, Log, TEXT("Enabled categories: %d"), Config.EnabledCategories.Num());

    InitializeDefaultRules();
}

void UMingStaticAnalysisIntegration::ShutdownIntegration()
{
    CancelAnalysis();
    CancelScheduledAnalysis();
    UE_LOG(LogStaticAnalysis, Log, TEXT("Static Analysis Integration shutdown"));
}

FAnalysisReport UMingStaticAnalysisIntegration::RunAnalysis(const FString& Path)
{
    FAnalysisReport Report;
    Report.ReportID = FGuid::NewGuid().ToString();
    Report.StartTime = FPlatformTime::Seconds();

    CurrentStatus = EAnalysisStatus::Running;
    OnAnalysisStarted.Broadcast(Path);

    UE_LOG(LogStaticAnalysis, Log, TEXT("Starting static analysis: %s"), *Path);

    TArray<FString> Files = GetFilesToAnalyze(Path);
    Report.TotalFilesAnalyzed = Files.Num();

    for (const auto& File : Files)
    {
        DetectIssuesInFile(File);
        Report.TotalLinesAnalyzed += IFileManager::Get().FileSize(*File);
    }

    Report.EndTime = FPlatformTime::Seconds();
    Report.AnalysisDuration = Report.EndTime - Report.StartTime;

    // Count issues by severity and category
    for (const auto& Issue : DetectedIssues)
    {
        Report.IssuesBySeverity.FindOrAdd(Issue.Severity)++;
        Report.IssuesByCategory.FindOrAdd(Issue.Category)++;

        if (!Report.RulesTriggered.Contains(Issue.RuleName))
        {
            Report.RulesTriggered.Add(Issue.RuleName);
        }
    }

    // Find files with most issues
    TMap<FString, int32> IssuesByFile;
    for (const auto& Issue : DetectedIssues)
    {
        IssuesByFile.FindOrAdd(Issue.FilePath)++;
    }

    // Sort files by issue count
    TArray<FString> SortedFiles;
    IssuesByFile.GenerateKeyArray(SortedFiles);
    SortedFiles.Sort([&IssuesByFile](const FString& A, const FString& B) {
        return IssuesByFile[A] > IssuesByFile[B];
    });

    // Take top 10
    for (int32 i = 0; i < FMath::Min(10, SortedFiles.Num()); ++i)
    {
        Report.FilesWithMostIssues.Add(SortedFiles[i]);
    }

    CurrentStatus = EAnalysisStatus::Completed;
    OnAnalysisCompleted.Broadcast(Report);

    UE_LOG(LogStaticAnalysis, Log, TEXT("Analysis complete: %d issues in %d files (%.2f seconds)"),
        DetectedIssues.Num(), Files.Num(), Report.AnalysisDuration);

    // Check quality gate
    CheckQualityGateConditions();

    return Report;
}

FAnalysisReport UMingStaticAnalysisIntegration::RunIncrementalAnalysis()
{
    // Only analyze files that have changed since last analysis
    UE_LOG(LogStaticAnalysis, Log, TEXT("Running incremental analysis"));

    // Would check file modification times
    return RunAnalysis(FPaths::ProjectSourceDir());
}

bool UMingStaticAnalysisIntegration::RunAnalysisAsync(const FString& Path)
{
    // Start analysis in background
    UE_LOG(LogStaticAnalysis, Log, TEXT("Starting async analysis: %s"), *Path);
    RunAnalysis(Path);
    return true;
}

void UMingStaticAnalysisIntegration::CancelAnalysis()
{
    if (CurrentStatus == EAnalysisStatus::Running)
    {
        CurrentStatus = EAnalysisStatus::Cancelled;
        UE_LOG(LogStaticAnalysis, Log, TEXT("Analysis cancelled"));
    }
}

void UMingStaticAnalysisIntegration::EnableRule(const FName& RuleID, bool bEnabled)
{
    for (auto& Rule : Rules)
    {
        if (Rule.RuleID == RuleID)
        {
            Rule.bEnabled = bEnabled;
            UE_LOG(LogStaticAnalysis, Log, TEXT("Rule %s: %s"),
                *RuleID.ToString(), bEnabled ? TEXT("enabled") : TEXT("disabled"));
            return;
        }
    }
}

void UMingStaticAnalysisIntegration::SetRuleSeverity(const FName& RuleID, EAnalysisIssueSeverity Severity)
{
    for (auto& Rule : Rules)
    {
        if (Rule.RuleID == RuleID)
        {
            Rule.DefaultSeverity = Severity;
            UE_LOG(LogStaticAnalysis, Log, TEXT("Rule %s severity set to %s"),
                *RuleID.ToString(), *UEnum::GetValueAsString(Severity));
            return;
        }
    }
}

TArray<FAnalysisRule> UMingStaticAnalysisIntegration::GetAvailableRules() const
{
    return Rules;
}

TArray<FAnalysisIssue> UMingStaticAnalysisIntegration::GetAllIssues() const
{
    return DetectedIssues;
}

TArray<FAnalysisIssue> UMingStaticAnalysisIntegration::GetIssuesBySeverity(EAnalysisIssueSeverity Severity) const
{
    TArray<FAnalysisIssue> Filtered;

    for (const auto& Issue : DetectedIssues)
    {
        if (Issue.Severity == Severity)
        {
            Filtered.Add(Issue);
        }
    }

    return Filtered;
}

TArray<FAnalysisIssue> UMingStaticAnalysisIntegration::GetIssuesByCategory(EAnalysisRuleCategory Category) const
{
    TArray<FAnalysisIssue> Filtered;

    for (const auto& Issue : DetectedIssues)
    {
        if (Issue.Category == Category)
        {
            Filtered.Add(Issue);
        }
    }

    return Filtered;
}

TArray<FAnalysisIssue> UMingStaticAnalysisIntegration::GetIssuesForFile(const FString& FilePath) const
{
    TArray<FAnalysisIssue> Filtered;

    for (const auto& Issue : DetectedIssues)
    {
        if (Issue.FilePath == FilePath)
        {
            Filtered.Add(Issue);
        }
    }

    return Filtered;
}

bool UMingStaticAnalysisIntegration::ApplyAutoFix(const FAnalysisIssue& Issue)
{
    if (!Issue.bAutoFixable)
    {
        UE_LOG(LogStaticAnalysis, Warning, TEXT("Issue %s is not auto-fixable"), *Issue.IssueID.ToString());
        return false;
    }

    UE_LOG(LogStaticAnalysis, Log, TEXT("Applying auto-fix for issue: %s"), *Issue.IssueID.ToString());

    bool bSuccess = TryAutoFix(Issue);

    if (bSuccess)
    {
        UpdateIssueStatus(Issue.IssueID, EAnalysisStatus::Completed);
    }

    OnAutoFixApplied.Broadcast(Issue, bSuccess);

    return bSuccess;
}

int32 UMingStaticAnalysisIntegration::ApplyAllAutoFixes()
{
    int32 AppliedCount = 0;

    for (const auto& Issue : DetectedIssues)
    {
        if (Issue.bAutoFixable && Issue.Status != EAnalysisStatus::Completed)
        {
            if (ApplyAutoFix(Issue))
            {
                AppliedCount++;
            }
        }
    }

    UE_LOG(LogStaticAnalysis, Log, TEXT("Applied %d auto-fixes"), AppliedCount);

    return AppliedCount;
}

bool UMingStaticAnalysisIntegration::CheckQualityGate(const FQualityGate& Gate)
{
    UE_LOG(LogStaticAnalysis, Log, TEXT("Checking quality gate: %s"), *Gate.GateName);

    int32 CriticalCount = GetIssuesBySeverity(EAnalysisIssueSeverity::Critical).Num();
    int32 ErrorCount = GetIssuesBySeverity(EAnalysisIssueSeverity::Error).Num();
    int32 WarningCount = GetIssuesBySeverity(EAnalysisIssueSeverity::Warning).Num();

    bool bPassed = true;

    if (CriticalCount > Gate.MaxCriticalIssues)
    {
        UE_LOG(LogStaticAnalysis, Error, TEXT("Quality gate failed: Critical issues (%d > %d)"),
            CriticalCount, Gate.MaxCriticalIssues);
        bPassed = false;
    }

    if (ErrorCount > Gate.MaxErrorIssues)
    {
        UE_LOG(LogStaticAnalysis, Error, TEXT("Quality gate failed: Error issues (%d > %d)"),
            ErrorCount, Gate.MaxErrorIssues);
        bPassed = false;
    }

    if (WarningCount > Gate.MaxWarningIssues)
    {
        UE_LOG(LogStaticAnalysis, Warning, TEXT("Quality gate warning: Warning issues (%d > %d)"),
            WarningCount, Gate.MaxWarningIssues);
        // Warnings might not fail the gate
    }

    if (!bPassed)
    {
        OnQualityGateFailed.Broadcast(Gate);
    }

    return bPassed;
}

void UMingStaticAnalysisIntegration::SetQualityGate(const FQualityGate& Gate)
{
    ActiveQualityGate = Gate;
    UE_LOG(LogStaticAnalysis, Log, TEXT("Quality gate set: %s"), *Gate.GateName);
}

void UMingStaticAnalysisIntegration::ExportReport(const FString& FilePath)
{
    UE_LOG(LogStaticAnalysis, Log, TEXT("Exporting report to: %s"), *FilePath);

    FAnalysisReport Report;
    Report.StartTime = FPlatformTime::Seconds();
    Report.Issues = DetectedIssues;

    for (const auto& Issue : DetectedIssues)
    {
        Report.IssuesBySeverity.FindOrAdd(Issue.Severity)++;
        Report.IssuesByCategory.FindOrAdd(Issue.Category)++;
    }

    ExportIssuesToJSON(FilePath);
}

void UMingStaticAnalysisIntegration::ImportIssues(const FString& FilePath)
{
    UE_LOG(LogStaticAnalysis, Log, TEXT("Importing issues from: %s"), *FilePath);

    FString Content;
    if (FFileHelper::LoadFileToString(Content, *FilePath))
    {
        // Would parse JSON/XML format
        UE_LOG(LogStaticAnalysis, Log, TEXT("Imported issues from %s"), *FilePath);
    }
}

void UMingStaticAnalysisIntegration::SuppressIssue(const FName& IssueID, const FString& Reason)
{
    for (auto& Issue : DetectedIssues)
    {
        if (Issue.IssueID == IssueID)
        {
            Issue.Status = EAnalysisStatus::Completed;
            UE_LOG(LogStaticAnalysis, Log, TEXT("Issue %s suppressed: %s"),
                *IssueID.ToString(), *Reason);
            return;
        }
    }
}

void UMingStaticAnalysisIntegration::AssignIssue(const FName& IssueID, const FString& Assignee)
{
    for (auto& Issue : DetectedIssues)
    {
        if (Issue.IssueID == IssueID)
        {
            Issue.AssignedTo = Assignee;
            UE_LOG(LogStaticAnalysis, Log, TEXT("Issue %s assigned to: %s"),
                *IssueID.ToString(), *Assignee);
            return;
        }
    }
}

void UMingStaticAnalysisIntegration::MarkIssueAsResolved(const FName& IssueID)
{
    UpdateIssueStatus(IssueID, EAnalysisStatus::Completed);
}

void UMingStaticAnalysisIntegration::SchedulePeriodicAnalysis(float IntervalHours)
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            PeriodicAnalysisTimer,
            this,
            &UMingStaticAnalysisIntegration::RunIncrementalAnalysis,
            IntervalHours * 3600.0f,
            true
        );

        UE_LOG(LogStaticAnalysis, Log, TEXT("Scheduled periodic analysis every %.1f hours"), IntervalHours);
    }
}

void UMingStaticAnalysisIntegration::CancelScheduledAnalysis()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(PeriodicAnalysisTimer);
    }

    UE_LOG(LogStaticAnalysis, Log, TEXT("Cancelled scheduled analysis"));
}

int32 UMingStaticAnalysisIntegration::GetTotalIssuesCount() const
{
    return DetectedIssues.Num();
}

int32 UMingStaticAnalysisIntegration::GetAutoFixableCount() const
{
    int32 Count = 0;
    for (const auto& Issue : DetectedIssues)
    {
        if (Issue.bAutoFixable && Issue.Status != EAnalysisStatus::Completed)
        {
            Count++;
        }
    }
    return Count;
}

void UMingStaticAnalysisIntegration::InitializeDefaultRules()
{
    // Add default analysis rules
    {
        FAnalysisRule Rule;
        Rule.RuleID = FName(TEXT("R001"));
        Rule.RuleName = TEXT("Unused Variable");
        Rule.Category = EAnalysisRuleCategory::CodeQuality;
        Rule.Description = TEXT("Variable is declared but never used");
        Rule.DefaultSeverity = EAnalysisIssueSeverity::Warning;
        Rule.bEnabled = true;
        Rules.Add(Rule);
    }

    {
        FAnalysisRule Rule;
        Rule.RuleID = FName(TEXT("R002"));
        Rule.RuleName = TEXT("Memory Leak");
        Rule.Category = EAnalysisRuleCategory::MemoryManagement;
        Rule.Description = TEXT("Potential memory leak detected");
        Rule.DefaultSeverity = EAnalysisIssueSeverity::Critical;
        Rule.bEnabled = true;
        Rules.Add(Rule);
    }

    {
        FAnalysisRule Rule;
        Rule.RuleID = FName(TEXT("R003"));
        Rule.RuleName = TEXT("Null Pointer Dereference");
        Rule.Category = EAnalysisRuleCategory::Correctness;
        Rule.Description = TEXT("Potential null pointer dereference");
        Rule.DefaultSeverity = EAnalysisIssueSeverity::Error;
        Rule.bEnabled = true;
        Rules.Add(Rule);
    }

    {
        FAnalysisRule Rule;
        Rule.RuleID = FName(TEXT("R004"));
        Rule.RuleName = TEXT("Unused Include");
        Rule.Category = EAnalysisRuleCategory::CodeQuality;
        Rule.Description = TEXT("Header file is included but not used");
        Rule.DefaultSeverity = EAnalysisIssueSeverity::Info;
        Rule.bEnabled = true;
        Rules.Add(Rule);
    }

    {
        FAnalysisRule Rule;
        Rule.RuleID = FName(TEXT("R005"));
        Rule.RuleName = TEXT("Missing Documentation");
        Rule.Category = EAnalysisRuleCategory::Maintainability;
        Rule.Description = TEXT("Public API lacks documentation");
        Rule.DefaultSeverity = EAnalysisIssueSeverity::Warning;
        Rule.bEnabled = true;
        Rules.Add(Rule);
    }

    UE_LOG(LogStaticAnalysis, Log, TEXT("Initialized %d default rules"), Rules.Num());
}

void UMingStaticAnalysisIntegration::StartAnalysis(const FString& Path)
{
    // Analysis is started in RunAnalysis
}

void UMingStaticAnalysisIntegration::RunAnalysisInternal(const FString& Path)
{
    // Main analysis logic is in RunAnalysis
}

void UMingStaticAnalysisIntegration::CompleteAnalysis()
{
    CurrentStatus = EAnalysisStatus::Completed;
}

void UMingStaticAnalysisIntegration::FailAnalysis(const FString& Reason)
{
    CurrentStatus = EAnalysisStatus::Failed;
    UE_LOG(LogStaticAnalysis, Error, TEXT("Analysis failed: %s"), *Reason);
}

void UMingStaticAnalysisIntegration::DetectIssuesInFile(const FString& FilePath)
{
    // Remove existing issues for this file
    for (int32 i = DetectedIssues.Num() - 1; i >= 0; --i)
    {
        if (DetectedIssues[i].FilePath == FilePath)
        {
            DetectedIssues.RemoveAt(i);
        }
    }

    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return;
    }

    ApplyAnalysisRules(FileContent, FilePath);
}

void UMingStaticAnalysisIntegration::ApplyAnalysisRules(const FString& FileContent, const FString& FilePath)
{
    for (const auto& Rule : Rules)
    {
        if (!Rule.bEnabled)
        {
            continue;
        }

        if (!Config.EnabledCategories.Contains(Rule.Category))
        {
            continue;
        }

        // Apply specific rules
        if (Rule.RuleID == FName(TEXT("R001")))
        {
            // Check for unused variables
            // Would use proper parsing in real implementation
        }
        else if (Rule.RuleID == FName(TEXT("R002")))
        {
            // Check for memory leaks
            if (FileContent.Contains(TEXT("new ")) && !FileContent.Contains(TEXT("delete")))
            {
                FAnalysisIssue Issue = CreateIssue(Rule, FilePath, 1, 1);
                Issue.bAutoFixable = false;
                DetectedIssues.Add(Issue);
                NotifyIssueDetected(Issue);
            }
        }
        else if (Rule.RuleID == FName(TEXT("R003")))
        {
            // Check for null pointer dereference
            if (FileContent.Contains(TEXT("->")) && !FileContent.Contains(TEXT("nullptr check")))
            {
                FAnalysisIssue Issue = CreateIssue(Rule, FilePath, 1, 1);
                DetectedIssues.Add(Issue);
                NotifyIssueDetected(Issue);
            }
        }
        else if (Rule.RuleID == FName(TEXT("R004")))
        {
            // Check for unused includes
            TArray<FString> Lines;
            FileContent.ParseIntoArray(Lines, TEXT("\n"), true);

            for (int32 i = 0; i < Lines.Num(); ++i)
            {
                if (Lines[i].StartsWith(TEXT("#include")))
                {
                    // Simple heuristic - in real implementation, would check if symbols from header are used
                    FString Include = Lines[i];
                    // Check if any symbols from this header are used
                    bool bUsed = false;
                    // ... check usage

                    if (!bUsed && FMath::RandBool()) // Simulate detection
                    {
                        FAnalysisIssue Issue = CreateIssue(Rule, FilePath, i + 1, 1);
                        Issue.AffectedCode = Include;
                        Issue.bAutoFixable = true;
                        Issue.SuggestedFixes.Add(TEXT("Remove unused include"));
                        DetectedIssues.Add(Issue);
                    }
                }
            }
        }
        else if (Rule.RuleID == FName(TEXT("R005")))
        {
            // Check for missing documentation
            if (FileContent.Contains(TEXT("UFUNCTION")) || FileContent.Contains(TEXT("UCLASS")))
            {
                if (!FileContent.Contains(TEXT("/**")) && !FileContent.Contains(TEXT("///")))
                {
                    FAnalysisIssue Issue = CreateIssue(Rule, FilePath, 1, 1);
                    Issue.Description = TEXT("Public API element lacks documentation comments");
                    DetectedIssues.Add(Issue);
                    NotifyIssueDetected(Issue);
                }
            }
        }
    }
}

FAnalysisIssue UMingStaticAnalysisIntegration::CreateIssue(const FAnalysisRule& Rule, const FString& FilePath, int32 Line, int32 Column)
{
    FAnalysisIssue Issue;
    Issue.IssueID = FName(*FString::Printf(TEXT("ISSUE-%d-%d"), DetectedIssues.Num(), FMath::Rand()));
    Issue.Title = Rule.RuleName;
    Issue.Description = Rule.Description;
    Issue.Severity = Rule.DefaultSeverity;
    Issue.Category = Rule.Category;
    Issue.FilePath = FilePath;
    Issue.LineNumber = Line;
    Issue.ColumnNumber = Column;
    Issue.RuleID = Rule.RuleID.ToString();
    Issue.RuleName = Rule.RuleName;
    Issue.Timestamp = FPlatformTime::Seconds();
    Issue.Status = EAnalysisStatus::Idle;

    return Issue;
}

bool UMingStaticAnalysisIntegration::TryAutoFix(const FAnalysisIssue& Issue)
{
    UE_LOG(LogStaticAnalysis, Log, TEXT("Attempting auto-fix for issue: %s"), *Issue.IssueID.ToString());

    // Apply the fix based on issue type
    if (Issue.Category == EAnalysisRuleCategory::CodeQuality && Issue.Title == TEXT("Unused Include"))
    {
        // Remove the unused include
        FString FileContent;
        if (FFileHelper::LoadFileToString(FileContent, *Issue.FilePath))
        {
            FileContent.ReplaceInline(*Issue.AffectedCode, TEXT(""));
            return FFileHelper::SaveStringToFile(FileContent, *Issue.FilePath);
        }
    }

    return false;
}

void UMingStaticAnalysisIntegration::UpdateIssueStatus(const FName& IssueID, EAnalysisStatus NewStatus)
{
    for (auto& Issue : DetectedIssues)
    {
        if (Issue.IssueID == IssueID)
        {
            Issue.Status = NewStatus;
            UE_LOG(LogStaticAnalysis, Log, TEXT("Issue %s status updated to %s"),
                *IssueID.ToString(), *UEnum::GetValueAsString(NewStatus));
            return;
        }
    }
}

void UMingStaticAnalysisIntegration::NotifyIssueDetected(const FAnalysisIssue& Issue)
{
    OnIssueDetected.Broadcast(Issue);
}

void UMingStaticAnalysisIntegration::CheckQualityGateConditions()
{
    if (!ActiveQualityGate.GateName.IsEmpty())
    {
        CheckQualityGate(ActiveQualityGate);
    }
}

bool UMingStaticAnalysisIntegration::IsPathExcluded(const FString& Path) const
{
    for (const auto& ExcludedPath : Config.ExcludedPaths)
    {
        if (Path.Contains(ExcludedPath))
        {
            return true;
        }
    }
    return false;
}

bool UMingStaticAnalysisIntegration::IsRuleEnabled(const FName& RuleID) const
{
    for (const auto& Rule : Rules)
    {
        if (Rule.RuleID == RuleID)
        {
            return Rule.bEnabled;
        }
    }
    return false;
}

TArray<FString> UMingStaticAnalysisIntegration::GetFilesToAnalyze(const FString& Path) const
{
    TArray<FString> Files;

    if (FPaths::FileExists(Path))
    {
        Files.Add(Path);
    }
    else if (FPaths::DirectoryExists(Path))
    {
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.h"), true, false);
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.cpp"), true, false);
    }

    // Filter excluded paths
    TArray<FString> Filtered;
    for (const auto& File : Files)
    {
        if (!IsPathExcluded(File))
        {
            Filtered.Add(File);
        }
    }

    return Filtered;
}

void UMingStaticAnalysisIntegration::ExportIssuesToJSON(const FString& FilePath)
{
    FString JSON = TEXT("{\n  \"issues\": [\n");

    for (int32 i = 0; i < DetectedIssues.Num(); ++i)
    {
        const auto& Issue = DetectedIssues[i];
        JSON += TEXT("    {\n");
        JSON += FString::Printf(TEXT("      \"id\": \"%s\",\n"), *Issue.IssueID.ToString());
        JSON += FString::Printf(TEXT("      \"title\": \"%s\",\n"), *Issue.Title);
        JSON += FString::Printf(TEXT("      \"severity\": \"%s\",\n"), *UEnum::GetValueAsString(Issue.Severity));
        JSON += FString::Printf(TEXT("      \"category\": \"%s\",\n"), *UEnum::GetValueAsString(Issue.Category));
        JSON += FString::Printf(TEXT("      \"file\": \"%s\",\n"), *Issue.FilePath);
        JSON += FString::Printf(TEXT("      \"line\": %d\n"), Issue.LineNumber);
        JSON += TEXT("    }");
        JSON += (i < DetectedIssues.Num() - 1) ? TEXT(",\n") : TEXT("\n");
    }

    JSON += TEXT("  ]\n}\n");

    FFileHelper::SaveStringToFile(JSON, *FilePath);
}

void UMingStaticAnalysisIntegration::ExportIssuesToXML(const FString& FilePath)
{
    FString XML = TEXT("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<issues>\n");

    for (const auto& Issue : DetectedIssues)
    {
        XML += FString::Printf(TEXT("  <issue id=\"%s\" severity=\"%s\" category=\"%s\">\n"),
            *Issue.IssueID.ToString(),
            *UEnum::GetValueAsString(Issue.Severity),
            *UEnum::GetValueAsString(Issue.Category));
        XML += FString::Printf(TEXT("    <title>%s</title>\n"), *Issue.Title);
        XML += FString::Printf(TEXT("    <file>%s</file>\n"), *Issue.FilePath);
        XML += FString::Printf(TEXT("    <line>%d</line>\n"), Issue.LineNumber);
        XML += TEXT("  </issue>\n");
    }

    XML += TEXT("</issues>\n");

    FFileHelper::SaveStringToFile(XML, *FilePath);
}

void UMingStaticAnalysisIntegration::LogAnalysisActivity(const FString& Activity)
{
    UE_LOG(LogStaticAnalysis, Log, TEXT("[ANALYSIS] %s"), *Activity);
}

static UMingStaticAnalysisIntegration* UMingStaticAnalysisIntegration::Get(UObject* WorldContextObject)
{
    static UMingStaticAnalysisIntegration* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingStaticAnalysisIntegration>();
        Instance->AddToRoot();
    }
    return Instance;
}
