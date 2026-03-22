// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 奉系軍閥 - Fengtian Clique Implementation

#include "RepublicEraFactions/Factions/FengtianClique.h"
#include "MingRTSPlayerController.h"

UFengtianClique::UFengtianClique()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UFengtianClique::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Fengtian Clique faction initialized"));
}

void UFengtianClique::InitializeAttributes()
{
    Attributes.FactionID = FName("FengtianClique");
    Attributes.FactionName = FText::FromString(TEXT("奉系軍閥"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Fengtian Clique"));
    Attributes.Difficulty = EFactionDifficulty::Normal;
    Attributes.Category = EFactionCategory::RegionalWarlord;
    Attributes.FactionColor = FLinearColor(0.2f, 0.6f, 0.3f, 1.0f); // Green
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("瀋陽"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("瀋陽")),
        FText::FromString(TEXT("吉林")),
        FText::FromString(TEXT("黑龍江"))
    };
    Attributes.StartingPosition.StartingYear = 1916;
    Attributes.StartingPosition.InitialGold = 1300;
    Attributes.StartingPosition.InitialManpower = 600;
    Attributes.StartingPosition.InitialReputation = 50;
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("東北工業基礎較好，經濟實力強")),
        FText::FromString(TEXT("日本軍事援助提供先進武器")),
        FText::FromString(TEXT("強大的騎兵部隊機動性極佳")),
        FText::FromString(TEXT("控制廣大的東北地區，戰略縱深大"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("依賴日本援助，獨立性受限")),
        FText::FromString(TEXT("張作霖個人威望維持統一，穩定性存疑")),
        FText::FromString(TEXT="容易成為其他軍閥的目標"),
        FText::FromString(TEXT("冬季作戰補給困難"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用工業優勢建立現代化軍隊")),
        FText::FromString(TEXT("發展騎兵優勢進行機動作戰")),
        FText::FromString(TEXT("適時向關內擴張勢力"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡騎兵作戰和工業發展的玩家"));
}

void UFengtianClique::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateNortheastArmyInfantry());
    UniqueUnits.Add(CreateWhiteRussianMercenaries());
    UniqueUnits.Add(CreateFengtianArmoredTrain());
    UniqueUnits.Add(CreateCavalryBrigade());
}

void UFengtianClique::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("ManchurianIndustry");
        Mechanic.MechanicName = FText::FromString(TEXT("東北工業"));
        Mechanic.Description = FText::FromString(TEXT("東北地區工業基礎較好，生產效率提高30%，可以更快建設軍工設施"));
        Mechanic.MechanicType = EFactionMechanicType::Economic;
        Mechanic.EffectMultiplier = 1.3f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("JapaneseSupport");
        Mechanic.MechanicName = FText::FromString(TEXT("日本援助"));
        Mechanic.Description = FText::FromString(TEXT("獲得日本軍事援助，部分部隊武器裝備和訓練質量提升，但需要付出政治代價"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.2f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UFengtianClique::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期鞏固東北根據地，利用工業優勢快速建立現代化軍隊。與日本保持良好關係獲取援助。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期利用騎兵優勢和工業基礎，向關內進軍擴張勢力。優先打擊弱小的鄰近軍閥。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期爭奪中央政權，與其他強大軍閥決戰。注意平衡與日本的關係。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用騎兵的快速機動能力，進行包抄和奇襲。裝甲列車可以作為移動堡壘使用。白俄傭兵戰鬥力強大但成本較高。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("與日本保持合作但保持一定獨立性。對其他軍閥採取時而對抗時而聯合的策略。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("重點發展重工業和軍工業，利用東北的資源優勢。確保軍隊的現代化裝備供應。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("如何平衡與日本的關係（獲取援助但保持獨立）")),
        FText::FromString(TEXT("何時開始向關內擴張")),
        FText::FromString(TEXT("是否接受日本更多援助的代價"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("統一東北並向全國擴張")),
        FText::FromString(TEXT("擊敗直系、皖系等競爭對手")),
        FText::FromString(TEXT("最終統一全國或建立獨立政權"))
    };
}

FFactionUnit UFengtianClique::CreateNortheastArmyInfantry() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 115;
    Stats.DefensePower = 110;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 110;
    Stats.ProductionCost = 115;
    Stats.UpkeepCost = 11;
    Stats.TrainingTime = 11.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("NortheastArmyInfantry");
    Unit.UnitName = FText::FromString(TEXT("東北軍步兵"));
    Unit.Description = FText::FromString(TEXT("裝備精良的現代化步兵，有工業基礎支持，戰鬥力穩定可靠"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnit UFengtianClique::CreateWhiteRussianMercenaries() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 130;
    Stats.DefensePower = 100;
    Stats.MovementSpeed = 90;
    Stats.HealthPoints = 95;
    Stats.ProductionCost = 180;
    Stats.UpkeepCost = 20;
    Stats.TrainingTime = 8.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("WhiteRussianMercenaries");
    Unit.UnitName = FText::FromString(TEXT("白俄僱傭兵"));
    Unit.Description = FText::FromString(TEXT("流亡的白俄軍人組成的僱傭部隊，戰鬥經驗豐富，戰鬥力強大但成本高昂"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Mercenary);
    Unit.Traits.Add(EUnitTrait::Foreign);
    
    return Unit;
}

FFactionUnit UFengtianClique::CreateFengtianArmoredTrain() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 150;
    Stats.DefensePower = 140;
    Stats.MovementSpeed = 80;
    Stats.HealthPoints = 200;
    Stats.ProductionCost = 300;
    Stats.UpkeepCost = 30;
    Stats.TrainingTime = 25.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("FengtianArmoredTrain");
    Unit.UnitName = FText::FromString(TEXT("奉天鐵甲車"));
    Unit.Description = FText::FromString(TEXT("裝甲列車可以沿鐵路移動，火力強大裝甲厚實，是移動的堡壘"));
    Unit.UnitType = EFactionUnitType::Special;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Defensive);
    
    return Unit;
}

FFactionUnit UFengtianClique::CreateCavalryBrigade() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 120;
    Stats.DefensePower = 85;
    Stats.MovementSpeed = 150;
    Stats.HealthPoints = 95;
    Stats.ProductionCost = 110;
    Stats.UpkeepCost = 12;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("CavalryBrigade");
    Unit.UnitName = FText::FromString(TEXT("騎兵旅"));
    Unit.Description = FText::FromString(TEXT("傳統的蒙古騎兵改良版，移動速度極快，擅長包抄和追擊"));
    Unit.UnitType = EFactionUnitType::Cavalry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::CavalryCharge);
    
    return Unit;
}

FFactionUnitStats UFengtianClique::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    // Cavalry gets +10% movement speed
    if (UnitType == EFactionUnitType::Cavalry)
    {
        ModifiedStats.MovementSpeed = FMath::RoundToInt(BaseStats.MovementSpeed * 1.1f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UFengtianClique::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("ProductionSpeedBonus"), 1.3f);
    Bonuses.Add(FName("CavalryMovementSpeed"), 1.1f);
    Bonuses.Add(FName("JapaneseWeaponQuality"), 1.15f);
    Bonuses.Add(FName("InitialTerritoryBonus"), 3.0f); // 3 provinces
    
    return Bonuses;
}

FGameplayGuide UFengtianClique::GetGameplayGuide() const
{
    return GameplayGuide;
}
