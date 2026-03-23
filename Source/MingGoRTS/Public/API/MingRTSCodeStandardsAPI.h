// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Code Standards API - 程式碼規範API

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSCodeStandardsAPI.generated.h"

UENUM(BlueprintType)
enum class ECodeStandardType : uint8
{
    Naming,             // 命名規範
    Formatting,         // 格式化規範
    Documentation,      // 文檔規範
    Architecture,       // 架構規範
    Security,           // 安全規範
    Performance,        // 性能規範
    Testing,            // 測試規範
    VersionControl,     // 版本控制規範
    Deployment,         // 部署規範
    Custom              // 自定義規範
};

UENUM(BlueprintType)
enum class ENamingConvention : uint8
{
    CamelCase,          // 駝峰命名法
    PascalCase,         // 帕斯卡命名法
    SnakeCase,          // 下劃線命名法
    KebabCase,          // 短橫線命名法
    Hungarian,          // 匈牙利命名法
    UpperCase,          // 全大寫
    LowerCase,          // 全小寫
    Custom              // 自定義
};

UENUM(BlueprintType)
enum class ECodeQuality : uint8
{
    Excellent,          // 優秀
    Good,               // 良好
    Average,            // 一般
    Poor,               // 較差
    Critical            // 嚴重
};

UENUM(BlueprintType)
enum class EViolationSeverity : uint8
{
    Info,               // 信息
    Warning,            // 警告
    Error,              // 錯誤
    Critical            // 嚴重錯誤
};

USTRUCT(BlueprintType)
struct FNamingRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming Rule")
    FString Pattern;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming Rule")
    ENamingConvention Convention;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming Rule")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming Rule")
    TArray<FString> Examples;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Naming Rule")
    bool bRequired;

    FNamingRule()
    {
        Pattern = TEXT("");
        Convention = ENamingConvention::CamelCase;
        Description = TEXT("");
        Examples.Empty();
        bRequired = true;
    }
};

USTRUCT(BlueprintType)
struct FFormattingRule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting Rule")
    FString RuleName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting Rule")
    FString Pattern;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting Rule")
    FString Replacement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting Rule")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Formatting Rule")
    bool bAutoFixable;

    FFormattingRule()
    {
        RuleName = TEXT("");
        Pattern = TEXT("");
        Replacement = TEXT("");
        Description = TEXT("");
        bAutoFixable = true;
    }
};

USTRUCT(BlueprintType)
struct FCodeViolation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    int32 LineNumber;

    UPROPERTY(BlueprintReadOnly)
    int32 ColumnNumber;

    UPROPERTY(BlueprintReadOnly)
    ECodeStandardType StandardType;

    UPROPERTY(BlueprintReadOnly)
    EViolationSeverity Severity;

    UPROPERTY(BlueprintReadOnly)
    FString RuleName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FString Suggestion;

    UPROPERTY(BlueprintReadOnly)
    FString OriginalCode;

    UPROPERTY(BlueprintReadOnly)
    FString FixedCode;

    UPROPERTY(BlueprintReadOnly)
    bool bAutoFixable;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    FCodeViolation()
    {
        FilePath = TEXT("");
        LineNumber = 0;
        ColumnNumber = 0;
        StandardType = ECodeStandardType::Naming;
        Severity = EViolationSeverity::Warning;
        RuleName = TEXT("");
        Description = TEXT("");
        Suggestion = TEXT("");
        OriginalCode = TEXT("");
        FixedCode = TEXT("");
        bAutoFixable = false;
        Timestamp = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FCodeQualityReport
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    ECodeQuality OverallQuality;

    UPROPERTY(BlueprintReadOnly)
    float QualityScore;

    UPROPERTY(BlueprintReadOnly)
    TArray<FCodeViolation> Violations;

    UPROPERTY(BlueprintReadOnly)
    TMap<ECodeStandardType, int32> ViolationCounts;

    UPROPERTY(BlueprintReadOnly)
    TMap<EViolationSeverity, int32> SeverityCounts;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Recommendations;

    UPROPERTY(BlueprintReadOnly)
    FDateTime AnalysisTime;

    FCodeQualityReport()
    {
        FilePath = TEXT("");
        OverallQuality = ECodeQuality::Average;
        QualityScore = 0.0f;
        Violations.Empty();
        ViolationCounts.Empty();
        SeverityCounts.Empty();
        Recommendations.Empty();
        AnalysisTime = FDateTime::Now();
    }
};

USTRUCT(BlueprintType)
struct FCodeStandardConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Standard Config")
    ECodeStandardType StandardType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Standard Config")
    bool bEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Standard Config")
    TArray<FString> FilePatterns;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Standard Config")
    TArray<FNamingRule> NamingRules;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Standard Config")
    TArray<FFormattingRule> FormattingRules;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Standard Config")
    TMap<FString, FString> CustomRules;

    FCodeStandardConfig()
    {
        StandardType = ECodeStandardType::Naming;
        bEnabled = true;
        FilePatterns.Empty();
        NamingRules.Empty();
        FormattingRules.Empty();
        CustomRules.Empty();
    }
};

/**
 * 程式碼規範API
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSCodeStandardsAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCodeStandardsAPI();

    /// 初始化程式碼規範API
    UFUNCTION(BlueprintCallable, Category = "Code Standards")
    void InitializeCodeStandards();

    /// 分析單個文件
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Analysis")
    FCodeQualityReport AnalyzeFile(const FString& FilePath);

    /// 分析整個項目
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Analysis")
    TArray<FCodeQualityReport> AnalyzeProject(const FString& ProjectPath);

    /// 分析特定目錄
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Analysis")
    TArray<FCodeQualityReport> AnalyzeDirectory(const FString& DirectoryPath);

    /// 檢查命名規範
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Naming")
    TArray<FCodeViolation> CheckNamingStandards(const FString& FilePath);

    /// 檢查格式化規範
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Formatting")
    TArray<FCodeViolation> CheckFormattingStandards(const FString& FilePath);

    /// 自動修復代碼
    UFUNCTION(BlueprintCallable, Category = "Code Standards|AutoFix")
    bool AutoFixCode(const FString& FilePath, const TArray<FCodeViolation>& Violations);

    /// 批量自動修復
    UFUNCTION(BlueprintCallable, Category = "Code Standards|AutoFix")
    TArray<FString> BatchAutoFix(const TArray<FString>& FilePaths);

    /// 生成規範報告
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Reporting")
    FString GenerateQualityReport(const TArray<FCodeQualityReport>& Reports);

    /// 生成HTML報告
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Reporting")
    FString GenerateHTMLReport(const TArray<FCodeQualityReport>& Reports);

    /// 導出報告到文件
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Reporting")
    bool ExportReport(const FString& ReportContent, const FString& OutputPath);

    /// 添加命名規則
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Configuration")
    void AddNamingRule(const FNamingRule& Rule);

    /// 添加格式化規則
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Configuration")
    void AddFormattingRule(const FFormattingRule& Rule);

    /// 獲取規範配置
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Configuration")
    FCodeStandardConfig GetStandardConfig(ECodeStandardType StandardType);

    /// 設置規範配置
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Configuration")
    void SetStandardConfig(const FCodeStandardConfig& Config);

    /// 啟用/禁用規範
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Configuration")
    void SetStandardEnabled(ECodeStandardType StandardType, bool bEnabled);

    /// 獲取代碼質量統計
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Statistics")
    TMap<ECodeQuality, int32> GetQualityStatistics(const TArray<FCodeQualityReport>& Reports);

    /// 獲取違規統計
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Statistics")
    TMap<ECodeStandardType, int32> GetViolationStatistics(const TArray<FCodeQualityReport>& Reports);

    /// 獲取最常見違規
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Statistics")
    TArray<FString> GetMostCommonViolations(int32 TopCount);

    /// 實時監控
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Monitoring")
    void EnableRealTimeMonitoring(bool bEnabled);

    /// 設置監控間隔
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Monitoring")
    void SetMonitoringInterval(float IntervalInSeconds);

    /// 獲取監控狀態
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Monitoring")
    bool IsMonitoringActive();

    /// 團隊協作
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Team")
    void ShareStandardsConfig(const FString& TeamMember);

    /// 導入團隊規範
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Team")
    bool ImportTeamStandards(const FString& ConfigPath);

    /// 代碼審查
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Review")
    TArray<FCodeViolation> ReviewCodeChanges(const FString& Changeset);

    /// 審查Pull Request
    UFUNCTION(BlueprintCallable, Category = "Code Standards|Review")
    FCodeQualityReport ReviewPullRequest(const FString& PRId);

    /// 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnViolationFound, const FCodeViolation&, Violation);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQualityReportGenerated, const FCodeQualityReport&, Report);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAutoFixCompleted, const FString&, FilePath);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStandardViolation, ECodeStandardType, StandardType, const FString&, Message);

    UPROPERTY(BlueprintAssignable)
    FOnViolationFound OnViolationFound;

    UPROPERTY(BlueprintAssignable)
    FOnQualityReportGenerated OnQualityReportGenerated;

    UPROPERTY(BlueprintAssignable)
    FOnAutoFixCompleted OnAutoFixCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnStandardViolation OnStandardViolation;

private:
    /// 規範配置
    UPROPERTY()
    TMap<ECodeStandardType, FCodeStandardConfig> StandardConfigs;

    /// 違規歷史
    UPROPERTY()
    TArray<FCodeViolation> ViolationHistory;

    /// 質量報告歷史
    UPROPERTY()
    TArray<FCodeQualityReport> ReportHistory;

    /// 監控狀態
    UPROPERTY()
    bool bMonitoringEnabled;

    UPROPERTY()
    float MonitoringInterval;

    /// 監控定時器
    FTimerHandle MonitoringTimer;

    /// 內部方法
    void InitializeDefaultStandards();
    void LoadStandardsConfig();
    void SaveStandardsConfig();
    FCodeQualityReport AnalyzeCodeContent(const FString& FilePath, const FString& Content);
    TArray<FCodeViolation> ValidateNaming(const FString& Content, const TArray<FNamingRule>& Rules);
    TArray<FCodeViolation> ValidateFormatting(const FString& Content, const TArray<FFormattingRule>& Rules);
    float CalculateQualityScore(const TArray<FCodeViolation>& Violations);
    ECodeQuality DetermineQualityLevel(float Score);
    void ProcessMonitoring();
    void NotifyViolation(const FCodeViolation& Violation);
    void GenerateRecommendations(FCodeQualityReport& Report);
};
