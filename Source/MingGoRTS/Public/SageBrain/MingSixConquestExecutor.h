#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSixConquestExecutor.generated.h"

USTRUCT(BlueprintType)
struct FConquestMindConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString TargetMindset;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> PsychologicalWeaknesses;
    
    UPROPERTY(BlueprintReadWrite)
    float DoubtSeedIntensity;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUsePropaganda;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUseDeception;
    
    FConquestMindConfig()
        : DoubtSeedIntensity(0.5f)
        , bUsePropaganda(false)
        , bUseDeception(false)
    {}
};

USTRUCT(BlueprintType)
struct FConquestSpiritConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    float MoraleReductionTarget;
    
    UPROPERTY(BlueprintReadWrite)
    float FatigueInductionRate;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUseContinuousPressure;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUseFalseHope;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> SpiritBreakingTactics;
    
    FConquestSpiritConfig()
        : MoraleReductionTarget(0.5f)
        , FatigueInductionRate(0.3f)
        , bUseContinuousPressure(true)
        , bUseFalseHope(false)
    {}
};

USTRUCT(BlueprintType)
struct FConquestMomentumConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    float MomentumDisruptionLevel;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> TimingInterferencePoints;
    
    UPROPERTY(BlueprintReadWrite)
    bool bReverseFlow;
    
    UPROPERTY(BlueprintReadWrite)
    bool bCreateFalseMomentum;
    
    UPROPERTY(BlueprintReadWrite)
    float AsymmetryLevel;
    
    FConquestMomentumConfig()
        : MomentumDisruptionLevel(0.6f)
        , bReverseFlow(false)
        , bCreateFalseMomentum(false)
        , AsymmetryLevel(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FConquestLifeConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> CriticalResources;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> SupplyLines;
    
    UPROPERTY(BlueprintReadWrite)
    bool bCutReinforcements;
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsolateFromAllies;
    
    UPROPERTY(BlueprintReadWrite)
    float SustainabilityReduction;
    
    FConquestLifeConfig()
        : bCutReinforcements(true)
        , bIsolateFromAllies(true)
        , SustainabilityReduction(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FConquestShadowConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    bool bCreateFalseImage;
    
    UPROPERTY(BlueprintReadWrite)
    bool bSpreadDisinformation;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> DeceptionChannels;
    
    UPROPERTY(BlueprintReadWrite)
    float ImageConfusionLevel;
    
    UPROPERTY(BlueprintReadWrite)
    bool bMimicEnemy;
    
    FConquestShadowConfig()
        : bCreateFalseImage(false)
        , bSpreadDisinformation(false)
        , ImageConfusionLevel(0.5f)
        , bMimicEnemy(false)
    {}
};

USTRUCT(BlueprintType)
struct FConquestNothingConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    bool bLeaveNoTrace;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUseDeniability;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> CutoutLayers;
    
    UPROPERTY(BlueprintReadWrite)
    float InvisibilityLevel;
    
    UPROPERTY(BlueprintReadWrite)
    bool bSelfDestructMechanism;
    
    FConquestNothingConfig()
        : bLeaveNoTrace(true)
        , bUseDeniability(true)
        , InvisibilityLevel(0.8f)
        , bSelfDestructMechanism(false)
    {}
};

USTRUCT(BlueprintType)
struct FConquestExecutionPlan
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    ESixConquestStrategy Strategy;
    
    UPROPERTY(BlueprintReadWrite)
    EStrategyApproach Approach;
    
    UPROPERTY(BlueprintReadWrite)
    FConquestTarget Target;
    
    UPROPERTY(BlueprintReadWrite)
    float ResourceRequirement;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> Prerequisites;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> RiskFactors;
    
    FConquestExecutionPlan()
        : Strategy(ESixConquestStrategy::ConquerMind)
        , Approach(EStrategyApproach::Righteous)
        , ResourceRequirement(0.0f)
        , Duration(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FConquestEffectivenessMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float DirectDamage;
    
    UPROPERTY(BlueprintReadOnly)
    float RippleEffect;
    
    UPROPERTY(BlueprintReadOnly)
    float SustainabilityImpact;
    
    UPROPERTY(BlueprintReadOnly)
    float RecoveryDifficulty;
    
    UPROPERTY(BlueprintReadOnly)
    float CostEfficiency;
    
    UPROPERTY(BlueprintReadOnly)
    float SideEffectSeverity;
    
    FConquestEffectivenessMetrics()
        : DirectDamage(0.0f)
        , RippleEffect(0.0f)
        , SustainabilityImpact(0.0f)
        , RecoveryDifficulty(0.0f)
        , CostEfficiency(0.0f)
        , SideEffectSeverity(0.0f)
    {}
};

/**
 * 六伐執行器
 * 實現伐心、伐氣、伐勢、伐命、伐影、伐無六種策略
 */
UCLASS()
class MINGGORTS_API UMingSixConquestExecutor : public UObject
{
    GENERATED_BODY()
    
public:
    UMingSixConquestExecutor();
    
    /** 初始化執行器 */
    UFUNCTION(BlueprintCallable)
    bool Initialize();
    
    // ========== 六伐執行接口 ==========
    
    /** 伐心 - 奪其志 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|ConquerMind")
    FConquestResult ExecuteConquerMind(
        const FConquestMindConfig& Config,
        const FConquestTarget& Target,
        EStrategyApproach Approach
    );
    
    /** 伐氣 - 挫其銳 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|ConquerSpirit")
    FConquestResult ExecuteConquerSpirit(
        const FConquestSpiritConfig& Config,
        const FConquestTarget& Target,
        EStrategyApproach Approach
    );
    
    /** 伐勢 - 逆其時 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|ConquerMomentum")
    FConquestResult ExecuteConquerMomentum(
        const FConquestMomentumConfig& Config,
        const FConquestTarget& Target,
        EStrategyApproach Approach
    );
    
    /** 伐命 - 斷其根 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|ConquerLife")
    FConquestResult ExecuteConquerLife(
        const FConquestLifeConfig& Config,
        const FConquestTarget& Target,
        EStrategyApproach Approach
    );
    
    /** 伐影 - 亂其象 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|ConquerShadow")
    FConquestResult ExecuteConquerShadow(
        const FConquestShadowConfig& Config,
        const FConquestTarget& Target,
        EStrategyApproach Approach
    );
    
    /** 伐無 - 藏其跡 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|ConquerNothing")
    FConquestResult ExecuteConquerNothing(
        const FConquestNothingConfig& Config,
        const FConquestTarget& Target,
        EStrategyApproach Approach
    );
    
    /** 通用執行接口 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests")
    FConquestResult ExecuteConquest(
        ESixConquestStrategy Strategy,
        const FConquestExecutionPlan& Plan
    );
    
    // ========== 策略規劃 ==========
    
    /** 生成伐心計劃 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Planning")
    FConquestExecutionPlan GenerateMindConquestPlan(
        const FConquestTarget& Target,
        EStrategyApproach Approach
    ) const;
    
    /** 評估策略適用性 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Planning")
    float EvaluateStrategyFit(
        ESixConquestStrategy Strategy,
        const FConquestTarget& Target,
        const FSituationAssessment& Situation
    ) const;
    
    /** 獲取推薦策略排序 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Planning")
    TArray<ESixConquestStrategy> GetRecommendedStrategyOrder(
        const FConquestTarget& Target,
        const FSituationAssessment& Situation
    ) const;
    
    /** 組合多種伐術 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Planning")
    FConquestExecutionPlan CreateCombinedConquestPlan(
        const TArray<ESixConquestStrategy>& Strategies,
        const FConquestTarget& Target
    ) const;
    
    // ========== 效果評估 ==========
    
    /** 評估執行效果 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Evaluation")
    FConquestEffectivenessMetrics EvaluateEffectiveness(
        const FConquestResult& Result,
        const FConquestExecutionPlan& Plan
    ) const;
    
    /** 計算道德成本 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Evaluation")
    float CalculateMoralCost(
        ESixConquestStrategy Strategy,
        EStrategyApproach Approach,
        const FConquestTarget& Target
    ) const;
    
    /** 預測長期影響 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Evaluation")
    TArray<FString> PredictLongTermEffects(
        const FConquestResult& Result,
        int32 PredictionHorizon
    ) const;
    
    // ========== 正逆差異化執行 ==========
    
    /** 正道伐心 - 立國 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Righteous")
    FConquestResult ExecuteRighteousConquerMind(const FConquestTarget& Target);
    
    /** 逆道伐心 - 不破人 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Reverse")
    FConquestResult ExecuteReverseConquerMind(const FConquestTarget& Target);
    
    /** 正道伐勢 - 立制 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Righteous")
    FConquestResult ExecuteRighteousConquerMomentum(const FConquestTarget& Target);
    
    /** 逆道伐勢 - 破局 */
    UFUNCTION(BlueprintCallable, Category = "SixConquests|Reverse")
    FConquestResult ExecuteReverseConquerMomentum(const FConquestTarget& Target);
    
private:
    UPROPERTY()
    TArray<FConquestResult> ExecutionHistory;
    
    UPROPERTY()
    TMap<ESixConquestStrategy, int32> ExecutionCounts;
    
    UPROPERTY()
    TMap<ESixConquestStrategy, float> AverageEffectiveness;
    
    /** 計算伐心效果 */
    float CalculateMindConquestEffect(
        const FConquestMindConfig& Config,
        const FConquestTarget& Target
    ) const;
    
    /** 計算伐氣效果 */
    float CalculateSpiritConquestEffect(
        const FConquestSpiritConfig& Config,
        const FConquestTarget& Target
    ) const;
    
    /** 計算伐勢效果 */
    float CalculateMomentumConquestEffect(
        const FConquestMomentumConfig& Config,
        const FConquestTarget& Target
    ) const;
    
    /** 計算伐命效果 */
    float CalculateLifeConquestEffect(
        const FConquestLifeConfig& Config,
        const FConquestTarget& Target
    ) const;
    
    /** 計算伐影效果 */
    float CalculateShadowConquestEffect(
        const FConquestShadowConfig& Config,
        const FConquestTarget& Target
    ) const;
    
    /** 計算伐無效果 */
    float CalculateNothingConquestEffect(
        const FConquestNothingConfig& Config,
        const FConquestTarget& Target
    ) const;
};
