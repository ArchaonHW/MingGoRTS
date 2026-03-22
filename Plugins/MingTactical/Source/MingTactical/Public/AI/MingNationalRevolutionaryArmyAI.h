#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingBaseCombatAI.h"
#include "MingNationalRevolutionaryArmyAI.generated.h"

/**
 * ?��X�命�?AI
 * 體現?��X��X��X�命軍�X��X�色
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingNationalRevolutionaryArmyAI : public UMingBaseCombatAI
{
    GENERATED_BODY()

public:
    // 建�?�?    UMingNationalRevolutionaryArmyAI(};

    // ?��X    virtual void Initialize(class AController* Controller) override;

    // 製�X��?決�?
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context) override;

    // 評估?�場局X    virtual float AssessBattlefieldSituation(const FMingAIContext& Context) override;

protected:
    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteNorthernExpeditionTactics(const FMingAIContext& Context};

    // ?�治工�?影響
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyPoliticalWorkEffects(FMingAIState& AIState};

    // ?�聯顧�?影響
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplySovietAdvisorInfluence(FMingTacticalDecision& Decision};

    // ?�命?��?影響
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyRevolutionaryZeal(FMingAIState& AIState};

    // 統�X��?策略
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteUnitedFrontStrategy(const FMingAIContext& Context};

    // 群眾?�員
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void MobilizeMasses(const FMingAIContext& Context};

    // ?�治�?��?��?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float CalculatePoliticalPropagandaEffect(const FVector& Location};

    // ?�聯?�助程度
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetSovietAidLevel() const { return SovietAidLevel; }

    // 設置?�聯?�助等�?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetSovietAidLevel(float Level) { SovietAidLevel = FMath::Clamp(Level, 0.0f, 1.0f}; }

    // ?��X�治委員影響X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetPoliticalCommissarInfluence() const { return PoliticalCommissarInfluence; }

    // 設置?�治委員影響X    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetPoliticalCommissarInfluence(float Influence) { PoliticalCommissarInfluence = FMath::Clamp(Influence, 0.0f, 1.0f}; }

protected:
    // ?�治委員影響X(0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Political")
    float PoliticalCommissarInfluence;

    // ?�聯顧�?等�? (0-5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Advisor")
    int32 SovietAdvisorLevel;

    // ?�聯?�助程度 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Aid")
    float SovietAidLevel;

    // ?�命?��? (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Morale")
    float RevolutionaryZeal;

    // 統�X��?強度 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|United")
    float UnitedFrontStrength;

    // 群眾?��?�?(0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Support")
    float MassSupport;

    // ?�治�?��?��? (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Propaganda")
    float PropagandaEffectiveness;

private:
    // ?��X�段
    int32 NorthernExpeditionPhase;

    // ?��X�治X
    FString CurrentPoliticalSlogan;

    // ?�聯軍�?顧�?影響
    float SovietMilitaryInfluence;

    // 三�?主義影響X    float ThreePrinciplesInfluence;

    // 計�X�治士氣?��?
    float CalculatePoliticalMoraleBonus(};

    // 計�?統�X��X��X��?
    float CalculateUnitedFrontBonus(const FMingAIContext& Context};

    // 決�X�否?��X�治?�勢
    bool ShouldLaunchPoliticalOffensive(const FMingAIContext& Context};
};

