#include "MingCharacterSystem.h"
#include "Engine/World.h"

UMingCharacterSystem::UMingCharacterSystem()
{
    WorldContext = GetWorld();
    InitializeCharacters();
}

void UMingCharacterSystem::InitializeCharacters()
{
    Characters.Empty();
    CharacterRelationships.Empty();
    CharacterEffects.Empty();
    HistoricalEvents.Empty();
    
    SetupRevolutionaryCharacters();
    SetupMilitaryCharacters();
    SetupPoliticalCharacters();
    SetupCulturalCharacters();
    
    UE_LOG(LogTemp, Log, TEXT("Character system initialized with %d characters"), Characters.Num());
}

void UMingCharacterSystem::SetupRevolutionaryCharacters()
{
    // Sun Yat-sen
    FMingHistoricalCharacter SunYatSen;
    SunYatSen.CharacterID = TEXT("SUN_YAT_SEN");
    SunYatSen.CharacterName = TEXT("孫中山");
    SunYatSen.PrimaryRole = EMingCharacterRole::Revolutionary;
    SunYatSen.SecondaryRoles.Add(EMingCharacterRole::Leader);
    SunYatSen.SecondaryRoles.Add(EMingCharacterRole::Diplomat);
    SunYatSen.Biography = TEXT("孫中山（1866-1925），中國近代民主革命家，中華民國國父。畢生致力於推翻清朝，建立共和。");
    SunYatSen.HistoricalSignificance = TEXT("提出三民主義，創建同盟會，領導辛亥革命，建立中華民國。");
    SunYatSen.BirthYear = 1866;
    SunYatSen.DeathYear = 1925;
    SunYatSen.BirthPlace = TEXT("廣東香山");
    SunYatSen.MajorAchievements.Add(TEXT("創立同盟會"));
    SunYatSen.MajorAchievements.Add(TEXT("領導辛亥革命"));
    SunYatSen.MajorAchievements.Add(TEXT("建立中華民國"));
    SunYatSen.MajorAchievements.Add(TEXT("提出三民主義"));
    
    // Skills
    FMingCharacterSkill LeadershipSkill;
    LeadershipSkill.SkillType = EMingSkillType::Leadership;
    LeadershipSkill.SkillName = TEXT("革命領袖");
    LeadershipSkill.Description = TEXT("卓越的領導才能，能激發追隨者的熱情");
    LeadershipSkill.Level = 8;
    LeadershipSkill.MaxLevel = 10;
    LeadershipSkill.Experience = 750;
    LeadershipSkill.ExperienceToNext = 850;
    LeadershipSkill.SkillEffects.Add(TEXT("LoyaltyBonus(+20%)"));
    LeadershipSkill.SkillEffects.Add(TEXT("InfluenceBonus(+15%)"));
    SunYatSen.Skills.Add(LeadershipSkill);
    
    FMingCharacterSkill DiplomacySkill;
    DiplomacySkill.SkillType = EMingSkillType::Diplomacy;
    DiplomacySkill.SkillName = TEXT("外交手腕");
    DiplomacySkill.Description = TEXT("善於談判，能夠爭取國際支持");
    DiplomacySkill.Level = 7;
    DiplomacySkill.MaxLevel = 10;
    DiplomacySkill.Experience = 600;
    DiplomacySkill.ExperienceToNext = 700;
    DiplomacySkill.SkillEffects.Add(TEXT("DiplomacyBonus(+25%)"));
    DiplomacySkill.SkillEffects.Add(TEXT("InternationalSupport(+10%)"));
    SunYatSen.Skills.Add(DiplomacySkill);
    
    FMingCharacterSkill CultureSkill;
    CultureSkill.SkillType = EMingSkillType::Culture;
    CultureSkill.SkillName = TEXT("思想啟蒙");
    CultureSkill.Description = TEXT("傳播民主思想，啟發民眾覺悟");
    CultureSkill.Level = 9;
    CultureSkill.MaxLevel = 10;
    CultureSkill.Experience = 850;
    CultureSkill.ExperienceToNext = 950;
    CultureSkill.SkillEffects.Add(TEXT("CulturalInfluence(+30%)"));
    CultureSkill.SkillEffects.Add(TEXT("PublicSupport(+20%)"));
    SunYatSen.Skills.Add(CultureSkill);
    
    // Traits
    FMingCharacterTrait VisionaryTrait;
    VisionaryTrait.TraitID = TEXT("VISIONARY");
    VisionaryTrait.TraitName = TEXT("遠見卓識");
    VisionaryTrait.Description = TEXT("具有超越時代的遠見，能夠預見歷史走向");
    VisionaryTrait.PositiveEffects.Add(TEXT("HistoricalAccuracy(+15%)"));
    VisionaryTrait.PositiveEffects.Add(TEXT("LongTermPlanning(+20%)"));
    VisionaryTrait.NegativeEffects.Add(TEXT("Impatience(-10%)"));
    VisionaryTrait.InfluenceWeight = 1.2f;
    SunYatSen.Traits.Add(VisionaryTrait);
    
    FMingCharacterTrait CharismaticTrait;
    CharismaticTrait.TraitID = TEXT("CHARISMATIC");
    CharismaticTrait.TraitName = TEXT("魅力非凡");
    CharismaticTrait.Description = TEXT("天生的領袖魅力，能夠凝聚人心");
    CharismaticTrait.PositiveEffects.Add(TEXT("LoyaltyBonus(+25%)"));
    CharismaticTrait.PositiveEffects.Add(TEXT("RecruitmentBonus(+30%)"));
    CharismaticTrait.InfluenceWeight = 1.1f;
    SunYatSen.Traits.Add(CharismaticTrait);
    
    Characters.Add(SunYatSen.CharacterID, SunYatSen);
    
    // Huang Xing
    FMingHistoricalCharacter HuangXing;
    HuangXing.CharacterID = TEXT("HUANG_XING");
    HuangXing.CharacterName = TEXT("黃興");
    HuangXing.PrimaryRole = EMingCharacterRole::Commander;
    HuangXing.SecondaryRoles.Add(EMingCharacterRole::Revolutionary);
    HuangXing.Biography = TEXT("黃興（1874-1916），革命軍事家，孫中山的重要戰友。");
    HuangXing.HistoricalSignificance = TEXT("指揮多次武裝起義，是辛亥革命的重要軍事領袖。");
    HuangXing.BirthYear = 1874;
    HuangXing.DeathYear = 1916;
    HuangXing.MajorAchievements.Add(TEXT("指揮武昌起義"));
    HuangXing.MajorAchievements.Add(TEXT("建立革命軍"));
    
    // Military skills
    FMingCharacterSkill MilitarySkill;
    MilitarySkill.SkillType = EMingSkillType::Military;
    MilitarySkill.SkillName = TEXT("軍事指揮");
    MilitarySkill.Description = TEXT("出色的軍事才能，善於指揮作戰");
    MilitarySkill.Level = 8;
    MilitarySkill.MaxLevel = 10;
    MilitarySkill.Experience = 700;
    MilitarySkill.ExperienceToNext = 800;
    MilitarySkill.SkillEffects.Add(TEXT("CombatBonus(+20%)"));
    MilitarySkill.SkillEffects.Add(TEXT("MoraleBonus(+15%)"));
    HuangXing.Skills.Add(MilitarySkill);
    
    FMingCharacterTrait BraveTrait;
    BraveTrait.TraitID = TEXT("BRAVE");
    BraveTrait.TraitName = TEXT("勇猛無畏");
    BraveTrait.Description = TEXT("作戰勇敢，身先士卒");
    BraveTrait.PositiveEffects.Add(TEXT("CombatBonus(+25%)"));
    BraveTrait.PositiveEffects.Add(TEXT("MoraleBonus(+20%)"));
    BraveTrait.NegativeEffects.Add(TEXT("Recklessness(-15%)"));
    HuangXing.Traits.Add(BraveTrait);
    
    Characters.Add(HuangXing.CharacterID, HuangXing);
}

void UMingCharacterSystem::SetupMilitaryCharacters()
{
    // Chiang Kai-shek
    FMingHistoricalCharacter ChiangKaiShek;
    ChiangKaiShek.CharacterID = TEXT("CHIANG_KAI_SHEK");
    ChiangKaiShek.CharacterName = TEXT("蔣介石");
    ChiangKaiShek.PrimaryRole = EMingCharacterRole::Commander;
    ChiangKaiShek.SecondaryRoles.Add(EMingCharacterRole::Leader);
    ChiangKaiShek.Biography = TEXT("蔣介石（1887-1975），國民黨領袖，軍事家。");
    ChiangKaiShek.HistoricalSignificance = TEXT("領導北伐，建立南京國民政府，抗戰時期擔任最高統帥。");
    ChiangKaiShek.BirthYear = 1887;
    ChiangKaiShek.DeathYear = 1975;
    ChiangKaiShek.MajorAchievements.Add(TEXT("領導北伐"));
    ChiangKaiShek.MajorAchievements.Add(TEXT("建立國民政府"));
    ChiangKaiShek.MajorAchievements.Add(TEXT("領導抗戰"));
    
    // Skills
    FMingCharacterSkill CommandSkill;
    CommandSkill.SkillType = EMingSkillType::Military;
    CommandSkill.SkillName = TEXT("統帥才能");
    CommandSkill.Description = TEXT("卓越的軍事統帥能力");
    CommandSkill.Level = 7;
    CommandSkill.MaxLevel = 10;
    CommandSkill.Experience = 600;
    CommandSkill.ExperienceToNext = 700;
    CommandSkill.SkillEffects.Add(TEXT("ArmyBonus(+20%)"));
    CommandSkill.SkillEffects.Add(TEXT("StrategyBonus(+15%)"));
    ChiangKaiShek.Skills.Add(CommandSkill);
    
    FMingCharacterSkill AdminSkill;
    AdminSkill.SkillType = EMingSkillType::Administration;
    AdminSkill.SkillName = TEXT("行政管理");
    AdminSkill.Description = TEXT("善於組織和管理");
    AdminSkill.Level = 6;
    AdminSkill.MaxLevel = 10;
    AdminSkill.Experience = 500;
    AdminSkill.ExperienceToNext = 600;
    AdminSkill.SkillEffects.Add(TEXT("EfficiencyBonus(+15%)"));
    AdminSkill.SkillEffects.Add(TEXT("OrganizationBonus(+20%)"));
    ChiangKaiShek.Skills.Add(AdminSkill);
    
    // Traits
    FMingCharacterTrait DisciplinedTrait;
    DisciplinedTrait.TraitID = TEXT("DISCIPLINED");
    DisciplinedTrait.TraitName = TEXT("紀律嚴明");
    DisciplinedTrait.Description = TEXT("重視紀律，要求嚴格");
    DisciplinedTrait.PositiveEffects.Add(TEXT("ArmyDiscipline(+25%)"));
    DisciplinedTrait.PositiveEffects.Add(TEXT("OrderBonus(+20%)"));
    DisciplinedTrait.NegativeEffects.Add(TEXT("Flexibility(-15%)"));
    ChiangKaiShek.Traits.Add(DisciplinedTrait);
    
    Characters.Add(ChiangKaiShek.CharacterID, ChiangKaiShek);
}

void UMingCharacterSystem::SetupPoliticalCharacters()
{
    // Song Jiaoren
    FMingHistoricalCharacter SongJiaoren;
    SongJiaoren.CharacterID = TEXT("SONG_JIAOREN");
    SongJiaoren.CharacterName = TEXT("宋教仁");
    SongJiaoren.PrimaryRole = EMingCharacterRole::Diplomat;
    SongJiaoren.SecondaryRoles.Add(EMingCharacterRole::Scholar);
    SongJiaoren.Biography = TEXT("宋教仁（1882-1913），革命家、政治家。");
    SongJiaoren.HistoricalSignificance = TEXT("致力於憲政建設，推動議會政治。");
    SongJiaoren.BirthYear = 1882;
    SongJiaoren.DeathYear = 1913;
    SongJiaoren.MajorAchievements.Add(TEXT("推動憲政"));
    SongJiaoren.MajorAchievements.Add(TEXT("建立國民黨"));
    
    // Skills
    FMingCharacterSkill PoliticsSkill;
    PoliticsSkill.SkillType = EMingSkillType::Diplomacy;
    PoliticsSkill.SkillName = TEXT("政治才能");
    PoliticsSkill.Description = TEXT("精通政治理論和實踐");
    PoliticsSkill.Level = 8;
    PoliticsSkill.MaxLevel = 10;
    PoliticsSkill.Experience = 750;
    PoliticsSkill.ExperienceToNext = 850;
    PoliticsSkill.SkillEffects.Add(TEXT("PoliticalInfluence(+25%)"));
    PoliticsSkill.SkillEffects.Add(TEXT("ConstitutionalBonus(+20%)"));
    SongJiaoren.Skills.Add(PoliticsSkill);
    
    FMingCharacterTrait IdealistTrait;
    IdealistTrait.TraitID = TEXT("IDEALIST");
    IdealistTrait.TraitName = TEXT("理想主義");
    IdealistTrait.Description = TEXT("堅持理想，追求完美");
    IdealistTrait.PositiveEffects.Add(TEXT("IntegrityBonus(+30%)"));
    IdealistTrait.PositiveEffects.Add(TEXT("PublicTrust(+25%)"));
    IdealistTrait.NegativeEffects.Add(TEXT("Pragmatism(-20%)"));
    SongJiaoren.Traits.Add(IdealistTrait);
    
    Characters.Add(SongJiaoren.CharacterID, SongJiaoren);
}

void UMingCharacterSystem::SetupCulturalCharacters()
{
    // Lu Xun
    FMingHistoricalCharacter LuXun;
    LuXun.CharacterID = TEXT("LU_XUN");
    LuXun.CharacterName = TEXT("魯迅");
    LuXun.PrimaryRole = EMingCharacterRole::Scholar;
    LuXun.SecondaryRoles.Add(EMingCharacterRole::Revolutionary);
    LuXun.Biography = TEXT("魯迅（1881-1936），文學家、思想家。");
    LuXun.HistoricalSignificance = TEXT("新文化運動的重要人物，用文學喚醒民眾。");
    LuXun.BirthYear = 1881;
    LuXun.DeathYear = 1936;
    LuXun.MajorAchievements.Add(TEXT("新文化運動"));
    LuXun.MajorAchievements.Add(TEXT("文學創作"));
    
    // Skills
    FMingCharacterSkill WritingSkill;
    WritingSkill.SkillType = EMingSkillType::Culture;
    WritingSkill.SkillName = TEXT("文學才華");
    WritingSkill.Description = TEXT("卓越的文學創作能力");
    WritingSkill.Level = 9;
    WritingSkill.MaxLevel = 10;
    WritingSkill.Experience = 850;
    WritingSkill.ExperienceToNext = 950;
    WritingSkill.SkillEffects.Add(TEXT("CulturalInfluence(+30%)"));
    WritingSkill.SkillEffects.Add(TEXT("PublicAwareness(+25%)"));
    LuXun.Skills.Add(WritingSkill);
    
    FMingCharacterSkill CriticalSkill;
    CriticalSkill.SkillType = EMingSkillType::Intelligence;
    CriticalSkill.SkillName = TEXT("批判思維");
    CriticalSkill.Description = TEXT("深刻的社會批判能力");
    CriticalSkill.Level = 8;
    CriticalSkill.MaxLevel = 10;
    CriticalSkill.Experience = 700;
    CriticalSkill.ExperienceToNext = 800;
    CriticalSkill.SkillEffects.Add(TEXT("SocialCritique(+25%)"));
    CriticalSkill.SkillEffects.Add(TEXT("TruthSeeking(+20%)"));
    LuXun.Skills.Add(CriticalSkill);
    
    FMingCharacterTrait CriticalTrait;
    CriticalTrait.TraitID = TEXT("CRITICAL");
    CriticalTrait.TraitName = TEXT("批判精神");
    CriticalTrait.Description = TEXT("善於批判，追求真相");
    CriticalTrait.PositiveEffects.Add(TEXT("TruthBonus(+25%)"));
    CriticalTrait.PositiveEffects.Add(TEXT("SocialAwareness(+20%)"));
    CriticalTrait.NegativeEffects.Add(TEXT("Popularity(-15%)"));
    LuXun.Traits.Add(CriticalTrait);
    
    Characters.Add(LuXun.CharacterID, LuXun);
}

void UMingCharacterSystem::RecruitCharacter(const FString& CharacterID)
{
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character not found: %s"), *CharacterID);
        return;
    }
    
    if (Character->bIsRecruited)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character already recruited: %s"), *CharacterID);
        return;
    }
    
    Character->bIsRecruited = true;
    Character->bIsAvailable = false;
    
    // Set initial loyalty
    Character->Loyalty = 50.0f;
    
    // Trigger event
    OnCharacterRecruited.Broadcast(*Character, GetRoleName(Character->PrimaryRole));
    
    UE_LOG(LogTemp, Log, TEXT("Character recruited: %s"), *Character->CharacterName);
}

void UMingCharacterSystem::DismissCharacter(const FString& CharacterID)
{
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (!Character || !Character->bIsRecruited)
    {
        return;
    }
    
    Character->bIsRecruited = false;
    Character->bIsAvailable = true;
    Character->Loyalty = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Character dismissed: %s"), *Character->CharacterName);
}

void UMingCharacterSystem::AssignCharacterRole(const FString& CharacterID, EMingCharacterRole Role)
{
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (!Character)
    {
        return;
    }
    
    Character->PrimaryRole = Role;
    
    UE_LOG(LogTemp, Log, TEXT("Character %s assigned role: %s"), 
           *Character->CharacterName, *GetRoleName(Role));
}

TArray<FMingHistoricalCharacter> UMingCharacterSystem::GetAllCharacters() const
{
    TArray<FMingHistoricalCharacter> Result;
    for (const auto& Pair : Characters)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

TArray<FMingHistoricalCharacter> UMingCharacterSystem::GetAvailableCharacters() const
{
    TArray<FMingHistoricalCharacter> Result;
    for (const auto& Pair : Characters)
    {
        if (Pair.Value.bIsAvailable && !Pair.Value.bIsRecruited)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

TArray<FMingHistoricalCharacter> UMingCharacterSystem::GetRecruitedCharacters() const
{
    TArray<FMingHistoricalCharacter> Result;
    for (const auto& Pair : Characters)
    {
        if (Pair.Value.bIsRecruited)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

FMingHistoricalCharacter UMingCharacterSystem::GetCharacter(const FString& CharacterID) const
{
    const FMingHistoricalCharacter* Character = Characters.Find(CharacterID);
    return Character ? *Character : FMingHistoricalCharacter();
}

void UMingCharacterSystem::AddSkillExperience(const FString& CharacterID, EMingSkillType SkillType, int32 Experience)
{
    FMingCharacterSkill* Skill = FindCharacterSkill(CharacterID, SkillType);
    if (!Skill)
    {
        return;
    }
    
    ProcessSkillExperience(*Skill, Experience);
    
    // Check for level up
    if (Skill->Experience >= Skill->ExperienceToNext && !Skill->bIsMaxLevel)
    {
        LevelUpSkill(CharacterID, SkillType);
    }
    
    // Apply skill effects
    ApplySkillEffects(CharacterID, *Skill);
}

void UMingCharacterSystem::LevelUpSkill(const FString& CharacterID, EMingSkillType SkillType)
{
    FMingCharacterSkill* Skill = FindCharacterSkill(CharacterID, SkillType);
    if (!Skill || Skill->bIsMaxLevel)
    {
        return;
    }
    
    Skill->Level++;
    Skill->Experience = 0;
    Skill->ExperienceToNext = Skill->Level * 100; // Simple progression
    
    if (Skill->Level >= Skill->MaxLevel)
    {
        Skill->bIsMaxLevel = true;
        Skill->ExperienceToNext = 0;
    }
    
    // Trigger event
    OnSkillLevelUp.Broadcast(CharacterID, *Skill);
    
    UE_LOG(LogTemp, Log, TEXT("Skill leveled up: %s - %s to level %d"), 
           *CharacterID, *Skill->SkillName, Skill->Level);
}

void UMingCharacterSystem::LearnNewSkill(const FString& CharacterID, const FString& SkillID)
{
    // This would add a new skill to the character
    UE_LOG(LogTemp, Log, TEXT("Character %s learned new skill: %s"), *CharacterID, *SkillID);
}

TArray<FMingCharacterSkill> UMingCharacterSystem::GetCharacterSkills(const FString& CharacterID) const
{
    const FMingHistoricalCharacter* Character = Characters.Find(CharacterID);
    return Character ? Character->Skills : TArray<FMingCharacterSkill>();
}

FMingCharacterSkill UMingCharacterSystem::GetSkill(const FString& CharacterID, EMingSkillType SkillType) const
{
    const FMingHistoricalCharacter* Character = Characters.Find(CharacterID);
    if (Character)
    {
        for (const FMingCharacterSkill& Skill : Character->Skills)
        {
            if (Skill.SkillType == SkillType)
            {
                return Skill;
            }
        }
    }
    
    return FMingCharacterSkill();
}

int32 UMingCharacterSystem::GetSkillLevel(const FString& CharacterID, EMingSkillType SkillType) const
{
    FMingCharacterSkill Skill = GetSkill(CharacterID, SkillType);
    return Skill.Level;
}

float UMingCharacterSystem::GetSkillBonus(const FString& CharacterID, EMingSkillType SkillType) const
{
    FMingCharacterSkill Skill = GetSkill(CharacterID, SkillType);
    return (float)Skill.Level / Skill.MaxLevel;
}

void UMingCharacterSystem::TrainCharacter(const FString& CharacterID, EMingSkillType SkillType, float Duration)
{
    // Calculate experience based on duration
    int32 Experience = FMath::FloorToInt(Duration * 10); // 10 exp per second
    AddSkillExperience(CharacterID, SkillType, Experience);
    
    UE_LOG(LogTemp, Log, TEXT("Training character %s in %s for %.1f seconds"), 
           *CharacterID, *GetSkillTypeName(SkillType), Duration);
}

void UMingCharacterSystem::PromoteCharacter(const FString& CharacterID)
{
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (!Character)
    {
        return;
    }
    
    // Increase influence
    Character->Influence += 10.0f;
    
    // Increase loyalty slightly
    Character->Loyalty = FMath::Min(Character->Loyalty + 5.0f, 100.0f);
    
    // Trigger event
    OnCharacterPromoted.Broadcast(CharacterID, FMath::FloorToInt(Character->Influence / 10.0f));
    
    UE_LOG(LogTemp, Log, TEXT("Character promoted: %s"), *Character->CharacterName);
}

void UMingCharacterSystem::DevelopTrait(const FString& CharacterID, const FString& TraitID)
{
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (!Character)
    {
        return;
    }
    
    // Check if character already has this trait
    for (const FMingCharacterTrait& Trait : Character->Traits)
    {
        if (Trait.TraitID == TraitID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Character already has trait: %s"), *TraitID);
            return;
        }
    }
    
    // Add new trait (in a real implementation, you'd have a trait database)
    UE_LOG(LogTemp, Log, TEXT("Character %s developed trait: %s"), *CharacterID, *TraitID);
    
    OnTraitDeveloped.Broadcast(CharacterID, TraitID);
}

float UMingCharacterSystem::GetCharacterExperience(const FString& CharacterID) const
{
    const FMingHistoricalCharacter* Character = Characters.Find(CharacterID);
    if (!Character)
    {
        return 0.0f;
    }
    
    // Calculate total experience from all skills
    int32 TotalExperience = 0;
    for (const FMingCharacterSkill& Skill : Character->Skills)
    {
        TotalExperience += Skill.Experience;
    }
    
    return (float)TotalExperience;
}

int32 UMingCharacterSystem::GetCharacterLevel(const FString& CharacterID) const
{
    const FMingHistoricalCharacter* Character = Characters.Find(CharacterID);
    if (!Character)
    {
        return 0;
    }
    
    // Calculate overall level from skills
    int32 TotalLevels = 0;
    for (const FMingCharacterSkill& Skill : Character->Skills)
    {
        TotalLevels += Skill.Level;
    }
    
    return Character->Skills.Num() > 0 ? TotalLevels / Character->Skills.Num() : 0;
}

void UMingCharacterSystem::SetCharacterLoyalty(const FString& CharacterID, float Loyalty)
{
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (Character)
    {
        Character->Loyalty = FMath::Clamp(Loyalty, 0.0f, 100.0f);
    }
}

void UMingCharacterSystem::ModifyRelationship(const FString& CharacterID1, const FString& CharacterID2, float Modifier)
{
    if (!CharacterRelationships.Contains(CharacterID1))
    {
        CharacterRelationships.Add(CharacterID1, TMap<FString, float>());
    }
    
    TMap<FString, float>& Relationships = CharacterRelationships[CharacterID1];
    float* CurrentRelationship = Relationships.Find(CharacterID2);
    
    if (CurrentRelationship)
    {
        *CurrentRelationship = FMath::Clamp(*CurrentRelationship + Modifier, -100.0f, 100.0f);
    }
    else
    {
        Relationships.Add(CharacterID2, FMath::Clamp(Modifier, -100.0f, 100.0f));
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Modified relationship: %s <-> %s by %.1f"), 
           *CharacterID1, *CharacterID2, Modifier);
}

float UMingCharacterSystem::GetCharacterLoyalty(const FString& CharacterID) const
{
    const FMingHistoricalCharacter* Character = Characters.Find(CharacterID);
    return Character ? Character->Loyalty : 0.0f;
}

float UMingCharacterSystem::GetRelationship(const FString& CharacterID1, const FString& CharacterID2) const
{
    const TMap<FString, float>* Relationships = CharacterRelationships.Find(CharacterID1);
    if (Relationships)
    {
        const float* Relationship = Relationships->Find(CharacterID2);
        return Relationship ? *Relationship : 0.0f;
    }
    
    return 0.0f;
}

TArray<FString> UMingCharacterSystem::GetActiveEffects(const FString& CharacterID) const
{
    const TArray<FString>* Effects = CharacterEffects.Find(CharacterID);
    return Effects ? *Effects : TArray<FString>();
}

void UMingCharacterSystem::ApplyCharacterEffect(const FString& CharacterID, const FString& EffectID)
{
    if (!CharacterEffects.Contains(CharacterID))
    {
        CharacterEffects.Add(CharacterID, TArray<FString>());
    }
    
    CharacterEffects[CharacterID].AddUnique(EffectID);
    UE_LOG(LogTemp, Log, TEXT("Applied effect %s to character %s"), *EffectID, *CharacterID);
}

void UMingCharacterSystem::RemoveCharacterEffect(const FString& CharacterID, const FString& EffectID)
{
    if (CharacterEffects.Contains(CharacterID))
    {
        CharacterEffects[CharacterID].Remove(EffectID);
        UE_LOG(LogTemp, Log, TEXT("Removed effect %s from character %s"), *EffectID, *CharacterID);
    }
}

void UMingCharacterSystem::ShowCharacterBiography(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetCharacter(CharacterID);
    if (!Character.CharacterID.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("=== %s ==="), *Character.CharacterName);
        UE_LOG(LogTemp, Log, TEXT("%s"), *Character.Biography);
        UE_LOG(LogTemp, Log, TEXT("歷史意義: %s"), *Character.HistoricalSignificance);
        UE_LOG(LogTemp, Log, TEXT("出生: %d, 逝世: %d"), Character.BirthYear, Character.DeathYear);
    }
}

void UMingCharacterSystem::AddHistoricalEvent(const FString& CharacterID, const FString& Event)
{
    if (!HistoricalEvents.Contains(CharacterID))
    {
        HistoricalEvents.Add(CharacterID, TArray<FString>());
    }
    
    HistoricalEvents[CharacterID].Add(Event);
}

TArray<FString> UMingCharacterSystem::GetCharacterEvents(const FString& CharacterID) const
{
    const TArray<FString>* Events = HistoricalEvents.Find(CharacterID);
    return Events ? *Events : TArray<FString>();
}

FString UMingCharacterSystem::GetRoleName(EMingCharacterRole Role)
{
    switch (Role)
    {
    case EMingCharacterRole::Leader: return TEXT("領袖");
    case EMingCharacterRole::Commander: return TEXT("指揮官");
    case EMingCharacterRole::Diplomat: return TEXT("外交家");
    case EMingCharacterRole::Scholar: return TEXT("學者");
    case EMingCharacterRole::Merchant: return TEXT("商人");
    case EMingCharacterRole::Spy: return TEXT("間諜");
    case EMingCharacterRole::Advisor: return TEXT("顧問");
    case EMingCharacterRole::Revolutionary: return TEXT("革命家");
    default: return TEXT("未知");
    }
}

FString UMingCharacterSystem::GetSkillTypeName(EMingSkillType SkillType)
{
    switch (SkillType)
    {
    case EMingSkillType::Leadership: return TEXT("領導力");
    case EMingSkillType::Military: return TEXT("軍事");
    case EMingSkillType::Diplomacy: return TEXT("外交");
    case EMingSkillType::Economics: return TEXT("經濟");
    case EMingSkillType::Culture: return TEXT("文化");
    case EMingSkillType::Intelligence: return TEXT("情報");
    case EMingSkillType::Administration: return TEXT("行政");
    case EMingSkillType::Combat: return TEXT("戰鬥");
    default: return TEXT("未知");
    }
}

FString UMingCharacterSystem::SaveCharacterData() const
{
    FString Result = TEXT("{\n");
    Result += TEXT("  \"recruited_characters\": [");
    
    bool First = true;
    for (const auto& Pair : Characters)
    {
        if (Pair.Value.bIsRecruited)
        {
            if (!First) Result += TEXT(",");
            Result += FString::Printf(TEXT("\"%s\""), *Pair.Key);
            First = false;
        }
    }
    
    Result += TEXT("],\n");
    Result += TEXT("  \"character_levels\": {\n");
    
    First = true;
    for (const auto& Pair : Characters)
    {
        if (Pair.Value.bIsRecruited)
        {
            if (!First) Result += TEXT(",\n");
            Result += FString::Printf(TEXT("    \"%s\": %d"), *Pair.Key, GetCharacterLevel(Pair.Key));
            First = false;
        }
    }
    
    Result += TEXT("\n  }\n");
    Result += TEXT("}\n");
    
    return Result;
}

void UMingCharacterSystem::LoadCharacterData(const FString& JsonString)
{
    // Parse JSON and restore character data
    UE_LOG(LogTemp, Log, TEXT("Loading character data"));
}

FMingHistoricalCharacter* UMingCharacterSystem::FindCharacter(const FString& CharacterID)
{
    return Characters.Find(CharacterID);
}

FMingCharacterSkill* UMingCharacterSystem::FindCharacterSkill(const FString& CharacterID, EMingSkillType SkillType)
{
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (!Character)
    {
        return nullptr;
    }
    
    for (FMingCharacterSkill& Skill : Character->Skills)
    {
        if (Skill.SkillType == SkillType)
        {
            return &Skill;
        }
    }
    
    return nullptr;
}

void UMingCharacterSystem::ProcessSkillExperience(FMingCharacterSkill& Skill, int32 Experience)
{
    Skill.Experience += Experience;
    
    // Handle multiple level ups
    while (Skill.Experience >= Skill.ExperienceToNext && !Skill.bIsMaxLevel)
    {
        Skill.Experience -= Skill.ExperienceToNext;
        Skill.Level++;
        Skill.ExperienceToNext = Skill.Level * 100;
        
        if (Skill.Level >= Skill.MaxLevel)
        {
            Skill.bIsMaxLevel = true;
            Skill.Experience = 0;
            Skill.ExperienceToNext = 0;
            break;
        }
    }
}

bool UMingCharacterSystem::CanLevelUpSkill(const FMingCharacterSkill& Skill) const
{
    return !Skill.bIsMaxLevel && Skill.Experience >= Skill.ExperienceToNext;
}

void UMingCharacterSystem::ApplySkillEffects(const FString& CharacterID, const FMingCharacterSkill& Skill)
{
    // Apply skill effects to character
    UE_LOG(LogTemp, Verbose, TEXT("Applying skill effects for %s - %s"), *CharacterID, *Skill.SkillName);
}

void UMingCharacterSystem::UpdateCharacterStats(const FString& CharacterID)
{
    // Update character stats based on skills and traits
    FMingHistoricalCharacter* Character = FindCharacter(CharacterID);
    if (!Character)
    {
        return;
    }
    
    // Calculate influence based on skills and traits
    float BaseInfluence = 0.0f;
    for (const FMingCharacterSkill& Skill : Character->Skills)
    {
        BaseInfluence += Skill.Level * 5.0f;
    }
    
    for (const FMingCharacterTrait& Trait : Character->Traits)
    {
        BaseInfluence *= Trait.InfluenceWeight;
    }
    
    Character->Influence = BaseInfluence;
}

void UMingCharacterSystem::CheckSkillPrerequisites(const FString& CharacterID, const FString& SkillID)
{
    // Check if character meets prerequisites for learning new skill
    UE_LOG(LogTemp, Log, TEXT("Checking skill prerequisites for %s - %s"), *CharacterID, *SkillID);
}

void UMingCharacterSystem::ProcessTraitEffects(const FString& CharacterID, const FMingCharacterTrait& Trait)
{
    // Process trait effects on character
    UE_LOG(LogTemp, Verbose, TEXT("Processing trait effects for %s - %s"), *CharacterID, *Trait.TraitName);
}
