#include "MingRTSEconomicSystem.h"
#include "MingRTSResourceManager.h"
#include "MingRTSBuildingSystem.h"
#include "MingRTSUnitManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

UMingRTSEconomicSystem::UMingRTSEconomicSystem()
    : EconomicUpdateInterval(1.0f)
    , PriceUpdateInterval(5.0f)
    , TradeUpdateInterval(2.0f)
    , BaseInflationRate(0.02f)
    , BaseInterestRate(0.03f)
    , BaseUnemploymentRate(0.05f)
    , bAutoAdjustPrices(true)
    , bAutoOptimizeTrade(true)
    , bIsInitialized(false)
    , LastEconomicUpdate(0.0f)
    , LastPriceUpdate(0.0f)
    , LastTradeUpdate(0.0f)
    , TotalTradeProfit(0.0f)
{
}

void UMingRTSEconomicSystem::InitializeEconomicSystem(UMingRTSResourceManager* InResourceManager, UMingRTSBuildingSystem* InBuildingSystem, UMingRTSUnitManager* InUnitManager)
{
    ResourceManager = InResourceManager;
    BuildingSystem = InBuildingSystem;
    UnitManager = InUnitManager;
    
    if (ResourceManager && BuildingSystem && UnitManager)
    {
        InitializeEconomicData();
        InitializeMarketData();
        bIsInitialized = true;
        
        UE_LOG(LogTemp, Log, TEXT("RTS Economic System initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RTS Economic System initialization failed"));
    }
}

void UMingRTSEconomicSystem::UpdateEconomicData(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    
    // 經濟更新
    if (CurrentTime - LastEconomicUpdate >= EconomicUpdateInterval)
    {
        ProcessEconomicCycle(DeltaTime);
        LastEconomicUpdate = CurrentTime;
    }
    
    // 價格更新
    if (CurrentTime - LastPriceUpdate >= PriceUpdateInterval)
    {
        ProcessPriceFluctuations(DeltaTime);
        LastPriceUpdate = CurrentTime;
    }
    
    // 貿易更新
    if (CurrentTime - LastTradeUpdate >= TradeUpdateInterval)
    {
        ProcessTradeActivity(DeltaTime);
        LastTradeUpdate = CurrentTime;
    }
    
    // 處理隨機事件
    HandleRandomEvents(DeltaTime);
}

void UMingRTSEconomicSystem::SetEconomicPhase(ERTSEconomicPhase NewPhase)
{
    if (EconomicData.CurrentPhase != NewPhase)
    {
        ERTSEconomicPhase OldPhase = EconomicData.CurrentPhase;
        EconomicData.CurrentPhase = NewPhase;
        
        // 根據經濟階段調整參數
        switch (NewPhase)
        {
            case ERTSEconomicPhase::Growth:
                EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate + 0.02f, 0.0f, 0.1f);
                EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate - 0.01f, 0.0f, 0.2f);
                break;
            case ERTSEconomicPhase::Stagnation:
                EconomicData.EconomicGrowthRate = 0.0f;
                break;
            case ERTSEconomicPhase::Recession:
                EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate - 0.03f, -0.1f, 0.0f);
                EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate + 0.02f, 0.0f, 0.3f);
                break;
            case ERTSEconomicPhase::Depression:
                EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate - 0.05f, -0.15f, 0.0f);
                EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate + 0.05f, 0.0f, 0.4f);
                EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence - 0.2f, 0.0f, 1.0f);
                break;
            case ERTSEconomicPhase::Recovery:
                EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate + 0.01f, 0.0f, 0.08f);
                EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate - 0.01f, 0.0f, 0.25f);
                break;
            case ERTSEconomicPhase::Boom:
                EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate + 0.04f, 0.02f, 0.12f);
                EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate - 0.02f, 0.0f, 0.15f);
                EconomicData.InflationRate = FMath::Clamp(EconomicData.InflationRate + 0.01f, 0.0f, 0.1f);
                break;
        }
        
        NotifyEconomicPhaseChanged(OldPhase, NewPhase);
        
        UE_LOG(LogTemp, Log, TEXT("Economic phase changed to: %d"), (int32)NewPhase);
    }
}

void UMingRTSEconomicSystem::SetMarketCondition(ERTSMarketCondition NewCondition)
{
    if (EconomicData.MarketCondition != NewCondition)
    {
        EconomicData.MarketCondition = NewCondition;
        
        // 根據市場條件調整參數
        switch (NewCondition)
        {
            case ERTSMarketCondition::BullMarket:
                EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence + 0.1f, 0.0f, 1.0f);
                break;
            case ERTSMarketCondition::BearMarket:
                EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence - 0.1f, 0.0f, 1.0f);
                break;
            case ERTSMarketCondition::Stable:
                // 穩定狀態，無需調整
                break;
            case ERTSMarketCondition::Volatile:
                // 波動狀態，增加價格波動性
                for (auto& MarketPair : MarketData)
                {
                    MarketPair.Value.Volatility = FMath::Clamp(MarketPair.Value.Volatility + 0.05f, 0.0f, 0.5f);
                }
                break;
        }
        
        NotifyMarketConditionChanged(NewCondition);
        
        UE_LOG(LogTemp, Log, TEXT("Market condition changed to: %d"), (int32)NewCondition);
    }
}

void UMingRTSEconomicSystem::UpdateResourcePrices()
{
    if (!bAutoAdjustPrices)
    {
        return;
    }

    for (auto& MarketPair : MarketData)
    {
        ERTSResourceType ResourceType = MarketPair.Key;
        FRTSMarketData& MarketData = MarketPair.Value;
        
        float NewPrice = CalculateResourcePrice(ResourceType);
        float OldPrice = MarketData.CurrentPrice;
        
        MarketData.CurrentPrice = NewPrice;
        MarketData.PriceHistory = OldPrice;
        MarketData.PriceChange24h = ((NewPrice - OldPrice) / OldPrice) * 100.0f;
        MarketData.bIsTrendingUp = NewPrice > OldPrice;
        
        if (FMath::Abs(NewPrice - OldPrice) > 0.01f)
        {
            NotifyResourcePriceChanged(ResourceType, NewPrice);
        }
    }
}

float UMingRTSEconomicSystem::GetResourcePrice(ERTSResourceType ResourceType) const
{
    if (MarketData.Contains(ResourceType))
    {
        return MarketData[ResourceType].CurrentPrice;
    }
    
    return 100.0f; // 默認價格
}

void UMingRTSEconomicSystem::SetResourcePrice(ERTSResourceType ResourceType, float Price)
{
    if (MarketData.Contains(ResourceType))
    {
        float OldPrice = MarketData[ResourceType].CurrentPrice;
        MarketData[ResourceType].CurrentPrice = FMath::Max(0.1f, Price);
        MarketData.PriceHistory = OldPrice;
        MarketData.PriceChange24h = ((Price - OldPrice) / OldPrice) * 100.0f;
        MarketData.bIsTrendingUp = Price > OldPrice;
        
        NotifyResourcePriceChanged(ResourceType, Price);
    }
}

float UMingRTSEconomicSystem::GetResourceDemand(ERTSResourceType ResourceType) const
{
    if (MarketData.Contains(ResourceType))
    {
        return MarketData[ResourceType].Demand;
    }
    
    return 50.0f; // 默認需求
}

float UMingRTSEconomicSystem::GetResourceSupply(ERTSResourceType ResourceType) const
{
    if (MarketData.Contains(ResourceType))
    {
        return MarketData[ResourceType].Supply;
    }
    
    return 50.0f; // 默認供應
}

void UMingRTSEconomicSystem::UpdateResourceDemand(ERTSResourceType ResourceType, float Demand)
{
    if (MarketData.Contains(ResourceType))
    {
        MarketData[ResourceType].Demand = FMath::Max(0.0f, Demand);
    }
}

void UMingRTSEconomicSystem::UpdateResourceSupply(ERTSResourceType ResourceType, float Supply)
{
    if (MarketData.Contains(ResourceType))
    {
        MarketData[ResourceType].Supply = FMath::Max(0.0f, Supply);
    }
}

FString UMingRTSEconomicSystem::CreateTradeRoute(const FString& Origin, const FString& Destination, ERTSTradeRoute RouteType)
{
    FRTSTradeRouteData Route;
    Route.RouteID = GenerateTradeRouteID();
    Route.Origin = Origin;
    Route.Destination = Destination;
    Route.RouteType = RouteType;
    Route.Distance = 1000.0f; // 默認距離
    Route.bIsActive = true;
    
    // 根據路線類型設置成本和容量
    switch (RouteType)
    {
        case ERTSTradeRoute::Land:
            Route.TransportCost = Route.Distance * 0.01f;
            Route.Capacity = 500.0f;
            break;
        case ERTSTradeRoute::Sea:
            Route.TransportCost = Route.Distance * 0.005f;
            Route.Capacity = 2000.0f;
            break;
        case ERTSTradeRoute::Air:
            Route.TransportCost = Route.Distance * 0.02f;
            Route.Capacity = 200.0f;
            break;
        case ERTSTradeRoute::Rail:
            Route.TransportCost = Route.Distance * 0.008f;
            Route.Capacity = 1000.0f;
            break;
        case ERTSTradeRoute::Digital:
            Route.TransportCost = Route.Distance * 0.001f;
            Route.Capacity = 5000.0f;
            break;
    }
    
    TradeRoutes.Add(Route.RouteID, Route);
    
    UE_LOG(LogTemp, Log, TEXT("Created trade route: %s from %s to %s"), *Route.RouteID, *Origin, *Destination);
    
    return Route.RouteID;
}

bool UMingRTSEconomicSystem::DestroyTradeRoute(const FString& RouteID)
{
    if (TradeRoutes.Contains(RouteID))
    {
        TradeRoutes.Remove(RouteID);
        UE_LOG(LogTemp, Log, TEXT("Destroyed trade route: %s"), *RouteID);
        return true;
    }
    
    return false;
}

FRTSTradeRouteData UMingRTSEconomicSystem::GetTradeRoute(const FString& RouteID) const
{
    if (TradeRoutes.Contains(RouteID))
    {
        return TradeRoutes[RouteID];
    }
    
    return FRTSTradeRouteData();
}

TArray<FRTSTradeRouteData> UMingRTSEconomicSystem::GetAllTradeRoutes() const
{
    TArray<FRTSTradeRouteData> AllRoutes;
    
    for (const auto& RoutePair : TradeRoutes)
    {
        AllRoutes.Add(RoutePair.Value);
    }
    
    return AllRoutes;
}

bool UMingRTSEconomicSystem::ExecuteTrade(const FString& RouteID, ERTSResourceType ResourceType, float Volume)
{
    if (!TradeRoutes.Contains(RouteID) || !ResourceManager)
    {
        return false;
    }

    FRTSTradeRouteData& Route = TradeRoutes[RouteID];
    
    // 檢查容量
    if (Route.CurrentLoad + Volume > Route.Capacity)
    {
        return false;
    }
    
    // 檢查是否有足夠的資源
    if (!ResourceManager->HasEnoughResource(ResourceType, Volume))
    {
        return false;
    }
    
    // 計算利潤
    float Profit = CalculateTradeProfit(RouteID, ResourceType, Volume);
    
    if (Profit > 0)
    {
        // 執行貿易
        ResourceManager->RemoveResource(ResourceType, Volume);
        Route.CurrentLoad += Volume;
        
        // 添加到貿易商品列表
        Route.TradeGoods.Add(ResourceType);
        Route.TradeVolumes.Add(Volume);
        
        TotalTradeProfit += Profit;
        
        NotifyTradeRouteCompleted(RouteID, Profit);
        
        UE_LOG(LogTemp, Log, TEXT("Executed trade on route %s: %.1f %s, profit: %.1f"), 
               *RouteID, Volume, *GetResourceName(ResourceType), Profit);
        
        return true;
    }
    
    return false;
}

float UMingRTSEconomicSystem::CalculateTradeProfit(const FString& RouteID, ERTSResourceType ResourceType, float Volume) const
{
    if (!TradeRoutes.Contains(RouteID))
    {
        return 0.0f;
    }

    const FRTSTradeRouteData& Route = TradeRoutes[RouteID];
    float ResourcePrice = GetResourcePrice(ResourceType);
    
    // 計算收入 (假設目的地價格高10%)
    float Revenue = Volume * ResourcePrice * 1.1f;
    
    // 計算成本
    float Cost = Volume * ResourcePrice + Route.TransportCost * Volume;
    
    return Revenue - Cost;
}

void UMingRTSEconomicSystem::OptimizeTradeRoutes()
{
    if (!bAutoOptimizeTrade)
    {
        return;
    }

    // 簡單的貿易路線優化
    for (auto& RoutePair : TradeRoutes)
    {
        FRTSTradeRouteData& Route = RoutePair.Value;
        
        // 計算路線效率
        float TotalVolume = 0.0f;
        for (float Volume : Route.TradeVolumes)
        {
            TotalVolume += Volume;
        }
        
        Route.Efficiency = Route.Capacity > 0.0f ? (TotalVolume / Route.Capacity) : 0.0f;
        
        // 如果效率太低，建議關閉路線
        if (Route.Efficiency < 0.3f)
        {
            Route.bIsActive = false;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Optimized trade routes"));
}

FRTSMarketData UMingRTSEconomicSystem::GetMarketData(ERTSResourceType ResourceType) const
{
    if (MarketData.Contains(ResourceType))
    {
        return MarketData[ResourceType];
    }
    
    return FRTSMarketData();
}

TArray<FRTSMarketData> UMingRTSEconomicSystem::GetAllMarketData() const
{
    TArray<FRTSMarketData> AllMarketData;
    
    for (const auto& MarketPair : MarketData)
    {
        AllMarketData.Add(MarketPair.Value);
    }
    
    return AllMarketData;
}

void UMingRTSEconomicSystem::AnalyzeMarketTrends()
{
    for (auto& MarketPair : MarketData)
    {
        FRTSMarketData& Market = MarketPair.Value;
        
        // 計算趨勢
        if (Market.PriceHistory > 0.0f)
        {
            float PriceChange = (Market.CurrentPrice - Market.PriceHistory) / Market.PriceHistory;
            Market.bIsTrendingUp = PriceChange > 0.01f;
        }
        
        // 計算波動性
        float DemandSupplyRatio = Market.Supply > 0.0f ? (Market.Demand / Market.Supply) : 1.0f;
        Market.Volatility = FMath::Abs(DemandSupplyRatio - 1.0f) * 0.1f;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Analyzed market trends"));
}

bool UMingRTSEconomicSystem::IsMarketBullish() const
{
    return EconomicData.MarketCondition == ERTSMarketCondition::BullMarket;
}

bool UMingRTSEconomicSystem::IsMarketBearish() const
{
    return EconomicData.MarketCondition == ERTSMarketCondition::BearMarket;
}

float UMingRTSEconomicSystem::GetMarketVolatility() const
{
    float TotalVolatility = 0.0f;
    int32 MarketCount = 0;
    
    for (const auto& MarketPair : MarketData)
    {
        TotalVolatility += MarketPair.Value.Volatility;
        MarketCount++;
    }
    
    return MarketCount > 0 ? (TotalVolatility / MarketCount) : 0.0f;
}

void UMingRTSEconomicSystem::SetInterestRate(float Rate)
{
    EconomicData.InterestRate = FMath::Clamp(Rate, 0.0f, 0.2f);
    
    // 利率影響經濟
    if (EconomicData.InterestRate > BaseInterestRate)
    {
        EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate - 0.01f, -0.1f, 0.1f);
    }
    else
    {
        EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate + 0.01f, -0.1f, 0.1f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Set interest rate to %.3f"), EconomicData.InterestRate);
}

void UMingRTSEconomicSystem::AdjustInflationRate(float Adjustment)
{
    EconomicData.InflationRate = FMath::Clamp(EconomicData.InflationRate + Adjustment, 0.0f, 0.2f);
    
    // 通脹影響消費者信心
    if (EconomicData.InflationRate > BaseInflationRate * 2.0f)
    {
        EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence - 0.05f, 0.0f, 1.0f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Adjusted inflation rate by %.3f to %.3f"), Adjustment, EconomicData.InflationRate);
}

void UMingRTSEconomicSystem::ImplementEconomicStimulus(float StimulusAmount)
{
    // 經濟刺激措施
    EconomicData.GDP += StimulusAmount;
    EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate + 0.02f, -0.1f, 0.15f);
    EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate - 0.01f, 0.0f, 0.3f);
    EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence + 0.1f, 0.0f, 1.0f);
    
    UE_LOG(LogTemp, Log, TEXT("Implemented economic stimulus: %.1f"), StimulusAmount);
}

void UMingRTSEconomicSystem::ImplementAusterityMeasures(float ReductionAmount)
{
    // 緊縮措施
    EconomicData.GDP = FMath::Max(0.0f, EconomicData.GDP - ReductionAmount);
    EconomicData.EconomicGrowthRate = FMath::Clamp(EconomicData.EconomicGrowthRate - 0.02f, -0.15f, 0.1f);
    EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate + 0.01f, 0.0f, 0.4f);
    EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence - 0.1f, 0.0f, 1.0f);
    
    UE_LOG(LogTemp, Log, TEXT("Implemented austerity measures: %.1f"), ReductionAmount);
}

void UMingRTSEconomicSystem::RegulateMarket(ERTSResourceType ResourceType, float RegulationLevel)
{
    if (MarketData.Contains(ResourceType))
    {
        FRTSMarketData& Market = MarketData[ResourceType];
        
        // 市場監管影響波動性
        Market.Volatility = FMath::Clamp(Market.Volatility * (1.0f - RegulationLevel), 0.0f, 0.5f);
        
        UE_LOG(LogTemp, Log, TEXT("Regulated market for %s with level %.2f"), *GetResourceName(ResourceType), RegulationLevel);
    }
}

float UMingRTSEconomicSystem::PredictGDP(float TimeInMonths) const
{
    // 簡單的GDP預測
    float MonthlyGrowth = EconomicData.EconomicGrowthRate / 12.0f;
    return EconomicData.GDP * FMath::Pow(1.0f + MonthlyGrowth, TimeInMonths);
}

ERTSEconomicPhase UMingRTSEconomicSystem::PredictEconomicPhase(float TimeInMonths) const
{
    // 基於當前趨勢預測經濟階段
    float PredictedGrowthRate = EconomicData.EconomicGrowthRate;
    
    if (PredictedGrowthRate > 0.05f)
    {
        return ERTSEconomicPhase::Boom;
    }
    else if (PredictedGrowthRate > 0.02f)
    {
        return ERTSEconomicPhase::Growth;
    }
    else if (PredictedGrowthRate > 0.0f)
    {
        return ERTSEconomicPhase::Stagnation;
    }
    else if (PredictedGrowthRate > -0.03f)
    {
        return ERTSEconomicPhase::Recession;
    }
    else
    {
        return ERTSEconomicPhase::Depression;
    }
}

float UMingRTSEconomicSystem::PredictResourcePrice(ERTSResourceType ResourceType, float TimeInMonths) const
{
    if (!MarketData.Contains(ResourceType))
    {
        return 100.0f;
    }

    const FRTSMarketData& Market = MarketData[ResourceType];
    
    // 簡單的價格預測
    float DemandSupplyRatio = Market.Supply > 0.0f ? (Market.Demand / Market.Supply) : 1.0f;
    float PriceChangeRate = (DemandSupplyRatio - 1.0f) * 0.1f; // 每月變化率
    
    return Market.CurrentPrice * FMath::Pow(1.0f + PriceChangeRate, TimeInMonths);
}

bool UMingRTSEconomicSystem::PredictRecession(float TimeInMonths) const
{
    ERTSEconomicPhase PredictedPhase = PredictEconomicPhase(TimeInMonths);
    return PredictedPhase == ERTSEconomicPhase::Recession || PredictedPhase == ERTSEconomicPhase::Depression;
}

void UMingRTSEconomicSystem::TriggerEconomicEvent(const FString& EventName, float Impact)
{
    // 經濟事件影響
    EconomicData.GDP *= (1.0f + Impact);
    EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence + Impact, 0.0f, 1.0f);
    
    // 根據影響調整經濟階段
    if (Impact > 0.1f)
    {
        SetEconomicPhase(ERTSEconomicPhase::Boom);
    }
    else if (Impact < -0.1f)
    {
        SetEconomicPhase(ERTSEconomicPhase::Recession);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Economic event triggered: %s with impact %.2f"), *EventName, Impact);
}

void UMingRTSEconomicSystem::HandleSupplyShock(ERTSResourceType ResourceType, float ShockMagnitude)
{
    if (MarketData.Contains(ResourceType))
    {
        FRTSMarketData& Market = MarketData[ResourceType];
        
        // 供給衝擊影響
        Market.Supply *= (1.0f - ShockMagnitude);
        Market.Volatility = FMath::Clamp(Market.Volatility + 0.1f, 0.0f, 0.5f);
        
        // 更新價格
        float NewPrice = CalculateResourcePrice(ResourceType);
        SetResourcePrice(ResourceType, NewPrice);
        
        UE_LOG(LogTemp, Warning, TEXT("Supply shock for %s: magnitude %.2f"), *GetResourceName(ResourceType), ShockMagnitude);
    }
}

void UMingRTSEconomicSystem::HandleDemandShock(ERTSResourceType ResourceType, float ShockMagnitude)
{
    if (MarketData.Contains(ResourceType))
    {
        FRTSMarketData& Market = MarketData[ResourceType];
        
        // 需求衝擊影響
        Market.Demand *= (1.0f + ShockMagnitude);
        Market.Volatility = FMath::Clamp(Market.Volatility + 0.1f, 0.0f, 0.5f);
        
        // 更新價格
        float NewPrice = CalculateResourcePrice(ResourceType);
        SetResourcePrice(ResourceType, NewPrice);
        
        UE_LOG(LogTemp, Warning, TEXT("Demand shock for %s: magnitude %.2f"), *GetResourceName(ResourceType), ShockMagnitude);
    }
}

void UMingRTSEconomicSystem::HandleFinancialCrisis(float CrisisSeverity)
{
    // 金融危機影響
    EconomicData.GDP *= (1.0f - CrisisSeverity);
    EconomicData.ConsumerConfidence = FMath::Clamp(EconomicData.ConsumerConfidence - CrisisSeverity * 0.5f, 0.0f, 1.0f);
    EconomicData.UnemploymentRate = FMath::Clamp(EconomicData.UnemploymentRate + CrisisSeverity * 0.1f, 0.0f, 0.4f);
    EconomicData.InterestRate = FMath::Clamp(EconomicData.InterestRate - CrisisSeverity * 0.01f, 0.0f, 0.2f);
    
    SetEconomicPhase(ERTSEconomicPhase::Recession);
    SetMarketCondition(ERTSMarketCondition::BearMarket);
    
    UE_LOG(LogTemp, Warning, TEXT("Financial crisis with severity %.2f"), CrisisSeverity);
}

int32 UMingRTSEconomicSystem::GetActiveTradeRouteCount() const
{
    int32 Count = 0;
    
    for (const auto& RoutePair : TradeRoutes)
    {
        if (RoutePair.Value.bIsActive)
        {
            Count++;
        }
    }
    
    return Count;
}

float UMingRTSEconomicSystem::GetTotalTradeVolume() const
{
    float TotalVolume = 0.0f;
    
    for (const auto& RoutePair : TradeRoutes)
    {
        const FRTSTradeRouteData& Route = RoutePair.Value;
        for (float Volume : Route.TradeVolumes)
        {
            TotalVolume += Volume;
        }
    }
    
    return TotalVolume;
}

float UMingRTSEconomicSystem::GetTotalTradeProfit() const
{
    return TotalTradeProfit;
}

float UMingRTSEconomicSystem::GetMarketCapitalization() const
{
    float TotalCap = 0.0f;
    
    for (const auto& MarketPair : MarketData)
    {
        const FRTSMarketData& Market = MarketPair.Value;
        TotalCap += Market.CurrentPrice * Market.Supply;
    }
    
    return TotalCap;
}

void UMingRTSEconomicSystem::InitializeEconomicData()
{
    EconomicData.GDP = 10000.0f;
    EconomicData.InflationRate = BaseInflationRate;
    EconomicData.UnemploymentRate = BaseUnemploymentRate;
    EconomicData.InterestRate = BaseInterestRate;
    EconomicData.TradeBalance = 0.0f;
    EconomicData.ConsumerConfidence = 0.7f;
    EconomicData.IndustrialProduction = 100.0f;
    EconomicData.CurrentPhase = ERTSEconomicPhase::Growth;
    EconomicData.MarketCondition = ERTSMarketCondition::Stable;
    EconomicData.EconomicGrowthRate = 0.03f;
    
    UE_LOG(LogTemp, Log, TEXT("Initialized economic data"));
}

void UMingRTSEconomicSystem::InitializeMarketData()
{
    // 初始化所有資源類型的市場數據
    TArray<ERTSResourceType> AllResourceTypes = {
        ERTSResourceType::Gold, ERTSResourceType::Wood, ERTSResourceType::Stone,
        ERTSResourceType::Iron, ERTSResourceType::Food, ERTSResourceType::Coal,
        ERTSResourceType::Oil, ERTSResourceType::Uranium, ERTSResourceType::Crystal,
        ERTSResourceType::Silk
    };
    
    for (ERTSResourceType ResourceType : AllResourceTypes)
    {
        FRTSMarketData Market;
        Market.ResourceType = ResourceType;
        Market.CurrentPrice = 100.0f; // 基準價格
        Market.BasePrice = 100.0f;
        Market.Demand = 50.0f;
        Market.Supply = 50.0f;
        Market.MarketShare = 0.1f;
        Market.Volatility = 0.1f;
        
        MarketData.Add(ResourceType, Market);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Initialized market data for %d resources"), AllResourceTypes.Num());
}

void UMingRTSEconomicSystem::ProcessEconomicCycle(float DeltaTime)
{
    UpdateGDP(DeltaTime);
    UpdateInflation(DeltaTime);
    UpdateUnemployment(DeltaTime);
    UpdateTradeBalance(DeltaTime);
    UpdateConsumerConfidence(DeltaTime);
    UpdateIndustrialProduction(DeltaTime);
    CalculateEconomicGrowth();
    DetermineEconomicPhase();
    DetermineMarketCondition();
    ApplyEconomicPolicies(DeltaTime);
}

void UMingRTSEconomicSystem::ProcessPriceFluctuations(float DeltaTime)
{
    UpdateResourcePrices();
    AnalyzeMarketTrends();
    
    if (bAutoOptimizeTrade)
    {
        OptimizeTradeRoutes();
    }
}

void UMingRTSEconomicSystem::ProcessTradeActivity(float DeltaTime)
{
    // 處理所有貿易路線
    TArray<FString> RoutesToRemove;
    
    for (auto& RoutePair : TradeRoutes)
    {
        FRTSTradeRouteData& Route = RoutePair.Value;
        
        if (Route.bIsActive)
        {
            ProcessTradeRoute(Route, DeltaTime);
        }
        
        // 檢查路線是否應該關閉
        if (Route.Efficiency < 0.1f || Route.CurrentLoad <= 0.0f)
        {
            RoutesToRemove.Add(Route.RouteID);
        }
    }
    
    // 移除無效路線
    for (const FString& RouteID : RoutesToRemove)
    {
        DestroyTradeRoute(RouteID);
    }
}

void UMingRTSEconomicSystem::UpdateGDP(float DeltaTime)
{
    // GDP計算基於工業生產和消費
    float ProductionContribution = EconomicData.IndustrialProduction * 10.0f;
    float ConsumptionContribution = EconomicData.ConsumerConfidence * EconomicData.GDP * 0.6f;
    float TradeContribution = GetTotalTradeVolume() * 0.5f;
    
    float NewGDP = ProductionContribution + ConsumptionContribution + TradeContribution;
    EconomicData.GDP = FMath::Max(0.0f, NewGDP);
}

void UMingRTSEconomicSystem::UpdateInflation(float DeltaTime)
{
    // 通脹基於需求和供給平衡
    float TotalDemand = 0.0f;
    float TotalSupply = 0.0f;
    
    for (const auto& MarketPair : MarketData)
    {
        TotalDemand += MarketPair.Value.Demand;
        TotalSupply += MarketPair.Value.Supply;
    }
    
    float DemandSupplyRatio = TotalSupply > 0.0f ? (TotalDemand / TotalSupply) : 1.0f;
    float InflationPressure = (DemandSupplyRatio - 1.0f) * 0.01f;
    
    EconomicData.InflationRate = FMath::Clamp(BaseInflationRate + InflationPressure, 0.0f, 0.2f);
}

void UMingRTSEconomicSystem::UpdateUnemployment(float DeltaTime)
{
    // 失業率基於經濟增長和工業生產
    float GrowthEffect = -EconomicData.EconomicGrowthRate * 0.5f;
    float ProductionEffect = (100.0f - EconomicData.IndustrialProduction) * 0.001f;
    
    EconomicData.UnemploymentRate = FMath::Clamp(BaseUnemploymentRate + GrowthEffect + ProductionEffect, 0.0f, 0.4f);
}

void UMingRTSEconomicSystem::UpdateTradeBalance(float DeltaTime)
{
    // 貿易平衡基於進出口
    float Exports = GetTotalTradeVolume() * 0.6f;
    float Imports = GetTotalTradeVolume() * 0.4f; // 假設進口較少
    
    EconomicData.TradeBalance = Exports - Imports;
}

void UMingRTSEconomicSystem::UpdateConsumerConfidence(float DeltaTime)
{
    // 消費者信心基於多種因素
    float EmploymentEffect = (1.0f - EconomicData.UnemploymentRate) * 0.3f;
    float InflationEffect = (1.0f - EconomicData.InflationRate) * 0.2f;
    float GrowthEffect = FMath::Clamp(EconomicData.EconomicGrowthRate * 2.0f, 0.0f, 1.0f) * 0.3f;
    float MarketEffect = IsMarketBullish() ? 0.2f : (IsMarketBearish() ? -0.2f : 0.0f);
    
    float NewConfidence = EmploymentEffect + InflationEffect + GrowthEffect + MarketEffect;
    EconomicData.ConsumerConfidence = FMath::Clamp(NewConfidence, 0.0f, 1.0f);
}

void UMingRTSEconomicSystem::UpdateIndustrialProduction(float DeltaTime)
{
    // 工業生產基於建築和單位活動
    float BuildingProduction = 0.0f;
    float UnitProduction = 0.0f;
    
    if (BuildingSystem)
    {
        int32 OperationalBuildings = BuildingSystem->GetOperationalBuildingCount();
        BuildingProduction = OperationalBuildings * 10.0f;
    }
    
    if (UnitManager)
    {
        int32 TotalUnits = UnitManager->GetTotalUnitCount();
        UnitProduction = TotalUnits * 2.0f;
    }
    
    EconomicData.IndustrialProduction = BuildingProduction + UnitProduction;
}

void UMingRTSEconomicSystem::CalculateEconomicGrowth()
{
    // 經濟增長率基於GDP變化
    static float LastGDP = EconomicData.GDP;
    float GDPChange = (EconomicData.GDP - LastGDP) / LastGDP;
    
    EconomicData.EconomicGrowthRate = FMath::Clamp(GDPChange, -0.2f, 0.2f);
    
    LastGDP = EconomicData.GDP;
}

void UMingRTSEconomicSystem::DetermineEconomicPhase()
{
    // 根據經濟指標確定經濟階段
    float GrowthRate = EconomicData.EconomicGrowthRate;
    float Unemployment = EconomicData.UnemploymentRate;
    float Confidence = EconomicData.ConsumerConfidence;
    
    ERTSEconomicPhase NewPhase;
    
    if (GrowthRate > 0.05f && Confidence > 0.8f && Unemployment < 0.03f)
    {
        NewPhase = ERTSEconomicPhase::Boom;
    }
    else if (GrowthRate > 0.02f && Confidence > 0.6f && Unemployment < 0.05f)
    {
        NewPhase = ERTSEconomicPhase::Growth;
    }
    else if (GrowthRate > 0.0f && Confidence > 0.4f)
    {
        NewPhase = ERTSEconomicPhase::Stagnation;
    }
    else if (GrowthRate > -0.03f && Unemployment < 0.1f)
    {
        NewPhase = ERTSEconomicPhase::Recovery;
    }
    else if (GrowthRate > -0.05f)
    {
        NewPhase = ERTSEconomicPhase::Recession;
    }
    else
    {
        NewPhase = ERTSEconomicPhase::Depression;
    }
    
    SetEconomicPhase(NewPhase);
}

void UMingRTSEconomicSystem::DetermineMarketCondition()
{
    // 根據市場波動性和趨勢確定市場條件
    float AvgVolatility = GetMarketVolatility();
    int32 TrendingUpCount = 0;
    int32 TotalMarkets = 0;
    
    for (const auto& MarketPair : MarketData)
    {
        if (MarketPair.Value.bIsTrendingUp)
        {
            TrendingUpCount++;
        }
        TotalMarkets++;
    }
    
    float UpTrendRatio = TotalMarkets > 0 ? (float)TrendingUpCount / TotalMarkets : 0.5f;
    
    ERTSMarketCondition NewCondition;
    
    if (AvgVolatility > 0.2f)
    {
        NewCondition = ERTSMarketCondition::Volatile;
    }
    else if (UpTrendRatio > 0.7f)
    {
        NewCondition = ERTSMarketCondition::BullMarket;
    }
    else if (UpTrendRatio < 0.3f)
    {
        NewCondition = ERTSMarketCondition::BearMarket;
    }
    else
    {
        NewCondition = ERTSMarketCondition::Stable;
    }
    
    SetMarketCondition(NewCondition);
}

float UMingRTSEconomicSystem::CalculateResourcePrice(ERTSResourceType ResourceType) const
{
    if (!MarketData.Contains(ResourceType))
    {
        return 100.0f;
    }

    const FRTSMarketData& Market = MarketData[ResourceType];
    
    // 基於供需關係計算價格
    float DemandSupplyRatio = Market.Supply > 0.0f ? (Market.Demand / Market.Supply) : 1.0f;
    float BasePrice = Market.BasePrice;
    
    // 應用供需影響
    float Price = BasePrice * DemandSupplyRatio;
    
    // 應用波動性
    float RandomFactor = FMath::FRandRange(-Market.Volatility, Market.Volatility);
    Price *= (1.0f + RandomFactor);
    
    // 應用通脹影響
    Price *= (1.0f + EconomicData.InflationRate);
    
    return FMath::Max(0.1f, Price);
}

void UMingRTSEconomicSystem::UpdateMarketData(ERTSResourceType ResourceType)
{
    if (!MarketData.Contains(ResourceType))
    {
        return;
    }

    FRTSMarketData& Market = MarketData[ResourceType];
    
    // 更新供需數據
    if (ResourceManager)
    {
        Market.Supply = ResourceManager->GetResourceAmount(ResourceType);
        Market.Demand = Market.Supply * 1.1f; // 假設需求略高於供給
    }
    
    // 更新市場份額
    float TotalMarketValue = GetMarketCapitalization();
    if (TotalMarketValue > 0.0f)
    {
        Market.MarketShare = (Market.CurrentPrice * Market.Supply) / TotalMarketValue;
    }
}

FString UMingRTSEconomicSystem::GenerateTradeRouteID() const
{
    static int32 RouteCounter = 0;
    return FString::Printf(TEXT("Route_%d"), RouteCounter++);
}

void UMingRTSEconomicSystem::ProcessTradeRoute(FRTSTradeRouteData& Route, float DeltaTime)
{
    // 處理貿易路線活動
    if (Route.bIsActive && Route.TradeGoods.Num() > 0)
    {
        // 計算運輸時間
        float TransportTime = Route.Distance / 100.0f; // 簡化計算
        
        // 模擬運輸完成
        for (int32 i = 0; i < Route.TradeGoods.Num(); ++i)
        {
            ERTSResourceType ResourceType = Route.TradeGoods[i];
            float Volume = Route.TradeVolumes[i];
            
            float Profit = CalculateTradeProfit(Route.RouteID, ResourceType, Volume);
            TotalTradeProfit += Profit;
            
            NotifyTradeRouteCompleted(Route.RouteID, Profit);
        }
        
        // 清空已完成的貿易
        Route.TradeGoods.Empty();
        Route.TradeVolumes.Empty();
        Route.CurrentLoad = 0.0f;
    }
}

void UMingRTSEconomicSystem::ApplyEconomicPolicies(float DeltaTime)
{
    // 應用自動經濟政策
    if (EconomicData.UnemploymentRate > 0.1f)
    {
        // 高失業率：實施刺激措施
        ImplementEconomicStimulus(100.0f * DeltaTime);
    }
    else if (EconomicData.InflationRate > 0.05f)
    {
        // 高通脹：實施緊縮措施
        ImplementAusterityMeasures(50.0f * DeltaTime);
    }
}

void UMingRTSEconomicSystem::HandleRandomEvents(float DeltaTime)
{
    // 隨機經濟事件
    float RandomChance = FMath::FRandRange(0.0f, 1.0f);
    
    if (RandomChance < 0.01f) // 1%機率
    {
        // 隨機供給或需求衝擊
        ERTSResourceType RandomResource = (ERTSResourceType)FMath::RandRange(0, 10);
        float ShockMagnitude = FMath::FRandRange(-0.2f, 0.2f);
        
        if (ShockMagnitude > 0.0f)
        {
            HandleDemandShock(RandomResource, ShockMagnitude);
        }
        else
        {
            HandleSupplyShock(RandomResource, FMath::Abs(ShockMagnitude));
        }
    }
}

void UMingRTSEconomicSystem::NotifyEconomicPhaseChanged(ERTSEconomicPhase OldPhase, ERTSEconomicPhase NewPhase)
{
    OnEconomicPhaseChanged.Broadcast(OldPhase, NewPhase);
}

void UMingRTSEconomicSystem::NotifyMarketConditionChanged(ERTSMarketCondition NewCondition)
{
    OnMarketConditionChanged.Broadcast(NewCondition);
}

void UMingRTSEconomicSystem::NotifyResourcePriceChanged(ERTSResourceType ResourceType, float NewPrice)
{
    OnResourcePriceChanged.Broadcast(ResourceType, NewPrice);
}

void UMingRTSEconomicSystem::NotifyTradeRouteCompleted(const FString& RouteID, float Profit)
{
    OnTradeRouteCompleted.Broadcast(RouteID, Profit);
}

// 輔助函數：獲取資源名稱
FString GetResourceName(ERTSResourceType ResourceType)
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

// 經濟循環平衡調整功能
void UMingRTSEconomicSystem::BalanceEconomicCycle()
{
    UE_LOG(LogTemp, Log, TEXT("Balancing economic cycle..."));
    
    // 優化經濟指標
    OptimizeEconomicIndicators();
    
    // 平衡貿易流動
    BalanceTradeFlows();
    
    // 計算最優經濟政策
    CalculateOptimalEconomicPolicy();
    
    UE_LOG(LogTemp, Log, TEXT("Economic cycle balanced"));
}

void UMingRTSEconomicSystem::StabilizeMarket()
{
    UE_LOG(LogTemp, Log, TEXT("Stabilizing market..."));
    
    // 穩定市場價格
    StabilizeMarketPrices();
    
    // 自動平衡經濟
    if (bEnableAutoBalancing)
    {
        AutoBalanceEconomy();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Market stabilized"));
}

void UMingRTSEconomicSystem::OptimizeTradeRoutes()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing trade routes..."));
    
    // 優化資源分配
    OptimizeResourceAllocation();
    
    // 平衡貿易流動
    BalanceTradeFlows();
    
    UE_LOG(LogTemp, Log, TEXT("Trade routes optimized"));
}

void UMingRTSEconomicSystem::ImplementSmartEconomicManagement()
{
    if (!bEnableSmartManagement)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Implementing smart economic management..."));
    
    // 計算經濟穩定性
    float Stability = CalculateEconomicStability();
    
    // 根據穩定性調整政策
    if (Stability < EconomicStabilityThreshold)
    {
        // 經濟不穩定，實施穩定措施
        StabilizeMarket();
        
        // 調整利率
        if (EconomicData.InflationRate > 0.05f)
        {
            SetInterestRate(EconomicData.InterestRate * 1.2f);
        }
        
        // 實施經濟刺激
        if (EconomicData.EconomicGrowthRate < 0.02f)
        {
            ImplementEconomicStimulus(EconomicData.GDP * 0.05f);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Smart economic management implemented"));
}

void UMingRTSEconomicSystem::OptimizeEconomicIndicators()
{
    // 優化GDP增長
    if (EconomicData.EconomicGrowthRate < 0.03f)
    {
        EconomicData.IndustrialProduction *= 1.1f;
        EconomicData.ConsumerConfidence *= 1.05f;
    }
    
    // 控制通脹
    if (EconomicData.InflationRate > 0.04f)
    {
        EconomicData.InterestRate *= 1.1f;
        EconomicData.ConsumerConfidence *= 0.95f;
    }
    
    // 降低失業率
    if (EconomicData.UnemploymentRate > 0.06f)
    {
        EconomicData.IndustrialProduction *= 1.05f;
        ImplementEconomicStimulus(EconomicData.GDP * 0.02f);
    }
}

void UMingRTSEconomicSystem::StabilizeMarketPrices()
{
    // 穩定所有資源價格
    for (auto& MarketPair : MarketData)
    {
        ERTSResourceType ResourceType = MarketPair.Key;
        FRTSMarketData& Market = MarketPair.Value;
        
        // 計算價格波動性
        float PriceVolatility = MarketVolatility;
        
        // 如果波動性過高，進行干預
        if (PriceVolatility > 0.15f)
        {
            // 調整供應和需求
            if (Market.CurrentPrice > Market.BasePrice * 1.2f)
            {
                // 價格過高，增加供應
                Market.Supply *= 1.1f;
                Market.Demand *= 0.9f;
            }
            else if (Market.CurrentPrice < Market.BasePrice * 0.8f)
            {
                // 價格過低，減少供應
                Market.Supply *= 0.9f;
                Market.Demand *= 1.1f;
            }
            
            // 應用穩定性加成
            Market.CurrentPrice = Market.BasePrice + (Market.CurrentPrice - Market.BasePrice) * 0.8f;
        }
    }
}

void UMingRTSEconomicSystem::BalanceTradeFlows()
{
    // 平衡進出口
    if (EconomicData.TradeBalance < -EconomicData.GDP * 0.1f)
    {
        // 貿易逆差過大，促進出口
        for (auto& RoutePair : TradeRoutes)
        {
            FRTSTradeRouteData& Route = RoutePair.Value;
            if (Route.bIsActive)
            {
                Route.ProfitMargin *= 1.1f;
                Route.TradeVolume *= 1.05f;
            }
        }
    }
    else if (EconomicData.TradeBalance > EconomicData.GDP * 0.1f)
    {
        // 貿易順差過大，適度放鬆
        for (auto& RoutePair : TradeRoutes)
        {
            FRTSTradeRouteData& Route = RoutePair.Value;
            if (Route.bIsActive)
            {
                Route.TradeVolume *= 0.95f;
            }
        }
    }
}

void UMingRTSEconomicSystem::CalculateOptimalEconomicPolicy()
{
    // 計算最優經濟政策
    float OptimalInterestRate = BaseInterestRate;
    float OptimalInflationRate = 0.02f;
    
    // 根據經濟狀況調整
    if (EconomicData.EconomicGrowthRate < 0.02f)
    {
        OptimalInterestRate *= 0.8f; // 降低利率刺激經濟
    }
    else if (EconomicData.EconomicGrowthRate > 0.05f)
    {
        OptimalInterestRate *= 1.2f; // 提高利率防止過熱
    }
    
    // 應用最優政策
    SetInterestRate(OptimalInterestRate);
    
    if (EconomicData.InflationRate > OptimalInflationRate)
    {
        AdjustInflationRate(-0.01f);
    }
    else if (EconomicData.InflationRate < OptimalInflationRate * 0.5f)
    {
        AdjustInflationRate(0.005f);
    }
}

float UMingRTSEconomicSystem::CalculateEconomicStability() const
{
    // 計算經濟穩定性指數
    float Stability = 1.0f;
    
    // GDP增長穩定性
    float GrowthStability = 1.0f - FMath::Abs(EconomicData.EconomicGrowthRate - 0.03f);
    Stability *= GrowthStability;
    
    // 通脹穩定性
    float InflationStability = 1.0f - FMath::Abs(EconomicData.InflationRate - 0.02f);
    Stability *= InflationStability;
    
    // 失業穩定性
    float UnemploymentStability = 1.0f - FMath::Abs(EconomicData.UnemploymentRate - 0.05f);
    Stability *= UnemploymentStability;
    
    // 市場穩定性
    float MarketStability = 1.0f - MarketVolatility;
    Stability *= MarketStability;
    
    return FMath::Clamp(Stability, 0.0f, 1.0f);
}

void UMingRTSEconomicSystem::AutoBalanceEconomy()
{
    if (!bEnableAutoBalancing)
    {
        return;
    }
    
    // 自動平衡經濟
    float CurrentStability = CalculateEconomicStability();
    
    if (CurrentStability < EconomicStabilityThreshold)
    {
        // 經濟不穩定，實施平衡措施
        OptimizeEconomicIndicators();
        StabilizeMarketPrices();
        BalanceTradeFlows();
    }
    
    // 調整市場穩定性加成
    if (CurrentStability > 0.9f)
    {
        MarketStabilityBonus = FMath::Max(MarketStabilityBonus * 0.95f, 1.0f);
    }
    else if (CurrentStability < 0.7f)
    {
        MarketStabilityBonus = FMath::Min(MarketStabilityBonus * 1.05f, 1.5f);
    }
}

void UMingRTSEconomicSystem::OptimizeResourceAllocation()
{
    // 優化資源分配
    for (auto& MarketPair : MarketData)
    {
        ERTSResourceType ResourceType = MarketPair.Key;
        FRTSMarketData& Market = MarketPair.Value;
        
        // 計算資源稀缺性
        float Scarcity = Market.Demand / FMath::Max(Market.Supply, 0.1f);
        
        // 根據稀缺性調整價格
        if (Scarcity > 1.5f)
        {
            Market.CurrentPrice *= 1.1f;
            Market.Supply *= 1.05f;
        }
        else if (Scarcity < 0.5f)
        {
            Market.CurrentPrice *= 0.95f;
            Market.Supply *= 0.98f;
        }
        
        // 應用市場穩定性加成
        Market.CurrentPrice *= MarketStabilityBonus;
    }
}
