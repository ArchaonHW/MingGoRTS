// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 滇系军阀 - Yunnan Clique Implementation

#include "RepublicEraFactions/Factions/YunnanClique.h"
#include "MingRTSPlayerController.h"

UYunnanClique::UYunnanClique()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UYunnanClique::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Yunnan Clique faction initialized"));
}

void UYunnanClique::InitializeAttributes()
{
    Attributes.FactionID = FName("YunnanClique");
    Attributes.FactionName = FText::FromString(TEXT("滇系军阀"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Yunnan Clique"));
    Attributes.Difficulty = EFactionDifficulty::Hard;
    Attributes.Category = EFactionCategory::RegionalWarlord;
    Attributes.FactionColor = FLinearColor(0.4f, 0.6f, 0.2f, 1.0f);
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("昆明"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("昆明")),
        FText::FromString(TEXT("雲南"))
    };
    Attributes.StartingPosition.StartingYear = 1915;
    Attributes.StartingPosition.InitialGold = 850;
    Attributes.StartingPosition.InitialManpower = 450;
    Attributes.StartingPosition.InitialReputation = 50;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("護國傳統，士氣高昂，政治合法性強")),
        FText::FromString(TEXT("可以招撫周邊少數民族力量")),
        FText::FromString(TEXT("雲南地形複雜，易守難攻")),
        FText::FromString(TEXT("對外擴張有正當性"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("地理位置偏遠，經濟發展受限")),
        FText::FromString(TEXT("人口較少，兵力有限")),
        FText::FromString(TEXT("現代化程度較低")),
        FText::FromString(TEXT("擴張需要面對複雜的民族關係"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用護國傳統凝聚人心")),
        FText::FromString(TEXT("招撫少數民族擴充實力")),
        FText::FromString(TEXT("伺機向周邊地區擴張"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡邊疆擴張和多民族聯合的玩家"));
}

void UYunnanClique::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateYunnanBorderDefenseDivision());
    UniqueUnits.Add(CreateEthnicMinorityCoalition());
    UniqueUnits.Add(CreateNationalProtectionElite());
    UniqueUnits.Add(CreateFrontierPatrolCorps());
}

void UYunnanClique::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("NationalProtectionTradition");
        Mechanic.MechanicName = FText::FromString(TEXT("護國傳統"));
        Mechanic.Description = FText::FromString(TEXT("繼承護國戰爭光榮傳統，部隊士氣高昂，政治合法性強，不容易發生叛亂"));
        Mechanic.MechanicType = EFactionMechanicType::Political;
        Mechanic.EffectMultiplier = 1.3f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("FrontierExpansion");
        Mechanic.MechanicName = FText::FromString(TEXT("邊疆擴張"));
        Mechanic.Description = FText::FromString(TEXT("對外擴張有政治正當性，征服新領土時部隊戰鬥力提升25%"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.25f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UYunnanClique::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期鞏固雲南內部，招撫少數民族部落，建立穩固的後方基地。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期利用護國軍聲望和軍事實力，向四川、廣西等地擴張勢力。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期參與全國性的政治軍事角逐，護國傳統可以為你贏得更多支持。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用少數民族部隊的特殊能力，護國軍精銳用於決定性戰鬥。邊疆地形作戰有天然優勢。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("利用護國傳統的政治資本，在全國政治舞台上發揮影響力。與少數民族保持良好關係。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("發展雲南特色經濟，控制邊境貿易路線，保護商隊安全。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("如何平衡漢族和少數民族關係")),
        FText::FromString(TEXT("選擇哪個方向進行擴張")),
        FText::FromString(TEXT("何時參與全國政治角逐"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("建立穩固的西南根據地")),
        FText::FromString(TEXT("成功擴張到周邊省份")),
        FText::FromString(TEXT("在全國統一進程中發揮關鍵作用"))
    };
}

FFactionUnit UYunnanClique::CreateYunnanBorderDefenseDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 110;
    Stats.DefensePower = 115;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 105;
    Stats.ProductionCost = 115;
    Stats.UpkeepCost = 11;
    Stats.TrainingTime = 11.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("YunnanBorderDefenseDivision");
    Unit.UnitName = FText::FromString(TEXT("滇軍邊防師"));
    Unit.Description = FText::FromString(TEXT("專門守衛邊疆的部隊，熟悉邊境地區，戰鬥力穩定可靠"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UYunnanClique::CreateEthnicMinorityCoalition() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 105;
    Stats.DefensePower = 95;
    Stats.MovementSpeed = 110;
    Stats.HealthPoints = 90;
    Stats.ProductionCost = 85;
    Stats.UpkeepCost = 7;
    Stats.TrainingTime = 7.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("EthnicMinorityCoalition");
    Unit.UnitName = FText::FromString(TEXT("少數民族聯軍"));
    Unit.Description = FText::FromString(TEXT("由雲南各少數民族組成的聯軍，熟悉山地叢林作戰"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Mountain);
    Unit.Traits.Add(EUnitTrait::Guerrilla);
    
    return Unit;
}

FFactionUnit UYunnanClique::CreateNationalProtectionElite() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 130;
    Stats.DefensePower = 120;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 115;
    Stats.ProductionCost = 150;
    Stats.UpkeepCost = 15;
    Stats.TrainingTime = 15.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("NationalProtectionElite");
    Unit.UnitName = FText::FromString(TEXT("護國軍精銳"));
    Unit.Description = FText::FromString(TEXT("護國戰爭的精銳老兵，戰鬥經驗豐富，士氣高昂，是滇軍的核心力量"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::ShockTroop);
    
    return Unit;
}

FFactionUnit UYunnanClique::CreateFrontierPatrolCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 95;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 120;
    Stats.HealthPoints = 85;
    Stats.ProductionCost = 80;
    Stats.UpkeepCost = 7;
    Stats.TrainingTime = 7.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("FrontierPatrolCorps");
    Unit.UnitName = FText::FromString(TEXT("邊疆巡邏隊"));
    Unit.Description = FText::FromString(TEXT("專門巡邏邊境地區的部隊，移動快速，熟悉地形"));
    Unit.UnitType = EFactionUnitType::Cavalry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnitStats UYunnanClique::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.AttackPower = FMath::RoundToInt(BaseStats.AttackPower * 1.1f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UYunnanClique::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("MoraleBonus"), 1.3f);
    Bonuses.Add(FName("EthnicRecruitmentBonus"), 1.25f);
    Bonuses.Add(FName("FrontierCombatBonus"), 1.25f);
    
    return Bonuses;
}

FGameplayGuide UYunnanClique::GetGameplayGuide() const
{
    return GameplayGuide;
}
