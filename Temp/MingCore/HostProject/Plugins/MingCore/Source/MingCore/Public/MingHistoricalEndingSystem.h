#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEndingSystem.generated.h"

// ��X��X
UENUM(BlueprintType)
enum class EMingEndingType : uint8
{
    Victory,            // X�Q��X
    Defeat,             // ��X��X
    Neutral,            // ��X��X
    Historical,         // ���v��X
    Alternative,        // X�N���v��X
    Secret              // X��X
};

// ��X�����ŧO
UENUM(BlueprintType)
enum class EMingEndingRating : uint8
{
    S_Rank,         // �v�֯�
    A_Rank,         // �u�q��
    B_Rank,         // �}�n��
    C_Rank,         // ���q��
    D_Rank,         // ���t��
    F_Rank          // ���ѯ�
};

// ��X���O
UENUM(BlueprintType)
enum class EMingEndingCategory : uint8
{
    PoliticalUnity,     // X�v��X
    MilitaryConquest,   // �xX��X
    EconomicProsperity, // �gX�c�a
    CulturalRenaissance, // X�_X
    SocialReform,       // ��X��
    NationalIndependence, // ��X
    WorldPower,         // �@X�jX
    HistoricalPreservation // ���v�OX
};

// ��X����
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEndingCondition
{
    GENERATED_BODY()

    // ����ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConditionID;

    // ����X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionName;

    // ����X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionDescription;

    // ������X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionType;

    // ����X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ConditionParameters;

    // �����vX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConditionWeight;

    // X�_X�nX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    // �����X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ComparisonOperator;

    FMingEndingCondition()
    {
        ConditionID = -1;
        ConditionName = TEXT(""};
        ConditionDescription = TEXT(""};
        ConditionType = TEXT(""};
        ConditionParameters.Empty(};
        ConditionWeight = 1.0f;
        bIsRequired = false;
        ComparisonOperator = TEXT(">="};
    }
};

// ���v��X
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalEnding
{
    GENERATED_BODY()

    // ��XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EndingID;

    // ��X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingName;

    // ��X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingDescription;

    // ��X�Բ�X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingDetails;

    // ��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingType EndingType;

    // ��X����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingRating EndingRating;

    // ��X���O
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingCategory EndingCategory;

    // X����X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingEndingCondition> UnlockConditions;

    // ��XĲ�o�~��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggerYear;

    // ���vX����X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // ��X�v�T
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EndingConsequences;

    // ��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingImagePath;

    // ��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingMusicPath;

    // ��X����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EndingTags;

    // X�_X��X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHiddenEnding;

    // ��X�}X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EndingRarity;

    FMingHistoricalEnding()
    {
        EndingID = -1;
        EndingName = TEXT(""};
        EndingDescription = TEXT(""};
        EndingDetails = TEXT(""};
        EndingType = EMingEndingType::Neutral;
        EndingRating = EMingEndingRating::C_Rank;
        EndingCategory = EMingEndingCategory::PoliticalUnity;
        UnlockConditions.Empty(};
        TriggerYear = 1949;
        HistoricalBackground = TEXT(""};
        EndingConsequences.Empty(};
        EndingImagePath = TEXT(""};
        EndingMusicPath = TEXT(""};
        EndingTags.Empty(};
        bIsHiddenEnding = false;
        EndingRarity = 1.0f;
    }
};

// ��X��X
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEndingScore
{
    GENERATED_BODY()

    // ��XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EndingID;

    // �`X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalScore;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> CategoryScores;

    // �MX��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, float> DecisionScores;

    // �ƥ��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, float> EventScores;

    // �H��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RelationshipScores;

    // ���v�ǽT��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HistoricalAccuracy;

    // X�sX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InnovationScore;

    // X�w��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralityScore;

    // X����X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StrategyScore;

    FMingEndingScore()
    {
        EndingID = -1;
        TotalScore = 0.0f;
        CategoryScores.Empty(};
        DecisionScores.Empty(};
        EventScores.Empty(};
        RelationshipScores.Empty(};
        HistoricalAccuracy = 0.0f;
        InnovationScore = 0.0f;
        MoralityScore = 0.0f;
        StrategyScore = 0.0f;
    }
};

// X�NX
USTRUCT(BlueprintType)
struct FINGCORE_API FMingAchievement
{
    GENERATED_BODY()

    // X�NID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AchievementID;

    // X�NX��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementName;

    // X�NX�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementDescription;

    // X�N��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementType;

    // X����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // X�NX��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementIconPath;

    // X�NX�y
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AchievementRewards;

    // X�_�w��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnlocked;

    // X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime UnlockTime;

    FMingAchievement()
    {
        AchievementID = -1;
        AchievementName = TEXT(""};
        AchievementDescription = TEXT(""};
        AchievementType = TEXT(""};
        UnlockConditions.Empty(};
        AchievementIconPath = TEXT(""};
        AchievementRewards.Empty(};
        bIsUnlocked = false;
        UnlockTime = FDateTime::Now(};
    }
};

// ��X�����eX
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndingDetermined, const FMingHistoricalEnding&, Ending};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndingScoreCalculated, const FMingEndingScore&, Score};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, const FMingAchievement&, Achievement};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEndingPreview, const FMingHistoricalEnding&, Ending, float, Probability};

/**
 * ���v��X�t��
 * �t�d����X�aX��X�M����|�ýTX��X��
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalEndingSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEndingSystem(};

    // X���t��
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool InitializeEndingSystem(};

    // ��X���v��X
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool RegisterHistoricalEnding(const FMingHistoricalEnding& Ending) {};

    // ����X�a��X
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    FMingEndingScore EvaluatePlayerPath(};

    // �TX��X��
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    FMingHistoricalEnding DetermineEnding(const FMingEndingScore& PlayerScore) {};

    // �ˬd��X����
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool CheckEndingConditions(int32 EndingID};

    // X��X�H��
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    FMingHistoricalEnding GetEndingInfo(int32 EndingID) const;

    // X�ε�X
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TArray<int32> GetAvailableEndings() const;

    // X��X��
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    TArray<FMingHistoricalEnding> GetEndingPreview(};

    // �pX��X��X
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    float CalculateEndingProbability(int32 EndingID};

    // X�N
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool UnlockAchievement(int32 AchievementID};

    // X�w��X    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TArray<FMingAchievement> GetUnlockedAchievements() const;

    // X��X��X
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TMap<EMingEndingType, int32> GetEndingStatistics() const;

    // �OX��X
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool SaveEndingData(};

    // ���J��X
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool LoadEndingData(};

    // X�m��X�t��
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    void ResetEndingSystem(};

    // �ƥ�eX
    UPROPERTY(BlueprintAssignable)
    FOnEndingDetermined OnEndingDetermined;

    UPROPERTY(BlueprintAssignable)
    FOnEndingScoreCalculated OnEndingScoreCalculated;

    UPROPERTY(BlueprintAssignable)
    FOnAchievementUnlocked OnAchievementUnlocked;

    UPROPERTY(BlueprintAssignable)
    FOnEndingPreview OnEndingPreview;

protected:
    // ��X    UPROPERTY()
    TMap<int32, FMingHistoricalEnding> EndingDatabase;

    // X�NX    UPROPERTY()
    TMap<int32, FMingAchievement> AchievementDatabase;

    // X�a��X���v
    UPROPERTY()
    TArray<FMingHistoricalEnding> PlayerEndingHistory;

    // X��X
    UPROPERTY()
    FMingEndingScore CurrentScore;

    // X�_�wX�lX
    UPROPERTY()
    bool bInitialized;

private:
    // ���JX�]��X
    void LoadDefaultEndings(};

    // ���JX�]X�N
    void LoadDefaultAchievements(};

    // ��X��X
    bool ValidateEndingData(const FMingHistoricalEnding& Ending) const;

    // �pX�MX��X
    float CalculateDecisionScore(};

    // �pX�ƥ��X
    float CalculateEventScore(};

    // �pX��X
    float CalculateRelationshipScore(};

    // �pX���v�ǽTX    float CalculateHistoricalAccuracy(};

    // �pX�sX    float CalculateInnovationScore(};

    // �pX�w��X
    float CalculateMoralityScore(};

    // �pX����X
    float CalculateStrategyScore(};

    // �ˬdX�N����
    bool CheckAchievementConditions(int32 AchievementID};

    // X��X�z
    FString GenerateEndingDescription(const FMingHistoricalEnding& Ending, const FMingEndingScore& Score) const;

    // X��X����
    EMingEndingRating GetEndingRating(float Score) const;
};

