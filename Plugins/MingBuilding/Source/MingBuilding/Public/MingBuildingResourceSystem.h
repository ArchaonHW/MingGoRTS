#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingBuildingResourceSystem.generated.h"

/**
 * 資源類型枚舉 | Resource Type Enum
 * 定義民國時期各種資源類型 | Define various resource types from Republican era
 */
UENUM(BlueprintType)
enum class EMingResourceType : uint8
{
    // 基礎資源 | Basic Resources
    Wood              UMETA(DisplayName = "木材 | Wood"),
    Stone             UMETA(DisplayName = "石頭 | Stone"),
    Metal             UMETA(DisplayName = "金屬 | Metal"),
    Food              UMETA(DisplayName = "食物 | Food"),
    Water             UMETA(DisplayName = "水 | Water"),
    
    // 礦產資源 | Mineral Resources
    Coal              UMETA(DisplayName = "煤炭 | Coal"),
    Iron              UMETA(DisplayName = "鐵礦 | Iron"),
    Copper            UMETA(DisplayName = "銅礦 | Copper"),
    Gold              UMETA(DisplayName = "黃金 | Gold"),
    Silver            UMETA(DisplayName = "白銀 | Silver"),
    
    // 農產品 | Agricultural Products
    Cotton            UMETA(DisplayName = "棉花 | Cotton"),
    Silk              UMETA(DisplayName = "絲綢 | Silk"),
    Tea               UMETA(DisplayName = "茶葉 | Tea"),
    
    // 特殊資源 | Special Resources
    Opium             UMETA(DisplayName = "鴉片 | Opium"),
    Medicine          UMETA(DisplayName = "藥品 | Medicine"),
    
    // 軍事資源 | Military Resources
    Weapons           UMETA(DisplayName = "武器 | Weapons"),
    Ammunition        UMETA(DisplayName = "彈藥 | Ammunition"),
    Uniforms          UMETA(DisplayName = "軍服 | Uniforms"),
    Horses            UMETA(DisplayName = "馬匹 | Horses"),
    Vehicles          UMETA(DisplayName = "車輛 | Vehicles"),
    
    // 工業資源 | Industrial Resources
    Fuel              UMETA(DisplayName = "燃料 | Fuel"),
    Rubber            UMETA(DisplayName = "橡膠 | Rubber"),
    Paper             UMETA(DisplayName = "紙張 | Paper"),
    Ink               UMETA(DisplayName = "墨水 | Ink"),
    Books             UMETA(DisplayName = "書籍 | Books"),
    Tools             UMETA(DisplayName = "工具 | Tools"),
    Machinery         UMETA(DisplayName = "機械 | Machinery"),
    
    // 通訊設備 | Communication Equipment
    Electronics       UMETA(DisplayName = "電子設備 | Electronics"),
    Radio             UMETA(DisplayName = "無線電 | Radio"),
    Telephone         UMETA(DisplayName = "電話 | Telephone"),
    Telegraph         UMETA(DisplayName = "電報機 | Telegraph")
};

/**
 * 資源稀有度 | Resource Rarity
 */
UENUM(BlueprintType)
enum class EMingResourceRarity : uint8
{
    Common            UMETA(DisplayName = "普通 | Common"),
    Uncommon          UMETA(DisplayName = "稀有 | Uncommon"),
    Rare              UMETA(DisplayName = "珍稀 | Rare"),
    Epic              UMETA(DisplayName = "史詩 | Epic"),
    Legendary         UMETA(DisplayName = "傳說 | Legendary")
};

/**
 * 資源品質 | Resource Quality
 */
UENUM(BlueprintType)
enum class EMingResourceQuality : uint8
{
    Poor              UMETA(DisplayName = "劣質 | Poor"),
    Normal            UMETA(DisplayName = "普通 | Normal"),
    Good              UMETA(DisplayName = "優良 | Good"),
    Excellent         UMETA(DisplayName = "卓越 | Excellent"),
    Perfect           UMETA(DisplayName = "完美 | Perfect")
};

/**
 * 收集方法 | Collection Method
 */
UENUM(BlueprintType)
enum class EMingCollectionMethod : uint8
{
    Manual            UMETA(DisplayName = "手動收集 | Manual"),
    Automatic         UMETA(DisplayName = "自動收集 | Automatic"),
    Production        UMETA(DisplayName = "生產製造 | Production"),
    Trade             UMETA(DisplayName = "貿易獲取 | Trade"),
    Plunder           UMETA(DisplayName = "掠奪獲取 | Plunder"),
    Harvest           UMETA(DisplayName = "採集收穫 | Harvest"),
    Mining            UMETA(DisplayName = "開採挖掘 | Mining"),
    Fishing           UMETA(DisplayName = "捕魚 | Fishing"),
    Hunting           UMETA(DisplayName = "狩獵 | Hunting"),
    Foraging          UMETA(DisplayName = "採集覓食 | Foraging")
};

/**
 * 資源數據結構 | Resource Data Structure
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
 * 資源收集數據 | Resource Collection Data
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
 * 資源存儲數據 | Resource Storage Data
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
 * 資源轉換配方 | Resource Conversion Recipe
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
 * 資源系統事件委派 | Resource System Event Delegates
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingResourceCollected, EMingResourceType, ResourceType, float, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnMingResourceStored, const FString&, StorageID, EMingResourceType, ResourceType, float, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingResourceConverted, const FString&, ConversionID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMingStorageFull, const FString&, StorageID, float, Capacity);

/**
 * 資源管理系統 | Resource Management System
 * 
 * 管理 MingGoRTS 遊戲中的所有資源收集、存儲和轉換：
 * - 40種民國時期資源類型
 * - 多種收集方式（採集、生產、貿易、掠奪）
 * - 資源存儲和管理
 * - 資源轉換和合成
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGBUILDING_API UMingBuildingResourceSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingBuildingResourceSystem();

    /** 資源系統事件 | Resource System Events */
    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceCollected OnResourceCollected;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceStored OnResourceStored;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingResourceConverted OnResourceConverted;

    UPROPERTY(BlueprintAssignable, Category = "Resource System")
    FOnMingStorageFull OnStorageFull;

    /** 初始化資源系統 | Initialize Resource System */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void InitializeResourceSystem();

    /** 獲取所有資源類型 | Get All Resource Types */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<EMingResourceType> GetAllResourceTypes() const;

    /** 獲取資源信息 | Get Resource Info */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetResourceInfo(EMingResourceType ResourceType) const;

    /** 創建資源收集 | Create Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceCollection(EMingResourceType ResourceType, EMingCollectionMethod Method, const FString& LocationID);

    /** 開始資源收集 | Start Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StartResourceCollection(const FString& CollectionID, const FString& CollectorID);

    /** 停止資源收集 | Stop Resource Collection */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StopResourceCollection(const FString& CollectionID);

    /** 收集資源 | Collect Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float CollectResource(const FString& CollectionID);

    /** 創建資源存儲 | Create Resource Storage */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceStorage(const FString& StorageName, const FString& LocationID, float MaxCapacity);

    /** 存儲資源 | Store Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool StoreResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity);

    /** 提取資源 | Retrieve Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    float RetrieveResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity);

    /** 獲取存儲資源 | Get Stored Resource */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceData GetStoredResource(const FString& StorageID, EMingResourceType ResourceType) const;

    /** 獲取存儲容量 | Get Storage Capacity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetStorageCapacity(const FString& StorageID) const;

    /** 轉換資源 | Convert Resources */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool ConvertResources(const FString& ConversionID);

    /** 創建資源轉換 | Create Resource Conversion */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    FString CreateResourceConversion(const FString& ConversionName, const TMap<EMingResourceType, float>& Input, const TMap<EMingResourceType, float>& Output);

    /** 計算收集效率 | Calculate Collection Efficiency */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float CalculateCollectionEfficiency(const FString& CollectionID) const;

    /** 獲取收集狀態 | Get Collection Status */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    FMingResourceCollection GetCollectionStatus(const FString& CollectionID) const;

    /** 獲取所有活動收集 | Get All Active Collections */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    TArray<FMingResourceCollection> GetActiveCollections() const;

    /** 資源運輸 | Transport Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TransportResource(const FString& FromStorageID, const FString& ToStorageID, EMingResourceType ResourceType, float Quantity);

    /** 資源交易 | Trade Resource */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    bool TradeResource(const FString& StorageID, EMingResourceType GiveResource, float GiveQuantity, EMingResourceType GetResource, float GetQuantity);

    /** 檢查資源充足性 | Check Resource Availability */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    bool CheckResourceAvailability(const FString& StorageID, EMingResourceType ResourceType, float RequiredQuantity) const;

    /** 獲取資源總量 | Get Total Resource Quantity */
    UFUNCTION(BlueprintPure, Category = "Resource System")
    float GetTotalResourceQuantity(EMingResourceType ResourceType) const;

    /** 資源衰減處理 | Process Resource Decay */
    UFUNCTION(BlueprintCallable, Category = "Resource System")
    void ProcessResourceDecay(float DeltaTime);

protected:
    /** 資源信息映射 | Resource Info Map */
    UPROPERTY()
    TMap<EMingResourceType, FMingResourceData> ResourceInfoMap;

    /** 資源收集映射 | Resource Collection Map */
    UPROPERTY()
    TMap<FString, FMingResourceCollection> ResourceCollections;

    /** 資源存儲映射 | Resource Storage Map */
    UPROPERTY()
    TMap<FString, FMingResourceStorage> ResourceStorages;

    /** 資源轉換映射 | Resource Conversion Map */
    UPROPERTY()
    TMap<FString, FMingResourceConversion> ResourceConversions;

    /** 是否已初始化 | Is Initialized */
    bool bIsInitialized;

    /** 初始化資源信息 | Initialize Resource Info */
    void InitializeResourceInfo();

    /** 更新收集產量 | Update Collection Yield */
    void UpdateCollectionYield(const FString& CollectionID);

    /** 檢查收集條件 | Check Collection Conditions */
    bool CheckCollectionConditions(const FString& CollectionID) const;

    /** 處理收集完成 | Process Collection Complete */
    void ProcessCollectionComplete(const FString& CollectionID);

    /** 更新存儲容量 | Update Storage Capacity */
    void UpdateStorageCapacity(const FString& StorageID);

    /** 檢查存儲限制 | Check Storage Limits */
    bool CheckStorageLimits(const FString& StorageID, EMingResourceType ResourceType, float Quantity) const;

    /** 處理存儲滿了 | Handle Storage Full */
    void HandleStorageFull(const FString& StorageID);

    /** 檢查轉換條件 | Check Conversion Conditions */
    bool CheckConversionConditions(const FString& ConversionID) const;

    /** 處理轉換完成 | Process Conversion Complete */
    void ProcessConversionComplete(const FString& ConversionID);

    /** 生成收集ID | Generate Collection ID */
    FString GenerateCollectionID(EMingResourceType ResourceType) const;

    /** 生成存儲ID | Generate Storage ID */
    FString GenerateStorageID(const FString& StorageName) const;

    /** 生成轉換ID | Generate Conversion ID */
    FString GenerateConversionID(const FString& ConversionName) const;

    /** 獲取資源類型名稱 | Get Resource Type Name */
    FString GetResourceTypeName(EMingResourceType ResourceType) const;
};
