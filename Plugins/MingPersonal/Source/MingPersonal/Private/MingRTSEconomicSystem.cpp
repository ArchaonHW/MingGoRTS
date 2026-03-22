#include "MingRTSEconomicSystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

UMingRTSEconomicSystem::UMingRTSEconomicSystem()
{
    CurrentWorld = nullptr;
    TaxRate = 0.1f; // 10% 稅率
    MilitarySpending = 0.0f;
    bFreeMarket = true;
    bProtectionism = false;
    bMercantilism = false;
    EconomicUpdateInterval = 1.0f; // 每秒更新
    LastEconomicUpdateTime = 0.0f;
    CrisisThreshold = -0.1f; // -10% 增長率為危機
    ProsperityThreshold = 0.05f; // 5% 增長率為繁榮

    // 初始化資源價值
    ResourceValues.Add(EResourceType::Gold, 100.0f);
    ResourceValues.Add(EResourceType::Food, 10.0f);
    ResourceValues.Add(EResourceType::Wood, 15.0f);
    ResourceValues.Add(EResourceType::Stone, 20.0f);
    ResourceValues.Add(EResourceType::Iron, 50.0f);
    ResourceValues.Add(EResourceType::Coal, 25.0f);
    ResourceValues.Add(EResourceType::Silk, 80.0f);
    ResourceValues.Add(EResourceType::Tea, 30.0f);
    ResourceValues.Add(EResourceType::Porcelain, 60.0f);
    ResourceValues.Add(EResourceType::Spice, 40.0f);

    // 初始化資源數據
    for (int32 i = 0; i < (int32)EResourceType::Spice + 1; i++)
    {
        EResourceType ResourceType = (EResourceType)i;
        FResourceData ResourceData;
        ResourceData.ResourceType = ResourceType;
        ResourceData.CurrentAmount = 100.0f; // 初始資源
        ResourceData.MaxStorage = 1000.0f;
        ResourceData.GenerationRate = 0.0f;
        ResourceData.ConsumptionRate = 0.0f;
        ResourceData.GenerationType = EResourceGenerationType::Natural;
        ResourceData.LastUpdateTime = 0.0f;
        
        Resources.Add(ResourceType, ResourceData);
    }

    // 黃金特殊處理
    Resources[EResourceType::Gold].CurrentAmount = 500.0f;
    Resources[EResourceType::Gold].MaxStorage = 10000.0f;
}

void UMingRTSEconomicSystem::InitializeEconomicSystem(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSEconomicSystem: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    LastEconomicUpdateTime = World->GetTimeSeconds();
    
    // 初始化經濟指標
    CalculateGDP();
    CalculateInflationRate();
    CalculateUnemploymentRate();
    CalculateTradeBalance();
    CalculateTaxRevenue();
    UpdateEconomicState();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSEconomicSystem initialized successfully"));
}

void UMingRTSEconomicSystem::UpdateEconomicSystem(float DeltaTime)
{
    if (!CurrentWorld)
    {
        return;
    }

    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    // 更新資源
    UpdateResources(DeltaTime);
    
    // 定期更新經濟指標
    if (CurrentTime - LastEconomicUpdateTime >= EconomicUpdateInterval)
    {
        ProcessTrade();
        CalculateEconomicGrowth();
        UpdateEconomicState();
        CheckEconomicHealth();
        ApplyEconomicPolicies();
        RecordEconomicData();
        
        LastEconomicUpdateTime = CurrentTime;
    }
}

bool UMingRTSEconomicSystem::AddResource(EResourceType ResourceType, float Amount)
{
    FResourceData* ResourceData = Resources.Find(ResourceType);
    if (!ResourceData)
    {
        return false;
    }

    float OldAmount = ResourceData->CurrentAmount;
    ResourceData->CurrentAmount = FMath::Min(ResourceData->CurrentAmount + Amount, ResourceData->MaxStorage);
    
    // 觸發資源變化事件
    OnResourceChanged.Broadcast(ResourceType);
    OnResourceAmountChanged.Broadcast(ResourceType, ResourceData->CurrentAmount);
    
    UE_LOG(LogTemp, Log, TEXT("Added %.2f %s (from %.2f to %.2f)"), 
        Amount, *GetResourceName(ResourceType), OldAmount, ResourceData->CurrentAmount);
    
    return true;
}

bool UMingRTSEconomicSystem::ConsumeResource(EResourceType ResourceType, float Amount)
{
    FResourceData* ResourceData = Resources.Find(ResourceType);
    if (!ResourceData)
    {
        return false;
    }

    if (ResourceData->CurrentAmount < Amount)
    {
        return false;
    }

    float OldAmount = ResourceData->CurrentAmount;
    ResourceData->CurrentAmount -= Amount;
    
    // 觸發資源變化事件
    OnResourceChanged.Broadcast(ResourceType);
    OnResourceAmountChanged.Broadcast(ResourceType, ResourceData->CurrentAmount);
    
    UE_LOG(LogTemp, Log, TEXT("Consumed %.2f %s (from %.2f to %.2f)"), 
        Amount, *GetResourceName(ResourceType), OldAmount, ResourceData->CurrentAmount);
    
    return true;
}

float UMingRTSEconomicSystem::GetResourceAmount(EResourceType ResourceType) const
{
    const FResourceData* ResourceData = Resources.Find(ResourceType);
    if (ResourceData)
    {
        return ResourceData->CurrentAmount;
    }
    return 0.0f;
}

void UMingRTSEconomicSystem::SetResourceGenerationRate(EResourceType ResourceType, float Rate)
{
    FResourceData* ResourceData = Resources.Find(ResourceType);
    if (ResourceData)
    {
        ResourceData->GenerationRate = Rate;
        UE_LOG(LogTemp, Log, TEXT("Set %s generation rate to %.2f/s"), 
            *GetResourceName(ResourceType), Rate);
    }
}

void UMingRTSEconomicSystem::SetResourceConsumptionRate(EResourceType ResourceType, float Rate)
{
    FResourceData* ResourceData = Resources.Find(ResourceType);
    if (ResourceData)
    {
        ResourceData->ConsumptionRate = Rate;
        UE_LOG(LogTemp, Log, TEXT("Set %s consumption rate to %.2f/s"), 
            *GetResourceName(ResourceType), Rate);
    }
}

void UMingRTSEconomicSystem::SetMaxStorage(EResourceType ResourceType, float MaxAmount)
{
    FResourceData* ResourceData = Resources.Find(ResourceType);
    if (ResourceData)
    {
        ResourceData->MaxStorage = MaxAmount;
        UE_LOG(LogTemp, Log, TEXT("Set %s max storage to %.2f"), 
            *GetResourceName(ResourceType), MaxAmount);
    }
}

FString UMingRTSEconomicSystem::CreateTradeRoute(const FString& RouteName, const TArray<EResourceType>& Exports, const TArray<EResourceType>& Imports)
{
    if (RouteName.IsEmpty() || TradeRoutes.Contains(RouteName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid or duplicate trade route name: %s"), *RouteName);
        return TEXT("");
    }

    FTradeRoute NewRoute;
    NewRoute.RouteName = RouteName;
    NewRoute.ExportResources = Exports;
    NewRoute.ImportResources = Imports;
    NewRoute.TradeVolume = 100.0f; // 默認交易量
    NewRoute.ProfitMargin = 0.1f; // 默認10%利潤
    NewRoute.bIsActive = true;
    NewRoute.RouteEfficiency = 1.0f;

    TradeRoutes.Add(RouteName, NewRoute);
    
    // 觸發貿易路線建立事件
    OnTradeRouteEstablished.Broadcast(NewRoute);
    
    UE_LOG(LogTemp, Log, TEXT("Created trade route: %s with %d exports and %d imports"), 
        *RouteName, Exports.Num(), Imports.Num());
    
    return RouteName;
}

void UMingRTSEconomicSystem::SetTradeRouteActive(const FString& RouteName, bool bActive)
{
    FTradeRoute* Route = TradeRoutes.Find(RouteName);
    if (Route)
    {
        Route->bIsActive = bActive;
        UE_LOG(LogTemp, Log, TEXT("Set trade route %s %s"), 
            *RouteName, bActive ? TEXT("active") : TEXT("inactive"));
    }
}

FTradeRoute UMingRTSEconomicSystem::GetTradeRoute(const FString& RouteName) const
{
    const FTradeRoute* Route = TradeRoutes.Find(RouteName);
    if (Route)
    {
        return *Route;
    }
    return FTradeRoute();
}

TArray<FTradeRoute> UMingRTSEconomicSystem::GetAllTradeRoutes() const
{
    TArray<FTradeRoute> AllRoutes;
    for (const TPair<FString, FTradeRoute>& RoutePair : TradeRoutes)
    {
        AllRoutes.Add(RoutePair.Value);
    }
    return AllRoutes;
}

void UMingRTSEconomicSystem::SetTaxRate(float Rate)
{
    TaxRate = FMath::Clamp(Rate, 0.0f, 0.5f); // 限制在0-50%
    CalculateTaxRevenue();
    UE_LOG(LogTemp, Log, TEXT("Set tax rate to %.1f%%"), TaxRate * 100.0f);
}

void UMingRTSEconomicSystem::SetMilitarySpending(float Amount)
{
    MilitarySpending = FMath::Max(0.0f, Amount);
    UE_LOG(LogTemp, Log, TEXT("Set military spending to %.2f"), MilitarySpending);
}

FEconomicMetrics UMingRTSEconomicSystem::GetEconomicMetrics() const
{
    return EconomicMetrics;
}

EEconomicState UMingRTSEconomicSystem::GetEconomicState() const
{
    return EconomicMetrics.CurrentState;
}

void UMingRTSEconomicSystem::BalanceEconomy()
{
    UE_LOG(LogTemp, Log, TEXT("Balancing economy..."));
    
    // 調整資源生成和消耗率
    for (TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        FResourceData& ResourceData = ResourcePair.Value;
        
        // 如果資源過少，增加生成率
        if (ResourceData.CurrentAmount < ResourceData.MaxStorage * 0.2f)
        {
            ResourceData.GenerationRate *= 1.2f;
            ResourceData.ConsumptionRate *= 0.9f;
        }
        // 如果資源過多，減少生成率
        else if (ResourceData.CurrentAmount > ResourceData.MaxStorage * 0.8f)
        {
            ResourceData.GenerationRate *= 0.9f;
            ResourceData.ConsumptionRate *= 1.1f;
        }
    }
    
    // 調整稅率
    if (EconomicMetrics.CurrentState == EEconomicState::Crisis || EconomicMetrics.CurrentState == EEconomicState::Depression)
    {
        SetTaxRate(FMath::Max(0.05f, TaxRate - 0.02f)); // 降低稅率
    }
    else if (EconomicMetrics.CurrentState == EEconomicState::Boom || EconomicMetrics.CurrentState == EEconomicState::Prosperous)
    {
        SetTaxRate(FMath::Min(0.3f, TaxRate + 0.02f)); // 提高稅率
    }
    
    UE_LOG(LogTemp, Log, TEXT("Economy balanced"));
}

void UMingRTSEconomicSystem::ProcessTrade()
{
    for (TPair<FString, FTradeRoute>& RoutePair : TradeRoutes)
    {
        FTradeRoute& Route = RoutePair.Value;
        if (Route.bIsActive)
        {
            ProcessResourceTrade(Route);
        }
    }
}

void UMingRTSEconomicSystem::CalculateEconomicGrowth()
{
    float PreviousGDP = EconomicMetrics.GDP;
    CalculateGDP();
    
    if (PreviousGDP > 0.0f)
    {
        EconomicMetrics.EconomicGrowthRate = (EconomicMetrics.GDP - PreviousGDP) / PreviousGDP;
        
        // 記錄增長歷史
        EconomicGrowthHistory.Add(EconomicMetrics.EconomicGrowthRate);
        if (EconomicGrowthHistory.Num() > 100) // 保留最近100個數據點
        {
            EconomicGrowthHistory.RemoveAt(0);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Economic growth rate: %.2f%%"), EconomicMetrics.EconomicGrowthRate * 100.0f);
}

void UMingRTSEconomicSystem::HandleEconomicCrisis()
{
    UE_LOG(LogTemp, Warning, TEXT("Handling economic crisis!"));
    
    // 危機處理措施
    BalanceEconomy();
    
    // 減少軍事開支
    SetMilitarySpending(MilitarySpending * 0.8f);
    
    // 激活所有貿易路線
    for (TPair<FString, FTradeRoute>& RoutePair : TradeRoutes)
    {
        RoutePair.Value.bIsActive = true;
        RoutePair.Value.RouteEfficiency *= 1.2f; // 提高效率
    }
    
    // 觸發危機事件
    TriggerEconomicEvent(TEXT("Economic Crisis"), -0.2f);
}

bool UMingRTSEconomicSystem::HasEnoughResource(EResourceType ResourceType, float Amount) const
{
    return GetResourceAmount(ResourceType) >= Amount;
}

float UMingRTSEconomicSystem::GetTotalResourceValue() const
{
    float TotalValue = 0.0f;
    for (const TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        const FResourceData& ResourceData = ResourcePair.Value;
        float ResourceValue = GetResourceValue(ResourceData.ResourceType);
        TotalValue += ResourceData.CurrentAmount * ResourceValue;
    }
    return TotalValue;
}

void UMingRTSEconomicSystem::SetEconomicPolicy(bool bFreeMarket, bool bProtectionism, bool bMercantilism)
{
    this->bFreeMarket = bFreeMarket;
    this->bProtectionism = bProtectionism;
    this->bMercantilism = bMercantilism;
    
    UE_LOG(LogTemp, Log, TEXT("Economic policy set: FreeMarket=%s, Protectionism=%s, Mercantilism=%s"),
        bFreeMarket ? TEXT("true") : TEXT("false"),
        bProtectionism ? TEXT("true") : TEXT("false"),
        bMercantilism ? TEXT("true") : TEXT("false"));
}

void UMingRTSEconomicSystem::UpdateResources(float DeltaTime)
{
    for (TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        FResourceData& ResourceData = ResourcePair.Value;
        
        // 計算淨變化
        float NetChange = (ResourceData.GenerationRate - ResourceData.ConsumptionRate) * DeltaTime;
        
        // 應用通脹影響
        if (EconomicMetrics.InflationRate > 0.0f)
        {
            NetChange *= (1.0f - EconomicMetrics.InflationRate * 0.1f);
        }
        
        // 更新資源數量
        float OldAmount = ResourceData.CurrentAmount;
        ResourceData.CurrentAmount = FMath::Clamp(
            ResourceData.CurrentAmount + NetChange,
            0.0f,
            ResourceData.MaxStorage
        );
        
        // 如果數量發生變化，觸發事件
        if (FMath::Abs(ResourceData.CurrentAmount - OldAmount) > 0.01f)
        {
            OnResourceAmountChanged.Broadcast(ResourceData.ResourceType, ResourceData.CurrentAmount);
        }
    }
}

void UMingRTSEconomicSystem::CalculateGDP()
{
    float PreviousGDP = EconomicMetrics.GDP;
    
    // GDP = 所有資源的價值總和 + 貿易收入 + 稅收
    EconomicMetrics.GDP = GetTotalResourceValue();
    
    // 添加貿易收入
    for (const TPair<FString, FTradeRoute>& RoutePair : TradeRoutes)
    {
        const FTradeRoute& Route = RoutePair.Value;
        if (Route.bIsActive)
        {
            EconomicMetrics.GDP += CalculateTradeProfit(Route);
        }
    }
    
    // 添加稅收
    EconomicMetrics.GDP += EconomicMetrics.TaxRevenue;
    
    // 減去軍事開支
    EconomicMetrics.GDP -= MilitarySpending;
    
    UE_LOG(LogTemp, Log, TEXT("GDP: %.2f (change: %.2f)"), EconomicMetrics.GDP, EconomicMetrics.GDP - PreviousGDP);
}

void UMingRTSEconomicSystem::CalculateInflationRate()
{
    // 基於資源供需計算通脹率
    float TotalDemand = 0.0f;
    float TotalSupply = 0.0f;
    
    for (const TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        const FResourceData& ResourceData = ResourcePair.Value;
        TotalDemand += ResourceData.ConsumptionRate;
        TotalSupply += ResourceData.GenerationRate;
    }
    
    if (TotalSupply > 0.0f)
    {
        float SupplyDemandRatio = TotalDemand / TotalSupply;
        EconomicMetrics.InflationRate = FMath::Clamp((SupplyDemandRatio - 1.0f) * 0.1f, -0.1f, 0.1f);
    }
    else
    {
        EconomicMetrics.InflationRate = 0.0f;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Inflation rate: %.2f%%"), EconomicMetrics.InflationRate * 100.0f);
}

void UMingRTSEconomicSystem::CalculateUnemploymentRate()
{
    // 簡化計算：基於經濟狀態和資源利用率
    float ResourceUtilization = 0.0f;
    int32 ResourceCount = 0;
    
    for (const TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        const FResourceData& ResourceData = ResourcePair.Value;
        if (ResourceData.MaxStorage > 0.0f)
        {
            ResourceUtilization += ResourceData.CurrentAmount / ResourceData.MaxStorage;
            ResourceCount++;
        }
    }
    
    if (ResourceCount > 0)
    {
        ResourceUtilization /= ResourceCount;
    }
    
    // 失業率與資源利用率成反比
    EconomicMetrics.UnemploymentRate = FMath::Clamp((1.0f - ResourceUtilization) * 0.5f, 0.0f, 0.3f);
    
    UE_LOG(LogTemp, Log, TEXT("Unemployment rate: %.2f%%"), EconomicMetrics.UnemploymentRate * 100.0f);
}

void UMingRTSEconomicSystem::CalculateTradeBalance()
{
    EconomicMetrics.TradeBalance = 0.0f;
    
    for (const TPair<FString, FTradeRoute>& RoutePair : TradeRoutes)
    {
        const FTradeRoute& Route = RoutePair.Value;
        if (Route.bIsActive)
        {
            EconomicMetrics.TradeBalance += CalculateTradeProfit(Route);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Trade balance: %.2f"), EconomicMetrics.TradeBalance);
}

void UMingRTSEconomicSystem::CalculateTaxRevenue()
{
    // 稅收 = GDP * 稅率
    EconomicMetrics.TaxRevenue = EconomicMetrics.GDP * TaxRate;
    UE_LOG(LogTemp, Log, TEXT("Tax revenue: %.2f"), EconomicMetrics.TaxRevenue);
}

void UMingRTSEconomicSystem::UpdateEconomicState()
{
    EEconomicState PreviousState = EconomicMetrics.CurrentState;
    
    // 基於經濟增長率確定狀態
    if (EconomicMetrics.EconomicGrowthRate < CrisisThreshold)
    {
        EconomicMetrics.CurrentState = EEconomicState::Crisis;
    }
    else if (EconomicMetrics.EconomicGrowthRate < -0.05f)
    {
        EconomicMetrics.CurrentState = EEconomicState::Depression;
    }
    else if (EconomicMetrics.EconomicGrowthRate < 0.0f)
    {
        EconomicMetrics.CurrentState = EEconomicState::Struggling;
    }
    else if (EconomicMetrics.EconomicGrowthRate < ProsperityThreshold)
    {
        EconomicMetrics.CurrentState = EEconomicState::Stable;
    }
    else if (EconomicMetrics.EconomicGrowthRate < 0.1f)
    {
        EconomicMetrics.CurrentState = EEconomicState::Prosperous;
    }
    else
    {
        EconomicMetrics.CurrentState = EEconomicState::Boom;
    }
    
    // 如果狀態改變，觸發事件
    if (PreviousState != EconomicMetrics.CurrentState)
    {
        OnEconomicStateChanged.Broadcast(EconomicMetrics.CurrentState);
        UE_LOG(LogTemp, Log, TEXT("Economic state changed to: %d"), (int32)EconomicMetrics.CurrentState);
    }
}

void UMingRTSEconomicSystem::ProcessResourceTrade(const FTradeRoute& Route)
{
    // 處理出口
    for (EResourceType ExportResource : Route.ExportResources)
    {
        float ExportAmount = Route.TradeVolume * Route.RouteEfficiency;
        if (HasEnoughResource(ExportResource, ExportAmount))
        {
            ConsumeResource(ExportResource, ExportAmount);
            // 獲得收入（以黃金形式）
            AddResource(EResourceType::Gold, ExportAmount * GetResourceValue(ExportResource) * Route.ProfitMargin);
        }
    }
    
    // 處理進口
    for (EResourceType ImportResource : Route.ImportResources)
    {
        float ImportAmount = Route.TradeVolume * Route.RouteEfficiency;
        // 支付成本（消耗黃金）
        float Cost = ImportAmount * GetResourceValue(ImportResource);
        if (HasEnoughResource(EResourceType::Gold, Cost))
        {
            ConsumeResource(EResourceType::Gold, Cost);
            AddResource(ImportResource, ImportAmount);
        }
    }
}

void UMingRTSEconomicSystem::ApplyEconomicPolicies()
{
    // 自由市場政策
    if (bFreeMarket)
    {
        // 提高貿易效率
        for (TPair<FString, FTradeRoute>& RoutePair : TradeRoutes)
        {
            RoutePair.Value.RouteEfficiency = FMath::Min(1.5f, RoutePair.Value.RouteEfficiency * 1.01f);
        }
    }
    
    // 保護主義政策
    if (bProtectionism)
    {
        // 降低進口，提高出口利潤
        for (TPair<FString, FTradeRoute>& RoutePair : TradeRoutes)
        {
            RoutePair.Value.ProfitMargin = FMath::Min(0.3f, RoutePair.Value.ProfitMargin * 1.02f);
        }
    }
    
    // 重商主義政策
    if (bMercantilism)
    {
        // 增加黃金儲備
        SetTaxRate(FMath::Min(0.2f, TaxRate + 0.01f));
    }
}

void UMingRTSEconomicSystem::CheckEconomicHealth()
{
    // 檢查是否需要處理危機
    if (EconomicMetrics.CurrentState == EEconomicState::Crisis)
    {
        HandleEconomicCrisis();
    }
    
    // 檢查是否繁榮
    if (EconomicMetrics.CurrentState == EEconomicState::Prosperous || EconomicMetrics.CurrentState == EEconomicState::Boom)
    {
        TriggerEconomicEvent(TEXT("Economic Prosperity"), 0.1f);
    }
}

FString UMingRTSEconomicSystem::GetResourceName(EResourceType ResourceType) const
{
    switch (ResourceType)
    {
        case EResourceType::Gold: return TEXT("Gold");
        case EResourceType::Food: return TEXT("Food");
        case EResourceType::Wood: return TEXT("Wood");
        case EResourceType::Stone: return TEXT("Stone");
        case EResourceType::Iron: return TEXT("Iron");
        case EResourceType::Coal: return TEXT("Coal");
        case EResourceType::Silk: return TEXT("Silk");
        case EResourceType::Tea: return TEXT("Tea");
        case EResourceType::Porcelain: return TEXT("Porcelain");
        case EResourceType::Spice: return TEXT("Spice");
        default: return TEXT("Unknown");
    }
}

float UMingRTSEconomicSystem::GetResourceValue(EResourceType ResourceType) const
{
    const float* Value = ResourceValues.Find(ResourceType);
    return Value ? *Value : 0.0f;
}

float UMingRTSEconomicSystem::CalculateTradeProfit(const FTradeRoute& Route) const
{
    float Profit = 0.0f;
    
    // 計算出口收入
    for (EResourceType ExportResource : Route.ExportResources)
    {
        Profit += Route.TradeVolume * GetResourceValue(ExportResource) * Route.ProfitMargin;
    }
    
    // 計算進口成本
    for (EResourceType ImportResource : Route.ImportResources)
    {
        Profit -= Route.TradeVolume * GetResourceValue(ImportResource);
    }
    
    return Profit * Route.RouteEfficiency;
}

void UMingRTSEconomicSystem::ApplyInflationEffects()
{
    // 通脹會降低資源的實際價值
    for (TPair<EResourceType, float>& ResourceValuePair : ResourceValues)
    {
        ResourceValuePair.Value *= (1.0f - EconomicMetrics.InflationRate * 0.05f);
    }
}

void UMingRTSEconomicSystem::ApplyTaxEffects()
{
    // 稅收會影響資源生成
    float TaxMultiplier = 1.0f - TaxRate * 0.1f;
    
    for (TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        FResourceData& ResourceData = ResourcePair.Value;
        ResourceData.GenerationRate *= TaxMultiplier;
    }
}

void UMingRTSEconomicSystem::TriggerEconomicEvent(const FString& EventName, float Impact)
{
    UE_LOG(LogTemp, Log, TEXT("Economic event triggered: %s (impact: %.2f%%)"), 
        *EventName, Impact * 100.0f);
    
    // 事件會影響所有資源的生成率
    for (TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        FResourceData& ResourceData = ResourcePair.Value;
        ResourceData.GenerationRate *= (1.0f + Impact);
    }
}

void UMingRTSEconomicSystem::RecordEconomicData()
{
    // 記錄當前經濟數據用於分析
    // 這裡可以添加更詳細的數據記錄邏輯
}

void UMingRTSEconomicSystem::PredictEconomicTrends()
{
    // 基於歷史數據預測經濟趨勢
    if (EconomicGrowthHistory.Num() >= 10)
    {
        float Sum = 0.0f;
        for (int32 i = EconomicGrowthHistory.Num() - 10; i < EconomicGrowthHistory.Num(); i++)
        {
            Sum += EconomicGrowthHistory[i];
        }
        
        float AverageGrowth = Sum / 10.0f;
        UE_LOG(LogTemp, Log, TEXT("Predicted economic growth: %.2f%%"), AverageGrowth * 100.0f);
    }
}

void UMingRTSEconomicSystem::AdjustResourcePrices()
{
    // 根據供需調整資源價格
    for (TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        FResourceData& ResourceData = ResourcePair.Value;
        
        float SupplyDemandRatio = ResourceData.GenerationRate / FMath::Max(0.01f, ResourceData.ConsumptionRate);
        
        // 供不應求時價格上漲
        if (SupplyDemandRatio < 1.0f)
        {
            float* ResourceValue = ResourceValues.Find(ResourceData.ResourceType);
            if (ResourceValue)
            {
                *ResourceValue *= 1.01f; // 每次更新上漲1%
            }
        }
        // 供過於求時價格下跌
        else if (SupplyDemandRatio > 1.0f)
        {
            float* ResourceValue = ResourceValues.Find(ResourceData.ResourceType);
            if (ResourceValue)
            {
                *ResourceValue *= 0.99f; // 每次更新下跌1%
            }
        }
    }
}

void UMingRTSEconomicSystem::ProcessSupplyDemand()
{
    // 處理市場供需平衡
    for (TPair<EResourceType, FResourceData>& ResourcePair : Resources)
    {
        FResourceData& ResourceData = ResourcePair.Value;
        
        float Supply = ResourceData.GenerationRate;
        float Demand = ResourceData.ConsumptionRate;
        
        // 如果需求大於供給，提高生成率
        if (Demand > Supply * 1.2f)
        {
            ResourceData.GenerationRate *= 1.05f;
        }
        // 如果供給大於需求，降低生成率
        else if (Supply > Demand * 1.2f)
        {
            ResourceData.GenerationRate *= 0.95f;
        }
    }
}
