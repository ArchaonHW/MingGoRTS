#pragma once

#include "CoreMinimal.h"
#include "MingMemoryOptimization.h"
#include "MingSageBrainOptimizedTypes.h"
#include "MingSageBrainCoreSystem.h"
#include "MingSageBrainOptimizationAdapter.generated.h"

/**
 * 聖者大腦系統優化適配器
 * 
 * 展示如何將現有的 SageBrain 系統與記憶體優化系統集成
 * 提供向後兼容的接口，同時內部使用優化結構
 */
UCLASS()
class MINGGORTS_API UMingSageBrainOptimizationAdapter : public UObject
{
    GENERATED_BODY()

public:
    /** 初始化適配器 */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimization")
    void Initialize();

    /** 啟用優化模式 */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimization")
    void EnableOptimizedMode(bool bEnable);

    /** 檢查是否使用優化模式 */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimization")
    bool IsOptimizedMode() const { return bUseOptimizedMode; }

    // ========== 優化的思考處理 ==========
    
    /**
     * 處理思考請求（優化版本）
     * 內部使用 FSageBrainThoughtResult_Optimized，返回時轉換為標準格式
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimized Thinking")
    FSageBrainThoughtResult ProcessThoughtOptimized(
        const FString& Input, 
        ESageBrainThinkingLayer TargetLayer
    );

    /**
     * 批次處理多個思考請求
     * 使用物件池減少記憶體分配
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimized Thinking")
    TArray<FSageBrainThoughtResult> ProcessThoughtBatchOptimized(
        const TArray<FString>& Inputs,
        ESageBrainThinkingLayer TargetLayer
    );

    // ========== 優化的學習處理 ==========
    
    /**
     * 訓練算法（優化版本）
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimized Learning")
    FSageBrainLearningResult TrainAlgorithmOptimized(
        ESageBrainLearningAlgorithm Algorithm, 
        const FString& TrainingData
    );

    /**
     * 獲取學習統計（優化版本）
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimized Learning")
    TArray<FSageBrainLearningResult> GetLearningStatisticsOptimized();

    // ========== 優化的哲學分析 ==========
    
    /**
     * 哲學分析（優化版本）
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimized Philosophy")
    FSageBrainPhilosophyAnalysis AnalyzeWithPhilosophyOptimized(
        const FString& Input, 
        ESageBrainPhilosophy Philosophy
    );

    /**
     * 獲取多角度哲學觀點（優化版本）
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Optimized Philosophy")
    TArray<FSageBrainPhilosophyAnalysis> GetPhilosophicalPerspectivesOptimized(
        const FString& Input
    );

    // ========== 記憶體管理 ==========
    
    /**
     * 執行記憶體壓縮
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Memory")
    void CompactMemory();

    /**
     * 獲取記憶體使用報告
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Memory")
    FString GetMemoryReport() const;

    /**
     * 檢查記憶體狀態
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Memory")
    bool IsMemoryCritical() const;

    // ========== 效能監控 ==========
    
    /**
     * 開始效能分析
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Profiling")
    void StartProfiling();

    /**
     * 停止效能分析並輸出報告
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Profiling")
    void StopProfiling();

    /**
     * 獲取優化統計
     */
    UFUNCTION(BlueprintCallable, Category = "SageBrain|Profiling")
    TMap<FString, float> GetOptimizationStats() const;

private:
    UPROPERTY()
    bool bUseOptimizedMode;

    UPROPERTY()
    UMingSageBrainCoreSystem* CoreSystem;

    UPROPERTY()
    UMingMemoryOptimizer* MemoryOptimizer;

    // 物件池
    UPROPERTY()
    UMingUObjectPool* ThoughtResultPool;

    // 優化數據緩存
    TMingFixedArray<FSageBrainThoughtResult_Optimized, 64> ThoughtCache;
    TMingFixedArray<FSageBrainLearningResult_Optimized, 32> LearningCache;
    TMingFixedArray<FSageBrainPhilosophyAnalysis_Optimized, 16> PhilosophyCache;

    // 效能統計
    float TotalProcessingTime;
    float TotalMemorySaved;
    int32 ProcessCount;

    // 內部輔助方法
    void InitializeObjectPools();
    void ReleaseObjectPools();
    void UpdateOptimizationStats(float ProcessingTime, float MemorySaved);
    
    // 處理函數（優化版本）
    FSageBrainThoughtResult_Optimized ProcessThoughtInternal(
        const FString& Input, 
        ESageBrainThinkingLayer TargetLayer
    );
    
    FSageBrainLearningResult_Optimized TrainAlgorithmInternal(
        ESageBrainLearningAlgorithm Algorithm, 
        const FString& TrainingData
    );
    
    FSageBrainPhilosophyAnalysis_Optimized AnalyzePhilosophyInternal(
        const FString& Input, 
        ESageBrainPhilosophy Philosophy
    );
};
