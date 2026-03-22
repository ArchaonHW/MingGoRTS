// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 中國共產黨 - Communist Party Implementation

#include "RepublicEraFactions/Factions/CommunistParty.h"
#include "MingRTSPlayerController.h"

UCommunistParty::UCommunistParty()
{
    InitializeAttributes();
    InitializeUniqueUnits();
    InitializeUniqueMechanics();
    InitializeGameplayGuide();
}

void UCommunistParty::InitializeFaction()
{
    Super::InitializeFaction();
    UE_LOG(LogTemp, Log, TEXT("Communist Party faction initialized"));
}

void UCommunistParty::InitializeAttributes()
{
    Attributes.FactionID = FName("CommunistParty");
    Attributes.FactionName = FText::FromString(TEXT("中國共產黨"));
    Attributes.FactionNameEnglish = FText::FromString(TEXT("Chinese Communist Party"));
    Attributes.Difficulty = EFactionDifficulty::Hard;
    Attributes.Category = EFactionCategory::Revolutionary;
    Attributes.FactionColor = FLinearColor(0.9f, 0.1f, 0.1f, 1.0f); // Bright Red
    
    Attributes.StartingPosition.CapitalCity = FText::FromString(TEXT("延安"));
    Attributes.StartingPosition.StartingTerritories = {
        FText::FromString(TEXT("延安")),
        FText::FromString(TEXT("江西"))
    };
    Attributes.StartingPosition.StartingYear = 1927;
    Attributes.StartingPosition.InitialGold = 600; // Lower resources
    Attributes.StartingPosition.InitialManpower = 400;
    Attributes.StartingPosition.InitialReputation = 30; // Low international recognition
    
    Attributes.Traits.Advantages = {
        FText::FromString(TEXT("人民戰爭動員能力，可以快速招募大量部隊")),
        FText::FromString(TEXT("游擊戰專精，在山地和農村地區戰鬥力極強")),
        FText::FromString(TEXT("土地改革獲得農民廣泛支持，經濟基礎穩固")),
        FText::FromString(TEXT("政治工作確保部隊高士氣和忠誠度"))
    };
    
    Attributes.Traits.Disadvantages = {
        FText::FromString(TEXT("起始資源和地盤都非常有限")),
        FText::FromString(TEXT("國際社會不承認，外交極度困難")),
        FText::FromString(TEXT("缺乏現代化武器和裝備")),
        FText::FromString(TEXT("必須不斷躲避政府軍圍剿"))
    };
    
    Attributes.Traits.RecommendedStrategies = {
        FText::FromString(TEXT("利用游擊戰避免正面決戰")),
        FText::FromString(TEXT("深入農村發動群眾建立根據地")),
        FText::FromString(TEXT("長期積累力量等待時機"))
    };
    
    Attributes.Traits.PlayerTypeRecommendation = FText::FromString(
        TEXT("推薦給喜歡游擊戰和人民戰爭的玩家，難度高但後期潛力巨大"));
}

void UCommunistParty::InitializeUniqueUnits()
{
    UniqueUnits.Add(CreateRedArmyGuerrillas());
    UniqueUnits.Add(CreateWorkersPeasantsRedGuards());
    UniqueUnits.Add(CreatePoliticalCommissars());
    UniqueUnits.Add(CreateLogisticsSupportCorps());
}

void UCommunistParty::InitializeUniqueMechanics()
{
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("PeoplesWar");
        Mechanic.MechanicName = FText::FromString(TEXT("人民戰爭"));
        Mechanic.Description = FText::FromString(TEXT("動員廣大農民參加革命，游擊戰鬥力提升50%，可以快速從農村地區招募部隊"));
        Mechanic.MechanicType = EFactionMechanicType::Military;
        Mechanic.EffectMultiplier = 1.5f;
        UniqueMechanics.Add(Mechanic);
    }
    
    {
        FFactionMechanic Mechanic;
        Mechanic.MechanicID = FName("LandReform");
        Mechanic.MechanicName = FText::FromString(TEXT("土地改革"));
        Mechanic.Description = FText::FromString(TEXT("分配土地給農民獲得廣泛支持，農村地區經濟收入增加40%，部隊補給更加充足"));
        Mechanic.MechanicType = EFactionMechanicType::Economic;
        Mechanic.EffectMultiplier = 1.4f;
        UniqueMechanics.Add(Mechanic);
    }
}

void UCommunistParty::InitializeGameplayGuide()
{
    GameplayGuide.EarlyGameStrategy = FText::FromString(
        TEXT("早期非常困難，必須在農村地區秘密發展組織。避免與政府軍正面衝突，保存實力。"));
    
    GameplayGuide.MidGameStrategy = FText::FromString(
        TEXT("中期建立農村根據地，發動土地改革爭取農民支持。利用游擊戰騷擾敵人，逐步壯大力量。"));
    
    GameplayGuide.LateGameStrategy = FText::FromString(
        TEXT("後期力量足夠後開始反攻，利用人民戰爭優勢席捲全國。最終推翻舊政府建立新政權。"));
    
    GameplayGuide.MilitaryTactics = FText::FromString(
        TEXT("絕對避免正面決戰，善用游擊戰、運動戰。敵進我退，敵駐我擾，敵疲我打，敵退我追。"));
    
    GameplayGuide.DiplomaticAdvice = FText::FromString(
        TEXT("在國際上尋求蘇聯支持。對其他勢力採取統一戰線策略，聯合次要敵人打擊主要敵人。"));
    
    GameplayGuide.EconomicFocus = FText::FromString(
        TEXT("主要依靠農村經濟和土地改革。不需要大量工業，但必須確保農民的衷心支持。"));
    
    GameplayGuide.KeyDecisions = {
        FText::FromString(TEXT("何時發動土地改革（太早會引發地主反撲，太晚錯失民心）")),
        FText::FromString(TEXT("長征路線的選擇和根據地的建立位置")),
        FText::FromString(TEXT("與國民黨的關係處理（暫時合作還是堅持對抗）"))
    };
    
    GameplayGuide.VictoryConditions = {
        FText::FromString(TEXT("推翻國民政府，建立新中國")),
        FText::FromString(TEXT="完成社會主義革命，實現共產主義理想")),
        FText::FromString(TEXT("獲得最終的歷史勝利"))
    };
}

FFactionUnit UCommunistParty::CreateRedArmyGuerrillas() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 110;
    Stats.DefensePower = 90;
    Stats.MovementSpeed = 130;
    Stats.HealthPoints = 90;
    Stats.ProductionCost = 70;
    Stats.UpkeepCost = 5;
    Stats.TrainingTime = 6.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("RedArmyGuerrillas");
    Unit.UnitName = FText::FromString(TEXT("紅軍游擊隊"));
    Unit.Description = FText::FromString(TEXT("擅長游擊戰的精銳戰士，在山區和農村地區戰鬥力極強，成本低廉"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Guerrilla);
    Unit.Traits.Add(EUnitTrait::Mountain);
    
    return Unit;
}

FFactionUnit UCommunistParty::CreateWorkersPeasantsRedGuards() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 80;
    Stats.DefensePower = 70;
    Stats.MovementSpeed = 100;
    Stats.HealthPoints = 80;
    Stats.ProductionCost = 40;
    Stats.UpkeepCost = 3;
    Stats.TrainingTime = 3.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("WorkersPeasantsRedGuards");
    Unit.UnitName = FText::FromString(TEXT("工農赤衛隊"));
    Unit.Description = FText::FromString(TEXT("由工人和農民組成的民兵組織，雖然戰鬥力一般但數量龐大且成本極低"));
    Unit.UnitType = EFactionUnitType::Infantry;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Political);
    
    return Unit;
}

FFactionUnit UCommunistParty::CreatePoliticalCommissars() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 50;
    Stats.DefensePower = 70;
    Stats.MovementSpeed = 90;
    Stats.HealthPoints = 70;
    Stats.ProductionCost = 90;
    Stats.UpkeepCost = 7;
    Stats.TrainingTime = 10.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("PoliticalCommissars");
    Unit.UnitName = FText::FromString(TEXT("政治委員隊"));
    Unit.Description = FText::FromString(TEXT("負責部隊的政治工作和思想教育，能大幅提升友軍士氣和忠誠度，防止崩潰"));
    Unit.UnitType = EFactionUnitType::Support;
    Unit.BaseStats = Stats;
    Unit.Traits.Add(EUnitTrait::Political);
    
    return Unit;
}

FFactionUnit UCommunistParty::CreateLogisticsSupportCorps() const
{
    FFactionUnitStats Stats;
    Stats.AttackPower = 40;
    Stats.DefensePower = 60;
    Stats.MovementSpeed = 80;
    Stats.HealthPoints = 60;
    Stats.ProductionCost = 60;
    Stats.UpkeepCost = 4;
    Stats.TrainingTime = 4.0f;
    
    FFactionUnit Unit;
    Unit.UnitID = FName("LogisticsSupportCorps");
    Unit.UnitName = FText::FromString(TEXT("後勤支援隊"));
    Unit.Description = FText::FromString(TEXT("由農民組成的後勤隊伍，利用人民戰爭優勢從農村獲取補給，成本極低"));
    Unit.UnitType = EFactionUnitType::Support;
    Unit.BaseStats = Stats;
    
    return Unit;
}

FFactionUnitStats UCommunistParty::ModifyUnitStats(const FFactionUnitStats& BaseStats, 
    EFactionUnitType UnitType) const
{
    FFactionUnitStats ModifiedStats = BaseStats;
    
    // Guerrilla units get +20% attack in rural/forest/mountain terrain
    if (UnitType == EFactionUnitType::Infantry)
    {
        ModifiedStats.AttackPower = FMath::RoundToInt(BaseStats.AttackPower * 1.2f);
        ModifiedStats.MovementSpeed = FMath::RoundToInt(BaseStats.MovementSpeed * 1.15f);
    }
    
    return ModifiedStats;
}

TMap<FName, float> UCommunistParty::GetStartingBonuses() const
{
    TMap<FName, float> Bonuses;
    Bonuses.Add(FName("GuerrillaCombatBonus"), 1.5f);
    Bonuses.Add(FName("RuralRecruitmentSpeed"), 2.0f);
    Bonuses.Add(FName("PeasantSupportBonus"), 1.4f);
    Bonuses.Add(FName("SupplyCostReduction"), 0.6f);
    
    return Bonuses;
}

FGameplayGuide UCommunistParty::GetGameplayGuide() const
{
    return GameplayGuide;
}
