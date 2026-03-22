#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "MingBlockchainWallet.h"
#include "MingBlockchainNFTSystem.generated.h"

/**
 * NFT rarity levels
 */
UENUM(BlueprintType)
enum class ENFTRarity : uint8
{
    Common     = 0,
    Uncommon   = 1,
    Rare       = 2,
    Epic       = 3,
    Legendary  = 4,
    Mythic     = 5
};

/**
 * NFT categories
 */
UENUM(BlueprintType)
enum class ENFTCategory : uint8
{
    Character    = 0,
    Weapon      = 1,
    Armor       = 2,
    Building    = 3,
    Resource    = 4,
    Artifact    = 5,
    Cosmetic    = 6,
    Land        = 7,
    Title       = 8,
    Special     = 9
};

/**
 * NFT metadata structure
 */
USTRUCT(BlueprintType)
struct FNFTMetadata
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FString ImageURL;

    UPROPERTY(BlueprintReadOnly)
    FString AnimationURL;

    UPROPERTY(BlueprintReadOnly)
    ENFTCategory Category;

    UPROPERTY(BlueprintReadOnly)
    ENFTRarity Rarity;

    UPROPERTY(BlueprintReadOnly)
    int32 Level;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Attributes;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CustomProperties;

    UPROPERTY(BlueprintReadOnly)
    FString Creator;

    UPROPERTY(BlueprintReadOnly)
    FString Collection;

    UPROPERTY(BlueprintReadOnly)
    float CreationDate;

    FNFTMetadata()
    {
        Name = TEXT("");
        Description = TEXT("");
        ImageURL = TEXT("");
        AnimationURL = TEXT("");
        Category = ENFTCategory::Character;
        Rarity = ENFTRarity::Common;
        Level = 1;
        Attributes.Empty();
        CustomProperties.Empty();
        Creator = TEXT("");
        Collection = TEXT("");
        CreationDate = 0.0f;
    }
};

/**
 * NFT asset data
 */
USTRUCT(BlueprintType)
struct FNFTAsset
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TokenID;

    UPROPERTY(BlueprintReadOnly)
    FString ContractAddress;

    UPROPERTY(BlueprintReadOnly)
    FString OwnerAddress;

    UPROPERTY(BlueprintReadOnly)
    FNFTMetadata Metadata;

    UPROPERTY(BlueprintReadOnly)
    bool IsTransferable;

    UPROPERTY(BlueprintReadOnly)
    bool IsBurnable;

    UPROPERTY(BlueprintReadOnly)
    float LastTransferDate;

    UPROPERTY(BlueprintReadOnly)
    int32 TransferCount;

    UPROPERTY(BlueprintReadOnly)
    float EstimatedValue;

    UPROPERTY(BlueprintReadOnly)
    FString BlockchainNetwork;

    FNFTAsset()
    {
        TokenID = TEXT("");
        ContractAddress = TEXT("");
        OwnerAddress = TEXT("");
        Metadata = FNFTMetadata();
        IsTransferable = true;
        IsBurnable = false;
        LastTransferDate = 0.0f;
        TransferCount = 0;
        EstimatedValue = 0.0f;
        BlockchainNetwork = TEXT("");
    }
};

/**
 * NFT collection data
 */
USTRUCT(BlueprintType)
struct FNFTCollection
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CollectionID;

    UPROPERTY(BlueprintReadOnly)
    FString Name;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FString CreatorAddress;

    UPROPERTY(BlueprintReadOnly)
    FString ImageURL;

    UPROPERTY(BlueprintReadOnly)
    int32 TotalSupply;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxSupply;

    UPROPERTY(BlueprintReadOnly)
    float FloorPrice;

    UPROPERTY(BlueprintReadOnly)
    float TotalVolume;

    UPROPERTY(BlueprintReadOnly)
    int32 HolderCount;

    UPROPERTY(BlueprintReadOnly)
    bool IsActive;

    UPROPERTY(BlueprintReadOnly)
    float CreationDate;

    FNFTCollection()
    {
        CollectionID = TEXT("");
        Name = TEXT("");
        Description = TEXT("");
        CreatorAddress = TEXT("");
        ImageURL = TEXT("");
        TotalSupply = 0;
        MaxSupply = 0;
        FloorPrice = 0.0f;
        TotalVolume = 0.0f;
        HolderCount = 0;
        IsActive = true;
        CreationDate = 0.0f;
    }
};

/**
 * NFT marketplace listing
 */
USTRUCT(BlueprintType)
struct FNFTMarketplaceListing
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ListingID;

    UPROPERTY(BlueprintReadOnly)
    FNFTAsset Asset;

    UPROPERTY(BlueprintReadOnly)
    FString SellerAddress;

    UPROPERTY(BlueprintReadOnly)
    float Price;

    UPROPERTY(BlueprintReadOnly)
    FString Currency;

    UPROPERTY(BlueprintReadOnly)
    float ExpirationDate;

    UPROPERTY(BlueprintReadOnly)
    bool IsActive;

    UPROPERTY(BlueprintReadOnly)
    FString BidToken;

    UPROPERTY(BlueprintReadOnly)
    float HighestBid;

    UPROPERTY(BlueprintReadOnly)
    int32 BidCount;

    FNFTMarketplaceListing()
    {
        ListingID = TEXT("");
        Asset = FNFTAsset();
        SellerAddress = TEXT("");
        Price = 0.0f;
        Currency = TEXT("ETH");
        ExpirationDate = 0.0f;
        IsActive = true;
        BidToken = TEXT("");
        HighestBid = 0.0f;
        BidCount = 0;
    }
};

// Forward declarations
class UMingBlockchainWallet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNFTMinted, const FNFTAsset&, NFT);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNFTTransferred, const FNFTAsset&, NFT, const FString&, ToAddress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNFTBurned, const FString&, TokenID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNFTListed, const FNFTMarketplaceListing&, Listing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNFTSold, const FNFTMarketplaceListing&, Listing);

/**
 * MingBlockchainNFTSystem
 * 
 * Comprehensive NFT (Non-Fungible Token) system for MingGoRTS that enables
 * creation, management, and trading of unique in-game assets as NFTs on the
 * blockchain. Supports character NFTs, item NFTs, land NFTs, and more.
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup=(MingBlockchain), meta=(BlueprintSpawnableComponent))
class MINGBLOCKCHAIN_API UMingBlockchainNFTSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingBlockchainNFTSystem();

    /**
     * Initialize NFT system
     * @param Wallet - Reference to blockchain wallet
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    void InitializeNFTSystem(UMingBlockchainWallet* Wallet);

    /**
     * Mint a new NFT
     * @param Metadata - NFT metadata
     * @param OwnerAddress - Owner of the NFT
     * @return NFT asset if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FNFTAsset MintNFT(const FNFTMetadata& Metadata, const FString& OwnerAddress);

    /**
     * Mint NFT from in-game asset
     * @param AssetID - In-game asset identifier
     * @param OwnerAddress - Owner of the NFT
     * @return NFT asset if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FNFTAsset MintNFTFromAsset(const FString& AssetID, const FString& OwnerAddress);

    /**
     * Transfer NFT to another address
     * @param TokenID - NFT token ID
     * @param ToAddress - Recipient address
     * @return True if transfer initiated successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    bool TransferNFT(const FString& TokenID, const FString& ToAddress);

    /**
     * Burn (destroy) an NFT
     * @param TokenID - NFT token ID
     * @return True if burn successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    bool BurnNFT(const FString& TokenID);

    /**
     * Get NFT by token ID
     * @param TokenID - NFT token ID
     * @return NFT asset data
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FNFTAsset GetNFT(const FString& TokenID) const;

    /**
     * Get all NFTs owned by address
     * @param OwnerAddress - Owner address
     * @return Array of owned NFTs
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    TArray<FNFTAsset> GetOwnedNFTs(const FString& OwnerAddress) const;

    /**
     * Get NFT collection
     * @param CollectionID - Collection identifier
     * @return Collection data
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FNFTCollection GetNFTCollection(const FString& CollectionID) const;

    /**
     * Create new NFT collection
     * @param Name - Collection name
     * @param Description - Collection description
     * @param CreatorAddress - Creator address
     * @param MaxSupply - Maximum supply (0 for unlimited)
     * @return Collection ID if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FString CreateNFTCollection(const FString& Name, const FString& Description, const FString& CreatorAddress, int32 MaxSupply = 0);

    /**
     * List NFT on marketplace
     * @param TokenID - NFT token ID
     * @param Price - Listing price
     * @param Currency - Currency type
     * @param Duration - Listing duration in seconds
     * @return Listing ID if successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FString ListNFTOnMarketplace(const FString& TokenID, float Price, const FString& Currency = TEXT("ETH"), float Duration = 604800.0f); // 7 days

    /**
     * Buy NFT from marketplace
     * @param ListingID - Listing identifier
     * @param BuyerAddress - Buyer address
     * @return True if purchase successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    bool BuyNFTFromMarketplace(const FString& ListingID, const FString& BuyerAddress);

    /**
     * Place bid on NFT
     * @param ListingID - Listing identifier
     * @param BidderAddress - Bidder address
     * @param BidAmount - Bid amount
     * @return True if bid placed successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    bool PlaceBid(const FString& ListingID, const FString& BidderAddress, float BidAmount);

    /**
     * Cancel NFT listing
     * @param ListingID - Listing identifier
     * @return True if cancelled successfully
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    bool CancelNFTListing(const FString& ListingID);

    /**
     * Get marketplace listings
     * @param Category - Filter by category (optional)
     * @param MinPrice - Minimum price filter (optional)
     * @param MaxPrice - Maximum price filter (optional)
     * @param Limit - Maximum number of listings to return
     * @return Array of marketplace listings
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    TArray<FNFTMarketplaceListing> GetMarketplaceListings(ENFTCategory Category = ENFTCategory::Special, float MinPrice = 0.0f, float MaxPrice = 0.0f, int32 Limit = 100) const;

    /**
     * Get NFT rarity distribution
     * @param CollectionID - Collection identifier
     * @return Map of rarity to count
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    TMap<ENFTRarity, int32> GetNFTDistribution(const FString& CollectionID) const;

    /**
     * Estimate NFT value
     * @param TokenID - NFT token ID
     * @return Estimated value in ETH
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    float EstimateNFTValue(const FString& TokenID) const;

    /**
     * Get NFT metadata from blockchain
     * @param TokenID - NFT token ID
     * @return NFT metadata
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FNFTMetadata GetNFTMetadata(const FString& TokenID) const;

    /**
     * Update NFT metadata
     * @param TokenID - NFT token ID
     * @param NewMetadata - New metadata
     * @return True if update successful
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    bool UpdateNFTMetadata(const FString& TokenID, const FNFTMetadata& NewMetadata);

    /**
     * Get NFT creation cost
     * @param Rarity - NFT rarity
     * @return Creation cost in ETH
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    float GetNFTCreationCost(ENFTRarity Rarity) const;

    /**
     * Get supported NFT categories
     * @return Array of supported categories
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    TArray<ENFTCategory> GetSupportedCategories() const;

    /**
     * Get rarity name
     * @param Rarity - Rarity level
     * @return Rarity name string
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FString GetRarityName(ENFTRarity Rarity) const;

    /**
     * Get category name
     * @param Category - NFT category
     * @return Category name string
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FString GetCategoryName(ENFTCategory Category) const;

    /**
     * Validate NFT metadata
     * @param Metadata - NFT metadata to validate
     * @return True if valid
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    bool ValidateNFTMetadata(const FNFTMetadata& Metadata) const;

    /**
     * Generate NFT image URL
     * @param TokenID - NFT token ID
     * @return Image URL
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FString GenerateNFTImageURL(const FString& TokenID) const;

    /**
     * Generate NFT metadata JSON
     * @param Metadata - NFT metadata
     * @return JSON string
     */
    UFUNCTION(BlueprintCallable, Category = "Blockchain|NFT")
    FString GenerateNFTMetadataJSON(const FNFTMetadata& Metadata) const;

public:
    /** Delegate fired when an NFT is minted */
    UPROPERTY(BlueprintAssignable)
    FOnNFTMinted OnNFTMinted;

    /** Delegate fired when an NFT is transferred */
    UPROPERTY(BlueprintAssignable)
    FOnNFTTransferred OnNFTTransferred;

    /** Delegate fired when an NFT is burned */
    UPROPERTY(BlueprintAssignable)
    FOnNFTBurned OnNFTBurned;

    /** Delegate fired when an NFT is listed on marketplace */
    UPROPERTY(BlueprintAssignable)
    FOnNFTListed OnNFTListed;

    /** Delegate fired when an NFT is sold on marketplace */
    UPROPERTY(BlueprintAssignable)
    FOnNFTSold OnNFTSold;

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

    /** Owned NFTs by address */
    UPROPERTY()
    TMap<FString, TArray<FNFTAsset>> OwnedNFTs;

    /** NFT collections */
    UPROPERTY()
    TMap<FString, FNFTCollection> NFTCollections;

    /** Marketplace listings */
    UPROPERTY()
    TArray<FNFTMarketplaceListing> MarketplaceListings;

    /** NFT creation costs by rarity */
    UPROPERTY()
    TMap<ENFTRarity, float> NFTCreationCosts;

    /** Default NFT contract address */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFT|Configuration")
    FString DefaultNFTContract;

    /** Marketplace fee percentage */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFT|Configuration")
    float MarketplaceFeePercentage;

    /** Maximum marketplace listings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFT|Configuration")
    int32 MaxMarketplaceListings;

    /** NFT transfer cooldown in seconds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFT|Configuration")
    float NFTTransferCooldown;

    /** Whether NFT system is enabled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFT|Configuration")
    bool bNFTSystemEnabled;

    /** Time since last marketplace update */
    float TimeSinceLastMarketplaceUpdate;

    /** Marketplace update interval */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NFT|Configuration")
    float MarketplaceUpdateInterval;

    /** Initialize NFT configuration */
    void InitializeNFTConfiguration();

    /** Generate unique token ID */
    FString GenerateTokenID() const;

    /** Generate unique collection ID */
    FString GenerateCollectionID() const;

    /** Generate unique listing ID */
    FString GenerateListingID() const;

    /** Mint NFT on blockchain */
    bool MintNFTOnBlockchain(FNFTAsset& NFT);

    /** Transfer NFT on blockchain */
    bool TransferNFTOnBlockchain(const FString& TokenID, const FString& FromAddress, const FString& ToAddress);

    /** Burn NFT on blockchain */
    bool BurnNFTOnBlockchain(const FString& TokenID, const FString& OwnerAddress);

    /** Update marketplace listings */
    void UpdateMarketplaceListings();

    /** Process expired listings */
    void ProcessExpiredListings();

    /** Calculate NFT rarity probability */
    float GetRarityProbability(ENFTRarity Rarity) const;

    /** Get NFT rarity from random roll */
    ENFTRarity GetRandomRarity() const;

    /** Get NFT creation cost including fees */
    float GetTotalNFTCreationCost(ENFTRarity Rarity) const;

    /** Validate NFT ownership */
    bool ValidateNFTOwnership(const FString& TokenID, const FString& OwnerAddress) const;

    /** Get marketplace fee amount */
    float GetMarketplaceFee(float Price) const;

    /** Save NFT data */
    void SaveNFTData();

    /** Load NFT data */
    void LoadNFTData();

    /** Clean up expired data */
    void CleanupExpiredData();

    /** Get NFT image base URL */
    FString GetNFTImageBaseURL() const;

    /** Format NFT metadata for blockchain */
    FString FormatMetadataForBlockchain(const FNFTMetadata& Metadata) const;

    /** Parse metadata from blockchain */
    FNFTMetadata ParseMetadataFromBlockchain(const FString& MetadataJSON) const;

    /** Get NFT attributes as string */
    FString GetNFTAttributesString(const TArray<FString>& Attributes) const;

    /** Calculate NFT estimated value */
    float CalculateNFTValue(const FNFTAsset& NFT) const;

    /** Check if NFT is transferable */
    bool IsNFTTransferable(const FNFTAsset& NFT) const;

    /** Get NFT transfer fee */
    float GetNFTTransferFee(const FNFTAsset& NFT) const;
};
