#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MingBlockchainMiningSystem.h"
#include "MingBlockchainWallet.h"
#include "MingBlockchainPlayToEarn.generated.h"

/**
 * Play-to-earn event types
 */
UENUM(BlueprintType)
enum class EPlayToEarnEventType: uint8 {
    BattleVictory      = 0,
    ResourceGather     = 1,
    BuildingComplete   = 2,
    AchievementUnlock  = 3,
    DailyLogin         = 4,
    QuestComplete      = 5,
    TournamentWin     = 6,
    SocialActivity     = 7,
    Exploration       = 8,
    Crafting         = 9
};

/**
 * Reward tier levels
 */
UENUM(BlueprintType)
enum class ERewardTier: uint8 {
    Bronze    = 0,
    Silver    = 1,
    Gold      = 2,
    Platinum  = 3,
    Diamond   = 4,
    Legendary = 5
};

/**
 * Play-to-earn event data
 */
USTRUCT(BlueprintType)
struct FPlayToEarnEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly)
    EPlayToEarnEventType EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    float Timestamp;

    UPROPERTY(BlueprintReadOnly)
    ERewardTier RewardTier;

    UPROPERTY(BlueprintReadOnly)
    float BaseRewardValue;

    UPROPERTY(BlueprintReadOnly)
    float Multiplier;

    UPROPERTY(BlueprintReadOnly)
    bool IsProcessed;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventData;

    FPlayToEarnEvent()
    {
        EventID = TEXT("");
        PlayerID = TEXT("");
        EventType = EPlayToEarnEventType::BattleVictory;
        EventName = TEXT("");
        Description = TEXT("");
        Timestamp = 0.0f;
        RewardTier = ERewardTier::Bronze;
        BaseRewardValue = 0.0f;
        Multiplier = 1.0f;
        IsProcessed = false;
    }
};

/**
 * Play-to-earn reward configuration
 */
USTRUCT(BlueprintType)
struct FPlayToEarnRewardConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EPlayToEarnEventType EventType;

    UPROPERTY(BlueprintReadOnly)
    ERewardTier MinTier;

    UPROPERTY(BlueprintReadOnly)
    ERewardTier MaxTier;

    UPROPERTY(BlueprintReadOnly)
    float MinReward;

    UPROPERTY(BlueprintReadOnly)
    float MaxReward;

    UPROPERTY(BlueprintReadOnly)
    float CooldownSeconds;

    UPROPERTY(BlueprintReadOnly)
    bool IsEnabled;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredConditions;

    FPlayToEarnRewardConfig()
    {
        EventType = EPlayToEarnEventType::BattleVictory;
        MinTier = ERewardTier::Bronze;
        MaxTier = ERewardTier::Gold;
        MinReward = 1.0f;
        MaxReward = 100.0f;
        CooldownSeconds = 0.0f;
        IsEnabled = true;
    }
};

/**
 * Player progress data
 */
USTRUCT(BlueprintType)
struct FPlayerProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly)
    int32 PlayerLevel;

    UPROPERTY(BlueprintReadOnly)
    float ExperiencePoints;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalEventsCompleted;

    UPROPERTY(BlueprintReadOnly)
    float TotalEarnings;

    UPROPERTY(BlueprintReadOnly)
    ERewardTier CurrentTier;

    UPROPERTY(BlueprintReadOnly)
    float NextTierProgress;

    UPROPERTY(BlueprintReadOnly)
    TMap<EPlayToEarnEventType, int32> EventCounts;

    UPROPERTY(BlueprintReadOnly)
    TMap<EPlayToEarnEventType, float> LastEventTime;

    FPlayerProgress()
    {
        PlayerID = TEXT("");
        PlayerLevel = 1;
        ExperiencePoints = 0.0f;
        TotalEventsCompleted = 0;
        TotalEarnings = 0.0f;
        CurrentTier = ERewardTier::Bronze;
        NextTierProgress = 0.0f;
    }
};

/**
 * Leaderboard entry
 */
USTRUCT(BlueprintType)
struct FLeaderboardEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly)
    int32 Rank;

    UPROPERTY(BlueprintReadOnly)
    float TotalEarnings;

    UPROPERTY(BlueprintReadOnly)
    int32 EventsCompleted;

    UPROPERTY(BlueprintReadOnly)
    ERewardTier Tier;

    UPROPERTY(BlueprintReadOnly)
    FString AvatarURL;

    FLeaderboardEntry()
    {
        PlayerID = TEXT("");
        PlayerName = TEXT("");
        Rank = 0;
        TotalEarnings = 0.0f;
        EventsCompleted = 0;
        Tier = ERewardTier::Bronze;
        AvatarURL = TEXT("");
    }
};

// Forward declarations
class UMingBlockchainMiningSystem;
class UMingBlockchainWallet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayToEarnEventTriggered, const FPlayToEarnEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRewardEarned, const FString&, PlayerID, const FMiningReward&, Reward);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerTierChanged, const FString&, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeaderboardUpdated, const TArray<FLeaderboardEntry>&, Leaderboard);

/**
 * MingBlockchainPlayToEarn
 * 
 * Play-to-earn reward mechanics system for MingGoRTS that converts
 * in-game activities into blockchain rewards. Players earn cryptocurrency
 * and NFTs by playing the game normally.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(MingBlockchain), meta=(BlueprintSpawnableComponent))
class MINGBLOCKCHAIN_API UMingBlockchainPlayToEarn : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingBlockchainPlayToEarn();

    /**
     * Initialize play-to-earn system
     * @param MiningSystem - Reference to mining system
     * @param Wallet - Reference to blockchain wallet
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    void InitializePlayToEarn(UMingBlockchainMiningSystem* MiningSystem, UMingBlockchainWallet* Wallet);

    /**
     * Trigger a play-to-earn event
     * @param PlayerID - Player identifier
     * @param EventType - Type of event
     * @param EventData - Additional event data
     * @return Event ID if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    FString TriggerPlayToEarnEvent(const FString& PlayerID, EPlayToEarnEventType EventType, const TMap<FString, FString>& EventData);

    /**
     * Process pending events and calculate rewards
     * @param PlayerID - Player to process events for
     * @return Number of events processed
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    int32 ProcessPendingEvents(const FString& PlayerID);

    /**
     * Get player progress information
     * @param PlayerID - Player identifier
     * @return Player progress data
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    FPlayerProgress GetPlayerProgress(const FString& PlayerID) const;

    /**
     * Get current leaderboard
     * @param Limit - Maximum number of entries to return
     * @return Leaderboard entries
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    TArray<FLeaderboardEntry> GetLeaderboard(int32 Limit = 100) const;

    /**
     * Calculate reward for event
     * @param Event - Play-to-earn event
     * @return Calculated reward
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    FMiningReward CalculateEventReward(const FPlayToEarnEvent& Event);

    /**
     * Get reward configuration for event type
     * @param EventType - Event type
     * @return Reward configuration
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    FPlayToEarnRewardConfig GetRewardConfig(EPlayToEarnEventType EventType) const;

    /**
     * Set reward configuration for event type
     * @param EventType - Event type
     * @param Config - Reward configuration
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    void SetRewardConfig(EPlayToEarnEventType EventType, const FPlayToEarnRewardConfig& Config);

    /**
     * Check if player is eligible for event
     * @param PlayerID - Player identifier
     * @param EventType - Event type
     * @return True if eligible
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    bool IsPlayerEligible(const FString& PlayerID, EPlayToEarnEventType EventType) const;

    /**
     * Get player's current reward tier
     * @param PlayerID - Player identifier
     * @return Current reward tier
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    ERewardTier GetPlayerTier(const FString& PlayerID) const;

    /**
     * Get experience required for next level
     * @param PlayerID - Player identifier
     * @return Experience required
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    float GetExperienceToNextLevel(const FString& PlayerID) const;

    /**
     * Get total earnings for player
     * @param PlayerID - Player identifier
     * @return Total earnings
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    float GetTotalEarnings(const FString& PlayerID) const;

    /**
     * Get event cooldown remaining time
     * @param PlayerID - Player identifier
     * @param EventType - Event type
     * @return Cooldown remaining in seconds
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    float GetEventCooldown(const FString& PlayerID, EPlayToEarnEventType EventType) const;

    /**
     * Reset player progress
     * @param PlayerID - Player identifier
     * @return True if reset successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    bool ResetPlayerProgress(const FString& PlayerID);

    /**
     * Get all supported event types
     * @return Array of event types
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    TArray<EPlayToEarnEventType> GetSupportedEventTypes() const;

    /**
     * Enable or disable play-to-earn system
     * @param bEnabled - Whether to enable the system
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    void SetPlayToEarnEnabled(bool bEnabled);

    /**
     * Check if play-to-earn is enabled
     * @return True if enabled
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    bool IsPlayToEarnEnabled() const;

    /**
     * Set global reward multiplier
     * @param Multiplier - Reward multiplier
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    void SetGlobalRewardMultiplier(float Multiplier);

    /**
     * Get global reward multiplier
     * @return Current multiplier
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    float GetGlobalRewardMultiplier() const;

    /**
     * Update player statistics
     * @param PlayerID - Player identifier
     * @param Event - Event data
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    void UpdatePlayerStatistics(const FString& PlayerID, const FPlayToEarnEvent& Event);

    /**
     * Get daily bonus status
     * @param PlayerID - Player identifier
     * @return Daily bonus available and amount
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    bool GetDailyBonusStatus(const FString& PlayerID, float& OutBonusAmount) const;

    /**
     * Claim daily bonus
     * @param PlayerID - Player identifier
     * @return Bonus amount if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    float ClaimDailyBonus(const FString& PlayerID);

    /**
     * Get referral bonus for player
     * @param PlayerID - Player identifier
     * @return Referral bonus amount
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|PlayToEarn")
    float GetReferralBonus(const FString& PlayerID) const;

public:
    /** Delegate fired when a play-to-earn event is triggered */
    UPROPERTY(BlueprintAssignable)
    FOnPlayToEarnEventTriggered OnPlayToEarnEventTriggered;

    /** Delegate fired when a reward is earned */
    UPROPERTY(BlueprintAssignable)
    FOnRewardEarned OnRewardEarned;

    /** Delegate fired when player tier changes */
    UPROPERTY(BlueprintAssignable)
    FOnPlayerTierChanged OnPlayerTierChanged;

    /** Delegate fired when leaderboard is updated */
    UPROPERTY(BlueprintAssignable)
    FOnLeaderboardUpdated OnLeaderboardUpdated;

protected:
    /** Called when the component begins play */
    virtual void BeginPlay() override;

    /** Called when the component ends play */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /** Called every frame */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /** Reference to mining system */
    UPROPERTY()
    TObjectPtr<UMingBlockchainMiningSystem> MiningSystem;

    /** Reference to wallet */
    UPROPERTY()
    TObjectPtr<UMingBlockchainWallet> BlockchainWallet;

    /** Pending events to process */
    UPROPERTY()
    TArray<FPlayToEarnEvent> PendingEvents;

    /** Player progress data */
    UPROPERTY()
    TMap<FString, FPlayerProgress> PlayerProgressData;

    /** Reward configurations per event type */
    UPROPERTY()
    TMap<EPlayToEarnEventType, FPlayToEarnRewardConfig> RewardConfigs;

    /** Current leaderboard */
    UPROPERTY()
    TArray<FLeaderboardEntry> Leaderboard;

    /** Time since last leaderboard update */
    float TimeSinceLastLeaderboardUpdate;

    /** Initialize reward configurations */
    void InitializeRewardConfigs();

    /** Generate unique event ID */
    FString GenerateEventID() const;

    /** Calculate reward tier based on player progress */
    ERewardTier CalculateRewardTier(const FPlayerProgress& Progress) const;

    /** Get experience required for tier */
    float GetExperienceForTier(ERewardTier Tier) const;

    /** Update leaderboard */
    void UpdateLeaderboard();

    /** Process event reward */
    void ProcessEventReward(const FPlayToEarnEvent& Event);

    /** Check event cooldown */
    bool IsEventOnCooldown(const FString& PlayerID, EPlayToEarnEventType EventType) const;

    /** Get event cooldown end time */
    float GetEventCooldownEnd(const FString& PlayerID, EPlayToEarnEventType EventType) const;

    /** Set event cooldown */
    void SetEventCooldown(const FString& PlayerID, EPlayToEarnEventType EventType, float Duration);

    /** Validate event data */
    bool ValidateEventData(const FPlayToEarnEvent& Event) const;

    /** Get event name for type */
    FString GetEventName(EPlayToEarnEventType EventType) const;

    /** Get event description */
    FString GetEventDescription(EPlayToEarnEventType EventType, const TMap<FString, FString>& EventData) const;

    /** Calculate base reward for event */
    float CalculateBaseReward(EPlayToEarnEventType EventType, ERewardTier Tier) const;

    /** Apply reward multipliers */
    float ApplyRewardMultipliers(float BaseReward, const FPlayToEarnEvent& Event) const;

    /** Get tier name */
    FString GetTierName(ERewardTier Tier) const;

    /** Save player progress */
    void SavePlayerProgress(const FString& PlayerID);

    /** Load player progress */
    void LoadPlayerProgress(const FString& PlayerID);

    /** Clean up old events */
    void CleanupOldEvents();

    /** Check daily reset */
    void CheckDailyReset(const FString& PlayerID);

    /** Get current day key */
    FString GetCurrentDayKey() const;
};
