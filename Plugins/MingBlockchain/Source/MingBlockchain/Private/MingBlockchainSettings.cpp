#include "MingBlockchainSettings.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingBlockchainSettings::UMingBlockchainSettings()
{
    ConfigVersion = TEXT("1.0.0");
    SettingsFileName = TEXT("BlockchainSettings.json");
    bSettingsModified = false;
}

void UMingBlockchainSettings::BeginPlay()
{
    Super::BeginPlay();
    InitializeSettings();
}

void UMingBlockchainSettings::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (bSettingsModified)
    {
        SaveSettings();
    }
    
    Super::EndPlay(EndPlayReason);
}

void UMingBlockchainSettings::InitializeSettings()
{
    InitializeNetworkConfigs();
    
    if (!LoadSettings())
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainSettings: Could not load settings, using defaults"));
        CurrentConfig = GetDefaultConfig();
        SaveSettings();
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Settings initialized"));
}

FBlockchainConfig UMingBlockchainSettings::GetBlockchainConfig() const
{
    return CurrentConfig;
}

bool UMingBlockchainSettings::UpdateBlockchainConfig(const FBlockchainConfig& NewConfig)
{
    if (!ValidateConfig(NewConfig))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Invalid configuration"));
        return false;
    }

    CurrentConfig = NewConfig;
    bSettingsModified = true;
    ApplyConfigurationChanges();
    NotifyConfigurationChanged();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Configuration updated"));
    return true;
}

FNetworkConfig UMingBlockchainSettings::GetNetworkConfig(EBlockchainNetwork Network) const
{
    if (const FNetworkConfig* Config = NetworkConfigs.Find(Network))
    {
        return *Config;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainSettings: Network %d not found"), (int32)Network);
    return FNetworkConfig();
}

void UMingBlockchainSettings::SetDefaultNetwork(EBlockchainNetwork Network)
{
    CurrentConfig.DefaultNetwork = Network;
    bSettingsModified = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Default network set to %d"), (int32)Network);
}

EBlockchainNetwork UMingBlockchainSettings::GetCurrentNetwork() const
{
    return CurrentConfig.DefaultNetwork;
}

void UMingBlockchainSettings::SetGasPriceStrategy(EGasPriceStrategy Strategy)
{
    CurrentConfig.GasPriceStrategy = Strategy;
    bSettingsModified = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Gas price strategy set to %d"), (int32)Strategy);
}

EGasPriceStrategy UMingBlockchainSettings::GetGasPriceStrategy() const
{
    return CurrentConfig.GasPriceStrategy;
}

void UMingBlockchainSettings::SetSecurityLevel(ESecurityLevel Level)
{
    CurrentConfig.SecurityLevel = Level;
    bSettingsModified = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Security level set to %d"), (int32)Level);
}

ESecurityLevel UMingBlockchainSettings::GetSecurityLevel() const
{
    return CurrentConfig.SecurityLevel;
}

void UMingBlockchainSettings::SetMiningEnabled(bool bEnabled)
{
    CurrentConfig.bMiningEnabled = bEnabled;
    bSettingsModified = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Mining %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingBlockchainSettings::IsMiningEnabled() const
{
    return CurrentConfig.bMiningEnabled;
}

void UMingBlockchainSettings::SetPlayToEarnEnabled(bool bEnabled)
{
    CurrentConfig.bPlayToEarnEnabled = bEnabled;
    bSettingsModified = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Play-to-earn %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingBlockchainSettings::IsPlayToEarnEnabled() const
{
    return CurrentConfig.bPlayToEarnEnabled;
}

void UMingBlockchainSettings::SetNFTSystemEnabled(bool bEnabled)
{
    CurrentConfig.bNFTSystemEnabled = bEnabled;
    bSettingsModified = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: NFT system %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingBlockchainSettings::IsNFTSystemEnabled() const
{
    return CurrentConfig.bNFTSystemEnabled;
}

TArray<EBlockchainNetwork> UMingBlockchainSettings::GetSupportedNetworks() const
{
    TArray<EBlockchainNetwork> Networks;
    Networks.Add(EBlockchainNetwork::EthereumMainnet);
    Networks.Add(EBlockchainNetwork::EthereumTestnet);
    Networks.Add(EBlockchainNetwork::PolygonMainnet);
    Networks.Add(EBlockchainNetwork::PolygonTestnet);
    Networks.Add(EBlockchainNetwork::BSCMainnet);
    Networks.Add(EBlockchainNetwork::BSCTestnet);
    Networks.Add(EBlockchainNetwork::LocalNetwork);
    Networks.Add(EBlockchainNetwork::CustomNetwork);
    
    return Networks;
}

FString UMingBlockchainSettings::GetNetworkName(EBlockchainNetwork Network) const
{
    FNetworkConfig Config = GetNetworkConfig(Network);
    return Config.NetworkName;
}

FString UMingBlockchainSettings::GetNetworkRPCURL(EBlockchainNetwork Network) const
{
    FNetworkConfig Config = GetNetworkConfig(Network);
    return Config.RPCURL;
}

bool UMingBlockchainSettings::ValidateConfig(const FBlockchainConfig& Config) const
{
    // Validate network configuration
    if (!ValidateNetworkConfig(GetNetworkConfig(Config.DefaultNetwork)))
    {
        return false;
    }

    // Validate gas settings
    if (Config.CustomGasPrice <= 0.0f || Config.MaxGasPrice <= 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Invalid gas prices"));
        return false;
    }

    if (Config.CustomGasPrice > Config.MaxGasPrice)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Custom gas price exceeds maximum"));
        return false;
    }

    // Validate mining settings
    if (Config.MaxConcurrentMiningSessions <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Invalid max concurrent mining sessions"));
        return false;
    }

    // Validate play-to-earn settings
    if (Config.MaxEventsPerDay <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Invalid max events per day"));
        return false;
    }

    // Validate NFT settings
    if (Config.MarketplaceFeePercentage < 0.0f || Config.MarketplaceFeePercentage > 1.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Invalid marketplace fee percentage"));
        return false;
    }

    // Validate transaction settings
    if (Config.MaxRetryAttempts < 0 || Config.TransactionTimeout <= 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Invalid transaction settings"));
        return false;
    }

    // Check security requirements
    if (!MeetsSecurityRequirements(Config, Config.SecurityLevel))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Configuration does not meet security requirements"));
        return false;
    }

    return true;
}

void UMingBlockchainSettings::ResetToDefaults()
{
    CurrentConfig = GetDefaultConfig();
    bSettingsModified = true;
    ApplyConfigurationChanges();
    NotifyConfigurationChanged();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Reset to default settings"));
}

bool UMingBlockchainSettings::SaveSettings()
{
    if (SaveConfigurationToFile())
    {
        bSettingsModified = false;
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Settings saved successfully"));
        return true;
    }

    UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Failed to save settings"));
    return false;
}

bool UMingBlockchainSettings::LoadSettings()
{
    if (LoadConfigurationFromFile())
    {
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Settings loaded successfully"));
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainSettings: Could not load settings"));
    return false;
}

FString UMingBlockchainSettings::ExportSettings() const
{
    return ConfigToJSON(CurrentConfig);
}

bool UMingBlockchainSettings::ImportSettings(const FString& JSONString)
{
    FBlockchainConfig ImportedConfig = ConfigFromJSON(JSONString);
    
    if (ValidateConfig(ImportedConfig))
    {
        CurrentConfig = ImportedConfig;
        bSettingsModified = true;
        ApplyConfigurationChanges();
        NotifyConfigurationChanged();

        UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Settings imported successfully"));
        return true;
    }

    UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Failed to import settings - invalid configuration"));
    return false;
}

FString UMingBlockchainSettings::GetSettingsFilePath() const
{
    return FPaths::ProjectSavedDir() / SettingsFileName;
}

bool UMingBlockchainSettings::IsDebugModeEnabled() const
{
    return CurrentConfig.bDebugMode;
}

void UMingBlockchainSettings::SetDebugModeEnabled(bool bEnabled)
{
    CurrentConfig.bDebugMode = bEnabled;
    CurrentConfig.bEnableVerboseLogging = bEnabled;
    bSettingsModified = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Debug mode %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

FString UMingBlockchainSettings::GetConfigVersion() const
{
    return ConfigVersion;
}

void UMingBlockchainSettings::InitializeNetworkConfigs()
{
    // Ethereum Mainnet
    FNetworkConfig EthereumMainnet;
    EthereumMainnet.NetworkType = EBlockchainNetwork::EthereumMainnet;
    EthereumMainnet.NetworkName = TEXT("Ethereum Mainnet");
    EthereumMainnet.RPCURL = TEXT("https://mainnet.infura.io/v3/YOUR_PROJECT_ID");
    EthereumMainnet.ChainID = 1;
    EthereumMainnet.CurrencySymbol = TEXT("ETH");
    EthereumMainnet.BlockExplorerURL = TEXT("https://etherscan.io");
    EthereumMainnet.IsTestnet = false;
    NetworkConfigs.Add(EBlockchainNetwork::EthereumMainnet, EthereumMainnet);

    // Ethereum Testnet (Goerli)
    FNetworkConfig EthereumTestnet;
    EthereumTestnet.NetworkType = EBlockchainNetwork::EthereumTestnet;
    EthereumTestnet.NetworkName = TEXT("Ethereum Testnet");
    EthereumTestnet.RPCURL = TEXT("https://goerli.infura.io/v3/YOUR_PROJECT_ID");
    EthereumTestnet.ChainID = 5;
    EthereumTestnet.CurrencySymbol = TEXT("ETH");
    EthereumTestnet.BlockExplorerURL = TEXT("https://goerli.etherscan.io");
    EthereumTestnet.IsTestnet = true;
    NetworkConfigs.Add(EBlockchainNetwork::EthereumTestnet, EthereumTestnet);

    // Polygon Mainnet
    FNetworkConfig PolygonMainnet;
    PolygonMainnet.NetworkType = EBlockchainNetwork::PolygonMainnet;
    PolygonMainnet.NetworkName = TEXT("Polygon Mainnet");
    PolygonMainnet.RPCURL = TEXT("https://polygon-rpc.com");
    PolygonMainnet.ChainID = 137;
    PolygonMainnet.CurrencySymbol = TEXT("MATIC");
    PolygonMainnet.BlockExplorerURL = TEXT("https://polygonscan.com");
    PolygonMainnet.IsTestnet = false;
    NetworkConfigs.Add(EBlockchainNetwork::PolygonMainnet, PolygonMainnet);

    // Polygon Testnet (Mumbai)
    FNetworkConfig PolygonTestnet;
    PolygonTestnet.NetworkType = EBlockchainNetwork::PolygonTestnet;
    PolygonTestnet.NetworkName = TEXT("Polygon Testnet");
    PolygonTestnet.RPCURL = TEXT("https://rpc-mumbai.maticvigil.com");
    PolygonTestnet.ChainID = 80001;
    PolygonTestnet.CurrencySymbol = TEXT("MATIC");
    PolygonTestnet.BlockExplorerURL = TEXT("https://mumbai.polygonscan.com");
    PolygonTestnet.IsTestnet = true;
    NetworkConfigs.Add(EBlockchainNetwork::PolygonTestnet, PolygonTestnet);

    // BSC Mainnet
    FNetworkConfig BSCMainnet;
    BSCMainnet.NetworkType = EBlockchainNetwork::BSCMainnet;
    BSCMainnet.NetworkName = TEXT("BSC Mainnet");
    BSCMainnet.RPCURL = TEXT("https://bsc-dataseed.binance.org");
    BSCMainnet.ChainID = 56;
    BSCMainnet.CurrencySymbol = TEXT("BNB");
    BSCMainnet.BlockExplorerURL = TEXT("https://bscscan.com");
    BSCMainnet.IsTestnet = false;
    NetworkConfigs.Add(EBlockchainNetwork::BSCMainnet, BSCMainnet);

    // BSC Testnet
    FNetworkConfig BSCTestnet;
    BSCTestnet.NetworkType = EBlockchainNetwork::BSCTestnet;
    BSCTestnet.NetworkName = TEXT("BSC Testnet");
    BSCTestnet.RPCURL = TEXT("https://data-seed-prebsc-1-s1.binance.org:8545");
    BSCTestnet.ChainID = 97;
    BSCTestnet.CurrencySymbol = TEXT("BNB");
    BSCTestnet.BlockExplorerURL = TEXT("https://testnet.bscscan.com");
    BSCTestnet.IsTestnet = true;
    NetworkConfigs.Add(EBlockchainNetwork::BSCTestnet, BSCTestnet);

    // Local Network
    FNetworkConfig LocalNetwork;
    LocalNetwork.NetworkType = EBlockchainNetwork::LocalNetwork;
    LocalNetwork.NetworkName = TEXT("Local Network");
    LocalNetwork.RPCURL = TEXT("http://localhost:8545");
    LocalNetwork.ChainID = 1337;
    LocalNetwork.CurrencySymbol = TEXT("ETH");
    LocalNetwork.BlockExplorerURL = TEXT("");
    LocalNetwork.IsTestnet = true;
    NetworkConfigs.Add(EBlockchainNetwork::LocalNetwork, LocalNetwork);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Network configurations initialized"));
}

FBlockchainConfig UMingBlockchainSettings::GetDefaultConfig() const
{
    FBlockchainConfig DefaultConfig;
    DefaultConfig.DefaultNetwork = EBlockchainNetwork::EthereumMainnet;
    DefaultConfig.CustomNetworkRPC = TEXT("http://localhost:8545");
    DefaultConfig.DefaultChainID = 1;
    DefaultConfig.GasPriceStrategy = EGasPriceStrategy::Standard;
    DefaultConfig.CustomGasPrice = 20.0f;
    DefaultConfig.MaxGasPrice = 1000.0f;
    DefaultConfig.SecurityLevel = ESecurityLevel::Medium;
    DefaultConfig.bMiningEnabled = true;
    DefaultConfig.DefaultMiningDifficulty = 1.0f;
    DefaultConfig.MaxConcurrentMiningSessions = 1;
    DefaultConfig.bPlayToEarnEnabled = true;
    DefaultConfig.GlobalRewardMultiplier = 1.0f;
    DefaultConfig.MaxEventsPerDay = 100;
    DefaultConfig.bNFTSystemEnabled = true;
    DefaultConfig.MarketplaceFeePercentage = 0.025f;
    DefaultConfig.MaxMarketplaceListings = 1000;
    DefaultConfig.bAutoRetryEnabled = true;
    DefaultConfig.MaxRetryAttempts = 3;
    DefaultConfig.TransactionTimeout = 300.0f;
    DefaultConfig.bAutoConnectWallet = false;
    DefaultConfig.DefaultWalletProvider = TEXT("MetaMask");
    DefaultConfig.bDebugMode = false;
    DefaultConfig.bEnableVerboseLogging = false;

    return DefaultConfig;
}

bool UMingBlockchainSettings::ValidateNetworkConfig(const FNetworkConfig& Config) const
{
    return !Config.NetworkName.IsEmpty() &&
           !Config.RPCURL.IsEmpty() &&
           ValidateRPCURL(Config.RPCURL) &&
           Config.ChainID > 0 &&
           !Config.CurrencySymbol.IsEmpty();
}

void UMingBlockchainSettings::ApplyConfigurationChanges()
{
    // Apply configuration changes to all blockchain systems
    // This would typically broadcast configuration change events
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Applied configuration changes"));
}

void UMingBlockchainSettings::NotifyConfigurationChanged()
{
    // Notify other systems that configuration has changed
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Configuration changed notification sent"));
}

bool UMingBlockchainSettings::SaveConfigurationToFile()
{
    FString FilePath = GetSettingsFilePath();
    FString JSONString = ConfigToJSON(CurrentConfig);

    if (!FFileHelper::SaveStringToFile(JSONString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainSettings: Failed to save settings to %s"), *FilePath);
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Settings saved to %s"), *FilePath);
    return true;
}

bool UMingBlockchainSettings::LoadConfigurationFromFile()
{
    FString FilePath = GetSettingsFilePath();
    FString JSONString;

    if (!FFileHelper::LoadFileToString(JSONString, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainSettings: Could not load settings from %s"), *FilePath);
        return false;
    }

    CurrentConfig = ConfigFromJSON(JSONString);
    
    // Validate loaded configuration
    if (!ValidateConfig(CurrentConfig))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainSettings: Loaded configuration is invalid, using defaults"));
        CurrentConfig = GetDefaultConfig();
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainSettings: Settings loaded from %s"), *FilePath);
    return true;
}

FString UMingBlockchainSettings::ConfigToJSON(const FBlockchainConfig& Config) const
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    
    JsonObject->SetStringField(TEXT("version"), ConfigVersion);
    JsonObject->SetNumberField(TEXT("defaultNetwork"), (int32)Config.DefaultNetwork);
    JsonObject->SetStringField(TEXT("customNetworkRPC"), Config.CustomNetworkRPC);
    JsonObject->SetNumberField(TEXT("defaultChainID"), Config.DefaultChainID);
    JsonObject->SetNumberField(TEXT("gasPriceStrategy"), (int32)Config.GasPriceStrategy);
    JsonObject->SetNumberField(TEXT("customGasPrice"), Config.CustomGasPrice);
    JsonObject->SetNumberField(TEXT("maxGasPrice"), Config.MaxGasPrice);
    JsonObject->SetNumberField(TEXT("securityLevel"), (int32)Config.SecurityLevel);
    JsonObject->SetBoolField(TEXT("miningEnabled"), Config.bMiningEnabled);
    JsonObject->SetNumberField(TEXT("defaultMiningDifficulty"), Config.DefaultMiningDifficulty);
    JsonObject->SetNumberField(TEXT("maxConcurrentMiningSessions"), Config.MaxConcurrentMiningSessions);
    JsonObject->SetBoolField(TEXT("playToEarnEnabled"), Config.bPlayToEarnEnabled);
    JsonObject->SetNumberField(TEXT("globalRewardMultiplier"), Config.GlobalRewardMultiplier);
    JsonObject->SetNumberField(TEXT("maxEventsPerDay"), Config.MaxEventsPerDay);
    JsonObject->SetBoolField(TEXT("nftSystemEnabled"), Config.bNFTSystemEnabled);
    JsonObject->SetNumberField(TEXT("marketplaceFeePercentage"), Config.MarketplaceFeePercentage);
    JsonObject->SetNumberField(TEXT("maxMarketplaceListings"), Config.MaxMarketplaceListings);
    JsonObject->SetBoolField(TEXT("autoRetryEnabled"), Config.bAutoRetryEnabled);
    JsonObject->SetNumberField(TEXT("maxRetryAttempts"), Config.MaxRetryAttempts);
    JsonObject->SetNumberField(TEXT("transactionTimeout"), Config.TransactionTimeout);
    JsonObject->SetBoolField(TEXT("autoConnectWallet"), Config.bAutoConnectWallet);
    JsonObject->SetStringField(TEXT("defaultWalletProvider"), Config.DefaultWalletProvider);
    JsonObject->SetBoolField(TEXT("debugMode"), Config.bDebugMode);
    JsonObject->SetBoolField(TEXT("enableVerboseLogging"), Config.bEnableVerboseLogging);

    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return OutputString;
}

FBlockchainConfig UMingBlockchainSettings::ConfigFromJSON(const FString& JSONString) const
{
    FBlockchainConfig Config = GetDefaultConfig();

    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // Check version compatibility
        FString LoadedVersion = JsonObject->GetStringField(TEXT("version"));
        if (LoadedVersion != ConfigVersion)
        {
            UE_LOG(LogTemp, Warning, TEXT("MingBlockchainSettings: Configuration version mismatch (loaded: %s, expected: %s)"), 
                   *LoadedVersion, *ConfigVersion);
        }

        // Load configuration values
        if (JsonObject->HasField(TEXT("defaultNetwork")))
            Config.DefaultNetwork = (EBlockchainNetwork)JsonObject->GetIntegerField(TEXT("defaultNetwork"));
        
        if (JsonObject->HasField(TEXT("customNetworkRPC")))
            Config.CustomNetworkRPC = JsonObject->GetStringField(TEXT("customNetworkRPC"));
        
        if (JsonObject->HasField(TEXT("defaultChainID")))
            Config.DefaultChainID = JsonObject->GetIntegerField(TEXT("defaultChainID"));
        
        if (JsonObject->HasField(TEXT("gasPriceStrategy")))
            Config.GasPriceStrategy = (EGasPriceStrategy)JsonObject->GetIntegerField(TEXT("gasPriceStrategy"));
        
        if (JsonObject->HasField(TEXT("customGasPrice")))
            Config.CustomGasPrice = JsonObject->GetNumberField(TEXT("customGasPrice"));
        
        if (JsonObject->HasField(TEXT("maxGasPrice")))
            Config.MaxGasPrice = JsonObject->GetNumberField(TEXT("maxGasPrice"));
        
        if (JsonObject->HasField(TEXT("securityLevel")))
            Config.SecurityLevel = (ESecurityLevel)JsonObject->GetIntegerField(TEXT("securityLevel"));
        
        if (JsonObject->HasField(TEXT("miningEnabled")))
            Config.bMiningEnabled = JsonObject->GetBoolField(TEXT("miningEnabled"));
        
        if (JsonObject->HasField(TEXT("defaultMiningDifficulty")))
            Config.DefaultMiningDifficulty = JsonObject->GetNumberField(TEXT("defaultMiningDifficulty"));
        
        if (JsonObject->HasField(TEXT("maxConcurrentMiningSessions")))
            Config.MaxConcurrentMiningSessions = JsonObject->GetIntegerField(TEXT("maxConcurrentMiningSessions"));
        
        if (JsonObject->HasField(TEXT("playToEarnEnabled")))
            Config.bPlayToEarnEnabled = JsonObject->GetBoolField(TEXT("playToEarnEnabled"));
        
        if (JsonObject->HasField(TEXT("globalRewardMultiplier")))
            Config.GlobalRewardMultiplier = JsonObject->GetNumberField(TEXT("globalRewardMultiplier"));
        
        if (JsonObject->HasField(TEXT("maxEventsPerDay")))
            Config.MaxEventsPerDay = JsonObject->GetIntegerField(TEXT("maxEventsPerDay"));
        
        if (JsonObject->HasField(TEXT("nftSystemEnabled")))
            Config.bNFTSystemEnabled = JsonObject->GetBoolField(TEXT("nftSystemEnabled"));
        
        if (JsonObject->HasField(TEXT("marketplaceFeePercentage")))
            Config.MarketplaceFeePercentage = JsonObject->GetNumberField(TEXT("marketplaceFeePercentage"));
        
        if (JsonObject->HasField(TEXT("maxMarketplaceListings")))
            Config.MaxMarketplaceListings = JsonObject->GetIntegerField(TEXT("maxMarketplaceListings"));
        
        if (JsonObject->HasField(TEXT("autoRetryEnabled")))
            Config.bAutoRetryEnabled = JsonObject->GetBoolField(TEXT("autoRetryEnabled"));
        
        if (JsonObject->HasField(TEXT("maxRetryAttempts")))
            Config.MaxRetryAttempts = JsonObject->GetIntegerField(TEXT("maxRetryAttempts"));
        
        if (JsonObject->HasField(TEXT("transactionTimeout")))
            Config.TransactionTimeout = JsonObject->GetNumberField(TEXT("transactionTimeout"));
        
        if (JsonObject->HasField(TEXT("autoConnectWallet")))
            Config.bAutoConnectWallet = JsonObject->GetBoolField(TEXT("autoConnectWallet"));
        
        if (JsonObject->HasField(TEXT("defaultWalletProvider")))
            Config.DefaultWalletProvider = JsonObject->GetStringField(TEXT("defaultWalletProvider"));
        
        if (JsonObject->HasField(TEXT("debugMode")))
            Config.bDebugMode = JsonObject->GetBoolField(TEXT("debugMode"));
        
        if (JsonObject->HasField(TEXT("enableVerboseLogging")))
            Config.bEnableVerboseLogging = JsonObject->GetBoolField(TEXT("enableVerboseLogging"));
    }

    return Config;
}

FString UMingBlockchainSettings::GetNetworkConfigKey(EBlockchainNetwork Network) const
{
    switch (Network)
    {
        case EBlockchainNetwork::EthereumMainnet: return TEXT("ethereum_mainnet");
        case EBlockchainNetwork::EthereumTestnet: return TEXT("ethereum_testnet");
        case EBlockchainNetwork::PolygonMainnet:   return TEXT("polygon_mainnet");
        case EBlockchainNetwork::PolygonTestnet:   return TEXT("polygon_testnet");
        case EBlockchainNetwork::BSCMainnet:       return TEXT("bsc_mainnet");
        case EBlockchainNetwork::BSCTestnet:       return TEXT("bsc_testnet");
        case EBlockchainNetwork::LocalNetwork:     return TEXT("local_network");
        case EBlockchainNetwork::CustomNetwork:    return TEXT("custom_network");
        default: return TEXT("unknown");
    }
}

bool UMingBlockchainSettings::ValidateRPCURL(const FString& URL) const
{
    if (URL.IsEmpty())
    {
        return false;
    }

    // Basic URL validation
    if (URL.StartsWith(TEXT("http://")) || URL.StartsWith(TEXT("https://")) || URL.StartsWith(TEXT("ws://")) || URL.StartsWith(TEXT("wss://")))
    {
        return true;
    }

    return false;
}

TArray<FString> UMingBlockchainSettings::GetSecurityLevelRequirements(ESecurityLevel Level) const
{
    TArray<FString> Requirements;

    switch (Level)
    {
        case ESecurityLevel::Low:
            Requirements.Add(TEXT("Basic transaction validation"));
            break;

        case ESecurityLevel::Medium:
            Requirements.Add(TEXT("Enhanced transaction validation"));
            Requirements.Add(TEXT("Gas price limits"));
            Requirements.Add(TEXT("Basic fraud detection"));
            break;

        case ESecurityLevel::High:
            Requirements.Add(TEXT("Advanced transaction validation"));
            Requirements.Add(TEXT("Strict gas price limits"));
            Requirements.Add(TEXT("Enhanced fraud detection"));
            Requirements.Add(TEXT("Multi-signature support"));
            break;

        case ESecurityLevel::Maximum:
            Requirements.Add(TEXT("Maximum transaction validation"));
            Requirements.Add(TEXT("Very strict gas price limits"));
            Requirements.Add(TEXT("Advanced fraud detection"));
            Requirements.Add(TEXT("Multi-signature required"));
            Requirements.Add(TEXT("Hardware wallet requirement"));
            break;
    }

    return Requirements;
}

bool UMingBlockchainSettings::MeetsSecurityRequirements(const FBlockchainConfig& Config, ESecurityLevel Level) const
{
    switch (Level)
    {
        case ESecurityLevel::Low:
            return true; // Always meets low requirements

        case ESecurityLevel::Medium:
            return Config.MaxGasPrice <= 500.0f && Config.bAutoRetryEnabled;

        case ESecurityLevel::High:
            return Config.MaxGasPrice <= 200.0f && 
                   Config.MaxRetryAttempts <= 5 && 
                   Config.TransactionTimeout <= 600.0f;

        case ESecurityLevel::Maximum:
            return Config.MaxGasPrice <= 100.0f && 
                   Config.MaxRetryAttempts <= 3 && 
                   Config.TransactionTimeout <= 300.0f &&
                   Config.SecurityLevel == ESecurityLevel::Maximum;

        default:
            return false;
    }
}
