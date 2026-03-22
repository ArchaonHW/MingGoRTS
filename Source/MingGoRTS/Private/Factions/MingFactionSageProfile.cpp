// Copyright Epic Games, Inc. All Rights Reserved.

#include "Factions/MingFactionSageProfile.h"

UMingFactionSageManager::UMingFactionSageManager()
    : PlayerFaction(ERepublicFaction::BeiyangGovernment)
    , bIsInitialized(false)
{
}

void UMingFactionSageManager::InitializeFactionSageManager()
{
    if (bIsInitialized)
    {
        return;
    }

    InitializeDefaultFactionProfiles();
    bIsInitialized = true;
}

void UMingFactionSageManager::InitializeDefaultFactionProfiles()
{
    // 初始化所有12勢力的預設配置
    RegisterFactionProfile(CreateBeiyangProfile());
    RegisterFactionProfile(CreateNationalistProfile());
    RegisterFactionProfile(CreateCommunistProfile());
    RegisterFactionProfile(CreateFengtianProfile());
    RegisterFactionProfile(CreateZhiliProfile());
    RegisterFactionProfile(CreateAnhuiProfile());
    RegisterFactionProfile(CreateShanxiProfile());
    RegisterFactionProfile(CreateGuangxiProfile());
    RegisterFactionProfile(CreateYunnanProfile());
    RegisterFactionProfile(CreateSichuanProfile());
    RegisterFactionProfile(CreateMaFamilyProfile());
    RegisterFactionProfile(CreateXinjiangProfile());
}

FFactionSageProfile UMingFactionSageManager::CreateBeiyangProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::BeiyangGovernment;
    Profile.DefaultCharacterType = ESageCharacterType::PseudoSage;
    Profile.Philosophy = EFactionCommandPhilosophy::Deceptive;
    Profile.FallThresholdModifier = -20; // 80 (偽聖者較低閾值)
    Profile.FactionDescription = TEXT("北洋政府：中央正統，外交優勢，但內部派系林立");
    Profile.PhilosophyDescription = TEXT("正統性偽裝：表面使用正策維護秩序，暗地裡頻繁使用逆策");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.3f; // 正統光環
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.0f;
    Profile.StrategyModifier.EvilFallValueModifier = 1.0f;

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 2; // 土
    Profile.WuXingAffinity.SecondaryElement = 4; // 水

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("正統恢復");
    Profile.AtonementConfig.AtonementDescription = TEXT("公開道歉，恢復議會運作，重建中央正統性");
    Profile.AtonementConfig.AtonementTargetType = 1; // 政治目標
    Profile.AtonementConfig.RewardType = 0; // 士氣
    Profile.AtonementConfig.RewardValue = 30.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateNationalistProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::NationalistGovernment;
    Profile.DefaultCharacterType = ESageCharacterType::Sage;
    Profile.Philosophy = EFactionCommandPhilosophy::Balanced;
    Profile.FallThresholdModifier = 50; // 150 (聖者標準閾值)
    Profile.FactionDescription = TEXT("國民政府：民族主義，現代化，革命正統");
    Profile.PhilosophyDescription = TEXT("正逆平衡：能夠在正逆策略間靈活切換而不受懲罰");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.25f; // 三民主義
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.0f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.5f; // 切換至逆策時無墮落值懲罰

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 1; // 火
    Profile.WuXingAffinity.SecondaryElement = 0; // 木

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("革命正統");
    Profile.AtonementConfig.AtonementDescription = TEXT("重新發動北伐，統一全國，建立革命正統");
    Profile.AtonementConfig.AtonementTargetType = 0; // 軍事勝利
    Profile.AtonementConfig.RewardType = 0; // 士氣
    Profile.AtonementConfig.RewardValue = 50.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateCommunistProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::CommunistParty;
    Profile.DefaultCharacterType = ESageCharacterType::Sage;
    Profile.Philosophy = EFactionCommandPhilosophy::RighteousPrimary;
    Profile.FallThresholdModifier = 50; // 150
    Profile.FactionDescription = TEXT("中國共產黨：人民戰爭，游擊戰，群眾基礎");
    Profile.PhilosophyDescription = TEXT("人民戰爭：以正策為核心，依靠人民支持");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.4f; // 群眾基礎
    Profile.StrategyModifier.EvilStrategyMultiplier = 0.8f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.5f; // 游擊戰時墮落值減半

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 4; // 水
    Profile.WuXingAffinity.SecondaryElement = 0; // 木

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("群眾路線");
    Profile.AtonementConfig.AtonementDescription = TEXT("深入農村發動群眾，建立根據地");
    Profile.AtonementConfig.AtonementTargetType = 1; // 政治目標
    Profile.AtonementConfig.RewardType = 1; // 徵兵
    Profile.AtonementConfig.RewardValue = 100.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateFengtianProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::FengtianClique;
    Profile.DefaultCharacterType = ESageCharacterType::DemonKing;
    Profile.Philosophy = EFactionCommandPhilosophy::EvilOnly;
    Profile.FallThresholdModifier = 100; // 200 (魔王高閾值)
    Profile.FactionDescription = TEXT("奉系軍閥：東北工業，日本援助，武力至上");
    Profile.PhilosophyDescription = TEXT("東北虎王：以武力為尊，頻繁使用逆策，無法停止");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 0.5f; // 幾乎不使用正策
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.6f; // 逆策效果+60%
    Profile.StrategyModifier.EvilCooldownModifier = 0.7f; // 逆策冷卻-30%
    Profile.StrategyModifier.EvilFallValueModifier = 1.0f;

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 3; // 金
    Profile.WuXingAffinity.SecondaryElement = 1; // 火

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("東北收復");
    Profile.AtonementConfig.AtonementDescription = TEXT("收復東北失地，驅逐日本勢力");
    Profile.AtonementConfig.AtonementTargetType = 0; // 軍事勝利
    Profile.AtonementConfig.RewardType = 2; // 威望
    Profile.AtonementConfig.RewardValue = 100.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateZhiliProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::ZhiliClique;
    Profile.DefaultCharacterType = ESageCharacterType::DemonKing;
    Profile.Philosophy = EFactionCommandPhilosophy::EvilPrimary;
    Profile.FallThresholdModifier = 100; // 200
    Profile.FactionDescription = TEXT("直系軍閥：中原霸主，兵力龐大，速戰速決");
    Profile.PhilosophyDescription = TEXT("中原霸主：依靠兵力優勢碾壓對手");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 0.7f;
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.5f;
    Profile.StrategyModifier.EvilCooldownModifier = 0.7f; // 逆策冷卻-30%
    Profile.StrategyModifier.EvilFallValueModifier = 0.8f;

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 3; // 金
    Profile.WuXingAffinity.SecondaryElement = 2; // 土

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("統一中原");
    Profile.AtonementConfig.AtonementDescription = TEXT("擊敗其他中原勢力，統一中原地區");
    Profile.AtonementConfig.AtonementTargetType = 0; // 軍事勝利
    Profile.AtonementConfig.RewardType = 1; // 兵力上限
    Profile.AtonementConfig.RewardValue = 30.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateAnhuiProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::AnhuiClique;
    Profile.DefaultCharacterType = ESageCharacterType::PseudoSage;
    Profile.Philosophy = EFactionCommandPhilosophy::Deceptive;
    Profile.FallThresholdModifier = -20; // 80
    Profile.FactionDescription = TEXT("皖系軍閥：政治操弄，日本援助，安福系");
    Profile.PhilosophyDescription = TEXT("政治操弄：擅長政治陰謀，表面維護正統");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.1f;
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.3f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.7f; // 墮落值積累-30%

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 4; // 水
    Profile.WuXingAffinity.SecondaryElement = 3; // 金

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("政治清明");
    Profile.AtonementConfig.AtonementDescription = TEXT("清除腐敗官員，重建政治清明");
    Profile.AtonementConfig.AtonementTargetType = 1; // 政治目標
    Profile.AtonementConfig.RewardType = 0; // 政治操作
    Profile.AtonementConfig.RewardValue = 40.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateShanxiProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::ShanxiClique;
    Profile.DefaultCharacterType = ESageCharacterType::Sage;
    Profile.Philosophy = EFactionCommandPhilosophy::RighteousPrimary;
    Profile.FallThresholdModifier = 50; // 150
    Profile.FactionDescription = TEXT("晉系軍閥：山西模範，防禦專精，教育興省");
    Profile.PhilosophyDescription = TEXT("山西模範：以正策建設為主，防守反擊");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.4f; // 立制效果+40%
    Profile.StrategyModifier.EvilStrategyMultiplier = 0.8f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.8f; // 墮落值-20%

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 2; // 土
    Profile.WuXingAffinity.SecondaryElement = 4; // 水

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("模範重建");
    Profile.AtonementConfig.AtonementDescription = TEXT("重建山西建設，恢復模範省地位");
    Profile.AtonementConfig.AtonementTargetType = 2; // 經濟建設
    Profile.AtonementConfig.RewardType = 1; // 經濟
    Profile.AtonementConfig.RewardValue = 50.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateGuangxiProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::GuangxiClique;
    Profile.DefaultCharacterType = ESageCharacterType::DemonKing;
    Profile.Philosophy = EFactionCommandPhilosophy::EvilPrimary;
    Profile.FallThresholdModifier = 100; // 200
    Profile.FactionDescription = TEXT("桂系軍閥：廣西狼兵，山地戰，北伐先鋒");
    Profile.PhilosophyDescription = TEXT("廣西狼兵：勇猛善戰，正逆策略都為了勝利");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 0.8f;
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.5f; // 狼兵兇猛
    Profile.StrategyModifier.EvilCooldownModifier = 0.8f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.9f;

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 0; // 木
    Profile.WuXingAffinity.SecondaryElement = 1; // 火

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("北伐完成");
    Profile.AtonementConfig.AtonementDescription = TEXT("完成北伐統一，實現國家統一");
    Profile.AtonementConfig.AtonementTargetType = 0; // 軍事勝利
    Profile.AtonementConfig.RewardType = 3; // 將領質量
    Profile.AtonementConfig.RewardValue = 2.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateYunnanProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::YunnanClique;
    Profile.DefaultCharacterType = ESageCharacterType::Sage;
    Profile.Philosophy = EFactionCommandPhilosophy::RighteousOnly;
    Profile.FallThresholdModifier = 50; // 150
    Profile.FactionDescription = TEXT("滇系軍閥：護國傳統，道義為先，邊疆擴張");
    Profile.PhilosophyDescription = TEXT("護國正統：打著正義旗號，道義為先");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.4f; // 護國傳統
    Profile.StrategyModifier.EvilStrategyMultiplier = 0.6f; // 極少使用逆策
    Profile.StrategyModifier.EvilFallValueModifier = 1.5f; // 使用逆策時墮落值翻倍

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 0; // 木
    Profile.WuXingAffinity.SecondaryElement = 4; // 水

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("護國再舉");
    Profile.AtonementConfig.AtonementDescription = TEXT("發動新的護國戰爭，維護共和");
    Profile.AtonementConfig.AtonementTargetType = 0; // 軍事勝利
    Profile.AtonementConfig.RewardType = 0; // 正策效果
    Profile.AtonementConfig.RewardValue = 50.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateSichuanProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::SichuanClique;
    Profile.DefaultCharacterType = ESageCharacterType::PseudoSage;
    Profile.Philosophy = EFactionCommandPhilosophy::Deceptive;
    Profile.FallThresholdModifier = -20; // 80
    Profile.FactionDescription = TEXT("川系軍閥：防區制，內部統一，派系林立");
    Profile.PhilosophyDescription = TEXT("防區割據：表面統一，實際割據");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.0f;
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.2f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.5f; // 對內使用逆策無墮落值

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 2; // 土
    Profile.WuXingAffinity.SecondaryElement = 0; // 木

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("四川統一");
    Profile.AtonementConfig.AtonementDescription = TEXT("統一四川全境，結束防區割據");
    Profile.AtonementConfig.AtonementTargetType = 1; // 政治目標
    Profile.AtonementConfig.RewardType = 1; // 防區產出
    Profile.AtonementConfig.RewardValue = 40.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateMaFamilyProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::MaFamily;
    Profile.DefaultCharacterType = ESageCharacterType::DemonKing;
    Profile.Philosophy = EFactionCommandPhilosophy::EvilOnly;
    Profile.FallThresholdModifier = 100; // 200
    Profile.FactionDescription = TEXT("馬家軍：回族騎兵，宗教團結，西北霸主");
    Profile.PhilosophyDescription = TEXT("宗教狂熱：為信仰而戰，逆策效果強大");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 0.6f; // 宗教性質的正策
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.5f; // 清真鐵騎
    Profile.StrategyModifier.EvilCooldownModifier = 0.8f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.0f; // 對異教徒使用無墮落值

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 1; // 火
    Profile.WuXingAffinity.SecondaryElement = 3; // 金

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("宗教和解");
    Profile.AtonementConfig.AtonementDescription = TEXT("與其他宗教勢力和解，建立宗教和平");
    Profile.AtonementConfig.AtonementTargetType = 3; // 外交成就
    Profile.AtonementConfig.RewardType = 0; // 騎兵攻擊
    Profile.AtonementConfig.RewardValue = 30.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::CreateXinjiangProfile() const
{
    FFactionSageProfile Profile;
    Profile.Faction = ERepublicFaction::XinjiangForces;
    Profile.DefaultCharacterType = ESageCharacterType::PseudoSage;
    Profile.Philosophy = EFactionCommandPhilosophy::Deceptive;
    Profile.FallThresholdModifier = -20; // 80
    Profile.FactionDescription = TEXT("新疆勢力：邊疆要塞，民族複雜，蘇聯援助");
    Profile.PhilosophyDescription = TEXT("邊疆要塞：維持表面平衡才能生存");

    // 策略修正
    Profile.StrategyModifier.RighteousStrategyMultiplier = 1.3f; // 維持民族平衡
    Profile.StrategyModifier.EvilStrategyMultiplier = 1.2f;
    Profile.StrategyModifier.EvilFallValueModifier = 0.5f; // 接受蘇聯援助時無視墮落值

    // 五行偏好
    Profile.WuXingAffinity.PrimaryElement = 4; // 水
    Profile.WuXingAffinity.SecondaryElement = 2; // 土

    // 贖罪任務
    Profile.AtonementConfig.FactionAtonementTaskName = TEXT("民族和諧");
    Profile.AtonementConfig.AtonementDescription = TEXT("平衡各民族關係，維持邊疆穩定");
    Profile.AtonementConfig.AtonementTargetType = 3; // 外交成就
    Profile.AtonementConfig.RewardType = 1; // 穩定性
    Profile.AtonementConfig.RewardValue = 50.0f;

    return Profile;
}

FFactionSageProfile UMingFactionSageManager::GetFactionProfile(ERepublicFaction Faction) const
{
    if (FactionProfiles.Contains(Faction))
    {
        return FactionProfiles[Faction];
    }
    return FFactionSageProfile();
}

void UMingFactionSageManager::SetPlayerFaction(ERepublicFaction Faction)
{
    PlayerFaction = Faction;
}

ESageCharacterType UMingFactionSageManager::GetPlayerCharacterType() const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);
    return Profile.DefaultCharacterType;
}

int32 UMingFactionSageManager::GetPlayerFallThreshold() const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);
    // 基礎墮落閾值根據角色類型
    int32 BaseThreshold = 100;
    switch (Profile.DefaultCharacterType)
    {
    case ESageCharacterType::Sage:
        BaseThreshold = 150;
        break;
    case ESageCharacterType::DemonKing:
        BaseThreshold = 200;
        break;
    case ESageCharacterType::PseudoSage:
        BaseThreshold = 80;
        break;
    default:
        BaseThreshold = 100;
        break;
    }
    return BaseThreshold + Profile.FallThresholdModifier;
}

float UMingFactionSageManager::CalculateStrategyEffectModifier(ESixStrategyType Strategy, bool bIsEvil) const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);

    if (bIsEvil)
    {
        return Profile.StrategyModifier.EvilStrategyMultiplier;
    }
    else
    {
        return Profile.StrategyModifier.RighteousStrategyMultiplier;
    }
}

float UMingFactionSageManager::CalculateWuXingPhaseBonus(int32 CurrentPhase) const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);

    if (IsFavorableWuXingPhase(CurrentPhase))
    {
        return Profile.WuXingAffinity.FavorablePhaseBonus;
    }
    else
    {
        return -Profile.WuXingAffinity.FavorablePhaseBonus * 0.5f;
    }
}

bool UMingFactionSageManager::IsFavorableWuXingPhase(int32 CurrentPhase) const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);
    return CurrentPhase == Profile.WuXingAffinity.PrimaryElement || 
           CurrentPhase == Profile.WuXingAffinity.SecondaryElement;
}

FString UMingFactionSageManager::GetFactionCommandAdvice() const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);
    
    FString Advice = FString::Printf(TEXT("【%s】指揮建議：\n"), 
        *UEnum::GetValueAsString(PlayerFaction));
    
    Advice += FString::Printf(TEXT("指揮哲學：%s\n"), *Profile.PhilosophyDescription);
    
    switch (Profile.Philosophy)
    {
    case EFactionCommandPhilosophy::RighteousOnly:
        Advice += TEXT("建議：始終堅持正策，避免使用逆策。");
        break;
    case EFactionCommandPhilosophy::RighteousPrimary:
        Advice += TEXT("建議：以正策為主，必要時謹慎使用逆策。");
        break;
    case EFactionCommandPhilosophy::Balanced:
        Advice += TEXT("建議：靈活運用正逆策略，保持平衡。");
        break;
    case EFactionCommandPhilosophy::EvilPrimary:
        Advice += TEXT("建議：以逆策為主，但注意墮落風險。");
        break;
    case EFactionCommandPhilosophy::EvilOnly:
        Advice += TEXT("建議：無所畏懼地使用逆策，接受魔王之道。");
        break;
    case EFactionCommandPhilosophy::Deceptive:
        Advice += TEXT("建議：表面維護正統，暗地靈活運用逆策。");
        break;
    default:
        break;
    }

    // 五行建議
    TArray<FString> ElementNames = { TEXT("木(春/立名)"), TEXT("火(夏/造勢)"), 
                                      TEXT("土(長夏/收權)"), TEXT("金(秋/裁斷)"), TEXT("水(冬/養機)") };
    Advice += FString::Printf(TEXT("\n優勢五行階段：%s, %s"),
        *ElementNames[Profile.WuXingAffinity.PrimaryElement],
        *ElementNames[Profile.WuXingAffinity.SecondaryElement]);

    return Advice;
}

FString UMingFactionSageManager::GetFactionAtonementTaskName() const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);
    return Profile.AtonementConfig.FactionAtonementTaskName;
}

FString UMingFactionSageManager::GetFactionAtonementDescription() const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);
    return Profile.AtonementConfig.AtonementDescription;
}

bool UMingFactionSageManager::CanUseStrategy(ESixStrategyType Strategy, bool bIsEvil) const
{
    FFactionSageProfile Profile = GetFactionProfile(PlayerFaction);

    // 檢查角色類型限制
    if (Profile.DefaultCharacterType == ESageCharacterType::PseudoSage && bIsEvil)
    {
        // 偽聖者理論上不能用逆策，但某些勢力有特殊規則
        if (Profile.Philosophy == EFactionCommandPhilosophy::Deceptive)
        {
            return true; // 欺騙型可以暗中使用
        }
        return false;
    }

    // 檢查專屬策略限制
    if (bIsEvil)
    {
        if (Profile.StrategyModifier.ExclusiveEvilStrategies.Num() > 0 &&
            !Profile.StrategyModifier.ExclusiveEvilStrategies.Contains((int32)Strategy))
        {
            return false;
        }
    }
    else
    {
        if (Profile.StrategyModifier.ExclusiveRighteousStrategies.Num() > 0 &&
            !Profile.StrategyModifier.ExclusiveRighteousStrategies.Contains((int32)Strategy))
        {
            return false;
        }
    }

    return true;
}

TArray<ESixStrategyType> UMingFactionSageManager::GetAvailableStrategies(bool bIsEvil) const
{
    TArray<ESixStrategyType> AvailableStrategies;

    // 檢查所有策略
    for (int32 i = 0; i < (int32)ESixStrategyType::Count; ++i)
    {
        ESixStrategyType Strategy = (ESixStrategyType)i;
        if (CanUseStrategy(Strategy, bIsEvil))
        {
            AvailableStrategies.Add(Strategy);
        }
    }

    return AvailableStrategies;
}

void UMingFactionSageManager::RegisterFactionProfile(const FFactionSageProfile& Profile)
{
    if (FactionProfiles.Contains(Profile.Faction))
    {
        FactionProfiles[Profile.Faction] = Profile;
    }
    else
    {
        FactionProfiles.Add(Profile.Faction, Profile);
    }
}
