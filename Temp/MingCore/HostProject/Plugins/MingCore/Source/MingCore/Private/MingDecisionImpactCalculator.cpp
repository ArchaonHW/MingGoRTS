#include "MingDecisionImpactCalculator.h"
#include "MingHistoricalDecisionManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DateTime.h"

UMingDecisionImpactCalculator::UMingDecisionImpactCalculator()
{
    HistoricalPaths.Empty();
    FactionData.Empty();
    ImpactWeights.Empty();
    HistoricalKnowledgeBase.Empty();
}

FMingDecisionImpactReport UMingDecisionImpactCalculator::CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID)
{
    FMingDecisionImpactReport Report;
    Report.DecisionID = DecisionID;
    Report.ChosenOptionID = ChosenOptionID;
    Report.CalculationTime = FDateTime::Now();

    UE_LOG(LogTemp, Log, TEXT("開始計算決策影響: 決策ID %d, 選項 %d"), DecisionID, ChosenOptionID);

    // 初始化數據
    InitializeHistoricalPaths();
    InitializeFactionData();
    InitializeImpactWeights();
    InitializeHistoricalKnowledge();

    // 計算各種影響
    Report.AffectedPaths = CalculateHistoricalPathImpact(DecisionID, ChosenOptionID);
    Report.ReputationChanges = CalculateReputationImpact(DecisionID, ChosenOptionID);
    Report.TerritoryChanges = CalculateTerritoryImpact(DecisionID, ChosenOptionID);
    Report.UpdatedEventConditions = UpdateEventTriggerConditions(DecisionID, ChosenOptionID);
    Report.EducationalSummary = GenerateEducationalContent(DecisionID, ChosenOptionID);

    // 生成詳細影響
    Report.ImpactDetails = GenerateImpactDetails(DecisionID, ChosenOptionID);

    // 計算總體影響評分
    Report.OverallImpactScore = CalculateImpactScore(Report.ImpactDetails);

    // 更新歷史路徑權重
    UpdateHistoricalPathWeights(Report);

    // 觸發事件
    OnImpactCalculated.Broadcast(Report);

    UE_LOG(LogTemp, Log, TEXT("決策影響計算完成，總體評分: %.2f"), Report.OverallImpactScore);

    return Report;
}

TArray<FMingHistoricalPath> UMingDecisionImpactCalculator::CalculateHistoricalPathImpact(int32 DecisionID, int32 ChosenOptionID)
{
    TArray<FMingHistoricalPath> AffectedPaths;

    // 根據決策ID和選項ID計算歷史路徑影響
    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            // 創建直系勝利路徑
            FMingHistoricalPath Path1;
            Path1.PathID = 101;
            Path1.PathName = TEXT("直系統一之路");
            Path1.PathDescription = TEXT("吳佩孚領導的直系軍閥最終統一中國，建立穩定的中央政府");
            Path1.PathTags = {TEXT("統一"), TEXT("穩定"), TEXT("中央集權")};
            Path1.PathWeight = 0.7f;
            Path1.bIsMainPath = true;
            AffectedPaths.Add(Path1);

            // 創建軍事政變路徑
            FMingHistoricalPath Path2;
            Path2.PathID = 102;
            Path2.PathName = TEXT("軍事政變之路");
            Path2.PathDescription = TEXT("直系內部發生軍事政變，導致政局不穩");
            Path2.PathTags = {TEXT("政變"), TEXT("混亂"), TEXT("分裂")};
            Path2.PathWeight = 0.3f;
            Path2.bIsMainPath = false;
            AffectedPaths.Add(Path2);
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            // 創建皖系復興路徑
            FMingHistoricalPath Path1;
            Path1.PathID = 201;
            Path1.PathName = TEXT("皖系復興之路");
            Path1.PathDescription = TEXT("段祺瑞領導的皖系軍閥重新崛起，實現政治改革");
            Path1.PathTags = {TEXT("改革"), TEXT("復興"), TEXT("民主")};
            Path1.PathWeight = 0.6f;
            Path1.bIsMainPath = true;
            AffectedPaths.Add(Path1);

            // 創建外國干涉路徑
            FMingHistoricalPath Path2;
            Path2.PathID = 202;
            Path2.PathName = TEXT("外國干涉之路");
            Path2.PathDescription = TEXT("外國勢力利用皖系內部分歧，加強對中國的控制");
            Path2.PathTags = {TEXT("干涉"), TEXT("殖民地"), TEXT("屈辱")};
            Path2.PathWeight = 0.4f;
            Path2.bIsMainPath = false;
            AffectedPaths.Add(Path2);
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            // 創建地方割據路徑
            FMingHistoricalPath Path1;
            Path1.PathID = 301;
            Path1.PathName = TEXT("地方割據之路");
            Path1.PathDescription = TEXT("各地方軍閥各自為政，中國進入長期分裂狀態");
            Path1.PathTags = {TEXT("分裂"), TEXT("割據"), TEXT("混戰")};
            Path1.PathWeight = 0.8f;
            Path1.bIsMainPath = true;
            AffectedPaths.Add(Path1);

            // 創和平發展路徑
            FMingHistoricalPath Path2;
            Path2.PathID = 302;
            Path2.PathName = TEXT("和平發展之路");
            Path2.PathDescription = TEXT("各派系達成和平協議，共同發展經濟");
            Path2.PathTags = {TEXT("和平"), TEXT("發展"), TEXT("合作")};
            Path2.PathWeight = 0.2f;
            Path2.bIsMainPath = false;
            AffectedPaths.Add(Path2);
        }
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        if (ChosenOptionID == 4) // 發展工業
        {
            // 創建工業強國路徑
            FMingHistoricalPath Path1;
            Path1.PathID = 401;
            Path1.PathName = TEXT("工業強國之路");
            Path1.PathDescription = TEXT("重工業發展帶動軍事現代化，中國成為地區強國");
            Path1.PathTags = {TEXT("工業化"), TEXT("強國"), TEXT("現代化")};
            Path1.PathWeight = 0.6f;
            Path1.bIsMainPath = true;
            AffectedPaths.Add(Path1);
        }
        else if (ChosenOptionID == 5) // 發展農業
        {
            // 創建農業大國路徑
            FMingHistoricalPath Path1;
            Path1.PathID = 501;
            Path1.PathName = TEXT("農業大國之路");
            Path1.PathDescription = TEXT("農業發展帶來社會穩定，為工業化奠定基礎");
            Path1.PathTags = {TEXT("農業"), TEXT("穩定"), TEXT("基礎")};
            Path1.PathWeight = 0.7f;
            Path1.bIsMainPath = true;
            AffectedPaths.Add(Path1);
        }
    }

    return AffectedPaths;
}

TMap<FString, float> UMingDecisionImpactCalculator::CalculateReputationImpact(int32 DecisionID, int32 ChosenOptionID)
{
    TMap<FString, float> ReputationChanges;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            ReputationChanges = {
                {TEXT("軍事聲望"), 25.0f},
                {TEXT("政治聲望"), 20.0f},
                {TEXT("民眾聲望"), 10.0f},
                {TEXT("國際聲望"), 15.0f}
            };
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            ReputationChanges = {
                {TEXT("軍事聲望"), 15.0f},
                {TEXT("政治聲望"), 18.0f},
                {TEXT("民眾聲望"), 8.0f},
                {TEXT("國際聲望"), 12.0f}
            };
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            ReputationChanges = {
                {TEXT("軍事聲望"), -5.0f},
                {TEXT("政治聲望"), -8.0f},
                {TEXT("民眾聲望"), 20.0f},
                {TEXT("國際聲望"), 5.0f}
            };
        }
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        if (ChosenOptionID == 4) // 發展工業
        {
            ReputationChanges = {
                {TEXT("經濟聲望"), 30.0f},
                {TEXT("軍事聲望"), 20.0f},
                {TEXT("民眾聲望"), -10.0f},
                {TEXT("國際聲望"), 25.0f}
            };
        }
        else if (ChosenOptionID == 5) // 發展農業
        {
            ReputationChanges = {
                {TEXT("經濟聲望"), 20.0f},
                {TEXT("軍事聲望"), 5.0f},
                {TEXT("民眾聲望"), 25.0f},
                {TEXT("國際聲望"), 10.0f}
            };
        }
    }

    return ReputationChanges;
}

TMap<FString, int32> UMingDecisionImpactCalculator::CalculateTerritoryImpact(int32 DecisionID, int32 ChosenOptionID)
{
    TMap<FString, int32> TerritoryChanges;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            TerritoryChanges = {
                {TEXT("直系控制區"), 5},
                {TEXT("皖系控制區"), -3},
                {TEXT("奉系控制區"), -2}
            };
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            TerritoryChanges = {
                {TEXT("皖系控制區"), 4},
                {TEXT("直系控制區"), -2},
                {TEXT("奉系控制區"), -2}
            };
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            TerritoryChanges = {
                {TEXT("個人控制區"), 3},
                {TEXT("直系控制區"), -1},
                {TEXT("皖系控制區"), -1},
                {TEXT("奉系控制區"), -1}
            };
        }
    }

    return TerritoryChanges;
}

FString UMingDecisionImpactCalculator::GenerateEducationalContent(int32 DecisionID, int32 ChosenOptionID)
{
    FString EducationalContent;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        EducationalContent = TEXT("1920年的軍閥選擇是中國近代史的重要轉折點。直系軍閥以吳佩孚為首，控制華北地區；皖系軍閥以段祺瑞為首，影響力主要在華中；奉系軍閥以張作霖為首，統治東北。您的選擇不僅影響個人前途，更關係到中國的統一與分裂。歷史告訴我們，軍閥混戰最終導致國力衰弱，為外國侵略提供了機會。");
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        EducationalContent = TEXT("1920年代的中國面臨工業化與傳統農業的選擇。重工業發展雖然能快速提升國力，但需要大量資源且可能加劇社會矛盾；農業發展雖然進程較慢，但能保障民生穩定。歷史上的工業化國家都經歷了痛苦的轉型期，而成功的農業改革則為長期發展奠定了基礎。");
    }

    return EducationalContent;
}

TArray<FString> UMingDecisionImpactCalculator::UpdateEventTriggerConditions(int32 DecisionID, int32 ChosenOptionID)
{
    TArray<FString> UpdatedConditions;

    if (DecisionID == 1) // 選擇效忠對象決策
    {
        if (ChosenOptionID == 1) // 效忠直系
        {
            UpdatedConditions.Add(TEXT("觸發直皖戰爭事件"));
            UpdatedConditions.Add(TEXT("解鎖華北地區決策"));
            UpdatedConditions.Add(TEXT("增加軍事衝突概率"));
        }
        else if (ChosenOptionID == 2) // 效忠皖系
        {
            UpdatedConditions.Add(TEXT("觸發皖系復興事件"));
            UpdatedConditions.Add(TEXT("解鎖華中地區決策"));
            UpdatedConditions.Add(TEXT("增加外交事件概率"));
        }
        else if (ChosenOptionID == 3) // 保持中立
        {
            UpdatedConditions.Add(TEXT("觸發地方割據事件"));
            UpdatedConditions.Add(TEXT("解鎖獨立發展決策"));
            UpdatedConditions.Add(TEXT("增加經濟發展機會"));
        }
    }
    else if (DecisionID == 2) // 經濟發展政策決策
    {
        if (ChosenOptionID == 4) // 發展工業
        {
            UpdatedConditions.Add(TEXT("觸發工業化事件"));
            UpdatedConditions.Add(TEXT("解鎖軍事現代化決策"));
            UpdatedConditions.Add(TEXT("增加技術引進機會"));
        }
        else if (ChosenOptionID == 5) // 發展農業
        {
            UpdatedConditions.Add(TEXT("觸發農業改革事件"));
            UpdatedConditions.Add(TEXT("解鎖社會福利決策"));
            UpdatedConditions.Add(TEXT("增加民生改善機會"));
        }
    }

    return UpdatedConditions;
}

EMingImpactMagnitude UMingDecisionImpactCalculator::GetImpactMagnitude(float ImpactValue)
{
    if (ImpactValue >= 40.0f)
    {
        return EMingImpactMagnitude::Catastrophic;
    }
    else if (ImpactValue >= 30.0f)
    {
        return EMingImpactMagnitude::Major;
    }
    else if (ImpactValue >= 20.0f)
    {
        return EMingImpactMagnitude::Significant;
    }
    else if (ImpactValue >= 10.0f)
    {
        return EMingImpactMagnitude::Moderate;
    }
    else if (ImpactValue >= 5.0f)
    {
        return EMingImpactMagnitude::Minor;
    }
    else
    {
        return EMingImpactMagnitude::Minimal;
    }
}

FString UMingDecisionImpactCalculator::GetImpactDescription(EMingImpactMagnitude Magnitude)
{
    switch (Magnitude)
    {
    case EMingImpactMagnitude::Minimal:
        return TEXT("極小影響");
    case EMingImpactMagnitude::Minor:
        return TEXT("輕微影響");
    case EMingImpactMagnitude::Moderate:
        return TEXT("中等影響");
    case EMingImpactMagnitude::Significant:
        return TEXT("顯著影響");
    case EMingImpactMagnitude::Major:
        return TEXT("重大影響");
    case EMingImpactMagnitude::Catastrophic:
        return TEXT("災難性影響");
    default:
        return TEXT("未知影響");
    }
}

TArray<FMingDecisionImpactDetail> UMingDecisionImpactCalculator::SimulateLongTermImpact(int32 DecisionID, int32 ChosenOptionID, int32 SimulationYears)
{
    TArray<FMingDecisionImpactDetail> LongTermImpacts;

    // 模擬長期影響
    for (int32 Year = 1; Year <= SimulationYears; Year++)
    {
        FMingDecisionImpactDetail Impact;
        Impact.StartYear = Year;
        Impact.DurationYears = SimulationYears - Year + 1;

        if (DecisionID == 1 && ChosenOptionID == 1) // 效忠直系的長期影響
        {
            if (Year <= 3)
            {
                Impact.ImpactType = EMingImpactType::ShortTerm;
                Impact.ImpactScope = EMingImpactScope::Regional;
                Impact.ImpactMagnitude = EMingImpactMagnitude::Significant;
                Impact.ImpactDescription = TEXT("直系軍事實力快速增長");
                Impact.ImpactValue = 25.0f;
            }
            else if (Year <= 10)
            {
                Impact.ImpactType = EMingImpactType::MediumTerm;
                Impact.ImpactScope = EMingImpactScope::National;
                Impact.ImpactMagnitude = EMingImpactMagnitude::Major;
                Impact.ImpactDescription = TEXT("直系逐漸統一全國");
                Impact.ImpactValue = 35.0f;
            }
            else
            {
                Impact.ImpactType = EMingImpactType::LongTerm;
                Impact.ImpactScope = EMingImpactScope::Historical;
                Impact.ImpactMagnitude = EMingImpactMagnitude::Catastrophic;
                Impact.ImpactDescription = TEXT("建立中央集權政府");
                Impact.ImpactValue = 45.0f;
            }
        }

        LongTermImpacts.Add(Impact);
    }

    return LongTermImpacts;
}

TMap<int32, FMingDecisionImpactReport> UMingDecisionImpactCalculator::CompareDecisionOptions(int32 DecisionID)
{
    TMap<int32, FMingDecisionImpactReport> ComparisonResults;

    // 這裡應該從決策管理器獲取所有可用選項
    // 暫時使用硬編碼的選項ID
    TArray<int32> OptionIDs = {1, 2, 3}; // 假設有三個選項

    for (int32 OptionID : OptionIDs)
    {
        FMingDecisionImpactReport Report = CalculateDecisionImpact(DecisionID, OptionID);
        ComparisonResults.Add(OptionID, Report);
    }

    return ComparisonResults;
}

TArray<FMingHistoricalPath> UMingDecisionImpactCalculator::GetCurrentHistoricalPaths()
{
    return HistoricalPaths;
}

float UMingDecisionImpactCalculator::CalculateDecisionWeight(int32 DecisionID, int32 ChosenOptionID)
{
    float Weight = 1.0f;

    // 根據決策類型和選項特性計算權重
    if (DecisionID == 1) // 關鍵政治決策
    {
        Weight *= 1.5f;
    }
    else if (DecisionID == 2) // 重要經濟決策
    {
        Weight *= 1.2f;
    }

    // 根據選項影響調整權重
    FMingDecisionImpactReport Report = CalculateDecisionImpact(DecisionID, ChosenOptionID);
    Weight *= Report.OverallImpactScore / 100.0f;

    return FMath::Clamp(Weight, 0.1f, 2.0f);
}

FMingDecisionImpactReport UMingDecisionImpactCalculator::GenerateImpactPrediction(int32 DecisionID, int32 OptionID)
{
    // 生成影響預測（與實際計算相同，但標記為預測）
    FMingDecisionImpactReport Prediction = CalculateDecisionImpact(DecisionID, OptionID);
    
    // 添加預測標記
    Prediction.EducationalSummary = TEXT("[預測] ") + Prediction.EducationalSummary;
    
    return Prediction;
}

void UMingDecisionImpactCalculator::InitializeHistoricalPaths()
{
    if (HistoricalPaths.Num() > 0)
    {
        return; // 已經初始化
    }

    // 初始化主要歷史路徑
    FMingHistoricalPath Path1;
    Path1.PathID = 1;
    Path1.PathName = TEXT("統一之路");
    Path1.PathDescription = TEXT("中國實現統一，建立強大的中央政府");
    Path1.PathTags = {TEXT("統一"), TEXT("強國"), TEXT("穩定")};
    Path1.PathWeight = 0.3f;
    Path1.bIsMainPath = true;
    HistoricalPaths.Add(Path1);

    FMingHistoricalPath Path2;
    Path2.PathID = 2;
    Path2.PathName = TEXT("分裂之路");
    Path2.PathDescription = TEXT("中國持續分裂，軍閥混戰");
    Path2.PathTags = {TEXT("分裂"), TEXT("混戰"), TEXT("衰弱")};
    Path2.PathWeight = 0.4f;
    Path2.bIsMainPath = true;
    HistoricalPaths.Add(Path2);

    FMingHistoricalPath Path3;
    Path3.PathID = 3;
    Path3.PathName = TEXT("改革之路");
    Path3.PathDescription = TEXT("通過政治改革實現現代化");
    Path3.PathTags = {TEXT("改革"), TEXT("現代化"), TEXT("民主")};
    Path3.PathWeight = 0.2f;
    Path3.bIsMainPath = false;
    HistoricalPaths.Add(Path3);

    FMingHistoricalPath Path4;
    Path4.PathID = 4;
    Path4.PathName = TEXT("殖民之路");
    Path4.PathDescription = TEXT("外國勢力控制中國，淪為半殖民地");
    Path4.PathTags = {TEXT("殖民"), TEXT("屈辱"), TEXT("依賴")};
    Path4.PathWeight = 0.1f;
    Path4.bIsMainPath = false;
    HistoricalPaths.Add(Path4);
}

void UMingDecisionImpactCalculator::InitializeFactionData()
{
    if (FactionData.Num() > 0)
    {
        return; // 已經初始化
    }

    // 初始化主要軍閥勢力
    FMingTerritoryData Zhili;
    Zhili.FactionName = TEXT("直系");
    Zhili.ControlledProvinces = 8;
    Zhili.ControlledCities = 45;
    Zhili.MilitaryStrength = 75.0f;
    Zhili.EconomicStrength = 65.0f;
    Zhili.PopularSupport = 60.0f;
    FactionData.Add(TEXT("直系"), Zhili);

    FMingTerritoryData Wanxi;
    Wanxi.FactionName = TEXT("皖系");
    Wanxi.ControlledProvinces = 5;
    Wanxi.ControlledCities = 28;
    Wanxi.MilitaryStrength = 60.0f;
    Wanxi.EconomicStrength = 55.0f;
    Wanxi.PopularSupport = 45.0f;
    FactionData.Add(TEXT("皖系"), Wanxi);

    FMingTerritoryData Fengxi;
    Fengxi.FactionName = TEXT("奉系");
    Fengxi.ControlledProvinces = 6;
    Fengxi.ControlledCities = 32;
    Fengxi.MilitaryStrength = 70.0f;
    Fengxi.EconomicStrength = 50.0f;
    Fengxi.PopularSupport = 55.0f;
    FactionData.Add(TEXT("奉系"), Fengxi);
}

void UMingDecisionImpactCalculator::InitializeImpactWeights()
{
    if (ImpactWeights.Num() > 0)
    {
        return; // 已經初始化
    }

    ImpactWeights = {
        {EMingDecisionType::Political, 1.5f},
        {EMingDecisionType::Military, 1.3f},
        {EMingDecisionType::Economic, 1.2f},
        {EMingDecisionType::Diplomatic, 1.1f},
        {EMingDecisionType::Personal, 0.8f}
    };
}

void UMingDecisionImpactCalculator::InitializeHistoricalKnowledge()
{
    if (HistoricalKnowledgeBase.Num() > 0)
    {
        return; // 已經初始化
    }

    HistoricalKnowledgeBase = {
        {1, TEXT("1920年直皖戰爭是北洋政府內部的重要衝突，標誌著直系軍閥的崛起。")},
        {2, TEXT("1920年代的中國經濟政策決定了後來的工業化道路。")},
        {3, TEXT("軍閥時期的政治選擇直接影響了中國的統一進程。")}
    };
}

float UMingDecisionImpactCalculator::CalculateImpactScore(const TArray<FMingDecisionImpactDetail>& Impacts)
{
    float TotalScore = 0.0f;

    for (const FMingDecisionImpactDetail& Impact : Impacts)
    {
        float Weight = 1.0f;

        // 根據影響類型調整權重
        switch (Impact.ImpactType)
        {
        case EMingImpactType::Permanent:
            Weight *= 2.0f;
            break;
        case EMingImpactType::LongTerm:
            Weight *= 1.5f;
            break;
        case EMingImpactType::MediumTerm:
            Weight *= 1.2f;
            break;
        case EMingImpactType::ShortTerm:
            Weight *= 1.0f;
            break;
        case EMingImpactType::Immediate:
            Weight *= 0.8f;
            break;
        }

        // 根據影響範圍調整權重
        switch (Impact.ImpactScope)
        {
        case EMingImpactScope::Historical:
            Weight *= 2.0f;
            break;
        case EMingImpactScope::International:
            Weight *= 1.8f;
            break;
        case EMingImpactScope::National:
            Weight *= 1.5f;
            break;
        case EMingImpactScope::Regional:
            Weight *= 1.2f;
            break;
        case EMingImpactScope::Personal:
            Weight *= 1.0f;
            break;
        }

        // 根據影響程度調整權重
        float MagnitudeWeight = 1.0f;
        switch (Impact.ImpactMagnitude)
        {
        case EMingImpactMagnitude::Catastrophic:
            MagnitudeWeight = 3.0f;
            break;
        case EMingImpactMagnitude::Major:
            MagnitudeWeight = 2.5f;
            break;
        case EMingImpactMagnitude::Significant:
            MagnitudeWeight = 2.0f;
            break;
        case EMingImpactMagnitude::Moderate:
            MagnitudeWeight = 1.5f;
            break;
        case EMingImpactMagnitude::Minor:
            MagnitudeWeight = 1.0f;
            break;
        case EMingImpactMagnitude::Minimal:
            MagnitudeWeight = 0.5f;
            break;
        }

        TotalScore += FMath::Abs(Impact.ImpactValue) * Weight * MagnitudeWeight;
    }

    return FMath::Clamp(TotalScore, 0.0f, 100.0f);
}

void UMingDecisionImpactCalculator::UpdateHistoricalPathWeights(const FMingDecisionImpactReport& Report)
{
    for (const FMingHistoricalPath& AffectedPath : Report.AffectedPaths)
    {
        // 更新對應歷史路徑的權重
        for (FMingHistoricalPath& Path : HistoricalPaths)
        {
            if (Path.PathID == AffectedPath.PathID)
            {
                float OldWeight = Path.PathWeight;
                Path.PathWeight = FMath::Clamp(AffectedPath.PathWeight, 0.0f, 1.0f);
                
                if (FMath::Abs(OldWeight - Path.PathWeight) > 0.01f)
                {
                    OnHistoricalPathChanged.Broadcast(Path.PathName, Path.PathWeight);
                }
                break;
            }
        }
    }
}

TArray<FMingDecisionImpactDetail> UMingDecisionImpactCalculator::GenerateImpactDetails(int32 DecisionID, int32 ChosenOptionID)
{
    TArray<FMingDecisionImpactDetail> Details;

    // 生成詳細影響信息
    FMingDecisionImpactDetail Detail1;
    Detail1.ImpactType = EMingImpactType::Immediate;
    Detail1.ImpactScope = EMingImpactScope::Personal;
    Detail1.ImpactMagnitude = EMingImpactMagnitude::Significant;
    Detail1.ImpactDescription = TEXT("個人聲望和地位立即受到影響");
    Detail1.ImpactValue = 15.0f;
    Detail1.DurationYears = 1;
    Detail1.StartYear = 0;
    Detail1.AffectedAttributes = {TEXT("聲望"), TEXT("地位")};
    Detail1.HistoricalKnowledge = HistoricalKnowledgeBase.FindRef(DecisionID);
    Details.Add(Detail1);

    FMingDecisionImpactDetail Detail2;
    Detail2.ImpactType = EMingImpactType::ShortTerm;
    Detail2.ImpactScope = EMingImpactScope::Regional;
    Detail2.ImpactMagnitude = EMingImpactMagnitude::Moderate;
    Detail2.ImpactDescription = TEXT("地區政治格局發生變化");
    Detail2.ImpactValue = 20.0f;
    Detail2.DurationYears = 3;
    Detail2.StartYear = 1;
    Detail2.AffectedAttributes = {TEXT("控制區"), TEXT("影響力")};
    Details.Add(Detail2);

    return Details;
}

bool UMingDecisionImpactCalculator::ValidateImpactData(const FMingDecisionImpactReport& Report) const
{
    if (Report.DecisionID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("決策ID必須大於0"));
        return false;
    }

    if (Report.ChosenOptionID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("選項ID必須大於0"));
        return false;
    }

    if (Report.ImpactDetails.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("決策影響沒有詳細信息"));
    }

    return true;
}
