#include "Factions/MingRTSFactionManager.h"
#include "Engine/Texture2D.h"

UMingRTSFactionManager::UMingRTSFactionManager()
    : PlayerFaction(EFactionType::Invalid)
    , bInitialized(false)
    , CurrentTurn(0)
{
}

void UMingRTSFactionManager::BeginDestroy()
{
    ShutdownFactionManager();
    Super::BeginDestroy();
}

void UMingRTSFactionManager::InitializeFactionManager()
{
    if (bInitialized)
    {
        return;
    }

    FactionRegistry.Empty();
    PlayerFaction = EFactionType::Invalid;
    CurrentTurn = 0;

    // 初始化所有12個默認勢力
    InitializeAllDefaultFactions();

    // 初始化勢力關係
    InitializeFactionRelations();

    bInitialized = true;
}

void UMingRTSFactionManager::ShutdownFactionManager()
{
    FactionRegistry.Empty();
    PlayerFaction = EFactionType::Invalid;
    bInitialized = false;
}

void UMingRTSFactionManager::InitializeAllDefaultFactions()
{
    InitializeBeiyangGovernment();
    InitializeNationalistGovernment();
    InitializeChineseCommunistParty();
    InitializeFengtianClique();
    InitializeZhiliClique();
    InitializeAnhuiClique();
    InitializeJinClique();
    InitializeGuangxiClique();
    InitializeYunnanClique();
    InitializeSichuanClique();
    InitializeMaFamily();
    InitializeXinjiangFaction();
}

void UMingRTSFactionManager::InitializeBeiyangGovernment()
{
    FFactionData Data;
    Data.FactionType = EFactionType::BeiyangGovernment;
    Data.FactionName = TEXT("北洋政府");
    Data.FactionDescription = TEXT("中央正統，外交優勢。掌握北京政權，擁有合法性和外交資源。");
    Data.Difficulty = EFactionDifficulty::Easy;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Beijing;
    Data.StartingConfig.StartingYear = 1912;
    Data.StartingConfig.StartingGold = 2000;
    Data.StartingConfig.StartingPopulation = 50000;
    Data.StartingConfig.StartingUnits = 8;
    Data.StartingConfig.StartingBuildings = 5;

    Data.FactionColor = FLinearColor(0.8f, 0.1f, 0.1f, 1.0f); // 深紅色

    // 特色機制
    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::CentralLegitimacy;
    Mechanic.MechanicName = TEXT("中央正統");
    Mechanic.Description = TEXT("外交關係+20%，其他勢力初始好感+10");
    Mechanic.EffectMultiplier = 1.2f;
    Data.SpecialMechanics.Add(Mechanic);

    // 專屬單位
    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::BeiyangEliteInfantry;
    Unit1.UnitName = TEXT("北洋精銳步兵");
    Unit1.Description = TEXT("訓練有素的正規軍，防禦力出色");
    Unit1.BaseHealth = 120;
    Unit1.BaseAttack = 15;
    Unit1.BaseDefense = 12;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::ZhongnanhaiGuard;
    Unit2.UnitName = TEXT("中南海衛隊");
    Unit2.Description = TEXT("精銳衛隊，護衛首領");
    Unit2.BaseHealth = 150;
    Unit2.BaseAttack = 20;
    Unit2.BaseDefense = 15;
    Data.UniqueUnits.Add(Unit2);

    // AI配置
    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::DiplomaticManipulation;
    Data.AIConfig.Aggressiveness = 0.3f;
    Data.AIConfig.DiplomacyWeight = 0.4f;
    Data.AIConfig.EconomicWeight = 0.3f;
    Data.AIConfig.MilitaryWeight = 0.3f;

    // 優勢劣勢
    Data.Advantages.Add(TEXT("外交優勢：容易獲得其他勢力認可"));
    Data.Advantages.Add(TEXT("經濟基礎：北京周邊經濟發達"));
    Data.Disadvantages.Add(TEXT("四面受敵：地理位置易受攻擊"));
    Data.Disadvantages.Add(TEXT("內部矛盾：軍閥派系林立"));
    Data.PlaystyleRecommendations.Add(TEXT("利用外交優勢結盟"));
    Data.PlaystyleRecommendations.Add(TEXT"優先發展經濟鞏固基礎");

    FactionRegistry.Add(EFactionType::BeiyangGovernment, Data);
}

void UMingRTSFactionManager::InitializeNationalistGovernment()
{
    FFactionData Data;
    Data.FactionType = EFactionType::NationalistGovernment;
    Data.FactionName = TEXT("國民政府");
    Data.FactionDescription = TEXT("民族主義，現代化。致力於統一中國，實現現代化。");
    Data.Difficulty = EFactionDifficulty::Normal;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Nanjing;
    Data.StartingConfig.SecondaryRegions.Add(EStartingRegion::Guangzhou);
    Data.StartingConfig.StartingYear = 1925;
    Data.StartingConfig.StartingGold = 1500;
    Data.StartingConfig.StartingPopulation = 40000;
    Data.StartingConfig.StartingUnits = 7;
    Data.StartingConfig.StartingBuildings = 4;

    Data.FactionColor = FLinearColor(0.0f, 0.3f, 0.8f, 1.0f); // 藍色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::Nationalism;
    Mechanic.MechanicName = TEXT("民族主義");
    Mechanic.Description = TEXT("招募速度+15%，部隊士氣+10%");
    Mechanic.EffectMultiplier = 1.15f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::WhampoaCadet;
    Unit1.UnitName = TEXT("黃埔學員");
    Unit1.Description = TEXT("軍校訓練的精英軍官");
    Unit1.BaseHealth = 110;
    Unit1.BaseAttack = 18;
    Unit1.BaseDefense = 10;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::NationalistGuard;
    Unit2.UnitName = TEXT("國民衛隊");
    Unit2.Description = TEXT("忠誠的國民革命軍");
    Unit2.BaseHealth = 115;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 11;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::AggressiveExpansion;
    Data.AIConfig.Aggressiveness = 0.6f;
    Data.AIConfig.DiplomacyWeight = 0.25f;
    Data.AIConfig.EconomicWeight = 0.25f;
    Data.AIConfig.MilitaryWeight = 0.5f;

    Data.Advantages.Add(TEXT("現代化部隊：黃埔軍校培養的精英"));
    Data.Advantages.Add(TEXT("政治正確：統一大業的正義性"));
    Data.Disadvantages.Add(TEXT("資金短缺：需要大量資金擴張"));
    Data.Disadvantages.Add(TEXT("內部分裂：各派閥爭鬥不休"));
    Data.PlaystyleRecommendations.Add(TEXT("快速擴張領土"));
    Data.PlaystyleRecommendations.Add(TEXT("利用政治優勢拉攏勢力"));

    FactionRegistry.Add(EFactionType::NationalistGovernment, Data);
}

void UMingRTSFactionManager::InitializeChineseCommunistParty()
{
    FFactionData Data;
    Data.FactionType = EFactionType::ChineseCommunistParty;
    Data.FactionName = TEXT("中國共產黨");
    Data.FactionDescription = TEXT("人民戰爭，游擊戰。動員群眾，持久抗戰。");
    Data.Difficulty = EFactionDifficulty::Hard;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Yanan;
    Data.StartingConfig.SecondaryRegions.Add(EStartingRegion::Jiangxi);
    Data.StartingConfig.StartingYear = 1927;
    Data.StartingConfig.StartingGold = 500;
    Data.StartingConfig.StartingPopulation = 20000;
    Data.StartingConfig.StartingUnits = 4;
    Data.StartingConfig.StartingBuildings = 2;

    Data.FactionColor = FLinearColor(0.9f, 0.1f, 0.1f, 1.0f); // 鮮紅色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::PeoplesWar;
    Mechanic.MechanicName = TEXT("人民戰爭");
    Mechanic.Description = TEXT("游擊戰傷害+25%，民眾支持度快速增長");
    Mechanic.EffectMultiplier = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::RedArmyVeteran;
    Unit1.UnitName = TEXT("紅軍老兵");
    Unit1.Description = TEXT("長征歷練的精銳戰士");
    Unit1.BaseHealth = 100;
    Unit1.BaseAttack = 14;
    Unit1.BaseDefense = 14;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::PeoplesGuerrilla;
    Unit2.UnitName = TEXT("人民游擊隊");
    Unit2.Description = TEXT("來去無蹤的游擊專家");
    Unit2.BaseHealth = 80;
    Unit2.BaseAttack = 12;
    Unit2.BaseDefense = 8;
    Unit2.MovementSpeed = 400.0f;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::AdaptiveStrategy;
    Data.AIConfig.Aggressiveness = 0.4f;
    Data.AIConfig.DiplomacyWeight = 0.2f;
    Data.AIConfig.EconomicWeight = 0.2f;
    Data.AIConfig.MilitaryWeight = 0.6f;
    Data.AIConfig.bAdaptiveBehavior = true;

    Data.Advantages.Add(TEXT("人民支持：高民眾忠誠度"));
    Data.Advantages.Add(TEXT("游擊專精：擅長不對稱作戰"));
    Data.Disadvantages.Add(TEXT("資源匱乏：起始資源極少"));
    Data.Disadvantages.Add(TEXT("敵對狀態：多數勢力視為敵人"));
    Data.PlaystyleRecommendations.Add(TEXT("發展游擊戰術"));
    Data.PlaystyleRecommendations.Add(TEXT("依靠民眾支持"));

    FactionRegistry.Add(EFactionType::ChineseCommunistParty, Data);
}

void UMingRTSFactionManager::InitializeFengtianClique()
{
    FFactionData Data;
    Data.FactionType = EFactionType::FengtianClique;
    Data.FactionName = TEXT("奉系軍閥");
    Data.FactionDescription = TEXT("東北工業，騎兵優勢。掌控東北三省，工業基礎雄厚。");
    Data.Difficulty = EFactionDifficulty::Normal;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Shenyang;
    Data.StartingConfig.StartingYear = 1916;
    Data.StartingConfig.StartingGold = 1800;
    Data.StartingConfig.StartingPopulation = 35000;
    Data.StartingConfig.StartingUnits = 7;
    Data.StartingConfig.StartingBuildings = 5;

    Data.FactionColor = FLinearColor(0.2f, 0.6f, 0.2f, 1.0f); // 綠色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::NortheastIndustry;
    Mechanic.MechanicName = TEXT("東北工業");
    Mechanic.Description = TEXT("生產效率+20%，騎兵單位+15%攻擊");
    Mechanic.EffectMultiplier = 1.2f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::NortheastCavalry;
    Unit1.UnitName = TEXT("東北騎兵");
    Unit1.Description = TEXT("馳騁東北的精銳騎兵");
    Unit1.BaseHealth = 130;
    Unit1.BaseAttack = 17;
    Unit1.BaseDefense = 9;
    Unit1.MovementSpeed = 450.0f;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::MukdenElite;
    Unit2.UnitName = TEXT("奉天精銳");
    Unit2.Description = TEXT("張作霖的親衛部隊");
    Unit2.BaseHealth = 140;
    Unit2.BaseAttack = 19;
    Unit2.BaseDefense = 13;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::MilitarySuperiority;
    Data.AIConfig.Aggressiveness = 0.5f;
    Data.AIConfig.DiplomacyWeight = 0.2f;
    Data.AIConfig.EconomicWeight = 0.3f;
    Data.AIConfig.MilitaryWeight = 0.5f;

    Data.Advantages.Add(TEXT("工業基礎：東北地區工業發達"));
    Data.Advantages.Add(TEXT("騎兵優勢：平原地形適合騎兵"));
    Data.Disadvantages.Add(TEXT("地理孤立：偏遠東北，擴張困難"));
    Data.Disadvantages.Add(TEXT("日俄影響：受外國勢力滲透"));
    Data.PlaystyleRecommendations.Add(TEXT("發展重工業"));
    Data.PlaystyleRecommendations.Add(TEXT("建立強大騎兵部隊"));

    FactionRegistry.Add(EFactionType::FengtianClique, Data);
}

void UMingRTSFactionManager::InitializeZhiliClique()
{
    FFactionData Data;
    Data.FactionType = EFactionType::ZhiliClique;
    Data.FactionName = TEXT("直系軍閥");
    Data.FactionDescription = TEXT("中原霸主，兵力龐大。控制中原要地，軍事實力強大。");
    Data.Difficulty = EFactionDifficulty::Normal;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Baoding;
    Data.StartingConfig.SecondaryRegions.Add(EStartingRegion::Luoyang);
    Data.StartingConfig.StartingYear = 1916;
    Data.StartingConfig.StartingGold = 1600;
    Data.StartingConfig.StartingPopulation = 45000;
    Data.StartingConfig.StartingUnits = 9;
    Data.StartingConfig.StartingBuildings = 4;

    Data.FactionColor = FLinearColor(0.6f, 0.4f, 0.2f, 1.0f); // 棕色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::CentralHegemony;
    Mechanic.MechanicName = TEXT("中原霸主");
    Mechanic.Description = TEXT("兵力上限+25%，徵兵速度+15%");
    Mechanic.EffectMultiplier = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::ZhiliRegularArmy;
    Unit1.UnitName = TEXT("直隸正規軍");
    Unit1.Description = TEXT("訓練有素的直隸部隊");
    Unit1.BaseHealth = 115;
    Unit1.BaseAttack = 15;
    Unit1.BaseDefense = 11;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::CentralPlainWarrior;
    Unit2.UnitName = TEXT("中原勇士");
    Unit2.Description = TEXT("驍勇善戰的中原將士");
    Unit2.BaseHealth = 110;
    Unit2.BaseAttack = 17;
    Unit2.BaseDefense = 10;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::AggressiveExpansion;
    Data.AIConfig.Aggressiveness = 0.7f;
    Data.AIConfig.DiplomacyWeight = 0.15f;
    Data.AIConfig.EconomicWeight = 0.2f;
    Data.AIConfig.MilitaryWeight = 0.65f;

    Data.Advantages.Add(TEXT("兵力優勢：可維持大量軍隊"));
    Data.Advantages.Add(TEXT("戰略位置：中原要地四通八達"));
    Data.Disadvantages.Add(TEXT":"四面受敵：易被多線攻擊"));
    Data.Disadvantages.Add(TEXT("經濟壓力：大軍消耗資源"));
    Data.PlaystyleRecommendations.Add(TEXT("快速擴張勢力範圍"));
    Data.PlaystyleRecommendations.Add(TEXT("利用數量優勢壓制敵人"));

    FactionRegistry.Add(EFactionType::ZhiliClique, Data);
}

void UMingRTSFactionManager::InitializeAnhuiClique()
{
    FFactionData Data;
    Data.FactionType = EFactionType::AnhuiClique;
    Data.FactionName = TEXT("皖系軍閥");
    Data.FactionDescription = TEXT("政治操作，日本援助。擅長政治外交，獲得日本支持。");
    Data.Difficulty = EFactionDifficulty::Normal;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Tianjin;
    Data.StartingConfig.SecondaryRegions.Add(EStartingRegion::Hefei);
    Data.StartingConfig.StartingYear = 1916;
    Data.StartingConfig.StartingGold = 1400;
    Data.StartingConfig.StartingPopulation = 30000;
    Data.StartingConfig.StartingUnits = 6;
    Data.StartingConfig.StartingBuildings = 4;

    Data.FactionColor = FLinearColor(0.4f, 0.3f, 0.6f, 1.0f); // 紫色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::PoliticalManeuvering;
    Mechanic.MechanicName = TEXT("政治操作");
    Mechanic.Description = TEXT("外交影響力+30%，可獲得日本援助");
    Mechanic.EffectMultiplier = 1.3f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::AnhuiMilitia;
    Unit1.UnitName = TEXT("皖系民兵");
    Unit1.Description = TEXT("地方訓練的防禦部隊");
    Unit1.BaseHealth = 100;
    Unit1.BaseAttack = 12;
    Unit1.BaseDefense = 12;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::PoliticalGuard;
    Unit2.UnitName = TEXT("政治衛隊");
    Unit2.Description = TEXT("段祺瑞的精銳護衛");
    Unit2.BaseHealth = 125;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 14;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::DiplomaticManipulation;
    Data.AIConfig.Aggressiveness = 0.4f;
    Data.AIConfig.DiplomacyWeight = 0.5f;
    Data.AIConfig.EconomicWeight = 0.25f;
    Data.AIConfig.MilitaryWeight = 0.25f;

    Data.Advantages.Add(TEXT("外交優勢：擅長政治操作"));
    Data.Advantages.Add(TEXT":"日本援助：可獲得外部支持"));
    Data.Disadvantages.Add(TEXT("軍力較弱：部隊戰鬥力一般"));
    Data.Disadvantages.Add(TEXT("依賴外援：受日本影響過深"));
    Data.PlaystyleRecommendations.Add(TEXT("利用外交手段分化敵人"));
    Data.PlaystyleRecommendations.Add(TEXT("獲取外部援助補充實力"));

    FactionRegistry.Add(EFactionType::AnhuiClique, Data);
}

void UMingRTSFactionManager::InitializeJinClique()
{
    FFactionData Data;
    Data.FactionType = EFactionType::JinClique;
    Data.FactionName = TEXT("晉系軍閥");
    Data.FactionDescription = TEXT("山西模範，防禦專精。閻錫山統治下的模範省，防禦工事堅固。");
    Data.Difficulty = EFactionDifficulty::Normal;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Taiyuan;
    Data.StartingConfig.StartingYear = 1911;
    Data.StartingConfig.StartingGold = 1300;
    Data.StartingConfig.StartingPopulation = 25000;
    Data.StartingConfig.StartingUnits = 6;
    Data.StartingConfig.StartingBuildings = 5;

    Data.FactionColor = FLinearColor(0.7f, 0.5f, 0.2f, 1.0f); // 土黃色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::ShanxiModel;
    Mechanic.MechanicName = TEXT("山西模範");
    Mechanic.Description = TEXT("防禦工事+30%，建築耐久+20%");
    Mechanic.EffectMultiplier = 1.3f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::ShanxiDefender;
    Unit1.UnitName = TEXT("山西衛士");
    Unit1.Description = TEXT("守衛家鄉的堅定戰士");
    Unit1.BaseHealth = 125;
    Unit1.BaseAttack = 12;
    Unit1.BaseDefense = 16;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::TaiyuanElite;
    Unit2.UnitName = TEXT("太原精銳");
    Unit2.Description = TEXT("閻錫山精心培養的部隊");
    Unit2.BaseHealth = 130;
    Unit2.BaseAttack = 14;
    Unit2.BaseDefense = 18;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::DefensiveConsolidation;
    Data.AIConfig.Aggressiveness = 0.3f;
    Data.AIConfig.DiplomacyWeight = 0.3f;
    Data.AIConfig.EconomicWeight = 0.3f;
    Data.AIConfig.MilitaryWeight = 0.4f;

    Data.Advantages.Add(TEXT("防禦優勢：極佳的防禦工事"));
    Data.Advantages.Add(TEXT("經濟穩定：模範省治理有方"));
    Data.Disadvantages.Add(TEXT("擴張困難：地理位置封閉"));
    Data.Disadvantages.Add(TEXT("人口有限：兵源相對不足"));
    Data.PlaystyleRecommendations.Add(TEXT("構建堅固防線"));
    Data.PlaystyleRecommendations.Add(TEXT":"穩步發展經濟"));

    FactionRegistry.Add(EFactionType::JinClique, Data);
}

void UMingRTSFactionManager::InitializeGuangxiClique()
{
    FFactionData Data;
    Data.FactionType = EFactionType::GuangxiClique;
    Data.FactionName = TEXT("桂系軍閥");
    Data.FactionDescription = TEXT("廣西民兵，山地戰。李宗仁、白崇禧統治，民風彪悍。");
    Data.Difficulty = EFactionDifficulty::Normal;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Guilin;
    Data.StartingConfig.SecondaryRegions.Add(EStartingRegion::Nanning);
    Data.StartingConfig.StartingYear = 1924;
    Data.StartingConfig.StartingGold = 1200;
    Data.StartingConfig.StartingPopulation = 28000;
    Data.StartingConfig.StartingUnits = 6;
    Data.StartingConfig.StartingBuildings = 3;

    Data.FactionColor = FLinearColor(0.3f, 0.7f, 0.3f, 1.0f); // 草綠色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::GuangxiMilitia;
    Mechanic.MechanicName = TEXT("廣西民兵");
    Mechanic.Description = TEXT("山地作戰+25%，民兵成本-20%");
    Mechanic.EffectMultiplier = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::GuangxiWolf;
    Unit1.UnitName = TEXT("廣西狼兵");
    Unit1.Description = TEXT("驍勇善戰的廣西勇士");
    Unit1.BaseHealth = 115;
    Unit1.BaseAttack = 16;
    Unit1.BaseDefense = 10;
    Unit1.MovementSpeed = 380.0f;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::MountainFighter;
    Unit2.UnitName = TEXT("山地戰士");
    Unit2.Description = TEXT("擅長山地作戰的專家");
    Unit2.BaseHealth = 105;
    Unit2.BaseAttack = 15;
    Unit2.BaseDefense = 13;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::MilitarySuperiority;
    Data.AIConfig.Aggressiveness = 0.6f;
    Data.AIConfig.DiplomacyWeight = 0.2f;
    Data.AIConfig.EconomicWeight = 0.2f;
    Data.AIConfig.MilitaryWeight = 0.6f;

    Data.Advantages.Add(TEXT("山地戰專精：複雜地形作戰優勢"));
    Data.Advantages.Add(TEXT("民風彪悍：部隊士氣高昂"));
    Data.Disadvantages.Add(TEXT("資源有限：經濟基礎薄弱"));
    Data.Disadvantages.Add(TEXT("地理偏遠：遠離政治中心"));
    Data.PlaystyleRecommendations.Add(TEXT("利用地形優勢"));
    Data.PlaystyleRecommendations.Add(TEXT("發展輕裝部隊"));

    FactionRegistry.Add(EFactionType::GuangxiClique, Data);
}

void UMingRTSFactionManager::InitializeYunnanClique()
{
    FFactionData Data;
    Data.FactionType = EFactionType::YunnanClique;
    Data.FactionName = TEXT("滇系軍閥");
    Data.FactionDescription = TEXT("護國傳統，邊疆擴張。唐繼堯領導，有護國戰爭光榮歷史。");
    Data.Difficulty = EFactionDifficulty::Hard;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Kunming;
    Data.StartingConfig.StartingYear = 1915;
    Data.StartingConfig.StartingGold = 1100;
    Data.StartingConfig.StartingPopulation = 22000;
    Data.StartingConfig.StartingUnits = 5;
    Data.StartingConfig.StartingBuildings = 3;

    Data.FactionColor = FLinearColor(0.2f, 0.5f, 0.7f, 1.0f); // 青藍色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::DefenseOfNation;
    Mechanic.MechanicName = TEXT("護國傳統");
    Mechanic.Description = TEXT("正義戰爭士氣+20%，邊疆擴張成本-15%");
    Mechanic.EffectMultiplier = 1.2f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::YunnanIronBull;
    Unit1.UnitName = TEXT("雲南鐵牛");
    Unit1.Description = TEXT("頑強耐戰的滇軍勇士");
    Unit1.BaseHealth = 120;
    Unit1.BaseAttack = 14;
    Unit1.BaseDefense = 13;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::FrontierDefender;
    Unit2.UnitName = TEXT("邊疆衛士");
    Unit2.Description = TEXT("守護邊境的忠誠部隊");
    Unit2.BaseHealth = 115;
    Unit2.BaseAttack = 13;
    Unit2.BaseDefense = 15;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::BalancedApproach;
    Data.AIConfig.Aggressiveness = 0.5f;
    Data.AIConfig.DiplomacyWeight = 0.25f;
    Data.AIConfig.EconomicWeight = 0.25f;
    Data.AIConfig.MilitaryWeight = 0.5f;

    Data.Advantages.Add(TEXT":"地理優勢：易守難攻"));
    Data.Advantages.Add(TEXT("政治正確：護國戰爭光環"));
    Data.Disadvantages.Add(TEXT("資源匱乏：邊疆地區貧困"));
    Data.Disadvantages.Add(TEXT("交通不便：對外聯繫困難"));
    Data.PlaystyleRecommendations.Add(TEXT("穩步向外擴張"));
    Data.PlaystyleRecommendations.Add(TEXT":"利用正義性吸引支持"));

    FactionRegistry.Add(EFactionType::YunnanClique, Data);
}

void UMingRTSFactionManager::InitializeSichuanClique()
{
    FFactionData Data;
    Data.FactionType = EFactionType::SichuanClique;
    Data.FactionName = TEXT("川系軍閥");
    Data.FactionDescription = TEXT("防區制，內部統一。劉湘統治，通過防區制維持內部穩定。");
    Data.Difficulty = EFactionDifficulty::Hard;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Chengdu;
    Data.StartingConfig.SecondaryRegions.Add(EStartingRegion::Chongqing);
    Data.StartingConfig.StartingYear = 1918;
    Data.StartingConfig.StartingGold = 1300;
    Data.StartingConfig.StartingPopulation = 35000;
    Data.StartingConfig.StartingUnits = 6;
    Data.StartingConfig.StartingBuildings = 4;

    Data.FactionColor = FLinearColor(0.5f, 0.3f, 0.3f, 1.0f); // 暗紅色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::DefenseZoneSystem;
    Mechanic.MechanicName = TEXT("防區制");
    Mechanic.Description = TEXT("內部穩定+30%，地方收入+15%");
    Mechanic.EffectMultiplier = 1.3f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::SichuanWarlord;
    Unit1.UnitName = TEXT("四川軍閥");
    Unit1.Description = TEXT("地方軍閥的精銳部隊");
    Unit1.BaseHealth = 118;
    Unit1.BaseAttack = 15;
    Unit1.BaseDefense = 11;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::BaFighter;
    Unit2.UnitName = TEXT("巴國戰士");
    Unit2.Description = TEXT("古巴蜀後裔的勇士");
    Unit2.BaseHealth = 112;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 10;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::DefensiveConsolidation;
    Data.AIConfig.Aggressiveness = 0.4f;
    Data.AIConfig.DiplomacyWeight = 0.3f;
    Data.AIConfig.EconomicWeight = 0.3f;
    Data.AIConfig.MilitaryWeight = 0.4f;

    Data.Advantages.Add(TEXT":"人口眾多：四川盆地人口密集"));
    Data.Advantages.Add(TEXT("天府之國：物產豐富"));
    Data.Disadvantages.Add(TEXT("內部不穩：防區制導致分裂"));
    Data.Disadvantages.Add(TEXT":"出川困難：周邊地形險峻"));
    Data.PlaystyleRecommendations.Add(TEXT("先統一內部"));
    Data.PlaystyleRecommendations.Add(TEXT("積蓄力量後出川"));

    FactionRegistry.Add(EFactionType::SichuanClique, Data);
}

void UMingRTSFactionManager::InitializeMaFamily()
{
    FFactionData Data;
    Data.FactionType = EFactionType::MaFamily;
    Data.FactionName = TEXT("馬家軍");
    Data.FactionDescription = TEXT("回族騎兵，宗教團結。西北三馬，以騎兵和宗教凝聚力著稱。");
    Data.Difficulty = EFactionDifficulty::Hard;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Lanzhou;
    Data.StartingConfig.SecondaryRegions.Add(EStartingRegion::Xining);
    Data.StartingConfig.StartingYear = 1912;
    Data.StartingConfig.StartingGold = 900;
    Data.StartingConfig.StartingPopulation = 15000;
    Data.StartingConfig.StartingUnits = 5;
    Data.StartingConfig.StartingBuildings = 3;

    Data.FactionColor = FLinearColor(0.6f, 0.6f, 0.2f, 1.0f); // 黃綠色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::ReligiousUnity;
    Mechanic.MechanicName = TEXT("宗教團結");
    Mechanic.Description = TEXT("部隊士氣+25%，騎兵衝鋒+20%傷害");
    Mechanic.EffectMultiplier = 1.25f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::HuiCavalry;
    Unit1.UnitName = TEXT("回民騎兵");
    Unit1.Description = TEXT("驍勇的回族騎兵");
    Unit1.BaseHealth = 110;
    Unit1.BaseAttack = 18;
    Unit1.BaseDefense = 8;
    Unit1.MovementSpeed = 480.0f;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::SilkRoadWarrior;
    Unit2.UnitName = TEXT("絲路勇士");
    Unit2.Description = TEXT("守護絲綢之路的戰士");
    Unit2.BaseHealth = 120;
    Unit2.BaseAttack = 16;
    Unit2.BaseDefense = 11;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::MilitarySuperiority;
    Data.AIConfig.Aggressiveness = 0.6f;
    Data.AIConfig.DiplomacyWeight = 0.15f;
    Data.AIConfig.EconomicWeight = 0.25f;
    Data.AIConfig.MilitaryWeight = 0.6f;

    Data.Advantages.Add(TEXT("騎兵優勢：西北騎兵戰力強大"));
    Data.Advantages.Add(TEXT("宗教團結：高凝聚力和士氣"));
    Data.Disadvantages.Add(TEXT("資源極少：西北貧瘠"));
    Data.Disadvantages.Add(TEXT("科技落後：現代化程度低"));
    Data.PlaystyleRecommendations.Add(TEXT("發展精銳騎兵"));
    Data.PlaystyleRecommendations.Add(TEXT":"快速突襲戰術"));

    FactionRegistry.Add(EFactionType::MaFamily, Data);
}

void UMingRTSFactionManager::InitializeXinjiangFaction()
{
    FFactionData Data;
    Data.FactionType = EFactionType::XinjiangFaction;
    Data.FactionName = TEXT("新疆勢力");
    Data.FactionDescription = TEXT("邊疆要塞，民族複雜。楊增新統治，地處邊陲，民族關係複雜。");
    Data.Difficulty = EFactionDifficulty::VeryHard;
    Data.CurrentState = EFactionState::Active;
    
    Data.StartingConfig.PrimaryRegion = EStartingRegion::Dihua;
    Data.StartingConfig.StartingYear = 1912;
    Data.StartingConfig.StartingGold = 600;
    Data.StartingConfig.StartingPopulation = 12000;
    Data.StartingConfig.StartingUnits = 4;
    Data.StartingConfig.StartingBuildings = 2;

    Data.FactionColor = FLinearColor(0.4f, 0.4f, 0.5f, 1.0f); // 灰藍色

    FFactionMechanicData Mechanic;
    Mechanic.MechanicType = EFactionSpecialMechanic::FrontierFortress;
    Mechanic.MechanicName = TEXT("邊疆要塞");
    Mechanic.Description = TEXT("邊境防禦+40%，貿易路線收入+25%");
    Mechanic.EffectMultiplier = 1.4f;
    Data.SpecialMechanics.Add(Mechanic);

    FUniqueUnitData Unit1;
    Unit1.UnitType = EUniqueUnitType::XinjiangGuard;
    Unit1.UnitName = TEXT("新疆衛隊");
    Unit1.Description = TEXT("守衛邊疆的多民族部隊");
    Unit1.BaseHealth = 115;
    Unit1.BaseAttack = 13;
    Unit1.BaseDefense = 14;
    Data.UniqueUnits.Add(Unit1);

    FUniqueUnitData Unit2;
    Unit2.UnitType = EUniqueUnitType::FrontierVeteran;
    Unit2.UnitName = TEXT("邊疆老兵");
    Unit2.Description = TEXT("久經沙場的邊防老兵");
    Unit2.BaseHealth = 125;
    Unit2.BaseAttack = 14;
    Unit2.BaseDefense = 16;
    Data.UniqueUnits.Add(Unit2);

    Data.AIConfig.PrimaryStrategy = EFactionAIStrategy::DefensiveConsolidation;
    Data.AIConfig.Aggressiveness = 0.2f;
    Data.AIConfig.DiplomacyWeight = 0.4f;
    Data.AIConfig.EconomicWeight = 0.3f;
    Data.AIConfig.MilitaryWeight = 0.3f;

    Data.Advantages.Add(TEXT("邊防優勢：極佳的邊境防禦"));
    Data.Advantages.Add(TEXT("貿易路線：絲綢之路收益"));
    Data.Disadvantages.Add(TEXT("資源極度匱乏：最貧困的起始"));
    Data.Disadvantages.Add(TEXT("民族複雜：內部不穩定因素"));
    Data.Disadvantages.Add(TEXT("完全孤立：遠離所有勢力"));
    Data.PlaystyleRecommendations.Add(TEXT":"專注防禦發展"));
    Data.PlaystyleRecommendations.Add(TEXT("利用貿易路線積累資源"));
    Data.PlaystyleRecommendations.Add(TEXT":"避免早期擴張"));

    FactionRegistry.Add(EFactionType::XinjiangFaction, Data);
}

void UMingRTSFactionManager::InitializeFactionRelations()
{
    // 設置初始勢力關係
    // 北洋 vs 國民黨 - 敵對
    SetFactionRelation(EFactionType::BeiyangGovernment, EFactionType::NationalistGovernment, EFactionRelationType::Hostile);
    
    // 北洋 vs 共產黨 - 戰爭
    SetFactionRelation(EFactionType::BeiyangGovernment, EFactionType::ChineseCommunistParty, EFactionRelationType::War);
    
    // 國民黨 vs 共產黨 - 敵對 (後期變為戰爭)
    SetFactionRelation(EFactionType::NationalistGovernment, EFactionType::ChineseCommunistParty, EFactionRelationType::Hostile);
    
    // 直系 vs 皖系 - 敵對 (直皖戰爭)
    SetFactionRelation(EFactionType::ZhiliClique, EFactionType::AnhuiClique, EFactionRelationType::Hostile);
    
    // 直系 vs 奉系 - 敵對 (直奉戰爭)
    SetFactionRelation(EFactionType::ZhiliClique, EFactionType::FengtianClique, EFactionRelationType::Hostile);
    
    // 奉系 vs 晉系 - 中立偏友好
    SetFactionRelation(EFactionType::FengtianClique, EFactionType::JinClique, EFactionRelationType::Neutral);
    ModifyFactionRelationValue(EFactionType::FengtianClique, EFactionType::JinClique, 10.0f);
    
    // 設置所有未設置關係為中立
    TArray<EFactionType> AllFactions = GetAllRegisteredFactions();
    for (EFactionType FactionA : AllFactions)
    {
        for (EFactionType FactionB : AllFactions)
        {
            if (FactionA != FactionB)
            {
                // 如果關係尚未設置，設為中立
                if (GetFactionRelation(FactionA, FactionB) == EFactionRelationType::Neutral &&
                    GetFactionRelationValue(FactionA, FactionB) == 0.0f)
                {
                    // 保持中立，但根據地理位置微調
                    // 相鄰勢力初始關係稍好
                    ModifyFactionRelationValue(FactionA, FactionB, 5.0f);
                }
            }
        }
    }
}

// 其餘函數實現...
void UMingRTSFactionManager::RegisterFaction(const FFactionData& FactionData)
{
    if (IsValidFaction(FactionData.FactionType))
    {
        FactionRegistry.Add(FactionData.FactionType, FactionData);
    }
}

void UMingRTSFactionManager::UnregisterFaction(EFactionType FactionType)
{
    if (FactionRegistry.Contains(FactionType))
    {
        FactionRegistry.Remove(FactionType);
    }
}

bool UMingRTSFactionManager::IsFactionRegistered(EFactionType FactionType) const
{
    return FactionRegistry.Contains(FactionType);
}

FFactionData UMingRTSFactionManager::GetFactionData(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return *Data;
    }
    return FFactionData();
}

void UMingRTSFactionManager::UpdateFactionData(EFactionType FactionType, const FFactionData& NewData)
{
    if (IsValidFaction(FactionType))
    {
        FactionRegistry.Add(FactionType, NewData);
    }
}

TArray<EFactionType> UMingRTSFactionManager::GetAllRegisteredFactions() const
{
    TArray<EFactionType> Result;
    FactionRegistry.GetKeys(Result);
    return Result;
}

TArray<EFactionType> UMingRTSFactionManager::GetActiveFactions() const
{
    TArray<EFactionType> Result;
    for (const auto& Pair : FactionRegistry)
    {
        if (Pair.Value.bIsAlive && Pair.Value.CurrentState != EFactionState::Defeated)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<FFactionData> UMingRTSFactionManager::GetAllFactionData() const
{
    TArray<FFactionData> Result;
    FactionRegistry.GenerateValueArray(Result);
    return Result;
}

void UMingRTSFactionManager::SetFactionRelation(EFactionType FactionA, EFactionType FactionB, EFactionRelationType RelationType)
{
    if (!AreFactionsValid(FactionA, FactionB))
    {
        return;
    }

    FFactionRelationData* DataA = GetRelationData(FactionA, FactionB);
    FFactionRelationData* DataB = GetRelationData(FactionB, FactionA);

    if (DataA && DataB)
    {
        DataA->RelationType = RelationType;
        DataB->RelationType = RelationType;

        // 根據關係類型設置關係值
        switch (RelationType)
        {
        case EFactionRelationType::Ally:
            DataA->RelationValue = 80.0f;
            DataB->RelationValue = 80.0f;
            break;
        case EFactionRelationType::Friendly:
            DataA->RelationValue = 50.0f;
            DataB->RelationValue = 50.0f;
            break;
        case EFactionRelationType::Neutral:
            DataA->RelationValue = 0.0f;
            DataB->RelationValue = 0.0f;
            break;
        case EFactionRelationType::Hostile:
            DataA->RelationValue = -50.0f;
            DataB->RelationValue = -50.0f;
            break;
        case EFactionRelationType::War:
            DataA->RelationValue = -100.0f;
            DataB->RelationValue = -100.0f;
            break;
        default:
            break;
        }

        OnFactionRelationChanged.Broadcast(FactionA, FactionB);
    }
}

EFactionRelationType UMingRTSFactionManager::GetFactionRelation(EFactionType FactionA, EFactionType FactionB) const
{
    if (const FFactionRelationData* Data = GetRelationData(FactionA, FactionB))
    {
        return Data->RelationType;
    }
    return EFactionRelationType::Neutral;
}

float UMingRTSFactionManager::GetFactionRelationValue(EFactionType FactionA, EFactionType FactionB) const
{
    if (const FFactionRelationData* Data = GetRelationData(FactionA, FactionB))
    {
        return Data->RelationValue;
    }
    return 0.0f;
}

void UMingRTSFactionManager::ModifyFactionRelationValue(EFactionType FactionA, EFactionType FactionB, float Delta)
{
    if (!AreFactionsValid(FactionA, FactionB))
    {
        return;
    }

    FFactionRelationData* DataA = GetRelationData(FactionA, FactionB);
    FFactionRelationData* DataB = GetRelationData(FactionB, FactionA);

    if (DataA && DataB)
    {
        DataA->RelationValue = FMath::Clamp(DataA->RelationValue + Delta, -100.0f, 100.0f);
        DataB->RelationValue = DataA->RelationValue;

        // 根據新的關係值更新關係類型
        DataA->RelationType = DetermineRelationTypeFromValue(DataA->RelationValue);
        DataB->RelationType = DataA->RelationType;

        OnFactionRelationChanged.Broadcast(FactionA, FactionB);
    }
}

void UMingRTSFactionManager::DeclareWar(EFactionType Aggressor, EFactionType Target)
{
    SetFactionRelation(Aggressor, Target, EFactionRelationType::War);
    
    // 更新戰爭計數
    FFactionRelationData* Data = GetRelationData(Aggressor, Target);
    if (Data)
    {
        Data->WarCount++;
        Data->LastWarTurn = CurrentTurn;
    }
}

void UMingRTSFactionManager::MakePeace(EFactionType FactionA, EFactionType FactionB)
{
    SetFactionRelation(FactionA, FactionB, EFactionRelationType::Hostile);
    ModifyFactionRelationValue(FactionA, FactionB, 10.0f); // 稍微改善關係
}

void UMingRTSFactionManager::FormAlliance(EFactionType FactionA, EFactionType FactionB)
{
    SetFactionRelation(FactionA, FactionB, EFactionRelationType::Ally);
}

void UMingRTSFactionManager::BreakAlliance(EFactionType FactionA, EFactionType FactionB)
{
    SetFactionRelation(FactionA, FactionB, EFactionRelationType::Friendly);
}

void UMingRTSFactionManager::SignNonAggressionPact(EFactionType FactionA, EFactionType FactionB, int32 DurationTurns)
{
    FFactionRelationData* DataA = GetRelationData(FactionA, FactionB);
    FFactionRelationData* DataB = GetRelationData(FactionB, FactionA);

    if (DataA && DataB)
    {
        DataA->bHasNonAggressionPact = true;
        DataA->NonAggressionPactTurns = DurationTurns;
        DataB->bHasNonAggressionPact = true;
        DataB->NonAggressionPactTurns = DurationTurns;
    }
}

TArray<EFactionType> UMingRTSFactionManager::GetAllies(EFactionType Faction) const
{
    TArray<EFactionType> Result;
    for (const auto& Pair : FactionRegistry)
    {
        if (Pair.Key != Faction && GetFactionRelation(Faction, Pair.Key) == EFactionRelationType::Ally)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<EFactionType> UMingRTSFactionManager::GetEnemies(EFactionType Faction) const
{
    TArray<EFactionType> Result;
    for (const auto& Pair : FactionRegistry)
    {
        if (Pair.Key != Faction)
        {
            EFactionRelationType Relation = GetFactionRelation(Faction, Pair.Key);
            if (Relation == EFactionRelationType::Hostile || Relation == EFactionRelationType::War)
            {
                Result.Add(Pair.Key);
            }
        }
    }
    return Result;
}

TArray<EFactionType> UMingRTSFactionManager::GetNeighbors(EFactionType Faction) const
{
    // 這個函數應該基於地圖數據返回相鄰勢力
    // 這裡提供一個簡化實現
    TArray<EFactionType> AllFactions = GetActiveFactions();
    AllFactions.Remove(Faction);
    return AllFactions;
}

void UMingRTSFactionManager::SetPlayerFaction(EFactionType FactionType)
{
    if (IsValidFaction(FactionType))
    {
        // 重置之前的玩家勢力
        if (IsValidFaction(PlayerFaction))
        {
            FFactionData OldData = GetFactionData(PlayerFaction);
            OldData.bIsPlayerControlled = false;
            UpdateFactionData(PlayerFaction, OldData);
        }

        // 設置新玩家勢力
        PlayerFaction = FactionType;
        FFactionData NewData = GetFactionData(FactionType);
        NewData.bIsPlayerControlled = true;
        UpdateFactionData(FactionType, NewData);

        OnPlayerFactionSelected.Broadcast(FactionType);
    }
}

EFactionType UMingRTSFactionManager::GetPlayerFaction() const
{
    return PlayerFaction;
}

bool UMingRTSFactionManager::IsPlayerFaction(EFactionType FactionType) const
{
    return FactionType == PlayerFaction;
}

void UMingRTSFactionManager::SetFactionState(EFactionType FactionType, EFactionState NewState)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->CurrentState = NewState;
    }
}

EFactionState UMingRTSFactionManager::GetFactionState(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return Data->CurrentState;
    }
    return EFactionState::Active;
}

void UMingRTSFactionManager::DefeatFaction(EFactionType FactionType)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->CurrentState = EFactionState::Defeated;
        Data->bIsAlive = false;
        OnFactionDefeated.Broadcast(FactionType);
    }
}

void UMingRTSFactionManager::ReviveFaction(EFactionType FactionType)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->CurrentState = EFactionState::Active;
        Data->bIsAlive = true;
    }
}

bool UMingRTSFactionManager::IsFactionAlive(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return Data->bIsAlive;
    }
    return false;
}

void UMingRTSFactionManager::AddVictoryPoints(EFactionType FactionType, int32 Points)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->VictoryPoints += Points;
        OnFactionVictoryPointsChanged.Broadcast(FactionType, Data->VictoryPoints);
    }
}

void UMingRTSFactionManager::SetVictoryPoints(EFactionType FactionType, int32 Points)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->VictoryPoints = FMath::Max(0, Points);
        OnFactionVictoryPointsChanged.Broadcast(FactionType, Data->VictoryPoints);
    }
}

int32 UMingRTSFactionManager::GetVictoryPoints(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return Data->VictoryPoints;
    }
    return 0;
}

EFactionType UMingRTSFactionManager::GetLeadingFaction() const
{
    EFactionType Leading = EFactionType::Invalid;
    int32 MaxPoints = -1;

    for (const auto& Pair : FactionRegistry)
    {
        if (Pair.Value.VictoryPoints > MaxPoints)
        {
            MaxPoints = Pair.Value.VictoryPoints;
            Leading = Pair.Key;
        }
    }

    return Leading;
}

TArray<EFactionType> UMingRTSFactionManager::GetFactionRanking() const
{
    TArray<EFactionType> Result;
    TArray<FFactionData> AllData = GetAllFactionData();

    // 按勝利點數排序
    AllData.Sort([](const FFactionData& A, const FFactionData& B) {
        return A.VictoryPoints > B.VictoryPoints;
    });

    for (const auto& Data : AllData)
    {
        Result.Add(Data.FactionType);
    }

    return Result;
}

void UMingRTSFactionManager::AddTerritory(EFactionType FactionType, int32 Count)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->ControlledTerritories += Count;
    }
}

void UMingRTSFactionManager::RemoveTerritory(EFactionType FactionType, int32 Count)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->ControlledTerritories = FMath::Max(0, Data->ControlledTerritories - Count);
    }
}

int32 UMingRTSFactionManager::GetTerritoryCount(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return Data->ControlledTerritories;
    }
    return 0;
}

void UMingRTSFactionManager::UpdateMilitaryStrength(EFactionType FactionType, int32 NewStrength)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->TotalMilitaryStrength = NewStrength;
    }
}

void UMingRTSFactionManager::UpdateEconomicOutput(EFactionType FactionType, int32 NewOutput)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->TotalEconomicOutput = NewOutput;
    }
}

int32 UMingRTSFactionManager::GetMilitaryStrength(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return Data->TotalMilitaryStrength;
    }
    return 0;
}

int32 UMingRTSFactionManager::GetEconomicOutput(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return Data->TotalEconomicOutput;
    }
    return 0;
}

void UMingRTSFactionManager::SetFactionAIStrategy(EFactionType FactionType, EFactionAIStrategy Strategy)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->AIConfig.PrimaryStrategy = Strategy;
    }
}

EFactionAIStrategy UMingRTSFactionManager::GetFactionAIStrategy(EFactionType FactionType) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        return Data->AIConfig.PrimaryStrategy;
    }
    return EFactionAIStrategy::BalancedApproach;
}

void UMingRTSFactionManager::SetFactionAIAdaptive(EFactionType FactionType, bool bAdaptive)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionType))
    {
        Data->AIConfig.bAdaptiveBehavior = bAdaptive;
    }
}

void UMingRTSFactionManager::OnTurnStart(int32 TurnNumber)
{
    CurrentTurn = TurnNumber;

    // 更新非侵略條約剩餘回合
    for (auto& Pair : FactionRegistry)
    {
        for (auto& RelationPair : Pair.Value.Relations)
        {
            if (RelationPair.Value.bHasNonAggressionPact && RelationPair.Value.NonAggressionPactTurns > 0)
            {
                RelationPair.Value.NonAggressionPactTurns--;
                if (RelationPair.Value.NonAggressionPactTurns <= 0)
                {
                    RelationPair.Value.bHasNonAggressionPact = false;
                }
            }
        }
    }
}

void UMingRTSFactionManager::OnTurnEnd(int32 TurnNumber)
{
    // 每回合結束時的處理
    // 例如：自動保存、統計更新等
}

FFactionData UMingRTSFactionManager::GetDefaultFactionData(EFactionType FactionType)
{
    // 創建臨時管理器獲取默認數據
    UMingRTSFactionManager* TempManager = NewObject<UMingRTSFactionManager>();
    TempManager->InitializeFactionManager();
    FFactionData Result = TempManager->GetFactionData(FactionType);
    TempManager->ShutdownFactionManager();
    return Result;
}

bool UMingRTSFactionManager::IsValidFaction(EFactionType FactionType) const
{
    return FactionType != EFactionType::Invalid && FactionType != EFactionType::Count;
}

bool UMingRTSFactionManager::AreFactionsValid(EFactionType FactionA, EFactionType FactionB) const
{
    return IsValidFaction(FactionA) && IsValidFaction(FactionB) && FactionA != FactionB;
}

FFactionRelationData* UMingRTSFactionManager::GetRelationData(EFactionType FactionA, EFactionType FactionB)
{
    if (FFactionData* Data = FactionRegistry.Find(FactionA))
    {
        return Data->Relations.Find(FactionB);
    }
    return nullptr;
}

const FFactionRelationData* UMingRTSFactionManager::GetRelationData(EFactionType FactionA, EFactionType FactionB) const
{
    if (const FFactionData* Data = FactionRegistry.Find(FactionA))
    {
        return Data->Relations.Find(FactionB);
    }
    return nullptr;
}

void UMingRTSFactionManager::UpdateRelationForBoth(EFactionType FactionA, EFactionType FactionB, const FFactionRelationData& Data)
{
    // 這個函數確保雙向關係一致性
    // 實際關係數據存儲在每個勢力的Relations映射中
}

EFactionRelationType UMingRTSFactionManager::DetermineRelationTypeFromValue(float Value) const
{
    if (Value >= 70.0f)
    {
        return EFactionRelationType::Ally;
    }
    else if (Value >= 30.0f)
    {
        return EFactionRelationType::Friendly;
    }
    else if (Value > -30.0f)
    {
        return EFactionRelationType::Neutral;
    }
    else if (Value > -70.0f)
    {
        return EFactionRelationType::Hostile;
    }
    else
    {
        return EFactionRelationType::War;
    }
}
