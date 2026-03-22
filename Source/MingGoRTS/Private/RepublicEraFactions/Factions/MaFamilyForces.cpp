// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 马家军 - Ma Family Forces Implementation

#include "RepublicEraFactions/Factions/MaFamilyForces.h"
#include "MingRTSPlayerController.h"

UMaFamilyForces::UMaFamilyForces()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UMaFamilyForces::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Ma Family Forces faction initialized"));
}

void UMaFamilyForces::InitializeAttributes()
{
    Attributes.FactionID = FName("MaFamilyForces");
    Attributes.FactionName = FText::FromString(TEXT("马家军"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Ma Family Forces"));
    Attributes.Difficulty = EFactionDifficulty::Hard;
    Attributes.Category = EFactionCategory::RegionalWarlord;
    Attributes.FactionColor = FLinearColor(0.3f, 0.4f, 0.5f, 1.0f);
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("兰州"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("兰州")),
        FText::FromString(TEXT("西宁")),
        FText::FromString(TEXT("青海"))
    };
    Attributes.StartingPosition.StartingYear = 1912;
    Attributes.StartingPosition.InitialGold = 700;
    Attributes.StartingPosition.InitialManpower = 400;
    Attributes.StartingPosition.InitialReputation = 30;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("回族骑兵战斗力极强，机动性优秀")),
        FText::FromString(TEXT("宗教团结使部队士气高昂")),
        FText::FromString(TEXT("马家统治稳定，内部凝聚力强")),
        FText::FromString(TEXT("熟悉西北地形，作战有优势"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("地处偏远，经济基础薄弱")),
        FText::FromString(TEXT("人口稀少，兵力有限")),
        FText::FromString(TEXT("现代化程度低")),
        FText::FromString(TEXT("对外扩张困难"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("发挥骑兵优势进行机动作战")),
        FText::FromString(TEXT("利用宗教团结维持部队稳定")),
        FText::FromString(TEXT("巩固西北根据地"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推荐喜欢骑兵作战和宗教文化特色的玩家"));
}

void UMaFamilyForces::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateHuiCavalryDivision());
    UniqueUnits.Add(CreateMuslimVolunteerCorps());
    UniqueUnits.Add(CreateMaFamilyEliteGuard());
    UniqueUnits.Add(CreateReligiousMobilizationCorps());
}

void UMaFamilyForces::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("HuiCavalryTradition");
        Mechanic.MechanicName = FText::FromString(TEXT("回族骑兵传统"));
        Mechanic.Description = FText::FromString(TEXT("回族骑兵战斗力提升40%，移动速度提升20%，是马家军的核心力量"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.4f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("ReligiousUnity");
        Mechanic.MechanicName = FText::FromString(TEXT("宗教团结"));
        Mechanic.Description = FText::FromString(TEXT("通过宗教信仰维持部队团结，士气不会轻易崩溃，不容易投降"));
        Mechanic.MechanicType = EFactionMechanicType::Political;
        Mechanic.EffectMultiplier = 1.3f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UMaFamilyForces::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期巩固西北根据地，发展骑兵部队。利用宗教团结稳定内部。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期利用骑兵优势进行扩张，可以向甘肃、宁夏等地发展势力。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("后期在全国政治中寻找盟友，保存实力，等待时机。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用骑兵的快速机动，进行突袭和包抄。避免与敌人正面消耗战。宗教动员队可以提升全军士气。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("与中央政府保持联系，争取合法地位。与其他军阀保持灵活关系。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("发展畜牧业支撑骑兵部队。控制丝绸之路贸易路线增加收入。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("如何平衡宗教与政治关系")),
        FText::FromString(TEXT("何时开始对外扩张")),
        FText::FromString(TEXT("选择哪个势力作为盟友"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("统一西北地区")),
        FText::FromString(TEXT("保持马家军的独立地位")),
        FText::FromString(TEXT("在全国政局中发挥重要作用"))
    };
}

FFactionUnit UMaFamilyForces::CreateHuiCavalryDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 140;
    Stats.DefensePower = 100;
    Stats.MovementSpeed = 160;
    Stats.HealthPoints = 100;
    Stats.ProductionCost = 130;
    Stats.UpkeepCost = 13;
    Stats.TrainingTime = 12.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("HuiCavalryDivision");
    Unit.UnitName = FText::FromString(TEXT("回族骑兵师"));
    Unit.Description = FText::FromString(TEXT("马家军的精锐骑兵部队，战斗力极强，机动性优秀"));
    Unit.UnitType = EFactionUnitType::Cavalry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::CavalryCharge);
    Unit.Traits.Add(EUnitTrait::Religious);
    
    return Unit;
}

FFactionUnit UMaFamilyForces::CreateMuslimVolunteerCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 100;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 95;
    Stats.HealthPoints = 85;
    Stats.ProductionCost = 70;
    Stats.UpkeepCost = 6;
    Stats.TrainingTime = 6.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("MuslimVolunteerCorps");
    Unit.UnitName = FText::FromString(TEXT("穆斯林义勇军"));
    Unit.Description = FText::FromString(TEXT("由回族穆斯林组成的志愿部队，士气高昂，忠诚度高"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Religious);
    
    return Unit;
}

FFactionUnit UMaFamilyForces::CreateMaFamilyEliteGuard() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 145;
    Stats.DefensePower = 120;
    Stats.MovementSpeed = 110;
    Stats.HealthPoints = 120;
    Stats.ProductionCost = 180;
    Stats.UpkeepCost = 18;
    Stats.TrainingTime = 15.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("MaFamilyEliteGuard");
    Unit.UnitName = FText::FromString(TEXT("马家精锐卫队"));
    Unit.Description = FText::FromString(TEXT("马家军的最高精锐，绝对忠诚，战斗力极强"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::ShockTroop);
    Unit.Traits.Add(EUnitTrait::Religious);
    
    return Unit;
}

FFactionUnit UMaFamilyForces::CreateReligiousMobilizationCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 70;
    Stats.DefensePower = 80;
    Stats.MovementSpeed = 85;
    Stats.HealthPoints = 75;
    Stats.ProductionCost = 60;
    Stats.UpkeepCost = 5;
    Stats.TrainingTime = 5.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("ReligiousMobilizationCorps");
    Unit.UnitName = FText::FromString(TEXT("宗教动员队"));
    Unit.Description = FText::FromString(TEXT("通过宗教动员的部队，可以提升全军士气，维持部队稳定"));
    Unit.UnitType = EFactionUnitType::Support;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Religious);
    Unit.Traits.Add(EUnitTrait::Political);
    
    return Unit;
}

FFactionUnitStats UMaFamilyForces::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    if (UnitType == EFactionUnitType::Cavalry)
    {
        ModifiedStats.AttackPower = FMath::RoundToInt(BaseStats.AttackPower * 1.2f);
        ModifiedStats.MovementSpeed = FMath::RoundToInt(BaseStats.MovementSpeed * 1.2f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UMaFamilyForces::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("CavalryAttackBonus"), 1.4f);
    Bonuses.Add(FName("CavalrySpeedBonus"), 1.2f);
    Bonuses.Add(FName("MoraleBonus"), 1.3f);
    
    return Bonuses;
}

FGameplayGuide UMaFamilyForces::GetGameplayGuide() const
{
    return GameplayGuide;
}
