#include "MingBlockchainWallet.h"
#include "MingBlockchainTransactionManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"

UMingBlockchainWallet::UMingBlockchainWallet()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    CurrentNetwork = TEXT("Ethereum Mainnet");
    bAutoRefreshEnabled = true;
    AutoRefreshInterval = 30.0f;
    MaxTransactionHistory = 1000;
    TimeSinceLastRefresh = 0.0f;

    TransactionManager = nullptr;
}

void UMingBlockchainWallet::BeginPlay()
{
    Super::BeginPlay();
    InitializeWalletConfiguration();
    LoadWalletData();
}

void UMingBlockchainWallet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    SaveWalletData();
    ClearSensitiveData();
    Super::EndPlay(EndPlayReason);
}

void UMingBlockchainWallet::InitializeWallet(UMingBlockchainTransactionManager* InTransactionManager)
{
    TransactionManager = InTransactionManager;

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Initialized with transaction manager"));
}

bool UMingBlockchainWallet::ConnectWallet(const FString& ProviderName)
{
    if (CurrentWalletInfo.ConnectionStatus == EWalletConnectionStatus::Connected)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainWallet: Wallet already connected"));
        return true;
    }

    CurrentWalletInfo.ConnectionStatus = EWalletConnectionStatus::Connecting;
    ConnectedProvider = ProviderName;

    // Simulate wallet connection process
    // In a real implementation, this would interface with actual wallet providers
    ProcessWalletConnection();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Connecting to provider %s"), *ProviderName);
    return true;
}

bool UMingBlockchainWallet::DisconnectWallet()
{
    if (CurrentWalletInfo.ConnectionStatus == EWalletConnectionStatus::Disconnected)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainWallet: Wallet already disconnected"));
        return true;
    }

    HandleWalletDisconnection();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Disconnected from wallet"));
    return true;
}

FWalletInfo UMingBlockchainWallet::GetWalletInfo() const
{
    return CurrentWalletInfo;
}

float UMingBlockchainWallet::GetTokenBalance(const FString& TokenAddress) const
{
    for (const FWalletBalance& Balance : CurrentBalances)
    {
        if (Balance.TokenAddress == TokenAddress)
        {
            return Balance.Balance;
        }
    }
    return 0.0f;
}

TArray<FWalletBalance> UMingBlockchainWallet::GetAllBalances() const
{
    return CurrentBalances;
}

FString UMingBlockchainWallet::SendTokens(const FString& ToAddress, const FString& TokenAddress, float Amount)
{
    if (!ValidateAddress(ToAddress))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainWallet: Invalid recipient address"));
        return TEXT("");
    }

    if (CurrentWalletInfo.ConnectionStatus != EWalletConnectionStatus::Connected)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainWallet: Wallet not connected"));
        return TEXT("");
    }

    float CurrentBalance = GetTokenBalance(TokenAddress);
    if (CurrentBalance < Amount)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainWallet: Insufficient balance"));
        return TEXT("");
    }

    // Create transaction
    FWalletTransaction Transaction;
    Transaction.TransactionID = FGuid::NewGuid().ToString();
    Transaction.FromAddress = CurrentWalletInfo.WalletAddress;
    Transaction.ToAddress = ToAddress;
    Transaction.TokenSymbol = GetTokenSymbol(TokenAddress);
    Transaction.Amount = Amount;
    Transaction.GasFee = EstimateGasFee(ToAddress, TokenAddress, Amount);
    Transaction.Status = ETransactionStatus::Pending;
    Transaction.Timestamp = FDateTime::Now().ToString();

    // Add to transaction history
    TransactionHistory.Add(Transaction);
    CurrentWalletInfo.TransactionCount++;

    // Update balance (pending)
    for (FWalletBalance& Balance : CurrentBalances)
    {
        if (Balance.TokenAddress == TokenAddress)
        {
            Balance.Balance -= Amount;
            OnBalanceChanged.Broadcast(Balance.TokenSymbol, Balance.Balance);
            break;
        }
    }

    // Send to transaction manager for processing
    if (TransactionManager)
    {
        TransactionManager->ProcessTransaction(Transaction);
    }

    OnTransactionSent.Broadcast(Transaction);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Sent %f %s to %s"), 
           Amount, *Transaction.TokenSymbol, *ToAddress);

    return Transaction.TransactionID;
}

bool UMingBlockchainWallet::ReceiveReward(const FMiningReward& Reward)
{
    if (CurrentWalletInfo.ConnectionStatus != EWalletConnectionStatus::Connected)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainWallet: Wallet not connected for reward receipt"));
        return false;
    }

    // Find or create balance entry for the reward token
    bool bBalanceFound = false;
    for (FWalletBalance& Balance : CurrentBalances)
    {
        if (Balance.TokenAddress == Reward.TokenContract)
        {
            Balance.Balance += Reward.Amount;
            Balance.UsdValue = Balance.Balance * Reward.Value / Reward.Amount;
            bBalanceFound = true;
            OnBalanceChanged.Broadcast(Balance.TokenSymbol, Balance.Balance);
            break;
        }
    }

    if (!bBalanceFound)
    {
        FWalletBalance NewBalance;
        NewBalance.TokenAddress = Reward.TokenContract;
        NewBalance.TokenSymbol = Reward.AssetName;
        NewBalance.TokenName = Reward.AssetName;
        NewBalance.Balance = Reward.Amount;
        NewBalance.UsdValue = Reward.Value;
        CurrentBalances.Add(NewBalance);
        OnBalanceChanged.Broadcast(NewBalance.TokenSymbol, NewBalance.Balance);
    }

    // Create reward transaction record
    FWalletTransaction RewardTransaction;
    RewardTransaction.TransactionID = FGuid::NewGuid().ToString();
    RewardTransaction.FromAddress = TEXT("0x0000000000000000000000000000000000000000"); // System address
    RewardTransaction.ToAddress = CurrentWalletInfo.WalletAddress;
    RewardTransaction.TokenSymbol = Reward.AssetName;
    RewardTransaction.Amount = Reward.Amount;
    RewardTransaction.GasFee = 0.0f; // Rewards typically have no gas fee
    RewardTransaction.Status = ETransactionStatus::Confirmed;
    RewardTransaction.Timestamp = FDateTime::Now().ToString();
    RewardTransaction.Metadata = Reward.Metadata;

    TransactionHistory.Add(RewardTransaction);
    CurrentWalletInfo.TransactionCount++;

    OnTransactionReceived.Broadcast(RewardTransaction);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Received reward %s (%f)"), 
           *Reward.AssetName, Reward.Amount);

    return true;
}

TArray<FWalletTransaction> UMingBlockchainWallet::GetTransactionHistory(int32 Limit) const
{
    TArray<FWalletTransaction> Result;
    int32 Count = FMath::Min(Limit, TransactionHistory.Num());
    
    for (int32 i = TransactionHistory.Num() - Count; i < TransactionHistory.Num(); ++i)
    {
        Result.Add(TransactionHistory[i]);
    }
    
    return Result;
}

FWalletTransaction UMingBlockchainWallet::GetTransaction(const FString& TransactionID) const
{
    for (const FWalletTransaction& Transaction : TransactionHistory)
    {
        if (Transaction.TransactionID == TransactionID)
        {
            return Transaction;
        }
    }
    return FWalletTransaction();
}

EWalletConnectionStatus UMingBlockchainWallet::GetConnectionStatus() const
{
    return CurrentWalletInfo.ConnectionStatus;
}

bool UMingBlockchainWallet::IsWalletConnected() const
{
    return CurrentWalletInfo.ConnectionStatus == EWalletConnectionStatus::Connected;
}

FString UMingBlockchainWallet::GetNetworkInfo() const
{
    return FString::Printf(TEXT("Network: %s, Chain ID: %s"), *CurrentNetwork, *CurrentWalletInfo.ChainID);
}

bool UMingBlockchainWallet::SwitchNetwork(const FString& NetworkName)
{
    if (CurrentNetwork == NetworkName)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainWallet: Already on network %s"), *NetworkName);
        return true;
    }

    // In a real implementation, this would switch the blockchain network
    CurrentNetwork = NetworkName;
    CurrentWalletInfo.NetworkName = NetworkName;
    
    // Update chain ID based on network
    if (NetworkName.Contains(TEXT("Mainnet")))
    {
        CurrentWalletInfo.ChainID = TEXT("1");
    }
    else if (NetworkName.Contains(TEXT("Testnet")))
    {
        CurrentWalletInfo.ChainID = TEXT("3");
    }
    else
    {
        CurrentWalletInfo.ChainID = TEXT("1337"); // Local network
    }

    // Refresh balances and transactions after network switch
    RefreshWalletData();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Switched to network %s"), *NetworkName);
    return true;
}

bool UMingBlockchainWallet::ImportWallet(const FString& PrivateKeyOrSeed)
{
    // Validate input
    if (PrivateKeyOrSeed.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainWallet: Empty private key or seed phrase"));
        return false;
    }

    // In a real implementation, this would validate and import the wallet
    // For now, simulate wallet import
    CurrentWalletInfo.WalletAddress = GenerateWalletAddress();
    CurrentWalletInfo.WalletName = TEXT("Imported Wallet");
    CurrentWalletInfo.ConnectionStatus = EWalletConnectionStatus::Connected;

    // Save encrypted wallet data
    SaveWalletData();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Imported wallet with address %s"), *CurrentWalletInfo.WalletAddress);
    return true;
}

FString UMingBlockchainWallet::ExportPrivateKey()
{
    if (CurrentWalletInfo.ConnectionStatus != EWalletConnectionStatus::Connected)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainWallet: No wallet connected"));
        return TEXT("");
    }

    // In a real implementation, this would require user confirmation
    // and return the actual private key
    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainWallet: Private key export requested (user confirmation required)"));
    return TEXT("PRIVATE_KEY_EXPORT_REQUIRES_CONFIRMATION");
}

FString UMingBlockchainWallet::CreateNewWallet()
{
    FString NewAddress = GenerateWalletAddress();
    FString BackupPhrase = GenerateBackupPhrase();

    CurrentWalletInfo.WalletAddress = NewAddress;
    CurrentWalletInfo.WalletName = TEXT("New Wallet");
    CurrentWalletInfo.ConnectionStatus = EWalletConnectionStatus::Connected;

    // Save wallet data
    SaveWalletData();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Created new wallet with address %s"), *NewAddress);
    return NewAddress;
}

bool UMingBlockchainWallet::ValidateAddress(const FString& Address) const
{
    // Basic Ethereum address validation
    if (Address.Len() != 42 || !Address.StartsWith(TEXT("0x")))
    {
        return false;
    }

    // Check if all characters after 0x are valid hexadecimal
    for (int32 i = 2; i < Address.Len(); ++i)
    {
        TCHAR Char = Address[i];
        bool bValid = (Char >= '0' && Char <= '9') || 
                     (Char >= 'a' && Char <= 'f') || 
                     (Char >= 'A' && Char <= 'F');
        
        if (!bValid)
        {
            return false;
        }
    }

    return true;
}

float UMingBlockchainWallet::EstimateGasFee(const FString& ToAddress, const FString& TokenAddress, float Amount) const
{
    // Simplified gas fee estimation
    // In a real implementation, this would query the blockchain for current gas prices
    float BaseGasPrice = 20.0f; // Gwei
    float GasLimit = 21000.0f; // Standard ETH transfer
    
    // For ERC20 tokens, higher gas limit
    if (TokenAddress != TEXT("0x0000000000000000000000000000000000000000"))
    {
        GasLimit = 65000.0f;
    }

    float GasFeeETH = (BaseGasPrice * GasLimit) / 1e9f; // Convert to ETH
    
    // Convert to USD (assuming $2000 per ETH)
    return GasFeeETH * 2000.0f;
}

TArray<FString> UMingBlockchainWallet::GetSupportedNetworks() const
{
    TArray<FString> Networks;
    Networks.Add(TEXT("Ethereum Mainnet"));
    Networks.Add(TEXT("Ethereum Testnet"));
    Networks.Add(TEXT("Polygon Mainnet"));
    Networks.Add(TEXT("BSC Mainnet"));
    Networks.Add(TEXT("Local Network"));
    return Networks;
}

bool UMingBlockchainWallet::RefreshWalletData()
{
    if (CurrentWalletInfo.ConnectionStatus != EWalletConnectionStatus::Connected)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainWallet: Cannot refresh - wallet not connected"));
        return false;
    }

    UpdateBalances();
    UpdateTransactionHistory();

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Wallet data refreshed"));
    return true;
}

FString UMingBlockchainWallet::GetBackupPhrase() const
{
    // In a real implementation, this would be securely stored and retrieved
    return TEXT("BACKUP_PHRASE_SECURELY_STORED");
}

void UMingBlockchainWallet::InitializeWalletConfiguration()
{
    // Load configuration from settings or use defaults
    CurrentWalletInfo.NetworkName = CurrentNetwork;
    CurrentWalletInfo.ChainID = TEXT("1"); // Ethereum mainnet
    CurrentWalletInfo.ConnectionStatus = EWalletConnectionStatus::Disconnected;

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Configuration initialized"));
}

void UMingBlockchainWallet::UpdateBalances()
{
    // Simulate balance updates
    // In a real implementation, this would query the blockchain
    if (CurrentBalances.Num() == 0 && CurrentWalletInfo.ConnectionStatus == EWalletConnectionStatus::Connected)
    {
        // Add some default balances for demonstration
        FWalletBalance ETHBalance;
        ETHBalance.TokenSymbol = TEXT("ETH");
        ETHBalance.TokenAddress = TEXT("0x0000000000000000000000000000000000000000");
        ETHBalance.TokenName = TEXT("Ethereum");
        ETHBalance.Balance = UKismetMathLibrary::RandomFloatInRange(0.5f, 5.0f);
        ETHBalance.UsdValue = ETHBalance.Balance * 2000.0f; // Assuming $2000 per ETH
        ETHBalance.Decimals = 18;
        CurrentBalances.Add(ETHBalance);

        FWalletBalance MingCoinBalance;
        MingCoinBalance.TokenSymbol = TEXT("MING");
        MingCoinBalance.TokenAddress = TEXT("0x1234567890abcdef1234567890abcdef12345678");
        MingCoinBalance.TokenName = TEXT("MingCoin");
        MingCoinBalance.Balance = UKismetMathLibrary::RandomFloatInRange(100.0f, 1000.0f);
        MingCoinBalance.UsdValue = MingCoinBalance.Balance * 0.1f; // Assuming $0.10 per MING
        MingCoinBalance.Decimals = 18;
        CurrentBalances.Add(MingCoinBalance);
    }

    CurrentWalletInfo.Balances = CurrentBalances;
}

void UMingBlockchainWallet::UpdateTransactionHistory()
{
    // Simulate transaction history updates
    // In a real implementation, this would query the blockchain
    CurrentWalletInfo.TransactionCount = TransactionHistory.Num();
}

void UMingBlockchainWallet::ProcessWalletConnection()
{
    // Simulate successful wallet connection
    CurrentWalletInfo.ConnectionStatus = EWalletConnectionStatus::Connected;
    CurrentWalletInfo.WalletName = ConnectedProvider;
    CurrentWalletInfo.NetworkName = CurrentNetwork;
    
    // Generate a mock wallet address if not already set
    if (CurrentWalletInfo.WalletAddress.IsEmpty())
    {
        CurrentWalletInfo.WalletAddress = GenerateWalletAddress();
    }

    // Update balances and transactions
    UpdateBalances();
    UpdateTransactionHistory();

    OnWalletConnected.Broadcast(CurrentWalletInfo);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Wallet connected successfully"));
}

void UMingBlockchainWallet::HandleWalletDisconnection()
{
    CurrentWalletInfo.ConnectionStatus = EWalletConnectionStatus::Disconnected;
    ConnectedProvider.Empty();
    
    OnWalletDisconnected.Broadcast(CurrentWalletInfo.WalletAddress);
}

bool UMingBlockchainWallet::ValidateWalletConfiguration() const
{
    return !CurrentNetwork.IsEmpty() && MaxTransactionHistory > 0;
}

FString UMingBlockchainWallet::GenerateWalletAddress() const
{
    // Generate a mock Ethereum address
    FString Address = TEXT("0x");
    for (int32 i = 0; i < 40; ++i)
    {
        int32 RandValue = UKismetMathLibrary::RandomIntegerInRange(0, 16);
        if (RandValue < 10)
        {
            Address += FString::FromInt(RandValue);
        }
        else
        {
            Address += TEXT("ABCDEF")[RandValue - 10];
        }
    }
    return Address;
}

FString UMingBlockchainWallet::GenerateBackupPhrase() const
{
    // Generate a mock 12-word seed phrase
    TArray<FString> Words = {
        TEXT("abandon"), TEXT("ability"), TEXT("able"), TEXT("about"), TEXT("above"), TEXT("absent"),
        TEXT("absorb"), TEXT("abstract"), TEXT("absurd"), TEXT("abuse"), TEXT("access"), TEXT("accident")
    };
    
    FString Phrase;
    for (int32 i = 0; i < 12; ++i)
    {
        if (i > 0) Phrase += TEXT(" ");
        Phrase += Words[UKismetMathLibrary::RandomIntegerInRange(0, Words.Num())];
    }
    
    return Phrase;
}

FString UMingBlockchainWallet::EncryptData(const FString& Data) const
{
    // In a real implementation, this would use proper encryption
    return TEXT("ENCRYPTED:") + Data;
}

FString UMingBlockchainWallet::DecryptData(const FString& EncryptedData) const
{
    // In a real implementation, this would use proper decryption
    if (EncryptedData.StartsWith(TEXT("ENCRYPTED:")))
    {
        return EncryptedData.RightChop(10);
    }
    return EncryptedData;
}

void UMingBlockchainWallet::SaveWalletData()
{
    // Save wallet data to local storage
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("WalletData.json");
    
    // In a real implementation, this would save encrypted wallet data
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Wallet data saved to %s"), *SavePath);
}

void UMingBlockchainWallet::LoadWalletData()
{
    // Load wallet data from local storage
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("WalletData.json");
    
    // In a real implementation, this would load and decrypt wallet data
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Wallet data loaded from %s"), *LoadPath);
}

void UMingBlockchainWallet::ClearSensitiveData()
{
    // Clear sensitive data from memory
    CurrentWalletInfo.WalletAddress.Empty();
    TransactionHistory.Empty();
    CurrentBalances.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainWallet: Sensitive data cleared"));
}

FString UMingBlockchainWallet::FormatAddress(const FString& Address) const
{
    if (Address.Len() < 10)
    {
        return Address;
    }
    
    return Address.Left(6) + TEXT("...") + Address.Right(4);
}

int32 UMingBlockchainWallet::GetTokenDecimals(const FString& TokenAddress) const
{
    for (const FWalletBalance& Balance : CurrentBalances)
    {
        if (Balance.TokenAddress == TokenAddress)
        {
            return Balance.Decimals;
        }
    }
    return 18; // Default to 18 decimals
}

int64 UMingBlockchainWallet::ConvertToWei(float Amount, int32 Decimals) const
{
    float Multiplier = FMath::Pow(10.0f, Decimals);
    return (int64)(Amount * Multiplier);
}

float UMingBlockchainWallet::ConvertFromWei(int64 Wei, int32 Decimals) const
{
    float Divisor = FMath::Pow(10.0f, Decimals);
    return (float)Wei / Divisor;
}

FString UMingBlockchainWallet::GetTokenSymbol(const FString& TokenAddress) const
{
    for (const FWalletBalance& Balance : CurrentBalances)
    {
        if (Balance.TokenAddress == TokenAddress)
        {
            return Balance.TokenSymbol;
        }
    }
    return TEXT("UNKNOWN");
}
