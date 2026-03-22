#include "MingGoRTSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

AMingGoRTSCharacter::AMingGoRTSCharacter()
{
    PrimaryActorTick.bCanEverTick = false;

    // 初始化默認值
    CharacterName = TEXT("未命名軍官");
    Background = ECharacterBackground::MilitaryAcademy;
    Age = 25;
    Biography = TEXT("");
    
    Experience = 0;
    Level = 1;
    Reputation = 50.0f;
    
    AvailableAttributePoints = 10;
    MaxLevel = 50;
    ExperiencePerLevel = 1000;
}

void AMingGoRTSCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // 應用背景加成
    ApplyBackgroundBonuses();
}

void AMingGoRTSCharacter::InitializeCharacter(const FString& Name, ECharacterBackground CharBackground, const FCharacterAttributes& InitialAttributes)
{
    CharacterName = Name;
    Background = CharBackground;
    Attributes = InitialAttributes;
    
    // 應用背景加成
    ApplyBackgroundBonuses();
    
    // 根據背景設置初始傳記
    switch (Background)
    {
    case ECharacterBackground::MilitaryAcademy:
        Biography = TEXT("畢業於黃埔軍校，接受現代軍事教育，具備良好的戰術素養。");
        break;
    case ECharacterBackground::WarlordSon:
        Biography = TEXT("出身軍閥世家，從小耳濡目染軍事事務，擁有豐厚的人脈資源。");
        break;
    case ECharacterBackground::Revolutionary:
        Biography = TEXT("懷揣革命理想，為國家前途奮鬥的熱血青年。");
        break;
    case ECharacterBackground::ScholarOfficial:
        Biography = TEXT("棄文從武的讀書人，以智謀和策略見長。");
        break;
    case ECharacterBackground::Merchant:
        Biography = TEXT("富商從軍，善於理財和後勤管理。");
        break;
    case ECharacterBackground::CommonSoldier:
        Biography = TEXT("從基層士兵一步步成長起來，經驗豐富，深得士兵擁戴。");
        break;
    }
}

void AMingGoRTSCharacter::AllocateAttributePoints(float LeadershipDelta, float IntelligenceDelta, float CourageDelta, float CharismaDelta, float ConstitutionDelta)
{
    float TotalDelta = LeadershipDelta + IntelligenceDelta + CourageDelta + CharismaDelta + ConstitutionDelta;
    
    if (TotalDelta > AvailableAttributePoints)
    {
        UE_LOG(LogTemp, Warning, TEXT("屬性點不足！需要 %.1f 點，只有 %d 點"), TotalDelta, AvailableAttributePoints);
        return;
    }
    
    if (TotalDelta <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("必須分配正數的屬性點"));
        return;
    }
    
    // 檢查屬性範圍
    if (Attributes.Leadership + LeadershipDelta < 0 || Attributes.Leadership + LeadershipDelta > 100 ||
        Attributes.Intelligence + IntelligenceDelta < 0 || Attributes.Intelligence + IntelligenceDelta > 100 ||
        Attributes.Courage + CourageDelta < 0 || Attributes.Courage + CourageDelta > 100 ||
        Attributes.Charisma + CharismaDelta < 0 || Attributes.Charisma + CharismaDelta > 100 ||
        Attributes.Constitution + ConstitutionDelta < 0 || Attributes.Constitution + ConstitutionDelta > 100)
    {
        UE_LOG(LogTemp, Warning, TEXT("屬性值超出範圍（0-100）"));
        return;
    }
    
    // 分配屬性點
    Attributes.Leadership += LeadershipDelta;
    Attributes.Intelligence += IntelligenceDelta;
    Attributes.Courage += CourageDelta;
    Attributes.Charisma += CharismaDelta;
    Attributes.Constitution += ConstitutionDelta;
    
    AvailableAttributePoints -= static_cast<int32>(TotalDelta);
    
    UE_LOG(LogTemp, Log, TEXT("屬性分配完成！剩餘屬性點：%d"), AvailableAttributePoints);
}

void AMingGoRTSCharacter::AddSkill(const FCharacterSkill& NewSkill)
{
    // 檢查技能是否已存在
    for (FCharacterSkill& ExistingSkill : Skills)
    {
        if (ExistingSkill.SkillID == NewSkill.SkillID)
        {
            UE_LOG(LogTemp, Warning, TEXT("技能 %s 已存在"), *NewSkill.SkillName);
            return;
        }
    }
    
    Skills.Add(NewSkill);
    UE_LOG(LogTemp, Log, TEXT("添加技能：%s"), *NewSkill.SkillName);
}

bool AMingGoRTSCharacter::UpgradeSkill(const FName& SkillID)
{
    for (FCharacterSkill& Skill : Skills)
    {
        if (Skill.SkillID == SkillID)
        {
            if (Skill.Level >= Skill.MaxLevel)
            {
                UE_LOG(LogTemp, Warning, TEXT("技能 %s 已達到最高等級"), *Skill.SkillName);
                return false;
            }
            
            Skill.Level++;
            UE_LOG(LogTemp, Log, TEXT("技能 %s 升級到等級 %d"), *Skill.SkillName, Skill.Level);
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("未找到技能 ID：%s"), *SkillID.ToString());
    return false;
}

void AMingGoRTSCharacter::AddExperience(int32 ExpAmount)
{
    if (ExpAmount <= 0) return;
    
    Experience += ExpAmount;
    UE_LOG(LogTemp, Log, TEXT("獲得 %d 經驗值，總經驗：%d"), ExpAmount, Experience);
    
    // 檢查是否升級
    CalculateLevel();
}

void AMingGoRTSCharacter::CalculateLevel()
{
    int32 NewLevel = (Experience / ExperiencePerLevel) + 1;
    
    if (NewLevel > MaxLevel)
    {
        NewLevel = MaxLevel;
    }
    
    if (NewLevel > Level)
    {
        int32 LevelsGained = NewLevel - Level;
        Level = NewLevel;
        
        // 每升一級獲得屬性點
        AvailableAttributePoints += LevelsGained * 2;
        
        UE_LOG(LogTemp, Log, TEXT("升級！新等級：%d，獲得 %d 屬性點"), Level, LevelsGained * 2);
        
        OnLevelUp();
    }
}

float AMingGoRTSCharacter::GetAttributeModifier(ECharacterBackground InBackground) const
{
    switch (InBackground)
    {
    case ECharacterBackground::MilitaryAcademy:
        return 1.1f; // 統帥 +10%
    case ECharacterBackground::WarlordSon:
        return 1.15f; // 魅力 +15%
    case ECharacterBackground::Revolutionary:
        return 1.2f; // 勇武 +20%
    case ECharacterBackground::ScholarOfficial:
        return 1.25f; // 智謀 +25%
    case ECharacterBackground::Merchant:
        return 1.1f; // 體質 +10%
    case ECharacterBackground::CommonSoldier:
        return 1.05f; // 全屬性 +5%
    default:
        return 1.0f;
    }
}

void AMingGoRTSCharacter::SaveCharacterData()
{
    // TODO: 實現角色數據保存到存檔系統
    UE_LOG(LogTemp, Log, TEXT("保存角色數據：%s"), *CharacterName);
}

bool AMingGoRTSCharacter::LoadCharacterData(const FString& SaveSlotName)
{
    // TODO: 實現從存檔系統載入角色數據
    UE_LOG(LogTemp, Log, TEXT("載入角色數據從存檔：%s"), *SaveSlotName);
    return true;
}

void AMingGoRTSCharacter::OnLevelUp()
{
    // 升級時的特效和音效
    if (UGameplayStatics::IsValidLowLevel())
    {
        // TODO: 播放升級特效
    }
    
    // 可以在這裡添加升級時的特殊邏輯
    Reputation += 5.0f;
}

void AMingGoRTSCharacter::ApplyBackgroundBonuses()
{
    float Modifier = GetAttributeModifier(Background);
    
    switch (Background)
    {
    case ECharacterBackground::MilitaryAcademy:
        Attributes.Leadership *= Modifier;
        break;
    case ECharacterBackground::WarlordSon:
        Attributes.Charisma *= Modifier;
        break;
    case ECharacterBackground::Revolutionary:
        Attributes.Courage *= Modifier;
        break;
    case ECharacterBackground::ScholarOfficial:
        Attributes.Intelligence *= Modifier;
        break;
    case ECharacterBackground::Merchant:
        Attributes.Constitution *= Modifier;
        break;
    case ECharacterBackground::CommonSoldier:
        // 全屬性小幅提升
        Attributes.Leadership *= Modifier;
        Attributes.Intelligence *= Modifier;
        Attributes.Courage *= Modifier;
        Attributes.Charisma *= Modifier;
        Attributes.Constitution *= Modifier;
        break;
    }
    
    // 確保屬性不超過最大值
    Attributes.Leadership = FMath::Clamp(Attributes.Leadership, 0.0f, 100.0f);
    Attributes.Intelligence = FMath::Clamp(Attributes.Intelligence, 0.0f, 100.0f);
    Attributes.Courage = FMath::Clamp(Attributes.Courage, 0.0f, 100.0f);
    Attributes.Charisma = FMath::Clamp(Attributes.Charisma, 0.0f, 100.0f);
    Attributes.Constitution = FMath::Clamp(Attributes.Constitution, 0.0f, 100.0f);
}
