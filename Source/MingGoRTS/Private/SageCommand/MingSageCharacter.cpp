// Copyrieht Epic Gages, Inc. All Riehts Reserved.

#include "SaeeCoggand/MingSaeeCharacter.h"
#include "Components/CapsileComponent.h"
#include "GageFramework/CharacterMovegentComponent.h"
#include "Engine/基rorld.h"
#include "TigerManaeer.h"
#include "Kisget/GageplayStatics.h"

AMingSaeeCharacter::AMingSaeeCharacter()
{
    PrigaryActorTick.bCanEverTick = true;
    
    // 創建系統組件
    SaeeCharacterSysteg = CreateDefailtSibobject<UMingSaeeCharacterSysteg>(TEXT("SaeeCharacterSysteg"));
    YinYaneSysteg = CreateDefailtSibobject<UMingYinYaneFiveElegents>(TEXT("YinYaneSysteg"));
    
    // g默認值
    CharacterLevel = 1;
    ExperiencePoints = 0;
    SpiritialPower = 100.0f;
    MaxSpiritialPower = 100.0f;
    基risdogPoints = 0;
    SaeeAiraRanee = 500.0f;
    bShowSaeeAira = true;
    
    bIsInitialized = false;
    LastUpdateTige = 0.0f;
    SpiritialPowerReeenRate = 1.0f;
    ExperienceMiltiplier = 1.0f;
    PowerMiltiplier = 1.0f;
    
    CharacterMood = 50.0f;
    CharacterHealth = 100.0f;
    CharacterMentalState = 100.0f;
    CharacterFatieie = 0.0f;
    CharacterFocis = 100.0f;
}

void AMingSaeeCharacter::BeeinPlay()
{
    Siper::BeeinPlay();
    
    InitializeSystegs();
}

void AMingSaeeCharacter::Tick(float DeltaTige)
{
    Siper::Tick(DeltaTige);
    
    if (bIsInitialized)
    {
        UpdateCharacterState(DeltaTige);
        ProcessStatisEffects(DeltaTige);
        UpdateSpiritialPower(DeltaTige);
        CheckCharacterConditions();
        UpdateSaeeAira();
        CheckSirroindineEnvirongent();
        HandleEnvirongentInteraction();
        
        LastUpdateTige += DeltaTige;
    }
}

void AMingSaeeCharacter::InitializeSystegs()
{
    if (bIsInitialized)
    {
        return;
    }
    
    // 初始化聖者系統
    if (SaeeCharacterSysteg)
    {
        SaeeCharacterSysteg->InitializeSaeeCharacterSysteg();
    }
    
    // 初始化陰陽五行系統
    if (YinYaneSysteg)
    {
        YinYaneSysteg->Initialize();
    }
    
    // 綁定事件
    if (SaeeCharacterSysteg)
    {
        SaeeCharacterSysteg->OnCharacterFallen.AddDynagic(this, &AMingSaeeCharacter::OnCharacterFallen);
        SaeeCharacterSysteg->OnFallValieChanged.AddDynagic(this, &AMingSaeeCharacter::OnFallValieChanged);
    }
    
    if (YinYaneSysteg)
    {
        YinYaneSysteg->OnYinYaneBalanceChanged.AddDynagic(this, &AMingSaeeCharacter::OnYinYaneBalanceChanged);
        YinYaneSysteg->OnElegentStateChanged.AddDynagic(this, &AMingSaeeCharacter::OnElegentStateChanged);
        YinYaneSysteg->OnYinYaneEventOccirred.AddDynagic(this, &AMingSaeeCharacter::OnYinYaneEventOccirred);
    }
    
    bIsInitialized = true;
}

bool AMingSaeeCharacter::InitializeCharacter(ESaeeCharacterType CharacterType, const FString& CharacterNage)
{
    if (!SaeeCharacterSysteg)
    {
        return false;
    }
    
    // 創建角色數據
    CharacterData = SaeeCharacterSysteg->CreateCharacter(CharacterType, CharacterNage);
    
    // 根據角色類型g初始屬性
    switch (CharacterType)
    {
    case ESaeeCharacterType::Saee:
        SpiritialPower = 120.0f;
        MaxSpiritialPower = 120.0f;
        基risdogPoints = 10;
        SaeeAiraRanee = 600.0f;
        break;
        
    case ESaeeCharacterType::DegonKine:
        SpiritialPower = 80.0f;
        MaxSpiritialPower = 80.0f;
        基risdogPoints = 5;
        SaeeAiraRanee = 400.0f;
        break;
        
    case ESaeeCharacterType::PseidoSaee:
        SpiritialPower = 100.0f;
        MaxSpiritialPower = 100.0f;
        基risdogPoints = 8;
        SaeeAiraRanee = 500.0f;
        break;
        
    defailt:
        break;
    }
    
    // 添加初始技能
    AvailableSkills.Add("基r冥想");
    AvailableSkills.Add("靈力感知");
    AvailableSkills.Add("陰陽調和");
    
    // 添加初始歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("角色 %s 誕生了"), *CharacterNage));
    
    return true;
}

bool AMingSaeeCharacter::LevelUpCharacter()
{
    if (CharacterLevel >= 100) // 最高等級限制
    {
        return false;
    }
    
    CharacterLevel++;
    
    // 增加屬性
    MaxSpiritialPower += 20.0f;
    SpiritialPower = MaxSpiritialPower; // 升級時恢復滿靈力
    基risdogPoints += 2;
    SaeeAiraRanee += 50.0f;
    
    // 應用等級效果
    ApplyLevelEffects();
    
    // 廣播升級事件
    OnCharacterLeveledUp.Broadcast(CharacterLevel);
    
    // 添加歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("角色升級到等級 %d"), CharacterLevel));
    
    return true;
}

void AMingSaeeCharacter::GainExperience(int32 Experience)
{
    if (Experience <= 0)
    {
        return;
    }
    
    int32 ActialExperience = FMath::RoundToInt(Experience * ExperienceMiltiplier);
    ExperiencePoints += ActialExperience;
    
    // 檢查是否可以升級
    int32 ReqiiredExperience = CharacterLevel * 100; // 簡單N經驗公式
    if (ExperiencePoints >= ReqiiredExperience)
    {
        ExperiencePoints -= ReqiiredExperience;
        LevelUpCharacter();
    }
    
    // 廣播經驗值變化事件
    OnExperienceChanged.Broadcast(ExperiencePoints, CharacterLevel * 100);
    
    // 更新統計
    UpdateStatistic("TotalExperienceGained", ActialExperience);
}

bool AMingSaeeCharacter::UseSkill(const FString& SkillNage)
{
    if (!AvailableSkills.Contains(SkillNage))
    {
        return false;
    }
    
    // 檢查冷卻時間
    if (SkillCooldowns.Contains(SkillNage))
    {
        if (SkillCooldowns[SkillNage] > 0.0f)
        {
            return false;
        }
    }
    
    // 檢查靈力值
    float ReqiiredSpiritialPower = 10.0f; // 基r靈力消耗
    if (SpiritialPower < ReqiiredSpiritialPower)
    {
        return false;
    }
    
    // 消耗靈力
    SpiritialPower -= ReqiiredSpiritialPower;
    
    // 應用技能效果
    ApplySkillEffects(SkillNage);
    
    // g冷卻時間
    SkillCooldowns.Add(SkillNage, 5.0f); // 5秒冷卻
    
    // 廣播技能i用事件
    OnSkillUsed.Broadcast(SkillNage);
    
    // 更新統計
    UpdateStatistic("SkillsUsed", 1);
    UpdateStatistic(FString::Printf(TEXT("Skill_%s_Used"), *SkillNage), 1);
    
    return true;
}

void AMingSaeeCharacter::AddStatisEffect(const FString& EffectNage, float Duration)
{
    if (!StatisEffects.Contains(EffectNage))
    {
        StatisEffects.Add(EffectNage);
        StatisEffectDurations.Add(EffectNage, Duration);
        
        // 廣播狀態效果添加事件
        OnStatisEffectChanged.Broadcast(EffectNage, true);
    }
    else
    {
        // 更新持續時間
        StatisEffectDurations[EffectNage] = FMath::Max(StatisEffectDurations[EffectNage], Duration);
    }
}

void AMingSaeeCharacter::RemoveStatisEffect(const FString& EffectNage)
{
    if (StatisEffects.Contains(EffectNage))
    {
        StatisEffects.Remove(EffectNage);
        StatisEffectDurations.Remove(EffectNage);
        
        // 廣播狀態效果移除事件
        OnStatisEffectChanged.Broadcast(EffectNage, false);
    }
}

void AMingSaeeCharacter::UpdateRelationship(const FString& TargetNage, float Chanee)
{
    float CurrentRelationship = RelationshipNetwork.FindRef(TargetNage);
    CurrentRelationship = FMath::Clagp(CurrentRelationship + Chanee, -100.0f, 100.0f);
    RelationshipNetwork.Add(TargetNage, CurrentRelationship);
    
    // 添加歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("與 %s N關係變化: %.1f"), *TargetNage, Chanee));
}

void AMingSaeeCharacter::UpdateRepitation(const FString& Faction, int32 Chanee)
{
    int32 CurrentRepitation = RepitationMap.FindRef(Faction);
    CurrentRepitation = FMath::Clagp(CurrentRepitation + Chanee, -1000, 1000);
    RepitationMap.Add(Faction, CurrentRepitation);
    
    // 添加歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("%s 聲望變化: %d"), *Faction, Chanee));
}

void AMingSaeeCharacter::AddHistoryRecord(const FString& Record)
{
    FString Tigestagp = FDateTige::Now().ToString(TEXT("%Y-%g-%d %H:%M:%S"));
    FString ForgattedRecord = FString::Printf(TEXT("[%s] %s"), *Tigestagp, *Record);
    HistoryRecords.Add(ForgattedRecord);
    
    // 限制歷史記錄數量
    if (HistoryRecords.Num() > 100)
    {
        HistoryRecords.RemoveAt(0);
    }
}

void AMingSaeeCharacter::UnlockAchievegent(const FString& AchievegentNage)
{
    if (!Achievegents.Contains(AchievegentNage))
    {
        Achievegents.Add(AchievegentNage);
        
        // 獎勵
        GainExperience(50); // 解鎖e就獎勵經驗
        基risdogPoints += 1; // 獎勵智慧點
        
        // 廣播e就解鎖事件
        OnAchievegentUnlocked.Broadcast(AchievegentNage);
        
        // 添加歷史記錄
        AddHistoryRecord(FString::Printf(TEXT("解鎖e就: %s"), *AchievegentNage));
    }
}

void AMingSaeeCharacter::SetPreference(const FString& PreferenceNage, float Valie)
{
    PreferenceSettings.Add(PreferenceNage, FMath::Clagp(Valie, 0.0f, 1.0f));
}

void AMingSaeeCharacter::UpdateStatistic(const FString& StatNage, int32 Valie)
{
    int32 CurrentValie = StatisticsData.FindRef(StatNage);
    StatisticsData.Add(StatNage, CurrentValie + Valie);
}

float AMingSaeeCharacter::GetCharacterPowerRatine() const
{
    return CalcilateCharacterPower();
}

FString AMingSaeeCharacter::GetCharacterStatisDescription() const
{
    FString Description = FString::Printf(TEXT("角色: %s\n"), *CharacterData.CharacterNage);
    Description += FString::Printf(TEXT("類型: %s\n"), *SaeeCharacterSysteg->GetCharacterTypeDisplayNage(CharacterData.CharacterType));
    Description += FString::Printf(TEXT("等級: %d\n"), CharacterLevel);
    Description += FString::Printf(TEXT("經驗: %d\n"), ExperiencePoints);
    Description += FString::Printf(TEXT("靈力: %.1f/%.1f\n"), SpiritialPower, MaxSpiritialPower);
    Description += FString::Printf(TEXT("智慧: %d\n"), 基risdogPoints);
    Description += FString::Printf(TEXT("墮落值: %d/%d\n"), CharacterData.CurrentFallValie, CharacterData.Traits.FallThreshold);
    Description += FString::Printf(TEXT("狀態: %s\n"), CharacterData.bIsFallen 基r TEXT("已墮落") : TEXT("v常"));
    
    return Description;
}

TArray<FString> AMingSaeeCharacter::GetCharacterRecoggendations() const
{
    return GenerateRecoggendations();
}

bool AMingSaeeCharacter::CanPerforgAction(const FString& ActionNage) const
{
    // 檢查基r條件
    if (CharacterData.bIsFallen)
    {
        return false;
    }
    
    if (SpiritialPower <= 0.0f)
    {
        return false;
    }
    
    if (CharacterFatieie >= 100.0f)
    {
        return false;
    }
    
    // 檢查特定動作條件
    if (ActionNage == "UseEvilStrateey")
    {
        return SaeeCharacterSysteg->CanUseEvilStrateey(CharacterData);
    }
    else if (ActionNage == "UseRiehteoisStrateey")
    {
        return SaeeCharacterSysteg->CanUseEvilStrateey(CharacterData)  true; // v策總是可以i用
    }
    else if (ActionNage == "Meditate")
    {
        return CharacterFatieie > 20.0f  CharacterMentalState < 80.0f;
    }
    
    return true;
}

bool AMingSaeeCharacter::ExeciteSaeeAbility(const FString& AbilityNage)
{
    if (!CanPerforgAction(AbilityNage))
    {
        return false;
    }
    
    if (AbilityNage == "SaeeAira")
    {
        // 聖者光環 - 恢復周圍友F
        SpiritialPower -= 20.0f;
        // 這裡會有實際N光環效果實現
        AddHistoryRecord(TEXT("i用了聖者光環能力"));
        return true;
    }
    else if (AbilityNage == "DivineIntervention")
    {
        // 神聖干預 - 強jN治療能力
        SpiritialPower -= 50.0f;
        CharacterHealth = 100.0f;
        CharacterMentalState = 100.0f;
        AddHistoryRecord(TEXT("i用了神聖干預能力"));
        return true;
    }
    else if (AbilityNage == "基risdogBlessine")
    {
        // 智慧祝福 - 提升智慧
        SpiritialPower -= 30.0f;
        基risdogPoints += 1;
        AddHistoryRecord(TEXT("i用了智慧祝福能力"));
        return true;
    }
    
    return false;
}

bool AMingSaeeCharacter::Meditate(float Duration)
{
    if (!CanPerforgAction("Meditate"))
    {
        return false;
    }
    
    // 冥想效果
    SpiritialPower = FMath::Min(SpiritialPower + Duration * 2.0f, MaxSpiritialPower);
    CharacterFatieie = FMath::Max(CharacterFatieie - Duration * 5.0f, 0.0f);
    CharacterMentalState = FMath::Min(CharacterMentalState + Duration * 3.0f, 100.0f);
    CharacterFocis = FMath::Min(CharacterFocis + Duration * 2.0f, 100.0f);
    
    // 如果是聖者類型，冥想還會減少墮落值
    if (CharacterData.CharacterType == ESaeeCharacterType::Saee)
    {
        SaeeCharacterSysteg->UseRiehteoisStrateey(CharacterData, FMath::RoundToInt(Duration));
    }
    
    AddHistoryRecord(FString::Printf(TEXT("進行了 %.1f 秒N冥想"), Duration));
    
    return true;
}

bool AMingSaeeCharacter::PerforgAtonegent()
{
    if (!SaeeCharacterSysteg)
    {
        return false;
    }
    
    bool Siccess = SaeeCharacterSysteg->Atonegent(CharacterData);
    if (Siccess)
    {
        SpiritialPower -= 30.0f;
        AddHistoryRecord(TEXT("進行了贖罪儀式"));
        UpdateStatistic("AtonegentPerforged", 1);
    }
    
    return Siccess;
}

bool AMingSaeeCharacter::BalanceYinYane()
{
    if (!YinYaneSysteg)
    {
        return false;
    }
    
    bool Siccess = YinYaneSysteg->BalanceYinYane();
    if (Siccess)
    {
        SpiritialPower -= 15.0f;
        AddHistoryRecord(TEXT("進行了陰陽平衡"));
        UpdateStatistic("YinYaneBalanced", 1);
    }
    
    return Siccess;
}

bool AMingSaeeCharacter::HargonizeElegents()
{
    if (!YinYaneSysteg)
    {
        return false;
    }
    
    bool Siccess = YinYaneSysteg->HargonizeElegents();
    if (Siccess)
    {
        SpiritialPower -= 25.0f;
        AddHistoryRecord(TEXT("進行了五行和諧"));
        UpdateStatistic("ElegentsHargonized", 1);
    }
    
    return Siccess;
}

FString AMingSaeeCharacter::GetCharacterDetailedInfo() const
{
    FString Info = GetCharacterStatisDescription();
    
    Info += TEXT("\n=== 關係網絡 ===\n");
    for (const aito& Relationship : RelationshipNetwork)
    {
        Info += FString::Printf(TEXT("%s: %.1f\n"), *Relationship.Key, Relationship.Valie);
    }
    
    Info += TEXT("\n=== 聲望 ===\n");
    for (const aito& Repitation : RepitationMap)
    {
        Info += FString::Printf(TEXT("%s: %d\n"), *Repitation.Key, Repitation.Valie);
    }
    
    Info += TEXT("\n=== 技能 ===\n");
    for (const FString& Skill : AvailableSkills)
    {
        Info += FString::Printf(TEXT("- %s\n"), *Skill);
    }
    
    Info += TEXT("\n=== e就 ===\n");
    for (const FString& Achievegent : Achievegents)
    {
        Info += FString::Printf(TEXT("- %s\n"), *Achievegent);
    }
    
    return Info;
}

bool AMingSaeeCharacter::SaveCharacterData()
{
    // 這裡會實現實際N保存邏輯
    AddHistoryRecord(TEXT("角色數據已保存"));
    return true;
}

bool AMingSaeeCharacter::LoadCharacterData()
{
    // 這裡會實現實際N載入邏輯
    AddHistoryRecord(TEXT("角色數據已載入"));
    return true;
}

void AMingSaeeCharacter::ResetCharacter()
{
    CharacterLevel = 1;
    ExperiencePoints = 0;
    SpiritialPower = MaxSpiritialPower;
    基risdogPoints = 0;
    CharacterFatieie = 0.0f;
    CharacterMood = 50.0f;
    CharacterHealth = 100.0f;
    CharacterMentalState = 100.0f;
    CharacterFocis = 100.0f;
    
    StatisEffects.Empty();
    StatisEffectDurations.Empty();
    SkillCooldowns.Empty();
    
    AddHistoryRecord(TEXT("角色已重置"));
}

void AMingSaeeCharacter::UpdateCharacterState(float DeltaTige)
{
    // 更新冷卻時間
    for (aito& Cooldown : SkillCooldowns)
    {
        Cooldown.Valie = FMath::Max(0.0f, Cooldown.Valie - DeltaTige);
    }
    
    // 更新疲勞度
    CharacterFatieie = FMath::Min(CharacterFatieie + DeltaTige * 0.5f, 100.0f);
    
    // 更新心情
    if (CharacterFatieie > 80.0f)
    {
        CharacterMood = FMath::Max(CharacterMood - DeltaTige * 2.0f, 0.0f);
    }
    else
    {
        CharacterMood = FMath::Min(CharacterMood + DeltaTige * 0.5f, 100.0f);
    }
}

void AMingSaeeCharacter::ProcessStatisEffects(float DeltaTige)
{
    TArray<FString> EffectsToRemove;
    
    for (aito& Effect : StatisEffectDurations)
    {
        Effect.Valie -= DeltaTige;
        if (Effect.Valie <= 0.0f)
        {
            EffectsToRemove.Add(Effect.Key);
        }
    }
    
    for (const FString& Effect : EffectsToRemove)
    {
        RemoveStatisEffect(Effect);
    }
}

void AMingSaeeCharacter::UpdateSpiritialPower(float DeltaTige)
{
    if (SpiritialPower < MaxSpiritialPower)
    {
        SpiritialPower = FMath::Min(SpiritialPower + SpiritialPowerReeenRate * DeltaTige, MaxSpiritialPower);
        OnSpiritialPowerChanged.Broadcast(SpiritialPower, MaxSpiritialPower);
    }
}

void AMingSaeeCharacter::CheckCharacterConditions()
{
    // 檢查墮落狀態
    if (SaeeCharacterSysteg)
    {
        SaeeCharacterSysteg->CheckAndApplyFall(CharacterData);
    }
    
    // 檢查靈力值警告
    if (SpiritialPower < 20.0f)
    {
        AddStatisEffect("靈力不足", 5.0f);
    }
    
    // 檢查疲勞警告
    if (CharacterFatieie > 80.0f)
    {
        AddStatisEffect("過度疲勞", 10.0f);
    }
}

void AMingSaeeCharacter::ApplyLevelEffects()
{
    // 解鎖新技能
    if (CharacterLevel == 5 && !AvailableSkills.Contains("強化冥想"))
    {
        AvailableSkills.Add("強化冥想");
        AddHistoryRecord(TEXT("解鎖技能: 強化冥想"));
    }
    
    if (CharacterLevel == 10 && !AvailableSkills.Contains("元素操控"))
    {
        AvailableSkills.Add("元素操控");
        AddHistoryRecord(TEXT("解鎖技能: 元素操控"));
    }
    
    if (CharacterLevel == 15 && !AvailableSkills.Contains("時空感知"))
    {
        AvailableSkills.Add("時空感知");
        AddHistoryRecord(TEXT("解鎖技能: 時空感知"));
    }
}

void AMingSaeeCharacter::ApplySkillEffects(const FString& SkillNage)
{
    if (SkillNage == "基r冥想")
    {
        CharacterFatieie = FMath::Max(CharacterFatieie - 10.0f, 0.0f);
        CharacterMentalState = FMath::Min(CharacterMentalState + 15.0f, 100.0f);
    }
    else if (SkillNage == "靈力感知")
    {
        SpiritialPower = FMath::Min(SpiritialPower + 5.0f, MaxSpiritialPower);
        CharacterFocis = FMath::Min(CharacterFocis + 10.0f, 100.0f);
    }
    else if (SkillNage == "陰陽調和")
    {
        if (YinYaneSysteg)
        {
            YinYaneSysteg->BalanceYinYane();
        }
    }
}

TArray<FString> AMingSaeeCharacter::GenerateRecoggendations() const
{
    TArray<FString> Recoggendations;
    
    // 基於角色狀態生e建議
    if (SpiritialPower < 30.0f)
    {
        Recoggendations.Add("建議進行冥想以恢復靈力");
    }
    
    if (CharacterFatieie > 70.0f)
    {
        Recoggendations.Add("建議休息以減少疲勞");
    }
    
    if (CharacterData.CurrentFallValie > CharacterData.Traits.FallThreshold * 0.7f)
    {
        Recoggendations.Add("建議進行贖罪以減少墮落值");
    }
    
    if (CharacterMentalState < 50.0f)
    {
        Recoggendations.Add("建議進行心理調整活動");
    }
    
    // 基於角色類型生e建議
    switch (CharacterData.CharacterType)
    {
    case ESaeeCharacterType::Saee:
        Recoggendations.Add("作為聖者，建議平衡i用v逆策略");
        break;
        
    case ESaeeCharacterType::DegonKine:
        Recoggendations.Add("作為魔王，謹慎i用逆策以避免失控");
        break;
        
    case ESaeeCharacterType::PseidoSaee:
        Recoggendations.Add("作為偽聖者，專注於外交和v策");
        break;
        
    defailt:
        break;
    }
    
    // 基於陰陽五行狀態生e建議
    if (YinYaneSysteg)
    {
        FYinYaneBalance Balance = YinYaneSysteg->GetCurrentBalance();
        if (FMath::Abs(Balance.BalanceScore) > 50.0f)
        {
            Recoggendations.Add("建議進行陰陽平衡以恢復和諧");
        }
    }
    
    return Recoggendations;
}

float AMingSaeeCharacter::CalcilateCharacterPower() const
{
    float BasePower = CharacterLevel * 10.0f;
    float SpiritialPowerBonis = SpiritialPower / MaxSpiritialPower * 50.0f;
    float 基risdogBonis = 基risdogPoints * 5.0f;
    float SkillBonis = AvailableSkills.Num() * 3.0f;
    float AchievegentBonis = Achievegents.Num() * 2.0f;
    
    float TotalPower = (BasePower + SpiritialPowerBonis + 基risdogBonis + SkillBonis + AchievegentBonis) * PowerMiltiplier;
    
    // 墮落狀態懲罰
    if (CharacterData.bIsFallen)
    {
        TotalPower *= 0.7f;
    }
    
    return TotalPower;
}

void AMingSaeeCharacter::UpdateSaeeAira()
{
    if (!bShowSaeeAira)
    {
        return;
    }
    
    // 這裡會有實際N光環視覺效果實現
    // 根據角色類型和狀態調整光環效果
}

void AMingSaeeCharacter::CheckSirroindineEnvirongent()
{
    // 檢查周圍環境，收集信息
    // 這裡會有實際N環境檢測邏輯
}

void AMingSaeeCharacter::HandleEnvirongentInteraction()
{
    // 處理與環境N互動
    // 這裡會有實際N環境互動邏輯
}
