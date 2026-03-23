#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSixStrategies.h"
#include "MingNegativeStrategies.generated.h"

// 破局策略結構
USTRUCT(BlueprintType)
struct FBreakSituationParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetSituationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetNationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Weaknesses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DisruptionIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SabotageTargets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCovertOperation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceInvestment;

    FBreakSituationParams()
    {
        DisruptionIntensity = 0.5f;
        bCovertOperation = false;
        ResourceInvestment = 200.0f;
    }
};

// 破局結果
USTRUCT(BlueprintType)
struct FBreakSituationResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DisruptionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetStabilityLoss;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> DestroyedStructures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceCost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationImpact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExecutionLog;

    FBreakSituationResult()
    {
        bSuccess = false;
        DisruptionEffect = 0.0f;
        TargetStabilityLoss = 0.0f;
        ResourceCost = 0.0f;
        ReputationImpact = -0.1f;
    }
};

// 破結構策略結構
USTRUCT(BlueprintType)
struct FBreakStructureParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetStructureId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString StructureType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> StructuralWeaknesses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DestructionIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPrecisionStrike;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> KeyPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RequiredForce;

    FBreakStructureParams()
    {
        DestructionIntensity = 0.6f;
        bPrecisionStrike = true;
        RequiredForce = 500.0f;
    }
};

// 破結構結果
USTRUCT(BlueprintType)
struct FBreakStructureResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StructureIntegrityLoss;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FunctionalDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleteDestruction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryEffort;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> CollateralDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExecutionLog;

    FBreakStructureResult()
    {
        bSuccess = false;
        StructureIntegrityLoss = 0.0f;
        FunctionalDamage = 0.0f;
        bCompleteDestruction = false;
        MilitaryEffort = 0.0f;
    }
};

// 不破人策略結構
USTRUCT(BlueprintType)
struct FPreservePeopleParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetNationId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ProtectedPeopleIds;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProtectionLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEvacuationPlan;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SafeZones;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResourceAllocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPoliticalAmnesty;

    FPreservePeopleParams()
    {
        ProtectionLevel = 0.7f;
        bEvacuationPlan = true;
        ResourceAllocation = 300.0f;
        bPoliticalAmnesty = false;
    }
};

// 不破人結果
USTRUCT(BlueprintType)
struct FPreservePeopleResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSuccess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ProtectedPeople;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PreservationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PopulationLoyaltyGain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralAuthorityGain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> RecruitedTalents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExecutionLog;

    FPreservePeopleResult()
    {
        bSuccess = false;
        ProtectedPeople = 0;
        PreservationRate = 0.0f;
        PopulationLoyaltyGain = 0.0f;
        MoralAuthorityGain = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSituationBroken, const FBreakSituationResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStructureBroken, const FBreakStructureResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeoplePreserved, const FPreservePeopleResult&, Result);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingNegativeStrategies : public UObject
{
    GENERATED_BODY()

public:
    UMingNegativeStrategies();

    // 破局策 - 打破敵對佈局
    UFUNCTION(BlueprintCallable, Category = "Negative Strategies|Break Situation")
    FBreakSituationResult ExecuteBreakSituation(const FBreakSituationParams& Params);

    // 破結構策 - 瓦解敵方結構
    UFUNCTION(BlueprintCallable, Category = "Negative Strategies|Break Structure")
    FBreakStructureResult ExecuteBreakStructure(const FBreakStructureParams& Params);

    // 不破人策 - 不破壞人才的征服
    UFUNCTION(BlueprintCallable, Category = "Negative Strategies|Preserve People")
    FPreservePeopleResult ExecutePreservePeople(const FPreservePeopleParams& Params);

    // 分析功能
    UFUNCTION(BlueprintPure, Category = "Negative Strategies|Analysis")
    TArray<FString> AnalyzeSituationWeaknesses(const FString& SituationId) const;

    UFUNCTION(BlueprintPure, Category = "Negative Strategies|Analysis")
    TArray<FString> IdentifyStructuralWeaknesses(const FString& StructureId) const;

    UFUNCTION(BlueprintPure, Category = "Negative Strategies|Analysis")
    float CalculatePreservationValue(const TArray<FString>& PeopleIds) const;

    // 計算功能
    UFUNCTION(BlueprintPure, Category = "Negative Strategies")
    float CalculateDisruptionPotential(const FBreakSituationParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Negative Strategies")
    float CalculateDestructionPotential(const FBreakStructureParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Negative Strategies")
    float CalculatePreservationEfficiency(const FPreservePeopleParams& Params) const;

    // 驗證功能
    UFUNCTION(BlueprintPure, Category = "Negative Strategies")
    bool CanBreakSituation(const FBreakSituationParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Negative Strategies")
    bool CanBreakStructure(const FBreakStructureParams& Params) const;

    UFUNCTION(BlueprintPure, Category = "Negative Strategies")
    bool CanPreservePeople(const FPreservePeopleParams& Params) const;

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Negative Strategies")
    FOnSituationBroken OnSituationBroken;

    UPROPERTY(BlueprintAssignable, Category = "Negative Strategies")
    FOnStructureBroken OnStructureBroken;

    UPROPERTY(BlueprintAssignable, Category = "Negative Strategies")
    FOnPeoplePreserved OnPeoplePreserved;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Negative Strategies")
    void OnBreakSituationStarted(const FBreakSituationParams& Params);

    UFUNCTION(BlueprintImplementableEvent, Category = "Negative Strategies")
    void OnBreakStructureStarted(const FBreakStructureParams& Params);

    UFUNCTION(BlueprintImplementableEvent, Category = "Negative Strategies")
    void OnPreservePeopleStarted(const FPreservePeopleParams& Params);

private:
    void InitializeDefaultValues();
    
    float CalculateWeaknessExploitation(const TArray<FString>& Weaknesses) const;
    float CalculatePrecisionBonus(bool bPrecisionStrike, const TArray<FString>& KeyPoints) const;
    float CalculateProtectionCoverage(const TArray<FString>& SafeZones) const;
    
    TArray<FString> IdentifyTargets(const FBreakSituationParams& Params) const;
    TArray<FString> AssessCollateralDamage(const FBreakStructureParams& Params) const;
    TArray<FString> RecruitTalents(const TArray<FString>& ProtectedPeople) const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Negative Strategies")
    TMap<FString, FBreakSituationResult> BreakSituationHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Negative Strategies")
    TMap<FString, FBreakStructureResult> BreakStructureHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Negative Strategies")
    TMap<FString, FPreservePeopleResult> PreservePeopleHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Negative Strategies")
    float BaseSuccessRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Negative Strategies")
    float CovertOperationBonus;
};
