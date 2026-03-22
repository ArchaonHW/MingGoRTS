#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEndingSystem.generated.h"

// ÁµêÂ?È°ûÂ??öË?
UENUM(BlueprintType)
enum class EMingEndingType : uint8
{
    Victory,            // ?ùÂà©ÁµêÂ?
    Defeat,             // Â§±Ê?ÁµêÂ?
    Neutral,            // ‰∏≠Á?ÁµêÂ?
    Historical,         // Ê≠∑Âè≤ÁµêÂ?
    Alternative,        // ?ø‰ª£Ê≠∑Âè≤ÁµêÂ?
    Secret              // ?±Ë?ÁµêÂ?
};

// ÁµêÂ?Ë©ïÂÉπÁ¥öÂà•
UENUM(BlueprintType)
enum class EMingEndingRating : uint8
{
    S_Rank,             // ?≤Ë©©Á¥?    A_Rank,             // ?™Á?Á¥?    B_Rank,             // ?ØÂ•ΩÁ¥?    C_Rank,             // ?ÆÈÄöÁ?
    D_Rank,             // ËºÉÂ∑ÆÁ¥?    F_Rank              // Â§±Ê?Á¥?};

// ÁµêÂ?È°ûÂà•
UENUM(BlueprintType)
enum class EMingEndingCategory : uint8
{
    PoliticalUnity,     // ?øÊ≤ªÁµ±‰?
    MilitaryConquest,   // Ëªç‰?ÂæÅÊ?
    EconomicProsperity, // Á∂ìÊ?ÁπÅÊ¶Æ
    CulturalRenaissance, // ?áÂ?Âæ©Ë?
    SocialReform,       // Á§æÊ??πÈù©
    NationalIndependence, // Ê∞ëÊ??®Á?
    WorldPower,         // ‰∏ñÁ?Âº∑Â?
    HistoricalPreservation // Ê≠∑Âè≤‰øùÂ?
};

// ÁµêÂ?Ê¢ù‰ª∂
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEndingCondition
{
    GENERATED_BODY()

    // Ê¢ù‰ª∂ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConditionID;

    // Ê¢ù‰ª∂?çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionName;

    // Ê¢ù‰ª∂?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionDescription;

    // Ê¢ù‰ª∂È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ConditionType;

    // Ê¢ù‰ª∂?ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ConditionParameters;

    // Ê¢ù‰ª∂Ê¨äÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConditionWeight;

    // ?ØÂê¶?∫Â?Ë¶ÅÊ?‰ª?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsRequired;

    // Ê¢ù‰ª∂ÊØîË??ãÁ?Á¨?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ComparisonOperator;

    FMingEndingCondition()
    {
        ConditionID = -1;
        ConditionName = TEXT("");
        ConditionDescription = TEXT("");
        ConditionType = TEXT("");
        ConditionParameters.Empty();
        ConditionWeight = 1.0f;
        bIsRequired = false;
        ComparisonOperator = TEXT(">=");
    }
};

// Ê≠∑Âè≤ÁµêÂ??∏Ê?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalEnding
{
    GENERATED_BODY()

    // ÁµêÂ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EndingID;

    // ÁµêÂ??çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingName;

    // ÁµêÂ??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingDescription;

    // ÁµêÂ?Ë©≥Á¥∞?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingDetails;

    // ÁµêÂ?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingType EndingType;

    // ÁµêÂ?Ë©ïÂÉπ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingRating EndingRating;

    // ÁµêÂ?È°ûÂà•
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEndingCategory EndingCategory;

    // Ëß??Ê¢ù‰ª∂?óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingEndingCondition> UnlockConditions;

    // ÁµêÂ?Ëß∏ÁôºÂπ¥‰ªΩ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TriggerYear;

    // Ê≠∑Âè≤?åÊôØË™™Ê?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalBackground;

    // ÁµêÂ?ÂΩ±Èüø
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EndingConsequences;

    // ÁµêÂ??ñÁ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingImagePath;

    // ÁµêÂ??≥Ê?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EndingMusicPath;

    // ÁµêÂ?Ê®ôÁ±§
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> EndingTags;

    // ?ØÂê¶?∫Èö±?èÁ?Â±Ä
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsHiddenEnding;

    // ÁµêÂ?Á®Ä?âÂ∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EndingRarity;

    FMingHistoricalEnding()
    {
        EndingID = -1;
        EndingName = TEXT("");
        EndingDescription = TEXT("");
        EndingDetails = TEXT("");
        EndingType = EMingEndingType::Neutral;
        EndingRating = EMingEndingRating::C_Rank;
        EndingCategory = EMingEndingCategory::PoliticalUnity;
        UnlockConditions.Empty();
        TriggerYear = 1949;
        HistoricalBackground = TEXT("");
        EndingConsequences.Empty();
        EndingImagePath = TEXT("");
        EndingMusicPath = TEXT("");
        EndingTags.Empty();
        bIsHiddenEnding = false;
        EndingRarity = 1.0f;
    }
};

// ÁµêÂ?Ë©ïÂ??∏Ê?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingEndingScore
{
    GENERATED_BODY()

    // ÁµêÂ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EndingID;

    // Á∏ΩÈ?Ë©ïÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalScore;

    // ?ÑÈ??•Ë???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> CategoryScores;

    // Ê±∫Á?Ë©ïÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, float> DecisionScores;

    // ‰∫ã‰ª∂Ë©ïÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<int32, float> EventScores;

    // ‰∫∫Áâ©?ú‰?Ë©ïÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RelationshipScores;

    // Ê≠∑Âè≤Ê∫ñÁ¢∫Â∫¶Ë???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HistoricalAccuracy;

    // ?µÊñ∞?ßË???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InnovationScore;

    // ?ìÂæ∑Ë©ïÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralityScore;

    // ?∞Áï•Ë©ïÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StrategyScore;

    FMingEndingScore()
    {
        EndingID = -1;
        TotalScore = 0.0f;
        CategoryScores.Empty();
        DecisionScores.Empty();
        EventScores.Empty();
        RelationshipScores.Empty();
        HistoricalAccuracy = 0.0f;
        InnovationScore = 0.0f;
        MoralityScore = 0.0f;
        StrategyScore = 0.0f;
    }
};

// ?êÂ∞±?∏Ê?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingAchievement
{
    GENERATED_BODY()

    // ?êÂ∞±ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AchievementID;

    // ?êÂ∞±?çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementName;

    // ?êÂ∞±?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementDescription;

    // ?êÂ∞±È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementType;

    // Ëß??Ê¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // ?êÂ∞±?ñÊ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AchievementIconPath;

    // ?êÂ∞±?éÂãµ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AchievementRewards;

    // ?ØÂê¶Â∑≤Ëß£??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnlocked;

    // Ëß???ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime UnlockTime;

    FMingAchievement()
    {
        AchievementID = -1;
        AchievementName = TEXT("");
        AchievementDescription = TEXT("");
        AchievementType = TEXT("");
        UnlockConditions.Empty();
        AchievementIconPath = TEXT("");
        AchievementRewards.Empty();
        bIsUnlocked = false;
        UnlockTime = FDateTime::Now();
    }
};

// ÁµêÂ?Ë©ïÂÉπÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndingDetermined, const FMingHistoricalEnding&, Ending);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndingScoreCalculated, const FMingEndingScore&, Score);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAchievementUnlocked, const FMingAchievement&, Achievement);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEndingPreview, const FMingHistoricalEnding&, Ending, float, Probability);

/**
 * Ê≠∑Âè≤ÁµêÂ?Á≥ªÁµ±
 * Ë≤†Ë≤¨Ë©ïÂÉπ?©ÂÆ∂?ÑÊ≠∑?≤Ê±∫Á≠ñË∑ØÂæë‰∏¶Á¢∫Â??ÄÁµÇÁ?Â±Ä
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalEndingSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEndingSystem();

    // ?ùÂ??ñÁ?Â±ÄÁ≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool InitializeEndingSystem();

    // Ë®ªÂ?Ê≠∑Âè≤ÁµêÂ?
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool RegisterHistoricalEnding(const FMingHistoricalEnding& Ending);

    // Ë©ïÂÉπ?©ÂÆ∂Ë∑ØÂ?
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    FMingEndingScore EvaluatePlayerPath();

    // Á¢∫Â??ÄÁµÇÁ?Â±Ä
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    FMingHistoricalEnding DetermineEnding(const FMingEndingScore& PlayerScore);

    // Ê™¢Êü•ÁµêÂ?Ê¢ù‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool CheckEndingConditions(int32 EndingID);

    // ?≤Â?ÁµêÂ?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    FMingHistoricalEnding GetEndingInfo(int32 EndingID) const;

    // ?≤Â??ØÁî®ÁµêÂ?
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TArray<int32> GetAvailableEndings() const;

    // ?≤Â?ÁµêÂ??êË¶Ω
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    TArray<FMingHistoricalEnding> GetEndingPreview();

    // Ë®àÁ?ÁµêÂ?Ê¶ÇÁ?
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    float CalculateEndingProbability(int32 EndingID);

    // Ëß???êÂ∞±
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool UnlockAchievement(int32 AchievementID);

    // ?≤Â?Â∑≤Ëß£?ñÊ?Â∞?    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TArray<FMingAchievement> GetUnlockedAchievements() const;

    // ?≤Â?ÁµêÂ?Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Historical Ending")
    TMap<EMingEndingType, int32> GetEndingStatistics() const;

    // ‰øùÂ?ÁµêÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool SaveEndingData();

    // ËºâÂÖ•ÁµêÂ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    bool LoadEndingData();

    // ?çÁΩÆÁµêÂ?Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "Historical Ending")
    void ResetEndingSystem();

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable)
    FOnEndingDetermined OnEndingDetermined;

    UPROPERTY(BlueprintAssignable)
    FOnEndingScoreCalculated OnEndingScoreCalculated;

    UPROPERTY(BlueprintAssignable)
    FOnAchievementUnlocked OnAchievementUnlocked;

    UPROPERTY(BlueprintAssignable)
    FOnEndingPreview OnEndingPreview;

protected:
    // ÁµêÂ??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingHistoricalEnding> EndingDatabase;

    // ?êÂ∞±?∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingAchievement> AchievementDatabase;

    // ?©ÂÆ∂ÁµêÂ?Ê≠∑Âè≤
    UPROPERTY()
    TArray<FMingHistoricalEnding> PlayerEndingHistory;

    // ?∂Â?Ë©ïÂ?
    UPROPERTY()
    FMingEndingScore CurrentScore;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠ÁµêÂ?
    void LoadDefaultEndings();

    // ËºâÂÖ•?êË®≠?êÂ∞±
    void LoadDefaultAchievements();

    // È©óË?ÁµêÂ??∏Ê?
    bool ValidateEndingData(const FMingHistoricalEnding& Ending) const;

    // Ë®àÁ?Ê±∫Á?Ë©ïÂ?
    float CalculateDecisionScore();

    // Ë®àÁ?‰∫ã‰ª∂Ë©ïÂ?
    float CalculateEventScore();

    // Ë®àÁ??ú‰?Ë©ïÂ?
    float CalculateRelationshipScore();

    // Ë®àÁ?Ê≠∑Âè≤Ê∫ñÁ¢∫Â∫?    float CalculateHistoricalAccuracy();

    // Ë®àÁ??µÊñ∞?ßË???    float CalculateInnovationScore();

    // Ë®àÁ??ìÂæ∑Ë©ïÂ?
    float CalculateMoralityScore();

    // Ë®àÁ??∞Áï•Ë©ïÂ?
    float CalculateStrategyScore();

    // Ê™¢Êü•?êÂ∞±Ê¢ù‰ª∂
    bool CheckAchievementConditions(int32 AchievementID);

    // ?üÊ?ÁµêÂ??èËø∞
    FString GenerateEndingDescription(const FMingHistoricalEnding& Ending, const FMingEndingScore& Score) const;

    // ?≤Â?ÁµêÂ?Ë©ïÂÉπ
    EMingEndingRating GetEndingRating(float Score) const;
};
