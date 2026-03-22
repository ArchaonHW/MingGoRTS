#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MingBlockchainWallet.h"
#include "MingBlockchainTransactionManager.generated.h"

/**
 * Transaction priority levels
 */
UENUM(BlueprintType)
enum class ETransactionPriority: uint8 {
    Low       = 0,
    Medium    = 1,
    High      = 2,
    Urgent    = 3,
    Critical  = 4
};

/**
 * Transaction fee types
 */
UENUM(BlueprintType)
enum class ETransactionFeeType: uint8 {
    Standard   = 0,
    Fast       = 1,
    Instant    = 2,
    Custom     = 3
};

/**
 * Blockchain transaction data
 */
USTRUCT(BlueprintType)
struct FBlockchainTransaction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TransactionHash;

    UPROPERTY(BlueprintReadOnly)
    FString FromAddress;

    UPROPERTY(BlueprintReadOnly)
    FString ToAddress;

    UPROPERTY(BlueprintReadOnly)
    FString TokenContract;

    UPROPERTY(BlueprintReadOnly)
    FString TokenSymbol;

    UPROPERTY(BlueprintReadOnly)
    float Amount;

    UPROPERTY(BlueprintReadOnly)
    float GasFee;

    UPROPERTY(BlueprintReadOnly)
    float GasPrice;

    UPROPERTY(BlueprintReadOnly)
    int32 GasLimit;

    UPROPERTY(BlueprintReadOnly)
    ETransactionPriority Priority;

    UPROPERTY(BlueprintReadOnly)
    ETransactionStatus Status;

    UPROPERTY(BlueprintReadOnly)
    FString BlockHash;

    UPROPERTY(BlueprintReadOnly)
    int32 BlockNumber;

    UPROPERTY(BlueprintReadOnly)
    int32 ConfirmationCount;

    UPROPERTY(BlueprintReadOnly)
    float Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString Metadata;

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    FBlockchainTransaction()
    {
        TransactionHash = TEXT("");
        FromAddress = TEXT("");
        ToAddress = TEXT("");
        TokenContract = TEXT("");
        TokenSymbol = TEXT("");
        Amount = 0.0f;
        GasFee = 0.0f;
        GasPrice = 0.0f;
        GasLimit = 21000;
        Priority = ETransactionPriority::Medium;
        Status = ETransactionStatus::Pending;
        BlockHash = TEXT("");
        BlockNumber = 0;
        ConfirmationCount = 0;
        Timestamp = 0.0f;
        Metadata = TEXT("");
        ErrorMessage = TEXT("");
    }
};

/**
 * Transaction batch
 */
USTRUCT(BlueprintType)
struct FTransactionBatch
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString BatchID;

    UPROPERTY(BlueprintReadOnly)
    TArray<FBlockchainTransaction> Transactions;

    UPROPERTY(BlueprintReadOnly)
    float TotalAmount;

    UPROPERTY(BlueprintReadOnly)
    float TotalGasFee;

    UPROPERTY(BlueprintReadOnly)
    ETransactionStatus BatchStatus;

    UPROPERTY(BlueprintReadOnly)
    float CreatedTime;

    UPROPERTY(BlueprintReadOnly)
    float ProcessedTime;

    FTransactionBatch()
    {
        BatchID = TEXT("");
        Transactions.Empty();
        TotalAmount = 0.0f;
        TotalGasFee = 0.0f;
        BatchStatus = ETransactionStatus::Pending;
        CreatedTime = 0.0f;
        ProcessedTime = 0.0f;
    }
};

/**
 * Transaction statistics
 */
USTRUCT(BlueprintType)
struct FTransactionStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalTransactions;

    UPROPERTY(BlueprintReadOnly)
    int32 SuccessfulTransactions;

    UPROPERTY(BlueprintReadOnly)
    int32 FailedTransactions;

    UPROPERTY(BlueprintReadOnly)
    int32 PendingTransactions;

    UPROPERTY(BlueprintReadOnly)
    float TotalVolume;

    UPROPERTY(BlueprintReadOnly)
    float TotalGasFees;

    UPROPERTY(BlueprintReadOnly)
    float AverageGasFee;

    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly)
    float AverageTransactionTime;

    FTransactionStatistics()
    {
        TotalTransactions = 0;
        SuccessfulTransactions = 0;
        FailedTransactions = 0;
        PendingTransactions = 0;
        TotalVolume = 0.0f;
        TotalGasFees = 0.0f;
        AverageGasFee = 0.0f;
        SuccessRate = 0.0f;
        AverageTransactionTime = 0.0f;
    }
};

// Forward declarations
class UMingBlockchainWallet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransactionCreated, const FBlockchainTransaction&, Transaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransactionConfirmed, const FBlockchainTransaction&, Transaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransactionFailed, const FBlockchainTransaction&, Transaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBatchProcessed, const FTransactionBatch&, Batch);

/**
 * MingBlockchainTransactionManager
 * 
 * Comprehensive blockchain transaction management system for MingGoRTS that handles
 * all cryptocurrency transactions, batch processing, gas optimization, and
 * transaction monitoring with real-time status updates.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(MingBlockchain), meta=(BlueprintSpawnableComponent))
class MINGBLOCKCHAIN_API UMingBlockchainTransactionManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingBlockchainTransactionManager();

    /**
     * Initialize transaction manager
     * @param Wallet - Reference to blockchain wallet
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    void InitializeTransactionManager(UMingBlockchainWallet* Wallet);

    /**
     * Create and send a new transaction
     * @param FromAddress - Sender address
     * @param ToAddress - Recipient address
     * @param TokenContract - Token contract address
     * @param Amount - Amount to send
     * @param Priority - Transaction priority
     * @return Transaction hash if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FString CreateTransaction(const FString& FromAddress, const FString& ToAddress, const FString& TokenContract, float Amount, ETransactionPriority Priority = ETransactionPriority::Medium);

    /**
     * Create a reward transaction
     * @param PlayerID - Player identifier
     * @param Reward - Mining reward to process
     * @return Transaction hash if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FString CreateRewardTransaction(const FString& PlayerID, const FMiningReward& Reward);

    /**
     * Process an existing transaction
     * @param Transaction - Transaction to process
     * @return True if processing started successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    bool ProcessTransaction(const FBlockchainTransaction& Transaction);

    /**
     * Create a batch of transactions
     * @param Transactions - Array of transactions to batch
     * @return Batch ID if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FString CreateTransactionBatch(const TArray<FBlockchainTransaction>& Transactions);

    /**
     * Process a transaction batch
     * @param BatchID - Batch identifier
     * @return True if batch processing started
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    bool ProcessTransactionBatch(const FString& BatchID);

    /**
     * Get transaction status
     * @param TransactionHash - Transaction hash
     * @return Transaction data
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FBlockchainTransaction GetTransaction(const FString& TransactionHash) const;

    /**
     * Get transaction batch
     * @param BatchID - Batch identifier
     * @return Batch data
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FTransactionBatch GetTransactionBatch(const FString& BatchID) const;

    /**
     * Get pending transactions
     * @return Array of pending transactions
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    TArray<FBlockchainTransaction> GetPendingTransactions() const;

    /**
     * Get transaction history
     * @param Limit - Maximum number of transactions to return
     * @return Array of transactions
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    TArray<FBlockchainTransaction> GetTransactionHistory(int32 Limit = 100) const;

    /**
     * Get transaction statistics
     * @param PlayerID - Player identifier (optional)
     * @return Transaction statistics
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FTransactionStatistics GetTransactionStatistics(const FString& PlayerID = TEXT("")) const;

    /**
     * Cancel a pending transaction
     * @param TransactionHash - Transaction hash
     * @return True if cancelled successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    bool CancelTransaction(const FString& TransactionHash);

    /**
     * Retry a failed transaction
     * @param TransactionHash - Transaction hash
     * @param NewPriority - New transaction priority
     * @return New transaction hash if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FString RetryTransaction(const FString& TransactionHash, ETransactionPriority NewPriority = ETransactionPriority::High);

    /**
     * Estimate gas fee for transaction
     * @param FromAddress - Sender address
     * @param ToAddress - Recipient address
     * @param TokenContract - Token contract address
     * @param Amount - Amount to send
     * @param Priority - Transaction priority
     * @return Estimated gas fee
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    float EstimateGasFee(const FString& FromAddress, const FString& ToAddress, const FString& TokenContract, float Amount, ETransactionPriority Priority = ETransactionPriority::Medium) const;

    /**
     * Get current gas price
     * @return Current gas price in Gwei
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    float GetCurrentGasPrice() const;

    /**
     * Get recommended gas price for priority
     * @param Priority - Transaction priority
     * @return Recommended gas price
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    float GetRecommendedGasPrice(ETransactionPriority Priority) const;

    /**
     * Set transaction fee type
     * @param FeeType - Fee type to use
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    void SetTransactionFeeType(ETransactionFeeType FeeType);

    /**
     * Get current transaction fee type
     * @return Current fee type
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    ETransactionFeeType GetTransactionFeeType() const;

    /**
     * Enable or disable auto-retry for failed transactions
     * @param bEnabled - Whether to enable auto-retry
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    void SetAutoRetryEnabled(bool bEnabled);

    /**
     * Check if auto-retry is enabled
     * @return True if auto-retry is enabled
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    bool IsAutoRetryEnabled() const;

    /**
     * Set maximum retry attempts
     * @param MaxAttempts - Maximum retry attempts
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    void SetMaxRetryAttempts(int32 MaxAttempts);

    /**
     * Get maximum retry attempts
     * @return Maximum retry attempts
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    int32 GetMaxRetryAttempts() const;

    /**
     * Get transaction confirmation count
     * @param TransactionHash - Transaction hash
     * @return Number of confirmations
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    int32 GetTransactionConfirmations(const FString& TransactionHash) const;

    /**
     * Check if transaction is confirmed
     * @param TransactionHash - Transaction hash
     * @param RequiredConfirmations - Required confirmation count
     * @return True if confirmed
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    bool IsTransactionConfirmed(const FString& TransactionHash, int32 RequiredConfirmations = 12) const;

    /**
     * Get transaction processing time
     * @param TransactionHash - Transaction hash
     * @return Processing time in seconds
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    float GetTransactionProcessingTime(const FString& TransactionHash) const;

    /**
     * Export transaction data
     * @param TransactionHash - Transaction hash
     * @param Format - Export format (JSON, CSV)
     * @return Exported data string
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FString ExportTransaction(const FString& TransactionHash, const FString& Format = TEXT("JSON")) const;

    /**
     * Import transaction data
     * @param Data - Transaction data string
     * @param Format - Import format (JSON, CSV)
     * @return True if imported successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    bool ImportTransaction(const FString& Data, const FString& Format = TEXT("JSON"));

    /**
     * Validate transaction data
     * @param Transaction - Transaction to validate
     * @return True if valid
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    bool ValidateTransaction(const FBlockchainTransaction& Transaction) const;

    /**
     * Get transaction error details
     * @param TransactionHash - Transaction hash
     * @return Error message
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Transaction")
    FString GetTransactionError(const FString& TransactionHash) const;

public:
    /** Delegate fired when a transaction is created */
    UPROPERTY(BlueprintAssignable)
    FOnTransactionCreated OnTransactionCreated;

    /** Delegate fired when a transaction is confirmed */
    UPROPERTY(BlueprintAssignable)
    FOnTransactionConfirmed OnTransactionConfirmed;

    /** Delegate fired when a transaction fails */
    UPROPERTY(BlueprintAssignable)
    FOnTransactionFailed OnTransactionFailed;

    /** Delegate fired when a batch is processed */
    UPROPERTY(BlueprintAssignable)
    FOnBatchProcessed OnBatchProcessed;

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

    /** Pending transactions */
    UPROPERTY()
    TArray<FBlockchainTransaction> PendingTransactions;

    /** Transaction history */
    UPROPERTY()
    TArray<FBlockchainTransaction> TransactionHistory;

    /** Transaction batches */
    UPROPERTY()
    TMap<FString, FTransactionBatch> TransactionBatches;

    /** Transaction statistics per player */
    UPROPERTY()
    TMap<FString, FTransactionStatistics> PlayerStatistics;

    /** Current transaction fee type */
    UPROPERTY(EditAnywhere, Category = "Transaction|Configuration")
    ETransactionFeeType CurrentFeeType;

    /** Whether auto-retry is enabled */
    UPROPERTY(EditAnywhere, Category = "Transaction|Configuration")
    bool bAutoRetryEnabled;

    /** Maximum retry attempts */
    UPROPERTY(EditAnywhere, Category = "Transaction|Configuration")
    int32 MaxRetryAttempts;

    /** Default gas price */
    UPROPERTY(EditAnywhere, Category = "Transaction|Configuration")
    float DefaultGasPrice;

    /** Maximum pending transactions */
    UPROPERTY(EditAnywhere, Category = "Transaction|Configuration")
    int32 MaxPendingTransactions;

    /** Transaction timeout in seconds */
    UPROPERTY(EditAnywhere, Category = "Transaction|Configuration")
    float TransactionTimeout;

    /** Time since last gas price update */
    float TimeSinceLastGasUpdate;

    /** Gas price update interval */
    UPROPERTY(EditAnywhere, Category = "Transaction|Configuration")
    float GasUpdateInterval;

    /** Initialize transaction configuration */
    void InitializeTransactionConfiguration();

    /** Generate unique transaction hash */
    FString GenerateTransactionHash() const;

    /** Generate unique batch ID */
    FString GenerateBatchID() const;

    /** Calculate optimal gas price */
    float CalculateOptimalGasPrice(ETransactionPriority Priority) const;

    /** Calculate gas limit for transaction */
    int32 CalculateGasLimit(const FBlockchainTransaction& Transaction) const;

    /** Process transaction on blockchain */
    bool ProcessTransactionOnBlockchain(FBlockchainTransaction& Transaction);

    /** Update transaction status */
    void UpdateTransactionStatus(const FString& TransactionHash, ETransactionStatus NewStatus, const FString& ErrorMessage = TEXT(""));

    /** Update transaction confirmations */
    void UpdateTransactionConfirmations(const FString& TransactionHash, int32 NewConfirmations);

    /** Process transaction batch */
    void ProcessBatchTransactions(FTransactionBatch& Batch);

    /** Handle transaction timeout */
    void HandleTransactionTimeout(const FString& TransactionHash);

    /** Retry failed transaction */
    void RetryFailedTransaction(const FBlockchainTransaction& Transaction);

    /** Update gas prices */
    void UpdateGasPrices();

    /** Get gas price from network */
    float GetNetworkGasPrice() const;

    /** Update transaction statistics */
    void UpdateTransactionStatistics(const FString& PlayerID, const FBlockchainTransaction& Transaction);

    /** Clean up old transactions */
    void CleanupOldTransactions();

    /** Save transaction data */
    void SaveTransactionData();

    /** Load transaction data */
    void LoadTransactionData();

    /** Validate transaction parameters */
    bool ValidateTransactionParameters(const FString& FromAddress, const FString& ToAddress, const FString& TokenContract, float Amount) const;

    /** Get transaction fee multiplier */
    float GetFeeMultiplier(ETransactionPriority Priority) const;

    /** Format transaction for export */
    FString FormatTransactionForExport(const FBlockchainTransaction& Transaction, const FString& Format) const;

    /** Parse transaction from import */
    FBlockchainTransaction ParseTransactionFromImport(const FString& Data, const FString& Format) const;

    /** Get transaction metadata */
    FString GetTransactionMetadata(const FBlockchainTransaction& Transaction) const;

    /** Check if address is contract */
    bool IsContractAddress(const FString& Address) const;

    /** Get transaction type */
    FString GetTransactionType(const FBlockchainTransaction& Transaction) const;
};
