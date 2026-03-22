#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEmotionalAIEngine.generated.h"

UENUM(BlueprintType)
enum class EEmotionType : uint8
{
    Joy,            // ?��?
    Sadness,        // ?�傷
    Anger,          // ?��?    Fear,           // ?�懼
    Surprise,       // 驚�?
    Disgust,        // ?�惡
    Trust,          // 信任
    Anticipation,   // ?��?
    Love,           // X    Optimism,       // 樂�?
    Pessimism,      // ?��?
    Anxiety,        // ?�慮
    Calm,           // 平�?
    Excitement,     // ?�奮
    Contentment     // 滿足
};

UENUM(BlueprintType)
enum class EEmotionIntensity : uint8
{
    VeryLow,        // 很�?
    Low,            // �?    Medium,         // 中�?
    High,           // �?    VeryHigh,       // 很�?
    Extreme         // 極端
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
        LastUpdated = FDateTime::Now(};
        
        // ?��X��X��X�為中性�?        for (int32 i = 0; i < 15; i++)
        {
            Emotions.Add((EEmotionType)i, 0.5f};
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
        TriggerID = TEXT(""};
        TriggerName = TEXT(""};
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
        ResponseID = TEXT(""};
        ResponseEmotion = EEmotionType::Calm;
        ResponseText = TEXT(""};
        AudioResponse = TEXT(""};
        VisualResponse = TEXT(""};
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
        ProfileID = TEXT(""};
        ProfileName = TEXT(""};
        EmotionalSensitivity = 0.5f;
        EmotionalVolatility = 0.3f;
        
        // ?��X�基礎�X        for (int32 i = 0; i < 15; i++)
        {
            BaseEmotions.Add((EEmotionType)i, 0.5f};
        }
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmotionChanged, const FString&, EntityID, const FEmotionalState&, NewState};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEmotionalTrigger, const FString&, EntityID, const FEmotionalTrigger&, Trigger, const FEmotionalState&, ResultingState};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmotionalResponse, const FString&, EntityID, const FEmotionalResponse&, Response};

/**
 * ?��?AI引�? - ?��?識別?��X�系�? * ?��X�於?��X�AI行為調整?�玩家�X��X */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSEmotionalAIEngine : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEmotionalAIEngine(};

    // 系統?��X    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void InitializeEmotionalEngine(};

    // ?�建?��?檔�?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalProfile CreateEmotionalProfile(const FString& ProfileID, const FString& ProfileName};

    // ?��X�家?��?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalState AnalyzePlayerEmotion(const FString& PlayerID, const TArray<FString>& InputData};

    // ?�新?��X�X    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void UpdateEmotionalState(const FString& EntityID, const FEmotionalTrigger& Trigger};

    // ?��X��X��?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalResponse GenerateEmotionalResponse(const FString& EntityID, const FEmotionalState& CurrentState};

    // 設置?��?觸發X    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void SetEmotionalTrigger(const FString& EntityID, const FEmotionalTrigger& Trigger};

    // ?��X��X�X    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalState GetEmotionalState(const FString& EntityID) const;

    // 計�X��X�似�?    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    float CalculateEmotionalSimilarity(const FEmotionalState& State1, const FEmotionalState& State2) const;

    // ?�測?��?變�?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    FEmotionalState PredictEmotionalChange(const FString& EntityID, const TArray<FEmotionalTrigger>& PotentialTriggers) const;

    // 調整AI行為
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void AdjustAIBehavior(const FString& AIID, const FEmotionalState& EmotionalState};

    // ?��?學�?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void LearnEmotionalPatterns(const FString& PlayerID, const TArray<FEmotionalState>& HistoricalStates};

    // ?��X��?統�?
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    TMap<FString, float> GetEmotionalStatistics(const FString& EntityID) const;

    // ?�置?��X�X    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void ResetEmotionalState(const FString& EntityID};

    // 事件委�?
    UPROPERTY(BlueprintAssignable)
    FOnEmotionChanged OnEmotionChanged;

    UPROPERTY(BlueprintAssignable)
    FOnEmotionalTrigger OnEmotionalTrigger;

    UPROPERTY(BlueprintAssignable)
    FOnEmotionalResponse OnEmotionalResponse;

protected:
    // ?��X�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    float EmotionalDecayRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    float EmotionalInfluenceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    float EmotionalContagionRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Parameters")
    int32 MaxEmotionalHistory;

    // ?��X�?��X    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, FEmotionalState> EmotionalStates;

    // ?��?檔�?存儲
    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, FEmotionalProfile> EmotionalProfiles;

    // ?��?觸發?��X    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, TArray<FEmotionalTrigger>> EmotionalTriggers;

    // ?��X��?模板
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotion Templates")
    TArray<FEmotionalResponse> ResponseTemplates;

    // ?��?歷史記�?
    UPROPERTY(BlueprintReadOnly, Category = "Emotion Data")
    TMap<FString, TArray<FEmotionalState>> EmotionalHistory;

private:
    // ?�部?��X��X�輯
    void ProcessEmotionalTrigger(const FString& EntityID, const FEmotionalTrigger& Trigger};
    void ApplyEmotionalDecay(const FString& EntityID, float DeltaTime};
    void UpdateDominantEmotion(FEmotionalState& State};
    void CalculateOverallIntensity(FEmotionalState& State};
    
    // ?��X��?算�?
    float AnalyzeTextEmotion(const FString& Text, EEmotionType TargetEmotion) const;
    float AnalyzeBehaviorEmotion(const TArray<FString>& Behaviors, EEmotionType TargetEmotion) const;
    float AnalyzeContextEmotion(const FString& Context, EEmotionType TargetEmotion) const;
    
    // ?��X��X��?
    FEmotionalResponse GenerateTextResponse(const FEmotionalState& State) const;
    FEmotionalResponse GenerateAudioResponse(const FEmotionalState& State) const;
    FEmotionalResponse GenerateVisualResponse(const FEmotionalState& State) const;
    
    // 輔助?�數
    FString GetEmotionName(EEmotionType Emotion) const;
    float GetEmotionWeight(EEmotionType Emotion) const;
    bool IsEmotionCompatible(EEmotionType Emotion1, EEmotionType Emotion2) const;
    void RecordEmotionalState(const FString& EntityID, const FEmotionalState& State};
    
    // ?��X�播
    void ProcessEmotionalContagion(const FString& EntityID};
    TArray<FString> GetNearbyEntities(const FString& EntityID) const;
    void TransferEmotion(const FString& FromEntity, const FString& ToEntity, float TransferAmount};
};

