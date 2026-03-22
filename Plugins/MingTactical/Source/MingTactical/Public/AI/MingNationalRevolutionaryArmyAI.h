#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI/MingBaseCombatAI.h"
#include "MingNationalRevolutionaryArmyAI.generated.h"

/**
 * 國民革命軍 AI
 * 體現北伐時期國民革命軍的戰術特色
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API UMingNationalRevolutionaryArmyAI : public UMingBaseCombatAI
{
    GENERATED_BODY()

public:
    // 建構子
    UMingNationalRevolutionaryArmyAI();

    // 初始化
    virtual void Initialize(class AController* Controller) override;

    // 製作戰術決策
    virtual FMingTacticalDecision MakeTacticalDecision(const FMingAIContext& Context) override;

    // 評估戰場局勢
    virtual float AssessBattlefieldSituation(const FMingAIContext& Context) override;

protected:
    // 北伐戰術特點
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteNorthernExpeditionTactics(const FMingAIContext& Context);

    // 政治工作影響
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyPoliticalWorkEffects(FMingAIState& AIState);

    // 蘇聯顧問影響
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplySovietAdvisorInfluence(FMingTacticalDecision& Decision);

    // 革命熱情影響
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ApplyRevolutionaryZeal(FMingAIState& AIState);

    // 統一戰線策略
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void ExecuteUnitedFrontStrategy(const FMingAIContext& Context);

    // 群眾動員
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void MobilizeMasses(const FMingAIContext& Context);

    // 政治宣傳效果
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float CalculatePoliticalPropagandaEffect(const FVector& Location);

    // 蘇聯援助程度
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetSovietAidLevel() const { return SovietAidLevel; }

    // 設置蘇聯援助等級
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetSovietAidLevel(float Level) { SovietAidLevel = FMath::Clamp(Level, 0.0f, 1.0f); }

    // 獲取政治委員影響力
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    float GetPoliticalCommissarInfluence() const { return PoliticalCommissarInfluence; }

    // 設置政治委員影響力
    UFUNCTION(BlueprintCallable, Category = "Ming|AI")
    void SetPoliticalCommissarInfluence(float Influence) { PoliticalCommissarInfluence = FMath::Clamp(Influence, 0.0f, 1.0f); }

protected:
    // 政治委員影響力 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Political")
    float PoliticalCommissarInfluence;

    // 蘇聯顧問等級 (0-5)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Advisor")
    int32 SovietAdvisorLevel;

    // 蘇聯援助程度 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Aid")
    float SovietAidLevel;

    // 革命熱情 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Morale")
    float RevolutionaryZeal;

    // 統一戰線強度 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|United")
    float UnitedFrontStrength;

    // 群眾支持度 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Support")
    float MassSupport;

    // 政治宣傳效果 (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NRA|Propaganda")
    float PropagandaEffectiveness;

private:
    // 北伐階段
    int32 NorthernExpeditionPhase;

    // 當前政治口號
    FString CurrentPoliticalSlogan;

    // 蘇聯軍事顧問影響
    float SovietMilitaryInfluence;

    // 三民主義影響力
    float ThreePrinciplesInfluence;

    // 計算政治士氣加成
    float CalculatePoliticalMoraleBonus();

    // 計算統一戰線戰術加成
    float CalculateUnitedFrontBonus(const FMingAIContext& Context);

    // 決定是否發動政治攻勢
    bool ShouldLaunchPoliticalOffensive(const FMingAIContext& Context);
};
