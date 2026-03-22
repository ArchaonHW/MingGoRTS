// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 國民政府 - Nationalist Government Implementation

#include "RepublicEraFactions/Factions/NationalistGovernment.h"
#include "MingRTSPlayerController.h"

UNationalistGovernment::UNationalistGovernment()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UNationalistGovernment::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Nationalist Government faction initialized"));
}

void UNationalistGovernment::InitializeAttributes()
{
    Attributes.FactionID = FName("NationalistGovernment");
    Attributes.FactionName = FText::FromString(TEXT("國民政府"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Nationalist Government"));
    Attributes.Difficulty = EFactionDifficulty::Normal;
    Attributes.Category = EFactionCategory::CentralGovernment;
    Attributes.FactionColor = FLinearColor(0.0f, 0.3f, 0.8f, 1.0f); // Blue
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("南京"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("南京")),
        FText::FromString(TEXT("上海")),
        FText::FromString(TEXT("廣州"))
    };
    Attributes.StartingPosition.StartingYear = 1925;
    Attributes.StartingPosition.InitialGold = 1200;
    Attributes.StartingPosition.InitialManpower = 550;
    Attributes.StartingPosition.InitialReputation = 60;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("強烈的民族主義動員能力")),
        FText::FromString(TEXT("現代化改革帶來的科技優勢")),
        FText::FromString(TEXT("黃埔軍校培養的優秀軍官")),
        FText::FromString(TEXT("控制富庶的江南地區，經濟實力強")),
        FText::FromString(TEXT("政治工作隊提升部隊士氣"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("內部派系鬥爭激烈")),
        FText::FromString(TEXT("與共產黨的意識形態衝突")),
        FText::FromString(TEXT("現代化改革需要大量資金")),
        FText::FromString(TEXT("需要同時應對內外敵人"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用民族主義快速動員兵力")),
        FText::FromString(TEXT("優先進行現代化改革")),
        FText::FromString(TEXT("發展空軍優勢進行戰略打擊"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡現代化軍隊和科技發展的玩家，難度中等"));
}

void UNationalistGovernment::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateWhampoaCadets());
    UniqueUnits.Add(CreateGermanEquippedDivisions());
    UniqueUnits.Add(CreateAirSupportGroup());
    UniqueUnits.Add(CreatePoliticalWorkTeams());
}

void UNationalistGovernment::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("Nationalism");
        Mechanic.MechanicName = FText::FromString(TEXT("民族主義"));
        Mechanic.Description = FText::FromString(TEXT("利用民族主義情緒動員民眾，部隊士氣高昂，招募速度加快"));
        Mechanic.MechanicType = EFactionMechanicType::Political;
        Mechanic.EffectMultiplier = 1.3f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("ModernizationDrive");
        Mechanic.MechanicName = FText::FromString(TEXT("現代化建設"));
        Mechanic.Description = FText::FromString(TEXT("科技研究速度加快25%，可以更快解鎖現代化部隊和裝備"));
        Mechanic.MechanicType = EFactionMechanicType::Technological;
        Mechanic.EffectMultiplier = 1.25f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UNationalistGovernment::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期鞏固廣州根據地，建立黃埔軍校培養軍官。利用民族主義動員民眾，快速擴充軍隊規模。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期發動北伐，利用現代化部隊優勢統一全國。優先消滅軍閥勢力，同時防範共產黨擴張。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期完成現代化改革，建立完整的工業和軍事體系。準備應對日本的全面侵略。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用黃埔軍校培養的精銳部隊進行決定性打擊。空軍支援可以大幅削弱敵軍防禦。政治工作隊確保部隊不會輕易崩潰。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("與西方國家建立良好關係獲取現代化援助。對地方軍閥採取招安和武力並用策略。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("重點發展江南地區的工業和商業。現代化改革需要大量資金，確保穩定的財政收入。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("何時開始北伐（太早實力不足，太晚失去先機）")),
        FText::FromString(TEXT("如何處理與共產黨的關係（合作、限制或消滅）")),
        FText::FromString(TEXT("現代化改革的資金分配比例"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("完成北伐，統一中國")),
        FText::FromString(TEXT("建立現代化的國民政府體制")),
        FText::FromString(TEXT("戰勝日本侵略，保衛國家獨立"))
    };
}

FFactionUnit UNationalistGovernment::CreateWhampoaCadets() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 130;
    Stats.DefensePower = 120;
    Stats.MovementSpeed = 105;
    Stats.HealthPoints = 115;
    Stats.ProductionCost = 140;
    Stats.UpkeepCost = 14;
    Stats.TrainingTime = 14.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("WhampoaCadets");
    Unit.UnitName = FText::FromString(TEXT("黃埔教導團"));
    Unit.Description = FText::FromString(TEXT("黃埔軍校培養的精英軍官團，指揮能力出色，能帶領部隊發揮超強戰鬥力"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::ShockTroop);
    Unit.Traits.Add(EUnitTrait::Political);
    
    return Unit;
}

FFactionUnit UNationalistGovernment::CreateGermanEquippedDivisions() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 125;
    Stats.DefensePower = 115;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 110;
    Stats.ProductionCost = 130;
    Stats.UpkeepCost = 13;
    Stats.TrainingTime = 13.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("GermanEquippedDivisions");
    Unit.UnitName = FText::FromString(TEXT("德械師"));
    Unit.Description = FText::FromString(TEXT("裝備德國先進武器的現代化步兵師，火力強大，訓練有素"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::ShockTroop);
    Unit.Traits.Add(EUnitTrait::Foreign);
    
    return Unit;
}

FFactionUnit UNationalistGovernment::CreateAirSupportGroup() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 100;
    Stats.DefensePower = 60;
    Stats.MovementSpeed = 200;
    Stats.HealthPoints = 70;
    Stats.ProductionCost = 200;
    Stats.UpkeepCost = 25;
    Stats.TrainingTime = 20.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("AirSupportGroup");
    Unit.UnitName = FText::FromString(TEXT("空軍支援隊"));
    Unit.Description = FText::FromString(TEXT("中國早期空軍力量，可以對敵軍進行轟炸和掃射，但成本高昂"));
    Unit.UnitType = EFactionUnitType::Air;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UNationalistGovernment::CreatePoliticalWorkTeams() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 60;
    Stats.DefensePower = 80;
    Stats.MovementSpeed = 90;
    Stats.HealthPoints = 75;
    Stats.ProductionCost = 100;
    Stats.UpkeepCost = 8;
    Stats.TrainingTime = 8.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("PoliticalWorkTeams");
    Unit.UnitName = FText::FromString(TEXT("政治工作隊"));
    Unit.Description = FText::FromString(TEXT("負責部隊的政治教育和士氣維護，能大幅提升友軍的戰鬥意志和組織度"));
    Unit.UnitType = EFactionUnitType::Support;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Political);
    
    return Unit;
}

FFactionUnitStats UNationalistGovernment::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    // All units get +5% morale boost from nationalism
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.AttackPower = FMath::RoundToInt(BaseStats.AttackPower * 1.05f);
        ModifiedStats.DefensePower = FMath::RoundToInt(BaseStats.DefensePower * 1.05f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UNationalistGovernment::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("RecruitmentSpeedBonus"), 1.3f);
    Bonuses.Add(FName("TechnologyResearchSpeed"), 1.25f);
    Bonuses.Add(FName("MoraleBonus"), 15.0f);
    Bonuses.Add(FName("EconomicIncomeMultiplier"), 1.15f);
    
    return Bonuses;
}

FGameplayGuide UNationalistGovernment::GetGameplayGuide() const
{
    return GameplayGuide;
}
