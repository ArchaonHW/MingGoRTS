#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingCombatAI.h"
#include "MingCombatAIBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * 單位數組包裝結構 (用於 TArray<TArray<>> 嵌套)
 */
USTRUCT(BlueprintType)
struct FCombatAIUnitArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TArray<AMingTacticalUnit*> Units;

    FCombatAIUnitArrayWrapper() {}
};

/**
 * 戰鬥AI藍圖函數庫
 * 提供藍圖可調用的戰鬥AI系統功能
 */
UCLASS()
class MINGTACTICAL_API UMingCombatAIBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取戰鬥AI系統
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static AMingCombatAI* GetCombatAI();

    /**
     * 初始化戰鬥AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void InitializeCombatAI();

    /**
     * 關閉戰鬥AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void ShutdownCombatAI();

    /**
     * 設置AI難度等級
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel);

    /**
     * 設置AI戰術類型
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAITacticalType(EAITacticalType TacticalType);

    /**
     * 設置AI學習類型
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAILearningType(EAILearningType LearningType);

    /**
     * 添加受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 移除受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 獲取所有受控單位
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * 執行AI決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAIDecision(AMingTacticalUnit* Unit);

    /**
     * 執行戰術分析
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static FAITacticalAnalysis ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 執行AI學習
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAILearning(const FAILearningData& LearningData);

    /**
     * 創建決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode CreateDecisionNode(
        const FString& NodeName,
        EAIDecisionType DecisionType,
        const FString& ConditionExpression,
        float Weight,
        int32 Priority
    );

    /**
     * 添加決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool AddDecisionNode(const FAIDecisionNode& Node);

    /**
     * 移除決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool RemoveDecisionNode(const FString& NodeID);

    /**
     * 獲取決策節點
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static FAIDecisionNode GetDecisionNode(const FString& NodeID);

    /**
     * 獲取所有決策節點
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static TArray<FAIDecisionNode> GetAllDecisionNodes();

    /**
     * 評估戰術局勢
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static FAITacticalAnalysis EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 生成戰術建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 預測敵方行動
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 計算威脅等級
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 計算機會等級
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 執行攻擊決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 執行防禦決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition);

    /**
     * 執行移動決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition);

    /**
     * 執行撤退決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition);

    /**
     * 執行側翼決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 執行伏擊決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits);

    /**
     * 執行支援決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 執行協調決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType);

    /**
     * 執行學習決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData);

    /**
     * 執行適應決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData);

    /**
     * 調整AI難度
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Difficulty")
    static void AdjustAIDifficulty(float PerformanceFactor);

    /**
     * 獲取AI性能指標
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static FAIPerformanceMetrics GetAIPerformanceMetrics();

    /**
     * 獲取AI統計數據
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * 獲取AI配置
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Configuration")
    static TMap<FString, FString> GetAIConfiguration();

    /**
     * 設置AI配置
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Configuration")
    static bool SetAIConfiguration(const TMap<FString, FString>& Configuration);

    /**
     * 獲取AI狀態名稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStateName(EAIState AIState);

    /**
     * 獲取決策類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDecisionTypeName(EAIDecisionType DecisionType);

    /**
     * 獲取戰術類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetTacticalTypeName(EAITacticalType TacticalType);

    /**
     * 獲取難度等級名稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDifficultyLevelName(EAIDifficultyLevel DifficultyLevel);

    /**
     * 獲取學習類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetLearningTypeName(EAILearningType LearningType);

    /**
     * 創建學習數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAILearningData CreateLearningData(
        EAILearningType LearningType,
        const TArray<float>& InputData,
        const TArray<float>& OutputData,
        float RewardValue,
        float PenaltyValue
    );

    /**
     * 設置決策節點參數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode SetDecisionNodeParameter(const FAIDecisionNode& Node, const FString& ParameterName, const FString& ParameterValue);

    /**
     * 添加決策節點子節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode AddDecisionNodeChild(const FAIDecisionNode& Node, const FString& ChildNodeID);

    /**
     * 創建戰術分析
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis CreateTacticalAnalysis(
        float ThreatAssessment,
        float OpportunityAssessment,
        float AdvantageAssessment,
        float DisadvantageAssessment
    );

    /**
     * 添加戰術建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddTacticalRecommendation(const FAITacticalAnalysis& Analysis, const FString& Recommendation);

    /**
     * 添加風險評估
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddRiskAssessment(const FAITacticalAnalysis& Analysis, const FString& Risk);

    /**
     * 添加機會識別
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddOpportunityIdentification(const FAITacticalAnalysis& Analysis, const FString& Opportunity);

    /**
     * 添加敵方預測
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddEnemyPrediction(const FAITacticalAnalysis& Analysis, const FString& Prediction);

    /**
     * 批量執行AI決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAIDecisions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 批量添加受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchAddControlledUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 批量移除受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchRemoveControlledUnits(const TArray<AMingTacticalUnit*>& Units);

    // 批量執行戰術分析 - 使用 FUnitArrayWrapper 包裝結構
    static TArray<FAITacticalAnalysis> BatchExecuteTacticalAnalysis(const TArray<FUnitArrayWrapper>& UnitGroups);

    /**
     * 批量執行AI學習
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAILearning(const TArray<FAILearningData>& LearningDataArray);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsCombatAIInitialized();

    /**
     * 獲取系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static FString GetCombatAIVersion();

    /**
     * 獲取受控單位總數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetControlledUnitCount();

    /**
     * 獲取決策節點總數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetDecisionNodeCount();

    /**
     * 獲取學習數據總數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetLearningDataCount();

    /**
     * 獲取戰術分析總數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetTacticalAnalysisCount();

    /**
     * 檢查單位是否受控
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit);

    /**
     * 檢查決策節點是否存在
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool DoesDecisionNodeExist(const FString& NodeID);

    /**
     * 檢查決策節點是否啟用
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsDecisionNodeEnabled(const FString& NodeID);

    /**
     * 獲取決策節點執行次數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static int32 GetDecisionNodeExecutionCount(const FString& NodeID);

    /**
     * 獲取決策節點成功率
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static float GetDecisionNodeSuccessRate(const FString& NodeID);

    /**
     * 計算AI性能分數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateAIPerformanceScore();

    /**
     * 計算決策質量分數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateDecisionQualityScore(const FString& NodeID);

    /**
     * 計算戰術分析分數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateTacticalAnalysisScore(const FAITacticalAnalysis& Analysis);

    /**
     * 計算學習效果分數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateLearningEffectScore(const FAILearningData& LearningData);

    /**
     * 獲取最佳決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetBestDecisionNode(AMingTacticalUnit* Unit);

    /**
     * 獲取最差決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetWorstDecisionNode(AMingTacticalUnit* Unit);

    /**
     * 獲取推薦決策類型
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDecisionType GetRecommendedDecisionType(AMingTacticalUnit* Unit);

    /**
     * 獲取推薦戰術類型
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAITacticalType GetRecommendedTacticalType(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 獲取推薦難度等級
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDifficultyLevel GetRecommendedDifficultyLevel(float PlayerPerformance);

    /**
     * 獲取AI建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetAIRecommendations(AMingTacticalUnit* Unit);

    /**
     * 獲取戰術建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 獲取戰略建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetStrategicRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 獲取攻擊建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<AMingTacticalUnit*> GetAttackRecommendations(AMingTacticalUnit* Unit);

    /**
     * 獲取防禦建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetDefenseRecommendations(AMingTacticalUnit* Unit);

    /**
     * 獲取移動建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetMovementRecommendations(AMingTacticalUnit* Unit);

    /**
     * 獲取撤退建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetRetreatRecommendations(AMingTacticalUnit* Unit);

    /**
     * 模擬AI決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAIDecisions(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount);

    /**
     * 模擬戰術分析
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<FAITacticalAnalysis> SimulateTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount);

    /**
     * 模擬AI學習
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAILearning(const TArray<FAILearningData>& LearningDataArray, int32 SimulationCount);

    /**
     * 獲取AI歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetAIHistory();

    /**
     * 獲取決策歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetDecisionHistory();

    /**
     * 獲取學習歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetLearningHistory();

    /**
     * 獲取戰術歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetTacticalHistory();

    /**
     * 獲取AI統計
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * 獲取決策統計
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetDecisionStatistics();

    /**
     * 獲取學習統計
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetLearningStatistics();

    /**
     * 獲取戰術統計
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetTacticalStatistics();

    /**
     * 獲取性能統計
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * 導出AI數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Export")
    static bool ExportAIData(const FString& FilePath);

    /**
     * 導入AI數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Import")
    static bool ImportAIData(const FString& FilePath);

    /**
     * 獲取AI報告
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIReport();

    /**
     * 獲取單位AI報告
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateUnitAIReport(AMingTacticalUnit* Unit);

    /**
     * 獲取決策報告
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateDecisionReport(const FString& NodeID);

    /**
     * 獲取戰術分析報告
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateTacticalAnalysisReport(const FAITacticalAnalysis& Analysis);

    /**
     * 獲取學習報告
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateLearningReport(const FAILearningData& LearningData);

    /**
     * 獲取AI分析報告
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIAnalysisReport();

    /**
     * 重置AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ResetAISystem();

    /**
     * 清除所有受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllControlledUnits();

    /**
     * 清除所有決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllDecisionNodes();

    /**
     * 清除所有學習數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllLearningData();

    /**
     * 清除所有戰術分析
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllTacticalAnalyses();

    /**
     * 備份AI數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool BackupAIData(const FString& BackupPath);

    /**
     * 恢復AI數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool RestoreAIData(const FString& BackupPath);

    /**
     * 優化AI性能
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeAIPerformance();

    /**
     * 優化決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeDecisionNodes();

    /**
     * 優化學習算法
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeLearningAlgorithms();

    /**
     * 調整決策權重
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdjustDecisionWeights();

    /**
     * 適應戰術變化
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdaptToTacticalChanges();

    /**
     * 學習玩家行為
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static void LearnPlayerBehavior();

    /**
     * 預測玩家意圖
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> PredictPlayerIntent();

    /**
     * 生成反制策略
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GenerateCounterStrategies();

    /**
     * 評估AI表現
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static float EvaluateAIPerformance();

    /**
     * 更新性能指標
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static void UpdatePerformanceMetrics();

    /**
     * 獲取AI狀態
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStatus();

    /**
     * 獲取AI模式
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIMode();

    /**
     * 獲取AI版本信息
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIVersionInfo();

    /**
     * 檢查AI健康狀態
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsAIHealthy();

    /**
     * 獲取AI錯誤信息
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static TArray<FString> GetAIErrorMessages();

    /**
     * 清除AI錯誤
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAIErrors();

    /**
     * 恢復AI狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void RecoverAIState();
};
