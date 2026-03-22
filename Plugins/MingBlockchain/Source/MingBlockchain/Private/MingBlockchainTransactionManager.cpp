#include "MingBlockchainTransactionManager.h"
#include "MingBlockchainWallet.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingBlockchainTransactionManager::UMingBlockchainTransactionManager()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    CurrentFeeType = ETransactionFeeType::Standard;
    bAutoRetryEnabled = true;
    MaxRetryAttempts = 3;
    DefaultGasPrice = 20.0f; // Gwei
    MaxPendingTransactions = 50;
    TransactionTimeout = 300.0f; // 5 minutes
    GasUpdateInterval = 30.0f; // Update every 30 seconds
    TimeSinceLastGasUpdate = 0.0f;

    BlockchainWallet = nullptr;
}

void UMingBlockchainTransactionManager::BeginPlay()
{
    Super::BeginPlay();
    InitializeTransactionConfiguration();
    LoadTransactionData();
}

void UMingBlockchainTransactionManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    SaveTransactionData();
    Super::EndPlay(EndPlayReason);
}

void UMingBlockchainTransactionManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    TimeSinceLastGasUpdate += DeltaTime;
    if (TimeSinceLastGasUpdate >= GasUpdateInterval)
    {
        UpdateGasPrices();
        TimeSinceLastGasUpdate = 0.0f;
    }

    // Check for transaction timeouts
    float CurrentTime = GetWorld()->GetTimeSeconds();
    for (int32 i = PendingTransactions.Num() - 1; i >= 0; --i)
    {
        const FBlockchainTransaction& Transaction = PendingTransactions[i];
        if (CurrentTime - Transaction.Timestamp > TransactionTimeout)
        {
            HandleTransactionTimeout(Transaction.TransactionHash);
        }
    }

    // Clean up old transactions periodically
    if (FMath::RandRange(0, 1000) < 10) // 1% chance per tick
    {
        CleanupOldTransactions();
    }
}

void UMingBlockchainTransactionManager::InitializeTransactionManager(UMingBlockchainWallet* Wallet)
{
    BlockchainWallet = Wallet;

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Initialized with wallet"));
}

FString UMingBlockchainTransactionManager::CreateTransaction(const FString& FromAddress, const FString& ToAddress, const FString& TokenContract, float Amount, ETransactionPriority Priority)
{
    if (!ValidateTransactionParameters(FromAddress, ToAddress, TokenContract, Amount))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainTransactionManager: Invalid transaction parameters"));
        return TEXT("");
    }

    if (PendingTransactions.Num() >= MaxPendingTransactions)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainTransactionManager: Maximum pending transactions reached"));
        return TEXT("");
    }

    FBlockchainTransaction Transaction;
    Transaction.TransactionHash = GenerateTransactionHash();
    Transaction.FromAddress = FromAddress;
    Transaction.ToAddress = ToAddress;
    Transaction.TokenContract = TokenContract;
    Transaction.TokenSymbol = GetTokenSymbol(TokenContract);
    Transaction.Amount = Amount;
    Transaction.GasPrice = CalculateOptimalGasPrice(Priority);
    Transaction.GasLimit = CalculateGasLimit(Transaction);
    Transaction.GasFee = (Transaction.GasPrice * Transaction.GasLimit) / 1e9f; // Convert to ETH
    Transaction.Priority = Priority;
    Transaction.Status = ETransactionStatus::Pending;
    Transaction.Timestamp = GetWorld()->GetTimeSeconds();

    PendingTransactions.Add(Transaction);
    OnTransactionCreated.Broadcast(Transaction);

    // Start processing
    ProcessTransaction(Transaction);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Created transaction %s"), *Transaction.TransactionHash);
    return Transaction.TransactionHash;
}

FString UMingBlockchainTransactionManager::CreateRewardTransaction(const FString& PlayerID, const FMiningReward& Reward)
{
    if (!BlockchainWallet || !BlockchainWallet->IsWalletConnected())
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainTransactionManager: Wallet not connected for reward transaction"));
        return TEXT("");
    }

    FString WalletAddress = BlockchainWallet->GetWalletInfo().WalletAddress;
    return CreateTransaction(
        TEXT("0x0000000000000000000000000000000000000"), // System address
        WalletAddress,
        Reward.TokenContract,
        Reward.Amount,
        ETransactionPriority::High
    );
}

bool UMingBlockchainTransactionManager::ProcessTransaction(const FBlockchainTransaction& Transaction)
{
    if (!ValidateTransaction(Transaction))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainTransactionManager: Invalid transaction data"));
        return false;
    }

    // Find and update the transaction in pending list
    for (FBlockchainTransaction& PendingTx : PendingTransactions)
    {
        if (PendingTx.TransactionHash == Transaction.TransactionHash)
        {
            return ProcessTransactionOnBlockchain(PendingTx);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainTransactionManager: Transaction %s not found in pending list"), *Transaction.TransactionHash);
    return false;
}

FString UMingBlockchainTransactionManager::CreateTransactionBatch(const TArray<FBlockchainTransaction>& Transactions)
{
    if (Transactions.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainTransactionManager: Cannot create empty batch"));
        return TEXT("");
    }

    FTransactionBatch Batch;
    Batch.BatchID = GenerateBatchID();
    Batch.Transactions = Transactions;
    Batch.CreatedTime = GetWorld()->GetTimeSeconds();
    Batch.BatchStatus = ETransactionStatus::Pending;

    // Calculate totals
    for (const FBlockchainTransaction& Transaction : Transactions)
    {
        Batch.TotalAmount += Transaction.Amount;
        Batch.TotalGasFee += Transaction.GasFee;
    }

    TransactionBatches.Add(Batch.BatchID, Batch);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Created batch %s with %d transactions"), 
           *Batch.BatchID, Transactions.Num());

    return Batch.BatchID;
}

bool UMingBlockchainTransactionManager::ProcessTransactionBatch(const FString& BatchID)
{
    if (!TransactionBatches.Contains(BatchID))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainTransactionManager: Batch %s not found"), *BatchID);
        return false;
    }

    FTransactionBatch& Batch = TransactionBatches[BatchID];
    ProcessBatchTransactions(Batch);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Processing batch %s"), *BatchID);
    return true;
}

FBlockchainTransaction UMingBlockchainTransactionManager::GetTransaction(const FString& TransactionHash) const
{
    // Check pending transactions first
    for (const FBlockchainTransaction& Transaction : PendingTransactions)
    {
        if (Transaction.TransactionHash == TransactionHash)
        {
            return Transaction;
        }
    }

    // Check transaction history
    for (const FBlockchainTransaction& Transaction : TransactionHistory)
    {
        if (Transaction.TransactionHash == TransactionHash)
        {
            return Transaction;
        }
    }

    return FBlockchainTransaction();
}

FTransactionBatch UMingBlockchainTransactionManager::GetTransactionBatch(const FString& BatchID) const
{
    if (const FTransactionBatch* Batch = TransactionBatches.Find(BatchID))
    {
        return *Batch;
    }
    return FTransactionBatch();
}

TArray<FBlockchainTransaction> UMingBlockchainTransactionManager::GetPendingTransactions() const
{
    return PendingTransactions;
}

TArray<FBlockchainTransaction> UMingBlockchainTransactionManager::GetTransactionHistory(int32 Limit) const
{
    TArray<FBlockchainTransaction> Result;
    int32 Count = FMath::Min(Limit, TransactionHistory.Num());
    
    for (int32 i = TransactionHistory.Num() - Count; i < TransactionHistory.Num(); ++i)
    {
        Result.Add(TransactionHistory[i]);
    }
    
    return Result;
}

FTransactionStatistics UMingBlockchainTransactionManager::GetTransactionStatistics(const FString& PlayerID) const
{
    if (PlayerID.IsEmpty())
    {
        // Return global statistics
        FTransactionStatistics GlobalStats;
        for (const FBlockchainTransaction& Transaction : TransactionHistory)
        {
            GlobalStats.TotalTransactions++;
            GlobalStats.TotalVolume += Transaction.Amount;
            GlobalStats.TotalGasFees += Transaction.GasFee;
            
            if (Transaction.Status == ETransactionStatus::Confirmed)
            {
                GlobalStats.SuccessfulTransactions++;
            }
            else if (Transaction.Status == ETransactionStatus::Failed)
            {
                GlobalStats.FailedTransactions++;
            }
        }
        
        GlobalStats.PendingTransactions = PendingTransactions.Num();
        GlobalStats.SuccessRate = GlobalStats.TotalTransactions > 0 ? 
            (float)GlobalStats.SuccessfulTransactions / GlobalStats.TotalTransactions : 0.0f;
        GlobalStats.AverageGasFee = GlobalStats.TotalTransactions > 0 ? 
            GlobalStats.TotalGasFees / GlobalStats.TotalTransactions : 0.0f;
        
        return GlobalStats;
    }
    else
    {
        // Return player-specific statistics
        if (const FTransactionStatistics* Stats = PlayerStatistics.Find(PlayerID))
        {
            return *Stats;
        }
        
        return FTransactionStatistics();
    }
}

bool UMingBlockchainTransactionManager::CancelTransaction(const FString& TransactionHash)
{
    for (int32 i = 0; i < PendingTransactions.Num(); ++i)
    {
        if (PendingTransactions[i].TransactionHash == TransactionHash)
        {
            FBlockchainTransaction& Transaction = PendingTransactions[i];
            Transaction.Status = ETransactionStatus::Cancelled;
            Transaction.ErrorMessage = TEXT("Transaction cancelled by user");
            
            // Move to history
            TransactionHistory.Add(Transaction);
            PendingTransactions.RemoveAt(i);
            
            OnTransactionFailed.Broadcast(Transaction);
            
            UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Cancelled transaction %s"), *TransactionHash);
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainTransactionManager: Transaction %s not found in pending list"), *TransactionHash);
    return false;
}

FString UMingBlockchainTransactionManager::RetryTransaction(const FString& TransactionHash, ETransactionPriority NewPriority)
{
    // Find the failed transaction
    FBlockchainTransaction FailedTransaction;
    bool bFound = false;
    
    for (const FBlockchainTransaction& Transaction : TransactionHistory)
    {
        if (Transaction.TransactionHash == TransactionHash && Transaction.Status == ETransactionStatus::Failed)
        {
            FailedTransaction = Transaction;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainTransactionManager: Failed transaction %s not found"), *TransactionHash);
        return TEXT("");
    }
    
    // Create new transaction with higher priority
    return CreateTransaction(
        FailedTransaction.FromAddress,
        FailedTransaction.ToAddress,
        FailedTransaction.TokenContract,
        FailedTransaction.Amount,
        NewPriority
    );
}

float UMingBlockchainTransactionManager::EstimateGasFee(const FString& FromAddress, const FString& ToAddress, const FString& TokenContract, float Amount, ETransactionPriority Priority) const
{
    float GasPrice = CalculateOptimalGasPrice(Priority);
    int32 GasLimit;
    
    if (TokenContract.IsEmpty() || TokenContract == TEXT("0x0000000000000000000000000000000000000"))
    {
        GasLimit = 21000; // Standard ETH transfer
    }
    else
    {
        GasLimit = 65000; // ERC20 token transfer
    }
    
    return (GasPrice * GasLimit) / 1e9f; // Convert to ETH
}

float UMingBlockchainTransactionManager::GetCurrentGasPrice() const
{
    return DefaultGasPrice;
}

float UMingBlockchainTransactionManager::GetRecommendedGasPrice(ETransactionPriority Priority) const
{
    return CalculateOptimalGasPrice(Priority);
}

void UMingBlockchainTransactionManager::SetTransactionFeeType(ETransactionFeeType FeeType)
{
    CurrentFeeType = FeeType;
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Transaction fee type set to %d"), (int32)FeeType);
}

ETransactionFeeType UMingBlockchainTransactionManager::GetTransactionFeeType() const
{
    return CurrentFeeType;
}

void UMingBlockchainTransactionManager::SetAutoRetryEnabled(bool bEnabled)
{
    bAutoRetryEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Auto-retry %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingBlockchainTransactionManager::IsAutoRetryEnabled() const
{
    return bAutoRetryEnabled;
}

void UMingBlockchainTransactionManager::SetMaxRetryAttempts(int32 MaxAttempts)
{
    MaxRetryAttempts = FMath::Max(1, MaxAttempts);
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Max retry attempts set to %d"), MaxRetryAttempts);
}

int32 UMingBlockchainTransactionManager::GetMaxRetryAttempts() const
{
    return MaxRetryAttempts;
}

int32 UMingBlockchainTransactionManager::GetTransactionConfirmations(const FString& TransactionHash) const
{
    FBlockchainTransaction Transaction = GetTransaction(TransactionHash);
    return Transaction.ConfirmationCount;
}

bool UMingBlockchainTransactionManager::IsTransactionConfirmed(const FString& TransactionHash, int32 RequiredConfirmations) const
{
    FBlockchainTransaction Transaction = GetTransaction(TransactionHash);
    return Transaction.Status == ETransactionStatus::Confirmed && 
           Transaction.ConfirmationCount >= RequiredConfirmations;
}

float UMingBlockchainTransactionManager::GetTransactionProcessingTime(const FString& TransactionHash) const
{
    FBlockchainTransaction Transaction = GetTransaction(TransactionHash);
    if (Transaction.Status == ETransactionStatus::Confirmed && !Transaction.BlockHash.IsEmpty())
    {
        return Transaction.Timestamp; // In a real implementation, would calculate actual processing time
    }
    return 0.0f;
}

FString UMingBlockchainTransactionManager::ExportTransaction(const FString& TransactionHash, const FString& Format) const
{
    FBlockchainTransaction Transaction = GetTransaction(TransactionHash);
    if (Transaction.TransactionHash.IsEmpty())
    {
        return TEXT("");
    }
    
    return FormatTransactionForExport(Transaction, Format);
}

bool UMingBlockchainTransactionManager::ImportTransaction(const FString& Data, const FString& Format)
{
    FBlockchainTransaction Transaction = ParseTransactionFromImport(Data, Format);
    if (ValidateTransaction(Transaction))
    {
        TransactionHistory.Add(Transaction);
        SaveTransactionData();
        
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Imported transaction %s"), *Transaction.TransactionHash);
        return true;
    }
    
    return false;
}

bool UMingBlockchainTransactionManager::ValidateTransaction(const FBlockchainTransaction& Transaction) const
{
    return !Transaction.TransactionHash.IsEmpty() &&
           !Transaction.FromAddress.IsEmpty() &&
           !Transaction.ToAddress.IsEmpty() &&
           Transaction.Amount > 0.0f &&
           Transaction.GasLimit > 0 &&
           Transaction.GasPrice >= 0.0f;
}

FString UMingBlockchainTransactionManager::GetTransactionError(const FString& TransactionHash) const
{
    FBlockchainTransaction Transaction = GetTransaction(TransactionHash);
    return Transaction.ErrorMessage;
}

void UMingBlockchainTransactionManager::InitializeTransactionConfiguration()
{
    // Load configuration from settings or use defaults
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Configuration initialized"));
}

FString UMingBlockchainTransactionManager::GenerateTransactionHash() const
{
    return FGuid::NewGuid().ToString();
}

FString UMingBlockchainTransactionManager::GenerateBatchID() const
{
    return TEXT("BATCH_") + FGuid::NewGuid().ToString();
}

float UMingBlockchainTransactionManager::CalculateOptimalGasPrice(ETransactionPriority Priority) const
{
    float BaseGasPrice = GetCurrentGasPrice();
    float Multiplier = GetFeeMultiplier(Priority);
    
    return BaseGasPrice * Multiplier;
}

int32 UMingBlockchainTransactionManager::CalculateGasLimit(const FBlockchainTransaction& Transaction) const
{
    if (Transaction.TokenContract.IsEmpty() || Transaction.TokenContract == TEXT("0x0000000000000000000000000000000000000"))
    {
        return 21000; // Standard ETH transfer
    }
    else
    {
        return 65000; // ERC20 token transfer
    }
}

bool UMingBlockchainTransactionManager::ProcessTransactionOnBlockchain(FBlockchainTransaction& Transaction)
{
    // Simulate blockchain processing
    // In a real implementation, this would interact with actual blockchain nodes
    
    // Simulate processing time based on gas price
    float ProcessingTime = 5.0f + (30.0f / Transaction.GasPrice); // Higher gas = faster processing
    
    // For simulation, we'll randomly succeed or fail
    bool bSuccess = UKismetMathLibrary::RandomBoolWithWeight(0.95f); // 95% success rate
    
    if (bSuccess)
    {
        Transaction.Status = ETransactionStatus::Confirmed;
        Transaction.BlockHash = TEXT("0x") + FGuid::NewGuid().ToString().Left(64);
        Transaction.BlockNumber = UKismetMathLibrary::RandomIntegerInRange(15000000, 16000000);
        Transaction.ConfirmationCount = 12; // Simulate full confirmation
        
        // Move from pending to history
        PendingTransactions.Remove(Transaction);
        TransactionHistory.Add(Transaction);
        
        OnTransactionConfirmed.Broadcast(Transaction);
        
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Transaction %s confirmed"), *Transaction.TransactionHash);
    }
    else
    {
        Transaction.Status = ETransactionStatus::Failed;
        Transaction.ErrorMessage = TEXT("Transaction failed: Insufficient gas or network error");
        
        // Move from pending to history
        PendingTransactions.Remove(Transaction);
        TransactionHistory.Add(Transaction);
        
        OnTransactionFailed.Broadcast(Transaction);
        
        // Auto-retry if enabled
        if (bAutoRetryEnabled)
        {
            RetryFailedTransaction(Transaction);
        }
        
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainTransactionManager: Transaction %s failed"), *Transaction.TransactionHash);
    }
    
    return bSuccess;
}

void UMingBlockchainTransactionManager::UpdateTransactionStatus(const FString& TransactionHash, ETransactionStatus NewStatus, const FString& ErrorMessage)
{
    for (FBlockchainTransaction& Transaction : PendingTransactions)
    {
        if (Transaction.TransactionHash == TransactionHash)
        {
            Transaction.Status = NewStatus;
            if (!ErrorMessage.IsEmpty())
            {
                Transaction.ErrorMessage = ErrorMessage;
            }
            
            if (NewStatus == ETransactionStatus::Confirmed)
            {
                OnTransactionConfirmed.Broadcast(Transaction);
            }
            else if (NewStatus == ETransactionStatus::Failed)
            {
                OnTransactionFailed.Broadcast(Transaction);
            }
            
            break;
        }
    }
}

void UMingBlockchainTransactionManager::UpdateTransactionConfirmations(const FString& TransactionHash, int32 NewConfirmations)
{
    for (FBlockchainTransaction& Transaction : PendingTransactions)
    {
        if (Transaction.TransactionHash == TransactionHash)
        {
            Transaction.ConfirmationCount = NewConfirmations;
            break;
        }
    }
}

void UMingBlockchainTransactionManager::ProcessBatchTransactions(FTransactionBatch& Batch)
{
    int32 SuccessCount = 0;
    int32 FailureCount = 0;
    
    for (FBlockchainTransaction& Transaction : Batch.Transactions)
    {
        if (ProcessTransactionOnBlockchain(Transaction))
        {
            SuccessCount++;
        }
        else
        {
            FailureCount++;
        }
    }
    
    Batch.ProcessedTime = GetWorld()->GetTimeSeconds();
    Batch.BatchStatus = FailureCount == 0 ? ETransactionStatus::Confirmed : ETransactionStatus::Failed;
    
    OnBatchProcessed.Broadcast(Batch);
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Batch %s processed: %d success, %d failed"), 
           *Batch.BatchID, SuccessCount, FailureCount);
}

void UMingBlockchainTransactionManager::HandleTransactionTimeout(const FString& TransactionHash)
{
    for (int32 i = 0; i < PendingTransactions.Num(); ++i)
    {
        if (PendingTransactions[i].TransactionHash == TransactionHash)
        {
            FBlockchainTransaction& Transaction = PendingTransactions[i];
            Transaction.Status = ETransactionStatus::Expired;
            Transaction.ErrorMessage = TEXT("Transaction timed out");
            
            // Move to history
            TransactionHistory.Add(Transaction);
            PendingTransactions.RemoveAt(i);
            
            OnTransactionFailed.Broadcast(Transaction);
            
            UE_LOG(LogTemp, Warning, TEXT("MingBlockchainTransactionManager: Transaction %s timed out"), *TransactionHash);
            break;
        }
    }
}

void UMingBlockchainTransactionManager::RetryFailedTransaction(const FBlockchainTransaction& Transaction)
{
    if (Transaction.Status == ETransactionStatus::Failed)
    {
        // Create retry with higher priority
        ETransactionPriority NewPriority = (ETransactionPriority)FMath::Min((int32)Transaction.Priority + 1, (int32)ETransactionPriority::Critical);
        
        FString NewHash = CreateTransaction(
            Transaction.FromAddress,
            Transaction.ToAddress,
            Transaction.TokenContract,
            Transaction.Amount,
            NewPriority
        );
        
        UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Retrying transaction %s as %s"), 
               *Transaction.TransactionHash, *NewHash);
    }
}

void UMingBlockchainTransactionManager::UpdateGasPrices()
{
    // In a real implementation, this would query current gas prices from the network
    // For now, simulate with some randomness
    float NetworkGasPrice = GetNetworkGasPrice();
    DefaultGasPrice = FMath::Lerp(DefaultGasPrice, NetworkGasPrice, 0.3f); // Smooth transition
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("MingBlockchainTransactionManager: Updated gas price to %f Gwei"), DefaultGasPrice);
}

float UMingBlockchainTransactionManager::GetNetworkGasPrice() const
{
    // Simulate network gas price with some variation
    return UKismetMathLibrary::RandomFloatInRange(15.0f, 35.0f);
}

void UMingBlockchainTransactionManager::UpdateTransactionStatistics(const FString& PlayerID, const FBlockchainTransaction& Transaction)
{
    FTransactionStatistics& Stats = PlayerStatistics.FindOrAdd(PlayerID);
    
    Stats.TotalTransactions++;
    Stats.TotalVolume += Transaction.Amount;
    Stats.TotalGasFees += Transaction.GasFee;
    
    if (Transaction.Status == ETransactionStatus::Confirmed)
    {
        Stats.SuccessfulTransactions++;
    }
    else if (Transaction.Status == ETransactionStatus::Failed)
    {
        Stats.FailedTransactions++;
    }
    
    Stats.SuccessRate = Stats.TotalTransactions > 0 ? 
        (float)Stats.SuccessfulTransactions / Stats.TotalTransactions : 0.0f;
    Stats.AverageGasFee = Stats.TotalTransactions > 0 ? 
        Stats.TotalGasFees / Stats.TotalTransactions : 0.0f;
}

void UMingBlockchainTransactionManager::CleanupOldTransactions()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    const float MaxAge = 86400.0f * 7.0f; // 7 days
    
    // Clean up old confirmed transactions
    TransactionHistory.RemoveAll([CurrentTime, MaxAge](const FBlockchainTransaction& Transaction) {
        return (CurrentTime - Transaction.Timestamp) > MaxAge;
    });
    
    // Clean up old batches
    for (auto It = TransactionBatches.CreateIterator(); It; ++It)
    {
        if ((CurrentTime - It->Value.CreatedTime) > MaxAge)
        {
            It.RemoveCurrent();
        }
    }
}

void UMingBlockchainTransactionManager::SaveTransactionData()
{
    // Save transaction data to persistent storage
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("Transactions.json");
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Transaction data saved to %s"), *SavePath);
}

void UMingBlockchainTransactionManager::LoadTransactionData()
{
    // Load transaction data from persistent storage
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("Transactions.json");
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainTransactionManager: Transaction data loaded from %s"), *LoadPath);
}

bool UMingBlockchainTransactionManager::ValidateTransactionParameters(const FString& FromAddress, const FString& ToAddress, const FString& TokenContract, float Amount) const
{
    return !FromAddress.IsEmpty() &&
           !ToAddress.IsEmpty() &&
           Amount > 0.0f &&
           FromAddress != ToAddress;
}

float UMingBlockchainTransactionManager::GetFeeMultiplier(ETransactionPriority Priority) const
{
    switch (Priority)
    {
        case ETransactionPriority::Low:       return 0.8f;
        case ETransactionPriority::Medium:    return 1.0f;
        case ETransactionPriority::High:      return 1.5f;
        case ETransactionPriority::Urgent:    return 2.0f;
        case ETransactionPriority::Critical:  return 3.0f;
        default: return 1.0f;
    }
}

FString UMingBlockchainTransactionManager::FormatTransactionForExport(const FBlockchainTransaction& Transaction, const FString& Format) const
{
    if (Format.ToUpper() == TEXT("JSON"))
    {
        TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
        JsonObject->SetStringField(TEXT("hash"), Transaction.TransactionHash);
        JsonObject->SetStringField(TEXT("from"), Transaction.FromAddress);
        JsonObject->SetStringField(TEXT("to"), Transaction.ToAddress);
        JsonObject->SetStringField(TEXT("token"), Transaction.TokenContract);
        JsonObject->SetNumberField(TEXT("amount"), Transaction.Amount);
        JsonObject->SetNumberField(TEXT("gasFee"), Transaction.GasFee);
        JsonObject->SetNumberField(TEXT("gasPrice"), Transaction.GasPrice);
        JsonObject->SetNumberField(TEXT("status"), (int32)Transaction.Status);
        JsonObject->SetStringField(TEXT("timestamp"), FString::SanitizeFloat(Transaction.Timestamp));
        
        FString OutputString;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
        FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
        
        return OutputString;
    }
    else if (Format.ToUpper() == TEXT("CSV"))
    {
        return FString::Printf(TEXT("%s,%s,%s,%s,%f,%f,%f,%d,%f"),
            *Transaction.TransactionHash,
            *Transaction.FromAddress,
            *Transaction.ToAddress,
            *Transaction.TokenSymbol,
            Transaction.Amount,
            Transaction.GasFee,
            Transaction.GasPrice,
            (int32)Transaction.Status,
            Transaction.Timestamp
        );
    }
    
    return TEXT("");
}

FBlockchainTransaction UMingBlockchainTransactionManager::ParseTransactionFromImport(const FString& Data, const FString& Format) const
{
    FBlockchainTransaction Transaction;
    
    if (Format.ToUpper() == TEXT("JSON"))
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Data);
        
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            Transaction.TransactionHash = JsonObject->GetStringField(TEXT("hash"));
            Transaction.FromAddress = JsonObject->GetStringField(TEXT("from"));
            Transaction.ToAddress = JsonObject->GetStringField(TEXT("to"));
            Transaction.TokenContract = JsonObject->GetStringField(TEXT("token"));
            Transaction.Amount = JsonObject->GetNumberField(TEXT("amount"));
            Transaction.GasFee = JsonObject->GetNumberField(TEXT("gasFee"));
            Transaction.GasPrice = JsonObject->GetNumberField(TEXT("gasPrice"));
            Transaction.Status = (ETransactionStatus)JsonObject->GetIntegerField(TEXT("status"));
            Transaction.Timestamp = JsonObject->GetNumberField(TEXT("timestamp"));
        }
    }
    
    return Transaction;
}

FString UMingBlockchainTransactionManager::GetTokenSymbol(const FString& TokenContract) const
{
    // In a real implementation, this would query the token contract
    if (TokenContract.IsEmpty() || TokenContract == TEXT("0x0000000000000000000000000000000000000"))
    {
        return TEXT("ETH");
    }
    else if (TokenContract.Contains(TEXT("12345678")))
    {
        return TEXT("MING");
    }
    else if (TokenContract.Contains(TEXT("abcdef12")))
    {
        return TEXT("NFT");
    }
    
    return TEXT("UNKNOWN");
}

bool UMingBlockchainTransactionManager::IsContractAddress(const FString& Address) const
{
    // In a real implementation, this would check if the address is a contract
    return Address.Len() == 42 && Address.StartsWith(TEXT("0x"));
}

FString UMingBlockchainTransactionManager::GetTransactionType(const FBlockchainTransaction& Transaction) const
{
    if (Transaction.TokenContract.IsEmpty() || Transaction.TokenContract == TEXT("0x0000000000000000000000000000000000000"))
    {
        return TEXT("ETH Transfer");
    }
    else
    {
        return TEXT("Token Transfer");
    }
}
