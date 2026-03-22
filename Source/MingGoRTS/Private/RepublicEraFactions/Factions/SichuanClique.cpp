// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 川系军阀 - Sichuan Clique Implementation

#include "RepublicEraFactions/Factions/SichuanClique.h"
#include "MingRTSPlayerController.h"

USichuanClique::USichuanClique()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void USichuanClique::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Sichuan Clique faction initialized"));
}

void USichuanClique::InitializeAttributes()
{
    Attributes.FactionID = FName("SichuanClique");
    Attributes.FactionName = FText::FromString(TEXT("川系军阀"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Sichuan Clique"));
    Attributes.Difficulty = EFactionDifficulty::Hard;
    Attributes.Category = EFactionCategory::RegionalWarlord;
    Attributes.FactionColor = FLinearColor(0.5f, 0.4f, 0.3f, 1.0f);
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("成都"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("成都")),
        FText::FromString(TEXT("重慶")),
        FText::FromString(TEXT("四川"))
    };
    Attributes.StartingPosition.StartingYear = 1918;
    Attributes.StartingPosition.InitialGold = 900;
    Attributes.StartingPosition.InitialManpower = 500;
    Attributes.StartingPosition.InitialReputation = 35;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("四川盆地資源豐富，經濟自足")),
        FText::FromString(TEXT("地形險要，易守難攻")),
        FText::FromString(TEXT("防區制可以分散管理壓力")),
        FText::FromString(TEXT("人口眾多，兵力充足"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("內部軍閥林立，統一困難")),
        FText::FromString(TEXT("防區制導致指揮不統一")),
        FText::FromString(TEXT("容易發生內部衝突")),
        FText::FromString(TEXT("對外擴張能力有限"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("逐步統一內部各防區")),
        FText::FromString(TEXT("利用地形優勢自保")),
        FText::FromString(TEXT("發展經濟增強實力"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡內政治理和防守的玩家"));
}

void USichuanClique::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateSichuanDefenseZoneDivision());
    UniqueUnits.Add(CreateSichuanMilitia());
    UniqueUnits.Add(CreateWarlordCoalitionForces());
    UniqueUnits.Add(CreateSichuanRiverNavy());
}

void USichuanClique::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("DefenseZoneSystem");
        Mechanic.MechanicName = FText::FromString(TEXT("防區制"));
        Mechanic.Description = FText::FromString(TEXT("將四川劃分為多個防區，各有軍閥管理，分散風險但統一指揮較難"));
        Mechanic.MechanicType = EFactionMechanicType::Political;
        Mechanic.EffectMultiplier = 1.2f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("InternalUnity");
        Mechanic.MechanicName = FText::FromString(TEXT("內部統一"));
        Mechanic.Description = FText::FromString(TEXT("需要維持內部各軍閥的團結，成功時戰鬥力提升，失敗時可能發生內亂"));
        Mechanic.MechanicType = EFactionMechanicType::Political;
        Mechanic.EffectMultiplier = 1.3f;
        UniqueMechanics.Add(Mechanic);
    }
}

void USichuanClique::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期重點維持內部穩定，協調各防區軍閥關係。利用四川地形自保。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期逐步整合內部勢力，發展經濟增強實力。可以尋找機會對外擴張。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期完成內部統一後，可以參與全國性角逐。四川的資源和人口是巨大的優勢。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用四川地形，以守為攻。內部統一時戰鬥力強大，但要注意防止內部分裂。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("對內要平衡各軍閥利益，對外要保持靈活立場。必要時可以聯合外部勢力壓制內部反對派。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("發展農業和手工業，利用四川豐富的資源。長江航運可以帶來額外收入。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("如何平衡各防區軍閥的利益")),
        FText::FromString(TEXT("是否強行統一內部勢力")),
        FText::FromString(TEXT("對外擴張的時機選擇"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("完成四川內部統一")),
        FText::FromString(TEXT("建立強大的西南根據地")),
        FText::FromString(TEXT("在全國統一中發揮重要作用"))
    };
}

FFactionUnit USichuanClique::CreateSichuanDefenseZoneDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 105;
    Stats.DefensePower = 115;
    Stats.MovementSpeed = 90;
    Stats.HealthPoints = 110;
    Stats.ProductionCost = 105;
    Stats.UpkeepCost = 10;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("SichuanDefenseZoneDivision");
    Unit.UnitName = FText::FromString(TEXT("川軍防區師"));
    Unit.Description = FText::FromString(TEXT("各防區的地方部隊，熟悉本地地形，防守能力強"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Defensive);
    
    return Unit;
}

FFactionUnit USichuanClique::CreateSichuanMilitia() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 80;
    Stats.DefensePower = 80;
    Stats.MovementSpeed = 85;
    Stats.HealthPoints = 85;
    Stats.ProductionCost = 45;
    Stats.UpkeepCost = 4;
    Stats.TrainingTime = 4.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("SichuanMilitia");
    Unit.UnitName = FText::FromString(TEXT("四川民團"));
    Unit.Description = FText::FromString(TEXT("四川本地的民兵組織，成本低，適合守衛地方"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit USichuanClique::CreateWarlordCoalitionForces() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 110;
    Stats.DefensePower = 100;
    Stats.MovementSpeed = 95;
    Stats.HealthPoints = 100;
    Stats.ProductionCost = 115;
    Stats.UpkeepCost = 11;
    Stats.TrainingTime = 11.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("WarlordCoalitionForces");
    Unit.UnitName = FText::FromString(TEXT("地方軍閥聯軍"));
    Unit.Description = FText::FromString(TEXT("各防區軍閥聯合組成的部隊，戰鬥力較強但協調較難"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit USichuanClique::CreateSichuanRiverNavy() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 90;
    Stats.DefensePower = 85;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 80;
    Stats.ProductionCost = 120;
    Stats.UpkeepCost = 12;
    Stats.TrainingTime = 12.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("SichuanRiverNavy");
    Unit.UnitName = FText::FromString(TEXT("川東水師"));
    Unit.Description = FText::FromString(TEXT("長江上游的水軍部隊，控制航道，可以進行水上運輸和作戰"));
    Unit.UnitType = EFactionUnitType::Naval;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnitStats USichuanClique::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.DefensePower = FMath::RoundToInt(BaseStats.DefensePower * 1.1f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> USichuanClique::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("EconomicSelfSufficiency"), 1.2f);
    Bonuses.Add(FName("DefenseBonus"), 1.15f);
    Bonuses.Add(FName("PopulationBonus"), 1.1f);
    
    return Bonuses;
}

FGameplayGuide USichuanClique::GetGameplayGuide() const
{
    return GameplayGuide;
}
