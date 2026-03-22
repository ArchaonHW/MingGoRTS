#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSResourceManager.generated.h"

UENUM(BlueprintType)
enum class ERTSResourceType : uint8
{
    Gold,           // 黃金
    Wood,           // 木材
    Stone,          // 石材
    Iron,           // 鐵礦
    Food,           // 糧食
    Coal,           // 煤炭
    Oil,            // 石油
    Uranium,        // 鈾礦
    Crystal,        // 水晶
    Silk            // 絲綢
};

UENUM(BlueprintType)
enum class ERTSResourceSource : uint8
{
    Mine,           // 礦山
    Forest,         // 森林
    Quarry,         // 採石場
    Farm,           // 農場
    OilWell,        // 油井
    CrystalCave,    // 水晶洞
    SilkFarm,       // 絲綢農場
    TradingPost     // 貿易站
};

UENUM(BlueprintType)
enum class ERTSResourceState : uint8
{
    Available,      // 可用
    Depleted,       // 耗盡
    Occupied,       // 被佔領
    UnderConstruction, // 建設中
    Damaged,        // 損壞
    Repairing        // 修復中
};

USTRUCT(BlueprintType)
struct FRTSResourceData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    ERTSResourceType ResourceType;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    float CurrentAmount;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    float MaxAmount;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    float ProductionRate;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    float ConsumptionRate;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    ERTSResourceState ResourceState;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    FString ResourceName;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Data")
    FString Description;

    FRTSResourceData()
    {
        ResourceType = ERTSResourceType::Gold;
        CurrentAmount = 0.0f;
        MaxAmount = 1000.0f;
        ProductionRate = 10.0f;
        ConsumptionRate = 0.0f;
        ResourceState = ERTSResourceState::Available;
        ResourceName = TEXT("");
        Description = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FRTSResourceSourceData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    FString SourceID;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    FString SourceName;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    ERTSResourceType ResourceType;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    ERTSResourceSource SourceType;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    FVector Location;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    float Yield;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    float MaxYield;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    ERTSResourceState SourceState;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    bool bIsOperational;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    TArray<FString> RequiredWorkers;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    int32 CurrentWorkers;

    UPROPERTY(BlueprintReadOnly, Category = "Resource Source Data")
    int32 MaxWorkers;

    FRTSResourceSourceData()
    {
        SourceID = TEXT("");
        SourceName = TEXT("");
        ResourceType = ERTSResourceType::Gold;
        SourceType = ERTSResourceSource::Mine;
        Location = FVector::ZeroVector;
        Yield = 100.0f;
        MaxYield = 1000.0f;
        SourceState = ERTSResourceState::Available;
        bIsOperational = false;
        CurrentWorkers = 0;
        MaxWorkers = 5;
    }
};

USTRUCT(BlueprintType)
struct FRTSTradeData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Trade Data")
    ERTSResourceType OfferResource;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Data")
    ERTSResourceType RequestResource;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Data")
    float OfferAmount;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Data")
    float RequestAmount;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Data")
    float ExchangeRate;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Data")
    bool bIsAvailable;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Data")
    FString TradingPartner;

    FRTSTradeData()
    {
        OfferResource = ERTSResourceType::Gold;
        RequestResource = ERTSResourceType::Wood;
        OfferAmount = 100.0f;
        RequestAmount = 50.0f;
        ExchangeRate = 2.0f;
        bIsAvailable = true;
        TradingPartner = TEXT("");
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceChanged, ERTSResourceType, ResourceType, float, NewAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceSourceStateChanged, const FString&, SourceID, ERTSResourceState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceDepleted, ERTSResourceType, ResourceType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTradeCompleted, const FRTSTradeData&, TradeData, bool, bSuccess);

/**
 * RTS資源管理器
 * 管理所有資源相關的邏輯
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSResourceManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSResourceManager();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void InitializeResourceManager();

    // 資源管理
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void AddResource(ERTSResourceType ResourceType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void RemoveResource(ERTSResourceType ResourceType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    bool HasEnoughResource(ERTSResourceType ResourceType, float Amount) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float GetResourceAmount(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float GetMaxResourceAmount(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void SetMaxResourceAmount(ERTSResourceType ResourceType, float MaxAmount);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void SetResourceProductionRate(ERTSResourceType ResourceType, float Rate);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void SetResourceConsumptionRate(ERTSResourceType ResourceType, float Rate);

    // 資源源管理
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void AddResourceSource(const FRTSResourceSourceData& SourceData);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void RemoveResourceSource(const FString& SourceID);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    FRTSResourceSourceData GetResourceSource(const FString& SourceID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    TArray<FRTSResourceSourceData> GetAllResourceSources() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    TArray<FRTSResourceSourceData> GetResourceSourcesByType(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void OperateResourceSource(const FString& SourceID, bool bOperate);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    bool IsResourceSourceOperational(const FString& SourceID) const;

    // 資源生產
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void UpdateResourceProduction(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float CalculateTotalProduction() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float CalculateTotalConsumption() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void OptimizeResourceAllocation();

    // 資源交易
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    bool ExecuteTrade(const FRTSTradeData& TradeData);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void AddTradeOffer(const FRTSTradeData& TradeData);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void RemoveTradeOffer(const FString& TradeID);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    TArray<FRTSTradeData> GetAvailableTrades() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float CalculateExchangeRate(ERTSResourceType OfferResource, ERTSResourceType RequestResource) const;

    // 資源分配
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void AllocateResource(ERTSResourceType ResourceType, float Amount, const FString& Purpose);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float GetAllocatedResource(ERTSResourceType ResourceType, const FString& Purpose) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void ClearResourceAllocation(const FString& Purpose);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    TMap<FString, float> GetAllAllocations(ERTSResourceType ResourceType) const;

    // 資源預測
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float PredictResourceAmount(ERTSResourceType ResourceType, float TimeInSeconds) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    bool WillResourceDeplete(ERTSResourceType ResourceType, float TimeInSeconds) const;

    // 資源管理系統優化
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void OptimizeResourceManagement();

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void BalanceResourceDistribution();

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void EnhanceResourceEfficiency();

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void ImplementSmartAllocation();

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float GetTimeToDepletion(ERTSResourceType ResourceType) const;

    // 資源效率
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    float GetResourceEfficiency(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void ImproveResourceEfficiency(ERTSResourceType ResourceType, float EfficiencyBonus);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void SetResourceEfficiency(ERTSResourceType ResourceType, float Efficiency);

    // 統計信息
    UFUNCTION(BlueprintPure, Category = "RTS Resource Manager")
    int32 GetTotalResourceSourceCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Resource Manager")
    int32 GetOperationalResourceSourceCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Resource Manager")
    float GetTotalResourceValue() const;

    UFUNCTION(BlueprintPure, Category = "RTS Resource Manager")
    TArray<ERTSResourceType> GetAvailableResourceTypes() const;

    // 資源事件
    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void OnResourceSourceDestroyed(const FString& SourceID);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void OnResourceSourceCaptured(const FString& SourceID, const FString& NewOwner);

    UFUNCTION(BlueprintCallable, Category = "RTS Resource Manager")
    void OnWorkerAssigned(const FString& SourceID, int32 WorkerCount);

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "RTS Resource Events")
    FOnResourceChanged OnResourceChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Resource Events")
    FOnResourceSourceStateChanged OnResourceSourceStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Resource Events")
    FOnResourceDepleted OnResourceDepleted;

    UPROPERTY(BlueprintAssignable, Category = "RTS Resource Events")
    FOnTradeCompleted OnTradeCompleted;

protected:
    // 資源數據
    UPROPERTY()
    TMap<ERTSResourceType, FRTSResourceData> Resources;

    UPROPERTY()
    TMap<FString, FRTSResourceSourceData> ResourceSources;

    UPROPERTY()
    TArray<FRTSTradeData> TradeOffers;

    // 注意：TMap<TMap> 不支持 UPROPERTY
    TMap<ERTSResourceType, TMap<FString, float>> ResourceAllocations;

    UPROPERTY()
    TMap<ERTSResourceType, float> ResourceEfficiencies;

    // 設置
    UPROPERTY(BlueprintReadWrite, Category = "Resource Settings")
    float BaseProductionMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Resource Settings")
    float BaseConsumptionMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Resource Settings")
    float TradeFee = 0.1f;

    UPROPERTY(BlueprintReadWrite, Category = "Resource Settings")
    bool bAutoTrade = true;

    UPROPERTY(BlueprintReadWrite, Category = "Resource Settings")
    bool bAutoAllocate = true;

    // 優化參數
    UPROPERTY(BlueprintReadWrite, Category = "Optimization Settings")
    float OptimizationThreshold = 0.8f;

    UPROPERTY(BlueprintReadWrite, Category = "Optimization Settings")
    float EfficiencyBonus = 1.2f;

    UPROPERTY(BlueprintReadWrite, Category = "Optimization Settings")
    bool bEnableSmartAllocation = true;

    UPROPERTY(BlueprintReadWrite, Category = "Optimization Settings")
    bool bEnableAutoBalancing = true;

    // 狀態
    UPROPERTY()
    bool bIsInitialized;

    // 內部函數
    void InitializeDefaultResources();
    void InitializeResourceEfficiencies();
    void ProcessResourceSource(FRTSResourceSourceData& Source, float DeltaTime);
    void UpdateResourceState(ERTSResourceType ResourceType);
    FString GenerateResourceSourceID(ERTSResourceSource SourceType) const;

    // 優化內部函數
    void OptimizeProductionRates();
    void BalanceResourceConsumption();
    void CalculateOptimalAllocation();
    void UpdateEfficiencyMetrics();
    float CalculateResourcePriority(ERTSResourceType ResourceType) const;
    void AutoBalanceResources();
    FString GetResourceName(ERTSResourceType ResourceType) const;
    FString GetResourceSourceName(ERTSResourceSource SourceType) const;
    float GetResourceValue(ERTSResourceType ResourceType) const;
    bool ValidateTrade(const FRTSTradeData& TradeData) const;

private:
    // 輔助函數
    void NotifyResourceChanged(ERTSResourceType ResourceType, float NewAmount);
    void NotifyResourceSourceStateChanged(const FString& SourceID, ERTSResourceState NewState);
    void NotifyResourceDepleted(ERTSResourceType ResourceType);
    void NotifyTradeCompleted(const FRTSTradeData& TradeData, bool bSuccess);
};
