// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SageCommand/MingSageCharacterSystem.h"
#include "SageCommand/MingYinYangFiveElements.h"
#include "MingIntelligentRecommendationSystem.generated.h"

// 建議類型枚舉
UENUM(BlueprintType)
enum class ERecommendationType : uint8
{
    None            UMETA(DisplayName = "None"),
    Strategic       UMETA(DisplayName = "戰略建議"),        // 戰略層面建議
    Tactical        UMETA(DisplayName = "戰術建議"),        // 戰術層面建議
    Resource        UMETA(DisplayName = "資源建議"),        // 資源管理建議
    Diplomatic      UMETA(DisplayName = "外交建議"),        // 外交關係建議
    Economic        UMETA(DisplayName = "經濟建議"),        // 經濟發展建議
    Military        UMETA(DisplayName = "軍事建議"),        // 軍事行動建議
    Cultural        UMETA(DisplayName = "文化建議"),        // 文化發展建議
    Personal        UMETA(DisplayName = "個人建議"),        // 個人發展建議
    Emergency       UMETA(DisplayName = "緊急建議"),        // 緊急情況建議
    LongTerm        UMETA(DisplayName = "長期建議")         // 長期規劃建議
};

// 建議優先級枚舉
UENUM(BlueprintType)
enum class ERecommendationPriority : uint8
{
    Low            UMETA(DisplayName = "低"),            // 低優先級
    Medium         UMETA(DisplayName = "中"),            // 中優先級
    High           UMETA(DisplayName = "高"),            // 高優先級
    Critical       UMETA(DisplayName = "緊急"),          // 緊急優先級
    SystemGenerated UMETA(DisplayName = "系統")           // 系統生成
};

// 建議來源枚舉
UENUM(BlueprintType)
enum class ERecommendationSource : uint8
{
    None            UMETA(DisplayName = "None"),
    AIAnalysis      UMETA(DisplayName = "AI分析"),        // AI 分析結果
    PlayerBehavior  UMETA(DisplayName = "玩家行為"),      // 玩家行為模式
    GameContext     UMETA(DisplayName = "遊戲情境"),      // 當前遊戲情境
    HistoricalData   UMETA(DisplayName = "歷史數據"),      // 歷史數據分析
    ExpertSystem     UMETA(DisplayName = "專家系統"),      // 專家規則系統
    MachineLearning UMETA(DisplayName = "機器學習"),      // 機器學習模型
    CommunityData   UMETA(DisplayName = "社群數據"),      // 社群數據統計
    Personalized    UMETA(DisplayName = "個人化")          // 個人化推薦
};

// 建議狀態枚舉
UENUM(BlueprintType)
enum class ERecommendationStatus : uint8
{
    Pending         UMETA(DisplayName = "待處理"),        // 待處理
    Active          UMETA(DisplayName = "活躍"),          // 當前活躍
    Accepted        UMETA(DisplayName = "已接受"),        // 玩家已接受
    Rejected        UMETA(DisplayName = "已拒絕"),        // 玩家已拒絕
    Expired         UMETA(DisplayName = "已過期"),        // 已過期
    Completed       UMETA(DisplayName = "已完成"),        // 已完成
    Ignored         UMETA(DisplayName = "已忽略")          // 玩家忽略
};

// 建議數據結構
USTRUCT(BlueprintType)
struct FRecommendationData
{
    GENERATED_BODY()

    // 建議ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString RecommendationID;

    // 建議類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationType RecommendationType = ERecommendationType::None;

    // 建議優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationPriority Priority = ERecommendationPriority::Low;

    // 建議來源
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationSource Source = ERecommendationSource::None;

    // 建議標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString Title;

    // 建議描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString Description;

    // 建議詳細內容
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString DetailedContent;

    // 建議原因
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FString Reason;

    // 建議狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    ERecommendationStatus Status = ERecommendationStatus::Pending;

    // 創建時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FDateTime CreatedTime;

    // 過期時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    FDateTime ExpiryTime;

    // 建議影響分數 (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float ImpactScore = 0.0f;

    // 建議可信度 (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float ConfidenceScore = 0.0f;

    // 建議相關性 (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float RelevanceScore = 0.0f;

    // 建議執行難度 (0-100)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float DifficultyScore = 0.0f;

    // 建議標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Tags;

    // 建議參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TMap<FString, FString> Parameters;

    // 建議條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Conditions;

    // 建議後果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Consequences;

    // 建議替代方案
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Alternatives;

    // 建議相關ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> RelatedRecommendationIDs;

    // 建議執行次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 ExecutionCount = 0;

    // 建議成功次數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 SuccessCount = 0;

    // 建議失敗次數
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

// 建議配置結構
USTRUCT(BlueprintType)
struct FRecommendationConfig
{
    GENERATED_BODY()

    // 最大同時建議數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    int32 MaxActiveRecommendations = 10;

    // 建議更新間隔 (秒)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float UpdateInterval = 30.0f;

    // 建議過期時間 (小時)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float ExpiryHours = 24.0f;

    // 最小可信度閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float MinConfidenceThreshold = 0.3f;

    // 最小相關性閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float MinRelevanceThreshold = 0.4f;

    // AI 分析權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float AIAnalysisWeight = 0.4f;

    // 玩家行為權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float PlayerBehaviorWeight = 0.3f;

    // 遊戲情境權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float GameContextWeight = 0.2f;

    // 歷史數據權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    float HistoricalDataWeight = 0.1f;

    // 是否啟用機器學習
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    bool bEnableMachineLearning = true;

    // 是否啟用個人化
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    bool bEnablePersonalization = true;

    // 是否啟用社群數據
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recommendation")
    bool bEnableCommunityData = true;

    FRecommendationConfig()
        : MaxActiveRecommendations(10)
        , UpdateInterval(30.0f)
        , ExpiryHours(24.0f)
        , MinConfidenceThreshold(0.3f)
        , MinRelevanceThreshold(0.4f)
        , AIAnalysisWeight(0.4f)
        , PlayerBehaviorWeight(0.3f)
        , GameContextWeight(0.2f)
        , HistoricalDataWeight(0.1f)
        , bEnableMachineLearning(true)
        , bEnablePersonalization(true)
        , bEnableCommunityData(true)
    {}
};

// 建議統計結構
USTRUCT(BlueprintType)
struct FRecommendationStatistics
{
    GENERATED_BODY()

    // 總建議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 TotalRecommendations = 0;

    // 已接受建議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 AcceptedRecommendations = 0;

    // 已拒絕建議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 RejectedRecommendations = 0;

    // 已完成建議數量
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    int32 CompletedRecommendations = 0;

    // 平均執行時間 (秒)
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    float AverageExecutionTime = 0.0f;

    // 成功率 (0-100)
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    float SuccessRate = 0.0f;

    // 平均玩家評分 (1-5)
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    float AveragePlayerRating = 0.0f;

    // 最常見建議類型
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    ERecommendationType MostCommonType = ERecommendationType::None;

    // 最常見建議來源
    UPROPERTY(BlueprintReadOnly, Category = "Recommendation")
    ERecommendationSource MostCommonSource = ERecommendationSource::None;

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
        , MostCommonType(ERecommendationType::None)
        , MostCommonSource(ERecommendationSource::None)
        , LastUpdateTime(FDateTime::Now())
    {}
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationGenerated, const FRecommendationData&, Recommendation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRecommendationStatusChanged, const FString&, RecommendationID, ERecommendationStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationAccepted, const FRecommendationData&, Recommendation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationRejected, const FRecommendationData&, Recommendation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecommendationCompleted, const FRecommendationData&, Recommendation);

/**
 * 智能建議系統
 * 基於AI分析、玩家行為、遊戲情境等多維度生成智能建議
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageCommand))
class MINGRTS_API UMingIntelligentRecommendationSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingIntelligentRecommendationSystem();

    // 系統初始化和清理
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool Initialize();

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void Cleanup();

    // 建議生成
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GenerateRecommendations(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    FRecommendationData GenerateSpecificRecommendation(ERecommendationType Type, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GetActiveRecommendations() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GetRecommendationsByType(ERecommendationType Type) const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> GetRecommendationsByPriority(ERecommendationPriority Priority) const;

    // 建議管理
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool AcceptRecommendation(const FString& RecommendationID);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool RejectRecommendation(const FString& RecommendationID);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool CompleteRecommendation(const FString& RecommendationID);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool IgnoreRecommendation(const FString& RecommendationID);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    bool ProvideFeedback(const FString& RecommendationID, const FString& Feedback, int32 Rating);

    // 建議過濾和排序
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> FilterRecommendations(const TArray<FRecommendationData>& Recommendations, const TArray<ERecommendationType>& Types);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> SortRecommendationsByPriority(const TArray<FRecommendationData>& Recommendations);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> SortRecommendationsByRelevance(const TArray<FRecommendationData>& Recommendations);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> SortRecommendationsByImpact(const TArray<FRecommendationData>& Recommendations);

    // 建議分析
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    FRecommendationStatistics GetRecommendationStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FString> GetRecommendationTrends() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    float CalculateRecommendationEffectiveness(const FString& RecommendationID) const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TArray<FString> GetPlayerBehaviorPatterns() const;

    // 個人化設置
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void SetPersonalizationPreferences(const TMap<FString, float>& Preferences);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    TMap<FString, float> GetPersonalizationPreferences() const;

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void UpdatePersonalizationModel(const FString& RecommendationID, bool bSuccess);

    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    void SetRecommendationConfig(const FRecommendationConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "IntelligentRecommendation")
    FRecommendationConfig GetRecommendationConfig() const;

    // 系統狀態
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
    void InitializeSystemComponents();

    // 更新系統狀態
    void UpdateSystemState(float DeltaTime);

    // 清理過期建議
    void CleanupExpiredRecommendations();

    // 更新統計數據
    void UpdateStatistics();

    // AI 分析
    TArray<FRecommendationData> PerformAIAnalysis(const FString& Context);

    // 玩家行為分析
    TArray<FRecommendationData> AnalyzePlayerBehavior(const FString& Context);

    // 遊戲情境分析
    TArray<FRecommendationData> AnalyzeGameContext(const FString& Context);

    // 歷史數據分析
    TArray<FRecommendationData> AnalyzeHistoricalData(const FString& Context);

    // 機器學習預測
    TArray<FRecommendationData> PredictRecommendations(const FString& Context);

    // 個人化推薦
    TArray<FRecommendationData> GeneratePersonalizedRecommendations(const FString& Context);

    // 社群數據分析
    TArray<FRecommendationData> AnalyzeCommunityData(const FString& Context);

    // 建議評分
    float ScoreRecommendation(const FRecommendationData& Recommendation) const;

    // 建議合併和去重
    TArray<FRecommendationData> MergeAndDeduplicateRecommendations(const TArray<TArray<FRecommendationData>>& RecommendationSets);

    // 建議驗證
    bool ValidateRecommendation(const FRecommendationData& Recommendation) const;

    // 建議格式化
    FString FormatRecommendation(const FRecommendationData& Recommendation) const;

    // 建議本地化
    FRecommendationData LocalizeRecommendation(const FRecommendationData& Recommendation) const;

protected:
    // 系統狀態
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    bool bSystemActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    float SystemPerformance = 100.0f;

    // 當前活躍建議
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> ActiveRecommendations;

    // 建議歷史
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TArray<FRecommendationData> RecommendationHistory;

    // 建議統計
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    FRecommendationStatistics Statistics;

    // 建議配置
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    FRecommendationConfig Config;

    // 個人化偏好
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TMap<FString, float> PersonalizationPreferences;

    // 玩家行為模式
    UPROPERTY(BlueprintReadOnly, Category = "IntelligentRecommendation")
    TMap<FString, int32> PlayerBehaviorPatterns;

    // 系統組件引用
    UPROPERTY()
    TObjectPtr<UMingSageCharacterSystem> SageCharacterSystem;

    UPROPERTY()
    TObjectPtr<UMingYinYangFiveElements> YinYangSystem;

private:
    // 生成唯一ID
    FString GenerateUniqueID() const;

    // 計算建議影響分數
    float CalculateImpactScore(const FRecommendationData& Recommendation) const;

    // 計算建議可信度
    float CalculateConfidenceScore(const FRecommendationData& Recommendation) const;

    // 計算建議相關性
    float CalculateRelevanceScore(const FRecommendationData& Recommendation, const FString& Context) const;

    // 計算建議執行難度
    float CalculateDifficultyScore(const FRecommendationData& Recommendation) const;

    // 更新玩家行為模式
    void UpdatePlayerBehaviorPattern(const FString& Action, int32 Frequency);

    // 獲取當前遊戲情境
    FString GetCurrentGameContext() const;

    // 獲取玩家當前狀態
    FString GetCurrentPlayerState() const;

    // 檢查建議條件
    bool CheckRecommendationConditions(const FRecommendationData& Recommendation) const;

    // 應用建議後果
    void ApplyRecommendationConsequences(const FRecommendationData& Recommendation);

    // 定時器句柄
    FTimerHandle UpdateTimerHandle;

    // 上次更新時間
    float LastUpdateTime = 0.0f;

    // 建議ID計數器
    int32 RecommendationIDCounter = 0;

    // 系統初始化標誌
    bool bIsInitialized = false;
};
