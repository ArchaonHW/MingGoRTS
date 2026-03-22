#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingBaseCombatAI.h"
#include "MingNationalRevolutionaryArmyAI.generated.h"

/**
 * ???X?�R??AI
 * ��{故事選項X?�R�x?X??X?��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingNationalRevolutionaryArmyAI : public UMingBaseCombatAI
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingNationalRevolutionaryArmyAI();

    // ???X
    virtual void Initialize(class AController* Controller) override;

    // �s?X???�M??
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context) override;

    // ����??����X
    virtual float AssessBattlefieldSituation(const FMingAIContext& Context) override;

protected:
    // 故事選項?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteNorthernExpeditionTactics(const FMingAIContext& Context);

    // ??�v�u??�v�T
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyPoliticalWorkEffects(FMingAIState& AIState);

    // ??�p�U??�v�T
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplySovietAdvisorInfluence(FMingTacticalDecision& Decision);

    // ??�R摧毀�v�T
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyRevolutionaryZeal(FMingAIState& AIState);

    // ��?X???����
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteUnitedFrontStrategy(const FMingAIContext& Context);

    // �s��??��
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void MobilizeMasses(const FMingAIContext& Context);

    // ??�v摧毀摧毀
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float CalculatePoliticalPropagandaEffect(const FVector& Location);

    // ??�p??�U�{��
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetSovietAidLevel() const { return SovietAidLevel; }

    // �]�m??�p??�U��??
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetSovietAidLevel(float Level) { SovietAidLevel = FMath::Clamp(Level, 0.0f, 1.0f}; }

    // ???X?�v�e���v�TX
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetPoliticalCommissarInfluence() const { return PoliticalCommissarInfluence; }

    // �]�m??�v�e���v�TX
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetPoliticalCommissarInfluence(float Influence) { PoliticalCommissarInfluence = FMath::Clamp(Influence, 0.0f, 1.0f}; }

protected:
    // ??�v�e���v�TX(0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Political")
    float PoliticalCommissarInfluence;

    // ??�p�U??��?? (0-5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Advisor")
    int32 SovietAdvisorLevel;

    // ??�p??�U�{�� (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Aid")
    float SovietAidLevel;

    // ??�R摧毀 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Morale")
    float RevolutionaryZeal;

    // ��?X???�j�� (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|United")
    float UnitedFrontStrength;

    // �s��摧毀??(0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Support")
    float MassSupport;

    // ??�v摧毀摧毀 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Propaganda")
    float PropagandaEffectiveness;

private:
    // ???X?�q
    int32 NorthernExpeditionPhase;

    // ???X?�vX
    FString CurrentPoliticalSlogan;

    // ??�p�x??�U??�v�T
    float SovietMilitaryInfluence;

    // �T??�D�q�v�TX
    float ThreePrinciplesInfluence;

    // �p?X?�v�h��摧毀
    float CalculatePoliticalMoraleBonus();

    // �p??��?X??X??X???
    float CalculateUnitedFrontBonus(const FMingAIContext& Context);

    // �M?X?�_???X?�v??��
    bool ShouldLaunchPoliticalOffensive(const FMingAIContext& Context);
);

