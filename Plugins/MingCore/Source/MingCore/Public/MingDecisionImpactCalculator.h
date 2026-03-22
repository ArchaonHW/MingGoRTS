#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionImpactCalculator.generated.h"

// ?v?T??X
UENUM(BlueprintType)
enum class EMingImpactType: uint8 {
    Immediate,      // ??Y?v?T
    ShortTerm,      // X?v?T (1-3X
    MediumTerm,     // ??X?v?T (3-10X
    LongTerm,       // X?v?T (10?~?HX
    Permanent       // ??X?v?T
);

// ?v?T?dX
UENUM(BlueprintType)
enum class EMingImpactScope : uint8
{
    Personal,       // X?H?h??
    Regional,       // X?h??
    National,       // X?a?h??
    International,  // X?h??
    Historical       // ???v?h??
};

// ?v?T?{??X
UENUM(BlueprintType)
enum class EMingImpactMagnitude: uint8 {
    Minimal,        // ??X
    Minor,          // ???L
    Moderate,       // ??X
    Significant,    // ??X
    Major,          // X?j
    Catastrophic,   // ?a摧毀
};

// ???v??X??
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalPath
{
    GENERATED_BODY()

    // ??XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PathID;

    // ??X??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathName;

    // ??X?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathDescription;

    // ??X摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PathTags;

    // ??X?vX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PathWeight;

    
    bool bIsMainPath;

    FMingHistoricalPath()
    {
        PathID = -1;
        PathName = TEXT(""};
        PathDescription = TEXT("");
        PathTags.Empty();
        PathWeight = 1.0f;
        bIsMainPath = false;
    }
};

// ?MX?v?T???H??
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionImpactDetail
{
    GENERATED_BODY()

    // ?v?T??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactType ImpactType;

    // ?v?T?dX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactScope ImpactScope;

    // ?v?T?{??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactMagnitude ImpactMagnitude;

    // ?v?TX?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImpactDescription;

    
    float ImpactValue;

    // ?v?TX (X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DurationYears;

    // X (?MX??~X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartYear;

    
    TArray<FString> AffectedAttributes;

    // ???vX?e
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalKnowledge;

    FMingDecisionImpactDetail()
    {
        ImpactType = EMingImpactType::Immediate;
        ImpactScope = EMingImpactScope::Personal;
        ImpactMagnitude = EMingImpactMagnitude::Moderate;
        ImpactDescription = TEXT(""};
        ImpactValue = 0.0f;
        DurationYears = 0;
        StartYear = 0;
        AffectedAttributes.Empty();
        HistoricalKnowledge = TEXT(""};
    }
};

// 摧毀MX?v?TX
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionImpactReport
{
    GENERATED_BODY()

    // ?MXID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // X??XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // ?pX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CalculationTime;

    // ?`X?v?T??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallImpactScore;

    // ???v??X?v?T
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingHistoricalPath> AffectedPaths;

    // ???v?TX??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDecisionImpactDetail> ImpactDetails;

    // 摧毀X??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ReputationChanges;

    // X?dX??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> TerritoryChanges;

    // ??X摧毀?o摧毀X?s
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UpdatedEventConditions;

    // X?|X?eX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EducationalSummary;

    FMingDecisionImpactReport()
    {
        DecisionID = -1;
        ChosenOptionID = -1;
        CalculationTime = FDateTime::Now();
        OverallImpactScore = 0.0f;
        AffectedPaths.Empty();
        ImpactDetails.Empty();
        ReputationChanges.Empty();
        TerritoryChanges.Empty();
        UpdatedEventConditions.Empty();
        EducationalSummary = TEXT(""};
    }
};

// X?dX
USTRUCT(BlueprintType)
struct FINGCORE_API FMingTerritoryData
{
    GENERATED_BODY()

    // X??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FactionName;

    // X??X??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlledProvinces;

    // X??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlledCities;

    // ?xX??X??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryStrength;

    // ?gX??X??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EconomicStrength;

    
    float PopularSupport;

    FMingTerritoryData()
    {
        FactionName = TEXT(""};
        ControlledProvinces = 0;
        ControlledCities = 0;
        MilitaryStrength = 0.0f;
        EconomicStrength = 0.0f;
        PopularSupport = 0.0f;
    }
);

// ?v?T?pX???eX



/**
 * ?MX?v?T?pX * ?t?d?pX??X?M??X??v?T
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingDecisionImpactCalculator : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionImpactCalculator();

    // ?pX?MX?v?T
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FMingDecisionImpactReport CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID);

    // ?pX???v??X?v?T
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FMingHistoricalPath> CalculateHistoricalPathImpact(int32 DecisionID, int32 ChosenOptionID);

    // ?pX摧毀X?v?T
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<FString, float> CalculateReputationImpact(int32 DecisionID, int32 ChosenOptionID);

    // ?pX?dX?v?T
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<FString, int32> CalculateTerritoryImpact(int32 DecisionID, int32 ChosenOptionID);

    // X?|X?e
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FString GenerateEducationalContent(int32 DecisionID, int32 ChosenOptionID);

    // X?s摧毀?o摧毀
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FString> UpdateEventTriggerConditions(int32 DecisionID, int32 ChosenOptionID);

    // X?v?T摧毀
    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    EMingImpactMagnitude GetImpactMagnitude(float ImpactValue);

    // X?v?TX?z
    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    FString GetImpactDescription(EMingImpactMagnitude Magnitude);

    // 摧毀X?v?T
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FMingDecisionImpactDetail> SimulateLongTermImpact(int32 DecisionID, int32 ChosenOptionID, int32 SimulationYears);

    
    TMap<int32, FMingDecisionImpactReport> CompareDecisionOptions(int32 DecisionID);

    
    TArray<FMingHistoricalPath> GetCurrentHistoricalPaths();

    // ?pX?MX?vX
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    float CalculateDecisionWeight(int32 DecisionID, int32 ChosenOptionID);

    // X?v?TX??
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FMingDecisionImpactReport GenerateImpactPrediction(int32 DecisionID, int32 OptionID);

    // ???eX
    UPROPERTY(BlueprintAssignable)
    FOnImpactCalculated OnImpactCalculated;

    UPROPERTY(BlueprintAssignable)
    FOnHistoricalPathChanged OnHistoricalPathChanged;

protected:
    
    TArray<FMingHistoricalPath> HistoricalPaths;

    // X
    UPROPERTY()
    TMap<FString, FMingTerritoryData> FactionData;

    // ?v?T?pX?WX
    UPROPERTY()
    TMap<EMingDecisionType, float> ImpactWeights;

    
    TMap<int32, FString> HistoricalKnowledgeBase;

private:
    // X??X??X
    void InitializeHistoricalPaths();

    // X??X??X
    void InitializeFactionData();

    // X?vX
    void InitializeImpactWeights();

    // X??X摧毀w
    void InitializeHistoricalKnowledge();

    // ?pX?v?T??X
    float CalculateImpactScore(const TArray<FMingDecisionImpactDetail>& Impacts);

    // X?s???v??X?vX
    void UpdateHistoricalPathWeights(const FMingDecisionImpactReport& Report) {};

    // X?v?T??X
    TArray<FMingDecisionImpactDetail> GenerateImpactDetails(int32 DecisionID, int32 ChosenOptionID);

    // ??X?v?TX
    bool ValidateImpactData(const FMingDecisionImpactReport& Report) const;
};

