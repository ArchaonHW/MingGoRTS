// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 民國史詩勢力系統 - 勢力系統管理器實現

#include "RepublicEraFactions/RepublicEraFactions.h"
#include "Engine/DataTable.h"
#include "Engine/AssetManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

UREpublicEraFactionSystem::UREpublicEraFactionSystem()
    : FactionDataTable(nullptr)
    , bInitialized(false)
{
}

void URepublicEraFactionSystem::InitializeFactionSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Faction system already initialized"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing Republic Era Faction System..."));

    // 創建數據表
    FactionDataTable = NewObject<UDataTable>();
    FactionDataTable->RowStruct = FFactionDataRow::StaticStruct();

    // 初始化默認勢力
    InitializeDefaultFactions();

    // 加載勢力數據
    LoadFactionData();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Republic Era Faction System initialized with %d factions"), AllFactions.Num());
}

void URepublicEraFactionSystem::InitializeDefaultFactions()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing default factions..."));

    // 1. 北洋政府
    FFactionData BeiyangGovernment;
    BeiyangGovernment.FactionID = 1;
    BeiyangGovernment.FactionName = TEXT("北洋政府");
    BeiyangGovernment.FactionType = EFactionType::Central;
    BeiyangGovernment.Difficulty = EFactionDifficulty::Easy;
    BeiyangGovernment.StartingLocation = TEXT("北京");
    BeiyangGovernment.StartYear = 1912;
    BeiyangGovernment.LeaderName = TEXT("袁世凱");
    BeiyangGovernment.Description = TEXT("中華民國中央政府，擁有最高正統性和外交優勢");
    BeiyangGovernment.InitialResources = 2000;
    BeiyangGovernment.InitialPopulation = 500000;
    BeiyangGovernment.InitialMilitary = 200;
    BeiyangGovernment.DiplomaticModifier = 0.3f;
    BeiyangGovernment.EconomicModifier = 0.2f;
    BeiyangGovernment.MilitaryModifier = 0.2f;
    BeiyangGovernment.TechnologyModifier = 0.1f;
    BeiyangGovernment.CultureModifier = 0.1f;

    // 北洋政府特色機制
    FFactionSpecialMechanic CentralLegitimacy;
    CentralLegitimacy.MechanicName = TEXT("中央正統性");
    CentralLegitimacy.Description = TEXT("擁有最高正統度，外交談判優勢");
    CentralLegitimacy.MechanicType = TEXT("Diplomatic");
    CentralLegitimacy.EffectValue = 0.3f;
    BeiyangGovernment.SpecialMechanics.Add(CentralLegitimacy);

    FFactionSpecialMechanic ModernMilitary;
    ModernMilitary.MechanicName = TEXT("現代化軍事");
    ModernMilitary.Description = TEXT("擁有最現代化的軍事裝備和訓練");
    ModernMilitary.MechanicType = TEXT("Military");
    ModernMilitary.EffectValue = 0.2f;
    BeiyangGovernment.SpecialMechanics.Add(ModernMilitary);

    // 北洋政府專屬單位
    FFactionUnit BeiyangNewArmy;
    BeiyangNewArmy.UnitName = TEXT("北洋新軍步兵");
    BeiyangNewArmy.UnitType = EFactionUnitType::Elite;
    BeiyangNewArmy.Description = TEXT("高訓練度、現代化裝備、高士氣");
    BeiyangNewArmy.Attack = 8;
    BeiyangNewArmy.Defense = 7;
    BeiyangNewArmy.Movement = 4;
    BeiyangNewArmy.Cost = 150;
    BeiyangNewArmy.Upkeep = 15;
    BeiyangNewArmy.SpecialAbility = TEXT("現代化訓練");
    BeiyangGovernment.UniqueUnits.Add(BeiyangNewArmy);

    FFactionUnit BeiyangHeavyArtillery;
    BeiyangHeavyArtillery.UnitName = TEXT("北洋重砲兵");
    BeiyangHeavyArtillery.UnitType = EFactionUnitType::Artillery;
    BeiyangHeavyArtillery.Description = TEXT("遠程火力、高破壞力");
    BeiyangHeavyArtillery.Attack = 12;
    BeiyangHeavyArtillery.Defense = 3;
    BeiyangHeavyArtillery.Movement = 2;
    BeiyangHeavyArtillery.Cost = 200;
    BeiyangHeavyArtillery.Upkeep = 20;
    BeiyangHeavyArtillery.SpecialAbility = TEXT("遠程砲擊");
    BeiyangGovernment.UniqueUnits.Add(BeiyangHeavyArtillery);

    FFactionUnit BeiyangMarines;
    BeiyangMarines.UnitName = TEXT("北洋海軍陸戰隊");
    BeiyangMarines.UnitType = EFactionUnitType::Elite;
    BeiyangMarines.Description = TEXT("可兩棲登陸、海陸協同作戰");
    BeiyangMarines.Attack = 9;
    BeiyangMarines.Defense = 8;
    BeiyangMarines.Movement = 4;
    BeiyangMarines.Cost = 180;
    BeiyangMarines.Upkeep = 18;
    BeiyangMarines.SpecialAbility = TEXT("兩棲作戰");
    BeiyangGovernment.UniqueUnits.Add(BeiyangMarines);

    FFactionUnit CentralGuard;
    CentralGuard.UnitName = TEXT("中央警衛軍");
    CentralGuard.UnitType = EFactionUnitType::Guard;
    CentralGuard.Description = TEXT("忠誠度極高、保護領袖");
    CentralGuard.Attack = 10;
    CentralGuard.Defense = 10;
    CentralGuard.Movement = 3;
    CentralGuard.Cost = 160;
    CentralGuard.Upkeep = 16;
    CentralGuard.SpecialAbility = TEXT("忠誠守衛");
    BeiyangGovernment.UniqueUnits.Add(CentralGuard);

    // 優勢劣勢
    BeiyangGovernment.Advantages.Add(TEXT("最高正統性，外交優勢明顯"));
    BeiyangGovernment.Advantages.Add(TEXT("軍事裝備最現代化"));
    BeiyangGovernment.Advantages.Add(TEXT("經濟實力最強"));
    BeiyangGovernment.Advantages.Add(TEXT("國際承認度高"));

    BeiyangGovernment.Disadvantages.Add(TEXT("內部派系鬥爭激烈"));
    BeiyangGovernment.Disadvantages.Add(TEXT("地方控制力弱"));
    BeiyangGovernment.Disadvantages.Add(TEXT("財政壓力大"));
    BeiyangGovernment.Disadvantages.Add(TEXT("民眾支持度低"));

    // 推薦玩法
    BeiyangGovernment.RecommendedStrategies.Add(TEXT("外交統一：利用正統性，通過外交手段統一全國"));
    BeiyangGovernment.RecommendedStrategies.Add(TEXT("軍事威懾：展示軍事實力，威懾地方軍閥"));
    BeiyangGovernment.RecommendedStrategies.Add(TEXT("經濟控制：控制全國經濟命脈，削弱地方實力"));
    BeiyangGovernment.RecommendedStrategies.Add(TEXT("漸進統一：逐步收編地方軍隊，穩固中央權力"));

    RegisterFaction(BeiyangGovernment);

    // 2. 國民政府
    FFactionData NationalistGovernment;
    NationalistGovernment.FactionID = 2;
    NationalistGovernment.FactionName = TEXT("國民政府");
    NationalistGovernment.FactionType = EFactionType::Revolutionary;
    NationalistGovernment.Difficulty = EFactionDifficulty::Medium;
    NationalistGovernment.StartingLocation = TEXT("南京/廣州");
    NationalistGovernment.StartYear = 1925;
    NationalistGovernment.LeaderName = TEXT("蔣介石");
    NationalistGovernment.Description = TEXT("以三民主義為指導的革命政府，致力於國家統一和現代化");
    NationalistGovernment.InitialResources = 1200;
    NationalistGovernment.InitialPopulation = 300000;
    NationalistGovernment.InitialMilitary = 150;
    NationalistGovernment.DiplomaticModifier = 0.1f;
    NationalistGovernment.EconomicModifier = 0.1f;
    NationalistGovernment.MilitaryModifier = 0.15f;
    NationalistGovernment.TechnologyModifier = 0.2f;
    NationalistGovernment.CultureModifier = 0.15f;

    // 國民政府特色機制
    FFactionSpecialMechanic Nationalism;
    Nationalism.MechanicName = TEXT("民族主義");
    Nationalism.Description = TEXT("三民主義理念，革命精神，群眾動員");
    Nationalism.MechanicType = TEXT("Political");
    Nationalism.EffectValue = 0.2f;
    NationalistGovernment.SpecialMechanics.Add(Nationalism);

    FFactionSpecialMechanic Modernization;
    Modernization.MechanicName = TEXT("現代化建設");
    Modernization.Description = TEXT("黃金十年，經濟快速發展，工業化加速");
    Modernization.MechanicType = TEXT("Economic");
    Modernization.EffectValue = 0.15f;
    NationalistGovernment.SpecialMechanics.Add(Modernization);

    // 國民政府專屬單位
    FFactionUnit WhampoaCadets;
    WhampoaCadets.UnitName = TEXT("黃埔軍校生");
    WhampoaCadets.UnitType = EFactionUnitType::Elite;
    WhampoaCadets.Description = TEXT("高素質軍官、政治忠誠、現代化訓練");
    WhampoaCadets.Attack = 7;
    WhampoaCadets.Defense = 8;
    WhampoaCadets.Movement = 4;
    WhampoaCadets.Cost = 140;
    WhampoaCadets.Upkeep = 14;
    WhampoaCadets.SpecialAbility = TEXT("政治教育");
    NationalistGovernment.UniqueUnits.Add(WhampoaCadets);

    FFactionUnit GermanEquippedDivision;
    GermanEquippedDivision.UnitName = TEXT("德械師步兵");
    GermanEquippedDivision.UnitType = EFactionUnitType::Elite;
    GermanEquippedDivision.Description = TEXT("德式裝備、嚴格訓練、高戰鬥力");
    GermanEquippedDivision.Attack = 9;
    GermanEquippedDivision.Defense = 8;
    GermanEquippedDivision.Movement = 4;
    GermanEquippedDivision.Cost = 170;
    GermanEquippedDivision.Upkeep = 17;
    GermanEquippedDivision.SpecialAbility = TEXT("德式訓練");
    NationalistGovernment.UniqueUnits.Add(GermanEquippedDivision);

    FFactionUnit CentralMilitaryPolice;
    CentralMilitaryPolice.UnitName = TEXT("中央軍憲兵");
    CentralMilitaryPolice.UnitType = EFactionUnitType::Political;
    CentralMilitaryPolice.Description = TEXT("維持紀律、反恐能力、高忠誠度");
    CentralMilitaryPolice.Attack = 6;
    CentralMilitaryPolice.Defense = 9;
    CentralMilitaryPolice.Movement = 4;
    CentralMilitaryPolice.Cost = 130;
    CentralMilitaryPolice.Upkeep = 13;
    CentralMilitaryPolice.SpecialAbility = TEXT("軍紀維護");
    NationalistGovernment.UniqueUnits.Add(CentralMilitaryPolice);

    FFactionUnit AirForcePilot;
    AirForcePilot.UnitName = TEXT("空軍飛行員");
    AirForcePilot.UnitType = EFactionUnitType::Air;
    AirForcePilot.Description = TEXT("空中支援、偵察、轟炸");
    AirForcePilot.Attack = 15;
    AirForcePilot.Defense = 4;
    AirForcePilot.Movement = 8;
    AirForcePilot.Cost = 250;
    AirForcePilot.Upkeep = 25;
    AirForcePilot.SpecialAbility = TEXT("空中優勢");
    NationalistGovernment.UniqueUnits.Add(AirForcePilot);

    // 優勢劣勢
    NationalistGovernment.Advantages.Add(TEXT("明確的政治理念和目標"));
    NationalistGovernment.Advantages.Add(TEXT("現代化建設能力強"));
    NationalistGovernment.Advantages.Add(TEXT("軍隊素質高，訓練有素"));
    NationalistGovernment.Advantages.Add(TEXT("國際支持度逐漸提升"));

    NationalistGovernment.Disadvantages.Add(TEXT("起始實力相對較弱"));
    NationalistGovernment.Disadvantages.Add(TEXT("內部派系複雜"));
    NationalistGovernment.Disadvantages.Add(TEXT("財政困難"));
    NationalistGovernment.Disadvantages.Add(TEXT("面臨多個敵對勢力"));

    // 推薦玩法
    NationalistGovernment.RecommendedStrategies.Add(TEXT("鞏固根基：先鞏固廣東根據地，再北伐統一"));
    NationalistGovernment.RecommendedStrategies.Add(TEXT("政治統一：利用政治手段，爭取地方勢力支持"));
    NationalistGovernment.RecommendedStrategies.Add(TEXT("現代化建設：大力發展經濟和教育，增強國力"));
    NationalistGovernment.RecommendedStrategies.Add(TEXT("軍事現代化：建立現代化軍隊，提升戰鬥力"));

    RegisterFaction(NationalistGovernment);

    // 3. 中國共產黨
    FFactionData CommunistParty;
    CommunistParty.FactionID = 3;
    CommunistParty.FactionName = TEXT("中國共產黨");
    CommunistParty.FactionType = EFactionType::Revolutionary;
    CommunistParty.Difficulty = EFactionDifficulty::Hard;
    CommunistParty.StartingLocation = TEXT("延安/江西");
    CommunistParty.StartYear = 1927;
    CommunistParty.LeaderName = TEXT("毛澤東");
    CommunistParty.Description = TEXT("以馬克思主義為指導的無產階級政黨，實行人民戰爭和游擊戰術");
    CommunistParty.InitialResources = 500;
    CommunistParty.InitialPopulation = 100000;
    CommunistParty.InitialMilitary = 50;
    CommunistParty.DiplomaticModifier = -0.1f;
    CommunistParty.EconomicModifier = -0.2f;
    CommunistParty.MilitaryModifier = 0.0f;
    CommunistParty.TechnologyModifier = -0.1f;
    CommunistParty.CultureModifier = 0.3f;

    // 共產黨特色機制
    FFactionSpecialMechanic PeoplesWar;
    PeoplesWar.MechanicName = TEXT("人民戰爭");
    PeoplesWar.Description = TEXT("群眾路線，游擊戰術，土地改革");
    PeoplesWar.MechanicType = TEXT("Military");
    PeoplesWar.EffectValue = 0.2f;
    CommunistParty.SpecialMechanics.Add(PeoplesWar);

    FFactionSpecialMechanic Ideology;
    Ideology.MechanicName = TEXT("意識形態");
    Ideology.Description = TEXT("馬克思主義，國際主義，階級鬥爭");
    Ideology.MechanicType = TEXT("Political");
    Ideology.EffectValue = 0.25f;
    CommunistParty.SpecialMechanics.Add(Ideology);

    // 共產黨專屬單位
    FFactionUnit RedArmySoldier;
    RedArmySoldier.UnitName = TEXT("紅軍戰士");
    RedArmySoldier.UnitType = EFactionUnitType::Infantry;
    RedArmySoldier.Description = TEXT("高士氣、游擊戰專家、輕裝備");
    RedArmySoldier.Attack = 6;
    RedArmySoldier.Defense = 5;
    RedArmySoldier.Movement = 5;
    RedArmySoldier.Cost = 80;
    RedArmySoldier.Upkeep = 8;
    RedArmySoldier.SpecialAbility = TEXT("游擊戰術");
    CommunistParty.UniqueUnits.Add(RedArmySoldier);

    FFactionUnit GuerrillaFighter;
    GuerrillaFighter.UnitName = TEXT("游擊隊員");
    GuerrillaFighter.UnitType = EFactionUnitType::Militia;
    GuerrillaFighter.Description = TEXT("偵察、破壞、奇襲專家");
    GuerrillaFighter.Attack = 7;
    GuerrillaFighter.Defense = 4;
    GuerrillaFighter.Movement = 6;
    GuerrillaFighter.Cost = 60;
    GuerrillaFighter.Upkeep = 6;
    GuerrillaFighter.SpecialAbility = TEXT("游擊作戰");
    CommunistParty.UniqueUnits.Add(GuerrillaFighter);

    FFactionUnit MilitiaSelfDefense;
    MilitiaSelfDefense.UnitName = TEXT("民兵自衛隊");
    MilitiaSelfDefense.UnitType = EFactionUnitType::Militia;
    MilitiaSelfDefense.Description = TEXT("本地人、熟悉地形、數量眾多");
    MilitiaSelfDefense.Attack = 4;
    MilitiaSelfDefense.Defense = 4;
    MilitiaSelfDefense.Movement = 4;
    MilitiaSelfDefense.Cost = 40;
    MilitiaSelfDefense.Upkeep = 4;
    MilitiaSelfDefense.SpecialAbility = TEXT("本土作戰");
    CommunistParty.UniqueUnits.Add(MilitiaSelfDefense);

    FFactionUnit PoliticalCommissar;
    PoliticalCommissar.UnitName = TEXT("政治委員");
    PoliticalCommissar.UnitType = EFactionUnitType::Political;
    PoliticalCommissar.Description = TEXT("提升士氣、政治教育、組織能力");
    PoliticalCommissar.Attack = 3;
    PoliticalCommissar.Defense = 3;
    PoliticalCommissar.Movement = 4;
    PoliticalCommissar.Cost = 100;
    PoliticalCommissar.Upkeep = 10;
    PoliticalCommissar.SpecialAbility = TEXT("政治工作");
    CommunistParty.UniqueUnits.Add(PoliticalCommissar);

    // 優勢劣勢
    CommunistParty.Advantages.Add(TEXT("群眾基礎深厚"));
    CommunistParty.Advantages.Add(TEXT("意識形態凝聚力強"));
    CommunistParty.Advantages.Add(TEXT("戰術靈活，適應性強"));
    CommunistParty.Advantages.Add(TEXT("國際支持穩定"));

    CommunistParty.Disadvantages.Add(TEXT("起始實力最弱"));
    CommunistParty.Disadvantages.Add(TEXT("裝備落後，物資缺乏"));
    CommunistParty.Disadvantages.Add(TEXT("面臨強敵圍剿"));
    CommunistParty.Disadvantages.Add(TEXT("經濟困難"));

    // 推薦玩法
    CommunistParty.RecommendedStrategies.Add(TEXT("農村包圍城市：先建立農村根據地，再向城市發展"));
    CommunistParty.RecommendedStrategies.Add(TEXT("游擊戰術：利用游擊戰術消耗敵人力量"));
    CommunistParty.RecommendedStrategies.Add(TEXT("群眾動員：深入群眾，建立廣泛統一戰線"));
    CommunistParty.RecommendedStrategies.Add(TEXT("逐步壯大：積蓄力量，等待時機"));

    RegisterFaction(CommunistParty);

    UE_LOG(LogTemp, Log, TEXT("Initialized %d default factions"), AllFactions.Num());
}

void URepublicEraFactionSystem::LoadFactionData()
{
    // 這裡可以從文件加載額外的勢力數據
    // 目前使用默認數據
    UE_LOG(LogTemp, Log, TEXT("Faction data loaded successfully"));
}

void URepublicEraFactionSystem::RegisterFaction(const FFactionData& FactionData)
{
    if (ValidateFactionData(FactionData))
    {
        AllFactions.Add(FactionData);
        
        // 添加到數據表
        FFactionDataRow* NewRow = new FFactionDataRow();
        NewRow->FactionData = FactionData;
        FString RowName = FString::Printf(TEXT("Faction_%d"), FactionData.FactionID);
        FactionDataTable->AddRow(FName(*RowName), *NewRow);
        
        UE_LOG(LogTemp, Log, TEXT("Registered faction: %s"), *FactionData.FactionName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to validate faction data: %s"), *FactionData.FactionName);
    }
}

TArray<FFactionData> URepublicEraFactionSystem::GetAllFactions() const
{
    return AllFactions;
}

FFactionData URepublicEraFactionSystem::GetFactionData(int32 FactionID) const
{
    for (const FFactionData& Faction : AllFactions)
    {
        if (Faction.FactionID == FactionID)
        {
            return Faction;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Faction with ID %d not found"), FactionID);
    return FFactionData();
}

FFactionData URepublicEraFactionSystem::GetFactionDataByName(const FString& FactionName) const
{
    for (const FFactionData& Faction : AllFactions)
    {
        if (Faction.FactionName == FactionName)
        {
            return Faction;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Faction with name %s not found"), *FactionName);
    return FFactionData();
}

TArray<FFactionUnit> URepublicEraFactionSystem::GetFactionUnits(int32 FactionID) const
{
    FFactionData FactionData = GetFactionData(FactionID);
    return FactionData.UniqueUnits;
}

TArray<FFactionSpecialMechanic> URepublicEraFactionSystem::GetFactionMechanics(int32 FactionID) const
{
    FFactionData FactionData = GetFactionData(FactionID);
    return FactionData.SpecialMechanics;
}

bool URepublicEraFactionSystem::IsFactionAvailable(int32 FactionID) const
{
    return GetFactionData(FactionID).FactionID != 0;
}

int32 URepublicEraFactionSystem::GetFactionCount() const
{
    return AllFactions.Num();
}

FFactionData URepublicEraFactionSystem::CreateFactionInstance(int32 FactionID)
{
    FFactionData FactionData = GetFactionData(FactionID);
    if (FactionData.FactionID != 0)
    {
        // 創建副本以避免修改原始數據
        FFactionData Instance = FactionData;
        ApplyFactionModifiers(Instance);
        return Instance;
    }
    
    return FFactionData();
}

void URepublicEraFactionSystem::ApplyFactionModifiers(FFactionData& FactionData)
{
    // 應用各種修正值
    FactionData.InitialResources = FMath::RoundToInt(FactionData.InitialResources * (1.0f + FactionData.EconomicModifier));
    FactionData.InitialMilitary = FMath::RoundToInt(FactionData.InitialMilitary * (1.0f + FactionData.MilitaryModifier));
    
    UE_LOG(LogTemp, Log, TEXT("Applied modifiers for faction: %s"), *FactionData.FactionName);
}

bool URepublicEraFactionSystem::ValidateFactionData(const FFactionData& FactionData) const
{
    if (FactionData.FactionName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Faction name is empty"));
        return false;
    }
    
    if (FactionData.FactionID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid faction ID: %d"), FactionData.FactionID);
        return false;
    }
    
    if (FactionData.UniqueUnits.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Faction %s has no unique units"), *FactionData.FactionName);
    }
    
    if (FactionData.SpecialMechanics.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Faction %s has no special mechanics"), *FactionData.FactionName);
    }
    
    return true;
}
