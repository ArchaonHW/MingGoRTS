// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 北洋政府 - Beiyang Government Implementation

#include "RepublicEraFactions/Factions/BeiyangGovernment.h"
#include "MingRTSPlayerController.h"

UBeiyangGovernment::UBeiyangGovernment()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UBeiyangGovernment::InitializeFaction()
{
    Super::InitializeFaction();
    
    UE_LOG(LogTemp, Log, TEXT("Beiyang Government faction initialized"));
    
    // Apply starting bonuses
    TMap<FName, float> Bonuses = GetStartingBonuses();
    // Apply bonuses to player controller
}

void UBeiyangGovernment::InitializeAttributes()
{
    Attributes.FactionID = FName("BeiyangGovernment");
    Attributes.FactionName = FText::FromString(TEXT("北洋政府"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Beiyang Government"));
    Attributes.Difficulty = EFactionDifficulty::Easy;
    Attributes.Category = EFactionCategory::CentralGovernment;
    Attributes.FactionColor = FLinearColor(0.8f, 0.1f, 0.1f, 1.0f); // Red
    
    // Starting position
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("北京"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("北京")),
        FText::FromString(TEXT("天津")),
        FText::FromString(TEXT("河北"))
    };
    Attributes.StartingPosition.StartingYear = 1912;
    Attributes.StartingPosition.InitialGold = 1500; // +50% for central government
    Attributes.StartingPosition.InitialManpower = 600;
    Attributes.StartingPosition.InitialReputation = 70; // High international recognition
    
    // Traits
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("國際外交承認，容易獲得外國支援")),
        FText::FromString(TEXT("可以從其他勢力徵收稅收")),
        FText::FromString(TEXT("德國軍事顧問訓練的現代化部隊")),
        FText::FromString(TEXT("起始資金和聲望較高"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("內部派系鬥爭嚴重，穩定性較低")),
        FText::FromString(TEXT("容易成為其他軍閥的共同敵人")),
        FText::FromString(TEXT("必須維護中央權威的額外壓力")),
        FText::FromString(TEXT("依賴外國援助，獨立性受限"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用外交優勢結盟")),
        FText::FromString(TEXT("優先發展現代化陸軍")),
        FText::FromString(TEXT("適時徵收稅收增加收入"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡外交和經濟管理的玩家，難度較低，適合新手"));
}

void UBeiyangGovernment::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateBeiyangEliteInfantry());
    UniqueUnits.Add(CreateGermanArtilleryCorps());
    UniqueUnits.Add(CreateBeiyangCavalryDivision());
    UniqueUnits.Add(CreateForeignAdvisorCorps());
}

void UBeiyangGovernment::InitializeUniqueMechanics()
{
    // Mechanic 1: Diplomatic Recognition
    {
        FFactionMechanic Mechanic = CreateMechanicDefinition(
            FName("DiplomaticRecognition"),
            FText::FromString(TEXT("外交承認")),
            FText::FromString(TEXT("作為中央政府，獲得國際外交承認，可以更容易獲得外國援助和軍事顧問")),
            EFactionMechanicType::Diplomatic,
            1.25f
        );
        Mechanic.RequiredTechnologies.Add(FName("ForeignRelations"));
        UniqueMechanics.Add(Mechanic);
    }
    
    // Mechanic 2: Central Authority
    {
        FFactionMechanic Mechanic = CreateMechanicDefinition(
            FName("CentralAuthority"),
            FText::FromString(TEXT("中央權威")),
            FText::FromString(TEXT("可以利用中央政府的地位，向地方軍閥徵收稅收，增加收入來源")),
            EFactionMechanicType::Political,
            1.2f
        );
        Mechanic.RequiredTechnologies.Add(FName("Taxation"));
        UniqueMechanics.Add(Mechanic);
    }
}

void UBeiyangGovernment::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期利用起始資金優勢快速建立現代化軍隊。通過外交與主要列強建立關係，獲取軍事援助。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期開始使用中央權威機制徵收稅收，擴大經濟優勢。優先消滅威脅最大的地方軍閥。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期利用經濟和軍事優勢統一全國。注意維護國際關係，確保外國持續支持。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("善用德式訓練的精銳步兵和砲兵進行正規戰。外國顧問團可以大幅提升部隊戰鬥力。騎兵適合追擊和巡邏。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("優先與英美德日等列強建立良好關係。對地方軍閥採取分化策略，拉攏次要敵人對抗主要威脅。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("重點發展軍工業和基礎設施。適時使用徵稅能力，但注意不要過度壓迫導致反抗。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("是否接受外國軍事援助（增加實力但降低獨立性）")),
        FText::FromString(TEXT("何時開始大規模徵收稅收（太早會引發反抗，太晚浪費優勢）")),
        FText::FromString(TEXT("如何處理內部派系鬥爭（平衡或鎮壓）"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("統一中國，消滅或招安所有軍閥")),
        FText::FromString(TEXT("獲得國際公認的統一政府地位")),
        FText::FromString(TEXT("建立穩定的現代化國家體制"))
    };
}

FFactionUnit UBeiyangGovernment::CreateBeiyangEliteInfantry() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 120;
    Stats.DefensePower = 110;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 110;
    Stats.ProductionCost = 120;
    Stats.UpkeepCost = 12;
    Stats.TrainingTime = 12.0f;
    
    FFactionUnit Unit = CreateUnitDefinition(
        FName("BeiyangEliteInfantry"),
        FText::FromString(TEXT("北洋精銳步兵")),
        FText::FromString(TEXT("德國教官訓練的現代化步兵，裝備精良，訓練有素，是北洋軍的核心力量")),
        EFactionUnitType::Infantry,
        Stats
    );
    
    Unit.Traits.Add(EUnitTrait::ShockTroop);
    Unit.Traits.Add(EUnitTrait::Foreign);
    
    return Unit;
}

FFactionUnit UBeiyangGovernment::CreateGermanArtilleryCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 140;
    Stats.DefensePower = 80;
    Stats.MovementSpeed = 60;
    Stats.HealthPoints = 90;
    Stats.ProductionCost = 150;
    Stats.UpkeepCost = 15;
    Stats.TrainingTime = 15.0f;
    
    FFactionUnit Unit = CreateUnitDefinition(
        FName("GermanArtilleryCorps"),
        FText::FromString(TEXT("德式砲兵團")),
        FText::FromString(TEXT("裝備德國製造的現代火砲，火力強大但移動緩慢，需要步兵保護")),
        EFactionUnitType::Artillery,
        Stats
    );
    
    Unit.Traits.Add(EUnitTrait::ArtillerySupport);
    Unit.Traits.Add(EUnitTrait::Foreign);
    
    return Unit;
}

FFactionUnit UBeiyangGovernment::CreateBeiyangCavalryDivision() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 110;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 140;
    Stats.HealthPoints = 100;
    Stats.ProductionCost = 100;
    Stats.UpkeepCost = 10;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit = CreateUnitDefinition(
        FName("BeiyangCavalryDivision"),
        FText::FromString(TEXT("北洋騎兵師")),
        FText::FromString(TEXT("傳統的蒙古馬和改良騎兵戰術結合，移動快速，擅長追擊和巡邏")),
        EFactionUnitType::Cavalry,
        Stats
    );
    
    Unit.Traits.Add(EUnitTrait::CavalryCharge);
    
    return Unit;
}

FFactionUnit UBeiyangGovernment::CreateForeignAdvisorCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 80;
    Stats.DefensePower = 100;
    Stats.MovementSpeed = 90;
    Stats.HealthPoints = 80;
    Stats.ProductionCost = 180;
    Stats.UpkeepCost = 20;
    Stats.TrainingTime = 8.0f;
    
    FFactionUnit Unit = CreateUnitDefinition(
        FName("ForeignAdvisorCorps"),
        FText::FromString(TEXT("外國顧問團")),
        FText::FromString(TEXT("由德國、日本等國軍事顧問組成，能大幅提升友軍單位的戰鬥力和組織度")),
        EFactionUnitType::Support,
        Stats
    );
    
    Unit.Traits.Add(EUnitTrait::Foreign);
    Unit.Traits.Add(EUnitTrait::Political);
    
    return Unit;
}

FFactionUnitStats UBeiyangGovernment::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    // Infantry get +10% attack and defense from German training
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.AttackPower = FMath::RoundToInt(BaseStats.AttackPower * 1.1f);
        ModifiedStats.DefensePower = FMath::RoundToInt(BaseStats.DefensePower * 1.1f);
    }
    
    // Artillery gets +15% attack from foreign equipment
    if (UnitType == EFactionUnitType::Artillery)
    {
        ModifiedStats.AttackPower = FMath::RoundToInt(BaseStats.AttackPower * 1.15f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UBeiyangGovernment::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("InitialGoldMultiplier"), 1.5f);
    Bonuses.Add(FName("InitialReputationBonus"), 20.0f);
    Bonuses.Add(FName("ForeignRelationBonus"), 25.0f);
    Bonuses.Add(FName("TaxIncomeMultiplier"), 1.2f);
    
    return Bonuses;
}

FGameplayGuide UBeiyangGovernment::GetGameplayGuide() const
{
    return GameplayGuide;
}
