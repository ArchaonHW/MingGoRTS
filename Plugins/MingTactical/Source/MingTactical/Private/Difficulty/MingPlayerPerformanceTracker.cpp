#include "Difficulty/MingPlayerPerformanceTracker.h"
#include "MingCoreEventBus.h"
#include "Algo/Accumulate.h"

float FPlayerPerformanceDataPoint::CalculatePerformanceScore() const
{
    if (!IsValid())
    {
        return 50.0f; // 默认中等分数
    }

    // 计算各项子分数
    
    // 1. 单位存活率分数 (0-25分)
    // 单位损失率越低越好
    float SurvivalScore = FMath::Clamp((1.0f - UnitLossRate) * 25.0f, 0.0f, 25.0f);
    
    // 2. 资源效率分数 (0-25分)
    float ResourceScore = FMath::Clamp(ResourceEfficiency * 25.0f, 0.0f, 25.0f);
    
    // 3. 任务完成时间分数 (0-25分)
    // 完成时间比预期快得越多，分数越高
    float TimeRatio = (ExpectedCompletionTime > 0.0f) ? 
        (ExpectedCompletionTime / MissionCompletionTime) : 1.0f;
    float TimeScore = FMath::Clamp(TimeRatio * 25.0f, 0.0f, 25.0f);
    
    // 4. 战斗效率分数 (0-25分)
    float CombatScore = FMath::Clamp(CombatEffectiveness * 25.0f, 0.0f, 25.0f);
    
    return SurvivalScore + ResourceScore + TimeScore + CombatScore;
}

bool FPlayerPerformanceDataPoint::IsValid() const
{
    return TotalUnits > 0 && ExpectedCompletionTime > 0.0f;
}

void FPlayerPerformanceStats::UpdateFromDataPoints(const TArray<FPlayerPerformanceDataPoint>& DataPoints)
{
    SampleCount = DataPoints.Num();
    if (SampleCount == 0)
    {
        AveragePerformanceScore = 50.0f;
        TrendSlope = 0.0f;
        Volatility = 0.0f;
        OverallRating = EPlayerPerformanceRating::Average;
        RecommendationScore = 0.0f;
        return;
    }

    // 计算平均分数
    float TotalScore = 0.0f;
    for (const auto& Point : DataPoints)
    {
        TotalScore += Point.CalculatePerformanceScore();
    }
    AveragePerformanceScore = TotalScore / SampleCount;

    // 计算趋势斜率（线性回归的简单版本）
    if (SampleCount >= 2)
    {
        float FirstHalfAvg = 0.0f;
        float SecondHalfAvg = 0.0f;
        int32 HalfCount = SampleCount / 2;
        
        for (int32 i = 0; i < HalfCount; ++i)
        {
            FirstHalfAvg += DataPoints[i].CalculatePerformanceScore();
        }
        for (int32 i = SampleCount - HalfCount; i < SampleCount; ++i)
        {
            SecondHalfAvg += DataPoints[i].CalculatePerformanceScore();
        }
        
        FirstHalfAvg /= HalfCount;
        SecondHalfAvg /= HalfCount;
        TrendSlope = (SecondHalfAvg - FirstHalfAvg) / HalfCount;
        
        // 归一化趋势斜率到 -1 到 1 范围
        TrendSlope = FMath::Clamp(TrendSlope / 10.0f, -1.0f, 1.0f);
    }

    // 计算波动性（标准差的简化计算）
    float VarianceSum = 0.0f;
    for (const auto& Point : DataPoints)
    {
        float Diff = Point.CalculatePerformanceScore() - AveragePerformanceScore;
        VarianceSum += Diff * Diff;
    }
    Volatility = FMath::Sqrt(VarianceSum / SampleCount) / 100.0f; // 归一化到 0-1
    Volatility = FMath::Clamp(Volatility, 0.0f, 1.0f);

    // 确定总体评级
    OverallRating = EPlayerPerformanceRating::Average;
    if (AveragePerformanceScore >= 85.0f)
    {
        OverallRating = EPlayerPerformanceRating::Excellent;
    }
    else if (AveragePerformanceScore >= 70.0f)
    {
        OverallRating = EPlayerPerformanceRating::Good;
    }
    else if (AveragePerformanceScore >= 50.0f)
    {
        OverallRating = EPlayerPerformanceRating::Average;
    }
    else if (AveragePerformanceScore >= 30.0f)
    {
        OverallRating = EPlayerPerformanceRating::Poor;
    }
    else
    {
        OverallRating = EPlayerPerformanceRating::Critical;
    }

    // 计算推荐分数（基于表现、趋势和波动性）
    // 表现稳定且优秀 -> 提高难度
    // 表现波动大或较差 -> 降低难度
    // 趋势上升 -> 倾向提高难度
    // 趋势下降 -> 倾向降低难度
    RecommendationScore = (AveragePerformanceScore - 50.0f) / 50.0f; // -1 到 1
    RecommendationScore += TrendSlope * 0.5f; // 趋势影响
    RecommendationScore *= (1.0f - Volatility * 0.5f); // 波动性降低确定性
    RecommendationScore = FMath::Clamp(RecommendationScore, -1.0f, 1.0f);
}

UMingPlayerPerformanceTracker::UMingPlayerPerformanceTracker()
    : bIsInitialized(false)
    , EvaluationInterval(60.0f)
    , TimeSinceLastEvaluation(0.0f)
    , MaxDataPoints(20)
    , CurrentTotalUnits(0)
    , CurrentUnitsLost(0)
    , CurrentResourcesCollected(0.0f)
    , CurrentExpectedResources(0.0f)
    , SessionEnemiesKilled(0)
    , SessionAlliesLost(0)
    , SessionDamageDealt(0.0f)
    , SessionDamageTaken(0.0f)
    , BestPerformanceScore(50.0f)
    , WorstPerformanceScore(50.0f)
{
}

void UMingPlayerPerformanceTracker::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    SetupEventSubscriptions();
    
    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("PlayerPerformanceTracker initialized"));
}

void UMingPlayerPerformanceTracker::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    CleanupEventSubscriptions();
    ResetTracking();
    
    bIsInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("PlayerPerformanceTracker shutdown"));
}

void UMingPlayerPerformanceTracker::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    TimeSinceLastEvaluation += DeltaTime;
    
    if (TimeSinceLastEvaluation >= EvaluationInterval)
    {
        EvaluatePerformance();
        TimeSinceLastEvaluation = 0.0f;
    }
}

void UMingPlayerPerformanceTracker::RecordUnitLost(int32 UnitId, int32 UnitType, float UnitValue)
{
    CurrentUnitsLost++;
    SessionAlliesLost++;
    
    // 计算当前单位损失率
    float LossRate = (CurrentTotalUnits > 0) ? 
        static_cast<float>(CurrentUnitsLost) / CurrentTotalUnits : 0.0f;
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit lost: %d, Loss rate: %.2f"), UnitId, LossRate);
}

void UMingPlayerPerformanceTracker::RecordUnitCreated(int32 UnitId, int32 UnitType, float UnitCost)
{
    CurrentTotalUnits++;
    UE_LOG(LogTemp, Verbose, TEXT("Unit created: %d, Total: %d"), UnitId, CurrentTotalUnits);
}

void UMingPlayerPerformanceTracker::RecordResourceCollected(float ResourceType, float Amount, float ExpectedRate)
{
    CurrentResourcesCollected += Amount;
    
    // 跟踪预期资源收集
    float TimeSinceStart = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    CurrentExpectedResources = ExpectedRate * TimeSinceStart;
    
    UE_LOG(LogTemp, Verbose, TEXT("Resources collected: %.2f, Total: %.2f"), Amount, CurrentResourcesCollected);
}

void UMingPlayerPerformanceTracker::RecordMissionStarted(const FString& MissionId, float ExpectedDuration)
{
    float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    ActiveMissions.Add(MissionId, CurrentTime);
    
    UE_LOG(LogTemp, Log, TEXT("Mission started: %s, Expected duration: %.2f"), 
        *MissionId, ExpectedDuration);
}

void UMingPlayerPerformanceTracker::RecordMissionCompleted(const FString& MissionId, bool bSuccess, float CompletionTime)
{
    float* StartTime = ActiveMissions.Find(MissionId);
    if (StartTime)
    {
        float ActualTime = GetWorld() ? GetWorld()->GetTimeSeconds() - *StartTime : CompletionTime;
        ActiveMissions.Remove(MissionId);
        
        UE_LOG(LogTemp, Log, TEXT("Mission completed: %s, Success: %s, Time: %.2f"),
            *MissionId, bSuccess ? TEXT("Yes") : TEXT("No"), ActualTime);
    }
}

void UMingPlayerPerformanceTracker::RecordCombatResult(int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken)
{
    SessionEnemiesKilled += EnemiesKilled;
    SessionAlliesLost += AlliesLost;
    SessionDamageDealt += DamageDealt;
    SessionDamageTaken += DamageTaken;
    
    UE_LOG(LogTemp, Verbose, TEXT("Combat result - Kills: %d, Losses: %d, Damage dealt: %.2f, Taken: %.2f"),
        EnemiesKilled, AlliesLost, DamageDealt, DamageTaken);
}

TArray<FString> UMingPlayerPerformanceTracker::GetPlayerStrengths() const
{
    TArray<FString> Strengths;
    
    if (!HasSufficientData())
    {
        return Strengths;
    }

    // 分析数据点找出强项
    float AvgLossRate = 0.0f;
    float AvgResourceEff = 0.0f;
    float AvgCombatEff = 0.0f;
    
    for (const auto& Point : DataPoints)
    {
        AvgLossRate += Point.UnitLossRate;
        AvgResourceEff += Point.ResourceEfficiency;
        AvgCombatEff += Point.CombatEffectiveness;
    }
    
    int32 Count = DataPoints.Num();
    AvgLossRate /= Count;
    AvgResourceEff /= Count;
    AvgCombatEff /= Count;
    
    if (AvgLossRate < 0.2f)
    {
        Strengths.Add(TEXT("单位保护"));
    }
    if (AvgResourceEff > 0.8f)
    {
        Strengths.Add(TEXT("资源管理"));
    }
    if (AvgCombatEff > 0.8f)
    {
        Strengths.Add(TEXT("战斗效率"));
    }
    
    return Strengths;
}

TArray<FString> UMingPlayerPerformanceTracker::GetPlayerWeaknesses() const
{
    TArray<FString> Weaknesses;
    
    if (!HasSufficientData())
    {
        return Weaknesses;
    }

    float AvgLossRate = 0.0f;
    float AvgResourceEff = 0.0f;
    float AvgCombatEff = 0.0f;
    
    for (const auto& Point : DataPoints)
    {
        AvgLossRate += Point.UnitLossRate;
        AvgResourceEff += Point.ResourceEfficiency;
        AvgCombatEff += Point.CombatEffectiveness;
    }
    
    int32 Count = DataPoints.Num();
    AvgLossRate /= Count;
    AvgResourceEff /= Count;
    AvgCombatEff /= Count;
    
    if (AvgLossRate > 0.5f)
    {
        Weaknesses.Add(TEXT("单位损失过高"));
    }
    if (AvgResourceEff < 0.5f)
    {
        Weaknesses.Add(TEXT("资源收集效率低"));
    }
    if (AvgCombatEff < 0.5f)
    {
        Weaknesses.Add(TEXT("战斗效率待提升"));
    }
    
    return Weaknesses;
}

EDifficultyChangeDirection UMingPlayerPerformanceTracker::GetDifficultyRecommendation() const
{
    if (!HasSufficientData())
    {
        return EDifficultyChangeDirection::Maintain;
    }

    // 基于推荐分数决定难度调整方向
    if (CurrentStats.RecommendationScore > 0.3f)
    {
        return EDifficultyChangeDirection::Increase;
    }
    else if (CurrentStats.RecommendationScore < -0.3f)
    {
        return EDifficultyChangeDirection::Decrease;
    }
    
    return EDifficultyChangeDirection::Maintain;
}

bool UMingPlayerPerformanceTracker::HasSufficientData(int32 MinSamples) const
{
    return DataPoints.Num() >= MinSamples;
}

void UMingPlayerPerformanceTracker::SetEvaluationInterval(float IntervalSeconds)
{
    EvaluationInterval = FMath::Max(10.0f, IntervalSeconds);
}

void UMingPlayerPerformanceTracker::SetMaxDataPoints(int32 MaxPoints)
{
    MaxDataPoints = FMath::Clamp(MaxPoints, 5, 100);
    
    // 如果超出限制，移除旧数据
    while (DataPoints.Num() > MaxDataPoints)
    {
        DataPoints.RemoveAt(0);
    }
}

void UMingPlayerPerformanceTracker::ResetTracking()
{
    DataPoints.Empty();
    CurrentStats = FPlayerPerformanceStats();
    
    CurrentTotalUnits = 0;
    CurrentUnitsLost = 0;
    CurrentResourcesCollected = 0.0f;
    CurrentExpectedResources = 0.0f;
    
    ActiveMissions.Empty();
    
    SessionEnemiesKilled = 0;
    SessionAlliesLost = 0;
    SessionDamageDealt = 0.0f;
    SessionDamageTaken = 0.0f;
    
    BestPerformanceScore = 50.0f;
    WorstPerformanceScore = 50.0f;
    
    TimeSinceLastEvaluation = 0.0f;
}

void UMingPlayerPerformanceTracker::PrintDebugInfo()
{
    UE_LOG(LogTemp, Log, TEXT("=== Player Performance Debug Info ==="));
    UE_LOG(LogTemp, Log, TEXT("Skill Index: %.2f"), GetSkillIndex());
    UE_LOG(LogTemp, Log, TEXT("Sample Count: %d"), CurrentStats.SampleCount);
    UE_LOG(LogTemp, Log, TEXT("Trend: %.2f"), CurrentStats.TrendSlope);
    UE_LOG(LogTemp, Log, TEXT("Volatility: %.2f"), CurrentStats.Volatility);
    UE_LOG(LogTemp, Log, TEXT("Recommendation: %d"), static_cast<int32>(GetDifficultyRecommendation()));
    UE_LOG(LogTemp, Log, TEXT("===================================="));
}

FString UMingPlayerPerformanceTracker::GetDebugString() const
{
    return FString::Printf(TEXT("Skill: %.1f | Trend: %.2f | Vol: %.2f | Rec: %d"),
        GetSkillIndex(), 
        CurrentStats.TrendSlope,
        CurrentStats.Volatility,
        static_cast<int32>(GetDifficultyRecommendation()));
}

void UMingPlayerPerformanceTracker::EvaluatePerformance()
{
    float CurrentTime = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    
    // 创建新的数据点
    FPlayerPerformanceDataPoint NewPoint(CurrentTime);
    
    // 计算单位损失率
    if (CurrentTotalUnits > 0)
    {
        NewPoint.UnitLossRate = static_cast<float>(CurrentUnitsLost) / CurrentTotalUnits;
        NewPoint.UnitsLost = CurrentUnitsLost;
        NewPoint.TotalUnits = CurrentTotalUnits;
    }
    
    // 计算资源效率
    if (CurrentExpectedResources > 0.0f)
    {
        NewPoint.ResourceEfficiency = CurrentResourcesCollected / CurrentExpectedResources;
        NewPoint.ResourcesCollected = CurrentResourcesCollected;
        NewPoint.ExpectedResourceCollection = CurrentExpectedResources;
    }
    
    // 计算战斗效率
    if (SessionAlliesLost + SessionEnemiesKilled > 0)
    {
        float KillLossRatio = static_cast<float>(SessionEnemiesKilled) / 
            FMath::Max(1, SessionAlliesLost);
        float DamageRatio = (SessionDamageTaken > 0.0f) ? 
            SessionDamageDealt / SessionDamageTaken : 1.0f;
        NewPoint.CombatEffectiveness = FMath::Clamp((KillLossRatio + DamageRatio) / 2.0f, 0.0f, 1.0f);
    }
    
    // 添加数据点
    DataPoints.Add(NewPoint);
    
    // 限制数据点数量
    if (DataPoints.Num() > MaxDataPoints)
    {
        DataPoints.RemoveAt(0);
    }
    
    // 更新统计
    CurrentStats.UpdateFromDataPoints(DataPoints);
    
    // 更新历史最佳/最差
    float Score = NewPoint.CalculatePerformanceScore();
    if (Score > BestPerformanceScore)
    {
        BestPerformanceScore = Score;
    }
    if (Score < WorstPerformanceScore)
    {
        WorstPerformanceScore = Score;
    }
    
    // 发布性能评估事件
    UE_LOG(LogTemp, Log, TEXT("Performance evaluated - Score: %.2f, Skill Index: %.2f"),
        Score, CurrentStats.AveragePerformanceScore);
    
    // 重置会话统计（保留累积数据）
    SessionEnemiesKilled = 0;
    SessionAlliesLost = 0;
    SessionDamageDealt = 0.0f;
    SessionDamageTaken = 0.0f;
}

void UMingPlayerPerformanceTracker::SetupEventSubscriptions()
{
    // 订阅单位相关事件
    // 实际实现需要与具体的事件类型配合
    UE_LOG(LogTemp, Log, TEXT("PerformanceTracker event subscriptions setup"));
}

void UMingPlayerPerformanceTracker::CleanupEventSubscriptions()
{
    UE_LOG(LogTemp, Log, TEXT("PerformanceTracker event subscriptions cleaned up"));
}
