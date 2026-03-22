#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEndingSystem.generated.h"

UENUM(BlueprintType)
enum class EMingStrategicEndingType : uint8
{
    Victory UMETA(DisplayName = "Victory"),
    Defeat UMETA(DisplayName = "Defeat"),
    Historical UMETA(DisplayName = "Historical"),
    Alternate UMETA(DisplayName = "Alternate"),
    Secret UMETA(DisplayName = "Secret"),
    Bad UMETA(DisplayName = "Bad Ending")
};

UENUM(BlueprintType)
enum class EMingEndingCondition : uint8
{
    MilitaryVictory UMETA(DisplayName = "Military Victory"),
    PoliticalUnity UMETA(DisplayName = "Political Unity"),
    EconomicProsperity UMETA(DisplayName = "Economic Prosperity"),
    CulturalAchievement UMETA(DisplayName = "Cultural Achievement"),
    HistoricalAccuracy UMETA(DisplayName = "Historical Accuracy"),
    TimeLimit UMETA(DisplayName = "Time Limit"),
    PlayerDeath UMETA(DisplayName = "Player Death"),
    ResourceDepletion UMETA(DisplayName = "Resource Depletion"),
    TerritoryLoss UMETA(DisplayName = "Territory Loss"),
    AllianceBetrayal UMETA(DisplayName = "Alliance Betrayal")
};

USTRUCT(BlueprintType)
struct FMingEndingRequirement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingCondition ConditionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionParameter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThresholdValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Weight;

    FMingEndingRequirement()
        : ConditionType(EMingEndingCondition::MilitaryVictory)
        , ThresholdValue(0.0f)
        , bIsRequired(true)
        , Weight(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingEnding
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EndingID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EndingTitle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString LongDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingEndingType EndingType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingEndingRequirement> Requirements;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EndingImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EndingMusic;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EndingDuration;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsSecret;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsUnlocked;

    UPROPERTY(BlueprintReadOnly)
    bool bHasBeenTriggered;

    UPROPERTY(BlueprintReadOnly)
    float TriggerTime;

    UPROPERTY(BlueprintReadOnly)
    float EndingScore;

    FMingEnding()
        : EndingType(EMingEndingType::Victory)
        , EndingDuration(30.0f)
        , bIsSecret(false)
        , bIsUnlocked(false)
        , bHasBeenTriggered(false)
        , TriggerTime(0.0f)
        , EndingScore(0.0f)
    {}
};

/**
 * Ending System for MingGoRTS
 * Manages victory/defeat conditions and ending scenarios
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGSTRATEGIC_API UMingEndingSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingEndingSystem();

    // Ending Management
    UFUNCTION(BlueprintCallable, Category = "Ending")
    void InitializeEndings();

    UFUNCTION(BlueprintCallable, Category = "Ending")
    void CheckEndingConditions();

    UFUNCTION(BlueprintCallable, Category = "Ending")
    void TriggerEnding(const FString& EndingID);

    UFUNCTION(BlueprintCallable, Category = "Ending")
    void ForceEnding(const FString& EndingID);

    UFUNCTION(BlueprintPure, Category = "Ending")
    TArray<FMingEnding> GetAllEndings() const;

    UFUNCTION(BlueprintPure, Category = "Ending")
    TArray<FMingEnding> GetAvailableEndings() const;

    UFUNCTION(BlueprintPure, Category = "Ending")
    TArray<FMingEnding> GetUnlockedEndings() const;

    UFUNCTION(BlueprintPure, Category = "Ending")
    FMingEnding GetEnding(const FString& EndingID) const;

    UFUNCTION(BlueprintPure, Category = "Ending")
    FMingEnding GetCurrentEnding() const;

    // Ending Evaluation
    UFUNCTION(BlueprintPure, Category = "Evaluation")
    float CalculateEndingScore(const FString& EndingID) const;

    UFUNCTION(BlueprintPure, Category = "Evaluation")
    bool AreEndingRequirementsMet(const FString& EndingID) const;

    UFUNCTION(BlueprintPure, Category = "Evaluation")
    TArray<FMingEndingRequirement> GetFailedRequirements(const FString& EndingID) const;

    UFUNCTION(BlueprintPure, Category = "Evaluation")
    float GetEndingProgress(const FString& EndingID) const;

    // Victory/Defeat Conditions
    UFUNCTION(BlueprintCallable, Category = "Conditions")
    void SetVictoryCondition(EMingEndingCondition Condition, float Value);

    UFUNCTION(BlueprintCallable, Category = "Conditions")
    void SetDefeatCondition(EMingEndingCondition Condition, float Value);

    UFUNCTION(BlueprintPure, Category = "Conditions")
    bool IsVictoryConditionMet(EMingEndingCondition Condition) const;

    UFUNCTION(BlueprintPure, Category = "Conditions")
    bool IsDefeatConditionMet(EMingEndingCondition Condition) const;

    // Ending Statistics
    UFUNCTION(BlueprintPure, Category = "Statistics")
    TMap<FString, float> GetAllEndingScores() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    int32 GetUnlockedEndingCount() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    int32 GetTotalEndingCount() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    float GetCompletionPercentage() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    FString GetBestEnding() const;

    // Secret Endings
    UFUNCTION(BlueprintCallable, Category = "Secret")
    void UnlockSecretEnding(const FString& EndingID);

    UFUNCTION(BlueprintPure, Category = "Secret")
    TArray<FMingEnding> GetSecretEndings() const;

    UFUNCTION(BlueprintPure, Category = "Secret")
    bool IsEndingSecret(const FString& EndingID) const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEndingTriggered, const FMingEnding&, Ending, float, Score);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVictoryAchieved, const FString&, EndingID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDefeatSuffered, const FString&, EndingID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSecretEndingUnlocked, const FString&, EndingID);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnEndingTriggered OnEndingTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVictoryAchieved OnVictoryAchieved;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDefeatSuffered OnDefeatSuffered;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSecretEndingUnlocked OnSecretEndingUnlocked;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetEndingTypeName(EMingEndingType Type);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetConditionTypeName(EMingEndingCondition Condition);

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveEndingData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadEndingData(const FString& JsonString);

protected:
    UPROPERTY()
    TMap<FString, FMingEnding> Endings;

    UPROPERTY()
    TMap<EMingEndingCondition, float> VictoryConditions;

    UPROPERTY()
    TMap<EMingEndingCondition, float> DefeatConditions;

    UPROPERTY()
    FString CurrentEndingID;

    UPROPERTY()
    TSet<FString> UnlockedEndings;

    UPROPERTY()
    TMap<FString, float> EndingScores;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Ending Setup
    void SetupHistoricalEndings();
    void SetupAlternateEndings();
    void SetupSecretEndings();
    void SetupBadEndings();

    // Internal Functions
    void ProcessEnding(const FMingEnding& Ending);
    void UpdateEndingScores();
    void UnlockEnding(const FString& EndingID);
    void RecordEndingStats(const FString& EndingID);

    // Condition Evaluation
    bool EvaluateCondition(const FMingEndingRequirement& Requirement) const;
    float GetConditionValue(EMingEndingCondition Condition) const;

    // Helpers
    FMingEnding* FindEnding(const FString& EndingID);
    void CalculateAllEndingScores();
};
