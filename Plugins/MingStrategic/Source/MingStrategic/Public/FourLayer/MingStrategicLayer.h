#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingStrategicLayer.generated.h"

// ?∞Áï•?ÆÊ?È°ûÂ?
UENUM(BlueprintType)
enum class EMingStrategicObjectiveType : uint8
{
    MilitaryConquest,    // Ëªç‰?ÂæÅÊ?
    PoliticalUnity,      // ?øÊ≤ªÁµ±‰?
    EconomicDevelopment,  // Á∂ìÊ??ºÂ?
    CulturalInfluence,    // ?áÂ?ÂΩ±Èüø
    TerritorialExpansion  // ?òÂ??¥Âºµ
};

// Â§ñ‰∫§?ú‰??Ä??UENUM(BlueprintType)
enum class EMingDiplomaticStatus : uint8
{
    Allied,         // ?åÁ?
    Friendly,       // ?ãÂ•Ω
    Neutral,        // ‰∏≠Á?
    Hostile,        // ?µÂ?
    AtWar          // ?∞Áà≠
};

// ?∞Áï•Ê±∫Á?È°ûÂ?
UENUM(BlueprintType)
enum class EMingStrategicDecisionType : uint8
{
    DeclareWar,         // ÂÆ?à∞
    FormAlliance,       // ÁµêÁ?
    EconomicReform,     // Á∂ìÊ??πÈù©
    MilitaryExpansion,   // Ëªç‰??¥Âºµ
    CulturalPolicy      // ?áÂ??øÁ?
};

// ?∞Áï•‰∫ã‰ª∂
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedFactions;

    UPROPERTY(BlueprintReadOnly)
    float StrategicImpact;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

// ?∞Áï•?ÆÊ?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicObjective
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ObjectiveID;

    UPROPERTY(BlueprintReadOnly)
    EMingStrategicObjectiveType ObjectiveType;

    UPROPERTY(BlueprintReadOnly)
    FString ObjectiveDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TargetRegions;

    UPROPERTY(BlueprintReadOnly)
    float Priority;

    UPROPERTY(BlueprintReadOnly)
    float Progress;

    UPROPERTY(BlueprintReadOnly)
    FDateTime Deadline;
};

// ?∞Áï•Ê±∫Á?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratLayerDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    EMingStrategicDecisionType DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float ExpectedImpact;

    UPROPERTY(BlueprintReadOnly)
    float RiskLevel;
};

// ?∞Áï•?Ä??USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CurrentStrategy;

    UPROPERTY(BlueprintReadOnly)
    float OverallStrength;

    UPROPERTY(BlueprintReadOnly)
    float EconomicPower;

    UPROPERTY(BlueprintReadOnly)
    float MilitaryPower;

    UPROPERTY(BlueprintReadOnly)
    float DiplomaticInfluence;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> TerritoryControl;
};

/**
 * ?∞Áï•Â±§Á≥ªÁµ? * Ë≤†Ë≤¨ÂÆèË??∞Áï•Ê±∫Á??ÅË?Ê∫êÂ??ç„ÄÅÂ?‰∫§È?‰øÇÁ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingStrategicLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingStrategicLayer();

    // ÂØ¶Áèæ‰ªãÈù¢?πÊ?
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingGameEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // ?∞Áï•Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void MakeStrategicDecision(const FMingStrategicDecision& Decision);

    // Ë®≠ÁΩÆ?∞Áï•?ÆÊ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void SetStrategicObjective(const FMingStrategicObjective& Objective);

    // ÁÆ°Á?Â§ñ‰∫§?ú‰?
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void ManageDiplomaticRelations(const FString& Faction, EMingDiplomaticStatus Status);

    // ?ÜÈ??∞Áï•Ë≥áÊ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    void AllocateStrategicResources(const TMap<EMingResourceType, float>& Allocation);

    // Ë©ï‰º∞?∞Áï•Â®ÅË?
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FString> AssessStrategicThreats();

    // Ë≠òÂà•?∞Áï•Ê©üÊ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FString> IdentifyStrategicOpportunities();

    // ?≤Â??∂Â??∞Áï•?Ä??    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    FMingStrategicState GetCurrentStrategicState() const;

    // ?≤Â??∞Áï•?ÆÊ??≤Â∫¶
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TArray<FMingStrategicObjective> GetStrategicObjectives() const;

    // ?≤Â?Â§ñ‰∫§?ú‰?
    UFUNCTION(BlueprintCallable, Category = "Ming|Strategic")
    TMap<FString, EMingDiplomaticStatus> GetDiplomaticRelations() const;

protected:
    // ?∞Áï•?Ä??    UPROPERTY(BlueprintReadOnly)
    FMingStrategicState StrategicState;

    // ?∞Áï•?ÆÊ??óË°®
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicObjective> StrategicObjectives;

    // Â§ñ‰∫§?ú‰?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EMingDiplomaticStatus> DiplomaticRelations;

    // ?∞Áï•Ë≥áÊ??ÜÈ?
    UPROPERTY(BlueprintReadOnly)
    TMap<EMingResourceType, float> StrategicResourceAllocation;

    // ?∞Áï•‰∫ã‰ª∂Ê≠∑Âè≤
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingStrategicEvent> StrategicHistory;

    // ?∂Â??∞Áï•
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    FString CurrentStrategy;

    // ?∞Áï•È¢®Ê†º
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    FString StrategicStyle;

    // ?ªÊ??ßÁ?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    float AggressivenessLevel;

    // Â§ñ‰∫§?æÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic|Settings")
    float DiplomaticTendency;

    // ?ÜÊ??∞Áï•Â±Ä??    void AnalyzeStrategicSituation();

    // Ë©ï‰º∞Ë≥áÊ??ÄÊ±?    void AssessResourceRequirements();

    // Ë®àÁ??∞Áï•È¢®Èö™
    float CalculateStrategicRisk(const FMingStrategicDecision& Decision);

    // ?êÊ∏¨Ê±∫Á?ÂæåÊ?
    TArray<FString> PredictDecisionOutcome(const FMingStrategicDecision& Decision);

    // Ê∞ëÂ??πËâ≤?∞Áï•
    void ApplyRepublicanEraStrategy();

    // ?ó‰?Áµ±‰??∞Áï•
    void ExecuteNorthernExpeditionStrategy();

    // ?óÊó•?∞Áà≠?∞Áï•
    void ExecuteAntiJapaneseStrategy();

    // Á∂ìÊ?Âª∫Ë®≠?∞Áï•
    void ExecuteEconomicDevelopmentStrategy();

    // ?áÂ?ÂΩ±Èüø?∞Áï•
    void ExecuteCulturalInfluenceStrategy();

private:
    // ?∞Áï•?¥Êñ∞?ìÈ?
    float StrategicUpdateInterval;

    // ‰∏äÊ¨°?¥Êñ∞?ÇÈ?
    float LastStrategicUpdate;

    // ?∞Áï•Â®ÅË?Ë©ï‰º∞
    TMap<FString, float> StrategicThreats;

    // ?∞Áï•Ê©üÊ?Ë©ï‰º∞
    TMap<FString, float> StrategicOpportunities;

    // ?∞Áï•Ê±∫Á?Ê≠∑Âè≤
    TArray<FMingStrategicDecision> DecisionHistory;
};
