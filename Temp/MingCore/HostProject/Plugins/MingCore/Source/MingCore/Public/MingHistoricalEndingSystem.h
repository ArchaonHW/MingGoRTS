#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEndingSystem.generated.h"

// 結局類型枚舉
UENUM(BlueprintType)
enum class EMingEndingType : uint8
{
    Victory,            // 勝利結局
    Defeat,             // 失敗結局
    Neutral,            // 中立結局
    Historical,         // 歷史結局
    Alternative,        // 替代歷史結局
    Secret              // 隱藏結局
};

// 結局評價級別
UENUM(BlueprintType)
enum class EMingEndingRating : uint8
{
    S_Rank,             // 史詩級
    A_Rank,             // 優秀級
    B_Rank,             // 良好級
    C_Rank,             // 普通級
    D_Rank,             // 較差級
    F_Rank              // 失敗級
};

// 結局類別
UENUM(BlueprintType)
enum class EMingEndingCategory : uint8
{
    PoliticalUnity,     // 政治統一
    MilitaryConquest,   // 軍事征服
    EconomicProsperity, // 經濟繁榮
    CulturalRenaissance, // 文化復興
    SocialReform,       // 社會改革
    NationalIndependence, // 民族獨立
    WorldPower,         // 世界強國
    HistoricalPreservation // 歷史保存
};

// 結局條件
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEndingCondition
{
    GENERATED_BODY()

    // 條件ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConditionID;

    // 條件名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionName;

    // 條件描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionDescription;

    // 條件類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionType;

    // 條件參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ConditionParameters;

    // 條件權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConditionWeight;

    // 是否為必要條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    // 條件比較運算符
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ComparisonOperator;

    FMingEndingCondition()
    {
        ConditionID = -1;
        ConditionName = TEXT("");
        ConditionDescription = TEXT("");
        ConditionType = TEXT("");
        ConditionParameters.Empty();
        ConditionWeight = 1.0f;
        bIsRequired = false;
        ComparisonOperator = TEXT(">=");
    }
};

// 歷史結局數據
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalEnding
{
    GENERATED_BODY()

    // 結局ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EndingID;

    // 結局名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingName;

    // 結局描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingDescription;

    // 結局詳細描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingDetails;

    // 結局類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingType EndingType;

    // 結局評價
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingRating EndingRating;

    // 結局類別
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingCategory EndingCategory;

    // 解鎖條件列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingEndingCondition> UnlockConditions;

    // 結局觸發年份
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggerYear;

    // 歷史背景說明
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // 結局影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EndingConsequences;

    // 結局圖片路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingImagePath;

    // 結局音樂路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingMusicPath;

    // 結局標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EndingTags;

    // 是否為隱藏結局
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHiddenEnding;

    // 結局稀有度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EndingRarity;

    FMingHistoricalEnding()
    {
        EndingID = -1;
        EndingName = TEXT("");
        EndingDescription = TEXT("");
        EndingDetails = TEXT("");
        EndingType = EMingEndingType::Neutral;
        EndingRating = EMingEndingRating::C_Rank;
        EndingCategory = EMingEndingCategory::PoliticalUnity;
        UnlockConditions.Empty();
        TriggerYear = 1949;
        HistoricalBackground = TEXT("");
        EndingConsequences.Empty();
        EndingImagePath = TEXT("");
        EndingMusicPath = TEXT("");
        EndingTags.Empty();
        bIsHiddenEnding = false;
        EndingRarity = 1.0f;
    }
};

// 結局評分數據
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEndingScore
{
    GENERATED_BODY()

    // 結局ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EndingID;

    // 總體評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalScore;

    // 各類別評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> CategoryScores;

    // 決策評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, float> DecisionScores;

    // 事件評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, float> EventScores;

    // 人物關係評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RelationshipScores;

    // 歷史準確度評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HistoricalAccuracy;

    // 創新性評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InnovationScore;

    // 道德評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralityScore;

    // 戰略評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StrategyScore;

    FMingEndingScore()
    {
        EndingID = -1;
        TotalScore = 0.0f;
        CategoryScores.Empty();
        DecisionScores.Empty();
        EventScores.Empty();
        RelationshipScores.Empty();
        HistoricalAccuracy = 0.0f;
        InnovationScore = 0.0f;
        MoralityScore = 0.0f;
        StrategyScore = 0.0f;
    }
};

// 成就數據
USTRUCT(BlueprintType)
struct FINGCORE_API FMingAchievement
{
    GENERATED_BODY()

    // 成就ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AchievementID;

    // 成就名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementName;

    // 成就描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementDescription;

    // 成就類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementType;

    // 解鎖條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // 成就圖標路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementIconPath;

    // 成就獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AchievementRewards;

    // 是否已解鎖
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnlocked;

    // 解鎖時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime UnlockTime;

    FMingAchievement()
    {
        AchievementID = -1;
        AchievementName = TEXT("");
        AchievementDescription = TEXT("");
        AchievementType = TEXT("");
        UnlockConditions.Empty();
        AchievementIconPath = TEXT("");
        AchievementRewards.Empty();
        bIsUnlocked = false;
        UnlockTime = FDateTime::Now();
    }
};

// 結局評價委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndingDetermined, const FMingHistoricalEnding&, Ending);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndingScoreCalculated, const FMingEndingScore&, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, const FMingAchievement&, Achievement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEndingPreview, const FMingHistoricalEnding&, Ending, float, Probability);

/**
 * 歷史結局系統
 * 負責評價玩家的歷史決策路徑並確定最終結局
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalEndingSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEndingSystem();

    // 初始化結局系統
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool InitializeEndingSystem();

    // 註冊歷史結局
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool RegisterHistoricalEnding(const FMingHistoricalEnding& Ending);

    // 評價玩家路徑
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    FMingEndingScore EvaluatePlayerPath();

    // 確定最終結局
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    FMingHistoricalEnding DetermineEnding(const FMingEndingScore& PlayerScore);

    // 檢查結局條件
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool CheckEndingConditions(int32 EndingID);

    // 獲取結局信息
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    FMingHistoricalEnding GetEndingInfo(int32 EndingID) const;

    // 獲取可用結局
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TArray<int32> GetAvailableEndings() const;

    // 獲取結局預覽
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    TArray<FMingHistoricalEnding> GetEndingPreview();

    // 計算結局概率
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    float CalculateEndingProbability(int32 EndingID);

    // 解鎖成就
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool UnlockAchievement(int32 AchievementID);

    // 獲取已解鎖成就
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TArray<FMingAchievement> GetUnlockedAchievements() const;

    // 獲取結局統計
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TMap<EMingEndingType, int32> GetEndingStatistics() const;

    // 保存結局數據
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool SaveEndingData();

    // 載入結局數據
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool LoadEndingData();

    // 重置結局系統
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    void ResetEndingSystem();

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnEndingDetermined OnEndingDetermined;

    UPROPERTY(BlueprintAssignable)
    FOnEndingScoreCalculated OnEndingScoreCalculated;

    UPROPERTY(BlueprintAssignable)
    FOnAchievementUnlocked OnAchievementUnlocked;

    UPROPERTY(BlueprintAssignable)
    FOnEndingPreview OnEndingPreview;

protected:
    // 結局數據庫
    UPROPERTY()
    TMap<int32, FMingHistoricalEnding> EndingDatabase;

    // 成就數據庫
    UPROPERTY()
    TMap<int32, FMingAchievement> AchievementDatabase;

    // 玩家結局歷史
    UPROPERTY()
    TArray<FMingHistoricalEnding> PlayerEndingHistory;

    // 當前評分
    UPROPERTY()
    FMingEndingScore CurrentScore;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設結局
    void LoadDefaultEndings();

    // 載入預設成就
    void LoadDefaultAchievements();

    // 驗證結局數據
    bool ValidateEndingData(const FMingHistoricalEnding& Ending) const;

    // 計算決策評分
    float CalculateDecisionScore();

    // 計算事件評分
    float CalculateEventScore();

    // 計算關係評分
    float CalculateRelationshipScore();

    // 計算歷史準確度
    float CalculateHistoricalAccuracy();

    // 計算創新性評分
    float CalculateInnovationScore();

    // 計算道德評分
    float CalculateMoralityScore();

    // 計算戰略評分
    float CalculateStrategyScore();

    // 檢查成就條件
    bool CheckAchievementConditions(int32 AchievementID);

    // 生成結局描述
    FString GenerateEndingDescription(const FMingHistoricalEnding& Ending, const FMingEndingScore& Score) const;

    // 獲取結局評價
    EMingEndingRating GetEndingRating(float Score) const;
};
