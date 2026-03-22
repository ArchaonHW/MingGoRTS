// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 皖系軍閥 - Anhui Clique Implementation

#include "RepublicEraFactions/Factions/AnhuiClique.h"
#include "MingRTSPlayerController.h"

UAnhuiClique::UAnhuiClique()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UAnhuiClique::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Anhui Clique faction initialized"));
}

void UAnhuiClique::InitializeAttributes()
{
    Attributes.FactionID = FName("AnhuiClique");
    Attributes.FactionName = FText::FromString(TEXT("皖系軍閥"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Anhui Clique"));
    Attributes.Difficulty = EFactionDifficulty::Normal;
    Attributes.Category = EFactionCategory::ForeignSupported;
    Attributes.FactionColor = FLinearColor(0.3f, 0.5f, 0.3f, 1.0f);
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("天津"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("天津")),
        FText::FromString(TEXT("合肥")),
        FText::FromString(TEXT("安徽"))
    };
    Attributes.StartingPosition.StartingYear = 1916;
    Attributes.StartingPosition.InitialGold = 1000;
    Attributes.StartingPosition.InitialManpower = 500;
    Attributes.StartingPosition.InitialReputation = 40;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("日本援助提供先進武器和訓練")),
        FText::FromString(TEXT("擅長政治操作和外交謀略")),
        FText::FromString(TEXT("控制天津等重要港口城市")),
        FText::FromString(TEXT("早期軍閥中實力較強"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("過度依賴日本支持")),
        FText::FromString(TEXT("段祺瑞個人權威難以維持長期穩定")),
        FText::FromString(TEXT("與其他軍閥關係緊張")),
        FText::FromString(TEXT("後期實力衰落較快"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用外交手段分化敵人")),
        FText::FromString(TEXT("依靠日本援助快速現代化")),
        FText::FromString(TEXT("優先打擊主要競爭對手"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡外交操作和短期擴張的玩家"));
}

void UAnhuiClique::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateAnhuiInfantry());
    UniqueUnits.Add(CreateJapaneseAdvisorCorps());
    UniqueUnits.Add(CreateAnhuiCavalry());
    UniqueUnits.Add(CreateLocalGarrison());
}

void UAnhuiClique::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("PoliticalManipulation");
        Mechanic.MechanicName = FText::FromString(TEXT("政治操作"));
        Mechanic.Description = FText::FromString(TEXT("擅長外交謀略，可以更容易與其他勢力結盟或分化敵人"));
        Mechanic.MechanicType = EFactionMechanicType::Political;
        Mechanic.EffectMultiplier = 1.25f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("JapaneseAid");
        Mechanic.MechanicName = FText::FromString(TEXT("日本援助"));
        Mechanic.Description = FText::FromString(TEXT("獲得日本軍事援助，部隊武器和訓練質量提升"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.2f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UAnhuiClique::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期利用日本援助快速建立現代化軍隊。同時在外交上積極活動，尋找盟友。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期利用政治手段分化敵人，避免同時與多個對手作戰。依靠外援維持軍事優勢。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期需要注意減少對外援的依賴，建立自主的軍事和經濟體系，否則難以長期維持。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("利用日本訓練的部隊進行現代化戰爭。日本顧問團可以大幅提升部隊效能。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("善用政治操作，拉攏次要敵人打擊主要敵人。與日本保持良好關係但注意代價。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("利用天津等港口的貿易收入。但要注意外援的代價和依賴性。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("如何平衡與日本的關係")),
        FText::FromString(TEXT("選擇正確的敵人和盟友")),
        FText::FromString(TEXT("何時開始減少外援依賴"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("建立穩固的政治聯盟")),
        FText::FromString(TEXT("擊敗主要競爭對手")),
        FText::FromString(TEXT("建立獨立的強大勢力"))
    };
}

FFactionUnit UAnhuiClique::CreateAnhuiInfantry() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 115;
    Stats.DefensePower = 105;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 105;
    Stats.ProductionCost = 110;
    Stats.UpkeepCost = 11;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("AnhuiInfantry");
    Unit.UnitName = FText::FromString(TEXT("皖軍步兵"));
    Unit.Description = FText::FromString(TEXT("接受日本訓練的現代化步兵，裝備和訓練質量較好"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UAnhuiClique::CreateJapaneseAdvisorCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 90;
    Stats.DefensePower = 80;
    Stats.MovementSpeed = 85;
    Stats.HealthPoints = 75;
    Stats.ProductionCost = 150;
    Stats.UpkeepCost = 18;
    Stats.TrainingTime = 12.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("JapaneseAdvisorCorps");
    Unit.UnitName = FText::FromString(TEXT("日本顧問團"));
    Unit.Description = FText::FromString(TEXT("日本軍事顧問組成的支援部隊，能提升友軍的現代化戰鬥能力"));
    Unit.UnitType = EFactionUnitType::Support;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Foreign);
    
    return Unit;
}

FFactionUnit UAnhuiClique::CreateAnhuiCavalry() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 110;
    Stats.DefensePower = 85;
    Stats.MovementSpeed = 125;
    Stats.HealthPoints = 95;
    Stats.ProductionCost = 105;
    Stats.UpkeepCost = 10;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("AnhuiCavalry");
    Unit.UnitName = FText::FromString(TEXT("皖系騎兵"));
    Unit.Description = FText::FromString(TEXT("傳統騎兵部隊，機動性良好，適合平原作戰"));
    Unit.UnitType = EFactionUnitType::Cavalry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::CavalryCharge);
    
    return Unit;
}

FFactionUnit UAnhuiClique::CreateLocalGarrison() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 85;
    Stats.DefensePower = 120;
    Stats.MovementSpeed = 70;
    Stats.HealthPoints = 110;
    Stats.ProductionCost = 80;
    Stats.UpkeepCost = 7;
    Stats.TrainingTime = 7.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("LocalGarrison");
    Unit.UnitName = FText::FromString(TEXT("地方守備隊"));
    Unit.Description = FText::FromString(TEXT("專門用於守備地方的部隊，防禦能力強但移動緩慢"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Defensive);
    
    return Unit;
}

FFactionUnitStats UAnhuiClique::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    // Defensive units get +10% defense
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.DefensePower = FMath::RoundToInt(BaseStats.DefensePower * 1.05f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UAnhuiClique::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("DiplomaticBonus"), 1.25f);
    Bonuses.Add(FName("JapaneseEquipmentQuality"), 1.15f);
    Bonuses.Add(FName("PortTradeBonus"), 1.2f);
    
    return Bonuses;
}

FGameplayGuide UAnhuiClique::GetGameplayGuide() const
{
    return GameplayGuide;
}
