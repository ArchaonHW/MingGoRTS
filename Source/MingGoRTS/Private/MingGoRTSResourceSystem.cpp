#include "MingGoRTSResourceSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSResourceSystem::UMingGoRTSResourceSystem()
{
    bIsInitialized = false;
}

void UMingGoRTSResourceSystem::InitializeResourceSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("資源系統已經初始化"));
        return;
    }

    // 初始化資源信息
    InitializeResourceInfo();
    
    // 初始化歷史資源
    InitializeHistoricalResources();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("資源系統初始化完成，共載入 %d 種資源類型"), ResourceInfoMap.Num());
}

TArray<EResourceType> UMingGoRTSResourceSystem::GetAllResourceTypes() const
{
    TArray<EResourceType> ResourceTypes;
    
    for (const auto& ResourcePair : ResourceInfoMap)
    {
        ResourceTypes.Add(ResourcePair.Key);
    }
    
    return ResourceTypes;
}

FResourceData UMingGoRTSResourceSystem::GetResourceInfo(EResourceType ResourceType) const
{
    if (const FResourceData* ResourceInfo = ResourceInfoMap.Find(ResourceType))
    {
        return *ResourceInfo;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到資源類型：%d"), static_cast<int32>(ResourceType));
    return FResourceData();
}

FString UMingGoRTSResourceSystem::CreateResourceCollection(EResourceType ResourceType, ECollectionMethod Method, const FString& LocationID)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("資源系統未初始化"));
        return TEXT("");
    }

    FString CollectionID = GenerateCollectionID(ResourceType);
    
    FResourceCollection NewCollection;
    NewCollection.CollectionID = CollectionID;
    NewCollection.ResourceType = ResourceType;
    NewCollection.CollectionMethod = Method;
    NewCollection.LocationID = LocationID;
    NewCollection.CollectionRate = 1.0f;
    NewCollection.Efficiency = CalculateCollectionEfficiency(CollectionID);
    NewCollection.BaseYield = GetResourceInfo(ResourceType).UnitValue * 10.0f;
    NewCollection.CurrentYield = NewCollection.BaseYield * NewCollection.Efficiency;
    NewCollection.bIsActive = false;
    NewCollection.bIsAutomated = (Method == ECollectionMethod::Automatic);
    NewCollection.StartTime = FDateTime::Now();
    NewCollection.TotalCollected = 0.0f;
    
    // 設置所需工具和技能
    SetRequiredToolsAndSkills(NewCollection);
    
    // 設置環境因素
    SetEnvironmentalFactors(NewCollection);
    
    ResourceCollections.Add(CollectionID, NewCollection);
    
    UE_LOG(LogTemp, Log, TEXT("創建資源收集：%s，類型：%s，方法：%s"), *CollectionID, *GetResourceTypeName(ResourceType), *StaticEnum<ECollectionMethod>()->GetValueAsString(Method));
    return CollectionID;
}

bool UMingGoRTSResourceSystem::StartResourceCollection(const FString& CollectionID, const FString& CollectorID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源收集：%s"), *CollectionID);
        return false;
    }

    FResourceCollection& Collection = ResourceCollections[CollectionID];
    
    if (!CheckCollectionConditions(CollectionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("資源收集條件不滿足：%s"), *CollectionID);
        return false;
    }

    Collection.CollectorID = CollectorID;
    Collection.bIsActive = true;
    Collection.StartTime = FDateTime::Now();
    Collection.LastCollectionTime = FDateTime::Now();
    
    UE_LOG(LogTemp, Log, TEXT("開始資源收集：%s，收集者：%s"), *CollectionID, *CollectorID);
    return true;
}

bool UMingGoRTSResourceSystem::StopResourceCollection(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源收集：%s"), *CollectionID);
        return false;
    }

    FResourceCollection& Collection = ResourceCollections[CollectionID];
    Collection.bIsActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("停止資源收集：%s"), *CollectionID);
    return true;
}

float UMingGoRTSResourceSystem::CollectResource(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源收集：%s"), *CollectionID);
        return 0.0f;
    }

    FResourceCollection& Collection = ResourceCollections[CollectionID];
    
    if (!Collection.bIsActive)
    {
        return 0.0f;
    }

    // 更新產量
    UpdateCollectionYield(CollectionID);
    
    // 計算收集量
    float CollectedAmount = Collection.CurrentYield * Collection.CollectionRate;
    
    // 應用環境影響
    float EnvironmentalImpact = CalculateEnvironmentalImpact(CollectionID);
    CollectedAmount *= EnvironmentalImpact;
    
    // 更新總收集量
    Collection.TotalCollected += CollectedAmount;
    Collection.LastCollectionTime = FDateTime::Now();
    
    // 廣播收集事件
    OnResourceCollected.Broadcast(Collection.ResourceType, CollectedAmount);
    
    // 添加歷史記錄
    AddResourceHistory(FString::Printf(TEXT("收集資源：%s，數量：%.2f"), *GetResourceTypeName(Collection.ResourceType), CollectedAmount));
    
    UE_LOG(LogTemp, Log, TEXT("收集資源：%s，數量：%.2f"), *GetResourceTypeName(Collection.ResourceType), CollectedAmount);
    return CollectedAmount;
}

FString UMingGoRTSResourceSystem::CreateResourceStorage(const FString& StorageName, const FString& LocationID, float MaxCapacity)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("資源系統未初始化"));
        return TEXT("");
    }

    FString StorageID = GenerateStorageID(StorageName);
    
    FResourceStorage NewStorage;
    NewStorage.StorageID = StorageID;
    NewStorage.StorageName = StorageName;
    NewStorage.LocationID = LocationID;
    NewStorage.MaxCapacity = MaxCapacity;
    NewStorage.CurrentCapacity = 0.0f;
    NewStorage.UsedCapacity = 0.0f;
    NewStorage.bIsClimateControlled = false;
    NewStorage.bIsSecure = false;
    NewStorage.SecurityLevel = 0.0f;
    
    // 設置允許的資源類型
    SetAllowedResourceTypes(NewStorage);
    
    ResourceStorages.Add(StorageID, NewStorage);
    
    UE_LOG(LogTemp, Log, TEXT("創建資源存儲：%s，容量：%.2f"), *StorageID, MaxCapacity);
    return StorageID;
}

bool UMingGoRTSResourceSystem::StoreResource(const FString& StorageID, EResourceType ResourceType, float Quantity)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s"), *StorageID);
        return false;
    }

    FResourceStorage& Storage = ResourceStorages[StorageID];
    
    // 檢查存儲限制
    if (!CheckStorageLimits(StorageID, ResourceType, Quantity))
    {
        HandleStorageFull(StorageID);
        return false;
    }

    // 獲取現有資源
    FResourceData* ExistingResource = Storage.StoredResources.Find(ResourceType);
    if (ExistingResource)
    {
        ExistingResource->Quantity += Quantity;
        ExistingResource->Quantity = FMath::Min(ExistingResource->Quantity, ExistingResource->MaxQuantity);
    }
    else
    {
        FResourceData NewResource = GetResourceInfo(ResourceType);
        NewResource.Quantity = Quantity;
        Storage.StoredResources.Add(ResourceType, NewResource);
    }
    
    // 更新存儲容量
    UpdateStorageCapacity(StorageID);
    
    // 廣播存儲事件
    OnResourceStored.Broadcast(StorageID, ResourceType, Quantity);
    
    // 添加歷史記錄
    AddResourceHistory(FString::Printf(TEXT("存儲資源：%s，數量：%.2f，存儲：%s"), *GetResourceTypeName(ResourceType), Quantity, *StorageID));
    
    UE_LOG(LogTemp, Log, TEXT("存儲資源：%s，數量：%.2f，存儲：%s"), *GetResourceTypeName(ResourceType), Quantity, *StorageID);
    return true;
}

float UMingGoRTSResourceSystem::RetrieveResource(const FString& StorageID, EResourceType ResourceType, float Quantity)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s"), *StorageID);
        return 0.0f;
    }

    FResourceStorage& Storage = ResourceStorages[StorageID];
    
    FResourceData* ExistingResource = Storage.StoredResources.Find(ResourceType);
    if (!ExistingResource || ExistingResource->Quantity <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("存儲中沒有足夠的資源：%s"), *GetResourceTypeName(ResourceType));
        return 0.0f;
    }
    
    // 計算實際提取量
    float RetrievedAmount = FMath::Min(Quantity, ExistingResource->Quantity);
    ExistingResource->Quantity -= RetrievedAmount;
    
    // 如果資源為0，從存儲中移除
    if (ExistingResource->Quantity <= 0.0f)
    {
        Storage.StoredResources.Remove(ResourceType);
    }
    
    // 更新存儲容量
    UpdateStorageCapacity(StorageID);
    
    // 添加歷史記錄
    AddResourceHistory(FString::Printf(TEXT("提取資源：%s，數量：%.2f，存儲：%s"), *GetResourceTypeName(ResourceType), RetrievedAmount, *StorageID));
    
    UE_LOG(LogTemp, Log, TEXT("提取資源：%s，數量：%.2f，存儲：%s"), *GetResourceTypeName(ResourceType), RetrievedAmount, *StorageID);
    return RetrievedAmount;
}

FResourceData UMingGoRTSResourceSystem::GetStoredResource(const FString& StorageID, EResourceType ResourceType) const
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s"), *StorageID);
        return FResourceData();
    }

    const FResourceStorage& Storage = ResourceStorages[StorageID];
    
    if (const FResourceData* Resource = Storage.StoredResources.Find(ResourceType))
    {
        return *Resource;
    }
    
    return FResourceData();
}

float UMingGoRTSResourceSystem::GetStorageCapacity(const FString& StorageID) const
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s"), *StorageID);
        return 0.0f;
    }

    const FResourceStorage& Storage = ResourceStorages[StorageID];
    return Storage.UsedCapacity / Storage.MaxCapacity;
}

bool UMingGoRTSResourceSystem::ConvertResources(const FString& ConversionID)
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源轉換：%s"), *ConversionID);
        return false;
    }

    FResourceConversion& Conversion = ResourceConversions[ConversionID];
    
    if (!CheckConversionConditions(ConversionID))
    {
        UE_LOG(LogTemp, Warning, TEXT("資源轉換條件不滿足：%s"), *ConversionID);
        return false;
    }

    // 檢查輸入資源
    for (const auto& InputPair : Conversion.InputResources)
    {
        float RequiredQuantity = InputPair.Value;
        bool bHasEnoughResource = false;
        
        // 檢查所有存儲
        for (const auto& StoragePair : ResourceStorages)
        {
            const FResourceStorage& Storage = StoragePair.Value;
            if (const FResourceData* Resource = Storage.StoredResources.Find(InputPair.Key))
            {
                if (Resource->Quantity >= RequiredQuantity)
                {
                    bHasEnoughResource = true;
                    break;
                }
            }
        }
        
        if (!bHasEnoughResource)
        {
            UE_LOG(LogTemp, Warning, TEXT("輸入資源不足：%s"), *GetResourceTypeName(InputPair.Key));
            return false;
        }
    }

    // 消耗輸入資源
    for (const auto& InputPair : Conversion.InputResources)
    {
        float RequiredQuantity = InputPair.Value;
        float RemainingQuantity = RequiredQuantity;
        
        for (auto& StoragePair : ResourceStorages)
        {
            FResourceStorage& Storage = StoragePair.Value;
            if (FResourceData* Resource = Storage.StoredResources.Find(InputPair.Key))
            {
                float ConsumedAmount = FMath::Min(RemainingQuantity, Resource->Quantity);
                Resource->Quantity -= ConsumedAmount;
                RemainingQuantity -= ConsumedAmount;
                
                if (Resource->Quantity <= 0.0f)
                {
                    Storage.StoredResources.Remove(InputPair.Key);
                }
                
                if (RemainingQuantity <= 0.0f)
                {
                    break;
                }
            }
        }
    }

    // 產生輸出資源
    for (const auto& OutputPair : Conversion.OutputResources)
    {
        float OutputQuantity = OutputPair.Value * Conversion.Efficiency;
        
        // 找到合適的存儲
        FString TargetStorageID = FindSuitableStorage(OutputPair.Key);
        if (!TargetStorageID.IsEmpty())
        {
            StoreResource(TargetStorageID, OutputPair.Key, OutputQuantity);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("無法找到合適的存儲：%s"), *GetResourceTypeName(OutputPair.Key));
        }
    }

    // 廣播轉換事件
    OnResourceConverted.Broadcast(ConversionID, true);
    
    // 添加歷史記錄
    AddResourceHistory(FString::Printf(TEXT("完成資源轉換：%s"), *ConversionID));
    
    UE_LOG(LogTemp, Log, TEXT("完成資源轉換：%s"), *ConversionID);
    return true;
}

FString UMingGoRTSResourceSystem::CreateResourceConversion(const FString& ConversionName, const TMap<EResourceType, float>& Input, const TMap<EResourceType, float>& Output)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("資源系統未初始化"));
        return TEXT("");
    }

    FString ConversionID = GenerateConversionID(ConversionName);
    
    FResourceConversion NewConversion;
    NewConversion.ConversionID = ConversionID;
    NewConversion.ConversionName = ConversionName;
    NewConversion.InputResources = Input;
    NewConversion.OutputResources = Output;
    NewConversion.ConversionTime = 1.0f;
    NewConversion.Efficiency = 1.0f;
    NewConversion.bIsAvailable = true;
    NewConversion.Description = FString::Printf(TEXT("轉換：%s"), *ConversionName);
    
    ResourceConversions.Add(ConversionID, NewConversion);
    
    UE_LOG(LogTemp, Log, TEXT("創建資源轉換：%s"), *ConversionID);
    return ConversionID;
}

float UMingGoRTSResourceSystem::CalculateCollectionEfficiency(const FString& CollectionID) const
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源收集：%s"), *CollectionID);
        return 0.0f;
    }

    const FResourceCollection& Collection = ResourceCollections[CollectionID];
    
    float BaseEfficiency = 1.0f;
    
    // 收集方法加成
    BaseEfficiency *= GetCollectionMethodBonus(Collection.CollectionMethod);
    
    // 環境因素影響
    float EnvironmentalImpact = CalculateEnvironmentalImpact(CollectionID);
    BaseEfficiency *= EnvironmentalImpact;
    
    return FMath::Clamp(BaseEfficiency, 0.1f, 2.0f);
}

void UMingGoRTSResourceSystem::UpdateCollectionEfficiency(const FString& CollectionID, float NewEfficiency)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源收集：%s"), *CollectionID);
        return;
    }

    FResourceCollection& Collection = ResourceCollections[CollectionID];
    Collection.Efficiency = FMath::Clamp(NewEfficiency, 0.1f, 2.0f);
    
    // 更新產量
    UpdateCollectionYield(CollectionID);
    
    UE_LOG(LogTemp, Log, TEXT("更新收集效率：%s，新效率：%.2f"), *CollectionID, NewEfficiency);
}

FResourceCollection UMingGoRTSResourceSystem::GetCollectionStatus(const FString& CollectionID) const
{
    if (const FResourceCollection* Collection = ResourceCollections.Find(CollectionID))
    {
        return *Collection;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到資源收集：%s"), *CollectionID);
    return FResourceCollection();
}

TArray<FResourceCollection> UMingGoRTSResourceSystem::GetActiveCollections() const
{
    TArray<FResourceCollection> ActiveCollections;
    
    for (const auto& CollectionPair : ResourceCollections)
    {
        const FResourceCollection& Collection = CollectionPair.Value;
        if (Collection.bIsActive)
        {
            ActiveCollections.Add(Collection);
        }
    }
    
    return ActiveCollections;
}

bool UMingGoRTSResourceSystem::ImproveResourceQuality(const FString& StorageID, EResourceType ResourceType, EResourceQuality NewQuality)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s"), *StorageID);
        return false;
    }

    FResourceStorage& Storage = ResourceStorages[StorageID];
    
    FResourceData* Resource = Storage.StoredResources.Find(ResourceType);
    if (!Resource)
    {
        UE_LOG(LogTemp, Warning, TEXT("存儲中沒有資源：%s"), *GetResourceTypeName(ResourceType));
        return false;
    }
    
    // 檢查質量是否可以提升
    if (Resource->Quality >= NewQuality)
    {
        UE_LOG(LogTemp, Warning, TEXT("資源質量已經達到或超過目標質量"));
        return false;
    }
    
    // 計算質量提升成本
    float ImprovementCost = CalculateQualityImprovementCost(Resource->Quality, NewQuality);
    
    // 應用質量提升
    Resource->Quality = NewQuality;
    Resource->UnitValue *= GetQualityBonus(NewQuality);
    
    // 添加歷史記錄
    AddResourceHistory(FString::Printf(TEXT("提升資源質量：%s，從 %s 到 %s"), *GetResourceTypeName(ResourceType), *StaticEnum<EResourceQuality>()->GetValueAsString(Resource->Quality), *StaticEnum<EResourceQuality>()->GetValueAsString(NewQuality)));
    
    UE_LOG(LogTemp, Log, TEXT("提升資源質量：%s，新質量：%s"), *GetResourceTypeName(ResourceType), *StaticEnum<EResourceQuality>()->GetValueAsString(NewQuality));
    return true;
}

bool UMingGoRTSResourceSystem::TransportResource(const FString& FromStorageID, const FString& ToStorageID, EResourceType ResourceType, float Quantity)
{
    if (!ResourceStorages.Contains(FromStorageID) || !ResourceStorages.Contains(ToStorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到源或目標存儲"));
        return false;
    }

    // 檢查目標存儲容量
    if (!CheckStorageLimits(ToStorageID, ResourceType, Quantity))
    {
        UE_LOG(LogTemp, Warning, TEXT("目標存儲容量不足"));
        return false;
    }

    // 從源存儲提取資源
    float RetrievedAmount = RetrieveResource(FromStorageID, ResourceType, Quantity);
    
    if (RetrievedAmount <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("源存儲中沒有足夠的資源"));
        return false;
    }
    
    // 存儲到目標存儲
    bool bStored = StoreResource(ToStorageID, ResourceType, RetrievedAmount);
    
    if (bStored)
    {
        // 添加歷史記錄
        AddResourceHistory(FString::Printf(TEXT("運輸資源：%s，數量：%.2f，從 %s 到 %s"), *GetResourceTypeName(ResourceType), RetrievedAmount, *FromStorageID, *ToStorageID));
        
        UE_LOG(LogTemp, Log, TEXT("運輸資源：%s，數量：%.2f，從 %s 到 %s"), *GetResourceTypeName(ResourceType), RetrievedAmount, *FromStorageID, *ToStorageID);
        return true;
    }
    
    // 如果存儲失敗，嘗試還原到源存儲
    StoreResource(FromStorageID, ResourceType, RetrievedAmount);
    
    UE_LOG(LogTemp, Error, TEXT("資源運輸失敗，已還原到源存儲"));
    return false;
}

bool UMingGoRTSResourceSystem::TradeResource(const FString& StorageID, EResourceType GiveResource, float GiveQuantity, EResourceType GetResource, float GetQuantity)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s"), *StorageID);
        return false;
    }

    // 檢查是否有足夠的資源進行交易
    if (!CheckResourceAvailability(StorageID, GiveResource, GiveQuantity))
    {
        UE_LOG(LogTemp, Warning, TEXT("沒有足夠的資源進行交易"));
        return false;
    }

    // 檢查存儲容量
    if (!CheckStorageLimits(StorageID, GetResource, GetQuantity))
    {
        UE_LOG(LogTemp, Warning, TEXT("存儲容量不足以接收交易資源"));
        return false;
    }

    // 計算交易價值
    float GiveValue = GetResourceInfo(GiveResource).UnitValue * GiveQuantity;
    float GetValue = GetResourceInfo(GetResource).UnitValue * GetQuantity;
    
    // 檢查交易是否公平（可調整這個閾值）
    if (FMath::Abs(GiveValue - GetValue) > GiveValue * 0.2f)
    {
        UE_LOG(LogTemp, Warning, TEXT("交易價值不平衡"));
        return false;
    }

    // 執行交易
    float RetrievedAmount = RetrieveResource(StorageID, GiveResource, GiveQuantity);
    
    if (RetrievedAmount > 0.0f)
    {
        bool bStored = StoreResource(StorageID, GetResource, GetQuantity);
        
        if (bStored)
        {
            // 添加歷史記錄
            AddResourceHistory(FString::Printf(TEXT("交易資源：給出 %s %.2f，獲得 %s %.2f"), *GetResourceTypeName(GiveResource), RetrievedAmount, *GetResourceTypeName(GetResource), GetQuantity));
            
            UE_LOG(LogTemp, Log, TEXT("交易完成：給出 %s %.2f，獲得 %s %.2f"), *GetResourceTypeName(GiveResource), RetrievedAmount, *GetResourceTypeName(GetResource), GetQuantity);
            return true;
        }
        
        // 交易失敗，還原資源
        StoreResource(StorageID, GiveResource, RetrievedAmount);
    }
    
    UE_LOG(LogTemp, Error, TEXT("交易失敗"));
    return false;
}

bool UMingGoRTSResourceSystem::AllocateResource(const FString& StorageID, EResourceType ResourceType, float Quantity, const FString& Purpose)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        UE_LOG(LogTemp, Error, TEXT("未找到資源存儲：%s"), *StorageID);
        return false;
    }

    // 檢查資源可用性
    if (!CheckResourceAvailability(StorageID, ResourceType, Quantity))
    {
        UE_LOG(LogTemp, Warning, TEXT("沒有足夠的資源進行分配"));
        return false;
    }

    // 標記資源為已分配（這裡可以實現更複雜的分配邏輯）
    float RetrievedAmount = RetrieveResource(StorageID, ResourceType, Quantity);
    
    if (RetrievedAmount > 0.0f)
    {
        // 添加歷史記錄
        AddResourceHistory(FString::Printf(TEXT("分配資源：%s %.2f，用途：%s"), *GetResourceTypeName(ResourceType), RetrievedAmount, *Purpose));
        
        UE_LOG(LogTemp, Log, TEXT("分配資源：%s %.2f，用途：%s"), *GetResourceTypeName(ResourceType), RetrievedAmount, *Purpose);
        return true;
    }
    
    UE_LOG(LogTemp, Error, TEXT("資源分配失敗"));
    return false;
}

bool UMingGoRTSResourceSystem::CheckResourceAvailability(const FString& StorageID, EResourceType ResourceType, float RequiredQuantity) const
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return false;
    }

    const FResourceStorage& Storage = ResourceStorages[StorageID];
    
    if (const FResourceData* Resource = Storage.StoredResources.Find(ResourceType))
    {
        return Resource->Quantity >= RequiredQuantity;
    }
    
    return false;
}

float UMingGoRTSResourceSystem::GetTotalResourceQuantity(EResourceType ResourceType) const
{
    float TotalQuantity = 0.0f;
    
    for (const auto& StoragePair : ResourceStorages)
    {
        const FResourceStorage& Storage = StoragePair.Value;
        if (const FResourceData* Resource = Storage.StoredResources.Find(ResourceType))
        {
            TotalQuantity += Resource->Quantity;
        }
    }
    
    return TotalQuantity;
}

void UMingGoRTSResourceSystem::ProcessResourceDecay(float DeltaTime)
{
    for (auto& StoragePair : ResourceStorages)
    {
        FResourceStorage& Storage = StoragePair.Value;
        
        for (auto& ResourcePair : Storage.StoredResources)
        {
            FResourceData& Resource = ResourcePair.Value;
            
            if (Resource.bIsPerishable && Resource.PerishRate > 0.0f)
            {
                float DecayAmount = Resource.Quantity * Resource.PerishRate * DeltaTime;
                Resource.Quantity -= DecayAmount;
                
                if (Resource.Quantity <= 0.0f)
                {
                    Resource.Quantity = 0.0f;
                }
                
                // 檢查是否過期
                if (!Resource.ExpiryDate.IsNull() && FDateTime::Now() > Resource.ExpiryDate)
                {
                    Resource.Quantity = 0.0f;
                }
            }
        }
        
        // 移除數量為0的資源
        for (auto It = Storage.StoredResources.CreateIterator(); It; ++It)
        {
            if (It->Value.Quantity <= 0.0f)
            {
                It.RemoveCurrent();
            }
        }
        
        UpdateStorageCapacity(Storage.StorageID);
    }
}

bool UMingGoRTSResourceSystem::SaveResourceData(const FString& SaveSlotName)
{
    // TODO: 實現資源數據保存
    UE_LOG(LogTemp, Log, TEXT("保存資源數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSResourceSystem::LoadResourceData(const FString& SaveSlotName)
{
    // TODO: 實現資源數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入資源數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSResourceSystem::InitializeResourceInfo()
{
    // 初始化所有資源類型
    CreateWoodResource();
    CreateStoneResource();
    CreateMetalResource();
    CreateFoodResource();
    CreateWaterResource();
    CreateCoalResource();
    CreateIronResource();
    CreateCopperResource();
    CreateGoldResource();
    CreateSilverResource();
    CreateCottonResource();
    CreateSilkResource();
    CreateTeaResource();
    CreateOpiumResource();
    CreateMedicineResource();
    CreateWeaponsResource();
    CreateAmmunitionResource();
    CreateUniformsResource();
    CreateHorsesResource();
    CreateVehiclesResource();
    CreateFuelResource();
    CreateRubberResource();
    CreatePaperResource();
    CreateInkResource();
    CreateBooksResource();
    CreateToolsResource();
    CreateMachineryResource();
    CreateElectronicsResource();
    CreateRadioResource();
    CreateTelephoneResource();
    CreateTelegraphResource();
}

void UMingGoRTSResourceSystem::InitializeHistoricalResources()
{
    // 初始化歷史時期特有的資源設置
    // 民國時期的資源特性和價值
    AdjustHistoricalResourceValues();
}

void UMingGoRTSResourceSystem::CreateWoodResource()
{
    FResourceData WoodResource;
    WoodResource.ResourceType = EResourceType::Wood;
    WoodResource.Rarity = EResourceRarity::Common;
    WoodResource.Quality = EResourceQuality::Normal;
    WoodResource.Quantity = 0.0f;
    WoodResource.MaxQuantity = 1000.0f;
    WoodResource.UnitWeight = 1.0f;
    WoodResource.UnitValue = 1.0f;
    WoodResource.Description = TEXT("基礎建築材料，用於建造房屋和設施");
    WoodResource.IconPath = TEXT("/Game/Icons/Resources/Wood");
    WoodResource.bIsStackable = true;
    WoodResource.bIsPerishable = false;
    WoodResource.Tags.Add(TEXT("Building"));
    WoodResource.Tags.Add(TEXT("Basic"));
    
    ResourceInfoMap.Add(EResourceType::Wood, WoodResource);
}

void UMingGoRTSResourceSystem::CreateStoneResource()
{
    FResourceData StoneResource;
    StoneResource.ResourceType = EResourceType::Stone;
    StoneResource.Rarity = EResourceRarity::Common;
    StoneResource.Quality = EResourceQuality::Normal;
    StoneResource.Quantity = 0.0f;
    StoneResource.MaxQuantity = 1000.0f;
    StoneResource.UnitWeight = 2.0f;
    StoneResource.UnitValue = 1.5f;
    StoneResource.Description = TEXT("堅固的建築材料，用於建造防禦設施");
    StoneResource.IconPath = TEXT("/Game/Icons/Resources/Stone");
    StoneResource.bIsStackable = true;
    StoneResource.bIsPerishable = false;
    StoneResource.Tags.Add(TEXT("Building"));
    StoneResource.Tags.Add(TEXT("Defense"));
    
    ResourceInfoMap.Add(EResourceType::Stone, StoneResource);
}

void UMingGoRTSResourceSystem::CreateMetalResource()
{
    FResourceData MetalResource;
    MetalResource.ResourceType = EResourceType::Metal;
    MetalResource.Rarity = EResourceRarity::Uncommon;
    MetalResource.Quality = EResourceQuality::Normal;
    MetalResource.Quantity = 0.0f;
    MetalResource.MaxQuantity = 500.0f;
    MetalResource.UnitWeight = 3.0f;
    MetalResource.UnitValue = 3.0f;
    MetalResource.Description = TEXT("重要的工業材料，用於製造武器和設備");
    MetalResource.IconPath = TEXT("/Game/Icons/Resources/Metal");
    MetalResource.bIsStackable = true;
    MetalResource.bIsPerishable = false;
    MetalResource.Tags.Add(TEXT("Industrial"));
    MetalResource.Tags.Add(TEXT("Military"));
    
    ResourceInfoMap.Add(EResourceType::Metal, MetalResource);
}

void UMingGoRTSResourceSystem::CreateFoodResource()
{
    FResourceData FoodResource;
    FoodResource.ResourceType = EResourceType::Food;
    FoodResource.Rarity = EResourceRarity::Common;
    FoodResource.Quality = EResourceQuality::Normal;
    FoodResource.Quantity = 0.0f;
    FoodResource.MaxQuantity = 2000.0f;
    FoodResource.UnitWeight = 0.5f;
    FoodResource.UnitValue = 2.0f;
    FoodResource.Description = TEXT("維持部隊和人口生存的基本需求");
    FoodResource.IconPath = TEXT("/Game/Icons/Resources/Food");
    FoodResource.bIsStackable = true;
    FoodResource.bIsPerishable = true;
    FoodResource.PerishRate = 0.01f;
    FoodResource.Tags.Add(TEXT("Consumable"));
    FoodResource.Tags.Add(TEXT("Essential"));
    
    ResourceInfoMap.Add(EResourceType::Food, FoodResource);
}

void UMingGoRTSResourceSystem::CreateWaterResource()
{
    FResourceData WaterResource;
    WaterResource.ResourceType = EResourceType::Water;
    WaterResource.Rarity = EResourceRarity::Common;
    WaterResource.Quality = EResourceQuality::Normal;
    WaterResource.Quantity = 0.0f;
    WaterResource.MaxQuantity = 5000.0f;
    WaterResource.UnitWeight = 1.0f;
    WaterResource.UnitValue = 0.5f;
    WaterResource.Description = TEXT("生活和生產必需的資源");
    WaterResource.IconPath = TEXT("/Game/Icons/Resources/Water");
    WaterResource.bIsStackable = true;
    WaterResource.bIsPerishable = false;
    WaterResource.Tags.Add(TEXT("Consumable"));
    WaterResource.Tags.Add(TEXT("Essential"));
    
    ResourceInfoMap.Add(EResourceType::Water, WaterResource);
}

void UMingGoRTSResourceSystem::CreateCoalResource()
{
    FResourceData CoalResource;
    CoalResource.ResourceType = EResourceType::Coal;
    CoalResource.Rarity = EResourceRarity::Uncommon;
    CoalResource.Quality = EResourceQuality::Normal;
    CoalResource.Quantity = 0.0f;
    CoalResource.MaxQuantity = 1000.0f;
    CoalResource.UnitWeight = 1.5f;
    CoalResource.UnitValue = 2.5f;
    CoalResource.Description = TEXT("重要的能源材料，用於發電和供暖");
    CoalResource.IconPath = TEXT("/Game/Icons/Resources/Coal");
    CoalResource.bIsStackable = true;
    CoalResource.bIsPerishable = false;
    CoalResource.Tags.Add(TEXT("Energy"));
    CoalResource.Tags.Add(TEXT("Industrial"));
    
    ResourceInfoMap.Add(EResourceType::Coal, CoalResource);
}

void UMingGoRTSResourceSystem::CreateIronResource()
{
    FResourceData IronResource;
    IronResource.ResourceType = EResourceType::Iron;
    IronResource.Rarity = EResourceRarity::Uncommon;
    IronResource.Quality = EResourceQuality::Normal;
    IronResource.Quantity = 0.0f;
    IronResource.MaxQuantity = 800.0f;
    IronResource.UnitWeight = 4.0f;
    IronResource.UnitValue = 4.0f;
    IronResource.Description = TEXT("製造武器和工具的重要材料");
    IronResource.IconPath = TEXT("/Game/Icons/Resources/Iron");
    IronResource.bIsStackable = true;
    IronResource.bIsPerishable = false;
    IronResource.Tags.Add(TEXT("Industrial"));
    IronResource.Tags.Add(TEXT("Military"));
    
    ResourceInfoMap.Add(EResourceType::Iron, IronResource);
}

void UMingGoRTSResourceSystem::CreateCopperResource()
{
    FResourceData CopperResource;
    CopperResource.ResourceType = EResourceType::Copper;
    CopperResource.Rarity = EResourceRarity::Uncommon;
    CopperResource.Quality = EResourceQuality::Normal;
    CopperResource.Quantity = 0.0f;
    CopperResource.MaxQuantity = 600.0f;
    CopperResource.UnitWeight = 3.5f;
    CopperResource.UnitValue = 3.5f;
    CopperResource.Description = TEXT("製造電線和通訊設備的材料");
    CopperResource.IconPath = TEXT("/Game/Icons/Resources/Copper");
    CopperResource.bIsStackable = true;
    CopperResource.bIsPerishable = false;
    CopperResource.Tags.Add(TEXT("Industrial"));
    CopperResource.Tags.Add(TEXT("Communication"));
    
    ResourceInfoMap.Add(EResourceType::Copper, CopperResource);
}

void UMingGoRTSResourceSystem::CreateGoldResource()
{
    FResourceData GoldResource;
    GoldResource.ResourceType = EResourceType::Gold;
    GoldResource.Rarity = EResourceRarity::Rare;
    GoldResource.Quality = EResourceQuality::Good;
    GoldResource.Quantity = 0.0f;
    GoldResource.MaxQuantity = 100.0f;
    GoldResource.UnitWeight = 2.0f;
    GoldResource.UnitValue = 50.0f;
    GoldResource.Description = TEXT("貴金屬，用於貨幣和裝飾");
    GoldResource.IconPath = TEXT("/Game/Icons/Resources/Gold");
    GoldResource.bIsStackable = true;
    GoldResource.bIsPerishable = false;
    GoldResource.Tags.Add(TEXT("Precious"));
    GoldResource.Tags.Add(TEXT("Currency"));
    
    ResourceInfoMap.Add(EResourceType::Gold, GoldResource);
}

void UMingGoRTSResourceSystem::CreateSilverResource()
{
    FResourceData SilverResource;
    SilverResource.ResourceType = EResourceType::Silver;
    SilverResource.Rarity = EResourceRarity::Rare;
    SilverResource.Quality = EResourceQuality::Good;
    SilverResource.Quantity = 0.0f;
    SilverResource.MaxQuantity = 200.0f;
    SilverResource.UnitWeight = 1.8f;
    SilverResource.UnitValue = 25.0f;
    SilverResource.Description = TEXT("貴金屬，用於貨幣和裝飾");
    SilverResource.IconPath = TEXT("/Game/Icons/Resources/Silver");
    SilverResource.bIsStackable = true;
    SilverResource.bIsPerishable = false;
    SilverResource.Tags.Add(TEXT("Precious"));
    SilverResource.Tags.Add(TEXT("Currency"));
    
    ResourceInfoMap.Add(EResourceType::Silver, SilverResource);
}

void UMingGoRTSResourceSystem::CreateCottonResource()
{
    FResourceData CottonResource;
    CottonResource.ResourceType = EResourceType::Cotton;
    CottonResource.Rarity = EResourceRarity::Common;
    CottonResource.Quality = EResourceQuality::Normal;
    CottonResource.Quantity = 0.0f;
    CottonResource.MaxQuantity = 1500.0f;
    CottonResource.UnitWeight = 0.3f;
    CottonResource.UnitValue = 1.5f;
    CottonResource.Description = TEXT("紡織原料，用於製作衣物");
    CottonResource.IconPath = TEXT("/Game/Icons/Resources/Cotton");
    CottonResource.bIsStackable = true;
    CottonResource.bIsPerishable = false;
    CottonResource.Tags.Add(TEXT("Textile"));
    CottonResource.Tags.Add(TEXT("RawMaterial"));
    
    ResourceInfoMap.Add(EResourceType::Cotton, CottonResource);
}

void UMingGoRTSResourceSystem::CreateSilkResource()
{
    FResourceData SilkResource;
    SilkResource.ResourceType = EResourceType::Silk;
    SilkResource.Rarity = EResourceRarity::Uncommon;
    SilkResource.Quality = EResourceQuality::Good;
    SilkResource.Quantity = 0.0f;
    SilkResource.MaxQuantity = 500.0f;
    SilkResource.UnitWeight = 0.2f;
    SilkResource.UnitValue = 5.0f;
    SilkResource.Description = TEXT("高級紡織原料，用於製作奢侈品");
    SilkResource.IconPath = TEXT("/Game/Icons/Resources/Silk");
    SilkResource.bIsStackable = true;
    SilkResource.bIsPerishable = false;
    SilkResource.Tags.Add(TEXT("Textile"));
    SilkResource.Tags.Add(TEXT("Luxury"));
    
    ResourceInfoMap.Add(EResourceType::Silk, SilkResource);
}

void UMingGoRTSResourceSystem::CreateTeaResource()
{
    FResourceData TeaResource;
    TeaResource.ResourceType = EResourceType::Tea;
    TeaResource.Rarity = EResourceRarity::Common;
    TeaResource.Quality = EResourceQuality::Good;
    TeaResource.Quantity = 0.0f;
    TeaResource.MaxQuantity = 1000.0f;
    TeaResource.UnitWeight = 0.1f;
    TeaResource.UnitValue = 3.0f;
    TeaResource.Description = TEXT("傳統飲品，具有文化和經濟價值");
    TeaResource.IconPath = TEXT("/Game/Icons/Resources/Tea");
    TeaResource.bIsStackable = true;
    TeaResource.bIsPerishable = true;
    TeaResource.PerishRate = 0.005f;
    TeaResource.Tags.Add(TEXT("Consumable"));
    TeaResource.Tags.Add(TEXT("Cultural"));
    
    ResourceInfoMap.Add(EResourceType::Tea, TeaResource);
}

void UMingGoRTSResourceSystem::CreateOpiumResource()
{
    FResourceData OpiumResource;
    OpiumResource.ResourceType = EResourceType::Opium;
    OpiumResource.Rarity = EResourceRarity::Rare;
    OpiumResource.Quality = EResourceQuality::Normal;
    OpiumResource.Quantity = 0.0f;
    OpiumResource.MaxCapacity = 100.0f;
    OpiumResource.UnitWeight = 0.2f;
    OpiumResource.UnitValue = 20.0f;
    OpiumResource.Description = TEXT("具有爭議性的藥用植物，具有經濟和社會影響");
    OpiumResource.IconPath = TEXT("/Game/Icons/Resources/Opium");
    OpiumResource.bIsStackable = true;
    OpiumResource.bIsPerishable = false;
    OpiumResource.Tags.Add(TEXT("Controversial"));
    OpiumResource.Tags.Add(TEXT("Medicinal"));
    
    ResourceInfoMap.Add(EResourceType::Opium, OpiumResource);
}

void UMingGoRTSResourceSystem::CreateMedicineResource()
{
    FResourceData MedicineResource;
    MedicineResource.ResourceType = EResourceType::Medicine;
    MedicineResource.Rarity = EResourceRarity::Uncommon;
    MedicineResource.Quality = EResourceQuality::Good;
    MedicineResource.Quantity = 0.0f;
    MedicineResource.MaxCapacity = 500.0f;
    MedicineResource.UnitWeight = 0.1f;
    MedicineResource.UnitValue = 8.0f;
    MedicineResource.Description = TEXT("治療疾病和傷口的藥品");
    MedicineResource.IconPath = TEXT("/Game/Icons/Resources/Medicine");
    MedicineResource.bIsStackable = true;
    MedicineResource.bIsPerishable = true;
    MedicineResource.PerishRate = 0.02f;
    MedicineResource.Tags.Add(TEXT("Medical"));
    MedicineResource.Tags.Add(TEXT("Essential"));
    
    ResourceInfoMap.Add(EResourceType::Medicine, MedicineResource);
}

void UMingGoRTSResourceSystem::CreateWeaponsResource()
{
    FResourceData WeaponsResource;
    WeaponsResource.ResourceType = EResourceType::Weapons;
    WeaponsResource.Rarity = EResourceRarity::Uncommon;
    WeaponsResource.Quality = EResourceQuality::Good;
    WeaponsResource.Quantity = 0.0f;
    WeaponsResource.MaxCapacity = 200.0f;
    WeaponsResource.UnitWeight = 5.0f;
    WeaponsResource.UnitValue = 15.0f;
    WeaponsResource.Description = TEXT("軍事武器，用於戰鬥");
    WeaponsResource.IconPath = TEXT("/Game/Icons/Resources/Weapons");
    WeaponsResource.bIsStackable = false;
    WeaponsResource.bIsPerishable = false;
    WeaponsResource.Tags.Add(TEXT("Military"));
    WeaponsResource.Tags.Add(TEXT("Equipment"));
    
    ResourceInfoMap.Add(EResourceType::Weapons, WeaponsResource);
}

void UMingGoRTSResourceSystem::CreateAmmunitionResource()
{
    FResourceData AmmunitionResource;
    AmmunitionResource.ResourceType = EResourceType::Ammunition;
    AmmunitionResource.Rarity = EResourceRarity::Common;
    AmmunitionResource.Quality = EResourceQuality::Normal;
    AmmunitionResource.Quantity = 0.0f;
    AmmunitionResource.MaxCapacity = 1000.0f;
    AmmunitionResource.UnitWeight = 0.5f;
    AmmunitionResource.UnitValue = 2.0f;
    AmmunitionResource.Description = TEXT("武器彈藥，消耗品");
    AmmunitionResource.IconPath = TEXT("/Game/Icons/Resources/Ammunition");
    AmmunitionResource.bIsStackable = true;
    AmmunitionResource.bIsPerishable = false;
    AmmunitionResource.Tags.Add(TEXT("Military"));
    AmmunitionResource.Tags.Add(TEXT("Consumable"));
    
    ResourceInfoMap.Add(EResourceType::Ammunition, AmmunitionResource);
}

void UMingGoRTSResourceSystem::CreateUniformsResource()
{
    FResourceData UniformsResource;
    UniformsResource.ResourceType = EResourceType::Uniforms;
    UniformsResource.Rarity = EResourceRarity::Common;
    UniformsResource.Quality = EResourceQuality::Normal;
    UniformsResource.Quantity = 0.0f;
    UniformsResource.MaxCapacity = 500.0f;
    UniformsResource.UnitWeight = 1.0f;
    UniformsResource.UnitValue = 5.0f;
    UniformsResource.Description = TEXT("軍隊制服，提供識別和保護");
    UniformsResource.IconPath = TEXT("/Game/Icons/Resources/Uniforms");
    UniformsResource.bIsStackable = false;
    UniformsResource.bIsPerishable = false;
    UniformsResource.Tags.Add(TEXT("Military"));
    UniformsResource.Tags.Add(TEXT("Equipment"));
    
    ResourceInfoMap.Add(EResourceType::Uniforms, UniformsResource);
}

void UMingGoRTSResourceSystem::CreateHorsesResource()
{
    FResourceData HorsesResource;
    HorsesResource.ResourceType = EResourceType::Horses;
    HorsesResource.Rarity = EResourceRarity::Uncommon;
    HorsesResource.Quality = EResourceQuality::Normal;
    HorsesResource.Quantity = 0.0f;
    HorsesResource.MaxCapacity = 100.0f;
    HorsesResource.UnitWeight = 100.0f;
    HorsesResource.UnitValue = 30.0f;
    HorsesResource.Description = TEXT("運輸和騎兵的重要資源");
    HorsesResource.IconPath = TEXT("/Game/Icons/Resources/Horses");
    HorsesResource.bIsStackable = false;
    HorsesResource.bIsPerishable = false;
    HorsesResource.Tags.Add(TEXT("Transportation"));
    HorsesResource.Tags.Add(TEXT("Military"));
    
    ResourceInfoMap.Add(EResourceType::Horses, HorsesResource);
}

void UMingGoRTSResourceSystem::CreateVehiclesResource()
{
    FResourceData VehiclesResource;
    VehiclesResource.ResourceType = EResourceType::Vehicles;
    VehiclesResource.Rarity = EResourceRarity::Rare;
    VehiclesResource.Quality = EResourceQuality::Good;
    VehiclesResource.Quantity = 0.0f;
    VehiclesResource.MaxCapacity = 50.0f;
    VehiclesResource.UnitWeight = 500.0f;
    VehiclesResource.UnitValue = 100.0f;
    VehiclesResource.Description = TEXT("機動車輛，提供快速運輸");
    VehiclesResource.IconPath = TEXT("/Game/Icons/Resources/Vehicles");
    VehiclesResource.bIsStackable = false;
    VehiclesResource.bIsPerishable = false;
    VehiclesResource.Tags.Add(TEXT("Transportation"));
    VehiclesResource.Tags.Add(TEXT("Modern"));
    
    ResourceInfoMap.Add(EResourceType::Vehicles, VehiclesResource);
}

void UMingGoRTSResourceSystem::CreateFuelResource()
{
    FResourceData FuelResource;
    FuelResource.ResourceType = EResourceType::Fuel;
    FuelResource.Rarity = EResourceRarity::Uncommon;
    FuelResource.Quality = EResourceQuality::Normal;
    FuelResource.Quantity = 0.0f;
    FuelResource.MaxCapacity = 1000.0f;
    FuelResource.UnitWeight = 0.8f;
    FuelResource.UnitValue = 4.0f;
    FuelResource.Description = TEXT("車輛和設備的能源");
    FuelResource.IconPath = TEXT("/Game/Icons/Resources/Fuel");
    FuelResource.bIsStackable = true;
    FuelResource.bIsPerishable = false;
    FuelResource.Tags.Add(TEXT("Energy"));
    FuelResource.Tags.Add(TEXT("Modern"));
    
    ResourceInfoMap.Add(EResourceType::Fuel, FuelResource);
}

void UMingGoRTSResourceSystem::CreateRubberResource()
{
    FResourceData RubberResource;
    RubberResource.ResourceType = EResourceType::Rubber;
    RubberResource.Rarity = EResourceRarity::Uncommon;
    RubberResource.Quality = EResourceQuality::Normal;
    RubberResource.Quantity = 0.0f;
    RubberResource.MaxCapacity = 800.0f;
    RubberResource.UnitWeight = 1.2f;
    RubberResource.UnitValue = 6.0f;
    RubberResource.Description = TEXT("製造輪胎和密封件的材料");
    RubberResource.IconPath = TEXT("/Game/Icons/Resources/Rubber");
    RubberResource.bIsStackable = true;
    RubberResource.bIsPerishable = false;
    RubberResource.Tags.Add(TEXT("Industrial"));
    RubberResource.Tags.Add(TEXT("Modern"));
    
    ResourceInfoMap.Add(EResourceType::Rubber, RubberResource);
}

void UMingGoRTSResourceSystem::CreatePaperResource()
{
    FResourceData PaperResource;
    PaperResource.ResourceType = EResourceType::Paper;
    PaperResource.Rarity = EResourceRarity::Common;
    PaperResource.Quality = EResourceQuality::Normal;
    PaperResource.Quantity = 0.0f;
    PaperResource.MaxCapacity = 2000.0f;
    PaperResource.UnitWeight = 0.1f;
    PaperResource.UnitValue = 1.0f;
    PaperResource.Description = TEXT("文書和印刷材料");
    PaperResource.IconPath = TEXT("/Game/Icons/Resources/Paper");
    PaperResource.bIsStackable = true;
    PaperResource.bIsPerishable = false;
    PaperResource.Tags.Add(TEXT("Stationery"));
    PaperResource.Tags.Add(TEXT("Communication"));
    
    ResourceInfoMap.Add(EResourceType::Paper, PaperResource);
}

void UMingGoRTSResourceSystem::CreateInkResource()
{
    FResourceData InkResource;
    InkResource.ResourceType = EResourceType::Ink;
    InkResource.Rarity = EResourceRarity::Common;
    InkResource.Quality = EResourceQuality::Normal;
    InkResource.Quantity = 0.0f;
    InkResource.MaxCapacity = 500.0f;
    InkResource.UnitWeight = 0.05f;
    InkResource.UnitValue = 2.0f;
    InkResource.Description = TEXT("書寫和印刷的墨水");
    InkResource.IconPath = TEXT("/Game/Icons/Resources/Ink");
    InkResource.bIsStackable = true;
    InkResource.bIsPerishable = false;
    InkResource.Tags.Add(TEXT("Stationery"));
    InkResource.Tags.Add(TEXT("Communication"));
    
    ResourceInfoMap.Add(EResourceType::Ink, InkResource);
}

void UMingGoRTSResourceSystem::CreateBooksResource()
{
    FResourceData BooksResource;
    BooksResource.ResourceType = EResourceType::Books;
    BooksResource.Rarity = EResourceRarity::Uncommon;
    BooksResource.Quality = EResourceQuality::Good;
    BooksResource.Quantity = 0.0f;
    BooksResource.MaxCapacity = 200.0f;
    BooksResource.UnitWeight = 0.5f;
    BooksResource.UnitValue = 10.0f;
    BooksResource.Description = TEXT("知識和文化的載體");
    BooksResource.IconPath = TEXT("/Game/Icons/Resources/Books");
    BooksResource.bIsStackable = false;
    BooksResource.bIsPerishable = false;
    BooksResource.Tags.Add(TEXT("Knowledge"));
    BooksResource.Tags.Add(TEXT("Cultural"));
    
    ResourceInfoMap.Add(EResourceType::Books, BooksResource);
}

void UMingGoRTSResourceSystem::CreateToolsResource()
{
    FResourceData ToolsResource;
    ToolsResource.ResourceType = EResourceType::Tools;
    ToolsResource.Rarity = EResourceRarity::Common;
    ToolsResource.Quality = EResourceQuality::Normal;
    ToolsResource.Quantity = 0.0f;
    ToolsResource.MaxCapacity = 300.0f;
    ToolsResource.UnitWeight = 2.0f;
    ToolsResource.UnitValue = 3.0f;
    ToolsResource.Description = TEXT("生產和建設的基本工具");
    ToolsResource.IconPath = TEXT("/Game/Icons/Resources/Tools");
    ToolsResource.bIsStackable = false;
    ToolsResource.bIsPerishable = false;
    ToolsResource.Tags.Add(TEXT("Equipment"));
    ToolsResource.Tags.Add(TEXT("Productive"));
    
    ResourceInfoMap.Add(EResourceType::Tools, ToolsResource);
}

void UMingGoRTSResourceSystem::CreateMachineryResource()
{
    FResourceData MachineryResource;
    MachineryResource.ResourceType = EResourceType::Machinery;
    MachineryResource.Rarity = EResourceRarity::Rare;
    MachineryResource.Quality = EResourceQuality::Good;
    MachineryResource.Quantity = 0.0f;
    MachineryResource.MaxCapacity = 100.0f;
    MachineryResource.UnitWeight = 50.0f;
    MachineryResource.UnitValue = 50.0f;
    MachineryResource.Description = TEXT("現代化生產設備");
    MachineryResource.IconPath = TEXT("/Game/Icons/Resources/Machinery");
    MachineryResource.bIsStackable = false;
    MachineryResource.bIsPerishable = false;
    MachineryResource.Tags.Add(TEXT("Industrial"));
    MachineryResource.Tags.Add(TEXT("Modern"));
    
    ResourceInfoMap.Add(EResourceType::Machinery, MachineryResource);
}

void UMingGoRTSResourceSystem::CreateElectronicsResource()
{
    FResourceData ElectronicsResource;
    ElectronicsResource.ResourceType = EResourceType::Electronics;
    ElectronicsResource.Rarity = EResourceRarity::Rare;
    ElectronicsResource.Quality = EResourceQuality::Excellent;
    ElectronicsResource.Quantity = 0.0f;
    ElectronicsResource.MaxCapacity = 50.0f;
    ElectronicsResource.UnitWeight = 5.0f;
    ElectronicsResource.UnitValue = 80.0f;
    ElectronicsResource.Description = TEXT("現代電子設備");
    ElectronicsResource.IconPath = TEXT("/Game/Icons/Resources/Electronics");
    ElectronicsResource.bIsStackable = false;
    ElectronicsResource.bIsPerishable = false;
    ElectronicsResource.Tags.Add(TEXT("Modern"));
    ElectronicsResource.Tags.Add(TEXT("Advanced"));
    
    ResourceInfoMap.Add(EResourceType::Electronics, ElectronicsResource);
}

void UMingGoRTSResourceSystem::CreateRadioResource()
{
    FResourceData RadioResource;
    RadioResource.ResourceType = EResourceType::Radio;
    RadioResource.Rarity = EResourceRarity::Rare;
    RadioResource.Quality = EResourceQuality::Good;
    RadioResource.Quantity = 0.0f;
    RadioResource.MaxCapacity = 30.0f;
    RadioResource.UnitWeight = 10.0f;
    RadioResource.UnitValue = 60.0f;
    RadioResource.Description = TEXT("無線電通訊設備");
    RadioResource.IconPath = TEXT("/Game/Icons/Resources/Radio");
    RadioResource.bIsStackable = false;
    RadioResource.bIsPerishable = false;
    RadioResource.Tags.Add(TEXT("Communication"));
    RadioResource.Tags.Add(TEXT("Modern"));
    
    ResourceInfoMap.Add(EResourceType::Radio, RadioResource);
}

void UMingGoRTSResourceSystem::CreateTelephoneResource()
{
    FResourceData TelephoneResource;
    TelephoneResource.ResourceType = EResourceType::Telephone;
    TelephoneResource.Rarity = EResourceRarity::Rare;
    TelephoneResource.Quality = EResourceQuality::Good;
    TelephoneResource.Quantity = 0.0f;
    TelephoneResource.MaxCapacity = 50.0f;
    TelephoneResource.UnitWeight = 3.0f;
    TelephoneResource.UnitValue = 40.0f;
    TelephoneResource.Description = TEXT("有線電話通訊設備");
    TelephoneResource.IconPath = TEXT("/Game/Icons/Resources/Telephone");
    TelephoneResource.bIsStackable = false;
    TelephoneResource.bIsPerishable = false;
    TelephoneResource.Tags.Add(TEXT("Communication"));
    TelephoneResource.Tags.Add(TEXT("Modern"));
    
    ResourceInfoMap.Add(EResourceType::Telephone, TelephoneResource);
}

void UMingGoRTSResourceSystem::CreateTelegraphResource()
{
    FResourceData TelegraphResource;
    TelegraphResource.ResourceType = EResourceType::Telegraph;
    TelegraphResource.Rarity = EResourceRarity::Uncommon;
    TelegraphResource.Quality = EResourceQuality::Normal;
    TelegraphResource.Quantity = 0.0f;
    TelegraphResource.MaxCapacity = 80.0f;
    TelegraphResource.UnitWeight = 15.0f;
    TelegraphResource.UnitValue = 25.0f;
    TelegraphResource.Description = TEXT("電報通訊設備");
    TelegraphResource.IconPath = TEXT("/Game/Icons/Resources/Telegraph");
    TelegraphResource.bIsStackable = false;
    TelegraphResource.bIsPerishable = false;
    TelegraphResource.Tags.Add(TEXT("Communication"));
    TelegraphResource.Tags.Add(TEXT("Modern"));
    
    ResourceInfoMap.Add(EResourceType::Telegraph, TelegraphResource);
}

// 輔助方法實現
void UMingGoRTSResourceSystem::UpdateCollectionYield(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return;
    }

    FResourceCollection& Collection = ResourceCollections[CollectionID];
    Collection.CurrentYield = Collection.BaseYield * Collection.Efficiency;
}

float UMingGoRTSResourceSystem::CalculateEnvironmentalImpact(const FString& CollectionID) const
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return 1.0f;
    }

    const FResourceCollection& Collection = ResourceCollections[CollectionID];
    float EnvironmentalImpact = 1.0f;
    
    // 計算環境因素影響
    for (const auto& FactorPair : Collection.EnvironmentalFactors)
    {
        EnvironmentalImpact *= FactorPair.Value;
    }
    
    return FMath::Clamp(EnvironmentalImpact, 0.5f, 1.5f);
}

bool UMingGoRTSResourceSystem::CheckCollectionConditions(const FString& CollectionID) const
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return false;
    }

    const FResourceCollection& Collection = ResourceCollections[CollectionID];
    
    // 檢查收集者是否有必要的工具
    // 這裡可以添加更複雜的條件檢查
    return true;
}

void UMingGoRTSResourceSystem::UpdateStorageCapacity(const FString& StorageID)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return;
    }

    FResourceStorage& Storage = ResourceStorages[StorageID];
    Storage.UsedCapacity = 0.0f;
    
    // 計算已使用容量
    for (const auto& ResourcePair : Storage.StoredResources)
    {
        const FResourceData& Resource = ResourcePair.Value;
        Storage.UsedCapacity += Resource.Quantity * Resource.UnitWeight;
    }
    
    Storage.CurrentCapacity = Storage.UsedCapacity / Storage.MaxCapacity;
}

bool UMingGoRTSResourceSystem::CheckStorageLimits(const FString& StorageID, EResourceType ResourceType, float Quantity) const
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return false;
    }

    const FResourceStorage& Storage = ResourceStorages[StorageID];
    
    // 檢查是否允許此資源類型
    if (Storage.AllowedTypes.Num() > 0 && !Storage.AllowedTypes.Contains(ResourceType))
    {
        return false;
    }
    
    // 檢查容量限制
    FResourceData ResourceInfo = GetResourceInfo(ResourceType);
    float RequiredCapacity = Quantity * ResourceInfo.UnitWeight;
    
    return (Storage.UsedCapacity + RequiredCapacity) <= Storage.MaxCapacity;
}

void UMingGoRTSResourceSystem::HandleStorageFull(const FString& StorageID)
{
    // 廣播存儲滿了事件
    OnStorageFull.Broadcast(StorageID, GetStorageCapacity(StorageID));
    
    // 添加歷史記錄
    AddResourceHistory(FString::Printf(TEXT("存儲已滿：%s"), *StorageID));
}

float UMingGoRTSResourceSystem::CalculateConversionCost(const FString& ConversionID) const
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        return 0.0f;
    }

    const FResourceConversion& Conversion = ResourceConversions[ConversionID];
    float TotalCost = 0.0f;
    
    // 計算輸入資源的總價值
    for (const auto& InputPair : Conversion.InputResources)
    {
        FResourceData ResourceInfo = GetResourceInfo(InputPair.Key);
        TotalCost += ResourceInfo.UnitValue * InputPair.Value;
    }
    
    return TotalCost;
}

bool UMingGoRTSResourceSystem::CheckConversionConditions(const FString& ConversionID) const
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        return false;
    }

    const FResourceConversion& Conversion = ResourceConversions[ConversionID];
    
    // 檢查轉換是否可用
    if (!Conversion.bIsAvailable)
    {
        return false;
    }
    
    // 檢查是否有必要的建築和技能
    // 這裡可以添加更複雜的條件檢查
    
    return true;
}

void UMingGoRTSResourceSystem::ProcessConversionComplete(const FString& ConversionID)
{
    // 添加歷史記錄
    AddResourceHistory(FString::Printf(TEXT("轉換完成：%s"), *ConversionID));
}

FString UMingGoRTSResourceSystem::GenerateCollectionID(EResourceType ResourceType) const
{
    return FString::Printf(TEXT("Collection_%s_%s"), *GetResourceTypeName(ResourceType), *FDateTime::Now().ToString());
}

FString UMingGoRTSResourceSystem::GenerateStorageID(const FString& StorageName) const
{
    return FString::Printf(TEXT("Storage_%s_%s"), *StorageName, *FDateTime::Now().ToString());
}

FString UMingGoRTSResourceSystem::GenerateConversionID(const FString& ConversionName) const
{
    return FString::Printf(TEXT("Conversion_%s_%s"), *ConversionName, *FDateTime::Now().ToString());
}

bool UMingGoRTSResourceSystem::ValidateResourceData(const FResourceData& ResourceData) const
{
    return ResourceData.Quantity >= 0.0f && ResourceData.UnitWeight > 0.0f && ResourceData.UnitValue > 0.0f;
}

FString UMingGoRTSResourceSystem::GetResourceTypeName(EResourceType ResourceType) const
{
    return StaticEnum<EResourceType>()->GetValueAsString(ResourceType);
}

float UMingGoRTSResourceSystem::GetResourceTypeWeight(EResourceType ResourceType) const
{
    FResourceData ResourceInfo = GetResourceInfo(ResourceType);
    return ResourceInfo.UnitWeight;
}

float UMingGoRTSResourceSystem::GetCollectionMethodBonus(ECollectionMethod Method) const
{
    switch (Method)
    {
    case ECollectionMethod::Manual:
        return 1.0f;
    case ECollectionMethod::Automatic:
        return 1.2f;
    case ECollectionMethod::Production:
        return 1.5f;
    case ECollectionMethod::Trade:
        return 1.1f;
    case ECollectionMethod::Plunder:
        return 0.8f;
    case ECollectionMethod::Harvest:
        return 1.3f;
    case ECollectionMethod::Mining:
        return 1.2f;
    case ECollectionMethod::Fishing:
        return 1.1f;
    case ECollectionMethod::Hunting:
        return 1.0f;
    case ECollectionMethod::Foraging:
        return 0.9f;
    default:
        return 1.0f;
    }
}

float UMingGoRTSResourceSystem::GetQualityBonus(EResourceQuality Quality) const
{
    switch (Quality)
    {
    case EResourceQuality::Poor:
        return 0.5f;
    case EResourceQuality::Normal:
        return 1.0f;
    case EResourceQuality::Good:
        return 1.5f;
    case EResourceQuality::Excellent:
        return 2.0f;
    case EResourceQuality::Perfect:
        return 3.0f;
    default:
        return 1.0f;
    }
}

float UMingGoRTSResourceSystem::CalculateResourceTotalValue(const FResourceData& ResourceData) const
{
    return ResourceData.Quantity * ResourceData.UnitValue * GetQualityBonus(ResourceData.Quality);
}

bool UMingGoRTSResourceSystem::CheckResourceCompatibility(EResourceType ResourceType1, EResourceType ResourceType2) const
{
    // 簡化實現：所有資源都兼容
    return true;
}

ECollectionMethod UMingGoRTSResourceSystem::GetRecommendedCollectionMethod(EResourceType ResourceType) const
{
    // 根據資源類型推薦收集方法
    switch (ResourceType)
    {
    case EResourceType::Wood:
    case EResourceType::Stone:
        return ECollectionMethod::Harvest;
    case EResourceType::Iron:
    case EResourceType::Copper:
    case EResourceType::Coal:
        return ECollectionMethod::Mining;
    case EResourceType::Food:
        return ECollectionMethod::Hunting;
    case EResourceType::Water:
        return ECollectionMethod::Harvest;
    case EResourceType::Cotton:
    case EResourceType::Tea:
        return ECollectionMethod::Harvest;
    case EResourceType::Silk:
        return ECollectionMethod::Harvest;
    case EResourceType::Opium:
        return ECollectionMethod::Harvest;
    default:
        return ECollectionMethod::Manual;
    }
}

TArray<FString> UMingGoRTSResourceSystem::GetResourceCollectionLocations(EResourceType ResourceType) const
{
    TArray<FString> Locations;
    
    // 根據資源類型返回收集地點
    switch (ResourceType)
    {
    case EResourceType::Wood:
        Locations.Add(TEXT("Forest"));
        Locations.Add(TEXT("Mountain"));
        break;
    case EResourceType::Stone:
        Locations.Add(TEXT("Mountain"));
        Locations.Add(TEXT("Quarry"));
        break;
    case EResourceType::Iron:
    case EResourceType::Copper:
        Locations.Add(TEXT("Mine"));
        Locations.Add(TEXT("Mountain"));
        break;
    case EResourceType::Food:
        Locations.Add(TEXT("Farm"));
        Locations.Add(TEXT("HuntingGround"));
        break;
    default:
        Locations.Add(TEXT("Base"));
        break;
    }
    
    return Locations;
}

TArray<FString> UMingGoRTSResourceSystem::GetResourceStorageRecommendations(EResourceType ResourceType) const
{
    TArray<FString> Recommendations;
    
    // 根據資源類型給出存儲建議
    if (ResourceType == EResourceType::Food)
    {
        Recommendations.Add(TEXT("需要冷藏存儲"));
        Recommendations.Add(TEXT("定期檢查過期"));
    }
    else if (ResourceType == EResourceType::Water)
    {
        Recommendations.Add(TEXT("需要密封容器"));
        Recommendations.Add(TEXT("定期檢查清潔度"));
    }
    else if (ResourceType == EResourceType::Weapons)
    {
        Recommendations.Add(TEXT("需要安全存儲"));
        Recommendations.Add(TEXT("定期維護保養"));
    }
    
    return Recommendations;
}

FString UMingGoRTSResourceSystem::GenerateResourceReport() const
{
    FString Report = TEXT("資源系統報告\n");
    Report += FString::Printf(TEXT("資源類型數量：%d\n"), ResourceInfoMap.Num());
    Report += FString::Printf(TEXT("活動收集數量：%d\n"), GetActiveCollections().Num());
    Report += FString::Printf(TEXT("存儲數量：%d\n"), ResourceStorages.Num());
    Report += FString::Printf(TEXT("轉換數量：%d\n"), ResourceConversions.Num());
    
    return Report;
}

void UMingGoRTSResourceSystem::AddResourceHistory(const FString& Event)
{
    ResourceHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (ResourceHistory.Num() > 1000)
    {
        ResourceHistory.RemoveAt(0);
    }
}

TArray<FString> UMingGoRTSResourceSystem::GetResourceHistory() const
{
    return ResourceHistory;
}

void UMingGoRTSResourceSystem::SetRequiredToolsAndSkills(FResourceCollection& Collection)
{
    // 根據資源類型和收集方法設置所需工具和技能
    switch (Collection.ResourceType)
    {
    case EResourceType::Wood:
        Collection.RequiredTools.Add(TEXT("Axe"));
        Collection.RequiredSkills.Add(TEXT("Lumberjack"));
        break;
    case EResourceType::Stone:
        Collection.RequiredTools.Add(TEXT("Pickaxe"));
        Collection.RequiredSkills.Add(TEXT("Mining"));
        break;
    case EResourceType::Iron:
    case EResourceType::Copper:
        Collection.RequiredTools.Add(TEXT("Pickaxe"));
        Collection.RequiredTools.Add(TEXT("Shovel"));
        Collection.RequiredSkills.Add(TEXT("Mining"));
        break;
    case EResourceType::Food:
        Collection.RequiredTools.Add(TEXT("HuntingRifle"));
        Collection.RequiredSkills.Add(TEXT("Hunting"));
        break;
    default:
        break;
    }
}

void UMingGoRTSResourceSystem::SetEnvironmentalFactors(FResourceCollection& Collection)
{
    // 根據收集地點設置環境因素
    Collection.EnvironmentalFactors.Add(TEXT("Weather"), 1.0f);
    Collection.EnvironmentalFactors.Add(TEXT("Season"), 1.0f);
    Collection.EnvironmentalFactors.Add(TEXT("TimeOfDay"), 1.0f);
    Collection.EnvironmentalFactors.Add(TEXT("Terrain"), 1.0f);
}

void UMingGoRTSResourceSystem::SetAllowedResourceTypes(FResourceStorage& Storage)
{
    // 根據存儲類型設置允許的資源類型
    // 簡化實現：允許所有資源類型
}

float UMingGoRTSResourceSystem::CalculateQualityImprovementCost(EResourceQuality CurrentQuality, EResourceQuality NewQuality) const
{
    float CostMultiplier = 1.0f;
    
    // 根據質量差異計算成本
    switch (CurrentQuality)
    {
    case EResourceQuality::Poor:
        CostMultiplier = 1.0f;
        break;
    case EResourceQuality::Normal:
        CostMultiplier = 2.0f;
        break;
    case EResourceQuality::Good:
        CostMultiplier = 3.0f;
        break;
    case EResourceQuality::Excellent:
        CostMultiplier = 5.0f;
        break;
    case EResourceQuality::Perfect:
        CostMultiplier = 10.0f;
        break;
    }
    
    return CostMultiplier;
}

FString UMingGoRTSResourceSystem::FindSuitableStorage(EResourceType ResourceType) const
{
    // 找到合適的存儲
    for (const auto& StoragePair : ResourceStorages)
    {
        const FResourceStorage& Storage = StoragePair.Value;
        
        // 檢查是否允許此資源類型
        if (Storage.AllowedTypes.Num() > 0 && !Storage.AllowedTypes.Contains(ResourceType))
        {
            continue;
        }
        
        // 檢查是否有足夠容量
        FResourceData ResourceInfo = GetResourceInfo(ResourceType);
        if (Storage.UsedCapacity + ResourceInfo.UnitWeight <= Storage.MaxCapacity)
        {
            return Storage.StorageID;
        }
    }
    
    return TEXT("");
}

void UMingGoRTSResourceSystem::AdjustHistoricalResourceValues()
{
    // 根據民國時期的歷史背景調整資源價值
    // 這裡可以根據歷史資料調整各種資源的稀有度和價值
}

void UMingGoRTSResourceSystem::UpdateCollectionYield(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return;
    }

    FResourceCollection& Collection = ResourceCollections[CollectionID];
    
    // 計算基礎產量
    float BaseYield = Collection.BaseYield;
    
    // 應用效率修正
    float EfficiencyModifier = Collection.Efficiency;
    
    // 應用收集方法加成
    float MethodBonus = GetCollectionMethodBonus(Collection.CollectionMethod);
    
    // 應用環境因素
    float EnvironmentalFactor = CalculateEnvironmentalImpact(CollectionID);
    
    // 計算最終產量
    Collection.CurrentYield = BaseYield * EfficiencyModifier * MethodBonus * EnvironmentalFactor;
}

float UMingGoRTSResourceSystem::CalculateEnvironmentalImpact(const FString& CollectionID) const
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return 1.0f;
    }

    const FResourceCollection& Collection = ResourceCollections[CollectionID];
    float TotalImpact = 1.0f;
    
    // 計算所有環境因素的影響
    for (const auto& FactorPair : Collection.EnvironmentalFactors)
    {
        TotalImpact *= FactorPair.Value;
    }
    
    return FMath::Clamp(TotalImpact, 0.1f, 2.0f);
}

bool UMingGoRTSResourceSystem::CheckCollectionConditions(const FString& CollectionID) const
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return false;
    }

    const FResourceCollection& Collection = ResourceCollections[CollectionID];
    
    // 檢查是否有必要的工具（這裡簡化實現）
    if (Collection.RequiredTools.Num() > 0)
    {
        // TODO: 檢查工具可用性
    }
    
    // 檢查是否有必要的技能（這裡簡化實現）
    if (Collection.RequiredSkills.Num() > 0)
    {
        // TODO: 檢查技能可用性
    }
    
    return true;
}

void UMingGoRTSResourceSystem::ProcessCollectionComplete(const FString& CollectionID)
{
    if (!ResourceCollections.Contains(CollectionID))
    {
        return;
    }

    FResourceCollection& Collection = ResourceCollections[CollectionID];
    
    // 處理收集完成的邏輯
    // 可以在這裡添加獎勵、經驗值等
    
    AddResourceHistory(FString::Printf(TEXT("收集完成：%s"), *CollectionID));
}

void UMingGoRTSResourceSystem::UpdateStorageCapacity(const FString& StorageID)
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return;
    }

    FResourceStorage& Storage = ResourceStorages[StorageID];
    
    // 計算已使用容量
    float UsedCapacity = 0.0f;
    for (const auto& ResourcePair : Storage.StoredResources)
    {
        const FResourceData& Resource = ResourcePair.Value;
        UsedCapacity += Resource.Quantity * Resource.UnitWeight;
    }
    
    Storage.UsedCapacity = UsedCapacity;
    Storage.CurrentCapacity = Storage.MaxCapacity - UsedCapacity;
}

bool UMingGoRTSResourceSystem::CheckStorageLimits(const FString& StorageID, EResourceType ResourceType, float Quantity) const
{
    if (!ResourceStorages.Contains(StorageID))
    {
        return false;
    }

    const FResourceStorage& Storage = ResourceStorages[StorageID];
    
    // 檢查是否允許此資源類型
    if (Storage.AllowedTypes.Num() > 0 && !Storage.AllowedTypes.Contains(ResourceType))
    {
        return false;
    }
    
    // 檢查容量限制
    FResourceData ResourceInfo = GetResourceInfo(ResourceType);
    float RequiredCapacity = Quantity * ResourceInfo.UnitWeight;
    
    return (Storage.UsedCapacity + RequiredCapacity) <= Storage.MaxCapacity;
}

void UMingGoRTSResourceSystem::HandleStorageFull(const FString& StorageID)
{
    // 處理存儲滿了的情況
    OnStorageFull.Broadcast(StorageID, GetStorageCapacity(StorageID));
    
    AddResourceHistory(FString::Printf(TEXT("存儲已滿：%s"), *StorageID));
}

float UMingGoRTSResourceSystem::CalculateConversionCost(const FString& ConversionID) const
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        return 0.0f;
    }

    const FResourceConversion& Conversion = ResourceConversions[ConversionID];
    float TotalCost = 0.0f;
    
    // 計算輸入資源的總價值
    for (const auto& InputPair : Conversion.InputResources)
    {
        FResourceData ResourceInfo = GetResourceInfo(InputPair.Key);
        TotalCost += ResourceInfo.UnitValue * InputPair.Value;
    }
    
    return TotalCost;
}

bool UMingGoRTSResourceSystem::CheckConversionConditions(const FString& ConversionID) const
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        return false;
    }

    const FResourceConversion& Conversion = ResourceConversions[ConversionID];
    
    // 檢查轉換是否可用
    if (!Conversion.bIsAvailable)
    {
        return false;
    }
    
    // 檢查是否有必要的建築（這裡簡化實現）
    if (Conversion.RequiredBuildings.Num() > 0)
    {
        // TODO: 檢查建築可用性
    }
    
    // 檢查是否有必要的技能（這裡簡化實現）
    if (Conversion.RequiredSkills.Num() > 0)
    {
        // TODO: 檢查技能可用性
    }
    
    return true;
}

void UMingGoRTSResourceSystem::ProcessConversionComplete(const FString& ConversionID)
{
    if (!ResourceConversions.Contains(ConversionID))
    {
        return;
    }

    // 處理轉換完成的邏輯
    AddResourceHistory(FString::Printf(TEXT("轉換完成：%s"), *ConversionID));
}

FString UMingGoRTSResourceSystem::GenerateCollectionID(EResourceType ResourceType) const
{
    return FString::Printf(TEXT("COLL_%s_%d"), *StaticEnum<EResourceType>()->GetValueAsString(ResourceType), FMath::RandRange(1000, 9999));
}

FString UMingGoRTSResourceSystem::GenerateStorageID(const FString& StorageName) const
{
    return FString::Printf(TEXT("STOR_%s_%d"), *StorageName, FMath::RandRange(1000, 9999));
}

FString UMingGoRTSResourceSystem::GenerateConversionID(const FString& ConversionName) const
{
    return FString::Printf(TEXT("CONV_%s_%d"), *ConversionName, FMath::RandRange(1000, 9999));
}

FString UMingGoRTSResourceSystem::GetResourceTypeName(EResourceType ResourceType) const
{
    const UEnum* EnumPtr = StaticEnum<EResourceType>();
    if (EnumPtr)
    {
        return EnumPtr->GetNameStringByValue(static_cast<int64>(ResourceType));
    }
    return TEXT("Unknown");
}

float UMingGoRTSResourceSystem::GetResourceTypeWeight(EResourceType ResourceType) const
{
    FResourceData ResourceInfo = GetResourceInfo(ResourceType);
    return ResourceInfo.UnitWeight;
}

float UMingGoRTSResourceSystem::GetCollectionMethodBonus(ECollectionMethod Method) const
{
    switch (Method)
    {
    case ECollectionMethod::Manual:
        return 1.0f;
    case ECollectionMethod::Automatic:
        return 1.2f;
    case ECollectionMethod::Production:
        return 1.5f;
    case ECollectionMethod::Trade:
        return 1.1f;
    case ECollectionMethod::Plunder:
        return 0.8f;
    case ECollectionMethod::Harvest:
        return 1.3f;
    case ECollectionMethod::Mining:
        return 1.1f;
    case ECollectionMethod::Fishing:
        return 1.2f;
    case ECollectionMethod::Hunting:
        return 1.1f;
    case ECollectionMethod::Foraging:
        return 0.9f;
    default:
        return 1.0f;
    }
}

float UMingGoRTSResourceSystem::GetQualityBonus(EResourceQuality Quality) const
{
    switch (Quality)
    {
    case EResourceQuality::Poor:
        return 0.5f;
    case EResourceQuality::Normal:
        return 1.0f;
    case EResourceQuality::Good:
        return 1.5f;
    case EResourceQuality::Excellent:
        return 2.0f;
    case EResourceQuality::Perfect:
        return 3.0f;
    default:
        return 1.0f;
    }
}

float UMingGoRTSResourceSystem::CalculateResourceTotalValue(const FResourceData& ResourceData) const
{
    return ResourceData.Quantity * ResourceData.UnitValue * GetQualityBonus(ResourceData.Quality);
}

bool UMingGoRTSResourceSystem::CheckResourceCompatibility(EResourceType ResourceType1, EResourceType ResourceType2) const
{
    // 簡化實現：大部分資源都兼容
    return true;
}

ECollectionMethod UMingGoRTSResourceSystem::GetRecommendedCollectionMethod(EResourceType ResourceType) const
{
    switch (ResourceType)
    {
    case EResourceType::Wood:
        return ECollectionMethod::Harvest;
    case EResourceType::Stone:
    case EResourceType::Iron:
    case EResourceType::Copper:
    case EResourceType::Coal:
        return ECollectionMethod::Mining;
    case EResourceType::Food:
        return ECollectionMethod::Hunting;
    case EResourceType::Water:
        return ECollectionMethod::Foraging;
    default:
        return ECollectionMethod::Manual;
    }
}
