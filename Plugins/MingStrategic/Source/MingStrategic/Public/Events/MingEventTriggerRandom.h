#pragma once

#include "CoreMinimal.h"
#include "Events/MingEventTrigger.h"
#include "MingEventTriggerRandom.generated.h"

/**
 * ?¨æ?è§¸ç™¼?¸é?
 */
USTRUCT(BlueprintType)
struct FRandomTriggerOption
{
    GENERATED_BODY()
    
    // ?¸é?ID (å°æ?ä¸å??„ä?ä»?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionEventId;
    
    // æ¬Šé? (?¨æ–¼? æ??¨æ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight;
    
    // ?€å°è§¸?¼é???(?·å»)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinInterval;
    
    // ä¸Šæ¬¡è§¸ç™¼?‚é?
    UPROPERTY()
    float LastTriggerTime;
    
    FRandomTriggerOption()
        : Weight(1.0f)
        , MinInterval(0.0f)
        , LastTriggerTime(-1.0f)
    {}
};

/**
 * ?¨æ?è§¸ç™¼?? * ?ºæ–¼æ¦‚ç?è§¸ç™¼äº‹ä»¶
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventTriggerRandom : public UMingEventTrigger
{
    GENERATED_BODY()

public:
    UMingEventTriggerRandom();

    // è¨­ç½®?ºç?è§¸ç™¼æ¦‚ç? (0-1)
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetBaseProbability(float Probability);

    // è¨­ç½®æ¦‚ç?è¡°æ? (æ¯æ¬¡è§¸ç™¼å¾Œæ??‡é?ä½?
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetProbabilityDecay(float DecayFactor);

    // è¨­ç½®?€å°??€å¤§è§¸?¼é???    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void SetIntervalRange(float MinInterval, float MaxInterval);

    // æ·»å?? æ??¨æ??¸é?
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void AddWeightedOption(const FRandomTriggerOption& Option);

    // æ¸…é™¤?€?‰é¸??    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void ClearOptions();

    // ?²å??¶å?æ¦‚ç?
    UFUNCTION(BlueprintPure, Category = "Random Trigger")
    float GetCurrentProbability() const { return CurrentProbability; }

    // ?²å?è·é›¢ä¸‹æ¬¡?¯èƒ½è§¸ç™¼?„æ???    UFUNCTION(BlueprintPure, Category = "Random Trigger")
    float GetTimeToNextPossibleTrigger() const;

    // ?‹å?è§¸ç™¼?¨æ??¸æ? (è¿”å??¸ä¸­?„ä?ä»¶ID)
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    FString TriggerRandomSelection();

    // ?æ–°è¨ˆç?æ¦‚ç?
    UFUNCTION(BlueprintCallable, Category = "Random Trigger")
    void RecalculateProbability(float DeltaTime);

protected:
    // ?ºç?æ¦‚ç?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Random Trigger")
    float BaseProbability;

    // ?¶å?æ¦‚ç?
    UPROPERTY()
    float CurrentProbability;

    // æ¦‚ç?è¡°æ?? å? (0-1, 1è¡¨ç¤ºä¸è¡°æ¸?
    UPROPERTY()
    float ProbabilityDecay;

    // æ¦‚ç?å¢é•·? å? (?¨æ??“å?? æ???
    UPROPERTY()
    float ProbabilityGrowth;

    // ?€å°è§¸?¼é???    UPROPERTY()
    float MinTriggerInterval;

    // ?€å¤§è§¸?¼é???    UPROPERTY()
    float MaxTriggerInterval;

    // è·é›¢ä¸Šæ¬¡è§¸ç™¼?„æ???    UPROPERTY()
    float TimeSinceLastTrigger;

    // ?¶å??‚é?çª—å£?§æ˜¯?¦å¯è§¸ç™¼
    UPROPERTY()
    bool bCanTriggerInCurrentWindow;

    // ? æ??¨æ??¸é??—è¡¨
    UPROPERTY()
    TArray<FRandomTriggerOption> WeightedOptions;

    // ?¯å¦ä½¿ç”¨? æ??¨æ?
    UPROPERTY()
    bool bUseWeightedOptions;

    // ?å¯«?ºé??¹æ?
    virtual void Initialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool PerformTrigger() override;
    virtual bool CheckTriggerCondition() const override;

    // ?·è??¨æ?æª¢æŸ¥
    bool RollProbability() const;

    // ?¸æ?? æ??¨æ??¸é?
    FString SelectWeightedOption();

    // ?´æ–°?‚é?çª—å£
    void UpdateTriggerWindow(float DeltaTime);

    // æª¢æŸ¥?¯å¦?šé??€å°é???    bool HasPassedMinInterval() const;

    // ?ç½®è§¸ç™¼?€??    virtual void Reset() override;
};
