#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingBaseCombatAI.h"
#include "MingNationalRevolutionaryArmyAI.generated.h"

/**
 * ?‹æ??©å‘½è»?AI
 * é«”ç¾?—ä??‚æ??‹æ??©å‘½è»ç??°è??¹è‰²
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingNationalRevolutionaryArmyAI : public UMingBaseCombatAI
{
    GENERATED_BODY()

public:
    // å»ºæ?å­?    UMingNationalRevolutionaryArmyAI(};

    // ?å???    virtual void Initialize(class AController* Controller) override;

    // è£½ä??°è?æ±ºç?
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context) override;

    // è©•ä¼°?°å ´å±€??    virtual float AssessBattlefieldSituation(const FMingAIContext& Context) override;

protected:
    // ?—ä??°è??¹é?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteNorthernExpeditionTactics(const FMingAIContext& Context};

    // ?¿æ²»å·¥ä?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyPoliticalWorkEffects(FMingAIState& AIState};

    // ?‡è¯é¡§å?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplySovietAdvisorInfluence(FMingTacticalDecision& Decision};

    // ?©å‘½?±æ?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyRevolutionaryZeal(FMingAIState& AIState};

    // çµ±ä??°ç?ç­–ç•¥
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteUnitedFrontStrategy(const FMingAIContext& Context};

    // ç¾¤çœ¾?•å“¡
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void MobilizeMasses(const FMingAIContext& Context};

    // ?¿æ²»å®?‚³?ˆæ?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float CalculatePoliticalPropagandaEffect(const FVector& Location};

    // ?‡è¯?´åŠ©ç¨‹åº¦
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetSovietAidLevel() const { return SovietAidLevel; }

    // è¨­ç½®?‡è¯?´åŠ©ç­‰ç?
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetSovietAidLevel(float Level) { SovietAidLevel = FMath::Clamp(Level, 0.0f, 1.0f}; }

    // ?²å??¿æ²»å§”å“¡å½±éŸ¿??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetPoliticalCommissarInfluence() const { return PoliticalCommissarInfluence; }

    // è¨­ç½®?¿æ²»å§”å“¡å½±éŸ¿??    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetPoliticalCommissarInfluence(float Influence) { PoliticalCommissarInfluence = FMath::Clamp(Influence, 0.0f, 1.0f}; }

protected:
    // ?¿æ²»å§”å“¡å½±éŸ¿??(0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Political")
    float PoliticalCommissarInfluence;

    // ?‡è¯é¡§å?ç­‰ç? (0-5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Advisor")
    int32 SovietAdvisorLevel;

    // ?‡è¯?´åŠ©ç¨‹åº¦ (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Aid")
    float SovietAidLevel;

    // ?©å‘½?±æ? (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Morale")
    float RevolutionaryZeal;

    // çµ±ä??°ç?å¼·åº¦ (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|United")
    float UnitedFrontStrength;

    // ç¾¤çœ¾?¯æ?åº?(0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Support")
    float MassSupport;

    // ?¿æ²»å®?‚³?ˆæ? (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Propaganda")
    float PropagandaEffectiveness;

private:
    // ?—ä??æ®µ
    int32 NorthernExpeditionPhase;

    // ?¶å??¿æ²»???
    FString CurrentPoliticalSlogan;

    // ?‡è¯è»ä?é¡§å?å½±éŸ¿
    float SovietMilitaryInfluence;

    // ä¸‰æ?ä¸»ç¾©å½±éŸ¿??    float ThreePrinciplesInfluence;

    // è¨ˆç??¿æ²»å£«æ°£? æ?
    float CalculatePoliticalMoraleBonus(};

    // è¨ˆç?çµ±ä??°ç??°è?? æ?
    float CalculateUnitedFrontBonus(const FMingAIContext& Context};

    // æ±ºå??¯å¦?¼å??¿æ²»?»å‹¢
    bool ShouldLaunchPoliticalOffensive(const FMingAIContext& Context};
};

