#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "GameFramework/Actor.h"
#include "MingSageCommandSystem.generated.h"

UENUM(BlueprintType)
enum class ESageType: uint8 {
    Sage,           // 聖者：能用而能封
    DemonKing,      // 魔王：能用而不能停
    FalseSage       // 偽聖者：不能用卻假裝不用
};

UENUM(BlueprintType)
enum class EYinYangNature: uint8 {
    Yang,           // 陽：秩序、公開、可承擔
    Yin             // 陰：破局、隱蔽、非常態
};

UENUM(BlueprintType)
enum class EFiveElements: uint8 {
    Wood,           // 木：立名與正當性
    Fire,           // 火：造勢與加速
    Earth,          // 土：收權與止亂
    Metal,          // 金：裁斷與清除
    Water           // 水：留白與不動
};

UENUM(BlueprintType)
enum class ESixStrategies: uint8 {
    // 正六策：立國、立制、立人
    EstablishNation,     // 立國：伐心之正道
    EstablishSystem,     // 立制：伐勢、伐命之正道
    EstablishPeople,     // 立人：伐氣、伐影、伐無之正道
    
    // 逆六策：破局、破結構、不破人
    BreakSituation,      // 破局：非常之手段
    BreakStructure,      // 破結構：隱蔽之破壞
    PreservePeople       // 不破人：道德底線
};

UENUM(BlueprintType)
enum class ECommandAuthority: uint8 {
    TaoAuthority,         // 道權：最高指揮權
    StrategyAuthority,     // 策策權：戰術制定權
    MilitaryAuthority     // 兵兵權：執行指揮權
};

UENUM(BlueprintType)
enum class ESageFallWarning: uint8 {
    None,                 // 無墮落徵象
    ObsessedWithVictory,   // 沉迷勝利
    MeansAsEnds,         // 手段即目的
    LossOfSelfControl     // 失去自制
};

USTRUCT(BlueprintType)
struct FSageProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESageType SageType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YinBalance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YangBalance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SelfControlLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralCompass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 VictoriesCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnconventionalActionsCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasFallen;

    FSageProfile()
    {
        SageType = ESageType::Sage;
        YinBalance = 50.0f;
        YangBalance = 50.0f;
        SelfControlLevel = 100.0f;
        MoralCompass = 100.0f;
        VictoriesCount = 0;
        UnconventionalActionsCount = 0;
        bHasFallen = false;
    }
};

USTRUCT(BlueprintType)
struct FSituationAnalysis
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFiveElements CurrentPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UrgencyLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChaosLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PublicSupport;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EnemyStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRequiresUnconventionalMeans;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanUseConventionalMeans;

    FSituationAnalysis()
    {
        CurrentPhase = EFiveElements::Wood;
        UrgencyLevel = 50.0f;
        ChaosLevel = 50.0f;
        PublicSupport = 50.0f;
        EnemyStrength = 50.0f;
        bRequiresUnconventionalMeans = false;
        bCanUseConventionalMeans = true;
    }
};

USTRUCT(BlueprintType)
struct FCommandDecision
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESixStrategies ChosenStrategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EYinYangNature Nature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionProbability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessProbability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Reasoning;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsReversible;

    FCommandDecision()
    {
        ChosenStrategy = ESixStrategies::EstablishNation;
        Nature = EYinYangNature::Yang;
        ExecutionProbability = 100.0f;
        SuccessProbability = 50.0f;
        MoralCost = 0.0f;
        Reasoning = TEXT("");
        bIsReversible = true;
    }
};

USTRUCT(BlueprintType)
struct FFiveElementsRotation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFiveElements CurrentElement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFiveElements NextElement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PhaseProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TransitionThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsTransitioning;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<ESixStrategies> AvailableStrategies;

    FFiveElementsRotation()
    {
        CurrentElement = EFiveElements::Wood;
        NextElement = EFiveElements::Fire;
        PhaseProgress = 0.0f;
        TransitionThreshold = 80.0f;
        bIsTransitioning = false;
        AvailableStrategies.Add(ESixStrategies::EstablishNation);
    }
};

// 聖者指揮學核心系統接口
UINTERFACE(BlueprintType)
class MINGCORE_API USageCommandInterface : public UInterface
{
    GENERATED_BODY()
};

class MINGCORE_API ISageCommandInterface
{
    GENERATED_BODY()

public:
    // 分析當前局勢
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FSituationAnalysis AnalyzeSituation();

    // 制定指揮決策
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FCommandDecision MakeCommandDecision(const FSituationAnalysis& Analysis);

    // 檢查墮落徵象
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    ESageFallWarning CheckFallWarnings(const FSageProfile& Profile);

    // 執行五行輪轉
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FFiveElementsRotation ExecuteFiveElementsRotation(const FFiveElementsRotation& CurrentRotation);

    // 平衡陰陽
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void BalanceYinYang(float& YinBalance, float& YangBalance);

    // 防止墮落
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool PreventFall(FSageProfile& Profile);
};

/**
 * 至聖者指揮學核心系統
 * 
 * 核心理念：
 * 1. 正邪皆兵，而聖者不墮
 * 2. 陰陽不是善惡，而是可見與不可見
 * 3. 五行不是屬性，而是指揮節奏
 * 4. 指揮 = 切換權，而非行動
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingSageCommandSystem : public UObject, public ISageCommandInterface
{
    GENERATED_BODY()

public:
    UMingSageCommandSystem();

    // 初始化聖者指揮系統
    UFUNCTION(BlueprintCallable)
    void InitializeSageCommandSystem();

    // 分析當前局勢
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    FSituationAnalysis AnalyzeSituation();

    // 制定指揮決策
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    FCommandDecision MakeCommandDecision(const FSituationAnalysis& Analysis);

    // 檢查墮落徵象
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    ESageFallWarning CheckFallWarnings(const FSageProfile& Profile);

    // 執行五行輪轉
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    FFiveElementsRotation ExecuteFiveElementsRotation(const FFiveElementsRotation& CurrentRotation);

    // 平衡陰陽
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    void BalanceYinYang(float& YinBalance, float& YangBalance);

    // 防止墮落
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
    bool PreventFall(FSageProfile& Profile);

    // 獲取聖者檔案
    UFUNCTION(BlueprintCallable)
    FSageProfile GetSageProfile() const { return CurrentProfile; }

    // 設置聖者檔案
    UFUNCTION(BlueprintCallable)
    void SetSageProfile(const FSageProfile& NewProfile) { CurrentProfile = NewProfile; }

    // 獲取當前五行輪轉狀態
    UFUNCTION(BlueprintCallable)
    FFiveElementsRotation GetCurrentRotation() const { return CurrentRotation; }

    // 設置五行輪轉狀態
    UFUNCTION(BlueprintCallable)
    void SetCurrentRotation(const FFiveElementsRotation& NewRotation) { CurrentRotation = NewRotation; }

    // 應用至聖者指揮學原則
    UFUNCTION(BlueprintCallable)
    void ApplySagePrinciples();

    // 檢查是否可以切換策略
    UFUNCTION(BlueprintCallable)
    bool CanSwitchStrategy(ESixStrategies FromStrategy, ESixStrategies ToStrategy);

    // 計算道德成本
    UFUNCTION(BlueprintCallable)
    float CalculateMoralCost(ESixStrategies Strategy, EYinYangNature Nature);

    // 獲取推薦策略
    UFUNCTION(BlueprintCallable)
    TArray<ESixStrategies> GetRecommendedStrategies(const FSituationAnalysis& Analysis);

protected:
    // 當前聖者檔案
    UPROPERTY(BlueprintReadOnly)
    FSageProfile CurrentProfile;

    // 當前五行輪轉狀態
    UPROPERTY(BlueprintReadOnly)
    FFiveElementsRotation CurrentRotation;

    // 歷史決策記錄
    UPROPERTY(BlueprintReadOnly)
    TArray<FCommandDecision> DecisionHistory;

    // 墮落警告閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FallWarningThreshold;

    // 陰陽平衡閾值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YinYangBalanceThreshold;

    // 五行輪轉速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FiveElementsRotationSpeed;

private:
    // 內部輔助函數
    void UpdateSageType();
    void CheckAndUpdateRotation();
    void RecordDecision(const FCommandDecision& Decision);
    bool IsStrategyCompatible(ESixStrategies Strategy, EFiveElements Phase);
};
