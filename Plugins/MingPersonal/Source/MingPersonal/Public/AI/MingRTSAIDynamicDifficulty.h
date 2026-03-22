#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAIDynamicDifficulty.generated.h"

// 難度等級
UENUM(BlueprintType)
enum class EDifficultyLevel: uint8 {
    VeryEasy            UMETA(DisplayName = "非常簡單"),
    Easy                UMETA(DisplayName = "簡單"),
    Normal              UMETA(DisplayName = "普通"),
    Hard                UMETA(DisplayName = "困難"),
    VeryHard            UMETA(DisplayName = "非常困難"),
    Nightmare           UMETA(DisplayName = "噩夢"),
    Adaptive            UMETA(DisplayName = "自適應")
};

// 玩家技能指標
UENUM(BlueprintType)
enum class ESkillMetric: uint8 {
    APM                 UMETA(DisplayName = "每分鐘操作數"),
    ReactionTime        UMETA(DisplayName = "反應時間"),
    StrategicThinking   UMETA(DisplayName = "戰略思維"),
    ResourceManagement  UMETA(DisplayName = "資源管理"),
    CombatEfficiency    UMETA(DisplayName = "戰鬥效率"),
    UnitControl         UMETA(DisplayName = "單位控制"),
    MapAwareness        UMETA(DisplayName = "地圖意識"),
    EconomyOptimization UMETA(DisplayName = "經濟優化"),
    WinRate             UMETA(DisplayName = "勝率"),
    LearningCurve       UMETA(DisplayName = "學習曲線")
};

// 難度調整類型
UENUM(BlueprintType)
enum class EDifficultyAdjustment: uint8 {
    AIIntelligence      UMETA(DisplayName = "AI智能"),
    AIResources         UMETA(DisplayName = "AI資源"),
    AIProductionSpeed   UMETA(DisplayName = "AI生產速度"),
    AICombatBonus       UMETA(DisplayName = "AI戰鬥加成"),
    PlayerResources     UMETA(DisplayName = "玩家資源"),
    PlayerProductionSpeed UMETA(DisplayName = "玩家生產速度"),
    PlayerCombatBonus   UMETA(DisplayName = "玩家戰鬥加成"),
    EventFrequency      UMETA(DisplayName = "事件頻率"),
    ChallengeIntensity  UMETA(DisplayName = "挑戰強度")
};

// 玩家技能數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FPlayerSkillData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<ESkillMetric, float> SkillScores;
    
    UPROPERTY(BlueprintReadOnly)
    float OverallSkillRating;
    
    UPROPERTY(BlueprintReadOnly)
    float ExperienceLevel;
    
    UPROPERTY(BlueprintReadOnly)
    float ConsistencyScore;
    
    UPROPERTY(BlueprintReadOnly)
    float ImprovementRate;
    
    UPROPERTY(BlueprintReadOnly)
    int32 GamesPlayed;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Wins;
    
    UPROPERTY(BlueprintReadOnly)
    int32 Losses;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentWinStreak;
    
    UPROPERTY(BlueprintReadOnly)
    float BestWinStreak;
    
    UPROPERTY(BlueprintReadOnly)
    float LastSessionPerformance;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageSessionPerformance;
    
    FPlayerSkillData()
        : OverallSkillRating(0.5f)
        , ExperienceLevel(1.0f)
        , ConsistencyScore(0.5f)
        , ImprovementRate(0.0f)
        , GamesPlayed(0)
        , Wins(0)
        , Losses(0)
        , CurrentWinStreak(0)
        , BestWinStreak(0)
        , LastSessionPerformance(0.5f)
        , AverageSessionPerformance(0.5f)
    {}
};

// 難度配置
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FDifficultyConfiguration
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDifficultyLevel BaseDifficulty;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableDynamicAdjustment;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AdjustmentSensitivity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinDifficultyMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDifficultyMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EvaluationWindow;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUsePlayerHistory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPreventSuddenSpikes;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpikeProtectionThreshold;
    
    FDifficultyConfiguration()
        : BaseDifficulty(EDifficultyLevel::Normal)
        , bEnableDynamicAdjustment(true)
        , AdjustmentSensitivity(0.5f)
        , MinDifficultyMultiplier(0.3f)
        , MaxDifficultyMultiplier(3.0f)
        , EvaluationWindow(5)
        , bUsePlayerHistory(true)
        , bPreventSuddenSpikes(true)
        , SpikeProtectionThreshold(0.3f)
    {}
};

// 難度調整參數
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FDifficultyAdjustmentParams
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    TMap<EDifficultyAdjustment, float> AdjustmentValues;
    
    UPROPERTY(BlueprintReadOnly)
    float EffectiveDifficultyMultiplier;
    
    UPROPERTY(BlueprintReadOnly)
    FString ReasonForAdjustment;
    
    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    FDifficultyAdjustmentParams()
        : EffectiveDifficultyMultiplier(1.0f)
        , ConfidenceLevel(0.5f)
        , Timestamp(0.0f)
    {}
};

// 會話性能數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FSessionPerformanceData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString SessionID;
    
    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;
    
    UPROPERTY(BlueprintReadOnly)
    float SessionDuration;
    
    UPROPERTY(BlueprintReadOnly)
    float PerformanceScore;
    
    UPROPERTY(BlueprintReadOnly)
    float DifficultyAtStart;
    
    UPROPERTY(BlueprintReadOnly)
    float DifficultyAtEnd;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ChallengesCompleted;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ChallengesFailed;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageChallengeCompletionTime;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ResourcesGathered;
    
    UPROPERTY(BlueprintReadOnly)
    int32 UnitsProduced;
    
    UPROPERTY(BlueprintReadOnly)
    int32 UnitsLost;
    
    UPROPERTY(BlueprintReadOnly)
    int32 EnemyUnitsDestroyed;
    
    FSessionPerformanceData()
        : SessionDuration(0.0f)
        , PerformanceScore(0.5f)
        , DifficultyAtStart(1.0f)
        , DifficultyAtEnd(1.0f)
        , ChallengesCompleted(0)
        , ChallengesFailed(0)
        , AverageChallengeCompletionTime(0.0f)
        , ResourcesGathered(0)
        , UnitsProduced(0)
        , UnitsLost(0)
        , EnemyUnitsDestroyed(0)
    {}
};

// 難度歷史記錄
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FDifficultyHistoryEntry
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    UPROPERTY(BlueprintReadOnly)
    float DifficultyMultiplier;
    
    UPROPERTY(BlueprintReadOnly)
    FString TriggerReason;
    
    UPROPERTY(BlueprintReadOnly)
    float PlayerPerformance;
    
    FDifficultyHistoryEntry()
        : Timestamp(0.0f)
        , DifficultyMultiplier(1.0f)
        , PlayerPerformance(0.5f)
    {}
};

// 委托聲明
declare_dynamic_multicast_delegate_three_params(FOnDifficultyAdjusted, FString, PlayerID, float, NewMultiplier, FString, Reason);
declare_dynamic_multicast_delegate_two_params(FOnSkillAssessmentUpdated, FString, PlayerID, FPlayerSkillData, SkillData);
declare_dynamic_multicast_delegate_three_params(FOnPerformanceEvaluated, FString, PlayerID, float, PerformanceScore, EDifficultyLevel, RecommendedDifficulty);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingRTSAIDynamicDifficulty : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSAIDynamicDifficulty();
    
    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty")
    void InitializeDifficultySystem(const FDifficultyConfiguration& Config);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty")
    void ShutdownDifficultySystem();
    
    // 玩家技能管理
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Player")
    void RegisterPlayer(const FString& PlayerID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Player")
    void UnregisterPlayer(const FString& PlayerID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Player")
    void UpdatePlayerSkill(const FString& PlayerID, ESkillMetric Metric, float Score);
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Player")
    FPlayerSkillData GetPlayerSkillData(const FString& PlayerID) const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Player")
    float GetPlayerSkillRating(const FString& PlayerID) const;
    
    // 難度調整
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Adjustment")
    void AdjustDifficulty(const FString& PlayerID, float PerformanceScore);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Adjustment")
    void SetBaseDifficulty(const FString& PlayerID, EDifficultyLevel NewDifficulty);
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Adjustment")
    float GetCurrentDifficultyMultiplier(const FString& PlayerID) const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Adjustment")
    EDifficultyLevel GetRecommendedDifficulty(const FString& PlayerID) const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Adjustment")
    void ApplyDifficultyAdjustment(const FString& PlayerID, const FDifficultyAdjustmentParams& Params);
    
    // 性能評估
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Performance")
    void StartSession(const FString& PlayerID, const FString& SessionID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Performance")
    void EndSession(const FString& PlayerID, const FString& SessionID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Performance")
    void RecordChallengeResult(const FString& PlayerID, bool bSuccess, float CompletionTime);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Performance")
    void RecordCombatEngagement(const FString& PlayerID, bool bVictory, int32 FriendlyLosses, int32 EnemyLosses);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Performance")
    void RecordResourceGathering(const FString& PlayerID, int32 Amount);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Performance")
    float EvaluateSessionPerformance(const FString& PlayerID, const FString& SessionID);
    
    // 自適應算法
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Adaptive")
    void EnableAdaptiveDifficulty(const FString& PlayerID);
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Adaptive")
    void DisableAdaptiveDifficulty(const FString& PlayerID);
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Adaptive")
    bool IsAdaptiveDifficultyEnabled(const FString& PlayerID) const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Adaptive")
    FDifficultyAdjustmentParams CalculateOptimalDifficulty(const FString& PlayerID);
    
    // 統計和分析
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Analytics")
    TArray<FDifficultyHistoryEntry> GetDifficultyHistory(const FString& PlayerID, int32 Count) const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Analytics")
    FSessionPerformanceData GetSessionPerformance(const FString& PlayerID, const FString& SessionID) const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Analytics")
    float GetAverageDifficultyTrend(const FString& PlayerID, int32 SessionCount) const;
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Analytics")
    float GetPlayerProgressRate(const FString& PlayerID) const;
    
    // 配置管理
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Config")
    void SetConfiguration(const FDifficultyConfiguration& NewConfig);
    
    UFUNCTION(BlueprintPure, Category = "AI|Difficulty|Config")
    FDifficultyConfiguration GetConfiguration() const;
    
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty|Config")
    void ResetToDefaults();
    
    // Tick更新
    UFUNCTION(BlueprintCallable, Category = "AI|Difficulty")
    void Tick(float DeltaTime);
    
    // 委托
    UPROPERTY(BlueprintAssignable, Category = "AI|Difficulty|Events")
    FOnDifficultyAdjusted OnDifficultyAdjusted;
    
    UPROPERTY(BlueprintAssignable, Category = "AI|Difficulty|Events")
    FOnSkillAssessmentUpdated OnSkillAssessmentUpdated;
    
    UPROPERTY(BlueprintAssignable, Category = "AI|Difficulty|Events")
    FOnPerformanceEvaluated OnPerformanceEvaluated;
    
protected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    float CurrentGameTime;
    
    UPROPERTY()
    FDifficultyConfiguration Configuration;
    
    UPROPERTY()
    TMap<FString, FPlayerSkillData> PlayerSkills;
    
    UPROPERTY()
    TMap<FString, float> CurrentDifficultyMultipliers;
    
    UPROPERTY()
    TMap<FString, TArray<FDifficultyHistoryEntry>> DifficultyHistories;
    
    UPROPERTY()
    TMap<FString, FSessionPerformanceData> ActiveSessions;
    
    UPROPERTY()
    TMap<FString, TArray<FSessionPerformanceData>> SessionHistories;
    
    UPROPERTY()
    TMap<FString, bool> AdaptiveEnabled;
    
    // 內部處理
    void ProcessSkillUpdates(float DeltaTime);
    void EvaluateAndAdjustDifficulty(const FString& PlayerID);
    void UpdatePlayerSkillRating(const FString& PlayerID);
    float CalculatePerformanceScore(const FString& PlayerID, const FSessionPerformanceData& Session);
    float GetDifficultyForLevel(EDifficultyLevel Level) const;
    FDifficultyAdjustmentParams GenerateAdjustmentParams(const FString& PlayerID, float TargetMultiplier);
    
    // 輔助函數
    float CalculateSkillConsistency(const TArray<float>& Scores) const;
    float CalculateImprovementRate(const TArray<float>& Scores) const;
    bool ShouldAdjustDifficulty(const FString& PlayerID, float CurrentPerformance) const;
    float ClampDifficultyMultiplier(float Multiplier) const;
    void LogDifficultyChange(const FString& PlayerID, float OldMultiplier, float NewMultiplier, const FString& Reason);
    
    // 技能評估算法
    float EvaluateAPM(const FString& PlayerID) const;
    float EvaluateStrategicThinking(const FString& PlayerID) const;
    float EvaluateResourceManagement(const FString& PlayerID) const;
    float EvaluateCombatEfficiency(const FString& PlayerID) const;
    float EvaluateWinRate(const FString& PlayerID) const;
};
