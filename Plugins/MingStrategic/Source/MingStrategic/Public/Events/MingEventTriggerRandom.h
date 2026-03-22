#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerRandom.generated.h"

/**
 * ?��?觸發?��?
 */
USTRUCT(BlueprintType)
struct FRandomTriggerOption
{
    GENERATED_BODY()
    
    // ?��?ID (對�?不�X��?�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionEventId;
    
    // 權�? (?�於?��X��?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight;
    
    // ?�小觸?��X(?�卻)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinInterval;
    
    // 上次觸發?��?
    UPROPERTY()
    float LastTriggerTime;
    
    FRandomTriggerOption()
        : Weight(1.0f)
        , MinInterval(0.0f)
        , LastTriggerTime(-1.0f)
    {}
};

/**
 * ?��?觸發X * ?�於概�?觸發事件
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerRandom : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingEventTriggerRandom();

    // 設置?��?觸發概�? (0-1)
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetBaseProbability(float Probability);

    // 設置概�?衰�? (每次觸發後�X��?�?
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetProbabilityDecay(float DecayFactor);

    // 設置?��X�大觸?��X    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetIntervalRange(float MinInterval, float MaxInterval);

    // 添�X��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void AddWeightedOption(const FRandomTriggerOption& Option);

    // 清除?�?�選X    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void ClearOptions();

    // ?��X��?概�?
    UFUNCTION(BlueprintPure, Category = "Random Trigger")
    float GetCurrentProbability() const { return CurrentProbability; }

    // ?��?距離下次?�能觸發?��X    UFUNCTION(BlueprintPure, Category = "Random Trigger")
    float GetTimeToNextPossibleTrigger() const;

    // ?��?觸發?��X��? (返�X�中?��?件ID)
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    FString TriggerRandomSelection();

    // ?�新計�?概�?
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void RecalculateProbability(float DeltaTime);

protected:
    // ?��?概�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Trigger")
    float BaseProbability;

    // ?��?概�?
    UPROPERTY()
    float CurrentProbability;

    // 概�?衰�X��? (0-1, 1表示不衰�?
    UPROPERTY()
    float ProbabilityDecay;

    // 概�?增長?��? (?��X��X��X
    UPROPERTY()
    float ProbabilityGrowth;

    // ?�小觸?��X    UPROPERTY()
    float MinTriggerInterval;

    // ?�大觸?��X    UPROPERTY()
    float MaxTriggerInterval;

    // 距離上次觸發?��X    UPROPERTY()
    float TimeSinceLastTrigger;

    // ?��X��?窗口?�是?�可觸發
    UPROPERTY()
    bool bCanTriggerInCurrentWindow;

    // ?��X��X��X�表
    UPROPERTY()
    TArray<FRandomTriggerOption> WeightedOptions;

    // ?�否使用?��X��?
    UPROPERTY()
    bool bUseWeightedOptions;

    // ?�寫?��X��?
    virtual void Initialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // ?��X��?檢查
    bool RollProbability() const;

    // ?��X��X��X��?
    FString SelectWeightedOption();

    // ?�新?��?窗口
    void UpdateTriggerWindow(float DeltaTime);

    // 檢查?�否?��X�小�X    bool HasPassedMinInterval() const;

    // ?�置觸發?�X    virtual void Reset() override;
};

