// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingSageCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AMingSageCharacter::AMingSageCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 創建系統組件
    SageCharacterSystem = CreateDefaultSubobject<UMingSageCharacterSystem>(TEXT("SageCharacterSystem"));
    YinYangSystem = CreateDefaultSubobject<UMingYinYangFiveElements>(TEXT("YinYangSystem"));
    
    // 設置默認值
    CharacterLevel = 1;
    ExperiencePoints = 0;
    SpiritualPower = 100.0f;
    MaxSpiritualPower = 100.0f;
    WisdomPoints = 0;
    SageAuraRange = 500.0f;
    bShowSageAura = true;
    
    bIsInitialized = false;
    LastUpdateTime = 0.0f;
    SpiritualPowerRegenRate = 1.0f;
    ExperienceMultiplier = 1.0f;
    PowerMultiplier = 1.0f;
    
    CharacterMood = 50.0f;
    CharacterHealth = 100.0f;
    CharacterMentalState = 100.0f;
    CharacterFatigue = 0.0f;
    CharacterFocus = 100.0f;
}

void AMingSageCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeSystems();
}

void AMingSageCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bIsInitialized)
    {
        UpdateCharacterState(DeltaTime);
        ProcessStatusEffects(DeltaTime);
        UpdateSpiritualPower(DeltaTime);
        CheckCharacterConditions();
        UpdateSageAura();
        CheckSurroundingEnvironment();
        HandleEnvironmentInteraction();
        
        LastUpdateTime += DeltaTime;
    }
}

void AMingSageCharacter::InitializeSystems()
{
    if (bIsInitialized)
    {
        return;
    }
    
    // 初始化聖者系統
    if (SageCharacterSystem)
    {
        SageCharacterSystem->InitializeSageCharacterSystem();
    }
    
    // 初始化陰陽五行系統
    if (YinYangSystem)
    {
        YinYangSystem->Initialize();
    }
    
    // 綁定事件
    if (SageCharacterSystem)
    {
        SageCharacterSystem->OnCharacterFallen.AddDynamic(this, &AMingSageCharacter::OnCharacterFallen);
        SageCharacterSystem->OnFallValueChanged.AddDynamic(this, &AMingSageCharacter::OnFallValueChanged);
    }
    
    if (YinYangSystem)
    {
        YinYangSystem->OnYinYangBalanceChanged.AddDynamic(this, &AMingSageCharacter::OnYinYangBalanceChanged);
        YinYangSystem->OnElementStateChanged.AddDynamic(this, &AMingSageCharacter::OnElementStateChanged);
        YinYangSystem->OnYinYangEventOccurred.AddDynamic(this, &AMingSageCharacter::OnYinYangEventOccurred);
    }
    
    bIsInitialized = true;
}

bool AMingSageCharacter::InitializeCharacter(ESageCharacterType CharacterType, const FString& CharacterName)
{
    if (!SageCharacterSystem)
    {
        return false;
    }
    
    // 創建角色數據
    CharacterData = SageCharacterSystem->CreateCharacter(CharacterType, CharacterName);
    
    // 根據角色類型設置初始屬性
    switch (CharacterType)
    {
    case ESageCharacterType::Sage:
        SpiritualPower = 120.0f;
        MaxSpiritualPower = 120.0f;
        WisdomPoints = 10;
        SageAuraRange = 600.0f;
        break;
        
    case ESageCharacterType::DemonKing:
        SpiritualPower = 80.0f;
        MaxSpiritualPower = 80.0f;
        WisdomPoints = 5;
        SageAuraRange = 400.0f;
        break;
        
    case ESageCharacterType::PseudoSage:
        SpiritualPower = 100.0f;
        MaxSpiritualPower = 100.0f;
        WisdomPoints = 8;
        SageAuraRange = 500.0f;
        break;
        
    default:
        break;
    }
    
    // 添加初始技能
    AvailableSkills.Add("基本冥想");
    AvailableSkills.Add("靈力感知");
    AvailableSkills.Add("陰陽調和");
    
    // 添加初始歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("角色 %s 誕生了"), *CharacterName));
    
    return true;
}

bool AMingSageCharacter::LevelUpCharacter()
{
    if (CharacterLevel >= 100) // 最高等級限制
    {
        return false;
    }
    
    CharacterLevel++;
    
    // 增加屬性
    MaxSpiritualPower += 20.0f;
    SpiritualPower = MaxSpiritualPower; // 升級時恢復滿靈力
    WisdomPoints += 2;
    SageAuraRange += 50.0f;
    
    // 應用等級效果
    ApplyLevelEffects();
    
    // 廣播升級事件
    OnCharacterLeveledUp.Broadcast(CharacterLevel);
    
    // 添加歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("角色升級到等級 %d"), CharacterLevel));
    
    return true;
}

void AMingSageCharacter::GainExperience(int32 Experience)
{
    if (Experience <= 0)
    {
        return;
    }
    
    int32 ActualExperience = FMath::RoundToInt(Experience * ExperienceMultiplier);
    ExperiencePoints += ActualExperience;
    
    // 檢查是否可以升級
    int32 RequiredExperience = CharacterLevel * 100; // 簡單的經驗公式
    if (ExperiencePoints >= RequiredExperience)
    {
        ExperiencePoints -= RequiredExperience;
        LevelUpCharacter();
    }
    
    // 廣播經驗值變化事件
    OnExperienceChanged.Broadcast(ExperiencePoints, CharacterLevel * 100);
    
    // 更新統計
    UpdateStatistic("TotalExperienceGained", ActualExperience);
}

bool AMingSageCharacter::UseSkill(const FString& SkillName)
{
    if (!AvailableSkills.Contains(SkillName))
    {
        return false;
    }
    
    // 檢查冷卻時間
    if (SkillCooldowns.Contains(SkillName))
    {
        if (SkillCooldowns[SkillName] > 0.0f)
        {
            return false;
        }
    }
    
    // 檢查靈力值
    float RequiredSpiritualPower = 10.0f; // 基本靈力消耗
    if (SpiritualPower < RequiredSpiritualPower)
    {
        return false;
    }
    
    // 消耗靈力
    SpiritualPower -= RequiredSpiritualPower;
    
    // 應用技能效果
    ApplySkillEffects(SkillName);
    
    // 設置冷卻時間
    SkillCooldowns.Add(SkillName, 5.0f); // 5秒冷卻
    
    // 廣播技能使用事件
    OnSkillUsed.Broadcast(SkillName);
    
    // 更新統計
    UpdateStatistic("SkillsUsed", 1);
    UpdateStatistic(FString::Printf(TEXT("Skill_%s_Used"), *SkillName), 1);
    
    return true;
}

void AMingSageCharacter::AddStatusEffect(const FString& EffectName, float Duration)
{
    if (!StatusEffects.Contains(EffectName))
    {
        StatusEffects.Add(EffectName);
        StatusEffectDurations.Add(EffectName, Duration);
        
        // 廣播狀態效果添加事件
        OnStatusEffectChanged.Broadcast(EffectName, true);
    }
    else
    {
        // 更新持續時間
        StatusEffectDurations[EffectName] = FMath::Max(StatusEffectDurations[EffectName], Duration);
    }
}

void AMingSageCharacter::RemoveStatusEffect(const FString& EffectName)
{
    if (StatusEffects.Contains(EffectName))
    {
        StatusEffects.Remove(EffectName);
        StatusEffectDurations.Remove(EffectName);
        
        // 廣播狀態效果移除事件
        OnStatusEffectChanged.Broadcast(EffectName, false);
    }
}

void AMingSageCharacter::UpdateRelationship(const FString& TargetName, float Change)
{
    float CurrentRelationship = RelationshipNetwork.FindRef(TargetName);
    CurrentRelationship = FMath::Clamp(CurrentRelationship + Change, -100.0f, 100.0f);
    RelationshipNetwork.Add(TargetName, CurrentRelationship);
    
    // 添加歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("與 %s 的關係變化: %.1f"), *TargetName, Change));
}

void AMingSageCharacter::UpdateReputation(const FString& Faction, int32 Change)
{
    int32 CurrentReputation = ReputationMap.FindRef(Faction);
    CurrentReputation = FMath::Clamp(CurrentReputation + Change, -1000, 1000);
    ReputationMap.Add(Faction, CurrentReputation);
    
    // 添加歷史記錄
    AddHistoryRecord(FString::Printf(TEXT("%s 聲望變化: %d"), *Faction, Change));
}

void AMingSageCharacter::AddHistoryRecord(const FString& Record)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
    FString FormattedRecord = FString::Printf(TEXT("[%s] %s"), *Timestamp, *Record);
    HistoryRecords.Add(FormattedRecord);
    
    // 限制歷史記錄數量
    if (HistoryRecords.Num() > 100)
    {
        HistoryRecords.RemoveAt(0);
    }
}

void AMingSageCharacter::UnlockAchievement(const FString& AchievementName)
{
    if (!Achievements.Contains(AchievementName))
    {
        Achievements.Add(AchievementName);
        
        // 獎勵
        GainExperience(50); // 解鎖成就獎勵經驗
        WisdomPoints += 1; // 獎勵智慧點
        
        // 廣播成就解鎖事件
        OnAchievementUnlocked.Broadcast(AchievementName);
        
        // 添加歷史記錄
        AddHistoryRecord(FString::Printf(TEXT("解鎖成就: %s"), *AchievementName));
    }
}

void AMingSageCharacter::SetPreference(const FString& PreferenceName, float Value)
{
    PreferenceSettings.Add(PreferenceName, FMath::Clamp(Value, 0.0f, 1.0f));
}

void AMingSageCharacter::UpdateStatistic(const FString& StatName, int32 Value)
{
    int32 CurrentValue = StatisticsData.FindRef(StatName);
    StatisticsData.Add(StatName, CurrentValue + Value);
}

float AMingSageCharacter::GetCharacterPowerRating() const
{
    return CalculateCharacterPower();
}

FString AMingSageCharacter::GetCharacterStatusDescription() const
{
    FString Description = FString::Printf(TEXT("角色: %s\n"), *CharacterData.CharacterName);
    Description += FString::Printf(TEXT("類型: %s\n"), *SageCharacterSystem->GetCharacterTypeDisplayName(CharacterData.CharacterType));
    Description += FString::Printf(TEXT("等級: %d\n"), CharacterLevel);
    Description += FString::Printf(TEXT("經驗: %d\n"), ExperiencePoints);
    Description += FString::Printf(TEXT("靈力: %.1f/%.1f\n"), SpiritualPower, MaxSpiritualPower);
    Description += FString::Printf(TEXT("智慧: %d\n"), WisdomPoints);
    Description += FString::Printf(TEXT("墮落值: %d/%d\n"), CharacterData.CurrentFallValue, CharacterData.Traits.FallThreshold);
    Description += FString::Printf(TEXT("狀態: %s\n"), CharacterData.bIsFallen ? TEXT("已墮落") : TEXT("正常"));
    
    return Description;
}

TArray<FString> AMingSageCharacter::GetCharacterRecommendations() const
{
    return GenerateRecommendations();
}

bool AMingSageCharacter::CanPerformAction(const FString& ActionName) const
{
    // 檢查基本條件
    if (CharacterData.bIsFallen)
    {
        return false;
    }
    
    if (SpiritualPower <= 0.0f)
    {
        return false;
    }
    
    if (CharacterFatigue >= 100.0f)
    {
        return false;
    }
    
    // 檢查特定動作條件
    if (ActionName == "UseEvilStrategy")
    {
        return SageCharacterSystem->CanUseEvilStrategy(CharacterData);
    }
    else if (ActionName == "UseRighteousStrategy")
    {
        return SageCharacterSystem->CanUseEvilStrategy(CharacterData) || true; // 正策總是可以使用
    }
    else if (ActionName == "Meditate")
    {
        return CharacterFatigue > 20.0f || CharacterMentalState < 80.0f;
    }
    
    return true;
}

bool AMingSageCharacter::ExecuteSageAbility(const FString& AbilityName)
{
    if (!CanPerformAction(AbilityName))
    {
        return false;
    }
    
    if (AbilityName == "SageAura")
    {
        // 聖者光環 - 恢復周圍友軍
        SpiritualPower -= 20.0f;
        // 這裡會有實際的光環效果實現
        AddHistoryRecord(TEXT("使用了聖者光環能力"));
        return true;
    }
    else if (AbilityName == "DivineIntervention")
    {
        // 神聖干預 - 強大的治療能力
        SpiritualPower -= 50.0f;
        CharacterHealth = 100.0f;
        CharacterMentalState = 100.0f;
        AddHistoryRecord(TEXT("使用了神聖干預能力"));
        return true;
    }
    else if (AbilityName == "WisdomBlessing")
    {
        // 智慧祝福 - 提升智慧
        SpiritualPower -= 30.0f;
        WisdomPoints += 1;
        AddHistoryRecord(TEXT("使用了智慧祝福能力"));
        return true;
    }
    
    return false;
}

bool AMingSageCharacter::Meditate(float Duration)
{
    if (!CanPerformAction("Meditate"))
    {
        return false;
    }
    
    // 冥想效果
    SpiritualPower = FMath::Min(SpiritualPower + Duration * 2.0f, MaxSpiritualPower);
    CharacterFatigue = FMath::Max(CharacterFatigue - Duration * 5.0f, 0.0f);
    CharacterMentalState = FMath::Min(CharacterMentalState + Duration * 3.0f, 100.0f);
    CharacterFocus = FMath::Min(CharacterFocus + Duration * 2.0f, 100.0f);
    
    // 如果是聖者類型，冥想還會減少墮落值
    if (CharacterData.CharacterType == ESageCharacterType::Sage)
    {
        SageCharacterSystem->UseRighteousStrategy(CharacterData, FMath::RoundToInt(Duration));
    }
    
    AddHistoryRecord(FString::Printf(TEXT("進行了 %.1f 秒的冥想"), Duration));
    
    return true;
}

bool AMingSageCharacter::PerformAtonement()
{
    if (!SageCharacterSystem)
    {
        return false;
    }
    
    bool Success = SageCharacterSystem->Atonement(CharacterData);
    if (Success)
    {
        SpiritualPower -= 30.0f;
        AddHistoryRecord(TEXT("進行了贖罪儀式"));
        UpdateStatistic("AtonementPerformed", 1);
    }
    
    return Success;
}

bool AMingSageCharacter::BalanceYinYang()
{
    if (!YinYangSystem)
    {
        return false;
    }
    
    bool Success = YinYangSystem->BalanceYinYang();
    if (Success)
    {
        SpiritualPower -= 15.0f;
        AddHistoryRecord(TEXT("進行了陰陽平衡"));
        UpdateStatistic("YinYangBalanced", 1);
    }
    
    return Success;
}

bool AMingSageCharacter::HarmonizeElements()
{
    if (!YinYangSystem)
    {
        return false;
    }
    
    bool Success = YinYangSystem->HarmonizeElements();
    if (Success)
    {
        SpiritualPower -= 25.0f;
        AddHistoryRecord(TEXT("進行了五行和諧"));
        UpdateStatistic("ElementsHarmonized", 1);
    }
    
    return Success;
}

FString AMingSageCharacter::GetCharacterDetailedInfo() const
{
    FString Info = GetCharacterStatusDescription();
    
    Info += TEXT("\n=== 關係網絡 ===\n");
    for (const auto& Relationship : RelationshipNetwork)
    {
        Info += FString::Printf(TEXT("%s: %.1f\n"), *Relationship.Key, Relationship.Value);
    }
    
    Info += TEXT("\n=== 聲望 ===\n");
    for (const auto& Reputation : ReputationMap)
    {
        Info += FString::Printf(TEXT("%s: %d\n"), *Reputation.Key, Reputation.Value);
    }
    
    Info += TEXT("\n=== 技能 ===\n");
    for (const FString& Skill : AvailableSkills)
    {
        Info += FString::Printf(TEXT("- %s\n"), *Skill);
    }
    
    Info += TEXT("\n=== 成就 ===\n");
    for (const FString& Achievement : Achievements)
    {
        Info += FString::Printf(TEXT("- %s\n"), *Achievement);
    }
    
    return Info;
}

bool AMingSageCharacter::SaveCharacterData()
{
    // 這裡會實現實際的保存邏輯
    AddHistoryRecord(TEXT("角色數據已保存"));
    return true;
}

bool AMingSageCharacter::LoadCharacterData()
{
    // 這裡會實現實際的載入邏輯
    AddHistoryRecord(TEXT("角色數據已載入"));
    return true;
}

void AMingSageCharacter::ResetCharacter()
{
    CharacterLevel = 1;
    ExperiencePoints = 0;
    SpiritualPower = MaxSpiritualPower;
    WisdomPoints = 0;
    CharacterFatigue = 0.0f;
    CharacterMood = 50.0f;
    CharacterHealth = 100.0f;
    CharacterMentalState = 100.0f;
    CharacterFocus = 100.0f;
    
    StatusEffects.Empty();
    StatusEffectDurations.Empty();
    SkillCooldowns.Empty();
    
    AddHistoryRecord(TEXT("角色已重置"));
}

void AMingSageCharacter::UpdateCharacterState(float DeltaTime)
{
    // 更新冷卻時間
    for (auto& Cooldown : SkillCooldowns)
    {
        Cooldown.Value = FMath::Max(0.0f, Cooldown.Value - DeltaTime);
    }
    
    // 更新疲勞度
    CharacterFatigue = FMath::Min(CharacterFatigue + DeltaTime * 0.5f, 100.0f);
    
    // 更新心情
    if (CharacterFatigue > 80.0f)
    {
        CharacterMood = FMath::Max(CharacterMood - DeltaTime * 2.0f, 0.0f);
    }
    else
    {
        CharacterMood = FMath::Min(CharacterMood + DeltaTime * 0.5f, 100.0f);
    }
}

void AMingSageCharacter::ProcessStatusEffects(float DeltaTime)
{
    TArray<FString> EffectsToRemove;
    
    for (auto& Effect : StatusEffectDurations)
    {
        Effect.Value -= DeltaTime;
        if (Effect.Value <= 0.0f)
        {
            EffectsToRemove.Add(Effect.Key);
        }
    }
    
    for (const FString& Effect : EffectsToRemove)
    {
        RemoveStatusEffect(Effect);
    }
}

void AMingSageCharacter::UpdateSpiritualPower(float DeltaTime)
{
    if (SpiritualPower < MaxSpiritualPower)
    {
        SpiritualPower = FMath::Min(SpiritualPower + SpiritualPowerRegenRate * DeltaTime, MaxSpiritualPower);
        OnSpiritualPowerChanged.Broadcast(SpiritualPower, MaxSpiritualPower);
    }
}

void AMingSageCharacter::CheckCharacterConditions()
{
    // 檢查墮落狀態
    if (SageCharacterSystem)
    {
        SageCharacterSystem->CheckAndApplyFall(CharacterData);
    }
    
    // 檢查靈力值警告
    if (SpiritualPower < 20.0f)
    {
        AddStatusEffect("靈力不足", 5.0f);
    }
    
    // 檢查疲勞警告
    if (CharacterFatigue > 80.0f)
    {
        AddStatusEffect("過度疲勞", 10.0f);
    }
}

void AMingSageCharacter::ApplyLevelEffects()
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

void AMingSageCharacter::ApplySkillEffects(const FString& SkillName)
{
    if (SkillName == "基本冥想")
    {
        CharacterFatigue = FMath::Max(CharacterFatigue - 10.0f, 0.0f);
        CharacterMentalState = FMath::Min(CharacterMentalState + 15.0f, 100.0f);
    }
    else if (SkillName == "靈力感知")
    {
        SpiritualPower = FMath::Min(SpiritualPower + 5.0f, MaxSpiritualPower);
        CharacterFocus = FMath::Min(CharacterFocus + 10.0f, 100.0f);
    }
    else if (SkillName == "陰陽調和")
    {
        if (YinYangSystem)
        {
            YinYangSystem->BalanceYinYang();
        }
    }
}

TArray<FString> AMingSageCharacter::GenerateRecommendations() const
{
    TArray<FString> Recommendations;
    
    // 基於角色狀態生成建議
    if (SpiritualPower < 30.0f)
    {
        Recommendations.Add("建議進行冥想以恢復靈力");
    }
    
    if (CharacterFatigue > 70.0f)
    {
        Recommendations.Add("建議休息以減少疲勞");
    }
    
    if (CharacterData.CurrentFallValue > CharacterData.Traits.FallThreshold * 0.7f)
    {
        Recommendations.Add("建議進行贖罪以減少墮落值");
    }
    
    if (CharacterMentalState < 50.0f)
    {
        Recommendations.Add("建議進行心理調整活動");
    }
    
    // 基於角色類型生成建議
    switch (CharacterData.CharacterType)
    {
    case ESageCharacterType::Sage:
        Recommendations.Add("作為聖者，建議平衡使用正逆策略");
        break;
        
    case ESageCharacterType::DemonKing:
        Recommendations.Add("作為魔王，謹慎使用逆策以避免失控");
        break;
        
    case ESageCharacterType::PseudoSage:
        Recommendations.Add("作為偽聖者，專注於外交和正策");
        break;
        
    default:
        break;
    }
    
    // 基於陰陽五行狀態生成建議
    if (YinYangSystem)
    {
        FYinYangBalance Balance = YinYangSystem->GetCurrentBalance();
        if (FMath::Abs(Balance.BalanceScore) > 50.0f)
        {
            Recommendations.Add("建議進行陰陽平衡以恢復和諧");
        }
    }
    
    return Recommendations;
}

float AMingSageCharacter::CalculateCharacterPower() const
{
    float BasePower = CharacterLevel * 10.0f;
    float SpiritualPowerBonus = SpiritualPower / MaxSpiritualPower * 50.0f;
    float WisdomBonus = WisdomPoints * 5.0f;
    float SkillBonus = AvailableSkills.Num() * 3.0f;
    float AchievementBonus = Achievements.Num() * 2.0f;
    
    float TotalPower = (BasePower + SpiritualPowerBonus + WisdomBonus + SkillBonus + AchievementBonus) * PowerMultiplier;
    
    // 墮落狀態懲罰
    if (CharacterData.bIsFallen)
    {
        TotalPower *= 0.7f;
    }
    
    return TotalPower;
}

void AMingSageCharacter::UpdateSageAura()
{
    if (!bShowSageAura)
    {
        return;
    }
    
    // 這裡會有實際的光環視覺效果實現
    // 根據角色類型和狀態調整光環效果
}

void AMingSageCharacter::CheckSurroundingEnvironment()
{
    // 檢查周圍環境，收集信息
    // 這裡會有實際的環境檢測邏輯
}

void AMingSageCharacter::HandleEnvironmentInteraction()
{
    // 處理與環境的互動
    // 這裡會有實際的環境互動邏輯
}
