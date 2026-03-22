#include "MingBuildingResourceSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingBuildingResourceSystem::UMingBuildingResourceSystem()
{
    bIsInitialized = false;
}

void UMingBuildingResourceSystem::InitializeResourceSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("資源系統已經初始化 | Resource system already initialized"));
        return;
    }

    // 初始化資源信息 | Initialize resource info
    InitializeResourceInfo();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("資源系統初始化完成，共載入 %d 種資源類型 | Resource system initialized, loaded %d resource types"), ResourceInfoMap.Num(), ResourceInfoMap.Num());
}

TArray<EMingResourceType> UMingBuildingResourceSystem::GetAllResourceTypes() const
{
    TArray<EMingResourceType> ResourceTypes;
    
    for (const auto& ResourcePair : ResourceInfoMap)
    {
        ResourceTypes.Add(ResourcePair.Key);
    }
    
    return ResourceTypes;
}

FMingResourceData UMingBuildingResourceSystem::GetResourceInfo(EMingResourceType ResourceType) const
{
    if (const FMingResourceData* ResourceInfo = ResourceInfoMap.Find(ResourceType))
    {
        return *ResourceInfo;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到資源類型：%d | Resource type not found: %d"), static_cast<int32>(ResourceType), static_cast<int32>(ResourceType));
    return FMingResourceData();
}

FString UMingBuildingResourceSystem::CreateResourceCollection(EMingResourceType ResourceType, EMingCollectionMethod Method, const FString& LocationID)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("資源系統未初始化 | Resource system not initialized"));
        return TEXT("");
    }

    FString CollectionID = GenerateCollectionID(ResourceType);
    
    FMingResourceCollection NewCollection;
    NewCollection.CollectionID = CollectionID;
    NewCollection.ResourceType = ResourceType;
    NewCollection.CollectionMethod = Method;
    NewCollection.LocationID = LocationID;
    NewCollection.CollectionRate = 1.0f;
    NewCollection.Efficiency = CalculateCollectionEfficiency(CollectionID);
    NewCollection.BaseYield = GetResourceInfo(ResourceType).UnitValue * 10.0f;
    NewCollection.CurrentYield = NewCollection.BaseYield * NewCollection.Efficiency;
    NewCollection.bIsActive = false;
    NewCollection.bIsAutomated = (Method == EMingCollectionMethod::Automatic);
    NewCollection.TotalCollected = 0.0f;
    
    ResourceCollections.Add(CollectionID, NewCollection);
    
    UE_LOG(LogTemp, Log, TEXT("創建資源收集：%s，類型：%s | Created resource collection: %s, type: %s"), 
        *CollectionID, *GetResourceTypeName(ResourceType), *CollectionID, *GetResourceTypeName(ResourceType));
    return CollectionID;
}

bool UMingBuildingResourceSystem::StartResourceCollection(const FString& CollectionID, const FString& CollectorID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源收集：%s | Resource collection not found: %s"), *CollectionID, *CollectionID);
        return false;
    }

    FMingResourceCollection& Collection = ResourceCollections[CollectionID];
    
    if (!CheckCollectionConditions(CollectionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("資源收集條件不滿足：%s | Collection conditions not met: %s"), *CollectionID, *CollectionID);
        return false;
    }
    
    Collection.bIsActive = true;
    
    UE_LOG(LogTemp, Log, TEXT("開始資源收集：%s | Started resource collection: %s"), *CollectionID, *CollectionID);
    return true;
}

bool UMingBuildingResourceSystem::StopResourceCollection(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return false;
    }

    FMingResourceCollection& Collection = ResourceCollections[CollectionID];
    Collection.bIsActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("停止資源收集：%s | Stopped resource collection: %s"), *CollectionID, *CollectionID);
    return true;
}

float UMingBuildingResourceSystem::CollectResource(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源收集：%s | Collection not found: %s"), *CollectionID, *CollectionID);
        return 0.0f;
    }

    FMingResourceCollection& Collection = ResourceCollections[CollectionID];
    
    if (!Collection.bIsActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("資源收集未激活：%s | Collection not active: %s"), *CollectionID, *CollectionID);
        return 0.0f;
    }
    
    // 計算實際產量 | Calculate actual yield
    float ActualYield = Collection.CurrentYield * Collection.Efficiency;
    Collection.TotalCollected += ActualYield;
    
    // 廣播收集事件 | Broadcast collection event
    OnResourceCollected.Broadcast(Collection.ResourceType, ActualYield);
    
    UE_LOG(LogTemp, Log, TEXT("收集資源：%s，產量：%.2f | Collected resource: %s, yield: %.2f"), 
        *GetResourceTypeName(Collection.ResourceType), ActualYield, *GetResourceTypeName(Collection.ResourceType), ActualYield);
    
    return ActualYield;
}

FString UMingBuildingResourceSystem::CreateResourceStorage(const FString& StorageName, const FString& LocationID, float MaxCapacity)
{
    FString StorageID = GenerateStorageID(StorageName);
    
    FMingResourceStorage NewStorage;
    NewStorage.StorageID = StorageID;
    NewStorage.StorageName = StorageName;
    NewStorage.LocationID = LocationID;
    NewStorage.MaxCapacity = MaxCapacity;
    NewStorage.CurrentCapacity = 0.0f;
    NewStorage.UsedCapacity = 0.0f;
    
    ResourceStorages.Add(StorageID, NewStorage);
    
    UE_LOG(LogTemp, Log, TEXT("創建資源存儲：%s，名稱：%s，容量：%.0f | Created storage: %s, name: %s, capacity: %.0f"), 
        *StorageID, *StorageName, MaxCapacity, *StorageID, *StorageName, MaxCapacity);
    return StorageID;
}

bool UMingBuildingResourceSystem::StoreResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s | Storage not found: %s"), *StorageID, *StorageID);
        return false;
    }

    if (!CheckStorageLimits(StorageID, ResourceType, Quantity))
    {
        UE_LOG(LogTemp, Warning, TEXT("存儲容量不足：%s | Storage capacity insufficient: %s"), *StorageID, *StorageID);
        return false;
    }

    FMingResourceStorage& Storage = ResourceStorages[StorageID];
    
    // 獲取或創建資源數據 | Get or create resource data
    FMingResourceData& ResourceData = Storage.StoredResources.FindOrAdd(ResourceType);
    ResourceData.ResourceType = ResourceType;
    ResourceData.Quantity += Quantity;
    
    // 更新容量 | Update capacity
    UpdateStorageCapacity(StorageID);
    
    // 廣播存儲事件 | Broadcast storage event
    OnResourceStored.Broadcast(StorageID, ResourceType, Quantity);
    
    UE_LOG(LogTemp, Log, TEXT("存儲資源：%s，數量：%.2f，存儲：%s | Stored resource: %s, quantity: %.2f, storage: %s"), 
        *GetResourceTypeName(ResourceType), Quantity, *StorageID, *GetResourceTypeName(ResourceType), Quantity, *StorageID);
    return true;
}

float UMingBuildingResourceSystem::RetrieveResource(const FString& StorageID, EMingResourceType ResourceType, float Quantity)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return 0.0f;
    }

    FMingResourceStorage& Storage = ResourceStorages[StorageID];
    
    if (!Storage.StoredResources.Contains(ResourceType))
    {
        return 0.0f;
    }

    FMingResourceData& ResourceData = Storage.StoredResources[ResourceType];
    
    // 計算實際可提取數量 | Calculate actual retrievable quantity
    float ActualQuantity = FMath::Min(Quantity, ResourceData.Quantity);
    ResourceData.Quantity -= ActualQuantity;
    
    // 如果數量為0，移除資源 | Remove resource if quantity is 0
    if (ResourceData.Quantity <= 0.0f)
    {
        Storage.StoredResources.Remove(ResourceType);
    }
    
    // 更新容量 | Update capacity
    UpdateStorageCapacity(StorageID);
    
    UE_LOG(LogTemp, Log, TEXT("提取資源：%s，數量：%.2f，存儲：%s | Retrieved resource: %s, quantity: %.2f, storage: %s"), 
        *GetResourceTypeName(ResourceType), ActualQuantity, *StorageID, *GetResourceTypeName(ResourceType), ActualQuantity, *StorageID);
    
    return ActualQuantity;
}

FMingResourceData UMingBuildingResourceSystem::GetStoredResource(const FString& StorageID, EMingResourceType ResourceType) const
{
    if (const FMingResourceStorage* Storage = ResourceStorages.Find(StorageID))
    {
        if (const FMingResourceData* ResourceData = Storage->StoredResources.Find(ResourceType))
        {
            return *ResourceData;
        }
    }
    return FMingResourceData();
}

float UMingBuildingResourceSystem::GetStorageCapacity(const FString& StorageID) const
{
    if (const FMingResourceStorage* Storage = ResourceStorages.Find(StorageID))
    {
        return Storage->CurrentCapacity;
    }
    return 0.0f;
}

bool UMingBuildingResourceSystem::ConvertResources(const FString& ConversionID)
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源轉換：%s | Conversion not found: %s"), *ConversionID, *ConversionID);
        return false;
    }

    FMingResourceConversion& Conversion = ResourceConversions[ConversionID];
    
    if (!Conversion.bIsAvailable)
    {
        UE_LOG(LogTemp, Warning, TEXT("資源轉換不可用：%s | Conversion not available: %s"), *ConversionID, *ConversionID);
        return false;
    }
    
    if (!CheckConversionConditions(ConversionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("資源轉換條件不滿足：%s | Conversion conditions not met: %s"), *ConversionID, *ConversionID);
        return false;
    }
    
    // 執行轉換 | Execute conversion
    ProcessConversionComplete(ConversionID);
    
    UE_LOG(LogTemp, Log, TEXT("轉換資源完成：%s | Conversion completed: %s"), *ConversionID, *ConversionID);
    return true;
}

FString UMingBuildingResourceSystem::CreateResourceConversion(const FString& ConversionName, const TMap<EMingResourceType, float>& Input, const TMap<EMingResourceType, float>& Output)
{
    FString ConversionID = GenerateConversionID(ConversionName);
    
    FMingResourceConversion NewConversion;
    NewConversion.ConversionID = ConversionID;
    NewConversion.ConversionName = ConversionName;
    NewConversion.InputResources = Input;
    NewConversion.OutputResources = Output;
    NewConversion.ConversionTime = 1.0f;
    NewConversion.Efficiency = 1.0f;
    NewConversion.bIsAvailable = true;
    
    ResourceConversions.Add(ConversionID, NewConversion);
    
    UE_LOG(LogTemp, Log, TEXT("創建資源轉換：%s，名稱：%s | Created conversion: %s, name: %s"), 
        *ConversionID, *ConversionName, *ConversionID, *ConversionName);
    return ConversionID;
}

float UMingBuildingResourceSystem::CalculateCollectionEfficiency(const FString& CollectionID) const
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return 0.0f;
    }

    const FMingResourceCollection& Collection = ResourceCollections[CollectionID];
    
    // 基礎效率 | Base efficiency
    float Efficiency = Collection.CollectionRate;
    
    // 方法加成 | Method bonus
    switch (Collection.CollectionMethod)
    {
    case EMingCollectionMethod::Automatic:
        Efficiency *= 1.5f;
        break;
    case EMingCollectionMethod::Production:
        Efficiency *= 2.0f;
        break;
    case EMingCollectionMethod::Trade:
        Efficiency *= 1.0f;
        break;
    default:
        break;
    }
    
    return FMath::Clamp(Efficiency, 0.1f, 5.0f);
}

FMingResourceCollection UMingBuildingResourceSystem::GetCollectionStatus(const FString& CollectionID) const
{
    if (const FMingResourceCollection* Collection = ResourceCollections.Find(CollectionID))
    {
        return *Collection;
    }
    return FMingResourceCollection();
}

TArray<FMingResourceCollection> UMingBuildingResourceSystem::GetActiveCollections() const
{
    TArray<FMingResourceCollection> ActiveCollections;
    
    for (const auto& Pair : ResourceCollections)
    {
        if (Pair.Value.bIsActive)
        {
            ActiveCollections.Add(Pair.Value);
        }
    }
    
    return ActiveCollections;
}

bool UMingBuildingResourceSystem::TransportResource(const FString& FromStorageID, const FString& ToStorageID, EMingResourceType ResourceType, float Quantity)
{
    // 從源存儲提取 | Retrieve from source
    float Retrieved = RetrieveResource(FromStorageID, ResourceType, Quantity);
    if (Retrieved <= 0.0f)
    {
        return false;
    }
    
    // 存儲到目標 | Store to destination
    bool bSuccess = StoreResource(ToStorageID, ResourceType, Retrieved);
    if (!bSuccess)
    {
        // 如果存儲失敗，退回原存儲 | Return to source if storage fails
        StoreResource(FromStorageID, ResourceType, Retrieved);
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("運輸資源：%s，數量：%.2f，從 %s 到 %s | Transported resource: %s, quantity: %.2f, from %s to %s"), 
        *GetResourceTypeName(ResourceType), Retrieved, *FromStorageID, *ToStorageID, 
        *GetResourceTypeName(ResourceType), Retrieved, *FromStorageID, *ToStorageID);
    return true;
}

bool UMingBuildingResourceSystem::TradeResource(const FString& StorageID, EMingResourceType GiveResource, float GiveQuantity, EMingResourceType GetResource, float GetQuantity)
{
    // 檢查是否有足夠資源 | Check if enough resources
    FMingResourceData CurrentResource = GetStoredResource(StorageID, GiveResource);
    if (CurrentResource.Quantity < GiveQuantity)
    {
        UE_LOG(LogTemp, Warning, TEXT("資源不足，無法交易 | Insufficient resources for trade"));
        return false;
    }
    
    // 提取給出的資源 | Retrieve given resource
    float Retrieved = RetrieveResource(StorageID, GiveResource, GiveQuantity);
    if (Retrieved <= 0.0f)
    {
        return false;
    }
    
    // 存儲獲得的資源 | Store received resource
    bool bSuccess = StoreResource(StorageID, GetResource, GetQuantity);
    if (!bSuccess)
    {
        // 如果存儲失敗，退回原資源 | Return original resource if storage fails
        StoreResource(StorageID, GiveResource, Retrieved);
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("交易完成：交出 %.2f %s，獲得 %.2f %s | Trade completed: gave %.2f %s, received %.2f %s"), 
        GiveQuantity, *GetResourceTypeName(GiveResource), GetQuantity, *GetResourceTypeName(GetResource),
        GiveQuantity, *GetResourceTypeName(GiveResource), GetQuantity, *GetResourceTypeName(GetResource));
    return true;
}

bool UMingBuildingResourceSystem::CheckResourceAvailability(const FString& StorageID, EMingResourceType ResourceType, float RequiredQuantity) const
{
    FMingResourceData Resource = GetStoredResource(StorageID, ResourceType);
    return Resource.Quantity >= RequiredQuantity;
}

float UMingBuildingResourceSystem::GetTotalResourceQuantity(EMingResourceType ResourceType) const
{
    float Total = 0.0f;
    
    for (const auto& Pair : ResourceStorages)
    {
        if (const FMingResourceData* Resource = Pair.Value.StoredResources.Find(ResourceType))
        {
            Total += Resource->Quantity;
        }
    }
    
    return Total;
}

void UMingBuildingResourceSystem::ProcessResourceDecay(float DeltaTime)
{
    for (auto& StoragePair : ResourceStorages)
    {
        FMingResourceStorage& Storage = StoragePair.Value;
        
        for (auto& ResourcePair : Storage.StoredResources)
        {
            FMingResourceData& Resource = ResourcePair.Value;
            
            if (Resource.bIsPerishable && Resource.PerishRate > 0.0f)
            {
                float DecayAmount = Resource.Quantity * Resource.PerishRate * DeltaTime;
                Resource.Quantity -= DecayAmount;
                
                if (Resource.Quantity <= 0.0f)
                {
                    Resource.Quantity = 0.0f;
                }
            }
        }
        
        // 更新存儲容量 | Update storage capacity
        UpdateStorageCapacity(StoragePair.Key);
    }
}

void UMingBuildingResourceSystem::InitializeResourceInfo()
{
    // 定義所有資源類型的基礎信息 | Define base info for all resource types
    TArray<EMingResourceType> AllTypes;
    AllTypes.Add(EMingResourceType::Wood);
    AllTypes.Add(EMingResourceType::Stone);
    AllTypes.Add(EMingResourceType::Metal);
    AllTypes.Add(EMingResourceType::Food);
    AllTypes.Add(EMingResourceType::Water);
    AllTypes.Add(EMingResourceType::Coal);
    AllTypes.Add(EMingResourceType::Iron);
    AllTypes.Add(EMingResourceType::Copper);
    AllTypes.Add(EMingResourceType::Gold);
    AllTypes.Add(EMingResourceType::Silver);
    AllTypes.Add(EMingResourceType::Cotton);
    AllTypes.Add(EMingResourceType::Silk);
    AllTypes.Add(EMingResourceType::Tea);
    AllTypes.Add(EMingResourceType::Opium);
    AllTypes.Add(EMingResourceType::Medicine);
    AllTypes.Add(EMingResourceType::Weapons);
    AllTypes.Add(EMingResourceType::Ammunition);
    AllTypes.Add(EMingResourceType::Uniforms);
    AllTypes.Add(EMingResourceType::Horses);
    AllTypes.Add(EMingResourceType::Vehicles);
    AllTypes.Add(EMingResourceType::Fuel);
    AllTypes.Add(EMingResourceType::Rubber);
    AllTypes.Add(EMingResourceType::Paper);
    AllTypes.Add(EMingResourceType::Ink);
    AllTypes.Add(EMingResourceType::Books);
    AllTypes.Add(EMingResourceType::Tools);
    AllTypes.Add(EMingResourceType::Machinery);
    AllTypes.Add(EMingResourceType::Electronics);
    AllTypes.Add(EMingResourceType::Radio);
    AllTypes.Add(EMingResourceType::Telephone);
    AllTypes.Add(EMingResourceType::Telegraph);
    
    for (EMingResourceType Type : AllTypes)
    {
        FMingResourceData ResourceData;
        ResourceData.ResourceType = Type;
        ResourceData.Rarity = EMingResourceRarity::Common;
        ResourceData.Quality = EMingResourceQuality::Normal;
        ResourceData.Quantity = 0.0f;
        ResourceData.MaxQuantity = 100.0f;
        ResourceData.UnitWeight = 1.0f;
        ResourceData.UnitValue = GetResourceTypeWeight(Type);
        ResourceData.Description = GetResourceTypeName(Type);
        ResourceData.bIsStackable = true;
        
        // 易腐資源 | Perishable resources
        ResourceData.bIsPerishable = (Type == EMingResourceType::Food);
        ResourceData.PerishRate = ResourceData.bIsPerishable ? 0.01f : 0.0f;
        
        ResourceInfoMap.Add(Type, ResourceData);
    }
}

void UMingBuildingResourceSystem::UpdateCollectionYield(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return;
    }

    FMingResourceCollection& Collection = ResourceCollections[CollectionID];
    Collection.CurrentYield = Collection.BaseYield * Collection.Efficiency;
}

bool UMingBuildingResourceSystem::CheckCollectionConditions(const FString& CollectionID) const
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return false;
    }

    const FMingResourceCollection& Collection = ResourceCollections[CollectionID];
    
    // 檢查是否已經激活 | Check if already active
    if (Collection.bIsActive)
    {
        return false;
    }
    
    return true;
}

void UMingBuildingResourceSystem::ProcessCollectionComplete(const FString& CollectionID)
{
    // 收集完成處理邏輯 | Collection completion logic
    UE_LOG(LogTemp, Log, TEXT("資源收集完成：%s | Collection completed: %s"), *CollectionID, *CollectionID);
}

void UMingBuildingResourceSystem::UpdateStorageCapacity(const FString& StorageID)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return;
    }

    FMingResourceStorage& Storage = ResourceStorages[StorageID];
    
    // 計算已用容量 | Calculate used capacity
    float UsedCapacity = 0.0f;
    for (const auto& Pair : Storage.StoredResources)
    {
        UsedCapacity += Pair.Value.Quantity * Pair.Value.UnitWeight;
    }
    
    Storage.UsedCapacity = UsedCapacity;
    Storage.CurrentCapacity = Storage.MaxCapacity - UsedCapacity;
    
    // 檢查存儲是否滿了 | Check if storage is full
    if (Storage.CurrentCapacity <= 0.0f)
    {
        HandleStorageFull(StorageID);
    }
}

bool UMingBuildingResourceSystem::CheckStorageLimits(const FString& StorageID, EMingResourceType ResourceType, float Quantity) const
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return false;
    }

    const FMingResourceStorage& Storage = ResourceStorages[StorageID];
    FMingResourceData ResourceInfo = GetResourceInfo(ResourceType);
    
    float RequiredCapacity = Quantity * ResourceInfo.UnitWeight;
    
    return Storage.CurrentCapacity >= RequiredCapacity;
}

void UMingBuildingResourceSystem::HandleStorageFull(const FString& StorageID)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return;
    }

    const FMingResourceStorage& Storage = ResourceStorages[StorageID];
    
    UE_LOG(LogTemp, Warning, TEXT("存儲已滿：%s | Storage full: %s"), *StorageID, *StorageID);
    
    // 廣播存儲滿了事件 | Broadcast storage full event
    OnStorageFull.Broadcast(StorageID, Storage.MaxCapacity);
}

bool UMingBuildingResourceSystem::CheckConversionConditions(const FString& ConversionID) const
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        return false;
    }

    const FMingResourceConversion& Conversion = ResourceConversions[ConversionID];
    
    // 檢查輸入資源是否充足 | Check if input resources are sufficient
    // 注意：實際實現需要檢查特定存儲 | Note: Actual implementation needs to check specific storage
    
    return true;
}

void UMingBuildingResourceSystem::ProcessConversionComplete(const FString& ConversionID)
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        return;
    }

    FMingResourceConversion& Conversion = ResourceConversions[ConversionID];
    
    // 廣播轉換完成事件 | Broadcast conversion complete event
    OnResourceConverted.Broadcast(ConversionID, true);
    
    UE_LOG(LogTemp, Log, TEXT("轉換完成：%s | Conversion completed: %s"), *Conversion.ConversionName, *Conversion.ConversionName);
}

FString UMingBuildingResourceSystem::GenerateCollectionID(EMingResourceType ResourceType) const
{
    FString TypeName = GetResourceTypeName(ResourceType);
    FString Timestamp = FString::FromInt(FMath::Rand());
    return FString::Printf(TEXT("COL_%s_%s"), *TypeName, *Timestamp);
}

FString UMingBuildingResourceSystem::GenerateStorageID(const FString& StorageName) const
{
    FString Timestamp = FString::FromInt(FMath::Rand());
    return FString::Printf(TEXT("STOR_%s_%s"), *StorageName, *Timestamp);
}

FString UMingBuildingResourceSystem::GenerateConversionID(const FString& ConversionName) const
{
    FString Timestamp = FString::FromInt(FMath::Rand());
    return FString::Printf(TEXT("CONV_%s_%s"), *ConversionName, *Timestamp);
}

FString UMingBuildingResourceSystem::GetResourceTypeName(EMingResourceType ResourceType) const
{
    switch (ResourceType)
    {
    case EMingResourceType::Wood: return TEXT("Wood");
    case EMingResourceType::Stone: return TEXT("Stone");
    case EMingResourceType::Metal: return TEXT("Metal");
    case EMingResourceType::Food: return TEXT("Food");
    case EMingResourceType::Water: return TEXT("Water");
    case EMingResourceType::Coal: return TEXT("Coal");
    case EMingResourceType::Iron: return TEXT("Iron");
    case EMingResourceType::Copper: return TEXT("Copper");
    case EMingResourceType::Gold: return TEXT("Gold");
    case EMingResourceType::Silver: return TEXT("Silver");
    case EMingResourceType::Cotton: return TEXT("Cotton");
    case EMingResourceType::Silk: return TEXT("Silk");
    case EMingResourceType::Tea: return TEXT("Tea");
    case EMingResourceType::Opium: return TEXT("Opium");
    case EMingResourceType::Medicine: return TEXT("Medicine");
    case EMingResourceType::Weapons: return TEXT("Weapons");
    case EMingResourceType::Ammunition: return TEXT("Ammunition");
    case EMingResourceType::Uniforms: return TEXT("Uniforms");
    case EMingResourceType::Horses: return TEXT("Horses");
    case EMingResourceType::Vehicles: return TEXT("Vehicles");
    case EMingResourceType::Fuel: return TEXT("Fuel");
    case EMingResourceType::Rubber: return TEXT("Rubber");
    case EMingResourceType::Paper: return TEXT("Paper");
    case EMingResourceType::Ink: return TEXT("Ink");
    case EMingResourceType::Books: return TEXT("Books");
    case EMingResourceType::Tools: return TEXT("Tools");
    case EMingResourceType::Machinery: return TEXT("Machinery");
    case EMingResourceType::Electronics: return TEXT("Electronics");
    case EMingResourceType::Radio: return TEXT("Radio");
    case EMingResourceType::Telephone: return TEXT("Telephone");
    case EMingResourceType::Telegraph: return TEXT("Telegraph");
    default: return TEXT("Unknown");
    }
}

float UMingBuildingResourceSystem::GetResourceTypeWeight(EMingResourceType ResourceType) const
{
    // 資源基礎價值 | Resource base values
    switch (ResourceType)
    {
    case EMingResourceType::Gold:
    case EMingResourceType::Silver:
        return 10.0f;
    case EMingResourceType::Weapons:
    case EMingResourceType::Ammunition:
    case EMingResourceType::Medicine:
        return 5.0f;
    case EMingResourceType::Iron:
    case EMingResourceType::Copper:
    case EMingResourceType::Coal:
        return 3.0f;
    case EMingResourceType::Silk:
    case EMingResourceType::Tea:
        return 4.0f;
    case EMingResourceType::Opium:
        return 8.0f;
    case EMingResourceType::Machinery:
    case EMingResourceType::Electronics:
        return 6.0f;
    default:
        return 1.0f;
    }
}
