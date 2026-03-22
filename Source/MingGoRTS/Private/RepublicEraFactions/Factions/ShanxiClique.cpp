// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 晋系军阀 - Shanxi Clique Implementation

#include "RepublicEraFactions/Factions/ShanxiClique.h"
#include "MingRTSPlayerController.h"

UShanxiClique::UShanxiClique()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UShanxiClique::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Shanxi Clique faction initialized"));
}

void UShanxiClique::InitializeAttributes()
{
    Attributes.FactionID = FName("ShanxiClique");
    Attributes.FactionName = FText::FromString(TEXT("晋系军阀"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Shanxi Clique"));
    Attributes.Difficulty = EFactionDifficulty::Normal;
    Attributes.Category = EFactionCategory::RegionalWarlord;
    Attributes.FactionColor = FLinearColor(0.4f, 0.3f, 0.6f, 1.0f);
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("太原"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("太原")),
        FText::FromString(TEXT("大同")),
        FText::FromString(TEXT("山西"))
    };
    Attributes.StartingPosition.StartingYear = 1911;
    Attributes.StartingPosition.InitialGold = 1000;
    Attributes.StartingPosition.InitialManpower = 500;
    Attributes.StartingPosition.InitialReputation = 45;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("山西模范省治理，行政效率高")),
        FText::FromString(TEXT("防御工事坚固，易守难攻")),
        FText::FromString(TEXT("地理位置封闭，不易受多方攻击")),
        FText::FromString(TEXT("阎锡山长期统治，内部相对稳定"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("地理位置偏远，扩张困难")),
        FText::FromString(TEXT("资源相对匮乏")),
        FText::FromString(TEXT("人口基数较小")),
        FText::FromString(TEXT("现代化程度相对较低"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("巩固山西根据地，建立坚固防线")),
        FText::FromString(TEXT("适时参与中原争霸")),
        FText::FromString(TEXT("发展地方经济和教育"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推荐喜欢防守和稳健发展的玩家"));
}

void UShanxiClique::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateShanxiDefenseDivision());
    UniqueUnits.Add(CreateShanxiEngineerCorps());
    UniqueUnits.Add(CreateShanxiMerchantGuards());
    UniqueUnits.Add(CreateLocalSecurityForces());
}

void UShanxiClique::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("ShanxiModel");
        Mechanic.MechanicName = FText::FromString(TEXT("山西模范"));
        Mechanic.Description = FText::FromString(TEXT("行政和经济管理效率高，建设速度加快25%，腐败程度较低"));
        Mechanic.MechanicType = EFactionMechanicType::Economic;
        Mechanic.EffectMultiplier = 1.25f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("DefensiveSpecialist");
        Mechanic.MechanicName = FText::FromString(TEXT("防御专精"));
        Mechanic.Description = FText::FromString(TEXT("防御工事效果提升40%，守军战斗力提升20%"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.4f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UShanxiClique::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期巩固山西根据地，修建防御工事。利用封闭地形保护自身发展。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期根据形势参与中原事务，可以支持一方打击另一方，保存实力。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("后期利用坚固防线抵御强敌，等待时机出击或联合其他势力。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用防御工事，以守为攻。工程兵可以快速修建防御设施。在山西地形中作战优势明显。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("保持灵活的外交政策，在各方势力间周旋。必要时可以暂时依附强者。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("发展山西本地经济，提高行政效率。教育投资可以带来长期收益。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("何时参与中原争霸")),
        FText::FromString(TEXT("选择哪个势力作为盟友")),
        FText::FromString(TEXT("是否接受外部援助的代价"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("巩固山西并向外扩张")),
        FText::FromString(TEXT("在军阀混战中生存到最后")),
        FText::FromString(TEXT("统一北方或保持独立"))
    };
}

FFactionUnit UShanxiClique::CreateShanxiDefenseDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 100;
    Stats.DefensePower = 130;
    Stats.MovementSpeed = 90;
    Stats.HealthPoints = 115;
    Stats.ProductionCost = 105;
    Stats.UpkeepCost = 10;
    Stats.TrainingTime = 11.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("ShanxiDefenseDivision");
    Unit.UnitName = FText::FromString(TEXT("晋军防守师"));
    Unit.Description = FText::FromString(TEXT("专精防御作战的部队，防御力极强，适合守备要塞"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Defensive);
    
    return Unit;
}

FFactionUnit UShanxiClique::CreateShanxiEngineerCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 80;
    Stats.DefensePower = 100;
    Stats.MovementSpeed = 70;
    Stats.HealthPoints = 90;
    Stats.ProductionCost = 120;
    Stats.UpkeepCost = 12;
    Stats.TrainingTime = 14.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("ShanxiEngineerCorps");
    Unit.UnitName = FText::FromString(TEXT("山西工兵队"));
    Unit.Description = FText::FromString(TEXT("专业工程兵部队，可以快速修建防御工事、桥梁和道路"));
    Unit.UnitType = EFactionUnitType::Support;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Engineer);
    
    return Unit;
}

FFactionUnit UShanxiClique::CreateShanxiMerchantGuards() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 90;
    Stats.DefensePower = 95;
    Stats.MovementSpeed = 85;
    Stats.HealthPoints = 90;
    Stats.ProductionCost = 95;
    Stats.UpkeepCost = 8;
    Stats.TrainingTime = 9.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("ShanxiMerchantGuards");
    Unit.UnitName = FText::FromString(TEXT("晋商资助队"));
    Unit.Description = FText::FromString(TEXT("由晋商资助的地方武装，保护商业路线，对经济有加成作用"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UShanxiClique::CreateLocalSecurityForces() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 75;
    Stats.DefensePower = 110;
    Stats.MovementSpeed = 75;
    Stats.HealthPoints = 100;
    Stats.ProductionCost = 70;
    Stats.UpkeepCost = 6;
    Stats.TrainingTime = 7.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("LocalSecurityForces");
    Unit.UnitName = FText::FromString(TEXT("地方保安团"));
    Unit.Description = FText::FromString(TEXT("地方治安部队，成本低，适合守备后方和维持秩序"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Defensive);
    
    return Unit;
}

FFactionUnitStats UShanxiClique::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    // Defensive units get +15% defense
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.DefensePower = FMath::RoundToInt(BaseStats.DefensePower * 1.15f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UShanxiClique::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("ConstructionSpeedBonus"), 1.25f);
    Bonuses.Add(FName("DefenseBonus"), 1.2f);
    Bonuses.Add(FName("CorruptionReduction"), 0.8f);
    
    return Bonuses;
}

FGameplayGuide UShanxiClique::GetGameplayGuide() const
{
    return GameplayGuide;
}
