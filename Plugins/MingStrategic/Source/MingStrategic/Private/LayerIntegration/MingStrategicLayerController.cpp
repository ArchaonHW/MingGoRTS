#include "LayerIntegration/MingStrategicLayerController.h"

UMingStrategicLayerController::UMingStrategicLayerController()
    : bIsActive(false)
{
}

void UMingStrategicLayerController::InitializeStrategicLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Strategic Layer Controller"));
    
    // Initialize default data
    InitializeDefaultFactions();
    InitializeDefaultRegions();
    SetupDiplomaticDefaults();
    
    UE_LOG(LogTemp, Log, TEXT("Strategic Layer initialized with %d regions and %d factions"), 
        Regions.Num(), Factions.Num());
}

void UMingStrategicLayerController::ShutdownStrategicLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Strategic Layer Controller"));
    
    ClearStrategicMap();
    
    Regions.Empty();
    Factions.Empty();
    DiplomaticRelations.Empty();
    
    bIsActive = false;
}

void UMingStrategicLayerController::ActivateStrategicLayer()
{
    bIsActive = true;
    UE_LOG(LogTemp, Log, TEXT("Strategic Layer activated"));
}

void UMingStrategicLayerController::DeactivateStrategicLayer()
{
    bIsActive = false;
    UE_LOG(LogTemp, Log, TEXT("Strategic Layer deactivated"));
}

// Map Management
void UMingStrategicLayerController::LoadStrategicMap(const FString& MapID)
{
    UE_LOG(LogTemp, Log, TEXT("Loading strategic map: %s"), *MapID);
    
    CurrentMapID = MapID;
    
    // In a real implementation, this would load map data from assets
    // For now, we initialize with defaults
    InitializeDefaultRegions();
}

void UMingStrategicLayerController::ClearStrategicMap()
{
    Regions.Empty();
    SelectedRegionID.Empty();
    UE_LOG(LogTemp, Log, TEXT("Strategic map cleared"));
}

void UMingStrategicLayerController::AddRegion(const FMingMapRegion& Region)
{
    if (Region.RegionID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot add region with empty ID"));
        return;
    }
    
    Regions.Add(Region.RegionID, Region);
    UE_LOG(LogTemp, Log, TEXT("Added region: %s"), *Region.RegionName);
}

void UMingStrategicLayerController::RemoveRegion(const FString& RegionID)
{
    Regions.Remove(RegionID);
    
    if (SelectedRegionID == RegionID)
    {
        SelectedRegionID.Empty();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Removed region: %s"), *RegionID);
}

bool UMingStrategicLayerController::GetRegion(const FString& RegionID, FMingMapRegion& OutRegion) const
{
    const FMingMapRegion* FoundRegion = Regions.Find(RegionID);
    if (FoundRegion)
    {
        OutRegion = *FoundRegion;
        return true;
    }
    return false;
}

TArray<FMingMapRegion> UMingStrategicLayerController::GetAllRegions() const
{
    TArray<FMingMapRegion> Result;
    Regions.GenerateValueArray(Result);
    return Result;
}

TArray<FMingMapRegion> UMingStrategicLayerController::GetRegionsByFaction(const FString& FactionID) const
{
    TArray<FMingMapRegion> Result;
    
    for (const auto& Pair : Regions)
    {
        if (Pair.Value.ControllingFaction == FactionID)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

TArray<FMingMapRegion> UMingStrategicLayerController::GetContestedRegions() const
{
    TArray<FMingMapRegion> Result;
    
    for (const auto& Pair : Regions)
    {
        if (Pair.Value.bIsContested)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

// Faction Management
void UMingStrategicLayerController::RegisterFaction(const FMingFactionInfo& Faction)
{
    if (Faction.FactionID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot register faction with empty ID"));
        return;
    }
    
    Factions.Add(Faction.FactionID, Faction);
    
    if (Faction.bIsPlayer)
    {
        PlayerFactionID = Faction.FactionID;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Registered faction: %s"), *Faction.FactionName);
}

void UMingStrategicLayerController::UnregisterFaction(const FString& FactionID)
{
    Factions.Remove(FactionID);
    
    // Remove related diplomatic relations
    TArray<FString> KeysToRemove;
    for (const auto& Pair : DiplomaticRelations)
    {
        if (Pair.Value.FactionA == FactionID || Pair.Value.FactionB == FactionID)
        {
            KeysToRemove.Add(Pair.Key);
        }
    }
    
    for (const FString& Key : KeysToRemove)
    {
        DiplomaticRelations.Remove(Key);
    }
    
    if (SelectedFactionID == FactionID)
    {
        SelectedFactionID.Empty();
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unregistered faction: %s"), *FactionID);
}

bool UMingStrategicLayerController::GetFaction(const FString& FactionID, FMingFactionInfo& OutFaction) const
{
    const FMingFactionInfo* FoundFaction = Factions.Find(FactionID);
    if (FoundFaction)
    {
        OutFaction = *FoundFaction;
        return true;
    }
    return false;
}

FMingFactionInfo UMingStrategicLayerController::GetPlayerFaction() const
{
    FMingFactionInfo PlayerFaction;
    GetFaction(PlayerFactionID, PlayerFaction);
    return PlayerFaction;
}

TArray<FMingFactionInfo> UMingStrategicLayerController::GetAllFactions() const
{
    TArray<FMingFactionInfo> Result;
    Factions.GenerateValueArray(Result);
    return Result;
}

void UMingStrategicLayerController::UpdateFactionStrength(const FString& FactionID, int32 MilitaryDelta, int32 EconomicDelta)
{
    FMingFactionInfo* Faction = Factions.Find(FactionID);
    if (Faction)
    {
        Faction->MilitaryStrength = FMath::Max(0, Faction->MilitaryStrength + MilitaryDelta);
        Faction->EconomicStrength = FMath::Max(0, Faction->EconomicStrength + EconomicDelta);
        
        NotifyFactionChanged(FactionID);
    }
}

// Diplomacy Management
void UMingStrategicLayerController::SetDiplomaticRelation(const FString& FactionA, const FString& FactionB, EMingFactionRelation Relation)
{
    FString Key = GenerateRelationKey(FactionA, FactionB);
    
    FMingDiplomaticRelation DiploRelation;
    DiploRelation.FactionA = FactionA;
    DiploRelation.FactionB = FactionB;
    DiploRelation.RelationType = Relation;
    
    switch (Relation)
    {
    case EMingFactionRelation::Allied:
        DiploRelation.bHasAlliance = true;
        DiploRelation.RelationValue = 80;
        break;
    case EMingFactionRelation::Friendly:
        DiploRelation.RelationValue = 50;
        break;
    case EMingFactionRelation::Neutral:
        DiploRelation.RelationValue = 0;
        break;
    case EMingFactionRelation::Hostile:
        DiploRelation.RelationValue = -50;
        break;
    case EMingFactionRelation::AtWar:
        DiploRelation.bIsAtWar = true;
        DiploRelation.RelationValue = -100;
        break;
    }
    
    DiplomaticRelations.Add(Key, DiploRelation);
    
    UE_LOG(LogTemp, Log, TEXT("Set relation between %s and %s to %d"), 
        *FactionA, *FactionB, (int32)Relation);
}

FMingDiplomaticRelation UMingStrategicLayerController::GetDiplomaticRelation(const FString& FactionA, const FString& FactionB) const
{
    FString Key = GenerateRelationKey(FactionA, FactionB);
    const FMingDiplomaticRelation* Found = DiplomaticRelations.Find(Key);
    
    if (Found)
    {
        return *Found;
    }
    
    // Return default neutral relation
    FMingDiplomaticRelation Default;
    Default.FactionA = FactionA;
    Default.FactionB = FactionB;
    return Default;
}

void UMingStrategicLayerController::DeclareWar(const FString& AttackerFaction, const FString& DefenderFaction)
{
    SetDiplomaticRelation(AttackerFaction, DefenderFaction, EMingFactionRelation::AtWar);
    
    // Update war status
    FString Key = GenerateRelationKey(AttackerFaction, DefenderFaction);
    FMingDiplomaticRelation* Relation = DiplomaticRelations.Find(Key);
    if (Relation)
    {
        Relation->bIsAtWar = true;
        Relation->WarDurationDays = 0;
    }
    
    UE_LOG(LogTemp, Log, TEXT("War declared: %s vs %s"), *AttackerFaction, *DefenderFaction);
    
    OnDiplomaticAction.Broadcast(DefenderFaction, EMingStrategicAction::DeclareWar);
}

void UMingStrategicLayerController::NegotiatePeace(const FString& FactionA, const FString& FactionB)
{
    FString Key = GenerateRelationKey(FactionA, FactionB);
    FMingDiplomaticRelation* Relation = DiplomaticRelations.Find(Key);
    
    if (Relation && Relation->bIsAtWar)
    {
        Relation->bIsAtWar = false;
        Relation->WarDurationDays = 0;
        Relation->RelationType = EMingFactionRelation::Hostile;
        Relation->RelationValue = -50;
        
        UE_LOG(LogTemp, Log, TEXT("Peace negotiated between %s and %s"), *FactionA, *FactionB);
    }
}

void UMingStrategicLayerController::FormAlliance(const FString& FactionA, const FString& FactionB, int32 DurationDays)
{
    SetDiplomaticRelation(FactionA, FactionB, EMingFactionRelation::Allied);
    
    FString Key = GenerateRelationKey(FactionA, FactionB);
    FMingDiplomaticRelation* Relation = DiplomaticRelations.Find(Key);
    if (Relation)
    {
        Relation->bHasAlliance = true;
        Relation->ActiveTreaties.Add(FString::Printf(TEXT("Alliance (%d days)"), DurationDays));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Alliance formed: %s and %s for %d days"), 
        *FactionA, *FactionB, DurationDays);
    
    OnDiplomaticAction.Broadcast(FactionB, EMingStrategicAction::FormAlliance);
}

void UMingStrategicLayerController::EstablishTradeAgreement(const FString& FactionA, const FString& FactionB)
{
    FString Key = GenerateRelationKey(FactionA, FactionB);
    FMingDiplomaticRelation* Relation = DiplomaticRelations.Find(Key);
    
    if (Relation)
    {
        Relation->bHasTradeAgreement = true;
        Relation->ActiveTreaties.Add(TEXT("Trade Agreement"));
        
        if (Relation->RelationType == EMingFactionRelation::Neutral)
        {
            Relation->RelationType = EMingFactionRelation::Friendly;
            Relation->RelationValue = 30;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Trade agreement established: %s and %s"), *FactionA, *FactionB);
    
    OnDiplomaticAction.Broadcast(FactionB, EMingStrategicAction::TradeAgreement);
}

void UMingStrategicLayerController::BreakTreaty(const FString& FactionA, const FString& FactionB, const FString& TreatyType)
{
    FString Key = GenerateRelationKey(FactionA, FactionB);
    FMingDiplomaticRelation* Relation = DiplomaticRelations.Find(Key);
    
    if (Relation)
    {
        if (TreatyType == TEXT("Alliance"))
        {
            Relation->bHasAlliance = false;
            Relation->RelationType = EMingFactionRelation::Neutral;
        }
        else if (TreatyType == TEXT("Trade"))
        {
            Relation->bHasTradeAgreement = false;
        }
        
        Relation->ActiveTreaties.Remove(TreatyType);
        Relation->RelationValue -= 20; // Breaking treaty damages relations
        
        UE_LOG(LogTemp, Log, TEXT("Treaty broken: %s between %s and %s"), *TreatyType, *FactionA, *FactionB);
    }
}

TArray<FMingDiplomaticRelation> UMingStrategicLayerController::GetFactionRelations(const FString& FactionID) const
{
    TArray<FMingDiplomaticRelation> Result;
    
    for (const auto& Pair : DiplomaticRelations)
    {
        if (Pair.Value.FactionA == FactionID || Pair.Value.FactionB == FactionID)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

TArray<FString> UMingStrategicLayerController::GetAllies(const FString& FactionID) const
{
    TArray<FString> Result;
    
    for (const auto& Pair : DiplomaticRelations)
    {
        if (Pair.Value.RelationType == EMingFactionRelation::Allied)
        {
            if (Pair.Value.FactionA == FactionID)
            {
                Result.Add(Pair.Value.FactionB);
            }
            else if (Pair.Value.FactionB == FactionID)
            {
                Result.Add(Pair.Value.FactionA);
            }
        }
    }
    
    return Result;
}

TArray<FString> UMingStrategicLayerController::GetEnemies(const FString& FactionID) const
{
    TArray<FString> Result;
    
    for (const auto& Pair : DiplomaticRelations)
    {
        if (Pair.Value.bIsAtWar)
        {
            if (Pair.Value.FactionA == FactionID)
            {
                Result.Add(Pair.Value.FactionB);
            }
            else if (Pair.Value.FactionB == FactionID)
            {
                Result.Add(Pair.Value.FactionA);
            }
        }
    }
    
    return Result;
}

// Strategic Actions
void UMingStrategicLayerController::ExecuteStrategicDecision(const FMingStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("Executing strategic decision: %s"), *Decision.DecisionName);
    
    OnStrategicOrderIssued.Broadcast(Decision);
    
    // Execute based on action type
    switch (Decision.ActionType)
    {
    case EMingStrategicAction::MoveTroops:
        if (Decision.InvolvedRegions.Num() >= 2)
        {
            MoveTroops(Decision.InvolvedRegions[0], Decision.InvolvedRegions[1], Decision.TroopRequirement, TEXT("Player"));
        }
        break;
        
    case EMingStrategicAction::DeclareWar:
        DeclareWar(TEXT("Player"), Decision.TargetFaction);
        break;
        
    case EMingStrategicAction::FormAlliance:
        FormAlliance(TEXT("Player"), Decision.TargetFaction, 30);
        break;
        
    default:
        UE_LOG(LogTemp, Log, TEXT("Strategic action type %d executed"), (int32)Decision.ActionType);
        break;
    }
}

void UMingStrategicLayerController::MoveTroops(const FString& FromRegion, const FString& ToRegion, int32 TroopCount, const FString& FactionID)
{
    FMingMapRegion* SourceRegion = Regions.Find(FromRegion);
    FMingMapRegion* TargetRegion = Regions.Find(ToRegion);
    
    if (SourceRegion && TargetRegion)
    {
        // Check if regions are connected
        if (SourceRegion->ConnectedRegions.Contains(ToRegion))
        {
            UE_LOG(LogTemp, Log, TEXT("Moving %d troops from %s to %s"), 
                TroopCount, *SourceRegion->RegionName, *TargetRegion->RegionName);
            
            // In real implementation, this would actually move troops
            // Update garrison strengths
            SourceRegion->GarrisonStrength = FMath::Max(0, SourceRegion->GarrisonStrength - TroopCount);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Regions %s and %s are not connected"), *FromRegion, *ToRegion);
        }
    }
}

void UMingStrategicLayerController::FortifyRegion(const FString& RegionID, int32 GarrisonIncrease)
{
    FMingMapRegion* Region = Regions.Find(RegionID);
    if (Region)
    {
        Region->GarrisonStrength += GarrisonIncrease;
        UE_LOG(LogTemp, Log, TEXT("Fortified %s. Garrison now: %d"), 
            *Region->RegionName, Region->GarrisonStrength);
        
        NotifyRegionChanged(RegionID);
    }
}

void UMingStrategicLayerController::DevelopRegion(const FString& RegionID, const FString& DevelopmentType)
{
    FMingMapRegion* Region = Regions.Find(RegionID);
    if (Region)
    {
        if (DevelopmentType == TEXT("Economic"))
        {
            Region->ResourceValue += 10;
        }
        else if (DevelopmentType == TEXT("Military"))
        {
            Region->StrategicValue += 10;
        }
        
        UE_LOG(LogTemp, Log, TEXT("Developed %s: %s"), *Region->RegionName, *DevelopmentType);
        NotifyRegionChanged(RegionID);
    }
}

void UMingStrategicLayerController::LaunchCampaign(const FString& FromRegion, const FString& TargetRegion, int32 TroopCount)
{
    UE_LOG(LogTemp, Log, TEXT("Launching campaign from %s to %s with %d troops"), 
        *FromRegion, *TargetRegion, TroopCount);
    
    FMingMapRegion* Target = Regions.Find(TargetRegion);
    if (Target)
    {
        Target->bIsContested = true;
        
        // Create a strategic decision for the battle
        FMingStrategicDecision Decision;
        Decision.DecisionID = FGuid::NewGuid().ToString();
        Decision.DecisionName = FString::Printf(TEXT("Campaign: %s"), *Target->RegionName);
        Decision.ActionType = EMingStrategicAction::None; // This triggers tactical layer
        Decision.InvolvedRegions = { FromRegion, TargetRegion };
        Decision.TroopRequirement = TroopCount;
        
        OnStrategicOrderIssued.Broadcast(Decision);
    }
}

// UI Interface
void UMingStrategicLayerController::SelectRegion(const FString& RegionID)
{
    SelectedRegionID = RegionID;
    
    FMingMapRegion Region;
    if (GetRegion(RegionID, Region))
    {
        OnRegionSelected.Broadcast(Region);
        UE_LOG(LogTemp, Log, TEXT("Selected region: %s"), *Region.RegionName);
    }
}

void UMingStrategicLayerController::SelectFaction(const FString& FactionID)
{
    SelectedFactionID = FactionID;
    
    FMingFactionInfo Faction;
    if (GetFaction(FactionID, Faction))
    {
        OnFactionSelected.Broadcast(Faction);
        UE_LOG(LogTemp, Log, TEXT("Selected faction: %s"), *Faction.FactionName);
    }
}

void UMingStrategicLayerController::ClearSelection()
{
    SelectedRegionID.Empty();
    SelectedFactionID.Empty();
    UE_LOG(LogTemp, Log, TEXT("Selection cleared"));
}

// Internal initialization
void UMingStrategicLayerController::InitializeDefaultFactions()
{
    // Player faction (Republican Army)
    FMingFactionInfo PlayerFaction;
    PlayerFaction.FactionID = TEXT("Player");
    PlayerFaction.FactionName = TEXT("國民革命軍");
    PlayerFaction.FactionColor = FLinearColor(0.0f, 0.3f, 0.7f);
    PlayerFaction.MilitaryStrength = 1000;
    PlayerFaction.EconomicStrength = 500;
    PlayerFaction.DiplomaticInfluence = 300;
    PlayerFaction.bIsPlayer = true;
    RegisterFaction(PlayerFaction);
    
    // Warlord faction
    FMingFactionInfo WarlordFaction;
    WarlordFaction.FactionID = TEXT("Warlord");
    WarlordFaction.FactionName = TEXT("北洋軍閥");
    WarlordFaction.FactionColor = FLinearColor(0.7f, 0.2f, 0.1f);
    WarlordFaction.MilitaryStrength = 800;
    WarlordFaction.EconomicStrength = 400;
    WarlordFaction.DiplomaticInfluence = 200;
    RegisterFaction(WarlordFaction);
    
    // Japanese faction
    FMingFactionInfo JapaneseFaction;
    JapaneseFaction.FactionID = TEXT("Japanese");
    JapaneseFaction.FactionName = TEXT("關東軍");
    JapaneseFaction.FactionColor = FLinearColor(0.9f, 0.1f, 0.1f);
    JapaneseFaction.MilitaryStrength = 1200;
    JapaneseFaction.EconomicStrength = 600;
    JapaneseFaction.DiplomaticInfluence = 250;
    RegisterFaction(JapaneseFaction);
    
    // Communist faction
    FMingFactionInfo CommunistFaction;
    CommunistFaction.FactionID = TEXT("Communist");
    CommunistFaction.FactionName = TEXT("工農紅軍");
    CommunistFaction.FactionColor = FLinearColor(0.9f, 0.1f, 0.1f);
    CommunistFaction.MilitaryStrength = 400;
    CommunistFaction.EconomicStrength = 200;
    CommunistFaction.DiplomaticInfluence = 150;
    RegisterFaction(CommunistFaction);
}

void UMingStrategicLayerController::InitializeDefaultRegions()
{
    // Beijing
    FMingMapRegion Beijing;
    Beijing.RegionID = TEXT("Beijing");
    Beijing.RegionName = TEXT("北平");
    Beijing.RegionType = EMingMapRegionType::City;
    Beijing.MapPosition = FVector2D(500, 300);
    Beijing.ControllingFaction = TEXT("Warlord");
    Beijing.ResourceValue = 50;
    Beijing.StrategicValue = 100;
    Beijing.GarrisonStrength = 200;
    Beijing.bIsCapital = true;
    Beijing.ConnectedRegions = { TEXT("Tianjin"), TEXT("Shijiazhuang") };
    AddRegion(Beijing);
    
    // Shanghai
    FMingMapRegion Shanghai;
    Shanghai.RegionID = TEXT("Shanghai");
    Shanghai.RegionName = TEXT("上海");
    Shanghai.RegionType = EMingMapRegionType::City;
    Shanghai.MapPosition = FVector2D(600, 500);
    Shanghai.ControllingFaction = TEXT("Player");
    Shanghai.ResourceValue = 80;
    Shanghai.StrategicValue = 90;
    Shanghai.GarrisonStrength = 150;
    Shanghai.ConnectedRegions = { TEXT("Nanjing"), TEXT("Hangzhou") };
    AddRegion(Shanghai);
    
    // Nanjing
    FMingMapRegion Nanjing;
    Nanjing.RegionID = TEXT("Nanjing");
    Nanjing.RegionName = TEXT("南京");
    Nanjing.RegionType = EMingMapRegionType::City;
    Nanjing.MapPosition = FVector2D(550, 480);
    Nanjing.ControllingFaction = TEXT("Player");
    Nanjing.ResourceValue = 60;
    Nanjing.StrategicValue = 85;
    Nanjing.GarrisonStrength = 180;
    Nanjing.ConnectedRegions = { TEXT("Shanghai"), TEXT("Hefei") };
    AddRegion(Nanjing);
    
    // Wuhan
    FMingMapRegion Wuhan;
    Wuhan.RegionID = TEXT("Wuhan");
    Wuhan.RegionName = TEXT("武漢");
    Wuhan.RegionType = EMingMapRegionType::StrategicPass;
    Wuhan.MapPosition = FVector2D(450, 450);
    Wuhan.ControllingFaction = TEXT("Warlord");
    Wuhan.ResourceValue = 40;
    Wuhan.StrategicValue = 95;
    Wuhan.GarrisonStrength = 100;
    Wuhan.ConnectedRegions = { TEXT("Nanjing"), TEXT("Changsha") };
    AddRegion(Wuhan);
}

void UMingStrategicLayerController::SetupDiplomaticDefaults()
{
    // Player vs Warlord - At War
    DeclareWar(TEXT("Player"), TEXT("Warlord"));
    
    // Player vs Communist - Neutral
    SetDiplomaticRelation(TEXT("Player"), TEXT("Communist"), EMingFactionRelation::Neutral);
    
    // Warlord vs Japanese - Allied
    FormAlliance(TEXT("Warlord"), TEXT("Japanese"), 60);
}

FString UMingStrategicLayerController::GenerateRelationKey(const FString& FactionA, const FString& FactionB) const
{
    // Sort to ensure consistent key regardless of order
    if (FactionA < FactionB)
    {
        return FString::Printf(TEXT("%s_%s"), *FactionA, *FactionB);
    }
    else
    {
        return FString::Printf(TEXT("%s_%s"), *FactionB, *FactionA);
    }
}

void UMingStrategicLayerController::NotifyRegionChanged(const FString& RegionID)
{
    FMingMapRegion Region;
    if (GetRegion(RegionID, Region))
    {
        OnRegionSelected.Broadcast(Region);
    }
}

void UMingStrategicLayerController::NotifyFactionChanged(const FString& FactionID)
{
    FMingFactionInfo Faction;
    if (GetFaction(FactionID, Faction))
    {
        OnFactionSelected.Broadcast(Faction);
    }
}
