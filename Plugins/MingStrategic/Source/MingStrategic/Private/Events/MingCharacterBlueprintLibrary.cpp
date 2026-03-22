#include "Events/MingCharacterBlueprintLibrary.h"
#include "Engine/Engine.h"

UMingHistoricalCharacterManager* UMingCharacterBlueprintLibrary::GetHistoricalCharacterManager()
{
    // 這裡應該從遊戲實例或單例獲取歷史人物管理器
    // 簡化實作
    static UMingHistoricalCharacterManager* ManagerInstance = nullptr;
    if (!ManagerInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            ManagerInstance = NewObject<UMingHistoricalCharacterManager>(World);
            ManagerInstance->Initialize();
        }
    }
    
    return ManagerInstance;
}

void UMingCharacterBlueprintLibrary::InitializeHistoricalCharacterSystem()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (Manager)
    {
        Manager->Initialize();
        UE_LOG(LogTemp, Log, TEXT("Historical character system initialized from Blueprint"));
    }
}

bool UMingCharacterBlueprintLibrary::RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        UE_LOG(LogTemp, Error, TEXT("Historical character manager not available"));
        return false;
    }
    
    return Manager->RegisterHistoricalCharacter(Character);
}

bool UMingCharacterBlueprintLibrary::UnregisterHistoricalCharacter(const FString& CharacterID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->UnregisterHistoricalCharacter(CharacterID);
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::GetHistoricalCharacter(const FString& CharacterID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return FMingHistoricalCharacter();
    }
    
    return Manager->GetHistoricalCharacter(CharacterID);
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetAllHistoricalCharacters()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetAllHistoricalCharacters();
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetCharactersByFaction(ECharacterFaction Faction)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetCharactersByFaction(Faction);
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetCharactersByProfession(ECharacterProfession Profession)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetCharactersByProfession(Profession);
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetKeyCharacters()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetKeyCharacters();
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetRecruitableCharacters()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetRecruitableCharacters();
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetActiveCharacters()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetActiveCharacters();
}

FCharacterInteractionResult UMingCharacterBlueprintLibrary::InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        FCharacterInteractionResult Result;
        Result.ResultDescription = TEXT("Historical character manager not available");
        return Result;
    }
    
    return Manager->InteractWithCharacter(CharacterID, InteractionType, InteractionDetails);
}

FDialogueEvent UMingCharacterBlueprintLibrary::StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        FDialogueEvent Event;
        Event.ResultDescription = TEXT("Historical character manager not available");
        return Event;
    }
    
    return Manager->StartDialogue(SpeakerID, AudienceID, DialogueTopic);
}

FDialogueEvent UMingCharacterBlueprintLibrary::ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        FDialogueEvent Event;
        Event.EventID = DialogueEventID;
        Event.ResultDescription = TEXT("Historical character manager not available");
        return Event;
    }
    
    return Manager->ProcessDialogueOption(DialogueEventID, OptionID);
}

bool UMingCharacterBlueprintLibrary::RecruitCharacter(const FString& CharacterID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->RecruitCharacter(CharacterID);
}

bool UMingCharacterBlueprintLibrary::DismissCharacter(const FString& CharacterID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->DismissCharacter(CharacterID);
}

bool UMingCharacterBlueprintLibrary::UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->UpgradeCharacterSkill(CharacterID, SkillType);
}

TArray<FCharacterRelationship> UMingCharacterBlueprintLibrary::GetCharacterRelationships(const FString& CharacterID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FCharacterRelationship>();
    }
    
    return Manager->GetCharacterRelationships(CharacterID);
}

bool UMingCharacterBlueprintLibrary::AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->AddCharacterRelationship(CharacterID, TargetCharacterID, RelationshipType, Strength);
}

bool UMingCharacterBlueprintLibrary::UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->UpdateCharacterRelationship(CharacterID, TargetCharacterID, NewStrength);
}

TMap<FString, int32> UMingCharacterBlueprintLibrary::GetCharacterStatistics()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TMap<FString, int32>();
    }
    
    return Manager->GetCharacterStatistics();
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetCharacterInfluenceRanking()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetCharacterInfluenceRanking();
}

TArray<FMingHistoricalCharacter> UMingCharacterBlueprintLibrary::GetRecommendedCharacters(const FString& PlayerFaction)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FMingHistoricalCharacter>();
    }
    
    return Manager->GetRecommendedCharacters(PlayerFaction);
}

FString UMingCharacterBlueprintLibrary::GetGenderName(ECharacterGender Gender)
{
    switch (Gender)
    {
    case ECharacterGender::Male:
        return TEXT("男性");
    case ECharacterGender::Female:
        return TEXT("女性");
    case ECharacterGender::Unknown:
        return TEXT("未知");
    default:
        return TEXT("未知性別");
    }
}

FString UMingCharacterBlueprintLibrary::GetProfessionName(ECharacterProfession Profession)
{
    switch (Profession)
    {
    case ECharacterProfession::Politician:
        return TEXT("政治家");
    case ECharacterProfession::Military:
        return TEXT("軍事家");
    case ECharacterProfession::Businessman:
        return TEXT("商人");
    case ECharacterProfession::Scholar:
        return TEXT("學者");
    case ECharacterProfession::Revolutionary:
        return TEXT("革命家");
    case ECharacterProfession::Diplomat:
        return TEXT("外交家");
    case ECharacterProfession::Journalist:
        return TEXT("記者");
    case ECharacterProfession::Artist:
        return TEXT("藝術家");
    case ECharacterProfession::Teacher:
        return TEXT("教師");
    case ECharacterProfession::Doctor:
        return TEXT("醫生");
    case ECharacterProfession::Engineer:
        return TEXT("工程師");
    case ECharacterProfession::Farmer:
        return TEXT("農民");
    case ECharacterProfession::Worker:
        return TEXT("工人");
    case ECharacterProfession::Merchant:
        return TEXT("商人");
    case ECharacterProfession::Noble:
        return TEXT("貴族");
    case ECharacterProfession::Unknown:
        return TEXT("未知");
    default:
        return TEXT("未知職業");
    }
}

FString UMingCharacterBlueprintLibrary::GetFactionName(ECharacterFaction Faction)
{
    switch (Faction)
    {
    case ECharacterFaction::Nationalist:
        return TEXT("國民黨");
    case ECharacterFaction::Communist:
        return TEXT("共產黨");
    case ECharacterFaction::Warlord:
        return TEXT("軍閥");
    case ECharacterFaction::Imperial:
        return TEXT("帝制派");
    case ECharacterFaction::Foreign:
        return TEXT("外國");
    case ECharacterFaction::Neutral:
        return TEXT("中立");
    case ECharacterFaction::Independent:
        return TEXT("獨立");
    case ECharacterFaction::Unknown:
        return TEXT("未知");
    default:
        return TEXT("未知陣營");
    }
}

FString UMingCharacterBlueprintLibrary::GetStateName(ECharacterState State)
{
    switch (State)
    {
    case ECharacterState::Active:
        return TEXT("活躍");
    case ECharacterState::Inactive:
        return TEXT("非活躍");
    case ECharacterState::Imprisoned:
        return TEXT("監禁");
    case ECharacterState::Exiled:
        return TEXT("流放");
    case ECharacterState::Deceased:
        return TEXT("已故");
    case ECharacterState::Retired:
        return TEXT("退休");
    case ECharacterState::Hidden:
        return TEXT("隱藏");
    case ECharacterState::Unknown:
        return TEXT("未知");
    default:
        return TEXT("未知狀態");
    }
}

FString UMingCharacterBlueprintLibrary::GetRelationshipTypeName(ECharacterRelationshipType RelationshipType)
{
    switch (RelationshipType)
    {
    case ECharacterRelationshipType::Family:
        return TEXT("家人");
    case ECharacterRelationshipType::Friend:
        return TEXT("朋友");
    case ECharacterRelationshipType::Ally:
        return TEXT("盟友");
    case ECharacterRelationshipType::Rival:
        return TEXT("對手");
    case ECharacterRelationshipType::Enemy:
        return TEXT("敵人");
    case ECharacterRelationshipType::Mentor:
        return TEXT("導師");
    case ECharacterRelationshipType::Student:
        return TEXT("學生");
    case ECharacterRelationshipType::Colleague:
        return TEXT("同事");
    case ECharacterRelationshipType::Subordinate:
        return TEXT("下屬");
    case ECharacterRelationshipType::Superior:
        return TEXT("上司");
    case ECharacterRelationshipType::Spouse:
        return TEXT("配偶");
    case ECharacterRelationshipType::Acquaintance:
        return TEXT("熟人");
    case ECharacterRelationshipType::Unknown:
        return TEXT("未知");
    default:
        return TEXT("未知關係");
    }
}

FString UMingCharacterBlueprintLibrary::GetSkillTypeName(ECharacterSkillType SkillType)
{
    switch (SkillType)
    {
    case ECharacterSkillType::Leadership:
        return TEXT("領導力");
    case ECharacterSkillType::Military:
        return TEXT("軍事才能");
    case ECharacterSkillType::Diplomacy:
        return TEXT("外交能力");
    case ECharacterSkillType::Economics:
        return TEXT("經濟才能");
    case ECharacterSkillType::Intelligence:
        return TEXT("智力水平");
    case ECharacterSkillType::Charisma:
        return TEXT("魅力值");
    case ECharacterSkillType::Administration:
        return TEXT("行政能力");
    case ECharacterSkillType::Strategy:
        return TEXT("戰略思維");
    case ECharacterSkillType::Oratory:
        return TEXT("演講能力");
    case ECharacterSkillType::Writing:
        return TEXT("寫作能力");
    case ECharacterSkillType::Science:
        return TEXT("科學知識");
    case ECharacterSkillType::Engineering:
        return TEXT("工程技術");
    case ECharacterSkillType::Medicine:
        return TEXT("醫學知識");
    case ECharacterSkillType::Art:
        return TEXT("藝術才能");
    case ECharacterSkillType::Unknown:
        return TEXT("未知技能");
    default:
        return TEXT("未知技能");
    }
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::CreateHistoricalCharacter(
    const FString& CharacterID,
    const FString& CharacterName,
    const FString& CourtesyName,
    const FString& EnglishName,
    const FString& CharacterDescription,
    ECharacterGender Gender,
    ECharacterProfession Profession,
    ECharacterFaction Faction,
    int32 BirthYear,
    int32 DeathYear,
    bool bIsKeyCharacter,
    bool bIsRecruitable)
{
    FMingHistoricalCharacter Character;
    Character.CharacterID = CharacterID;
    Character.CharacterName = CharacterName;
    Character.CourtesyName = CourtesyName;
    Character.EnglishName = EnglishName;
    Character.CharacterDescription = CharacterDescription;
    Character.Gender = Gender;
    Character.Profession = Profession;
    Character.Faction = Faction;
    Character.BirthYear = BirthYear;
    Character.DeathYear = DeathYear;
    Character.State = (DeathYear > 0 && DeathYear < 2026) ? ECharacterState::Deceased : ECharacterState::Active;
    Character.bIsKeyCharacter = bIsKeyCharacter;
    Character.bIsRecruitable = bIsRecruitable;
    
    return Character;
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::SetCharacterAttributes(
    const FMingHistoricalCharacter& Character,
    float Leadership,
    float Military,
    float Diplomacy,
    float Economics,
    float Intelligence,
    float Charisma,
    float Administration,
    float Strategy,
    float Oratory,
    float Writing,
    float Science,
    float Engineering,
    float Medicine,
    float Art)
{
    FMingHistoricalCharacter ModifiedCharacter = Character;
    
    ModifiedCharacter.Attributes.Leadership = FMath::Clamp(Leadership, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Military = FMath::Clamp(Military, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Diplomacy = FMath::Clamp(Diplomacy, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Economics = FMath::Clamp(Economics, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Intelligence = FMath::Clamp(Intelligence, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Charisma = FMath::Clamp(Charisma, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Administration = FMath::Clamp(Administration, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Strategy = FMath::Clamp(Strategy, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Oratory = FMath::Clamp(Oratory, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Writing = FMath::Clamp(Writing, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Science = FMath::Clamp(Science, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Engineering = FMath::Clamp(Engineering, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Medicine = FMath::Clamp(Medicine, 0.0f, 100.0f);
    ModifiedCharacter.Attributes.Art = FMath::Clamp(Art, 0.0f, 100.0f);
    
    return ModifiedCharacter;
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::AddCharacterSkill(
    const FMingHistoricalCharacter& Character,
    ECharacterSkillType SkillType,
    const FString& SkillName,
    const FString& SkillDescription,
    int32 SkillLevel,
    bool bIsUnlocked)
{
    FMingHistoricalCharacter ModifiedCharacter = Character;
    
    FCharacterSkill Skill;
    Skill.SkillType = SkillType;
    Skill.SkillName = SkillName;
    Skill.SkillDescription = SkillDescription;
    Skill.SkillLevel = FMath::Clamp(SkillLevel, 1, 10);
    Skill.bIsUnlocked = bIsUnlocked;
    Skill.SkillExperience = 0;
    Skill.CooldownTime = 0.0f;
    Skill.LastUsedTime = 0.0f;
    
    ModifiedCharacter.Skills.Add(Skill);
    
    return ModifiedCharacter;
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::AddCharacterTag(
    const FMingHistoricalCharacter& Character,
    const FString& Tag)
{
    FMingHistoricalCharacter ModifiedCharacter = Character;
    ModifiedCharacter.CharacterTags.Add(Tag);
    return ModifiedCharacter;
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::AddHistoricalEvent(
    const FMingHistoricalCharacter& Character,
    const FString& Event)
{
    FMingHistoricalCharacter ModifiedCharacter = Character;
    ModifiedCharacter.HistoricalEvents.Add(Event);
    return ModifiedCharacter;
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::AddAchievement(
    const FMingHistoricalCharacter& Character,
    const FString& Achievement)
{
    FMingHistoricalCharacter ModifiedCharacter = Character;
    ModifiedCharacter.Achievements.Add(Achievement);
    return ModifiedCharacter;
}

FMingHistoricalCharacter UMingCharacterBlueprintLibrary::SetInfluenceAndReputation(
    const FMingHistoricalCharacter& Character,
    float Influence,
    float Reputation)
{
    FMingHistoricalCharacter ModifiedCharacter = Character;
    ModifiedCharacter.Influence = FMath::Clamp(Influence, 0.0f, 1000.0f);
    ModifiedCharacter.Reputation = FMath::Clamp(Reputation, 0.0f, 1000.0f);
    return ModifiedCharacter;
}

TArray<FCharacterInteractionResult> UMingCharacterBlueprintLibrary::BatchInteractWithCharacters(const TArray<FString>& CharacterIDs, const FString& InteractionType)
{
    TArray<FCharacterInteractionResult> Results;
    
    for (const FString& CharacterID : CharacterIDs)
    {
        Results.Add(InteractWithCharacter(CharacterID, InteractionType, TEXT("")));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch interacted with %d characters"), Results.Num());
    return Results;
}

TArray<bool> UMingCharacterBlueprintLibrary::BatchRecruitCharacters(const TArray<FString>& CharacterIDs)
{
    TArray<bool> Results;
    
    for (const FString& CharacterID : CharacterIDs)
    {
        Results.Add(RecruitCharacter(CharacterID));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch recruited %d characters"), Results.Num());
    return Results;
}

bool UMingCharacterBlueprintLibrary::IsHistoricalCharacterSystemInitialized()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    return Manager != nullptr;
}

FString UMingCharacterBlueprintLibrary::GetHistoricalCharacterSystemVersion()
{
    return TEXT("1.0.0");
}

int32 UMingCharacterBlueprintLibrary::GetRegisteredCharacterCount()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return 0;
    }
    
    return Manager->GetAllHistoricalCharacters().Num();
}

int32 UMingCharacterBlueprintLibrary::GetActiveCharacterCount()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return 0;
    }
    
    return Manager->GetActiveCharacters().Num();
}

int32 UMingCharacterBlueprintLibrary::GetKeyCharacterCount()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return 0;
    }
    
    return Manager->GetKeyCharacters().Num();
}

int32 UMingCharacterBlueprintLibrary::GetRecruitableCharacterCount()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return 0;
    }
    
    return Manager->GetRecruitableCharacters().Num();
}

FString UMingCharacterBlueprintLibrary::GetCharacterDescription(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.CharacterDescription;
}

int32 UMingCharacterBlueprintLibrary::GetCharacterAge(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    
    if (Character.DeathYear == 0)
    {
        return 2026 - Character.BirthYear; // 假設當前年份
    }
    else
    {
        return Character.DeathYear - Character.BirthYear;
    }
}

bool UMingCharacterBlueprintLibrary::IsCharacterRecruitable(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.bIsRecruitable && Character.State == ECharacterState::Active;
}

bool UMingCharacterBlueprintLibrary::IsCharacterKeyCharacter(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.bIsKeyCharacter;
}

int32 UMingCharacterBlueprintLibrary::GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    
    for (const FCharacterSkill& Skill : Character.Skills)
    {
        if (Skill.SkillType == SkillType)
        {
            return Skill.SkillLevel;
        }
    }
    
    return 0;
}

TArray<FCharacterSkill> UMingCharacterBlueprintLibrary::GetCharacterSkills(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Skills;
}

bool UMingCharacterBlueprintLibrary::HasCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType)
{
    return GetCharacterSkillLevel(CharacterID, SkillType) > 0;
}

float UMingCharacterBlueprintLibrary::GetRelationshipStrength(const FString& CharacterID, const FString& TargetCharacterID)
{
    TArray<FCharacterRelationship> Relationships = GetCharacterRelationships(CharacterID);
    
    for (const FCharacterRelationship& Relationship : Relationships)
    {
        if (Relationship.TargetCharacterID == TargetCharacterID)
        {
            return Relationship.RelationshipStrength;
        }
    }
    
    return 0.0f;
}

ECharacterRelationshipType UMingCharacterBlueprintLibrary::GetRelationshipType(const FString& CharacterID, const FString& TargetCharacterID)
{
    TArray<FCharacterRelationship> Relationships = GetCharacterRelationships(CharacterID);
    
    for (const FCharacterRelationship& Relationship : Relationships)
    {
        if (Relationship.TargetCharacterID == TargetCharacterID)
        {
            return Relationship.RelationshipType;
        }
    }
    
    return ECharacterRelationshipType::Unknown;
}

bool UMingCharacterBlueprintLibrary::HasRelationship(const FString& CharacterID, const FString& TargetCharacterID)
{
    return GetRelationshipStrength(CharacterID, TargetCharacterID) > 0.0f;
}

float UMingCharacterBlueprintLibrary::GetCharacterInfluence(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Influence;
}

float UMingCharacterBlueprintLibrary::GetCharacterReputation(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Reputation;
}

float UMingCharacterBlueprintLibrary::GetCharacterLoyalty(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Loyalty;
}

float UMingCharacterBlueprintLibrary::GetCharacterHealth(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Health;
}

float UMingCharacterBlueprintLibrary::GetCharacterMorale(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Morale;
}

int32 UMingCharacterBlueprintLibrary::GetCharacterLevel(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Attributes.Level;
}

int32 UMingCharacterBlueprintLibrary::GetCharacterExperience(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    return Character.Attributes.Experience;
}

float UMingCharacterBlueprintLibrary::GetCharacterOverallScore(const FString& CharacterID)
{
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    
    float Score = 0.0f;
    
    // 基於屬性計算總分
    Score += Character.Attributes.Leadership * 0.15f;
    Score += Character.Attributes.Military * 0.15f;
    Score += Character.Attributes.Diplomacy * 0.15f;
    Score += Character.Attributes.Economics * 0.10f;
    Score += Character.Attributes.Intelligence * 0.10f;
    Score += Character.Attributes.Charisma * 0.10f;
    Score += Character.Attributes.Administration * 0.10f;
    Score += Character.Attributes.Strategy * 0.10f;
    Score += Character.Attributes.Oratory * 0.05f;
    Score += Character.Attributes.Writing * 0.05f;
    Score += Character.Attributes.Science * 0.05f;
    Score += Character.Attributes.Engineering * 0.05f;
    Score += Character.Attributes.Medicine * 0.05f;
    Score += Character.Attributes.Art * 0.05f;
    
    // 基於等級加分
    Score += Character.Attributes.Level * 10.0f;
    
    // 基於影響力加分
    Score += Character.Influence / 100.0f;
    
    // 基於聲望加分
    Score += Character.Reputation / 100.0f;
    
    return FMath::Clamp(Score, 0.0f, 1000.0f);
}

TArray<FString> UMingCharacterBlueprintLibrary::GetInteractionRecommendations(const FString& CharacterID)
{
    TArray<FString> Recommendations;
    
    FMingHistoricalCharacter Character = GetHistoricalCharacter(CharacterID);
    
    // 基於人物屬性推薦互動類型
    if (Character.Attributes.Charisma >= 80.0f)
    {
        Recommendations.Add(TEXT("conversation"));
        Recommendations.Add(TEXT("diplomacy"));
    }
    
    if (Character.Attributes.Intelligence >= 80.0f)
    {
        Recommendations.Add(TEXT("debate"));
        Recommendations.Add(TEXT("scholarly_discussion"));
    }
    
    if (Character.Attributes.Leadership >= 80.0f)
    {
        Recommendations.Add(TEXT("leadership_guidance"));
        Recommendations.Add(TEXT("strategic_planning"));
    }
    
    if (Character.Attributes.Military >= 80.0f)
    {
        Recommendations.Add(TEXT("military_tactics"));
        Recommendations.Add(TEXT("combat_training"));
    }
    
    return Recommendations;
}

TArray<FString> UMingCharacterBlueprintLibrary::GetDialogueRecommendations(const FString& SpeakerID, const FString& AudienceID)
{
    TArray<FString> Recommendations;
    
    FMingHistoricalCharacter Speaker = GetHistoricalCharacter(SpeakerID);
    FMingHistoricalCharacter Audience = GetHistoricalCharacter(AudienceID);
    
    // 基於人物背景推薦對話主題
    if (Speaker.Profession == ECharacterProfession::Politician && Audience.Profession == ECharacterProfession::Politician)
    {
        Recommendations.Add(TEXT("politics"));
        Recommendations.Add(TEXT("governance"));
        Recommendations.Add(TEXT("policy"));
    }
    else if (Speaker.Profession == ECharacterProfession::Scholar && Audience.Profession == ECharacterProfession::Scholar)
    {
        Recommendations.Add(TEXT("academia"));
        Recommendations.Add(TEXT("research"));
        Recommendations.Add(TEXT("philosophy"));
    }
    else if (Speaker.Profession == ECharacterProfession::Military && Audience.Profession == ECharacterProfession::Military)
    {
        Recommendations.Add(TEXT("military_strategy"));
        Recommendations.Add(TEXT("tactics"));
        Recommendations.Add(TEXT("defense"));
    }
    else
    {
        Recommendations.Add(TEXT("general"));
        Recommendations.Add(TEXT("culture"));
        Recommendations.Add(TEXT("current_events"));
    }
    
    return Recommendations;
}

TArray<FString> UMingCharacterBlueprintLibrary::GetRecruitmentRecommendations(const FString& PlayerFaction)
{
    TArray<FString> Recommendations;
    
    TArray<FMingHistoricalCharacter> RecommendedCharacters = GetRecommendedCharacters(PlayerFaction);
    
    for (const FMingHistoricalCharacter& Character : RecommendedCharacters)
    {
        FString Recommendation = FString::Printf(TEXT("推薦招募: %s (%s) - %s"), 
            *Character.CharacterName, 
            *GetProfessionName(Character.Profession),
            *Character.CharacterDescription);
        Recommendations.Add(Recommendation);
    }
    
    return Recommendations;
}

TMap<FString, TArray<FString>> UMingCharacterBlueprintLibrary::GetCharacterNetwork(const FString& CharacterID)
{
    TMap<FString, TArray<FString>> Network;
    
    TArray<FCharacterRelationship> Relationships = GetCharacterRelationships(CharacterID);
    
    for (const FCharacterRelationship& Relationship : Relationships)
    {
        TArray<FString>* Connections = Network.Find(Relationship.TargetCharacterID);
        if (!Connections)
        {
            TArray<FString> NewConnections;
            Network.Add(Relationship.TargetCharacterID, NewConnections);
            Connections = Network.Find(Relationship.TargetCharacterID);
        }
        
        Connections->Add(CharacterID);
    }
    
    return Network;
}

TMap<FString, float> UMingCharacterBlueprintLibrary::GetRelationshipImpactAnalysis(const FString& CharacterID)
{
    TMap<FString, float> ImpactAnalysis;
    
    TArray<FCharacterRelationship> Relationships = GetCharacterRelationships(CharacterID);
    
    for (const FCharacterRelationship& Relationship : Relationships)
    {
        float Impact = Relationship.RelationshipStrength / 100.0f;
        
        // 基於關係類型調整影響
        switch (Relationship.RelationshipType)
        {
        case ECharacterRelationshipType::Family:
            Impact *= 1.5f;
            break;
        case ECharacterRelationshipType::Friend:
            Impact *= 1.2f;
            break;
        case ECharacterRelationshipType::Ally:
            Impact *= 1.1f;
            break;
        case ECharacterRelationshipType::Rival:
            Impact *= 0.8f;
            break;
        case ECharacterRelationshipType::Enemy:
            Impact *= 0.5f;
            break;
        default:
            break;
        }
        
        ImpactAnalysis.Add(Relationship.TargetCharacterID, Impact);
    }
    
    return ImpactAnalysis;
}

TArray<FCharacterInteractionResult> UMingCharacterBlueprintLibrary::SimulateCharacterInteractions(const FString& CharacterID, int32 SimulationCount)
{
    TArray<FCharacterInteractionResult> SimulationResults;
    
    // 保存原始狀態
    FMingHistoricalCharacter OriginalCharacter = GetHistoricalCharacter(CharacterID);
    
    TArray<FString> InteractionTypes = {TEXT("conversation"), TEXT("debate"), TEXT("collaboration"), TEXT("mentoring"), TEXT("diplomacy")};
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        FString InteractionType = InteractionTypes[FMath::RandRange(0, InteractionTypes.Num() - 1)];
        FCharacterInteractionResult Result = InteractWithCharacter(CharacterID, InteractionType, TEXT(""));
        SimulationResults.Add(Result);
    }
    
    return SimulationResults;
}

TArray<FDialogueEvent> UMingCharacterBlueprintLibrary::SimulateDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, int32 SimulationCount)
{
    TArray<FDialogueEvent> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        FDialogueEvent Event = StartDialogue(SpeakerID, AudienceID, DialogueTopic);
        
        // 模擬處理對話選項
        if (Event.DialogueOptions.Num() > 0)
        {
            FString OptionID = Event.DialogueOptions[0].OptionID;
            Event = ProcessDialogueOption(Event.EventID, OptionID);
        }
        
        SimulationResults.Add(Event);
    }
    
    return SimulationResults;
}

bool UMingCharacterBlueprintLibrary::ExportCharacterData(const FString& FilePath)
{
    // 簡化實作：記錄導出操作
    UE_LOG(LogTemp, Log, TEXT("Character data would be exported to: %s"), *FilePath);
    return true;
}

bool UMingCharacterBlueprintLibrary::ImportCharacterData(const FString& FilePath)
{
    // 簡化實作：記錄導入操作
    UE_LOG(LogTemp, Log, TEXT("Character data would be imported from: %s"), *FilePath);
    return true;
}

FString UMingCharacterBlueprintLibrary::GenerateCharacterReport(const FString& CharacterID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TEXT("Character manager not available");
    }
    
    return Manager->GenerateCharacterReport(CharacterID);
}

FString UMingCharacterBlueprintLibrary::GenerateStatisticsReport()
{
    TMap<FString, int32> Stats = GetCharacterStatistics();
    
    FString Report = TEXT("=== 人物系統統計報告 ===\n");
    Report += FString::Printf(TEXT("註冊人物數量: %d\n"), GetRegisteredCharacterCount());
    Report += FString::Printf(TEXT("活躍人物數量: %d\n"), GetActiveCharacterCount());
    Report += FString::Printf(TEXT("關鍵人物數量: %d\n"), GetKeyCharacterCount());
    Report += FString::Printf(TEXT("可招募人物數量: %d\n"), GetRecruitableCharacterCount());
    
    Report += TEXT("\n=== 統計詳情 ===\n");
    for (const auto& Stat : Stats)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), *Stat.Key, Stat.Value);
    }
    
    return Report;
}

FString UMingCharacterBlueprintLibrary::GenerateRelationshipReport(const FString& CharacterID)
{
    TArray<FCharacterRelationship> Relationships = GetCharacterRelationships(CharacterID);
    
    FString Report = TEXT("=== 人物關係報告 ===\n");
    Report += FString::Printf(TEXT("人物: %s\n"), *CharacterID);
    Report += FString::Printf(TEXT("關係數量: %d\n\n"), Relationships.Num());
    
    for (const FCharacterRelationship& Relationship : Relationships)
    {
        Report += FString::Printf(TEXT("- %s: %s (%.1f) - %s\n"), 
            *Relationship.TargetCharacterID,
            *GetRelationshipTypeName(Relationship.RelationshipType),
            Relationship.RelationshipStrength,
            *Relationship.RelationshipDescription);
    }
    
    return Report;
}

TArray<FDialogueEvent> UMingCharacterBlueprintLibrary::GetDialogueHistory()
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FDialogueEvent>();
    }
    
    // 簡化實作：返回空數組，實際應該從管理器獲取
    return TArray<FDialogueEvent>();
}

TArray<FDialogueEvent> UMingCharacterBlueprintLibrary::GetDialogueHistoryForCharacter(const FString& CharacterID)
{
    UMingHistoricalCharacterManager* Manager = GetHistoricalCharacterManager();
    if (!Manager)
    {
        return TArray<FDialogueEvent>();
    }
    
    // 簡化實作：返回空數組，實際應該從管理器獲取
    return TArray<FDialogueEvent>();
}
