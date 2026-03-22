#include "AI/MingRTSAIDynamicDifficulty.h"
#include "Math/UnrealMathUtility.h"

UMingRTSAIDynamicDifficulty::UMingRTSAIDynamicDifficulty()
    : bIsInitialized(false)
    , CurrentGameTime(0.0f)
{
}

void UMingRTSAIDynamicDifficulty::InitializeDifficultySystem(const FDifficultyConfiguration& Config)
{
    if (bIsInitialized)
    {
        return;
    }
    
    Configuration = Config;
    CurrentGameTime = 0.0f;
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Initialized with base difficulty %d"),
        static_cast<int32>(Configuration.BaseDifficulty));
}

void UMingRTSAIDynamicDifficulty::ShutdownDifficultySystem()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    PlayerSkills.Empty();
    CurrentDifficultyMultipliers.Empty();
    DifficultyHistories.Empty();
    ActiveSessions.Empty();
    SessionHistories.Empty();
    AdaptiveEnabled.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Shutdown"));
}

void UMingRTSAIDynamicDifficulty::RegisterPlayer(const FString& PlayerID)
{
    if (!PlayerSkills.Contains(PlayerID))
    {
        FPlayerSkillData NewSkillData;
        NewSkillData.PlayerID = PlayerID;
        PlayerSkills.Add(PlayerID, NewSkillData);
        
        CurrentDifficultyMultipliers.Add(PlayerID, GetDifficultyForLevel(Configuration.BaseDifficulty));
        AdaptiveEnabled.Add(PlayerID, Configuration.bEnableDynamicAdjustment);
        
        UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Registered player %s"), *PlayerID);
    }
}

void UMingRTSAIDynamicDifficulty::UnregisterPlayer(const FString& PlayerID)
{
    PlayerSkills.Remove(PlayerID);
    CurrentDifficultyMultipliers.Remove(PlayerID);
    DifficultyHistories.Remove(PlayerID);
    SessionHistories.Remove(PlayerID);
    AdaptiveEnabled.Remove(PlayerID);
    ActiveSessions.Remove(PlayerID);
}

void UMingRTSAIDynamicDifficulty::UpdatePlayerSkill(const FString& PlayerID, ESkillMetric Metric, float Score)
{
    if (!PlayerSkills.Contains(PlayerID))
    {
        RegisterPlayer(PlayerID);
    }
    
    FPlayerSkillData& SkillData = PlayerSkills[PlayerID];
    SkillData.SkillScores.Add(Metric, FMath::Clamp(Score, 0.0f, 1.0f));
    
    // Update overall rating
    UpdatePlayerSkillRating(PlayerID);
    
    OnSkillAssessmentUpdated.Broadcast(PlayerID, SkillData);
}

FPlayerSkillData UMingRTSAIDynamicDifficulty::GetPlayerSkillData(const FString& PlayerID) const
{
    if (PlayerSkills.Contains(PlayerID))
    {
        return PlayerSkills[PlayerID];
    }
    return FPlayerSkillData();
}

float UMingRTSAIDynamicDifficulty::GetPlayerSkillRating(const FString& PlayerID) const
{
    if (PlayerSkills.Contains(PlayerID))
    {
        return PlayerSkills[PlayerID].OverallSkillRating;
    }
    return 0.5f;
}

void UMingRTSAIDynamicDifficulty::AdjustDifficulty(const FString& PlayerID, float PerformanceScore)
{
    if (!bIsInitialized || !CurrentDifficultyMultipliers.Contains(PlayerID))
    {
        return;
    }
    
    if (!AdaptiveEnabled[PlayerID])
    {
        return;
    }
    
    float CurrentMultiplier = CurrentDifficultyMultipliers[PlayerID];
    float TargetMultiplier = CurrentMultiplier;
    
    // 根據性能調整難度
    if (PerformanceScore > 0.8f)
    {
        // 表現太好，增加難度
        TargetMultiplier += Configuration.AdjustmentSensitivity * 0.1f;
    }
    else if (PerformanceScore < 0.3f)
    {
        // 表現太差，降低難度
        TargetMultiplier -= Configuration.AdjustmentSensitivity * 0.15f;
    }
    else if (PerformanceScore > 0.6f)
    {
        // 表現良好，小幅增加
        TargetMultiplier += Configuration.AdjustmentSensitivity * 0.05f;
    }
    else if (PerformanceScore < 0.5f)
    {
        // 表現一般，小幅降低
        TargetMultiplier -= Configuration.AdjustmentSensitivity * 0.05f;
    }
    
    // 防止突變
    if (Configuration.bPreventSuddenSpikes)
    {
        float MaxChange = CurrentMultiplier * Configuration.SpikeProtectionThreshold;
        TargetMultiplier = FMath::Clamp(TargetMultiplier, CurrentMultiplier - MaxChange, CurrentMultiplier + MaxChange);
    }
    
    // 限制範圍
    TargetMultiplier = ClampDifficultyMultiplier(TargetMultiplier);
    
    if (FMath::Abs(TargetMultiplier - CurrentMultiplier) > 0.01f)
    {
        FString Reason = FString::Printf(TEXT("Performance score: %.2f"), PerformanceScore);
        LogDifficultyChange(PlayerID, CurrentMultiplier, TargetMultiplier, Reason);
        
        CurrentDifficultyMultipliers[PlayerID] = TargetMultiplier;
        
        // 記錄歷史
        FDifficultyHistoryEntry Entry;
        Entry.Timestamp = CurrentGameTime;
        Entry.DifficultyMultiplier = TargetMultiplier;
        Entry.TriggerReason = Reason;
        Entry.PlayerPerformance = PerformanceScore;
        
        if (!DifficultyHistories.Contains(PlayerID))
        {
            DifficultyHistories.Add(PlayerID, TArray<FDifficultyHistoryEntry>());
        }
        DifficultyHistories[PlayerID].Add(Entry);
        
        OnDifficultyAdjusted.Broadcast(PlayerID, TargetMultiplier, Reason);
    }
}

void UMingRTSAIDynamicDifficulty::SetBaseDifficulty(const FString& PlayerID, EDifficultyLevel NewDifficulty)
{
    if (!CurrentDifficultyMultipliers.Contains(PlayerID))
    {
        RegisterPlayer(PlayerID);
    }
    
    float NewMultiplier = GetDifficultyForLevel(NewDifficulty);
    CurrentDifficultyMultipliers[PlayerID] = NewMultiplier;
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Set player %s base difficulty to %d (%.2f)"),
        *PlayerID, static_cast<int32>(NewDifficulty), NewMultiplier);
}

float UMingRTSAIDynamicDifficulty::GetCurrentDifficultyMultiplier(const FString& PlayerID) const
{
    if (CurrentDifficultyMultipliers.Contains(PlayerID))
    {
        return CurrentDifficultyMultipliers[PlayerID];
    }
    return GetDifficultyForLevel(Configuration.BaseDifficulty);
}

EDifficultyLevel UMingRTSAIDynamicDifficulty::GetRecommendedDifficulty(const FString& PlayerID) const
{
    float Rating = GetPlayerSkillRating(PlayerID);
    
    if (Rating < 0.2f) return EDifficultyLevel::VeryEasy;
    if (Rating < 0.35f) return EDifficultyLevel::Easy;
    if (Rating < 0.55f) return EDifficultyLevel::Normal;
    if (Rating < 0.75f) return EDifficultyLevel::Hard;
    if (Rating < 0.9f) return EDifficultyLevel::VeryHard;
    return EDifficultyLevel::Nightmare;
}

void UMingRTSAIDynamicDifficulty::ApplyDifficultyAdjustment(const FString& PlayerID, const FDifficultyAdjustmentParams& Params)
{
    if (!CurrentDifficultyMultipliers.Contains(PlayerID))
    {
        return;
    }
    
    float NewMultiplier = ClampDifficultyMultiplier(Params.EffectiveDifficultyMultiplier);
    float OldMultiplier = CurrentDifficultyMultipliers[PlayerID];
    
    CurrentDifficultyMultipliers[PlayerID] = NewMultiplier;
    
    LogDifficultyChange(PlayerID, OldMultiplier, NewMultiplier, Params.ReasonForAdjustment);
    
    OnDifficultyAdjusted.Broadcast(PlayerID, NewMultiplier, Params.ReasonForAdjustment);
}

void UMingRTSAIDynamicDifficulty::StartSession(const FString& PlayerID, const FString& SessionID)
{
    if (!PlayerSkills.Contains(PlayerID))
    {
        RegisterPlayer(PlayerID);
    }
    
    FSessionPerformanceData NewSession;
    NewSession.SessionID = SessionID;
    NewSession.PlayerID = PlayerID;
    NewSession.DifficultyAtStart = GetCurrentDifficultyMultiplier(PlayerID);
    
    ActiveSessions.Add(SessionID, NewSession);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Started session %s for player %s"), *SessionID, *PlayerID);
}

void UMingRTSAIDynamicDifficulty::EndSession(const FString& PlayerID, const FString& SessionID)
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return;
    }
    
    FSessionPerformanceData& Session = ActiveSessions[SessionID];
    Session.SessionDuration = CurrentGameTime - Session.SessionDuration;
    Session.DifficultyAtEnd = GetCurrentDifficultyMultiplier(PlayerID);
    
    // 評估性能
    float PerformanceScore = CalculatePerformanceScore(PlayerID, Session);
    Session.PerformanceScore = PerformanceScore;
    
    // 保存到歷史
    if (!SessionHistories.Contains(PlayerID))
    {
        SessionHistories.Add(PlayerID, TArray<FSessionPerformanceData>());
    }
    SessionHistories[PlayerID].Add(Session);
    
    // 更新玩家技能數據
    FPlayerSkillData& SkillData = PlayerSkills[PlayerID];
    SkillData.GamesPlayed++;
    SkillData.LastSessionPerformance = PerformanceScore;
    
    // 計算平均性能
    if (SessionHistories[PlayerID].Num() > 0)
    {
        float TotalPerformance = 0.0f;
        for (const auto& HistSession : SessionHistories[PlayerID])
        {
            TotalPerformance += HistSession.PerformanceScore;
        }
        SkillData.AverageSessionPerformance = TotalPerformance / SessionHistories[PlayerID].Num();
    }
    
    // 更新勝敗記錄
    if (PerformanceScore > 0.6f)
    {
        SkillData.Wins++;
        SkillData.CurrentWinStreak++;
        SkillData.BestWinStreak = FMath::Max(SkillData.BestWinStreak, SkillData.CurrentWinStreak);
    }
    else
    {
        SkillData.Losses++;
        SkillData.CurrentWinStreak = 0;
    }
    
    // 評估後調整難度
    EvaluateAndAdjustDifficulty(PlayerID);
    
    EDifficultyLevel RecommendedDifficulty = GetRecommendedDifficulty(PlayerID);
    OnPerformanceEvaluated.Broadcast(PlayerID, PerformanceScore, RecommendedDifficulty);
    
    // 移除活動會話
    ActiveSessions.Remove(SessionID);
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Ended session %s, performance: %.2f"), *SessionID, PerformanceScore);
}

void UMingRTSAIDynamicDifficulty::RecordChallengeResult(const FString& PlayerID, bool bSuccess, float CompletionTime)
{
    // 找到玩家的活動會話
    FString ActiveSessionID;
    for (const auto& Pair : ActiveSessions)
    {
        if (Pair.Value.PlayerID == PlayerID)
        {
            ActiveSessionID = Pair.Key;
            break;
        }
    }
    
    if (ActiveSessionID.IsEmpty())
    {
        return;
    }
    
    FSessionPerformanceData& Session = ActiveSessions[ActiveSessionID];
    
    if (bSuccess)
    {
        Session.ChallengesCompleted++;
    }
    else
    {
        Session.ChallengesFailed++;
    }
    
    // 更新平均完成時間
    float TotalTime = Session.AverageChallengeCompletionTime * (Session.ChallengesCompleted + Session.ChallengesFailed - 1);
    TotalTime += CompletionTime;
    int32 TotalChallenges = Session.ChallengesCompleted + Session.ChallengesFailed;
    Session.AverageChallengeCompletionTime = TotalTime / TotalChallenges;
}

void UMingRTSAIDynamicDifficulty::RecordCombatEngagement(const FString& PlayerID, bool bVictory, int32 FriendlyLosses, int32 EnemyLosses)
{
    FString ActiveSessionID;
    for (const auto& Pair : ActiveSessions)
    {
        if (Pair.Value.PlayerID == PlayerID)
        {
            ActiveSessionID = Pair.Key;
            break;
        }
    }
    
    if (ActiveSessionID.IsEmpty())
    {
        return;
    }
    
    FSessionPerformanceData& Session = ActiveSessions[ActiveSessionID];
    Session.UnitsLost += FriendlyLosses;
    Session.EnemyUnitsDestroyed += EnemyLosses;
}

void UMingRTSAIDynamicDifficulty::RecordResourceGathering(const FString& PlayerID, int32 Amount)
{
    FString ActiveSessionID;
    for (const auto& Pair : ActiveSessions)
    {
        if (Pair.Value.PlayerID == PlayerID)
        {
            ActiveSessionID = Pair.Key;
            break;
        }
    }
    
    if (!ActiveSessionID.IsEmpty())
    {
        ActiveSessions[ActiveSessionID].ResourcesGathered += Amount;
    }
}

float UMingRTSAIDynamicDifficulty::EvaluateSessionPerformance(const FString& PlayerID, const FString& SessionID)
{
    if (!ActiveSessions.Contains(SessionID))
    {
        return 0.5f;
    }
    
    const FSessionPerformanceData& Session = ActiveSessions[SessionID];
    return CalculatePerformanceScore(PlayerID, Session);
}

void UMingRTSAIDynamicDifficulty::EnableAdaptiveDifficulty(const FString& PlayerID)
{
    AdaptiveEnabled[PlayerID] = true;
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Enabled adaptive difficulty for player %s"), *PlayerID);
}

void UMingRTSAIDynamicDifficulty::DisableAdaptiveDifficulty(const FString& PlayerID)
{
    AdaptiveEnabled[PlayerID] = false;
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Disabled adaptive difficulty for player %s"), *PlayerID);
}

bool UMingRTSAIDynamicDifficulty::IsAdaptiveDifficultyEnabled(const FString& PlayerID) const
{
    if (AdaptiveEnabled.Contains(PlayerID))
    {
        return AdaptiveEnabled[PlayerID];
    }
    return Configuration.bEnableDynamicAdjustment;
}

FDifficultyAdjustmentParams UMingRTSAIDynamicDifficulty::CalculateOptimalDifficulty(const FString& PlayerID)
{
    FDifficultyAdjustmentParams Params;
    
    if (!PlayerSkills.Contains(PlayerID))
    {
        Params.EffectiveDifficultyMultiplier = 1.0f;
        return Params;
    }
    
    const FPlayerSkillData& SkillData = PlayerSkills[PlayerID];
    
    // 基於技能評級計算最佳難度
    float SkillBasedMultiplier = 0.5f + SkillData.OverallSkillRating;
    
    // 根據一致性調整
    if (SkillData.ConsistencyScore > 0.7f)
    {
        // 穩定玩家可以增加難度
        SkillBasedMultiplier += 0.2f;
    }
    else if (SkillData.ConsistencyScore < 0.3f)
    {
        // 不穩定玩家降低難度
        SkillBasedMultiplier -= 0.2f;
    }
    
    // 考慮改進率
    if (SkillData.ImprovementRate > 0.1f)
    {
        // 快速進步的玩家可以承受更高難度
        SkillBasedMultiplier += 0.1f;
    }
    
    Params.EffectiveDifficultyMultiplier = ClampDifficultyMultiplier(SkillBasedMultiplier);
    Params.ReasonForAdjustment = TEXT("Optimal difficulty based on skill analysis");
    Params.ConfidenceLevel = SkillData.ConsistencyScore;
    Params.Timestamp = CurrentGameTime;
    
    return Params;
}

TArray<FDifficultyHistoryEntry> UMingRTSAIDynamicDifficulty::GetDifficultyHistory(const FString& PlayerID, int32 Count) const
{
    TArray<FDifficultyHistoryEntry> Result;
    
    if (DifficultyHistories.Contains(PlayerID))
    {
        const TArray<FDifficultyHistoryEntry>& History = DifficultyHistories[PlayerID];
        
        int32 StartIndex = FMath::Max(0, History.Num() - Count);
        for (int32 i = StartIndex; i < History.Num(); ++i)
        {
            Result.Add(History[i]);
        }
    }
    
    return Result;
}

FSessionPerformanceData UMingRTSAIDynamicDifficulty::GetSessionPerformance(const FString& PlayerID, const FString& SessionID) const
{
    if (ActiveSessions.Contains(SessionID))
    {
        return ActiveSessions[SessionID];
    }
    
    if (SessionHistories.Contains(PlayerID))
    {
        for (const auto& Session : SessionHistories[PlayerID])
        {
            if (Session.SessionID == SessionID)
            {
                return Session;
            }
        }
    }
    
    return FSessionPerformanceData();
}

float UMingRTSAIDynamicDifficulty::GetAverageDifficultyTrend(const FString& PlayerID, int32 SessionCount) const
{
    if (!DifficultyHistories.Contains(PlayerID) || DifficultyHistories[PlayerID].Num() < 2)
    {
        return 0.0f;
    }
    
    const TArray<FDifficultyHistoryEntry>& History = DifficultyHistories[PlayerID];
    
    int32 Count = FMath::Min(SessionCount, History.Num());
    if (Count < 2)
    {
        return 0.0f;
    }
    
    float FirstValue = History[History.Num() - Count].DifficultyMultiplier;
    float LastValue = History.Last().DifficultyMultiplier;
    
    return (LastValue - FirstValue) / FirstValue;
}

float UMingRTSAIDynamicDifficulty::GetPlayerProgressRate(const FString& PlayerID) const
{
    if (!PlayerSkills.Contains(PlayerID))
    {
        return 0.0f;
    }
    
    return PlayerSkills[PlayerID].ImprovementRate;
}

void UMingRTSAIDynamicDifficulty::SetConfiguration(const FDifficultyConfiguration& NewConfig)
{
    Configuration = NewConfig;
}

FDifficultyConfiguration UMingRTSAIDynamicDifficulty::GetConfiguration() const
{
    return Configuration;
}

void UMingRTSAIDynamicDifficulty::ResetToDefaults()
{
    Configuration = FDifficultyConfiguration();
    
    // 重置所有玩家難度
    for (auto& Pair : CurrentDifficultyMultipliers)
    {
        Pair.Value = GetDifficultyForLevel(Configuration.BaseDifficulty);
    }
}

void UMingRTSAIDynamicDifficulty::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    CurrentGameTime += DeltaTime;
    
    ProcessSkillUpdates(DeltaTime);
}

void UMingRTSAIDynamicDifficulty::ProcessSkillUpdates(float DeltaTime)
{
    // 定期更新玩家技能評級
    static float LastUpdateTime = 0.0f;
    
    if (CurrentGameTime - LastUpdateTime > 30.0f) // 每30秒
    {
        for (const auto& Pair : PlayerSkills)
        {
            UpdatePlayerSkillRating(Pair.Key);
        }
        LastUpdateTime = CurrentGameTime;
    }
}

void UMingRTSAIDynamicDifficulty::EvaluateAndAdjustDifficulty(const FString& PlayerID)
{
    if (!Configuration.bEnableDynamicAdjustment || !AdaptiveEnabled[PlayerID])
    {
        return;
    }
    
    // 獲取最近會話性能
    if (!SessionHistories.Contains(PlayerID) || SessionHistories[PlayerID].Num() < Configuration.EvaluationWindow)
    {
        return;
    }
    
    const TArray<FSessionPerformanceData>& Sessions = SessionHistories[PlayerID];
    
    // 計算平均性能
    float TotalPerformance = 0.0f;
    int32 StartIndex = FMath::Max(0, Sessions.Num() - Configuration.EvaluationWindow);
    for (int32 i = StartIndex; i < Sessions.Num(); ++i)
    {
        TotalPerformance += Sessions[i].PerformanceScore;
    }
    
    float AveragePerformance = TotalPerformance / (Sessions.Num() - StartIndex);
    
    // 根據平均性能調整難度
    AdjustDifficulty(PlayerID, AveragePerformance);
}

void UMingRTSAIDynamicDifficulty::UpdatePlayerSkillRating(const FString& PlayerID)
{
    if (!PlayerSkills.Contains(PlayerID))
    {
        return;
    }
    
    FPlayerSkillData& SkillData = PlayerSkills[PlayerID];
    
    // 計算整體技能評級
    float TotalScore = 0.0f;
    int32 Count = 0;
    
    for (const auto& Pair : SkillData.SkillScores)
    {
        TotalScore += Pair.Value;
        Count++;
    }
    
    if (Count > 0)
    {
        SkillData.OverallSkillRating = TotalScore / Count;
    }
    
    // 計算一致性
    if (SessionHistories.Contains(PlayerID) && SessionHistories[PlayerID].Num() >= 3)
    {
        TArray<float> RecentPerformance;
        int32 StartIndex = FMath::Max(0, SessionHistories[PlayerID].Num() - 5);
        for (int32 i = StartIndex; i < SessionHistories[PlayerID].Num(); ++i)
        {
            RecentPerformance.Add(SessionHistories[PlayerID][i].PerformanceScore);
        }
        
        SkillData.ConsistencyScore = CalculateSkillConsistency(RecentPerformance);
        SkillData.ImprovementRate = CalculateImprovementRate(RecentPerformance);
    }
    
    // 經驗等級基於遊戲次數
    SkillData.ExperienceLevel = 1.0f + FMath::LogX(10.0f, static_cast<float>(SkillData.GamesPlayed) + 1.0f);
}

float UMingRTSAIDynamicDifficulty::CalculatePerformanceScore(const FString& PlayerID, const FSessionPerformanceData& Session)
{
    float Score = 0.5f; // 基礎分數
    
    // 挑戰完成率
    int32 TotalChallenges = Session.ChallengesCompleted + Session.ChallengesFailed;
    if (TotalChallenges > 0)
    {
        float CompletionRate = static_cast<float>(Session.ChallengesCompleted) / TotalChallenges;
        Score += CompletionRate * 0.2f;
    }
    
    // 單位交換比
    if (Session.UnitsLost > 0 || Session.EnemyUnitsDestroyed > 0)
    {
        float ExchangeRatio = static_cast<float>(Session.EnemyUnitsDestroyed) / (Session.UnitsLost + 1);
        Score += FMath::Clamp(ExchangeRatio / 5.0f, 0.0f, 0.2f);
    }
    
    // 資源收集
    if (Session.ResourcesGathered > 0)
    {
        // 假設1000資源為良好
        Score += FMath::Min(0.1f, Session.ResourcesGathered / 10000.0f);
    }
    
    // 根據開始和結束難度調整
    float DifficultyChange = Session.DifficultyAtEnd - Session.DifficultyAtStart;
    if (DifficultyChange > 0)
    {
        // 難度增加表示系統認為玩家表現好
        Score += 0.1f;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingRTSAIDynamicDifficulty::GetDifficultyForLevel(EDifficultyLevel Level) const
{
    switch (Level)
    {
    case EDifficultyLevel::VeryEasy: return 0.4f;
    case EDifficultyLevel::Easy: return 0.6f;
    case EDifficultyLevel::Normal: return 1.0f;
    case EDifficultyLevel::Hard: return 1.4f;
    case EDifficultyLevel::VeryHard: return 1.8f;
    case EDifficultyLevel::Nightmare: return 2.5f;
    case EDifficultyLevel::Adaptive: return 1.0f; // 基礎值，會動態調整
    default: return 1.0f;
    }
}

FDifficultyAdjustmentParams UMingRTSAIDynamicDifficulty::GenerateAdjustmentParams(const FString& PlayerID, float TargetMultiplier)
{
    FDifficultyAdjustmentParams Params;
    Params.EffectiveDifficultyMultiplier = TargetMultiplier;
    Params.ReasonForAdjustment = TEXT("Dynamic adjustment");
    Params.ConfidenceLevel = 0.7f;
    Params.Timestamp = CurrentGameTime;
    
    // 計算各項調整值
    float CurrentMultiplier = GetCurrentDifficultyMultiplier(PlayerID);
    float Ratio = TargetMultiplier / CurrentMultiplier;
    
    // AI智能調整
    Params.AdjustmentValues.Add(EDifficultyAdjustment::AIIntelligence, FMath::Clamp(Ratio, 0.5f, 2.0f));
    
    // AI資源調整
    Params.AdjustmentValues.Add(EDifficultyAdjustment::AIResources, FMath::Clamp(Ratio, 0.7f, 1.5f));
    
    // AI生產速度
    Params.AdjustmentValues.Add(EDifficultyAdjustment::AIProductionSpeed, FMath::Clamp(Ratio, 0.8f, 1.3f));
    
    // AI戰鬥加成
    float CombatBonus = (Ratio - 1.0f) * 0.3f;
    Params.AdjustmentValues.Add(EDifficultyAdjustment::AICombatBonus, 1.0f + CombatBonus);
    
    return Params;
}

float UMingRTSAIDynamicDifficulty::CalculateSkillConsistency(const TArray<float>& Scores) const
{
    if (Scores.Num() < 2)
    {
        return 0.5f;
    }
    
    // 計算標準差
    float Mean = 0.0f;
    for (float Score : Scores)
    {
        Mean += Score;
    }
    Mean /= Scores.Num();
    
    float Variance = 0.0f;
    for (float Score : Scores)
    {
        Variance += FMath::Square(Score - Mean);
    }
    Variance /= Scores.Num();
    
    float StdDev = FMath::Sqrt(Variance);
    
    // 一致性 = 1 - 相對標準差
    float Consistency = 1.0f - (StdDev / Mean);
    return FMath::Clamp(Consistency, 0.0f, 1.0f);
}

float UMingRTSAIDynamicDifficulty::CalculateImprovementRate(const TArray<float>& Scores) const
{
    if (Scores.Num() < 2)
    {
        return 0.0f;
    }
    
    // 簡單線性回歸斜率
    float First = Scores[0];
    float Last = Scores.Last();
    
    return (Last - First) / Scores.Num();
}

bool UMingRTSAIDynamicDifficulty::ShouldAdjustDifficulty(const FString& PlayerID, float CurrentPerformance) const
{
    // 如果性能極端，應該調整
    if (CurrentPerformance > 0.85f || CurrentPerformance < 0.25f)
    {
        return true;
    }
    
    // 檢查歷史趨勢
    if (SessionHistories.Contains(PlayerID))
    {
        const TArray<FSessionPerformanceData>& Sessions = SessionHistories[PlayerID];
        if (Sessions.Num() >= Configuration.EvaluationWindow)
        {
            // 檢查是否連續表現過好或過差
            int32 HighPerformanceCount = 0;
            int32 LowPerformanceCount = 0;
            
            int32 StartIndex = FMath::Max(0, Sessions.Num() - Configuration.EvaluationWindow);
            for (int32 i = StartIndex; i < Sessions.Num(); ++i)
            {
                if (Sessions[i].PerformanceScore > 0.75f) HighPerformanceCount++;
                if (Sessions[i].PerformanceScore < 0.35f) LowPerformanceCount++;
            }
            
            if (HighPerformanceCount >= Configuration.EvaluationWindow * 0.7f ||
                LowPerformanceCount >= Configuration.EvaluationWindow * 0.7f)
            {
                return true;
            }
        }
    }
    
    return false;
}

float UMingRTSAIDynamicDifficulty::ClampDifficultyMultiplier(float Multiplier) const
{
    return FMath::Clamp(Multiplier, Configuration.MinDifficultyMultiplier, Configuration.MaxDifficultyMultiplier);
}

void UMingRTSAIDynamicDifficulty::LogDifficultyChange(const FString& PlayerID, float OldMultiplier, float NewMultiplier, const FString& Reason)
{
    UE_LOG(LogTemp, Log, TEXT("MingRTSAIDynamicDifficulty: Player %s difficulty changed from %.2f to %.2f (%s)"),
        *PlayerID, OldMultiplier, NewMultiplier, *Reason);
}
