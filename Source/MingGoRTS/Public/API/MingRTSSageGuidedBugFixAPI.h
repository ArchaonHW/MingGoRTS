// MingGoRTS 聖者指導BUG修復API
// 智慧化編譯錯誤修復系統

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSSageCommandAPI.h"
#include "MingRTSSageGuidedBugFixAPI.generated.h"

// ========== 聖者BUG修復枚舉 ==========

UENUM(BlueprintType)
enum class ESageBugFixType : uint8
{
    EOFError UMETA(DisplayName = "EOF錯誤"),
    SyntaxError UMETA(DisplayName = "語法錯誤"),
    ScopeError UMETA(DisplayName = "作用域錯誤"),
    IncludeError UMETA(DisplayName = "包含錯誤"),
    LinkError UMETA(DisplayName = "鏈接錯誤"),
    RuntimeError UMETA(DisplayName = "運行時錯誤"),
    LogicError UMETA(DisplayName = "邏輯錯誤"),
    PerformanceError UMETA(DisplayName = "性能錯誤"),
    AllErrors UMETA(DisplayName = "全部錯誤")
};

UENUM(BlueprintType)
enum class ESageFixStatus : uint8
{
    Pending UMETA(DisplayName = "待處理"),
    InProgress UMETA(DisplayName = "處理中"),
    Success UMETA(DisplayName = "成功"),
    Failed UMETA(DisplayName = "失敗"),
    RequiresManual UMETA(DisplayName = "需要人工"),
    Deferred UMETA(DisplayName = "延後處理")
};

UENUM(BlueprintType)
enum class ESageFixStrategy : uint8
{
    Tactical UMETA(DisplayName = "戰術修復"),
    Strategic UMETA(DisplayName = "戰略修復"),
    Philosophical UMETA(DisplayName = "哲學修復"),
    Innovative UMETA(DisplayName = "創新修復"),
    Predictive UMETA(DisplayName = "預測修復"),
    Intelligent UMETA(DisplayName = "智慧修復")
};

// ========== 聖者BUG修復數據結構 ==========

USTRUCT(BlueprintType)
struct FSageBugAnalysis
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> BugPatterns;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RootCauses;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> FixStrategies;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreventionMeasures;

    UPROPERTY(BlueprintReadOnly)
    FString WisdomInsight;

    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;

    UPROPERTY(BlueprintReadOnly)
    FDateTime AnalysisTime;

    FSageBugAnalysis()
    {
        ConfidenceLevel = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSageBugFixResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    ESageBugFixType FixType;

    UPROPERTY(BlueprintReadOnly)
    ESageFixStatus Status;

    UPROPERTY(BlueprintReadOnly)
    FString FixDescription;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    FString WisdomApplied;

    UPROPERTY(BlueprintReadOnly)
    FDateTime FixTime;

    UPROPERTY(BlueprintReadOnly)
    float FixConfidence;

    FSageBugFixResult()
    {
        FixType = ESageBugFixType::AllErrors;
        Status = ESageFixStatus::Pending;
        FixConfidence = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSageComprehensiveFixResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TArray<FSageBugFixResult> EOFFixResults;

    UPROPERTY(BlueprintReadOnly)
    TArray<FSageBugFixResult> SyntaxFixResults;

    UPROPERTY(BlueprintReadOnly)
    TArray<FSageBugFixResult> ScopeFixResults;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalFilesProcessed;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulFixes;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedFixes;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    FString WisdomSummary;

    UPROPERTY(BlueprintReadOnly)
    FString OverallInsight;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CompletionTime;

    FSageComprehensiveFixResult()
    {
        TotalFilesProcessed = 0;
        SuccessfulFixes = 0;
        FailedFixes = 0;
        SuccessRate = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSagePredictedBug
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    TArray<ESageBugFixType> PredictedBugTypes;

    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreventionSuggestions;

    UPROPERTY(BlueprintReadOnly)
    FString WisdomAdvice;

    UPROPERTY(BlueprintReadOnly)
    FDateTime PredictionTime;

    FSagePredictedBug()
    {
        ConfidenceLevel = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSageBugFixHistory
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString FilePath;

    UPROPERTY(BlueprintReadOnly)
    ESageBugFixType FixType;

    UPROPERTY(BlueprintReadOnly)
    FDateTime FixTime;

    UPROPERTY(BlueprintReadOnly)
    bool Success;

    UPROPERTY(BlueprintReadOnly)
    FString WisdomApplied;

    FSageBugFixHistory()
    {
        FixType = ESageBugFixType::AllErrors;
        Success = false;
    }
};

/**
 * 聖者指導BUG修復API - 智慧化編譯錯誤修復系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSageGuidedBugFixAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSageGuidedBugFixAPI();

    // ========== 聖者智慧BUG分析 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Analysis")
    FSageBugAnalysis AnalyzeBugWithWisdom(const FString& BugReport);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Analysis")
    TArray<FString> IdentifyBugPatterns(const FString& CodeContent);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Analysis")
    TArray<FString> AnalyzeRootCauses(const FString& BugContext);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Analysis")
    TArray<FString> GenerateFixStrategies(const TArray<FString>& BugPatterns);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Analysis")
    TArray<FString> GeneratePreventionMeasures(const TArray<FString>& RootCauses);

    // ========== 智慧化EOF錯誤修復 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|EOF Errors")
    TArray<FSageBugFixResult> FixEOFErrorsWithWisdom();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|EOF Errors")
    bool ApplyWisdomToEOFFile(const FString& FilePath, const FSageThoughtProcess& Wisdom);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|EOF Errors")
    TArray<FString> GetEOFErrorFiles();

    // ========== 智慧化語法錯誤修復 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Syntax Errors")
    TArray<FSageBugFixResult> FixSyntaxErrorsWithWisdom();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Syntax Errors")
    bool ApplyWisdomToSyntaxFile(const FString& FilePath, const FSageThoughtProcess& Wisdom);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Syntax Errors")
    TArray<FString> GetSyntaxErrorFiles();

    // ========== 智慧化作用域錯誤修復 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Scope Errors")
    TArray<FSageBugFixResult> FixScopeErrorsWithWisdom();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Scope Errors")
    bool ApplyWisdomToScopeFile(const FString& FilePath, const FSageThoughtProcess& Wisdom);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Scope Errors")
    TArray<FString> GetScopeErrorFiles();

    // ========== 綜合智慧修復 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Comprehensive")
    FSageComprehensiveFixResult PerformComprehensiveWisdomFix();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Comprehensive")
    void EnableAutoFix(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Comprehensive")
    void SetFixStrategy(ESageFixStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Comprehensive")
    ESageFixStrategy GetCurrentFixStrategy();

    // ========== 預測性BUG檢測 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Prediction")
    TArray<FSagePredictedBug> PredictFutureBugs();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Prediction")
    float CalculateBugPredictionConfidence(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Prediction")
    TArray<ESageBugFixType> PredictBugTypesForFile(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Prediction")
    TArray<FString> GeneratePreventionSuggestions(const FString& FilePath);

    // ========== 智慧報告生成 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Reporting")
    void GenerateWisdomFixReport(const FString& ReportPath);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Reporting")
    FString GenerateFixStatistics();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Reporting")
    FString GenerateDetailedFixResults();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Reporting")
    FString GenerateWisdomInsights();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|Reporting")
    FString GenerateFuturePredictions();

    // ========== 歷史和學習 ==========
    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|History")
    TArray<FSageBugFixHistory> GetFixHistory();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|History")
    void RecordBugFix(const FString& FilePath, ESageBugFixType FixType, const FSageBugFixResult& Result);

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|History")
    void LearnFromFixResults();

    UFUNCTION(BlueprintCallable, Category = "Sage Bug Fix|History")
    void UpdateWisdomFromHistory();

    // ========== 事件委託 ==========
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBugFixCompleted, const FSageBugFixResult&, FixResult);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComprehensiveFixCompleted, const FSageComprehensiveFixResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWisdomFixReportGenerated, const FString&, ReportPath);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBugPredicted, const FSagePredictedBug&, PredictedBug);

    UPROPERTY(BlueprintAssignable)
    FOnBugFixCompleted OnBugFixCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnComprehensiveFixCompleted OnComprehensiveFixCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnWisdomFixReportGenerated OnWisdomFixReportGenerated;

    UPROPERTY(BlueprintAssignable)
    FOnBugPredicted OnBugPredicted;

private:
    // ========== 聖者思考系統 ==========
    UPROPERTY()
    TObjectPtr<UMingRTSSageCommandAPI> SageCommandAPI;

    // ========== 修復配置 ==========
    UPROPERTY()
    bool bSageModeEnabled;

    UPROPERTY()
    bool bAutoFixEnabled;

    UPROPERTY()
    bool bVerboseLogging;

    UPROPERTY()
    ESageFixStrategy FixStrategy;

    // ========== BUG數據 ==========
    UPROPERTY()
    TMap<FString, FString> BugPatterns;

    UPROPERTY()
    TMap<ESageFixStrategy, FString> FixStrategies;

    UPROPERTY()
    TArray<FSageBugFixHistory> FixHistory;

    // ========== 內部方法 ==========
    void InitializeSageBugFixSystem();
    void LoadBugPatterns();
    void InitializeFixStrategies();
    
    FSageThoughtProcess GenerateSageThought(const FString& Context, ESageThinkingLevel Level);
    FSageThoughtProcess SynthesizeThoughts(const TArray<FSageThoughtProcess>& Thoughts);
    float CalculateConfidenceLevel(const FSageBugAnalysis& Analysis);
    
    TArray<FString> GetAllSourceFiles();
    FString GetFixStrategyName(ESageFixStrategy Strategy);
    
    // 統計方法
    int32 CountSuccessfulFixes(const FSageComprehensiveFixResult& Result);
    int32 CountFailedFixes(const FSageComprehensiveFixResult& Result);
    FString GenerateWisdomSummary(const FSageComprehensiveFixResult& Result, const FSageThoughtProcess& HolisticThought);
};
