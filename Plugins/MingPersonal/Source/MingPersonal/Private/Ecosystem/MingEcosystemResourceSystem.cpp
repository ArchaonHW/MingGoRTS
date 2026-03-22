#include "Ecosystem/MingEcosystemResourceSystem.h"
#include "Math/UnrealMathUtility.h"

UMingEcosystemResourceSystem::UMingEcosystemResourceSystem()
    : bIsInitialized(false)
    , CurrentGameTime(0.0f)
{
}

void UMingEcosystemResourceSystem::InitializeResourceSystem()
{
    if (bIsInitialized)
    {
        return;
    }
    
    InitializeDefaultRates();
    
    CurrentGameTime = 0.0f;
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Initialized"));
}

void UMingEcosystemResourceSystem::ShutdownResourceSystem()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    ResourceZones.Empty();
    AllResourceNodes.Empty();
    ActiveExtractions.Empty();
    ActiveTrades.Empty();
    ZoneConsumptions.Empty();
    DefaultRegenerationRates.Empty();
    ExtractionEfficiencyMultipliers.Empty();
    EnvironmentalImpactMultipliers.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Shutdown"));
}

void UMingEcosystemResourceSystem::InitializeDefaultRates()
{
    // Set default regeneration rates for different resource types
    DefaultRegenerationRates.Add(EResourceNodeType::Forest, 2.0f);
    DefaultRegenerationRates.Add(EResourceNodeType::Farm, 5.0f);
    DefaultRegenerationRates.Add(EResourceNodeType::FishingGround, 3.0f);
    DefaultRegenerationRates.Add(EResourceNodeType::HuntingGround, 1.5f);
    DefaultRegenerationRates.Add(EResourceNodeType::WaterSource, 10.0f);
    DefaultRegenerationRates.Add(EResourceNodeType::MagicalSpring, 8.0f);
    DefaultRegenerationRates.Add(EResourceNodeType::Mine, 0.1f); // Very slow
    DefaultRegenerationRates.Add(EResourceNodeType::Quarry, 0.05f); // Very slow
    DefaultRegenerationRates.Add(EResourceNodeType::OilField, 0.02f); // Extremely slow
    DefaultRegenerationRates.Add(EResourceNodeType::CrystalFormation, 0.5f);
    DefaultRegenerationRates.Add(EResourceNodeType::AncientRuins, 0.0f); // Non-renewable
    DefaultRegenerationRates.Add(EResourceNodeType::GasDeposit, 0.01f); // Extremely slow
    
    // Set extraction efficiency multipliers
    ExtractionEfficiencyMultipliers.Add(EResourceExtractionMethod::Manual, 0.5f);
    ExtractionEfficiencyMultipliers.Add(EResourceExtractionMethod::Tool, 0.8f);
    ExtractionEfficiencyMultipliers.Add(EResourceExtractionMethod::Mechanized, 1.2f);
    ExtractionEfficiencyMultipliers.Add(EResourceExtractionMethod::Automated, 1.5f);
    ExtractionEfficiencyMultipliers.Add(EResourceExtractionMethod::Advanced, 2.0f);
    ExtractionEfficiencyMultipliers.Add(EResourceExtractionMethod::Magical, 3.0f);
    ExtractionEfficiencyMultipliers.Add(EResourceExtractionMethod::Sustainable, 1.0f);
    
    // Set environmental impact multipliers
    EnvironmentalImpactMultipliers.Add(EResourceExtractionMethod::Manual, 0.1f);
    EnvironmentalImpactMultipliers.Add(EResourceExtractionMethod::Tool, 0.2f);
    EnvironmentalImpactMultipliers.Add(EResourceExtractionMethod::Mechanized, 0.6f);
    EnvironmentalImpactMultipliers.Add(EResourceExtractionMethod::Automated, 0.8f);
    EnvironmentalImpactMultipliers.Add(EResourceExtractionMethod::Advanced, 0.5f);
    EnvironmentalImpactMultipliers.Add(EResourceExtractionMethod::Magical, 0.2f);
    EnvironmentalImpactMultipliers.Add(EResourceExtractionMethod::Sustainable, 0.05f);
}

FString UMingEcosystemResourceSystem::CreateResourceZone(const FVector& Center, float Radius)
{
    FString ZoneID = FGuid::NewGuid().ToString();
    
    FResourceZone NewZone;
    NewZone.ZoneID = ZoneID;
    NewZone.CenterLocation = Center;
    NewZone.Radius = Radius;
    
    ResourceZones.Add(ZoneID, NewZone);
    ZoneConsumptions.Add(ZoneID, TMap<EResourceType, FResourceConsumption>());
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Created resource zone %s"), *ZoneID);
    
    return ZoneID;
}

void UMingEcosystemResourceSystem::RemoveResourceZone(const FString& ZoneID)
{
    if (ResourceZones.Contains(ZoneID))
    {
        // Remove all nodes in this zone
        FResourceZone& Zone = ResourceZones[ZoneID];
        for (const FResourceNode& Node : Zone.ResourceNodes)
        {
            AllResourceNodes.Remove(Node.NodeID);
            
            // Remove associated extraction operations
            TArray<FString> OpsToRemove;
            for (const auto& Op : ActiveExtractions)
            {
                if (Op.Value.NodeID == Node.NodeID)
                {
                    OpsToRemove.Add(Op.Key);
                }
            }
            for (const FString& OpID : OpsToRemove)
            {
                ActiveExtractions.Remove(OpID);
            }
        }
    }
    
    ResourceZones.Remove(ZoneID);
    ZoneConsumptions.Remove(ZoneID);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Removed resource zone %s"), *ZoneID);
}

FResourceZone UMingEcosystemResourceSystem::GetResourceZone(const FString& ZoneID) const
{
    if (ResourceZones.Contains(ZoneID))
    {
        return ResourceZones[ZoneID];
    }
    return FResourceZone();
}

TArray<FString> UMingEcosystemResourceSystem::GetAllZoneIDs() const
{
    TArray<FString> ZoneIDs;
    ResourceZones.GetKeys(ZoneIDs);
    return ZoneIDs;
}

FString UMingEcosystemResourceSystem::CreateResourceNode(const FString& ZoneID, EResourceNodeType Type, const FVector& Location, float InitialReserve)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return FString();
    }
    
    FString NodeID = FGuid::NewGuid().ToString();
    
    FResourceNode NewNode;
    NewNode.NodeID = NodeID;
    NewNode.NodeType = Type;
    NewNode.Location = Location;
    NewNode.TotalReserve = InitialReserve;
    NewNode.CurrentAmount = InitialReserve;
    NewNode.NaturalCapacity = InitialReserve;
    
    // Set default regeneration rate
    if (DefaultRegenerationRates.Contains(Type))
    {
        NewNode.RegenerationRate = DefaultRegenerationRates[Type];
    }
    
    // Determine if renewable
    NewNode.bIsRenewable = (NewNode.RegenerationRate > 0.0f);
    
    // Add to zone and global list
    FResourceZone& Zone = ResourceZones[ZoneID];
    Zone.ResourceNodes.Add(NewNode);
    AllResourceNodes.Add(NodeID, NewNode);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Created resource node %s of type %d in zone %s"),
        *NodeID, static_cast<int32>(Type), *ZoneID);
    
    return NodeID;
}

void UMingEcosystemResourceSystem::RemoveResourceNode(const FString& NodeID)
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return;
    }
    
    FResourceNode Node = AllResourceNodes[NodeID];
    
    // Remove from zone
    for (auto& ZonePair : ResourceZones)
    {
        FResourceZone& Zone = ZonePair.Value;
        for (int32 i = Zone.ResourceNodes.Num() - 1; i >= 0; --i)
        {
            if (Zone.ResourceNodes[i].NodeID == NodeID)
            {
                Zone.ResourceNodes.RemoveAt(i);
                break;
            }
        }
    }
    
    // Remove associated extraction operations
    TArray<FString> OpsToRemove;
    for (const auto& Op : ActiveExtractions)
    {
        if (Op.Value.NodeID == NodeID)
        {
            OpsToRemove.Add(Op.Key);
        }
    }
    for (const FString& OpID : OpsToRemove)
    {
        ActiveExtractions.Remove(OpID);
    }
    
    AllResourceNodes.Remove(NodeID);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Removed resource node %s"), *NodeID);
}

FResourceNode UMingEcosystemResourceSystem::GetResourceNode(const FString& NodeID) const
{
    if (AllResourceNodes.Contains(NodeID))
    {
        return AllResourceNodes[NodeID];
    }
    return FResourceNode();
}

void UMingEcosystemResourceSystem::SetNodeQuality(const FString& NodeID, EResourceQuality Quality)
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return;
    }
    
    FResourceNode& Node = AllResourceNodes[NodeID];
    Node.Quality = Quality;
    
    // Set quality modifier
    switch (Quality)
    {
    case EResourceQuality::Poor:
        Node.QualityModifier = 0.5f;
        break;
    case EResourceQuality::Common:
        Node.QualityModifier = 1.0f;
        break;
    case EResourceQuality::Good:
        Node.QualityModifier = 1.3f;
        break;
    case EResourceQuality::Excellent:
        Node.QualityModifier = 1.6f;
        break;
    case EResourceQuality::Legendary:
        Node.QualityModifier = 2.0f;
        break;
    }
    
    // Update in zone
    for (auto& ZonePair : ResourceZones)
    {
        FResourceZone& Zone = ZonePair.Value;
        for (FResourceNode& ZoneNode : Zone.ResourceNodes)
        {
            if (ZoneNode.NodeID == NodeID)
            {
                ZoneNode.Quality = Quality;
                ZoneNode.QualityModifier = Node.QualityModifier;
                break;
            }
        }
    }
}

void UMingEcosystemResourceSystem::UpgradeExtractionMethod(const FString& NodeID, EResourceExtractionMethod Method)
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return;
    }
    
    FResourceNode& Node = AllResourceNodes[NodeID];
    
    // Update extraction efficiency
    float NewEfficiency = GetExtractionEfficiency(Method);
    Node.ExtractionEfficiency = NewEfficiency;
    
    // Update environmental impact
    Node.EnvironmentalImpact = GetEnvironmentalImpactMultiplier(Method);
    
    // Update in zone
    for (auto& ZonePair : ResourceZones)
    {
        FResourceZone& Zone = ZonePair.Value;
        for (FResourceNode& ZoneNode : Zone.ResourceNodes)
        {
            if (ZoneNode.NodeID == NodeID)
            {
                ZoneNode.ExtractionEfficiency = Node.ExtractionEfficiency;
                ZoneNode.EnvironmentalImpact = Node.EnvironmentalImpact;
                break;
            }
        }
    }
}

FString UMingEcosystemResourceSystem::StartExtraction(const FString& NodeID, EResourceExtractionMethod Method, float Rate)
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return FString();
    }
    
    FString OperationID = FGuid::NewGuid().ToString();
    
    FExtractionOperation NewOp;
    NewOp.OperationID = OperationID;
    NewOp.NodeID = NodeID;
    NewOp.Method = Method;
    NewOp.ExtractionRate = Rate;
    NewOp.Efficiency = GetExtractionEfficiency(Method);
    NewOp.bIsActive = true;
    NewOp.EnvironmentalImpact = GetEnvironmentalImpactMultiplier(Method) * Rate;
    NewOp.LaborRequirement = (Method == EResourceExtractionMethod::Manual || 
                              Method == EResourceExtractionMethod::Tool) ? Rate * 2.0f : Rate * 0.5f;
    NewOp.EnergyRequirement = (Method == EResourceExtractionMethod::Mechanized ||
                               Method == EResourceExtractionMethod::Automated) ? Rate * 5.0f : Rate;
    
    ActiveExtractions.Add(OperationID, NewOp);
    
    // Update node extraction rate
    FResourceNode& Node = AllResourceNodes[NodeID];
    Node.ExtractionRate += Rate;
    Node.EnvironmentalImpact = FMath::Max(Node.EnvironmentalImpact, NewOp.EnvironmentalImpact);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Started extraction operation %s on node %s"),
        *OperationID, *NodeID);
    
    return OperationID;
}

void UMingEcosystemResourceSystem::StopExtraction(const FString& OperationID)
{
    if (!ActiveExtractions.Contains(OperationID))
    {
        return;
    }
    
    FExtractionOperation Op = ActiveExtractions[OperationID];
    
    // Reduce node extraction rate
    if (AllResourceNodes.Contains(Op.NodeID))
    {
        FResourceNode& Node = AllResourceNodes[Op.NodeID];
        Node.ExtractionRate = FMath::Max(0.0f, Node.ExtractionRate - Op.ExtractionRate);
    }
    
    ActiveExtractions.Remove(OperationID);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Stopped extraction operation %s"), *OperationID);
}

float UMingEcosystemResourceSystem::ExtractResource(const FString& NodeID, float Amount)
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return 0.0f;
    }
    
    FResourceNode& Node = AllResourceNodes[NodeID];
    
    // Calculate actual extractable amount
    float ExtractableAmount = FMath::Min(Amount, Node.CurrentAmount);
    
    // Apply extraction efficiency
    ExtractableAmount *= Node.ExtractionEfficiency;
    
    // Apply quality modifier
    ExtractableAmount *= Node.QualityModifier;
    
    // Reduce node amount
    Node.CurrentAmount = FMath::Max(0.0f, Node.CurrentAmount - Amount);
    Node.LastExtractionTime = CurrentGameTime;
    
    // Update depletion level
    Node.DepletionLevel = 1.0f - (Node.CurrentAmount / Node.TotalReserve);
    
    // Check for depletion
    if (Node.CurrentAmount <= 0.0f && !Node.bIsDepleted)
    {
        DepleteNode(Node);
    }
    
    // Update in zone
    for (auto& ZonePair : ResourceZones)
    {
        FResourceZone& Zone = ZonePair.Value;
        for (FResourceNode& ZoneNode : Zone.ResourceNodes)
        {
            if (ZoneNode.NodeID == NodeID)
            {
                ZoneNode.CurrentAmount = Node.CurrentAmount;
                ZoneNode.DepletionLevel = Node.DepletionLevel;
                ZoneNode.bIsDepleted = Node.bIsDepleted;
                break;
            }
        }
    }
    
    return ExtractableAmount;
}

void UMingEcosystemResourceSystem::ModifyExtractionRate(const FString& OperationID, float NewRate)
{
    if (!ActiveExtractions.Contains(OperationID))
    {
        return;
    }
    
    FExtractionOperation& Op = ActiveExtractions[OperationID];
    
    // Update node extraction rate
    if (AllResourceNodes.Contains(Op.NodeID))
    {
        FResourceNode& Node = AllResourceNodes[Op.NodeID];
        Node.ExtractionRate = Node.ExtractionRate - Op.ExtractionRate + NewRate;
    }
    
    Op.ExtractionRate = NewRate;
    Op.EnvironmentalImpact = GetEnvironmentalImpactMultiplier(Op.Method) * NewRate;
}

FExtractionOperation UMingEcosystemResourceSystem::GetExtractionOperation(const FString& OperationID) const
{
    if (ActiveExtractions.Contains(OperationID))
    {
        return ActiveExtractions[OperationID];
    }
    return FExtractionOperation();
}

void UMingEcosystemResourceSystem::RegisterConsumption(const FString& ZoneID, EResourceType Resource, float Rate, float Urgency)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    if (!ZoneConsumptions.Contains(ZoneID))
    {
        ZoneConsumptions.Add(ZoneID, TMap<EResourceType, FResourceConsumption>());
    }
    
    FResourceConsumption Consumption;
    Consumption.Resource = Resource;
    Consumption.ConsumptionRate = Rate;
    Consumption.UrgencyLevel = FMath::Clamp(Urgency, 0.0f, 2.0f);
    
    ZoneConsumptions[ZoneID].Add(Resource, Consumption);
}

void UMingEcosystemResourceSystem::UpdateConsumptionRate(const FString& ZoneID, EResourceType Resource, float NewRate)
{
    if (!ZoneConsumptions.Contains(ZoneID))
    {
        return;
    }
    
    auto& Consumptions = ZoneConsumptions[ZoneID];
    if (Consumptions.Contains(Resource))
    {
        Consumptions[Resource].ConsumptionRate = NewRate;
    }
}

void UMingEcosystemResourceSystem::SetRecyclingRate(const FString& ZoneID, EResourceType Resource, float Rate)
{
    if (!ZoneConsumptions.Contains(ZoneID))
    {
        return;
    }
    
    auto& Consumptions = ZoneConsumptions[ZoneID];
    if (Consumptions.Contains(Resource))
    {
        Consumptions[Resource].RecyclingRate = FMath::Clamp(Rate, 0.0f, 1.0f);
    }
}

FResourceConsumption UMingEcosystemResourceSystem::GetConsumptionData(const FString& ZoneID, EResourceType Resource) const
{
    if (ZoneConsumptions.Contains(ZoneID))
    {
        const auto& Consumptions = ZoneConsumptions[ZoneID];
        if (Consumptions.Contains(Resource))
        {
            return Consumptions[Resource];
        }
    }
    return FResourceConsumption();
}

void UMingEcosystemResourceSystem::SimulateResourceRegeneration(const FString& ZoneID, float DeltaTime)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    FResourceZone& Zone = ResourceZones[ZoneID];
    
    for (FResourceNode& Node : Zone.ResourceNodes)
    {
        SimulateNodeRegeneration(Node, DeltaTime);
        
        // Update global node list
        if (AllResourceNodes.Contains(Node.NodeID))
        {
            AllResourceNodes[Node.NodeID] = Node;
        }
    }
    
    // Update zone sustainability
    UpdateZoneSustainabilityScore(ZoneID);
}

void UMingEcosystemResourceSystem::SetRegenerationRate(const FString& NodeID, float NewRate)
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return;
    }
    
    FResourceNode& Node = AllResourceNodes[NodeID];
    Node.RegenerationRate = NewRate;
    Node.bIsRenewable = (NewRate > 0.0f);
    
    // Update in zone
    for (auto& ZonePair : ResourceZones)
    {
        FResourceZone& Zone = ZonePair.Value;
        for (FResourceNode& ZoneNode : Zone.ResourceNodes)
        {
            if (ZoneNode.NodeID == NodeID)
            {
                ZoneNode.RegenerationRate = NewRate;
                ZoneNode.bIsRenewable = Node.bIsRenewable;
                break;
            }
        }
    }
}

void UMingEcosystemResourceSystem::BoostRegeneration(const FString& ZoneID, EResourceType Resource, float BoostFactor)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    FResourceZone& Zone = ResourceZones[ZoneID];
    
    for (FResourceNode& Node : Zone.ResourceNodes)
    {
        if (MapNodeTypeToResource(Node.NodeType) == Resource)
        {
            Node.RegenerationRate *= BoostFactor;
        }
    }
}

void UMingEcosystemResourceSystem::RestoreDepletedNode(const FString& NodeID, float Amount)
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return;
    }
    
    FResourceNode& Node = AllResourceNodes[NodeID];
    
    if (Node.bIsDepleted)
    {
        Node.CurrentAmount = Amount;
        Node.bIsDepleted = false;
        Node.DepletionLevel = 0.0f;
        Node.RecoveryCooldown = 0.0f;
        
        // Update in zone
        for (auto& ZonePair : ResourceZones)
        {
            FResourceZone& Zone = ZonePair.Value;
            for (FResourceNode& ZoneNode : Zone.ResourceNodes)
            {
                if (ZoneNode.NodeID == NodeID)
                {
                    ZoneNode.CurrentAmount = Node.CurrentAmount;
                    ZoneNode.bIsDepleted = Node.bIsDepleted;
                    ZoneNode.DepletionLevel = Node.DepletionLevel;
                    break;
                }
            }
        }
        
        OnResourceNodeRegenerated.Broadcast(NodeID, Node.CurrentAmount);
        
        UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Restored depleted node %s"), *NodeID);
    }
}

float UMingEcosystemResourceSystem::CalculateNaturalRegeneration(const FString& NodeID, float DeltaTime) const
{
    if (!AllResourceNodes.Contains(NodeID))
    {
        return 0.0f;
    }
    
    const FResourceNode& Node = AllResourceNodes[NodeID];
    
    if (!Node.bIsRenewable || Node.bIsDepleted)
    {
        return 0.0f;
    }
    
    // Calculate regeneration based on current state
    float CapacityRatio = 1.0f - (Node.CurrentAmount / Node.NaturalCapacity);
    float RegenerationAmount = Node.RegenerationRate * CapacityRatio * DeltaTime;
    
    // Apply sustainability factors
    float SustainabilityFactor = Node.SustainabilityScore;
    RegenerationAmount *= SustainabilityFactor;
    
    return RegenerationAmount;
}

void UMingEcosystemResourceSystem::CalculateResourceBalance(const FString& ZoneID)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    FResourceZone& Zone = ResourceZones[ZoneID];
    
    // Calculate total supply for each resource type
    TMap<EResourceType, float> TotalSupply;
    TMap<EResourceType, float> TotalExtractionCapacity;
    
    for (const FResourceNode& Node : Zone.ResourceNodes)
    {
        EResourceType Resource = MapNodeTypeToResource(Node.NodeType);
        
        if (!TotalSupply.Contains(Resource))
        {
            TotalSupply.Add(Resource, 0.0f);
            TotalExtractionCapacity.Add(Resource, 0.0f);
        }
        
        TotalSupply[Resource] += Node.CurrentAmount;
        TotalExtractionCapacity[Resource] += Node.ExtractionRate;
    }
    
    // Calculate demand from consumption
    TMap<EResourceType, float> TotalDemand;
    if (ZoneConsumptions.Contains(ZoneID))
    {
        for (const auto& ConsumptionPair : ZoneConsumptions[ZoneID])
        {
            TotalDemand.Add(ConsumptionPair.Key, ConsumptionPair.Value.ConsumptionRate);
        }
    }
    
    // Create or update resource balances
    for (const auto& SupplyPair : TotalSupply)
    {
        EResourceType Resource = SupplyPair.Key;
        
        FResourceBalance Balance;
        Balance.Resource = Resource;
        Balance.TotalSupply = SupplyPair.Value;
        Balance.TotalDemand = TotalDemand.Contains(Resource) ? TotalDemand[Resource] : 0.0f;
        Balance.NetBalance = Balance.TotalSupply - Balance.TotalDemand;
        Balance.ExtractionCapacity = TotalExtractionCapacity[Resource];
        Balance.ConsumptionRate = Balance.TotalDemand;
        Balance.bIsDeficit = (Balance.NetBalance < 0.0f);
        
        // Calculate reserve days
        if (Balance.ConsumptionRate > 0.0f)
        {
            Balance.ReserveDays = Balance.TotalSupply / Balance.ConsumptionRate / 24.0f; // Convert to days
        }
        else
        {
            Balance.ReserveDays = 999.0f;
        }
        
        // Calculate sustainability index
        float RegenerationTotal = 0.0f;
        float ExtractionTotal = 0.0f;
        for (const FResourceNode& Node : Zone.ResourceNodes)
        {
            if (MapNodeTypeToResource(Node.NodeType) == Resource)
            {
                RegenerationTotal += Node.RegenerationRate;
                ExtractionTotal += Node.ExtractionRate;
            }
        }
        
        if (ExtractionTotal > 0.0f)
        {
            Balance.SustainabilityIndex = RegenerationTotal / ExtractionTotal;
        }
        else
        {
            Balance.SustainabilityIndex = 1.0f;
        }
        
        // Calculate criticality level
        if (Balance.ReserveDays < 1.0f)
        {
            Balance.CriticalityLevel = 1.0f;
        }
        else if (Balance.ReserveDays < 7.0f)
        {
            Balance.CriticalityLevel = 0.7f;
        }
        else if (Balance.ReserveDays < 30.0f)
        {
            Balance.CriticalityLevel = 0.4f;
        }
        else
        {
            Balance.CriticalityLevel = 0.0f;
        }
        
        Zone.ResourceBalances.Add(Resource, Balance);
        
        OnResourceBalanceChanged.Broadcast(ZoneID, Resource, Balance.NetBalance);
    }
}

FResourceBalance UMingEcosystemResourceSystem::GetResourceBalance(const FString& ZoneID, EResourceType Resource) const
{
    if (ResourceZones.Contains(ZoneID))
    {
        const FResourceZone& Zone = ResourceZones[ZoneID];
        if (Zone.ResourceBalances.Contains(Resource))
        {
            return Zone.ResourceBalances[Resource];
        }
    }
    return FResourceBalance();
}

bool UMingEcosystemResourceSystem::IsResourceCritical(const FString& ZoneID, EResourceType Resource) const
{
    FResourceBalance Balance = GetResourceBalance(ZoneID, Resource);
    return Balance.CriticalityLevel > 0.7f || Balance.ReserveDays < 3.0f;
}

TArray<EResourceType> UMingEcosystemResourceSystem::GetDeficitResources(const FString& ZoneID) const
{
    TArray<EResourceType> Deficits;
    
    if (ResourceZones.Contains(ZoneID))
    {
        const FResourceZone& Zone = ResourceZones[ZoneID];
        for (const auto& BalancePair : Zone.ResourceBalances)
        {
            if (BalancePair.Value.bIsDeficit)
            {
                Deficits.Add(BalancePair.Key);
            }
        }
    }
    
    return Deficits;
}

float UMingEcosystemResourceSystem::GetResourceAvailability(const FString& ZoneID, EResourceType Resource) const
{
    FResourceBalance Balance = GetResourceBalance(ZoneID, Resource);
    
    if (Balance.TotalSupply > 0.0f)
    {
        return FMath::Clamp(Balance.NetBalance / Balance.TotalSupply, -1.0f, 1.0f);
    }
    
    return 0.0f;
}

FString UMingEcosystemResourceSystem::CreateResourceTrade(const FString& SourceZoneID, const FString& TargetZoneID, EResourceType Resource, float Amount)
{
    FString TradeID = FGuid::NewGuid().ToString();
    
    FResourceTrade NewTrade;
    NewTrade.TradeID = TradeID;
    NewTrade.SourceZoneID = SourceZoneID;
    NewTrade.TargetZoneID = TargetZoneID;
    NewTrade.Resource = Resource;
    NewTrade.Amount = Amount;
    NewTrade.TradeValue = CalculateTradeValue(Resource, Amount);
    NewTrade.TransportationCost = FVector::Distance(
        GetResourceZone(SourceZoneID).CenterLocation,
        GetResourceZone(TargetZoneID).CenterLocation) * 0.1f;
    NewTrade.Efficiency = 0.9f;
    NewTrade.bIsActive = false;
    
    ActiveTrades.Add(TradeID, NewTrade);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Created trade %s from zone %s to zone %s"),
        *TradeID, *SourceZoneID, *TargetZoneID);
    
    return TradeID;
}

void UMingEcosystemResourceSystem::ExecuteTrade(const FString& TradeID)
{
    if (!ActiveTrades.Contains(TradeID))
    {
        return;
    }
    
    FResourceTrade& Trade = ActiveTrades[TradeID];
    
    // Validate resource availability
    if (!ValidateResourceAvailability(Trade.SourceZoneID, Trade.Resource, Trade.Amount))
    {
        UE_LOG(LogTemp, Warning, TEXT("MingEcosystemResourceSystem: Trade %s failed - insufficient resources"), *TradeID);
        return;
    }
    
    // Process the trade
    ProcessTrade(Trade);
    
    OnResourceTradeCompleted.Broadcast(TradeID, Trade.TradeValue);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Executed trade %s for value %.2f"),
        *TradeID, Trade.TradeValue);
}

void UMingEcosystemResourceSystem::CancelTrade(const FString& TradeID)
{
    ActiveTrades.Remove(TradeID);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Cancelled trade %s"), *TradeID);
}

FResourceTrade UMingEcosystemResourceSystem::GetTradeInfo(const FString& TradeID) const
{
    if (ActiveTrades.Contains(TradeID))
    {
        return ActiveTrades[TradeID];
    }
    return FResourceTrade();
}

FSustainabilityMetrics UMingEcosystemResourceSystem::CalculateSustainability(const FString& ZoneID)
{
    FSustainabilityMetrics Metrics;
    
    if (!ResourceZones.Contains(ZoneID))
    {
        return Metrics;
    }
    
    const FResourceZone& Zone = ResourceZones[ZoneID];
    
    // Calculate overall sustainability
    float TotalSustainability = 0.0f;
    int32 NodeCount = 0;
    float TotalDepletionRate = 0.0f;
    float TotalRegeneration = 0.0f;
    float TotalEnvironmentalImpact = 0.0f;
    float TotalEfficiency = 0.0f;
    
    for (const FResourceNode& Node : Zone.ResourceNodes)
    {
        float NodeSustainability = CalculateNodeSustainability(Node);
        TotalSustainability += NodeSustainability;
        NodeCount++;
        
        // Calculate depletion rate
        if (Node.ExtractionRate > 0.0f && Node.TotalReserve > 0.0f)
        {
            float DepletionRate = Node.ExtractionRate / Node.TotalReserve;
            TotalDepletionRate += DepletionRate;
        }
        
        // Total regeneration
        TotalRegeneration += Node.RegenerationRate;
        
        // Environmental impact
        TotalEnvironmentalImpact += Node.EnvironmentalImpact;
        
        // Efficiency
        TotalEfficiency += Node.ExtractionEfficiency;
    }
    
    if (NodeCount > 0)
    {
        Metrics.OverallSustainability = TotalSustainability / NodeCount;
        Metrics.ResourceEfficiency = TotalEfficiency / NodeCount;
        Metrics.EnvironmentalImpactScore = TotalEnvironmentalImpact / NodeCount;
    }
    
    Metrics.ResourceDepletionRate = TotalDepletionRate;
    
    // Regeneration effectiveness
    float TotalExtraction = 0.0f;
    for (const FResourceNode& Node : Zone.ResourceNodes)
    {
        TotalExtraction += Node.ExtractionRate;
    }
    
    if (TotalExtraction > 0.0f)
    {
        Metrics.RegenerationEffectiveness = TotalRegeneration / TotalExtraction;
    }
    else
    {
        Metrics.RegenerationEffectiveness = 1.0f;
    }
    
    // Long-term viability
    Metrics.LongTermViability = Metrics.OverallSustainability * Metrics.RegenerationEffectiveness;
    
    // Check for sustainability warnings
    if (Metrics.ResourceDepletionRate > 0.1f)
    {
        Metrics.SustainabilityWarnings.Add(TEXT("资源消耗速度过快"));
    }
    if (Metrics.RegenerationEffectiveness < 0.5f)
    {
        Metrics.SustainabilityWarnings.Add(TEXT("再生效率不足"));
    }
    if (Metrics.EnvironmentalImpactScore > 0.5f)
    {
        Metrics.SustainabilityWarnings.Add(TEXT("环境影响过大"));
    }
    
    return Metrics;
}

void UMingEcosystemResourceSystem::ImplementSustainablePractices(const FString& ZoneID, EResourceType Resource)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    FResourceZone& Zone = ResourceZones[ZoneID];
    
    // Improve sustainability for nodes of this resource type
    for (FResourceNode& Node : Zone.ResourceNodes)
    {
        if (MapNodeTypeToResource(Node.NodeType) == Resource)
        {
            // Increase regeneration rate
            Node.RegenerationRate *= 1.5f;
            
            // Reduce environmental impact
            Node.EnvironmentalImpact *= 0.7f;
            
            // Improve sustainability score
            Node.SustainabilityScore = FMath::Min(1.0f, Node.SustainabilityScore + 0.2f);
        }
    }
    
    OnSustainabilityAlert.Broadcast(TEXT("可持续发展措施已实施"));
}

float UMingEcosystemResourceSystem::GetEnvironmentalImpact(const FString& ZoneID) const
{
    if (ResourceZones.Contains(ZoneID))
    {
        return ResourceZones[ZoneID].TotalEnvironmentalImpact;
    }
    return 0.0f;
}

void UMingEcosystemResourceSystem::ReduceEnvironmentalImpact(const FString& ZoneID, float ReductionFactor)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    FResourceZone& Zone = ResourceZones[ZoneID];
    
    for (FResourceNode& Node : Zone.ResourceNodes)
    {
        Node.EnvironmentalImpact *= (1.0f - ReductionFactor);
    }
    
    Zone.TotalEnvironmentalImpact *= (1.0f - ReductionFactor);
}

float UMingEcosystemResourceSystem::GetTotalResourceAmount(const FString& ZoneID, EResourceType Resource) const
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return 0.0f;
    }
    
    const FResourceZone& Zone = ResourceZones[ZoneID];
    float Total = 0.0f;
    
    for (const FResourceNode& Node : Zone.ResourceNodes)
    {
        if (MapNodeTypeToResource(Node.NodeType) == Resource)
        {
            Total += Node.CurrentAmount;
        }
    }
    
    return Total;
}

float UMingEcosystemResourceSystem::GetTotalExtractionRate(const FString& ZoneID, EResourceType Resource) const
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return 0.0f;
    }
    
    const FResourceZone& Zone = ResourceZones[ZoneID];
    float Total = 0.0f;
    
    for (const FResourceNode& Node : Zone.ResourceNodes)
    {
        if (MapNodeTypeToResource(Node.NodeType) == Resource)
        {
            Total += Node.ExtractionRate;
        }
    }
    
    return Total;
}

float UMingEcosystemResourceSystem::GetResourceDiversityIndex(const FString& ZoneID) const
{
    return CalculateDiversityIndex(ZoneID);
}

TMap<EResourceType, float> UMingEcosystemResourceSystem::GetResourceDistribution(const FString& ZoneID) const
{
    TMap<EResourceType, float> Distribution;
    
    if (!ResourceZones.Contains(ZoneID))
    {
        return Distribution;
    }
    
    const FResourceZone& Zone = ResourceZones[ZoneID];
    float TotalAmount = 0.0f;
    
    // Calculate total amounts
    for (const FResourceNode& Node : Zone.ResourceNodes)
    {
        EResourceType Resource = MapNodeTypeToResource(Node.NodeType);
        if (!Distribution.Contains(Resource))
        {
            Distribution.Add(Resource, 0.0f);
        }
        Distribution[Resource] += Node.CurrentAmount;
        TotalAmount += Node.CurrentAmount;
    }
    
    // Convert to percentages
    if (TotalAmount > 0.0f)
    {
        for (auto& Pair : Distribution)
        {
            Pair.Value = Pair.Value / TotalAmount;
        }
    }
    
    return Distribution;
}

void UMingEcosystemResourceSystem::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    CurrentGameTime += DeltaTime;
    
    // Process each zone
    TArray<FString> ZoneIDs;
    ResourceZones.GetKeys(ZoneIDs);
    
    for (const FString& ZoneID : ZoneIDs)
    {
        // Simulate regeneration
        SimulateResourceRegeneration(ZoneID, DeltaTime);
        
        // Update extraction operations
        UpdateExtractionOperations(DeltaTime);
        
        // Process consumption
        ProcessConsumption(ZoneID, DeltaTime);
        
        // Calculate balances
        CalculateResourceBalance(ZoneID);
        
        // Check sustainability thresholds
        CheckSustainabilityThresholds(ZoneID);
    }
    
    // Regenerate depleted nodes
    RegenerateDepletedNodes(DeltaTime);
}

void UMingEcosystemResourceSystem::SimulateNodeRegeneration(FResourceNode& Node, float DeltaTime)
{
    if (!Node.bIsRenewable || Node.bIsDepleted)
    {
        return;
    }
    
    // Check recovery cooldown for depleted nodes
    if (Node.RecoveryCooldown > 0.0f)
    {
        Node.RecoveryCooldown -= DeltaTime;
        return;
    }
    
    // Calculate regeneration amount
    float RegenerationAmount = CalculateNaturalRegeneration(Node.NodeID, DeltaTime);
    
    // Apply regeneration
    float NewAmount = FMath::Min(Node.NaturalCapacity, Node.CurrentAmount + RegenerationAmount);
    
    if (NewAmount > Node.CurrentAmount)
    {
        Node.CurrentAmount = NewAmount;
        
        // Update depletion level
        Node.DepletionLevel = 1.0f - (Node.CurrentAmount / Node.TotalReserve);
        
        // Notify if significant regeneration
        if (RegenerationAmount > Node.NaturalCapacity * 0.01f)
        {
            OnResourceNodeRegenerated.Broadcast(Node.NodeID, Node.CurrentAmount);
        }
    }
}

void UMingEcosystemResourceSystem::UpdateExtractionOperations(float DeltaTime)
{
    for (auto& OpPair : ActiveExtractions)
    {
        FExtractionOperation& Op = OpPair.Value;
        
        if (!Op.bIsActive)
        {
            continue;
        }
        
        // Calculate extraction amount
        float ExtractionAmount = Op.ExtractionRate * Op.Efficiency * DeltaTime;
        
        // Extract from node
        float ActualExtracted = ExtractResource(Op.NodeID, ExtractionAmount);
        
        // Update operation stats
        Op.TotalExtracted += ActualExtracted;
    }
}

void UMingEcosystemResourceSystem::ProcessConsumption(const FString& ZoneID, float DeltaTime)
{
    if (!ZoneConsumptions.Contains(ZoneID))
    {
        return;
    }
    
    auto& Consumptions = ZoneConsumptions[ZoneID];
    
    for (auto& ConsumptionPair : Consumptions)
    {
        FResourceConsumption& Consumption = ConsumptionPair.Value;
        
        // Calculate actual consumption
        float ActualConsumption = Consumption.ConsumptionRate * DeltaTime;
        
        // Apply recycling
        float RecycledAmount = ActualConsumption * Consumption.RecyclingRate;
        float NetConsumption = ActualConsumption - RecycledAmount;
        
        // Track total consumption
        Consumption.TotalConsumed += NetConsumption;
        
        // TODO: Deduct from resource pools
    }
}

void UMingEcosystemResourceSystem::UpdateResourceBalances(const FString& ZoneID)
{
    // Recalculate all balances
    CalculateResourceBalance(ZoneID);
}

float UMingEcosystemResourceSystem::CalculateNodeSustainability(const FResourceNode& Node) const
{
    // Sustainability based on regeneration vs extraction
    if (!Node.bIsRenewable)
    {
        // Non-renewable resources are only sustainable if barely used
        if (Node.ExtractionRate <= 0.0f)
        {
            return 1.0f;
        }
        else
        {
            // Calculate based on remaining reserves
            float ReserveRatio = Node.CurrentAmount / Node.TotalReserve;
            return ReserveRatio * 0.5f;
        }
    }
    
    // For renewable resources
    if (Node.ExtractionRate <= 0.0f)
    {
        return 1.0f; // No extraction = sustainable
    }
    
    float SustainabilityRatio = Node.RegenerationRate / Node.ExtractionRate;
    
    // Cap at 1.0 (100% sustainable)
    return FMath::Min(1.0f, SustainabilityRatio);
}

EResourceType UMingEcosystemResourceSystem::MapNodeTypeToResource(EResourceNodeType NodeType) const
{
    switch (NodeType)
    {
    case EResourceNodeType::Forest:
        return EResourceType::OrganicMatter;
    case EResourceNodeType::Farm:
        return EResourceType::OrganicMatter;
    case EResourceNodeType::WaterSource:
    case EResourceNodeType::FishingGround:
        return EResourceType::Water;
    case EResourceNodeType::Mine:
    case EResourceNodeType::Quarry:
        return EResourceType::Minerals;
    case EResourceNodeType::CrystalFormation:
        return EResourceType::Carbon; // Magical/energy resource
    case EResourceNodeType::MagicalSpring:
        return EResourceType::Water;
    case EResourceNodeType::OilField:
    case EResourceNodeType::GasDeposit:
        return EResourceType::Carbon;
    default:
        return EResourceType::Minerals;
    }
}

float UMingEcosystemResourceSystem::GetExtractionEfficiency(EResourceExtractionMethod Method) const
{
    if (ExtractionEfficiencyMultipliers.Contains(Method))
    {
        return ExtractionEfficiencyMultipliers[Method];
    }
    return 1.0f;
}

float UMingEcosystemResourceSystem::GetEnvironmentalImpactMultiplier(EResourceExtractionMethod Method) const
{
    if (EnvironmentalImpactMultipliers.Contains(Method))
    {
        return EnvironmentalImpactMultipliers[Method];
    }
    return 0.1f;
}

void UMingEcosystemResourceSystem::CheckSustainabilityThresholds(const FString& ZoneID)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    const FResourceZone& Zone = ResourceZones[ZoneID];
    
    // Check for critical resource levels
    for (const auto& BalancePair : Zone.ResourceBalances)
    {
        if (BalancePair.Value.CriticalityLevel > 0.7f)
        {
            FString Warning = FString::Printf(TEXT("资源 %d 处于临界状态"), 
                static_cast<int32>(BalancePair.Key));
            OnSustainabilityAlert.Broadcast(Warning);
        }
    }
    
    // Check overall sustainability
    FSustainabilityMetrics Metrics = CalculateSustainability(ZoneID);
    if (Metrics.OverallSustainability < 0.3f)
    {
        OnSustainabilityAlert.Broadcast(TEXT("区域可持续性严重下降"));
    }
}

void UMingEcosystemResourceSystem::DepleteNode(FResourceNode& Node)
{
    if (Node.bIsDepleted)
    {
        return;
    }
    
    Node.bIsDepleted = true;
    Node.CurrentAmount = 0.0f;
    Node.DepletionLevel = 1.0f;
    Node.RecoveryCooldown = 300.0f; // 5 minutes cooldown before any recovery
    
    OnResourceNodeDepleted.Broadcast(Node.NodeID, Node.NodeType);
    
    UE_LOG(LogTemp, Warning, TEXT("MingEcosystemResourceSystem: Node %s of type %d has been depleted"),
        *Node.NodeID, static_cast<int32>(Node.NodeType));
}

void UMingEcosystemResourceSystem::RegenerateDepletedNodes(float DeltaTime)
{
    for (auto& NodePair : AllResourceNodes)
    {
        FResourceNode& Node = NodePair.Value;
        
        if (Node.bIsDepleted && Node.bIsRenewable)
        {
            // Reduce cooldown
            if (Node.RecoveryCooldown > 0.0f)
            {
                Node.RecoveryCooldown -= DeltaTime;
                continue;
            }
            
            // Begin slow recovery
            float RecoveryAmount = Node.NaturalCapacity * 0.001f * DeltaTime; // Slow recovery
            Node.CurrentAmount = FMath::Min(Node.NaturalCapacity * 0.1f, Node.CurrentAmount + RecoveryAmount);
            
            if (Node.CurrentAmount >= Node.NaturalCapacity * 0.05f)
            {
                Node.bIsDepleted = false;
                Node.DepletionLevel = 0.95f;
                
                OnResourceNodeRegenerated.Broadcast(Node.NodeID, Node.CurrentAmount);
                
                UE_LOG(LogTemp, Log, TEXT("MingEcosystemResourceSystem: Node %s recovering from depletion"),
                    *Node.NodeID);
            }
        }
    }
}

float UMingEcosystemResourceSystem::CalculateDiversityIndex(const FString& ZoneID) const
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return 0.0f;
    }
    
    const FResourceZone& Zone = ResourceZones[ZoneID];
    
    // Count unique resource types
    TSet<EResourceType> ResourceTypes;
    for (const FResourceNode& Node : Zone.ResourceNodes)
    {
        ResourceTypes.Add(MapNodeTypeToResource(Node.NodeType));
    }
    
    int32 TypeCount = ResourceTypes.Num();
    int32 NodeCount = Zone.ResourceNodes.Num();
    
    if (NodeCount == 0)
    {
        return 0.0f;
    }
    
    // Simple diversity index: number of types / log of total nodes
    float Diversity = static_cast<float>(TypeCount) / FMath::Loge(static_cast<float>(FMath::Max(1, NodeCount)));
    
    return FMath::Clamp(Diversity, 0.0f, 1.0f);
}

void UMingEcosystemResourceSystem::ProcessTrade(FResourceTrade& Trade)
{
    // Deduct from source
    if (ResourceZones.Contains(Trade.SourceZoneID))
    {
        FResourceZone& SourceZone = ResourceZones[Trade.SourceZoneID];
        
        for (FResourceNode& Node : SourceZone.ResourceNodes)
        {
            if (MapNodeTypeToResource(Node.NodeType) == Trade.Resource)
            {
                float DeductAmount = FMath::Min(Trade.Amount, Node.CurrentAmount);
                Node.CurrentAmount -= DeductAmount;
                
                // Update global node
                if (AllResourceNodes.Contains(Node.NodeID))
                {
                    AllResourceNodes[Node.NodeID].CurrentAmount = Node.CurrentAmount;
                }
                
                break;
            }
        }
    }
    
    // Add to target
    if (ResourceZones.Contains(Trade.TargetZoneID))
    {
        FResourceZone& TargetZone = ResourceZones[Trade.TargetZoneID];
        
        // Add to first matching node or create new capacity
        for (FResourceNode& Node : TargetZone.ResourceNodes)
        {
            if (MapNodeTypeToResource(Node.NodeType) == Trade.Resource)
            {
                Node.CurrentAmount = FMath::Min(Node.NaturalCapacity, Node.CurrentAmount + Trade.Amount);
                
                if (AllResourceNodes.Contains(Node.NodeID))
                {
                    AllResourceNodes[Node.NodeID].CurrentAmount = Node.CurrentAmount;
                }
                
                break;
            }
        }
    }
    
    Trade.bIsActive = true;
}

float UMingEcosystemResourceSystem::CalculateTradeValue(EResourceType Resource, float Amount) const
{
    // Base values for different resources
    TMap<EResourceType, float> BaseValues;
    BaseValues.Add(EResourceType::Water, 1.0f);
    BaseValues.Add(EResourceType::OrganicMatter, 2.0f);
    BaseValues.Add(EResourceType::Minerals, 5.0f);
    BaseValues.Add(EResourceType::Nitrogen, 3.0f);
    BaseValues.Add(EResourceType::Phosphorus, 4.0f);
    BaseValues.Add(EResourceType::Carbon, 10.0f);
    
    float BaseValue = BaseValues.Contains(Resource) ? BaseValues[Resource] : 1.0f;
    
    return BaseValue * Amount;
}

void UMingEcosystemResourceSystem::UpdateZoneSustainabilityScore(const FString& ZoneID)
{
    if (!ResourceZones.Contains(ZoneID))
    {
        return;
    }
    
    FResourceZone& Zone = ResourceZones[ZoneID];
    
    // Calculate average sustainability of all nodes
    float TotalSustainability = 0.0f;
    float TotalEnvironmentalImpact = 0.0f;
    
    for (FResourceNode& Node : Zone.ResourceNodes)
    {
        TotalSustainability += CalculateNodeSustainability(Node);
        TotalEnvironmentalImpact += Node.EnvironmentalImpact;
    }
    
    if (Zone.ResourceNodes.Num() > 0)
    {
        Zone.SustainabilityScore = TotalSustainability / Zone.ResourceNodes.Num();
        Zone.TotalEnvironmentalImpact = TotalEnvironmentalImpact;
    }
    
    Zone.ResourceDiversityIndex = CalculateDiversityIndex(ZoneID);
}

bool UMingEcosystemResourceSystem::ValidateResourceAvailability(const FString& ZoneID, EResourceType Resource, float Amount) const
{
    float Available = GetTotalResourceAmount(ZoneID, Resource);
    return Available >= Amount;
}
