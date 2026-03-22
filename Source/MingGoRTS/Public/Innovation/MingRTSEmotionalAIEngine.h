#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEmotionalAIEngine.generated.h"

UENUM(BlueprintType)
enum class EEmotionType : uint8
{
    Joy,            // ?úÊ?
    Sadness,        // ?≤ÂÇ∑
    Anger,          // ?§ÊÄ?    Fear,           // ?êÊáº
    Surprise,       // È©öË?
    Disgust,        // ?≠ÊÉ°
    Trust,          // ‰ø°‰ªª
    Anticipation,   // ?üÂ?
    Love,           // ??    Optimism,       // Ê®ÇË?
    Pessimism,      // ?≤Ë?
    Anxiety,        // ?¶ÊÖÆ
    Calm,           // Âπ≥È?
    Excitement,     // ?àÂ•Æ
    Contentment     // ÊªøË∂≥
};

UENUM(BlueprintType)
enum class EEmotionIntensity : uint8
{
    VeryLow,        // Âæà‰?
    Low,            // ‰Ω?    Medium,         // ‰∏≠Á?
    High,           // È´?    VeryHigh,       // ÂæàÈ?
    Extreme         // Ê•µÁ´Ø
};

USTRUCT(BlueprintType)
struct FEmotionalState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EEmotionType, float> Emotions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEmotionType DominantEmotion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEmotionIntensity OverallIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmotionalStability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastUpdated;

    FEmotionalState()
    {
        DominantEmotion = EEmotionType::Calm;
        OverallIntensity = EEmotionIntensity::Medium;
        EmotionalStability = 0.5f;
        LastUpdated = FDateTime::Now();
        
        // ?ùÂ??ñÊ??âÊ??üÁÇ∫‰∏≠ÊÄßÂÄ?        for (int32 i = 0; i < 15; i++)
        {
            Emotions.Add((EEmotionType)i, 0.5f);
        }
    }
};

USTRUCT(BlueprintType)
struct FEmotionalTrigger
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TriggerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TriggerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEmotionType TargetEmotion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TriggerStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TriggerConditions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPersistent;

    FEmotionalTrigger()
    {
        TriggerID = TEXT("");
        TriggerName = TEXT("");
        TargetEmotion = EEmotionType::Calm;
        TriggerStrength = 0.5f;
        Duration = 10.0f;
        bIsPersistent = false;
    }
};

USTRUCT(BlueprintType)
struct FEmotionalResponse
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ResponseID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEmotionType ResponseEmotion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ResponseText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AudioResponse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString VisualResponse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ResponseIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ResponseActions;

    FEmotionalResponse()
    {
        ResponseID = TEXT("");
        ResponseEmotion = EEmotionType::Calm;
        ResponseText = TEXT("");
        AudioResponse = TEXT("");
        VisualResponse = TEXT("");
        ResponseIntensity = 0.5f;
    }
};

USTRUCT(BlueprintType)
struct FEmotionalProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ProfileID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ProfileName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EEmotionType, float> BaseEmotions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmotionalSensitivity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmotionalVolatility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EEmotionType> PreferredEmotions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EEmotionType> AvoidedEmotions;

    FEmotionalProfile()
    {
        ProfileID = TEXT("");
        ProfileName = TEXT("");
        EmotionalSensitivity = 0.5f;
        EmotionalVolatility = 0.3f;
        
        // ?ùÂ??ñÂü∫Á§éÊ???        for (int32 i = 0; i < 15; i++)
        {
            BaseEmotions.Add((EEmotionType)i, 0.5f);
        }
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmotionChanged, const FString&, EntityID, const FEmotionalState&, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEmotionalTrigger, const FString&, EntityID, const FEmotionalTrigger&, Trigger, const FEmotionalState&, ResultingState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmotionalResponse, const FString&, EntityID, const FEmotionalResponse&, Response);

/**
 * ?ÖÊ?AIÂºïÊ? - ?ÖÊ?Ë≠òÂà•?åÂ??âÁ≥ªÁµ? * ?ê‰??∫Êñº?ÖÊ??ÑAIË°åÁÇ∫Ë™øÊï¥?åÁé©ÂÆ∂Ê??üÂ??? */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSEmotionalAIEngine : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEmotionalAIEngine();

    // Á≥ªÁµ±?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void InitializeEmotionalEngine();

    // ?µÂª∫?ÖÊ?Ê™îÊ?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalProfile CreateEmotionalProfile(const FString& ProfileID, const FString& ProfileName);

    // ?ÜÊ??©ÂÆ∂?ÖÊ?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalState AnalyzePlayerEmotion(const FString& PlayerID, const TArray<FString>& InputData);

    // ?¥Êñ∞?ÖÊ??Ä??    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void UpdateEmotionalState(const FString& EntityID, const FEmotionalTrigger& Trigger);

    // ?üÊ??ÖÊ??ûÊ?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalResponse GenerateEmotionalResponse(const FString& EntityID, const FEmotionalState& CurrentState);

    // Ë®≠ÁΩÆ?ÖÊ?Ëß∏Áôº??    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void SetEmotionalTrigger(const FString& EntityID, const FEmotionalTrigger& Trigger);

    // ?≤Â??ÖÊ??Ä??    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalState GetEmotionalState(const FString& EntityID) const;

    // Ë®àÁ??ÖÊ??∏‰ººÂ∫?    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    float CalculateEmotionalSimilarity(const FEmotionalState& State1, const FEmotionalState& State2) const;

    // ?êÊ∏¨?ÖÊ?ËÆäÂ?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalState PredictEmotionalChange(const FString& EntityID, const TArray<FEmotionalTrigger>& PotentialTriggers) const;

    // Ë™øÊï¥AIË°åÁÇ∫
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void AdjustAIBehavior(const FString& AIID, const FEmotionalState& EmotionalState);

    // ?ÖÊ?Â≠∏Á?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void LearnEmotionalPatterns(const FString& PlayerID, const TArray<FEmotionalState>& HistoricalStates);

    // ?≤Â??ÖÊ?Áµ±Ë?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    TMap<FString, float> GetEmotionalStatistics(const FString& EntityID) const;

    // ?çÁΩÆ?ÖÊ??Ä??    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void ResetEmotionalState(const FString& EntityID);

    // ‰∫ã‰ª∂ÂßîÊ?
    UPROPERTY(BlueprintAssignable)
    FOnEmotionChanged OnEmotionChanged;

    UPROPERTY(BlueprintAssignable)
    FOnEmotionalTrigger OnEmotionalTrigger;

    UPROPERTY(BlueprintAssignable)
    FOnEmotionalResponse OnEmotionalResponse;

protected:
    // ?ÖÊ??ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    float EmotionalDecayRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    float EmotionalInfluenceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    float EmotionalContagionRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    int32 MaxEmotionalHistory;

    // ?ÖÊ??Ä?ãÂ???    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, FEmotionalState> EmotionalStates;

    // ?ÖÊ?Ê™îÊ?Â≠òÂÑ≤
    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, FEmotionalProfile> EmotionalProfiles;

    // ?ÖÊ?Ëß∏Áôº?®Â???    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, TArray<FEmotionalTrigger>> EmotionalTriggers;

    // ?ÖÊ??ûÊ?Ê®°Êùø
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Templates")
    TArray<FEmotionalResponse> ResponseTemplates;

    // ?ÖÊ?Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, TArray<FEmotionalState>> EmotionalHistory;

private:
    // ?ßÈÉ®?ÖÊ??ïÁ??èËºØ
    void ProcessEmotionalTrigger(const FString& EntityID, const FEmotionalTrigger& Trigger);
    void ApplyEmotionalDecay(const FString& EntityID, float DeltaTime);
    void UpdateDominantEmotion(FEmotionalState& State);
    void CalculateOverallIntensity(FEmotionalState& State);
    
    // ?ÖÊ??ÜÊ?ÁÆóÊ?
    float AnalyzeTextEmotion(const FString& Text, EEmotionType TargetEmotion) const;
    float AnalyzeBehaviorEmotion(const TArray<FString>& Behaviors, EEmotionType TargetEmotion) const;
    float AnalyzeContextEmotion(const FString& Context, EEmotionType TargetEmotion) const;
    
    // ?ÖÊ??ûÊ??üÊ?
    FEmotionalResponse GenerateTextResponse(const FEmotionalState& State) const;
    FEmotionalResponse GenerateAudioResponse(const FEmotionalState& State) const;
    FEmotionalResponse GenerateVisualResponse(const FEmotionalState& State) const;
    
    // ËºîÂä©?ΩÊï∏
    FString GetEmotionName(EEmotionType Emotion) const;
    float GetEmotionWeight(EEmotionType Emotion) const;
    bool IsEmotionCompatible(EEmotionType Emotion1, EEmotionType Emotion2) const;
    void RecordEmotionalState(const FString& EntityID, const FEmotionalState& State);
    
    // ?ÖÊ??≥Êí≠
    void ProcessEmotionalContagion(const FString& EntityID);
    TArray<FString> GetNearbyEntities(const FString& EntityID) const;
    void TransferEmotion(const FString& FromEntity, const FString& ToEntity, float TransferAmount);
};
