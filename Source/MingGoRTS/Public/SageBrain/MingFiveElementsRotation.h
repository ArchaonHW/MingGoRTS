#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingFiveElementsRotation.generated.h"

USTRUCT(BlueprintType)
struct FWoodPhaseConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString PhaseName;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    FString RighteousFlagName;  // 正旗名稱
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> Objectives;  // 目標列表
    
    UPROPERTY(BlueprintReadWrite)
    float LegitimacyTarget;  // 正當性目標值
    
    FWoodPhaseConfig()
        : Duration(300.0f)
        , LegitimacyTarget(0.8f)
    {}
};

USTRUCT(BlueprintType)
struct FFirePhaseConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString PhaseName;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    float MomentumTarget;  // 造勢目標
    
    UPROPERTY(BlueprintReadWrite)
    float EvilFuelIntensity;  // 邪燃料強度
    
    UPROPERTY(BlueprintReadWrite)
    bool bUseCovertSupport;  // 是否使用隱蔽支持
    
    UPROPERTY(BlueprintReadWrite)
    float MaxTemperature;  // 最高熱度限制
    
    FFirePhaseConfig()
        : Duration(200.0f)
        , MomentumTarget(0.9f)
        , EvilFuelIntensity(0.3f)
        , bUseCovertSupport(true)
        , MaxTemperature(0.95f)
    {}
};

USTRUCT(BlueprintType)
struct FEarthPhaseConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString PhaseName;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    bool bSealEvilForces;  // 是否封存邪兵
    
    UPROPERTY(BlueprintReadWrite)
    bool bStopMomentum;  // 是否停止造勢
    
    UPROPERTY(BlueprintReadWrite)
    bool bConsolidatePower;  // 是否收權
    
    UPROPERTY(BlueprintReadWrite)
    float PowerConsolidationTarget;  // 權力收攏目標
    
    FEarthPhaseConfig()
        : Duration(250.0f)
        , bSealEvilForces(true)
        , bStopMomentum(true)
        , bConsolidatePower(true)
        , PowerConsolidationTarget(0.85f)
    {}
};

USTRUCT(BlueprintType)
struct FMetalPhaseConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString PhaseName;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> EliminationTargets;  // 清除目標
    
    UPROPERTY(BlueprintReadWrite)
    bool bColdJudgment;  // 是否冷裁斷
    
    UPROPERTY(BlueprintReadWrite)
    bool bRemoveRedundancy;  // 是否去冗
    
    UPROPERTY(BlueprintReadWrite)
    float JudgmentSeverity;  // 裁斷嚴厲程度
    
    FMetalPhaseConfig()
        : Duration(150.0f)
        , bColdJudgment(true)
        , bRemoveRedundancy(true)
        , JudgmentSeverity(0.7f)
    {}
};

USTRUCT(BlueprintType)
struct FWaterPhaseConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString PhaseName;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    bool bHideStrength;  // 是否藏鋒
    
    UPROPERTY(BlueprintReadWrite)
    bool bWaitForOpportunity;  // 是否待機
    
    UPROPERTY(BlueprintReadWrite)
    bool bLetEnemyExhaust;  // 是否讓敵自竭
    
    UPROPERTY(BlueprintReadWrite)
    float InactionLevel;  // 不作為程度
    
    FWaterPhaseConfig()
        : Duration(400.0f)
        , bHideStrength(true)
        , bWaitForOpportunity(true)
        , bLetEnemyExhaust(true)
        , InactionLevel(0.8f)
    {}
};

USTRUCT(BlueprintType)
struct FRotationConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    float RighteousRatio;  // 正兵比例
    
    UPROPERTY(BlueprintReadWrite)
    float EvilRatio;  // 邪兵比例
    
    UPROPERTY(BlueprintReadWrite)
    float RotationSpeed;  // 輪轉速度
    
    UPROPERTY(BlueprintReadWrite)
    bool bAllowHybridDeployment;  // 是否允許混合部署
    
    UPROPERTY(BlueprintReadWrite)
    TMap<EFiveElementPhase, float> PhaseForceDistribution;
    
    FRotationConfig()
        : RighteousRatio(0.7f)
        , EvilRatio(0.3f)
        , RotationSpeed(1.0f)
        , bAllowHybridDeployment(true)
    {}
};

USTRUCT(BlueprintType)
struct FPhaseMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EFiveElementPhase Phase;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentLegitimacy;  // 當前正當性
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentMomentum;  // 當前勢能
    
    UPROPERTY(BlueprintReadOnly)
    float PowerConsolidation;  // 權力收攏度
    
    UPROPERTY(BlueprintReadOnly)
    float ClarityLevel;  // 清晰度
    
    UPROPERTY(BlueprintReadOnly)
    float HiddenStrength;  // 潛藏實力
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsPhaseComplete;
    
    FPhaseMetrics()
        : Phase(EFiveElementPhase::None)
        , CurrentLegitimacy(0.0f)
        , CurrentMomentum(0.0f)
        , PowerConsolidation(0.0f)
        , ClarityLevel(0.0f)
        , HiddenStrength(0.0f)
        , bIsPhaseComplete(false)
    {}
};

/**
 * 五行輪轉器
 * 實現木火土金水五階段的正邪輪轉
 */
UCLASS()
class MINGGORTS_API UMingFiveElementsRotation : public UObject
{
    GENERATED_BODY()
    
public:
    UMingFiveElementsRotation();
    
    /** 初始化輪轉器 */
    UFUNCTION(BlueprintCallable)
    bool Initialize();
    
    // ========== 五階段管理 ==========
    
    /** 進入木階段（立名） */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Wood")
    bool StartWoodPhase(const FWoodPhaseConfig& Config);
    
    /** 進入火階段（造勢） */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Fire")
    bool StartFirePhase(const FFirePhaseConfig& Config);
    
    /** 進入土階段（收權） */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Earth")
    bool StartEarthPhase(const FEarthPhaseConfig& Config);
    
    /** 進入金階段（裁斷） */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Metal")
    bool StartMetalPhase(const FMetalPhaseConfig& Config);
    
    /** 進入水階段（留白） */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Water")
    bool StartWaterPhase(const FWaterPhaseConfig& Config);
    
    /** 結束當前階段 */
    UFUNCTION(BlueprintCallable)
    bool EndCurrentPhase();
    
    /** 強制轉換到指定階段 */
    UFUNCTION(BlueprintCallable)
    bool ForceTransitionToPhase(EFiveElementPhase TargetPhase);
    
    // ========== 正邪輪轉 ==========
    
    /** 在當前階段部署正邪兵種 */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Rotation")
    bool RotateRighteousAndEvil(const FRotationConfig& Config);
    
    /** 檢查正邪比例是否合適 */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Rotation")
    bool IsForceRatioAppropriate() const;
    
    /** 獲取當前階段推薦的正邪比例 */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Rotation")
    FRotationConfig GetRecommendedRotationConfig() const;
    
    /** 封鎖邪兵（土階段用） */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Rotation")
    bool SealEvilForces();
    
    /** 解封邪兵 */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|Rotation")
    bool UnsealEvilForces();
    
    // ========== 階段檢查與評估 ==========
    
    /** 檢查是否適合階段轉換 */
    UFUNCTION(BlueprintCallable)
    bool CanTransitionTo(EFiveElementPhase TargetPhase) const;
    
    /** 獲取當前階段指標 */
    UFUNCTION(BlueprintCallable)
    FPhaseMetrics GetCurrentPhaseMetrics() const;
    
    /** 檢查當前階段是否完成 */
    UFUNCTION(BlueprintCallable)
    bool IsCurrentPhaseComplete() const;
    
    /** 獲取建議的下一階段 */
    UFUNCTION(BlueprintCallable)
    EFiveElementPhase GetRecommendedNextPhase() const;
    
    /** 評估階段執行效果 */
    UFUNCTION(BlueprintCallable)
    float EvaluatePhaseEffectiveness(EFiveElementPhase Phase) const;
    
    // ========== 特殊階段功能 ==========
    
    /** 木火階段：正旗邪燃料模式 */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|WoodFire")
    bool ApplyRighteousFlagEvilFuelPattern();
    
    /** 土階段：封邪止勢收權 */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|EarthSeal")
    bool ApplyEarthSealPattern();
    
    /** 金水階段：裁斷與不作為 */
    UFUNCTION(BlueprintCallable, Category = "FiveElements|MetalWater")
    bool ApplyMetalWaterPattern();
    
    /** 檢查是否需要強制進入土階段（火太旺） */
    UFUNCTION(BlueprintCallable)
    bool IsEarthPhaseForced() const;
    
private:
    UPROPERTY()
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY()
    FWoodPhaseConfig WoodConfig;
    
    UPROPERTY()
    FFirePhaseConfig FireConfig;
    
    UPROPERTY()
    FEarthPhaseConfig EarthConfig;
    
    UPROPERTY()
    FMetalPhaseConfig MetalConfig;
    
    UPROPERTY()
    FWaterPhaseConfig WaterConfig;
    
    UPROPERTY()
    FPhaseMetrics CurrentMetrics;
    
    UPROPERTY()
    FDateTime PhaseStartTime;
    
    UPROPERTY()
    bool bEvilForcesSealed;
    
    UPROPERTY()
    TArray<EFiveElementPhase> PhaseHistory;
    
    /** 計算階段自然流向 */
    EFiveElementPhase CalculateNaturalFlow() const;
    
    /** 更新階段指標 */
    void UpdatePhaseMetrics();
    
    /** 檢查火是否太旺 */
    bool IsFireTooStrong() const;
};
