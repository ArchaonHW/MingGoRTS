#include "SageBrain/MingSageCharacterSystem.h"
#include "Engine/Engine.h"
#include "Kismet/KismetSystemLibrary.h"

UMingSageCharacterSystem::UMingSageCharacterSystem()
{
    CurrentCharacterType = ESageCharacterType::None;
    bIsInitialized = false;
    ExperienceMultiplier = 1.0f;
    MaxCharacterLevel = 100;
    
    InitializeDefaultSkills();
}

void UMingSageCharacterSystem::InitializeCharacter(ESageCharacterType CharacterType)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character already initialized"));
        return;
    }

    CurrentCharacterType = CharacterType;
    CharacterProgression = FSageCharacterProgression();
    
    ApplyCharacterModifiers();
    
    bIsInitialized = true;
    
    OnCharacterInitialized(CharacterType);
    
    UE_LOG(LogTemp, Log, TEXT("Sage Character initialized as type: %d"), (int32)CharacterType);
}

void UMingSageCharacterSystem::SetCharacterType(ESageCharacterType NewType)
{
    if (CurrentCharacterType == NewType)
    {
        return;
    }

    ESageCharacterType OldType = CurrentCharacterType;
    CurrentCharacterType = NewType;
    
    ApplyCharacterModifiers();
    
    OnCharacterTypeChanged.Broadcast(OldType, NewType);
    
    UE_LOG(LogTemp, Log, TEXT("Character type changed from %d to %d"), (int32)OldType, (int32)NewType);
}

void UMingSageCharacterSystem::AddExperience(float Amount)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character not initialized"));
        return;
    }

    float ModifiedAmount = Amount * ExperienceMultiplier;
    CharacterProgression.Experience += ModifiedAmount;
    
    CheckForLevelUp();
    
    UE_LOG(LogTemp, Log, TEXT("Added %.2f experience (total: %.2f)"), ModifiedAmount, CharacterProgression.Experience);
}

bool UMingSageCharacterSystem::UnlockAbility(const FString& AbilityName)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (HasAbility(AbilityName))
    {
        UE_LOG(LogTemp, Warning, TEXT("Ability %s already unlocked"), *AbilityName);
        return false;
    }

    CharacterProgression.UnlockedAbilities.Add(AbilityName);
    OnAbilityUnlocked.Broadcast(AbilityName);
    OnAbilityLearned(AbilityName);
    
    UE_LOG(LogTemp, Log, TEXT("Unlocked ability: %s"), *AbilityName);
    return true;
}

void UMingSageCharacterSystem::UpgradeSkill(const FString& SkillName, float Amount)
{
    if (!bIsInitialized)
    {
        return;
    }

    float& CurrentLevel = CharacterProgression.SkillLevels.FindOrAdd(SkillName, 0.0f);
    CurrentLevel = FMath::Clamp(CurrentLevel + Amount, 0.0f, 100.0f);
    
    UE_LOG(LogTemp, Log, TEXT("Upgraded skill %s to %.2f"), *SkillName, CurrentLevel);
}

bool UMingSageCharacterSystem::HasAbility(const FString& AbilityName) const
{
    return CharacterProgression.UnlockedAbilities.Contains(AbilityName);
}

float UMingSageCharacterSystem::GetSkillLevel(const FString& SkillName) const
{
    if (const float* Level = CharacterProgression.SkillLevels.Find(SkillName))
    {
        return *Level;
    }
    return 0.0f;
}

void UMingSageCharacterSystem::ApplyCharacterModifiers()
{
    switch (CurrentCharacterType)
    {
    case ESageCharacterType::Sage:
        ApplySageTraits();
        break;
    case ESageCharacterType::DemonKing:
        ApplyDemonKingTraits();
        break;
    case ESageCharacterType::FalseSage:
        ApplyFalseSageTraits();
        break;
    default:
        break;
    }
    
    OnTraitsModified(CurrentTraits);
}

TArray<FString> UMingSageCharacterSystem::GetAvailableAbilities() const
{
    TArray<FString> AvailableAbilities;
    
    switch (CurrentCharacterType)
    {
    case ESageCharacterType::Sage:
        AvailableAbilities = {"聖光加持", "道德感召", "智慧啟迪", "民心歸向", "正義審判"};
        break;
    case ESageCharacterType::DemonKing:
        AvailableAbilities = {"魔威震懾", "黑暗統治", "恐懼散播", "邪惡腐化", "毀滅指令"};
        break;
    case ESageCharacterType::FalseSage:
        AvailableAbilities = {"偽善欺騙", "陰謀策劃", "暗中操控", "背叛突襲", "虛假宣傳"};
        break;
    default:
        break;
    }
    
    return AvailableAbilities;
}

bool UMingSageCharacterSystem::CanEvolveToNextStage() const
{
    if (!bIsInitialized || CharacterProgression.CurrentLevel >= MaxCharacterLevel)
    {
        return false;
    }
    
    return CharacterProgression.CurrentLevel >= 10 && CharacterProgression.CurrentLevel % 10 == 0;
}

void UMingSageCharacterSystem::EvolveCharacter()
{
    if (!CanEvolveToNextStage())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot evolve character at this time"));
        return;
    }

    UnlockAbility("進化能力");
    
    CurrentTraits.MoralAuthority += 5.0f;
    CurrentTraits.StrategicThinking += 5.0f;
    CurrentTraits.MilitaryCommand += 5.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Character evolved to next stage"));
}

FString UMingSageCharacterSystem::GetCharacterDescription() const
{
    switch (CurrentCharacterType)
    {
    case ESageCharacterType::Sage:
        return TEXT("聖者：以道德和智慧引導眾人，擁有強大的感召力和正義力量。");
    case ESageCharacterType::DemonKing:
        return TEXT("魔王：以恐懼和力量統治，擅長黑暗魔法和心理戰術。");
    case ESageCharacterType::FalseSage:
        return TEXT("偽聖者：外表聖潔內心邪惡，精通欺騙和陰謀術。");
    default:
        return TEXT("未知角色類型");
    }
}

FString UMingSageCharacterSystem::GetCharacterTitle() const
{
    switch (CurrentCharacterType)
    {
    case ESageCharacterType::Sage:
        return FString::Printf(TEXT("第%d級聖者"), CharacterProgression.CurrentLevel);
    case ESageCharacterType::DemonKing:
        return FString::Printf(TEXT("第%d級魔王"), CharacterProgression.CurrentLevel);
    case ESageCharacterType::FalseSage:
        return FString::Printf(TEXT("第%d級偽聖者"), CharacterProgression.CurrentLevel);
    default:
        return TEXT("無職位");
    }
}

void UMingSageCharacterSystem::ApplySageTraits()
{
    CurrentTraits = FSageCharacterTraits();
    CurrentTraits.CharacterType = ESageCharacterType::Sage;
    CurrentTraits.MoralAuthority = 85.0f;
    CurrentTraits.StrategicThinking = 75.0f;
    CurrentTraits.MilitaryCommand = 60.0f;
    CurrentTraits.CharismaBonus = 30.0f;
    CurrentTraits.LearningSpeed = 1.5f;
    CurrentTraits.DecisionSpeed = 1.2f;
    CurrentTraits.RiskTolerance = 0.3f;
    CurrentTraits.MaxFollowers = 500;
    CurrentTraits.SpecialAbilities = {"道德感召", "智慧啟迪", "民心歸向"};
}

void UMingSageCharacterSystem::ApplyDemonKingTraits()
{
    CurrentTraits = FSageCharacterTraits();
    CurrentTraits.CharacterType = ESageCharacterType::DemonKing;
    CurrentTraits.MoralAuthority = 20.0f;
    CurrentTraits.StrategicThinking = 80.0f;
    CurrentTraits.MilitaryCommand = 90.0f;
    CurrentTraits.CharismaBonus = -10.0f;
    CurrentTraits.LearningSpeed = 1.0f;
    CurrentTraits.DecisionSpeed = 1.8f;
    CurrentTraits.RiskTolerance = 0.8f;
    CurrentTraits.MaxFollowers = 1000;
    CurrentTraits.SpecialAbilities = {"魔威震懾", "恐懼散播", "黑暗統治"};
}

void UMingSageCharacterSystem::ApplyFalseSageTraits()
{
    CurrentTraits = FSageCharacterTraits();
    CurrentTraits.CharacterType = ESageCharacterType::FalseSage;
    CurrentTraits.MoralAuthority = 40.0f;
    CurrentTraits.StrategicThinking = 85.0f;
    CurrentTraits.MilitaryCommand = 70.0f;
    CurrentTraits.CharismaBonus = 15.0f;
    CurrentTraits.LearningSpeed = 1.3f;
    CurrentTraits.DecisionSpeed = 1.5f;
    CurrentTraits.RiskTolerance = 0.6f;
    CurrentTraits.MaxFollowers = 300;
    CurrentTraits.SpecialAbilities = {"偽善欺騙", "陰謀策劃", "暗中操控"};
}

void UMingSageCharacterSystem::CalculateExperienceToNext()
{
    CharacterProgression.ExperienceToNext = 100.0f * FMath::Pow(1.5f, CharacterProgression.CurrentLevel);
}

void UMingSageCharacterSystem::CheckForLevelUp()
{
    while (CharacterProgression.Experience >= CharacterProgression.ExperienceToNext && 
           CharacterProgression.CurrentLevel < MaxCharacterLevel)
    {
        CharacterProgression.Experience -= CharacterProgression.ExperienceToNext;
        CharacterProgression.CurrentLevel++;
        CalculateExperienceToNext();
        
        OnCharacterLevelUp.Broadcast(CharacterProgression.CurrentLevel);
        
        UE_LOG(LogTemp, Log, TEXT("Character leveled up to %d"), CharacterProgression.CurrentLevel);
    }
}

void UMingSageCharacterSystem::InitializeDefaultSkills()
{
    CharacterProgression.SkillLevels.Add("領導力", 50.0f);
    CharacterProgression.SkillLevels.Add("戰術", 50.0f);
    CharacterProgression.SkillLevels.Add("外交", 50.0f);
    CharacterProgression.SkillLevels.Add("經濟", 50.0f);
    CharacterProgression.SkillLevels.Add("情報", 50.0f);
}
