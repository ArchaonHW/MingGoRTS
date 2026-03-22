#include "MingMercenaryMode.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingMercenaryMode::UMingMercenaryMode()
{
    WorldContext = GetWorld();
    CurrentBalance = 10000.0f; // Starting funds
    TotalEarnings = 0.0f;
}

void UMingMercenaryMode::InitializeMercenaryMode()
{
    InitializeDefaultMercenaries();
    InitializeDefaultContracts();
    SetupMercenarySkills();
    
    UE_LOG(LogTemp, Log, TEXT("Mercenary Mode initialized with %d mercenaries and %d contracts"), 
           Mercenaries.Num(), Contracts.Num());
}

void UMingMercenaryMode::InitializeDefaultMercenaries()
{
    Mercenaries.Empty();
    
    // Chen "Ghost" Wei - Elite Sniper
    FMingMercenary ChenWei;
    ChenWei.MercenaryID = TEXT("MERC_CHEN_WEI");
    ChenWei.Name = TEXT("陳偉");
    ChenWei.Nickname = TEXT("幽靈");
    ChenWei.MercType = EMingMercenaryType::Sniper;
    ChenWei.Rank = EMingMercenaryRank::Elite;
    ChenWei.Age = 32;
    ChenWei.Nationality = TEXT("中國");
    ChenWei.Background = TEXT("前軍隊狙擊手，因戰爭創傷退役，成為職業傭兵。以百發百中的射擊精度聞名。");
    ChenWei.Experience = 850.0f;
    ChenWei.Reputation = 85.0f;
    ChenWei.DailyRate = 500.0f;
    ChenWei.bIsAvailable = true;
    ChenWei.bIsHired = false;
    ChenWei.Portrait = TEXT("Textures/Mercenaries/ChenWei");
    
    // Elite skills for sniper
    ChenWei.Skills.MarksmanshipSkill = 95.0f;
    ChenWei.Skills.StealthSkill = 90.0f;
    ChenWei.Skills.CombatSkill = 75.0f;
    ChenWei.Skills.SurvivalSkill = 80.0f;
    ChenWei.Skills.MedicalSkill = 60.0f;
    ChenWei.Skills.EngineeringSkill = 40.0f;
    ChenWei.Skills.LeadershipSkill = 50.0f;
    ChenWei.Skills.DiplomacySkill = 30.0f;
    
    ChenWei.Specializations.Add(TEXT("遠程狙擊"));
    ChenWei.Specializations.Add(TEXT("潛行滲透"));
    ChenWei.Specializations.Add(TEXT("偵察"));
    
    ChenWei.Equipment.Add(TEXT("精密狙擊步槍"));
    ChenWei.Equipment.Add(TEXT("高倍率瞄準鏡"));
    ChenWei.Equipment.Add(TEXT("消音器"));
    ChenWei.Equipment.Add(TEXT("吉利服"));
    
    Mercenaries.Add(ChenWei.MercenaryID, ChenWei);
    
    // Li "Iron Fist" Zhang - Heavy Infantry
    FMingMercenary LiZhang;
    LiZhang.MercenaryID = TEXT("MERC_LI_ZHANG");
    LiZhang.Name = TEXT("李強");
    LiZhang.Nickname = TEXT("鐵拳");
    LiZhang.MercType = EMingMercenaryType::Heavy;
    LiZhang.Rank = EMingMercenaryRank::Veteran;
    LiZhang.Age = 28;
    LiZhang.Nationality = TEXT("中國");
    LiZhang.Background = TEXT("前特種部隊成員，精通近身格鬥和重型武器。性格直率，作戰勇猛。");
    LiZhang.Experience = 650.0f;
    LiZhang.Reputation = 75.0f;
    LiZhang.DailyRate = 350.0f;
    LiZhang.bIsAvailable = true;
    LiZhang.bIsHired = false;
    LiZhang.Portrait = TEXT("Textures/Mercenaries/LiZhang");
    
    // Veteran heavy skills
    LiZhang.Skills.CombatSkill = 90.0f;
    LiZhang.Skills.StealthSkill = 40.0f;
    LiZhang.Skills.MedicalSkill = 55.0f;
    LiZhang.Skills.EngineeringSkill = 60.0f;
    LiZhang.Skills.LeadershipSkill = 70.0f;
    LiZhang.Skills.MarksmanshipSkill = 75.0f;
    LiZhang.Skills.SurvivalSkill = 80.0f;
    LiZhang.Skills.DiplomacySkill = 35.0f;
    
    LiZhang.Specializations.Add(TEXT("近身格鬥"));
    LiZhang.Specializations.Add(TEXT("重型武器"));
    LiZhang.Specializations.Add(TEXT("防禦作戰"));
    
    LiZhang.Equipment.Add(TEXT("重型機槍"));
    LiZhang.Equipment.Add(TEXT("防彈背心"));
    LiZhang.Equipment.Add(TEXT("戰術頭盔"));
    LiZhang.Equipment.Add(TEXT("破片手榴彈"));
    
    Mercenaries.Add(LiZhang.MercenaryID, LiZhang);
    
    // Wang "Swift" Mei - Scout
    FMingMercenary WangMei;
    WangMei.MercenaryID = TEXT("MERC_WANG_MEI");
    WangMei.Name = TEXT("王梅");
    WangMei.Nickname = TEXT("迅捷");
    WangMei.MercType = EMingMercenaryType::Scout;
    WangMei.Rank = EMingMercenaryRank::Veteran;
    WangMei.Age = 26;
    WangMei.Nationality = TEXT("中國");
    WangMei.Background = TEXT("前情報部隊成員，擅長偵察和滲透。行動迅速，反應敏捷。");
    WangMei.Experience = 550.0f;
    WangMei.Reputation = 70.0f;
    WangMei.DailyRate = 300.0f;
    WangMei.bIsAvailable = true;
    WangMei.bIsHired = false;
    WangMei.Portrait = TEXT("Textures/Mercenaries/WangMei");
    
    // Scout skills
    WangMei.Skills.StealthSkill = 85.0f;
    WangMei.Skills.SurvivalSkill = 90.0f;
    WangMei.Skills.MarksmanshipSkill = 70.0f;
    WangMei.Skills.CombatSkill = 65.0f;
    WangMei.Skills.MedicalSkill = 60.0f;
    WangMei.Skills.EngineeringSkill = 55.0f;
    WangMei.Skills.LeadershipSkill = 45.0f;
    WangMei.Skills.DiplomacySkill = 50.0f;
    
    WangMei.Specializations.Add(TEXT("偵察"));
    WangMei.Specializations.Add(TEXT("滲透"));
    WangMei.Specializations.Add(TEXT("追蹤"));
    
    WangMei.Equipment.Add(TEXT("偵察望遠鏡"));
    WangMei.Equipment.Add(TEXT("夜視儀"));
    WangMei.Equipment.Add(TEXT("通訊設備"));
    WangMei.Equipment.Add(TEXT("輕型步槍"));
    
    Mercenaries.Add(WangMei.MercenaryID, WangMei);
    
    // Zhao "Healer" Lin - Medic
    FMingMercenary ZhaoLin;
    ZhaoLin.MercenaryID = TEXT("MERC_ZHAO_LIN");
    ZhaoLin.Name = TEXT("趙林");
    ZhaoLin.Nickname = TEXT("醫者");
    ZhaoLin.MercType = EMingMercenaryType::Medic;
    ZhaoLin.Rank = EMingMercenaryRank::Elite;
    ZhaoLin.Age = 35;
    ZhaoLin.Nationality = TEXT("中國");
    ZhaoLin.Background = TEXT("前軍醫，戰地經驗豐富。不僅醫術高超，戰鬥能力也很強。");
    ZhaoLin.Experience = 750.0f;
    ZhaoLin.Reputation = 80.0f;
    ZhaoLin.DailyRate = 400.0f;
    ZhaoLin.bIsAvailable = true;
    ZhaoLin.bIsHired = false;
    ZhaoLin.Portrait = TEXT("Textures/Mercenaries/ZhaoLin");
    
    // Elite medic skills
    ZhaoLin.Skills.MedicalSkill = 95.0f;
    ZhaoLin.Skills.CombatSkill = 70.0f;
    ZhaoLin.Skills.StealthSkill = 60.0f;
    ZhaoLin.Skills.EngineeringSkill = 65.0f;
    ZhaoLin.Skills.LeadershipSkill = 75.0f;
    ZhaoLin.Skills.MarksmanshipSkill = 65.0f;
    ZhaoLin.Skills.SurvivalSkill = 80.0f;
    ZhaoLin.Skills.DiplomacySkill = 70.0f;
    
    ZhaoLin.Specializations.Add(TEXT("戰地醫療"));
    ZhaoLin.Specializations.Add(TEXT("急救"));
    ZhaoLin.Specializations.Add(TEXT("疾病防控"));
    
    ZhaoLin.Equipment.Add(TEXT("醫療包"));
    ZhaoLin.Equipment.Add(TEXT("手術器械"));
    ZhaoLin.Equipment.Add(TEXT("藥品"));
    ZhaoLin.Equipment.Add(TEXT("防護裝備"));
    
    Mercenaries.Add(ZhaoLin.MercenaryID, ZhaoLin);
    
    // Zhang "Demolition" Hu - Engineer
    FMingMercenary ZhangHu;
    ZhangHu.MercenaryID = TEXT("MERC_ZHANG_HU");
    ZhangHu.Name = TEXT("張虎");
    ZhangHu.Nickname = TEXT("爆破");
    ZhangHu.MercType = EMingMercenaryType::Engineer;
    ZhangHu.Rank = EMingMercenaryRank::Veteran;
    ZhangHu.Age = 30;
    ZhangHu.Nationality = TEXT("中國");
    ZhangHu.Background = TEXT("前工兵部隊爆破專家，精通各種爆炸物的使用和拆解。");
    ZhangHu.Experience = 600.0f;
    ZhangHu.Reputation = 72.0f;
    ZhangHu.DailyRate = 325.0f;
    ZhangHu.bIsAvailable = true;
    ZhangHu.bIsHired = false;
    ZhangHu.Portrait = TEXT("Textures/Mercenaries/ZhangHu");
    
    // Engineer skills
    ZhangHu.Skills.EngineeringSkill = 90.0f;
    ZhangHu.Skills.CombatSkill = 70.0f;
    ZhangHu.Skills.StealthSkill = 55.0f;
    ZhangHu.Skills.MedicalSkill = 50.0f;
    ZhangHu.Skills.LeadershipSkill = 60.0f;
    ZhangHu.Skills.MarksmanshipSkill = 60.0f;
    ZhangHu.Skills.SurvivalSkill = 75.0f;
    ZhangHu.Skills.DiplomacySkill = 40.0f;
    
    ZhangHu.Specializations.Add(TEXT("爆破"));
    ZhangHu.Specializations.Add(TEXT("拆彈"));
    ZhangHu.Specializations.Add(TEXT("工事建設"));
    
    ZhangHu.Equipment.Add(TEXT("爆破工具包"));
    ZhangHu.Equipment.Add(TEXT("探測器"));
    ZhangHu.Equipment.Add(TEXT("防護服"));
    ZhangHu.Equipment.Add(TEXT("工程工具"));
    
    Mercenaries.Add(ZhangHu.MercenaryID, ZhangHu);
    
    // Liu "Thunder" Biao - Artillery
    FMingMercenary LiuBiao;
    LiuBiao.MercenaryID = TEXT("MERC_LIU_BIAO");
    LiuBiao.Name = TEXT("劉彪");
    LiuBiao.Nickname = TEXT("雷霆");
    LiuBiao.MercType = EMingMercenaryType::Artillery;
    LiuBiao.Rank = EMingMercenaryRank::Elite;
    LiuBiao.Age = 33;
    LiuBiao.Nationality = TEXT("中國");
    LiuBiao.Background = TEXT("前炮兵部隊指揮官，精通各種火炮的射擊和校正。");
    LiuBiao.Experience = 800.0f;
    LiuBiao.Reputation = 82.0f;
    LiuBiao.DailyRate = 450.0f;
    LiuBiao.bIsAvailable = true;
    LiuBiao.bIsHired = false;
    LiuBiao.Portrait = TEXT("Textures/Mercenaries/LiuBiao");
    
    // Elite artillery skills
    LiuBiao.Skills.MarksmanshipSkill = 85.0f; // For artillery targeting
    LiuBiao.Skills.EngineeringSkill = 80.0f;
    LiuBiao.Skills.CombatSkill = 70.0f;
    LiuBiao.Skills.StealthSkill = 35.0f;
    LiuBiao.Skills.MedicalSkill = 45.0f;
    LiuBiao.Skills.LeadershipSkill = 75.0f;
    LiuBiao.Skills.SurvivalSkill = 65.0f;
    LiuBiao.Skills.DiplomacySkill = 40.0f;
    
    LiuBiao.Specializations.Add(TEXT("炮兵射擊"));
    LiuBiao.Specializations.Add(TEXT("火力支援"));
    LiuBiao.Specializations.Add(TEXT("彈道計算"));
    
    LiuBiao.Equipment.Add(TEXT("炮兵觀測儀"));
    LiuBiao.Equipment.Add(TEXT("無線電"));
    LiuBiao.Equipment.Add(TEXT("地圖工具"));
    LiuBiao.Equipment.Add(TEXT("防護裝備"));
    
    Mercenaries.Add(LiuBiao.MercenaryID, LiuBiao);
    
    // Sun "Shadow" Ying - Rookie Infiltrator
    FMingMercenary SunYing;
    SunYing.MercenaryID = TEXT("MERC_SUN_YING");
    SunYing.Name = TEXT("孫穎");
    SunYing.Nickname = TEXT("影子");
    SunYing.MercType = EMingMercenaryType::Scout;
    SunYing.Rank = EMingMercenaryRank::Rookie;
    SunYing.Age = 22;
    SunYing.Nationality = TEXT("中國");
    SunYing.Background = TEXT("年輕的潛行專家，雖然經驗不足但潛力巨大。渴望證明自己。");
    SunYing.Experience = 150.0f;
    SunYing.Reputation = 45.0f;
    SunYing.DailyRate = 150.0f;
    SunYing.bIsAvailable = true;
    SunYing.bIsHired = false;
    SunYing.Portrait = TEXT("Textures/Mercenaries/SunYing");
    
    // Rookie skills with potential
    SunYing.Skills.StealthSkill = 70.0f;
    SunYing.Skills.SurvivalSkill = 65.0f;
    SunYing.Skills.CombatSkill = 45.0f;
    SunYing.Skills.MarksmanshipSkill = 50.0f;
    SunYing.Skills.MedicalSkill = 40.0f;
    SunYing.Skills.EngineeringSkill = 35.0f;
    SunYing.Skills.LeadershipSkill = 30.0f;
    SunYing.Skills.DiplomacySkill = 45.0f;
    
    SunYing.Specializations.Add(TEXT("潛行"));
    SunYing.Specializations.Add(TEXT("偷竊"));
    
    SunYing.Equipment.Add(TEXT("消音手槍"));
    SunYing.Equipment.Add(TEXT("撬鎖工具"));
    SunYing.Equipment.Add(TEXT("夜視儀"));
    
    Mercenaries.Add(SunYing.MercenaryID, SunYing);
}

void UMingMercenaryMode::InitializeDefaultContracts()
{
    Contracts.Empty();
    
    // High-value assassination contract
    FMingContract AssassinationContract;
    AssassinationContract.ContractID = TEXT("CONTRACT_ASSASSINATION_001");
    AssassinationContract.Title = TEXT("高價暗殺");
    AssassinationContract.Description = TEXT("目標是敵對軍閥的高級指揮官，位於重兵把守的別墅中。需要頂尖的滲透和暗殺技能。");
    AssassinationContract.ContractType = EMingContractType::Assassination;
    AssassinationContract.Client = TEXT("匿名客戶");
    AssassinationContract.Target = TEXT("軍閥指揮官王大帥");
    AssassinationContract.Location = TEXT("北平西郊別墅");
    AssassinationContract.Difficulty = 85.0f;
    AssassinationContract.Reward = 15000.0f;
    AssassinationContract.TimeLimit = 7200.0f; // 2 hours
    AssassinationContract.RequiredSkills.Add(TEXT("潛行"));
    AssassinationContract.RequiredSkills.Add(TEXT("射擊"));
    AssassinationContract.RequiredEquipment.Add(TEXT("消音武器"));
    AssassinationContract.Objectives.Add(TEXT("消滅目標"));
    AssassinationContract.Objectives.Add(TEXT("安全撤離"));
    AssassinationContract.Restrictions.Add(TEXT("不得傷害無辜"));
    AssassinationContract.Restrictions.Add(TEXT("必須潛行進入"));
    
    Contracts.Add(AssassinationContract.ContractID, AssassinationContract);
    
    // VIP escort contract
    FMingContract EscortContract;
    EscortContract.ContractID = TEXT("CONTRACT_ESCORT_001");
    EscortContract.Title = TEXT("VIP護送");
    EscortContract.Description = TEXT("護送重要政治人物從上海到南京，途中可能遭遇多方威脅。需要全面的保護能力。");
    EscortContract.ContractType = EMingContractType::Escort;
    EscortContract.Client = TEXT("國民政府");
    EscortContract.Target = TEXT("重要官員");
    EscortContract.Location = TEXT("上海-南京公路");
    EscortContract.Difficulty = 70.0f;
    EscortContract.Reward = 8000.0f;
    EscortContract.TimeLimit = 14400.0f; // 4 hours
    EscortContract.RequiredSkills.Add(TEXT("戰鬥"));
    EscortContract.RequiredSkills.Add(TEXT("醫療"));
    EscortContract.RequiredSkills.Add(TEXT("駕駛"));
    EscortContract.Objectives.Add(TEXT("保護VIP安全"));
    EscortContract.Objectives.Add(TEXT("安全抵達目的地"));
    EscortContract.Restrictions.Add(TEXT("VIP不能受傷"));
    EscortContract.Restrictions.Add(TEXT("避免公眾注意"));
    
    Contracts.Add(EscortContract.ContractID, EscortContract);
    
    // Sabotage contract
    FMingContract SabotageContract;
    SabotageContract.ContractID = TEXT("CONTRACT_SABOTAGE_001");
    SabotageContract.Title = TEXT("破壞行動");
    SabotageContract.Description = TEXT("破壞敵方的軍火庫，削弱其作戰能力。需要精確的爆破技能和時機把握。");
    SabotageContract.ContractType = EMingContractType::Sabotage;
    SabotageContract.Client = TEXT("革命軍");
    SabotageContract.Target = TEXT("軍火庫");
    SabotageContract.Location = TEXT("武漢軍區");
    SabotageContract.Difficulty = 75.0f;
    SabotageContract.Reward = 10000.0f;
    SabotageContract.TimeLimit = 10800.0f; // 3 hours
    SabotageContract.RequiredSkills.Add(TEXT("爆破"));
    SabotageContract.RequiredSkills.Add(TEXT("工程"));
    SabotageContract.RequiredEquipment.Add(TEXT("爆破工具"));
    SabotageContract.Objectives.Add(TEXT("安裝炸藥"));
    SabotageContract.Objectives.Add(TEXT("安全引爆"));
    SabotageContract.Objectives.Add(TEXT("撤離現場"));
    SabotageContract.Restrictions.Add(TEXT("避免提前引爆"));
    SabotageContract.Restrictions.Add(TEXT("最小化附帶損害"));
    
    Contracts.Add(SabotageContract.ContractID, SabotageContract);
    
    // Rescue contract
    FMingContract RescueContract;
    RescueContract.ContractID = TEXT("CONTRACT_RESCUE_001");
    RescueContract.Title = TEXT("營救行動");
    RescueContract.Description = TEXT("營救被俘的革命同志，從敵方監獄中救出重要情報人員。");
    RescueContract.ContractType = EMingContractType::Rescue;
    RescueContract.Client = TEXT("革命黨");
    RescueContract.Target = TEXT("被捕同志");
    RescueContract.Location = TEXT("秘密監獄");
    RescueContract.Difficulty = 80.0f;
    RescueContract.Reward = 12000.0f;
    RescueContract.TimeLimit = 9000.0f; // 2.5 hours
    RescueContract.RequiredSkills.Add(TEXT("潛行"));
    RescueContract.RequiredSkills.Add(TEXT("戰鬥"));
    RescueContract.RequiredSkills.Add(TEXT("醫療"));
    RescueContract.Objectives.Add(TEXT("潛入監獄"));
    RescueContract.Objectives.Add(TEXT("解救人質"));
    RescueContract.Objectives.Add(TEXT("安全撤離"));
    RescueContract.Restrictions.Add(TEXT("人質必須存活"));
    RescueContract.Restrictions.Add(TEXT("避免警報"));
    
    Contracts.Add(RescueContract.ContractID, RescueContract);
    
    // Defense contract
    FMingContract DefenseContract;
    DefenseContract.ContractID = TEXT("CONTRACT_DEFENSE_001");
    DefenseContract.Title = TEXT("防禦作戰");
    DefenseContract.Description = TEXT("保護革命根據地免受敵方攻擊，堅守到援軍到達。");
    DefenseContract.ContractType = EMingContractType::Defense;
    DefenseContract.Client = TEXT("革命軍指揮部");
    DefenseContract.Target = TEXT("革命根據地");
    DefenseContract.Location = TEXT("廣州革命基地");
    DefenseContract.Difficulty = 65.0f;
    DefenseContract.Reward = 6000.0f;
    DefenseContract.TimeLimit = 21600.0f; // 6 hours
    DefenseContract.RequiredSkills.Add(TEXT("防禦"));
    DefenseContract.RequiredSkills.Add(TEXT("戰鬥"));
    DefenseContract.RequiredSkills.Add(TEXT("醫療"));
    DefenseContract.Objectives.Add(TEXT("守住基地"));
    DefenseContract.Objectives.Add(TEXT("保護指揮部"));
    DefenseContract.Objectives.Add(TEXT("堅持到援軍到達"));
    DefenseContract.Restrictions.Add(TEXT("指揮部不能被摧毀"));
    DefenseContract.Restrictions.Add(TEXT("減少友軍傷亡"));
    
    Contracts.Add(DefenseContract.ContractID, DefenseContract);
    
    // Reconnaissance contract
    FMingContract ReconContract;
    ReconContract.ContractID = TEXT("CONTRACT_RECON_001");
    ReconContract.Title = TEXT("偵察任務");
    ReconContract.Description = TEXT("深入敵後，收集敵軍部署情報，為即將到來的攻擊做準備。");
    ReconContract.ContractType = EMingContractType::Reconnaissance;
    ReconContract.Client = TEXT("北伐軍司令部");
    ReconContract.Target = TEXT("敵軍情報");
    ReconContract.Location = TEXT("敵占區");
    ReconContract.Difficulty = 60.0f;
    ReconContract.Reward = 5000.0f;
    ReconContract.TimeLimit = 7200.0f; // 2 hours
    ReconContract.RequiredSkills.Add(TEXT("偵察"));
    ReconContract.RequiredSkills.Add(TEXT("潛行"));
    ReconContract.RequiredEquipment.Add(TEXT("偵察設備"));
    ReconContract.Objectives.Add(TEXT("收集情報"));
    ReconContract.Objectives.Add(TEXT("拍攲照片"));
    ReconContract.Objectives.Add(TEXT("安全返回"));
    ReconContract.Restrictions.Add(TEXT("不能被發現"));
    ReconContract.Restrictions.Add(TEXT("情報必須完整"));
    
    Contracts.Add(ReconContract.ContractID, ReconContract);
    
    // Extraction contract
    FMingContract ExtractionContract;
    ExtractionContract.ContractID = TEXT("CONTRACT_EXTRACTION_001");
    ExtractionContract.Title = TEXT("撤離行動");
    ExtractionContract.Description = TEXT("在敵方控制區域撤離重要人員和設備，需要精確的計劃和執行。");
    ExtractionContract.ContractType = EMingContractType::Extraction;
    ExtractionContract.Client = TEXT("國際聯盟");
    ExtractionContract.Target = TEXT("重要人員和設備");
    ExtractionContract.Location = TEXT("敵控城市");
    ExtractionContract.Difficulty = 78.0f;
    ExtractionContract.Reward = 11000.0f;
    ExtractionContract.TimeLimit = 12600.0f; // 3.5 hours
    ExtractionContract.RequiredSkills.Add(TEXT("潛行"));
    ExtractionContract.RequiredSkills.Add(TEXT("戰鬥"));
    ExtractionContract.RequiredSkills.Add(TEXT("工程"));
    ExtractionContract.Objectives.Add(TEXT("接觸目標"));
    ExtractionContract.Objectives.Add(TEXT("護送撤離"));
    ExtractionContract.Objectives.Add(TEXT("安全離開");
    ExtractionContract.Restrictions.Add(TEXT("目標必須安全"));
    ExtractionContract.Restrictions.Add(TEXT("設備不能損壞"));
    
    Contracts.Add(ExtractionContract.ContractID, ExtractionContract);
}

void UMingMercenaryMode::SetupMercenarySkills()
{
    // Calculate initial costs based on skills and experience
    for (auto& Pair : Mercenaries)
    {
        FMingMercenary& Merc = Pair.Value;
        CalculateMercenaryCost(Merc);
    }
}

void UMingMercenaryMode::HireMercenary(const FString& MercenaryID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (!Merc)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mercenary not found: %s"), *MercenaryID);
        return;
    }
    
    if (Merc->bIsHired)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mercenary already hired: %s"), *Merc->Name);
        return;
    }
    
    if (!CanAffordMercenary(MercenaryID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot afford mercenary: %s"), *Merc->Name);
        return;
    }
    
    // Pay hiring cost
    float HiringCost = Merc->DailyRate * 7; // One week advance
    CurrentBalance -= HiringCost;
    
    Merc->bIsHired = true;
    Merc->bIsAvailable = false;
    
    OnMercenaryHired.Broadcast(*Merc, HiringCost);
    
    UE_LOG(LogTemp, Log, TEXT("Hired mercenary: %s for %.2f"), *Merc->Name, HiringCost);
}

void UMingMercenaryMode::FireMercenary(const FString& MercenaryID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (!Merc || !Merc->bIsHired)
    {
        return;
    }
    
    // Calculate severance pay
    float SeverancePay = Merc->DailyRate * 3; // 3 days severance
    
    Merc->bIsHired = false;
    Merc->bIsAvailable = true;
    
    // Remove from any active contracts
    for (auto& Pair : MercenaryContractAssignments)
    {
        if (Pair.Value == MercenaryID)
        {
            RemoveMercenaryFromContract(MercenaryID);
            break;
        }
    }
    
    OnMercenaryFired.Broadcast(MercenaryID, SeverancePay);
    
    UE_LOG(LogTemp, Log, TEXT("Fired mercenary: %s"), *Merc->Name);
}

void UMingMercenaryMode::TrainMercenary(const FString& MercenaryID, const FString& SkillType, float Duration)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (!Merc)
    {
        return;
    }
    
    // Improve skill based on training duration
    float Improvement = Duration * 0.1f; // 0.1 skill points per second
    
    if (SkillType == TEXT("Combat"))
    {
        Merc->Skills.CombatSkill = FMath::Min(Merc->Skills.CombatSkill + Improvement, 100.0f);
    }
    else if (SkillType == TEXT("Stealth"))
    {
        Merc->Skills.StealthSkill = FMath::Min(Merc->Skills.StealthSkill + Improvement, 100.0f);
    }
    else if (SkillType == TEXT("Medical"))
    {
        Merc->Skills.MedicalSkill = FMath::Min(Merc->Skills.MedicalSkill + Improvement, 100.0f);
    }
    else if (SkillType == TEXT("Engineering"))
    {
        Merc->Skills.EngineeringSkill = FMath::Min(Merc->Skills.EngineeringSkill + Improvement, 100.0f);
    }
    else if (SkillType == TEXT("Leadership"))
    {
        Merc->Skills.LeadershipSkill = FMath::Min(Merc->Skills.LeadershipSkill + Improvement, 100.0f);
    }
    else if (SkillType == TEXT("Marksmanship"))
    {
        Merc->Skills.MarksmanshipSkill = FMath::Min(Merc->Skills.MarksmanshipSkill + Improvement, 100.0f);
    }
    else if (SkillType == TEXT("Survival"))
    {
        Merc->Skills.SurvivalSkill = FMath::Min(Merc->Skills.SurvivalSkill + Improvement, 100.0f);
    }
    else if (SkillType == TEXT("Diplomacy"))
    {
        Merc->Skills.DiplomacySkill = FMath::Min(Merc->Skills.DiplomacySkill + Improvement, 100.0f);
    }
    
    // Update experience and potentially rank
    Merc->Experience += Duration * 0.5f;
    UpdateMercenaryStats(MercenaryID);
    
    UE_LOG(LogTemp, Log, TEXT("Trained mercenary %s in %s"), *Merc->Name, *SkillType);
}

void UMingMercenaryMode::UpgradeMercenary(const FString& MercenaryID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (!Merc)
    {
        return;
    }
    
    // Upgrade cost based on current rank
    float UpgradeCost = Merc->DailyRate * 30; // 30 days cost
    
    if (CurrentBalance < UpgradeCost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot afford upgrade for %s"), *Merc->Name);
        return;
    }
    
    CurrentBalance -= UpgradeCost;
    
    // Improve all skills slightly
    Merc->Skills.CombatSkill = FMath::Min(Merc->Skills.CombatSkill + 5.0f, 100.0f);
    Merc->Skills.StealthSkill = FMath::Min(Merc->Skills.StealthSkill + 5.0f, 100.0f);
    Merc->Skills.MedicalSkill = FMath::Min(Merc->Skills.MedicalSkill + 5.0f, 100.0f);
    Merc->Skills.EngineeringSkill = FMath::Min(Merc->Skills.EngineeringSkill + 5.0f, 100.0f);
    Merc->Skills.LeadershipSkill = FMath::Min(Merc->Skills.LeadershipSkill + 5.0f, 100.0f);
    Merc->Skills.MarksmanshipSkill = FMath::Min(Merc->Skills.MarksmanshipSkill + 5.0f, 100.0f);
    Merc->Skills.SurvivalSkill = FMath::Min(Merc->Skills.SurvivalSkill + 5.0f, 100.0f);
    Merc->Skills.DiplomacySkill = FMath::Min(Merc->Skills.DiplomacySkill + 5.0f, 100.0f);
    
    Merc->Experience += 100.0f;
    Merc->Reputation += 5.0f;
    
    // Increase daily rate
    Merc->DailyRate *= 1.2f;
    
    UpdateMercenaryStats(MercenaryID);
    
    UE_LOG(LogTemp, Log, TEXT("Upgraded mercenary: %s"), *Merc->Name);
}

TArray<FMingMercenary> UMingMercenaryMode::GetAvailableMercenaries() const
{
    TArray<FMingMercenary> Result;
    for (const auto& Pair : Mercenaries)
    {
        if (Pair.Value.bIsAvailable && !Pair.Value.bIsHired)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingMercenary> UMingMercenaryMode::GetHiredMercenaries() const
{
    TArray<FMingMercenary> Result;
    for (const auto& Pair : Mercenaries)
    {
        if (Pair.Value.bIsHired)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FMingMercenary UMingMercenaryMode::GetMercenary(const FString& MercenaryID) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    return Merc ? *Merc : FMingMercenary();
}

float UMingMercenaryMode::GetMercenaryCost(const FString& MercenaryID) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    return Merc ? Merc->DailyRate : 0.0f;
}

void UMingMercenaryMode::AcceptContract(const FString& ContractID)
{
    FMingContract* Contract = FindContract(ContractID);
    if (!Contract)
    {
        UE_LOG(LogTemp, Warning, TEXT("Contract not found: %s"), *ContractID);
        return;
    }
    
    if (!CanAcceptContract(ContractID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot accept contract: %s"), *Contract->Title);
        return;
    }
    
    Contract->bIsActive = true;
    Contract->StartTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Assign optimal mercenaries
    AssignOptimalMercenaries(ContractID);
    
    OnContractAccepted.Broadcast(*Contract, TEXT("Auto-assigned"));
    
    UE_LOG(LogTemp, Log, TEXT("Accepted contract: %s"), *Contract->Title);
}

void UMingMercenaryMode::CompleteContract(const FString& ContractID)
{
    FMingContract* Contract = FindContract(ContractID);
    if (!Contract || !Contract->bIsActive)
    {
        return;
    }
    
    Contract->bIsActive = false;
    Contract->bIsCompleted = true;
    Contract->CompletionTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    
    // Collect reward
    CollectContractReward(ContractID);
    
    // Update mercenary experience and reputation
    TArray<FString> AssignedMercs = GetAssignedMercenaries(ContractID);
    for (const FString& MercID : AssignedMercs)
    {
        FMingMercenary* Merc = FindMercenary(MercID);
        if (Merc)
        {
            Merc->Experience += Contract->Difficulty * 2.0f;
            Merc->Reputation += Contract->Difficulty * 0.5f;
            UpdateMercenaryStats(MercID);
        }
    }
    
    OnContractCompleted.Broadcast(ContractID, Contract->Reward);
    
    UE_LOG(LogTemp, Log, TEXT("Completed contract: %s - Reward: %.2f"), *Contract->Title, Contract->Reward);
}

void UMingMercenaryMode::FailContract(const FString& ContractID)
{
    FMingContract* Contract = FindContract(ContractID);
    if (!Contract || !Contract->bIsActive)
    {
        return;
    }
    
    Contract->bIsActive = false;
    Contract->bIsCompleted = false;
    
    // Penalty for failure
    CurrentBalance -= Contract->Reward * 0.2f; // 20% penalty
    
    // Update mercenary reputation (negative impact)
    TArray<FString> AssignedMercs = GetAssignedMercenaries(ContractID);
    for (const FString& MercID : AssignedMercs)
    {
        FMingMercenary* Merc = FindMercenary(MercID);
        if (Merc)
        {
            Merc->Reputation -= Contract->Difficulty * 0.3f;
            Merc->Reputation = FMath::Max(Merc->Reputation, 0.0f);
            UpdateMercenaryStats(MercID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Failed contract: %s"), *Contract->Title);
}

void UMingMercenaryMode::AbandonContract(const FString& ContractID)
{
    FMingContract* Contract = FindContract(ContractID);
    if (!Contract || !Contract->bIsActive)
    {
        return;
    }
    
    Contract->bIsActive = false;
    
    // Remove all assignments
    TArray<FString> AssignedMercs = GetAssignedMercenaries(ContractID);
    for (const FString& MercID : AssignedMercs)
    {
        RemoveMercenaryFromContract(MercID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Abandoned contract: %s"), *Contract->Title);
}

TArray<FMingContract> UMingMercenaryMode::GetAvailableContracts() const
{
    TArray<FMingContract> Result;
    for (const auto& Pair : Contracts)
    {
        if (!Pair.Value.bIsActive && !Pair.Value.bIsCompleted)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingContract> UMingMercenaryMode::GetActiveContracts() const
{
    TArray<FMingContract> Result;
    for (const auto& Pair : Contracts)
    {
        if (Pair.Value.bIsActive)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingContract> UMingMercenaryMode::GetCompletedContracts() const
{
    TArray<FMingContract> Result;
    for (const auto& Pair : Contracts)
    {
        if (Pair.Value.bIsCompleted)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FMingContract UMingMercenaryMode::GetContract(const FString& ContractID) const
{
    const FMingContract* Contract = Contracts.Find(ContractID);
    return Contract ? *Contract : FMingContract();
}

bool UMingMercenaryMode::CanAcceptContract(const FString& ContractID) const
{
    const FMingContract* Contract = Contracts.Find(ContractID);
    if (!Contract || Contract->bIsActive || Contract->bIsCompleted)
    {
        return false;
    }
    
    // Check if we have mercenaries with required skills
    TArray<FMingMercenary> AvailableMercs = GetHiredMercenaries();
    for (const FString& RequiredSkill : Contract->RequiredSkills)
    {
        bool HasSkill = false;
        for (const FMingMercenary& Merc : AvailableMercs)
        {
            if (HasRequiredSkills(Merc.MercenaryID, TArray<FString>{RequiredSkill}))
            {
                HasSkill = true;
                break;
            }
        }
        if (!HasSkill)
        {
            return false;
        }
    }
    
    return true;
}

void UMingMercenaryMode::ImproveSkill(const FString& MercenaryID, const FString& SkillType, float Amount)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (!Merc)
    {
        return;
    }
    
    if (SkillType == TEXT("Combat"))
    {
        Merc->Skills.CombatSkill = FMath::Min(Merc->Skills.CombatSkill + Amount, 100.0f);
    }
    else if (SkillType == TEXT("Stealth"))
    {
        Merc->Skills.StealthSkill = FMath::Min(Merc->Skills.StealthSkill + Amount, 100.0f);
    }
    else if (SkillType == TEXT("Medical"))
    {
        Merc->Skills.MedicalSkill = FMath::Min(Merc->Skills.MedicalSkill + Amount, 100.0f);
    }
    else if (SkillType == TEXT("Engineering"))
    {
        Merc->Skills.EngineeringSkill = FMath::Min(Merc->Skills.EngineeringSkill + Amount, 100.0f);
    }
    else if (SkillType == TEXT("Leadership"))
    {
        Merc->Skills.LeadershipSkill = FMath::Min(Merc->Skills.LeadershipSkill + Amount, 100.0f);
    }
    else if (SkillType == TEXT("Marksmanship"))
    {
        Merc->Skills.MarksmanshipSkill = FMath::Min(Merc->Skills.MarksmanshipSkill + Amount, 100.0f);
    }
    else if (SkillType == TEXT("Survival"))
    {
        Merc->Skills.SurvivalSkill = FMath::Min(Merc->Skills.SurvivalSkill + Amount, 100.0f);
    }
    else if (SkillType == TEXT("Diplomacy"))
    {
        Merc->Skills.DiplomacySkill = FMath::Min(Merc->Skills.DiplomacySkill + Amount, 100.0f);
    }
    
    UpdateMercenaryStats(MercenaryID);
}

void UMingMercenaryMode::AddSpecialization(const FString& MercenaryID, const FString& Specialization)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (Merc && !Merc->Specializations.Contains(Specialization))
    {
        Merc->Specializations.Add(Specialization);
        UE_LOG(LogTemp, Log, TEXT("Added specialization %s to mercenary %s"), *Specialization, *Merc->Name);
    }
}

float UMingMercenaryMode::GetSkillLevel(const FString& MercenaryID, const FString& SkillType) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    if (!Merc)
    {
        return 0.0f;
    }
    
    if (SkillType == TEXT("Combat"))
    {
        return Merc->Skills.CombatSkill;
    }
    else if (SkillType == TEXT("Stealth"))
    {
        return Merc->Skills.StealthSkill;
    }
    else if (SkillType == TEXT("Medical"))
    {
        return Merc->Skills.MedicalSkill;
    }
    else if (SkillType == TEXT("Engineering"))
    {
        return Merc->Skills.EngineeringSkill;
    }
    else if (SkillType == TEXT("Leadership"))
    {
        return Merc->Skills.LeadershipSkill;
    }
    else if (SkillType == TEXT("Marksmanship"))
    {
        return Merc->Skills.MarksmanshipSkill;
    }
    else if (SkillType == TEXT("Survival"))
    {
        return Merc->Skills.SurvivalSkill;
    }
    else if (SkillType == TEXT("Diplomacy"))
    {
        return Merc->Skills.DiplomacySkill;
    }
    
    return 0.0f;
}

TArray<FString> UMingMercenaryMode::GetMercenarySpecializations(const FString& MercenaryID) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    return Merc ? Merc->Specializations : TArray<FString>();
}

void UMingMercenaryMode::AddReputation(const FString& MercenaryID, float Amount)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (Merc)
    {
        Merc->Reputation = FMath::Clamp(Merc->Reputation + Amount, 0.0f, 100.0f);
        UpdateMercenaryStats(MercenaryID);
    }
}

void UMingMercenaryMode::RemoveReputation(const FString& MercenaryID, float Amount)
{
    AddReputation(MercenaryID, -Amount);
}

float UMingMercenaryMode::GetReputation(const FString& MercenaryID) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    return Merc ? Merc->Reputation : 0.0f;
}

EMingMercenaryRank UMingMercenaryMode::GetRank(const FString& MercenaryID) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    return Merc ? Merc->Rank : EMingMercenaryRank::Rookie;
}

void UMingMercenaryMode::PromoteMercenary(const FString& MercenaryID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (!Merc)
    {
        return;
    }
    
    EMingMercenaryRank OldRank = Merc->Rank;
    
    if (Merc->Rank == EMingMercenaryRank::Rookie && Merc->Reputation >= 60.0f)
    {
        Merc->Rank = EMingMercenaryRank::Veteran;
        Merc->DailyRate *= 1.5f;
    }
    else if (Merc->Rank == EMingMercenaryRank::Veteran && Merc->Reputation >= 80.0f)
    {
        Merc->Rank = EMingMercenaryRank::Elite;
        Merc->DailyRate *= 1.5f;
    }
    else if (Merc->Rank == EMingMercenaryRank::Elite && Merc->Reputation >= 95.0f)
    {
        Merc->Rank = EMingMercenaryRank::Legend;
        Merc->DailyRate *= 2.0f;
    }
    
    if (OldRank != Merc->Rank)
    {
        OnMercenaryPromoted.Broadcast(MercenaryID, Merc->Rank);
        UE_LOG(LogTemp, Log, TEXT("Promoted mercenary %s to %s"), *Merc->Name, *GetRankName(Merc->Rank));
    }
}

void UMingMercenaryMode::EquipItem(const FString& MercenaryID, const FString& ItemID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (Merc && !Merc->Equipment.Contains(ItemID))
    {
        Merc->Equipment.Add(ItemID);
        UE_LOG(LogTemp, Log, TEXT("Equipped item %s to mercenary %s"), *ItemID, *Merc->Name);
    }
}

void UMingMercenaryMode::UnequipItem(const FString& MercenaryID, const FString& ItemID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (Merc)
    {
        Merc->Equipment.Remove(ItemID);
        UE_LOG(LogTemp, Log, TEXT("Unequipped item %s from mercenary %s"), *ItemID, *Merc->Name);
    }
}

TArray<FString> UMingMercenaryMode::GetMercenaryEquipment(const FString& MercenaryID) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    return Merc ? Merc->Equipment : TArray<FString>();
}

void UMingMercenaryMode::PurchaseEquipment(const FString& ItemID)
{
    // This would implement equipment purchasing
    UE_LOG(LogTemp, Log, TEXT("Purchased equipment: %s"), *ItemID);
}

void UMingMercenaryMode::PayMercenaries()
{
    TArray<FMingMercenary> HiredMercs = GetHiredMercenaries();
    float TotalCost = 0.0f;
    
    for (const FMingMercenary& Merc : HiredMercs)
    {
        TotalCost += Merc.DailyRate;
    }
    
    if (CurrentBalance >= TotalCost)
    {
        CurrentBalance -= TotalCost;
        UE_LOG(LogTemp, Log, TEXT("Paid mercenaries: %.2f total"), TotalCost);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient funds to pay mercenaries"));
    }
}

void UMingMercenaryMode::CollectContractReward(const FString& ContractID)
{
    const FMingContract* Contract = Contracts.Find(ContractID);
    if (Contract && Contract->bIsCompleted)
    {
        CurrentBalance += Contract->Reward;
        TotalEarnings += Contract->Reward;
        UE_LOG(LogTemp, Log, TEXT("Collected contract reward: %.2f"), Contract->Reward);
    }
}

float UMingMercenaryMode::GetDailyCosts() const
{
    float TotalCost = 0.0f;
    TArray<FMingMercenary> HiredMercs = GetHiredMercenaries();
    
    for (const FMingMercenary& Merc : HiredMercs)
    {
        TotalCost += Merc.DailyRate;
    }
    
    return TotalCost;
}

float UMingMercenaryMode::GetTotalEarnings() const
{
    return TotalEarnings;
}

float UMingMercenaryMode::GetCurrentBalance() const
{
    return CurrentBalance;
}

void UMingMercenaryMode::AssignMercenaryToContract(const FString& MercenaryID, const FString& ContractID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    FMingContract* Contract = FindContract(ContractID);
    
    if (Merc && Contract && Merc->bIsHired && Contract->bIsActive)
    {
        MercenaryContractAssignments.Add(MercenaryID, ContractID);
        UE_LOG(LogTemp, Log, TEXT("Assigned mercenary %s to contract %s"), *Merc->Name, *Contract->Title);
    }
}

void UMingMercenaryMode::RemoveMercenaryFromContract(const FString& MercenaryID)
{
    if (MercenaryContractAssignments.Contains(MercenaryID))
    {
        FString ContractID = MercenaryContractAssignments[MercenaryID];
        MercenaryContractAssignments.Remove(MercenaryID);
        UE_LOG(LogTemp, Log, TEXT("Removed mercenary %s from contract %s"), *MercenaryID, *ContractID);
    }
}

TArray<FString> UMingMercenaryMode::GetAssignedMercenaries(const FString& ContractID) const
{
    TArray<FString> Result;
    for (const auto& Pair : MercenaryContractAssignments)
    {
        if (Pair.Value == ContractID)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

FString UMingMercenaryMode::GetCurrentContract(const FString& MercenaryID) const
{
    const FString* ContractID = MercenaryContractAssignments.Find(MercenaryID);
    return ContractID ? *ContractID : FString();
}

FString UMingMercenaryMode::GetMercenaryTypeName(EMingMercenaryType MercType)
{
    switch (MercType)
    {
    case EMingMercenaryType::Infantry: return TEXT("步兵");
    case EMingMercenaryType::Cavalry: return TEXT("騎兵");
    case EMingMercenaryType::Artillery: return TEXT("炮兵");
    case EMingMercenaryType::Sniper: return TEXT("狙擊手");
    case EMingMercenaryType::Medic: return TEXT("醫療兵");
    case EMingMercenaryType::Engineer: return TEXT("工程兵");
    case EMingMercenaryType::Scout: return TEXT("斥候");
    case EMingMercenaryType::Heavy: return TEXT("重裝兵");
    default: return TEXT("未知");
    }
}

FString UMingMercenaryMode::GetRankName(EMingMercenaryRank Rank)
{
    switch (Rank)
    {
    case EMingMercenaryRank::Rookie: return TEXT("新兵");
    case EMingMercenaryRank::Veteran: return TEXT("老兵");
    case EMingMercenaryRank::Elite: return TEXT("精英");
    case EMingMercenaryRank::Legend: return TEXT("傳奇");
    default: return TEXT("未知");
    }
}

FString UMingMercenaryMode::GetContractTypeName(EMingContractType ContractType)
{
    switch (ContractType)
    {
    case EMingContractType::Assassination: return TEXT("暗殺");
    case EMingContractType::Escort: return TEXT("護送");
    case EMingContractType::Sabotage: return TEXT("破壞");
    case EMingContractType::Rescue: return TEXT("營救");
    case EMingContractType::Capture: return TEXT("捕獲");
    case EMingContractType::Defense: return TEXT("防禦");
    case EMingContractType::Reconnaissance: return TEXT("偵察");
    case EMingContractType::Extraction: return TEXT("撤離");
    default: return TEXT("未知");
    }
}

FString UMingMercenaryMode::SaveMercenaryData() const
{
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"current_balance\": %.2f,\n"), CurrentBalance);
    Result += FString::Printf(TEXT("  \"total_earnings\": %.2f,\n"), TotalEarnings);
    Result += FString::Printf(TEXT("  \"mercenary_count\": %d,\n"), Mercenaries.Num());
    Result += FString::Printf(TEXT("  \"contract_count\": %d,\n"), Contracts.Num());
    Result += FString::Printf(TEXT("  \"hired_mercenaries\": %d,\n"), GetHiredMercenaries().Num());
    Result += FString::Printf(TEXT("  \"active_contracts\": %d\n"), GetActiveContracts().Num());
    Result += TEXT("}\n");
    
    return Result;
}

void UMingMercenaryMode::LoadMercenaryData(const FString& JsonString)
{
    // Parse JSON and restore mercenary data
    UE_LOG(LogTemp, Log, TEXT("Loading mercenary data"));
}

void UMingMercenaryMode::UpdateMercenaryStats(const FString& MercenaryID)
{
    FMingMercenary* Merc = FindMercenary(MercenaryID);
    if (!Merc)
    {
        return;
    }
    
    // Update reputation effects and check for promotion
    UpdateReputationEffects(MercenaryID);
    CheckRankPromotion(MercenaryID);
    
    // Recalculate daily rate based on skills and rank
    CalculateMercenaryCost(*Merc);
}

void UMingMercenaryMode::CalculateMercenaryCost(FMingMercenary& Merc)
{
    // Base cost calculation
    float BaseCost = 100.0f;
    
    // Skill bonuses
    float SkillBonus = (Merc.Skills.CombatSkill + Merc.Skills.StealthSkill + Merc.Skills.MedicalSkill + 
                       Merc.Skills.EngineeringSkill + Merc.Skills.LeadershipSkill + Merc.Skills.MarksmanshipSkill + 
                       Merc.Skills.SurvivalSkill + Merc.Skills.DiplomacySkill) / 800.0f; // Average of all skills
    
    // Rank multiplier
    float RankMultiplier = 1.0f;
    switch (Merc.Rank)
    {
    case EMingMercenaryRank::Veteran: RankMultiplier = 1.5f; break;
    case EMingMercenaryRank::Elite: RankMultiplier = 2.0f; break;
    case EMingMercenaryRank::Legend: RankMultiplier = 3.0f; break;
    default: RankMultiplier = 1.0f; break;
    }
    
    // Experience bonus
    float ExperienceBonus = 1.0f + (Merc.Experience / 1000.0f);
    
    // Reputation bonus
    float ReputationBonus = 1.0f + (Merc.Reputation / 200.0f);
    
    Merc.DailyRate = BaseCost * SkillBonus * RankMultiplier * ExperienceBonus * ReputationBonus;
}

bool UMingMercenaryMode::CanAffordMercenary(const FString& MercenaryID) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    if (!Merc)
    {
        return false;
    }
    
    float HiringCost = Merc->DailyRate * 7; // One week advance
    return CurrentBalance >= HiringCost;
}

void UMingMercenaryMode::GenerateNewContracts()
{
    // This would generate new contracts based on current game state
    UE_LOG(LogTemp, Log, TEXT("Generating new contracts"));
}

void UMingMercenaryMode::UpdateContractDifficulty()
{
    // This would update contract difficulty based on player progress
    UE_LOG(LogTemp, Log, TEXT("Updating contract difficulties"));
}

void UMingMercenaryMode::EvaluateContractPerformance(const FString& ContractID)
{
    // This would evaluate how well a contract was performed
    UE_LOG(LogTemp, Log, TEXT("Evaluating contract performance: %s"), *ContractID);
}

void UMingMercenaryMode::UpdateReputationEffects(const FString& MercenaryID)
{
    // Apply reputation-based effects
    UE_LOG(LogTemp, Verbose, TEXT("Updating reputation effects for mercenary: %s"), *MercenaryID);
}

void UMingMercenaryMode::CheckRankPromotion(const FString& MercenaryID)
{
    PromoteMercenary(MercenaryID);
}

void UMingMercenaryMode::ProcessDailyExpenses()
{
    PayMercenaries();
}

void UMingMercenaryMode::CalculateContractReward(FMingContract& Contract)
{
    // Base reward calculation
    float BaseReward = 1000.0f;
    
    // Difficulty multiplier
    float DifficultyMultiplier = Contract.Difficulty / 50.0f;
    
    // Time bonus (faster completion = higher reward)
    float TimeBonus = 1.0f;
    
    Contract.Reward = BaseReward * DifficultyMultiplier * TimeBonus;
}

void UMingMercenaryMode::UpdateFinancialRecords()
{
    // Update financial statistics
    UE_LOG(LogTemp, Verbose, TEXT("Updating financial records"));
}

void UMingMercenaryMode::AssignOptimalMercenaries(const FString& ContractID)
{
    FMingContract* Contract = FindContract(ContractID);
    if (!Contract)
    {
        return;
    }
    
    // Find best mercenaries for this contract
    TArray<FMingMercenary> AvailableMercs = GetHiredMercenaries();
    TArray<FMingMercenary> BestMercs;
    
    for (const FString& RequiredSkill : Contract->RequiredSkills)
    {
        FMingMercenary BestMerc;
        float BestSkillLevel = 0.0f;
        
        for (const FMingMercenary& Merc : AvailableMercs)
        {
            if (HasRequiredSkills(Merc.MercenaryID, TArray<FString>{RequiredSkill}))
            {
                float SkillLevel = GetSkillLevel(Merc.MercenaryID, RequiredSkill);
                if (SkillLevel > BestSkillLevel)
                {
                    BestSkillLevel = SkillLevel;
                    BestMerc = Merc;
                }
            }
        }
        
        if (BestMerc.MercenaryID.IsEmpty())
        {
            continue;
        }
        
        // Check if already assigned
        bool bAlreadyAssigned = false;
        for (const FMingMercenary& Assigned : BestMercs)
        {
            if (Assigned.MercenaryID == BestMerc.MercenaryID)
            {
                bAlreadyAssigned = true;
                break;
            }
        }
        
        if (!bAlreadyAssigned)
        {
            BestMercs.Add(BestMerc);
            AssignMercenaryToContract(BestMerc.MercenaryID, ContractID);
        }
    }
}

void UMingMercenaryMode::EvaluateMercenaryPerformance(const FString& MercenaryID, const FString& ContractID)
{
    // This would evaluate how well a mercenary performed on a contract
    UE_LOG(LogTemp, Log, TEXT("Evaluating performance: %s on %s"), *MercenaryID, *ContractID);
}

FMingMercenary* UMingMercenaryMode::FindMercenary(const FString& MercenaryID)
{
    return Mercenaries.Find(MercenaryID);
}

FMingContract* UMingMercenaryMode::FindContract(const FString& ContractID)
{
    return Contracts.Find(ContractID);
}

bool UMingMercenaryMode::HasRequiredSkills(const FString& MercenaryID, const TArray<FString>& RequiredSkills) const
{
    const FMingMercenary* Merc = Mercenaries.Find(MercenaryID);
    if (!Merc)
    {
        return false;
    }
    
    for (const FString& Skill : RequiredSkills)
    {
        float SkillLevel = 0.0f;
        
        if (Skill == TEXT("戰鬥") || Skill == TEXT("Combat"))
        {
            SkillLevel = Merc->Skills.CombatSkill;
        }
        else if (Skill == TEXT("潛行") || Skill == TEXT("Stealth"))
        {
            SkillLevel = Merc->Skills.StealthSkill;
        }
        else if (Skill == TEXT("醫療") || Skill == TEXT("Medical"))
        {
            SkillLevel = Merc->Skills.MedicalSkill;
        }
        else if (Skill == TEXT("工程") || Skill == TEXT("Engineering"))
        {
            SkillLevel = Merc->Skills.EngineeringSkill;
        }
        else if (Skill == TEXT("領導") || Skill == TEXT("Leadership"))
        {
            SkillLevel = Merc->Skills.LeadershipSkill;
        }
        else if (Skill == TEXT("射擊") || Skill == TEXT("Marksmanship"))
        {
            SkillLevel = Merc->Skills.MarksmanshipSkill;
        }
        else if (Skill == TEXT("生存") || Skill == TEXT("Survival"))
        {
            SkillLevel = Merc->Skills.SurvivalSkill;
        }
        else if (Skill == TEXT("外交") || Skill == TEXT("Diplomacy"))
        {
            SkillLevel = Merc->Skills.DiplomacySkill;
        }
        
        if (SkillLevel < 50.0f) // Minimum skill requirement
        {
            return false;
        }
    }
    
    return true;
}

float UMingMercenaryMode::CalculateContractSuccessChance(const FString& ContractID) const
{
    const FMingContract* Contract = Contracts.Find(ContractID);
    if (!Contract)
    {
        return 0.0f;
    }
    
    // Calculate success chance based on assigned mercenaries and contract difficulty
    TArray<FString> AssignedMercs = GetAssignedMercenaries(ContractID);
    
    if (AssignedMercs.Num() == 0)
    {
        return 0.0f;
    }
    
    float TotalSkillLevel = 0.0f;
    int32 SkillCount = 0;
    
    for (const FString& MercID : AssignedMercs)
    {
        const FMingMercenary* Merc = Mercenaries.Find(MercID);
        if (Merc)
        {
            TotalSkillLevel += (Merc->Skills.CombatSkill + Merc->Skills.StealthSkill + 
                               Merc->Skills.MedicalSkill + Merc->Skills.EngineeringSkill + 
                               Merc->Skills.LeadershipSkill + Merc->Skills.MarksmanshipSkill + 
                               Merc->Skills.SurvivalSkill + Merc->Skills.DiplomacySkill) / 8.0f;
            SkillCount++;
        }
    }
    
    float AverageSkillLevel = TotalSkillLevel / SkillCount;
    float SuccessChance = (AverageSkillLevel / 100.0f) * (100.0f - Contract->Difficulty);
    
    return FMath::Clamp(SuccessChance, 0.0f, 100.0f);
}
