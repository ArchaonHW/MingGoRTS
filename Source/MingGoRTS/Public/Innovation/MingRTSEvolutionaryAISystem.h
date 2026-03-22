#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEvolutionaryAISystem.generated.h"

UENUM(BlueprintType)
enum class EEvolutionPhase : uint8
{
    Adaptation,     // ?��X�段
    Learning,       // 學�X�段
    Evolution,      // ?��X�段
    Specialization  // ?��X�段
};

UENUM(BlueprintType)
enum class EAIFitnessLevel : uint8
{
    VeryLow,        // 很�?
    Low,            // �?    Medium,         // 中�?
    High,           // �?    VeryHigh,       // 很�?
    Exceptional     // ?��?
};

USTRUCT(BlueprintType)
struct FEvolutionaryTrait
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TraitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TraitValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MutationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InheritanceStrength;

    FEvolutionaryTrait()
    {
        TraitName = TEXT(""};
        TraitValue = 0.0f;
        MutationRate = 0.1f;
        InheritanceStrength = 0.8f;
    }
};

USTRUCT(BlueprintType)
struct FAIGenome
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GenomeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FEvolutionaryTrait> Traits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAIFitnessLevel FitnessLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FitnessScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Generation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ParentGenomes;

    FAIGenome()
    {
        GenomeID = TEXT(""};
        FitnessLevel = EAIFitnessLevel::Medium;
        FitnessScore = 0.5f;
        Generation = 1;
    }
};

USTRUCT(BlueprintType)
struct FEvolutionRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecordID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEvolutionPhase Phase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAIGenome PreviousGenome;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAIGenome CurrentGenome;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EvolutionTrigger;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EnvironmentalFactors;

    FEvolutionRecord()
    {
        RecordID = TEXT(""};
        Phase = EEvolutionPhase::Adaptation;
        EvolutionTrigger = TEXT(""};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAIEvolution, const FString&, AIID, const FAIGenome&, OldGenome, const FAIGenome&, NewGenome};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFitnessUpdate, const FString&, AIID, float, NewFitnessScore};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvolutionPhase, const FString&, AIID, TEnumAsByte<EEvolutionPhase>, NewPhase};

/**
 * ?�進�?AI系統 - ?�於?�傳算�X�深度強?�學習�?AI?��?系統
 * ?��?AI?�能體�X��X��X�適?��X��X��?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSEvolutionaryAISystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEvolutionaryAISystem(};

    // 系統?��X    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void InitializeEvolutionarySystem(};

    // ?�建AI?��?�?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome CreateAIGenome(const FString& AIID, const TArray<FEvolutionaryTrait>& InitialTraits};

    // AI?��X��?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void ProcessEvolution(const FString& AIID, const TArray<FString>& EnvironmentalFactors};

    // ?��?組交?��?�?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome CrossoverGenomes(const FAIGenome& Parent1, const FAIGenome& Parent2};

    // ?��?組�X    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome MutateGenome(const FAIGenome& Genome, float MutationIntensity};

    // ?��?度�?�?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    float EvaluateFitness(const FString& AIID, const FAIGenome& Genome};

    // ?��X�佳基?��?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    TArray<FAIGenome> SelectBestGenomes(const TArray<FAIGenome>& Population, int32 SelectionCount};

    // ?��X�段管�?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void SetEvolutionPhase(const FString& AIID, EEvolutionPhase NewPhase};

    // ?��?AI?��?�?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome GetAIGenome(const FString& AIID) const;

    // ?��X��?歷史
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    TArray<FEvolutionRecord> GetEvolutionHistory(const FString& AIID) const;

    // 設置?��X�數
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void SetEvolutionParameters(float CrossoverRate, float MutationRate, float SelectionPressure};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void ProcessBatchEvolution(const TArray<FString>& AIIDs};

    // ?��?統�X��?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    TMap<FString, float> GetEvolutionStatistics() const;

    // ?�置AI?��?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void ResetAIEvolution(const FString& AIID};

    // 事件委�?
    UPROPERTY(BlueprintAssignable)
    FOnAIEvolution OnAIEvolution;

    UPROPERTY(BlueprintAssignable)
    FOnFitnessUpdate OnFitnessUpdate;

    UPROPERTY(BlueprintAssignable)
    FOnEvolutionPhase OnEvolutionPhase;

protected:
    // ?��X�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolution Parameters")
    float CrossoverRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolution Parameters")
    float MutationRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolution Parameters")
    float SelectionPressure;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolution Parameters")
    int32 MaxGeneration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolution Parameters")
    float FitnessThreshold;

    // AI?��?組�X    UPROPERTY(BlueprintReadOnly, Category = "Evolution Data")
    TMap<FString, FAIGenome> AIGenomes;

    // ?��?歷史記�?
    UPROPERTY(BlueprintReadOnly, Category = "Evolution Data")
    TMap<FString, TArray<FEvolutionRecord>> EvolutionHistories;

    // ?��X��?權�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolution Parameters")
    TMap<FString, float> EnvironmentalWeights;

    // ?��X�段管�?
    UPROPERTY(BlueprintReadOnly, Category = "Evolution Data")
    TMap<FString, EEvolutionPhase> AIEvolutionPhases;

private:
    // ?�部?��X�輯
    void PerformCrossover(FAIGenome& Offspring, const FAIGenome& Parent1, const FAIGenome& Parent2};
    void PerformMutation(FAIGenome& Genome};
    float CalculateTraitFitness(const FEvolutionaryTrait& Trait, const TArray<FString>& EnvironmentalFactors};
    void UpdateEvolutionPhase(const FString& AIID};
    void RecordEvolution(const FString& AIID, const FAIGenome& OldGenome, const FAIGenome& NewGenome, const FString& Trigger};
    
    // ?��?度函X    float CalculateAdaptationFitness(const FAIGenome& Genome};
    float CalculateLearningFitness(const FAIGenome& Genome};
    float CalculateEvolutionFitness(const FAIGenome& Genome};
    float CalculateSpecializationFitness(const FAIGenome& Genome};
    
    // 輔助?�數
    FEvolutionaryTrait BlendTraits(const FEvolutionaryTrait& Trait1, const FEvolutionaryTrait& Trait2};
    bool ShouldEvolve(const FString& AIID};
    TArray<FString> GetActiveEnvironmentalFactors() const;
};

