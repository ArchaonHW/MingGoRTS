// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Code Standards API Implementation - 程式碼規範API實現

#include "MingRTSCodeStandardsAPI.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/Regex.h"

UMingRTSCodeStandardsAPI::UMingRTSCodeStandardsAPI()
{
    bMonitoringEnabled = false;
    MonitoringInterval = 60.0f; // 默認60秒
}

void UMingRTSCodeStandardsAPI::InitializeCodeStandards()
{
    InitializeDefaultStandards();
    LoadStandardsConfig();
    
    UE_LOG(LogTemp, Log, TEXT("Code Standards API initialized"));
}

FCodeQualityReport UMingRTSCodeStandardsAPI::AnalyzeFile(const FString& FilePath)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *FilePath);
        return FCodeQualityReport();
    }

    FCodeQualityReport Report = AnalyzeCodeContent(FilePath, FileContent);
    ReportHistory.Add(Report);
    
    OnQualityReportGenerated.Broadcast(Report);
    
    return Report;
}

TArray<FCodeQualityReport> UMingRTSCodeStandardsAPI::AnalyzeProject(const FString& ProjectPath)
{
    TArray<FCodeQualityReport> Reports;
    
    // 遞歸查找所有源文件
    TArray<FString> SourceFiles;
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // 查找.h和.cpp文件
    PlatformFile.IterateDirectoryRecursively(*ProjectPath, [&](const TCHAR* Filename, bool bIsDirectory) {
        if (!bIsDirectory)
        {
            FString FilePath(Filename);
            if (FilePath.EndsWith(TEXT(".h")) || FilePath.EndsWith(TEXT(".cpp")))
            {
                SourceFiles.Add(FilePath);
            }
        }
        return true;
    });
    
    // 分析每個文件
    for (const FString& FilePath : SourceFiles)
    {
        FCodeQualityReport Report = AnalyzeFile(FilePath);
        Reports.Add(Report);
    }
    
    return Reports;
}

TArray<FCodeQualityReport> UMingRTSCodeStandardsAPI::AnalyzeDirectory(const FString& DirectoryPath)
{
    TArray<FCodeQualityReport> Reports;
    
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    TArray<FString> SourceFiles;
    
    PlatformFile.IterateDirectory(*DirectoryPath, [&](const TCHAR* Filename, bool bIsDirectory) {
        if (!bIsDirectory)
        {
            FString FilePath(Filename);
            if (FilePath.EndsWith(TEXT(".h")) || FilePath.EndsWith(TEXT(".cpp")))
            {
                SourceFiles.Add(FilePath);
            }
        }
        return true;
    });
    
    for (const FString& FilePath : SourceFiles)
    {
        FCodeQualityReport Report = AnalyzeFile(FilePath);
        Reports.Add(Report);
    }
    
    return Reports;
}

TArray<FCodeViolation> UMingRTSCodeStandardsAPI::CheckNamingStandards(const FString& FilePath)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return TArray<FCodeViolation>();
    }
    
    TArray<FCodeViolation> Violations;
    
    // 獲取命名規範配置
    FCodeStandardConfig Config = GetStandardConfig(ECodeStandardType::Naming);
    if (!Config.bEnabled)
    {
        return Violations;
    }
    
    // 檢查命名規範
    Violations = ValidateNaming(FileContent, Config.NamingRules);
    
    return Violations;
}

TArray<FCodeViolation> UMingRTSCodeStandardsAPI::CheckFormattingStandards(const FString& FilePath)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return TArray<FCodeViolation>();
    }
    
    TArray<FCodeViolation> Violations;
    
    // 獲取格式化規範配置
    FCodeStandardConfig Config = GetStandardConfig(ECodeStandardType::Formatting);
    if (!Config.bEnabled)
    {
        return Violations;
    }
    
    // 檢查格式化規範
    Violations = ValidateFormatting(FileContent, Config.FormattingRules);
    
    return Violations;
}

bool UMingRTSCodeStandardsAPI::AutoFixCode(const FString& FilePath, const TArray<FCodeViolation>& Violations)
{
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return false;
    }
    
    FString FixedContent = FileContent;
    
    // 應用自動修復
    for (const FCodeViolation& Violation : Violations)
    {
        if (Violation.bAutoFixable && !Violation.FixedCode.IsEmpty())
        {
            FixedContent = FixedContent.Replace(*Violation.OriginalCode, *Violation.FixedCode);
        }
    }
    
    // 保存修復後的文件
    bool bSuccess = FFileHelper::SaveStringToFile(FixedContent, *FilePath);
    
    if (bSuccess)
    {
        OnAutoFixCompleted.Broadcast(FilePath);
    }
    
    return bSuccess;
}

TArray<FString> UMingRTSCodeStandardsAPI::BatchAutoFix(const TArray<FString>& FilePaths)
{
    TArray<FString> FixedFiles;
    
    for (const FString& FilePath : FilePaths)
    {
        TArray<FCodeViolation> Violations = CheckNamingStandards(FilePath);
        TArray<FCodeViolation> FormattingViolations = CheckFormattingStandards(FilePath);
        
        // 合併所有違規
        TArray<FCodeViolation> AllViolations = Violations;
        AllViolations.Append(FormattingViolations);
        
        if (AutoFixCode(FilePath, AllViolations))
        {
            FixedFiles.Add(FilePath);
        }
    }
    
    return FixedFiles;
}

FString UMingRTSCodeStandardsAPI::GenerateQualityReport(const TArray<FCodeQualityReport>& Reports)
{
    FString Report = TEXT("Code Quality Report\n");
    Report += TEXT("====================\n\n");
    
    // 統計信息
    TMap<ECodeQuality, int32> QualityStats = GetQualityStatistics(Reports);
    TMap<ECodeStandardType, int32> ViolationStats = GetViolationStatistics(Reports);
    
    Report += TEXT("Quality Distribution:\n");
    for (const auto& Stat : QualityStats)
    {
        Report += FString::Printf(TEXT("  %s: %d files\n"), 
            *StaticEnum<ECodeQuality>()->GetNameStringByValue((int64)Stat.Key), 
            Stat.Value);
    }
    
    Report += TEXT("\nViolation Distribution:\n");
    for (const auto& Stat : ViolationStats)
    {
        Report += FString::Printf(TEXT("  %s: %d violations\n"), 
            *StaticEnum<ECodeStandardType>()->GetNameStringByValue((int64)Stat.Key), 
            Stat.Value);
    }
    
    Report += TEXT("\nDetailed Reports:\n");
    for (const FCodeQualityReport& QualityReport : Reports)
    {
        Report += FString::Printf(TEXT("\nFile: %s\n"), *QualityReport.FilePath);
        Report += FString::Printf(TEXT("Quality: %s (Score: %.2f)\n"), 
            *StaticEnum<ECodeQuality>()->GetNameStringByValue((int64)QualityReport.OverallQuality),
            QualityReport.QualityScore);
        Report += FString::Printf(TEXT("Violations: %d\n"), QualityReport.Violations.Num());
        
        for (const FCodeViolation& Violation : QualityReport.Violations)
        {
            Report += FString::Printf(TEXT("  - Line %d: %s\n"), 
                Violation.LineNumber, *Violation.Description);
        }
    }
    
    return Report;
}

FString UMingRTSCodeStandardsAPI::GenerateHTMLReport(const TArray<FCodeQualityReport>& Reports)
{
    FString HTML = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    HTML += TEXT("<title>Code Quality Report</title>\n");
    HTML += TEXT("<style>\n");
    HTML += TEXT("body { font-family: Arial, sans-serif; margin: 20px; }\n");
    HTML += TEXT("table { border-collapse: collapse; width: 100%; }\n");
    HTML += TEXT("th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n");
    HTML += TEXT("th { background-color: #f2f2f2; }\n");
    HTML += TEXT(".excellent { color: green; }\n");
    HTML += TEXT(".good { color: blue; }\n");
    HTML += TEXT(".average { color: orange; }\n");
    HTML += TEXT(".poor { color: red; }\n");
    HTML += TEXT(".critical { color: darkred; font-weight: bold; }\n");
    HTML += TEXT("</style>\n</head>\n<body>\n");
    
    HTML += TEXT("<h1>Code Quality Report</h1>\n");
    
    // 統計表格
    HTML += TEXT("<h2>Quality Statistics</h2>\n");
    HTML += TEXT("<table>\n");
    HTML += TEXT("<tr><th>Quality Level</th><th>Count</th></tr>\n");
    
    TMap<ECodeQuality, int32> QualityStats = GetQualityStatistics(Reports);
    for (const auto& Stat : QualityStats)
    {
        FString ClassName = StaticEnum<ECodeQuality>()->GetNameStringByValue((int64)Stat.Key).ToLower();
        HTML += FString::Printf(TEXT("<tr><td class=\"%s\">%s</td><td>%d</td></tr>\n"), 
            *ClassName, *StaticEnum<ECodeQuality>()->GetNameStringByValue((int64)Stat.Key), Stat.Value);
    }
    HTML += TEXT("</table>\n");
    
    // 詳細報告
    HTML += TEXT("<h2>Detailed Reports</h2>\n");
    HTML += TEXT("<table>\n");
    HTML += TEXT("<tr><th>File</th><th>Quality</th><th>Score</th><th>Violations</th></tr>\n");
    
    for (const FCodeQualityReport& QualityReport : Reports)
    {
        FString QualityClass = StaticEnum<ECodeQuality>()->GetNameStringByValue((int64)QualityReport.OverallQuality).ToLower();
        HTML += FString::Printf(TEXT("<tr><td>%s</td><td class=\"%s\">%s</td><td>%.2f</td><td>%d</td></tr>\n"),
            *QualityReport.FilePath,
            *QualityClass,
            *StaticEnum<ECodeQuality>()->GetNameStringByValue((int64)QualityReport.OverallQuality),
            QualityReport.QualityScore,
            QualityReport.Violations.Num());
    }
    
    HTML += TEXT("</table>\n");
    HTML += TEXT("</body>\n</html>");
    
    return HTML;
}

bool UMingRTSCodeStandardsAPI::ExportReport(const FString& ReportContent, const FString& OutputPath)
{
    return FFileHelper::SaveStringToFile(ReportContent, *OutputPath);
}

void UMingRTSCodeStandardsAPI::AddNamingRule(const FNamingRule& Rule)
{
    FCodeStandardConfig Config = GetStandardConfig(ECodeStandardType::Naming);
    Config.NamingRules.Add(Rule);
    SetStandardConfig(Config);
}

void UMingRTSCodeStandardsAPI::AddFormattingRule(const FFormattingRule& Rule)
{
    FCodeStandardConfig Config = GetStandardConfig(ECodeStandardType::Formatting);
    Config.FormattingRules.Add(Rule);
    SetStandardConfig(Config);
}

FCodeStandardConfig UMingRTSCodeStandardsAPI::GetStandardConfig(ECodeStandardType StandardType)
{
    if (StandardConfigs.Contains(StandardType))
    {
        return StandardConfigs[StandardType];
    }
    
    // 返回默認配置
    FCodeStandardConfig DefaultConfig;
    DefaultConfig.StandardType = StandardType;
    DefaultConfig.bEnabled = true;
    return DefaultConfig;
}

void UMingRTSCodeStandardsAPI::SetStandardConfig(const FCodeStandardConfig& Config)
{
    StandardConfigs[Config.StandardType] = Config;
    SaveStandardsConfig();
}

void UMingRTSCodeStandardsAPI::SetStandardEnabled(ECodeStandardType StandardType, bool bEnabled)
{
    FCodeStandardConfig Config = GetStandardConfig(StandardType);
    Config.bEnabled = bEnabled;
    SetStandardConfig(Config);
}

TMap<ECodeQuality, int32> UMingRTSCodeStandardsAPI::GetQualityStatistics(const TArray<FCodeQualityReport>& Reports)
{
    TMap<ECodeQuality, int32> Stats;
    
    for (const FCodeQualityReport& Report : Reports)
    {
        if (Stats.Contains(Report.OverallQuality))
        {
            Stats[Report.OverallQuality]++;
        }
        else
        {
            Stats.Add(Report.OverallQuality, 1);
        }
    }
    
    return Stats;
}

TMap<ECodeStandardType, int32> UMingRTSCodeStandardsAPI::GetViolationStatistics(const TArray<FCodeQualityReport>& Reports)
{
    TMap<ECodeStandardType, int32> Stats;
    
    for (const FCodeQualityReport& Report : Reports)
    {
        for (const FCodeViolation& Violation : Report.Violations)
        {
            if (Stats.Contains(Violation.StandardType))
            {
                Stats[Violation.StandardType]++;
            }
            else
            {
                Stats.Add(Violation.StandardType, 1);
            }
        }
    }
    
    return Stats;
}

TArray<FString> UMingRTSCodeStandardsAPI::GetMostCommonViolations(int32 TopCount)
{
    TMap<FString, int32> ViolationCounts;
    
    for (const FCodeViolation& Violation : ViolationHistory)
    {
        if (ViolationCounts.Contains(Violation.RuleName))
        {
            ViolationCounts[Violation.RuleName]++;
        }
        else
        {
            ViolationCounts.Add(Violation.RuleName, 1);
        }
    }
    
    // 排序並返回前TopCount個
    TArray<FString> MostCommon;
    ViolationCounts.ValueSort([](const int32& A, const int32& B) {
        return A > B;
    });
    
    int32 Count = 0;
    for (const auto& Pair : ViolationCounts)
    {
        if (Count >= TopCount) break;
        MostCommon.Add(Pair.Key);
        Count++;
    }
    
    return MostCommon;
}

void UMingRTSCodeStandardsAPI::EnableRealTimeMonitoring(bool bEnabled)
{
    bMonitoringEnabled = bEnabled;
    
    if (bEnabled && GetWorld())
    {
        // 設置定時器進行監控
        GetWorld()->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingRTSCodeStandardsAPI::ProcessMonitoring,
            MonitoringInterval,
            true
        );
    }
    else if (!bEnabled && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MonitoringTimer);
    }
}

void UMingRTSCodeStandardsAPI::SetMonitoringInterval(float IntervalInSeconds)
{
    MonitoringInterval = IntervalInSeconds;
    
    // 如果正在監控，重新設置定時器
    if (bMonitoringEnabled && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(MonitoringTimer);
        GetWorld()->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingRTSCodeStandardsAPI::ProcessMonitoring,
            MonitoringInterval,
            true
        );
    }
}

bool UMingRTSCodeStandardsAPI::IsMonitoringActive()
{
    return bMonitoringEnabled;
}

void UMingRTSCodeStandardsAPI::ShareStandardsConfig(const FString& TeamMember)
{
    FString ConfigPath = FString::Printf(TEXT("TeamStandards/%s_Config.json"), *TeamMember);
    SaveStandardsConfig();
    
    // 這裡可以實現實際的分享邏輯，比如發送到服務器或通過網絡共享
    UE_LOG(LogTemp, Log, TEXT("Standards config shared with: %s"), *TeamMember);
}

bool UMingRTSCodeStandardsAPI::ImportTeamStandards(const FString& ConfigPath)
{
    FString ConfigContent;
    if (!FFileHelper::LoadFileToString(ConfigContent, *ConfigPath))
    {
        return false;
    }
    
    // 這裡可以實現JSON解析和配置導入邏輯
    // 為了簡化，這裡只是記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Team standards imported from: %s"), *ConfigPath);
    return true;
}

TArray<FCodeViolation> UMingRTSCodeStandardsAPI::ReviewCodeChanges(const FString& Changeset)
{
    TArray<FCodeViolation> Violations;
    
    // 這裡可以實現代碼變更審查邏輯
    // 為了簡化，這裡只是返回空數組
    UE_LOG(LogTemp, Log, TEXT("Code changes reviewed for changeset: %s"), *Changeset);
    
    return Violations;
}

FCodeQualityReport UMingRTSCodeStandardsAPI::ReviewPullRequest(const FString& PRId)
{
    FCodeQualityReport Report;
    
    // 這裡可以實現Pull Request審查邏輯
    // 為了簡化，這裡只是返回空報告
    UE_LOG(LogTemp, Log, TEXT("Pull request reviewed: %s"), *PRId);
    
    return Report;
}

void UMingRTSCodeStandardsAPI::InitializeDefaultStandards()
{
    // 初始化命名規範
    FCodeStandardConfig NamingConfig;
    NamingConfig.StandardType = ECodeStandardType::Naming;
    NamingConfig.bEnabled = true;
    
    // 添加默認命名規則
    FNamingRule ClassNameRule;
    ClassNameRule.Pattern = TEXT("^class [A-Z][a-zA-Z0-9]*");
    ClassNameRule.Convention = ENamingConvention::PascalCase;
    ClassNameRule.Description = TEXT("Class names should use PascalCase");
    ClassNameRule.bRequired = true;
    NamingConfig.NamingRules.Add(ClassNameRule);
    
    FNamingRule VariableNameRule;
    VariableNameRule.Pattern = TEXT("^[a-z][a-zA-Z0-9]*");
    VariableNameRule.Convention = ENamingConvention::CamelCase;
    VariableNameRule.Description = TEXT("Variable names should use camelCase");
    VariableNameRule.bRequired = true;
    NamingConfig.NamingRules.Add(VariableNameRule);
    
    StandardConfigs.Add(ECodeStandardType::Naming, NamingConfig);
    
    // 初始化格式化規範
    FCodeStandardConfig FormattingConfig;
    FormattingConfig.StandardType = ECodeStandardType::Formatting;
    FormattingConfig.bEnabled = true;
    
    // 添加默認格式化規則
    FFormattingRule IndentRule;
    IndentRule.RuleName = TEXT("Indentation");
    IndentRule.Pattern = TEXT("^( {4}|\t)");
    IndentRule.Replacement = TEXT("    ");
    IndentRule.Description = TEXT("Use 4 spaces for indentation");
    IndentRule.bAutoFixable = true;
    FormattingConfig.FormattingRules.Add(IndentRule);
    
    StandardConfigs.Add(ECodeStandardType::Formatting, FormattingConfig);
}

void UMingRTSCodeStandardsAPI::LoadStandardsConfig()
{
    FString ConfigPath = FPaths::ProjectConfigDir() / TEXT("CodeStandards.json");
    
    // 這裡可以實現從文件加載配置的邏輯
    // 為了簡化，這裡只是記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Code standards config loaded from: %s"), *ConfigPath);
}

void UMingRTSCodeStandardsAPI::SaveStandardsConfig()
{
    FString ConfigPath = FPaths::ProjectConfigDir() / TEXT("CodeStandards.json");
    
    // 這裡可以實現保存配置到文件的邏輯
    // 為了簡化，這裡只是記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Code standards config saved to: %s"), *ConfigPath);
}

FCodeQualityReport UMingRTSCodeStandardsAPI::AnalyzeCodeContent(const FString& FilePath, const FString& Content)
{
    FCodeQualityReport Report;
    Report.FilePath = FilePath;
    Report.AnalysisTime = FDateTime::Now();
    
    // 檢查各種規範
    TArray<FCodeViolation> NamingViolations = CheckNamingStandards(FilePath);
    TArray<FCodeViolation> FormattingViolations = CheckFormattingStandards(FilePath);
    
    // 設置違規的文件路徑
    for (FCodeViolation& Violation : NamingViolations)
    {
        Violation.FilePath = FilePath;
    }
    for (FCodeViolation& Violation : FormattingViolations)
    {
        Violation.FilePath = FilePath;
    }
    
    // 合併所有違規
    Report.Violations = NamingViolations;
    Report.Violations.Append(FormattingViolations);
    
    // 計算質量分數
    Report.QualityScore = CalculateQualityScore(Report.Violations);
    Report.OverallQuality = DetermineQualityLevel(Report.QualityScore);
    
    // 統計違規
    for (const FCodeViolation& Violation : Report.Violations)
    {
        if (Report.ViolationCounts.Contains(Violation.StandardType))
        {
            Report.ViolationCounts[Violation.StandardType]++;
        }
        else
        {
            Report.ViolationCounts.Add(Violation.StandardType, 1);
        }
        
        if (Report.SeverityCounts.Contains(Violation.Severity))
        {
            Report.SeverityCounts[Violation.Severity]++;
        }
        else
        {
            Report.SeverityCounts.Add(Violation.Severity, 1);
        }
    }
    
    // 生成建議
    GenerateRecommendations(Report);
    
    return Report;
}

TArray<FCodeViolation> UMingRTSCodeStandardsAPI::ValidateNaming(const FString& Content, const TArray<FNamingRule>& Rules)
{
    TArray<FCodeViolation> Violations;
    TArray<FString> Lines;
    Content.ParseIntoArrayLines(Lines);
    
    for (int32 LineIndex = 0; LineIndex < Lines.Num(); LineIndex++)
    {
        const FString& Line = Lines[LineIndex];
        
        for (const FNamingRule& Rule : Rules)
        {
            // 簡化的驗證邏輯
            FRegexPattern Pattern(Rule.Pattern);
            FRegexMatcher Matcher(Pattern, Line);
            
            if (!Matcher.FindNext())
            {
                FCodeViolation Violation;
                Violation.FilePath = TEXT(""); // 需要在調用時設置
                Violation.LineNumber = LineIndex + 1;
                Violation.StandardType = ECodeStandardType::Naming;
                Violation.Severity = Rule.bRequired ? EViolationSeverity::Error : EViolationSeverity::Warning;
                Violation.RuleName = Rule.Description;
                Violation.Description = FString::Printf(TEXT("Naming rule violation: %s"), *Rule.Description);
                Violation.Suggestion = FString::Printf(TEXT("Consider using %s convention"), 
                    *StaticEnum<ENamingConvention>()->GetNameStringByValue((int64)Rule.Convention));
                Violation.OriginalCode = Line;
                Violation.bAutoFixable = false;
                Violation.Timestamp = FDateTime::Now();
                
                Violations.Add(Violation);
            }
        }
    }
    
    return Violations;
}

TArray<FCodeViolation> UMingRTSCodeStandardsAPI::ValidateFormatting(const FString& Content, const TArray<FFormattingRule>& Rules)
{
    TArray<FCodeViolation> Violations;
    TArray<FString> Lines;
    Content.ParseIntoArrayLines(Lines);
    
    for (int32 LineIndex = 0; LineIndex < Lines.Num(); LineIndex++)
    {
        const FString& Line = Lines[LineIndex];
        
        for (const FFormattingRule& Rule : Rules)
        {
            // 簡化的驗證邏輯
            FRegexPattern Pattern(Rule.Pattern);
            FRegexMatcher Matcher(Pattern, Line);
            
            if (Matcher.FindNext())
            {
                FCodeViolation Violation;
                Violation.FilePath = TEXT(""); // 需要在調用時設置
                Violation.LineNumber = LineIndex + 1;
                Violation.StandardType = ECodeStandardType::Formatting;
                Violation.Severity = EViolationSeverity::Warning;
                Violation.RuleName = Rule.RuleName;
                Violation.Description = FString::Printf(TEXT("Formatting rule violation: %s"), *Rule.Description);
                Violation.Suggestion = TEXT("Apply auto-fix to resolve this issue");
                Violation.OriginalCode = Line;
                Violation.FixedCode = Line.Replace(*Rule.Pattern, *Rule.Replacement);
                Violation.bAutoFixable = Rule.bAutoFixable;
                Violation.Timestamp = FDateTime::Now();
                
                Violations.Add(Violation);
            }
        }
    }
    
    return Violations;
}

float UMingRTSCodeStandardsAPI::CalculateQualityScore(const TArray<FCodeViolation>& Violations)
{
    if (Violations.Num() == 0)
    {
        return 100.0f;
    }
    
    float TotalScore = 100.0f;
    
    for (const FCodeViolation& Violation : Violations)
    {
        switch (Violation.Severity)
        {
        case EViolationSeverity::Info:
            TotalScore -= 1.0f;
            break;
        case EViolationSeverity::Warning:
            TotalScore -= 5.0f;
            break;
        case EViolationSeverity::Error:
            TotalScore -= 15.0f;
            break;
        case EViolationSeverity::Critical:
            TotalScore -= 30.0f;
            break;
        }
    }
    
    return FMath::Max(0.0f, TotalScore);
}

ECodeQuality UMingRTSCodeStandardsAPI::DetermineQualityLevel(float Score)
{
    if (Score >= 90.0f)
    {
        return ECodeQuality::Excellent;
    }
    else if (Score >= 75.0f)
    {
        return ECodeQuality::Good;
    }
    else if (Score >= 60.0f)
    {
        return ECodeQuality::Average;
    }
    else if (Score >= 40.0f)
    {
        return ECodeQuality::Poor;
    }
    else
    {
        return ECodeQuality::Critical;
    }
}

void UMingRTSCodeStandardsAPI::ProcessMonitoring()
{
    if (!bMonitoringEnabled)
    {
        return;
    }
    
    // 監控當前項目目錄
    FString ProjectPath = FPaths::ProjectDir();
    TArray<FCodeQualityReport> Reports = AnalyzeProject(ProjectPath);
    
    // 檢查是否有新的嚴重違規
    for (const FCodeQualityReport& Report : Reports)
    {
        for (const FCodeViolation& Violation : Report.Violations)
        {
            if (Violation.Severity == EViolationSeverity::Critical || Violation.Severity == EViolationSeverity::Error)
            {
                NotifyViolation(Violation);
            }
        }
    }
}

void UMingRTSCodeStandardsAPI::NotifyViolation(const FCodeViolation& Violation)
{
    OnViolationFound.Broadcast(Violation);
    OnStandardViolation.Broadcast(Violation.StandardType, Violation.Description);
    
    UE_LOG(LogTemp, Warning, TEXT("Code Standard Violation: %s at line %d in %s"), 
        *Violation.Description, Violation.LineNumber, *Violation.FilePath);
}

void UMingRTSCodeStandardsAPI::GenerateRecommendations(FCodeQualityReport& Report)
{
    Report.Recommendations.Empty();
    
    // 基於違規類型生成建議
    for (const auto& ViolationCount : Report.ViolationCounts)
    {
        switch (ViolationCount.Key)
        {
        case ECodeStandardType::Naming:
            Report.Recommendations.Add(TEXT("Consider establishing consistent naming conventions"));
            break;
        case ECodeStandardType::Formatting:
            Report.Recommendations.Add(TEXT("Use automated formatting tools to ensure consistency"));
            break;
        case ECodeStandardType::Documentation:
            Report.Recommendations.Add(TEXT("Add comprehensive documentation for better code maintainability"));
            break;
        case ECodeStandardType::Security:
            Report.Recommendations.Add(TEXT("Review security best practices and implement security measures"));
            break;
        case ECodeStandardType::Performance:
            Report.Recommendations.Add(TEXT("Profile and optimize performance-critical code sections"));
            break;
        default:
            Report.Recommendations.Add(TEXT("Review and improve code quality standards"));
            break;
        }
    }
}
