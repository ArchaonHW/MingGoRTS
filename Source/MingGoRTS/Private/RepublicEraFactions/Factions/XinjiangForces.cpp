// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 新疆勢力 - Xinjiang Forces Implementation

#include "RepublicEraFactions/Factions/XinjiangForces.h"
#include "MingRTSPlayerController.h"

UXinjiangForces::UXinjiangForces()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UXinjiangForces::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Xinjiang Forces faction initialized"));
}

void UXinjiangForces::InitializeAttributes()
{
    Attributes.FactionID = FName("XinjiangForces");
    Attributes.FactionName = FText::FromString(TEXT("新疆勢力"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Xinjiang Forces"));
    Attributes.Difficulty = EFactionDifficulty::VeryHard;
    Attributes.Category = EFactionCategory::MinorPower;
    Attributes.FactionColor = FLinearColor(0.6f, 0.5f, 0.3f, 1.0f);
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("迪化"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("迪化")),
        FText::FromString(TEXT("新疆"))
    };
    Attributes.StartingPosition.StartingYear = 1912;
    Attributes.StartingPosition.InitialGold = 500;
    Attributes.StartingPosition.InitialManpower = 300;
    Attributes.StartingPosition.InitialReputation = 25;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("控制絲绸之路，貿易收入可觀")),
        FText::FromString(TEXT("地形險要，極易防守")),
        FText::FromString(TEXT("可以聯合多個民族力量")),
        FText::FromString(TEXT("地理位置特殊，難以被消滅"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("地理位置極度偏遠，與中原聯繫困難")),
        FText::FromString(TEXT("資金和人口都非常有限")),
        FText::FromString(TEXT("民族複雜，管理困難")),
        FText::FromString(TEXT("現代化程度極低"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("發展絲绸之路貿易增加收入")),
        FText::FromString(TEXT("聯合各民族維持穩定")),
        FText::FromString(TEXT("利用地形自保，等待時機"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡挑戰極限難度的玩家，需要高超的技巧"));
}

void UXinjiangForces::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateXinjiangBorderDefenseArmy());
    UniqueUnits.Add(CreateMultiEthnicCoalitionForces());
    UniqueUnits.Add(CreateFrontierCavalryDivision());
    UniqueUnits.Add(CreateLocalPowerAlliance());
}

void UXinjiangForces::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("FrontierFortress");
        Mechanic.MechanicName = FText::FromString(TEXT("邊疆要塞"));
        Mechanic.Description = FText::FromString(TEXT("在新疆地區作戰時防禦力提升50%，敵人補給困難，進攻成本高昂"));
        Mechanic.MechanicType = EFactionMechanicType::Geographical;
        Mechanic.EffectMultiplier = 1.5f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("MultiEthnicCoalition");
        Mechanic.MechanicName = FText::FromString(TEXT("多民族聯合"));
        Mechanic.Description = FText::FromString(TEXT("可以招募維吾爾、哈薩克等多民族部隊，各有特色，但需要維持民族平衡"));
        Mechanic.MechanicType = EFactionMechanicType::Political;
        Mechanic.EffectMultiplier = 1.25f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("SilkRoadControl");
        Mechanic.MechanicName = FText::FromString(TEXT("絲绸之路控制"));
        Mechanic.Description = FText::FromString(TEXT("控制絲绸之路貿易路線，可以獲得額外的貿易收入"));
        Mechanic.MechanicType = EFactionMechanicType::Economic;
        Mechanic.EffectMultiplier = 1.3f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UXinjiangForces::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期極度困難，需要謹慎管理有限資源。聯合各民族維持穩定是最重要的。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期發展絲绸之路貿易增加收入。維持與周邊勢力的和平關係。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期在全國政局中尋找機會，可以聯合其他勢力或保持獨立。極難統一全國但可以成為重要棋子。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("利用新疆的地理優勢進行防守作戰。騎兵部隊適合在廣袤的沙漠和草原機動。避免與強敵正面交鋒。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("與蘇聯和中央政府都保持良好關係。利用地理位置的特殊性謀求最大利益。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("重點發展絲绸之路貿易，保護商路安全。畜牧业可以提供基本生存資源。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("如何平衡各民族關係")),
        FText::FromString(TEXT("選擇與哪個外部勢力結盟")),
        FText::FromString(TEXT("是否嘗試向中原進軍（極高風險）"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("在新疆建立穩固的獨立政權")),
        FText::FromString(TEXT("控制絲绸之路成為富庶的邊疆勢力")),
        FText::FromString(TEXT("在全國統一進程中發揮關鍵作用"))
    };
}

FFactionUnit UXinjiangForces::CreateXinjiangBorderDefenseArmy() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 100;
    Stats.DefensePower = 120;
    Stats.MovementSpeed = 85;
    Stats.HealthPoints = 110;
    Stats.ProductionCost = 100;
    Stats.UpkeepCost = 9;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("XinjiangBorderDefenseArmy");
    Unit.UnitName = FText::FromString(TEXT("新疆邊防軍"));
    Unit.Description = FText::FromString(TEXT("專門守衛新疆邊疆的部隊，在新疆作戰時戰鬥力大幅提升"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Defensive);
    
    return Unit;
}

FFactionUnit UXinjiangForces::CreateMultiEthnicCoalitionForces() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 105;
    Stats.DefensePower = 95;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 90;
    Stats.ProductionCost = 85;
    Stats.UpkeepCost = 7;
    Stats.TrainingTime = 7.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("MultiEthnicCoalitionForces");
    Unit.UnitName = FText::FromString(TEXT("多民族聯軍"));
    Unit.Description = FText::FromString(TEXT("由維吾爾、哈薩克等多民族組成的聯軍，適應性強但協調較難"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UXinjiangForces::CreateFrontierCavalryDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 115;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 140;
    Stats.HealthPoints = 95;
    Stats.ProductionCost = 110;
    Stats.UpkeepCost = 10;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("FrontierCavalryDivision");
    Unit.UnitName = FText::FromString(TEXT("邊疆騎兵師"));
    Unit.Description = FText::FromString(TEXT("適應新疆廣袤地形的騎兵部隊，機動性極強，適合快速突襲"));
    Unit.UnitType = EFactionUnitType::Cavalry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::CavalryCharge);
    
    return Unit;
}

FFactionUnit UXinjiangForces::CreateLocalPowerAlliance() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 85;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 80;
    Stats.HealthPoints = 85;
    Stats.ProductionCost = 70;
    Stats.UpkeepCost = 6;
    Stats.TrainingTime = 6.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("LocalPowerAlliance");
    Unit.UnitName = FText::FromString(TEXT("地方勢力聯盟"));
    Unit.Description = FText::FromString(TEXT("與新疆各地地方勢力聯合組成的部隊，熟悉本地情況"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnitStats UXinjiangForces::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.DefensePower = FMath::RoundToInt(BaseStats.DefensePower * 1.15f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UXinjiangForces::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("FrontierDefenseBonus"), 1.5f);
    Bonuses.Add(FName("TradeIncomeBonus"), 1.3f);
    Bonuses.Add(FName("EnemySupplyPenalty"), 1.5f);
    
    return Bonuses;
}

FGameplayGuide UXinjiangForces::GetGameplayGuide() const
{
    return GameplayGuide;
}
