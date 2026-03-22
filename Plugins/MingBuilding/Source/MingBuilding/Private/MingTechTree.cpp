#include "MingTechTree.h"
#include "MingBuildingActor.h"
#include "MingTactical/Public/MingTacticalUnit.h"
#include "Engine/World.h"

UMingTechTree::UMingTechTree()
{
}

void UMingTechTree::InitializeDefaultTechTree()
{
    TechNodes.Empty();
    UnlockedBuildings.Empty();
    UnlockedAbilities.Empty();
    BuildingUpgrades.Empty();

    // Setup default tech trees by category
    SetupDefaultMilitaryTechs();
    SetupDefaultEconomyTechs();
    SetupDefaultInfrastructureTechs();
    SetupDefaultDefenseTechs();

    // Command Center is always unlocked
    UnlockedBuildings.Add(EMingBuildingType::CommandCenter);

    UE_LOG(LogTemp, Log, TEXT("Default tech tree initialized with %d techs"), TechNodes.Num());
}

void UMingTechTree::SetupDefaultMilitaryTechs()
{
    // Tier 1: Basic Military
    FMingTechNode BarracksTech;
    BarracksTech.TechID = TEXT("MIL_BARRACKS");
    BarracksTech.TechName = TEXT("兵營建設");
    BarracksTech.Description = TEXT("解鎖兵營，可訓練基礎步兵單位");
    BarracksTech.Category = EMingTechCategory::Military;
    BarracksTech.ResearchTime = 20.0f;
    BarracksTech.TechLevel = 1;
    BarracksTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 200});
    BarracksTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Materials, 100});
    BarracksTech.Unlock.UnlockedBuilding = EMingBuildingType::Barracks;
    AddTechNode(BarracksTech);

    // Tier 2: Advanced Infantry
    FMingTechNode AdvancedInfantry;
    AdvancedInfantry.TechID = TEXT("MIL_ADV_INFANTRY");
    AdvancedInfantry.TechName = TEXT("高級步兵訓練");
    AdvancedInfantry.Description = TEXT("提升步兵單位的生命值和攻擊力");
    AdvancedInfantry.Category = EMingTechCategory::Military;
    AdvancedInfantry.ResearchTime = 30.0f;
    AdvancedInfantry.TechLevel = 2;
    AdvancedInfantry.Prerequisites.Add(TEXT("MIL_BARRACKS"));
    AdvancedInfantry.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 400});
    AdvancedInfantry.ResearchCost.Add(FMingResourceCost{EMingResourceType::Materials, 200});
    AdvancedInfantry.Unlock.StatName = TEXT("InfantryHealth");
    AdvancedInfantry.Unlock.StatModifier = 0.2f;
    AddTechNode(AdvancedInfantry);

    // Tier 2: Factory
    FMingTechNode FactoryTech;
    FactoryTech.TechID = TEXT("MIL_FACTORY");
    FactoryTech.TechName = TEXT("軍工廠");
    FactoryTech.Description = TEXT("解鎖軍工廠，可生產裝甲單位");
    FactoryTech.Category = EMingTechCategory::Military;
    FactoryTech.ResearchTime = 40.0f;
    FactoryTech.TechLevel = 2;
    FactoryTech.Prerequisites.Add(TEXT("MIL_BARRACKS"));
    FactoryTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 500});
    FactoryTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Materials, 300});
    FactoryTech.Unlock.UnlockedBuilding = EMingBuildingType::Factory;
    AddTechNode(FactoryTech);
}

void UMingTechTree::SetupDefaultEconomyTechs()
{
    // Tier 1: Resource Gathering
    FMingTechNode ResourceTech;
    ResourceTech.TechID = TEXT("ECO_RESOURCES");
    ResourceTech.TechName = TEXT("資源採集技術");
    ResourceTech.Description = TEXT("解鎖資源採集建築");
    ResourceTech.Category = EMingTechCategory::Economy;
    ResourceTech.ResearchTime = 15.0f;
    ResourceTech.TechLevel = 1;
    ResourceTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 150});
    ResourceTech.Unlock.UnlockedBuilding = EMingBuildingType::ResourceGatherer;
    AddTechNode(ResourceTech);

    // Tier 2: Advanced Economy
    FMingTechNode SupplyDepot;
    SupplyDepot.TechID = TEXT("ECO_SUPPLY");
    SupplyDepot.TechName = TEXT("供應倉庫");
    SupplyDepot.Description = TEXT("增加資源存儲容量");
    SupplyDepot.Category = EMingTechCategory::Economy;
    SupplyDepot.ResearchTime = 25.0f;
    SupplyDepot.TechLevel = 2;
    SupplyDepot.Prerequisites.Add(TEXT("ECO_RESOURCES"));
    SupplyDepot.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 300});
    SupplyDepot.ResearchCost.Add(FMingResourceCost{EMingResourceType::Materials, 150});
    SupplyDepot.Unlock.UnlockedBuilding = EMingBuildingType::SupplyDepot;
    SupplyDepot.Unlock.StatName = TEXT("ResourceCapacity");
    SupplyDepot.Unlock.StatModifier = 0.5f;
    AddTechNode(SupplyDepot);

    // Tier 3: Trade
    FMingTechNode TradeTech;
    TradeTech.TechID = TEXT("ECO_TRADE");
    TradeTech.TechName = TEXT("貿易路線");
    TradeTech.Description = TEXT("提升資金收入");
    TradeTech.Category = EMingTechCategory::Economy;
    TradeTech.ResearchTime = 35.0f;
    TradeTech.TechLevel = 3;
    TradeTech.Prerequisites.Add(TEXT("ECO_SUPPLY"));
    TradeTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 600});
    TradeTech.Unlock.StatName = TEXT("MoneyProduction");
    TradeTech.Unlock.StatModifier = 0.3f;
    AddTechNode(TradeTech);
}

void UMingTechTree::SetupDefaultInfrastructureTechs()
{
    // Infrastructure techs focus on building efficiency and speed
    FMingTechNode FastBuild;
    FastBuild.TechID = TEXT("INF_FASTBUILD");
    FastBuild.TechName = TEXT("快速建造");
    FastBuild.Description = TEXT("加快建築建造速度20%");
    FastBuild.Category = EMingTechCategory::Infrastructure;
    FastBuild.ResearchTime = 20.0f;
    FastBuild.TechLevel = 1;
    FastBuild.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 200});
    FastBuild.ResearchCost.Add(FMingResourceCost{EMingResourceType::Materials, 100});
    FastBuild.Unlock.StatName = TEXT("BuildSpeed");
    FastBuild.Unlock.StatModifier = 0.2f;
    AddTechNode(FastBuild);
}

void UMingTechTree::SetupDefaultDefenseTechs()
{
    // Defense Tower
    FMingTechNode DefenseTech;
    DefenseTech.TechID = TEXT("DEF_TOWER");
    DefenseTech.TechName = TEXT("防禦塔");
    DefenseTech.Description = TEXT("解鎖防禦塔建築");
    DefenseTech.Category = EMingTechCategory::Defense;
    DefenseTech.ResearchTime = 30.0f;
    DefenseTech.TechLevel = 1;
    DefenseTech.Prerequisites.Add(TEXT("MIL_BARRACKS"));
    DefenseTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 300});
    DefenseTech.ResearchCost.Add(FMingResourceCost{EMingResourceType::Materials, 200});
    DefenseTech.Unlock.UnlockedBuilding = EMingBuildingType::DefenseTower;
    AddTechNode(DefenseTech);

    // Fortification
    FMingTechNode Fortification;
    Fortification.TechID = TEXT("DEF_FORT");
    Fortification.TechName = TEXT("建築加固");
    Fortification.Description = TEXT("提升所有建築生命值25%");
    Fortification.Category = EMingTechCategory::Defense;
    Fortification.ResearchTime = 40.0f;
    Fortification.TechLevel = 2;
    Fortification.Prerequisites.Add(TEXT("DEF_TOWER"));
    Fortification.ResearchCost.Add(FMingResourceCost{EMingResourceType::Money, 400});
    Fortification.ResearchCost.Add(FMingResourceCost{EMingResourceType::Materials, 300});
    Fortification.Unlock.StatName = TEXT("BuildingHealth");
    Fortification.Unlock.StatModifier = 0.25f;
    AddTechNode(Fortification);
}

void UMingTechTree::LoadTechTree(const TArray<FMingTechNode>& TechNodesToLoad)
{
    TechNodes.Empty();
    for (const FMingTechNode& Node : TechNodesToLoad)
    {
        TechNodes.Add(Node.TechID, Node);
    }
    RecalculateUnlockedBuildings();
}

void UMingTechTree::AddTechNode(const FMingTechNode& Node)
{
    TechNodes.Add(Node.TechID, Node);
}

bool UMingTechTree::RemoveTechNode(const FString& TechID)
{
    if (TechNodes.Contains(TechID))
    {
        TechNodes.Remove(TechID);
        return true;
    }
    return false;
}

FMingTechNode UMingTechTree::GetTechNode(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    return Node ? *Node : FMingTechNode();
}

TArray<FMingTechNode> UMingTechTree::GetAllTechNodes() const
{
    TArray<FMingTechNode> Result;
    for (const auto& Pair : TechNodes)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FMingTechNode> UMingTechTree::GetTechNodesByCategory(EMingTechCategory Category) const
{
    TArray<FMingTechNode> Result;
    for (const auto& Pair : TechNodes)
    {
        if (Pair.Value.Category == Category)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingTechNode> UMingTechTree::GetAvailableTechs() const
{
    TArray<FMingTechNode> Result;
    for (const auto& Pair : TechNodes)
    {
        const FMingTechNode& Node = Pair.Value;
        if (!Node.bIsResearched && !Node.bIsResearching && ArePrerequisitesMet(Node.TechID))
        {
            Result.Add(Node);
        }
    }
    return Result;
}

TArray<FMingTechNode> UMingTechTree::GetResearchedTechs() const
{
    TArray<FMingTechNode> Result;
    for (const auto& Pair : TechNodes)
    {
        if (Pair.Value.bIsResearched)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

bool UMingTechTree::CanResearchTech(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    if (!Node)
    {
        return false;
    }

    if (Node->bIsResearched || Node->bIsResearching)
    {
        return false;
    }

    if (!ArePrerequisitesMet(TechID))
    {
        return false;
    }

    return HasEnoughResources(Node->ResearchCost);
}

bool UMingTechTree::StartResearch(const FString& TechID)
{
    if (!CanResearchTech(TechID))
    {
        return false;
    }

    FMingTechNode* Node = TechNodes.Find(TechID);
    if (!Node)
    {
        return false;
    }

    // Consume resources
    if (!ConsumeResources(Node->ResearchCost))
    {
        return false;
    }

    Node->bIsResearching = true;
    Node->ResearchProgress = 0.0f;
    CurrentlyResearching.Add(TechID);

    UE_LOG(LogTemp, Log, TEXT("Started research: %s"), *Node->TechName);

    return true;
}

void UMingTechTree::CancelResearch(const FString& TechID)
{
    FMingTechNode* Node = TechNodes.Find(TechID);
    if (Node && Node->bIsResearching)
    {
        Node->bIsResearching = false;
        Node->ResearchProgress = 0.0f;
        CurrentlyResearching.Remove(TechID);

        // Partial refund (50%)
        UE_LOG(LogTemp, Log, TEXT("Cancelled research: %s"), *Node->TechName);
    }
}

void UMingTechTree::UpdateResearch(float DeltaTime)
{
    TArray<FString> CompletedResearch;

    for (const FString& TechID : CurrentlyResearching)
    {
        FMingTechNode* Node = TechNodes.Find(TechID);
        if (Node && Node->bIsResearching)
        {
            Node->ResearchProgress += DeltaTime / Node->ResearchTime;

            // Broadcast progress
            OnResearchProgress.Broadcast(*Node);

            if (Node->ResearchProgress >= 1.0f)
            {
                CompletedResearch.Add(TechID);
            }
        }
    }

    // Complete finished research
    for (const FString& TechID : CompletedResearch)
    {
        CompleteResearch(TechID);
    }
}

void UMingTechTree::CompleteResearch(const FString& TechID)
{
    FMingTechNode* Node = TechNodes.Find(TechID);
    if (!Node)
    {
        return;
    }

    Node->bIsResearching = false;
    Node->bIsResearched = true;
    Node->ResearchProgress = 1.0f;
    CurrentlyResearching.Remove(TechID);

    // Apply unlock
    ApplyTechUnlock(Node->Unlock);

    // Broadcast event
    OnTechResearched.Broadcast(*Node);

    UE_LOG(LogTemp, Log, TEXT("Research completed: %s"), *Node->TechName);
}

void UMingTechTree::ApplyTechUnlock(const FMingTechUnlock& Unlock)
{
    if (Unlock.UnlockedBuilding != EMingBuildingType::CommandCenter)
    {
        UnlockBuilding(Unlock.UnlockedBuilding);
    }

    if (Unlock.UnlockedUnit)
    {
        UnlockUnit(Unlock.UnlockedUnit);
    }

    if (!Unlock.UnlockedAbility.IsEmpty())
    {
        UnlockAbility(Unlock.UnlockedAbility);
    }
}

void UMingTechTree::UnlockBuilding(EMingBuildingType BuildingType)
{
    if (!UnlockedBuildings.Contains(BuildingType))
    {
        UnlockedBuildings.Add(BuildingType);
        OnBuildingUnlocked.Broadcast(BuildingType);
        UE_LOG(LogTemp, Log, TEXT("Building unlocked: %s"), *UEnum::GetValueAsString(BuildingType));
    }
}

void UMingTechTree::UnlockUnit(TSubclassOf<AMingTacticalUnit> UnitClass)
{
    // Store unlocked unit class for use by spawners
    UE_LOG(LogTemp, Log, TEXT("Unit unlocked: %s"), UnitClass ? *UnitClass->GetName() : TEXT("None"));
}

void UMingTechTree::UnlockAbility(const FString& AbilityName)
{
    if (!UnlockedAbilities.Contains(AbilityName))
    {
        UnlockedAbilities.Add(AbilityName);
        UE_LOG(LogTemp, Log, TEXT("Ability unlocked: %s"), *AbilityName);
    }
}

bool UMingTechTree::IsTechResearched(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    return Node ? Node->bIsResearched : false;
}

bool UMingTechTree::IsTechResearching(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    return Node ? Node->bIsResearching : false;
}

float UMingTechTree::GetResearchProgress(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    return Node ? Node->ResearchProgress : 0.0f;
}

bool UMingTechTree::IsBuildingUnlocked(EMingBuildingType BuildingType) const
{
    return UnlockedBuildings.Contains(BuildingType);
}

TArray<EMingBuildingType> UMingTechTree::GetUnlockedBuildings() const
{
    TArray<EMingBuildingType> Result;
    for (EMingBuildingType Building : UnlockedBuildings)
    {
        Result.Add(Building);
    }
    return Result;
}

TArray<EMingBuildingType> UMingTechTree::GetLockedBuildings() const
{
    TArray<EMingBuildingType> Result;
    UEnum* EnumPtr = StaticEnum<EMingBuildingType>();
    if (EnumPtr)
    {
        for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
        {
            EMingBuildingType BuildingType = static_cast<EMingBuildingType>(EnumPtr->GetValueByIndex(i));
            if (!UnlockedBuildings.Contains(BuildingType))
            {
                Result.Add(BuildingType);
            }
        }
    }
    return Result;
}

FString UMingTechTree::GetBuildingUnlockTech(EMingBuildingType BuildingType) const
{
    for (const auto& Pair : TechNodes)
    {
        if (Pair.Value.Unlock.UnlockedBuilding == BuildingType)
        {
            return Pair.Key;
        }
    }
    return FString();
}

bool UMingTechTree::ArePrerequisitesMet(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    if (!Node)
    {
        return false;
    }

    for (const FString& Prereq : Node->Prerequisites)
    {
        if (!IsTechResearched(Prereq))
        {
            return false;
        }
    }

    return true;
}

TArray<FString> UMingTechTree::GetMissingPrerequisites(const FString& TechID) const
{
    TArray<FString> Missing;
    const FMingTechNode* Node = TechNodes.Find(TechID);
    if (Node)
    {
        for (const FString& Prereq : Node->Prerequisites)
        {
            if (!IsTechResearched(Prereq))
            {
                Missing.Add(Prereq);
            }
        }
    }
    return Missing;
}

TArray<FMingResourceCost> UMingTechTree::GetResearchCost(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    return Node ? Node->ResearchCost : TArray<FMingResourceCost>();
}

float UMingTechTree::GetResearchTime(const FString& TechID) const
{
    const FMingTechNode* Node = TechNodes.Find(TechID);
    return Node ? Node->ResearchTime : 0.0f;
}

float UMingTechTree::GetStatModifier(const FString& StatName) const
{
    float TotalModifier = 0.0f;
    for (const auto& Pair : TechNodes)
    {
        const FMingTechNode& Node = Pair.Value;
        if (Node.bIsResearched && Node.Unlock.StatName == StatName)
        {
            TotalModifier += Node.Unlock.StatModifier;
        }
    }
    return TotalModifier;
}

bool UMingTechTree::HasAbilityUnlocked(const FString& AbilityName) const
{
    return UnlockedAbilities.Contains(AbilityName);
}

FString UMingTechTree::GetTechCategoryName(EMingTechCategory Category)
{
    switch (Category)
    {
    case EMingTechCategory::Military: return TEXT("軍事");
    case EMingTechCategory::Economy: return TEXT("經濟");
    case EMingTechCategory::Infrastructure: return TEXT("基礎設施");
    case EMingTechCategory::Defense: return TEXT("防禦");
    case EMingTechCategory::Special: return TEXT("特殊");
    default: return TEXT("未知");
    }
}

FLinearColor UMingTechTree::GetTechCategoryColor(EMingTechCategory Category)
{
    switch (Category)
    {
    case EMingTechCategory::Military: return FLinearColor(0.8f, 0.2f, 0.2f); // Red
    case EMingTechCategory::Economy: return FLinearColor(1.0f, 0.8f, 0.0f); // Gold
    case EMingTechCategory::Infrastructure: return FLinearColor(0.3f, 0.6f, 0.9f); // Blue
    case EMingTechCategory::Defense: return FLinearColor(0.5f, 0.5f, 0.5f); // Gray
    case EMingTechCategory::Special: return FLinearColor(0.8f, 0.2f, 0.8f); // Purple
    default: return FLinearColor::White;
    }
}

FString UMingTechTree::SerializeTechTree() const
{
    // Simple JSON-like serialization
    FString Result = TEXT("{\n");
    Result += TEXT("  \"researched_techs\": [");

    bool First = true;
    for (const auto& Pair : TechNodes)
    {
        if (Pair.Value.bIsResearched)
        {
            if (!First) Result += TEXT(",");
            Result += FString::Printf(TEXT("\"%s\""), *Pair.Key);
            First = false;
        }
    }

    Result += TEXT("],\n");
    Result += FString::Printf(TEXT("  \"unlocked_buildings\": %d\n"), UnlockedBuildings.Num());
    Result += TEXT("}\n");

    return Result;
}

void UMingTechTree::DeserializeTechTree(const FString& JsonString)
{
    // Simplified deserialization
    UE_LOG(LogTemp, Log, TEXT("Deserializing tech tree"));
}

void UMingTechTree::RecalculateUnlockedBuildings()
{
    UnlockedBuildings.Empty();
    UnlockedBuildings.Add(EMingBuildingType::CommandCenter);

    for (const auto& Pair : TechNodes)
    {
        if (Pair.Value.bIsResearched)
        {
            if (Pair.Value.Unlock.UnlockedBuilding != EMingBuildingType::CommandCenter)
            {
                UnlockedBuildings.Add(Pair.Value.Unlock.UnlockedBuilding);
            }
        }
    }
}

bool UMingTechTree::HasEnoughResources(const TArray<FMingResourceCost>& Costs) const
{
    // This would check against the resource system
    // For now, placeholder implementation
    return true;
}

bool UMingTechTree::ConsumeResources(const TArray<FMingResourceCost>& Costs)
{
    // This would consume from the resource system
    // For now, placeholder implementation
    return true;
}

// Building Upgrade methods
void UMingTechTree::RegisterBuildingUpgrade(EMingBuildingType BuildingType, const FMingBuildingUpgrade& Upgrade)
{
    if (!BuildingUpgrades.Contains(BuildingType))
    {
        BuildingUpgrades.Add(BuildingType, TArray<FMingBuildingUpgrade>());
    }
    BuildingUpgrades[BuildingType].Add(Upgrade);
}

bool UMingTechTree::CanUpgradeBuilding(const FString& UpgradeID) const
{
    // Find upgrade by ID
    for (const auto& Pair : BuildingUpgrades)
    {
        for (const FMingBuildingUpgrade& Upgrade : Pair.Value)
        {
            if (Upgrade.UpgradeID == UpgradeID)
            {
                return Upgrade.CurrentLevel < Upgrade.MaxUpgradeLevel;
            }
        }
    }
    return false;
}

bool UMingTechTree::UpgradeBuilding(const FString& UpgradeID)
{
    for (auto& Pair : BuildingUpgrades)
    {
        for (FMingBuildingUpgrade& Upgrade : Pair.Value)
        {
            if (Upgrade.UpgradeID == UpgradeID && Upgrade.CurrentLevel < Upgrade.MaxUpgradeLevel)
            {
                Upgrade.CurrentLevel++;
                OnBuildingUpgraded.Broadcast(Pair.Key, Upgrade.CurrentLevel);
                return true;
            }
        }
    }
    return false;
}

FMingBuildingUpgrade UMingTechTree::GetBuildingUpgrade(const FString& UpgradeID) const
{
    for (const auto& Pair : BuildingUpgrades)
    {
        for (const FMingBuildingUpgrade& Upgrade : Pair.Value)
        {
            if (Upgrade.UpgradeID == UpgradeID)
            {
                return Upgrade;
            }
        }
    }
    return FMingBuildingUpgrade();
}

TArray<FMingBuildingUpgrade> UMingTechTree::GetAvailableUpgradesForBuilding(EMingBuildingType BuildingType) const
{
    const TArray<FMingBuildingUpgrade>* Upgrades = BuildingUpgrades.Find(BuildingType);
    return Upgrades ? *Upgrades : TArray<FMingBuildingUpgrade>();
}

int32 UMingTechTree::GetUpgradeLevel(const FString& UpgradeID) const
{
    for (const auto& Pair : BuildingUpgrades)
    {
        for (const FMingBuildingUpgrade& Upgrade : Pair.Value)
        {
            if (Upgrade.UpgradeID == UpgradeID)
            {
                return Upgrade.CurrentLevel;
            }
        }
    }
    return 0;
}

void UMingTechTree::SetupDefaultBuildingUpgrades()
{
    // 指揮中心升級 | Command Center Upgrades
    {
        FMingBuildingUpgrade CommandCenterHealth;
        CommandCenterHealth.UpgradeID = TEXT("CommandCenter_Health");
        CommandCenterHealth.UpgradeName = TEXT("強化結構 | Reinforced Structure");
        CommandCenterHealth.Description = TEXT("增加指揮中心生命值 | Increase Command Center health");
        CommandCenterHealth.MaxUpgradeLevel = 3;
        CommandCenterHealth.CurrentLevel = 0;
        CommandCenterHealth.HealthMultiplier = 1.25f;
        CommandCenterHealth.ProductionMultiplier = 1.0f;
        CommandCenterHealth.DefenseMultiplier = 1.1f;
        CommandCenterHealth.UpgradeTime = 30.0f;
        
        FMingResourceCost Cost1;
        Cost1.ResourceType = EMingResourceType::Materials;
        Cost1.Amount = 500;
        CommandCenterHealth.UpgradeCost.Add(Cost1);
        
        RegisterBuildingUpgrade(EMingBuildingType::CommandCenter, CommandCenterHealth);
    }
    
    // 兵營升級 | Barracks Upgrades
    {
        FMingBuildingUpgrade BarracksTraining;
        BarracksTraining.UpgradeID = TEXT("Barracks_Training");
        BarracksTraining.UpgradeName = TEXT("高效訓練 | Efficient Training");
        BarracksTraining.Description = TEXT("加快部隊訓練速度 | Faster unit training");
        BarracksTraining.MaxUpgradeLevel = 3;
        BarracksTraining.CurrentLevel = 0;
        BarracksTraining.HealthMultiplier = 1.0f;
        BarracksTraining.ProductionMultiplier = 1.3f;
        BarracksTraining.DefenseMultiplier = 1.0f;
        BarracksTraining.UpgradeTime = 25.0f;
        
        FMingResourceCost Cost2;
        Cost2.ResourceType = EMingResourceType::Materials;
        Cost2.Amount = 300;
        BarracksTraining.UpgradeCost.Add(Cost2);
        
        FMingResourceCost Cost2b;
        Cost2b.ResourceType = EMingResourceType::Money;
        Cost2b.Amount = 200;
        BarracksTraining.UpgradeCost.Add(Cost2b);
        
        RegisterBuildingUpgrade(EMingBuildingType::Barracks, BarracksTraining);
    }
    
    {
        FMingBuildingUpgrade BarracksDefense;
        BarracksDefense.UpgradeID = TEXT("Barracks_Defense");
        BarracksDefense.UpgradeName = TEXT("防禦工事 | Defensive Works");
        BarracksDefense.Description = TEXT("增加兵營防禦力 | Increase barracks defense");
        BarracksDefense.MaxUpgradeLevel = 2;
        BarracksDefense.CurrentLevel = 0;
        BarracksDefense.HealthMultiplier = 1.15f;
        BarracksDefense.ProductionMultiplier = 1.0f;
        BarracksDefense.DefenseMultiplier = 1.25f;
        BarracksDefense.UpgradeTime = 20.0f;
        
        FMingResourceCost Cost3;
        Cost3.ResourceType = EMingResourceType::Materials;
        Cost3.Amount = 400;
        BarracksDefense.UpgradeCost.Add(Cost3);
        
        RegisterBuildingUpgrade(EMingBuildingType::Barracks, BarracksDefense);
    }
    
    // 資源建築升級 | Resource Building Upgrades
    {
        FMingBuildingUpgrade FarmProduction;
        FarmProduction.UpgradeID = TEXT("Farm_Production");
        FarmProduction.UpgradeName = TEXT"現代農業 | Modern Agriculture");
        FarmProduction.Description = TEXT("提高糧食產量 | Increase food production");
        FarmProduction.MaxUpgradeLevel = 3;
        FarmProduction.CurrentLevel = 0;
        FarmProduction.HealthMultiplier = 1.0f;
        FarmProduction.ProductionMultiplier = 1.4f;
        FarmProduction.DefenseMultiplier = 1.0f;
        FarmProduction.UpgradeTime = 20.0f;
        
        FMingResourceCost Cost4;
        Cost4.ResourceType = EMingResourceType::Money;
        Cost4.Amount = 150;
        FarmProduction.UpgradeCost.Add(Cost4);
        
        RegisterBuildingUpgrade(EMingBuildingType::Farm, FarmProduction);
    }
    
    {
        FMingBuildingUpgrade MineProduction;
        MineProduction.UpgradeID = TEXT("Mine_Production");
        MineProduction.UpgradeName = TEXT("深層開採 | Deep Mining");
        MineProduction.Description = TEXT("提高礦產產量 | Increase mining output");
        MineProduction.MaxUpgradeLevel = 3;
        MineProduction.CurrentLevel = 0;
        MineProduction.HealthMultiplier = 1.0f;
        MineProduction.ProductionMultiplier = 1.35f;
        MineProduction.DefenseMultiplier = 1.0f;
        MineProduction.UpgradeTime = 25.0f;
        
        FMingResourceCost Cost5;
        Cost5.ResourceType = EMingResourceType::Materials;
        Cost5.Amount = 250;
        MineProduction.UpgradeCost.Add(Cost5);
        
        RegisterBuildingUpgrade(EMingBuildingType::Mine, MineProduction);
    }
    
    // 防禦建築升級 | Defense Building Upgrades
    {
        FMingBuildingUpgrade WallDefense;
        WallDefense.UpgradeID = TEXT("Wall_Defense");
        WallDefense.UpgradeName = TEXT("加固城牆 | Fortified Walls");
        WallDefense.Description = TEXT("大幅提升城牆防禦 | Greatly increase wall defense");
        WallDefense.MaxUpgradeLevel = 2;
        WallDefense.CurrentLevel = 0;
        WallDefense.HealthMultiplier = 1.3f;
        WallDefense.ProductionMultiplier = 1.0f;
        WallDefense.DefenseMultiplier = 1.4f;
        WallDefense.UpgradeTime = 40.0f;
        
        FMingResourceCost Cost6;
        Cost6.ResourceType = EMingResourceType::Materials;
        Cost6.Amount = 600;
        WallDefense.UpgradeCost.Add(Cost6);
        
        FMingResourceCost Cost6b;
        Cost6b.ResourceType = EMingResourceType::Money;
        Cost6b.Amount = 300;
        WallDefense.UpgradeCost.Add(Cost6b);
        
        RegisterBuildingUpgrade(EMingBuildingType::Wall, WallDefense);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Default building upgrades configured successfully"));
}
