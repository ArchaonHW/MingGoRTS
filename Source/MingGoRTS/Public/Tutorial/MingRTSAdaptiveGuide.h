#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAdaptiveGuide.generated.h"

/**
 * ÂºïÂ??êÁ§∫È°ûÂ?
 */
UENUM(BlueprintType)
enum class EGuideTipType : uint8
{
    Info                UMETA(DisplayName = "‰ø°ÊÅØ?êÁ§∫"),
    Warning             UMETA(DisplayName = "Ë≠¶Â??êÁ§∫"),
    Suggestion          UMETA(DisplayName = "Âª∫Ë≠∞?êÁ§∫"),
    Alert               UMETA(DisplayName = "Á∑äÊÄ•Ê?Á§?),
    Tutorial            UMETA(DisplayName = "?ôÂ≠∏?êÁ§∫"),
    Contextual          UMETA(DisplayName = "?ÖÂ??êÁ§∫")
};

/**
 * ÂºïÂ??êÁ§∫?∏Ê?
 */
USTRUCT(BlueprintType)
struct FAdaptiveGuideTip
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TipID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EGuideTipType TipType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Message;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TriggerCondition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownSeconds;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanBeDisabled;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RelatedTutorialStage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Keywords;
    
    FAdaptiveGuideTip()
        : TipType(EGuideTipType::Info)
        , CooldownSeconds(60.0f)
        , bCanBeDisabled(true)
    {}
};

/**
 * ?©ÂÆ∂Ë°åÁÇ∫?∏Ê?
 */
USTRUCT(BlueprintType)
struct FPlayerBehaviorMetrics
{
    GENERATED_BODY()
    
    UPROPERTY()
    int32 IdleWorkerCount;
    
    UPROPERTY()
    int32 UnspentResources;
    
    UPROPERTY()
    int32 UnusedPopulation;
    
    UPROPERTY()
    float TimeSinceLastScout;
    
    UPROPERTY()
    int32 ArmySize;
    
    UPROPERTY()
    int32 EnemyArmySize;
    
    UPROPERTY()
    bool bIsUnderAttack;
    
    UPROPERTY()
    bool bHasIdleProduction;
    
    UPROPERTY()
    float APM;
    
    FPlayerBehaviorMetrics()
        : IdleWorkerCount(0)
        , UnspentResources(0)
        , UnusedPopulation(0)
        , TimeSinceLastScout(0.0f)
        , ArmySize(0)
        , EnemyArmySize(0)
        , bIsUnderAttack(false)
        , bHasIdleProduction(false)
        , APM(0.0f)
    {}
};

/**
 * ?™ÈÅ©?âÂ?Â∞éÁ≥ªÁµ?- ?πÊ??©ÂÆ∂Ë°åÁÇ∫?ê‰??∫ËÉΩ?êÁ§∫
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSAdaptiveGuide : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSAdaptiveGuide();
    
    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void InitializeAdaptiveGuide();
    
    // ?¥Êñ∞?©ÂÆ∂Ë°åÁÇ∫?∏Ê? (ÊØèÂ??ñÂ??ÇË™ø??
    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void UpdatePlayerMetrics(const FPlayerBehaviorMetrics& Metrics);
    
    // Ëß∏Áôº?ÖÂ??êÁ§∫
    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void TriggerContextualTip(FName TipID);
    
    // Ê™¢Êü•‰∏¶È°ØÁ§∫ÈÅ©?∂Á??êÁ§∫
    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void EvaluateAndShowTips();
    
    // Á¶ÅÁî®?πÂ??êÁ§∫
    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void DisableTip(FName TipID);
    
    // ?çÊñ∞?üÁî®?êÁ§∫
    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void EnableTip(FName TipID);
    
    // Ë®≠ÁΩÆÂºïÂ??üÁî®?Ä??    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void SetGuideEnabled(bool bEnabled);
    
    // ?≤Â??êÁ§∫Ê≠∑Âè≤
    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    TArray<FName> GetRecentTips(float WithinSeconds) const;
    
    // ?ãÂ?È°ØÁ§∫?êÁ§∫
    UFUNCTION(BlueprintCallable, Category = "AdaptiveGuide")
    void ShowManualTip(const FText& Title, const FText& Message, EGuideTipType Type);
    
    // ‰∫ã‰ª∂ÂßîË?
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGuideTipShown, const FAdaptiveGuideTip&, Tip);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGuideTipTriggered, FName, TipID, EGuideTipType, Type);
    
    UPROPERTY(BlueprintAssignable, Category = "AdaptiveGuide|Events")
    FOnGuideTipShown OnGuideTipShown;
    
    UPROPERTY(BlueprintAssignable, Category = "AdaptiveGuide|Events")
    FOnGuideTipTriggered OnGuideTipTriggered;
    
private:
    UPROPERTY()
    TArray<FAdaptiveGuideTip> GuideTips;
    
    UPROPERTY()
    FPlayerBehaviorMetrics CurrentMetrics;
    
    UPROPERTY()
    TMap<FName, float> LastShownTime;
    
    UPROPERTY()
    TArray<FName> DisabledTips;
    
    UPROPERTY()
    bool bGuideEnabled;
    
    UPROPERTY()
    float EvaluationInterval;
    
    UPROPERTY()
    FTimerHandle EvaluationTimerHandle;
    
    void LoadGuideTips();
    void StartEvaluationTimer();
    void StopEvaluationTimer();
    
    bool ShouldShowTip(const FAdaptiveGuideTip& Tip) const;
    void ShowTip(const FAdaptiveGuideTip& Tip);
    
    // ?πÂ?Ê¢ù‰ª∂Ê™¢Êü•
    bool CheckIdleWorkersCondition() const;
    bool CheckUnspentResourcesCondition() const;
    bool CheckNoScoutingCondition() const;
    bool CheckUnderAttackCondition() const;
    bool CheckIdleProductionCondition() const;
    bool CheckLowAPMCondition() const;
    bool CheckArmySizeDisparityCondition() const;
    bool CheckTechAvailableCondition() const;
    bool CheckPopulationCapCondition() const;
};
