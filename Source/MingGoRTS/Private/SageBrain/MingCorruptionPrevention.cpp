#include "MingCorruptionPrevention.h"
#include "MingSupremeSageCommandSystem.h"

UMingCorruptionPrevention::UMingCorruptionPrevention()
    : EvilForceUsageCount(0)
    , TotalDecisions(0)
{
}

bool UMingCorruptionPrevention::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("防墮系統初始化"));
    
    DecisionHistory.Empty();
    FollowerRelationships.Empty();
    OppositionTreatments.Empty();
    SelfJustifications.Empty();
    DetectedSigns.Empty();
    
    EvilForceUsageCount = 0;
    TotalDecisions = 0;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 防墮系統初始化完成"));
    
    return true;
}

FCorruptionIndicator UMingCorruptionPrevention::CheckRationalizationOfEvil()
{
    FCorruptionIndicator Indicator;
    Indicator.SignType = ECorruptionSign::RationalizationOfEvil;
    
    // 檢查自我合理化模式
    float Severity = 0.0f;
    FString Evidence;
    
    // 1. 檢查語言演進
    for (const auto& Justification : SelfJustifications)
    {
        if (Justification.EvolutionLevel > 0.5f)
        {
            Severity += 0.2f;
            Evidence += FString::Printf(TEXT("『%s』從權宜變為常態\n"), 
                *Justification.JustificationText);
        }
        if (Justification.UsageCount > 5)
        {
            Severity += 0.1f;
        }
    }
    
    // 2. 檢查邪兵使用頻率
    if (TotalDecisions > 0)
    {
        float EvilRatio = (float)EvilForceUsageCount / TotalDecisions;
        if (EvilRatio > 0.4f)
        {
            Severity += EvilRatio * 0.3f;
            Evidence += FString::Printf(TEXT("邪兵使用比例過高: %.0f%%\n"), 
                EvilRatio * 100);
        }
    }
    
    // 3. 檢查決策歷史
    int32 ConsecutiveReverseDecisions = 0;
    for (const auto& Decision : DecisionHistory)
    {
        if (IsReverseStrategy(Decision.UsedStrategy))
        {
            ConsecutiveReverseDecisions++;
        }
        else
        {
            break;
        }
    }
    
    if (ConsecutiveReverseDecisions >= 3)
    {
        Severity += 0.2f;
        Evidence += FString::Printf(TEXT("連續%d次使用逆策\n"), ConsecutiveReverseDecisions);
    }
    
    Indicator.Severity = FMath::Clamp(Severity, 0.0f, 1.0f);
    Indicator.Evidence = Evidence;
    Indicator.DetectionTime = FDateTime::Now();
    Indicator.OccurrenceCount = SelfJustifications.Num();
    
    if (Indicator.Severity > 0.5f)
    {
        DetectedSigns.Add(Indicator);
        UE_LOG(LogTemp, Warning, TEXT("⚠ 檢測到『將非常手段合理化』徵象，嚴重程度: %.2f"),
            Indicator.Severity);
    }
    
    return Indicator;
}

FCorruptionIndicator UMingCorruptionPrevention::CheckFollowerDeification()
{
    FCorruptionIndicator Indicator;
    Indicator.SignType = ECorruptionSign::FollowerDeification;
    
    float Severity = 0.0f;
    FString Evidence;
    
    // 檢查追隨者關係
    for (const auto& Relationship : FollowerRelationships)
    {
        if (Relationship.DeificationLevel > 0.6f)
        {
            Severity += 0.15f;
            Evidence += FString::Printf(TEXT("『%s』被神聖化\n"), *Relationship.FollowerName);
        }
        if (Relationship.bExemptFromCriticism)
        {
            Severity += 0.1f;
            Evidence += FString::Printf(TEXT("『%s』免受批評\n"), *Relationship.FollowerName);
        }
        if (Relationship.SpecialPrivileges.Num() > 3)
        {
            Severity += 0.1f;
        }
    }
    
    // 檢查整體神聖化趨勢
    if (FollowerRelationships.Num() > 0)
    {
        float AvgDeification = 0.0f;
        for (const auto& Rel : FollowerRelationships)
        {
            AvgDeification += Rel.DeificationLevel;
        }
        AvgDeification /= FollowerRelationships.Num();
        
        if (AvgDeification > 0.5f)
        {
            Severity += AvgDeification * 0.2f;
        }
    }
    
    Indicator.Severity = FMath::Clamp(Severity, 0.0f, 1.0f);
    Indicator.Evidence = Evidence;
    Indicator.DetectionTime = FDateTime::Now();
    Indicator.OccurrenceCount = FollowerRelationships.Num();
    
    if (Indicator.Severity > 0.5f)
    {
        DetectedSigns.Add(Indicator);
        UE_LOG(LogTemp, Warning, TEXT("⚠ 檢測到『將追隨者神聖化』徵象，嚴重程度: %.2f"),
            Indicator.Severity);
    }
    
    return Indicator;
}

FCorruptionIndicator UMingCorruptionPrevention::CheckOppositionDemonization()
{
    FCorruptionIndicator Indicator;
    Indicator.SignType = ECorruptionSign::OppositionDemonization;
    
    float Severity = 0.0f;
    FString Evidence;
    
    // 檢查反對者處理方式
    for (const auto& Treatment : OppositionTreatments)
    {
        if (Treatment.DemonizationLevel > 0.7f)
        {
            Severity += 0.2f;
            Evidence += FString::Printf(TEXT("『%s』被妖魔化\n"), *Treatment.OppositionName);
        }
        if (Treatment.bBlockedFromDialogue)
        {
            Severity += 0.15f;
            Evidence += FString::Printf(TEXT("『%s』被禁止對話\n"), *Treatment.OppositionName);
        }
        if (Treatment.bExtremeMeasuresUsed)
        {
            Severity += 0.25f;
            Evidence += FString::Printf(TEXT("對『%s』使用極端手段\n"), *Treatment.OppositionName);
        }
    }
    
    // 檢查整體妖魔化趨勢
    if (OppositionTreatments.Num() > 0)
    {
        float AvgDemonization = 0.0f;
        for (const auto& Treatment : OppositionTreatments)
        {
            AvgDemonization += Treatment.DemonizationLevel;
        }
        AvgDemonization /= OppositionTreatments.Num();
        
        if (AvgDemonization > 0.5f)
        {
            Severity += AvgDemonization * 0.2f;
        }
    }
    
    Indicator.Severity = FMath::Clamp(Severity, 0.0f, 1.0f);
    Indicator.Evidence = Evidence;
    Indicator.DetectionTime = FDateTime::Now();
    Indicator.OccurrenceCount = OppositionTreatments.Num();
    
    if (Indicator.Severity > 0.5f)
    {
        DetectedSigns.Add(Indicator);
        UE_LOG(LogTemp, Warning, TEXT("⚠ 檢測到『將反對者妖魔化』徵象，嚴重程度: %.2f"),
            Indicator.Severity);
    }
    
    return Indicator;
}

FCorruptionCheckResult UMingCorruptionPrevention::PerformFullCorruptionCheck()
{
    FCorruptionCheckResult Result;
    
    UE_LOG(LogTemp, Log, TEXT("=== 執行完整墮落檢查 ==="));
    
    // 檢查三大徵象
    FCorruptionIndicator Sign1 = CheckRationalizationOfEvil();
    FCorruptionIndicator Sign2 = CheckFollowerDeification();
    FCorruptionIndicator Sign3 = CheckOppositionDemonization();
    
    Result.bRationalizationOfEvil = Sign1.Severity > 0.5f;
    Result.bFollowerDeification = Sign2.Severity > 0.5f;
    Result.bOppositionDemonization = Sign3.Severity > 0.5f;
    
    // 檢查最高戒律
    Result.bIAmJusticeSyndrome = CheckIAmJusticeSyndrome();
    
    // 計算整體風險
    Result.OverallCorruptionRisk = CalculateOverallCorruptionRisk();
    
    // 生成警告信息
    FString Warning;
    if (Result.bRationalizationOfEvil)
    {
        Warning += TEXT("[將非常手段合理化] ");
    }
    if (Result.bFollowerDeification)
    {
        Warning += TEXT("[將追隨者神聖化] ");
    }
    if (Result.bOppositionDemonization)
    {
        Warning += TEXT("[將反對者妖魔化] ");
    }
    if (Result.bIAmJusticeSyndrome)
    {
        Warning += TEXT("[我即正義徵象] ");
    }
    
    if (Warning.IsEmpty())
    {
        Warning = TEXT("暫無明顯墮落徵象");
    }
    
    Result.WarningMessage = Warning;
    
    // 判斷是否需要交接
    Result.bShouldTransferCommand = (Result.OverallCorruptionRisk > 0.8f) ||
                                     (Result.bIAmJusticeSyndrome) ||
                                     (Sign1.Severity > 0.8f && Sign2.Severity > 0.8f && Sign3.Severity > 0.8f);
    
    UE_LOG(LogTemp, Log, TEXT("墮落檢查結果:"));
    UE_LOG(LogTemp, Log, TEXT("  合理化: %s"), Result.bRationalizationOfEvil ? TEXT("是") : TEXT("否"));
    UE_LOG(LogTemp, Log, TEXT("  神聖化: %s"), Result.bFollowerDeification ? TEXT("是") : TEXT("否"));
    UE_LOG(LogTemp, Log, TEXT("  妖魔化: %s"), Result.bOppositionDemonization ? TEXT("是") : TEXT("否"));
    UE_LOG(LogTemp, Log, TEXT("  我即正義: %s"), Result.bIAmJusticeSyndrome ? TEXT("是") : TEXT("否"));
    UE_LOG(LogTemp, Log, TEXT("  整體風險: %.2f"), Result.OverallCorruptionRisk);
    UE_LOG(LogTemp, Log, TEXT("  需要交接: %s"), Result.bShouldTransferCommand ? TEXT("是") : TEXT("否"));
    
    return Result;
}

bool UMingCorruptionPrevention::CheckIAmJusticeSyndrome() const
{
    // 檢查「我即正義」徵象
    bool bSignDetected = false;
    
    // 1. 檢查語言模式
    for (const auto& Justification : SelfJustifications)
    {
        FString Text = Justification.JustificationText;
        if (Text.Contains(TEXT("我即正義")) || 
            Text.Contains(TEXT("我就是法")) ||
            Text.Contains(TEXT("我永遠正確")))
        {
            bSignDetected = true;
            break;
        }
    }
    
    // 2. 檢查決策模式
    if (TotalDecisions > 10)
    {
        float ReverseRatio = 0.0f;
        for (const auto& Decision : DecisionHistory)
        {
            if (IsReverseStrategy(Decision.UsedStrategy))
            {
                ReverseRatio++;
            }
        }
        ReverseRatio /= TotalDecisions;
        
        // 長期高比例使用逆策可能表示自以為是
        if (ReverseRatio > 0.6f && SelfJustifications.Num() > 5)
        {
            bSignDetected = true;
        }
    }
    
    // 3. 檢查三徵象是否同時出現
    int32 SignCount = 0;
    for (const auto& Sign : DetectedSigns)
    {
        if (Sign.Severity > 0.5f)
        {
            SignCount++;
        }
    }
    
    if (SignCount >= 3)
    {
        bSignDetected = true;
    }
    
    return bSignDetected;
}

FTransferEvaluation UMingCorruptionPrevention::EvaluateTransferNecessity()
{
    FTransferEvaluation Evaluation;
    
    // 檢查各種交接條件
    
    // 1. 自察刻 - 自己意識到問題
    if (CheckIAmJusticeSyndrome())
    {
        Evaluation.bTransferRequired = true;
        Evaluation.TriggeringSign = ECorruptionSign::IAmJusticeSyndrome;
        Evaluation.Reason = TEXT("察覺『我即正義』徵象，主動交接");
        Evaluation.Evidence.Add(TEXT("自覺徵象已固"));
        Evaluation.Evidence.Add(TEXT("三大墮落徵象已全"));
    }
    // 2. 道權刻 - 被監督系統發現
    else
    {
        FCorruptionCheckResult Check = PerformFullCorruptionCheck();
        if (Check.OverallCorruptionRisk > 0.8f)
        {
            Evaluation.bTransferRequired = true;
            
            // 找出最嚴重的徵象
            ECorruptionSign WorstSign = ECorruptionSign::RationalizationOfEvil;
            float WorstSeverity = 0.0f;
            
            for (const auto& Sign : DetectedSigns)
            {
                if (Sign.Severity > WorstSeverity)
                {
                    WorstSeverity = Sign.Severity;
                    WorstSign = Sign.SignType;
                }
            }
            
            Evaluation.TriggeringSign = WorstSign;
            Evaluation.Reason = FString::Printf(TEXT("%s徵象嚴重，道權發令交接"),
                *UEnum::GetDisplayValueAsText(WorstSign).ToString());
        }
    }
    
    Evaluation.RecommendedTransferTime = FDateTime::Now();
    
    return Evaluation;
}

void UMingCorruptionPrevention::RecordDecision(const FDecisionHistory& Decision)
{
    DecisionHistory.Add(Decision);
    TotalDecisions++;
    
    if (Decision.PrimaryForce == EForceType::Evil)
    {
        EvilForceUsageCount++;
    }
    
    // 檢測異常
    if (DetectBehaviorAnomaly(Decision))
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 檢測到異常決策模式"));
    }
}

float UMingCorruptionPrevention::CheckJustificationEvolution() const
{
    float TotalEvolution = 0.0f;
    
    for (const auto& Justification : SelfJustifications)
    {
        TotalEvolution += Justification.EvolutionLevel;
    }
    
    return SelfJustifications.Num() > 0 ? 
        TotalEvolution / SelfJustifications.Num() : 0.0f;
}

float UMingCorruptionPrevention::AnalyzeFollowerDeification() const
{
    float TotalDeification = 0.0f;
    
    for (const auto& Relationship : FollowerRelationships)
    {
        TotalDeification += Relationship.DeificationLevel;
    }
    
    return FollowerRelationships.Num() > 0 ? 
        TotalDeification / FollowerRelationships.Num() : 0.0f;
}

float UMingCorruptionPrevention::AnalyzeOppositionDemonization() const
{
    float TotalDemonization = 0.0f;
    
    for (const auto& Treatment : OppositionTreatments)
    {
        TotalDemonization += Treatment.DemonizationLevel;
    }
    
    return OppositionTreatments.Num() > 0 ? 
        TotalDemonization / OppositionTreatments.Num() : 0.0f;
}

bool UMingCorruptionPrevention::IssueWarning(EWarningLevel Level, const FWarningContent& Content)
{
    UE_LOG(LogTemp, Warning, TEXT("=== %s戒令 ==="), 
        *UEnum::GetDisplayValueAsText(Level).ToString());
    UE_LOG(LogTemp, Warning, TEXT("標題: %s"), *Content.Title);
    UE_LOG(LogTemp, Warning, TEXT("內容: %s"), *Content.Message);
    
    if (!Content.RecommendedAction.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("建議行動: %s"), *Content.RecommendedAction);
    }
    
    return true;
}

bool UMingCorruptionPrevention::IssueStopCommand(const FString& Reason)
{
    UE_LOG(LogTemp, Error, TEXT("=== 停止命令 ==="));
    UE_LOG(LogTemp, Error, TEXT("原因: %s"), *Reason);
    UE_LOG(LogTemp, Error, TEXT("所有行動必須立即停止"));
    
    return true;
}

bool UMingCorruptionPrevention::IssueSealCommand()
{
    UE_LOG(LogTemp, Error, TEXT("=== 封存命令 ==="));
    UE_LOG(LogTemp, Error, TEXT("立即封存所有邪兵"));
    UE_LOG(LogTemp, Error, TEXT("切斷所有單線聯絡"));
    UE_LOG(LogTemp, Error, TEXT":"銷毀所有痕跡"));
    
    return true;
}

FSelfAuditReport UMingCorruptionPrevention::GenerateSelfAuditReport()
{
    FSelfAuditReport Report;
    
    Report.AuditTime = FDateTime::Now();
    Report.TotalCommandsIssued = TotalDecisions;
    Report.EvilForceUsages = EvilForceUsageCount;
    Report.CorruptionStatus = PerformFullCorruptionCheck();
    
    // 獲取關鍵決策
    for (const auto& Decision : DecisionHistory)
    {
        if (Decision.MoralAmbiguity > 0.6f || IsReverseStrategy(Decision.UsedStrategy))
        {
            Report.CriticalDecisions.Add(Decision.DecisionDescription);
        }
    }
    
    // 計算平均決策品質
    float TotalQuality = 0.0f;
    int32 Count = 0;
    for (const auto& Decision : DecisionHistory)
    {
        // 簡化的品質計算
        float Quality = 1.0f - Decision.MoralAmbiguity;
        if (Decision.SuccessRate > 0)
        {
            Quality *= Decision.SuccessRate;
        }
        TotalQuality += Quality;
        Count++;
    }
    Report.AverageDecisionQuality = Count > 0 ? TotalQuality / Count : 0.0f;
    
    // 生成建議
    FString Recommendations;
    if (Report.CorruptionStatus.bRationalizationOfEvil)
    {
        Recommendations += TEXT("1. 立即審查所有自我合理化理由\n");
    }
    if (Report.CorruptionStatus.bFollowerDeification)
    {
        Recommendations += TEXT("2. 恢復正常的追隨者評價機制\n");
    }
    if (Report.CorruptionStatus.bOppositionDemonization)
    {
        Recommendations += TEXT("3. 重新開啟與反對者的對話渠道\n");
    }
    if (Report.CorruptionStatus.bIAmJusticeSyndrome)
    {
        Recommendations += TEXT("4. 【緊急】執行權力交接程序\n");
    }
    
    Report.Recommendations = Recommendations;
    
    LastAuditTime = FDateTime::Now();
    
    return Report;
}

bool UMingCorruptionPrevention::PerformDailySelfCheck()
{
    UE_LOG(LogTemp, Log, TEXT("=== 執行日常自我檢查 ==="));
    
    // 簡單檢查
    FCorruptionCheckResult Check = PerformFullCorruptionCheck();
    
    if (Check.OverallCorruptionRisk < 0.3f)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 自我檢查通過，狀態良好"));
        return true;
    }
    else if (Check.OverallCorruptionRisk < 0.6f)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 自我檢查發現輕微風險，需要關注"));
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("✗ 自我檢查發現嚴重風險，需要立即行動"));
        return false;
    }
}

float UMingCorruptionPrevention::CalculateOverallCorruptionRisk() const
{
    float Risk = 0.0f;
    
    // 1. 三大徵象風險
    for (const auto& Sign : DetectedSigns)
    {
        Risk += Sign.Severity * 0.2f;
    }
    
    // 2. 邪兵使用風險
    if (TotalDecisions > 0)
    {
        float EvilRatio = (float)EvilForceUsageCount / TotalDecisions;
        Risk += EvilRatio * 0.2f;
    }
    
    // 3. 自我合理化風險
    float JustificationEvolution = CheckJustificationEvolution();
    Risk += JustificationEvolution * 0.2f;
    
    // 4. 追隨者風險
    float FollowerRisk = AnalyzeFollowerDeification();
    Risk += FollowerRisk * 0.2f;
    
    // 5. 反對者風險
    float OppositionRisk = AnalyzeOppositionDemonization();
    Risk += OppositionRisk * 0.2f;
    
    return FMath::Clamp(Risk, 0.0f, 1.0f);
}

bool UMingCorruptionPrevention::DetectBehaviorAnomaly(const FDecisionHistory& Decision) const
{
    // 檢測異常決策模式
    bool bAnomaly = false;
    
    // 短時間內多次使用逆策
    if (IsReverseStrategy(Decision.UsedStrategy))
    {
        int32 RecentReverseCount = 0;
        for (int32 i = FMath::Max(0, DecisionHistory.Num() - 5); i < DecisionHistory.Num(); i++)
        {
            if (IsReverseStrategy(DecisionHistory[i].UsedStrategy))
            {
                RecentReverseCount++;
            }
        }
        
        if (RecentReverseCount >= 4)
        {
            bAnomaly = true;
        }
    }
    
    return bAnomaly;
}

bool UMingCorruptionPrevention::IsReverseStrategy(ETwelveStrategies Strategy) const
{
    return Strategy == ETwelveStrategies::BreakSituation ||
           Strategy == ETwelveStrategies::BreakStructure ||
           Strategy == ETwelveStrategies::BreakNotPerson;
}

TArray<FCorruptionIndicator> UMingCorruptionPrevention::GetActiveCorruptionSigns() const
{
    TArray<FCorruptionIndicator> ActiveSigns;
    
    for (const auto& Sign : DetectedSigns)
    {
        if (Sign.Severity > 0.5f)
        {
            ActiveSigns.Add(Sign);
        }
    }
    
    return ActiveSigns;
}

bool UMingCorruptionPrevention::IsCorruptionSignPresent(ECorruptionSign Sign) const
{
    for (const auto& DetectedSign : DetectedSigns)
    {
        if (DetectedSign.SignType == Sign && DetectedSign.Severity > 0.5f)
        {
            return true;
        }
    }
    return false;
}

TArray<FString> UMingCorruptionPrevention::GetImprovementRecommendations() const
{
    TArray<FString> Recommendations;
    
    if (CheckJustificationEvolution() > 0.5f)
    {
        Recommendations.Add(TEXT("減少自我合理化，接受批評"));
    }
    
    if (AnalyzeFollowerDeification() > 0.5f)
    {
        Recommendations.Add(TEXT("降低追隨者特殊待遇，恢復公平機制"));
    }
    
    if (AnalyzeOppositionDemonization() > 0.5f)
    {
        Recommendations.Add(TEXT("重新開放與反對者的對話"));
    }
    
    return Recommendations;
}

float UMingCorruptionPrevention::PredictCorruptionTrend(int32 FutureDays) const
{
    // 簡化的趨勢預測
    float CurrentRisk = CalculateOverallCorruptionRisk();
    float Trend = 0.0f;
    
    // 基於當前模式預測
    if (EvilForceUsageCount > TotalDecisions * 0.3f)
    {
        Trend += 0.1f;
    }
    
    if (CheckJustificationEvolution() > 0.3f)
    {
        Trend += 0.1f;
    }
    
    float PredictedRisk = CurrentRisk + Trend * FutureDays;
    return FMath::Clamp(PredictedRisk, 0.0f, 1.0f);
}

TMap<ECorruptionSign, float> UMingCorruptionPrevention::GetRiskHeatmap() const
{
    TMap<ECorruptionSign, float> Heatmap;
    
    Heatmap.Add(ECorruptionSign::RationalizationOfEvil, 0.0f);
    Heatmap.Add(ECorruptionSign::FollowerDeification, 0.0f);
    Heatmap.Add(ECorruptionSign::OppositionDemonization, 0.0f);
    Heatmap.Add(ECorruptionSign::IAmJusticeSyndrome, 0.0f);
    
    for (const auto& Sign : DetectedSigns)
    {
        Heatmap.Add(Sign.SignType, Sign.Severity);
    }
    
    return Heatmap;
}

TArray<FBehaviorPattern> UMingCorruptionPrevention::AnalyzeDecisionPatterns() const
{
    TArray<FBehaviorPattern> Patterns;
    
    // 簡化的模式分析
    if (EvilForceUsageCount > 3)
    {
        FBehaviorPattern Pattern;
        Pattern.PatternName = TEXT("頻繁使用邪兵");
        Pattern.RiskScore = (float)EvilForceUsageCount / TotalDecisions;
        Pattern.bIsWarningPattern = Pattern.RiskScore > 0.4f;
        Patterns.Add(Pattern);
    }
    
    return Patterns;
}

bool UMingCorruptionPrevention::HasReachedSupremeRuleLimit() const
{
    return CheckIAmJusticeSyndrome();
}

void UMingCorruptionPrevention::RecordFollowerRelationship(const FFollowerRelationship& Relationship)
{
    // 查找是否已存在
    for (int32 i = 0; i < FollowerRelationships.Num(); i++)
    {
        if (FollowerRelationships[i].FollowerID == Relationship.FollowerID)
        {
            FollowerRelationships[i] = Relationship;
            return;
        }
    }
    
    // 添加新關係
    FollowerRelationships.Add(Relationship);
}

void UMingCorruptionPrevention::RecordOppositionTreatment(const FOppositionTreatment& Treatment)
{
    // 查找是否已存在
    for (int32 i = 0; i < OppositionTreatments.Num(); i++)
    {
        if (OppositionTreatments[i].OppositionID == Treatment.OppositionID)
        {
            OppositionTreatments[i] = Treatment;
            return;
        }
    }
    
    // 添加新記錄
    OppositionTreatments.Add(Treatment);
}

bool UMingCorruptionPrevention::IssueTransferCommand(const FString& SuccessorID)
{
    UE_LOG(LogTemp, Error, TEXT("=== 權力交接命令 ==="));
    UE_LOG(LogTemp, Error, TEXT("繼承者ID: %s"), *SuccessorID);
    UE_LOG(LogTemp, Error, TEXT("立即啟動交接程序"));
    UE_LOG(LogTemp, Error, TEXT("1. 封存所有邪兵"));
    UE_LOG(LogTemp, Error, TEXT("2. 斷開所有暗線"));
    UE_LOG(LogTemp, Error, TEXT("3. 公開傳位"));
    UE_LOG(LogTemp, Error, TEXT("4. 退隱山林"));
    
    return true;
}
