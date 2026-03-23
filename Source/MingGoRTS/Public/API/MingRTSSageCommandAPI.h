#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAPISystem.h"
#include "MingRTSSageCommandAPI.generated.h"

UENUM(BlueprintType)
enum class ESageThinkingLevel : uint8
{
    Tactical UMETA(DisplayName = "戰術思考"),
    Strategic UMETA(DisplayName = "戰略思考"),
    Philosophical UMETA(DisplayName = "哲學思考"),
    Historical UMETA(DisplayName = "歷史思考"),
    Cultural UMETA(DisplayName = "文化思考"),
    Innovative UMETA(DisplayName = "創新思考"),
    Predictive UMETA(DisplayName = "預測思考"),
    Holistic UMETA(DisplayName = "整體思考")
};

UENUM(BlueprintType)
enum class ESageCommandType : uint8
{
    Analysis UMETA(DisplayName = "分析指令"),
    Strategy UMETA(DisplayName = "戰略指令"),
    Optimization UMETA(DisplayName = "優化指令"),
    Innovation UMETA(DisplayName = "創新指令"),
    Coordination UMETA(DisplayName = "協調指令"),
    Learning UMETA(DisplayName = "學習指令"),
    Prediction UMETA(DisplayName = "預測指令"),
    Synthesis UMETA(DisplayName = "綜合指令")
};

USTRUCT(BlueprintType)
struct FSageThoughtProcess
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ThoughtId;

    UPROPERTY(BlueprintReadOnly)
    ESageThinkingLevel ThinkingLevel;

    UPROPERTY(BlueprintReadOnly)
    FString ThoughtContent;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ReasoningSteps;

    UPROPERTY(BlueprintReadOnly)
    FString Conclusion;

    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedContexts;

    FSageThoughtProcess()
    {
        ThinkingLevel = ESageThinkingLevel::Tactical;
        ConfidenceLevel = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSageCommand
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CommandId;

    UPROPERTY(BlueprintReadOnly)
    ESageCommandType CommandType;

    UPROPERTY(BlueprintReadOnly)
    FString CommandContent;

    UPROPERTY(BlueprintReadOnly)
    ESageThinkingLevel RequiredThinkingLevel;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Parameters;

    UPROPERTY(BlueprintReadOnly)
    FString ExpectedOutcome;

    UPROPERTY(BlueprintReadOnly)
    int32 Priority;

    UPROPERTY(BlueprintReadOnly)
    bool bExecuted;

    FSageCommand()
    {
        CommandType = ESageCommandType::Analysis;
        RequiredThinkingLevel = ESageThinkingLevel::Tactical;
        Priority = 1;
        bExecuted = false;
    }
};

USTRUCT(BlueprintType)
struct FSageInsight
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString InsightId;

    UPROPERTY(BlueprintReadOnly)
    FString InsightTitle;

    UPROPERTY(BlueprintReadOnly)
    FString InsightDescription;

    UPROPERTY(BlueprintReadOnly)
    ESageThinkingLevel SourceThinkingLevel;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SupportingEvidence;

    UPROPERTY(BlueprintReadOnly)
    float InnovationScore;

    UPROPERTY(BlueprintReadOnly)
    float PracticalityScore;

    UPROPERTY(BlueprintReadOnly)
    FString RecommendedAction;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedCommands;

    FSageInsight()
    {
        SourceThinkingLevel = ESageThinkingLevel::Tactical;
        InnovationScore = 0.0f;
        PracticalityScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FSageWisdom
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString WisdomId;

    UPROPERTY(BlueprintReadOnly)
    FString WisdomTitle;

    UPROPERTY(BlueprintReadOnly)
    FString WisdomContent;

    UPROPERTY(BlueprintReadOnly)
    ESageThinkingLevel PrimaryThinkingLevel;

    UPROPERTY(BlueprintReadOnly)
    TArray<ESageThinkingLevel> SecondaryLevels;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> HistoricalReferences;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CulturalContexts;

    UPROPERTY(BlueprintReadOnly)
    float WisdomDepth;

    UPROPERTY(BlueprintReadOnly)
    float ApplicabilityScore;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PracticalApplications;

    FSageWisdom()
    {
        PrimaryThinkingLevel = ESageThinkingLevel::Philosophical;
        WisdomDepth = 0.0f;
        ApplicabilityScore = 0.0f;
    }
};

/**
 * 聖者指揮學核心智慧思考API - 具備多層次智慧思考能力的指揮系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSageCommandAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSageCommandAPI();

    // 核心思考功能
    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Thinking")
    FSageThoughtProcess GenerateThought(const FString& Topic, ESageThinkingLevel Level);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Thinking")
    TArray<FSageThoughtProcess> GenerateThinkingChain(const FString& InitialTopic, int32 Depth);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Thinking")
    FSageThoughtProcess SynthesizeThoughts(const TArray<FSageThoughtProcess>& Thoughts);

    // 指揮決策功能
    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Command")
    FSageCommand GenerateCommand(const FString& Situation, ESageCommandType Type);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Command")
    TArray<FSageCommand> GenerateCommandSequence(const FString& Objective);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Command")
    bool ExecuteCommand(const FSageCommand& Command);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Command")
    void EvaluateCommandEffectiveness(const FString& CommandId, float Effectiveness);

    // 洞察生成功能
    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Insight")
    FSageInsight GenerateInsight(const TArray<FSageThoughtProcess>& Thoughts);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Insight")
    TArray<FSageInsight> AnalyzeSituation(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Insight")
    FSageInsight SynthesizeInsights(const TArray<FSageInsight>& Insights);

    // 智慧管理功能
    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Wisdom")
    FSageWisdom CreateWisdom(const FString& Experience, ESageThinkingLevel Level);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Wisdom")
    TArray<FSageWisdom> GetRelevantWisdom(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Wisdom")
    void UpdateWisdomApplicability(const FString& WisdomId, float Score);

    // 多層次思考
    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Multi-Level")
    TArray<FSageThoughtProcess> PerformMultiLevelThinking(const FString& Problem);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Multi-Level")
    FSageThoughtProcess ElevateThinking(const FSageThoughtProcess& BaseThought, ESageThinkingLevel TargetLevel);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Multi-Level")
    TArray<ESageThinkingLevel> DetermineOptimalThinkingLevels(const FString& Situation);

    // 自我學習與進化
    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Learning")
    void LearnFromThinkingResult(const FSageThoughtProcess& Thought, const FString& Result);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Learning")
    void LearnFromCommandExecution(const FSageCommand& Command, const FString& Outcome);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Learning")
    void EvolveThinkingPatterns(const TArray<FSageThoughtProcess>& SuccessfulThoughts);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Learning")
    void UpdateWisdomFromExperience(const FString& Experience, const FString& Lesson);

    // 跨API協調
    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Integration")
    void RequestCompilationGuidance(const FString& CodeContext);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Integration")
    void RequestOptimizationStrategy(const FString& PerformanceIssue);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Integration")
    void RequestDebuggingInsights(const TArray<FString>& ErrorPatterns);

    UFUNCTION(BlueprintCallable, Category = "Sage Command API|Integration")
    void UpdateSmartPanelWithWisdom(const TArray<FSageWisdom>& Wisdom);

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThoughtGenerated, const FSageThoughtProcess&, Thought);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCommandExecuted, const FSageCommand&, Command);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInsightDiscovered, const FSageInsight&, Insight);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWisdomCreated, const FSageWisdom&, Wisdom);

    UPROPERTY(BlueprintAssignable)
    FOnThoughtGenerated OnThoughtGenerated;

    UPROPERTY(BlueprintAssignable)
    FOnCommandExecuted OnCommandExecuted;

    UPROPERTY(BlueprintAssignable)
    FOnInsightDiscovered OnInsightDiscovered;

    UPROPERTY(BlueprintAssignable)
    FOnWisdomCreated OnWisdomCreated;

private:
    // 思考數據
    UPROPERTY()
    TArray<FSageThoughtProcess> ThoughtHistory;

    UPROPERTY()
    TArray<FSageCommand> CommandHistory;

    UPROPERTY()
    TArray<FSageInsight> InsightDatabase;

    UPROPERTY()
    TArray<FSageWisdom> WisdomDatabase;

    // 學習數據
    UPROPERTY()
    TMap<ESageThinkingLevel, TArray<FString>> ThinkingPatterns;

    UPROPERTY()
    TMap<FString, float> CommandEffectiveness;

    UPROPERTY()
    TMap<FString, TArray<FString>> WisdomApplications;

    // 內部方法
    void InitializeSageThinking();
    void SaveSageData();
    void LoadSageData();
    FString GenerateThoughtContent(const FString& Topic, ESageThinkingLevel Level);
    TArray<FString> GenerateReasoningSteps(const FString& Topic, ESageThinkingLevel Level);
    float CalculateThoughtConfidence(const FSageThoughtProcess& Thought);
    bool ValidateCommand(const FSageCommand& Command);
    FString SynthesizeWisdomContent(const TArray<FSageThoughtProcess>& Thoughts);
};
