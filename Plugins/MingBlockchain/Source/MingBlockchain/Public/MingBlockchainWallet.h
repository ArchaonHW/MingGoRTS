#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "MingBlockchainMiningSystem.h"
#include "MingBlockchainWallet.generated.h"

/**
 * Wallet connection status
 */
UENUM(BlueprintType)
enum class EWalletConnectionStatus: uint8 {
    Disconnected    = 0,
    Connecting      = 1,
    Connected       = 2,
    Error           = 3,
    Syncing         = 4
};

/**
 * Transaction status
 */
UENUM(BlueprintType)
enum class ETransactionStatus: uint8 {
    Pending         = 0,
    Confirmed       = 1,
    Failed          = 2,
    Cancelled       = 3,
    Expired         = 4
};

/**
 * Wallet balance information
 */
USTRUCT(BlueprintType)
struct FWalletBalance
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TokenSymbol;

    UPROPERTY(BlueprintReadOnly)
    FString TokenAddress;

    UPROPERTY(BlueprintReadOnly)
    float Balance;

    UPROPERTY(BlueprintReadOnly)
    float UsdValue;

    UPROPERTY(BlueprintReadOnly)
    FString TokenName;

    UPROPERTY(BlueprintReadOnly)
    int32 Decimals;

    FWalletBalance()
    {
        TokenSymbol = TEXT("");
        TokenAddress = TEXT("");
        Balance = 0.0f;
        UsdValue = 0.0f;
        TokenName = TEXT("");
        Decimals = 18;
    }
};

/**
 * Transaction record
 */
USTRUCT(BlueprintType)
struct FWalletTransaction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TransactionID;

    UPROPERTY(BlueprintReadOnly)
    FString FromAddress;

    UPROPERTY(BlueprintReadOnly)
    FString ToAddress;

    UPROPERTY(BlueprintReadOnly)
    FString TokenSymbol;

    UPROPERTY(BlueprintReadOnly)
    float Amount;

    UPROPERTY(BlueprintReadOnly)
    float GasFee;

    UPROPERTY(BlueprintReadOnly)
    ETransactionStatus Status;

    UPROPERTY(BlueprintReadOnly)
    FString BlockHash;

    UPROPERTY(BlueprintReadOnly)
    int32 BlockNumber;

    UPROPERTY(BlueprintReadOnly)
    FString Timestamp;

    UPROPERTY(BlueprintReadOnly)
    FString Metadata;

    FWalletTransaction()
    {
        TransactionID = TEXT("");
        FromAddress = TEXT("");
        ToAddress = TEXT("");
        TokenSymbol = TEXT("");
        Amount = 0.0f;
        GasFee = 0.0f;
        Status = ETransactionStatus::Pending;
        BlockHash = TEXT("");
        BlockNumber = 0;
        Timestamp = TEXT("");
        Metadata = TEXT("");
    }
};

/**
 * Wallet information
 */
USTRUCT(BlueprintType)
struct FWalletInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString WalletAddress;

    UPROPERTY(BlueprintReadOnly)
    FString WalletName;

    UPROPERTY(BlueprintReadOnly)
    EWalletConnectionStatus ConnectionStatus;

    UPROPERTY(BlueprintReadOnly)
    FString NetworkName;

    UPROPERTY(BlueprintReadOnly)
    FString ChainID;

    UPROPERTY(BlueprintReadOnly)
    TArray<FWalletBalance> Balances;

    UPROPERTY(BlueprintReadOnly)
    int32 TransactionCount;

    FWalletInfo()
    {
        WalletAddress = TEXT("");
        WalletName = TEXT("");
        ConnectionStatus = EWalletConnectionStatus::Disconnected;
        NetworkName = TEXT("");
        ChainID = TEXT("");
        TransactionCount = 0;
    }
};

// Forward declarations
class UMingBlockchainTransactionManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWalletConnected, const FWalletInfo&, WalletInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWalletDisconnected, const FString&, WalletAddress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBalanceChanged, const FString&, TokenSymbol, float, NewBalance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransactionReceived, const FWalletTransaction&, Transaction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTransactionSent, const FWalletTransaction&, Transaction);

/**
 * MingBlockchainWallet
 * 
 * Cryptocurrency wallet system for MingGoRTS that manages player digital assets,
 * including tokens, NFTs, and transaction history. Integrates with various
 * blockchain networks and wallet providers.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(MingBlockchain), meta=(BlueprintSpawnableComponent))
class MINGBLOCKCHAIN_API UMingBlockchainWallet : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingBlockchainWallet();

    /**
     * Initialize the wallet system
     * @param TransactionManager - Reference to transaction manager
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    void InitializeWallet(UMingBlockchainTransactionManager* TransactionManager);

    /**
     * Connect to a wallet provider
     * @param ProviderName - Name of the wallet provider (e.g., "MetaMask", "WalletConnect")
     * @return True if connection initiated successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool ConnectWallet(const FString& ProviderName);

    /**
     * Disconnect from current wallet
     * @return True if disconnected successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool DisconnectWallet();

    /**
     * Get current wallet information
     * @return Wallet information structure
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    FWalletInfo GetWalletInfo() const;

    /**
     * Get wallet balance for specific token
     * @param TokenAddress - Token contract address
     * @return Token balance
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    float GetTokenBalance(const FString& TokenAddress) const;

    /**
     * Get all wallet balances
     * @return Array of token balances
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    TArray<FWalletBalance> GetAllBalances() const;

    /**
     * Send tokens to another address
     * @param ToAddress - Recipient address
     * @param TokenAddress - Token contract address
     * @param Amount - Amount to send
     * @return Transaction ID if successful, empty string otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    FString SendTokens(const FString& ToAddress, const FString& TokenAddress, float Amount);

    /**
     * Receive a mining reward
     * @param Reward - Mining reward to receive
     * @return True if reward received successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool ReceiveReward(const FMiningReward& Reward);

    /**
     * Get transaction history
     * @param Limit - Maximum number of transactions to return
     * @return Array of transactions
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    TArray<FWalletTransaction> GetTransactionHistory(int32 Limit = 50) const;

    /**
     * Get transaction by ID
     * @param TransactionID - Transaction identifier
     * @return Transaction data if found
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    FWalletTransaction GetTransaction(const FString& TransactionID) const;

    /**
     * Get wallet connection status
     * @return Current connection status
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    EWalletConnectionStatus GetConnectionStatus() const;

    /**
     * Check if wallet is connected
     * @return True if connected
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool IsWalletConnected() const;

    /**
     * Get current network information
     * @return Network name and chain ID
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    FString GetNetworkInfo() const;

    /**
     * Switch to different network
     * @param NetworkName - Name of the network to switch to
     * @return True if network switch initiated
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool SwitchNetwork(const FString& NetworkName);

    /**
     * Import wallet from private key or seed phrase
     * @param PrivateKeyOrSeed - Private key or seed phrase
     * @return True if import successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool ImportWallet(const FString& PrivateKeyOrSeed);

    /**
     * Export wallet private key (with user confirmation)
     * @return Private key if confirmed, empty string otherwise
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    FString ExportPrivateKey();

    /**
     * Create a new wallet
     * @return New wallet address if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    FString CreateNewWallet();

    /**
     * Validate wallet address
     * @param Address - Address to validate
     * @return True if address is valid
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool ValidateAddress(const FString& Address) const;

    /**
     * Estimate gas fee for transaction
     * @param ToAddress - Recipient address
     * @param TokenAddress - Token contract address
     * @param Amount - Amount to send
     * @return Estimated gas fee
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    float EstimateGasFee(const FString& ToAddress, const FString& TokenAddress, float Amount) const;

    /**
     * Get supported networks
     * @return Array of supported network names
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    TArray<FString> GetSupportedNetworks() const;

    /**
     * Refresh wallet data
     * @return True if refresh initiated
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    bool RefreshWalletData();

    /**
     * Get wallet backup phrase
     * @return Seed phrase if available
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Wallet")
    FString GetBackupPhrase() const;

public:
    /** Delegate fired when wallet is connected */
    UPROPERTY(BlueprintAssignable)
    FOnWalletConnected OnWalletConnected;

    /** Delegate fired when wallet is disconnected */
    UPROPERTY(BlueprintAssignable)
    FOnWalletDisconnected OnWalletDisconnected;

    /** Delegate fired when balance changes */
    UPROPERTY(BlueprintAssignable)
    FOnBalanceChanged OnBalanceChanged;

    /** Delegate fired when transaction is received */
    UPROPERTY(BlueprintAssignable)
    FOnTransactionReceived OnTransactionReceived;

    /** Delegate fired when transaction is sent */
    UPROPERTY(BlueprintAssignable)
    FOnTransactionSent OnTransactionSent;

protected:
    /** Called when the component begins play */
    virtual void BeginPlay() override;

    /** Called when the component ends play */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    /** Reference to transaction manager */
    UPROPERTY()
    TObjectPtr<UMingBlockchainTransactionManager> TransactionManager;

    /** Current wallet information */
    UPROPERTY()
    FWalletInfo CurrentWalletInfo;

    /** Transaction history */
    UPROPERTY()
    TArray<FWalletTransaction> TransactionHistory;

    /** Current balances */
    UPROPERTY()
    TArray<FWalletBalance> CurrentBalances;

    /** Connected wallet provider */
    UPROPERTY()
    FString ConnectedProvider;

    /** Current network configuration */
    UPROPERTY(EditAnywhere, Category = "Wallet|Configuration")
    FString CurrentNetwork;

    /** Whether auto-refresh is enabled */
    UPROPERTY(EditAnywhere, Category = "Wallet|Configuration")
    bool bAutoRefreshEnabled;

    /** Auto-refresh interval in seconds */
    UPROPERTY(EditAnywhere, Category = "Wallet|Configuration")
    float AutoRefreshInterval;

    /** Maximum transaction history */
    UPROPERTY(EditAnywhere, Category = "Wallet|Configuration")
    int32 MaxTransactionHistory;

    /** Time since last refresh */
    float TimeSinceLastRefresh;

    /** Initialize wallet configuration */
    void InitializeWalletConfiguration();

    /** Update wallet balances */
    void UpdateBalances();

    /** Update transaction history */
    void UpdateTransactionHistory();

    /** Process wallet connection */
    void ProcessWalletConnection();

    /** Handle wallet disconnection */
    void HandleWalletDisconnection();

    /** Validate wallet configuration */
    bool ValidateWalletConfiguration() const;

    /** Generate new wallet address */
    FString GenerateWalletAddress() const;

    /** Generate backup phrase */
    FString GenerateBackupPhrase() const;

    /** Encrypt sensitive data */
    FString EncryptData(const FString& Data) const;

    /** Decrypt sensitive data */
    FString DecryptData(const FString& EncryptedData) const;

    /** Save wallet data to local storage */
    void SaveWalletData();

    /** Load wallet data from local storage */
    void LoadWalletData();

    /** Clear sensitive data */
    void ClearSensitiveData();

    /** Format wallet address for display */
    FString FormatAddress(const FString& Address) const;

    /** Get token decimals */
    int32 GetTokenDecimals(const FString& TokenAddress) const;

    /** Convert token amount to wei */
    int64 ConvertToWei(float Amount, int32 Decimals) const;

    /** Convert wei to token amount */
    float ConvertFromWei(int64 Wei, int32 Decimals) const;
};
