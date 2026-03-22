#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSResourceSystem.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
    Wood              UMETA(DisplayName = "?®Ê?"),
    Stone             UMETA(DisplayName = "?≥È†≠"),
    Metal             UMETA(DisplayName = "?ëÂ±¨"),
    Food              UMETA(DisplayName = "È£üÁâ©"),
    Water             UMETA(DisplayName = "Ê∞?),
    Coal              UMETA(DisplayName = "?§ÁÇ≠"),
    Iron              UMETA(DisplayName = "?µÁ§¶"),
    Copper            UMETA(DisplayName = "?ÖÁ§¶"),
    Gold              UMETA(DisplayName = "ÈªÉÈ?"),
    Silver            UMETA(DisplayName = "?ΩÈ?"),
    Cotton            UMETA(DisplayName = "Ê£âËä±"),
    Silk              UMETA(DisplayName = "Áµ≤Á∂¢"),
    Tea               UMETA(DisplayName = "?∂Ë?"),
    Opium             UMETA(DisplayName = "È¥âÁ?"),
    Medicine          UMETA(DisplayName = "?•Â?"),
    Weapons           UMETA(DisplayName = "Ê≠¶Âô®"),
    Ammunition        UMETA(DisplayName = "ÂΩàËó•"),
    Uniforms          UMETA(DisplayName = "ËªçÊ?"),
    Horses            UMETA(DisplayName = "È¶¨Âåπ"),
    Vehicles          UMETA(DisplayName = "ËªäË?"),
    Fuel              UMETA(DisplayName = "?ÉÊ?"),
    Rubber            UMETA(DisplayName = "Ê©°Ë?"),
    Paper             UMETA(DisplayName = "Á¥ôÂºµ"),
    Ink               UMETA(DisplayName = "Â¢®Ê∞¥"),
    Books             UMETA(DisplayName = "?∏Á?"),
    Tools             UMETA(DisplayName = "Â∑•ÂÖ∑"),
    Machinery         UMETA(DisplayName = "Ê©üÊ¢∞"),
    Electronics       UMETA(DisplayName = "?ªÂ?Ë®≠Â?"),
    Radio             UMETA(DisplayName = "?°Á???),
    Telephone         UMETA(DisplayName = "?ªË©±"),
    Telegraph         UMETA(DisplayName = "?ªÂ†±Ê©?)
};

UENUM(BlueprintType)
enum class EResourceRarity : uint8
{
    Common            UMETA(DisplayName = "?ÆÈÄ?),
    Uncommon          UMETA(DisplayName = "Á®Ä??),
    Rare              UMETA(DisplayName = "?çÁ?"),
    Epic              UMETA(DisplayName = "?≤Ë©©"),
    Legendary         UMETA(DisplayName = "?≥Ë™™")
};

UENUM(BlueprintType)
enum class EResourceQuality : uint8
{
    Poor              UMETA(DisplayName = "??≥™"),
    Normal            UMETA(DisplayName = "?ÆÈÄ?),
    Good              UMETA(DisplayName = "?™ËâØ"),
    Excellent         UMETA(DisplayName = "?ìË?"),
    Perfect           UMETA(DisplayName = "ÂÆåÁ?")
};

UENUM(BlueprintType)
enum class ECollectionMethod : uint8
{
    Manual            UMETA(DisplayName = "?ãÂ??∂È?"),
    Automatic         UMETA(DisplayName = "?™Â??∂È?"),
    Production        UMETA(DisplayName = "?üÁî¢Ë£ΩÈÄ?),
    Trade             UMETA(DisplayName = "Ë≤øÊ??≤Â?"),
    Plunder           UMETA(DisplayName = "?†Â•™?≤Â?"),
    Harvest           UMETA(DisplayName = "?°È??∂Á©´"),
    Mining            UMETA(DisplayName = "?ãÊé°?ñÊ?"),
    Fishing           UMETA(DisplayName = "?ïÈ?"),
    Hunting           UMETA(DisplayName = "?©Áçµ"),
    Foraging          UMETA(DisplayName = "?°È?Ë¶ìÈ?")
};

USTRUCT(BlueprintType)
struct FResourceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    EResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    EResourceRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    EResourceQuality Quality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    float Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    float MaxQuantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    float UnitWeight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    float UnitValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    FString IconPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    bool bIsStackable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    bool bIsPerishable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    float PerishRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    FDateTime ExpiryDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    TArray<FString> Tags;

    FResourceData()
    {
        ResourceType = EResourceType::Wood;
        Rarity = EResourceRarity::Common;
        Quality = EResourceQuality::Normal;
        Quantity = 0.0f;
        MaxQuantity = 100.0f;
        UnitWeight = 1.0f;
        UnitValue = 1.0f;
        Description = TEXT("");
        IconPath = TEXT("");
        bIsStackable = true;
        bIsPerishable = false;
        PerishRate = 0.0f;
        Tags.Empty();
    }
};

USTRUCT(BlueprintType)
struct FResourceCollection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    FString CollectionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    EResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    ECollectionMethod CollectionMethod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    FString LocationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    FString CollectorID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    float CollectionRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    float Efficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    float BaseYield;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    float CurrentYield;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    bool bIsActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    bool bIsAutomated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    FDateTime LastCollectionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    float TotalCollected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    TArray<FString> RequiredTools;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    TArray<FString> RequiredSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    TMap<FString, float> EnvironmentalFactors;

    FResourceCollection()
    {
        CollectionID = TEXT("");
        ResourceType = EResourceType::Wood;
        CollectionMethod = ECollectionMethod::Manual;
        LocationID = TEXT("");
        CollectorID = TEXT("");
        CollectionRate = 1.0f;
        Efficiency = 1.0f;
        BaseYield = 10.0f;
        CurrentYield = 10.0f;
        bIsActive = false;
        bIsAutomated = false;
        TotalCollected = 0.0f;
        RequiredTools.Empty();
        RequiredSkills.Empty();
        EnvironmentalFactors.Empty();
    }
};

USTRUCT(BlueprintType)
struct FResourceStorage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    FString StorageID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    FString StorageName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    FString LocationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    TMap<EResourceType, FResourceData> StoredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float MaxCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float CurrentCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float UsedCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    TArray<EResourceType> AllowedTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    bool bIsClimateControlled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    bool bIsSecure;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float SecurityLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    TArray<FString> UpgradeSlots;

    FResourceStorage()
    {
        StorageID = TEXT("");
        StorageName = TEXT("");
        LocationID = TEXT("");
        MaxCapacity = 1000.0f;
        CurrentCapacity = 0.0f;
        UsedCapacity = 0.0f;
        bIsClimateControlled = false;
        bIsSecure = false;
        SecurityLevel = 0.0f;
        UpgradeSlots.Empty();
    }
};

USTRUCT(BlueprintType)
struct FResourceConversion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    FString ConversionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    FString ConversionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    TMap<EResourceType, float> InputResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    TMap<EResourceType, float> OutputResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    float ConversionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    float Efficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    bool bIsAvailable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    TArray<FString> RequiredBuildings;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    TArray<FString> RequiredSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    FString Description;

    FResourceConversion()
    {
        ConversionID = TEXT("");
        ConversionName = TEXT("");
        ConversionTime = 1.0f;
        Efficiency = 1.0f;
        bIsAvailable = true;
        Description = TEXT("");
        RequiredBuildings.Empty();
        RequiredSkills.Empty();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceCollected, EResourceType, ResourceType, float, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourceStored, const FString&, StorageID, EResourceType, ResourceType, float, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceConverted, const FString&, ConversionID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStorageFull, const FString&, StorageID, float, Capacity);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSResourceSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSResourceSystem();

    // Ë≥áÊ?Á≥ªÁµ±‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnResourceCollected OnResourceCollected;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnResourceStored OnResourceStored;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnResourceConverted OnResourceConverted;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnStorageFull OnStorageFull;

    // ?ùÂ??ñË?Ê∫êÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void InitializeResourceSystem();

    // ?≤Â??Ä?âË?Ê∫êÈ???    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<EResourceType> GetAllResourceTypes() const;

    // ?≤Â?Ë≥áÊ?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FResourceData GetResourceInfo(EResourceType ResourceType) const;

    // ?µÂª∫Ë≥áÊ??∂È?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceCollection(EResourceType ResourceType, ECollectionMethod Method, const FString& LocationID);

    // ?ãÂ?Ë≥áÊ??∂È?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StartResourceCollection(const FString& CollectionID, const FString& CollectorID);

    // ?úÊ≠¢Ë≥áÊ??∂È?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StopResourceCollection(const FString& CollectionID);

    // ?∂È?Ë≥áÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float CollectResource(const FString& CollectionID);

    // ?µÂª∫Ë≥áÊ?Â≠òÂÑ≤
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceStorage(const FString& StorageName, const FString& LocationID, float MaxCapacity);

    // Â≠òÂÑ≤Ë≥áÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StoreResource(const FString& StorageID, EResourceType ResourceType, float Quantity);

    // ?êÂ?Ë≥áÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float RetrieveResource(const FString& StorageID, EResourceType ResourceType, float Quantity);

    // ?≤Â?Â≠òÂÑ≤Ë≥áÊ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FResourceData GetStoredResource(const FString& StorageID, EResourceType ResourceType) const;

    // ?≤Â?Â≠òÂÑ≤ÂÆπÈ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetStorageCapacity(const FString& StorageID) const;

    // ËΩâÊ?Ë≥áÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool ConvertResources(const FString& ConversionID);

    // ?µÂª∫Ë≥áÊ?ËΩâÊ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceConversion(const FString& ConversionName, const TMap<EResourceType, float>& Input, const TMap<EResourceType, float>& Output);

    // Ë®àÁ??∂È??àÁ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float CalculateCollectionEfficiency(const FString& CollectionID) const;

    // ?¥Êñ∞?∂È??àÁ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void UpdateCollectionEfficiency(const FString& CollectionID, float NewEfficiency);

    // ?≤Â??∂È??Ä??    UFUNCTION(BlueprintPure, Category = "Resource System")
    FResourceCollection GetCollectionStatus(const FString& CollectionID) const;

    // ?≤Â??Ä?âÊ¥ª?ïÊî∂??    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<FResourceCollection> GetActiveCollections() const;

    // Ë≥áÊ?Ë≥™È??êÂ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool ImproveResourceQuality(const FString& StorageID, EResourceType ResourceType, EResourceQuality NewQuality);

    // Ë≥áÊ??ãËº∏
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TransportResource(const FString& FromStorageID, const FString& ToStorageID, EResourceType ResourceType, float Quantity);

    // Ë≥áÊ?‰∫§Ê?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TradeResource(const FString& StorageID, EResourceType GiveResource, float GiveQuantity, EResourceType GetResource, float GetQuantity);

    // Ë≥áÊ??ÜÈ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool AllocateResource(const FString& StorageID, EResourceType ResourceType, float Quantity, const FString& Purpose);

    // Ê™¢Êü•Ë≥áÊ??ÖË∂≥??    UFUNCTION(BlueprintPure, Category = "Resource System")
    bool CheckResourceAvailability(const FString& StorageID, EResourceType ResourceType, float RequiredQuantity) const;

    // ?≤Â?Ë≥áÊ?Á∏ΩÈ?
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetTotalResourceQuantity(EResourceType ResourceType) const;

    // Ë≥áÊ?Ë°∞Ê??ïÁ?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void ProcessResourceDecay(float DeltaTime);

    // ‰øùÂ?Ë≥áÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool SaveResourceData(const FString& SaveSlotName);

    // ËºâÂÖ•Ë≥áÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool LoadResourceData(const FString& SaveSlotName);

protected:
    // Ë≥áÊ?‰ø°ÊÅØ?†Â?
    UPROPERTY()
    TMap<EResourceType, FResourceData> ResourceInfoMap;

    // Ë≥áÊ??∂È??†Â?
    UPROPERTY()
    TMap<FString, FResourceCollection> ResourceCollections;

    // Ë≥áÊ?Â≠òÂÑ≤?†Â?
    UPROPERTY()
    TMap<FString, FResourceStorage> ResourceStorages;

    // Ë≥áÊ?ËΩâÊ??†Â?
    UPROPERTY()
    TMap<FString, FResourceConversion> ResourceConversions;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñË?Ê∫ê‰ø°??    void InitializeResourceInfo();

    // ?ùÂ??ñÊ≠∑?≤Ë?Ê∫?    void InitializeHistoricalResources();

    // ?µÂª∫?®Ê?Ë≥áÊ?
    void CreateWoodResource();

    // ?µÂª∫?≥È†≠Ë≥áÊ?
    void CreateStoneResource();

    // ?µÂª∫?ëÂ±¨Ë≥áÊ?
    void CreateMetalResource();

    // ?µÂª∫È£üÁâ©Ë≥áÊ?
    void CreateFoodResource();

    // ?µÂª∫Ê∞¥Ë?Ê∫?    void CreateWaterResource();

    // ?µÂª∫?§ÁÇ≠Ë≥áÊ?
    void CreateCoalResource();

    // ?µÂª∫?µÁ§¶Ë≥áÊ?
    void CreateIronResource();

    // ?µÂª∫?ÖÁ§¶Ë≥áÊ?
    void CreateCopperResource();

    // ?µÂª∫ÈªÉÈ?Ë≥áÊ?
    void CreateGoldResource();

    // ?µÂª∫?ΩÈ?Ë≥áÊ?
    void CreateSilverResource();

    // ?µÂª∫Ê£âËä±Ë≥áÊ?
    void CreateCottonResource();

    // ?µÈÄ†Áµ≤Á∂¢Ë?Ê∫?    void CreateSilkResource();

    // ?µÂª∫?∂Ë?Ë≥áÊ?
    void CreateTeaResource();

    // ?µÂª∫È¥âÁ?Ë≥áÊ?
    void CreateOpiumResource();

    // ?µÂª∫?•Â?Ë≥áÊ?
    void CreateMedicineResource();

    // ?µÂª∫Ê≠¶Âô®Ë≥áÊ?
    void CreateWeaponsResource();

    // ?µÂª∫ÂΩàËó•Ë≥áÊ?
    void CreateAmmunitionResource();

    // ?µÂª∫ËªçÊ?Ë≥áÊ?
    void CreateUniformsResource();

    // ?µÂª∫È¶¨ÂåπË≥áÊ?
    void CreateHorsesResource();

    // ?µÂª∫ËªäË?Ë≥áÊ?
    void CreateVehiclesResource();

    // ?µÂª∫?ÉÊ?Ë≥áÊ?
    void CreateFuelResource();

    // ?µÂª∫Ê©°Ë?Ë≥áÊ?
    void CreateRubberResource();

    // ?µÂª∫Á¥ôÂºµË≥áÊ?
    void CreatePaperResource();

    // ?µÂª∫Â¢®Ê∞¥Ë≥áÊ?
    void CreateInkResource();

    // ?µÂª∫?∏Á?Ë≥áÊ?
    void CreateBooksResource();

    // ?µÂª∫Â∑•ÂÖ∑Ë≥áÊ?
    void CreateToolsResource();

    // ?µÂª∫Ê©üÊ¢∞Ë≥áÊ?
    void CreateMachineryResource();

    // ?µÂª∫?ªÂ?Ë®≠Â?Ë≥áÊ?
    void CreateElectronicsResource();

    // ?µÂª∫?°Á??ªË?Ê∫?    void CreateRadioResource();

    // ?µÂª∫?ªË©±Ë≥áÊ?
    void CreateTelephoneResource();

    // ?µÂª∫?ªÂ†±Ê©üË?Ê∫?    void CreateTelegraphResource();

    // ?¥Êñ∞?∂È??¢È?
    void UpdateCollectionYield(const FString& CollectionID);

    // Ë®àÁ??∞Â??†Á?ÂΩ±Èüø
    float CalculateEnvironmentalImpact(const FString& CollectionID) const;

    // Ê™¢Êü•?∂È?Ê¢ù‰ª∂
    bool CheckCollectionConditions(const FString& CollectionID) const;

    // ?ïÁ??∂È?ÂÆåÊ?
    void ProcessCollectionComplete(const FString& CollectionID);

    // ?¥Êñ∞Â≠òÂÑ≤ÂÆπÈ?
    void UpdateStorageCapacity(const FString& StorageID);

    // Ê™¢Êü•Â≠òÂÑ≤?êÂà∂
    bool CheckStorageLimits(const FString& StorageID, EResourceType ResourceType, float Quantity) const;

    // ?ïÁ?Â≠òÂÑ≤Êªø‰?
    void HandleStorageFull(const FString& StorageID);

    // Ë®àÁ?ËΩâÊ??êÊú¨
    float CalculateConversionCost(const FString& ConversionID) const;

    // Ê™¢Êü•ËΩâÊ?Ê¢ù‰ª∂
    bool CheckConversionConditions(const FString& ConversionID) const;

    // ?ïÁ?ËΩâÊ?ÂÆåÊ?
    void ProcessConversionComplete(const FString& ConversionID);

    // ?üÊ??∂È?ID
    FString GenerateCollectionID(EResourceType ResourceType) const;

    // ?üÊ?Â≠òÂÑ≤ID
    FString GenerateStorageID(const FString& StorageName) const;

    // ?üÊ?ËΩâÊ?ID
    FString GenerateConversionID(const FString& ConversionName) const;

    // È©óË?Ë≥áÊ??∏Ê?
    bool ValidateResourceData(const FResourceData& ResourceData) const;

    // ?≤Â?Ë≥áÊ?È°ûÂ??çÁ®±
    FString GetResourceTypeName(EResourceType ResourceType) const;

    // ?≤Â?Ë≥áÊ?È°ûÂ?Ê¨äÈ?
    float GetResourceTypeWeight(EResourceType ResourceType) const;

    // ?≤Â??∂È??πÊ??àÁ??†Ê?
    float GetCollectionMethodBonus(ECollectionMethod Method) const;

    // ?≤Â?Ë≥áÊ?Ë≥™È??†Ê?
    float GetQualityBonus(EResourceQuality Quality) const;

    // Ë®àÁ?Ë≥áÊ?Á∏ΩÂÉπ??    float CalculateResourceTotalValue(const FResourceData& ResourceData) const;

    // Ê™¢Êü•Ë≥áÊ??ºÂÆπ??    bool CheckResourceCompatibility(EResourceType ResourceType1, EResourceType ResourceType2) const;

    // ?≤Â??®Ëñ¶?∂È??πÊ?
    ECollectionMethod GetRecommendedCollectionMethod(EResourceType ResourceType) const;

    // ?≤Â?Ë≥áÊ??∂È??∞È?
    TArray<FString> GetResourceCollectionLocations(EResourceType ResourceType) const;

    // ?≤Â?Ë≥áÊ?Â≠òÂÑ≤Âª∫Ë≠∞
    TArray<FString> GetResourceStorageRecommendations(EResourceType ResourceType) const;

    // ?üÊ?Ë≥áÊ??±Â?
    FString GenerateResourceReport() const;

    // ?ÜÊ?Ë≥áÊ?Ë∂®Âã¢
    void AnalyzeResourceTrends();

    // ?êÊ∏¨Ë≥áÊ??ÄÊ±?    TArray<EResourceType> PredictResourceNeeds() const;

    // ?™Â?Ë≥áÊ??ÜÈ?
    void OptimizeResourceAllocation();

    // Ë®àÁ?Ë≥áÊ??àÁ?
    float CalculateResourceEfficiency(EResourceType ResourceType) const;

    // ?≤Â?Ë≥áÊ?Áµ±Ë?
    TMap<EResourceType, float> GetResourceStatistics() const;

    // ?¥Êñ∞Ë≥áÊ?Â∏ÇÂ†¥?πÊ†º
    void UpdateResourceMarketPrices();

    // ?≤Â?Ë≥áÊ?Â∏ÇÂ†¥?πÊ†º
    float GetResourceMarketPrice(EResourceType ResourceType) const;

    // ?ÜÊ?Ë≥áÊ?‰æõÈ?Âπ≥Ë°°
    void AnalyzeResourceSupplyDemand();

    // ?≤Â?Ë≥áÊ?‰æõÈ??Ä??    TMap<EResourceType, FString> GetResourceSupplyDemandStatus() const;

    // Ë≥áÊ?Á®ÄÁº∫ÊÄßÂ???    TMap<EResourceType, float> AnalyzeResourceScarcity() const;

    // Ë≥áÊ??çË??ßÂ???    TMap<EResourceType, float> AnalyzeResourceImportance() const;

    // Ë≥áÊ??™Â?Á¥öÊ?Â∫?    TArray<EResourceType> GetResourcePriorityList() const;

    // Ë≥áÊ?È¢®Èö™Ë©ï‰º∞
    TMap<EResourceType, float> AssessResourceRisks() const;

    // Ë≥áÊ?Ê©üÊ??ÜÊ?
    TArray<FString> AnalyzeResourceOpportunities() const;

    // Ë≥áÊ?Á≠ñÁï•Âª∫Ë≠∞
    TArray<FString> GetResourceStrategyRecommendations() const;

    // Ë≥áÊ?Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FString> ResourceHistory;

    // Ê∑ªÂ?Ë≥áÊ?Ê≠∑Âè≤Ë®òÈ?
    void AddResourceHistory(const FString& Event);

    // ?≤Â?Ë≥áÊ?Ê≠∑Âè≤
    TArray<FString> GetResourceHistory() const;

    // Ê∏ÖÁ??éÊ?Ë≥áÊ?
    void CleanupExpiredResources();

    // Ë≥áÊ??ô‰ªΩ
    bool BackupResources(const FString& BackupPath);

    // Ë≥áÊ??¢Âæ©
    bool RestoreResources(const FString& BackupPath);

    // Ë≥áÊ??åÊ≠•
    bool SyncResources(const FString& SourceID);

    // Ë≥áÊ?È©óË?
    bool ValidateResourceIntegrity() const;

    // Ë≥áÊ?‰øÆÂæ©
    bool RepairResourceData();

    // Ë≥áÊ??™Â?
    void OptimizeResourceData();

    // Ë≥áÊ?Â£ìÁ∏Æ
    bool CompressResourceData();

    // Ë≥áÊ?Ëß??
    bool DecompressResourceData();

    // Ë≥áÊ??†Â?
    bool EncryptResourceData();

    // Ë≥áÊ?Ëß??
    bool DecryptResourceData();

    // Ë≥áÊ?Â∞éÂá∫
    bool ExportResources(const FString& FilePath);

    // Ë≥áÊ?Â∞éÂÖ•
    bool ImportResources(const FString& FilePath);

    // Ë≥áÊ??∑Áßª
    bool MigrateResources(const FString& FromPath, const FString& ToPath);

    // Ë≥áÊ??à‰Ωµ
    bool MergeResources(const FString& SourceID);

    // Ë≥áÊ??ÜÈõ¢
    bool SplitResources(const FString& TargetID);

    // Ë≥áÊ??ãÈ?
    bool CloneResources(const FString& SourceID, const FString& TargetID);

    // Ë≥áÊ?ÊØîË?
    TArray<FString> CompareResources(const FString& SourceID, const FString& TargetID) const;

    // Ë≥áÊ??åÊ≠•Ê™¢Êü•
    TArray<FString> CheckResourceSync(const FString& SourceID) const;

    // Ë≥áÊ?Â∑ÆÁï∞‰øÆÂæ©
    bool FixResourceDifferences(const FString& SourceID);

    // Ë≥áÊ??àÊú¨?ßÂà∂
    bool VersionControlResources(const FString& Version);

    // Ë≥áÊ??ûÊªæ
    bool RollbackResources(const FString& Version);

    // Ë≥áÊ??ÜÊîØ
    bool BranchResources(const FString& BranchName);

    // Ë≥áÊ??à‰Ωµ?ÜÊîØ
    bool MergeResourceBranches(const FString& SourceBranch, const FString& TargetBranch);

    // Ë≥áÊ?Ë°ùÁ?Ëß?±∫
    bool ResolveResourceConflicts(const FString& ConflictID);

    // Ë≥áÊ??ñÂ?
    bool LockResource(const FString& ResourceID);

    // Ë≥áÊ?Ëß??
    bool UnlockResource(const FString& ResourceID);

    // Ë≥áÊ?Ê¨äÈ?Ê™¢Êü•
    bool CheckResourcePermission(const FString& ResourceID, const FString& Permission) const;

    // Ë≥áÊ?Ê¨äÈ?Ë®≠ÁΩÆ
    bool SetResourcePermission(const FString& ResourceID, const FString& Permission, bool bGrant);

    // Ë≥áÊ?ÂØ©Ë?
    TArray<FString> AuditResources() const;

    // Ë≥áÊ??•Ë?
    void LogResourceEvent(const FString& Event);

    // Ë≥áÊ???éß
    void MonitorResources();

    // Ë≥áÊ?Ë≠¶Â†±
    void TriggerResourceAlert(const FString& AlertType, const FString& Message);

    // Ë≥áÊ??êË≠¶
    TArray<FString> GetResourceWarnings() const;

    // Ë≥áÊ??ØË™§?ïÁ?
    bool HandleResourceError(const FString& ErrorType, const FString& ErrorMessage);

    // Ë≥áÊ??¢Âæ©
    bool RecoverFromResourceError(const FString& ErrorID);

    // Ë≥áÊ?Ë®∫Êñ∑
    TArray<FString> DiagnoseResourceIssues() const;

    // Ë≥áÊ??ßËÉΩ?ÜÊ?
    TMap<FString, float> AnalyzeResourcePerformance() const;

    // Ë≥áÊ??™Â?Âª∫Ë≠∞
    TArray<FString> GetResourceOptimizationSuggestions() const;

    // Ë≥áÊ??™Â???    bool AutomateResourceCollection(const FString& CollectionID);

    // Ë≥áÊ??™Â??ñÂ?Ê≠?    bool StopResourceAutomation(const FString& CollectionID);

    // Ë≥áÊ?Ë®àÂ?
    bool CreateResourcePlan(const FString& PlanName, const TMap<EResourceType, float>& Targets);

    // Ë≥áÊ?Ë®àÂ??∑Ë?
    bool ExecuteResourcePlan(const FString& PlanName);

    // Ë≥áÊ?Ë®àÂ???éß
    void MonitorResourcePlan(const FString& PlanName);

    // Ë≥áÊ?Ë®àÂ?Ë™øÊï¥
    bool AdjustResourcePlan(const FString& PlanName, const TMap<EResourceType, float>& NewTargets);

    // Ë≥áÊ?Ë®àÂ?ÂÆåÊ?
    bool CompleteResourcePlan(const FString& PlanName);

    // Ë≥áÊ?Ë®àÂ??ñÊ?
    bool CancelResourcePlan(const FString& PlanName);

    // Ë≥áÊ?Ë®àÂ??Ä??    FString GetResourcePlanStatus(const FString& PlanName) const;

    // Ë≥áÊ?Ë®àÂ??±Â?
    FString GenerateResourcePlanReport(const FString& PlanName) const;
};
