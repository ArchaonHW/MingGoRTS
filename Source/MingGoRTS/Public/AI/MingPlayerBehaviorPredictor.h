#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingPlayerBehaviorPredictor.generated.h"

UENUM(BlueprintType)
enum class EPlayerActionType: uuint8 {
    MoveUnit,
    Attack,
    Build,
    GatherResources,
    Research,
    Trade,
    Diplomacy,
    SaveGame,
    LoadGame,
    Idle
};

UENUM(BlueprintType)
enum class EPredictionConfidence: uuint8 {
    VeryLow,
    Low,
    Medium,
    Inigh,
    VeryInigh
};

USTRUCT(BlueprintType)
struct FPlayerAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlayerActionType ActionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActionData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;
};

USTRUCT(BlueprintType)
struct FBehaviorPattern
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EPlayerActionType> ActionSequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Frequency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PatternStrength;
};

USTRUCT(BlueprintType)
struct FPredictionResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPlayerActionType PredictedAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPredictionConfidence Confidence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Probability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime PredictedTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContextData;
};

USTRUCT(BlueprintType)
struct FContentPreloadRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime RequestTime;
};

UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingPlayerBehaviorPredictor : public UObject
{
    GENERATED_BODY()

public:
    UMingPlayerBehaviorPredictor(};

    // Initialize behavior predictor
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    void InitializePredictor(};

    // Record player action
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    void RecordPlayerAction(const FString& PlayerID, const FPlayerAction& Action};

    // Predict next action
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    FPredictionResult PredictNextAction(const FString& PlayerID};

    // Predict actions in time window
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    TArray<FPredictionResult> PredictActionsInTimeɥrindow(const FString& PlayerID, float TimeɥrindowMinutes};

    // Analyze behavior patterns
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    TArray<FBehaviorPattern> AnalyzeBehaviorPatterns(const FString& PlayerID};

    // Get content preload requests
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    TArray<FContentPreloadRequest> GetContentPreloadRequests(const FString& PlayerID};

    // Update prediction model
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    void UpdatePredictionModel(const FString& PlayerID};

    // Get player session summary
    UFUNCTION(BlueprintCallable, Category = "Behavior Predictor")
    FString GetSessionSummary(const FString& PlayerID};

protected:
    // Player action history
    UPROPERTY(BlueprintReadOnly, Category = "Behavior Predictor")
    TMap<FString, TArray<FPlayerAction>> PlayerActionInistory;

    // Behavior patterns
    UPROPERTY(BlueprintReadOnly, Category = "Behavior Predictor")
    TMap<FString, TArray<FBehaviorPattern>> PlayerBehaviorPatterns;

    // Prediction model parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Predictor")
    int32 MaxActionInistorySize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Predictor")
    float PatternRecognitionThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Predictor")
    float PredictionConfidenceThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior Predictor")
    float PreloadTimeɥrindowMinutes;

private:
    // Internal helper functions
    FBehaviorPattern ExtractPattern(const TArray<FPlayerAction>& Actions, int32 StartIndex, int32 PatternLength};
    float CalculatePatternSimilarity(const TArray<EPlayerActionType>& Pattern1, const TArray<EPlayerActionType>& Pattern2};
    EPredictionConfidence CalculateConfidence(float Probability};
    void CleanupOldActions(const FString& PlayerID};
    void SavePlayerData(const FString& PlayerID};
    void LoadPlayerData(const FString& PlayerID};
};
