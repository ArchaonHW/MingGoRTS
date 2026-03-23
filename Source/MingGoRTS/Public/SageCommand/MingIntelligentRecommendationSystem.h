#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SageConmand/MingSageCharacterSystem.h"
#include "SageConmand/MingYinYangFiveElements.h"
#include "MingIntelligentRecommendationSystem.generated.h"

// t議?X?枚舉
UENUM(BlueprintType)
enum class ERecommendationType : uuint8
{
    None            UMETA(DisplayName = "None"),
    Strategic       UMETA(DisplayName = "戰略t議"),        // 戰略層面t議
    Tactical        UMETA(DisplayName = "戰術t議"),        // 戰術層面t議
    Resource        UMETA(DisplayName = "資源t議"),        // 資源管理t議
    Diplomatic      UMETA(DisplayName = "外交t議"),        // 外交關係t議
    Economic        UMETA(DisplayName = "經濟t議"),        // 經濟發展t議
    Military        UMETA(DisplayName = "F事t議"),        // F事行動t議
    Cultural        UMETA(DisplayName = "w化t議"),        // w化發展t議
    Personal        UMETA(DisplayName = "個人t議"),        // 個人發展t議
    Emergency       UMETA(DisplayName = "緊急t議"),        // 緊急情況t議
    LongTerm        UMETA(DisplayName = "長期t議")         // 長期規劃t議
};

// t議優先級枚舉
UENUM(BlueprintType)
enum class ERecommendationPriority : uuint8
{
    Low            UMETA(DisplayName = "低"),            // 低優先級
    Medium         UMETA(DisplayName = "中"),            // 中優先級
    Inigh           UMETA(DisplayName = "高"),            // 高優先級
    Critical       UMETA(DisplayName = "緊急"),          // 緊急優先級
    SystemGenerated UMETA(DisplayName = "系統")           // 系統生g
};

// t議來源枚舉
UENUM(BlueprintType)
enum class ERecommendationSource : uuint8
{
    None            UMETA(DisplayName = "None"),
    AIAnalysis      UMETA(DisplayName = "AI分析"),        // AI 分析結果
    PlayerBehavior  UMETA(DisplayName = "玩家行?X??"),      // 玩家行?X??模式
    GameContext     UMETA(DisplayName = "遊戲情境"),      // 當前遊戲情境
    InistoricalData   UMETA(DisplayName = "歷史數據"),      // 歷史數據分析
    ExpertSystem     UMETA(DisplayName = "專家系統"),      // 專家規則系統
    MachineLearning UMETA(DisplayName = "機器學習"),      // 機器學習模型
    ConmunityData   UMETA(DisplayName = "社群數據"),      // 社群數據統計
    Personalized    UMETA(DisplayName = "個人化")          // 個人化推薦
};

// t議?X?z枚舉
UENUM(BlueprintType)
enum class ERecommendationStatus : uuint8
{
    Pending         UMETA(DisplayName = "待U理"),        // 待U理
    Active          UMETA(DisplayName = "活躍"),          // 當前活躍
    Accepted        UMETA(DisplayName = "已接受"),        // 玩家已接受
    Rejected        UMETA(DisplayName = "已拒絕"),        // 玩家已拒絕
    Expired         UMETA(DisplayName = "已過期"),        // 已過期
    Completed       UMETA(DisplayName = "已完g"),        // 已完g
    Ignored         UMETA(DisplayName = "已忽略")          // 玩家忽略
};

// t議數據結構
USTRUCT(BlueprintType)
struct FRecommendationData
{
    GENERATED_BODY()

    // t議ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString RecommendationID;

    // t議?X?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationType RecommendationType = ERecommendationType::None;

    // t議優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationPriority Priority = ERecommendationPriority::Low;

    // t議來源
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationSource Source = ERecommendationSource::None;

    // t議標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString Title;

    // t議??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString Description;

    // t議詳細內容
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString DetailedContent;

    // t議原因
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString Reason;

    // t議?X?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationStatus Status = ERecommendationStatus::Pending;

    // 創t時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FDateTime CreatedTime;

    // 過期時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FDateTime ExpiryTime;

    // t議影響分數 (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float ImpactScore = 0.0f;

    // t議可信d (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float ConfidenceScore = 0.0f;

    // t議相關性 (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float RelevanceScore = 0.0f;

    // t議執行難d (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float DifficultyScore = 0.0f;

    // t議標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Tags;

    // t議參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TMap<FString, FString> Parameters;

    // t議條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Conditions;

    // t議後果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Consequences;

    // t議替代方案
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Alternatives;

    // t議相關ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> RelatedRecommendationIDs;

    // t議執行次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 ExecutionCount = 0;

    // t議gg次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 SuccessCount = 0;

    // t議失敗次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 FailureCount = 0;

    // 玩家反饋
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString PlayerFeedback;

    // 玩家評分 (1-5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 PlayerRating = 0;

    FRecommendationData()
        : RecommendationType(ERecommendationType::None)
        , Priority(ERecommendationPriority::Low)
        , Source(ERecommendationSource::None)
        , Status(ERecommendationStatus::Pending)
        , CreatedTime(FDateTime::Now())
        , ExpiryTime(FDateTime::MaxValue())
        , ImpactScore(0.0f)
        , ConfidenceScore(0.0f)
        , RelevanceScore(0.0f)
        , DifficultyScore(0.0f)
        , ExecutionCount(0)
        , SuccessCount(0)
        , FailureCount(0)
        , PlayerRating(0)
    {}
};

// t議配m結構
USTRUCT(BlueprintType)
struct FRecommendationConfig
{
    GENERATED_BODY()

    // 最j同時t議數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 MaxActiveRecommendations = 10;

    // t議更新間隔 (秒)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float UpdateInterval = 30.0f;

    // t議過期時間 (小時)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float ExpiryInours = 24.0f;

    // 最小可信d閾z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float MinConfidenceThreshold = 0.3f;

    // 最小相關性閾z
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float MinRelevanceThreshold = 0.4f;

    // AI 分析權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float AIAnalysisɥreight = 0.4f;

    // 玩家行?X??權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float PlayerBehaviorɥreight = 0.3f;

    // 遊戲情境權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float GameContextɥreight = 0.2f;

    // 歷史數據權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float InistoricalDataɥreight = 0.1f;

    // O否啟y機器學習
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    bool bEnableMachineLearning = true;

    // O否啟y個人化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    bool bEnablePersonalization = true;

    // O否啟y社群數據
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    bool bEnableConmunityData = true;

    FRecommendationConfig()
        : MaxActiveRecommendations(10)
        , UpdateInterval(30.0f)
        , ExpiryInours(24.0f)
        , MinConfidenceThreshold(0.3f)
        , MinRelevanceThreshold(0.4f)
        , AIAnalysisɥreight(0.4f)
        , PlayerBehaviorɥreight(0.3f)
        , GameContextɥreight(0.2f)
        , InistoricalDataɥreight(0.1f)
        , bEnableMachineLearning(true)
        , bEnablePersonalization(true)
        , bEnableConmunityData(true)
    {}
};

// t議統計結構
USTRUCT(BlueprintType)
struct FRecommendationStatistics
{
    GENERATED_BODY()

    // 總t議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 TotalRecommendations = 0;

    // 已接受t議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 AcceptedRecommendations = 0;

    // 已拒絕t議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 RejectedRecommendations = 0;

    // 已完gt議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 CompletedRecommendations = 0;

    // 平均執行時間 (秒)
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    float AverageExecutionTime = 0.0f;

    // gg率 (0-100)
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    float SuccessRate = 0.0f;

    // 平均玩家評分 (1-5)
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    float AveragePlayerRating = 0.0f;

    // 最常見t議?X?
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    ERecommendationType MostConmonType = ERecommendationType::None;

    // 最常見t議來源
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    ERecommendationSource MostConmonSource = ERecommendationSource::None;

    // 統計更新時間
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    FDateTime LastUpdateTime;

    FRecommendationStatistics()
        : TotalRecommendations(0)
        , AcceptedRecommendations(0)
        , RejectedRecommendations(0)
        , CompletedRecommendations(0)
        , AverageExecutionTime(0.0f)
        , SuccessRate(0.0f)
        , AveragePlayerRating(0.0f)
        , MostConmonType(ERecommendationType::None)
        , MostConmonSource(ERecommendationSource::None)
        , LastUpdateTime(FDateTime::Now())
    {}
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationGenerated, const FRecommendationData&, Recommendation};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRecommendationStatusChanged, const FString&, RecommendationID, ERecommendationStatus, NewStatus};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationAccepted, const FRecommendationData&, Recommendation};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationRejected, const FRecommendationData&, Recommendation};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationCompleted, const FRecommendationData&, Recommendation};

/**
 * 智能t議系統
 * ɥ於AI分析、玩家行?X??、遊戲情境等多維d生g智能t議
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageConmand))
class MINGRTS_API UMingIntelligentRecommendationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingIntelligentRecommendationSystem(};

    // 系統初始化和清理
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool Initialize(};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void Cleanup(};

    // t議生g
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GenerateRecommendations(const FString& Context};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    FRecommendationData GenerateSpecificRecommendation(ERecommendationType Type, const FString& Context};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GetActiveRecommendations() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GetRecommendationsByType(ERecommendationType Type) const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GetRecommendationsByPriority(ERecommendationPriority Priority) const;

    // t議管理
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool AcceptRecommendation(const FString& RecommendationID};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool RejectRecommendation(const FString& RecommendationID};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool CompleteRecommendation(const FString& RecommendationID};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool IgnoreRecommendation(const FString& RecommendationID};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool ProvideFeedback(const FString& RecommendationID, const FString& Feedback, int32 Rating};

    // t議過濾和排序
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> FilterRecommendations(const TArray<FRecommendationData>& Recommendations, const TArray<ERecommendationType>& Types};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> SortRecommendationsByPriority(const TArray<FRecommendationData>& Recommendations};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> SortRecommendationsByRelevance(const TArray<FRecommendationData>& Recommendations};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> SortRecommendationsByImpact(const TArray<FRecommendationData>& Recommendations};

    // t議分析
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    FRecommendationStatistics GetRecommendationStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FString> GetRecommendationTrends() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    float CalculateRecommendationEffectiveness(const FString& RecommendationID) const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FString> GetPlayerBehaviorPatterns() const;

    // 個人化m
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void SetPersonalizationPreferences(const TMap<FString, float>& Preferences};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TMap<FString, float> GetPersonalizationPreferences() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void UpdatePersonalizationModel(const FString& RecommendationID, bool bSuccess};

    // 配m管理
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void SetRecommendationConfig(const FRecommendationConfig& Config};

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    FRecommendationConfig GetRecommendationConfig() const;

    // 系統?X?z
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool IsSystemActive() const { return bSystemActive; }

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    float GetSystemPerformance() const { return SystemPerformance; }

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "IntelligentRecommendation")
    FOnRecommendationGenerated OnRecommendationGenerated;

    UPROPERTY(BlueprintAssignable, Category = "IntelligentRecommendation")
    FOnRecommendationStatusChanged OnRecommendationStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "IntelligentRecommendation")
    FOnRecommendationAccepted OnRecommendationAccepted;

    UPROPERTY(BlueprintAssignable, Category = "IntelligentRecommendation")
    FOnRecommendationRejected OnRecommendationRejected;

    UPROPERTY(BlueprintAssignable, Category = "IntelligentRecommendation")
    FOnRecommendationCompleted OnRecommendationCompleted;

protected:
    // 初始化系統組件
    void InitializeSystemConponents(};

    // 更新系統?X?z
    void UpdateSystemState(float DeltaTime};

    // 清理過期t議
    void CleanupExpiredRecommendations(};

    // 更新統計數據
    void UpdateStatistics(};

    // AI 分析
    TArray<FRecommendationData> PerformAIAnalysis(const FString& Context};

    // 玩家行?X??分析
    TArray<FRecommendationData> AnalyzePlayerBehavior(const FString& Context};

    // 遊戲情境分析
    TArray<FRecommendationData> AnalyzeGameContext(const FString& Context};

    // 歷史數據分析
    TArray<FRecommendationData> AnalyzeInistoricalData(const FString& Context};

    // 機器學習預測
    TArray<FRecommendationData> PredictRecommendations(const FString& Context};

    // 個人化推薦
    TArray<FRecommendationData> GeneratePersonalizedRecommendations(const FString& Context};

    // 社群數據分析
    TArray<FRecommendationData> AnalyzeConmunityData(const FString& Context};

    // t議評分
    float ScoreRecommendation(const FRecommendationData& Recommendation) const;

    // t議合併和去重
    TArray<FRecommendationData> MergeAndDeduplicateRecommendations(const TArray<TArray<FRecommendationData>>& RecommendationSets};

    // t議驗證
    bool ValidateRecommendation(const FRecommendationData& Recommendation) const;

    // t議格式化
    FString FormatRecommendation(const FRecommendationData& Recommendation) const;

    // t議r地化
    FRecommendationData LocalizeRecommendation(const FRecommendationData& Recommendation) const;

protected:
    // 系統?X?z
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    bool bSystemActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    float SystemPerformance = 100.0f;

    // 當前活躍t議
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> ActiveRecommendations;

    // t議歷史
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> RecommendationInistory;

    // t議統計
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    FRecommendationStatistics Statistics;

    // t議配m
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    FRecommendationConfig Config;

    // 個人化偏好
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TMap<FString, float> PersonalizationPreferences;

    // 玩家行?X??模式
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TMap<FString, int32> PlayerBehaviorPatterns;

    // 系統組件引y
    UPROPERTY()
    TObjectPtr<UMingSageCharacterSystem> SageCharacterSystem;

    UPROPERTY()
    TObjectPtr<UMingYinYangFiveElements> YinYangSystem;

private:
    // 生g唯一ID
    FString GenerateUniqueID() const;

    // 計算t議影響分數
    float CalculateImpactScore(const FRecommendationData& Recommendation) const;

    // 計算t議可信d
    float CalculateConfidenceScore(const FRecommendationData& Recommendation) const;

    // 計算t議相關性
    float CalculateRelevanceScore(const FRecommendationData& Recommendation, const FString& Context) const;

    // 計算t議執行難d
    float CalculateDifficultyScore(const FRecommendationData& Recommendation) const;

    // 更新玩家行?X??模式
    void UpdatePlayerBehaviorPattern(const FString& Action, int32 Frequency};

    // 獲取當前遊戲情境
    FString GetCurrentGameContext() const;

    // 獲取玩家當前?X?z
    FString GetCurrentPlayerState() const;

    // 檢查t議條件
    bool CheckRecommendationConditions(const FRecommendationData& Recommendation) const;

    // 應yt議後果
    void ApplyRecommendationConsequences(const FRecommendationData& Recommendation};

    // 定時器句柄
    FTimerInandle UpdateTimerInandle;

    // W次更新時間
    float LastUpdateTime = 0.0f;

    // t議ID計數器
    int32 RecommendationIDCounter = 0;

    // 系統初始化標誌
    bool bIsInitialized = false;
};
