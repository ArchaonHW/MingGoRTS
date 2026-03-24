// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingAIAdaptationSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingAIAdaptationSystem::UMingAIAdaptationSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化系統參數
    LearningRate = 0.1f;
    MinSamplesForPattern = 5;
    AdaptationConfidenceThreshold = 0.6f;
    
    // 初始化狀態
    bSystemInitialized = false;
    CurrentDecisionMode = EAIDecisionMode::Strategic;
    CurrentOpponentID = TEXT("");
}

void UMingAIAdaptationSystem::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化系統
    InitializeAIAdaptationSystem();
    
    // 啟動定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            AnalysisTimer,
            this,
            &UMingAIAdaptationSystem::UpdateThreatAssessments,
            ANALYSIS_INTERVAL,
            true
        );
        
        GetWorld()->GetTimerManager().SetTimer(
            LearningUpdateTimer,
            this,
            &UMingAIAdaptationSystem::OptimizeAdaptationStrategy,
            LEARNING_UPDATE_INTERVAL,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI適配系統初始化完e"));
}

void UMingAIAdaptationSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bSystemInitialized)
    {
        return;
    }
    
    // 實時分析當前對手
    if (!CurrentOpponentID.IsEmpty())
    {
        UpdateThreatAssessments();
    }
}

bool UMingAIAdaptationSystem::InitializeAIAdaptationSystem()
{
    if (bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI適配系統已經初始化"));
        return true;
    }
    
    UE_LOG(LogTemp, Log, TEXT("開始初始化AI適配系統"));
    
    // 清空所有數據庫
    OpponentProfiles.Empty();
    PlayerAnalyses.Empty();
    LearningDatabase.Empty();
    CurrentThreats.Empty();
    
    // 初始化當前AI檔案
    CurrentAIProfile = FAICommanderProfile();
    CurrentAIProfile.CommanderID = TEXT("AI_001");
    CurrentAIProfile.CommanderName = TEXT("聖者指揮官");
    CurrentAIProfile.OpponentType = EAIOpponentType::SageCommander;
    CurrentAIProfile.PreferredMode = EAIDecisionMode::Strategic;
    CurrentAIProfile.Aggressiveness = 0.4f;
    CurrentAIProfile.Defensiveness = 0.6f;
    CurrentAIProfile.Adaptability = 0.8f;
    CurrentAIProfile.Deceptiveness = 0.3f;
    CurrentAIProfile.EconomicFocus = 0.5f;
    CurrentAIProfile.MilitaryFocus = 0.5f;
    
    bSystemInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("✓ AI適配系統初始化完e"));
    UE_LOG(LogTemp, Log, TEXT("  當前AI類型: 聖者指揮官"));
    UE_LOG(LogTemp, Log, TEXT("  適配能力: %.0f%%"), CurrentAIProfile.Adaptability * 100.0f);
    
    return true;
}

void UMingAIAdaptationSystem::ResetAIAdaptationSystem()
{
    UE_LOG(LogTemp, Log, TEXT("重置AI適配系統"));
    
    // 停止定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AnalysisTimer);
        GetWorld()->GetTimerManager().ClearTimer(LearningUpdateTimer);
    }
    
    // 清空數據
    OpponentProfiles.Empty();
    PlayerAnalyses.Empty();
    LearningDatabase.Empty();
    CurrentThreats.Empty();
    
    CurrentOpponentID = TEXT("");
    bSystemInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("AI適配系統已重置"));
}

FAICommanderProfile UMingAIAdaptationSystem::IdentifyOpponent(const FString& OpponentID)
{
    // 檢查是否已經識別過
    if (OpponentProfiles.Contains(OpponentID))
    {
        return OpponentProfiles[OpponentID];
    }
    
    // 創建新N對手檔案
    FAICommanderProfile Profile;
    Profile.CommanderID = OpponentID;
    Profile.CommanderName = FString::Printf(TEXT("對手_%s"), *OpponentID);
    
    // 根據初始行為分析確定類型
    // 這裡可以根據實際觀察到N行為進行更準確N識別
    Profile.OpponentType = EAIOpponentType::BalancedCommander;
    Profile.PreferredMode = EAIDecisionMode::Strategic;
    Profile.Aggressiveness = 0.5f;
    Profile.Defensiveness = 0.5f;
    Profile.Adaptability = 0.5f;
    Profile.Deceptiveness = 0.3f;
    Profile.EconomicFocus = 0.5f;
    Profile.MilitaryFocus = 0.5f;
    
    // 添加到檔案庫
    OpponentProfiles.Add(OpponentID, Profile);
    
    // 分析玩家行為
    FPlayerBehaviorAnalysis Analysis = AnalyzeOpponentBehavior(OpponentID);
    
    // 觸發識別事件
    TriggerOpponentIdentifiedEvent(Profile, Analysis);
    
    // g當前對手
    CurrentOpponentID = OpponentID;
    
    UE_LOG(LogTemp, Log, TEXT("識別新對手: %s, 類型: %d"), 
           *OpponentID, (int32)Profile.OpponentType);
    
    return Profile;
}

FPlayerBehaviorAnalysis UMingAIAdaptationSystem::AnalyzeOpponentBehavior(const FString& OpponentID)
{
    FPlayerBehaviorAnalysis Analysis;
    Analysis.PlayerID = OpponentID;
    Analysis.LastUpdateTime = FDateTime::Now();
    
    // 如果已有分析數據，基於歷史更新
    if (PlayerAnalyses.Contains(OpponentID))
    {
        Analysis = PlayerAnalyses[OpponentID];
    }
    
    // 基於策略i用頻率分析侵略性
    if (Analysis.StrategyUsageFrequency.Num() > 0)
    {
        int32 AggressiveCount = 0;
        int32 TotalCount = 0;
        
        for (const auto& Pair : Analysis.StrategyUsageFrequency)
        {
            ESixStrategyType Strategy = Pair.Key;
            int32 Count = Pair.Value;
            TotalCount += Count;
            
            // 破局、破結構、不破人被視為侵略性策略
            if (Strategy == ESixStrategyType::BreakSituation ||
                Strategy == ESixStrategyType::BreakStructure ||
                Strategy == ESixStrategyType::BreakNotPeople)
            {
                AggressiveCount += Count;
            }
        }
        
        if (TotalCount > 0)
        {
            Analysis.AggressionLevel = (float)AggressiveCount / TotalCount;
        }
        
        // 策略多樣性
        Analysis.StrategyVariety = (float)Analysis.StrategyUsageFrequency.Num() / 12.0f;
    }
    
    // 道德一致性分析
    Analysis.MoralConsistency = 0.7f; // 默認值，可以根據實際行為調整
    
    // 陰陽平衡分析
    int32 RighteousCount = 0;
    int32 ReverseCount = 0;
    
    for (const auto& Pair : Analysis.StrategyUsageFrequency)
    {
        ESixStrategyType Strategy = Pair.Key;
        int32 Count = Pair.Value;
        
        // v策
        if (Strategy == ESixStrategyType::EstablishNation ||
            Strategy == ESixStrategyType::EstablishSystem ||
            Strategy == ESixStrategyType::EstablishPeople)
        {
            RighteousCount += Count;
        }
        // 逆策
        else if (Strategy == ESixStrategyType::BreakSituation ||
                 Strategy == ESixStrategyType::BreakStructure ||
                 Strategy == ESixStrategyType::BreakNotPeople)
        {
            ReverseCount += Count;
        }
    }
    
    if (RighteousCount + ReverseCount > 0)
    {
        Analysis.YinYangBalance = 1.0f - FMath::Abs((float)(RighteousCount - ReverseCount) / (RighteousCount + ReverseCount));
    }
    
    // 保存分析結果
    PlayerAnalyses.Add(OpponentID, Analysis);
    
    // 檢測弱點
    Analysis.DetectedWeaknesses = DetectOpponentWeaknesses(OpponentID);
    
    return Analysis;
}

FAICommanderProfile UMingAIAdaptationSystem::GetOpponentProfile(const FString& OpponentID) const
{
    if (OpponentProfiles.Contains(OpponentID))
    {
        return OpponentProfiles[OpponentID];
    }
    
    return FAICommanderProfile();
}

TArray<EStrategyEffectType> UMingAIAdaptationSystem::DetectOpponentWeaknesses(const FString& OpponentID)
{
    TArray<EStrategyEffectType> Weaknesses;
    
    if (!PlayerAnalyses.Contains(OpponentID))
    {
        return Weaknesses;
    }
    
    const FPlayerBehaviorAnalysis& Analysis = PlayerAnalyses[OpponentID];
    
    // 基於侵略性水平檢測弱點
    if (Analysis.AggressionLevel > 0.7f)
    {
        // 高侵略性意味著防禦薄弱
        Weaknesses.Add(EStrategyEffectType::Defensive);
        Weaknesses.Add(EStrategyEffectType::StrategicPositioning);
    }
    else if (Analysis.AggressionLevel < 0.3f)
    {
        // 低侵略性意味著經濟或外交薄弱
        Weaknesses.Add(EStrategyEffectType::Offensive);
        Weaknesses.Add(EStrategyEffectType::MilitaryAdvantage);
    }
    
    // 基於策略多樣性檢測弱點
    if (Analysis.StrategyVariety < 0.4f)
    {
        // 策略單一可以被預測
        Weaknesses.Add(EStrategyEffectType::PsychologicalWarfare);
    }
    
    // 基於道德一致性檢測弱點
    if (Analysis.MoralConsistency < 0.5f)
    {
        // 道德不穩定可能出現墮落
        Weaknesses.Add(EStrategyEffectType::DiplomaticInfluence);
    }
    
    return Weaknesses;
}

bool UMingAIAdaptationSystem::RecordPlayerBehavior(const FString& PlayerID, ESixStrategyType StrategyUsed, float Outcome)
{
    // 確保分析對象存在
    if (!PlayerAnalyses.Contains(PlayerID))
    {
        FPlayerBehaviorAnalysis NewAnalysis;
        NewAnalysis.PlayerID = PlayerID;
        PlayerAnalyses.Add(PlayerID, NewAnalysis);
    }
    
    FPlayerBehaviorAnalysis& Analysis = PlayerAnalyses[PlayerID];
    
    // 更新策略i用頻率
    if (!Analysis.StrategyUsageFrequency.Contains(StrategyUsed))
    {
        Analysis.StrategyUsageFrequency.Add(StrategyUsed, 0);
    }
    Analysis.StrategyUsageFrequency[StrategyUsed]++;
    
    // 更新資源效率（基於結果）
    Analysis.ResourceEfficiency = Analysis.ResourceEfficiency * 0.9f + Outcome * 0.1f;
    
    Analysis.LastUpdateTime = FDateTime::Now();
    
    // 更新學習數據
    if (!LearningDatabase.Contains(PlayerID))
    {
        FAdaptationLearningData NewData;
        NewData.LastEncounter = FDateTime::Now();
        LearningDatabase.Add(PlayerID, NewData);
    }
    
    FAdaptationLearningData& LearningData = LearningDatabase[PlayerID];
    LearningData.EncounterCount++;
    LearningData.LastEncounter = FDateTime::Now();
    
    // 更新e功率
    float NewSuccessRate = Outcome > 0.5f ? 1.0f : 0.0f;
    LearningData.SuccessRate = LearningData.SuccessRate * 0.95f + NewSuccessRate * 0.05f;
    
    UE_LOG(LogTemp, Log, TEXT("記錄玩家行為: %s i用策略 %d, 結果: %.2f"), 
           *PlayerID, (int32)StrategyUsed, Outcome);
    
    return true;
}

FPlayerBehaviorAnalysis UMingAIAdaptationSystem::AnalyzePlayerPattern(const FString& PlayerID)
{
    if (PlayerAnalyses.Contains(PlayerID))
    {
        // 重新分析以獲取最新結果
        return AnalyzeOpponentBehavior(PlayerID);
    }
    
    return FPlayerBehaviorAnalysis();
}

ESixStrategyType UMingAIAdaptationSystem::PredictPlayerNextMove(const FString& PlayerID)
{
    if (!PlayerAnalyses.Contains(PlayerID))
    {
        return ESixStrategyType::None;
    }
    
    const FPlayerBehaviorAnalysis& Analysis = PlayerAnalyses[PlayerID];
    
    // 找到最常用N策略
    ESixStrategyType MostUsed = ESixStrategyType::None;
    int32 MaxCount = 0;
    
    for (const auto& Pair : Analysis.StrategyUsageFrequency)
    {
        if (Pair.Value > MaxCount)
        {
            MaxCount = Pair.Value;
            MostUsed = Pair.Key;
        }
    }
    
    // 如果樣r數不足，返回無
    if (MaxCount < MinSamplesForPattern)
    {
        return ESixStrategyType::None;
    }
    
    return MostUsed;
}

FAIAdaptationDecision UMingAIAdaptationSystem::GenerateAdaptationDecision(const FString& OpponentID, ESixStrategyType OpponentStrategy)
{
    FAIAdaptationDecision Decision;
    
    if (!bSystemInitialized)
    {
        return Decision;
    }
    
    // 獲取對手分析
    FPlayerBehaviorAnalysis Analysis = GetPlayerAnalysis(OpponentID);
    FAICommanderProfile Profile = GetOpponentProfile(OpponentID);
    
    // 選擇最佳反制策略
    Decision.RecommendedCounterStrategy = SelectBestCounterStrategy(OpponentID, OpponentStrategy);
    
    // 決定響應類型
    if (Decision.RecommendedCounterStrategy != ESixStrategyType::None)
    {
        // 檢查是否可以有效反制
        if (CalculateDecisionConfidence(Decision) > AdaptationConfidenceThreshold)
        {
            Decision.ResponseType = EAIAdaptationResponse::CounterStrategy;
        }
        else
        {
            // 置信度不足，選擇防禦姿態
            Decision.ResponseType = EAIAdaptationResponse::DefensivePosture;
        }
    }
    else
    {
        // 無法找到有效反制，採用干擾策略
        Decision.ResponseType = EAIAdaptationResponse::DisruptStrategy;
    }
    
    // 選擇決策模式
    switch (Decision.ResponseType)
    {
    case EAIAdaptationResponse::CounterStrategy:
        Decision.RecommendedMode = EAIDecisionMode::Strategic;
        break;
    case EAIAdaptationResponse::DefensivePosture:
        Decision.RecommendedMode = EAIDecisionMode::Defensive;
        break;
    case EAIAdaptationResponse::AggressiveExpansion:
        Decision.RecommendedMode = EAIDecisionMode::Aggressive;
        break;
    default:
        Decision.RecommendedMode = EAIDecisionMode::Reactive;
        break;
    }
    
    // 計算置信度
    Decision.Confidence = CalculateDecisionConfidence(Decision);
    
    // 計算預期效果
    Decision.ExpectedEffectiveness = EvaluateStrategyEffectiveness(Decision.RecommendedCounterStrategy, Analysis);
    
    // 風險評估
    Decision.RiskAssessment = CalculateMoralRisk(); // 這裡可以根據實際情況調整
    
    // 生e決策理由
    Decision.Reasoning = GenerateDecisionReasoningInternal(Decision);
    
    // g目標弱點
    Decision.TargetVulnerabilities = Profile.ExploitableWeaknesses;
    
    // 觸發決策事件
    TriggerDecisionMadeEvent(Decision);
    
    UE_LOG(LogTemp, Log, TEXT("生e適配決策: %s 對策略 %d, 推薦反制: %d, 置信度: %.2f"), 
           *OpponentID, (int32)OpponentStrategy, (int32)Decision.RecommendedCounterStrategy, Decision.Confidence);
    
    return Decision;
}

ESixStrategyType UMingAIAdaptationSystem::SelectBestCounterStrategy(const FString& OpponentID, ESixStrategyType OpponentStrategy)
{
    // 生e反制策略映射
    TMap<ESixStrategyType, ESixStrategyType> CounterMap = GenerateCounterStrategyMap();
    
    if (CounterMap.Contains(OpponentStrategy))
    {
        return CounterMap[OpponentStrategy];
    }
    
    // 如果沒有直接反制，根據對手弱點選擇
    TArray<EStrategyEffectType> Weaknesses = DetectOpponentWeaknesses(OpponentID);
    
    // 基於弱點選擇策略
    if (Weaknesses.Contains(EStrategyEffectType::Defensive))
    {
        return ESixStrategyType::BreakSituation; // 突破防禦
    }
    else if (Weaknesses.Contains(EStrategyEffectType::Offensive))
    {
        return ESixStrategyType::EstablishSystem; // 加強防禦
    }
    else if (Weaknesses.Contains(EStrategyEffectType::PsychologicalWarfare))
    {
        return ESixStrategyType::BreakNotPeople; // 心理戰
    }
    
    // 默認反制
    return ESixStrategyType::EstablishNation;
}

float UMingAIAdaptationSystem::EvaluateDecisionEffectiveness(const FAIAdaptationDecision& Decision) const
{
    if (!Decision.Reasoning.IsEmpty())
    {
        // 如果有理由支持，效果較好
        return 0.7f + Decision.Confidence * 0.3f;
    }
    
    return Decision.Confidence * 0.5f;
}

TArray<FString> UMingAIAdaptationSystem::GenerateDecisionReasoning(const FAIAdaptationDecision& Decision) const
{
    return GenerateDecisionReasoningInternal(Decision);
}

FThreatAssessment UMingAIAdaptationSystem::AssessThreat(const FString& OpponentID, ESixStrategyType OpponentStrategy)
{
    FThreatAssessment Threat;
    Threat.ThreatSource = OpponentID;
    
    // 獲取對手分析
    FPlayerBehaviorAnalysis Analysis = GetPlayerAnalysis(OpponentID);
    
    // 基於策略評估威脅
    switch (OpponentStrategy)
    {
    case ESixStrategyType::BreakSituation:
    case ESixStrategyType::BreakStructure:
    case ESixStrategyType::BreakNotPeople:
        // 逆策威脅較j
        Threat.ThreatMagnitude = 0.7f + Analysis.AggressionLevel * 0.3f;
        break;
        
    case ESixStrategyType::EstablishNation:
    case ESixStrategyType::EstablishSystem:
    case ESixStrategyType::EstablishPeople:
        // v策威脅較小
        Threat.ThreatMagnitude = 0.4f;
        break;
        
    default:
        Threat.ThreatMagnitude = 0.5f;
        break;
    }
    
    // 根據威脅程度分類
    if (Threat.ThreatMagnitude > 0.8f)
    {
        Threat.ThreatLevel = EAIThreatLevel::Critical;
        Threat.bIsImmediate = true;
        Threat.TimeToImpact = 30.0f;
    }
    else if (Threat.ThreatMagnitude > 0.6f)
    {
        Threat.ThreatLevel = EAIThreatLevel::High;
        Threat.bIsImmediate = false;
        Threat.TimeToImpact = 60.0f;
    }
    else if (Threat.ThreatMagnitude > 0.4f)
    {
        Threat.ThreatLevel = EAIThreatLevel::Moderate;
        Threat.TimeToImpact = 120.0f;
    }
    else
    {
        Threat.ThreatLevel = EAIThreatLevel::Low;
        Threat.TimeToImpact = 300.0f;
    }
    
    // 識別脆弱點
    TArray<EStrategyEffectType> Weaknesses = DetectOpponentWeaknesses(OpponentID);
    for (EStrategyEffectType Weakness : Weaknesses)
    {
        switch (Weakness)
        {
        case EStrategyEffectType::Defensive:
            Threat.VulnerablePoints.Add(TEXT("防禦體系"));
            break;
        case EStrategyEffectType::Offensive:
            Threat.VulnerablePoints.Add(TEXT("進攻能力"));
            break;
        case EStrategyEffectType::PsychologicalWarfare:
            Threat.VulnerablePoints.Add(TEXT("心理防線"));
            break;
        default:
            break;
        }
    }
    
    // 生e緩解策略
    Threat.MitigationStrategies = GenerateMitigationStrategies(Threat);
    
    // 添加到當前威脅
    CurrentThreats.Add(Threat);
    
    // 觸發威脅事件
    TriggerThreatDetectedEvent(Threat);
    
    UE_LOG(LogTemp, Log, TEXT("評估威脅: %s 威脅等級 %d, 強度 %.2f"), 
           *OpponentID, (int32)Threat.ThreatLevel, Threat.ThreatMagnitude);
    
    return Threat;
}

TArray<FThreatAssessment> UMingAIAdaptationSystem::GetCurrentThreats() const
{
    return CurrentThreats;
}

FThreatAssessment UMingAIAdaptationSystem::GetMostCriticalThreat() const
{
    FThreatAssessment MostCritical;
    MostCritical.ThreatLevel = EAIThreatLevel::None;
    
    for (const auto& Threat : CurrentThreats)
    {
        if ((int32)Threat.ThreatLevel > (int32)MostCritical.ThreatLevel)
        {
            MostCritical = Threat;
        }
        else if (Threat.ThreatLevel == MostCritical.ThreatLevel && 
                 Threat.ThreatMagnitude > MostCritical.ThreatMagnitude)
        {
            MostCritical = Threat;
        }
    }
    
    return MostCritical;
}

TArray<FString> UMingAIAdaptationSystem::GenerateMitigationStrategies(const FThreatAssessment& Threat)
{
    TArray<FString> Strategies;
    
    switch (Threat.ThreatLevel)
    {
    case EAIThreatLevel::Critical:
        Strategies.Add(TEXT("立即啟動全面防禦"));
        Strategies.Add(TEXT("i用道權進行道德監督"));
        Strategies.Add(TEXT("啟動緊急策略轉換"));
        break;
        
    case EAIThreatLevel::High:
        Strategies.Add(TEXT("加強情報收集"));
        Strategies.Add(TEXT("強化邊界防禦"));
        Strategies.Add(TEXT("準備反制措施"));
        break;
        
    case EAIThreatLevel::Moderate:
        Strategies.Add(TEXT("保持警惕"));
        Strategies.Add(TEXT("優化資源分配"));
        break;
        
    default:
        Strategies.Add(TEXT("繼續監控"));
        break;
    }
    
    return Strategies;
}

bool UMingAIAdaptationSystem::SetAIDecisionMode(EAIDecisionMode Mode)
{
    EAIDecisionMode PreviousMode = CurrentDecisionMode;
    CurrentDecisionMode = Mode;
    
    UE_LOG(LogTemp, Log, TEXT("切換AI決策模式: %d -> %d"), 
           (int32)PreviousMode, (int32)Mode);
    
    return true;
}

EAIDecisionMode UMingAIAdaptationSystem::GetCurrentDecisionMode() const
{
    return CurrentDecisionMode;
}

bool UMingAIAdaptationSystem::ExecuteAIDecision(const FAIAdaptationDecision& Decision)
{
    // 執行決策N邏輯
    // 這裡可以調用其他系統來實際執行決策
    
    UE_LOG(LogTemp, Log, TEXT("執行AI決策: 響應類型 %d, 策略 %d"), 
           (int32)Decision.ResponseType, (int32)Decision.RecommendedCounterStrategy);
    
    // 觸發策略反制事件
    if (Decision.RecommendedCounterStrategy != ESixStrategyType::None)
    {
        // 這裡應該有對手N原始策略，但簡化處理
        OnStrategyCountered.Broadcast(ESixStrategyType::None, Decision.RecommendedCounterStrategy);
    }
    
    return true;
}

TArray<FAIAdaptationDecision> UMingAIAdaptationSystem::GetAIRecommendations(int32 MaxRecommendations)
{
    TArray<FAIAdaptationDecision> Recommendations;
    
    if (CurrentOpponentID.IsEmpty())
    {
        return Recommendations;
    }
    
    // 獲取對手N預測行為
    ESixStrategyType PredictedMove = PredictPlayerNextMove(CurrentOpponentID);
    
    if (PredictedMove != ESixStrategyType::None)
    {
        // 生e主要推薦
        FAIAdaptationDecision MainDecision = GenerateAdaptationDecision(CurrentOpponentID, PredictedMove);
        Recommendations.Add(MainDecision);
        
        // 生e備選推薦
        for (int32 i = 1; i < MaxRecommendations && i < 3; ++i)
        {
            FAIAdaptationDecision AltDecision;
            AltDecision.ResponseType = EAIAdaptationResponse::DefensivePosture;
            AltDecision.RecommendedMode = EAIDecisionMode::Defensive;
            AltDecision.Confidence = MainDecision.Confidence * 0.7f;
            AltDecision.ExpectedEffectiveness = MainDecision.ExpectedEffectiveness * 0.8f;
            
            Recommendations.Add(AltDecision);
        }
    }
    
    return Recommendations;
}

bool UMingAIAdaptationSystem::UpdateLearningData(const FString& OpponentID, EAIAdaptationResponse Response, float Effectiveness)
{
    if (!LearningDatabase.Contains(OpponentID))
    {
        FAdaptationLearningData NewData;
        LearningDatabase.Add(OpponentID, NewData);
    }
    
    FAdaptationLearningData& Data = LearningDatabase[OpponentID];
    
    // 更新響應效果
    if (!Data.ResponseEffectiveness.Contains(Response))
    {
        Data.ResponseEffectiveness.Add(Response, 0.5f);
    }
    
    float& CurrentEffectiveness = Data.ResponseEffectiveness[Response];
    CurrentEffectiveness = CurrentEffectiveness * 0.9f + Effectiveness * 0.1f;
    
    // 更新e功率
    Data.SuccessRate = Data.SuccessRate * 0.95f + Effectiveness * 0.05f;
    
    // 檢查是否充分了解
    Data.bIsWellUnderstood = (Data.EncounterCount >= MIN_LEARNING_SAMPLES);
    
    // 觸發學習更新事件
    TriggerLearningUpdatedEvent(Data);
    
    UE_LOG(LogTemp, Log, TEXT("更新學習數據: %s 響應 %d, 效果 %.2f"), 
           *OpponentID, (int32)Response, Effectiveness);
    
    return true;
}

bool UMingAIAdaptationSystem::OptimizeAdaptationStrategy(const FString& OpponentID)
{
    if (OpponentID.IsEmpty())
    {
        // 優化所有對手N策略
        for (const auto& Pair : LearningDatabase)
        {
            OptimizeAdaptationStrategy(Pair.Key);
        }
        return true;
    }
    
    if (!LearningDatabase.Contains(OpponentID))
    {
        return false;
    }
    
    FAdaptationLearningData& Data = LearningDatabase[OpponentID];
    
    // 基於學習數據優化策略選擇
    // 找到最有效N響應
    EAIAdaptationResponse BestResponse = EAIAdaptationResponse::None;
    float BestEffectiveness = 0.0f;
    
    for (const auto& Pair : Data.ResponseEffectiveness)
    {
        if (Pair.Value > BestEffectiveness)
        {
            BestEffectiveness = Pair.Value;
            BestResponse = Pair.Key;
        }
    }
    
    // 更新AI檔案
    if (OpponentProfiles.Contains(OpponentID))
    {
        FAICommanderProfile& Profile = OpponentProfiles[OpponentID];
        Profile.Adaptability = FMath::Min(1.0f, Profile.Adaptability + 0.05f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("優化適配策略: %s, 最佳響應: %d, 效果: %.2f"), 
           *OpponentID, (int32)BestResponse, BestEffectiveness);
    
    return true;
}

// ============================================================================
// 內部方法實現
// ============================================================================

TMap<ESixStrategyType, ESixStrategyType> UMingAIAdaptationSystem::GenerateCounterStrategyMap()
{
    TMap<ESixStrategyType, ESixStrategyType> CounterMap;
    
    // v逆六策N克制關係
    // v策克制v策，逆策克制逆策
    // 立國克制立制（制度需要國家基礎）
    CounterMap.Add(ESixStrategyType::EstablishNation, ESixStrategyType::BreakStructure);
    CounterMap.Add(ESixStrategyType::EstablishSystem, ESixStrategyType::BreakSituation);
    CounterMap.Add(ESixStrategyType::EstablishPeople, ESixStrategyType::BreakNotPeople);
    
    // 逆策克制v策（非常規對常規）
    CounterMap.Add(ESixStrategyType::BreakSituation, ESixStrategyType::EstablishNation);
    CounterMap.Add(ESixStrategyType::BreakStructure, ESixStrategyType::EstablishSystem);
    CounterMap.Add(ESixStrategyType::BreakNotPeople, ESixStrategyType::EstablishPeople);
    
    return CounterMap;
}

float UMingAIAdaptationSystem::EvaluateStrategyEffectiveness(ESixStrategyType Strategy, const FPlayerBehaviorAnalysis& Analysis)
{
    float BaseEffectiveness = 0.5f;
    
    // 基於策略類型調整
    switch (Strategy)
    {
    case ESixStrategyType::EstablishNation:
        BaseEffectiveness += (1.0f - Analysis.MoralConsistency) * 0.3f;
        break;
    case ESixStrategyType::BreakSituation:
        BaseEffectiveness += Analysis.AggressionLevel * 0.3f;
        break;
    case ESixStrategyType::BreakNotPeople:
        BaseEffectiveness += (1.0f - Analysis.StrategyVariety) * 0.4f;
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseEffectiveness, 0.0f, 1.0f);
}

ESixStrategyType UMingAIAdaptationSystem::SelectOptimalCounter(const TArray<ESixStrategyType>& Options, const FPlayerBehaviorAnalysis& Analysis)
{
    if (Options.Num() == 0)
    {
        return ESixStrategyType::None;
    }
    
    ESixStrategyType BestOption = Options[0];
    float BestScore = EvaluateStrategyEffectiveness(BestOption, Analysis);
    
    for (int32 i = 1; i < Options.Num(); ++i)
    {
        float Score = EvaluateStrategyEffectiveness(Options[i], Analysis);
        if (Score > BestScore)
        {
            BestScore = Score;
            BestOption = Options[i];
        }
    }
    
    return BestOption;
}

void UMingAIAdaptationSystem::UpdateThreatAssessments()
{
    // 清理過期N威脅
    FDateTime CurrentTime = FDateTime::Now();
    for (int32 i = CurrentThreats.Num() - 1; i >= 0; --i)
    {
        // 簡單N過期檢查，可以根據需要調整
        if (CurrentThreats[i].ThreatLevel == EAIThreatLevel::Low)
        {
            CurrentThreats.RemoveAt(i);
        }
    }
}

float UMingAIAdaptationSystem::CalculateDecisionConfidence(const FAIAdaptationDecision& Decision)
{
    float Confidence = 0.5f;
    
    // 基於響應類型
    switch (Decision.ResponseType)
    {
    case EAIAdaptationResponse::CounterStrategy:
        Confidence += 0.2f;
        break;
    case EAIAdaptationResponse::DefensivePosture:
        Confidence += 0.1f;
        break;
    default:
        break;
    }
    
    // 基於預期效果
    Confidence += Decision.ExpectedEffectiveness * 0.2f;
    
    return FMath::Clamp(Confidence, 0.0f, 1.0f);
}

TArray<FString> UMingAIAdaptationSystem::GenerateDecisionReasoningInternal(const FAIAdaptationDecision& Decision)
{
    TArray<FString> Reasoning;
    
    // 基於響應類型生e理由
    switch (Decision.ResponseType)
    {
    case EAIAdaptationResponse::CounterStrategy:
        Reasoning.Add(TEXT("選擇反制策略可以有效應對對手N攻勢"));
        Reasoning.Add(FString::Printf(TEXT("預期效果: %.0f%%"), Decision.ExpectedEffectiveness * 100.0f));
        break;
        
    case EAIAdaptationResponse::DefensivePosture:
        Reasoning.Add(TEXT("當前情況適合採取守勢"));
        Reasoning.Add(TEXT("等待更好N反擊時機"));
        break;
        
    case EAIAdaptationResponse::ExploitWeakness:
        Reasoning.Add(TEXT("識別到對手N薄弱點"));
        Reasoning.Add(TEXT("可以針對性攻擊以獲得優勢"));
        break;
        
    default:
        Reasoning.Add(TEXT("基於當前局勢做出N最優選擇"));
        break;
    }
    
    // 添加風險評估
    if (Decision.RiskAssessment > 0.5f)
    {
        Reasoning.Add(TEXT("注意：此決策存在一定風險"));
    }
    
    return Reasoning;
}

void UMingAIAdaptationSystem::TriggerOpponentIdentifiedEvent(const FAICommanderProfile& Profile, const FPlayerBehaviorAnalysis& Analysis)
{
    OnOpponentIdentified.Broadcast(Profile, Analysis);
}

void UMingAIAdaptationSystem::TriggerDecisionMadeEvent(const FAIAdaptationDecision& Decision)
{
    OnAdaptationDecisionMade.Broadcast(Decision);
}

void UMingAIAdaptationSystem::TriggerThreatDetectedEvent(const FThreatAssessment& Threat)
{
    OnThreatDetected.Broadcast(Threat);
}

void UMingAIAdaptationSystem::TriggerLearningUpdatedEvent(const FAdaptationLearningData& LearningData)
{
    OnLearningUpdated.Broadcast(LearningData);
}

float UMingAIAdaptationSystem::CalculateMoralRisk() const
{
    // 簡單N道德風險計算
    return 0.2f; // 默認低風險
}
