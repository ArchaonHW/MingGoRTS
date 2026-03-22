#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEvolutionaryAISystem.generated.h"

UENUM(BlueprintType)
enum class EEvolutionPhase : uint8
{
    Adaptation, UMETA(DisplayName = "Adaptation Phase"),
    Learning, UMETA(DisplayName = "Learning Phase"),
    Evolution, UMETA(DisplayName = "Evolution Phase"),
    Specialization, UMETA(DisplayName = "Specialization Phase")
};

UENUM(BlueprintType)
enum class EAIFitnessLevel : uint8
{
    VeryLow, UMETA(DisplayName = "Very Low"),
    Low, UMETA(DisplayName = "Low"),
    Medium, UMETA(DisplayName = "Medium"),
    High, UMETA(DisplayName = "High"),
    VeryHigh, UMETA(DisplayName = "Very High"),
    Exceptional, UMETA(DisplayName = "Exceptional")
};

USTRUCT(BlueprintType)
struct FEvolutionaryTrait
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolutionary Trait")
    FString TraitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolutionary Trait")
    float TraitValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolutionary Trait")
    EAIFitnessLevel FitnessLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolutionary Trait")
    EEvolutionPhase EvolutionPhase;

    FEvolutionaryTrait()
        : TraitValue(0.0f)
        , FitnessLevel(EAIFitnessLevel::Medium)
        , EvolutionPhase(EEvolutionPhase::Adaptation)
    {}
};

/**
 * MingGoRTS Evolutionary AI System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSEvolutionaryAISystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEvolutionaryAISystem();

    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void InitializeEvolutionaryAI();

    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void StartEvolutionProcess();

    UFUNCTION(BlueprintPure, Category = "Evolutionary AI")
    EEvolutionPhase GetCurrentEvolutionPhase() const;

    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void AddEvolutionaryTrait(const FEvolutionaryTrait& Trait);

    UFUNCTION(BlueprintPure, Category = "Evolutionary AI")
    TArray<FEvolutionaryTrait> GetEvolutionaryTraits() const;

    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    bool EvolveToNextPhase();

    UFUNCTION(BlueprintPure, Category = "Evolutionary AI")
    EAIFitnessLevel CalculateFitnessLevel() const;

protected:
    UPROPERTY()
    EEvolutionPhase CurrentPhase;

    UPROPERTY()
    TArray<FEvolutionaryTrait> EvolutionaryTraits;

    UPROPERTY()
    float EvolutionProgress;

    void ProcessEvolution();
    void OptimizeTraits();
    void SelectBestTraits();
    void MutateTraits();
};
