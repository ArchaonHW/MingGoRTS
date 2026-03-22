#include "MingGoRTSSkillSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMingGoRTSSkillSystem::UMingGoRTSSkillSystem()
{
    CurrentSpecializationPath = ESpecializationPath::Generalist;
    AvailableSkillPoints = 0;
    bIsInitialized = false;
}

void UMingGoRTSSkillSystem::InitializeSkillSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能系統已經初始化"));
        return;
    }

    // 初始化技能庫
    InitializeSkillLibrary();

    // 初始化技能樹
    InitializeSkillTrees();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("技能系統初始化完成，共載入 %d 個技能"), AllSkills.Num());
}

TArray<FMingSkill> UMingGoRTSSkillSystem::GetAllSkills() const
{
    return AllSkills;
}

FMingSkill UMingGoRTSSkillSystem::GetSkill(const FName& SkillID) const
{
    if (const FMingSkill* Skill = SkillMap.Find(SkillID))
    {
        return *Skill;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到技能 ID：%s"), *SkillID.ToString());
    return FMingSkill();
}

bool UMingGoRTSSkillSystem::UnlockSkill(const FName& SkillID)
{
    if (!SkillMap.Contains(SkillID))
    {
        UE_LOG(LogTemp, Error, TEXT("技能不存在：%s"), *SkillID.ToString());
        return false;
    }

    FMingSkill& Skill = SkillMap[SkillID];
    if (Skill.bIsUnlocked)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能已解鎖：%s"), *SkillID.ToString());
        return false;
    }

    // 檢查技能點數
    if (AvailableSkillPoints <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能點數不足，無法解鎖技能：%s"), *SkillID.ToString());
        return false;
    }

    // 解鎖技能
    Skill.bIsUnlocked = true;
    Skill.CurrentLevel = 1;
    Skill.CurrentExperience = 0;
    AvailableSkillPoints--;

    // 應用技能效果
    ApplySkillEffects(SkillID);

    // 廣播技能解鎖事件
    OnSkillUnlocked.Broadcast(SkillID, Skill);

    UE_LOG(LogTemp, Log, TEXT("技能解鎖成功：%s，剩餘技能點：%d"), *SkillID.ToString(), AvailableSkillPoints);
    return true;
}

bool UMingGoRTSSkillSystem::UpgradeSkill(const FName& SkillID, int32 ExperienceAmount)
{
    if (!SkillMap.Contains(SkillID))
    {
        UE_LOG(LogTemp, Error, TEXT("技能不存在：%s"), *SkillID.ToString());
        return false;
    }

    FMingSkill& Skill = SkillMap[SkillID];
    if (!Skill.bIsUnlocked)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能未解鎖，無法升級：%s"), *SkillID.ToString());
        return false;
    }

    if (Skill.CurrentLevel >= Skill.MaxLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能已達最高等級：%s"), *SkillID.ToString());
        return false;
    }

    // 添加經驗值
    if (ExperienceAmount > 0)
    {
        AddSkillExperience(SkillID, ExperienceAmount);
    }

    // 檢查是否可以升級
    if (Skill.CurrentExperience >= Skill.ExperienceToNextLevel)
    {
        Skill.CurrentLevel++;
        Skill.CurrentExperience = 0;
        Skill.ExperienceToNextLevel = CalculateExperienceRequirement(Skill.CurrentLevel);

        // 應用技能效果
        ApplySkillEffects(SkillID);

        // 廣播升級事件
        OnSkillLevelUp.Broadcast(SkillID, Skill.CurrentLevel);

        UE_LOG(LogTemp, Log, TEXT("技能升級：%s -> 等級 %d"), *SkillID.ToString(), Skill.CurrentLevel);
        return true;
    }

    return false;
}

void UMingGoRTSSkillSystem::AddSkillExperience(const FName& SkillID, int32 ExperienceAmount)
{
    if (!SkillMap.Contains(SkillID))
    {
        UE_LOG(LogTemp, Warning, TEXT("技能不存在：%s"), *SkillID.ToString());
        return;
    }

    FMingSkill& Skill = SkillMap[SkillID];
    if (!Skill.bIsUnlocked)
    {
        return;
    }

    Skill.CurrentExperience += ExperienceAmount;
    UE_LOG(LogTemp, Log, TEXT("技能 %s 獲得 %d 經驗，當前：%d/%d"), 
           *SkillID.ToString(), ExperienceAmount, Skill.CurrentExperience, Skill.ExperienceToNextLevel);

    // 自動檢查升級
    UpgradeSkill(SkillID, 0);
}

bool UMingGoRTSSkillSystem::CanUnlockSkill(const FName& SkillID, const FMingCharacterData& CharacterData) const
{
    if (!SkillMap.Contains(SkillID))
    {
        return false;
    }

    const FMingSkill& Skill = SkillMap[SkillID];
    if (Skill.bIsUnlocked)
    {
        return false;
    }

    // 檢查技能點數
    if (AvailableSkillPoints <= 0)
    {
        return false;
    }

    // 檢查所有需求
    for (const FSkillRequirement& Requirement : Skill.Requirements)
    {
        if (!ValidateSkillRequirements(Requirement, CharacterData))
        {
            return false;
        }
    }

    return true;
}

bool UMingGoRTSSkillSystem::CanUpgradeSkill(const FName& SkillID) const
{
    if (!SkillMap.Contains(SkillID))
    {
        return false;
    }

    const FMingSkill& Skill = SkillMap[SkillID];
    return Skill.bIsUnlocked && Skill.CurrentLevel < Skill.MaxLevel;
}

TArray<FSkillTree> UMingGoRTSSkillSystem::GetSkillTrees() const
{
    return SkillTrees;
}

void UMingGoRTSSkillSystem::ChooseSpecializationPath(ESpecializationPath Path)
{
    if (CurrentSpecializationPath != ESpecializationPath::Generalist)
    {
        UE_LOG(LogTemp, Warning, TEXT("已經選擇了專精路徑，無法更改"));
        return;
    }

    CurrentSpecializationPath = Path;

    // 根據專精路徑給予初始技能點和技能
    switch (Path)
    {
    case ESpecializationPath::Commander:
        AvailableSkillPoints += 3;
        break;
    case ESpecializationPath::Tactician:
        AvailableSkillPoints += 2;
        break;
    case ESpecializationPath::Diplomat:
        AvailableSkillPoints += 2;
        break;
    case ESpecializationPath::Spy:
        AvailableSkillPoints += 2;
        break;
    case ESpecializationPath::Economist:
        AvailableSkillPoints += 2;
        break;
    case ESpecializationPath::Generalist:
        AvailableSkillPoints += 1;
        break;
    }

    // 廣播專精路徑選擇事件
    OnSpecializationChosen.Broadcast(Path);

    UE_LOG(LogTemp, Log, TEXT("選擇專精路徑：%s，獲得 %d 技能點"), 
           *StaticEnum<ESpecializationPath>()->GetValueAsString(Path), AvailableSkillPoints);
}

ESpecializationPath UMingGoRTSSkillSystem::GetCurrentSpecializationPath() const
{
    return CurrentSpecializationPath;
}

float UMingGoRTSSkillSystem::CalculateSkillEffect(const FName& SkillID, const FString& EffectType) const
{
    if (!SkillMap.Contains(SkillID))
    {
        return 0.0f;
    }

    const FMingSkill& Skill = SkillMap[SkillID];
    if (!Skill.bIsUnlocked)
    {
        return 0.0f;
    }

    float TotalEffect = 0.0f;
    for (const FSkillEffect& Effect : Skill.Effects)
    {
        if (Effect.EffectType == EffectType)
        {
            TotalEffect += Effect.BaseValue + (Effect.ScalingPerLevel * (Skill.CurrentLevel - 1));
        }
    }

    return TotalEffect;
}

TArray<FMingSkill> UMingGoRTSSkillSystem::GetUnlockedSkills() const
{
    TArray<FMingSkill> UnlockedSkills;
    
    for (const FMingSkill& Skill : AllSkills)
    {
        if (Skill.bIsUnlocked)
        {
            UnlockedSkills.Add(Skill);
        }
    }
    
    return UnlockedSkills;
}

TArray<FMingSkill> UMingGoRTSSkillSystem::GetAvailableSkills(const FMingCharacterData& CharacterData) const
{
    TArray<FMingSkill> AvailableSkills;
    
    for (const FMingSkill& Skill : AllSkills)
    {
        if (!Skill.bIsUnlocked && CanUnlockSkill(Skill.SkillID, CharacterData))
        {
            AvailableSkills.Add(Skill);
        }
    }
    
    return AvailableSkills;
}

void UMingGoRTSSkillSystem::ResetSkillSystem()
{
    for (FMingSkill& Skill : AllSkills)
    {
        Skill.bIsUnlocked = false;
        Skill.CurrentLevel = 0;
        Skill.CurrentExperience = 0;
        Skill.bIsActive = false;
    }

    CurrentSpecializationPath = ESpecializationPath::Generalist;
    AvailableSkillPoints = 0;

    UE_LOG(LogTemp, Log, TEXT("技能系統已重置"));
}

bool UMingGoRTSSkillSystem::SaveSkillData(const FString& SaveSlotName)
{
    // TODO: 實現技能數據保存
    UE_LOG(LogTemp, Log, TEXT("保存技能數據到：%s"), *SaveSlotName);
    return true;
}

bool UMingGoRTSSkillSystem::LoadSkillData(const FString& SaveSlotName)
{
    // TODO: 實現技能數據載入
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入技能數據"), *SaveSlotName);
    return true;
}

void UMingGoRTSSkillSystem::InitializeSkillLibrary()
{
    CreateCombatSkills();
    CreateStrategySkills();
    CreateDiplomacySkills();
    CreateLeadershipSkills();
    CreateIntelligenceSkills();
    CreateEconomicSkills();

    // 構建技能映射
    SkillMap.Empty();
    for (const FMingSkill& Skill : AllSkills)
    {
        SkillMap.Add(Skill.SkillID, Skill);
    }
}

void UMingGoRTSSkillSystem::CreateCombatSkills()
{
    // 戰術指揮
    FMingSkill TacticalCommand;
    TacticalCommand.SkillID = GenerateSkillID(TEXT("TacticalCommand"), ESkillCategory::Combat);
    TacticalCommand.SkillName = TEXT("戰術指揮");
    TacticalCommand.Description = TEXT("提升部隊指揮效率，增強戰鬥協調能力");
    TacticalCommand.Category = ESkillCategory::Combat;
    TacticalCommand.SkillType = ESkillType::Passive;
    TacticalCommand.SpecializationPath = ESpecializationPath::Commander;
    TacticalCommand.MaxLevel = 5;
    
    FSkillEffect CommandEffect;
    CommandEffect.EffectType = TEXT("LeadershipBonus");
    CommandEffect.BaseValue = 5.0f;
    CommandEffect.ScalingPerLevel = 3.0f;
    CommandEffect.TargetAttribute = TEXT("Leadership");
    CommandEffect.Description = TEXT("統帥屬性加成");
    TacticalCommand.Effects.Add(CommandEffect);
    
    AllSkills.Add(TacticalCommand);

    // 近戰精通
    FMingSkill MeleeMastery;
    MeleeMastery.SkillID = GenerateSkillID(TEXT("MeleeMastery"), ESkillCategory::Combat);
    MeleeMastery.SkillName = TEXT("近戰精通");
    MeleeMastery.Description = TEXT("提升個人近戰能力和士氣影響");
    MeleeMastery.Category = ESkillCategory::Combat;
    MeleeMastery.SkillType = ESkillType::Active;
    MeleeMastery.SpecializationPath = ESpecializationPath::Generalist;
    MeleeMastery.MaxLevel = 3;
    
    FSkillEffect MeleeEffect;
    MeleeEffect.EffectType = TEXT("CourageBonus");
    MeleeEffect.BaseValue = 8.0f;
    MeleeEffect.ScalingPerLevel = 4.0f;
    MeleeEffect.TargetAttribute = TEXT("Courage");
    MeleeEffect.Description = TEXT("勇武屬性加成");
    MeleeMastery.Effects.Add(MeleeEffect);
    
    AllSkills.Add(MeleeMastery);
}

void UMingGoRTSSkillSystem::CreateStrategySkills()
{
    // 戰略規劃
    FMingSkill StrategicPlanning;
    StrategicPlanning.SkillID = GenerateSkillID(TEXT("StrategicPlanning"), ESkillCategory::Strategy);
    StrategicPlanning.SkillName = TEXT("戰略規劃");
    StrategicPlanning.Description = TEXT("提升長期戰略思維和資源規劃能力");
    StrategicPlanning.Category = ESkillCategory::Strategy;
    StrategicPlanning.SkillType = ESkillType::Passive;
    StrategicPlanning.SpecializationPath = ESpecializationPath::Tactician;
    StrategicPlanning.MaxLevel = 5;
    
    FSkillEffect StrategyEffect;
    StrategyEffect.EffectType = TEXT("IntelligenceBonus");
    StrategyEffect.BaseValue = 6.0f;
    StrategyEffect.ScalingPerLevel = 2.5f;
    StrategyEffect.TargetAttribute = TEXT("Intelligence");
    StrategyEffect.Description = TEXT("智謀屬性加成");
    StrategicPlanning.Effects.Add(StrategyEffect);
    
    AllSkills.Add(StrategicPlanning);
}

void UMingGoRTSSkillSystem::CreateDiplomacySkills()
{
    // 外交手腕
    FMingSkill DiplomacySkill;
    DiplomacySkill.SkillID = GenerateSkillID(TEXT("Diplomacy"), ESkillCategory::Diplomacy);
    DiplomacySkill.SkillName = TEXT("外交手腕");
    DiplomacySkill.Description = TEXT("提升談判和聯盟能力，增強外交影響力");
    DiplomacySkill.Category = ESkillCategory::Diplomacy;
    DiplomacySkill.SkillType = ESkillType::Active;
    DiplomacySkill.SpecializationPath = ESpecializationPath::Diplomat;
    DiplomacySkill.MaxLevel = 5;
    
    FSkillEffect DiplomacyEffect;
    DiplomacyEffect.EffectType = TEXT("CharismaBonus");
    DiplomacyEffect.BaseValue = 7.0f;
    DiplomacyEffect.ScalingPerLevel = 3.0f;
    DiplomacyEffect.TargetAttribute = TEXT("Charisma");
    DiplomacyEffect.Description = TEXT("魅力屬性加成");
    DiplomacySkill.Effects.Add(DiplomacyEffect);
    
    AllSkills.Add(DiplomacySkill);
}

void UMingGoRTSSkillSystem::CreateLeadershipSkills()
{
    // 領導力
    FMingSkill LeadershipSkill;
    LeadershipSkill.SkillID = GenerateSkillID(TEXT("Leadership"), ESkillCategory::Leadership);
    LeadershipSkill.SkillName = TEXT("領導力");
    LeadershipSkill.Description = TEXT("提升部隊士氣和指揮效率");
    LeadershipSkill.Category = ESkillCategory::Leadership;
    LeadershipSkill.SkillType = ESkillType::Passive;
    LeadershipSkill.SpecializationPath = ESpecializationPath::Commander;
    LeadershipSkill.MaxLevel = 5;
    
    FSkillEffect LeadershipEffect;
    LeadershipEffect.EffectType = TEXT("MoraleBonus");
    LeadershipEffect.BaseValue = 10.0f;
    LeadershipEffect.ScalingPerLevel = 2.0f;
    LeadershipEffect.TargetAttribute = TEXT("Morale");
    LeadershipEffect.Description = TEXT("士氣加成");
    LeadershipSkill.Effects.Add(LeadershipEffect);
    
    AllSkills.Add(LeadershipSkill);
}

void UMingGoRTSSkillSystem::CreateIntelligenceSkills()
{
    // 情報分析
    FMingSkill IntelligenceAnalysis;
    IntelligenceAnalysis.SkillID = GenerateSkillID(TEXT("IntelligenceAnalysis"), ESkillCategory::Intelligence);
    IntelligenceAnalysis.SkillName = TEXT("情報分析");
    IntelligenceAnalysis.Description = TEXT("提升情報處理和分析能力");
    IntelligenceAnalysis.Category = ESkillCategory::Intelligence;
    IntelligenceAnalysis.SkillType = ESkillType::Active;
    IntelligenceAnalysis.SpecializationPath = ESpecializationPath::Spy;
    IntelligenceAnalysis.MaxLevel = 4;
    
    FSkillEffect IntelligenceEffect;
    IntelligenceEffect.EffectType = TEXT("IntelBonus");
    IntelligenceEffect.BaseValue = 5.0f;
    IntelligenceEffect.ScalingPerLevel = 3.5f;
    IntelligenceEffect.TargetAttribute = TEXT("Intelligence");
    IntelligenceEffect.Description = TEXT("情報能力加成");
    IntelligenceAnalysis.Effects.Add(IntelligenceEffect);
    
    AllSkills.Add(IntelligenceAnalysis);
}

void UMingGoRTSSkillSystem::CreateEconomicSkills()
{
    // 經濟管理
    FMingSkill EconomicManagement;
    EconomicManagement.SkillID = GenerateSkillID(TEXT("EconomicManagement"), ESkillCategory::Economic);
    EconomicManagement.SkillName = TEXT("經濟管理");
    EconomicManagement.Description = TEXT("提升資源管理和經濟運營效率");
    EconomicManagement.Category = ESkillCategory::Economic;
    EconomicManagement.SkillType = ESkillType::Passive;
    EconomicManagement.SpecializationPath = ESpecializationPath::Economist;
    EconomicManagement.MaxLevel = 5;
    
    FSkillEffect EconomicEffect;
    EconomicEffect.EffectType = TEXT("ResourceBonus");
    EconomicEffect.BaseValue = 8.0f;
    EconomicEffect.ScalingPerLevel = 2.5f;
    EconomicEffect.TargetAttribute = TEXT("ResourceEfficiency");
    EconomicEffect.Description = TEXT("資源效率加成");
    EconomicManagement.Effects.Add(EconomicEffect);
    
    AllSkills.Add(EconomicManagement);
}

void UMingGoRTSSkillSystem::InitializeSkillTrees()
{
    // 指揮官技能樹
    FSkillTree CommanderTree;
    CommanderTree.Path = ESpecializationPath::Commander;
    CommanderTree.PathName = TEXT("指揮官路徑");
    CommanderTree.PathDescription = TEXT("專注於部隊指揮和戰術領導");
    CommanderTree.PathColor = FLinearColor(1.0f, 0.2f, 0.2f); // 紅色
    CommanderTree.RootSkills.Add(FName("TacticalCommand"));
    CommanderTree.RootSkills.Add(FName("Leadership"));
    
    SkillTrees.Add(CommanderTree);

    // 戰術家技能樹
    FSkillTree TacticianTree;
    TacticianTree.Path = ESpecializationPath::Tactician;
    TacticianTree.PathName = TEXT("戰術家路徑");
    TacticianTree.PathDescription = TEXT("專注於戰略規劃和戰術執行");
    TacticianTree.PathColor = FLinearColor(0.2f, 0.2f, 1.0f); // 藍色
    TacticianTree.RootSkills.Add(FName("StrategicPlanning"));
    
    SkillTrees.Add(TacticianTree);

    // 外交官技能樹
    FSkillTree DiplomatTree;
    DiplomatTree.Path = ESpecializationPath::Diplomat;
    DiplomatTree.PathName = TEXT("外交官路徑");
    DiplomatTree.PathDescription = TEXT("專注於外交談判和政治聯盟");
    DiplomatTree.PathColor = FLinearColor(0.2f, 1.0f, 0.2f); // 綠色
    DiplomatTree.RootSkills.Add(FName("Diplomacy"));
    
    SkillTrees.Add(DiplomatTree);

    // 間諜技能樹
    FSkillTree SpyTree;
    SpyTree.Path = ESpecializationPath::Spy;
    SpyTree.PathName = TEXT("間諜路徑");
    SpyTree.PathDescription = TEXT("專注於情報收集和秘密行動");
    SpyTree.PathColor = FLinearColor(0.8f, 0.2f, 0.8f); // 紫色
    SpyTree.RootSkills.Add(FName("IntelligenceAnalysis"));
    
    SkillTrees.Add(SpyTree);

    // 經濟家技能樹
    FSkillTree EconomistTree;
    EconomistTree.Path = ESpecializationPath::Economist;
    EconomistTree.PathName = TEXT("經濟家路徑");
    EconomistTree.PathDescription = TEXT("專注於資源管理和經濟發展");
    EconomistTree.PathColor = FLinearColor(1.0f, 0.8f, 0.2f); // 金色
    EconomistTree.RootSkills.Add(FName("EconomicManagement"));
    
    SkillTrees.Add(EconomistTree);

    // 通才技能樹
    FSkillTree GeneralistTree;
    GeneralistTree.Path = ESpecializationPath::Generalist;
    GeneralistTree.PathName = TEXT("通才路徑");
    GeneralistTree.PathDescription = TEXT("平衡發展各項能力");
    GeneralistTree.PathColor = FLinearColor(0.7f, 0.7f, 0.7f); // 灰色
    GeneralistTree.RootSkills.Add(FName("MeleeMastery"));
    
    SkillTrees.Add(GeneralistTree);
}

bool UMingGoRTSSkillSystem::ValidateSkillRequirements(const FSkillRequirement& Requirement, const FMingCharacterData& CharacterData) const
{
    // 檢查角色等級
    if (CharacterData.Age < Requirement.MinimumCharacterLevel)
    {
        return false;
    }

    // 檢查背景需求
    if (Requirement.RequiredBackground != ECharacterBackground::MilitaryAcademy && 
        Requirement.RequiredBackground != CharacterData.Background)
    {
        return false;
    }

    // 檢查前置技能
    if (Requirement.RequiredSkillID != NAME_None)
    {
        if (!SkillMap.Contains(Requirement.RequiredSkillID))
        {
            return false;
        }

        const FMingSkill& RequiredSkill = SkillMap[Requirement.RequiredSkillID];
        if (!RequiredSkill.bIsUnlocked || RequiredSkill.CurrentLevel < Requirement.RequiredLevel)
        {
            return false;
        }
    }

    return true;
}

int32 UMingGoRTSSkillSystem::CalculateExperienceRequirement(int32 SkillLevel) const
{
    // 經驗需求公式：基礎值 * (等級 ^ 1.5)
    return static_cast<int32>(100.0f * FMath::Pow(SkillLevel, 1.5f));
}

void UMingGoRTSSkillSystem::ApplySkillEffects(const FName& SkillID)
{
    // TODO: 實現技能效果應用到角色屬性
    UE_LOG(LogTemp, Log, TEXT("應用技能效果：%s"), *SkillID.ToString());
}

void UMingGoRTSSkillSystem::UnlockConnectedSkills(const FName& SkillID)
{
    // TODO: 實現連接技能的自動解鎖邏輯
    UE_LOG(LogTemp, Log, TEXT("檢查連接技能解鎖：%s"), *SkillID.ToString());
}

FLinearColor UMingGoRTSSkillSystem::GetSpecializationPathColor(ESpecializationPath Path) const
{
    switch (Path)
    {
    case ESpecializationPath::Commander:
        return FLinearColor(1.0f, 0.2f, 0.2f);
    case ESpecializationPath::Tactician:
        return FLinearColor(0.2f, 0.2f, 1.0f);
    case ESpecializationPath::Diplomat:
        return FLinearColor(0.2f, 1.0f, 0.2f);
    case ESpecializationPath::Spy:
        return FLinearColor(0.8f, 0.2f, 0.8f);
    case ESpecializationPath::Economist:
        return FLinearColor(1.0f, 0.8f, 0.2f);
    case ESpecializationPath::Generalist:
        return FLinearColor(0.7f, 0.7f, 0.7f);
    default:
        return FLinearColor::White;
    }
}

FName UMingGoRTSSkillSystem::GenerateSkillID(const FString& BaseName, ESkillCategory Category) const
{
    FString CategoryPrefix;
    switch (Category)
    {
    case ESkillCategory::Combat:
        CategoryPrefix = TEXT("COM");
        break;
    case ESkillCategory::Strategy:
        CategoryPrefix = TEXT("STR");
        break;
    case ESkillCategory::Diplomacy:
        CategoryPrefix = TEXT("DIP");
        break;
    case ESkillCategory::Leadership:
        CategoryPrefix = TEXT("LED");
        break;
    case ESkillCategory::Intelligence:
        CategoryPrefix = TEXT("INT");
        break;
    case ESkillCategory::Economic:
        CategoryPrefix = TEXT("ECO");
        break;
    default:
        CategoryPrefix = TEXT("GEN");
        break;
    }

    return FName(*FString::Printf(TEXT("%s_%s"), *CategoryPrefix, *BaseName));
}
