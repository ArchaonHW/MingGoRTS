#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingAchievementSystem.generated.h"

UENUM(BlueprintType)
enum class EMingAchievementCategory : uint8
{
    Combat UMETA(DisplayName = "Combat"),
    Campaign UMETA(DisplayName = "Campaign"),
    Exploration UMETA(DisplayName = "Exploration"),
    Mercenary UMETA(DisplayName = "Mercenary"),
    Historical UMETA(DisplayName = "Historical"),
    Economic UMETA(DisplayName = "Economic"),
    Special UMETA(DisplayName = "Special"),
    Secret UMETA(DisplayName = "Secret")
};

UENUM(BlueprintType)
enum class EMingAchievementDifficulty : uint8
{
    Easy UMETA(DisplayName = "Easy"),
    Medium UMETA(DisplayName = "Medium"),
    Hard UMETA(DisplayName = "Hard"),
    Expert UMETA(DisplayName = "Expert"),
    Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EMingAchievementType : uint8
{
    Progress UMETA(DisplayName = "Progress"),
    Cumulative UMETA(DisplayName = "Cumulative"),
    OneTime UMETA(DisplayName = "One Time"),
    Chain UMETA(DisplayName = "Chain"),
    Timed UMETA(DisplayName = "Timed"),
    Hidden UMETA(DisplayName = "Hidden")
};

USTRUCT(BlueprintType)
struct FMingAchievementProgress
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CompletionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ProgressData;

    FMingAchievementProgress()
        : CurrentProgress(0.0f)
        , MaxProgress(100.0f)
        , bIsCompleted(false)
        , CompletionTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingAchievement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString AchievementID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString LongDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingAchievementCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingAchievementDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingAchievementType AchievementType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString IconPath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString BadgePath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Requirements;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> Rewards;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Points;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsSecret;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsHidden;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bIsUnlocked;

    UPROPERTY(BlueprintReadOnly)
    FMingAchievementProgress Progress;

    UPROPERTY(BlueprintReadOnly)
    float UnlockTime;

    UPROPERTY(BlueprintReadOnly)
    int32 CompletionCount;

    FMingAchievement()
        : Category(EMingAchievementCategory::Combat)
        , Difficulty(EMingAchievementDifficulty::Medium)
        , AchievementType(EMingAchievementType::Progress)
        , Points(10)
        , bIsSecret(false)
        , bIsHidden(false)
        , bIsUnlocked(false)
        , UnlockTime(0.0f)
        , CompletionCount(0)
    {}
};

USTRUCT(BlueprintType)
struct FMingAchievementChain
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChainID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ChainName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AchievementIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FinalReward;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentIndex;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCompleted;

    FMingAchievementChain()
        : CurrentIndex(0)
        , bIsCompleted(false)
    {}
};

/**
 * Achievement System for MingGoRTS
 * Manages achievements, progress tracking, and rewards
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGACHIEVEMENTS_API UMingAchievementSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingAchievementSystem();

    // Achievement Management
    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void InitializeAchievements();

    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void UnlockAchievement(const FString& AchievementID};

    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void UpdateProgress(const FString& AchievementID, float Progress};

    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void ResetProgress(const FString& AchievementID};

    UFUNCTION(BlueprintCallable, Category = "Achievements")
    void CompleteAchievement(const FString& AchievementID};

    UFUNCTION(BlueprintPure, Category = "Achievements")
    TArray<FMingAchievement> GetAllAchievements() const;

    UFUNCTION(BlueprintPure, Category = "Achievements")
    TArray<FMingAchievement> GetUnlockedAchievements() const;

    UFUNCTION(BlueprintPure, Category = "Achievements")
    TArray<FMingAchievement> GetLockedAchievements() const;

    UFUNCTION(BlueprintPure, Category = "Achievements")
    TArray<FMingAchievement> GetAchievementsByCategory(EMingAchievementCategory Category) const;

    UFUNCTION(BlueprintPure, Category = "Achievements")
    FMingAchievement GetAchievement(const FString& AchievementID) const;

    // Progress Tracking
    UFUNCTION(BlueprintCallable, Category = "Progress")
    void RegisterProgressEvent(const FString& EventName, float Value = 1.0f};

    UFUNCTION(BlueprintCallable, Category = "Progress")
    void TrackStatistic(const FString& StatName, float Value};

    UFUNCTION(BlueprintCallable, Category = "Progress")
    void UpdateGameStatistics();

    UFUNCTION(BlueprintPure, Category = "Progress")
    float GetAchievementProgress(const FString& AchievementID) const;

    UFUNCTION(BlueprintPure, Category = "Progress")
    bool IsAchievementCompleted(const FString& AchievementID) const;

    UFUNCTION(BlueprintPure, Category = "Progress")
    float GetOverallProgress() const;

    // Achievement Chains
    UFUNCTION(BlueprintCallable, Category = "Chains")
    void CreateAchievementChain(const FMingAchievementChain& Chain};

    UFUNCTION(BlueprintPure, Category = "Chains")
    TArray<FMingAchievementChain> GetAllChains() const;

    UFUNCTION(BlueprintPure, Category = "Chains")
    FMingAchievementChain GetChain(const FString& ChainID) const;

    UFUNCTION(BlueprintPure, Category = "Chains")
    bool IsChainCompleted(const FString& ChainID) const;

    // Statistics
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void IncrementKillCount(const FString& UnitType};

    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void AddToPlayTime(float Time};

    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordVictory(const FString& MissionID};

    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordDefeat(const FString& MissionID};

    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordResourceCollection(EMingResourceType ResourceType, float Amount};

    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordBuildingConstructed(const FString& BuildingType};

    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void RecordUnitTrained(const FString& UnitType};

    UFUNCTION(BlueprintPure, Category = "Statistics")
    int32 GetTotalAchievements() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    int32 GetUnlockedCount() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    int32 GetTotalPoints() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    float GetCompletionPercentage() const;

    // Rewards
    UFUNCTION(BlueprintCallable, Category = "Rewards")
    void GrantReward(const FString& AchievementID};

    UFUNCTION(BlueprintCallable, Category = "Rewards")
    void GrantAllRewards();

    UFUNCTION(BlueprintPure, Category = "Rewards")
    TArray<FString> GetPendingRewards() const;

    UFUNCTION(BlueprintCallable, Category = "Rewards")
    void ClaimReward(const FString& RewardID};

    // Notifications
    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowAchievementUnlocked(const FString& AchievementID};

    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowProgressUpdate(const FString& AchievementID};

    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowChainCompleted(const FString& ChainID};

    UFUNCTION(BlueprintCallable, Category = "Notifications")
    void ShowMilestoneReached(float Milestone};

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAchievementUnlocked, const FMingAchievement&, Achievement, float, Points};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProgressUpdated, const FString&, AchievementID, float, Progress};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChainCompleted, const FString&, ChainID, const FString&, FinalReward};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllAchievementsCompleted, int32, TotalPoints};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAchievementUnlocked OnAchievementUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnProgressUpdated OnProgressUpdated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnChainCompleted OnChainCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAllAchievementsCompleted OnAllAchievementsCompleted;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetCategoryName(EMingAchievementCategory Category};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetDifficultyName(EMingAchievementDifficulty Difficulty};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetTypeName(EMingAchievementType Type};

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveAchievementData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadAchievementData(const FString& JsonString};

protected:
    UPROPERTY()
    TMap<FString, FMingAchievement> Achievements;

    UPROPERTY()
    TMap<FString, FMingAchievementChain> AchievementChains;

    UPROPERTY()
    TMap<FString, float> GameStatistics;

    UPROPERTY()
    TMap<FString, float> ProgressEvents;

    UPROPERTY()
    TArray<FString> PendingRewards;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void InitializeCombatAchievements();
    void InitializeCampaignAchievements();
    void InitializeExplorationAchievements();
    void InitializeMercenaryAchievements();
    void InitializeHistoricalAchievements();
    void InitializeEconomicAchievements();
    void InitializeSpecialAchievements();
    void InitializeSecretAchievements();

    // Achievement Processing
    void ProcessProgressEvent(const FString& EventName, float Value};
    void CheckAchievementConditions(const FString& AchievementID};
    void UpdateAchievementChains(const FString& AchievementID};
    void GrantAchievementRewards(const FMingAchievement& Achievement};

    // Progress Helpers
    void UpdateProgressAchievement(const FString& AchievementID, float Progress};
    void UpdateCumulativeAchievement(const FString& AchievementID, float Progress};
    void UpdateOneTimeAchievement(const FString& AchievementID};
    void UpdateTimedAchievement(const FString& AchievementID};

    // Validation
    bool ValidateAchievementRequirements(const FString& AchievementID) const;
    bool ArePrerequisitesMet(const FString& AchievementID) const;

    // Helpers
    FMingAchievement* FindAchievement(const FString& AchievementID};
    FMingAchievementChain* FindChain(const FString& ChainID};
    float CalculateProgressPercentage(const FString& AchievementID) const;
    void UnlockHiddenAchievements();
};

