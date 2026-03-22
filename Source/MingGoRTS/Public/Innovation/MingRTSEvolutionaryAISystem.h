#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEvolutionaryAISystem.generated.h"

UENUM(BlueprintType)
enum class EEvolutionPhase : uint8
{
    Adaptation,     // ?©Ê??éÊÆµ
    Learning,       // Â≠∏Á??éÊÆµ
    Evolution,      // ?≤Â??éÊÆµ
    Specialization  // ?πÂ??éÊÆµ
};

UENUM(BlueprintType)
enum class EAIFitnessLevel : uint8
{
    VeryLow,        // Âæà‰?
    Low,            // ‰Ω?    Medium,         // ‰∏≠Á?
    High,           // È´?    VeryHigh,       // ÂæàÈ?
    Exceptional     // ?ìË?
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
        TraitName = TEXT("");
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
        GenomeID = TEXT("");
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
        RecordID = TEXT("");
        Phase = EEvolutionPhase::Adaptation;
        EvolutionTrigger = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAIEvolution, const FString&, AIID, const FAIGenome&, OldGenome, const FAIGenome&, NewGenome);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFitnessUpdate, const FString&, AIID, float, NewFitnessScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEvolutionPhase, const FString&, AIID, TEnumAsByte<EEvolutionPhase>, NewPhase);

/**
 * ?™ÈÄ≤Â?AIÁ≥ªÁµ± - ?∫Êñº?∫ÂÇ≥ÁÆóÊ??åÊ∑±Â∫¶Âº∑?ñÂ≠∏ÁøíÁ?AI?≤Â?Á≥ªÁµ±
 * ?ê‰?AI?∫ËÉΩÈ´îÁ??ÅÁ??≤Â??ÅÈÅ©?âÂ??πÂ??ΩÂ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSEvolutionaryAISystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEvolutionaryAISystem();

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void InitializeEvolutionarySystem();

    // ?µÂª∫AI?∫Â?Áµ?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome CreateAIGenome(const FString& AIID, const TArray<FEvolutionaryTrait>& InitialTraits);

    // AI?≤Â??ïÁ?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void ProcessEvolution(const FString& AIID, const TArray<FString>& EnvironmentalFactors);

    // ?∫Â?ÁµÑ‰∫§?âÁ?ÊÆ?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome CrossoverGenomes(const FAIGenome& Parent1, const FAIGenome& Parent2);

    // ?∫Â?ÁµÑË???    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome MutateGenome(const FAIGenome& Genome, float MutationIntensity);

    // ?©Ê?Â∫¶Ë?‰º?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    float EvaluateFitness(const FString& AIID, const FAIGenome& Genome);

    // ?∏Ê??Ä‰Ω≥Âü∫?†Á?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    TArray<FAIGenome> SelectBestGenomes(const TArray<FAIGenome>& Population, int32 SelectionCount);

    // ?≤Â??éÊÆµÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void SetEvolutionPhase(const FString& AIID, EEvolutionPhase NewPhase);

    // ?≤Â?AI?∫Â?Áµ?    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    FAIGenome GetAIGenome(const FString& AIID) const;

    // ?≤Â??≤Â?Ê≠∑Âè≤
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    TArray<FEvolutionRecord> GetEvolutionHistory(const FString& AIID) const;

    // Ë®≠ÁΩÆ?≤Â??ÉÊï∏
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void SetEvolutionParameters(float CrossoverRate, float MutationRate, float SelectionPressure);

    // ?πÈ??≤Â??ïÁ?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void ProcessBatchEvolution(const TArray<FString>& AIIDs);

    // ?≤Â?Áµ±Ë??ÜÊ?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    TMap<FString, float> GetEvolutionStatistics() const;

    // ?çÁΩÆAI?≤Â?
    UFUNCTION(BlueprintCallable, Category = "Evolutionary AI")
    void ResetAIEvolution(const FString& AIID);

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnAIEvolution OnAIEvolution;

    UPROPERTY(BlueprintAssignable)
    FOnFitnessUpdate OnFitnessUpdate;

    UPROPERTY(BlueprintAssignable)
    FOnEvolutionPhase OnEvolutionPhase;

protected:
    // ?≤Â??ÉÊï∏
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

    // AI?∫Â?ÁµÑÂ???    UPROPERTY(BlueprintReadOnly, Category = "Evolution Data")
    TMap<FString, FAIGenome> AIGenomes;

    // ?≤Â?Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY(BlueprintReadOnly, Category = "Evolution Data")
    TMap<FString, TArray<FEvolutionRecord>> EvolutionHistories;

    // ?∞Â??†Á?Ê¨äÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evolution Parameters")
    TMap<FString, float> EnvironmentalWeights;

    // ?≤Â??éÊÆµÁÆ°Á?
    UPROPERTY(BlueprintReadOnly, Category = "Evolution Data")
    TMap<FString, EEvolutionPhase> AIEvolutionPhases;

private:
    // ?ßÈÉ®?≤Â??èËºØ
    void PerformCrossover(FAIGenome& Offspring, const FAIGenome& Parent1, const FAIGenome& Parent2);
    void PerformMutation(FAIGenome& Genome);
    float CalculateTraitFitness(const FEvolutionaryTrait& Trait, const TArray<FString>& EnvironmentalFactors);
    void UpdateEvolutionPhase(const FString& AIID);
    void RecordEvolution(const FString& AIID, const FAIGenome& OldGenome, const FAIGenome& NewGenome, const FString& Trigger);
    
    // ?©Ê?Â∫¶ÂáΩ??    float CalculateAdaptationFitness(const FAIGenome& Genome);
    float CalculateLearningFitness(const FAIGenome& Genome);
    float CalculateEvolutionFitness(const FAIGenome& Genome);
    float CalculateSpecializationFitness(const FAIGenome& Genome);
    
    // ËºîÂä©?ΩÊï∏
    FEvolutionaryTrait BlendTraits(const FEvolutionaryTrait& Trait1, const FEvolutionaryTrait& Trait2);
    bool ShouldEvolve(const FString& AIID);
    TArray<FString> GetActiveEnvironmentalFactors() const;
};
