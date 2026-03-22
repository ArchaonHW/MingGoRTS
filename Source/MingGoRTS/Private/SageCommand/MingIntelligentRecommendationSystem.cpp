// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingIntelligentRecommendationSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"

UMingIntelligentRecommendationSystem::UMingIntelligentRecommendationSystem()
    : bSystemActive(false)
    , SystemPerformance(100.0f)
    , LastUpdateTime(0.0f)
    , RecommendationIDCounter(0)
    , bIsInitialized(false)
{
}

bool UMingIntelligentRecommendationSystem::Initialize()
{
    if (bIsInitialized)
    {
        return true;
    }

    InitializeSystemComponents();
    
    // 設置定時更新
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            UpdateTimerHandle,
            this,
            &UMingIntelligentRecommendationSystem::UpdateSystemState,
            Config.UpdateInterval,
            true
        );
    }

    bSystemActive = true;
    bIsInitialized = true;

    // 生成初始建議
    TArray<FRecommendationData> InitialRecommendations = GenerateRecommendations("SystemInitialization");
    
    UE_LOG(LogTemp, Log, TEXT("智能建議系統初始化完成，生成 %d 條初始建議"), InitialRecommendations.Num());
    
    return true;
}

void UMingIntelligentRecommendationSystem::Cleanup()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(UpdateTimerHandle);
    }

    ActiveRecommendations.Empty();
    RecommendationHistory.Empty();
    PlayerBehaviorPatterns.Empty();
    PersonalizationPreferences.Empty();

    bSystemActive = false;
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("智能建議系統已清理"));
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::GenerateRecommendations(const FString& Context)
{
    if (!bSystemActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("智能建議系統未激活"));
        return TArray<FRecommendationData>();
    }

    TArray<FRecommendationData> AllRecommendations;
    
    // AI 分析
    TArray<FRecommendationData> AIRecommendations = PerformAIAnalysis(Context);
    AllRecommendations.Append(AIRecommendations);

    // 玩家行為分析
    TArray<FRecommendationData> BehaviorRecommendations = AnalyzePlayerBehavior(Context);
    AllRecommendations.Append(BehaviorRecommendations);

    // 遊戲情境分析
    TArray<FRecommendationData> ContextRecommendations = AnalyzeGameContext(Context);
    AllRecommendations.Append(ContextRecommendations);

    // 歷史數據分析
    TArray<FRecommendationData> HistoricalRecommendations = AnalyzeHistoricalData(Context);
    AllRecommendations.Append(HistoricalRecommendations);

    // 機器學習預測
    if (Config.bEnableMachineLearning)
    {
        TArray<FRecommendationData> MLRecommendations = PredictRecommendations(Context);
        AllRecommendations.Append(MLRecommendations);
    }

    // 個人化推薦
    if (Config.bEnablePersonalization)
    {
        TArray<FRecommendationData> PersonalizedRecommendations = GeneratePersonalizedRecommendations(Context);
        AllRecommendations.Append(PersonalizedRecommendations);
    }

    // 社群數據分析
    if (Config.bEnableCommunityData)
    {
        TArray<FRecommendationData> CommunityRecommendations = AnalyzeCommunityData(Context);
        AllRecommendations.Append(CommunityRecommendations);
    }

    // 合併和去重
    TArray<TArray<FRecommendationData>> RecommendationSets;
    RecommendationSets.Add(AIRecommendations);
    RecommendationSets.Add(BehaviorRecommendations);
    RecommendationSets.Add(ContextRecommendations);
    RecommendationSets.Add(HistoricalRecommendations);
    
    TArray<FRecommendationData> MergedRecommendations = MergeAndDeduplicateRecommendations(RecommendationSets);

    // 評分和排序
    for (FRecommendationData& Recommendation : MergedRecommendations)
    {
        Recommendation.ImpactScore = CalculateImpactScore(Recommendation);
        Recommendation.ConfidenceScore = CalculateConfidenceScore(Recommendation);
        Recommendation.RelevanceScore = CalculateRelevanceScore(Recommendation, Context);
        Recommendation.DifficultyScore = CalculateDifficultyScore(Recommendation);
    }

    // 根據優先級和相關性排序
    MergedRecommendations = SortRecommendationsByPriority(MergedRecommendations);
    MergedRecommendations = SortRecommendationsByRelevance(MergedRecommendations);

    // 限制數量
    if (MergedRecommendations.Num() > Config.MaxActiveRecommendations)
    {
        MergedRecommendations.SetNum(Config.MaxActiveRecommendations);
    }

    // 驗證建議
    TArray<FRecommendationData> ValidRecommendations;
    for (const FRecommendationData& Recommendation : MergedRecommendations)
    {
        if (ValidateRecommendation(Recommendation))
        {
            ValidRecommendations.Add(Recommendation);
            
            // 添加到活躍建議
            ActiveRecommendations.Add(Recommendation);
            
            // 添加到歷史
            RecommendationHistory.Add(Recommendation);
            
            // 廣播事件
            OnRecommendationGenerated.Broadcast(Recommendation);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("為情境 '%s' 生成了 %d 條有效建議"), *Context, ValidRecommendations.Num());
    
    return ValidRecommendations;
}

FRecommendationData UMingIntelligentRecommendationSystem::GenerateSpecificRecommendation(ERecommendationType Type, const FString& Context)
{
    FRecommendationData Recommendation;
    Recommendation.RecommendationID = GenerateUniqueID();
    Recommendation.RecommendationType = Type;
    Recommendation.CreatedTime = FDateTime::Now();
    Recommendation.ExpiryTime = Recommendation.CreatedTime + FTimespan::FromHours(Config.ExpiryHours);
    Recommendation.Status = ERecommendationStatus::Pending;

    switch (Type)
    {
    case ERecommendationType::Strategic:
        Recommendation.Title = TEXT("戰略建議");
        Recommendation.Description = TEXT("基於當前戰略局勢的建議");
        Recommendation.Source = ERecommendationSource::AIAnalysis;
        Recommendation.Priority = ERecommendationPriority::High;
        break;

    case ERecommendationType::Tactical:
        Recommendation.Title = TEXT("戰術建議");
        Recommendation.Description = TEXT("基於當前戰術局勢的建議");
        Recommendation.Source = ERecommendationSource::GameContext;
        Recommendation.Priority = ERecommendationPriority::Medium;
        break;

    case ERecommendationType::Resource:
        Recommendation.Title = TEXT("資源建議");
        Recommendation.Description = TEXT("資源管理和分配的建議");
        Recommendation.Source = ERecommendationSource::PlayerBehavior;
        Recommendation.Priority = ERecommendationPriority::Medium;
        break;

    case ERecommendationType::Diplomatic:
        Recommendation.Title = TEXT("外交建議");
        Recommendation.Description = TEXT("外交關係和策略的建議");
        Recommendation.Source = ERecommendationSource::HistoricalData;
        Recommendation.Priority = ERecommendationPriority::High;
        break;

    case ERecommendationType::Economic:
        Recommendation.Title = TEXT("經濟建議");
        Recommendation.Description = TEXT("經濟發展和管理的建議");
        Recommendation.Source = ERecommendationSource::AIAnalysis;
        Recommendation.Priority = ERecommendationPriority::Medium;
        break;

    case ERecommendationType::Military:
        Recommendation.Title = TEXT("軍事建議");
        Recommendation.Description = TEXT("軍事行動和部署的建議");
        Recommendation.Source = ERecommendationSource::GameContext;
        Recommendation.Priority = ERecommendationPriority::High;
        break;

    case ERecommendationType::Cultural:
        Recommendation.Title = TEXT("文化建議");
        Recommendation.Description = TEXT("文化發展和傳播的建議");
        Recommendation.Source = ERecommendationSource::ExpertSystem;
        Recommendation.Priority = ERecommendationPriority::Low;
        break;

    case ERecommendationType::Personal:
        Recommendation.Title = TEXT("個人建議");
        Recommendation.Description = TEXT("個人發展和成長的建議");
        Recommendation.Source = ERecommendationSource::Personalized;
        Recommendation.Priority = ERecommendationPriority::Medium;
        break;

    case ERecommendationType::Emergency:
        Recommendation.Title = TEXT("緊急建議");
        Recommendation.Description = TEXT("緊急情況的應對建議");
        Recommendation.Source = ERecommendationSource::SystemGenerated;
        Recommendation.Priority = ERecommendationPriority::Critical;
        break;

    case ERecommendationType::LongTerm:
        Recommendation.Title = TEXT("長期建議");
        Recommendation.Description = TEXT("長期發展規劃的建議");
        Recommendation.Source = ERecommendationSource::MachineLearning;
        Recommendation.Priority = ERecommendationPriority::Low;
        break;

    default:
        break;
    }

    // 計算分數
    Recommendation.ImpactScore = CalculateImpactScore(Recommendation);
    Recommendation.ConfidenceScore = CalculateConfidenceScore(Recommendation);
    Recommendation.RelevanceScore = CalculateRelevanceScore(Recommendation, Context);
    Recommendation.DifficultyScore = CalculateDifficultyScore(Recommendation);

    return Recommendation;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::GetActiveRecommendations() const
{
    return ActiveRecommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::GetRecommendationsByType(ERecommendationType Type) const
{
    TArray<FRecommendationData> FilteredRecommendations;
    
    for (const FRecommendationData& Recommendation : ActiveRecommendations)
    {
        if (Recommendation.RecommendationType == Type)
        {
            FilteredRecommendations.Add(Recommendation);
        }
    }
    
    return FilteredRecommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::GetRecommendationsByPriority(ERecommendationPriority Priority) const
{
    TArray<FRecommendationData> FilteredRecommendations;
    
    for (const FRecommendationData& Recommendation : ActiveRecommendations)
    {
        if (Recommendation.Priority == Priority)
        {
            FilteredRecommendations.Add(Recommendation);
        }
    }
    
    return FilteredRecommendations;
}

bool UMingIntelligentRecommendationSystem::AcceptRecommendation(const FString& RecommendationID)
{
    for (FRecommendationData& Recommendation : ActiveRecommendations)
    {
        if (Recommendation.RecommendationID == RecommendationID)
        {
            Recommendation.Status = ERecommendationStatus::Accepted;
            Recommendation.ExecutionCount++;
            
            // 應用建議後果
            ApplyRecommendationConsequences(Recommendation);
            
            // 廣播事件
            OnRecommendationAccepted.Broadcast(Recommendation);
            OnRecommendationStatusChanged.Broadcast(RecommendationID, ERecommendationStatus::Accepted);
            
            // 更新統計
            Statistics.AcceptedRecommendations++;
            UpdateStatistics();
            
            UE_LOG(LogTemp, Log, TEXT("建議 %s 已被接受"), *RecommendationID);
            return true;
        }
    }
    
    return false;
}

bool UMingIntelligentRecommendationSystem::RejectRecommendation(const FString& RecommendationID)
{
    for (FRecommendationData& Recommendation : ActiveRecommendations)
    {
        if (Recommendation.RecommendationID == RecommendationID)
        {
            Recommendation.Status = ERecommendationStatus::Rejected;
            
            // 廣播事件
            OnRecommendationRejected.Broadcast(Recommendation);
            OnRecommendationStatusChanged.Broadcast(RecommendationID, ERecommendationStatus::Rejected);
            
            // 更新統計
            Statistics.RejectedRecommendations++;
            UpdateStatistics();
            
            UE_LOG(LogTemp, Log, TEXT("建議 %s 已被拒絕"), *RecommendationID);
            return true;
        }
    }
    
    return false;
}

bool UMingIntelligentRecommendationSystem::CompleteRecommendation(const FString& RecommendationID)
{
    for (FRecommendationData& Recommendation : ActiveRecommendations)
    {
        if (Recommendation.RecommendationID == RecommendationID)
        {
            Recommendation.Status = ERecommendationStatus::Completed;
            Recommendation.SuccessCount++;
            
            // 廣播事件
            OnRecommendationCompleted.Broadcast(Recommendation);
            OnRecommendationStatusChanged.Broadcast(RecommendationID, ERecommendationStatus::Completed);
            
            // 更新統計
            Statistics.CompletedRecommendations++;
            UpdateStatistics();
            
            // 更新個人化模型
            if (Config.bEnablePersonalization)
            {
                UpdatePersonalizationModel(RecommendationID, true);
            }
            
            UE_LOG(LogTemp, Log, TEXT("建議 %s 已完成"), *RecommendationID);
            return true;
        }
    }
    
    return false;
}

bool UMingIntelligentRecommendationSystem::IgnoreRecommendation(const FString& RecommendationID)
{
    for (FRecommendationData& Recommendation : ActiveRecommendations)
    {
        if (Recommendation.RecommendationID == RecommendationID)
        {
            Recommendation.Status = ERecommendationStatus::Ignored;
            
            // 廣播事件
            OnRecommendationStatusChanged.Broadcast(RecommendationID, ERecommendationStatus::Ignored);
            
            UE_LOG(LogTemp, Log, TEXT("建議 %s 已被忽略"), *RecommendationID);
            return true;
        }
    }
    
    return false;
}

bool UMingIntelligentRecommendationSystem::ProvideFeedback(const FString& RecommendationID, const FString& Feedback, int32 Rating)
{
    for (FRecommendationData& Recommendation : ActiveRecommendations)
    {
        if (Recommendation.RecommendationID == RecommendationID)
        {
            Recommendation.PlayerFeedback = Feedback;
            Recommendation.PlayerRating = FMath::Clamp(Rating, 1, 5);
            
            // 更新個人化模型
            if (Config.bEnablePersonalization)
            {
                UpdatePersonalizationModel(RecommendationID, Recommendation.PlayerRating >= 3);
            }
            
            UE_LOG(LogTemp, Log, TEXT("建議 %s 收到反饋: %s, 評分: %d"), *RecommendationID, *Feedback, Rating);
            return true;
        }
    }
    
    return false;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::FilterRecommendations(const TArray<FRecommendationData>& Recommendations, const TArray<ERecommendationType>& Types)
{
    TArray<FRecommendationData> FilteredRecommendations;
    
    for (const FRecommendationData& Recommendation : Recommendations)
    {
        if (Types.Contains(Recommendation.RecommendationType))
        {
            FilteredRecommendations.Add(Recommendation);
        }
    }
    
    return FilteredRecommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::SortRecommendationsByPriority(const TArray<FRecommendationData>& Recommendations)
{
    TArray<FRecommendationData> SortedRecommendations = Recommendations;
    
    SortedRecommendations.Sort([](const FRecommendationData& A, const FRecommendationData& B)
    {
        // 優先級排序：Critical > High > Medium > Low > SystemGenerated
        if (A.Priority != B.Priority)
        {
            return static_cast<int32>(A.Priority) < static_cast<int32>(B.Priority);
        }
        
        // 相同優先級按影響分數排序
        return A.ImpactScore > B.ImpactScore;
    });
    
    return SortedRecommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::SortRecommendationsByRelevance(const TArray<FRecommendationData>& Recommendations)
{
    TArray<FRecommendationData> SortedRecommendations = Recommendations;
    
    SortedRecommendations.Sort([](const FRecommendationData& A, const FRecommendationData& B)
    {
        return A.RelevanceScore > B.RelevanceScore;
    });
    
    return SortedRecommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::SortRecommendationsByImpact(const TArray<FRecommendationData>& Recommendations)
{
    TArray<FRecommendationData> SortedRecommendations = Recommendations;
    
    SortedRecommendations.Sort([](const FRecommendationData& A, const FRecommendationData& B)
    {
        return A.ImpactScore > B.ImpactScore;
    });
    
    return SortedRecommendations;
}

FRecommendationStatistics UMingIntelligentRecommendationSystem::GetRecommendationStatistics() const
{
    return Statistics;
}

TArray<FString> UMingIntelligentRecommendationSystem::GetRecommendationTrends() const
{
    TArray<FString> Trends;
    
    // 分析最常見的建議類型
    TMap<ERecommendationType, int32> TypeCounts;
    for (const FRecommendationData& Recommendation : RecommendationHistory)
    {
        TypeCounts.FindOrAdd(Recommendation.RecommendationType, 0)++;
    }
    
    // 找出最常見的類型
    ERecommendationType MostCommonType = ERecommendationType::None;
    int32 MaxCount = 0;
    for (const auto& TypeCount : TypeCounts)
    {
        if (TypeCount.Value > MaxCount)
        {
            MaxCount = TypeCount.Value;
            MostCommonType = TypeCount.Key;
        }
    }
    
    // 生成趨勢描述
    if (MostCommonType != ERecommendationType::None)
    {
        FString TrendDescription = FString::Printf(TEXT("最常見的建議類型: %s (%d 次)"), 
            *UEnum::GetValueAsString(MostCommonType), MaxCount);
        Trends.Add(TrendDescription);
    }
    
    // 分析成功率趨勢
    if (Statistics.TotalRecommendations > 0)
    {
        FString SuccessTrend = FString::Printf(TEXT("建議成功率: %.1f%%"), Statistics.SuccessRate);
        Trends.Add(SuccessTrend);
    }
    
    // 分析玩家評分趨勢
    if (Statistics.AveragePlayerRating > 0)
    {
        FString RatingTrend = FString::Printf(TEXT("平均玩家評分: %.1f/5.0"), Statistics.AveragePlayerRating);
        Trends.Add(RatingTrend);
    }
    
    return Trends;
}

float UMingIntelligentRecommendationSystem::CalculateRecommendationEffectiveness(const FString& RecommendationID) const
{
    for (const FRecommendationData& Recommendation : RecommendationHistory)
    {
        if (Recommendation.RecommendationID == RecommendationID)
        {
            if (Recommendation.ExecutionCount > 0)
            {
                return (static_cast<float>(Recommendation.SuccessCount) / Recommendation.ExecutionCount) * 100.0f;
            }
        }
    }
    
    return 0.0f;
}

TArray<FString> UMingIntelligentRecommendationSystem::GetPlayerBehaviorPatterns() const
{
    TArray<FString> Patterns;
    
    for (const auto& Pattern : PlayerBehaviorPatterns)
    {
        FString PatternDescription = FString::Printf(TEXT("%s: %d 次"), *Pattern.Key, Pattern.Value);
        Patterns.Add(PatternDescription);
    }
    
    return Patterns;
}

void UMingIntelligentRecommendationSystem::SetPersonalizationPreferences(const TMap<FString, float>& Preferences)
{
    PersonalizationPreferences = Preferences;
}

TMap<FString, float> UMingIntelligentRecommendationSystem::GetPersonalizationPreferences() const
{
    return PersonalizationPreferences;
}

void UMingIntelligentRecommendationSystem::UpdatePersonalizationModel(const FString& RecommendationID, bool bSuccess)
{
    // 更新個人化偏好
    for (const FRecommendationData& Recommendation : RecommendationHistory)
    {
        if (Recommendation.RecommendationID == RecommendationID)
        {
            FString TypeString = UEnum::GetValueAsString(Recommendation.RecommendationType);
            float CurrentPreference = PersonalizationPreferences.FindRef(TypeString);
            
            // 根據成功/失敗調整偏好
            if (bSuccess)
            {
                CurrentPreference = FMath::Min(CurrentPreference + 0.1f, 1.0f);
            }
            else
            {
                CurrentPreference = FMath::Max(CurrentPreference - 0.05f, 0.0f);
            }
            
            PersonalizationPreferences.Add(TypeString, CurrentPreference);
            break;
        }
    }
}

void UMingIntelligentRecommendationSystem::SetRecommendationConfig(const FRecommendationConfig& Config)
{
    this->Config = Config;
}

FRecommendationConfig UMingIntelligentRecommendationSystem::GetRecommendationConfig() const
{
    return Config;
}

void UMingIntelligentRecommendationSystem::InitializeSystemComponents()
{
    // 初始化統計數據
    Statistics.LastUpdateTime = FDateTime::Now();
    
    // 初始化個人化偏好
    PersonalizationPreferences.Add(TEXT("Strategic"), 0.5f);
    PersonalizationPreferences.Add(TEXT("Tactical"), 0.5f);
    PersonalizationPreferences.Add(TEXT("Resource"), 0.5f);
    PersonalizationPreferences.Add(TEXT("Diplomatic"), 0.5f);
    PersonalizationPreferences.Add(TEXT("Economic"), 0.5f);
    PersonalizationPreferences.Add(TEXT("Military"), 0.5f);
    PersonalizationPreferences.Add(TEXT("Cultural"), 0.5f);
    PersonalizationPreferences.Add(TEXT("Personal"), 0.5f);
}

void UMingIntelligentRecommendationSystem::UpdateSystemState(float DeltaTime)
{
    if (!bSystemActive)
    {
        return;
    }
    
    LastUpdateTime += DeltaTime;
    
    // 每隔一定時間更新一次
    if (LastUpdateTime >= Config.UpdateInterval)
    {
        CleanupExpiredRecommendations();
        UpdateStatistics();
        LastUpdateTime = 0.0f;
    }
}

void UMingIntelligentRecommendationSystem::CleanupExpiredRecommendations()
{
    FDateTime CurrentTime = FDateTime::Now();
    TArray<int32> ExpiredIndices;
    
    for (int32 i = 0; i < ActiveRecommendations.Num(); ++i)
    {
        const FRecommendationData& Recommendation = ActiveRecommendations[i];
        if (Recommendation.ExpiryTime <= CurrentTime)
        {
            Recommendation.Status = ERecommendationStatus::Expired;
            ExpiredIndices.Add(i);
            
            // 廣播事件
            OnRecommendationStatusChanged.Broadcast(Recommendation.RecommendationID, ERecommendationStatus::Expired);
        }
    }
    
    // 移除過期建議
    for (int32 i = ExpiredIndices.Num() - 1; i >= 0; --i)
    {
        ActiveRecommendations.RemoveAt(ExpiredIndices[i]);
    }
}

void UMingIntelligentRecommendationSystem::UpdateStatistics()
{
    Statistics.TotalRecommendations = RecommendationHistory.Num();
    Statistics.LastUpdateTime = FDateTime::Now();
    
    // 計算成功率
    if (Statistics.TotalRecommendations > 0)
    {
        Statistics.SuccessRate = (static_cast<float>(Statistics.CompletedRecommendations) / Statistics.TotalRecommendations) * 100.0f;
    }
    
    // 計算平均玩家評分
    int32 RatedCount = 0;
    int32 TotalRating = 0;
    for (const FRecommendationData& Recommendation : RecommendationHistory)
    {
        if (Recommendation.PlayerRating > 0)
        {
            TotalRating += Recommendation.PlayerRating;
            RatedCount++;
        }
    }
    
    if (RatedCount > 0)
    {
        Statistics.AveragePlayerRating = static_cast<float>(TotalRating) / RatedCount;
    }
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::PerformAIAnalysis(const FString& Context)
{
    TArray<FRecommendationData> Recommendations;
    
    // 基於AI分析的建議生成
    // 這裡會有實際的AI分析邏輯
    
    FRecommendationData AIRecommendation = GenerateSpecificRecommendation(ERecommendationType::Strategic, Context);
    AIRecommendation.Source = ERecommendationSource::AIAnalysis;
    AIRecommendation.DetailedContent = TEXT("基於深度AI分析，當前戰略局勢建議採取防禦姿態，同時尋找反擊機會。");
    AIRecommendation.Reason = TEXT("AI分析顯示敵方正在集結力量，防禦是當前最佳選擇。");
    
    Recommendations.Add(AIRecommendation);
    
    return Recommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::AnalyzePlayerBehavior(const FString& Context)
{
    TArray<FRecommendationData> Recommendations;
    
    // 分析玩家行為模式
    UpdatePlayerBehaviorPattern("Attack", 5);
    UpdatePlayerBehaviorPattern("Defend", 3);
    UpdatePlayerBehaviorPattern("Trade", 2);
    
    FRecommendationData BehaviorRecommendation = GenerateSpecificRecommendation(ERecommendationType::Personal, Context);
    BehaviorRecommendation.Source = ERecommendationSource::PlayerBehavior;
    BehaviorRecommendation.DetailedContent = TEXT("基於您的行為模式分析，建議平衡攻防策略，避免過度激進。");
    BehaviorRecommendation.Reason = TEXT("分析顯示您傾向於激進進攻，但當前局勢需要更謹慎的approach。");
    
    Recommendations.Add(BehaviorRecommendation);
    
    return Recommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::AnalyzeGameContext(const FString& Context)
{
    TArray<FRecommendationData> Recommendations;
    
    // 分析當前遊戲情境
    FString CurrentContext = GetCurrentGameContext();
    
    FRecommendationData ContextRecommendation = GenerateSpecificRecommendation(ERecommendationType::Tactical, Context);
    ContextRecommendation.Source = ERecommendationSource::GameContext;
    ContextRecommendation.DetailedContent = TEXT("基於當前遊戲情境，建議優先控制資源點，建立防線。");
    ContextRecommendation.Reason = FString::Printf(TEXT("當前情境: %s，需要相應的戰術調整。"), *CurrentContext);
    
    Recommendations.Add(ContextRecommendation);
    
    return Recommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::AnalyzeHistoricalData(const FString& Context)
{
    TArray<FRecommendationData> Recommendations;
    
    // 分析歷史數據
    FRecommendationData HistoricalRecommendation = GenerateSpecificRecommendation(ERecommendationType::LongTerm, Context);
    HistoricalRecommendation.Source = ERecommendationSource::HistoricalData;
    HistoricalRecommendation.DetailedContent = TEXT("基於歷史數據分析，長期發展應該專注於經濟建設和科技研發。");
    HistoricalRecommendation.Reason = TEXT("歷史數據顯示，穩定的經濟基礎是長期成功的關鍵。");
    
    Recommendations.Add(HistoricalRecommendation);
    
    return Recommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::PredictRecommendations(const FString& Context)
{
    TArray<FRecommendationData> Recommendations;
    
    // 機器學習預測
    FRecommendationData MLRecommendation = GenerateSpecificRecommendation(ERecommendationType::Strategic, Context);
    MLRecommendation.Source = ERecommendationSource::MachineLearning;
    MLRecommendation.DetailedContent = TEXT("基於機器學習模型預測，建議提前準備應對即將到來的挑戰。");
    MLRecommendation.Reason = TEXT("ML模型預測未來30分鐘內可能出現重要戰略機會。");
    
    Recommendations.Add(MLRecommendation);
    
    return Recommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::GeneratePersonalizedRecommendations(const FString& Context)
{
    TArray<FRecommendationData> Recommendations;
    
    // 個人化推薦
    FRecommendationData PersonalizedRecommendation = GenerateSpecificRecommendation(ERecommendationType::Personal, Context);
    PersonalizedRecommendation.Source = ERecommendationSource::Personalized;
    PersonalizedRecommendation.DetailedContent = TEXT("基於您的個人偏好和遊戲風格，為您量身定制的建議。");
    PersonalizedRecommendation.Reason = TEXT("根據您的歷史選擇和成功模式，這是最適合您的策略。");
    
    Recommendations.Add(PersonalizedRecommendation);
    
    return Recommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::AnalyzeCommunityData(const FString& Context)
{
    TArray<FRecommendationData> Recommendations;
    
    // 社群數據分析
    FRecommendationData CommunityRecommendation = GenerateSpecificRecommendation(ERecommendationType::Strategic, Context);
    CommunityRecommendation.Source = ERecommendationSource::CommunityData;
    CommunityRecommendation.DetailedContent = TEXT("基於社群數據分析，這是在類似情況下最受歡迎的策略選擇。");
    CommunityRecommendation.Reason = TEXT("社群數據顯示，85%的玩家在類似情況下選擇了此策略。");
    
    Recommendations.Add(CommunityRecommendation);
    
    return Recommendations;
}

TArray<FRecommendationData> UMingIntelligentRecommendationSystem::MergeAndDeduplicateRecommendations(const TArray<TArray<FRecommendationData>>& RecommendationSets)
{
    TArray<FRecommendationData> MergedRecommendations;
    TSet<FString> SeenTitles;
    
    for (const TArray<FRecommendationData>& Set : RecommendationSets)
    {
        for (const FRecommendationData& Recommendation : Set)
        {
            // 基於標題去重
            if (!SeenTitles.Contains(Recommendation.Title))
            {
                MergedRecommendations.Add(Recommendation);
                SeenTitles.Add(Recommendation.Title);
            }
        }
    }
    
    return MergedRecommendations;
}

bool UMingIntelligentRecommendationSystem::ValidateRecommendation(const FRecommendationData& Recommendation) const
{
    // 檢查基本驗證
    if (Recommendation.Title.IsEmpty() || Recommendation.Description.IsEmpty())
    {
        return false;
    }
    
    // 檢查可信度閾值
    if (Recommendation.ConfidenceScore < Config.MinConfidenceThreshold)
    {
        return false;
    }
    
    // 檢查相關性閾值
    if (Recommendation.RelevanceScore < Config.MinRelevanceThreshold)
    {
        return false;
    }
    
    // 檢查條件
    if (!CheckRecommendationConditions(Recommendation))
    {
        return false;
    }
    
    return true;
}

FString UMingIntelligentRecommendationSystem::FormatRecommendation(const FRecommendationData& Recommendation) const
{
    FString FormattedRecommendation = FString::Printf(TEXT("[%s] %s\n\n%s\n\n影響分數: %.1f\n可信度: %.1f%%\n\n原因: %s"), 
        *UEnum::GetValueAsString(Recommendation.Priority),
        *Recommendation.Title,
        *Recommendation.Description,
        Recommendation.ImpactScore,
        Recommendation.ConfidenceScore * 100.0f,
        *Recommendation.Reason);
    
    if (!Recommendation.DetailedContent.IsEmpty())
    {
        FormattedRecommendation += FString::Printf(TEXT("\n\n詳細內容:\n%s"), *Recommendation.DetailedContent);
    }
    
    return FormattedRecommendation;
}

FString UMingIntelligentRecommendationSystem::GenerateUniqueID() const
{
    return FString::Printf(TEXT("REC_%s_%d"), *FDateTime::Now().ToString(TEXT("%Y%m%d%H%M%S")), ++RecommendationIDCounter);
}

float UMingIntelligentRecommendationSystem::CalculateImpactScore(const FRecommendationData& Recommendation) const
{
    // 基於建議類型和優先級計算影響分數
    float BaseScore = 50.0f;
    
    // 類型加成
    switch (Recommendation.RecommendationType)
    {
    case ERecommendationType::Emergency:
        BaseScore += 30.0f;
        break;
    case ERecommendationType::Strategic:
        BaseScore += 20.0f;
        break;
    case ERecommendationType::Military:
        BaseScore += 15.0f;
        break;
    case ERecommendationType::Diplomatic:
        BaseScore += 10.0f;
        break;
    default:
        break;
    }
    
    // 優先級加成
    switch (Recommendation.Priority)
    {
    case ERecommendationPriority::Critical:
        BaseScore += 25.0f;
        break;
    case ERecommendationPriority::High:
        BaseScore += 15.0f;
        break;
    case ERecommendationPriority::Medium:
        BaseScore += 5.0f;
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseScore, 0.0f, 100.0f);
}

float UMingIntelligentRecommendationSystem::CalculateConfidenceScore(const FRecommendationData& Recommendation) const
{
    // 基於來源和歷史成功率計算可信度
    float BaseConfidence = 0.5f;
    
    // 來源可信度
    switch (Recommendation.Source)
    {
    case ERecommendationSource::AIAnalysis:
        BaseConfidence += 0.3f;
        break;
    case ERecommendationSource::MachineLearning:
        BaseConfidence += 0.25f;
        break;
    case ERecommendationSource::PlayerBehavior:
        BaseConfidence += 0.2f;
        break;
    case ERecommendationSource::GameContext:
        BaseConfidence += 0.15f;
        break;
    case ERecommendationSource::HistoricalData:
        BaseConfidence += 0.1f;
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseConfidence, 0.0f, 1.0f);
}

float UMingIntelligentRecommendationSystem::CalculateRelevanceScore(const FRecommendationData& Recommendation, const FString& Context) const
{
    // 基於當前情境計算相關性
    float BaseRelevance = 0.5f;
    
    // 這裡會有實際的相關性計算邏輯
    // 基於關鍵詞匹配、情境相似度等
    
    return FMath::Clamp(BaseRelevance, 0.0f, 1.0f);
}

float UMingIntelligentRecommendationSystem::CalculateDifficultyScore(const FRecommendationData& Recommendation) const
{
    // 基於建議複雜度計算執行難度
    float BaseDifficulty = 0.5f;
    
    // 基於建議類型調整難度
    switch (Recommendation.RecommendationType)
    {
    case ERecommendationType::Strategic:
        BaseDifficulty += 0.3f;
        break;
    case ERecommendationType::Military:
        BaseDifficulty += 0.2f;
        break;
    case ERecommendationType::Diplomatic:
        BaseDifficulty += 0.15f;
        break;
    case ERecommendationType::Emergency:
        BaseDifficulty -= 0.1f; // 緊急建議通常更直接
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseDifficulty, 0.0f, 1.0f);
}

void UMingIntelligentRecommendationSystem::UpdatePlayerBehaviorPattern(const FString& Action, int32 Frequency)
{
    PlayerBehaviorPatterns.FindOrAdd(Action, 0) += Frequency;
}

FString UMingIntelligentRecommendationSystem::GetCurrentGameContext() const
{
    // 獲取當前遊戲情境
    // 這裡會有實際的情境檢測邏輯
    return TEXT("MidGame_Conflict");
}

FString UMingIntelligentRecommendationSystem::GetCurrentPlayerState() const
{
    // 獲取玩家當前狀態
    // 這裡會有實際的狀態檢測邏輯
    return TEXT("Active_Strategic");
}

bool UMingIntelligentRecommendationSystem::CheckRecommendationConditions(const FRecommendationData& Recommendation) const
{
    // 檢查建議的先決條件
    for (const FString& Condition : Recommendation.Conditions)
    {
        // 這裡會有實際的條件檢查邏輯
        // 例如：檢查資源是否足夠、單位是否可用等
    }
    
    return true;
}

void UMingIntelligentRecommendationSystem::ApplyRecommendationConsequences(const FRecommendationData& Recommendation)
{
    // 應用建議的後果
    for (const FString& Consequence : Recommendation.Consequences)
    {
        // 這裡會有實際的後果應用邏輯
        // 例如：修改遊戲狀態、觸發事件等
    }
}
