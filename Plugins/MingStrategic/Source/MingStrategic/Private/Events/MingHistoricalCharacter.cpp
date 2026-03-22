#include "Events/MingHistoricalCharacter.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingHistoricalCharacterManager::UMingHistoricalCharacterManager()
    : bIsInitialized(false)
{
    // 預分配容量
    RegisteredCharacters.Reserve(100);
    CharacterStatistics.Reserve(50);
    DialogueHistory.Reserve(200);
}

void UMingHistoricalCharacterManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 初始化歷史人物庫
    InitializeHistoricalCharacterLibrary();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingHistoricalCharacterManager initialized"));
    UE_LOG(LogTemp, Log, TEXT("Loaded %d historical characters"), RegisteredCharacters.Num());
}

void UMingHistoricalCharacterManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    RegisteredCharacters.Empty();
    CharacterMap.Empty();
    RelationshipNetwork.Empty();
    DialogueHistory.Empty();
    CharacterStatistics.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingHistoricalCharacterManager shutdown"));
}

bool UMingHistoricalCharacterManager::RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterManager not initialized"));
        return false;
    }

    if (Character.CharacterID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Character ID cannot be empty"));
        return false;
    }

    // 檢查是否已存在
    if (CharacterMap.Contains(Character.CharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Character %s already registered"), *Character.CharacterID);
        return false;
    }

    // 驗證人物配置
    if (!ValidateCharacterConfig(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid character configuration: %s"), *Character.CharacterID);
        return false;
    }

    // 添加到註冊列表
    RegisteredCharacters.Add(Character);
    CharacterMap.Add(Character.CharacterID, Character);
    
    UE_LOG(LogTemp, Log, TEXT("Historical character registered: %s - %s"), 
        *Character.CharacterID, *Character.CharacterName);
    
    return true;
}

bool UMingHistoricalCharacterManager::UnregisterHistoricalCharacter(const FString& CharacterID)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (!CharacterMap.Contains(CharacterID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Character %s not found"), *CharacterID);
        return false;
    }

    // 從註冊列表移除
    for (int32 i = 0; i < RegisteredCharacters.Num(); ++i)
    {
        if (RegisteredCharacters[i].CharacterID == CharacterID)
        {
            RegisteredCharacters.RemoveAt(i);
            break;
        }
    }
    
    CharacterMap.Remove(CharacterID);
    
    UE_LOG(LogTemp, Log, TEXT("Historical character unregistered: %s"), *CharacterID);
    
    return true;
}

FMingHistoricalCharacter UMingHistoricalCharacterManager::GetHistoricalCharacter(const FString& CharacterID) const
{
    const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    return Character ? *Character : FMingHistoricalCharacter();
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetAllHistoricalCharacters() const
{
    return RegisteredCharacters;
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetCharactersByFaction(ECharacterFaction Faction) const
{
    TArray<FMingHistoricalCharacter> Result;
    
    for (const FMingHistoricalCharacter& Character : RegisteredCharacters)
    {
        if (Character.Faction == Faction)
        {
            Result.Add(Character);
        }
    }
    
    return Result;
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetCharactersByProfession(ECharacterProfession Profession) const
{
    TArray<FMingHistoricalCharacter> Result;
    
    for (const FMingHistoricalCharacter& Character : RegisteredCharacters)
    {
        if (Character.Profession == Profession)
        {
            Result.Add(Character);
        }
    }
    
    return Result;
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetKeyCharacters() const
{
    TArray<FMingHistoricalCharacter> Result;
    
    for (const FMingHistoricalCharacter& Character : RegisteredCharacters)
    {
        if (Character.bIsKeyCharacter)
        {
            Result.Add(Character);
        }
    }
    
    return Result;
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetRecruitableCharacters() const
{
    TArray<FMingHistoricalCharacter> Result;
    
    for (const FMingHistoricalCharacter& Character : RegisteredCharacters)
    {
        if (Character.bIsRecruitable && Character.State == ECharacterState::Active)
        {
            Result.Add(Character);
        }
    }
    
    return Result;
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetActiveCharacters() const
{
    TArray<FMingHistoricalCharacter> Result;
    
    for (const FMingHistoricalCharacter& Character : RegisteredCharacters)
    {
        if (Character.State == ECharacterState::Active)
        {
            Result.Add(Character);
        }
    }
    
    return Result;
}

FCharacterInteractionResult UMingHistoricalCharacterManager::InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails)
{
    FCharacterInteractionResult Result;
    Result.InteractionType = InteractionType;
    Result.TargetCharacterID = CharacterID;
    Result.InteractionTime = FPlatformTime::Seconds();
    
    if (!bIsInitialized)
    {
        Result.ResultDescription = TEXT("CharacterManager not initialized");
        return Result;
    }

    const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        Result.ResultDescription = FString::Printf(TEXT("Character %s not found"), *CharacterID);
        return Result;
    }

    // 檢查互動條件
    if (!CheckInteractionConditions(CharacterID, InteractionType))
    {
        Result.ResultDescription = TEXT("Interaction conditions not met");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Interacting with character: %s - %s"), *CharacterID, *InteractionType);

    // 處理人物互動邏輯
    HandleCharacterInteraction(CharacterID, InteractionType, Result);
    
    // 應用互動效果
    ApplyInteractionEffects(CharacterID, InteractionType, Result);
    
    // 更新人物關係
    if (InteractionDetails.Contains(TEXT("target_character")))
    {
        UpdateCharacterRelationships(CharacterID, InteractionDetails, InteractionType);
    }
    
    // 計算互動分數
    Result.InteractionScore = CalculateInteractionScore(CharacterID, InteractionType);
    
    // 更新統計
    int32* Count = CharacterStatistics.Find(TEXT("interactions"));
    if (Count)
    {
        (*Count)++;
    }
    else
    {
        CharacterStatistics.Add(TEXT("interactions"), 1);
    }
    
    Result.bSuccess = true;
    
    // 記錄人物日誌
    LogCharacterEvent(CharacterID, FString::Printf(TEXT("Interaction: %s - %s"), *InteractionType, *Result.ResultDescription));
    
    UE_LOG(LogTemp, Log, TEXT("Character interaction completed: %s"), *CharacterID);
    
    return Result;
}

FDialogueEvent UMingHistoricalCharacterManager::StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic)
{
    FDialogueEvent Event;
    Event.EventID = FString::Printf(TEXT("dialogue_%s_%s_%d"), *SpeakerID, *AudienceID, FDateTime::Now().GetMillisecond());
    Event.SpeakerID = SpeakerID;
    Event.AudienceID = AudienceID;
    Event.DialogueTopic = DialogueTopic;
    Event.DialogueTime = FPlatformTime::Seconds();
    
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterManager not initialized"));
        return Event;
    }

    const FMingHistoricalCharacter* Speaker = CharacterMap.Find(SpeakerID);
    const FMingHistoricalCharacter* Audience = CharacterMap.Find(AudienceID);
    
    if (!Speaker || !Audience)
    {
        UE_LOG(LogTemp, Error, TEXT("Speaker or audience not found"));
        return Event;
    }

    // 處理對話邏輯
    HandleDialogueLogic(SpeakerID, AudienceID, DialogueTopic, Event);
    
    // 記錄對話事件
    RecordDialogueEvent(Event);
    
    UE_LOG(LogTemp, Log, TEXT("Dialogue started: %s -> %s (%s)"), *SpeakerID, *AudienceID, *DialogueTopic);
    
    return Event;
}

FDialogueEvent UMingHistoricalCharacterManager::ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID)
{
    FDialogueEvent Result;
    Result.EventID = DialogueEventID;
    
    // 查找對話事件
    for (const FDialogueEvent& Event : DialogueHistory)
    {
        if (Event.EventID == DialogueEventID)
        {
            Result = Event;
            break;
        }
    }
    
    // 處理對話選項
    for (const FDialogueOption& Option : Result.DialogueOptions)
    {
        if (Option.OptionID == OptionID)
        {
            // 應用選項後果
            for (const FString& Consequence : Option.Consequences)
            {
                // 簡化實作：記錄後果
                UE_LOG(LogTemp, Log, TEXT("Dialogue consequence: %s"), *Consequence);
            }
            
            // 更新關係
            UpdateCharacterRelationships(Result.SpeakerID, Result.AudienceID, TEXT("dialogue"));
            
            break;
        }
    }
    
    return Result;
}

bool UMingHistoricalCharacterManager::RecruitCharacter(const FString& CharacterID)
{
    const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("Character %s not found"), *CharacterID);
        return false;
    }

    if (!Character->bIsRecruitable)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character %s is not recruitable"), *CharacterID);
        return false;
    }

    // 檢查招募成本
    // 簡化實作：假設成本充足
    
    // 更新人物狀態
    UpdateCharacterState(CharacterID, ECharacterState::Active);
    
    UE_LOG(LogTemp, Log, TEXT("Character recruited: %s"), *CharacterID);
    
    return true;
}

bool UMingHistoricalCharacterManager::DismissCharacter(const FString& CharacterID)
{
    const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("Character %s not found"), *CharacterID);
        return false;
    }

    // 更新人物狀態
    UpdateCharacterState(CharacterID, ECharacterState::Inactive);
    
    UE_LOG(LogTemp, Log, TEXT("Character dismissed: %s"), *CharacterID);
    
    return true;
}

bool UMingHistoricalCharacterManager::UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType)
{
    FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        UE_LOG(LogTemp, Error, TEXT("Character %s not found"), *CharacterID);
        return false;
    }

    // 查找並升級技能
    for (FCharacterSkill& Skill : Character->Skills)
    {
        if (Skill.SkillType == SkillType && Skill.SkillLevel < 10)
        {
            Skill.SkillLevel++;
            Skill.SkillExperience = 0;
            
            UE_LOG(LogTemp, Log, TEXT("Character skill upgraded: %s - %s to level %d"), 
                *CharacterID, *Skill.SkillName, Skill.SkillLevel);
            
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Skill not found or already at max level: %s"), *CharacterID);
    return false;
}

TArray<FCharacterRelationship> UMingHistoricalCharacterManager::GetCharacterRelationships(const FString& CharacterID) const
{
    const TArray<FCharacterRelationship>* Relationships = RelationshipNetwork.Find(CharacterID);
    return Relationships ? *Relationships : TArray<FCharacterRelationship>();
}

bool UMingHistoricalCharacterManager::AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength)
{
    FCharacterRelationship Relationship;
    Relationship.RelationshipType = RelationshipType;
    Relationship.TargetCharacterID = TargetCharacterID;
    Relationship.RelationshipStrength = Strength;
    Relationship.EstablishedTime = FPlatformTime::Seconds();
    Relationship.InteractionCount = 0;
    Relationship.LastInteractionTime = Relationship.EstablishedTime;
    
    // 添加到關係網絡
    TArray<FCharacterRelationship>* Relationships = RelationshipNetwork.Find(CharacterID);
    if (!Relationships)
    {
        TArray<FCharacterRelationship> NewRelationships;
        NewRelationships.Add(Relationship);
        RelationshipNetwork.Add(CharacterID, NewRelationships);
    }
    else
    {
        Relationships->Add(Relationship);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Character relationship added: %s -> %s (%s)"), 
        *CharacterID, *TargetCharacterID, *UEnum::GetValueAsString(RelationshipType));
    
    return true;
}

bool UMingHistoricalCharacterManager::UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength)
{
    TArray<FCharacterRelationship>* Relationships = RelationshipNetwork.Find(CharacterID);
    if (!Relationships)
    {
        return false;
    }
    
    for (FCharacterRelationship& Relationship : *Relationships)
    {
        if (Relationship.TargetCharacterID == TargetCharacterID)
        {
            Relationship.RelationshipStrength = NewStrength;
            Relationship.LastInteractionTime = FPlatformTime::Seconds();
            Relationship.InteractionCount++;
            
            UE_LOG(LogTemp, Verbose, TEXT("Character relationship updated: %s -> %s (%.1f)"), 
                *CharacterID, *TargetCharacterID, NewStrength);
            
            return true;
        }
    }
    
    return false;
}

TMap<FString, int32> UMingHistoricalCharacterManager::GetCharacterStatistics() const
{
    return CharacterStatistics;
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetCharacterInfluenceRanking() const
{
    TArray<FMingHistoricalCharacter> Result = RegisteredCharacters;
    
    // 按影響力排序
    Result.Sort([this](const FMingHistoricalCharacter& A, const FMingHistoricalCharacter& B)
    {
        return CalculateCharacterInfluence(A) > CalculateCharacterInfluence(B);
    });
    
    return Result;
}

TArray<FMingHistoricalCharacter> UMingHistoricalCharacterManager::GetRecommendedCharacters(const FString& PlayerFaction) const
{
    TArray<FMingHistoricalCharacter> Result;
    
    for (const FMingHistoricalCharacter& Character : RegisteredCharacters)
    {
        if (Character.bIsRecruitable && Character.State == ECharacterState::Active)
        {
            float Score = GetCharacterRecommendationScore(Character, PlayerFaction);
            if (Score > 0.5f)
            {
                Result.Add(Character);
            }
        }
    }
    
    // 按推薦分數排序
    Result.Sort([this, &PlayerFaction](const FMingHistoricalCharacter& A, const FMingHistoricalCharacter& B)
    {
        return GetCharacterRecommendationScore(A, PlayerFaction) > GetCharacterRecommendationScore(B, PlayerFaction);
    });
    
    return Result;
}

// === 內部方法實現 ===

void UMingHistoricalCharacterManager::InitializeHistoricalCharacterLibrary()
{
    CreatePoliticalCharacters();
    CreateMilitaryCharacters();
    CreateBusinessCharacters();
    CreateScholarCharacters();
    CreateRevolutionaryCharacters();
    CreateDiplomaticCharacters();
    CreateCulturalCharacters();
}

void UMingHistoricalCharacterManager::CreatePoliticalCharacters()
{
    // 孫中山
    FMingHistoricalCharacter SunYatSen;
    SunYatSen.CharacterID = TEXT("sun_yat_sen");
    SunYatSen.CharacterName = TEXT("孫中山");
    SunYatSen.CourtesyName = TEXT("逸仙");
    SunYatSen.EnglishName = TEXT("Sun Yat-sen");
    SunYatSen.CharacterDescription = TEXT("中國近代民主革命家，中華民國國父");
    SunYatSen.Gender = ECharacterGender::Male;
    SunYatSen.Profession = ECharacterProfession::Politician;
    SunYatSen.Faction = ECharacterFaction::Nationalist;
    SunYatSen.BirthYear = 1866;
    SunYatSen.DeathYear = 1925;
    SunYatSen.State = ECharacterState::Deceased;
    SunYatSen.Attributes.Leadership = 95.0f;
    SunYatSen.Attributes.Charisma = 90.0f;
    SunYatSen.Attributes.Diplomacy = 85.0f;
    SunYatSen.Attributes.Strategy = 80.0f;
    SunYatSen.Attributes.Oratory = 95.0f;
    SunYatSen.Influence = 950.0f;
    SunYatSen.Reputation = 980.0f;
    SunYatSen.bIsKeyCharacter = true;
    SunYatSen.bIsRecruitable = false;
    SunYatSen.CharacterTags.Add(TEXT("founder"));
    SunYatSen.CharacterTags.Add(TEXT("revolutionary"));
    SunYatSen.HistoricalEvents.Add(TEXT("xinhai_revolution"));
    SunYatSen.Achievements.Add(TEXT("founded_republic"));
    
    RegisterHistoricalCharacter(SunYatSen);

    // 蔣介石
    FMingHistoricalCharacter ChiangKaiShek;
    ChiangKaiShek.CharacterID = TEXT("chiang_kai_shek");
    ChiangKaiShek.CharacterName = TEXT("蔣介石");
    ChiangKaiShek.CourtesyName = TEXT("介石");
    ChiangKaiShek.EnglishName = TEXT("Chiang Kai-shek");
    ChiangKaiShek.CharacterDescription = TEXT("中華民國政治家、軍事家");
    ChiangKaiShek.Gender = ECharacterGender::Male;
    ChiangKaiShek.Profession = ECharacterProfession::Military;
    ChiangKaiShek.Faction = ECharacterFaction::Nationalist;
    ChiangKaiShek.BirthYear = 1887;
    ChiangKaiShek.DeathYear = 1975;
    ChiangKaiShek.State = ECharacterState::Deceased;
    ChiangKaiShek.Attributes.Leadership = 85.0f;
    ChiangKaiShek.Attributes.Military = 90.0f;
    ChiangKaiShek.Attributes.Administration = 80.0f;
    ChiangKaiShek.Attributes.Strategy = 85.0f;
    ChiangKaiShek.Influence = 850.0f;
    ChiangKaiShek.Reputation = 800.0f;
    ChiangKaiShek.bIsKeyCharacter = true;
    ChiangKaiShek.bIsRecruitable = false;
    ChiangKaiShek.CharacterTags.Add(TEXT("military_leader"));
    ChiangKaiShek.CharacterTags.Add(TEXT("nationalist"));
    ChiangKaiShek.HistoricalEvents.Add(TEXT("northern_expedition"));
    ChiangKaiShek.HistoricalEvents.Add(TEXT("second_sino_japanese_war"));
    ChiangKaiShek.Achievements.Add(TEXT("unified_china"));
    
    RegisterHistoricalCharacter(ChiangKaiShek);
}

void UMingHistoricalCharacterManager::CreateMilitaryCharacters()
{
    // 毛澤東
    FMingHistoricalCharacter MaoZedong;
    MaoZedong.CharacterID = TEXT("mao_zedong");
    MaoZedong.CharacterName = TEXT("毛澤東");
    MaoZedong.CourtesyName = TEXT("潤之");
    MaoZedong.EnglishName = TEXT("Mao Zedong");
    MaoZedong.CharacterDescription = TEXT("中國共產黨主要創立人，中華人民共和國領袖");
    MaoZedong.Gender = ECharacterGender::Male;
    MaoZedong.Profession = ECharacterProfession::Revolutionary;
    MaoZedong.Faction = ECharacterFaction::Communist;
    MaoZedong.BirthYear = 1893;
    MaoZedong.DeathYear = 1976;
    MaoZedong.State = ECharacterState::Deceased;
    MaoZedong.Attributes.Leadership = 90.0f;
    MaoZedong.Attributes.Strategy = 85.0f;
    MaoZedong.Attributes.Writing = 80.0f;
    MaoZedong.Attributes.Intelligence = 85.0f;
    MaoZedong.Influence = 950.0f;
    MaoZedong.Reputation = 900.0f;
    MaoZedong.bIsKeyCharacter = true;
    MaoZedong.bIsRecruitable = false;
    MaoZedong.CharacterTags.Add(TEXT("communist_leader"));
    MaoZedong.CharacterTags.Add(TEXT("revolutionary"));
    MaoZedong.HistoricalEvents.Add(TEXT("chinese_civil_war"));
    MaoZedong.Achievements.Add(TEXT("founded_prc"));
    
    RegisterHistoricalCharacter(MaoZedong);
}

void UMingHistoricalCharacterManager::CreateBusinessCharacters()
{
    // 張謇
    FMingHistoricalCharacter ZhangXueliang;
    ZhangXueliang.CharacterID = TEXT("zhang_xueliang");
    ZhangXueliang.CharacterName = TEXT("張學良");
    ZhangXueliang.EnglishName = TEXT("Zhang Xueliang");
    ZhangXueliang.CharacterDescription = TEXT("東北軍閥，西安事變發動者");
    ZhangXueliang.Gender = ECharacterGender::Male;
    ZhangXueliang.Profession = ECharacterProfession::Military;
    ZhangXueliang.Faction = ECharacterFaction::Warlord;
    ZhangXueliang.BirthYear = 1901;
    ZhangXueliang.DeathYear = 2001;
    ZhangXueliang.State = ECharacterState::Deceased;
    ZhangXueliang.Attributes.Military = 75.0f;
    ZhangXueliang.Attributes.Diplomacy = 70.0f;
    ZhangXueliang.Attributes.Leadership = 70.0f;
    ZhangXueliang.Influence = 600.0f;
    ZhangXueliang.Reputation = 650.0f;
    ZhangXueliang.bIsKeyCharacter = true;
    ZhangXueliang.bIsRecruitable = true;
    ZhangXueliang.CharacterTags.Add(TEXT("warlord"));
    ZhangXueliang.CharacterTags.Add(TEXT("xian_incident"));
    ZhangXueliang.HistoricalEvents.Add(TEXT("xian_incident"));
    ZhangXueliang.Achievements.Add(TEXT("forced_united_front"));
    
    RegisterHistoricalCharacter(ZhangXueliang);
}

void UMingHistoricalCharacterManager::CreateScholarCharacters()
{
    // 胡適
    FMingHistoricalCharacter HuShih;
    HuShih.CharacterID = TEXT("hu_shih");
    HuShih.CharacterName = TEXT("胡適");
    HuShih.EnglishName = TEXT("Hu Shih");
    HuShih.CharacterDescription = TEXT("中國現代學者、思想家、文學家");
    HuShih.Gender = ECharacterGender::Male;
    HuShih.Profession = ECharacterProfession::Scholar;
    HuShih.Faction = ECharacterFaction::Neutral;
    HuShih.BirthYear = 1891;
    HuShih.DeathYear = 1962;
    HuShih.State = ECharacterState::Deceased;
    HuShih.Attributes.Intelligence = 95.0f;
    HuShih.Attributes.Writing = 90.0f;
    HuShih.Attributes.Science = 85.0f;
    HuShih.Attributes.Charisma = 80.0f;
    HuShih.Influence = 700.0f;
    HuShih.Reputation = 750.0f;
    HuShih.bIsKeyCharacter = false;
    HuShih.bIsRecruitable = true;
    HuShih.CharacterTags.Add(TEXT("scholar"));
    HuShih.CharacterTags.Add(TEXT("literary"));
    HuShih.Achievements.Add(TEXT("new_culture_movement"));
    
    RegisterHistoricalCharacter(HuShih);
}

void UMingHistoricalCharacterManager::CreateRevolutionaryCharacters()
{
    // 宋慶齡
    FMingHistoricalCharacter SoongChingLing;
    SoongChingLing.CharacterID = TEXT("soong_ching_ling");
    SoongChingLing.CharacterName = TEXT("宋慶齡");
    SoongChingLing.EnglishName = TEXT("Soong Ching-ling");
    SoongChingLing.CharacterDescription = TEXT("中華人民共和國名譽主席，孫中山夫人");
    SoongChingLing.Gender = ECharacterGender::Female;
    SoongChingLing.Profession = ECharacterProfession::Politician;
    SoongChingLing.Faction = ECharacterFaction::Communist;
    SoongChingLing.BirthYear = 1893;
    SoongChingLing.DeathYear = 1981;
    SoongChingLing.State = ECharacterState::Deceased;
    SoongChingLing.Attributes.Charisma = 85.0f;
    SoongChingLing.Attributes.Diplomacy = 80.0f;
    SoongChingLing.Attributes.Intelligence = 75.0f;
    SoongChingLing.Influence = 800.0f;
    SoongChingLing.Reputation = 850.0f;
    SoongChingLing.bIsKeyCharacter = true;
    SoongChingLing.bIsRecruitable = false;
    SoongChingLing.CharacterTags.Add(TEXT("female_leader"));
    SoongChingLing.CharacterTags.Add(TEXT("revolutionary"));
    SoongChingLing.Achievements.Add(TEXT("women_rights_advocate"));
    
    RegisterHistoricalCharacter(SoongChingLing);
}

void UMingHistoricalCharacterManager::CreateDiplomaticCharacters()
{
    // 周恩來
    FMingHistoricalCharacter ZhouEnlai;
    ZhouEnlai.CharacterID = TEXT("zhou_enlai");
    ZhouEnlai.CharacterName = TEXT("周恩來");
    ZhouEnlai.EnglishName = TEXT("Zhou Enlai");
    ZhouEnlai.CharacterDescription = TEXT("中華人民共和國第一任國務院總理");
    ZhouEnlai.Gender = ECharacterGender::Male;
    ZhouEnlai.Profession = ECharacterProfession::Diplomat;
    ZhouEnlai.Faction = ECharacterFaction::Communist;
    ZhouEnlai.BirthYear = 1898;
    ZhouEnlai.DeathYear = 1976;
    ZhouEnlai.State = ECharacterState::Deceased;
    ZhouEnlai.Attributes.Diplomacy = 95.0f;
    ZhouEnlai.Attributes.Charisma = 90.0f;
    ZhouEnlai.Attributes.Administration = 85.0f;
    ZhouEnlai.Attributes.Intelligence = 85.0f;
    ZhouEnlai.Influence = 900.0f;
    ZhouEnlai.Reputation = 950.0f;
    ZhouEnlai.bIsKeyCharacter = true;
    ZhouEnlai.bIsRecruitable = false;
    ZhouEnlai.CharacterTags.Add(TEXT("diplomat"));
    ZhouEnlai.CharacterTags.Add(TEXT("premier"));
    ZhouEnlai.Achievements.Add(TEXT("established_diplomatic_relations"));
    
    RegisterHistoricalCharacter(ZhouEnlai);
}

void UMingHistoricalCharacterManager::CreateCulturalCharacters()
{
    // 魯迅
    FMingHistoricalCharacter LuXun;
    LuXun.CharacterID = TEXT("lu_xun");
    LuXun.CharacterName = TEXT("魯迅");
    LuXun.EnglishName = TEXT("Lu Xun");
    LuXun.CharacterDescription = TEXT("中國現代文學家、思想家");
    LuXun.Gender = ECharacterGender::Male;
    LuXun.Profession = ECharacterProfession::Artist;
    LuXun.Faction = ECharacterFaction::Neutral;
    LuXun.BirthYear = 1881;
    LuXun.DeathYear = 1936;
    LuXun.State = ECharacterState::Deceased;
    LuXun.Attributes.Writing = 95.0f;
    LuXun.Attributes.Intelligence = 85.0f;
    LuXun.Attributes.Charisma = 80.0f;
    LuXun.Influence = 750.0f;
    LuXun.Reputation = 800.0f;
    LuXun.bIsKeyCharacter = false;
    LuXun.bIsRecruitable = true;
    LuXun.CharacterTags.Add(TEXT("writer"));
    LuXun.CharacterTags.Add(TEXT("literary"));
    LuXun.Achievements.Add(TEXT("modern_chinese_literature"));
    
    RegisterHistoricalCharacter(LuXun);
}

// === 互動處理方法 ===

void UMingHistoricalCharacterManager::HandleCharacterInteraction(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result)
{
    FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        return;
    }

    // 根據互動類型處理
    if (InteractionType == TEXT("conversation"))
    {
        Result.ResultDescription = TEXT("Had a meaningful conversation");
        Result.GainedExperience = 10;
        
        // 增加魅力和外交能力
        Character->Attributes.Charisma = FMath::Clamp(Character->Attributes.Charisma + 1.0f, 0.0f, 100.0f);
        Character->Attributes.Diplomacy = FMath::Clamp(Character->Attributes.Diplomacy + 0.5f, 0.0f, 100.0f);
    }
    else if (InteractionType == TEXT("debate"))
    {
        Result.ResultDescription = TEXT("Engaged in intellectual debate");
        Result.GainedExperience = 15;
        
        // 增加智力演講能力
        Character->Attributes.Intelligence = FMath::Clamp(Character->Attributes.Intelligence + 1.5f, 0.0f, 100.0f);
        Character->Attributes.Oratory = FMath::Clamp(Character->Attributes.Oratory + 1.0f, 0.0f, 100.0f);
    }
    else if (InteractionType == TEXT("collaboration"))
    {
        Result.ResultDescription = TEXT("Collaborated on a project");
        Result.GainedExperience = 20;
        
        // 增加行政和領導力
        Character->Attributes.Administration = FMath::Clamp(Character->Attributes.Administration + 1.0f, 0.0f, 100.0f);
        Character->Attributes.Leadership = FMath::Clamp(Character->Attributes.Leadership + 0.5f, 0.0f, 100.0f);
    }
    else
    {
        Result.ResultDescription = FString::Printf(TEXT("Interacted with: %s"), *InteractionType);
        Result.GainedExperience = 5;
    }

    // 增加經驗值
    Character->Attributes.Experience += Result.GainedExperience;
    
    // 檢查升級
    if (Character->Attributes.Experience >= Character->Attributes.Level * 100)
    {
        Character->Attributes.Level++;
        Character->Attributes.Experience = 0;
        Result.ResultDescription += FString::Printf(TEXT(" - Level up to %d!"), Character->Attributes.Level);
    }
}

void UMingHistoricalCharacterManager::HandleDialogueLogic(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, FDialogueEvent& Event)
{
    const FMingHistoricalCharacter* Speaker = CharacterMap.Find(SpeakerID);
    const FMingHistoricalCharacter* Audience = CharacterMap.Find(AudienceID);
    
    if (!Speaker || !Audience)
    {
        return;
    }

    // 生成對話文本
    if (DialogueTopic == TEXT("politics"))
    {
        Event.DialogueText = FString::Printf(TEXT("%s discusses political matters with %s"), *Speaker->CharacterName, *Audience->CharacterName);
        Event.DialogueMood = TEXT("serious");
    }
    else if (DialogueTopic == TEXT("military"))
    {
        Event.DialogueText = FString::Printf(TEXT("%s talks about military strategy with %s"), *Speaker->CharacterName, *Audience->CharacterName);
        Event.DialogueMood = TEXT("focused");
    }
    else if (DialogueTopic == TEXT("culture"))
    {
        Event.DialogueText = FString::Printf(TEXT("%s shares cultural insights with %s"), *Speaker->CharacterName, *Audience->CharacterName);
        Event.DialogueMood = TEXT("relaxed");
    }
    else
    {
        Event.DialogueText = FString::Printf(TEXT("%s and %s have a conversation"), *Speaker->CharacterName, *Audience->CharacterName);
        Event.DialogueMood = TEXT("neutral");
    }

    // 生成對話選項
    FDialogueOption Option1;
    Option1.OptionID = TEXT("agree");
    Option1.OptionText = TEXT("I agree with your point of view");
    Option1.OptionDescription = TEXT("Express agreement");
    Option1.Weight = 1.0f;
    
    FDialogueOption Option2;
    Option2.OptionID = TEXT("disagree");
    Option2.OptionText = TEXT("I have a different perspective");
    Option2.OptionDescription = TEXT("Express disagreement");
    Option2.Weight = 0.8f;
    
    FDialogueOption Option3;
    Option3.OptionID = TEXT("question");
    Option3.OptionText = TEXT("Can you elaborate on that?");
    Option3.OptionDescription = TEXT("Ask for clarification");
    Option3.Weight = 0.6f;
    
    Event.DialogueOptions.Add(Option1);
    Event.DialogueOptions.Add(Option2);
    Event.DialogueOptions.Add(Option3);
    
    // 設置對話重要性
    Event.DialogueImportance = 0.5f;
    if (Speaker->bIsKeyCharacter || Audience->bIsKeyCharacter)
    {
        Event.DialogueImportance = 0.8f;
    }
}

float UMingHistoricalCharacterManager::CalculateInteractionScore(const FString& CharacterID, const FString& InteractionType) const
{
    const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        return 0.0f;
    }

    float Score = 0.5f; // 基礎分數
    
    // 根據人物影響力加分
    Score += Character->Influence / 1000.0f * 0.3f;
    
    // 根據互動類型加分
    if (InteractionType == TEXT("collaboration"))
    {
        Score += 0.2f;
    }
    else if (InteractionType == TEXT("debate"))
    {
        Score += 0.1f;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

void UMingHistoricalCharacterManager::UpdateCharacterRelationships(const FString& CharacterID, const FString& TargetCharacterID, const FString& InteractionType)
{
    // 獲取現有關係
    TArray<FCharacterRelationship>* Relationships = RelationshipNetwork.Find(CharacterID);
    if (!Relationships)
    {
        // 創建新關係
        AddCharacterRelationship(CharacterID, TargetCharacterID, ECharacterRelationshipType::Acquaintance, 50.0f);
        return;
    }
    
    // 更新現有關係
    for (FCharacterRelationship& Relationship : *Relationships)
    {
        if (Relationship.TargetCharacterID == TargetCharacterID)
        {
            float StrengthChange = 0.0f;
            
            if (InteractionType == TEXT("conversation"))
            {
                StrengthChange = 5.0f;
            }
            else if (InteractionType == TEXT("collaboration"))
            {
                StrengthChange = 10.0f;
            }
            else if (InteractionType == TEXT("debate"))
            {
                StrengthChange = -2.0f; // 辯論可能降低關係
            }
            
            Relationship.RelationshipStrength = FMath::Clamp(Relationship.RelationshipStrength + StrengthChange, 0.0f, 100.0f);
            Relationship.LastInteractionTime = FPlatformTime::Seconds();
            Relationship.InteractionCount++;
            
            // 更新關係類型
            if (Relationship.RelationshipStrength >= 80.0f)
            {
                Relationship.RelationshipType = ECharacterRelationshipType::Friend;
            }
            else if (Relationship.RelationshipStrength >= 60.0f)
            {
                Relationship.RelationshipType = ECharacterRelationshipType::Colleague;
            }
            else if (Relationship.RelationshipStrength <= 20.0f)
            {
                Relationship.RelationshipType = ECharacterRelationshipType::Rival;
            }
            
            break;
        }
    }
}

void UMingHistoricalCharacterManager::ApplyInteractionEffects(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result)
{
    FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        return;
    }

    // 記錄影響的屬性
    if (InteractionType == TEXT("conversation"))
    {
        Result.AffectedAttributes.Add(TEXT("charisma"), 1.0f);
        Result.AffectedAttributes.Add(TEXT("diplomacy"), 0.5f);
    }
    else if (InteractionType == TEXT("debate"))
    {
        Result.AffectedAttributes.Add(TEXT("intelligence"), 1.5f);
        Result.AffectedAttributes.Add(TEXT("oratory"), 1.0f);
    }
    else if (InteractionType == TEXT("collaboration"))
    {
        Result.AffectedAttributes.Add(TEXT("administration"), 1.0f);
        Result.AffectedAttributes.Add(TEXT("leadership"), 0.5f);
    }
    
    // 更新影響力
    Character->Influence = FMath::Clamp(Character->Influence + Result.InteractionScore * 10.0f, 0.0f, 1000.0f);
}

bool UMingHistoricalCharacterManager::CheckInteractionConditions(const FString& CharacterID, const FString& InteractionType) const
{
    const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        return false;
    }

    // 檢查人物狀態
    if (Character->State != ECharacterState::Active)
    {
        return false;
    }

    // 檢查健康狀況
    if (Character->Health < 20.0f)
    {
        return false;
    }

    // 檢查精神狀態
    if (Character->Morale < 10.0f)
    {
        return false;
    }

    return true;
}

float UMingHistoricalCharacterManager::CalculateCharacterInfluence(const FMingHistoricalCharacter& Character) const
{
    float Influence = 0.0f;
    
    // 基於屬性計算影響力
    Influence += Character.Attributes.Leadership * 2.0f;
    Influence += Character.Attributes.Charisma * 1.5f;
    Influence += Character.Attributes.Diplomacy * 1.5f;
    Influence += Character.Attributes.Intelligence * 1.0f;
    
    // 基於職業加分
    switch (Character.Profession)
    {
    case ECharacterProfession::Politician:
        Influence += 100.0f;
        break;
    case ECharacterProfession::Military:
        Influence += 80.0f;
        break;
    case ECharacterProfession::Diplomat:
        Influence += 70.0f;
        break;
    case ECharacterProfession::Revolutionary:
        Influence += 60.0f;
        break;
    default:
        break;
    }
    
    // 基於關鍵性加分
    if (Character.bIsKeyCharacter)
    {
        Influence += 200.0f;
    }
    
    return FMath::Clamp(Influence, 0.0f, 1000.0f);
}

float UMingHistoricalCharacterManager::CalculateCharacterReputation(const FMingHistoricalCharacter& Character) const
{
    float Reputation = 0.0f;
    
    // 基於屬性計算聲望
    Reputation += Character.Attributes.Leadership * 1.5f;
    Reputation += Character.Attributes.Charisma * 1.0f;
    Reputation += Character.Attributes.Administration * 1.0f;
    Reputation += Character.Attributes.Strategy * 1.0f;
    
    // 基於成就加分
    Reputation += Character.Achievements.Num() * 50.0f;
    
    // 基於歷史事件加分
    Reputation += Character.HistoricalEvents.Num() * 30.0f;
    
    return FMath::Clamp(Reputation, 0.0f, 1000.0f);
}

void UMingHistoricalCharacterManager::UpdateCharacterState(const FString& CharacterID, ECharacterState NewState)
{
    FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        return;
    }
    
    Character->State = NewState;
    UE_LOG(LogTemp, Log, TEXT("Character state updated: %s -> %s"), *CharacterID, *UEnum::GetValueAsString(NewState));
}

void UMingHistoricalCharacterManager::RecordDialogueEvent(const FDialogueEvent& Event)
{
    DialogueHistory.Add(Event);
    
    // 限制歷史記錄數量
    if (DialogueHistory.Num() > 1000)
    {
        DialogueHistory.RemoveAt(0);
    }
}

float UMingHistoricalCharacterManager::GetCharacterRecommendationScore(const FMingHistoricalCharacter& Character, const FString& PlayerFaction) const
{
    float Score = 0.5f; // 基礎分數
    
    // 基於陣營匹配度
    if (PlayerFaction == UEnum::GetValueAsString(Character.Faction))
    {
        Score += 0.3f;
    }
    else if (Character.Faction == ECharacterFaction::Neutral)
    {
        Score += 0.1f;
    }
    else
    {
        Score -= 0.2f;
    }
    
    // 基於影響力
    Score += Character.Influence / 1000.0f * 0.3f;
    
    // 基於可招募性
    if (Character.bIsRecruitable && Character.State == ECharacterState::Active)
    {
        Score += 0.2f;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

bool UMingHistoricalCharacterManager::ValidateCharacterConfig(const FMingHistoricalCharacter& Character) const
{
    if (Character.CharacterID.IsEmpty())
    {
        return false;
    }
    
    if (Character.CharacterName.IsEmpty())
    {
        return false;
    }
    
    if (Character.BirthYear <= 0 || Character.DeathYear < Character.BirthYear)
    {
        return false;
    }
    
    return true;
}

void UMingHistoricalCharacterManager::LogCharacterEvent(const FString& CharacterID, const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[Character %s] %s"), *CharacterID, *Message);
}

FString UMingHistoricalCharacterManager::GenerateCharacterReport(const FString& CharacterID) const
{
    const FMingHistoricalCharacter* Character = CharacterMap.Find(CharacterID);
    if (!Character)
    {
        return TEXT("Character not found");
    }
    
    FString Report = TEXT("=== 人物報告 ===\n");
    Report += FString::Printf(TEXT("姓名: %s (%s)\n"), *Character->CharacterName, *Character->CourtesyName);
    Report += FString::Printf(TEXT("職業: %s\n"), *UEnum::GetValueAsString(Character->Profession));
    Report += FString::Printf(TEXT("陣營: %s\n"), *UEnum::GetValueAsString(Character->Faction));
    Report += FString::Printf(TEXT("生卒: %d-%d\n"), Character->BirthYear, Character->DeathYear);
    Report += FString::Printf(TEXT("狀態: %s\n"), *UEnum::GetValueAsString(Character->State));
    Report += FString::Printf(TEXT("影響力: %.1f\n"), Character->Influence);
    Report += FString::Printf(TEXT("聲望: %.1f\n"), Character->Reputation);
    Report += FString::Printf(TEXT("等級: %d\n"), Character->Attributes.Level);
    Report += FString::Printf(TEXT("經驗: %d\n"), Character->Attributes.Experience);
    
    return Report;
}
