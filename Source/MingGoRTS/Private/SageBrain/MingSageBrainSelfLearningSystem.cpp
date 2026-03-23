#include "SageBrain/MingSageBrainSelfLearningSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingSageBrainSelfLearningSystem::UMingSageBrainSelfLearningSystem()
    : bIsInitialized(false)
    , bIsSelfThinkingActive(false)
    , bIsSelfLearningActive(false)
    , CoreSageBrainSystem(nullptr)
{
}

bool UMingSageBrainSelfLearningSystem::InitializeSelfLearningSystem(UMingSageBrainCoreSystem* CoreSystem)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("自我學習系統已經初始化"));
        return true;
    }

    if (!CoreSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("無效的聖者大腦核心系統"));
        return false;
    }

    CoreSageBrainSystem = CoreSystem;
    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("聖者大腦自我學習系統初始化完成 ✓"));
    LogSelfLearningActivity(TEXT("系統初始化完成"));

    return true;
}

void UMingSageBrainSelfLearningSystem::ShutdownSelfLearningSystem()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 停止所有活動
    StopSelfThinkingLoop();
    StopContinuousLearning();

    // 保存數據（如果需要）
    ConsolidateKnowledge();

    bIsInitialized = false;
    CoreSageBrainSystem = nullptr;

    UE_LOG(LogTemp, Log, TEXT("聖者大腦自我學習系統已關閉"));
}

// ==================== 自我思考功能 ====================

bool UMingSageBrainSelfLearningSystem::StartSelfThinkingLoop(ESelfThinkingMode Mode, float IntervalSeconds)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("自我學習系統未初始化"));
        return false;
    }

    if (bIsSelfThinkingActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("自我思考循環已經在運行"));
        return true;
    }

    // 使用計時器啟動定期思考
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            SelfThinkingTimerHandle,
            this,
            &UMingSageBrainSelfLearningSystem::SelfThinkingLoop,
            IntervalSeconds,
            true
        );

        bIsSelfThinkingActive = true;
        UE_LOG(LogTemp, Log, TEXT("自我思考循環已啟動 - 模式: %s, 間隔: %.1f秒"),
            *UEnum::GetDisplayValueAsText(Mode).ToString(), IntervalSeconds);

        return true;
    }

    return false;
}

void UMingSageBrainSelfLearningSystem::StopSelfThinkingLoop()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(SelfThinkingTimerHandle);
    }

    bIsSelfThinkingActive = false;
    UE_LOG(LogTemp, Log, TEXT("自我思考循環已停止"));
}

FSelfThinkingRecord UMingSageBrainSelfLearningSystem::PerformSelfThinking(const FString& Context, ESelfThinkingMode Mode)
{
    FSelfThinkingRecord Record;
    Record.TriggerContext = Context;
    Record.Mode = Mode;

    // 根據模式選擇思考層次
    switch (Mode)
    {
    case ESelfThinkingMode::Passive:
        Record.Layer = ESageBrainThinkingLayer::Tactical;
        break;
    case ESelfThinkingMode::Active:
        Record.Layer = ESageBrainThinkingLayer::Strategic;
        break;
    case ESelfThinkingMode::Reflective:
        Record.Layer = ESageBrainThinkingLayer::Philosophical;
        break;
    case ESelfThinkingMode::Creative:
        Record.Layer = ESageBrainThinkingLayer::Innovative;
        break;
    case ESelfThinkingMode::Critical:
        Record.Layer = ESageBrainThinkingLayer::Historical;
        break;
    case ESelfThinkingMode::Meditative:
        Record.Layer = ESageBrainThinkingLayer::Transcendental;
        break;
    default:
        Record.Layer = ESageBrainThinkingLayer::Strategic;
    }

    // 生成思考內容
    Record.ThoughtContent = GenerateSelfThoughtContent(Context, Mode);
    Record.DepthScore = CalculateThoughtDepth(Record.ThoughtContent);
    Record.RelatedThoughts = FindRelatedThoughts(Record.ThoughtContent);

    // 提取見解
    Record.Insights.Add(TEXT("創新性"), FMath::RandRange(0.3f, 0.9f));
    Record.Insights.Add(TEXT("實用性"), FMath::RandRange(0.4f, 0.95f));
    Record.Insights.Add(TEXT("深度"), Record.DepthScore);
    Record.Insights.Add(TEXT("清晰度"), FMath::RandRange(0.5f, 0.98f));

    // 添加到歷史
    ThinkingHistory.Add(Record);

    // 限制歷史記錄大小
    if (ThinkingHistory.Num() > 1000)
    {
        ThinkingHistory.RemoveAt(0);
    }

    // 廣播事件
    OnSelfThoughtGenerated.Broadcast(Record);

    LogSelfLearningActivity(FString::Printf(TEXT("生成自我思考: %s (深度: %.2f)"), 
        *Record.ThoughtID, Record.DepthScore));

    return Record;
}

TArray<FSelfThinkingRecord> UMingSageBrainSelfLearningSystem::PerformDeepSelfReflection(int32 ReflectionDepth)
{
    TArray<FSelfThinkingRecord> ReflectionChain;

    FString CurrentContext = TEXT("深度自我反思起始");

    for (int32 i = 0; i < ReflectionDepth; i++)
    {
        FSelfThinkingRecord Reflection = PerformSelfThinking(CurrentContext, ESelfThinkingMode::Reflective);
        ReflectionChain.Add(Reflection);

        // 下一層反思以上一層的思考為基礎
        CurrentContext = FString::Printf(TEXT("反思層級 %d: %s"), i + 1, *Reflection.ThoughtContent.Left(100));

        UE_LOG(LogTemp, Log, TEXT("反思層級 %d/%d 完成"), i + 1, ReflectionDepth);
    }

    LogSelfLearningActivity(FString::Printf(TEXT("完成深度反思: %d 層"), ReflectionDepth));

    return ReflectionChain;
}

FSelfThinkingRecord UMingSageBrainSelfLearningSystem::PerformCreativeThinking(const FString& Topic)
{
    FSelfThinkingRecord Record = PerformSelfThinking(Topic, ESelfThinkingMode::Creative);

    // 增強創造性內容
    Record.Insights.Add(TEXT("原創性"), FMath::RandRange(0.6f, 0.99f));
    Record.Insights.Add(TEXT("新穎性"), FMath::RandRange(0.5f, 0.95f));
    Record.Insights.Add(TEXT("藝術性"), FMath::RandRange(0.4f, 0.9f));

    LogSelfLearningActivity(FString::Printf(TEXT("創造性思考完成: %s"), *Topic));

    return Record;
}

FSelfThinkingRecord UMingSageBrainSelfLearningSystem::PerformCriticalAnalysis(const FString& Subject)
{
    FSelfThinkingRecord Record = PerformSelfThinking(Subject, ESelfThinkingMode::Critical);

    // 增強批判性分析
    Record.Insights.Add(TEXT("邏輯性"), FMath::RandRange(0.7f, 0.98f));
    Record.Insights.Add(TEXT("客觀性"), FMath::RandRange(0.6f, 0.95f));
    Record.Insights.Add(TEXT("批判性"), FMath::RandRange(0.5f, 0.9f));

    LogSelfLearningActivity(FString::Printf(TEXT("批判性分析完成: %s"), *Subject));

    return Record;
}

TArray<FSelfThinkingRecord> UMingSageBrainSelfLearningSystem::GetThinkingHistory(int32 Count) const
{
    int32 StartIndex = FMath::Max(0, ThinkingHistory.Num() - Count);
    TArray<FSelfThinkingRecord> Result;

    for (int32 i = StartIndex; i < ThinkingHistory.Num(); i++)
    {
        Result.Add(ThinkingHistory[i]);
    }

    return Result;
}

TMap<ESelfThinkingMode, float> UMingSageBrainSelfLearningSystem::AnalyzeThinkingPatterns() const
{
    TMap<ESelfThinkingMode, int32> ModeCounts;
    TMap<ESelfThinkingMode, float> ModePercentages;

    // 統計各種思考模式的數量
    for (const auto& Record : ThinkingHistory)
    {
        ModeCounts.FindOrAdd(Record.Mode)++;
    }

    // 計算百分比
    if (ThinkingHistory.Num() > 0)
    {
        for (const auto& Pair : ModeCounts)
        {
            ModePercentages.Add(Pair.Key, (float)Pair.Value / ThinkingHistory.Num());
        }
    }

    return ModePercentages;
}

// ==================== 自我學習功能 ====================

FLearningExperience UMingSageBrainSelfLearningSystem::LearnFromExperience(const FString& Context, 
    const FString& Input, const FString& Output, const FString& Feedback, float SuccessRate)
{
    FLearningExperience Experience;
    Experience.Context = Context;
    Experience.Input = Input;
    Experience.Output = Output;
    Experience.Feedback = Feedback;
    Experience.SuccessRate = SuccessRate;

    // 提取學習模式
    Experience.LearnedPatterns.Add(ExtractPatterns(Input, Output));

    // 計算知識收益
    Experience.KnowledgeGains = CalculateKnowledgeGains(Context, SuccessRate);

    // 添加到學習經驗
    LearningExperiences.Add(Experience);

    // 限制經驗數量
    if (LearningExperiences.Num() > 500)
    {
        LearningExperiences.RemoveAt(0);
    }

    // 廣播事件
    OnExperienceLearned.Broadcast(Experience);

    LogSelfLearningActivity(FString::Printf(TEXT("學習經驗記錄: %s (成功率: %.2f%%)"),
        *Experience.ExperienceID, SuccessRate * 100));

    return Experience;
}

bool UMingSageBrainSelfLearningSystem::StartContinuousLearning(float CheckIntervalSeconds)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (bIsSelfLearningActive)
    {
        return true;
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            ContinuousLearningTimerHandle,
            this,
            &UMingSageBrainSelfLearningSystem::ContinuousLearningLoop,
            CheckIntervalSeconds,
            true
        );

        bIsSelfLearningActive = true;
        UE_LOG(LogTemp, Log, TEXT("持續學習模式已啟動 - 檢查間隔: %.1f秒"), CheckIntervalSeconds);

        return true;
    }

    return false;
}

void UMingSageBrainSelfLearningSystem::StopContinuousLearning()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(ContinuousLearningTimerHandle);
    }

    bIsSelfLearningActive = false;
    UE_LOG(LogTemp, Log, TEXT("持續學習模式已停止"));
}

bool UMingSageBrainSelfLearningSystem::AdvanceLearningStage(FLearningExperience& Experience)
{
    // 推進到下一個學習階段
    int32 CurrentStage = (int32)Experience.Stage;
    int32 NextStage = CurrentStage + 1;

    if (NextStage <= (int32)ESelfLearningStage::Reflection)
    {
        Experience.Stage = (ESelfLearningStage)NextStage;

        OnLearningStageAdvanced.Broadcast(Experience.ExperienceID, Experience.Stage);

        LogSelfLearningActivity(FString::Printf(TEXT("學習階段推進: %s -> %s"),
            *UEnum::GetDisplayValueAsText((ESelfLearningStage)CurrentStage).ToString(),
            *UEnum::GetDisplayValueAsText(Experience.Stage).ToString()));

        return true;
    }

    return false;
}

TMap<ESelfLearningStage, int32> UMingSageBrainSelfLearningSystem::GetLearningStageStatistics() const
{
    TMap<ESelfLearningStage, int32> Statistics;

    for (const auto& Experience : LearningExperiences)
    {
        Statistics.FindOrAdd(Experience.Stage)++;
    }

    return Statistics;
}

TArray<FLearningExperience> UMingSageBrainSelfLearningSystem::GetAllLearningExperiences() const
{
    return LearningExperiences;
}

// ==================== 知識管理 ====================

FSelfKnowledgeBase UMingSageBrainSelfLearningSystem::AddKnowledge(const FString& Category, 
    const FString& Content, float InitialConfidence)
{
    FSelfKnowledgeBase Knowledge;
    Knowledge.Category = Category;
    Knowledge.Content = Content;
    Knowledge.Confidence = FMath::Clamp(InitialConfidence, 0.0f, 1.0f);

    KnowledgeBase.Add(Knowledge);

    // 廣播事件
    OnKnowledgeAdded.Broadcast(Knowledge);

    LogSelfLearningActivity(FString::Printf(TEXT("新增知識: [%s] %s"), *Category, *Content.Left(50)));

    return Knowledge;
}

TArray<FSelfKnowledgeBase> UMingSageBrainSelfLearningSystem::QueryKnowledge(const FString& Category, 
    const FString& Keyword) const
{
    TArray<FSelfKnowledgeBase> Results;

    for (const auto& Knowledge : KnowledgeBase)
    {
        bool bMatchesCategory = Category.IsEmpty() || Knowledge.Category.Equals(Category, ESearchCase::IgnoreCase);
        bool bMatchesKeyword = Keyword.IsEmpty() || Knowledge.Content.Contains(Keyword, ESearchCase::IgnoreCase);

        if (bMatchesCategory && bMatchesKeyword)
        {
            Results.Add(Knowledge);
        }
    }

    return Results;
}

bool UMingSageBrainSelfLearningSystem::UpdateKnowledgeConfidence(const FString& KnowledgeID, float NewConfidence)
{
    for (auto& Knowledge : KnowledgeBase)
    {
        if (Knowledge.KnowledgeID == KnowledgeID)
        {
            Knowledge.Confidence = FMath::Clamp(NewConfidence, 0.0f, 1.0f);
            return true;
        }
    }

    return false;
}

bool UMingSageBrainSelfLearningSystem::UseKnowledge(const FString& KnowledgeID)
{
    for (auto& Knowledge : KnowledgeBase)
    {
        if (Knowledge.KnowledgeID == KnowledgeID)
        {
            Knowledge.UsageCount++;
            Knowledge.LastUsedTime = FDateTime::Now();
            return true;
        }
    }

    return false;
}

TArray<FSelfKnowledgeBase> UMingSageBrainSelfLearningSystem::GetTopKnowledge(int32 Count) const
{
    // 按使用次數和信心度排序
    TArray<FSelfKnowledgeBase> SortedKnowledge = KnowledgeBase;

    SortedKnowledge.Sort([](const FSelfKnowledgeBase& A, const FSelfKnowledgeBase& B)
    {
        float ScoreA = A.UsageCount * A.Confidence;
        float ScoreB = B.UsageCount * B.Confidence;
        return ScoreA > ScoreB;
    });

    // 返回前 N 個
    TArray<FSelfKnowledgeBase> Results;
    for (int32 i = 0; i < FMath::Min(Count, SortedKnowledge.Num()); i++)
    {
        Results.Add(SortedKnowledge[i]);
    }

    return Results;
}

bool UMingSageBrainSelfLearningSystem::ConsolidateKnowledge()
{
    UE_LOG(LogTemp, Log, TEXT("開始知識合併與優化..."));

    // 找出相似的知識並合併
    TArray<FSelfKnowledgeBase> NewKnowledgeBase;
    TArray<FString> MergedIDs;

    for (int32 i = 0; i < KnowledgeBase.Num(); i++)
    {
        if (MergedIDs.Contains(KnowledgeBase[i].KnowledgeID))
        {
            continue;
        }

        TArray<FSelfKnowledgeBase> SimilarKnowledge;
        SimilarKnowledge.Add(KnowledgeBase[i]);

        for (int32 j = i + 1; j < KnowledgeBase.Num(); j++)
        {
            if (MergedIDs.Contains(KnowledgeBase[j].KnowledgeID))
            {
                continue;
            }

            float Similarity = CalculateKnowledgeSimilarity(KnowledgeBase[i].Content, KnowledgeBase[j].Content);
            if (Similarity > 0.8f)
            {
                SimilarKnowledge.Add(KnowledgeBase[j]);
                MergedIDs.Add(KnowledgeBase[j].KnowledgeID);
            }
        }

        if (SimilarKnowledge.Num() > 1)
        {
            FSelfKnowledgeBase Merged = MergeSimilarKnowledge(SimilarKnowledge);
            NewKnowledgeBase.Add(Merged);
        }
        else
        {
            NewKnowledgeBase.Add(KnowledgeBase[i]);
        }
    }

    int32 OriginalCount = KnowledgeBase.Num();
    KnowledgeBase = NewKnowledgeBase;
    int32 NewCount = KnowledgeBase.Num();

    UE_LOG(LogTemp, Log, TEXT("知識合併完成: %d -> %d (減少 %d 項)"), 
        OriginalCount, NewCount, OriginalCount - NewCount);

    return true;
}

// ==================== 自我改進 ====================

TArray<FSelfImprovementSuggestion> UMingSageBrainSelfLearningSystem::GenerateSelfImprovementSuggestions()
{
    TArray<FSelfImprovementSuggestion> Suggestions = AnalyzeForImprovements();

    for (const auto& Suggestion : Suggestions)
    {
        ImprovementSuggestions.Add(Suggestion);
        OnImprovementSuggested.Broadcast(Suggestion);
    }

    LogSelfLearningActivity(FString::Printf(TEXT("生成 %d 個改進建議"), Suggestions.Num()));

    return Suggestions;
}

bool UMingSageBrainSelfLearningSystem::EvaluateImprovementSuggestion(const FString& SuggestionID)
{
    for (auto& Suggestion : ImprovementSuggestions)
    {
        if (Suggestion.SuggestionID == SuggestionID)
        {
            // 評估建議的價值
            float BenefitScore = FMath::RandRange(0.5f, 0.95f);
            float RiskScore = FMath::RandRange(0.1f, 0.4f);

            Suggestion.Priority = BenefitScore * (1.0f - RiskScore);

            return true;
        }
    }

    return false;
}

bool UMingSageBrainSelfLearningSystem::ImplementImprovementSuggestion(const FString& SuggestionID)
{
    for (auto& Suggestion : ImprovementSuggestions)
    {
        if (Suggestion.SuggestionID == SuggestionID)
        {
            if (ApplyImprovement(Suggestion))
            {
                Suggestion.bImplemented = true;
                LogSelfLearningActivity(FString::Printf(TEXT("實施改進: %s"), *Suggestion.TargetArea));
                return true;
            }
        }
    }

    return false;
}

TArray<FSelfImprovementSuggestion> UMingSageBrainSelfLearningSystem::GetImprovementHistory(bool bOnlyImplemented) const
{
    TArray<FSelfImprovementSuggestion> Results;

    for (const auto& Suggestion : ImprovementSuggestions)
    {
        if (!bOnlyImplemented || Suggestion.bImplemented)
        {
            Results.Add(Suggestion);
        }
    }

    return Results;
}

TMap<FString, float> UMingSageBrainSelfLearningSystem::PerformSelfAssessment()
{
    TMap<FString, float> Assessment;

    // 評估各個維度
    Assessment.Add(TEXT("知識廣度"), FMath::Min(1.0f, KnowledgeBase.Num() / 100.0f));
    Assessment.Add(TEXT("學習深度"), GetLearningStageStatistics().Contains(ESelfLearningStage::Reflection) ? 
        0.8f : 0.4f);
    Assessment.Add(TEXT("思考活躍度"), bIsSelfThinkingActive ? 0.9f : 0.3f);
    Assessment.Add(TEXT"改進意識", ImprovementSuggestions.Num() > 0 ? 0.85f : 0.5f);
    Assessment.Add(TEXT("經驗積累"), FMath::Min(1.0f, LearningExperiences.Num() / 200.0f));

    float OverallScore = 0.0f;
    for (const auto& Score : Assessment)
    {
        OverallScore += Score.Value;
    }
    OverallScore /= Assessment.Num();
    Assessment.Add(TEXT("總體評分"), OverallScore);

    LogSelfLearningActivity(FString::Printf(TEXT("自我評估完成 - 總體評分: %.2f"), OverallScore));

    return Assessment;
}

// ==================== 智能建議 ====================

TArray<FString> UMingSageBrainSelfLearningSystem::GenerateSmartSuggestions(const FString& Context) const
{
    TArray<FString> Suggestions;

    // 基於知識庫提供建議
    TArray<FSelfKnowledgeBase> RelevantKnowledge = QueryKnowledge(TEXT(""), Context);

    for (const auto& Knowledge : RelevantKnowledge)
    {
        if (Knowledge.Confidence > 0.7f)
        {
            Suggestions.Add(FString::Printf(TEXT("基於知識 '%s': %s"), 
                *Knowledge.Category, *Knowledge.Content.Left(100)));
        }
    }

    // 基於歷史經驗提供建議
    for (const auto& Experience : LearningExperiences)
    {
        if (Experience.Context.Contains(Context) && Experience.SuccessRate > 0.7f)
        {
            Suggestions.Add(FString::Printf(TEXT("經驗建議: 在 '%s' 情境下，'%s' 效果良好"),
                *Experience.Context, *Experience.Output.Left(50)));
        }
    }

    return Suggestions;
}

FString UMingSageBrainSelfLearningSystem::PredictBestAction(const FString& Context, 
    const TArray<FString>& PossibleActions) const
{
    if (PossibleActions.Num() == 0)
    {
        return TEXT("無可行動");
    }

    // 簡單的預測：選擇與成功經驗最相關的行動
    FString BestAction = PossibleActions[0];
    float BestScore = 0.0f;

    for (const auto& Action : PossibleActions)
    {
        float Score = 0.0f;

        for (const auto& Experience : LearningExperiences)
        {
            if (Experience.Output.Contains(Action))
            {
                Score += Experience.SuccessRate;
            }
        }

        if (Score > BestScore)
        {
            BestScore = Score;
            BestAction = Action;
        }
    }

    return BestAction;
}

float UMingSageBrainSelfLearningSystem::EvaluateDecisionQuality(const FString& Decision, 
    const FString& Outcome) const
{
    // 評估決策質量
    float Quality = 0.5f;

    // 檢查是否有類似的成功經驗
    for (const auto& Experience : LearningExperiences)
    {
        if (Experience.Output.Contains(Decision) && Experience.Feedback.Contains(Outcome))
        {
            Quality = FMath::Max(Quality, Experience.SuccessRate);
        }
    }

    return Quality;
}

// ==================== 狀態查詢 ====================

FString UMingSageBrainSelfLearningSystem::GetSelfLearningStatus() const
{
    return FString::Printf(TEXT("自我思考: %s | 持續學習: %s | 知識庫: %d | 經驗: %d"),
        bIsSelfThinkingActive ? TEXT("啟用") : TEXT("停用"),
        bIsSelfLearningActive ? TEXT("啟用") : TEXT("停用"),
        KnowledgeBase.Num(),
        LearningExperiences.Num());
}

// ==================== 內部輔助函數 ====================

void UMingSageBrainSelfLearningSystem::SelfThinkingLoop()
{
    // 自主思考循環
    FString Contexts[] = {
        TEXT("系統狀態分析"),
        TEXT("性能優化思考"),
        TEXT("用戶體驗改進"),
        TEXT("新功能探索"),
        TEXT("問題預測與預防")
    };

    int32 RandomIndex = FMath::RandRange(0, 4);
    PerformSelfThinking(Contexts[RandomIndex], ESelfThinkingMode::Active);
}

void UMingSageBrainSelfLearningSystem::ContinuousLearningLoop()
{
    // 持續學習循環
    // 分析最近的思考歷史並學習
    if (ThinkingHistory.Num() > 0)
    {
        const FSelfThinkingRecord& LastThought = ThinkingHistory.Last();

        LearnFromExperience(
            LastThought.TriggerContext,
            LastThought.ThoughtContent,
            TEXT("思考完成"),
            TEXT("自動評估"),
            LastThought.DepthScore
        );
    }
}

FString UMingSageBrainSelfLearningSystem::GenerateSelfThoughtContent(const FString& Context, ESelfThinkingMode Mode)
{
    // 生成思考內容
    FString ModeName = UEnum::GetDisplayValueAsText(Mode).ToString();

    return FString::Printf(TEXT("[%s] 關於 '%s' 的思考：\n深入分析當前狀況，綜合既有知識與經驗，")
        TEXT("形成系統性見解。考慮多種可能性，評估各種方案的優劣，")
        TEXT("最終得出具有建設性的結論。"),
        *ModeName, *Context);
}

float UMingSageBrainSelfLearningSystem::CalculateThoughtDepth(const FString& Content)
{
    // 計算思考深度
    int32 Length = Content.Len();
    float Depth = FMath::Min(1.0f, Length / 500.0f);
    Depth += FMath::RandRange(0.0f, 0.3f);
    return FMath::Min(1.0f, Depth);
}

TArray<FString> UMingSageBrainSelfLearningSystem::FindRelatedThoughts(const FString& Content)
{
    TArray<FString> Related;

    for (const auto& Record : ThinkingHistory)
    {
        // 簡單的關聯性檢查
        if (Record.ThoughtContent.Len() > 0 && FMath::RandBool())
        {
            Related.Add(Record.ThoughtID);
        }
    }

    return Related;
}

FString UMingSageBrainSelfLearningSystem::ExtractPatterns(const FString& Input, const FString& Output)
{
    return FString::Printf(TEXT("輸入 '%s' 與輸出 '%s' 之間的模式"), *Input.Left(30), *Output.Left(30));
}

TMap<FString, float> UMingSageBrainSelfLearningSystem::CalculateKnowledgeGains(const FString& Context, float SuccessRate)
{
    TMap<FString, float> Gains;
    Gains.Add(Context + TEXT("_理解度"), SuccessRate);
    Gains.Add(Context + TEXT("_熟練度"), SuccessRate * 0.9f);
    return Gains;
}

float UMingSageBrainSelfLearningSystem::CalculateKnowledgeSimilarity(const FString& ContentA, const FString& ContentB)
{
    // 簡單的相似度計算
    int32 CommonLength = FMath::Min(ContentA.Len(), ContentB.Len());
    int32 MaxLength = FMath::Max(ContentA.Len(), ContentB.Len());

    if (MaxLength == 0) return 0.0f;

    return (float)CommonLength / MaxLength;
}

FSelfKnowledgeBase UMingSageBrainSelfLearningSystem::MergeSimilarKnowledge(const TArray<FSelfKnowledgeBase>& SimilarKnowledge)
{
    FSelfKnowledgeBase Merged;
    Merged.Category = SimilarKnowledge[0].Category;

    FString CombinedContent;
    float TotalConfidence = 0.0f;
    int32 TotalUsage = 0;

    for (const auto& Knowledge : SimilarKnowledge)
    {
        if (!CombinedContent.IsEmpty())
        {
            CombinedContent += TEXT(" | ");
        }
        CombinedContent += Knowledge.Content;
        TotalConfidence += Knowledge.Confidence;
        TotalUsage += Knowledge.UsageCount;
    }

    Merged.Content = CombinedContent;
    Merged.Confidence = TotalConfidence / SimilarKnowledge.Num();
    Merged.UsageCount = TotalUsage;

    return Merged;
}

TArray<FSelfImprovementSuggestion> UMingSageBrainSelfLearningSystem::AnalyzeForImprovements()
{
    TArray<FSelfImprovementSuggestion> Suggestions;

    // 分析知識庫 gaps
    if (KnowledgeBase.Num() < 50)
    {
        FSelfImprovementSuggestion Suggestion;
        Suggestion.TargetArea = TEXT("知識庫擴展");
        Suggestion.CurrentState = FString::Printf(TEXT("當前知識項目: %d"), KnowledgeBase.Num());
        Suggestion.SuggestedChange = TEXT("增加更多領域的知識項目，提升系統廣度");
        Suggestion.ExpectedBenefit = TEXT("提升問題解決能力和建議質量");
        Suggestion.Priority = 0.8f;
        Suggestions.Add(Suggestion);
    }

    // 分析學習經驗
    if (LearningExperiences.Num() < 100)
    {
        FSelfImprovementSuggestion Suggestion;
        Suggestion.TargetArea = TEXT("經驗積累");
        Suggestion.CurrentState = FString::Printf(TEXT("當前經驗數量: %d"), LearningExperiences.Num());
        Suggestion.SuggestedChange = TEXT("增加更多實踐經驗，特別是邊界情況");
        Suggestion.ExpectedBenefit = TEXT("提升預測準確性和決策質量");
        Suggestion.Priority = 0.75f;
        Suggestions.Add(Suggestion);
    }

    // 分析思考模式
    if (!bIsSelfThinkingActive)
    {
        FSelfImprovementSuggestion Suggestion;
        Suggestion.TargetArea = TEXT("自主思考");
        Suggestion.CurrentState = TEXT("自我思考循環未啟動");
        Suggestion.SuggestedChange = TEXT("啟用定期自我思考，增強系統自主性");
        Suggestion.ExpectedBenefit = TEXT("持續的自我反思和改進");
        Suggestion.Priority = 0.9f;
        Suggestions.Add(Suggestion);
    }

    return Suggestions;
}

bool UMingSageBrainSelfLearningSystem::ApplyImprovement(const FSelfImprovementSuggestion& Suggestion)
{
    UE_LOG(LogTemp, Log, TEXT("應用改進: %s"), *Suggestion.TargetArea);

    // 實際應用改進
    if (Suggestion.TargetArea == TEXT("自主思考"))
    {
        return StartSelfThinkingLoop(ESelfThinkingMode::Active, 60.0f);
    }
    else if (Suggestion.TargetArea == TEXT("持續學習"))
    {
        return StartContinuousLearning(30.0f);
    }

    return true;
}

void UMingSageBrainSelfLearningSystem::LogSelfLearningActivity(const FString& Activity) const
{
    UE_LOG(LogTemp, Log, TEXT("[自我學習] %s"), *Activity);
}
