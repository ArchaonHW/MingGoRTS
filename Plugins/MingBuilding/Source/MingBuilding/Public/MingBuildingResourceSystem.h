#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingResourceSystem.generated.h"

/**
 * ��??��?X??? | Resource Type Enum
 * �w�q��?X??X?�ظ�??��?? | Define various resource types from Republican era
 */
UENUM(BlueprintType)
enum class EMingResourceType: uint8 {
    // 摧毀��?? | Basic Resources
    Wood              UMETA(DisplayName = "摧毀 | Wood"),
    Stone             UMETA(DisplayName = "??�Y | Stone"),
    Metal             UMETA(DisplayName = "??�� | Metal"),
    Food              UMETA(DisplayName = "���� | Food"),
    Water             UMETA(DisplayName = "??| Water"),
    
    // �q����?? | Mineral Resources
    Coal              UMETA(DisplayName = "??�� | Coal"),
    Iron              UMETA(DisplayName = "??�q | Iron"),
    Copper            UMETA(DisplayName = "??�q | Copper"),
    Gold              UMETA(DisplayName = "��?? | Gold"),
    Silver            UMETA(DisplayName = "摧毀 | Silver"),
    
    // �A��X| Agricultural Products
    Cotton            UMETA(DisplayName = "�֪� | Cotton"),
    Silk              UMETA(DisplayName = "���� | Silk"),
    Tea               UMETA(DisplayName = "摧毀 | Tea"),
    
    // 摧毀��?? | Special Resources
    Opium             UMETA(DisplayName = "�~?? | Opium"),
    Medicine          UMETA(DisplayName = "摧毀 | Medicine"),
    
    // �x??��?? | Military Resources
    Weapons           UMETA(DisplayName = "�Z�� | Weapons"),
    Ammunition        UMETA(DisplayName = "�u�� | Ammunition"),
    Uniforms          UMETA(DisplayName = "�x?? | Uniforms"),
    Horses            UMETA(DisplayName = "���� | Horses"),
    Vehicles          UMETA(DisplayName = "��?? | Vehicles"),
    
    // �u�~��?? | Industrial Resources
    Fuel              UMETA(DisplayName = "摧毀 | Fuel"),
    Rubber            UMETA(DisplayName = "��?? | Rubber"),
    Paper             UMETA(DisplayName = "�ȱi | Paper"),
    Ink               UMETA(DisplayName = "���� | Ink"),
    Books             UMETA(DisplayName = "摧毀 | Books"),
    Tools             UMETA(DisplayName = "�u�� | Tools"),
    Machinery         UMETA(DisplayName = "���� | Machinery"),
    
    // 摧毀�]?? | Communication Equipment
    Electronics       UMETA(DisplayName = "摧毀�]?? | Electronics"),
    Radio             UMETA(DisplayName = "???X| Radio"),
    Telephone         UMETA(DisplayName = "??�� | Telephone"),
    Telegraph         UMETA(DisplayName = "??��??| Telegraph")
};

/**
 * ��??�}??�� | Resource Rarity
 */
UENUM(BlueprintType)
enum class EMingResourceRarity: uint8 {
    Common            UMETA(DisplayName = "摧毀| Common"),
    Uncommon          UMETA(DisplayName = "�}X| Uncommon"),
    Rare              UMETA(DisplayName = "摧毀 | Rare"),
    Epic              UMETA(DisplayName = "??�� | Epic"),
    Legendary         UMETA(DisplayName = "??�� | Legendary")
};

/**
 * ��?X?�� | Resource Quality
 */
UENUM(BlueprintType)
enum class EMingResourceQuality: uint8 {
    Poor              UMETA(DisplayName = "X?? | Poor"),
    Normal            UMETA(DisplayName = "摧毀| Normal"),
    Good              UMETA(DisplayName = "??�} | Good"),
    Excellent         UMETA(DisplayName = "摧毀 | Excellent"),
    Perfect           UMETA(DisplayName = "��?? | Perfect")
};

/**
 * 目標數量 | Collection Method
 */
UENUM(BlueprintType)
enum class EMingCollectionMethod: uint8 {
    Manual            UMETA(DisplayName = "目標數量 | Manual"),
    Automatic         UMETA(DisplayName = "目標數量 | Automatic"),
    Production        UMETA(DisplayName = "??���s??| Production"),
    Trade             UMETA(DisplayName = "�T?X??? | Trade"),
    Plunder           UMETA(DisplayName = "??��摧毀 | Plunder"),
    Harvest           UMETA(DisplayName = "???X?ì | Harvest"),
    Mining            UMETA(DisplayName = "??��摧毀 | Mining"),
    Fishing           UMETA(DisplayName = "摧毀 | Fishing"),
    Hunting           UMETA(DisplayName = "??�y | Hunting"),
    Foraging          UMETA(DisplayName = "摧毀�V?? | Foraging")
};

/**
 * ��?X???��?? | Resource Data Structure
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
);

/**
 * ��?X??X??? | Resource Collection Data
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
);

/**
 * ��??�s�x摧毀 | Resource Storage Data
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
);

/**
 * ��??��?X?�� | Resource Conversion Recipe
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
);

/**
 * ��??�t�Ψƥ�e�� | Resource System Event Delegates
 */





/**
 * ��??��??�t�� | Resource Management System
 * 
 * ��?? MingGoRTS ??����?X摧毀?����??�B?X???��摧毀 * - 40��?X??X???��?X * - �h��目標數量�]��??�B?X?�B�T??�B??��??
 * - ��??�s�x??��X * - ��??��?X??X */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGBUILDING_API UMingBuildingResourceSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingBuildingResourceSystem();

    /** ��??�t�Ψƥ� | Resource System Events */
    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceCollected OnResourceCollected;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceStored OnResourceStored;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceConverted OnResourceConverted;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingStorageFull OnStorageFull;

    /** 目標數量���t??| Initialize Resource System */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void InitializeResourceSystem();

    /** 目標數量??��?X| Get All Resource Types */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<EMingResourceType> GetAllResourceTypes() const;

    /** 摧毀��??�H�� | Get Resource Info */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetResourceInfo(EMingResourceType ResourceType) const;

    /** ??�ظ�?X??? | Create Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceCollection(EMingResourceType ResourceType, EMingCollectionMethod Method, const FString& LocationID);

    /** 摧毀��?X??? | Start Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StartResourceCollection(const FString& CollectionID, const FString& CollectorID);

    /** ??���?X??? | Stop Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StopResourceCollection(const FString& CollectionID);

    /** 摧毀��?? | Collect Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float CollectResource(const FString& CollectionID);

    /** ??�ظ�??�s�x | Create Resource Storage */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceStorage(const FString& StorageName, const FString& LocationID, float MaxCapacity);

    /** �s�x��?? | Store Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StoreResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity);

    /** 摧毀��?? | Retrieve Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float RetrieveResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity);

    /** 摧毀�s�x��?? | Get Stored Resource */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetStoredResource(const FString& StorageID, EMingResourceType ResourceType) const;

    /** 摧毀�s�x�e?? | Get Storage Capacity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetStorageCapacity(const FString& StorageID) const;

    /** ��??��?? | Convert Resources */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool ConvertResources(const FString& ConversionID);

    /** ??�ظ�??��?? | Create Resource Conversion */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceConversion(const FString& ConversionName, const TMap<EMingResourceType, float>& Input, const TMap<EMingResourceType, float>& Output);

    /** �p?X??X??? | Calculate Collection Efficiency */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float CalculateCollectionEfficiency(const FString& CollectionID) const;

    /** 故事重要性?X| Get Collection Status */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceCollection GetCollectionStatus(const FString& CollectionID) const;

    /** 目標數量��??��X| Get All Active Collections */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<FMingResourceCollection> GetActiveCollections() const;

    /** ��?X?�� | Transport Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TransportResource(const FString& FromStorageID, const FString& ToStorageID, EMingResourceType ResourceType, float Quantity);

    /** ��??��?? | Trade Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TradeResource(const FString& StorageID, EMingResourceType GiveResource, float GiveQuantity, EMingResourceType GetResource, float GetQuantity);

    /** �ˬd��?X?��X| Check Resource Availability */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    bool CheckResourceAvailability(const FString& StorageID, EMingResourceType ResourceType, float RequiredQuantity) const;

    /** 摧毀��??�`?? | Get Total Resource Quantity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetTotalResourceQuantity(EMingResourceType ResourceType) const;

    /** ��??�I?X??? | Process Resource Decay */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void ProcessResourceDecay(float DeltaTime);

protected:
    /** ��??�H��摧毀 | Resource Info Map */
    UPROPERTY()
    TMap<EMingResourceType, FMingResourceData> ResourceInfoMap;

    /** ��?X??X??? | Resource Collection Map */
    UPROPERTY()
    TMap<FString, FMingResourceCollection> ResourceCollections;

    /** ��??�s�x摧毀 | Resource Storage Map */
    UPROPERTY()
    TMap<FString, FMingResourceStorage> ResourceStorages;

    /** ��??��?X??? | Resource Conversion Map */
    UPROPERTY()
    TMap<FString, FMingResourceConversion> ResourceConversions;

    /** ??�_�w??�l?? | Is Initialized */
    bool bIsInitialized;

    /** 目標數量���HX| Initialize Resource Info */
    void InitializeResourceInfo();

    /** ??�s目標數量 | Update Collection Yield */
    void UpdateCollectionYield(const FString& CollectionID);

    /** �ˬd摧毀���� | Check Collection Conditions */
    bool CheckCollectionConditions(const FString& CollectionID) const;

    /** 目標數量��?? | Process Collection Complete */
    void ProcessCollectionComplete(const FString& CollectionID);

    /** ??�s�s�x�e?? | Update Storage Capacity */
    void UpdateStorageCapacity(const FString& StorageID);

    /** �ˬd�s�x??�� | Check Storage Limits */
    bool CheckStorageLimits(const FString& StorageID, EMingResourceType ResourceType, float Quantity) const;

    /** 摧毀�s�x��?? | Handle Storage Full */
    void HandleStorageFull(const FString& StorageID);

    /** �ˬd��??���� | Check Conversion Conditions */
    bool CheckConversionConditions(const FString& ConversionID) const;

    /** 摧毀��??��?? | Process Conversion Complete */
    void ProcessConversionComplete(const FString& ConversionID);

    /** 目標數量ID | Generate Collection ID */
    FString GenerateCollectionID(EMingResourceType ResourceType) const;

    /** 摧毀�s�xID | Generate Storage ID */
    FString GenerateStorageID(const FString& StorageName) const;

    /** 摧毀��??ID | Generate Conversion ID */
    FString GenerateConversionID(const FString& ConversionName) const;

    /** 摧毀��??��?X?�� | Get Resource Type Name */
    FString GetResourceTypeName(EMingResourceType ResourceType) const;
};

