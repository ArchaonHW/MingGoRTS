#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingResourceSystem.generated.h"

/**
 * 資??類?X??? | Resource Type Enum
 * 定義民?X??X?種資??類?? | Define various resource types from Republican era
 */
UENUM(BlueprintType)
enum class EMingResourceType : uint8
{
    // ????資?? | Basic Resources
    Wood              UMETA(DisplayName = "???? | Wood"),
    Stone             UMETA(DisplayName = "??頭 | Stone"),
    Metal             UMETA(DisplayName = "??屬 | Metal"),
    Food              UMETA(DisplayName = "食物 | Food"),
    Water             UMETA(DisplayName = "??| Water"),
    
    // 礦產資?? | Mineral Resources
    Coal              UMETA(DisplayName = "??炭 | Coal"),
    Iron              UMETA(DisplayName = "??礦 | Iron"),
    Copper            UMETA(DisplayName = "??礦 | Copper"),
    Gold              UMETA(DisplayName = "黃?? | Gold"),
    Silver            UMETA(DisplayName = "???? | Silver"),
    
    // 農產X| Agricultural Products
    Cotton            UMETA(DisplayName = "棉花 | Cotton"),
    Silk              UMETA(DisplayName = "絲綢 | Silk"),
    Tea               UMETA(DisplayName = "???? | Tea"),
    
    // ????資?? | Special Resources
    Opium             UMETA(DisplayName = "鴉?? | Opium"),
    Medicine          UMETA(DisplayName = "???? | Medicine"),
    
    // 軍??資?? | Military Resources
    Weapons           UMETA(DisplayName = "武器 | Weapons"),
    Ammunition        UMETA(DisplayName = "彈藥 | Ammunition"),
    Uniforms          UMETA(DisplayName = "軍?? | Uniforms"),
    Horses            UMETA(DisplayName = "馬匹 | Horses"),
    Vehicles          UMETA(DisplayName = "車?? | Vehicles"),
    
    // 工業資?? | Industrial Resources
    Fuel              UMETA(DisplayName = "???? | Fuel"),
    Rubber            UMETA(DisplayName = "橡?? | Rubber"),
    Paper             UMETA(DisplayName = "紙張 | Paper"),
    Ink               UMETA(DisplayName = "墨水 | Ink"),
    Books             UMETA(DisplayName = "???? | Books"),
    Tools             UMETA(DisplayName = "工具 | Tools"),
    Machinery         UMETA(DisplayName = "機械 | Machinery"),
    
    // ????設?? | Communication Equipment
    Electronics       UMETA(DisplayName = "????設?? | Electronics"),
    Radio             UMETA(DisplayName = "???X| Radio"),
    Telephone         UMETA(DisplayName = "??話 | Telephone"),
    Telegraph         UMETA(DisplayName = "??報??| Telegraph")
};

/**
 * 資??稀??度 | Resource Rarity
 */
UENUM(BlueprintType)
enum class EMingResourceRarity : uint8
{
    Common            UMETA(DisplayName = "????| Common"),
    Uncommon          UMETA(DisplayName = "稀X| Uncommon"),
    Rare              UMETA(DisplayName = "???? | Rare"),
    Epic              UMETA(DisplayName = "??詩 | Epic"),
    Legendary         UMETA(DisplayName = "??說 | Legendary")
};

/**
 * 資?X?質 | Resource Quality
 */
UENUM(BlueprintType)
enum class EMingResourceQuality : uint8
{
    Poor              UMETA(DisplayName = "X?? | Poor"),
    Normal            UMETA(DisplayName = "????| Normal"),
    Good              UMETA(DisplayName = "??良 | Good"),
    Excellent         UMETA(DisplayName = "???? | Excellent"),
    Perfect           UMETA(DisplayName = "完?? | Perfect")
};

/**
 * ???X??? | Collection Method
 */
UENUM(BlueprintType)
enum class EMingCollectionMethod : uint8
{
    Manual            UMETA(DisplayName = "???X??? | Manual"),
    Automatic         UMETA(DisplayName = "???X??? | Automatic"),
    Production        UMETA(DisplayName = "??產製??| Production"),
    Trade             UMETA(DisplayName = "貿?X??? | Trade"),
    Plunder           UMETA(DisplayName = "??奪???? | Plunder"),
    Harvest           UMETA(DisplayName = "???X?穫 | Harvest"),
    Mining            UMETA(DisplayName = "??採???? | Mining"),
    Fishing           UMETA(DisplayName = "???? | Fishing"),
    Hunting           UMETA(DisplayName = "??獵 | Hunting"),
    Foraging          UMETA(DisplayName = "????覓?? | Foraging")
};

/**
 * 資?X???結?? | Resource Data Structure
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
        Description = TEXT(""};
        bIsStackable = true;
        bIsPerishable = false;
        PerishRate = 0.0f;
    }
};

/**
 * 資?X??X??? | Resource Collection Data
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
        CollectionID = TEXT(""};
        ResourceType = EMingResourceType::Wood;
        CollectionMethod = EMingCollectionMethod::Manual;
        LocationID = TEXT(""};
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
 * 資??存儲???? | Resource Storage Data
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
        StorageID = TEXT(""};
        StorageName = TEXT(""};
        LocationID = TEXT(""};
        MaxCapacity = 1000.0f;
        CurrentCapacity = 0.0f;
        UsedCapacity = 0.0f;
        bIsClimateControlled = false;
        bIsSecure = false;
        SecurityLevel = 0.0f;
    }
};

/**
 * 資??轉?X?方 | Resource Conversion Recipe
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
        ConversionID = TEXT(""};
        ConversionName = TEXT(""};
        ConversionTime = 1.0f;
        Efficiency = 1.0f;
        bIsAvailable = true;
    }
};

/**
 * 資??系統事件委派 | Resource System Event Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingResourceCollected, EMingResourceType, ResourceType, float, Quantity};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMingResourceStored, const FString&, StorageID, EMingResourceType, ResourceType, float, Quantity};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingResourceConverted, const FString&, ConversionID, bool, bSuccess};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingStorageFull, const FString&, StorageID, float, Capacity};

/**
 * 資??管??系統 | Resource Management System
 * 
 * 管?? MingGoRTS ??戲中?X?????源收??、?X???轉???? * - 40種?X??X???源?X * - 多種???X???（採??、?X?、貿??、??奪??
 * - 資??存儲??管X * - 資??轉?X??X */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGBUILDING_API UMingBuildingResourceSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingBuildingResourceSystem();

    /** 資??系統事件 | Resource System Events */
    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceCollected OnResourceCollected;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceStored OnResourceStored;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceConverted OnResourceConverted;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingStorageFull OnStorageFull;

    /** ???X???源系??| Initialize Resource System */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void InitializeResourceSystem();

    /** ???X?????源?X| Get All Resource Types */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<EMingResourceType> GetAllResourceTypes() const;

    /** ????資??信息 | Get Resource Info */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetResourceInfo(EMingResourceType ResourceType) const;

    /** ??建資?X??? | Create Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceCollection(EMingResourceType ResourceType, EMingCollectionMethod Method, const FString& LocationID};

    /** ????資?X??? | Start Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StartResourceCollection(const FString& CollectionID, const FString& CollectorID};

    /** ??止資?X??? | Stop Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StopResourceCollection(const FString& CollectionID};

    /** ????資?? | Collect Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float CollectResource(const FString& CollectionID};

    /** ??建資??存儲 | Create Resource Storage */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceStorage(const FString& StorageName, const FString& LocationID, float MaxCapacity};

    /** 存儲資?? | Store Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StoreResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity};

    /** ????資?? | Retrieve Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float RetrieveResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity};

    /** ????存儲資?? | Get Stored Resource */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetStoredResource(const FString& StorageID, EMingResourceType ResourceType) const;

    /** ????存儲容?? | Get Storage Capacity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetStorageCapacity(const FString& StorageID) const;

    /** 轉??資?? | Convert Resources */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool ConvertResources(const FString& ConversionID};

    /** ??建資??轉?? | Create Resource Conversion */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceConversion(const FString& ConversionName, const TMap<EMingResourceType, float>& Input, const TMap<EMingResourceType, float>& Output};

    /** 計?X??X??? | Calculate Collection Efficiency */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float CalculateCollectionEfficiency(const FString& CollectionID) const;

    /** ???X??X?X| Get Collection Status */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceCollection GetCollectionStatus(const FString& CollectionID) const;

    /** ???X???活??收X| Get All Active Collections */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<FMingResourceCollection> GetActiveCollections() const;

    /** 資?X?輸 | Transport Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TransportResource(const FString& FromStorageID, const FString& ToStorageID, EMingResourceType ResourceType, float Quantity};

    /** 資??交?? | Trade Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TradeResource(const FString& StorageID, EMingResourceType GiveResource, float GiveQuantity, EMingResourceType GetResource, float GetQuantity};

    /** 檢查資?X?足X| Check Resource Availability */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    bool CheckResourceAvailability(const FString& StorageID, EMingResourceType ResourceType, float RequiredQuantity) const;

    /** ????資??總?? | Get Total Resource Quantity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetTotalResourceQuantity(EMingResourceType ResourceType) const;

    /** 資??衰?X??? | Process Resource Decay */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void ProcessResourceDecay(float DeltaTime};

protected:
    /** 資??信息???? | Resource Info Map */
    UPROPERTY()
    TMap<EMingResourceType, FMingResourceData> ResourceInfoMap;

    /** 資?X??X??? | Resource Collection Map */
    UPROPERTY()
    TMap<FString, FMingResourceCollection> ResourceCollections;

    /** 資??存儲???? | Resource Storage Map */
    UPROPERTY()
    TMap<FString, FMingResourceStorage> ResourceStorages;

    /** 資??轉?X??? | Resource Conversion Map */
    UPROPERTY()
    TMap<FString, FMingResourceConversion> ResourceConversions;

    /** ??否已??始?? | Is Initialized */
    bool bIsInitialized;

    /** ???X???源信X| Initialize Resource Info */
    void InitializeResourceInfo();

    /** ??新???X??? | Update Collection Yield */
    void UpdateCollectionYield(const FString& CollectionID};

    /** 檢查????條件 | Check Collection Conditions */
    bool CheckCollectionConditions(const FString& CollectionID) const;

    /** ???X???完?? | Process Collection Complete */
    void ProcessCollectionComplete(const FString& CollectionID};

    /** ??新存儲容?? | Update Storage Capacity */
    void UpdateStorageCapacity(const FString& StorageID};

    /** 檢查存儲??制 | Check Storage Limits */
    bool CheckStorageLimits(const FString& StorageID, EMingResourceType ResourceType, float Quantity) const;

    /** ????存儲滿?? | Handle Storage Full */
    void HandleStorageFull(const FString& StorageID};

    /** 檢查轉??條件 | Check Conversion Conditions */
    bool CheckConversionConditions(const FString& ConversionID) const;

    /** ????轉??完?? | Process Conversion Complete */
    void ProcessConversionComplete(const FString& ConversionID};

    /** ???X???ID | Generate Collection ID */
    FString GenerateCollectionID(EMingResourceType ResourceType) const;

    /** ????存儲ID | Generate Storage ID */
    FString GenerateStorageID(const FString& StorageName) const;

    /** ????轉??ID | Generate Conversion ID */
    FString GenerateConversionID(const FString& ConversionName) const;

    /** ????資??類?X?稱 | Get Resource Type Name */
    FString GetResourceTypeName(EMingResourceType ResourceType) const;
};

