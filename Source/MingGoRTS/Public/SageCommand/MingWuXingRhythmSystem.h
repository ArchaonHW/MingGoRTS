// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingWuXingRhythmSystem.generated.h"

/**
 * 五行階段枚舉
 * 木(春)→火(夏)→土(長夏)→金(秋)→水(冬)→木(春)
 */
UENUM(BlueprintType)
enum class EWuXingPhase: uint8 {
    None    UMETA(DisplayName = "None"),
    Wood    UMETA(DisplayName = "木 (春/立名)"),      // 春、生、曲直
    Fire    UMETA(DisplayName = "火 (夏/造勢)"),      // 夏、炎、明
    Earth   UMETA(DisplayName = "土 (長夏/收權)"),  // 長夏、信、載
    Metal   UMETA(DisplayName = "金 (秋/裁斷)"),    // 秋、從革、殺
    Water   UMETA(DisplayName = "水 (冬/養機)"),    // 冬、藏、智
    Count
};

/**
 * 五行節奏狀態結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FWuXingRhythmState
{
    GENERATED_BODY()

    // 當前階段
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing")
    EWuXingPhase CurrentPhase = EWuXingPhase::None;

    // 下一階段
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing")
    EWuXingPhase NextPhase = EWuXingPhase::None;

    // 階段開始時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing")
    FDateTime PhaseStartTime;

    // 預計階段結束時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing")
    FDateTime PhaseEndTime;

    // 連擊計數 (正確順序累加)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing")
    int32 ComboCount = 0;

    // 當前效果倍率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing")
    float CurrentEffectMultiplier = 1.0f;

    // 是否處於相生狀態
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXing")
    bool bIsInGeneratingCycle = false;

    FWuXingRhythmState()
        : CurrentPhase(EWuXingPhase::None)
        , NextPhase(EWuXingPhase::None)
        , ComboCount(0)
        , CurrentEffectMultiplier(1.0f)
        , bIsInGeneratingCycle(false)
    {}
};

/**
 * 五行效果數據結構
 */
USTRUCT(BlueprintType)
struct MINGRTS_API FWuXingEffectData
{
    GENERATED_BODY()

    // 正策效果加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float RighteousStrategyBonus = 0.0f;

    // 逆策隱蔽加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float EvilStrategyStealthBonus = 0.0f;

    // 攻擊力加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float AttackBonus = 0.0f;

    // 防禦力加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float DefenseBonus = 0.0f;

    // 士氣增長加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float MoraleGrowthBonus = 0.0f;

    // 經濟效率加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float EconomicBonus = 0.0f;

    // 徵兵速度加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float RecruitmentBonus = 0.0f;

    // 將領成長加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float LeaderGrowthBonus = 0.0f;

    // 裁斷效果加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float JudgmentBonus = 0.0f;

    // 情報獲取加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WuXingEffect")
    float IntelligenceBonus = 0.0f;

    FWuXingEffectData()
        : RighteousStrategyBonus(0.0f)
        , EvilStrategyStealthBonus(0.0f)
        , AttackBonus(0.0f)
        , DefenseBonus(0.0f)
        , MoraleGrowthBonus(0.0f)
        , EconomicBonus(0.0f)
        , RecruitmentBonus(0.0f)
        , LeaderGrowthBonus(0.0f)
        , JudgmentBonus(0.0f)
        , IntelligenceBonus(0.0f)
    {}
};

/**
 * 至聖者指揮學 - 五行節奏系統 (WuXing Rhythm System)
 * 木(春/立名) → 火(夏/造勢) → 土(長夏/收權) → 金(秋/裁斷) → 水(冬/養機) → 木(春/再起)
 * 五行相生提供加成，五行相剋造成減益
 */
UCLASS(ClassGroup = (SageCommand), meta = (BlueprintSpawnableComponent))
class MINGRTS_API UMingWuXingRhythmSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingWuXingRhythmSystem();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    void InitializeWuXingSystem();

    // 開始新循環 (從木開始)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    void StartNewCycle();

    // 轉換到下一階段
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    bool AdvanceToNextPhase();

    // 強制設置當前階段
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    void SetCurrentPhase(EWuXingPhase NewPhase);

    // 獲取當前狀態
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    FWuXingRhythmState GetCurrentState() const { return CurrentState; }

    // 獲取當前階段效果
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    FWuXingEffectData GetCurrentPhaseEffects() const;

    // 獲取下一個應該的階段 (相生順序)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    EWuXingPhase GetNextPhaseInCycle(EWuXingPhase CurrentPhase) const;

    // 檢查階段轉換是否正確 (是否符合相生)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    bool IsPhaseTransitionCorrect(EWuXingPhase FromPhase, EWuXingPhase ToPhase) const;

    // 檢查是否相剋
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    bool IsPhaseOvercoming(EWuXingPhase Phase1, EWuXingPhase Phase2) const;

    // 獲取相生加成倍率
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    float GetGeneratingMultiplier() const;

    // 獲取相剋減益倍率
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    float GetOvercomingPenalty() const;

    // 獲取階段描述
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    FString GetPhaseDescription(EWuXingPhase Phase) const;

    // 獲取適合當前階段的策略建議
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    FString GetRecommendedStrategyForCurrentPhase() const;

    // 更新系統 (應在Tick中調用)
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    void UpdateSystem();

    // 重置連擊
    UFUNCTION(BlueprintCallable, Category = "SageCommand|WuXing")
    void ResetCombo();

    // 事件：階段轉換
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPhaseChanged, EWuXingPhase, OldPhase, EWuXingPhase, NewPhase);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|WuXing")
    FOnPhaseChanged OnPhaseChanged;

    // 事件：相生連擊達成
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGeneratingCombo, int32, ComboCount);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|WuXing")
    FOnGeneratingCombo OnGeneratingCombo;

    // 事件：相剋發生
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOvercomingOccured, EWuXingPhase, CurrentPhase, EWuXingPhase, WrongPhase);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|WuXing")
    FOnOvercomingOccured OnOvercomingOccured;

    // 事件：循環完成
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCycleCompleted);
    UPROPERTY(BlueprintAssignable, Category = "SageCommand|WuXing")
    FOnCycleCompleted OnCycleCompleted;

protected:
    // 獲取階段效果數據
    FWuXingEffectData GetPhaseEffectData(EWuXingPhase Phase) const;

    // 計算連擊加成
    float CalculateComboMultiplier() const;

    // 檢查循環是否完成
    bool CheckCycleCompletion() const;

    // 獲取相生關係
    EWuXingPhase GetGeneratedPhase(EWuXingPhase Phase) const;

    // 獲取相剋關係
    EWuXingPhase GetOvercomePhase(EWuXingPhase Phase) const;

protected:
    // 當前狀態
    UPROPERTY()
    FWuXingRhythmState CurrentState;

    // 階段持續時間 (秒)
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|WuXing")
    float PhaseDuration = 120.0f; // 默認2分鐘每階段

    // 相生加成基礎值
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|WuXing")
    float GeneratingBaseMultiplier = 1.2f;

    // 相剋減益基礎值
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|WuXing")
    float OvercomingBasePenalty = 0.8f;

    // 連擊加成遞增
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|WuXing")
    float ComboBonusIncrement = 0.1f;

    // 最大連擊數
    UPROPERTY(EditDefaultsOnly, Category = "SageCommand|WuXing")
    int32 MaxComboCount = 10;

    // 是否已初始化
    UPROPERTY()
    bool bIsInitialized = false;
};

