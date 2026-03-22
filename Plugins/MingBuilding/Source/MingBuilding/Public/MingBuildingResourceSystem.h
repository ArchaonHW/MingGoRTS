#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingResourceSystem.generated.h"

/**
 * Ë≥áÊ?È°ûÂ??öË? | Resource Type Enum
 * ÂÆöÁæ©Ê∞ëÂ??ÇÊ??ÑÁ®ÆË≥áÊ?È°ûÂ? | Define various resource types from Republican era
 */
UENUM(BlueprintType)
enum class EMingResourceType : uint8
{
    // ?∫Á?Ë≥áÊ? | Basic Resources
    Wood              UMETA(DisplayName = "?®Ê? | Wood"),
    Stone             UMETA(DisplayName = "?≥È†≠ | Stone"),
    Metal             UMETA(DisplayName = "?ëÂ±¨ | Metal"),
    Food              UMETA(DisplayName = "È£üÁâ© | Food"),
    Water             UMETA(DisplayName = "Ê∞?| Water"),
    
    // Á§¶Áî¢Ë≥áÊ? | Mineral Resources
    Coal              UMETA(DisplayName = "?§ÁÇ≠ | Coal"),
    Iron              UMETA(DisplayName = "?µÁ§¶ | Iron"),
    Copper            UMETA(DisplayName = "?ÖÁ§¶ | Copper"),
    Gold              UMETA(DisplayName = "ÈªÉÈ? | Gold"),
    Silver            UMETA(DisplayName = "?ΩÈ? | Silver"),
    
    // Ëæ≤Áî¢??| Agricultural Products
    Cotton            UMETA(DisplayName = "Ê£âËä± | Cotton"),
    Silk              UMETA(DisplayName = "Áµ≤Á∂¢ | Silk"),
    Tea               UMETA(DisplayName = "?∂Ë? | Tea"),
    
    // ?πÊ?Ë≥áÊ? | Special Resources
    Opium             UMETA(DisplayName = "È¥âÁ? | Opium"),
    Medicine          UMETA(DisplayName = "?•Â? | Medicine"),
    
    // Ëªç‰?Ë≥áÊ? | Military Resources
    Weapons           UMETA(DisplayName = "Ê≠¶Âô® | Weapons"),
    Ammunition        UMETA(DisplayName = "ÂΩàËó• | Ammunition"),
    Uniforms          UMETA(DisplayName = "ËªçÊ? | Uniforms"),
    Horses            UMETA(DisplayName = "È¶¨Âåπ | Horses"),
    Vehicles          UMETA(DisplayName = "ËªäË? | Vehicles"),
    
    // Â∑•Ê•≠Ë≥áÊ? | Industrial Resources
    Fuel              UMETA(DisplayName = "?ÉÊ? | Fuel"),
    Rubber            UMETA(DisplayName = "Ê©°Ë? | Rubber"),
    Paper             UMETA(DisplayName = "Á¥ôÂºµ | Paper"),
    Ink               UMETA(DisplayName = "Â¢®Ê∞¥ | Ink"),
    Books             UMETA(DisplayName = "?∏Á? | Books"),
    Tools             UMETA(DisplayName = "Â∑•ÂÖ∑ | Tools"),
    Machinery         UMETA(DisplayName = "Ê©üÊ¢∞ | Machinery"),
    
    // ?öË?Ë®≠Â? | Communication Equipment
    Electronics       UMETA(DisplayName = "?ªÂ?Ë®≠Â? | Electronics"),
    Radio             UMETA(DisplayName = "?°Á???| Radio"),
    Telephone         UMETA(DisplayName = "?ªË©± | Telephone"),
    Telegraph         UMETA(DisplayName = "?ªÂ†±Ê©?| Telegraph")
};

/**
 * Ë≥áÊ?Á®Ä?âÂ∫¶ | Resource Rarity
 */
UENUM(BlueprintType)
enum class EMingResourceRarity : uint8
{
    Common            UMETA(DisplayName = "?ÆÈÄ?| Common"),
    Uncommon          UMETA(DisplayName = "Á®Ä??| Uncommon"),
    Rare              UMETA(DisplayName = "?çÁ? | Rare"),
    Epic              UMETA(DisplayName = "?≤Ë©© | Epic"),
    Legendary         UMETA(DisplayName = "?≥Ë™™ | Legendary")
};

/**
 * Ë≥áÊ??ÅË≥™ | Resource Quality
 */
UENUM(BlueprintType)
enum class EMingResourceQuality : uint8
{
    Poor              UMETA(DisplayName = "??≥™ | Poor"),
    Normal            UMETA(DisplayName = "?ÆÈÄ?| Normal"),
    Good              UMETA(DisplayName = "?™ËâØ | Good"),
    Excellent         UMETA(DisplayName = "?ìË? | Excellent"),
    Perfect           UMETA(DisplayName = "ÂÆåÁ? | Perfect")
};

/**
 * ?∂È??πÊ? | Collection Method
 */
UENUM(BlueprintType)
enum class EMingCollectionMethod : uint8
{
    Manual            UMETA(DisplayName = "?ãÂ??∂È? | Manual"),
    Automatic         UMETA(DisplayName = "?™Â??∂È? | Automatic"),
    Production        UMETA(DisplayName = "?üÁî¢Ë£ΩÈÄ?| Production"),
    Trade             UMETA(DisplayName = "Ë≤øÊ??≤Â? | Trade"),
    Plunder           UMETA(DisplayName = "?†Â•™?≤Â? | Plunder"),
    Harvest           UMETA(DisplayName = "?°È??∂Á©´ | Harvest"),
    Mining            UMETA(DisplayName = "?ãÊé°?ñÊ? | Mining"),
    Fishing           UMETA(DisplayName = "?ïÈ? | Fishing"),
    Hunting           UMETA(DisplayName = "?©Áçµ | Hunting"),
    Foraging          UMETA(DisplayName = "?°È?Ë¶ìÈ? | Foraging")
};

/**
 * Ë≥áÊ??∏Ê?ÁµêÊ? | Resource Data Structure
 */
USTRUCT(BlueprintType)
struct FMingResourceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    EMingResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    EMingResourceRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    EMingResourceQuality Quality;

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
    bool bIsStackable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    bool bIsPerishable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Data")
    float PerishRate;

    FMingResourceData()
    {
        ResourceType = EMingResourceType::Wood;
        Rarity = EMingResourceRarity::Common;
        Quality = EMingResourceQuality::Normal;
        Quantity = 0.0f;
        MaxQuantity = 100.0f;
        UnitWeight = 1.0f;
        UnitValue = 1.0f;
        Description = TEXT("");
        bIsStackable = true;
        bIsPerishable = false;
        PerishRate = 0.0f;
    }
};

/**
 * Ë≥áÊ??∂È??∏Ê? | Resource Collection Data
 */
USTRUCT(BlueprintType)
struct FMingResourceCollection
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    FString CollectionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    EMingResourceType ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    EMingCollectionMethod CollectionMethod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Collection")
    FString LocationID;

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
    float TotalCollected;

    FMingResourceCollection()
    {
        CollectionID = TEXT("");
        ResourceType = EMingResourceType::Wood;
        CollectionMethod = EMingCollectionMethod::Manual;
        LocationID = TEXT("");
        CollectionRate = 1.0f;
        Efficiency = 1.0f;
        BaseYield = 10.0f;
        CurrentYield = 10.0f;
        bIsActive = false;
        bIsAutomated = false;
        TotalCollected = 0.0f;
    }
};

/**
 * Ë≥áÊ?Â≠òÂÑ≤?∏Ê? | Resource Storage Data
 */
USTRUCT(BlueprintType)
struct FMingResourceStorage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    FString StorageID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    FString StorageName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    FString LocationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    TMap<EMingResourceType, FMingResourceData> StoredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float MaxCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float CurrentCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float UsedCapacity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    bool bIsClimateControlled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    bool bIsSecure;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Storage")
    float SecurityLevel;

    FMingResourceStorage()
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
    }
};

/**
 * Ë≥áÊ?ËΩâÊ??çÊñπ | Resource Conversion Recipe
 */
USTRUCT(BlueprintType)
struct FMingResourceConversion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    FString ConversionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    FString ConversionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    TMap<EMingResourceType, float> InputResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    TMap<EMingResourceType, float> OutputResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    float ConversionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    float Efficiency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Conversion")
    bool bIsAvailable;

    FMingResourceConversion()
    {
        ConversionID = TEXT("");
        ConversionName = TEXT("");
        ConversionTime = 1.0f;
        Efficiency = 1.0f;
        bIsAvailable = true;
    }
};

/**
 * Ë≥áÊ?Á≥ªÁµ±‰∫ã‰ª∂ÂßîÊ¥æ | Resource System Event Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingResourceCollected, EMingResourceType, ResourceType, float, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMingResourceStored, const FString&, StorageID, EMingResourceType, ResourceType, float, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingResourceConverted, const FString&, ConversionID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingStorageFull, const FString&, StorageID, float, Capacity);

/**
 * Ë≥áÊ?ÁÆ°Á?Á≥ªÁµ± | Resource Management System
 * 
 * ÁÆ°Á? MingGoRTS ?äÊà≤‰∏≠Á??Ä?âË?Ê∫êÊî∂?Ü„ÄÅÂ??≤Â?ËΩâÊ?Ôº? * - 40Á®ÆÊ??ãÊ??üË?Ê∫êÈ??? * - Â§öÁ®Æ?∂È??πÂ?ÔºàÊé°?Ü„ÄÅÁ??¢„ÄÅË≤ø?ì„ÄÅÊ?Â•™Ô?
 * - Ë≥áÊ?Â≠òÂÑ≤?åÁÆ°?? * - Ë≥áÊ?ËΩâÊ??åÂ??? */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGBUILDING_API UMingBuildingResourceSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingBuildingResourceSystem();

    /** Ë≥áÊ?Á≥ªÁµ±‰∫ã‰ª∂ | Resource System Events */
    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceCollected OnResourceCollected;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceStored OnResourceStored;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceConverted OnResourceConverted;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingStorageFull OnStorageFull;

    /** ?ùÂ??ñË?Ê∫êÁ≥ªÁµ?| Initialize Resource System */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void InitializeResourceSystem();

    /** ?≤Â??Ä?âË?Ê∫êÈ???| Get All Resource Types */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<EMingResourceType> GetAllResourceTypes() const;

    /** ?≤Â?Ë≥áÊ?‰ø°ÊÅØ | Get Resource Info */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetResourceInfo(EMingResourceType ResourceType) const;

    /** ?µÂª∫Ë≥áÊ??∂È? | Create Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceCollection(EMingResourceType ResourceType, EMingCollectionMethod Method, const FString& LocationID);

    /** ?ãÂ?Ë≥áÊ??∂È? | Start Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StartResourceCollection(const FString& CollectionID, const FString& CollectorID);

    /** ?úÊ≠¢Ë≥áÊ??∂È? | Stop Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StopResourceCollection(const FString& CollectionID);

    /** ?∂È?Ë≥áÊ? | Collect Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float CollectResource(const FString& CollectionID);

    /** ?µÂª∫Ë≥áÊ?Â≠òÂÑ≤ | Create Resource Storage */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceStorage(const FString& StorageName, const FString& LocationID, float MaxCapacity);

    /** Â≠òÂÑ≤Ë≥áÊ? | Store Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StoreResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity);

    /** ?êÂ?Ë≥áÊ? | Retrieve Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float RetrieveResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity);

    /** ?≤Â?Â≠òÂÑ≤Ë≥áÊ? | Get Stored Resource */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetStoredResource(const FString& StorageID, EMingResourceType ResourceType) const;

    /** ?≤Â?Â≠òÂÑ≤ÂÆπÈ? | Get Storage Capacity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetStorageCapacity(const FString& StorageID) const;

    /** ËΩâÊ?Ë≥áÊ? | Convert Resources */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool ConvertResources(const FString& ConversionID);

    /** ?µÂª∫Ë≥áÊ?ËΩâÊ? | Create Resource Conversion */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceConversion(const FString& ConversionName, const TMap<EMingResourceType, float>& Input, const TMap<EMingResourceType, float>& Output);

    /** Ë®àÁ??∂È??àÁ? | Calculate Collection Efficiency */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float CalculateCollectionEfficiency(const FString& CollectionID) const;

    /** ?≤Â??∂È??Ä??| Get Collection Status */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceCollection GetCollectionStatus(const FString& CollectionID) const;

    /** ?≤Â??Ä?âÊ¥ª?ïÊî∂??| Get All Active Collections */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<FMingResourceCollection> GetActiveCollections() const;

    /** Ë≥áÊ??ãËº∏ | Transport Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TransportResource(const FString& FromStorageID, const FString& ToStorageID, EMingResourceType ResourceType, float Quantity);

    /** Ë≥áÊ?‰∫§Ê? | Trade Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TradeResource(const FString& StorageID, EMingResourceType GiveResource, float GiveQuantity, EMingResourceType GetResource, float GetQuantity);

    /** Ê™¢Êü•Ë≥áÊ??ÖË∂≥??| Check Resource Availability */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    bool CheckResourceAvailability(const FString& StorageID, EMingResourceType ResourceType, float RequiredQuantity) const;

    /** ?≤Â?Ë≥áÊ?Á∏ΩÈ? | Get Total Resource Quantity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetTotalResourceQuantity(EMingResourceType ResourceType) const;

    /** Ë≥áÊ?Ë°∞Ê??ïÁ? | Process Resource Decay */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void ProcessResourceDecay(float DeltaTime);

protected:
    /** Ë≥áÊ?‰ø°ÊÅØ?†Â? | Resource Info Map */
    UPROPERTY()
    TMap<EMingResourceType, FMingResourceData> ResourceInfoMap;

    /** Ë≥áÊ??∂È??†Â? | Resource Collection Map */
    UPROPERTY()
    TMap<FString, FMingResourceCollection> ResourceCollections;

    /** Ë≥áÊ?Â≠òÂÑ≤?†Â? | Resource Storage Map */
    UPROPERTY()
    TMap<FString, FMingResourceStorage> ResourceStorages;

    /** Ë≥áÊ?ËΩâÊ??†Â? | Resource Conversion Map */
    UPROPERTY()
    TMap<FString, FMingResourceConversion> ResourceConversions;

    /** ?ØÂê¶Â∑≤Â?ÂßãÂ? | Is Initialized */
    bool bIsInitialized;

    /** ?ùÂ??ñË?Ê∫ê‰ø°??| Initialize Resource Info */
    void InitializeResourceInfo();

    /** ?¥Êñ∞?∂È??¢È? | Update Collection Yield */
    void UpdateCollectionYield(const FString& CollectionID);

    /** Ê™¢Êü•?∂È?Ê¢ù‰ª∂ | Check Collection Conditions */
    bool CheckCollectionConditions(const FString& CollectionID) const;

    /** ?ïÁ??∂È?ÂÆåÊ? | Process Collection Complete */
    void ProcessCollectionComplete(const FString& CollectionID);

    /** ?¥Êñ∞Â≠òÂÑ≤ÂÆπÈ? | Update Storage Capacity */
    void UpdateStorageCapacity(const FString& StorageID);

    /** Ê™¢Êü•Â≠òÂÑ≤?êÂà∂ | Check Storage Limits */
    bool CheckStorageLimits(const FString& StorageID, EMingResourceType ResourceType, float Quantity) const;

    /** ?ïÁ?Â≠òÂÑ≤Êªø‰? | Handle Storage Full */
    void HandleStorageFull(const FString& StorageID);

    /** Ê™¢Êü•ËΩâÊ?Ê¢ù‰ª∂ | Check Conversion Conditions */
    bool CheckConversionConditions(const FString& ConversionID) const;

    /** ?ïÁ?ËΩâÊ?ÂÆåÊ? | Process Conversion Complete */
    void ProcessConversionComplete(const FString& ConversionID);

    /** ?üÊ??∂È?ID | Generate Collection ID */
    FString GenerateCollectionID(EMingResourceType ResourceType) const;

    /** ?üÊ?Â≠òÂÑ≤ID | Generate Storage ID */
    FString GenerateStorageID(const FString& StorageName) const;

    /** ?üÊ?ËΩâÊ?ID | Generate Conversion ID */
    FString GenerateConversionID(const FString& ConversionName) const;

    /** ?≤Â?Ë≥áÊ?È°ûÂ??çÁ®± | Get Resource Type Name */
    FString GetResourceTypeName(EMingResourceType ResourceType) const;
};
