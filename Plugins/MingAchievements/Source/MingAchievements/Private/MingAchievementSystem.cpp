#include "MingAchievementSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingAchievementSystem::UMingAchievementSystem()
{
    WorldContext = GetWorld();
}

void UMingAchievementSystem::InitializeAchievements()
{
    Achievements.Empty();
    AchievementChains.Empty();
    GameStatistics.Empty();
    ProgressEvents.Empty();
    PendingRewards.Empty();
    
    InitializeCombatAchievements();
    InitializeCampaignAchievements();
    InitializeExplorationAchievements();
    InitializeMercenaryAchievements();
    InitializeHistoricalAchievements();
    InitializeEconomicAchievements();
    InitializeSpecialAchievements();
    InitializeSecretAchievements();
    
    UE_LOG(LogTemp, Log, TEXT("Achievement system initialized with %d achievements"), Achievements.Num());
}

void UMingAchievementSystem::InitializeCombatAchievements()
{
    // First Blood - First enemy kill
    FMingAchievement FirstBlood;
    FirstBlood.AchievementID = TEXT("ACHIEVEMENT_FIRST_BLOOD");
    FirstBlood.Title = TEXT("初見血");
    FirstBlood.Description = TEXT("擊殺第一個敵人");
    FirstBlood.LongDescription = TEXT("在戰場上取得你的第一次勝利，這是你軍事生涯的開始。");
    FirstBlood.Category = EMingAchievementCategory::Combat;
    FirstBlood.Difficulty = EMingAchievementDifficulty::Easy;
    FirstBlood.AchievementType = EMingAchievementType::OneTime;
    FirstBlood.IconPath = TEXT("UI/Achievements/FirstBlood");
    FirstBlood.BadgePath = TEXT("UI/Badges/CombatBronze");
    FirstBlood.Requirements.Add(TEXT("Kill_Enemy"));
    FirstBlood.Rewards.Add(TEXT("Combat_Bonus_5%"));
    FirstBlood.Points = 5;
    FirstBlood.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(FirstBlood.AchievementID, FirstBlood);
    
    // Warrior - 100 kills
    FMingAchievement Warrior;
    Warrior.AchievementID = TEXT("ACHIEVEMENT_WARRIOR");
    Warrior.Title = TEXT("戰士");
    Warrior.Description = TEXT("擊殺100個敵人");
    Warrior.LongDescription = TEXT("成為一名真正的戰士，在戰場上證明你的勇氣和技巧。");
    Warrior.Category = EMingAchievementCategory::Combat;
    Warrior.Difficulty = EMingAchievementDifficulty::Medium;
    Warrior.AchievementType = EMingAchievementType::Cumulative;
    Warrior.IconPath = TEXT("UI/Achievements/Warrior");
    Warrior.BadgePath = TEXT("UI/Badges/CombatSilver");
    Warrior.Requirements.Add(TEXT("Kill_Count_100"));
    Warrior.Rewards.Add(TEXT("Combat_Bonus_10%"));
    Warrior.Points = 15;
    Warrior.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Warrior.AchievementID, Warrior);
    
    // Legend - 1000 kills
    FMingAchievement Legend;
    Legend.AchievementID = TEXT("ACHIEVEMENT_LEGEND");
    Legend.Title = TEXT("傳奇");
    Legend.Description = TEXT("擊殺1000個敵人");
    Legend.LongDescription = TEXT("成為戰場上的傳奇人物，你的名字將被後世永遠銘記。");
    Legend.Category = EMingAchievementCategory::Combat;
    Legend.Difficulty = EMingAchievementDifficulty::Hard;
    Legend.AchievementType = EMingAchievementType::Cumulative;
    Legend.IconPath = TEXT("UI/Achievements/Legend");
    Legend.BadgePath = TEXT("UI/Badges/CombatGold");
    Legend.Requirements.Add(TEXT("Kill_Count_1000"));
    Legend.Rewards.Add(TEXT("Combat_Bonus_20%"));
    Legend.Points = 30;
    Legend.Progress.MaxProgress = 1000.0f;
    
    Achievements.Add(Legend.AchievementID, Legend);
    
    // Sharpshooter - 100 headshots
    FMingAchievement Sharpshooter;
    Sharpshooter.AchievementID = TEXT("ACHIEVEMENT_SHARPSHOOTER");
    Sharpshooter.Title = TEXT("神射手");
    Sharpshooter.Description = TEXT("完成100次爆頭擊殺");
    Sharpshooter.LongDescription = TEXT("精準的射擊是戰場上最致命的武器，證明你的槍法如神。");
    Sharpshooter.Category = EMingAchievementCategory::Combat;
    Sharpshooter.Difficulty = EMingAchievementDifficulty::Hard;
    Sharpshooter.AchievementType = EMingAchievementType::Cumulative;
    Sharpshooter.IconPath = TEXT("UI/Achievements/Sharpshooter");
    Sharpshooter.BadgePath = TEXT("UI/Badges/CombatExpert");
    Sharpshooter.Requirements.Add(TEXT("Headshot_Count_100"));
    Sharpshooter.Rewards.Add(TEXT("Accuracy_Bonus_15%"));
    Sharpshooter.Points = 25;
    Sharpshooter.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Sharpshooter.AchievementID, Sharpshooter);
    
    // Untouchable - Complete mission without taking damage
    FMingAchievement Untouchable;
    Untouchable.AchievementID = TEXT("ACHIEVEMENT_UNTOUCHABLE");
    Untouchable.Title = TEXT("無敵");
    Untouchable.Description = TEXT("無傷完成一個任務");
    Untouchable.LongDescription = TEXT("完美的戰術執行，讓敵人甚至無法觸碰到你。");
    Untouchable.Category = EMingAchievementCategory::Combat;
    Untouchable.Difficulty = EMingAchievementDifficulty::Expert;
    Untouchable.AchievementType = EMingAchievementType::OneTime;
    Untouchable.IconPath = TEXT("UI/Achievements/Untouchable");
    Untouchable.BadgePath = TEXT("UI/Badges/CombatMaster");
    Untouchable.Requirements.Add(TEXT("Mission_No_Damage"));
    Untouchable.Rewards.Add(TEXT("Defense_Bonus_25%"));
    Untouchable.Points = 35;
    Untouchable.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(Untouchable.AchievementID, Untouchable);
}

void UMingAchievementSystem::InitializeCampaignAchievements()
{
    // Revolutionary - Complete first campaign
    FMingAchievement Revolutionary;
    Revolutionary.AchievementID = TEXT("ACHIEVEMENT_REVOLUTIONARY");
    Revolutionary.Title = TEXT("革命者");
    Revolutionary.Description = TEXT("完成第一個戰役");
    Revolutionary.LongDescription = TEXT("踏上革命之路，為國家的未來而戰。");
    Revolutionary.Category = EMingAchievementCategory::Campaign;
    Revolutionary.Difficulty = EMingAchievementDifficulty::Easy;
    Revolutionary.AchievementType = EMingAchievementType::OneTime;
    Revolutionary.IconPath = TEXT("UI/Achievements/Revolutionary");
    Revolutionary.BadgePath = TEXT("UI/Badges/CampaignBronze");
    Revolutionary.Requirements.Add(TEXT("Complete_First_Campaign"));
    Revolutionary.Rewards.Add(TEXT("Campaign_Bonus_5%"));
    Revolutionary.Points = 10;
    Revolutionary.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(Revolutionary.AchievementID, Revolutionary);
    
    // Unifier - Complete all campaigns
    FMingAchievement Unifier;
    Unifier.AchievementID = TEXT("ACHIEVEMENT_UNIFIER");
    Unifier.Title = TEXT("統一者");
    Unifier.Description = TEXT("完成所有戰役");
    Unifier.LongDescription = TEXT("統一中國，實現國家的完整統一，你將成為歷史的見證者。");
    Unifier.Category = EMingAchievementCategory::Campaign;
    Unifier.Difficulty = EMingAchievementDifficulty::Legendary;
    Unifier.AchievementType = EMingAchievementType::Progress;
    Unifier.IconPath = TEXT("UI/Achievements/Unifier");
    Unifier.BadgePath = TEXT("UI/Badges/CampaignLegendary");
    Unifier.Requirements.Add(TEXT("Complete_All_Campaigns"));
    Unifier.Rewards.Add(TEXT("Campaign_Bonus_50%"));
    Unifier.Points = 50;
    Unifier.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Unifier.AchievementID, Unifier);
    
    // Strategist - Complete campaign with perfect strategy
    FMingAchievement Strategist;
    Strategist.AchievementID = TEXT("ACHIEVEMENT_STRATEGIST");
    Strategist.Title = TEXT("戰略家");
    Strategist.Description = TEXT("以完美戰略完成戰役");
    Strategist.LongDescription = TEXT("運用卓越的戰略思維，以最小的代價取得最大的勝利。");
    Strategist.Category = EMingAchievementCategory::Campaign;
    Strategist.Difficulty = EMingAchievementDifficulty::Expert;
    Strategist.AchievementType = EMingAchievementType::OneTime;
    Strategist.IconPath = TEXT("UI/Achievements/Strategist");
    Strategist.BadgePath = TEXT("UI/Badges/CampaignExpert");
    Strategist.Requirements.Add(TEXT("Perfect_Strategy"));
    Strategist.Rewards.Add(TEXT("Strategy_Bonus_30%"));
    Strategist.Points = 40;
    Strategist.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(Strategist.AchievementID, Strategist);
    
    // Speed Runner - Complete campaign in record time
    FMingAchievement SpeedRunner;
    SpeedRunner.AchievementID = TEXT("ACHIEVEMENT_SPEED_RUNNER");
    SpeedRunner.Title = TEXT("速通者");
    SpeedRunner.Description = TEXT("以破紀錄時間完成戰役");
    SpeedRunner.LongDescription = TEXT("速度與效率的完美結合，證明你的執行力無與倫比。");
    SpeedRunner.Category = EMingAchievementCategory::Campaign;
    SpeedRunner.Difficulty = EMingAchievementDifficulty::Hard;
    SpeedRunner.AchievementType = EMingAchievementType::OneTime;
    SpeedRunner.IconPath = TEXT("UI/Achievements/SpeedRunner");
    SpeedRunner.BadgePath = TEXT("UI/Badges/CampaignGold");
    SpeedRunner.Requirements.Add(TEXT("Record_Time"));
    SpeedRunner.Rewards.Add(TEXT("Speed_Bonus_20%"));
    SpeedRunner.Points = 30;
    SpeedRunner.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(SpeedRunner.AchievementID, SpeedRunner);
}

void UMingAchievementSystem::InitializeExplorationAchievements()
{
    // Explorer - Discover all locations
    FMingAchievement Explorer;
    Explorer.AchievementID = TEXT("ACHIEVEMENT_EXPLORER");
    Explorer.Title = TEXT("探險家");
    Explorer.Description = TEXT("發現所有地點");
    Explorer.LongDescription = TEXT("探索每一個角落，發現隱藏的秘密和寶藏。");
    Explorer.Category = EMingAchievementCategory::Exploration;
    Explorer.Difficulty = EMingAchievementDifficulty::Medium;
    Explorer.AchievementType = EMingAchievementType::Progress;
    Explorer.IconPath = TEXT("UI/Achievements/Explorer");
    Explorer.BadgePath = TEXT("UI/Badges/ExplorationSilver");
    Explorer.Requirements.Add(TEXT("Discover_All_Locations"));
    Explorer.Rewards.Add(TEXT("Vision_Bonus_15%"));
    Explorer.Points = 20;
    Explorer.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Explorer.AchievementID, Explorer);
    
    // Cartographer - Map all areas
    FMingAchievement Cartographer;
    Cartographer.AchievementID = TEXT("ACHIEVEMENT_CARTOGRAPHER");
    Cartographer.Title = TEXT("地圖製作者");
    Cartographer.Description = TEXT("繪製所有地區");
    Cartographer.LongDescription = TEXT("成為一名地圖製作者，為後人留下詳細的地圖。");
    Cartographer.Category = EMingAchievementCategory::Exploration;
    Cartographer.Difficulty = EMingAchievementDifficulty::Hard;
    Cartographer.AchievementType = EMingAchievementType::Progress;
    Cartographer.IconPath = TEXT("UI/Achievements/Cartographer");
    Cartographer.BadgePath = TEXT("UI/Badges/ExplorationGold");
    Cartographer.Requirements.Add(TEXT("Map_All_Areas"));
    Cartographer.Rewards.Add(TEXT("Map_Reveal_Bonus_25%"));
    Cartographer.Points = 35;
    Cartographer.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Cartographer.AchievementID, Cartographer);
    
    // Lost Treasure - Find hidden treasure
    FMingAchievement LostTreasure;
    LostTreasure.AchievementID = TEXT("ACHIEVEMENT_LOST_TREASURE");
    LostTreasure.Title = TEXT("失落的寶藏");
    LostTreasure.Description = TEXT("找到隱藏的寶藏");
    LostTreasure.LongDescription = TEXT發現歷史遺留下來的寶藏，獲得豐厚的獎勵。");
    LostTreasure.Category = EMingAchievementCategory::Exploration;
    LostTreasure.Difficulty = EMingAchievementDifficulty::Medium;
    LostTreasure.AchievementType = EMingAchievementType::OneTime;
    LostTreasure.IconPath = TEXT("UI/Achievements/LostTreasure");
    LostTreasure.BadgePath = TEXT("UI/Badges/ExplorationSpecial");
    LostTreasure.Requirements.Add(TEXT("Find_Hidden_Treasure"));
    LostTreasure.Rewards.Add(TEXT("Resource_Bonus_1000"));
    LostTreasure.Points = 25;
    LostTreasure.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(LostTreasure.AchievementID, LostTreasure);
}

void UMingAchievementSystem::InitializeMercenaryAchievements()
{
    // Contractor - Complete first mercenary contract
    FMingAchievement Contractor;
    Contractor.AchievementID = TEXT("ACHIEVEMENT_CONTRACTOR");
    Contractor.Title = TEXT("承包商");
    Contractor.Description = TEXT("完成第一個傭兵合約");
    Contractor.LongDescription = TEXT("開始你的傭兵生涯，接受第一個合約並成功完成。");
    Contractor.Category = EMingAchievementCategory::Mercenary;
    Contractor.Difficulty = EMingAchievementDifficulty::Easy;
    Contractor.AchievementType = EMingAchievementType::OneTime;
    Contractor.IconPath = TEXT("UI/Achievements/Contractor");
    Contractor.BadgePath = TEXT("UI/Badges/MercenaryBronze");
    Contractor.Requirements.Add(TEXT("Complete_First_Contract"));
    Contractor.Rewards.Add(TEXT("Contract_Bonus_5%"));
    Contractor.Points = 10;
    Contractor.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(Contractor.AchievementID, Contractor);
    
    // Mercenary Lord - Complete 50 contracts
    FMingAchievement MercenaryLord;
    MercenaryLord.AchievementID = TEXT("ACHIEVEMENT_MERCENARY_LORD");
    MercenaryLord.Title = TEXT("傭兵領主");
    MercenaryLord.Description = TEXT("完成50個傭兵合約");
    MercenaryLord.LongDescription = TEXT("成為傭兵界的領主，你的名字在黑暗世界中無人不知。");
    MercenaryLord.Category = EMingAchievementCategory::Mercenary;
    MercenaryLord.Difficulty = EMingAchievementDifficulty::Hard;
    MercenaryLord.AchievementType = EMingAchievementType::Cumulative;
    MercenaryLord.IconPath = TEXT("UI/Achievements/MercenaryLord");
    MercenaryLord.BadgePath = TEXT("UI/Badges/MercenaryGold");
    MercenaryLord.Requirements.Add(TEXT("Complete_50_Contracts"));
    MercenaryLord.Rewards.Add(TEXT("Contract_Bonus_30%"));
    MercenaryLord.Points = 40;
    MercenaryLord.Progress.MaxProgress = 50.0f;
    
    Achievements.Add(MercenaryLord.AchievementID, MercenaryLord);
    
    // Elite Squad - Build elite mercenary team
    FMingAchievement EliteSquad;
    EliteSquad.AchievementID = TEXT("ACHIEVEMENT_ELITE_SQUAD");
    EliteSquad.Title = TEXT("精英小隊");
    EliteSquad.Description = TEXT("建立精英傭兵小隊");
    EliteSquad.LongDescription = TEXT("組建一支由精英傭兵組成的無敵小隊。");
    EliteSquad.Category = EMingAchievementCategory::Mercenary;
    EliteSquad.Difficulty = EMingAchievementDifficulty::Expert;
    EliteSquad.AchievementType = EMingAchievementType::OneTime;
    EliteSquad.IconPath = TEXT("UI/Achievements/EliteSquad");
    EliteSquad.BadgePath = TEXT("UI/Badges/MercenaryExpert");
    EliteSquad.Requirements.Add(TEXT("Build_Elite_Team"));
    EliteSquad.Rewards.Add(TEXT("Team_Bonus_25%"));
    EliteSquad.Points = 35;
    EliteSquad.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(EliteSquad.AchievementID, EliteSquad);
}

void UMingAchievementSystem::InitializeHistoricalAchievements()
{
    // Historian - Complete all historical events
    FMingAchievement Historian;
    Historian.AchievementID = TEXT("ACHIEVEMENT_HISTORIAN");
    Historian.Title = TEXT("歷史學家");
    Historian.Description = TEXT("完成所有歷史事件");
    Historian.LongDescription = TEXT("深入了解歷史，見證每一個重要的歷史時刻。");
    Historian.Category = EMingAchievementCategory::Historical;
    Historian.Difficulty = EMingAchievementDifficulty::Hard;
    Historian.AchievementType = EMingAchievementType::Progress;
    Historian.IconPath = TEXT("UI/Achievements/Historian");
    Historian.BadgePath = TEXT("UI/Badges/HistoricalGold");
    Historian.Requirements.Add(TEXT("Complete_All_Historical_Events"));
    Historian.Rewards.Add(TEXT("Historical_Accuracy_Bonus_20%"));
    Historian.Points = 35;
    Historian.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Historian.AchievementID, Historian);
    
    // Time Traveler - Achieve 95% historical accuracy
    FMingAchievement TimeTraveler;
    TimeTraveler.AchievementID = TEXT("ACHIEVEMENT_TIME_TRAVELER");
    TimeTraveler.Title = TEXT("時空旅行者");
    TimeTraveler.Description = TEXT("達到95%歷史準確度");
    TimeTraveler.LongDescription = TEXT("完美重現歷史，彷彿你真的回到了過去。");
    TimeTraveler.Category = EMingAchievementCategory::Historical;
    TimeTraveler.Difficulty = EMingAchievementDifficulty::Legendary;
    TimeTraveler.AchievementType = EMingAchievementType::OneTime;
    TimeTraveler.IconPath = TEXT("UI/Achievements/TimeTraveler");
    TimeTraveler.BadgePath = TEXT("UI/Badges/HistoricalLegendary");
    TimeTraveler.Requirements.Add(TEXT("Historical_Accuracy_95%"));
    TimeTraveler.Rewards.Add(TEXT("Historical_Accuracy_Bonus_50%"));
    TimeTraveler.Points = 50;
    TimeTraveler.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(TimeTraveler.AchievementID, TimeTraveler);
    
    // Revolutionary Hero - Complete revolution perfectly
    FMingAchievement RevolutionaryHero;
    RevolutionaryHero.AchievementID = TEXT("ACHIEVEMENT_REVOLUTIONARY_HERO");
    RevolutionaryHero.Title = TEXT("革命英雄");
    RevolutionaryHero.Description = TEXT("完美完成革命");
    RevolutionaryHero.LongDescription = TEXT("成為真正的革命英雄，為國家的未來做出巨大貢獻。");
    RevolutionaryHero.Category = EMingAchievementCategory::Historical;
    RevolutionaryHero.Difficulty = EMingAchievementDifficulty::Expert;
    RevolutionaryHero.AchievementType = EMingAchievementType::OneTime;
    RevolutionaryHero.IconPath = TEXT("UI/Achievements/RevolutionaryHero");
    RevolutionaryHero.BadgePath = TEXT("UI/Badges/HistoricalExpert");
    RevolutionaryHero.Requirements.Add(TEXT("Perfect_Revolution"));
    RevolutionaryHero.Rewards.Add(TEXT("Revolution_Bonus_30%"));
    RevolutionaryHero.Points = 40;
    RevolutionaryHero.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(RevolutionaryHero.AchievementID, RevolutionaryHero);
}

void UMingAchievementSystem::InitializeEconomicAchievements()
{
    // Tycoon - Accumulate 100,000 resources
    FMingAchievement Tycoon;
    Tycoon.AchievementID = TEXT("ACHIEVEMENT_TYCOON");
    Tycoon.Title = TEXT("大亨");
    Tycoon.Description = TEXT("累積100,000資源");
    Tycoon.LongDescription = TEXT("成為經濟大亨，掌握巨大的財富和資源。");
    Tycoon.Category = EMingAchievementCategory::Economic;
    Tycoon.Difficulty = EMingAchievementDifficulty::Medium;
    Tycoon.AchievementType = EMingAchievementType::Cumulative;
    Tycoon.IconPath = TEXT("UI/Achievements/Tycoon");
    Tycoon.BadgePath = TEXT("UI/Badges/EconomicSilver");
    Tycoon.Requirements.Add(TEXT("Resource_Accumulation_100k"));
    Tycoon.Rewards.Add(TEXT("Resource_Bonus_15%"));
    Tycoon.Points = 20;
    Tycoon.Progress.MaxProgress = 100000.0f;
    
    Achievements.Add(Tycoon.AchievementID, Tycoon);
    
    // Industrialist - Build 100 buildings
    FMingAchievement Industrialist;
    Industrialist.AchievementID = TEXT("ACHIEVEMENT_INDUSTRIALIST");
    Industrialist.Title = TEXT("工業家");
    Industrialist.Description = TEXT("建造100座建築");
    Industrialist.LongDescription = TEXT("成為工業家，建立龐大的建築帝國。");
    Industrialist.Category = EMingAchievementCategory::Economic;
    Industrialist.Difficulty = EMingAchievementDifficulty::Medium;
    Industrialist.AchievementType = EMingAchievementType::Cumulative;
    Industrialist.IconPath = TEXT("UI/Achievements/Industrialist");
    Industrialist.BadgePath = TEXT("UI/Badges/EconomicGold");
    Industrialist.Requirements.Add(TEXT("Build_100_Buildings"));
    Industrialist.Rewards.Add(TEXT("Building_Bonus_20%"));
    Industrialist.Points = 25;
    Industrialist.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Industrialist.AchievementID, Industrialist);
    
    // Banker - Complete 1000 trades
    FMingAchievement Banker;
    Banker.AchievementID = TEXT("ACHIEVEMENT_BANKER");
    Banker.Title = TEXT("銀行家");
    Banker.Description = TEXT("完成1000次交易");
    Banker.LongDescription = TEXT("成為銀行家，掌握複雜的貿易網絡。");
    Banker.Category = EMingAchievementCategory::Economic;
    Banker.Difficulty = EMingAchievementDifficulty::Hard;
    Banker.AchievementType = EMingAchievementType::Cumulative;
    Banker.IconPath = TEXT("UI/Achievements/Banker");
    Banker.BadgePath = TEXT("UI/Badges/EconomicExpert");
    Banker.Requirements.Add(TEXT("Complete_1000_Trades"));
    Banker.Rewards.Add(TEXT("Trade_Bonus_25%"));
    Banker.Points = 35;
    Banker.Progress.MaxProgress = 1000.0f;
    
    Achievements.Add(Banker.AchievementID, Banker);
}

void UMingAchievementSystem::InitializeSpecialAchievements()
{
    // Perfectionist - Complete game 100%
    FMingAchievement Perfectionist;
    Perfectionist.AchievementID = TEXT("ACHIEVEMENT_PERFECTIONIST");
    Perfectionist.Title = TEXT("完美主義者");
    Perfectionist.Description = TEXT("100%完成遊戲");
    Perfectionist.LongDescription = TEXT("完美完成遊戲的每一個方面，成為真正的完美主義者。");
    Perfectionist.Category = EMingAchievementCategory::Special;
    Perfectionist.Difficulty = EMingAchievementDifficulty::Legendary;
    Perfectionist.AchievementType = EMingAchievementType::Progress;
    Perfectionist.IconPath = TEXT("UI/Achievements/Perfectionist");
    Perfectionist.BadgePath = TEXT("UI/Badges/SpecialLegendary");
    Perfectionist.Requirements.Add(TEXT("Complete_Game_100%"));
    Perfectionist.Rewards.Add(TEXT("All_Bonus_50%"));
    Perfectionist.Points = 100;
    Perfectionist.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(Perfectionist.AchievementID, Perfectionist);
    
    // Speed Demon - Complete game in under 10 hours
    FMingAchievement SpeedDemon;
    SpeedDemon.AchievementID = TEXT("ACHIEVEMENT_SPEED_DEMON");
    SpeedDemon.Title = TEXT("速度惡魔");
    SpeedDemon.Description = TEXT("10小時內完成遊戲");
    SpeedDemon.LongDescription = TEXT("以驚人的速度完成整個遊戲，證明你的效率無與倫比。");
    SpeedDemon.Category = EMingAchievementCategory::Special;
    SpeedDemon.Difficulty = EMingAchievementDifficulty::Expert;
    SpeedDemon.AchievementType = EMingAchievementType::Timed;
    SpeedDemon.IconPath = TEXT("UI/Achievements/SpeedDemon");
    SpeedDemon.BadgePath = TEXT("UI/Badges/SpecialExpert");
    SpeedDemon.Requirements.Add(TEXT("Complete_Game_Under_10h"));
    SpeedDemon.Rewards.Add(TEXT("Speed_Bonus_40%"));
    SpeedDemon.Points = 45;
    SpeedDemon.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(SpeedDemon.AchievementID, SpeedDemon);
    
    // Marathon Runner - Play for 100 hours
    FMingAchievement MarathonRunner;
    MarathonRunner.AchievementID = TEXT("ACHIEVEMENT_MARATHON_RUNNER");
    MarathonRunner.Title = TEXT("馬拉松選手");
    MarathonRunner.Description = TEXT("遊戲時間達到100小時");
    MarathonRunner.LongDescription = TEXT("長時間投入遊戲，成為真正的馬拉松選手。");
    MarathonRunner.Category = EMingAchievementCategory::Special;
    MarathonRunner.Difficulty = EMingAchievementDifficulty::Medium;
    MarathonRunner.AchievementType = EMingAchievementType::Cumulative;
    MarathonRunner.IconPath = TEXT("UI/Achievements/MarathonRunner");
    MarathonRunner.BadgePath = TEXT("UI/Badges/SpecialGold");
    MarathonRunner.Requirements.Add(TEXT("Play_Time_100h"));
    MarathonRunner.Rewards.Add(TEXT("Endurance_Bonus_20%"));
    MarathonRunner.Points = 30;
    MarathonRunner.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(MarathonRunner.AchievementID, MarathonRunner);
}

void UMingAchievementSystem::InitializeSecretAchievements()
{
    // Hidden Path - Discover secret route
    FMingAchievement HiddenPath;
    HiddenPath.AchievementID = TEXT("ACHIEVEMENT_HIDDEN_PATH");
    HiddenPath.Title = TEXT("隱藏路徑");
    HiddenPath.Description = TEXT("發現秘密路徑");
    HiddenPath.LongDescription = TEXT("發現隱藏的道路，開啟新的可能性。");
    HiddenPath.Category = EMingAchievementCategory::Secret;
    HiddenPath.Difficulty = EMingAchievementDifficulty::Expert;
    HiddenPath.AchievementType = EMingAchievementType::OneTime;
    HiddenPath.bIsSecret = true;
    HiddenPath.bIsHidden = true;
    HiddenPath.IconPath = TEXT("UI/Achievements/HiddenPath");
    HiddenPath.BadgePath = TEXT("UI/Badges/SecretExpert");
    HiddenPath.Requirements.Add(TEXT("Find_Secret_Route"));
    HiddenPath.Rewards.Add(TEXT("Secret_Bonus_30%"));
    HiddenPath.Points = 40;
    HiddenPath.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(HiddenPath.AchievementID, HiddenPath);
    
    // Truth Seeker - Uncover all secrets
    FMingAchievement TruthSeeker;
    TruthSeeker.AchievementID = TEXT("ACHIEVEMENT_TRUTH_SEEKER");
    TruthSeeker.Title = TEXT("真理追求者");
    TruthSeeker.Description = TEXT("揭露所有秘密");
    TruthSeeker.LongDescription = TEXT("追求真理，揭露所有隱藏的秘密。");
    TruthSeeker.Category = EMingAchievementCategory::Secret;
    TruthSeeker.Difficulty = EMingAchievementDifficulty::Legendary;
    TruthSeeker.AchievementType = EMingAchievementType::Progress;
    TruthSeeker.bIsSecret = true;
    TruthSeeker.bIsHidden = true;
    TruthSeeker.IconPath = TEXT("UI/Achievements/TruthSeeker");
    TruthSeeker.BadgePath = TEXT("UI/Badges/SecretLegendary");
    TruthSeeker.Requirements.Add(TEXT("Uncover_All_Secrets"));
    TruthSeeker.Rewards.Add(TEXT("Secret_Bonus_100%"));
    TruthSeeker.Points = 75;
    TruthSeeker.Progress.MaxProgress = 100.0f;
    
    Achievements.Add(TruthSeeker.AchievementID, TruthSeeker);
    
    // Alternate Reality - Complete alternate ending
    FMingAchievement AlternateReality;
    AlternateReality.AchievementID = TEXT("ACHIEVEMENT_ALTERNATE_REALITY");
    AlternateReality.Title = TEXT("平行現實");
    AlternateReality.Description = TEXT("完成分支結局");
    AlternateReality.LongDescription = TEXT("探索不同的歷史走向，見證平行現實的發展。");
    AlternateReality.Category = EMingAchievementCategory::Secret;
    AlternateReality.Difficulty = EMingAchievementDifficulty::Expert;
    AlternateReality.AchievementType = EMingAchievementType::OneTime;
    AlternateReality.bIsSecret = true;
    AlternateReality.bIsHidden = true;
    AlternateReality.IconPath = TEXT("UI/Achievements/AlternateReality");
    AlternateReality.BadgePath = TEXT("UI/Badges/SecretExpert");
    AlternateReality.Requirements.Add(TEXT("Complete_Alternate_Ending"));
    AlternateReality.Rewards.Add(TEXT("Alternate_Bonus_35%"));
    AlternateReality.Points = 45;
    AlternateReality.Progress.MaxProgress = 1.0f;
    
    Achievements.Add(AlternateReality.AchievementID, AlternateReality);
}

void UMingAchievementSystem::UnlockAchievement(const FString& AchievementID)
{
    FMingAchievement* Achievement = FindAchievement(AchievementID);
    if (!Achievement)
    {
        UE_LOG(LogTemp, Warning, TEXT("Achievement not found: %s"), *AchievementID);
        return;
    }
    
    if (Achievement->bIsUnlocked)
    {
        UE_LOG(LogTemp, Warning, TEXT("Achievement already unlocked: %s"), *Achievement->Title);
        return;
    }
    
    Achievement->bIsUnlocked = true;
    Achievement->Progress.bIsCompleted = true;
    Achievement->UnlockTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
    Achievement->Progress.CurrentProgress = Achievement->Progress.MaxProgress;
    
    // Grant rewards
    GrantAchievementRewards(*Achievement);
    
    // Update chains
    UpdateAchievementChains(AchievementID);
    
    // Unlock hidden achievements
    UnlockHiddenAchievements();
    
    // Show notification
    ShowAchievementUnlocked(AchievementID);
    
    // Trigger event
    OnAchievementUnlocked.Broadcast(*Achievement, Achievement->Points);
    
    UE_LOG(LogTemp, Log, TEXT("Achievement unlocked: %s (%d points)"), *Achievement->Title, Achievement->Points);
}

void UMingAchievementSystem::UpdateProgress(const FString& AchievementID, float Progress)
{
    FMingAchievement* Achievement = FindAchievement(AchievementID);
    if (!Achievement || Achievement->bIsUnlocked)
    {
        return;
    }
    
    float OldProgress = Achievement->Progress.CurrentProgress;
    Achievement->Progress.CurrentProgress = FMath::Min(Progress, Achievement->Progress.MaxProgress);
    
    // Check if completed
    if (!Achievement->Progress.bIsCompleted && 
        Achievement->Progress.CurrentProgress >= Achievement->Progress.MaxProgress)
    {
        CompleteAchievement(AchievementID);
    }
    else if (OldProgress != Achievement->Progress.CurrentProgress)
    {
        ShowProgressUpdate(AchievementID);
        OnProgressUpdated.Broadcast(AchievementID, Achievement->Progress.CurrentProgress);
    }
}

void UMingAchievementSystem::ResetProgress(const FString& AchievementID)
{
    FMingAchievement* Achievement = FindAchievement(AchievementID);
    if (!Achievement)
    {
        return;
    }
    
    Achievement->Progress.CurrentProgress = 0.0f;
    Achievement->Progress.bIsCompleted = false;
    Achievement->Progress.CompletionTime = 0.0f;
    Achievement->Progress.ProgressData.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Reset progress for achievement: %s"), *Achievement->Title);
}

void UMingAchievementSystem::CompleteAchievement(const FString& AchievementID)
{
    FMingAchievement* Achievement = FindAchievement(AchievementID);
    if (!Achievement)
    {
        return;
    }
    
    if (!Achievement->bIsUnlocked)
    {
        UnlockAchievement(AchievementID);
    }
    else
    {
        Achievement->CompletionCount++;
        Achievement->Progress.CompletionTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
        
        UE_LOG(LogTemp, Log, TEXT("Completed achievement again: %s (count: %d)"), 
               *Achievement->Title, Achievement->CompletionCount);
    }
}

TArray<FMingAchievement> UMingAchievementSystem::GetAllAchievements() const
{
    TArray<FMingAchievement> Result;
    for (const auto& Pair : Achievements)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FMingAchievement> UMingAchievementSystem::GetUnlockedAchievements() const
{
    TArray<FMingAchievement> Result;
    for (const auto& Pair : Achievements)
    {
        if (Pair.Value.bIsUnlocked)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingAchievement> UMingAchievementSystem::GetLockedAchievements() const
{
    TArray<FMingAchievement> Result;
    for (const auto& Pair : Achievements)
    {
        if (!Pair.Value.bIsUnlocked)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingAchievement> UMingAchievementSystem::GetAchievementsByCategory(EMingAchievementCategory Category) const
{
    TArray<FMingAchievement> Result;
    for (const auto& Pair : Achievements)
    {
        if (Pair.Value.Category == Category)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FMingAchievement UMingAchievementSystem::GetAchievement(const FString& AchievementID) const
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    return Achievement ? *Achievement : FMingAchievement();
}

void UMingAchievementSystem::RegisterProgressEvent(const FString& EventName, float Value)
{
    ProgressEvents.Add(EventName, Value);
    ProcessProgressEvent(EventName, Value);
}

void UMingAchievementSystem::TrackStatistic(const FString& StatName, float Value)
{
    float* CurrentValue = GameStatistics.Find(StatName);
    if (CurrentValue)
    {
        *CurrentValue += Value;
    }
    else
    {
        GameStatistics.Add(StatName, Value);
    }
    
    // Check achievements related to this statistic
    UpdateGameStatistics();
}

void UMingAchievementSystem::UpdateGameStatistics()
{
    // Update achievements based on current statistics
    for (auto& Pair : Achievements)
    {
        FMingAchievement& Achievement = Pair.Value;
        if (Achievement.bIsUnlocked)
        {
            continue;
        }
        
        CheckAchievementConditions(Achievement.AchievementID);
    }
}

float UMingAchievementSystem::GetAchievementProgress(const FString& AchievementID) const
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    if (!Achievement)
    {
        return 0.0f;
    }
    
    return CalculateProgressPercentage(AchievementID);
}

bool UMingAchievementSystem::IsAchievementCompleted(const FString& AchievementID) const
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    return Achievement ? Achievement->bIsUnlocked : false;
}

float UMingAchievementSystem::GetOverallProgress() const
{
    if (Achievements.Num() == 0)
    {
        return 0.0f;
    }
    
    int32 UnlockedCount = 0;
    for (const auto& Pair : Achievements)
    {
        if (Pair.Value.bIsUnlocked)
        {
            UnlockedCount++;
        }
    }
    
    return (float)UnlockedCount / Achievements.Num() * 100.0f;
}

void UMingAchievementSystem::CreateAchievementChain(const FMingAchievementChain& Chain)
{
    AchievementChains.Add(Chain.ChainID, Chain);
    UE_LOG(LogTemp, Log, TEXT("Created achievement chain: %s"), *Chain.ChainName);
}

TArray<FMingAchievementChain> UMingAchievementSystem::GetAllChains() const
{
    TArray<FMingAchievementChain> Result;
    for (const auto& Pair : AchievementChains)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

FMingAchievementChain UMingAchievementSystem::GetChain(const FString& ChainID) const
{
    const FMingAchievementChain* Chain = AchievementChains.Find(ChainID);
    return Chain ? *Chain : FMingAchievementChain();
}

bool UMingAchievementSystem::IsChainCompleted(const FString& ChainID) const
{
    const FMingAchievementChain* Chain = AchievementChains.Find(ChainID);
    return Chain ? Chain->bIsCompleted : false;
}

void UMingAchievementSystem::IncrementKillCount(const FString& UnitType)
{
    FString StatName = FString::Printf(TEXT("Kill_Count_%s"), *UnitType);
    TrackStatistic(StatName, 1.0f);
    
    // Also update total kill count
    TrackStatistic(TEXT("Total_Kill_Count"), 1.0f);
    
    // Check kill-related achievements
    RegisterProgressEvent(TEXT("Kill_Enemy"), 1.0f);
    
    float TotalKills = *GameStatistics.Find(TEXT("Total_Kill_Count"));
    if (TotalKills >= 100.0f)
    {
        UpdateProgress(TEXT("ACHIEVEMENT_WARRIOR"), TotalKills);
    }
    if (TotalKills >= 1000.0f)
    {
        UpdateProgress(TEXT("ACHIEVEMENT_LEGEND"), TotalKills);
    }
}

void UMingAchievementSystem::AddToPlayTime(float Time)
{
    TrackStatistic(TEXT("Total_Play_Time"), Time);
    
    float TotalTime = *GameStatistics.Find(TEXT("Total_Play_Time"));
    if (TotalTime >= 360000.0f) // 100 hours in seconds
    {
        UpdateProgress(TEXT("ACHIEVEMENT_MARATHON_RUNNER"), TotalTime / 3600.0f);
    }
}

void UMingAchievementSystem::RecordVictory(const FString& MissionID)
{
    TrackStatistic(FString::Printf(TEXT("Victory_%s"), *MissionID), 1.0f);
    TrackStatistic(TEXT("Total_Victories"), 1.0f);
    
    RegisterProgressEvent(TEXT("Mission_Victory"), 1.0f);
}

void UMingAchievementSystem::RecordDefeat(const FString& MissionID)
{
    TrackStatistic(FString::Printf(TEXT("Defeat_%s"), *MissionID), 1.0f);
    TrackStatistic(TEXT("Total_Defeats"), 1.0f);
    
    RegisterProgressEvent(TEXT("Mission_Defeat"), 1.0f);
}

void UMingAchievementSystem::RecordResourceCollection(EMingResourceType ResourceType, float Amount)
{
    FString ResourceName;
    switch (ResourceType)
    {
    case EMingResourceType::Gold: ResourceName = TEXT("Gold"); break;
    case EMingResourceType::Food: ResourceName = TEXT("Food"); break;
    case EMingResourceType::Wood: ResourceName = TEXT("Wood"); break;
    case EMingResourceType::Stone: ResourceName = TEXT("Stone"); break;
    default: ResourceName = TEXT("Unknown"); break;
    }
    
    TrackStatistic(FString::Printf(TEXT("Resource_%s"), *ResourceName), Amount);
    TrackStatistic(TEXT("Total_Resources"), Amount);
    
    float TotalResources = *GameStatistics.Find(TEXT("Total_Resources"));
    if (TotalResources >= 100000.0f)
    {
        UpdateProgress(TEXT("ACHIEVEMENT_TYCOON"), TotalResources);
    }
}

void UMingAchievementSystem::RecordBuildingConstructed(const FString& BuildingType)
{
    TrackStatistic(FString::Printf(TEXT("Building_%s"), *BuildingType), 1.0f);
    TrackStatistic(TEXT("Total_Buildings"), 1.0f);
    
    float TotalBuildings = *GameStatistics.Find(TEXT("Total_Buildings"));
    if (TotalBuildings >= 100.0f)
    {
        UpdateProgress(TEXT("ACHIEVEMENT_INDUSTRIALIST"), TotalBuildings);
    }
}

void UMingAchievementSystem::RecordUnitTrained(const FString& UnitType)
{
    TrackStatistic(FString::Printf(TEXT("Unit_%s"), *UnitType), 1.0f);
    TrackStatistic(TEXT("Total_Units"), 1.0f);
}

int32 UMingAchievementSystem::GetTotalAchievements() const
{
    return Achievements.Num();
}

int32 UMingAchievementSystem::GetUnlockedCount() const
{
    int32 Count = 0;
    for (const auto& Pair : Achievements)
    {
        if (Pair.Value.bIsUnlocked)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingAchievementSystem::GetTotalPoints() const
{
    int32 TotalPoints = 0;
    for (const auto& Pair : Achievements)
    {
        if (Pair.Value.bIsUnlocked)
        {
            TotalPoints += Pair.Value.Points;
        }
    }
    return TotalPoints;
}

float UMingAchievementSystem::GetCompletionPercentage() const
{
    return GetOverallProgress();
}

void UMingAchievementSystem::GrantReward(const FString& AchievementID)
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    if (!Achievement)
    {
        return;
    }
    
    GrantAchievementRewards(*Achievement);
}

void UMingAchievementSystem::GrantAllRewards()
{
    for (const FString& RewardID : PendingRewards)
    {
        ClaimReward(RewardID);
    }
}

TArray<FString> UMingAchievementSystem::GetPendingRewards() const
{
    return PendingRewards;
}

void UMingAchievementSystem::ClaimReward(const FString& RewardID)
{
    PendingRewards.Remove(RewardID);
    UE_LOG(LogTemp, Log, TEXT("Claimed reward: %s"), *RewardID);
}

void UMingAchievementSystem::ShowAchievementUnlocked(const FString& AchievementID)
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    if (Achievement)
    {
        UE_LOG(LogTemp, Log, TEXT("Achievement Unlocked: %s - %s"), *Achievement->Title, *Achievement->Description);
    }
}

void UMingAchievementSystem::ShowProgressUpdate(const FString& AchievementID)
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    if (Achievement)
    {
        float Progress = CalculateProgressPercentage(AchievementID);
        UE_LOG(LogTemp, Log, TEXT("Achievement Progress: %s - %.1f%%"), *Achievement->Title, Progress);
    }
}

void UMingAchievementSystem::ShowChainCompleted(const FString& ChainID)
{
    const FMingAchievementChain* Chain = AchievementChains.Find(ChainID);
    if (Chain)
    {
        UE_LOG(LogTemp, Log, TEXT("Achievement Chain Completed: %s - %s"), *Chain->ChainName, *Chain->FinalReward);
        OnChainCompleted.Broadcast(ChainID, Chain->FinalReward);
    }
}

void UMingAchievementSystem::ShowMilestoneReached(float Milestone)
{
    UE_LOG(LogTemp, Log, TEXT("Achievement Milestone Reached: %.1f%%"), Milestone);
    
    if (Milestone >= 100.0f)
    {
        OnAllAchievementsCompleted.Broadcast(GetTotalPoints());
    }
}

FString UMingAchievementSystem::GetCategoryName(EMingAchievementCategory Category)
{
    switch (Category)
    {
    case EMingAchievementCategory::Combat: return TEXT("戰鬥");
    case EMingAchievementCategory::Campaign: return TEXT("戰役");
    case EMingAchievementCategory::Exploration: return TEXT("探索");
    case EMingAchievementCategory::Mercenary: return TEXT("傭兵");
    case EMingAchievementCategory::Historical: return TEXT("歷史");
    case EMingAchievementCategory::Economic: return TEXT("經濟");
    case EMingAchievementCategory::Special: return TEXT("特殊");
    case EMingAchievementCategory::Secret: return TEXT("秘密");
    default: return TEXT("未知");
    }
}

FString UMingAchievementSystem::GetDifficultyName(EMingAchievementDifficulty Difficulty)
{
    switch (Difficulty)
    {
    case EMingAchievementDifficulty::Easy: return TEXT("簡單");
    case EMingAchievementDifficulty::Medium: return TEXT("中等");
    case EMingAchievementDifficulty::Hard: return TEXT("困難");
    case EMingAchievementDifficulty::Expert: return TEXT("專家");
    case EMingAchievementDifficulty::Legendary: return TEXT("傳奇");
    default: return TEXT("未知");
    }
}

FString UMingAchievementSystem::GetTypeName(EMingAchievementType Type)
{
    switch (Type)
    {
    case EMingAchievementType::Progress: return TEXT("進度");
    case EMingAchievementType::Cumulative: return TEXT("累計");
    case EMingAchievementType::OneTime: return TEXT("一次性");
    case EMingAchievementType::Chain: return TEXT("鏈式");
    case EMingAchievementType::Timed: return TEXT("限時");
    case EMingAchievementType::Hidden: return TEXT("隱藏");
    default: return TEXT("未知");
    }
}

FString UMingAchievementSystem::SaveAchievementData() const
{
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"total_achievements\": %d,\n"), GetTotalAchievements());
    Result += FString::Printf(TEXT("  \"unlocked_count\": %d,\n"), GetUnlockedCount());
    Result += FString::Printf(TEXT("  \"total_points\": %d,\n"), GetTotalPoints());
    Result += FString::Printf(TEXT("  \"completion_percentage\": %.2f,\n"), GetCompletionPercentage());
    Result += FString::Printf(TEXT("  \"pending_rewards\": %d,\n"), PendingRewards.Num());
    Result += FString::Printf(TEXT("  \"game_statistics\": %d,\n"), GameStatistics.Num());
    Result += FString::Printf(TEXT("  \"achievement_chains\": %d\n"), AchievementChains.Num());
    Result += TEXT("}\n");
    
    return Result;
}

void UMingAchievementSystem::LoadAchievementData(const FString& JsonString)
{
    // Parse JSON and restore achievement data
    UE_LOG(LogTemp, Log, TEXT("Loading achievement data"));
}

void UMingAchievementSystem::ProcessProgressEvent(const FString& EventName, float Value)
{
    // Process specific progress events and update related achievements
    if (EventName == TEXT("Kill_Enemy"))
    {
        UpdateProgress(TEXT("ACHIEVEMENT_FIRST_BLOOD"), 1.0f);
    }
    else if (EventName == TEXT("Mission_Victory"))
    {
        UpdateProgress(TEXT("ACHIEVEMENT_REVOLUTIONARY"), 1.0f);
    }
    else if (EventName == TEXT("Contract_Completed"))
    {
        UpdateProgress(TEXT("ACHIEVEMENT_CONTRACTOR"), Value);
    }
}

void UMingAchievementSystem::CheckAchievementConditions(const FString& AchievementID)
{
    FMingAchievement* Achievement = FindAchievement(AchievementID);
    if (!Achievement || Achievement->bIsUnlocked)
    {
        return;
    }
    
    // Check if all requirements are met
    bool bAllRequirementsMet = true;
    
    for (const FString& Requirement : Achievement->Requirements)
    {
        if (!ValidateAchievementRequirement(Requirement))
        {
            bAllRequirementsMet = false;
            break;
        }
    }
    
    if (bAllRequirementsMet && ArePrerequisitesMet(AchievementID))
    {
        CompleteAchievement(AchievementID);
    }
}

void UMingAchievementSystem::UpdateAchievementChains(const FString& AchievementID)
{
    // Update any chains that include this achievement
    for (auto& Pair : AchievementChains)
    {
        FMingAchievementChain& Chain = Pair.Value;
        
        if (Chain.AchievementIDs.Contains(AchievementID))
        {
            // Find the position of this achievement in the chain
            int32 Index = Chain.AchievementIDs.Find(AchievementID);
            
            // Check if this is the next achievement in the chain
            if (Index == Chain.CurrentIndex)
            {
                Chain.CurrentIndex++;
                
                // Check if chain is completed
                if (Chain.CurrentIndex >= Chain.AchievementIDs.Num())
                {
                    Chain.bIsCompleted = true;
                    ShowChainCompleted(Chain.ChainID);
                }
            }
        }
    }
}

void UMingAchievementSystem::GrantAchievementRewards(const FMingAchievement& Achievement)
{
    for (const FString& Reward : Achievement.Rewards)
    {
        PendingRewards.Add(Reward);
        UE_LOG(LogTemp, Log, TEXT("Added reward: %s"), *Reward);
    }
}

void UMingAchievementSystem::UpdateProgressAchievement(const FString& AchievementID, float Progress)
{
    UpdateProgress(AchievementID, Progress);
}

void UMingAchievementSystem::UpdateCumulativeAchievement(const FString& AchievementID, float Progress)
{
    UpdateProgress(AchievementID, Progress);
}

void UMingAchievementSystem::UpdateOneTimeAchievement(const FString& AchievementID)
{
    CompleteAchievement(AchievementID);
}

void UMingAchievementSystem::UpdateTimedAchievement(const FString& AchievementID)
{
    // Check time-based conditions
    FMingAchievement* Achievement = FindAchievement(AchievementID);
    if (Achievement)
    {
        float CurrentTime = WorldContext.IsValid() ? WorldContext->GetTimeSeconds() : 0.0f;
        Achievement->Progress.CompletionTime = CurrentTime;
        
        // Check if time condition is met
        if (ValidateAchievementRequirements(AchievementID))
        {
            CompleteAchievement(AchievementID);
        }
    }
}

bool UMingAchievementSystem::ValidateAchievementRequirements(const FString& AchievementID) const
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    if (!Achievement)
    {
        return false;
    }
    
    for (const FString& Requirement : Achievement->Requirements)
    {
        if (!ValidateAchievementRequirement(Requirement))
        {
            return false;
        }
    }
    
    return true;
}

bool UMingAchievementSystem::ValidateAchievementRequirement(const FString& Requirement) const
{
    // Parse and validate specific requirements
    if (Requirement.StartsWith(TEXT("Kill_Count_")))
    {
        FString CountStr = Requirement.RightChop(11); // Remove "Kill_Count_"
        int32 RequiredCount = FCString::Atoi(*CountStr);
        
        const float* TotalKills = GameStatistics.Find(TEXT("Total_Kill_Count"));
        return TotalKills && *TotalKills >= RequiredCount;
    }
    else if (Requirement.StartsWith(TEXT("Resource_Accumulation_")))
    {
        FString AmountStr = Requirement.RightChop(20); // Remove "Resource_Accumulation_"
        float RequiredAmount = FCString::Atof(*AmountStr);
        
        const float* TotalResources = GameStatistics.Find(TEXT("Total_Resources"));
        return TotalResources && *TotalResources >= RequiredAmount;
    }
    else if (Requirement == TEXT("Complete_First_Campaign"))
    {
        const float* Victories = GameStatistics.Find(TEXT("Total_Victories"));
        return Victories && *Victories >= 1.0f;
    }
    else if (Requirement == TEXT("Complete_All_Campaigns"))
    {
        const float* Victories = GameStatistics.Find(TEXT("Total_Victories"));
        return Victories && *Victories >= 10.0f; // Assuming 10 campaigns
    }
    
    return false;
}

bool UMingAchievementSystem::ArePrerequisitesMet(const FString& AchievementID) const
{
    // Check if prerequisite achievements are unlocked
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    if (!Achievement)
    {
        return false;
    }
    
    // This would check for any prerequisite achievements
    // For now, return true as we don't have explicit prerequisites
    return true;
}

FMingAchievement* UMingAchievementSystem::FindAchievement(const FString& AchievementID)
{
    return Achievements.Find(AchievementID);
}

FMingAchievementChain* UMingAchievementSystem::FindChain(const FString& ChainID)
{
    return AchievementChains.Find(ChainID);
}

float UMingAchievementSystem::CalculateProgressPercentage(const FString& AchievementID) const
{
    const FMingAchievement* Achievement = Achievements.Find(AchievementID);
    if (!Achievement)
    {
        return 0.0f;
    }
    
    if (Achievement->Progress.MaxProgress <= 0.0f)
    {
        return 0.0f;
    }
    
    return (Achievement->Progress.CurrentProgress / Achievement->Progress.MaxProgress) * 100.0f;
}

void UMingAchievementSystem::UnlockHiddenAchievements()
{
    // Unlock hidden achievements based on current progress
    float OverallProgress = GetOverallProgress();
    
    if (OverallProgress >= 50.0f) // 50% progress unlocks some hidden achievements
    {
        for (auto& Pair : Achievements)
        {
            FMingAchievement& Achievement = Pair.Value;
            if (Achievement.bIsHidden && !Achievement.bIsUnlocked)
            {
                Achievement.bIsHidden = false;
                UE_LOG(LogTemp, Log, TEXT("Unlocked hidden achievement: %s"), *Achievement.Title);
            }
        }
    }
}
