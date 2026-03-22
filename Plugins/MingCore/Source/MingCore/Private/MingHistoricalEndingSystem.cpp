#include "MingHistoricalEndingSystem.h"
#include "MingHistoricalDecisionManager.h"
#include "MingHistoricalEventTrigger.h"
#include "MingHistoricalCharacterSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

UMingHistoricalEndingSystem::UMingHistoricalEndingSystem()
{
    bInitialized = false;
    EndingDatabase.Empty();
    AchievementDatabase.Empty();
    PlayerEndingHistory.Empty();
    CurrentScore = FMingEndingScore();
}

bool UMingHistoricalEndingSystem::InitializeEndingSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("歷史結局系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化歷史結局系統..."));

    // 載入預設結局
    LoadDefaultEndings();

    // 載入預設成就
    LoadDefaultAchievements();

    // 載入已保存的結局數據
    LoadEndingData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("歷史結局系統初始化完成，共載入 %d 個結局，%d 個成就"), EndingDatabase.Num(), AchievementDatabase.Num());
    
    return true;
}

bool UMingHistoricalEndingSystem::RegisterHistoricalEnding(const FMingHistoricalEnding& Ending)
{
    if (!ValidateEndingData(Ending))
    {
        UE_LOG(LogTemp, Error, TEXT("結局數據驗證失敗: %s"), *Ending.EndingName);
        return false;
    }

    if (EndingDatabase.Contains(Ending.EndingID))
    {
        UE_LOG(LogTemp, Warning, TEXT("結局ID %d 已存在，將覆蓋"), Ending.EndingID);
    }

    EndingDatabase.Add(Ending.EndingID, Ending);
    
    UE_LOG(LogTemp, Log, TEXT("註冊歷史結局: %s (ID: %d)"), *Ending.EndingName, Ending.EndingID);
    
    return true;
}

FMingEndingScore UMingHistoricalEndingSystem::EvaluatePlayerPath()
{
    UE_LOG(LogTemp, Log, TEXT("開始評價玩家歷史路徑..."));

    FMingEndingScore Score;
    Score.EndingID = -1; // 暫時未分配結局ID

    // 計算各類評分
    Score.DecisionScores = TMap<int32, float>();
    Score.EventScores = TMap<int32, float>();
    Score.RelationshipScores = TMap<FString, float>();

    // 計算決策評分
    float DecisionScore = CalculateDecisionScore();
    Score.CategoryScores.Add(TEXT("決策評分"), DecisionScore);

    // 計算事件評分
    float EventScore = CalculateEventScore();
    Score.CategoryScores.Add(TEXT("事件評分"), EventScore);

    // 計算關係評分
    float RelationshipScore = CalculateRelationshipScore();
    Score.CategoryScores.Add(TEXT("關係評分"), RelationshipScore);

    // 計算歷史準確度
    Score.HistoricalAccuracy = CalculateHistoricalAccuracy();
    Score.CategoryScores.Add(TEXT("歷史準確度"), Score.HistoricalAccuracy);

    // 計算創新性評分
    Score.InnovationScore = CalculateInnovationScore();
    Score.CategoryScores.Add(TEXT("創新性"), Score.InnovationScore);

    // 計算道德評分
    Score.MoralityScore = CalculateMoralityScore();
    Score.CategoryScores.Add(TEXT("道德評分"), Score.MoralityScore);

    // 計算戰略評分
    Score.StrategyScore = CalculateStrategyScore();
    Score.CategoryScores.Add(TEXT("戰略評分"), Score.StrategyScore);

    // 計算總體評分
    Score.TotalScore = (DecisionScore * 0.25f + EventScore * 0.20f + RelationshipScore * 0.15f + 
                       Score.HistoricalAccuracy * 0.15f + Score.InnovationScore * 0.10f + 
                       Score.MoralityScore * 0.10f + Score.StrategyScore * 0.05f);

    // 保存當前評分
    CurrentScore = Score;

    // 觸發評分計算事件
    OnEndingScoreCalculated.Broadcast(Score);

    UE_LOG(LogTemp, Log, TEXT("玩家路徑評價完成，總體評分: %.2f"), Score.TotalScore);
    
    return Score;
}

FMingHistoricalEnding UMingHistoricalEndingSystem::DetermineEnding(const FMingEndingScore& PlayerScore)
{
    UE_LOG(LogTemp, Log, TEXT("確定最終結局..."));

    TArray<FMingHistoricalEnding> AvailableEndings;
    
    // 檢查所有結局的解鎖條件
    for (const TPair<int32, FMingHistoricalEnding>& Pair : EndingDatabase)
    {
        int32 EndingID = Pair.Key;
        const FMingHistoricalEnding& Ending = Pair.Value;
        
        if (CheckEndingConditions(EndingID))
        {
            AvailableEndings.Add(Ending);
        }
    }

    if (AvailableEndings.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("沒有可用的結局，返回默認結局"));
        return GetDefaultEnding();
    }

    // 根據評分選擇最適合的結局
    FMingHistoricalEnding BestEnding = AvailableEndings[0];
    float BestMatchScore = 0.0f;

    for (const FMingHistoricalEnding& Ending : AvailableEndings)
    {
        float MatchScore = CalculateEndingMatchScore(Ending, PlayerScore);
        
        if (MatchScore > BestMatchScore)
        {
            BestMatchScore = MatchScore;
            BestEnding = Ending;
        }
    }

    // 設置結局評價
    BestEnding.EndingRating = GetEndingRating(PlayerScore.TotalScore);

    // 添加到玩家結局歷史
    PlayerEndingHistory.Add(BestEnding);

    // 觸發結局確定事件
    OnEndingDetermined.Broadcast(BestEnding);

    UE_LOG(LogTemp, Log, TEXT("確定結局: %s (評價: %s)"), *BestEnding.EndingName, *GetEndingRatingName(BestEnding.EndingRating));
    
    return BestEnding;
}

bool UMingHistoricalEndingSystem::CheckEndingConditions(int32 EndingID)
{
    if (!EndingDatabase.Contains(EndingID))
    {
        UE_LOG(LogTemp, Error, TEXT("結局ID %d 不存在"), EndingID);
        return false;
    }

    const FMingHistoricalEnding& Ending = EndingDatabase[EndingID];

    // 檢查所有解鎖條件
    for (const FMingEndingCondition& Condition : Ending.UnlockConditions)
    {
        bool bConditionMet = false;

        if (Condition.ConditionType == TEXT("total_score"))
        {
            float RequiredScore = Condition.ConditionParameters.FindRef(TEXT("score"));
            bConditionMet = CompareValues(CurrentScore.TotalScore, RequiredScore, Condition.ComparisonOperator);
        }
        else if (Condition.ConditionType == TEXT("historical_accuracy"))
        {
            float RequiredAccuracy = Condition.ConditionParameters.FindRef(TEXT("accuracy"));
            bConditionMet = CompareValues(CurrentScore.HistoricalAccuracy, RequiredAccuracy, Condition.ComparisonOperator);
        }
        else if (Condition.ConditionType == TEXT("specific_decision"))
        {
            int32 DecisionID = (int32)Condition.ConditionParameters.FindRef(TEXT("decision_id"));
            int32 RequiredChoice = (int32)Condition.ConditionParameters.FindRef(TEXT("choice_id"));
            bConditionMet = CheckSpecificDecision(DecisionID, RequiredChoice);
        }
        else if (Condition.ConditionType == TEXT("relationship"))
        {
            FString CharacterName = Condition.ConditionParameters.FindRef(TEXT("character"));
            float RequiredRelationship = Condition.ConditionParameters.FindRef(TEXT("relationship"));
            bConditionMet = CheckRelationshipCondition(CharacterName, RequiredRelationship, Condition.ComparisonOperator);
        }

        if (Condition.bIsRequired && !bConditionMet)
        {
            return false; // 必要條件不滿足
        }
    }

    return true;
}

FMingHistoricalEnding UMingHistoricalEndingSystem::GetEndingInfo(int32 EndingID) const
{
    if (EndingDatabase.Contains(EndingID))
    {
        return EndingDatabase[EndingID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("結局ID %d 不存在，返回默認結局"), EndingID);
    return GetDefaultEnding();
}

TArray<int32> UMingHistoricalEndingSystem::GetAvailableEndings() const
{
    TArray<int32> AvailableEndingIDs;
    
    for (const TPair<int32, FMingHistoricalEnding>& Pair : EndingDatabase)
    {
        int32 EndingID = Pair.Key;
        if (CheckEndingConditions(EndingID))
        {
            AvailableEndingIDs.Add(EndingID);
        }
    }

    return AvailableEndingIDs;
}

TArray<FMingHistoricalEnding> UMingHistoricalEndingSystem::GetEndingPreview()
{
    TArray<FMingHistoricalEnding> PreviewEndings;
    
    for (const TPair<int32, FMingHistoricalEnding>& Pair : EndingDatabase)
    {
        int32 EndingID = Pair.Key;
        const FMingHistoricalEnding& Ending = Pair.Value;
        
        float Probability = CalculateEndingProbability(EndingID);
        
        // 只顯示概率大於0的結局
        if (Probability > 0.0f)
        {
            PreviewEndings.Add(Ending);
            OnEndingPreview.Broadcast(Ending, Probability);
        }
    }

    return PreviewEndings;
}

float UMingHistoricalEndingSystem::CalculateEndingProbability(int32 EndingID)
{
    if (!EndingDatabase.Contains(EndingID))
    {
        return 0.0f;
    }

    const FMingHistoricalEnding& Ending = EndingDatabase[EndingID];
    
    float TotalProbability = 0.0f;
    float MetWeight = 0.0f;

    for (const FMingEndingCondition& Condition : Ending.UnlockConditions)
    {
        float ConditionWeight = Condition.ConditionWeight;
        bool bConditionMet = false;

        if (Condition.ConditionType == TEXT("total_score"))
        {
            float RequiredScore = Condition.ConditionParameters.FindRef(TEXT("score"));
            bConditionMet = CompareValues(CurrentScore.TotalScore, RequiredScore, Condition.ComparisonOperator);
        }
        else if (Condition.ConditionType == TEXT("historical_accuracy"))
        {
            float RequiredAccuracy = Condition.ConditionParameters.FindRef(TEXT("accuracy"));
            bConditionMet = CompareValues(CurrentScore.HistoricalAccuracy, RequiredAccuracy, Condition.ComparisonOperator);
        }

        if (bConditionMet)
        {
            MetWeight += ConditionWeight;
        }
        TotalProbability += ConditionWeight;
    }

    return (TotalProbability > 0.0f) ? (MetWeight / TotalProbability) : 0.0f;
}

bool UMingHistoricalEndingSystem::UnlockAchievement(int32 AchievementID)
{
    if (!AchievementDatabase.Contains(AchievementID))
    {
        UE_LOG(LogTemp, Error, TEXT("成就ID %d 不存在"), AchievementID);
        return false;
    }

    FMingAchievement& Achievement = AchievementDatabase[AchievementID];

    if (Achievement.bIsUnlocked)
    {
        UE_LOG(LogTemp, Warning, TEXT("成就 %s 已經解鎖"), *Achievement.AchievementName);
        return false;
    }

    if (!CheckAchievementConditions(AchievementID))
    {
        UE_LOG(LogTemp, Warning, TEXT("成就 %s 解鎖條件不滿足"), *Achievement.AchievementName);
        return false;
    }

    // 解鎖成就
    Achievement.bIsUnlocked = true;
    Achievement.UnlockTime = FDateTime::Now();

    // 觸發成就解鎖事件
    OnAchievementUnlocked.Broadcast(Achievement);

    UE_LOG(LogTemp, Log, TEXT("解鎖成就: %s"), *Achievement.AchievementName);
    
    return true;
}

TArray<FMingAchievement> UMingHistoricalEndingSystem::GetUnlockedAchievements() const
{
    TArray<FMingAchievement> UnlockedAchievements;
    
    for (const TPair<int32, FMingAchievement>& Pair : AchievementDatabase)
    {
        const FMingAchievement& Achievement = Pair.Value;
        if (Achievement.bIsUnlocked)
        {
            UnlockedAchievements.Add(Achievement);
        }
    }

    return UnlockedAchievements;
}

TMap<EMingEndingType, int32> UMingHistoricalEndingSystem::GetEndingStatistics() const
{
    TMap<EMingEndingType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingEndingType::Victory, 0);
    Statistics.Add(EMingEndingType::Defeat, 0);
    Statistics.Add(EMingEndingType::Neutral, 0);
    Statistics.Add(EMingEndingType::Historical, 0);
    Statistics.Add(EMingEndingType::Alternative, 0);
    Statistics.Add(EMingEndingType::Secret, 0);
    
    // 統計結局類型
    for (const FMingHistoricalEnding& Ending : PlayerEndingHistory)
    {
        int32& Count = Statistics[Ending.EndingType];
        Count++;
    }
    
    return Statistics;
}

bool UMingHistoricalEndingSystem::SaveEndingData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("HistoricalEndings.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("結局數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個結局，%d 個成就"), PlayerEndingHistory.Num(), AchievementDatabase.Num());
    
    return true;
}

bool UMingHistoricalEndingSystem::LoadEndingData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("HistoricalEndings.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入結局數據"), *LoadPath);
    
    return true;
}

void UMingHistoricalEndingSystem::ResetEndingSystem()
{
    PlayerEndingHistory.Empty();
    CurrentScore = FMingEndingScore();
    
    // 重置成就解鎖狀態
    for (TPair<int32, FMingAchievement>& Pair : AchievementDatabase)
    {
        FMingAchievement& Achievement = Pair.Value;
        Achievement.bIsUnlocked = false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("結局系統已重置"));
}

void UMingHistoricalEndingSystem::LoadDefaultEndings()
{
    // 結局1: 統一中國 (勝利結局)
    FMingHistoricalEnding Ending1;
    Ending1.EndingID = 1;
    Ending1.EndingName = TEXT("統一中國");
    Ending1.EndingDescription = TEXT("成功統一中國，建立強大的中央政府");
    Ending1.EndingDetails = TEXT("在您的領導下，中國結束了軍閥混戰的局面，實現了真正的統一。您建立的政府不僅穩定了國內局勢，還使中國開始走向現代化和繁榮。");
    Ending1.EndingType = EMingEndingType::Victory;
    Ending1.EndingRating = EMingEndingRating::S_Rank;
    Ending1.EndingCategory = EMingEndingCategory::PoliticalUnity;
    Ending1.TriggerYear = 1949;
    Ending1.HistoricalBackground = TEXT("1949年，中華人民共和國成立，標誌著中國百年來的分裂局勢終於結束。");
    Ending1.EndingConsequences = {TEXT("國家統一"), TEXT("經濟發展"), TEXT("國際地位提升"), TEXT("人民生活改善")};
    Ending1.EndingImagePath = TEXT("Textures/Endings/Unity_China.png");
    Ending1.EndingMusicPath = TEXT("Audio/Music/Ending_Victory.mp3");
    Ending1.EndingTags = {TEXT("victory"), TEXT("unity"), TEXT("prosperity")};
    Ending1.bIsHiddenEnding = false;
    Ending1.EndingRarity = 0.1f; // 稀有結局
    
    // 添加解鎖條件
    FMingEndingCondition Condition1;
    Condition1.ConditionID = 1;
    Condition1.ConditionName = TEXT("高總體評分");
    Condition1.ConditionDescription = TEXT("總體評分達到80以上");
    Condition1.ConditionType = TEXT("total_score");
    Condition1.ConditionParameters = {
        {TEXT("score"), 80.0f}
    };
    Condition1.ConditionWeight = 0.4f;
    Condition1.bIsRequired = true;
    Condition1.ComparisonOperator = TEXT(">=");
    Ending1.UnlockConditions.Add(Condition1);
    
    FMingEndingCondition Condition2;
    Condition2.ConditionID = 2;
    Condition2.ConditionName = TEXT("關鍵決策正確");
    Condition2.ConditionDescription = TEXT("在直皖戰爭中選擇正確的陣營");
    Condition2.ConditionType = TEXT("specific_decision");
    Condition2.ConditionParameters = {
        {TEXT("decision_id"), 1.0f},
        {TEXT("choice_id"), 1.0f}
    };
    Condition2.ConditionWeight = 0.3f;
    Condition2.bIsRequired = true;
    Ending1.UnlockConditions.Add(Condition2);
    
    RegisterHistoricalEnding(Ending1);
    
    // 結局2: 軍閥割據 (失敗結局)
    FMingHistoricalEnding Ending2;
    Ending2.EndingID = 2;
    Ending2.EndingName = TEXT("軍閥割據");
    Ending2.EndingDescription = TEXT("中國陷入長期分裂，各地軍閥割據一方");
    Ending2.EndingDetails = TEXT("由於您的決策失誤，中國未能實現統一，各地軍閥繼續混戰，國家陷入長期的分裂和混亂。人民生活在戰爭和貧困中。");
    Ending2.EndingType = EMingEndingType::Defeat;
    Ending2.EndingRating = EMingEndingRating::D_Rank;
    Ending2.EndingCategory = EMingEndingCategory::PoliticalUnity;
    Ending2.TriggerYear = 1949;
    Ending2.HistoricalBackground = TEXT("如果沒有強有力的中央政府，中國很容易陷入地方割據的局面。");
    Ending2.EndingConsequences = {TEXT("國家分裂"), TEXT("經濟衰退"), TEXT("外國干涉"), TEXT("民不聊生")};
    Ending2.EndingImagePath = TEXT("Textures/Endings/Warlord_Era.png");
    Ending2.EndingMusicPath = TEXT("Audio/Music/Ending_Defeat.mp3");
    Ending2.EndingTags = {TEXT("defeat"), TEXT("division"), TEXT("chaos")};
    Ending2.bIsHiddenEnding = false;
    Ending2.EndingRarity = 0.3f;
    
    // 添加解鎖條件
    FMingEndingCondition Condition3;
    Condition3.ConditionID = 3;
    Condition3.ConditionName = TEXT("低總體評分");
    Condition3.ConditionDescription = TEXT("總體評分低於40");
    Condition3.ConditionType = TEXT("total_score");
    Condition3.ConditionParameters = {
        {TEXT("score"), 40.0f}
    };
    Condition3.ConditionWeight = 0.5f;
    Condition3.bIsRequired = true;
    Condition3.ComparisonOperator = TEXT("<");
    Ending2.UnlockConditions.Add(Condition3);
    
    RegisterHistoricalEnding(Ending2);
    
    // 結局3: 改革成功 (歷史結局)
    FMingHistoricalEnding Ending3;
    Ending3.EndingID = 3;
    Ending3.EndingName = TEXT("改革成功");
    Ending3.EndingDescription = TEXT("推動政治改革，建立現代化國家");
    Ending3.EndingDetails = TEXT("您選擇了改革之路，雖然困難重重，但最終成功建立了現代化的政治制度。中國走上了民主和憲政的道路。");
    Ending3.EndingType = EMingEndingType::Historical;
    Ending3.EndingRating = EMingEndingRating::A_Rank;
    Ending3.EndingCategory = EMingEndingCategory::SocialReform;
    Ending3.TriggerYear = 1949;
    Ending3.HistoricalBackground = TEXT("20世紀初，許多中國知識份子和政治家都期望通過改革實現國家現代化。");
    Ending3.EndingConsequences = {TEXT("政治現代化"), TEXT("民主制度"), TEXT("法治社會"), TEXT("文化繁榮")};
    Ending3.EndingImagePath = TEXT("Textures/Endings/Reform_Success.png");
    Ending3.EndingMusicPath = TEXT("Audio/Music/Ending_Historical.mp3");
    Ending3.EndingTags = {TEXT("reform"), TEXT("democracy"), TEXT("modernization")};
    Ending3.bIsHiddenEnding = false;
    Ending3.EndingRarity = 0.2f;
    
    RegisterHistoricalEnding(Ending3);
    
    // 結局4: 經濟強國 (替代歷史結局)
    FMingHistoricalEnding Ending4;
    Ending4.EndingID = 4;
    Ending4.EndingName = TEXT("經濟強國");
    Ending4.EndingDescription = TEXT("專注經濟發展，成為世界經濟強國");
    Ending4.EndingDetails = TEXT("您選擇了經濟發展優先的道路，雖然政治上仍有挑戰，但中國成為了世界經濟強國，人民生活水平大幅提高。");
    Ending4.EndingType = EMingEndingType::Alternative;
    Ending4.EndingRating = EMingEndingRating::A_Rank;
    Ending4.EndingCategory = EMingEndingCategory::EconomicProsperity;
    Ending4.TriggerYear = 1949;
    Ending4.EndingConsequences = {TEXT("經濟繁榮"), TEXT("科技進步"), TEXT("國際影響力"), TEXT("生活富裕")};
    Ending4.EndingImagePath = TEXT("Textures/Endings/Economic_Power.png");
    Ending4.EndingMusicPath = TEXT("Audio/Music/Ending_Alternative.mp3");
    Ending4.EndingTags = {TEXT("economy"), TEXT("prosperity"), TEXT("innovation")};
    Ending4.bIsHiddenEnding = false;
    Ending4.EndingRarity = 0.15f;
    
    RegisterHistoricalEnding(Ending4);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設歷史結局"), EndingDatabase.Num());
}

void UMingHistoricalEndingSystem::LoadDefaultAchievements()
{
    // 成就1: 統一中國
    FMingAchievement Achievement1;
    Achievement1.AchievementID = 1;
    Achievement1.AchievementName = TEXT("統一中國");
    Achievement1.AchievementDescription = TEXT("成功統一中國，結束百年分裂");
    Achievement1.AchievementType = TEXT("ending");
    Achievement1.UnlockConditions = {TEXT("ending_id=1")};
    Achievement1.AchievementIconPath = TEXT("Textures/Achievements/Unity_China.png");
    Achievement1.AchievementRewards = {TEXT("解鎖新結局"), TEXT("增加聲望")};
    RegisterAchievement(Achievement1);
    
    // 成就2: 歷史學者
    FMingAchievement Achievement2;
    Achievement2.AchievementID = 2;
    Achievement2.AchievementName = TEXT("歷史學者");
    Achievement2.AchievementDescription = TEXT("歷史準確度達到90%以上");
    Achievement2.AchievementType = TEXT("accuracy");
    Achievement2.UnlockConditions = {TEXT("historical_accuracy>=90")};
    Achievement2.AchievementIconPath = TEXT("Textures/Achievements/Historical_Scholar.png");
    Achievement2.AchievementRewards = {TEXT("解鎖歷史文檔"), TEXT("增加知識點")};
    RegisterAchievement(Achievement2);
    
    // 成就3: 和平使者
    FMingAchievement Achievement3;
    Achievement3.AchievementID = 3;
    Achievement3.AchievementName = TEXT("和平使者");
    Achievement3.AchievementDescription = TEXT("道德評分達到95%以上");
    Achievement3.AchievementType = TEXT("morality");
    Achievement3.UnlockConditions = {TEXT("morality_score>=95")};
    Achievement3.AchievementIconPath = TEXT("Textures/Achievements/Peace_Maker.png");
    Achievement3.AchievementRewards = {TEXT("解鎖和平結局"), TEXT("增加民眾支持")};
    RegisterAchievement(Achievement3);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設成就"), AchievementDatabase.Num());
}

bool UMingHistoricalEndingSystem::ValidateEndingData(const FMingHistoricalEnding& Ending) const
{
    if (Ending.EndingID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("結局ID必須大於0"));
        return false;
    }
    
    if (Ending.EndingName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("結局名稱不能為空"));
        return false;
    }
    
    if (Ending.UnlockConditions.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("結局沒有解鎖條件，將永遠可用"));
    }
    
    return true;
}

float UMingHistoricalEndingSystem::CalculateDecisionScore()
{
    // 這裡應該從決策管理器獲取決策評分
    // 暫時返回模擬評分
    return 75.0f;
}

float UMingHistoricalEndingSystem::CalculateEventScore()
{
    // 這裡應該從事件觸發器獲取事件評分
    // 暫時返回模擬評分
    return 70.0f;
}

float UMingHistoricalEndingSystem::CalculateRelationshipScore()
{
    // 這裡應該從人物系統獲取關係評分
    // 暫時返回模擬評分
    return 65.0f;
}

float UMingHistoricalEndingSystem::CalculateHistoricalAccuracy()
{
    // 基於玩家選擇與歷史實際的一致性計算
    // 暫時返回模擬評分
    return 80.0f;
}

float UMingHistoricalEndingSystem::CalculateInnovationScore()
{
    // 基於玩家的創新性選擇計算
    // 暫時返回模擬評分
    return 60.0f;
}

float UMingHistoricalEndingSystem::CalculateMoralityScore()
{
    // 基於玩家的道德選擇計算
    // 暫時返回模擬評分
    return 85.0f;
}

float UMingHistoricalEndingSystem::CalculateStrategyScore()
{
    // 基於玩家的戰略決策計算
    // 暫時返回模擬評分
    return 70.0f;
}

bool UMingHistoricalEndingSystem::CheckAchievementConditions(int32 AchievementID)
{
    if (!AchievementDatabase.Contains(AchievementID))
    {
        return false;
    }

    const FMingAchievement& Achievement = AchievementDatabase[AchievementID];

    for (const FString& Condition : Achievement.UnlockConditions)
    {
        if (Condition.Contains(TEXT("ending_id")))
        {
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT("="));
            if (Parts.Num() == 2)
            {
                int32 RequiredEndingID = FCString::Atoi(*Parts[1]);
                // 檢查是否達到該結局
                for (const FMingHistoricalEnding& Ending : PlayerEndingHistory)
                {
                    if (Ending.EndingID == RequiredEndingID)
                    {
                        return true;
                    }
                }
            }
        }
        else if (Condition.Contains(TEXT("historical_accuracy")))
        {
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(">="));
            if (Parts.Num() == 2)
            {
                float RequiredAccuracy = FCString::Atof(*Parts[1]);
                return CurrentScore.HistoricalAccuracy >= RequiredAccuracy;
            }
        }
    }

    return false;
}

FString UMingHistoricalEndingSystem::GenerateEndingDescription(const FMingHistoricalEnding& Ending, const FMingEndingScore& Score) const
{
    FString Description = Ending.EndingDetails;
    
    // 根據評分添加額外描述
    if (Score.TotalScore >= 90.0f)
    {
        Description += TEXT("\n\n您的表現堪稱完美，將被載入史冊！");
    }
    else if (Score.TotalScore >= 70.0f)
    {
        Description += TEXT("\n\n您的表現優秀，為國家做出了重要貢獻。");
    }
    else if (Score.TotalScore >= 50.0f)
    {
        Description += TEXT("\n\n您的表現尚可，但仍有改進空間。");
    }
    else
    {
        Description += TEXT("\n\n您的表現不佳，歷史將給您深刻的教訓。");
    }
    
    return Description;
}

EMingEndingRating UMingHistoricalEndingSystem::GetEndingRating(float Score) const
{
    if (Score >= 95.0f)
    {
        return EMingEndingRating::S_Rank;
    }
    else if (Score >= 85.0f)
    {
        return EMingEndingRating::A_Rank;
    }
    else if (Score >= 70.0f)
    {
        return EMingEndingRating::B_Rank;
    }
    else if (Score >= 55.0f)
    {
        return EMingEndingRating::C_Rank;
    }
    else if (Score >= 40.0f)
    {
        return EMingEndingRating::D_Rank;
    }
    else
    {
        return EMingEndingRating::F_Rank;
    }
}

// 輔助函數
FMingHistoricalEnding UMingHistoricalEndingSystem::GetDefaultEnding() const
{
    FMingHistoricalEnding DefaultEnding;
    DefaultEnding.EndingID = 0;
    DefaultEnding.EndingName = TEXT("默認結局");
    DefaultEnding.EndingDescription = TEXT("由於特殊原因，觸發了默認結局");
    DefaultEnding.EndingType = EMingEndingType::Neutral;
    DefaultEnding.EndingRating = EMingEndingRating::C_Rank;
    return DefaultEnding;
}

float UMingHistoricalEndingSystem::CalculateEndingMatchScore(const FMingHistoricalEnding& Ending, const FMingEndingScore& PlayerScore) const
{
    float MatchScore = 0.0f;
    
    // 根據結局類別和評分計算匹配度
    switch (Ending.EndingCategory)
    {
    case EMingEndingCategory::PoliticalUnity:
        MatchScore = PlayerScore.DecisionScores.Num() > 0 ? PlayerScore.CategoryScores.FindRef(TEXT("決策評分")) : 0.0f;
        break;
    case EMingEndingCategory::SocialReform:
        MatchScore = PlayerScore.MoralityScore;
        break;
    case EMingEndingCategory::EconomicProsperity:
        MatchScore = PlayerScore.StrategyScore;
        break;
    default:
        MatchScore = PlayerScore.TotalScore;
        break;
    }
    
    return MatchScore;
}

bool UMingHistoricalEndingSystem::CompareValues(float Value1, float Value2, const FString& Operator) const
{
    if (Operator == TEXT(">="))
    {
        return Value1 >= Value2;
    }
    else if (Operator == TEXT("<="))
    {
        return Value1 <= Value2;
    }
    else if (Operator == TEXT(">"))
    {
        return Value1 > Value2;
    }
    else if (Operator == TEXT("<"))
    {
        return Value1 < Value2;
    }
    else if (Operator == TEXT("=="))
    {
        return FMath::IsNearlyEqual(Value1, Value2);
    }
    
    return false;
}

bool UMingHistoricalEndingSystem::CheckSpecificDecision(int32 DecisionID, int32 RequiredChoice) const
{
    // 這裡應該檢查決策管理器中特定決策的選擇
    // 暫時返回true
    return true;
}

bool UMingHistoricalEndingSystem::CheckRelationshipCondition(const FString& CharacterName, float RequiredRelationship, const FString& Operator) const
{
    // 這裡應該檢查人物系統中的關係值
    // 暫時返回true
    return true;
}

FString UMingHistoricalEndingSystem::GetEndingRatingName(EMingEndingRating Rating) const
{
    switch (Rating)
    {
    case EMingEndingRating::S_Rank:
        return TEXT("史詩級");
    case EMingEndingRating::A_Rank:
        return TEXT("優秀級");
    case EMingEndingRating::B_Rank:
        return TEXT("良好級");
    case EMingEndingRating::C_Rank:
        return TEXT("普通級");
    case EMingEndingRating::D_Rank:
        return TEXT("較差級");
    case EMingEndingRating::F_Rank:
        return TEXT("失敗級");
    default:
        return TEXT("未知");
    }
}

void UMingHistoricalEndingSystem::RegisterAchievement(const FMingAchievement& Achievement)
{
    if (AchievementDatabase.Contains(Achievement.AchievementID))
    {
        UE_LOG(LogTemp, Warning, TEXT("成就ID %d 已存在，將覆蓋"), Achievement.AchievementID);
    }

    AchievementDatabase.Add(Achievement.AchievementID, Achievement);
    
    UE_LOG(LogTemp, Log, TEXT("註冊成就: %s (ID: %d)"), *Achievement.AchievementName, Achievement.AchievementID);
}
