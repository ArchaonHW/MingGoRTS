// 智能建議系統使用示例

// 1. 初始化系統
UMingIntelligentRecommendationSystem* RecommendationSystem = NewObject<UMingIntelligentRecommendationSystem>();
RecommendationSystem->Initialize();

// 2. 綁定事件
RecommendationSystem->OnRecommendationGenerated.AddDynamic(this, &AMyActor::OnRecommendationGenerated);
RecommendationSystem->OnRecommendationAccepted.AddDynamic(this, &AMyActor::OnRecommendationAccepted);

// 3. 生成建議
FString Context = TEXT("MidGame");
TArray<FRecommendationData> Recommendations = RecommendationSystem->GenerateRecommendations(Context);

// 4. 獲取特定類型建議
TArray<FRecommendationData> StrategicRecommendations = RecommendationSystem->GetRecommendationsByType(ERecommendationType::Strategic);

// 5. 處理建議
if (Recommendations.Num() > 0)
{
    const FRecommendationData& TopRecommendation = Recommendations[0];
    
    // 接受建議
    RecommendationSystem->AcceptRecommendation(TopRecommendation.RecommendationID);
    
    // 提供反饋
    RecommendationSystem->ProvideFeedback(TopRecommendation.RecommendationID, TEXT("非常有用的建議"), 5);
}

// 4. 獲取統計信息
FRecommendationStatistics Stats = RecommendationSystem->GetRecommendationStatistics();
UE_LOG(LogTemp, Log, TEXT("總建議數: %d, 成功率: %.1f%%"), 
    Stats.TotalRecommendations, Stats.SuccessRate);

// 5. 獲取趨勢分析
TArray<FString> Trends = RecommendationSystem->GetRecommendationTrends();
for (const FString& Trend : Trends)
{
    UE_LOG(LogTemp, Log, TEXT("趨勢: %s"), *Trend);
}
