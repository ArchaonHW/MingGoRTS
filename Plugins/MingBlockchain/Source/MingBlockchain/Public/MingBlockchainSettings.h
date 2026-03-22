#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MingBlockchainSettings.generated.h"

/**
 * Blockchain network types
 */
UENUM(BlueprintType)
enum class EBlockchainNetwork : uint8
{
    EthereumMainnet  = 0,
    EthereumTestnet  = 1,
    PolygonMainnet   = 2,
    PolygonTestnet   = 3,
    BSCMainnet       = 4,
    BSCTestnet       = 5,
    LocalNetwork     = 6,
    CustomNetwork    = 7
};

/**
 * Gas price strategies
 */
UENUM(BlueprintType)
enum class EGasPriceStrategy : uint8
{
    Slow       = 0,
    Standard   = 1,
    Fast       = 2,
    Instant    = 3,
    Custom     = 4
};

/**
 * Security levels
 */
UENUM(BlueprintType)
enum class ESecurityLevel : uint8
{
    Low        = 0,
    Medium     = 1,
    High       = 2,
    Maximum    = 3
};

/**
 * Blockchain configuration settings
 */
USTRUCT(BlueprintType)
struct FBlockchainConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
    EBlockchainNetwork DefaultNetwork;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
    FString CustomNetworkRPC;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Network")
    int32 DefaultChainID;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gas")
    EGasPriceStrategy GasPriceStrategy;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gas")
    float CustomGasPrice;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gas")
    float MaxGasPrice;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Security")
    ESecurityLevel SecurityLevel;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mining")
    bool bMiningEnabled;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mining")
    float DefaultMiningDifficulty;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mining")
    int32 MaxConcurrentMiningSessions;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayToEarn")
    bool bPlayToEarnEnabled;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayToEarn")
    float GlobalRewardMultiplier;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayToEarn")
    int32 MaxEventsPerDay;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NFT")
    bool bNFTSystemEnabled;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NFT")
    float MarketplaceFeePercentage;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "NFT")
    int32 MaxMarketplaceListings;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transaction")
    bool bAutoRetryEnabled;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transaction")
    int32 MaxRetryAttempts;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Transaction")
    float TransactionTimeout;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wallet")
    bool bAutoConnectWallet;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wallet")
    FString DefaultWalletProvider;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
    bool bDebugMode;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
    bool bEnableVerboseLogging;

    FBlockchainConfig()
    {
        DefaultNetwork = EBlockchainNetwork::EthereumMainnet;
        CustomNetworkRPC = TEXT("http://localhost:8545");
        DefaultChainID = 1;
        GasPriceStrategy = EGasPriceStrategy::Standard;
        CustomGasPrice = 20.0f;
        MaxGasPrice = 1000.0f;
        SecurityLevel = ESecurityLevel::Medium;
        bMiningEnabled = true;
        DefaultMiningDifficulty = 1.0f;
        MaxConcurrentMiningSessions = 1;
        bPlayToEarnEnabled = true;
        GlobalRewardMultiplier = 1.0f;
        MaxEventsPerDay = 100;
        bNFTSystemEnabled = true;
        MarketplaceFeePercentage = 0.025f;
        MaxMarketplaceListings = 1000;
        bAutoRetryEnabled = true;
        MaxRetryAttempts = 3;
        TransactionTimeout = 300.0f;
        bAutoConnectWallet = false;
        DefaultWalletProvider = TEXT("MetaMask");
        bDebugMode = false;
        bEnableVerboseLogging = false;
    }
};

/**
 * Network configuration
 */
USTRUCT(BlueprintType)
struct FNetworkConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EBlockchainNetwork NetworkType;

    UPROPERTY(BlueprintReadOnly)
    FString NetworkName;

    UPROPERTY(BlueprintReadOnly)
    FString RPCURL;

    UPROPERTY(BlueprintReadOnly)
    int32 ChainID;

    UPROPERTY(BlueprintReadOnly)
    FString CurrencySymbol;

    UPROPERTY(BlueprintReadOnly)
    FString BlockExplorerURL;

    UPROPERTY(BlueprintReadOnly)
    bool IsTestnet;

    FNetworkConfig()
    {
        NetworkType = EBlockchainNetwork::EthereumMainnet;
        NetworkName = TEXT("");
        RPCURL = TEXT("");
        ChainID = 1;
        CurrencySymbol = TEXT("ETH");
        BlockExplorerURL = TEXT("");
        IsTestnet = false;
    }
};

/**
 * MingBlockchainSettings
 * 
 * Central configuration system for all blockchain-related settings in MingGoRTS.
 * Manages network configuration, security settings, mining parameters, and user preferences.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(MingBlockchain), meta=(BlueprintSpawnableComponent))
class MINGBLOCKCHAIN_API UMingBlockchainSettings : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingBlockchainSettings();

    /**
     * Initialize blockchain settings
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void InitializeSettings();

    /**
     * Get current blockchain configuration
     * @return Current configuration
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    FBlockchainConfig GetBlockchainConfig() const;

    /**
     * Update blockchain configuration
     * @param NewConfig - New configuration
     * @return True if updated successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool UpdateBlockchainConfig(const FBlockchainConfig& NewConfig);

    /**
     * Get network configuration
     * @param Network - Network type
     * @return Network configuration
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    FNetworkConfig GetNetworkConfig(EBlockchainNetwork Network) const;

    /**
     * Set default network
     * @param Network - Default network
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void SetDefaultNetwork(EBlockchainNetwork Network);

    /**
     * Get current network
     * @return Current network
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    EBlockchainNetwork GetCurrentNetwork() const;

    /**
     * Set gas price strategy
     * @param Strategy - Gas price strategy
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void SetGasPriceStrategy(EGasPriceStrategy Strategy);

    /**
     * Get gas price strategy
     * @return Current gas price strategy
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    EGasPriceStrategy GetGasPriceStrategy() const;

    /**
     * Set security level
     * @param Level - Security level
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void SetSecurityLevel(ESecurityLevel Level);

    /**
     * Get security level
     * @return Current security level
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    ESecurityLevel GetSecurityLevel() const;

    /**
     * Enable/disable mining
     * @param bEnabled - Whether to enable mining
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void SetMiningEnabled(bool bEnabled);

    /**
     * Check if mining is enabled
     * @return True if mining is enabled
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool IsMiningEnabled() const;

    /**
     * Enable/disable play-to-earn
     * @param bEnabled - Whether to enable play-to-earn
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void SetPlayToEarnEnabled(bool bEnabled);

    /**
     * Check if play-to-earn is enabled
     * @return True if play-to-earn is enabled
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool IsPlayToEarnEnabled() const;

    /**
     * Enable/disable NFT system
     * @param bEnabled - Whether to enable NFT system
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void SetNFTSystemEnabled(bool bEnabled);

    /**
     * Check if NFT system is enabled
     * @return True if NFT system is enabled
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool IsNFTSystemEnabled() const;

    /**
     * Get supported networks
     * @return Array of supported networks
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    TArray<EBlockchainNetwork> GetSupportedNetworks() const;

    /**
     * Get network name
     * @param Network - Network type
     * @return Network name
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    FString GetNetworkName(EBlockchainNetwork Network) const;

    /**
     * Get network RPC URL
     * @param Network - Network type
     * @return RPC URL
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    FString GetNetworkRPCURL(EBlockchainNetwork Network) const;

    /**
     * Validate configuration
     * @param Config - Configuration to validate
     * @return True if valid
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool ValidateConfig(const FBlockchainConfig& Config) const;

    /**
     * Reset to default settings
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void ResetToDefaults();

    /**
     * Save settings to file
     * @return True if saved successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool SaveSettings();

    /**
     * Load settings from file
     * @return True if loaded successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool LoadSettings();

    /**
     * Export settings to JSON
     * @return JSON string
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    FString ExportSettings() const;

    /**
     * Import settings from JSON
     * @param JSONString - JSON string
     * @return True if imported successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool ImportSettings(const FString& JSONString);

    /**
     * Get settings file path
     * @return File path
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    FString GetSettingsFilePath() const;

    /**
     * Check if debug mode is enabled
     * @return True if debug mode is enabled
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    bool IsDebugModeEnabled() const;

    /**
     * Enable/disable debug mode
     * @param bEnabled - Whether to enable debug mode
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    void SetDebugModeEnabled(bool bEnabled);

    /**
     * Get current configuration version
     * @return Configuration version
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|Settings")
    FString GetConfigVersion() const;

protected:
    /** Called when the component begins play */
    virtual void BeginPlay() override;

    /** Called when the component ends play */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    /** Current blockchain configuration */
    UPROPERTY()
    FBlockchainConfig CurrentConfig;

    /** Network configurations */
    UPROPERTY()
    TMap<EBlockchainNetwork, FNetworkConfig> NetworkConfigs;

    /** Configuration version */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FString ConfigVersion;

    /** Settings file name */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    FString SettingsFileName;

    /** Whether settings have been modified */
    bool bSettingsModified;

    /** Initialize network configurations */
    void InitializeNetworkConfigs();

    /** Get default configuration */
    FBlockchainConfig GetDefaultConfig() const;

    /** Validate network configuration */
    bool ValidateNetworkConfig(const FNetworkConfig& Config) const;

    /** Apply configuration changes */
    void ApplyConfigurationChanges();

    /** Notify configuration changed */
    void NotifyConfigurationChanged();

    /** Save configuration to file */
    bool SaveConfigurationToFile();

    /** Load configuration from file */
    bool LoadConfigurationFromFile();

    /** Convert configuration to JSON */
    FString ConfigToJSON(const FBlockchainConfig& Config) const;

    /** Parse configuration from JSON */
    FBlockchainConfig ConfigFromJSON(const FString& JSONString) const;

    /** Get network configuration key */
    FString GetNetworkConfigKey(EBlockchainNetwork Network) const;

    /** Validate RPC URL */
    bool ValidateRPCURL(const FString& URL) const;

    /** Get security level requirements */
    TArray<FString> GetSecurityLevelRequirements(ESecurityLevel Level) const;

    /** Check if configuration meets security requirements */
    bool MeetsSecurityRequirements(const FBlockchainConfig& Config, ESecurityLevel Level) const;
};
