// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 桂系军阀 - Guangxi Clique Implementation

#include "RepublicEraFactions/Factions/GuangxiClique.h"
#include "MingRTSPlayerController.h"

UGuangxiClique::UGuangxiClique()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UGuangxiClique::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Guangxi Clique faction initialized"));
}

void UGuangxiClique::InitializeAttributes()
{
    Attributes.FactionID = FName("GuangxiClique");
    Attributes.FactionName = FText::FromString(TEXT("桂系军阀"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Guangxi Clique"));
    Attributes.Difficulty = EFactionDifficulty::Normal;
    Attributes.Category = EFactionCategory::RegionalWarlord;
    Attributes.FactionColor = FLinearColor(0.2f, 0.5f, 0.4f, 1.0f);
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("桂林"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("桂林")),
        FText::FromString(TEXT("南寧")),
        FText::FromString(TEXT("廣西"))
    };
    Attributes.StartingPosition.StartingYear = 1924;
    Attributes.StartingPosition.InitialGold = 900;
    Attributes.StartingPosition.InitialManpower = 550;
    Attributes.StartingPosition.InitialReputation = 40;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("廣西民團制度，可以快速動員大量兵力")),
        FText::FromString(TEXT("山地戰專精，在複雜地形中戰鬥力強")),
        FText::FromString(TEXT("李宗仁、白崇禧等將領能力出衆")),
        FText::FromString(TEXT("地理位置險要，易守難攻"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("經濟相對落後，資金收入較低")),
        FText::FromString(TEXT("地處偏遠，擴張選擇有限")),
        FText::FromString(TEXT("現代化程度較低")),
        FText::FromString(TEXT("需要面對周邊多個勢力"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用地形優勢進行防守")),
        FText::FromString(TEXT("發展民團擴充軍力")),
        FText::FromString(TEXT("尋找機會北上擴張"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦喜歡山地戰和防守反擊的玩家"));
}

void UGuangxiClique::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateGuangxiMountainDivision());
    UniqueUnits.Add(CreateGuangxiMilitia());
    UniqueUnits.Add(CreateMountainGuerrillas());
    UniqueUnits.Add(CreateLocalAllianceForces());
}

void UGuangxiClique::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("GuangxiMilitiaSystem");
        Mechanic.MechanicName = FText::FromString(TEXT("廣西民團"));
        Mechanic.Description = FText::FromString(TEXT("全民皆兵制度，可以快速動員大量民兵，成本低但訓練較少"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.5f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("MountainWarfare");
        Mechanic.MechanicName = FText::FromString(TEXT("山地戰專精"));
        Mechanic.Description = FText::FromString(TEXT("在山地、丘陵地區戰鬥力提升30%，移動速度不受地形影響"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.3f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UGuangxiClique::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期建立民團制度，利用地形優勢鞏固廣西。訓練部隊適應山地作戰。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期可以尋找機會北上湖南或東進廣東，擴張勢力範圍。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期利用地形和兵力優勢，在軍閥混戰中謀求最大利益。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用山地地形，設伏擊和防守戰。民團數量龐大適合消耗戰。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("與周邊勢力保持靈活關係，必要時可以聯合對抗強敵。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("發展農業和地方手工業，控制貿易路線增加收入。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("何時開始擴張")),
        FText::FromString(TEXT("選擇哪個方向擴張")),
        FText::FromString(TEXT("如何平衡民團數量和質量"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("統一廣西並向外擴張")),
        FText::FromString(TEXT("成為南方重要勢力")),
        FText::FromString(TEXT("在全國統一進程中發揮重要作用"))
    };
}

FFactionUnit UGuangxiClique::CreateGuangxiMountainDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 115;
    Stats.DefensePower = 110;
    Stats.MovementSpeed = 105;
    Stats.HealthPoints = 105;
    Stats.ProductionCost = 110;
    Stats.UpkeepCost = 11;
    Stats.TrainingTime = 11.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("GuangxiMountainDivision");
    Unit.UnitName = FText::FromString(TEXT("桂軍山地師"));
    Unit.Description = FText::FromString(TEXT("專門訓練的山地部隊，在複雜地形中戰鬥力極強"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Mountain);
    
    return Unit;
}

FFactionUnit UGuangxiClique::CreateGuangxiMilitia() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 80;
    Stats.DefensePower = 75;
    Stats.MovementSpeed = 90;
    Stats.HealthPoints = 80;
    Stats.ProductionCost = 40;
    Stats.UpkeepCost = 4;
    Stats.TrainingTime = 3.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("GuangxiMilitia");
    Unit.UnitName = FText::FromString(TEXT("廣西民團"));
    Unit.Description = FText::FromString(TEXT("全民皆兵的民兵組織，成本低，可以快速大量動員"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UGuangxiClique::CreateMountainGuerrillas() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 105;
    Stats.DefensePower = 85;
    Stats.MovementSpeed = 115;
    Stats.HealthPoints = 85;
    Stats.ProductionCost = 75;
    Stats.UpkeepCost = 6;
    Stats.TrainingTime = 6.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("MountainGuerrillas");
    Unit.UnitName = FText::FromString(TEXT("山地游擊隊"));
    Unit.Description = FText::FromString(TEXT("熟悉本地地形的游擊部隊，擅長伏擊和騷擾戰術"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Guerrilla);
    Unit.Traits.Add(EUnitTrait::Mountain);
    
    return Unit;
}

FFactionUnit UGuangxiClique::CreateLocalAllianceForces() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 90;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 85;
    Stats.HealthPoints = 90;
    Stats.ProductionCost = 85;
    Stats.UpkeepCost = 7;
    Stats.TrainingTime = 8.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("LocalAllianceForces");
    Unit.UnitName = FText::FromString(TEXT("地方勢力聯盟"));
    Unit.Description = FText::FromString(TEXT("與地方勢力聯合的部隊，熟悉本地情況，適合協同作戰"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnitStats UGuangxiClique::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.MovementSpeed = FMath::RoundToInt(BaseStats.MovementSpeed * 1.1f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UGuangxiClique::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("MilitiaRecruitmentSpeed"), 1.5f);
    Bonuses.Add(FName("MountainCombatBonus"), 1.3f);
    Bonuses.Add(FName("TerrainMovementBonus"), 1.1f);
    
    return Bonuses;
}

FGameplayGuide UGuangxiClique::GetGameplayGuide() const
{
    return GameplayGuide;
}
