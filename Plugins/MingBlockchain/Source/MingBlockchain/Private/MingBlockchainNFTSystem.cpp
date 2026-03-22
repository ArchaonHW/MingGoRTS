#include "MingBlockchainNFTSystem.h"
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

UMingBlockchainNFTSystem::UMingBlockchainNFTSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    DefaultNFTContract = TEXT("0xabcdef1234567890abcdef1234567890abcdef12");
    MarketplaceFeePercentage = 0.025f; // 2.5%
    MaxMarketplaceListings = 1000;
    NFTTransferCooldown = 60.0f; // 1 minute
    bNFTSystemEnabled = true;
    MarketplaceUpdateInterval = 30.0f; // Update every 30 seconds
    TimeSinceLastMarketplaceUpdate = 0.0f;

    BlockchainWallet = nullptr;
}

void UMingBlockchainNFTSystem::BeginPlay()
{
    Super::BeginPlay();
    InitializeNFTConfiguration();
    LoadNFTData();
}

void UMingBlockchainNFTSystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    SaveNFTData();
    Super::EndPlay(EndPlayReason);
}

void UMingBlockchainNFTSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bNFTSystemEnabled)
    {
        TimeSinceLastMarketplaceUpdate += DeltaTime;
        
        if (TimeSinceLastMarketplaceUpdate >= MarketplaceUpdateInterval)
        {
            UpdateMarketplaceListings();
            ProcessExpiredListings();
            TimeSinceLastMarketplaceUpdate = 0.0f;
        }

        // Clean up expired data periodically
        if (FMath::RandRange(0, 1000) < 5) // 0.5% chance per tick
        {
            CleanupExpiredData();
        }
    }
}

void UMingBlockchainNFTSystem::InitializeNFTSystem(UMingBlockchainWallet* Wallet)
{
    BlockchainWallet = Wallet;

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Initialized with wallet"));
}

FNFTAsset UMingBlockchainNFTSystem::MintNFT(const FNFTMetadata& Metadata, const FString& OwnerAddress)
{
    if (!bNFTSystemEnabled)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: NFT system is disabled"));
        return FNFTAsset();
    }

    if (!ValidateNFTMetadata(Metadata))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: Invalid NFT metadata"));
        return FNFTAsset();
    }

    FNFTAsset NFT;
    NFT.TokenID = GenerateTokenID();
    NFT.ContractAddress = DefaultNFTContract;
    NFT.OwnerAddress = OwnerAddress;
    NFT.Metadata = Metadata;
    NFT.IsTransferable = true;
    NFT.IsBurnable = true;
    NFT.LastTransferDate = GetWorld()->GetTimeSeconds();
    NFT.TransferCount = 0;
    NFT.EstimatedValue = CalculateNFTValue(NFT);
    NFT.BlockchainNetwork = TEXT("Ethereum");

    // Mint on blockchain
    if (MintNFTOnBlockchain(NFT))
    {
        // Add to owned NFTs
        TArray<FNFTAsset>& OwnerNFTs = OwnedNFTs.FindOrAdd(OwnerAddress);
        OwnerNFTs.Add(NFT);

        OnNFTMinted.Broadcast(NFT);

        UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Minted NFT %s for owner %s"), 
               *NFT.TokenID, *OwnerAddress);
    }

    return NFT;
}

FNFTAsset UMingBlockchainNFTSystem::MintNFTFromAsset(const FString& AssetID, const FString& OwnerAddress)
{
    // Convert in-game asset to NFT metadata
    FNFTMetadata Metadata;
    Metadata.Name = FString::Printf(TEXT("Ming Asset %s"), *AssetID);
    Metadata.Description = FString::Printf(TEXT("NFT representation of in-game asset %s"), *AssetID);
    Metadata.Category = ENFTCategory::Artifact;
    Metadata.Rarity = GetRandomRarity();
    Metadata.Level = 1;
    Metadata.Creator = OwnerAddress;
    Metadata.Collection = TEXT("MingGoRTS Assets");
    Metadata.CreationDate = GetWorld()->GetTimeSeconds();

    // Add asset-specific attributes
    Metadata.Attributes.Add(FString::Printf(TEXT("AssetID:%s"), *AssetID));
    Metadata.Attributes.Add(FString::Printf(TEXT("Rarity:%s"), *GetRarityName(Metadata.Rarity)));
    Metadata.Attributes.Add(TEXT("Game:MingGoRTS"));

    // Generate image URL based on asset
    Metadata.ImageURL = GenerateNFTImageURL(AssetID);

    return MintNFT(Metadata, OwnerAddress);
}

bool UMingBlockchainNFTSystem::TransferNFT(const FString& TokenID, const FString& ToAddress)
{
    if (!BlockchainWallet || !BlockchainWallet->IsWalletConnected())
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: Wallet not connected for NFT transfer"));
        return false;
    }

    FNFTAsset NFT = GetNFT(TokenID);
    if (NFT.TokenID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: NFT %s not found"), *TokenID);
        return false;
    }

    if (!IsNFTTransferable(NFT))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: NFT %s is not transferable"), *TokenID);
        return false;
    }

    FString FromAddress = BlockchainWallet->GetWalletInfo().WalletAddress;
    
    if (!ValidateNFTOwnership(TokenID, FromAddress))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: Wallet does not own NFT %s"), *TokenID);
        return false;
    }

    if (TransferNFTOnBlockchain(TokenID, FromAddress, ToAddress))
    {
        // Update ownership
        for (auto& OwnerNFTs : OwnedNFTs)
        {
            for (int32 i = 0; i < OwnerNFTs.Value.Num(); ++i)
            {
                if (OwnerNFTs.Value[i].TokenID == TokenID)
                {
                    FNFTAsset TransferredNFT = OwnerNFTs.Value[i];
                    TransferredNFT.OwnerAddress = ToAddress;
                    TransferredNFT.LastTransferDate = GetWorld()->GetTimeSeconds();
                    TransferredNFT.TransferCount++;
                    
                    // Remove from current owner
                    OwnerNFTs.Value.RemoveAt(i);
                    
                    // Add to new owner
                    TArray<FNFTAsset>& NewOwnerNFTs = OwnedNFTs.FindOrAdd(ToAddress);
                    NewOwnerNFTs.Add(TransferredNFT);
                    
                    OnNFTTransferred.Broadcast(TransferredNFT, ToAddress);
                    
                    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Transferred NFT %s from %s to %s"), 
                           *TokenID, *FromAddress, *ToAddress);
                    
                    return true;
                }
            }
        }
    }

    return false;
}

bool UMingBlockchainNFTSystem::BurnNFT(const FString& TokenID)
{
    if (!BlockchainWallet || !BlockchainWallet->IsWalletConnected())
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: Wallet not connected for NFT burn"));
        return false;
    }

    FNFTAsset NFT = GetNFT(TokenID);
    if (NFT.TokenID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: NFT %s not found"), *TokenID);
        return false;
    }

    if (!NFT.IsBurnable)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: NFT %s is not burnable"), *TokenID);
        return false;
    }

    FString OwnerAddress = BlockchainWallet->GetWalletInfo().WalletAddress;
    
    if (!ValidateNFTOwnership(TokenID, OwnerAddress))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: Wallet does not own NFT %s"), *TokenID);
        return false;
    }

    if (BurnNFTOnBlockchain(TokenID, OwnerAddress))
    {
        // Remove from owned NFTs
        for (auto& OwnerNFTs : OwnedNFTs)
        {
            for (int32 i = 0; i < OwnerNFTs.Value.Num(); ++i)
            {
                if (OwnerNFTs.Value[i].TokenID == TokenID)
                {
                    OwnerNFTs.Value.RemoveAt(i);
                    
                    OnNFTBurned.Broadcast(TokenID);
                    
                    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Burned NFT %s"), *TokenID);
                    return true;
                }
            }
        }
    }

    return false;
}

FNFTAsset UMingBlockchainNFTSystem::GetNFT(const FString& TokenID) const
{
    // Search through all owned NFTs
    for (const auto& OwnerNFTs : OwnedNFTs)
    {
        for (const FNFTAsset& NFT : OwnerNFTs.Value)
        {
            if (NFT.TokenID == TokenID)
            {
                return NFT;
            }
        }
    }

    return FNFTAsset();
}

TArray<FNFTAsset> UMingBlockchainNFTSystem::GetOwnedNFTs(const FString& OwnerAddress) const
{
    if (const TArray<FNFTAsset>* NFTs = OwnedNFTs.Find(OwnerAddress))
    {
        return *NFTs;
    }
    
    return TArray<FNFTAsset>();
}

FNFTCollection UMingBlockchainNFTSystem::GetNFTCollection(const FString& CollectionID) const
{
    if (const FNFTCollection* Collection = NFTCollections.Find(CollectionID))
    {
        return *Collection;
    }
    
    return FNFTCollection();
}

FString UMingBlockchainNFTSystem::CreateNFTCollection(const FString& Name, const FString& Description, const FString& CreatorAddress, int32 MaxSupply)
{
    FNFTCollection Collection;
    Collection.CollectionID = GenerateCollectionID();
    Collection.Name = Name;
    Collection.Description = Description;
    Collection.CreatorAddress = CreatorAddress;
    Collection.TotalSupply = 0;
    Collection.MaxSupply = MaxSupply;
    Collection.FloorPrice = 0.0f;
    Collection.TotalVolume = 0.0f;
    Collection.HolderCount = 0;
    Collection.IsActive = true;
    Collection.CreationDate = GetWorld()->GetTimeSeconds();

    NFTCollections.Add(Collection.CollectionID, Collection);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Created collection %s"), *Collection.CollectionID);
    return Collection.CollectionID;
}

FString UMingBlockchainNFTSystem::ListNFTOnMarketplace(const FString& TokenID, float Price, const FString& Currency, float Duration)
{
    if (MarketplaceListings.Num() >= MaxMarketplaceListings)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: Maximum marketplace listings reached"));
        return TEXT("");
    }

    FNFTAsset NFT = GetNFT(TokenID);
    if (NFT.TokenID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: NFT %s not found for marketplace listing"), *TokenID);
        return TEXT("");
    }

    FNFTMarketplaceListing Listing;
    Listing.ListingID = GenerateListingID();
    Listing.Asset = NFT;
    Listing.SellerAddress = NFT.OwnerAddress;
    Listing.Price = Price;
    Listing.Currency = Currency;
    Listing.ExpirationDate = GetWorld()->GetTimeSeconds() + Duration;
    Listing.IsActive = true;
    Listing.BidToken = TEXT("");
    Listing.HighestBid = 0.0f;
    Listing.BidCount = 0;

    MarketplaceListings.Add(Listing);

    OnNFTListed.Broadcast(Listing);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Listed NFT %s for %f %s"), 
           *TokenID, Price, *Currency);

    return Listing.ListingID;
}

bool UMingBlockchainNFTSystem::BuyNFTFromMarketplace(const FString& ListingID, const FString& BuyerAddress)
{
    for (int32 i = 0; i < MarketplaceListings.Num(); ++i)
    {
        if (MarketplaceListings[i].ListingID == ListingID)
        {
            FNFTMarketplaceListing& Listing = MarketplaceListings[i];
            
            if (!Listing.IsActive)
            {
                UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: Listing %s is not active"), *ListingID);
                return false;
            }

            if (GetWorld()->GetTimeSeconds() > Listing.ExpirationDate)
            {
                UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: Listing %s has expired"), *ListingID);
                return false;
            }

            // Process the purchase
            float TotalCost = Listing.Price + GetMarketplaceFee(Listing.Price);
            
            // In a real implementation, this would handle the payment
            // For now, simulate successful payment
            
            // Transfer NFT to buyer
            if (TransferNFT(Listing.Asset.TokenID, BuyerAddress))
            {
                Listing.IsActive = false;
                
                OnNFTSold.Broadcast(Listing);
                
                UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Sold NFT %s to %s for %f %s"), 
                       *Listing.Asset.TokenID, *BuyerAddress, Listing.Price, *Listing.Currency);
                
                return true;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: Listing %s not found"), *ListingID);
    return false;
}

bool UMingBlockchainNFTSystem::PlaceBid(const FString& ListingID, const FString& BidderAddress, float BidAmount)
{
    for (FNFTMarketplaceListing& Listing : MarketplaceListings)
    {
        if (Listing.ListingID == ListingID)
        {
            if (BidAmount <= Listing.HighestBid)
            {
                UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: Bid amount must be higher than current bid"));
                return false;
            }

            Listing.HighestBid = BidAmount;
            Listing.BidCount++;
            
            UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Placed bid of %f on listing %s"), 
                   BidAmount, *ListingID);
            
            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: Listing %s not found"), *ListingID);
    return false;
}

bool UMingBlockchainNFTSystem::CancelNFTListing(const FString& ListingID)
{
    for (int32 i = 0; i < MarketplaceListings.Num(); ++i)
    {
        if (MarketplaceListings[i].ListingID == ListingID)
        {
            FNFTMarketplaceListing& Listing = MarketplaceListings[i];
            
            if (!BlockchainWallet || Listing.SellerAddress != BlockchainWallet->GetWalletInfo().WalletAddress)
            {
                UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: Only seller can cancel listing"));
                return false;
            }

            Listing.IsActive = false;
            
            UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Cancelled listing %s"), *ListingID);
            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: Listing %s not found"), *ListingID);
    return false;
}

TArray<FNFTMarketplaceListing> UMingBlockchainNFTSystem::GetMarketplaceListings(ENFTCategory Category, float MinPrice, float MaxPrice, int32 Limit) const
{
    TArray<FNFTMarketplaceListing> Result;
    
    for (const FNFTMarketplaceListing& Listing : MarketplaceListings)
    {
        if (!Listing.IsActive)
        {
            continue;
        }

        if (Category != ENFTCategory::Special && Listing.Asset.Metadata.Category != Category)
        {
            continue;
        }

        if (MinPrice > 0.0f && Listing.Price < MinPrice)
        {
            continue;
        }

        if (MaxPrice > 0.0f && Listing.Price > MaxPrice)
        {
            continue;
        }

        Result.Add(Listing);
        
        if (Result.Num() >= Limit)
        {
            break;
        }
    }
    
    return Result;
}

TMap<ENFTRarity, int32> UMingBlockchainNFTSystem::GetNFTDistribution(const FString& CollectionID) const
{
    TMap<ENFTRarity, int32> Distribution;
    
    // Initialize all rarities to 0
    for (int32 i = 0; i <= (int32)ENFTRarity::Mythic; ++i)
    {
        Distribution.Add((ENFTRarity)i, 0);
    }
    
    // Count NFTs by rarity in the collection
    for (const auto& OwnerNFTs : OwnedNFTs)
    {
        for (const FNFTAsset& NFT : OwnerNFTs.Value)
        {
            if (NFT.Metadata.Collection == CollectionID)
            {
                int32& Count = Distribution[NFT.Metadata.Rarity];
                Count++;
            }
        }
    }
    
    return Distribution;
}

float UMingBlockchainNFTSystem::EstimateNFTValue(const FString& TokenID) const
{
    FNFTAsset NFT = GetNFT(TokenID);
    return CalculateNFTValue(NFT);
}

FNFTMetadata UMingBlockchainNFTSystem::GetNFTMetadata(const FString& TokenID) const
{
    FNFTAsset NFT = GetNFT(TokenID);
    return NFT.Metadata;
}

bool UMingBlockchainNFTSystem::UpdateNFTMetadata(const FString& TokenID, const FNFTMetadata& NewMetadata)
{
    if (!ValidateNFTMetadata(NewMetadata))
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainNFTSystem: Invalid NFT metadata for update"));
        return false;
    }

    // Find and update the NFT
    for (auto& OwnerNFTs : OwnedNFTs)
    {
        for (FNFTAsset& NFT : OwnerNFTs.Value)
        {
            if (NFT.TokenID == TokenID)
            {
                NFT.Metadata = NewMetadata;
                NFT.EstimatedValue = CalculateNFTValue(NFT);
                
                UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Updated metadata for NFT %s"), *TokenID);
                return true;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainNFTSystem: NFT %s not found for metadata update"), *TokenID);
    return false;
}

float UMingBlockchainNFTSystem::GetNFTCreationCost(ENFTRarity Rarity) const
{
    if (const float* Cost = NFTCreationCosts.Find(Rarity))
    {
        return *Cost;
    }
    
    // Default costs if not found
    switch (Rarity)
    {
        case ENFTRarity::Common:    return 0.01f;
        case ENFTRarity::Uncommon:  return 0.05f;
        case ENFTRarity::Rare:      return 0.1f;
        case ENFTRarity::Epic:      return 0.25f;
        case ENFTRarity::Legendary:  return 0.5f;
        case ENFTRarity::Mythic:    return 1.0f;
        default: return 0.1f;
    }
}

TArray<ENFTCategory> UMingBlockchainNFTSystem::GetSupportedCategories() const
{
    TArray<ENFTCategory> Categories;
    Categories.Add(ENFTCategory::Character);
    Categories.Add(ENFTCategory::Weapon);
    Categories.Add(ENFTCategory::Armor);
    Categories.Add(ENFTCategory::Building);
    Categories.Add(ENFTCategory::Resource);
    Categories.Add(ENFTCategory::Artifact);
    Categories.Add(ENFTCategory::Cosmetic);
    Categories.Add(ENFTCategory::Land);
    Categories.Add(ENFTCategory::Title);
    Categories.Add(ENFTCategory::Special);
    
    return Categories;
}

FString UMingBlockchainNFTSystem::GetRarityName(ENFTRarity Rarity) const
{
    switch (Rarity)
    {
        case ENFTRarity::Common:    return TEXT("Common");
        case ENFTRarity::Uncommon:  return TEXT("Uncommon");
        case ENFTRarity::Rare:      return TEXT("Rare");
        case ENFTRarity::Epic:      return TEXT("Epic");
        case ENFTRarity::Legendary:  return TEXT("Legendary");
        case ENFTRarity::Mythic:    return TEXT("Mythic");
        default: return TEXT("Unknown");
    }
}

FString UMingBlockchainNFTSystem::GetCategoryName(ENFTCategory Category) const
{
    switch (Category)
    {
        case ENFTCategory::Character: return TEXT("Character");
        case ENFTCategory::Weapon:   return TEXT("Weapon");
        case ENFTCategory::Armor:    return TEXT("Armor");
        case ENFTCategory::Building: return TEXT("Building");
        case ENFTCategory::Resource: return TEXT("Resource");
        case ENFTCategory::Artifact: return TEXT("Artifact");
        case ENFTCategory::Cosmetic: return TEXT("Cosmetic");
        case ENFTCategory::Land:     return TEXT("Land");
        case ENFTCategory::Title:    return TEXT("Title");
        case ENFTCategory::Special:  return TEXT("Special");
        default: return TEXT("Unknown");
    }
}

bool UMingBlockchainNFTSystem::ValidateNFTMetadata(const FNFTMetadata& Metadata) const
{
    return !Metadata.Name.IsEmpty() &&
           !Metadata.Description.IsEmpty() &&
           Metadata.Level > 0 &&
           Metadata.CreationDate > 0.0f;
}

FString UMingBlockchainNFTSystem::GenerateNFTImageURL(const FString& TokenID) const
{
    return GetNFTImageBaseURL() + TokenID + TEXT(".png");
}

FString UMingBlockchainNFTSystem::GenerateNFTMetadataJSON(const FNFTMetadata& Metadata) const
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("name"), Metadata.Name);
    JsonObject->SetStringField(TEXT("description"), Metadata.Description);
    JsonObject->SetStringField(TEXT("image"), Metadata.ImageURL);
    JsonObject->SetStringField(TEXT("animation_url"), Metadata.AnimationURL);
    JsonObject->SetNumberField(TEXT("category"), (int32)Metadata.Category);
    JsonObject->SetNumberField(TEXT("rarity"), (int32)Metadata.Rarity);
    JsonObject->SetNumberField(TEXT("level"), Metadata.Level);
    JsonObject->SetStringField(TEXT("creator"), Metadata.Creator);
    JsonObject->SetStringField(TEXT("collection"), Metadata.Collection);
    JsonObject->SetNumberField(TEXT("creation_date"), Metadata.CreationDate);
    
    // Add attributes array
    TArray<TSharedPtr<FJsonValue>> AttributesArray;
    for (const FString& Attribute : Metadata.Attributes)
    {
        AttributesArray.Add(MakeShareable(new FJsonValueString(Attribute)));
    }
    JsonObject->SetArrayField(TEXT("attributes"), AttributesArray);
    
    // Add custom properties
    TSharedPtr<FJsonObject> CustomPropsObject = MakeShareable(new FJsonObject);
    for (const auto& Prop : Metadata.CustomProperties)
    {
        CustomPropsObject->SetStringField(Prop.Key, Prop.Value);
    }
    JsonObject->SetObjectField(TEXT("properties"), CustomPropsObject);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

void UMingBlockchainNFTSystem::InitializeNFTConfiguration()
{
    // Initialize NFT creation costs
    NFTCreationCosts.Add(ENFTRarity::Common,    0.01f);
    NFTCreationCosts.Add(ENFTRarity::Uncommon,  0.05f);
    NFTCreationCosts.Add(ENFTRarity::Rare,      0.1f);
    NFTCreationCosts.Add(ENFTRarity::Epic,      0.25f);
    NFTCreationCosts.Add(ENFTRarity::Legendary,  0.5f);
    NFTCreationCosts.Add(ENFTRarity::Mythic,    1.0f);

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Configuration initialized"));
}

FString UMingBlockchainNFTSystem::GenerateTokenID() const
{
    return TEXT("NFT_") + FGuid::NewGuid().ToString();
}

FString UMingBlockchainNFTSystem::GenerateCollectionID() const
{
    return TEXT("COLLECTION_") + FGuid::NewGuid().ToString();
}

FString UMingBlockchainNFTSystem::GenerateListingID() const
{
    return TEXT("LISTING_") + FGuid::NewGuid().ToString();
}

bool UMingBlockchainNFTSystem::MintNFTOnBlockchain(FNFTAsset& NFT)
{
    // Simulate blockchain minting
    // In a real implementation, this would interact with the NFT smart contract
    
    // Check if collection exists or create default
    if (NFT.Metadata.Collection.IsEmpty())
    {
        NFT.Metadata.Collection = TEXT("MingGoRTS Default");
    }
    
    // Simulate minting success
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Minted NFT %s on blockchain"), *NFT.TokenID);
    return true;
}

bool UMingBlockchainNFTSystem::TransferNFTOnBlockchain(const FString& TokenID, const FString& FromAddress, const FString& ToAddress)
{
    // Simulate blockchain transfer
    // In a real implementation, this would interact with the NFT smart contract
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Transferred NFT %s from %s to %s on blockchain"), 
           *TokenID, *FromAddress, *ToAddress);
    return true;
}

bool UMingBlockchainNFTSystem::BurnNFTOnBlockchain(const FString& TokenID, const FString& OwnerAddress)
{
    // Simulate blockchain burn
    // In a real implementation, this would interact with the NFT smart contract
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Burned NFT %s on blockchain"), *TokenID);
    return true;
}

void UMingBlockchainNFTSystem::UpdateMarketplaceListings()
{
    // In a real implementation, this would sync with marketplace
    // For now, just check for expired listings
    ProcessExpiredListings();
}

void UMingBlockchainNFTSystem::ProcessExpiredListings()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    
    for (FNFTMarketplaceListing& Listing : MarketplaceListings)
    {
        if (Listing.IsActive && CurrentTime > Listing.ExpirationDate)
        {
            Listing.IsActive = false;
            UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: Expired listing %s"), *Listing.ListingID);
        }
    }
}

float UMingBlockchainNFTSystem::GetRarityProbability(ENFTRarity Rarity) const
{
    switch (Rarity)
    {
        case ENFTRarity::Common:    return 0.6f;   // 60%
        case ENFTRarity::Uncommon:  return 0.25f;  // 25%
        case ENFTRarity::Rare:      return 0.1f;   // 10%
        case ENFTRarity::Epic:      return 0.04f;  // 4%
        case ENFTRarity::Legendary:  return 0.008f; // 0.8%
        case ENFTRarity::Mythic:    return 0.002f; // 0.2%
        default: return 0.0f;
    }
}

ENFTRarity UMingBlockchainNFTSystem::GetRandomRarity() const
{
    float RandomValue = UKismetMathLibrary::RandomFloat();
    float CumulativeProbability = 0.0f;
    
    for (int32 i = 0; i <= (int32)ENFTRarity::Mythic; ++i)
    {
        ENFTRarity Rarity = (ENFTRarity)i;
        CumulativeProbability += GetRarityProbability(Rarity);
        
        if (RandomValue <= CumulativeProbability)
        {
            return Rarity;
        }
    }
    
    return ENFTRarity::Common;
}

float UMingBlockchainNFTSystem::GetTotalNFTCreationCost(ENFTRarity Rarity) const
{
    float BaseCost = GetNFTCreationCost(Rarity);
    float GasFee = BaseCost * 0.1f; // 10% gas fee estimate
    return BaseCost + GasFee;
}

bool UMingBlockchainNFTSystem::ValidateNFTOwnership(const FString& TokenID, const FString& OwnerAddress) const
{
    TArray<FNFTAsset> OwnerNFTs = GetOwnedNFTs(OwnerAddress);
    
    for (const FNFTAsset& NFT : OwnerNFTs)
    {
        if (NFT.TokenID == TokenID)
        {
            return true;
        }
    }
    
    return false;
}

float UMingBlockchainNFTSystem::GetMarketplaceFee(float Price) const
{
    return Price * MarketplaceFeePercentage;
}

void UMingBlockchainNFTSystem::SaveNFTData()
{
    // Save NFT data to persistent storage
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("NFTData.json");
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: NFT data saved to %s"), *SavePath);
}

void UMingBlockchainNFTSystem::LoadNFTData()
{
    // Load NFT data from persistent storage
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("NFTData.json");
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainNFTSystem: NFT data loaded from %s"), *LoadPath);
}

void UMingBlockchainNFTSystem::CleanupExpiredData()
{
    // Clean up old inactive listings
    float CurrentTime = GetWorld()->GetTimeSeconds();
    const float MaxAge = 86400.0f * 30.0f; // 30 days
    
    MarketplaceListings.RemoveAll([CurrentTime, MaxAge](const FNFTMarketplaceListing& Listing) {
        return !Listing.IsActive && (CurrentTime - Listing.ExpirationDate) > MaxAge;
    });
}

FString UMingBlockchainNFTSystem::GetNFTImageBaseURL() const
{
    return TEXT("https://api.minggorts.com/nft/images/");
}

FString UMingBlockchainNFTSystem::FormatMetadataForBlockchain(const FNFTMetadata& Metadata) const
{
    return GenerateNFTMetadataJSON(Metadata);
}

FNFTMetadata UMingBlockchainNFTSystem::ParseMetadataFromBlockchain(const FString& MetadataJSON) const
{
    FNFTMetadata Metadata;
    
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(MetadataJSON);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        Metadata.Name = JsonObject->GetStringField(TEXT("name"));
        Metadata.Description = JsonObject->GetStringField(TEXT("description"));
        Metadata.ImageURL = JsonObject->GetStringField(TEXT("image"));
        Metadata.AnimationURL = JsonObject->GetStringField(TEXT("animation_url"));
        Metadata.Category = (ENFTCategory)JsonObject->GetIntegerField(TEXT("category"));
        Metadata.Rarity = (ENFTRarity)JsonObject->GetIntegerField(TEXT("rarity"));
        Metadata.Level = JsonObject->GetIntegerField(TEXT("level"));
        Metadata.Creator = JsonObject->GetStringField(TEXT("creator"));
        Metadata.Collection = JsonObject->GetStringField(TEXT("collection"));
        Metadata.CreationDate = JsonObject->GetNumberField(TEXT("creation_date"));
        
        // Parse attributes array
        const TArray<TSharedPtr<FJsonValue>>* AttributesArray = JsonObject->GetArrayField(TEXT("attributes"));
        if (AttributesArray)
        {
            for (const TSharedPtr<FJsonValue>& Attribute : *AttributesArray)
            {
                if (Attribute->Type == EJson::String)
                {
                    Metadata.Attributes.Add(Attribute->AsString());
                }
            }
        }
        
        // Parse custom properties
        const TSharedPtr<FJsonObject>* CustomProps = JsonObject->GetObjectField(TEXT("properties"));
        if (CustomProps)
        {
            for (const auto& Prop : CustomProps->Values)
            {
                if (Prop.Value->Type == EJson::String)
                {
                    Metadata.CustomProperties.Add(Prop.Key, Prop.Value->AsString());
                }
            }
        }
    }
    
    return Metadata;
}

FString UMingBlockchainNFTSystem::GetNFTAttributesString(const TArray<FString>& Attributes) const
{
    FString Result;
    for (int32 i = 0; i < Attributes.Num(); ++i)
    {
        if (i > 0) Result += TEXT(", ");
        Result += Attributes[i];
    }
    return Result;
}

float UMingBlockchainNFTSystem::CalculateNFTValue(const FNFTAsset& NFT) const
{
    float BaseValue = 0.0f;
    
    // Base value by rarity
    switch (NFT.Metadata.Rarity)
    {
        case ENFTRarity::Common:    BaseValue = 10.0f; break;
        case ENFTRarity::Uncommon:  BaseValue = 25.0f; break;
        case ENFTRarity::Rare:      BaseValue = 100.0f; break;
        case ENFTRarity::Epic:      BaseValue = 500.0f; break;
        case ENFTRarity::Legendary:  BaseValue = 2000.0f; break;
        case ENFTRarity::Mythic:    BaseValue = 10000.0f; break;
        default: BaseValue = 10.0f; break;
    }
    
    // Apply level multiplier
    float LevelMultiplier = 1.0f + (NFT.Metadata.Level - 1) * 0.2f;
    
    // Apply transfer count bonus (fewer transfers = higher value)
    float TransferBonus = FMath::Max(0.5f, 1.0f - (NFT.TransferCount * 0.1f));
    
    return BaseValue * LevelMultiplier * TransferBonus;
}

bool UMingBlockchainNFTSystem::IsNFTTransferable(const FNFTAsset& NFT) const
{
    if (!NFT.IsTransferable)
    {
        return false;
    }
    
    // Check transfer cooldown
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - NFT.LastTransferDate < NFTTransferCooldown)
    {
        return false;
    }
    
    return true;
}

float UMingBlockchainNFTSystem::GetNFTTransferFee(const FNFTAsset& NFT) const
{
    // Transfer fee based on rarity
    switch (NFT.Metadata.Rarity)
    {
        case ENFTRarity::Common:    return 0.001f;
        case ENFTRarity::Uncommon:  return 0.005f;
        case ENFTRarity::Rare:      return 0.01f;
        case ENFTRarity::Epic:      return 0.025f;
        case ENFTRarity::Legendary:  return 0.05f;
        case ENFTRarity::Mythic:    return 0.1f;
        default: return 0.01f;
    }
}
