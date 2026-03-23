#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "MingSageBrainCoreSystem.h"
#include "MingSageBrainSelfLearningSystem.generated.h"

// 自我思考模式枚舉
UENUM(BlueprintType)
enum class ESelfThinkingMode : uint8
{
    Passive      UMETA(DisplayName = "被動模式"),     // 只在收到輸入時思考
    Active       UMETA(DisplayName = "主動模式"),     // 定期自主思考
    Reflective   UMETA(DisplayName = "反思模式"),   // 深度自我反思
    Creative     UMETA(DisplayName = "創造模式"),    // 創造性思考
    Critical     UMETA(DisplayName = "批判模式"),    // 批判性分析
    Meditative   UMETA(DisplayName = "冥想模式")     // 深度冥想思考
};

// 自我學習階段枚舉
UENUM(BlueprintType)
enum class ESelfLearningStage : uint8
{
    Observation   UMETA(DisplayName = "觀察階段"),    // 收集數據
    Analysis      UMETA(DisplayName = "分析階段"),    // 分析模式
    Synthesis     UMETA(DisplayName = "綜合階段"),    // 綜合知識
    Integration   UMETA(DisplayName = "整合階段"),   // 整合到系統
    Application   UMETA(DisplayName = "應用階段"),    // 應用新知識
    Reflection    UMETA(DisplayName = "反思階段")     // 反思學習成果
};

// 思考歷史記錄
USTRUCT(BlueprintType)
struct MINGGORTS_API FSelfThinkingRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    FString ThoughtID;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    FString TriggerContext;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    FString ThoughtContent;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    ESelfThinkingMode Mode;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    ESageBrainThinkingLayer Layer;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    float DepthScore;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    TArray<FString> RelatedThoughts;

    UPROPERTY(BlueprintReadOnly, Category = "Self Thinking")
    TMap<FString, float> Insights;

    FSelfThinkingRecord()
    {
        ThoughtID = FGuid::NewGuid().ToString();
        TriggerContext = TEXT("");
        ThoughtContent = TEXT("");
        Mode = ESelfThinkingMode::Passive;
        Layer = ESageBrainThinkingLayer::Tactical;
        DepthScore = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

// 學習經驗記錄
USTRUCT(BlueprintType)
struct MINGGORTS_API FLearningExperience
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    FString ExperienceID;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    FString Context;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    FString Input;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    FString Output;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    FString Feedback;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    ESelfLearningStage Stage;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    TArray<FString> LearnedPatterns;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Experience")
    TMap<FString, float> KnowledgeGains;

    FLearningExperience()
    {
        ExperienceID = FGuid::NewGuid().ToString();
        Context = TEXT("");
        Input = TEXT("");
        Output = TEXT("");
        Feedback = TEXT("");
        SuccessRate = 0.0f;
        Stage = ESelfLearningStage::Observation;
        Timestamp = FDateTime::Now();
    }
};

// 自我知識庫
USTRUCT(BlueprintType)
struct MINGGORTS_API FSelfKnowledgeBase
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    FString KnowledgeID;

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    FString Category;

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    FString Content;

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    float Confidence;

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    int32 UsageCount;

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    FDateTime CreatedTime;

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    FDateTime LastUsedTime;

    UPROPERTY(BlueprintReadOnly, Category = "Knowledge Base")
    TArray<FString> RelatedExperiences;

    FSelfKnowledgeBase()
    {
        KnowledgeID = FGuid::NewGuid().ToString();
        Category = TEXT("General");
        Content = TEXT("");
        Confidence = 0.5f;
        UsageCount = 0;
        CreatedTime = FDateTime::Now();
        LastUsedTime = FDateTime::Now();
    }
};

// 自我改進建議
USTRUCT(BlueprintType)
struct MINGGORTS_API FSelfImprovementSuggestion
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    FString SuggestionID;

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    FString TargetArea;

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    FString CurrentState;

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    FString SuggestedChange;

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    FString ExpectedBenefit;

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    float Priority;

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    bool bImplemented;

    UPROPERTY(BlueprintReadOnly, Category = "Self Improvement")
    FDateTime CreatedTime;

    FSelfImprovementSuggestion()
    {
        SuggestionID = FGuid::NewGuid().ToString();
        TargetArea = TEXT("");
        CurrentState = TEXT("");
        SuggestedChange = TEXT("");
        ExpectedBenefit = TEXT("");
        Priority = 0.5f;
        bImplemented = false;
        CreatedTime = FDateTime::Now();
    }
};

/**
 * 聖者大腦自我學習系統
 * 提供自我思考、自我學習、自我改進的高級AI功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainSelfLearningSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainSelfLearningSystem();

    // ==================== 系統初始化 ====================
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    bool InitializeSelfLearningSystem(UMingSageBrainCoreSystem* CoreSystem);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    void ShutdownSelfLearningSystem();

    // ==================== 自我思考功能 ====================
    
    // 啟動自主思考循環
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    bool StartSelfThinkingLoop(ESelfThinkingMode Mode, float IntervalSeconds);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    void StopSelfThinkingLoop();

    // 執行一次自我思考
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    FSelfThinkingRecord PerformSelfThinking(const FString& Context, ESelfThinkingMode Mode);

    // 深度自我反思
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    TArray<FSelfThinkingRecord> PerformDeepSelfReflection(int32 ReflectionDepth);

    // 創造性思考
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    FSelfThinkingRecord PerformCreativeThinking(const FString& Topic);

    // 批判性分析
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    FSelfThinkingRecord PerformCriticalAnalysis(const FString& Subject);

    // 獲取思考歷史
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    TArray<FSelfThinkingRecord> GetThinkingHistory(int32 Count = 50) const;

    // 分析思考模式
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Thinking")
    TMap<ESelfThinkingMode, float> AnalyzeThinkingPatterns() const;

    // ==================== 自我學習功能 ====================
    
    // 從經驗學習
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    FLearningExperience LearnFromExperience(const FString& Context, const FString& Input, 
        const FString& Output, const FString& Feedback, float SuccessRate);

    // 啟動持續學習模式
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    bool StartContinuousLearning(float CheckIntervalSeconds);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    void StopContinuousLearning();

    // 學習階段推進
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    bool AdvanceLearningStage(FLearningExperience& Experience);

    // 獲取學習統計
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    TMap<ESelfLearningStage, int32> GetLearningStageStatistics() const;

    // 獲取所有學習經驗
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning")
    TArray<FLearningExperience> GetAllLearningExperiences() const;

    // ==================== 知識管理 ====================
    
    // 添加知識
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Knowledge")
    FSelfKnowledgeBase AddKnowledge(const FString& Category, const FString& Content, float InitialConfidence);

    // 查詢知識
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Knowledge")
    TArray<FSelfKnowledgeBase> QueryKnowledge(const FString& Category, const FString& Keyword) const;

    // 更新知識信心度
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Knowledge")
    bool UpdateKnowledgeConfidence(const FString& KnowledgeID, float NewConfidence);

    // 使用知識（增加使用次數）
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Knowledge")
    bool UseKnowledge(const FString& KnowledgeID);

    // 獲取熱門知識
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Knowledge")
    TArray<FSelfKnowledgeBase> GetTopKnowledge(int32 Count = 10) const;

    // 知識合併與優化
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Knowledge")
    bool ConsolidateKnowledge();

    // ==================== 自我改進 ====================
    
    // 生成自我改進建議
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Improvement")
    TArray<FSelfImprovementSuggestion> GenerateSelfImprovementSuggestions();

    // 評估改進建議
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Improvement")
    bool EvaluateImprovementSuggestion(const FString& SuggestionID);

    // 實施改進建議
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Improvement")
    bool ImplementImprovementSuggestion(const FString& SuggestionID);

    // 獲取改進歷史
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Improvement")
    TArray<FSelfImprovementSuggestion> GetImprovementHistory(bool bOnlyImplemented = false) const;

    // 自我評估
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Improvement")
    TMap<FString, float> PerformSelfAssessment();

    // ==================== 智能建議 ====================
    
    // 基於學習歷史提供建議
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Smart Suggestions")
    TArray<FString> GenerateSmartSuggestions(const FString& Context) const;

    // 預測最佳行動
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Smart Suggestions")
    FString PredictBestAction(const FString& Context, const TArray<FString>& PossibleActions) const;

    // 評估決策質量
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Smart Suggestions")
    float EvaluateDecisionQuality(const FString& Decision, const FString& Outcome) const;

    // ==================== 狀態查詢 ====================
    
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning|Status")
    bool IsSelfLearningActive() const { return bIsSelfLearningActive; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning|Status")
    bool IsSelfThinkingActive() const { return bIsSelfThinkingActive; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning|Status")
    int32 GetKnowledgeCount() const { return KnowledgeBase.Num(); }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning|Status")
    int32 GetExperienceCount() const { return LearningExperiences.Num(); }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Self Learning|Status")
    FString GetSelfLearningStatus() const;

    // ==================== 事件委託 ====================
    
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelfThoughtGenerated, const FSelfThinkingRecord&, Thought);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExperienceLearned, const FLearningExperience&, Experience);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKnowledgeAdded, const FSelfKnowledgeBase&, Knowledge);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImprovementSuggested, const FSelfImprovementSuggestion&, Suggestion);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLearningStageAdvanced, const FString&, ExperienceID, ESelfLearningStage, NewStage);

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Self Learning|Events")
    FOnSelfThoughtGenerated OnSelfThoughtGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Self Learning|Events")
    FOnExperienceLearned OnExperienceLearned;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Self Learning|Events")
    FOnKnowledgeAdded OnKnowledgeAdded;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Self Learning|Events")
    FOnImprovementSuggested OnImprovementSuggested;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Self Learning|Events")
    FOnLearningStageAdvanced OnLearningStageAdvanced;

private:
    bool bIsInitialized;
    bool bIsSelfThinkingActive;
    bool bIsSelfLearningActive;
    
    UPROPERTY()
    TObjectPtr<UMingSageBrainCoreSystem> CoreSageBrainSystem;

    // 資料存儲
    UPROPERTY()
    TArray<FSelfThinkingRecord> ThinkingHistory;

    UPROPERTY()
    TArray<FLearningExperience> LearningExperiences;

    UPROPERTY()
    TArray<FSelfKnowledgeBase> KnowledgeBase;

    UPROPERTY()
    TArray<FSelfImprovementSuggestion> ImprovementSuggestions;

    // 計時器
    FTimerHandle SelfThinkingTimerHandle;
    FTimerHandle ContinuousLearningTimerHandle;

    // 內部輔助函數
    FString GenerateSelfThoughtContent(const FString& Context, ESelfThinkingMode Mode);
    float CalculateThoughtDepth(const FString& Content);
    TArray<FString> FindRelatedThoughts(const FString& Content);
    
    FString ExtractPatterns(const FString& Input, const FString& Output);
    TMap<FString, float> CalculateKnowledgeGains(const FString& Context, float SuccessRate);
    
    void SelfThinkingLoop();
    void ContinuousLearningLoop();
    
    FSelfKnowledgeBase MergeSimilarKnowledge(const TArray<FSelfKnowledgeBase>& SimilarKnowledge);
    float CalculateKnowledgeSimilarity(const FString& ContentA, const FString& ContentB);
    
    TArray<FSelfImprovementSuggestion> AnalyzeForImprovements();
    bool ApplyImprovement(const FSelfImprovementSuggestion& Suggestion);
    
    void LogSelfLearningActivity(const FString& Activity) const;
};
