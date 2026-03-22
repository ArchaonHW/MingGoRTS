#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSBlockchainIntegration.generated.h"

UENUM(BlueprintType)
enum class EBlockchainType : uint8
{
    Ethereum UMETA(DisplayName = "以太坊"),
    BinanceSmartChain UMETA(DisplayName = "幣安智能鏈"),
    Polygon UMETA(DisplayName = "Polygon"),
    Solana UMETA(DisplayName = "Solana"),
    Private UMETA(DisplayName = "私有鏈"),
    Count UMETA(DisplayName = "區塊鏈類型數量")
};

UENUM(BlueprintType)
enum class ETokenType : uint8
{
    ERC20 UMETA(DisplayName = "ERC20"),
    ERC721 UMETA(DisplayName = "ERC721"),
    ERC1155 UMETA(DisplayName = "ERC1155"),
    Custom UMETA(DisplayName = "自定義"),
    Count UMETA(DisplayName = "代幣類型數量")
};

USTRUCT(BlueprintType)
struct FBlockchainAccount
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString Address;

    UPROPERTY(BlueprintReadWrite)
    FString PrivateKeyHash;

    UPROPERTY(BlueprintReadWrite)
    float Balance;

    UPROPERTY(BlueprintReadWrite)
    bool bIsActive;

    FBlockchainAccount()
        : Balance(0.0f)
        , bIsActive(false)
    {}
};

USTRUCT(BlueprintType)
struct FBlockchainTransaction
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString TransactionHash;

    UPROPERTY(BlueprintReadWrite)
    FString FromAddress;

    UPROPERTY(BlueprintReadWrite)
    FString ToAddress;

    UPROPERTY(BlueprintReadWrite)
    float Value;

    UPROPERTY(BlueprintReadWrite)
    FString Data;

    UPROPERTY(BlueprintReadWrite)
    FString Timestamp;

    UPROPERTY(BlueprintReadWrite)
    bool bIsConfirmed;

    UPROPERTY(BlueprintReadWrite)
    int32 Confirmations;

    FBlockchainTransaction()
        : Value(0.0f)
        , bIsConfirmed(false)
        , Confirmations(0)
    {}
};

UCLASS(ClassGroup = (MingStrategic), BlueprintType)
class MINGSTRATEGIC_API UMingRTSBlockchainIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSBlockchainIntegration();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Blockchain|System")
    void InitializeBlockchain(EBlockchainType ChainType);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Blockchain|Account")
    FBlockchainAccount CreateAccount(const FString& PlayerId);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Blockchain|Transaction")
    FString CreateTransaction(const FString& FromAddress, const FString& ToAddress, float Value, const FString& Data);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Blockchain|Token")
    void MintToken(const FString& ToAddress, ETokenType TokenType, const FString& TokenId, float Amount);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Blockchain|Transaction")
    bool VerifyTransaction(const FString& TransactionHash);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Blockchain|Account")
    float GetBalance(const FString& Address) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Blockchain|System")
    bool IsConnected() const { return bIsConnected; }

private:
    UPROPERTY()
    EBlockchainType CurrentChain;

    UPROPERTY()
    TMap<FString, FBlockchainAccount> Accounts;

    UPROPERTY()
    TArray<FBlockchainTransaction> Transactions;

    UPROPERTY()
    bool bIsConnected;

    UPROPERTY()
    FString LastBlockHash;

    UPROPERTY()
    int32 BlockNumber;

    void ConnectToChain(EBlockchainType ChainType);
    void ProcessPendingTransactions();
};
