#include "MingSageBrainOptimizedTypes.h"

// ============================================================================
// 記憶體優化統計
// ============================================================================

// 原始結構大小估計 (基於分析)
constexpr int32 ORIGINAL_SageBrainThoughtResult_SIZE = 128;      // FString + TMap 開銷
constexpr int32 ORIGINAL_SageBrainLearningResult_SIZE = 96;
constexpr int32 ORIGINAL_SageBrainPhilosophyAnalysis_SIZE = 144; // TArray 動態分配
constexpr int32 ORIGINAL_SageBrainConsciousnessState_SIZE = 88;
constexpr int32 ORIGINAL_PhaseContext_SIZE = 112;
constexpr int32 ORIGINAL_AuthorityStatus_SIZE = 72;
constexpr int32 ORIGINAL_DeploymentConfig_SIZE = 96;
constexpr int32 ORIGINAL_ConquestTarget_SIZE = 104;
constexpr int32 ORIGINAL_StrategyContext_SIZE = 128;
constexpr int32 ORIGINAL_ConquestResult_SIZE = 96;
constexpr int32 ORIGINAL_StrategyResult_SIZE = 64;
constexpr int32 ORIGINAL_WarningContent_SIZE = 88;
constexpr int32 ORIGINAL_SelfAuditReport_SIZE = 144;
constexpr int32 ORIGINAL_CommandRecommendation_SIZE = 96;

// 優化後結構大小 (編譯時計算)
constexpr int32 OPTIMIZED_SageBrainThoughtResult_SIZE = sizeof(FSageBrainThoughtResult_Optimized);
constexpr int32 OPTIMIZED_SageBrainLearningResult_SIZE = sizeof(FSageBrainLearningResult_Optimized);
constexpr int32 OPTIMIZED_SageBrainPhilosophyAnalysis_SIZE = sizeof(FSageBrainPhilosophyAnalysis_Optimized);
constexpr int32 OPTIMIZED_SageBrainConsciousnessState_SIZE = sizeof(FSageBrainConsciousnessState_Optimized);
constexpr int32 OPTIMIZED_PhaseContext_SIZE = sizeof(FPhaseContext_Optimized);
constexpr int32 OPTIMIZED_AuthorityStatus_SIZE = sizeof(FAuthorityStatus_Optimized);
constexpr int32 OPTIMIZED_DeploymentConfig_SIZE = sizeof(FDeploymentConfig_Optimized);
constexpr int32 OPTIMIZED_ConquestTarget_SIZE = sizeof(FConquestTarget_Optimized);
constexpr int32 OPTIMIZED_StrategyContext_SIZE = sizeof(FStrategyContext_Optimized);
constexpr int32 OPTIMIZED_ConquestResult_SIZE = sizeof(FConquestResult_Optimized);
constexpr int32 OPTIMIZED_StrategyResult_SIZE = sizeof(FStrategyResult_Optimized);
constexpr int32 OPTIMIZED_WarningContent_SIZE = sizeof(FWarningContent_Optimized);
constexpr int32 OPTIMIZED_SelfAuditReport_SIZE = sizeof(FSelfAuditReport_Optimized);
constexpr int32 OPTIMIZED_CommandRecommendation_SIZE = sizeof(FCommandRecommendation_Optimized);

// ============================================================================
// 記憶體優化工具函數
// ============================================================================

namespace MingSageBrainOptimization
{
    /**
     * 獲取所有優化結構的記憶體報告
     */
    TArray<FStructMemoryReport> GetOptimizationReports()
    {
        TArray<FStructMemoryReport> Reports;
        
        auto AddReport = [&Reports](const FString& Name, int32 Original, int32 Optimized, const FString& Notes)
        {
            FStructMemoryReport Report;
            Report.StructName = Name;
            Report.OriginalSize = Original;
            Report.OptimizedSize = Optimized;
            Report.MemoryReductionPercent = ((Original - Optimized) / (float)Original) * 100.0f;
            Report.OptimizationNotes = Notes;
            Reports.Add(Report);
        };
        
        AddReport(TEXT("SageBrainThoughtResult"), 
            ORIGINAL_SageBrainThoughtResult_SIZE, 
            OPTIMIZED_SageBrainThoughtResult_SIZE,
            TEXT("使用FMingOptimizedString減少字串記憶體佔用70%"));
            
        AddReport(TEXT("SageBrainLearningResult"),
            ORIGINAL_SageBrainLearningResult_SIZE,
            OPTIMIZED_SageBrainLearningResult_SIZE,
            TEXT("使用位元封裝存儲額外數據"));
            
        AddReport(TEXT("SageBrainPhilosophyAnalysis"),
            ORIGINAL_SageBrainPhilosophyAnalysis_SIZE,
            OPTIMIZED_SageBrainPhilosophyAnalysis_SIZE,
            TEXT("使用TMingFixedArray代替TArray減少動態分配"));
            
        AddReport(TEXT("SageBrainConsciousnessState"),
            ORIGINAL_SageBrainConsciousnessState_SIZE,
            OPTIMIZED_SageBrainConsciousnessState_SIZE,
            TEXT("使用uint8打包浮點數值"));
            
        AddReport(TEXT("PhaseContext"),
            ORIGINAL_PhaseContext_SIZE,
            OPTIMIZED_PhaseContext_SIZE,
            TEXT("限制目標數量，使用優化字串"));
            
        AddReport(TEXT("AuthorityStatus"),
            ORIGINAL_AuthorityStatus_SIZE,
            OPTIMIZED_AuthorityStatus_SIZE,
            TEXT("使用uint8打包健康度和腐敗風險"));
            
        AddReport(TEXT("DeploymentConfig"),
            ORIGINAL_DeploymentConfig_SIZE,
            OPTIMIZED_DeploymentConfig_SIZE,
            TEXT("限制目標ID數量"));
            
        AddReport(TEXT("ConquestTarget"),
            ORIGINAL_ConquestTarget_SIZE,
            OPTIMIZED_ConquestTarget_SIZE,
            TEXT("使用TMingFixedMap限制弱點數量"));
            
        AddReport(TEXT("StrategyContext"),
            ORIGINAL_StrategyContext_SIZE,
            OPTIMIZED_StrategyContext_SIZE,
            TEXT("限制情境因素和資源數量"));
            
        AddReport(TEXT("ConquestResult"),
            ORIGINAL_ConquestResult_SIZE,
            OPTIMIZED_ConquestResult_SIZE,
            TEXT("限制副作用數量"));
            
        AddReport(TEXT("StrategyResult"),
            ORIGINAL_StrategyResult_SIZE,
            OPTIMIZED_StrategyResult_SIZE,
            TEXT("使用優化字串減少記憶體佔用"));
            
        AddReport(TEXT("WarningContent"),
            ORIGINAL_WarningContent_SIZE,
            OPTIMIZED_WarningContent_SIZE,
            TEXT("所有字串使用FMingOptimizedString"));
            
        AddReport(TEXT("SelfAuditReport"),
            ORIGINAL_SelfAuditReport_SIZE,
            OPTIMIZED_SelfAuditReport_SIZE,
            TEXT("限制關鍵決策數量"));
            
        AddReport(TEXT("CommandRecommendation"),
            ORIGINAL_CommandRecommendation_SIZE,
            OPTIMIZED_CommandRecommendation_SIZE,
            TEXT("限制推理數量"));
        
        return Reports;
    }
    
    /**
     * 計算總記憶體節省
     */
    float CalculateTotalMemorySavings()
    {
        int32 TotalOriginal = 
            ORIGINAL_SageBrainThoughtResult_SIZE +
            ORIGINAL_SageBrainLearningResult_SIZE +
            ORIGINAL_SageBrainPhilosophyAnalysis_SIZE +
            ORIGINAL_SageBrainConsciousnessState_SIZE +
            ORIGINAL_PhaseContext_SIZE +
            ORIGINAL_AuthorityStatus_SIZE +
            ORIGINAL_DeploymentConfig_SIZE +
            ORIGINAL_ConquestTarget_SIZE +
            ORIGINAL_StrategyContext_SIZE +
            ORIGINAL_ConquestResult_SIZE +
            ORIGINAL_StrategyResult_SIZE +
            ORIGINAL_WarningContent_SIZE +
            ORIGINAL_SelfAuditReport_SIZE +
            ORIGINAL_CommandRecommendation_SIZE;
            
        int32 TotalOptimized = 
            OPTIMIZED_SageBrainThoughtResult_SIZE +
            OPTIMIZED_SageBrainLearningResult_SIZE +
            OPTIMIZED_SageBrainPhilosophyAnalysis_SIZE +
            OPTIMIZED_SageBrainConsciousnessState_SIZE +
            OPTIMIZED_PhaseContext_SIZE +
            OPTIMIZED_AuthorityStatus_SIZE +
            OPTIMIZED_DeploymentConfig_SIZE +
            OPTIMIZED_ConquestTarget_SIZE +
            OPTIMIZED_StrategyContext_SIZE +
            OPTIMIZED_ConquestResult_SIZE +
            OPTIMIZED_StrategyResult_SIZE +
            OPTIMIZED_WarningContent_SIZE +
            OPTIMIZED_SelfAuditReport_SIZE +
            OPTIMIZED_CommandRecommendation_SIZE;
        
        return ((TotalOriginal - TotalOptimized) / (float)TotalOriginal) * 100.0f;
    }
    
    /**
     * 輸出優化報告到日誌
     */
    void LogOptimizationReport()
    {
        UE_LOG(LogTemp, Log, TEXT("========================================"));
        UE_LOG(LogTemp, Log, TEXT("MingGoRTS 記憶體優化報告"));
        UE_LOG(LogTemp, Log, TEXT("========================================"));
        
        TArray<FStructMemoryReport> Reports = GetOptimizationReports();
        for (const auto& Report : Reports)
        {
            UE_LOG(LogTemp, Log, TEXT("%s: %d bytes → %d bytes (減少 %.1f%%) - %s"),
                *Report.StructName.ToString(),
                Report.OriginalSize,
                Report.OptimizedSize,
                Report.MemoryReductionPercent,
                *Report.OptimizationNotes.ToString());
        }
        
        UE_LOG(LogTemp, Log, TEXT("----------------------------------------"));
        UE_LOG(LogTemp, Log, TEXT("總體記憶體節省: %.1f%%"), CalculateTotalMemorySavings());
        UE_LOG(LogTemp, Log, TEXT("========================================"));
    }
}

// ============================================================================
// 轉換函數：原始類型 ↔ 優化類型
// ============================================================================

class FMingSageBrainTypeConverter
{
public:
    // FSageBrainThoughtResult 轉換
    static FSageBrainThoughtResult_Optimized ConvertToOptimized(const FSageBrainThoughtResult& Original)
    {
        FSageBrainThoughtResult_Optimized Optimized;
        Optimized.Layer = Original.Layer;
        Optimized.Content = Original.Content;
        Optimized.Confidence = Original.Confidence;
        Optimized.Timestamp = Original.Timestamp;
        
        // 轉換元數據 (限制32項)
        for (const auto& Pair : Original.Metadata)
        {
            if (Optimized.Metadata.Num() < 32)
            {
                Optimized.Metadata.Add(Pair.Key, Pair.Value);
            }
        }
        
        return Optimized;
    }
    
    static FSageBrainThoughtResult ConvertFromOptimized(const FSageBrainThoughtResult_Optimized& Optimized)
    {
        FSageBrainThoughtResult Original;
        Original.Layer = Optimized.Layer;
        Original.Content = Optimized.Content.ToString();
        Original.Confidence = Optimized.Confidence;
        Original.Timestamp = Optimized.Timestamp;
        
        // 轉換元數據
        for (const auto& Pair : Optimized.Metadata)
        {
            Original.Metadata.Add(Pair.Key.ToString(), Pair.Value);
        }
        
        return Original;
    }
    
    // FPhaseContext 轉換
    static FPhaseContext_Optimized ConvertToOptimized(const FPhaseContext& Original)
    {
        FPhaseContext_Optimized Optimized;
        Optimized.PhaseName = Original.PhaseName;
        Optimized.DurationEstimate = Original.DurationEstimate;
        Optimized.PrimaryForce = Original.PrimaryForce;
        Optimized.StartTime = Original.StartTime;
        Optimized.ExpectedEndTime = Original.ExpectedEndTime;
        
        // 轉換目標 (限制16項)
        for (const auto& Objective : Original.Objectives)
        {
            if (Optimized.Objectives.Num() < 16)
            {
                Optimized.Objectives.Add(Objective);
            }
        }
        
        return Optimized;
    }
    
    static FPhaseContext ConvertFromOptimized(const FPhaseContext_Optimized& Optimized)
    {
        FPhaseContext Original;
        Original.PhaseName = Optimized.PhaseName.ToString();
        Original.DurationEstimate = Optimized.DurationEstimate;
        Original.PrimaryForce = Optimized.PrimaryForce;
        Original.StartTime = Optimized.StartTime;
        Original.ExpectedEndTime = Optimized.ExpectedEndTime;
        
        // 轉換目標
        for (const auto& Objective : Optimized.Objectives)
        {
            Original.Objectives.Add(Objective.ToString());
        }
        
        return Original;
    }
    
    // FAuthorityStatus 轉換
    static FAuthorityStatus_Optimized ConvertToOptimized(const FAuthorityStatus& Original)
    {
        FAuthorityStatus_Optimized Optimized;
        Optimized.AuthorityType = Original.AuthorityType;
        Optimized.bIsActive = Original.bIsActive;
        Optimized.SetHealthLevel(Original.HealthLevel);
        Optimized.SetCorruptionRisk(Original.CorruptionRisk);
        Optimized.LastAction = Original.LastAction;
        Optimized.LastCheckTime = Original.LastCheckTime;
        return Optimized;
    }
    
    static FAuthorityStatus ConvertFromOptimized(const FAuthorityStatus_Optimized& Optimized)
    {
        FAuthorityStatus Original;
        Original.AuthorityType = Optimized.AuthorityType;
        Original.bIsActive = Optimized.bIsActive;
        Original.HealthLevel = Optimized.GetHealthLevel();
        Original.CorruptionRisk = Optimized.GetCorruptionRisk();
        Original.LastAction = Optimized.LastAction.ToString();
        Original.LastCheckTime = Optimized.LastCheckTime;
        return Original;
    }
};
