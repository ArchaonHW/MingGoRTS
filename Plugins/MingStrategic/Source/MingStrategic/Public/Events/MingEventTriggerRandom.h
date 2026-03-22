#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerRandom.generated.h"

/**
 * 隨機觸發選項
 */
USTRUCT(BlueprintType)
struct FRandomTriggerOption
{
    GENERATED_BODY()
    
    // 選項ID (對應不同的事件)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionEventId;
    
    // 權重 (用於加權隨機)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight;
    
    // 最小觸發間隔 (冷卻)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinInterval;
    
    // 上次觸發時間
    UPROPERTY()
    float LastTriggerTime;
    
    FRandomTriggerOption()
        : Weight(1.0f)
        , MinInterval(0.0f)
        , LastTriggerTime(-1.0f)
    {}
};

/**
 * 隨機觸發器
 * 基於概率觸發事件
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerRandom : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingEventTriggerRandom();

    // 設置基礎觸發概率 (0-1)
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetBaseProbability(float Probability);

    // 設置概率衰減 (每次觸發後概率降低)
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetProbabilityDecay(float DecayFactor);

    // 設置最小/最大觸發間隔
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetIntervalRange(float MinInterval, float MaxInterval);

    // 添加加權隨機選項
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void AddWeightedOption(const FRandomTriggerOption& Option);

    // 清除所有選項
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void ClearOptions();

    // 獲取當前概率
    UFUNCTION(BlueprintPure, Category = "Random Trigger")
    float GetCurrentProbability() const { return CurrentProbability; }

    // 獲取距離下次可能觸發的時間
    UFUNCTION(BlueprintPure, Category = "Random Trigger")
    float GetTimeToNextPossibleTrigger() const;

    // 手動觸發隨機選擇 (返回選中的事件ID)
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    FString TriggerRandomSelection();

    // 重新計算概率
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void RecalculateProbability(float DeltaTime);

protected:
    // 基礎概率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Trigger")
    float BaseProbability;

    // 當前概率
    UPROPERTY()
    float CurrentProbability;

    // 概率衰減因子 (0-1, 1表示不衰減)
    UPROPERTY()
    float ProbabilityDecay;

    // 概率增長因子 (隨時間增加概率)
    UPROPERTY()
    float ProbabilityGrowth;

    // 最小觸發間隔
    UPROPERTY()
    float MinTriggerInterval;

    // 最大觸發間隔
    UPROPERTY()
    float MaxTriggerInterval;

    // 距離上次觸發的時間
    UPROPERTY()
    float TimeSinceLastTrigger;

    // 當前時間窗口內是否可觸發
    UPROPERTY()
    bool bCanTriggerInCurrentWindow;

    // 加權隨機選項列表
    UPROPERTY()
    TArray<FRandomTriggerOption> WeightedOptions;

    // 是否使用加權隨機
    UPROPERTY()
    bool bUseWeightedOptions;

    // 重寫基類方法
    virtual void Initialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // 執行隨機檢查
    bool RollProbability() const;

    // 選擇加權隨機選項
    FString SelectWeightedOption();

    // 更新時間窗口
    void UpdateTriggerWindow(float DeltaTime);

    // 檢查是否通過最小間隔
    bool HasPassedMinInterval() const;

    // 重置觸發狀態
    virtual void Reset() override;
};
