#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSRelationshipDynamics.generated.h"

UENUM(BlueprintType)
enum class ERelationshipChangeType : uint8
{
    Improvement       UMETA(DisplayName = "?ú‰??πÂ?"),
    Deterioration     UMETA(DisplayName = "?ú‰??°Â?"),
    Stabilization     UMETA(DisplayName = "?ú‰?Á©©Â?"),
    Conflict          UMETA(DisplayName = "?ú‰?Ë°ùÁ?"),
    Resolution        UMETA(DisplayName = "?ú‰?Ëß?±∫"),
    Transformation    UMETA(DisplayName = "?ú‰?ËΩâË?")
};

UENUM(BlueprintType)
enum class EInfluencePropagationType : uint8
{
    Direct            UMETA(DisplayName = "?¥Êé•ÂΩ±Èüø"),
    Indirect          UMETA(DisplayName = "?ìÊé•ÂΩ±Èüø"),
    Cascading          UMETA(DisplayName = "Á¥öËÅØÂΩ±Èüø"),
    Network           UMETA(DisplayName = "Á∂≤Áµ°ÂΩ±Èüø"),
    Global            UMETA(DisplayName = "?®Â?ÂΩ±Èüø")
};

UENUM(BlueprintType)
enum class ERelationshipStability : uint8
{
    VeryStable        UMETA(DisplayName = "?ûÂ∏∏Á©©Â?"),
    Stable            UMETA(DisplayName = "Á©©Â?"),
    Moderate          UMETA(DisplayName = "‰∏≠Á?"),
    Unstable          UMETA(DisplayName = "‰∏çÁ©©ÂÆ?),
    VeryUnstable      UMETA(DisplayName = "?ûÂ∏∏‰∏çÁ©©ÂÆ?)
};

USTRUCT(BlueprintType)
struct FRelationshipChange
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString ChangeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString SourceCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString TargetCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    ERelationshipChangeType ChangeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float OldValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float NewValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float ChangeMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString ChangeReason;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString TriggerEventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FDateTime ChangeTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float DecayRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    bool bIsPermanent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    bool bPropagates;

    FRelationshipChange()
    {
        ChangeID = TEXT("");
        SourceCharacterID = TEXT("");
        TargetCharacterID = TEXT("");
        ChangeType = ERelationshipChangeType::Improvement;
        OldValue = 0.0f;
        NewValue = 0.0f;
        ChangeMagnitude = 0.0f;
        ChangeReason = TEXT("");
        Context = TEXT("");
        TriggerEventID = TEXT("");
        DecayRate = 0.0f;
        bIsPermanent = false;
        bPropagates = true;
    }
};

USTRUCT(BlueprintType)
struct FRelationshipInfluencePropagation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FString PropagationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FString SourceCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FString InfluenceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    EInfluencePropagationType PropagationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    float InitialInfluence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    float PropagationStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    int32 MaxDepth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    float AttenuationFactor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    TArray<FString> AffectedCharacters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    TMap<FString, float> InfluenceValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FDateTime EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    bool bIsActive;

    FRelationshipInfluencePropagation()
    {
        PropagationID = TEXT("");
        SourceCharacterID = TEXT("");
        InfluenceType = TEXT("");
        PropagationType = EInfluencePropagationType::Direct;
        InitialInfluence = 0.0f;
        PropagationStrength = 1.0f;
        MaxDepth = 3;
        AttenuationFactor = 0.5f;
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
        bIsActive = false;
    }
};

USTRUCT(BlueprintType)
struct FRelationshipStabilityAnalysis
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    FString NetworkID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    ERelationshipStability OverallStability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    float StabilityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TMap<FString, float> CharacterStability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> UnstableRelationships;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> CriticalRelationships;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> RiskFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> StabilizingFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    float PredictedStability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    FDateTime AnalysisTime;

    FRelationshipStabilityAnalysis()
    {
        NetworkID = TEXT("");
        OverallStability = ERelationshipStability::Moderate;
        StabilityScore = 0.0f;
        PredictedStability = 0.0f;
        AnalysisTime = FDateTime::Now();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRelationshipChanged, const FString&, SourceID, const FString&, TargetID, ERelationshipChangeType, ChangeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInfluencePropagated, const FString&, PropagationID, const TArray<FString>&, AffectedCharacters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStabilityAnalysisComplete, const FString&, NetworkID, const FRelationshipStabilityAnalysis&, Analysis);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRelationshipDynamics : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSRelationshipDynamics();

    // ?ú‰??ïÊ?‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Relationship Dynamics")
    FOnRelationshipChanged OnRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Dynamics")
    FOnInfluencePropagated OnInfluencePropagated;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Dynamics")
    FOnStabilityAnalysisComplete OnStabilityAnalysisComplete;

    // ?ùÂ??ñÈ?‰øÇÂ??ãÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void InitializeRelationshipDynamics();

    // ?âÁî®?ú‰?ËÆäÂ?
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool ApplyRelationshipChange(const FString& SourceID, const FString& TargetID, ERelationshipChangeType ChangeType, float ChangeValue, const FString& Reason);

    // ?≥Êí≠?ú‰?ÂΩ±Èüø
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    FString PropagateInfluence(const FString& SourceID, const FString& InfluenceType, float InfluenceValue, EInfluencePropagationType PropagationType, int32 MaxDepth);

    // ?ÜÊ?Á∂≤Áµ°Á©©Â???    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void AnalyzeNetworkStability(const FString& NetworkID);

    // ?≤Â??ú‰?ËÆäÂ?Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FRelationshipChange> GetRelationshipChangeHistory(const FString& SourceID, const FString& TargetID) const;

    // ?≤Â?ÂΩ±Èüø?≥Êí≠?Ä??    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FRelationshipInfluencePropagation> GetActivePropagations() const;

    // ?≤Â?Á©©Â??ßÂ??êÁ???    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    FRelationshipStabilityAnalysis GetStabilityAnalysis(const FString& NetworkID) const;

    // ?êÊ∏¨?ú‰??ºÂ?Ë∂®Âã¢
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<float> PredictRelationshipTrend(const FString& SourceID, const FString& TargetID, int32 TimeSteps) const;

    // Ë®àÁ??ú‰?Ë°∞Ê?
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    float CalculateRelationshipDecay(const FString& SourceID, const FString& TargetID, float TimeElapsed) const;

    // ?âÁî®?ÇÈ?Ë°∞Ê?
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void ApplyTimeDecay(float DeltaTime);

    // Ê™¢Êü•?ú‰?Ë°ùÁ?
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FString> CheckRelationshipConflicts(const FString& NetworkID) const;

    // Ëß?±∫?ú‰?Ë°ùÁ?
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool ResolveRelationshipConflict(const FString& SourceID, const FString& TargetID);

    // Ë®àÁ?Á∂≤Áµ°?ïÊ??áÊ?
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TMap<FString, float> CalculateNetworkDynamics(const FString& NetworkID) const;

    // ?≤Â??ú‰?ËÆäÂ?Âª∫Ë≠∞
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FString> GetRelationshipChangeSuggestions(const FString& CharacterID) const;

    // Ê®°Êì¨?ú‰??ºÂ?
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void SimulateRelationshipDevelopment(const FString& NetworkID, int32 SimulationSteps);

    // ?≤Â??ú‰?Á©©Â??ßÈ?Ê∏?    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    float PredictRelationshipStability(const FString& SourceID, const FString& TargetID) const;

    // Ë®≠ÁΩÆ?ú‰?Ë°∞Ê??ÉÊï∏
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void SetDecayParameters(float BaseDecayRate, float InfluenceDecayRate);

    // ?≤Â??ïÊ?Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TMap<FString, float> GetDynamicsStatistics() const;

    // ‰øùÂ??ïÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool SaveDynamicsData(const FString& SaveSlotName);

    // ËºâÂÖ•?ïÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool LoadDynamicsData(const FString& SaveSlotName);

protected:
    // ?ú‰?ËÆäÂ?Ê≠∑Âè≤
    UPROPERTY()
    TArray<FRelationshipChange> RelationshipChanges;

    // Ê¥ªÂ??ÑÂΩ±?øÂÇ≥??    UPROPERTY()
    TArray<FRelationshipInfluencePropagation> ActivePropagations;

    // Á©©Â??ßÂ??êÁ???    UPROPERTY()
    TMap<FString, FRelationshipStabilityAnalysis> StabilityAnalyses;

    // ?ú‰?Á∂≤Áµ°Á≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    class UMingGoRTSRelationshipNetwork* RelationshipNetwork;

    // Ë°∞Ê??ÉÊï∏
    float BaseDecayRate;
    float InfluenceDecayRate;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ïÁ??ú‰?ËÆäÂ?
    void ProcessRelationshipChange(const FRelationshipChange& Change);

    // ?∑Ë?ÂΩ±Èüø?≥Êí≠
    void ExecuteInfluencePropagation(FRelationshipInfluencePropagation& Propagation);

    // Ë®àÁ??≥Êí≠Ë°∞Ê?
    float CalculatePropagationDecay(float InitialValue, int32 Depth, float AttenuationFactor) const;

    // ?¥Êñ∞?ú‰?Á©©Â???    void UpdateRelationshipStability(const FString& NetworkID);

    // Ê™¢Ê∏¨?ú‰?Ê®°Â?
    void DetectRelationshipPatterns(const FString& NetworkID);

    // ?êÊ∏¨Á∂≤Áµ°ËÆäÂ?
    void PredictNetworkChanges(const FString& NetworkID);

    // Ë®àÁ??ú‰?Ê¨äÈ?
    float CalculateRelationshipWeight(const FString& SourceID, const FString& TargetID) const;

    // ?≤Â??ú‰?ËÆäÂ?È°ûÂ?Ê¨äÈ?
    float GetChangeTypeWeight(ERelationshipChangeType ChangeType) const;

    // Ê™¢Êü•?ú‰??æÂÄ?    bool CheckRelationshipThreshold(const FString& SourceID, const FString& TargetID, float Threshold) const;

    // ?âÁî®?ú‰??æÂÄ?    void ApplyRelationshipThreshold(const FString& SourceID, const FString& TargetID);

    // Ë®àÁ?Á∂≤Áµ°ÂΩ±Èüø??    float CalculateNetworkInfluence(const FString& NetworkID) const;

    // Ê™¢Ê∏¨?ú‰?????çÊ?
    void DetectRelationshipChainReactions(const FString& NetworkID);

    // ?ïÁ?????çÊ?
    void ProcessChainReaction(const TArray<FString>& AffectedCharacters);

    // Ë®àÁ??ú‰??¢Âæ©??    float CalculateRelationshipResilience(const FString& SourceID, const FString& TargetID) const;

    // ?êÊ∏¨?ú‰??¢Âæ©?ÇÈ?
    float PredictRelationshipRecoveryTime(const FString& SourceID, const FString& TargetID) const;

    // ?üÊ?ËÆäÂ?ID
    FString GenerateChangeID(const FString& SourceID, const FString& TargetID) const;

    // ?üÊ??≥Êí≠ID
    FString GeneratePropagationID(const FString& SourceID, const FString& InfluenceType) const;

    // Ê∏ÖÁ??éÊ??ÑÂÇ≥??    void CleanupExpiredPropagations();

    // ?¥Êñ∞Áµ±Ë??∏Ê?
    void UpdateStatistics();

    // Ë®òÈ??ïÊ?‰∫ã‰ª∂
    void RecordDynamicsEvent(const FString& EventType, const FString& Description);

    // ?ÜÊ??ú‰?Ë∂®Âã¢
    void AnalyzeRelationshipTrends(const FString& NetworkID);

    // Ë®àÁ?Ë∂®Âã¢?úÁ?
    float CalculateTrendSlope(const TArray<float>& Values) const;

    // ?êÊ∏¨Ë∂®Âã¢?πÂ?
    int32 PredictTrendDirection(const TArray<float>& Values) const;

    // ?≤Â??ú‰?ËÆäÂ??ªÁ?
    float GetRelationshipChangeFrequency(const FString& SourceID, const FString& TargetID) const;

    // Ë®àÁ??ú‰?ËÆäÂ?ÂπÖÂ∫¶
    float CalculateRelationshipChangeMagnitude(const FString& SourceID, const FString& TargetID) const;

    // Ê™¢Êü•?ú‰?Âπ≥Ë°°
    bool CheckRelationshipBalance(const FString& NetworkID) const;

    // ?¢Âæ©?ú‰?Âπ≥Ë°°
    void RestoreRelationshipBalance(const FString& NetworkID);

    // Ë®àÁ?Á∂≤Áµ°Âπ≥Ë°°Â∫?    float CalculateNetworkBalance(const FString& NetworkID) const;

    // Ê™¢Ê∏¨?ú‰??∞Â∏∏
    TArray<FString> DetectRelationshipAnomalies(const FString& NetworkID) const;

    // ?ïÁ??ú‰??∞Â∏∏
    void HandleRelationshipAnomaly(const FString& SourceID, const FString& TargetID);

    // Ë®àÁ??∞Â∏∏?ÜÊï∏
    float CalculateAnomalyScore(const FString& SourceID, const FString& TargetID) const;

    // ?êÊ∏¨?ú‰?Â¥©ÊΩ∞È¢®Èö™
    float PredictRelationshipCollapseRisk(const FString& SourceID, const FString& TargetID) const;

    // Ë®àÁ?Á∂≤Áµ°?åÊÄßÊ???    float CalculateNetworkResilienceIndex(const FString& NetworkID) const;

    // Ê™¢Ê∏¨Á∂≤Áµ°?ÜÂº±Èª?    TArray<FString> DetectNetworkVulnerabilities(const FString& NetworkID) const;

    // Ë®àÁ??ú‰??≥Ê?È¢®Èö™
    float CalculateRelationshipContagionRisk(const FString& NetworkID) const;

    // Ê®°Êì¨?ú‰??≥Êí≠
    void SimulateRelationshipContagion(const FString& NetworkID, const FString& SourceID, float ContagionStrength);

    // Ë®àÁ??ú‰??îÈõ¢?àÊ?
    float CalculateRelationshipIsolationEffect(const FString& NetworkID, const FString& CharacterID) const;

    // ?êÊ∏¨?ú‰?ÊºîÂ?Ë∑ØÂ?
    TArray<FString> PredictRelationshipEvolutionPath(const FString& SourceID, const FString& TargetID) const;

    // Ë®àÁ??ú‰?ÊºîÂ?Ê¶ÇÁ?
    TMap<ERelationshipType, float> CalculateRelationshipEvolutionProbabilities(const FString& SourceID, const FString& TargetID) const;

    // ?üÊ??ú‰??ºÂ??±Â?
    FString GenerateRelationshipDevelopmentReport(const FString& NetworkID) const;
};
