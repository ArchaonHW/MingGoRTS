#include "MingAIUIManager.h"
#include "MingPersonalManager.h"
#include "MingCoreEventBus.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"

UMingAIUIManager::UMingAIUIManager()
    : UIManager(nullptr)
    , RelationshipManager(nullptr)
    , bAIEnabled(true)
    , AdaptationSensitivity(0.7f)
    , LearningRate(0.1f)
    , MinDataPointsForAnalysis(10)
{
    InitializeDefaultAdaptations();
}

void UMingAIUIManager::InitializeAIUIManager(UMingPersonalUIManager* InUIManager, 
                                            UMingRelationshipManager* InRelationshipManager)
{
    UIManager = InUIManager;
    RelationshipManager = InRelationshipManager;

    if (UIManager && RelationshipManager)
    {
        LoadProfileData();
        SetupEventBindings();
        
        // 開始行為追蹤
        TrackUserBehavior(TEXT("SessionStart"), TEXT("AIUIManager"), 1.0f);
        
        UE_LOG(LogTemp, Log, TEXT("AI UI Manager initialized successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to initialize AI UI Manager - missing dependencies"));
    }
}

void UMingAIUIManager::TrackUserBehavior(const FString& BehaviorType, const FString& Context, float Value)
{
    if (!bAIEnabled)
    {
        return;
    }

    // 記錄行為
    FString BehaviorEntry = FString::Printf(TEXT("%s|%s|%s|%.2f|%s"), 
        *FDateTime::Now().ToString(), 
        *BehaviorType, 
        *Context, 
        Value, 
        *FDateTime::Now().ToIso8601());
    
    BehaviorHistory.Add(BehaviorEntry);
    BehaviorCounts.FindOrAdd(BehaviorType, 0)++;
    BehaviorCounts[BehaviorType]++;

    // 限制歷史記錄大小
    if (BehaviorHistory.Num() > 10000)
    {
        BehaviorHistory.RemoveAt(0, BehaviorHistory.Num() - 10000);
    }

    // 定期分析
    if (BehaviorHistory.Num() % MinDataPointsForAnalysis == 0)
    {
        AnalyzeUserBehavior();
    }

    OnAIBehaviorAnalyzed.Broadcast(BehaviorType, Value);
}

void UMingAIUIManager::TrackPanelUsage(EPersonalUIType PanelType, float Duration)
{
    if (!bAIEnabled)
    {
        return;
    }

    PanelUsageTimes.FindOrAdd(PanelType, 0.0f) += Duration;
    
    // 更新用戶檔案中的常用面板
    if (!CurrentProfile.FrequentlyUsedPanels.Contains(PanelType))
    {
        CurrentProfile.FrequentlyUsedPanels.Add(PanelType);
    }

    TrackUserBehavior(TEXT("PanelUsage"), *StaticEnum<EPersonalUIType>()->GetValueAsString(PanelType), Duration);
}

void UMingAIUIManager::TrackInteractionPattern(const FString& InteractionType, const FString& Target)
{
    if (!bAIEnabled)
    {
        return;
    }

    FString PatternKey = FString::Printf(TEXT("%s_%s"), *InteractionType, *Target);
    InteractionPatterns.FindOrAdd(PatternKey, 0.0f) += 1.0f;

    TrackUserBehavior(TEXT("InteractionPattern"), PatternKey, 1.0f);
}

void UMingAIUIManager::TrackSessionMetrics(float SessionTime, int32 ActionsPerformed)
{
    if (!bAIEnabled)
    {
        return;
    }

    CurrentProfile.PlayTimeHours += SessionTime / 3600.0f;
    
    TrackUserBehavior(TEXT("SessionMetrics"), FString::Printf(TEXT("Time_%.2f_Actions_%d"), SessionTime, ActionsPerformed), ActionsPerformed);

    // 分析會話模式
    float ActionsPerMinute = ActionsPerformed / (SessionTime / 60.0f);
    if (ActionsPerMinute > 60.0f)
    {
        CurrentProfile.AdaptationPreferences.FindOrAdd(EUIAdaptationType::AnimationSpeed, 0.0f) += 0.1f;
    }
    else if (ActionsPerMinute < 20.0f)
    {
        CurrentProfile.AdaptationPreferences.FindOrAdd(EUIAdaptationType::AnimationSpeed, 0.0f) -= 0.1f;
    }
}

void UMingAIUIManager::AnalyzeUserBehavior()
{
    if (!bAIEnabled || BehaviorHistory.Num() < MinDataPointsForAnalysis)
    {
        return;
    }

    ProcessBehaviorData();
    CalculateProfileScores();
    UpdateUserProfile();
    GenerateAdaptations();

    OnAIProfileUpdated.Broadcast(CurrentProfile);
}

void UMingAIUIManager::UpdateUserProfile()
{
    // 計算各種分數
    CalculateProfileScores();
    
    // 確定用戶類型
    EAIUserProfile NewProfile = DetermineUserProfile();
    
    if (NewProfile != CurrentProfile.ProfileType)
    {
        CurrentProfile.ProfileType = NewProfile;
        
        // 應用新的適應配置
        OptimizeLayoutForProfile();
        
        UE_LOG(LogTemp, Log, TEXT("User profile updated to: %s"), 
            *StaticEnum<EAIUserProfile>()->GetValueAsString(NewProfile));
    }

    SaveProfileData();
}

EAIUserProfile UMingAIUIManager::DetermineUserProfile()
{
    // 基於行為數據分類玩家
    float SocialScore = CalculateBehaviorScore(TEXT("SocialInteraction"));
    float CombatScore = CalculateBehaviorScore(TEXT("Combat"));
    float ExplorationScore = CalculateBehaviorScore(TEXT("Exploration"));
    float QuestScore = CalculateBehaviorScore(TEXT("QuestCompletion"));
    float RelationshipScore = CalculateBehaviorScore(TEXT("RelationshipBuilding"));

    // 權重決策
    if (SocialScore > 0.7f && RelationshipScore > 0.6f)
    {
        return EAIUserProfile::SocialPlayer;
    }
    else if (CombatScore > 0.8f && CurrentProfile.CombatWinRate > 0.7f)
    {
        return EAIUserProfile::PowerPlayer;
    }
    else if (ExplorationScore > 0.7f)
    {
        return EAIUserProfile::Explorer;
    }
    else if (QuestScore > 0.8f)
    {
        return EAIUserProfile::AchievementHunter;
    }
    else if (CurrentProfile.PlayTimeHours < 5.0f)
    {
        return EAIUserProfile::NewPlayer;
    }
    else if (CombatScore < 0.3f && SocialScore < 0.3f)
    {
        return EAIUserProfile::CasualPlayer;
    }
    else
    {
        return EAIUserProfile::StrategicPlayer;
    }
}

TArray<FAIUIAdaptationConfig> UMingAIUIManager::GenerateAdaptationRecommendations()
{
    RecommendedAdaptations.Empty();

    if (!bAIEnabled)
    {
        return RecommendedAdaptations;
    }

    // 分析面板使用模式
    for (const auto& PanelUsage : PanelUsageTimes)
    {
        if (PanelUsage.Value > 300.0f) // 5分鐘以上
        {
            FAIUIAdaptationConfig Adaptation;
            Adaptation.AdaptationType = EUIAdaptationType::Layout;
            Adaptation.AdaptationValue = 1.0f; // 提高優先級
            Adaptation.bIsEnabled = true;
            Adaptation.ConfidenceScore = FMath::Clamp(PanelUsage.Value / 600.0f, 0.0f, 1.0f);
            Adaptation.Reason = FString::Printf(TEXT("Frequent use of %s panel"), 
                *StaticEnum<EPersonalUIType>()->GetValueAsString(PanelUsage.Key));
            
            RecommendedAdaptations.Add(Adaptation);
        }
    }

    // 分析交互速度
    float AvgActionTime = CalculateBehaviorScore(TEXT("ActionSpeed"));
    if (AvgActionTime < 2.0f) // 快速交互
    {
        FAIUIAdaptationConfig SpeedAdaptation;
        SpeedAdaptation.AdaptationType = EUIAdaptationType::AnimationSpeed;
        SpeedAdaptation.AdaptationValue = 1.5f; // 加快動畫
        SpeedAdaptation.bIsEnabled = true;
        SpeedAdaptation.ConfidenceScore = 0.8f;
        SpeedAdaptation.Reason = TEXT("User interacts quickly with UI");
        
        RecommendedAdaptations.Add(SpeedAdaptation);
    }

    // 分析信息密度偏好
    float InfoDensityScore = CalculateBehaviorScore(TEXT("InformationDensity"));
    if (InfoDensityScore > 0.7f)
    {
        FAIUIAdaptationConfig DensityAdaptation;
        DensityAdaptation.AdaptationType = EUIAdaptationType::InformationDensity;
        DensityAdaptation.AdaptationValue = 1.2f; // 增加信息密度
        DensityAdaptation.bIsEnabled = true;
        DensityAdaptation.ConfidenceScore = 0.7f;
        DensityAdaptation.Reason = TEXT("User prefers detailed information");
        
        RecommendedAdaptations.Add(DensityAdaptation);
    }

    return RecommendedAdaptations;
}

void UMingAIUIManager::ApplyAdaptation(const FAIUIAdaptationConfig& Adaptation)
{
    if (!bAIEnabled || !UIManager)
    {
        return;
    }

    ApplyAdaptationToUI(Adaptation);
    
    // 記錄到活動適應列表
    ActiveAdaptations.Add(Adaptation);
    
    OnUIAdaptationApplied.Broadcast(Adaptation);
    LogAdaptation(Adaptation);
}

void UMingAIUIManager::ApplyAllRecommendedAdaptations()
{
    for (const auto& Adaptation : RecommendedAdaptations)
    {
        if (Adaptation.bIsEnabled && Adaptation.ConfidenceScore > AdaptationSensitivity)
        {
            ApplyAdaptation(Adaptation);
        }
    }
}

void UMingAIUIManager::OptimizeLayoutForProfile()
{
    if (!UIManager)
    {
        return;
    }

    switch (CurrentProfile.ProfileType)
    {
    case EAIUserProfile::NewPlayer:
        // 新玩家：簡化界面，增加提示
        ShowContextualHelp(TEXT("Welcome! Here are the basics..."));
        HighlightImportantFeatures();
        break;
        
    case EAIUserProfile::PowerPlayer:
        // 重度玩家：最大化信息密度
        AdjustInformationDensity();
        SetAnimationSpeed(); // 加快動畫
        break;
        
    case EAIUserProfile::SocialPlayer:
        // 社交玩家：強化關係面板
        UIManager->OpenUIPanel(EPersonalUIType::RelationshipPanel);
        SuggestNextAction();
        break;
        
    case EAIUserProfile::Explorer:
        // 探索者：強化地區和任務信息
        UIManager->OpenUIPanel(EPersonalUIType::QuestPanel);
        break;
        
    default:
        // 默認適應
        ApplyAllRecommendedAdaptations();
        break;
    }
}

void UMingAIUIManager::ShowContextualHelp(const FString& Context)
{
    if (!UIManager)
    {
        return;
    }

    // 根據用戶檔案調整幫助內容
    FString HelpMessage = Context;
    
    if (CurrentProfile.ProfileType == EAIUserProfile::NewPlayer)
    {
        HelpMessage += TEXT("\n\nTip: Use the relationship panel to track your connections with other characters.");
    }
    else if (CurrentProfile.ProfileType == EAIUserProfile::StrategicPlayer)
    {
        HelpMessage += TEXT("\n\nTip: Monitor your reputation across different regions for optimal quest access.");
    }

    UIManager->ShowNotification(HelpMessage, 5.0f);
}

void UMingAIUIManager::SuggestNextAction()
{
    if (!RelationshipManager || !UIManager)
    {
        return;
    }

    // 基於當前狀態建議下一步行動
    TArray<FString> Suggestions;
    
    // 分析關係狀態
    if (CurrentProfile.ProfileType == EAIUserProfile::SocialPlayer)
    {
        // 找出關係值較低但重要的角色
        for (const auto& Relationship : RelationshipManager->GetAllRelationships())
        {
            if (Relationship.Value.RelationshipValue < 30.0f && Relationship.Value.RelationshipType != ERelationshipType::Enemy)
            {
                Suggestions.Add(FString::Printf(TEXT("Improve relationship with %s"), *Relationship.Key));
            }
        }
    }
    
    if (Suggestions.Num() > 0)
    {
        FString SuggestionText = FString::Join(TEXT("\n"), Suggestions);
        UIManager->ShowNotification(SuggestionText, 8.0f);
    }
}

TArray<FString> UMingAIUIManager::GetRecommendedQuests()
{
    TArray<FString> RecommendedQuests;
    
    // 基於用戶檔案推薦任務
    if (CurrentProfile.ProfileType == EAIUserProfile::AchievementHunter)
    {
        // 推薦高難度任務
        RecommendedQuests.Add(TEXT("Dragon_Slayer_Quest"));
        RecommendedQuests.Add(TEXT("Master_Strategist_Challenge"));
    }
    else if (CurrentProfile.ProfileType == EAIUserProfile::SocialPlayer)
    {
        // 推薦社交任務
        RecommendedQuests.Add(TEXT("Diplomatic_Mission"));
        RecommendedQuests.Add(TEXT("Alliance_Building"));
    }
    
    return RecommendedQuests;
}

void UMingAIUIManager::ProcessBehaviorData()
{
    // 處理行為數據，計算趨勢和模式
    for (const auto& BehaviorCount : BehaviorCounts)
    {
        TArray<float> Trend = GetBehaviorTrend(BehaviorCount.Key, 5);
        if (Trend.Num() >= 3)
        {
            // 計算趨勢斜率
            float Slope = (Trend.Last() - Trend[0]) / Trend.Num();
            if (FMath::Abs(Slope) > 0.1f)
            {
                // 記錄顯著行為變化
                OnAIBehaviorAnalyzed.Broadcast(BehaviorCount.Key + TEXT("_Trend"), FMath::Abs(Slope));
            }
        }
    }
}

void UMingAIUIManager::CalculateProfileScores()
{
    // 計算社交分數
    float SocialInteractions = CalculateBehaviorScore(TEXT("SocialInteraction"));
    CurrentProfile.SocialInteractionScore = FMath::Clamp(SocialInteractions, 0.0f, 1.0f);
    
    // 計算探索分數
    float ExplorationActions = CalculateBehaviorScore(TEXT("Exploration"));
    CurrentProfile.ExplorationScore = FMath::Clamp(ExplorationActions, 0.0f, 1.0f);
    
    // 更新關係數量
    if (RelationshipManager)
    {
        CurrentProfile.RelationshipsFormed = RelationshipManager->GetAllRelationships().Num();
    }
    
    // 計算戰鬥勝率（這裡需要從遊戲系統獲取實際數據）
    // CurrentProfile.CombatWinRate = GetCombatWinRateFromGameSystem();
}

float UMingAIUIManager::CalculateBehaviorScore(const FString& BehaviorType) const
{
    const int32* Count = BehaviorCounts.Find(BehaviorType);
    if (!Count)
    {
        return 0.0f;
    }
    
    // 正規化到0-1範圍
    return FMath::Clamp(static_cast<float>(*Count) / static_cast<float>(BehaviorHistory.Num()), 0.0f, 1.0f);
}

void UMingAIUIManager::ApplyAdaptationToUI(const FAIUIAdaptationConfig& Adaptation)
{
    if (!UIManager)
    {
        return;
    }

    switch (Adaptation.AdaptationType)
    {
    case EUIAdaptationType::AnimationSpeed:
        UIManager->SetAnimationSpeed(Adaptation.AdaptationValue);
        break;
        
    case EUIAdaptationType::Layout:
        // 重新排列面板順序
        break;
        
    case EUIAdaptationType::ColorScheme:
        // 調整配色方案
        break;
        
    case EUIAdaptationType::InformationDensity:
        // 調整信息密度
        break;
        
    default:
        break;
    }
}

void UMingAIUIManager::InitializeDefaultAdaptations()
{
    // 初始化默認適應配置
    FAIUIAdaptationConfig DefaultAdaptation;
    DefaultAdaptation.AdaptationType = EUIAdaptationType::AnimationSpeed;
    DefaultAdaptation.AdaptationValue = 1.0f;
    DefaultAdaptation.bIsEnabled = true;
    DefaultAdaptation.ConfidenceScore = 0.5f;
    DefaultAdaptation.Reason = TEXT("Default animation speed");
    
    RecommendedAdaptations.Add(DefaultAdaptation);
}

void UMingAIUIManager::SaveProfileData()
{
    // 保存用戶檔案到文件
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("AIUserProfile.json");
    // 實現JSON序列化保存邏輯
}

void UMingAIUIManager::LoadProfileData()
{
    // 從文件加載用戶檔案
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("AIUserProfile.json");
    // 實現JSON反序列化加載邏輯
}

void UMingAIUIManager::LogAdaptation(const FAIUIAdaptationConfig& Adaptation)
{
    FString LogMessage = FString::Printf(TEXT("AI Adaptation Applied: %s = %.2f (Confidence: %.2f) - %s"),
        *StaticEnum<EUIAdaptationType>()->GetValueAsString(Adaptation.AdaptationType),
        Adaptation.AdaptationValue,
        Adaptation.ConfidenceScore,
        *Adaptation.Reason);
    
    UE_LOG(LogTemp, Log, TEXT("%s"), *LogMessage);
}
