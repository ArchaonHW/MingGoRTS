#include "MingRTSResourceManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

UMingRTSResourceManager::UMingRTSResourceManager()
    : BaseProductionMultiplier(1.0f)
    , BaseConsumptionMultiplier(1.0f)
    , TradeFee(0.1f)
    , bAutoTrade(true)
    , bAutoAllocate(true)
    , bIsInitialized(false)
{
}

void UMingRTSResourceManager::InitializeResourceManager()
{
    InitializeDefaultResources();
    InitializeResourceEfficiencies();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("RTS Resource Manager initialized"));
}

void UMingRTSResourceManager::AddResource(ERTSResourceType ResourceType, float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }

    if (Resources.Contains(ResourceType))
    {
        FRTSResourceData& ResourceData = Resources[ResourceType];
        ResourceData.CurrentAmount = FMath::Clamp(ResourceData.CurrentAmount + Amount, 0.0f, ResourceData.MaxAmount);
        
        NotifyResourceChanged(ResourceType, ResourceData.CurrentAmount);
        
        // 檢查是否資源耗盡
        if (ResourceData.CurrentAmount <= 0.0f && ResourceData.ResourceState != ERTSResourceState::Depleted)
        {
            ResourceData.ResourceState = ERTSResourceState::Depleted;
            NotifyResourceDepleted(ResourceType);
        }
        else if (ResourceData.CurrentAmount > 0.0f && ResourceData.ResourceState == ERTSResourceState::Depleted)
        {
            ResourceData.ResourceState = ERTSResourceState::Available;
        }
        
        UE_LOG(LogTemp, Log, TEXT("Added %.1f %s"), Amount, *GetResourceName(ResourceType));
    }
}

void UMingRTSResourceManager::RemoveResource(ERTSResourceType ResourceType, float Amount)
{
    AddResource(ResourceType, -Amount);
}

bool UMingRTSResourceManager::HasEnoughResource(ERTSResourceType ResourceType, float Amount) const
{
    if (Resources.Contains(ResourceType))
    {
        return Resources[ResourceType].CurrentAmount >= Amount;
    }
    return false;
}

float UMingRTSResourceManager::GetResourceAmount(ERTSResourceType ResourceType) const
{
    if (Resources.Contains(ResourceType))
    {
        return Resources[ResourceType].CurrentAmount;
    }
    return 0.0f;
}

float UMingRTSResourceManager::GetMaxResourceAmount(ERTSResourceType ResourceType) const
{
    if (Resources.Contains(ResourceType))
    {
        return Resources[ResourceType].MaxAmount;
    }
    return 0.0f;
}

void UMingRTSResourceManager::SetMaxResourceAmount(ERTSResourceType ResourceType, float MaxAmount)
{
    if (Resources.Contains(ResourceType))
    {
        Resources[ResourceType].MaxAmount = FMath::Max(0.0f, MaxAmount);
        
        // 調整當前數量以符合新的最大值
        FRTSResourceData& ResourceData = Resources[ResourceType];
        ResourceData.CurrentAmount = FMath::Min(ResourceData.CurrentAmount, ResourceData.MaxAmount);
        
        UE_LOG(LogTemp, Log, TEXT("Set max %s amount to %.1f"), *GetResourceName(ResourceType), MaxAmount);
    }
}

void UMingRTSResourceManager::SetResourceProductionRate(ERTSResourceType ResourceType, float Rate)
{
    if (Resources.Contains(ResourceType))
    {
        Resources[ResourceType].ProductionRate = FMath::Max(0.0f, Rate);
        UE_LOG(LogTemp, Log, TEXT("Set %s production rate to %.1f"), *GetResourceName(ResourceType), Rate);
    }
}

void UMingRTSResourceManager::SetResourceConsumptionRate(ERTSResourceType ResourceType, float Rate)
{
    if (Resources.Contains(ResourceType))
    {
        Resources[ResourceType].ConsumptionRate = FMath::Max(0.0f, Rate);
        UE_LOG(LogTemp, Log, TEXT("Set %s consumption rate to %.1f"), *GetResourceName(ResourceType), Rate);
    }
}

void UMingRTSResourceManager::AddResourceSource(const FRTSResourceSourceData& SourceData)
{
    if (SourceData.SourceID.IsEmpty())
    {
        return;
    }

    ResourceSources.Add(SourceData.SourceID, SourceData);
    
    UE_LOG(LogTemp, Log, TEXT("Added resource source: %s"), *SourceData.SourceName);
}

void UMingRTSResourceManager::RemoveResourceSource(const FString& SourceID)
{
    if (ResourceSources.Contains(SourceID))
    {
        ResourceSources.Remove(SourceID);
        UE_LOG(LogTemp, Log, TEXT("Removed resource source: %s"), *SourceID);
    }
}

FRTSResourceSourceData UMingRTSResourceManager::GetResourceSource(const FString& SourceID) const
{
    if (ResourceSources.Contains(SourceID))
    {
        return ResourceSources[SourceID];
    }
    
    return FRTSResourceSourceData();
}

TArray<FRTSResourceSourceData> UMingRTSResourceManager::GetAllResourceSources() const
{
    TArray<FRTSResourceSourceData> AllSources;
    
    for (const auto& SourcePair : ResourceSources)
    {
        AllSources.Add(SourcePair.Value);
    }
    
    return AllSources;
}

TArray<FRTSResourceSourceData> UMingRTSResourceManager::GetResourceSourcesByType(ERTSResourceType ResourceType) const
{
    TArray<FRTSResourceSourceData> SourcesByType;
    
    for (const auto& SourcePair : ResourceSources)
    {
        if (SourcePair.Value.ResourceType == ResourceType)
        {
            SourcesByType.Add(SourcePair.Value);
        }
    }
    
    return SourcesByType;
}

void UMingRTSResourceManager::OperateResourceSource(const FString& SourceID, bool bOperate)
{
    if (ResourceSources.Contains(SourceID))
    {
        FRTSResourceSourceData& Source = ResourceSources[SourceID];
        Source.bIsOperational = bOperate;
        
        if (bOperate)
        {
            Source.SourceState = ERTSResourceState::Available;
        }
        else
        {
            Source.SourceState = ERTSResourceState::Occupied;
        }
        
        NotifyResourceSourceStateChanged(SourceID, Source.SourceState);
        
        UE_LOG(LogTemp, Log, TEXT("Resource source %s operational: %s"), *SourceID, bOperate ? TEXT("true") : TEXT("false"));
    }
}

bool UMingRTSResourceManager::IsResourceSourceOperational(const FString& SourceID) const
{
    if (ResourceSources.Contains(SourceID))
    {
        return ResourceSources[SourceID].bIsOperational;
    }
    return false;
}

void UMingRTSResourceManager::UpdateResourceProduction(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    // 處理所有資源源
    TArray<FString> SourcesToUpdate;
    
    for (auto& SourcePair : ResourceSources)
    {
        FRTSResourceSourceData& Source = SourcePair.Value;
        
        if (Source.bIsOperational && Source.SourceState == ERTSResourceState::Available)
        {
            ProcessResourceSource(Source, DeltaTime);
            
            // 檢查是否需要更新狀態
            if (Source.Yield <= 0.0f)
            {
                Source.SourceState = ERTSResourceState::Depleted;
                NotifyResourceSourceStateChanged(Source.SourceID, Source.SourceState);
            }
        }
    }
    
    // 更新資源生產和消耗
    for (auto& ResourcePair : Resources)
    {
        FRTSResourceData& ResourceData = ResourcePair.Value;
        
        // 計算淨生產
        float NetProduction = (ResourceData.ProductionRate * BaseProductionMultiplier) - 
                            (ResourceData.ConsumptionRate * BaseConsumptionMultiplier);
        
        // 應用效率加成
        if (ResourceEfficiencies.Contains(ResourceData.ResourceType))
        {
            NetProduction *= ResourceEfficiencies[ResourceData.ResourceType];
        }
        
        // 更新資源數量
        float ProductionAmount = NetProduction * DeltaTime;
        AddResource(ResourceData.ResourceType, ProductionAmount);
    }
}

float UMingRTSResourceManager::CalculateTotalProduction() const
{
    float TotalProduction = 0.0f;
    
    for (const auto& ResourcePair : Resources)
    {
        const FRTSResourceData& ResourceData = ResourcePair.Value;
        TotalProduction += ResourceData.ProductionRate * BaseProductionMultiplier;
    }
    
    return TotalProduction;
}

float UMingRTSResourceManager::CalculateTotalConsumption() const
{
    float TotalConsumption = 0.0f;
    
    for (const auto& ResourcePair : Resources)
    {
        const FRTSResourceData& ResourceData = ResourcePair.Value;
        TotalConsumption += ResourceData.ConsumptionRate * BaseConsumptionMultiplier;
    }
    
    return TotalConsumption;
}

void UMingRTSResourceManager::OptimizeResourceAllocation()
{
    if (!bAutoAllocate)
    {
        return;
    }

    // 簡單的資源分配優化
    for (auto& ResourcePair : Resources)
    {
        ERTSResourceType ResourceType = ResourcePair.Key;
        FRTSResourceData& ResourceData = ResourcePair.Value;
        
        // 如果資源過多，分配到其他用途
        if (ResourceData.CurrentAmount > ResourceData.MaxAmount * 0.8f)
        {
            float Surplus = ResourceData.CurrentAmount - (ResourceData.MaxAmount * 0.5f);
            AllocateResource(ResourceType, Surplus, TEXT("Trade"));
        }
        
        // 如果資源不足，從其他用途回收
        else if (ResourceData.CurrentAmount < ResourceData.MaxAmount * 0.2f)
        {
            ClearResourceAllocation(TEXT("Trade"));
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Optimized resource allocation"));
}

bool UMingRTSResourceManager::ExecuteTrade(const FRTSTradeData& TradeData)
{
    if (!ValidateTrade(TradeData))
    {
        return false;
    }

    // 檢查是否有足夠的資源進行交易
    if (!HasEnoughResource(TradeData.OfferResource, TradeData.OfferAmount))
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient resources for trade"));
        return false;
    }

    // 計算交易費用
    float Fee = TradeData.OfferAmount * TradeFee;
    float TotalOffer = TradeData.OfferAmount + Fee;
    
    // 執行交易
    RemoveResource(TradeData.OfferResource, TotalOffer);
    AddResource(TradeData.RequestResource, TradeData.RequestAmount);
    
    NotifyTradeCompleted(TradeData, true);
    
    UE_LOG(LogTemp, Log, TEXT("Trade completed: %.1f %s for %.1f %s"), 
           TradeData.OfferAmount, *GetResourceName(TradeData.OfferResource),
           TradeData.RequestAmount, *GetResourceName(TradeData.RequestResource));
    
    return true;
}

void UMingRTSResourceManager::AddTradeOffer(const FRTSTradeData& TradeData)
{
    TradeOffers.Add(TradeData);
    
    UE_LOG(LogTemp, Log, TEXT("Added trade offer"));
}

void UMingRTSResourceManager::RemoveTradeOffer(const FString& TradeID)
{
    // 這裡需要根據TradeID移除交易
    // 暫時移除最後一個
    if (TradeOffers.Num() > 0)
    {
        TradeOffers.RemoveAt(TradeOffers.Num() - 1);
        UE_LOG(LogTemp, Log, TEXT("Removed trade offer"));
    }
}

TArray<FRTSTradeData> UMingRTSResourceManager::GetAvailableTrades() const
{
    TArray<FRTSTradeData> AvailableTrades;
    
    for (const FRTSTradeData& Trade : TradeOffers)
    {
        if (Trade.bIsAvailable)
        {
            AvailableTrades.Add(Trade);
        }
    }
    
    return AvailableTrades;
}

float UMingRTSResourceManager::CalculateExchangeRate(ERTSResourceType OfferResource, ERTSResourceType RequestResource) const
{
    // 簡單的匯率計算（基於資源價值）
    float OfferValue = GetResourceValue(OfferResource);
    float RequestValue = GetResourceValue(RequestResource);
    
    if (RequestValue > 0.0f)
    {
        return OfferValue / RequestValue;
    }
    
    return 1.0f;
}

void UMingRTSResourceManager::AllocateResource(ERTSResourceType ResourceType, float Amount, const FString& Purpose)
{
    if (!ResourceAllocations.Contains(ResourceType))
    {
        ResourceAllocations.Add(ResourceType, TMap<FString, float>());
    }
    
    TMap<FString, float>& Allocations = ResourceAllocations[ResourceType];
    
    if (Allocations.Contains(Purpose))
    {
        Allocations[Purpose] += Amount;
    }
    else
    {
        Allocations.Add(Purpose, Amount);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Allocated %.1f %s to %s"), Amount, *GetResourceName(ResourceType), *Purpose);
}

float UMingRTSResourceManager::GetAllocatedResource(ERTSResourceType ResourceType, const FString& Purpose) const
{
    if (ResourceAllocations.Contains(ResourceType))
    {
        const TMap<FString, float>& Allocations = ResourceAllocations[ResourceType];
        if (Allocations.Contains(Purpose))
        {
            return Allocations[Purpose];
        }
    }
    
    return 0.0f;
}

void UMingRTSResourceManager::ClearResourceAllocation(const FString& Purpose)
{
    for (auto& AllocationPair : ResourceAllocations)
    {
        TMap<FString, float>& Allocations = AllocationPair.Value;
        Allocations.Remove(Purpose);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Cleared resource allocation for: %s"), *Purpose);
}

TMap<FString, float> UMingRTSResourceManager::GetAllAllocations(ERTSResourceType ResourceType) const
{
    if (ResourceAllocations.Contains(ResourceType))
    {
        return ResourceAllocations[ResourceType];
    }
    
    return TMap<FString, float>();
}

float UMingRTSResourceManager::PredictResourceAmount(ERTSResourceType ResourceType, float TimeInSeconds) const
{
    if (!Resources.Contains(ResourceType))
    {
        return 0.0f;
    }

    const FRTSResourceData& ResourceData = Resources[ResourceType];
    
    // 計算淨生產率
    float NetProduction = (ResourceData.ProductionRate * BaseProductionMultiplier) - 
                          (ResourceData.ConsumptionRate * BaseConsumptionMultiplier);
    
    // 應用效率加成
    float Efficiency = 1.0f;
    if (ResourceEfficiencies.Contains(ResourceType))
    {
        Efficiency = ResourceEfficiencies[ResourceType];
    }
    
    // 預測未來數量
    float PredictedAmount = ResourceData.CurrentAmount + (NetProduction * Efficiency * TimeInSeconds);
    
    return FMath::Clamp(PredictedAmount, 0.0f, ResourceData.MaxAmount);
}

bool UMingRTSResourceManager::WillResourceDeplete(ERTSResourceType ResourceType, float TimeInSeconds) const
{
    float PredictedAmount = PredictResourceAmount(ResourceType, TimeInSeconds);
    return PredictedAmount <= 0.0f;
}

float UMingRTSResourceManager::GetTimeToDepletion(ERTSResourceType ResourceType) const
{
    if (!Resources.Contains(ResourceType))
    {
        return -1.0f;
    }

    const FRTSResourceData& ResourceData = Resources[ResourceType];
    
    if (ResourceData.CurrentAmount <= 0.0f)
    {
        return 0.0f;
    }
    
    // 計算淨消耗率
    float NetProduction = (ResourceData.ProductionRate * BaseProductionMultiplier) - 
                          (ResourceData.ConsumptionRate * BaseConsumptionMultiplier);
    
    if (NetProduction >= 0.0f)
    {
        return -1.0f; // 不會耗盡
    }
    
    // 計算耗盡時間
    float TimeToDepletion = ResourceData.CurrentAmount / FMath::Abs(NetProduction);
    
    return TimeToDepletion;
}

float UMingRTSResourceManager::GetResourceEfficiency(ERTSResourceType ResourceType) const
{
    if (ResourceEfficiencies.Contains(ResourceType))
    {
        return ResourceEfficiencies[ResourceType];
    }
    
    return 1.0f;
}

void UMingRTSResourceManager::ImproveResourceEfficiency(ERTSResourceType ResourceType, float EfficiencyBonus)
{
    float CurrentEfficiency = GetResourceEfficiency(ResourceType);
    SetResourceEfficiency(ResourceType, CurrentEfficiency + EfficiencyBonus);
}

void UMingRTSResourceManager::SetResourceEfficiency(ERTSResourceType ResourceType, float Efficiency)
{
    ResourceEfficiencies.Add(ResourceType, FMath::Clamp(Efficiency, 0.1f, 3.0f));
    
    UE_LOG(LogTemp, Log, TEXT("Set %s efficiency to %.2f"), *GetResourceName(ResourceType), Efficiency);
}

int32 UMingRTSResourceManager::GetTotalResourceSourceCount() const
{
    return ResourceSources.Num();
}

int32 UMingRTSResourceManager::GetOperationalResourceSourceCount() const
{
    int32 Count = 0;
    
    for (const auto& SourcePair : ResourceSources)
    {
        if (SourcePair.Value.bIsOperational)
        {
            Count++;
        }
    }
    
    return Count;
}

float UMingRTSResourceManager::GetTotalResourceValue() const
{
    float TotalValue = 0.0f;
    
    for (const auto& ResourcePair : Resources)
    {
        const FRTSResourceData& ResourceData = ResourcePair.Value;
        TotalValue += ResourceData.CurrentAmount * GetResourceValue(ResourceData.ResourceType);
    }
    
    return TotalValue;
}

TArray<ERTSResourceType> UMingRTSResourceManager::GetAvailableResourceTypes() const
{
    TArray<ERTSResourceType> AvailableTypes;
    
    for (const auto& ResourcePair : Resources)
    {
        if (ResourcePair.Value.CurrentAmount > 0.0f)
        {
            AvailableTypes.Add(ResourcePair.Key);
        }
    }
    
    return AvailableTypes;
}

void UMingRTSResourceManager::OnResourceSourceDestroyed(const FString& SourceID)
{
    if (ResourceSources.Contains(SourceID))
    {
        FRTSResourceSourceData& Source = ResourceSources[SourceID];
        Source.SourceState = ERTSResourceState::Damaged;
        Source.bIsOperational = false;
        
        NotifyResourceSourceStateChanged(SourceID, Source.SourceState);
        
        UE_LOG(LogTemp, Warning, TEXT("Resource source destroyed: %s"), *SourceID);
    }
}

void UMingRTSResourceManager::OnResourceSourceCaptured(const FString& SourceID, const FString& NewOwner)
{
    if (ResourceSources.Contains(SourceID))
    {
        FRTSResourceSourceData& Source = ResourceSources[SourceID];
        Source.SourceState = ERTSResourceState::Occupied;
        Source.bIsOperational = false;
        
        NotifyResourceSourceStateChanged(SourceID, Source.SourceState);
        
        UE_LOG(LogTemp, Warning, TEXT("Resource source captured: %s by %s"), *SourceID, *NewOwner);
    }
}

void UMingRTSResourceManager::OnWorkerAssigned(const FString& SourceID, int32 WorkerCount)
{
    if (ResourceSources.Contains(SourceID))
    {
        FRTSResourceSourceData& Source = ResourceSources[SourceID];
        Source.CurrentWorkers = FMath::Clamp(WorkerCount, 0, Source.MaxWorkers);
        
        // 根據工人數量調整產量
        float WorkerEfficiency = (float)Source.CurrentWorkers / (float)Source.MaxWorkers;
        Source.Yield = Source.MaxYield * WorkerEfficiency;
        
        UE_LOG(LogTemp, Log, TEXT("Assigned %d workers to resource source: %s"), WorkerCount, *SourceID);
    }
}

void UMingRTSResourceManager::InitializeDefaultResources()
{
    // 初始化所有資源類型
    TArray<ERTSResourceType> AllResourceTypes = {
        ERTSResourceType::Gold, ERTSResourceType::Wood, ERTSResourceType::Stone,
        ERTSResourceType::Iron, ERTSResourceType::Food, ERTSResourceType::Coal,
        ERTSResourceType::Oil, ERTSResourceType::Uranium, ERTSResourceType::Crystal,
        ERTSResourceType::Silk
    };
    
    for (ERTSResourceType ResourceType : AllResourceTypes)
    {
        FRTSResourceData ResourceData;
        ResourceData.ResourceType = ResourceType;
        ResourceData.ResourceName = GetResourceName(ResourceType);
        ResourceData.CurrentAmount = 100.0f; // 初始資源
        ResourceData.MaxAmount = 10000.0f;
        ResourceData.ProductionRate = 10.0f;
        ResourceData.ConsumptionRate = 5.0f;
        ResourceData.ResourceState = ERTSResourceState::Available;
        
        Resources.Add(ResourceType, ResourceData);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Initialized default resources"));
}

void UMingRTSResourceManager::InitializeResourceEfficiencies()
{
    // 設置默認效率
    ResourceEfficiencies.Add(ERTSResourceType::Gold, 1.0f);
    ResourceEfficiencies.Add(ERTSResourceType::Wood, 1.2f);
    ResourceEfficiencies.Add(ERTSResourceType::Stone, 1.0f);
    ResourceEfficiencies.Add(ERTSResourceType::Iron, 0.8f);
    ResourceEfficiencies.Add(ERTSResourceType::Food, 1.5f);
    ResourceEfficiencies.Add(ERTSResourceType::Coal, 0.9f);
    ResourceEfficiencies.Add(ERTSResourceType::Oil, 0.7f);
    ResourceEfficiencies.Add(ERTSResourceType::Uranium, 0.5f);
    ResourceEfficiencies.Add(ERTSResourceType::Crystal, 0.3f);
    ResourceEfficiencies.Add(ERTSResourceType::Silk, 0.6f);
    
    UE_LOG(LogTemp, Log, TEXT("Initialized resource efficiencies"));
}

void UMingRTSResourceManager::ProcessResourceSource(FRTSResourceSourceData& Source, float DeltaTime)
{
    if (!Source.bIsOperational || Source.SourceState != ERTSResourceState::Available)
    {
        return;
    }

    // 計算生產量
    float Production = Source.Yield * DeltaTime;
    
    // 根據工人數量調整
    if (Source.MaxWorkers > 0)
    {
        float WorkerEfficiency = (float)Source.CurrentWorkers / (float)Source.MaxWorkers;
        Production *= WorkerEfficiency;
    }
    
    // 添加資源
    AddResource(Source.ResourceType, Production);
    
    // 減少源產量
    Source.Yield -= Production * 0.1f; // 每次生產消耗10%的源產量
    Source.Yield = FMath::Max(0.0f, Source.Yield);
}

void UMingRTSResourceManager::UpdateResourceState(ERTSResourceType ResourceType)
{
    if (Resources.Contains(ResourceType))
    {
        FRTSResourceData& ResourceData = Resources[ResourceType];
        
        if (ResourceData.CurrentAmount <= 0.0f)
        {
            ResourceData.ResourceState = ERTSResourceState::Depleted;
        }
        else if (ResourceData.CurrentAmount >= ResourceData.MaxAmount * 0.9f)
        {
            ResourceData.ResourceState = ERTSResourceState::Available;
        }
        else
        {
            ResourceData.ResourceState = ERTSResourceState::Available;
        }
    }
}

FString UMingRTSResourceManager::GenerateResourceSourceID(ERTSResourceSource SourceType) const
{
    static int32 SourceCounter = 0;
    return FString::Printf(TEXT("Source_%d_%d"), (int32)SourceType, SourceCounter++);
}

FString UMingRTSResourceManager::GetResourceName(ERTSResourceType ResourceType) const
{
    switch (ResourceType)
    {
        case ERTSResourceType::Gold: return TEXT("黃金");
        case ERTSResourceType::Wood: return TEXT("木材");
        case ERTSResourceType::Stone: return TEXT("石材");
        case ERTSResourceType::Iron: return TEXT("鐵礦");
        case ERTSResourceType::Food: return TEXT("糧食");
        case ERTSResourceType::Coal: return TEXT("煤炭");
        case ERTSResourceType::Oil: return TEXT("石油");
        case ERTSResourceType::Uranium: return TEXT("鈾礦");
        case ERTSResourceType::Crystal: return TEXT("水晶");
        case ERTSResourceType::Silk: return TEXT("絲綢");
        default: return TEXT("未知");
    }
}

FString UMingRTSResourceManager::GetResourceSourceName(ERTSResourceSource SourceType) const
{
    switch (SourceType)
    {
        case ERTSResourceSource::Mine: return TEXT("礦山");
        case ERTSResourceSource::Forest: return TEXT("森林");
        case ERTSResourceSource::Quarry: return TEXT("採石場");
        case ERTSResourceSource::Farm: return TEXT("農場");
        case ERTSResourceSource::OilWell: return TEXT("油井");
        case ERTSResourceSource::CrystalCave: return TEXT("水晶洞");
        case ERTSResourceSource::SilkFarm: return TEXT("絲綢農場");
        case ERTSResourceSource::TradingPost: return TEXT("貿易站");
        default: return TEXT("未知");
    }
}

float UMingRTSResourceManager::GetResourceValue(ERTSResourceType ResourceType) const
{
    switch (ResourceType)
    {
        case ERTSResourceType::Gold: return 10.0f;
        case ERTSResourceType::Wood: return 1.0f;
        case ERTSResourceType::Stone: return 1.5f;
        case ERTSResourceType::Iron: return 3.0f;
        case ERTSResourceType::Food: return 2.0f;
        case ERTSResourceType::Coal: return 2.5f;
        case ERTSResourceType::Oil: return 8.0f;
        case ERTSResourceType::Uranium: return 15.0f;
        case ERTSResourceType::Crystal: return 20.0f;
        case ERTSResourceType::Silk: return 5.0f;
        default: return 1.0f;
    }
}

bool UMingRTSResourceManager::ValidateTrade(const FRTSTradeData& TradeData) const
{
    // 檢查交易是否有效
    if (TradeData.OfferAmount <= 0.0f || TradeData.RequestAmount <= 0.0f)
    {
        return false;
    }
    
    if (TradeData.ExchangeRate <= 0.0f)
    {
        return false;
    }
    
    if (!HasEnoughResource(TradeData.OfferResource, TradeData.OfferAmount))
    {
        return false;
    }
    
    return true;
}

void UMingRTSResourceManager::NotifyResourceChanged(ERTSResourceType ResourceType, float NewAmount)
{
    OnResourceChanged.Broadcast(ResourceType, NewAmount);
}

void UMingRTSResourceManager::NotifyResourceSourceStateChanged(const FString& SourceID, ERTSResourceState NewState)
{
    OnResourceSourceStateChanged.Broadcast(SourceID, NewState);
}

void UMingRTSResourceManager::NotifyResourceDepleted(ERTSResourceType ResourceType)
{
    OnResourceDepleted.Broadcast(ResourceType);
}

void UMingRTSResourceManager::NotifyTradeCompleted(const FRTSTradeData& TradeData, bool bSuccess)
{
    OnTradeCompleted.Broadcast(TradeData, bSuccess);
}

// 資源管理系統優化功能
void UMingRTSResourceManager::OptimizeResourceManagement()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing resource management..."));
    
    // 優化生產率
    OptimizeProductionRates();
    
    // 平衡消耗
    BalanceResourceConsumption();
    
    // 更新效率指標
    UpdateEfficiencyMetrics();
    
    UE_LOG(LogTemp, Log, TEXT("Resource management optimized"));
}

void UMingRTSResourceManager::BalanceResourceDistribution()
{
    UE_LOG(LogTemp, Log, TEXT("Balancing resource distribution..."));
    
    // 自動平衡資源
    AutoBalanceResources();
    
    // 計算最優分配
    CalculateOptimalAllocation();
    
    UE_LOG(LogTemp, Log, TEXT("Resource distribution balanced"));
}

void UMingRTSResourceManager::EnhanceResourceEfficiency()
{
    UE_LOG(LogTemp, Log, TEXT("Enhancing resource efficiency..."));
    
    // 提高所有資源效率
    for (auto& ResourcePair : ResourceEfficiencies)
    {
        ResourcePair.Value *= EfficiencyBonus;
        ResourcePair.Value = FMath::Min(ResourcePair.Value, 2.0f); // 限制最大效率
    }
    
    UE_LOG(LogTemp, Log, TEXT("Resource efficiency enhanced"));
}

void UMingRTSResourceManager::ImplementSmartAllocation()
{
    if (!bEnableSmartAllocation)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Implementing smart allocation..."));
    
    // 基於優先級進行智能分配
    TArray<ERTSResourceType> ResourceTypes;
    ResourceTypes.Add(ERTSResourceType::Gold);
    ResourceTypes.Add(ERTSResourceType::Food);
    ResourceTypes.Add(ERTSResourceType::Wood);
    ResourceTypes.Add(ERTSResourceType::Stone);
    ResourceTypes.Add(ERTSResourceType::Iron);
    ResourceTypes.Add(ERTSResourceType::Coal);
    ResourceTypes.Add(ERTSResourceType::Oil);
    ResourceTypes.Add(ERTSResourceType::Uranium);
    ResourceTypes.Add(ERTSResourceType::Crystal);
    ResourceTypes.Add(ERTSResourceType::Silk);
    
    // 按優先級排序
    ResourceTypes.Sort([this](ERTSResourceType A, ERTSResourceType B)
    {
        return CalculateResourcePriority(A) > CalculateResourcePriority(B);
    });
    
    // 智能分配資源
    for (ERTSResourceType ResourceType : ResourceTypes)
    {
        float CurrentAmount = GetResourceAmount(ResourceType);
        float MaxAmount = GetMaxResourceAmount(ResourceType);
        float Ratio = CurrentAmount / MaxAmount;
        
        if (Ratio < OptimizationThreshold)
        {
            // 資源不足，優先分配
            float Priority = CalculateResourcePriority(ResourceType);
            if (Priority > 0.7f)
            {
                // 高優先級資源，提高生產率
                FRTSResourceData ResourceData = Resources[ResourceType];
                ResourceData.ProductionRate *= 1.2f;
                Resources[ResourceType] = ResourceData;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Smart allocation implemented"));
}

float UMingRTSResourceManager::GetTimeToDepletion(ERTSResourceType ResourceType) const
{
    if (!Resources.Contains(ResourceType))
    {
        return -1.0f;
    }
    
    const FRTSResourceData& ResourceData = Resources[ResourceType];
    float NetRate = ResourceData.ProductionRate - ResourceData.ConsumptionRate;
    
    if (NetRate >= 0.0f)
    {
        return -1.0f; // 不會耗盡
    }
    
    return ResourceData.CurrentAmount / FMath::Abs(NetRate);
}

void UMingRTSResourceManager::OptimizeProductionRates()
{
    for (auto& ResourcePair : Resources)
    {
        ERTSResourceType ResourceType = ResourcePair.Key;
        FRTSResourceData& ResourceData = ResourcePair.Value;
        
        // 根據當前庫存調整生產率
        float CurrentRatio = ResourceData.CurrentAmount / ResourceData.MaxAmount;
        
        if (CurrentRatio < 0.3f)
        {
            // 庫存低，提高生產率
            ResourceData.ProductionRate *= 1.3f;
        }
        else if (CurrentRatio > 0.8f)
        {
            // 庫存高，降低生產率以節約資源
            ResourceData.ProductionRate *= 0.8f;
        }
    }
}

void UMingRTSResourceManager::BalanceResourceConsumption()
{
    // 計算總消耗和總生產
    float TotalProduction = 0.0f;
    float TotalConsumption = 0.0f;
    
    for (const auto& ResourcePair : Resources)
    {
        TotalProduction += ResourcePair.Value.ProductionRate;
        TotalConsumption += ResourcePair.Value.ConsumptionRate;
    }
    
    // 如果消耗大於生產，調整消耗率
    if (TotalConsumption > TotalProduction * 1.2f)
    {
        float ReductionFactor = TotalProduction * 1.1f / TotalConsumption;
        
        for (auto& ResourcePair : Resources)
        {
            ResourcePair.Value.ConsumptionRate *= ReductionFactor;
        }
    }
}

void UMingRTSResourceManager::CalculateOptimalAllocation()
{
    // 計算每種資源的最優分配比例
    TMap<ERTSResourceType, float> OptimalRatios;
    
    // 基於資源類型設置最優比例
    OptimalRatios.Add(ERTSResourceType::Food, 0.3f);    // 糧食最重要
    OptimalRatios.Add(ERTSResourceType::Gold, 0.2f);    // 黃金次重要
    OptimalRatios.Add(ERTSResourceType::Wood, 0.15f);   // 木材中等
    OptimalRatios.Add(ERTSResourceType::Stone, 0.1f);   // 石材較低
    OptimalRatios.Add(ERTSResourceType::Iron, 0.1f);    // 鐵礦較低
    OptimalRatios.Add(ERTSResourceType::Coal, 0.05f);   // 煤炭低
    OptimalRatios.Add(ERTSResourceType::Oil, 0.05f);    // 石油低
    OptimalRatios.Add(ERTSResourceType::Uranium, 0.02f); // 鈾礦很低
    OptimalRatios.Add(ERTSResourceType::Crystal, 0.02f); // 水晶很低
    OptimalRatios.Add(ERTSResourceType::Silk, 0.01f);   // 絲綢最低
    
    // 調整當前分配趨向最優比例
    for (const auto& OptimalPair : OptimalRatios)
    {
        ERTSResourceType ResourceType = OptimalPair.Key;
        float OptimalRatio = OptimalPair.Value;
        
        if (Resources.Contains(ResourceType))
        {
            FRTSResourceData& ResourceData = Resources[ResourceType];
            float CurrentRatio = ResourceData.CurrentAmount / ResourceData.MaxAmount;
            
            // 調整生產率以達到最優比例
            if (CurrentRatio < OptimalRatio * 0.8f)
            {
                ResourceData.ProductionRate *= 1.1f;
            }
            else if (CurrentRatio > OptimalRatio * 1.2f)
            {
                ResourceData.ProductionRate *= 0.9f;
            }
        }
    }
}

void UMingRTSResourceManager::UpdateEfficiencyMetrics()
{
    for (auto& ResourcePair : Resources)
    {
        ERTSResourceType ResourceType = ResourcePair.Key;
        FRTSResourceData& ResourceData = ResourcePair.Value;
        
        // 計算當前效率
        float CurrentEfficiency = ResourceData.ProductionRate / FMath::Max(ResourceData.ConsumptionRate, 0.1f);
        
        // 更新效率記錄
        if (ResourceEfficiencies.Contains(ResourceType))
        {
            ResourceEfficiencies[ResourceType] = (ResourceEfficiencies[ResourceType] * 0.8f) + (CurrentEfficiency * 0.2f);
        }
        else
        {
            ResourceEfficiencies.Add(ResourceType, CurrentEfficiency);
        }
    }
}

float UMingRTSResourceManager::CalculateResourcePriority(ERTSResourceType ResourceType) const
{
    // 基於多種因素計算資源優先級
    float Priority = 0.5f; // 基礎優先級
    
    // 基於當前庫存
    if (Resources.Contains(ResourceType))
    {
        const FRTSResourceData& ResourceData = Resources[ResourceType];
        float StockRatio = ResourceData.CurrentAmount / ResourceData.MaxAmount;
        
        // 庫存越低，優先級越高
        if (StockRatio < 0.2f)
        {
            Priority += 0.4f;
        }
        else if (StockRatio < 0.5f)
        {
            Priority += 0.2f;
        }
    }
    
    // 基於資源類型重要性
    switch (ResourceType)
    {
    case ERTSResourceType::Food:
        Priority += 0.3f; // 糧食最重要
        break;
    case ERTSResourceType::Gold:
        Priority += 0.25f; // 黃金很重要
        break;
    case ERTSResourceType::Wood:
    case ERTSResourceType::Stone:
        Priority += 0.15f; // 基礎建材中等
        break;
    case ERTSResourceType::Iron:
        Priority += 0.1f; // 鐵礦較重要
        break;
    default:
        Priority += 0.05f; // 其他資源優先級較低
        break;
    }
    
    return FMath::Clamp(Priority, 0.0f, 1.0f);
}

void UMingRTSResourceManager::AutoBalanceResources()
{
    if (!bEnableAutoBalancing)
    {
        return;
    }
    
    // 自動平衡所有資源
    for (auto& ResourcePair : Resources)
    {
        ERTSResourceType ResourceType = ResourcePair.Key;
        FRTSResourceData& ResourceData = ResourcePair.Value;
        
        float CurrentRatio = ResourceData.CurrentAmount / ResourceData.MaxAmount;
        
        // 自動調整以達到平衡
        if (CurrentRatio < 0.4f)
        {
            // 庫存不足，提高生產
            ResourceData.ProductionRate *= 1.15f;
            ResourceData.ConsumptionRate *= 0.9f;
        }
        else if (CurrentRatio > 0.9f)
        {
            // 庫存過多，降低生產
            ResourceData.ProductionRate *= 0.85f;
        }
    }
}
