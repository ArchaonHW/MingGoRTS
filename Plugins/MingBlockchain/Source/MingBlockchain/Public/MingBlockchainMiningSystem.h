#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/WeakObjectPtr.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MingBlockchainMiningSystem.generated.h"

/**
 * Mining difficulty levels
 */
UENUM(BlueprintType)
enum class EMiningDifficulty : uint8
{
    Easy       = 0,
    Medium     = 1,
    Hard       = 2,
    Expert     = 3,
    Master     = 4
};

/**
 * Mining algorithm types
 */
UENUM(BlueprintType)
enum class EMiningAlgorithm : uint8
{
    ProofOfWork    = 0,
    ProofOfStake   = 1,
    Hybrid         = 2,
    Custom         = 3
};

/**
 * Mining reward types
 */
UENUM(BlueprintType)
enum class EMiningRewardType : uint8
{
    Token          = 0,
    NFT            = 1,
    Experience     = 2,
    Item           = 3,
    Currency       = 4
};

/**
 * Mining session data
 */
USTRUCT(BlueprintType)
struct FMiningSession
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SessionID;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerID;

    UPROPERTY(BlueprintReadOnly)
    EMiningAlgorithm Algorithm;

    UPROPERTY(BlueprintReadOnly)
    EMiningDifficulty Difficulty;

    UPROPERTY(BlueprintReadOnly)
    float StartTime;

    UPROPERTY(BlueprintReadOnly)
    float ElapsedTime;

    UPROPERTY(BlueprintReadOnly)
    float HashRate;

    UPROPERTY(BlueprintReadOnly)
    int32 AttemptsCount;

    UPROPERTY(BlueprintReadOnly)
    bool IsActive;

    UPROPERTY(BlueprintReadOnly)
    float Progress;

    FMiningSession()
    {
        SessionID = TEXT("");
        PlayerID = TEXT("");
        Algorithm = EMiningAlgorithm::ProofOfWork;
        Difficulty = EMiningDifficulty::Easy;
        StartTime = 0.0f;
        ElapsedTime = 0.0f;
        HashRate = 0.0f;
        AttemptsCount = 0;
        IsActive = false;
        Progress = 0.0f;
    }
};

/**
 * Mining reward data
 */
USTRUCT(BlueprintType)
struct FMiningReward
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMiningRewardType RewardType;

    UPROPERTY(BlueprintReadOnly)
    FString RewardID;

    UPROPERTY(BlueprintReadOnly)
    FString AssetName;

    UPROPERTY(BlueprintReadOnly)
    int32 Amount;

    UPROPERTY(BlueprintReadOnly)
    float Value;

    UPROPERTY(BlueprintReadOnly)
    FString TokenContract;

    UPROPERTY(BlueprintReadOnly)
    FString Metadata;

    FMiningReward()
    {
        RewardType = EMiningRewardType::Token;
        RewardID = TEXT("");
        AssetName = TEXT("");
        Amount = 0;
        Value = 0.0f;
        TokenContract = TEXT("");
        Metadata = TEXT("");
    }
};

/**
 * Mining statistics
 */
USTRUCT(BlueprintType)
struct FMiningStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float TotalMiningTime;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalBlocksMined;

    UPROPERTY(BlueprintReadOnly)
    float TotalEarnings;

    UPROPERTY(BlueprintReadOnly)
    float AverageHashRate;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulMines;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedMines;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    FMiningStatistics()
    {
        TotalMiningTime = 0.0f;
        TotalBlocksMined = 0;
        TotalEarnings = 0.0f;
        AverageHashRate = 0.0f;
        SuccessfulMines = 0;
        FailedMines = 0;
        SuccessRate = 0.0f;
    }
};

// Forward declarations
class UMingBlockchainWallet;
class UMingBlockchainTransactionManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiningSessionStarted, const FMiningSession&, Session);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiningSessionCompleted, const FMiningSession&, Session);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiningRewardEarned, const FMiningReward&, Reward);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMiningProgressUpdated, const FString&, SessionID, float, Progress);

/**
 * MingBlockchainMiningSystem
 * 
 * Core blockchain mining system for MingGoRTS that enables play-to-earn functionality.
 * Players can mine cryptocurrency while playing the game, earning real value rewards.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(MingBlockchain), meta=(BlueprintSpawnableComponent))
class MINGBLOCKCHAIN_API UMingBlockchainMiningSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingBlockchainMiningSystem();

    /**
     * Initialize the mining system
     * @param Wallet - Reference to the blockchain wallet
     * @param TransactionManager - Reference to transaction manager
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    void InitializeMiningSystem(UMingBlockchainWallet* Wallet, UMingBlockchainTransactionManager* TransactionManager);

    /**
     * Start a new mining session
     * @param PlayerID - Unique player identifier
     * @param Algorithm - Mining algorithm to use
     * @param Difficulty - Mining difficulty level
     * @return Session ID if successful, empty string otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    FString StartMiningSession(const FString& PlayerID, EMiningAlgorithm Algorithm, EMiningDifficulty Difficulty);

    /**
     * Stop an active mining session
     * @param SessionID - Session to stop
     * @return True if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    bool StopMiningSession(const FString& SessionID);

    /**
     * Get current mining session
     * @return Current session data
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    FMiningSession GetCurrentMiningSession() const;

    /**
     * Get all active mining sessions
     * @return Array of active sessions
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    TArray<FMiningSession> GetActiveMiningSessions() const;

    /**
     * Calculate mining reward based on performance
     * @param Session - Mining session data
     * @return Calculated reward
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    FMiningReward CalculateMiningReward(const FMiningSession& Session);

    /**
     * Get mining statistics for a player
     * @param PlayerID - Player identifier
     * @return Mining statistics
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    FMiningStatistics GetMiningStatistics(const FString& PlayerID) const;

    /**
     * Set mining difficulty
     * @param NewDifficulty - New difficulty level
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    void SetMiningDifficulty(EMiningDifficulty NewDifficulty);

    /**
     * Get current mining difficulty
     * @return Current difficulty level
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    EMiningDifficulty GetMiningDifficulty() const;

    /**
     * Check if mining is enabled
     * @return True if mining is enabled
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    bool IsMiningEnabled() const;

    /**
     * Enable or disable mining
     * @param bEnabled - Whether to enable mining
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    void SetMiningEnabled(bool bEnabled);

    /**
     * Get mining hash rate
     * @return Current hash rate (MH/s)
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    float GetCurrentHashRate() const;

    /**
     * Estimate mining time for current difficulty
     * @return Estimated time in seconds
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    float EstimateMiningTime() const;

    /**
     * Process mining simulation tick
     * @param DeltaTime - Time since last tick
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    void ProcessMiningTick(float DeltaTime);

    /**
     * Validate mining configuration
     * @return True if configuration is valid
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    bool ValidateMiningConfiguration() const;

    /**
     * Get supported mining algorithms
     * @return Array of supported algorithms
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    TArray<EMiningAlgorithm> GetSupportedAlgorithms() const;

    /**
     * Get mining efficiency bonus
     * @return Efficiency multiplier (1.0 = normal)
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    float GetMiningEfficiencyBonus() const;

    /**
     * Set mining efficiency bonus
     * @param Bonus - Efficiency multiplier
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Mining")
    void SetMiningEfficiencyBonus(float Bonus);

public:
    /** Delegate fired when a mining session starts */
    UPROPERTY(BlueprintAssignable)
    FOnMiningSessionStarted OnMiningSessionStarted;

    /** Delegate fired when a mining session completes */
    UPROPERTY(BlueprintAssignable)
    FOnMiningSessionCompleted OnMiningSessionCompleted;

    /** Delegate fired when a mining reward is earned */
    UPROPERTY(BlueprintAssignable)
    FOnMiningRewardEarned OnMiningRewardEarned;

    /** Delegate fired when mining progress is updated */
    UPROPERTY(BlueprintAssignable)
    FOnMiningProgressUpdated OnMiningProgressUpdated;

protected:
    /** Called when the component begins play */
    virtual void BeginPlay() override;

    /** Called when the component ends play */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /** Called every frame */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    /** Reference to blockchain wallet */
    UPROPERTY()
    TObjectPtr<UMingBlockchainWallet> BlockchainWallet;

    /** Reference to transaction manager */
    UPROPERTY()
    TObjectPtr<UMingBlockchainTransactionManager> TransactionManager;

    /** Current active mining sessions */
    UPROPERTY()
    TArray<FMiningSession> ActiveSessions;

    /** Current mining session */
    UPROPERTY()
    FMiningSession CurrentSession;

    /** Mining statistics per player */
    UPROPERTY()
    TMap<FString, FMiningStatistics> PlayerStatistics;

    /** Current mining difficulty */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining|Configuration")
    EMiningDifficulty CurrentDifficulty;

    /** Whether mining is enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining|Configuration")
    bool bMiningEnabled;

    /** Current hash rate */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mining|Status")
    float CurrentHashRate;

    /** Mining efficiency bonus */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining|Configuration")
    float MiningEfficiencyBonus;

    /** Base mining reward amount */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining|Configuration")
    float BaseRewardAmount;

    /** Mining progress update interval */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining|Configuration")
    float ProgressUpdateInterval;

    /** Maximum concurrent mining sessions */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mining|Configuration")
    int32 MaxConcurrentSessions;

    /** Time since last progress update */
    float TimeSinceLastProgressUpdate;

    /** Initialize mining configuration */
    void InitializeMiningConfiguration();

    /** Generate unique session ID */
    FString GenerateSessionID() const;

    /** Calculate mining progress based on hash rate and difficulty */
    float CalculateMiningProgress(const FMiningSession& Session, float DeltaTime) const;

    /** Simulate mining hash calculation */
    bool SimulateMiningHash(const FMiningSession& Session) const;

    /** Update mining statistics */
    void UpdateMiningStatistics(const FString& PlayerID, const FMiningSession& Session, bool bSuccess);

    /** Process mining reward */
    void ProcessMiningReward(const FMiningSession& Session);

    /** Get difficulty multiplier */
    float GetDifficultyMultiplier(EMiningDifficulty Difficulty) const;

    /** Get algorithm multiplier */
    float GetAlgorithmMultiplier(EMiningAlgorithm Algorithm) const;

    /** Clean up completed sessions */
    void CleanupCompletedSessions();

    /** Validate mining session */
    bool ValidateMiningSession(const FMiningSession& Session) const;
};
