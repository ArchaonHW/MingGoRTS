#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionImpactCalculator.generated.h"

// å½±éŸ¿é¡å??šè?
UENUM(BlueprintType)
enum class EMingImpactType : uint8
{
    Immediate,      // ç«‹å³å½±éŸ¿
    ShortTerm,      // ?­æ?å½±éŸ¿ (1-3å¹?
    MediumTerm,     // ä¸­æ?å½±éŸ¿ (3-10å¹?
    LongTerm,       // ?·æ?å½±éŸ¿ (10å¹´ä»¥ä¸?
    Permanent       // æ°¸ä?å½±éŸ¿
};

// å½±éŸ¿ç¯„å??šè?
UENUM(BlueprintType)
enum class EMingImpactScope : uint8
{
    Personal,       // ?‹äººå±¤é¢
    Regional,       // ?°å?å±¤é¢
    National,       // ?‹å®¶å±¤é¢
    International,  // ?‹é?å±¤é¢
    Historical       // æ­·å²å±¤é¢
};

// å½±éŸ¿ç¨‹åº¦?šè?
UENUM(BlueprintType)
enum class EMingImpactMagnitude : uint8
{
    Minimal,        // æ¥µå?
    Minor,          // è¼•å¾®
    Moderate,       // ä¸­ç?
    Significant,    // é¡¯è?
    Major,          // ?å¤§
    Catastrophic    // ?½é›£??};

// æ­·å²è·¯å??†æ”¯
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalPath
{
    GENERATED_BODY()

    // è·¯å?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PathID;

    // è·¯å??ç¨±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathName;

    // è·¯å??è¿°
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathDescription;

    // è·¯å?æ¨™ç±¤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PathTags;

    // è·¯å?æ¬Šé?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PathWeight;

    // ?¯å¦?ºä¸»è¦è·¯å¾?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsMainPath;

    FMingHistoricalPath()
    {
        PathID = -1;
        PathName = TEXT("");
        PathDescription = TEXT("");
        PathTags.Empty();
        PathWeight = 1.0f;
        bIsMainPath = false;
    }
};

// æ±ºç?å½±éŸ¿è©³ç´°ä¿¡æ¯
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionImpactDetail
{
    GENERATED_BODY()

    // å½±éŸ¿é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactType ImpactType;

    // å½±éŸ¿ç¯„å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactScope ImpactScope;

    // å½±éŸ¿ç¨‹åº¦
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactMagnitude ImpactMagnitude;

    // å½±éŸ¿?è¿°
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImpactDescription;

    // å½±éŸ¿?¸å€?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ImpactValue;

    // å½±éŸ¿?ç??‚é? (å¹?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DurationYears;

    // ?‹å??‚é? (æ±ºç?å¾Œå¹´??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartYear;

    // å½±éŸ¿?„å±¬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffectedAttributes;

    // æ­·å²?¥è??§å®¹
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalKnowledge;

    FMingDecisionImpactDetail()
    {
        ImpactType = EMingImpactType::Immediate;
        ImpactScope = EMingImpactScope::Personal;
        ImpactMagnitude = EMingImpactMagnitude::Moderate;
        ImpactDescription = TEXT("");
        ImpactValue = 0.0f;
        DurationYears = 0;
        StartYear = 0;
        AffectedAttributes.Empty();
        HistoricalKnowledge = TEXT("");
    }
};

// å®Œæ•´æ±ºç?å½±éŸ¿?±å?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionImpactReport
{
    GENERATED_BODY()

    // æ±ºç?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // ?¸æ??„é¸?…ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // è¨ˆç??‚é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CalculationTime;

    // ç¸½é?å½±éŸ¿è©•å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallImpactScore;

    // æ­·å²è·¯å?å½±éŸ¿
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingHistoricalPath> AffectedPaths;

    // è©³ç´°å½±éŸ¿?—è¡¨
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDecisionImpactDetail> ImpactDetails;

    // è§’è‰²?²æ?è®Šå?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ReputationChanges;

    // ?¢å?ç¯„å?è®Šå?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> TerritoryChanges;

    // å¾Œç?äº‹ä»¶è§¸ç™¼æ¢ä»¶?´æ–°
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UpdatedEventConditions;

    // ?™è‚²?§å®¹?˜è?
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
        EducationalSummary = TEXT("");
    }
};

// ?¢å?ç¯„å??¸æ?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingTerritoryData
{
    GENERATED_BODY()

    // ?¢å??ç¨±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FactionName;

    // ?§åˆ¶?ä»½?¸é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlledProvinces;

    // ?§åˆ¶?å??¸é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlledCities;

    // è»ä?å¯¦å??‡æ•¸
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryStrength;

    // ç¶“æ?å¯¦å??‡æ•¸
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EconomicStrength;

    // æ°‘çœ¾?¯æ?åº?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PopularSupport;

    FMingTerritoryData()
    {
        FactionName = TEXT("");
        ControlledProvinces = 0;
        ControlledCities = 0;
        MilitaryStrength = 0.0f;
        EconomicStrength = 0.0f;
        PopularSupport = 0.0f;
    }
};

// å½±éŸ¿è¨ˆç?äº‹ä»¶å§”è?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImpactCalculated, const FMingDecisionImpactReport&, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHistoricalPathChanged, const FString&, PathName, float, NewWeight);

/**
 * æ±ºç?å½±éŸ¿è¨ˆç??? * è² è²¬è¨ˆç??Œå??æ­·?²æ±ºç­–ç??„ç¨®å½±éŸ¿
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingDecisionImpactCalculator : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionImpactCalculator();

    // è¨ˆç?æ±ºç?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FMingDecisionImpactReport CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID);

    // è¨ˆç?æ­·å²è·¯å?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FMingHistoricalPath> CalculateHistoricalPathImpact(int32 DecisionID, int32 ChosenOptionID);

    // è¨ˆç?è§’è‰²?²æ?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<FString, float> CalculateReputationImpact(int32 DecisionID, int32 ChosenOptionID);

    // è¨ˆç??¢å?ç¯„å?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<FString, int32> CalculateTerritoryImpact(int32 DecisionID, int32 ChosenOptionID);

    // ?Ÿæ??™è‚²?§å®¹
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FString GenerateEducationalContent(int32 DecisionID, int32 ChosenOptionID);

    // ?´æ–°äº‹ä»¶è§¸ç™¼æ¢ä»¶
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FString> UpdateEventTriggerConditions(int32 DecisionID, int32 ChosenOptionID);

    // ?²å?å½±éŸ¿è©•ä¼°
    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    EMingImpactMagnitude GetImpactMagnitude(float ImpactValue);

    // ?²å?å½±éŸ¿?è¿°
    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    FString GetImpactDescription(EMingImpactMagnitude Magnitude);

    // æ¨¡æ“¬?·æ?å½±éŸ¿
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FMingDecisionImpactDetail> SimulateLongTermImpact(int32 DecisionID, int32 ChosenOptionID, int32 SimulationYears);

    // æ¯”è?ä¸å??¸æ??„å½±??    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<int32, FMingDecisionImpactReport> CompareDecisionOptions(int32 DecisionID);

    // ?²å??¶å?æ­·å²è·¯å??€??    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    TArray<FMingHistoricalPath> GetCurrentHistoricalPaths();

    // è¨ˆç?æ±ºç?æ¬Šé?
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    float CalculateDecisionWeight(int32 DecisionID, int32 ChosenOptionID);

    // ?Ÿæ?å½±éŸ¿?æ¸¬
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FMingDecisionImpactReport GenerateImpactPrediction(int32 DecisionID, int32 OptionID);

    // äº‹ä»¶å§”è?
    UPROPERTY(BlueprintAssignable)
    FOnImpactCalculated OnImpactCalculated;

    UPROPERTY(BlueprintAssignable)
    FOnHistoricalPathChanged OnHistoricalPathChanged;

protected:
    // æ­·å²è·¯å??¸æ?åº?    UPROPERTY()
    TArray<FMingHistoricalPath> HistoricalPaths;

    // ?¢å??¸æ?
    UPROPERTY()
    TMap<FString, FMingTerritoryData> FactionData;

    // å½±éŸ¿è¨ˆç?è¦å?
    UPROPERTY()
    TMap<EMingDecisionType, float> ImpactWeights;

    // æ­·å²?¥è?åº?    UPROPERTY()
    TMap<int32, FString> HistoricalKnowledgeBase;

private:
    // ?å??–æ­·?²è·¯å¾?    void InitializeHistoricalPaths();

    // ?å??–å‹¢?›æ•¸??    void InitializeFactionData();

    // ?å??–å½±?¿æ???    void InitializeImpactWeights();

    // ?å??–æ­·?²çŸ¥è­˜åº«
    void InitializeHistoricalKnowledge();

    // è¨ˆç?å½±éŸ¿è©•å?
    float CalculateImpactScore(const TArray<FMingDecisionImpactDetail>& Impacts);

    // ?´æ–°æ­·å²è·¯å?æ¬Šé?
    void UpdateHistoricalPathWeights(const FMingDecisionImpactReport& Report);

    // ?Ÿæ?å½±éŸ¿ç´°ç?
    TArray<FMingDecisionImpactDetail> GenerateImpactDetails(int32 DecisionID, int32 ChosenOptionID);

    // é©—è?å½±éŸ¿?¸æ?
    bool ValidateImpactData(const FMingDecisionImpactReport& Report) const;
};
