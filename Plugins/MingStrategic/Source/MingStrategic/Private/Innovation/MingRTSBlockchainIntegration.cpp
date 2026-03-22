#include "Innovation/MingRTSBlockchainIntegration.h"
#include "Math/UnrealMathUtility.h"

UMingRTSBlockchainIntegration::UMingRTSBlockchainIntegration()
    : CurrentChain(EBlockchainType::Private)
    , bIsConnected(false)
    , BlockNumber(0)
{
}

void UMingRTSBlockchainIntegration::InitializeBlockchain(EBlockchainType ChainType)
{
    CurrentChain = ChainType;
    bIsConnected = false;
    BlockNumber = 0;
    LastBlockHash = TEXT("0x");

    ConnectToChain(ChainType);
}

FBlockchainAccount UMingRTSBlockchainIntegration::CreateAccount(const FString& PlayerId)
{
    FBlockchainAccount Account;

    // 生成模擬的區塊鏈地址
    FString AddressPrefix;
    switch (CurrentChain)
    {
    case EBlockchainType::Ethereum:
    case EBlockchainType::Polygon:
        AddressPrefix = TEXT("0x");
        break;
    case EBlockchainType::BinanceSmartChain:
        AddressPrefix = TEXT("0x");
        break;
    case EBlockchainType::Solana:
        AddressPrefix = TEXT("");
        break;
    case EBlockchainType::Private:
        AddressPrefix = TEXT("0xMG");
        break;
    default:
        AddressPrefix = TEXT("0x");
        break;
    }

    // 生成40字符的十六進制地址
    FString RandomHex;
    for (int32 i = 0; i < 40; ++i)
    {
        RandomHex += FString::Printf(TEXT("%x"), FMath::RandRange(0, 15));
    }

    Account.Address = AddressPrefix + RandomHex;
    Account.PrivateKeyHash = FString::Printf(TEXT("0x%x"), FMath::RandRange(0, MAX_int32));
    Account.Balance = 0.0f;
    Account.bIsActive = true;

    Accounts.Add(PlayerId, Account);

    return Account;
}

FString UMingRTSBlockchainIntegration::CreateTransaction(const FString& FromAddress, const FString& ToAddress, float Value, const FString& Data)
{
    if (!bIsConnected)
    {
        UE_LOG(LogTemp, Warning, TEXT("Blockchain not connected"));
        return TEXT("");
    }

    // 驗證發送方地址
    bool bValidSender = false;
    for (const auto& Pair : Accounts)
    {
        if (Pair.Value.Address == FromAddress)
        {
            bValidSender = true;
            if (Pair.Value.Balance < Value)
            {
                UE_LOG(LogTemp, Warning, TEXT("Insufficient balance"));
                return TEXT("");
            }
            break;
        }
    }

    if (!bValidSender)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid sender address"));
        return TEXT("");
    }

    FBlockchainTransaction Transaction;
    Transaction.FromAddress = FromAddress;
    Transaction.ToAddress = ToAddress;
    Transaction.Value = Value;
    Transaction.Data = Data;
    Transaction.Timestamp = FDateTime::Now().ToString();
    Transaction.bIsConfirmed = false;
    Transaction.Confirmations = 0;

    // 生成交易哈希
    FString HashData = FromAddress + ToAddress + FString::SanitizeFloat(Value) + Transaction.Timestamp;
    uint32 HashValue = FCString::Strlen(*HashData);
    for (int32 i = 0; i < HashData.Len(); ++i)
    {
        HashValue = HashValue * 31 + HashData[i];
    }
    Transaction.TransactionHash = FString::Printf(TEXT("0x%x"), HashValue);

    Transactions.Add(Transaction);

    // 更新餘額
    for (auto& Pair : Accounts)
    {
        if (Pair.Value.Address == FromAddress)
        {
            Pair.Value.Balance -= Value;
        }
        if (Pair.Value.Address == ToAddress)
        {
            Pair.Value.Balance += Value;
        }
    }

    ProcessPendingTransactions();

    return Transaction.TransactionHash;
}

void UMingRTSBlockchainIntegration::MintToken(const FString& ToAddress, ETokenType TokenType, const FString& TokenId, float Amount)
{
    if (!bIsConnected)
    {
        return;
    }

    // 創造代幣鑄造交易
    FString TokenData = FString::Printf(TEXT("Mint_%s_%s_%.6f"),
        *UEnum::GetValueAsString(TokenType),
        *TokenId,
        Amount);

    // 使用特殊地址作為發送方（鑄造地址）
    FString MintAddress = TEXT("0x0000000000000000000000000000000000000000");
    CreateTransaction(MintAddress, ToAddress, 0.0f, TokenData);

    UE_LOG(LogTemp, Log, TEXT("Minted %f tokens of type %s to address %s"),
        Amount,
        *UEnum::GetValueAsString(TokenType),
        *ToAddress);
}

bool UMingRTSBlockchainIntegration::VerifyTransaction(const FString& TransactionHash)
{
    for (const auto& Transaction : Transactions)
    {
        if (Transaction.TransactionHash == TransactionHash)
        {
            return Transaction.bIsConfirmed && Transaction.Confirmations >= 6;
        }
    }
    return false;
}

float UMingRTSBlockchainIntegration::GetBalance(const FString& Address) const
{
    for (const auto& Pair : Accounts)
    {
        if (Pair.Value.Address == Address)
        {
            return Pair.Value.Balance;
        }
    }
    return 0.0f;
}

void UMingRTSBlockchainIntegration::ConnectToChain(EBlockchainType ChainType)
{
    UE_LOG(LogTemp, Log, TEXT("Connecting to blockchain: %s"), *UEnum::GetValueAsString(ChainType));

    // 模擬連接延遲
    float ConnectionTime = FMath::RandRange(0.5f, 2.0f);

    // 模擬連接成功
    bIsConnected = true;
    BlockNumber = FMath::RandRange(1000000, 9999999);
    LastBlockHash = FString::Printf(TEXT("0x%x"), FMath::RandRange(0, MAX_int32));

    UE_LOG(LogTemp, Log, TEXT("Connected to %s. Block number: %d"),
        *UEnum::GetValueAsString(ChainType),
        BlockNumber);
}

void UMingRTSBlockchainIntegration::ProcessPendingTransactions()
{
    for (auto& Transaction : Transactions)
    {
        if (!Transaction.bIsConfirmed)
        {
            // 模擬確認過程
            float ConfirmChance = 0.3f;
            if (FMath::RandRange(0.0f, 1.0f) < ConfirmChance)
            {
                Transaction.bIsConfirmed = true;
                Transaction.Confirmations = FMath::RandRange(1, 12);

                UE_LOG(LogTemp, Log, TEXT("Transaction %s confirmed with %d confirmations"),
                    *Transaction.TransactionHash,
                    Transaction.Confirmations);
            }
        }
        else
        {
            // 增加確認數
            if (Transaction.Confirmations < 12 && FMath::RandRange(0.0f, 1.0f) < 0.5f)
            {
                Transaction.Confirmations++;
            }
        }
    }
}
