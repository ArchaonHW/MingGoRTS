// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 直系軍閥 - Zhili Clique Implementation

#include "RepublicEraFactions/Factions/ZhiliClique.h"
#include "MingRTSPlayerController.h"

UZhiliClique::UZhiliClique()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UZhiliClique::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Zhili Clique faction initialized"));
}

void UZhiliClique::InitializeAttributes()
{
    Attributes.FactionID = FName("ZhiliClique");
    Attributes.FactionName = FText::FromString(TEXT("直系軍閥"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Zhili Clique"));
    Attributes.Difficulty = EFactionDifficulty::Normal;
    Attributes.Category = EFactionCategory::RegionalWarlord;
    Attributes.FactionColor = FLinearColor(0.5f, 0.3f, 0.1f, 1.0f); // Brown
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("洛陽"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("洛陽")),
        FText::FromString(TEXT("保定")),
        FText::FromString(TEXT("河南"))
    };
    Attributes.StartingPosition.StartingYear = 1920;
    Attributes.StartingPosition.InitialGold = 1100;
    Attributes.StartingPosition.InitialManpower = 700; // High manpower
    Attributes.StartingPosition.InitialReputation = 45;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("控制人口稠密的中原地區，人力資源豐富")),
        FText::FromString(TEXT("可以快速徵召大量部隊")),
        FText::FromString(TEXT("吳佩孚等將領軍事才能出眾")),
        FText::FromString(TEXT("地理位置重要，戰略價值高"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("徵召部隊質量較低，訓練不足")),
        FText::FromString(TEXT("經濟基礎不如沿海地區")),
        FText::FromString(TEXT("四面受敵，容易遭到圍攻")),
        FText::FromString(TEXT("與奉系、皖系都有矛盾"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用人力優勢建立龐大軍隊")),
        FText::FromString(TEXT("優先消滅一個方向的敵人")),
        FText::FromString(TEXT("發展中原經濟提高資金收入"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡大規模作戰和人海戰術的玩家"));
}

void UZhiliClique::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateZhiliMainForceDivision());
    UniqueUnits.Add(CreateWuPeifuEliteGuard());
    UniqueUnits.Add(CreateCentralPlainsCavalry());
    UniqueUnits.Add(CreateLocalMilitiaCorps());
}

void UZhiliClique::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("CentralPlainsHegemony");
        Mechanic.MechanicName = FText::FromString(TEXT("中原霸主"));
        Mechanic.Description = FText::FromString(TEXT("控制中原人口稠密地區，人力資源豐富，徵兵速度加快50%"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.5f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("MassConscription");
        Mechanic.MechanicName = FText::FromString(TEXT("大規模徵兵"));
        Mechanic.Description = FText::FromString(TEXT("可以快速從農村徵召大量民兵，雖然質量較低但數量龐大"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 2.0f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UZhiliClique::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期利用人口優勢快速建立大規模軍隊。優先鞏固中原地區的控制。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期利用兵力優勢擴張勢力，可以同時在多個方向作戰。注意選擇正確的敵人。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期可能需要面對多個敵人的圍攻，需要謹慎使用兵力，避免過度擴張。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用數量優勢進行消耗戰。精銳部隊用於關鍵突破，民兵用於牽制和守備。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("盡量避免同時與多個強敵開戰。可以暫時與某些勢力妥協，集中打擊主要敵人。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("發展中原農業和手工業，提高經濟收入來維持龐大軍隊。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("如何分配兵力在多個戰線")),
        FText::FromString(TEXT("何時停止擴張鞏固內部")),
        FText::FromString(TEXT("對奉系和皖系的策略選擇"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("控制中原並統一北方")),
        FText::FromString(TEXT("擊敗主要競爭對手奉系和皖系")),
        FText::FromString(TEXT("最終統一全國"))
    };
}

FFactionUnit UZhiliClique::CreateZhiliMainForceDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 110;
    Stats.DefensePower = 105;
    Stats.MovementSpeed = 95;
    Stats.HealthPoints = 105;
    Stats.ProductionCost = 90;
    Stats.UpkeepCost = 8;
    Stats.TrainingTime = 8.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("ZhiliMainForceDivision");
    Unit.UnitName = FText::FromString(TEXT("直軍主力師"));
    Unit.Description = FText::FromString(TEXT("直系軍閥的主力步兵部隊，人數眾多，基礎戰鬥力可靠"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UZhiliClique::CreateWuPeifuEliteGuard() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 135;
    Stats.DefensePower = 125;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 120;
    Stats.ProductionCost = 160;
    Stats.UpkeepCost = 16;
    Stats.TrainingTime = 15.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("WuPeifuEliteGuard");
    Unit.UnitName = FText::FromString(TEXT("吳佩孚衛隊"));
    Unit.Description = FText::FromString(TEXT="吳佩孚的個人精銳衛隊，戰鬥力強大，忠誠度極高"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::ShockTroop);
    
    return Unit;
}

FFactionUnit UZhiliClique::CreateCentralPlainsCavalry() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 115;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 120;
    Stats.HealthPoints = 100;
    Stats.ProductionCost = 100;
    Stats.UpkeepCost = 10;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("CentralPlainsCavalry");
    Unit.UnitName = FText::FromString(TEXT("中原騎兵"));
    Unit.Description = FText::FromString(TEXT("中原地區傳統騎兵，移動快速，適合平原地區作戰"));
    Unit.UnitType = EFactionUnitType::Cavalry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::CavalryCharge);
    
    return Unit;
}

FFactionUnit UZhiliClique::CreateLocalMilitiaCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 70;
    Stats.DefensePower = 60;
    Stats.MovementSpeed = 85;
    Stats.HealthPoints = 70;
    Stats.ProductionCost = 35;
    Stats.UpkeepCost = 3;
    Stats.TrainingTime = 3.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("LocalMilitiaCorps");
    Unit.UnitName = FText::FromString(TEXT("地方民團"));
    Unit.Description = FText::FromString(TEXT("從農村快速徵召的民兵，戰鬥力較弱但數量龐大，成本極低"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnitStats UZhiliClique::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    // Infantry produced in larger quantities
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.ProductionCost = FMath::RoundToInt(BaseStats.ProductionCost * 0.9f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UZhiliClique::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("StartingManpowerBonus"), 1.4f);
    Bonuses.Add(FName("RecruitmentSpeedBonus"), 1.5f);
    Bonuses.Add(FName("MilitiaCostReduction"), 0.5f);
    
    return Bonuses;
}

FGameplayGuide UZhiliClique::GetGameplayGuide() const
{
    return GameplayGuide;
}
