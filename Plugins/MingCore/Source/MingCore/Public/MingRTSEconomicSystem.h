#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEconomicSystem.generated.h"

class UMingRTSResourceManager;
class UMingRTSBuildingSystem;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSEconomicPhase : uint8
{
    Growth,         // ??????
    Stagnation,     // ??????
    Recession,      // ?I?h??
    Depression,     // ??????
    Recovery,       // ?_?d??
    Boom            // ?c?a??
};

UENUM(BlueprintType)
enum class ERTSTradeRoute : uint8
{
    Land,           // X??
    Sea,            // ????
    Air,            // ??X
    Rail,           // X??
    Digital         // X?TX
};

UENUM(BlueprintType)
enum class ERTSMarketCondition : uint8
{
    BullMarket,     // X
    BearMarket,     // X
    Stable,         // iX
    Volatile        // ?iX
};

USTRUCT(BlueprintType)
struct FRTSEconomicData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float GDP;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float InflationRate;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float UnemploymentRate;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float InterestRate;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float TradeBalance;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float ConsumerConfidence;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float IndustrialProduction;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    ERTSEconomicPhase CurrentPhase;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    ERTSMarketCondition MarketCondition;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    float EconomicGrowthRate;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    TMap<ERTSResourceType, float> ResourcePrices;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    TMap<ERTSResourceType, float> ResourceDemands;

    UPROPERTY(BlueprintReadOnly, Category = "Economic Data")
    TMap<ERTSResourceType, float> ResourceSupplies;

    FRTSEconomicData()
    {
        GDP = 10000.0f;
        InflationRate = 0.02f;
        UnemploymentRate = 0.05f;
        InterestRate = 0.03f;
        TradeBalance = 0.0f;
        ConsumerConfidence = 0.7f;
        IndustrialProduction = 100.0f;
        CurrentPhase = ERTSEconomicPhase::Growth;
        MarketCondition = ERTSMarketCondition::Stable;
        EconomicGrowthRate = 0.03f;
    }
};

USTRUCT(BlueprintType)
struct FRTSTradeRouteData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    FString RouteID;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    FString Origin;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    FString Destination;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    ERTSTradeRoute RouteType;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    float Distance;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    float TransportCost;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    float Capacity;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    float CurrentLoad;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    TArray<ERTSResourceType> TradeGoods;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    TArray<float> TradeVolumes;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly, Category = "Trade Route Data")
    float Efficiency;

    FRTSTradeRouteData()
    {
        RouteID = TEXT(""};
        Origin = TEXT(""};
        Destination = TEXT(""};
        RouteType = ERTSTradeRoute::Land;
        Distance = 0.0f;
        TransportCost = 10.0f;
        Capacity = 1000.0f;
        CurrentLoad = 0.0f;
        bIsActive = true;
        Efficiency = 1.0f;
    }
};

USTRUCT(BlueprintType)
struct FRTSMarketData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    ERTSResourceType ResourceType;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float CurrentPrice;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float BasePrice;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float PriceHistory;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float Demand;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float Supply;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float MarketShare;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float Volatility;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    bool bIsTrendingUp;

    UPROPERTY(BlueprintReadOnly, Category = "Market Data")
    float PriceChange24h;

    FRTSMarketData()
    {
        ResourceType = ERTSResourceType::Gold;
        CurrentPrice = 100.0f;
        BasePrice = 100.0f;
        PriceHistory = 0.0f;
        Demand = 50.0f;
        Supply = 50.0f;
        MarketShare = 0.1f;
        Volatility = 0.1f;
        bIsTrendingUp = false;
        PriceChange24h = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEconomicPhaseChanged, ERTSEconomicPhase, OldPhase, ERTSEconomicPhase, NewPhase};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMarketConditionChanged, ERTSMarketCondition, NewCondition};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourcePriceChanged, ERTSResourceType, ResourceType, float, NewPrice};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTradeRouteCompleted, const FString&, RouteID, float, Profit};

/**
 * RTS?gX?t??
 * ??X???X??
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSEconomicSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEconomicSystem();

    // X    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void InitializeEconomicSystem(UMingRTSResourceManager* InResourceManager, UMingRTSBuildingSystem* InBuildingSystem, UMingRTSUnitManager* InUnitManager};

    // ?gX??X
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void UpdateEconomicData(float DeltaTime};

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    FRTSEconomicData GetEconomicData() const { return EconomicData; }

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void SetEconomicPhase(ERTSEconomicPhase NewPhase};

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    ERTSEconomicPhase GetCurrentEconomicPhase() const { return EconomicData.CurrentPhase; }

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void SetMarketCondition(ERTSMarketCondition NewCondition};

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    ERTSMarketCondition GetMarketCondition() const { return EconomicData.MarketCondition; }

    // ??X???X
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void UpdateResourcePrices();

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetResourcePrice(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void SetResourcePrice(ERTSResourceType ResourceType, float Price};

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetResourceDemand(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetResourceSupply(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void UpdateResourceDemand(ERTSResourceType ResourceType, float Demand};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void UpdateResourceSupply(ERTSResourceType ResourceType, float Supply};

    // ?TX??X??X
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    FString CreateTradeRoute(const FString& Origin, const FString& Destination, ERTSTradeRoute RouteType};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    bool DestroyTradeRoute(const FString& RouteID) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    FRTSTradeRouteData GetTradeRoute(const FString& RouteID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    TArray<FRTSTradeRouteData> GetAllTradeRoutes() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    bool ExecuteTrade(const FString& RouteID, ERTSResourceType ResourceType, float Volume};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    float CalculateTradeProfit(const FString& RouteID, ERTSResourceType ResourceType, float Volume) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void OptimizeTradeRoutes();

    // ????X
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    FRTSMarketData GetMarketData(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    TArray<FRTSMarketData> GetAllMarketData() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void AnalyzeMarketTrends();

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    bool IsMarketBullish() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    bool IsMarketBearish() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    float GetMarketVolatility() const;

    // ?gX
    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetGDP() const { return EconomicData.GDP; }

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetInflationRate() const { return EconomicData.InflationRate; }

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetUnemploymentRate() const { return EconomicData.UnemploymentRate; }

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetInterestRate() const { return EconomicData.InterestRate; }

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetTradeBalance() const { return EconomicData.TradeBalance; }

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetConsumerConfidence() const { return EconomicData.ConsumerConfidence; }

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetIndustrialProduction() const { return EconomicData.IndustrialProduction; }

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetEconomicGrowthRate() const { return EconomicData.EconomicGrowthRate; }

    // ?gX
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void SetInterestRate(float Rate};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void AdjustInflationRate(float Adjustment};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void ImplementEconomicStimulus(float StimulusAmount};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void ImplementAusterityMeasures(float ReductionAmount};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void RegulateMarket(ERTSResourceType ResourceType, float RegulationLevel};

    // ?gX??
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    float PredictGDP(float TimeInMonths) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    ERTSEconomicPhase PredictEconomicPhase(float TimeInMonths) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    float PredictResourcePrice(ERTSResourceType ResourceType, float TimeInMonths) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    bool PredictRecession(float TimeInMonths) const;

    // ?gX???
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void TriggerEconomicEvent(const FString& EventName, float Impact};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void HandleSupplyShock(ERTSResourceType ResourceType, float ShockMagnitude};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void HandleDemandShock(ERTSResourceType ResourceType, float ShockMagnitude};

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void HandleFinancialCrisis(float CrisisSeverity};

    // ??X?H??
    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    int32 GetActiveTradeRouteCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetTotalTradeVolume() const;

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetTotalTradeProfit() const;

    UFUNCTION(BlueprintPure, Category = "RTS Economic System")
    float GetMarketCapitalization() const;

    // ?gX?`???????
    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void BalanceEconomicCycle();

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void StabilizeMarket();

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void OptimizeTradeRoutes();

    UFUNCTION(BlueprintCallable, Category = "RTS Economic System")
    void ImplementSmartEconomicManagement();

    // ???
    UPROPERTY(BlueprintAssignable, Category = "RTS Economic Events")
    FOnEconomicPhaseChanged OnEconomicPhaseChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Economic Events")
    FOnMarketConditionChanged OnMarketConditionChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Economic Events")
    FOnResourcePriceChanged OnResourcePriceChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Economic Events")
    FOnTradeRouteCompleted OnTradeRouteCompleted;

protected:
    // ?t?£V??
    UPROPERTY()
    TObjectPtr<UMingRTSResourceManager> ResourceManager;

    UPROPERTY()
    TObjectPtr<UMingRTSBuildingSystem> BuildingSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // ?gX
    UPROPERTY()
    FRTSEconomicData EconomicData;

    UPROPERTY()
    TMap<ERTSResourceType, FRTSMarketData> MarketData;

    UPROPERTY()
    TMap<FString, FRTSTradeRouteData> TradeRoutes;

    // ?]?m
    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    float EconomicUpdateInterval = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    float PriceUpdateInterval = 5.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    float TradeUpdateInterval = 2.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    float BaseInflationRate = 0.02f;

    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    float BaseInterestRate = 0.03f;

    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    float BaseUnemploymentRate = 0.05f;

    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    bool bAutoAdjustPrices = true;

    UPROPERTY(BlueprintReadWrite, Category = "Economic Settings")
    bool bAutoOptimizeTrade = true;

    // ??????X??
    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    float EconomicStabilityThreshold = 0.8f;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    float MarketStabilityBonus = 1.1f;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    bool bEnableSmartManagement = true;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    bool bEnableAutoBalancing = true;

    // X    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    float LastEconomicUpdate;

    UPROPERTY()
    float LastPriceUpdate;

    UPROPERTY()
    float LastTradeUpdate;

    UPROPERTY()
    float TotalTradeProfit;

    // X??X??
    void InitializeEconomicData();
    void InitializeMarketData();
    void ProcessEconomicCycle(float DeltaTime};
    void ProcessPriceFluctuations(float DeltaTime};
    void ProcessTradeActivity(float DeltaTime};
    void UpdateGDP(float DeltaTime};
    void UpdateInflation(float DeltaTime};
    void UpdateUnemployment(float DeltaTime};
    void UpdateTradeBalance(float DeltaTime};
    void UpdateConsumerConfidence(float DeltaTime};
    void UpdateIndustrialProduction(float DeltaTime};
    void CalculateEconomicGrowth();
    void DetermineEconomicPhase();
    void DetermineMarketCondition();
    float CalculateResourcePrice(ERTSResourceType ResourceType) const;
    void UpdateMarketData(ERTSResourceType ResourceType};
    FString GenerateTradeRouteID() const;
    void ProcessTradeRoute(FRTSTradeRouteData& Route, float DeltaTime};
    void ApplyEconomicPolicies(float DeltaTime};
    void HandleRandomEvents(float DeltaTime};

    // ??????X??X??
    void OptimizeEconomicIndicators();
    void StabilizeMarketPrices();
    void BalanceTradeFlows();
    void CalculateOptimalEconomicPolicy();
    float CalculateEconomicStability() const;
    void AutoBalanceEconomy();
    void OptimizeResourceAllocation();

private:
    // ???UX??
    void NotifyEconomicPhaseChanged(ERTSEconomicPhase OldPhase, ERTSEconomicPhase NewPhase};
    void NotifyMarketConditionChanged(ERTSMarketCondition NewCondition};
    void NotifyResourcePriceChanged(ERTSResourceType ResourceType, float NewPrice};
    void NotifyTradeRouteCompleted(const FString& RouteID, float Profit};
};

