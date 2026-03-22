#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalCharacter.generated.h"

/**
 * 人物?�別
 */
UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
    Male                UMETA(DisplayName = "Male"),
    Female              UMETA(DisplayName = "Female"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 人物?�業/身份
 */
UENUM(BlueprintType)
enum class ECharacterProfession : uint8
{
    Politician          UMETA(DisplayName = "Politician"),
    Military            UMETA(DisplayName = "Military"),
    Businessman         UMETA(DisplayName = "Businessman"),
    Scholar             UMETA(DisplayName = "Scholar"),
    Revolutionary      UMETA(DisplayName = "Revolutionary"),
    Diplomat            UMETA(DisplayName = "Diplomat"),
    Journalist          UMETA(DisplayName = "Journalist"),
    Artist              UMETA(DisplayName = "Artist"),
    Teacher             UMETA(DisplayName = "Teacher"),
    Doctor              UMETA(DisplayName = "Doctor"),
    Engineer            UMETA(DisplayName = "Engineer"),
    Farmer              UMETA(DisplayName = "Farmer"),
    Worker              UMETA(DisplayName = "Worker"),
    Merchant            UMETA(DisplayName = "Merchant"),
    Noble               UMETA(DisplayName = "Noble"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 人物X/派系
 */
UENUM(BlueprintType)
enum class ECharacterFaction : uint8
{
    Nationalist         UMETA(DisplayName = "Nationalist"),
    Communist           UMETA(DisplayName = "Communist"),
    Warlord             UMETA(DisplayName = "Warlord"),
    Imperial            UMETA(DisplayName = "Imperial"),
    Foreign             UMETA(DisplayName = "Foreign"),
    Neutral             UMETA(DisplayName = "Neutral"),
    Independent         UMETA(DisplayName = "Independent"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 人物?�X */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    Active              UMETA(DisplayName = "Active"),
    Inactive            UMETA(DisplayName = "Inactive"),
    Imprisoned          UMETA(DisplayName = "Imprisoned"),
    Exiled              UMETA(DisplayName = "Exiled"),
    Deceased            UMETA(DisplayName = "Deceased"),
    Retired             UMETA(DisplayName = "Retired"),
    Hidden              UMETA(DisplayName = "Hidden"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 人物?��?類�?
 */
UENUM(BlueprintType)
enum class ECharacterRelationshipType : uint8
{
    Family              UMETA(DisplayName = "Family"),
    Friend              UMETA(DisplayName = "Friend"),
    Ally                UMETA(DisplayName = "Ally"),
    Rival               UMETA(DisplayName = "Rival"),
    Enemy               UMETA(DisplayName = "Enemy"),
    Mentor              UMETA(DisplayName = "Mentor"),
    Student             UMETA(DisplayName = "Student"),
    Colleague           UMETA(DisplayName = "Colleague"),
    Subordinate         UMETA(DisplayName = "Subordinate"),
    Superior            UMETA(DisplayName = "Superior"),
    Spouse              UMETA(DisplayName = "Spouse"),
    Acquaintance        UMETA(DisplayName = "Acquaintance"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 人物?�?��X */
UENUM(BlueprintType)
enum class ECharacterSkillType : uint8
{
    Leadership          UMETA(DisplayName = "Leadership"),
    Military            UMETA(DisplayName = "Military"),
    Diplomacy           UMETA(DisplayName = "Diplomacy"),
    Economics           UMETA(DisplayName = "Economics"),
    Intelligence        UMETA(DisplayName = "Intelligence"),
    Charisma            UMETA(DisplayName = "Charisma"),
    Administration     UMETA(DisplayName = "Administration"),
    Strategy            UMETA(DisplayName = "Strategy"),
    Oratory             UMETA(DisplayName = "Oratory"),
    Writing             UMETA(DisplayName = "Writing"),
    Science             UMETA(DisplayName = "Science"),
    Engineering         UMETA(DisplayName = "Engineering"),
    Medicine            UMETA(DisplayName = "Medicine"),
    Art                 UMETA(DisplayName = "Art"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 人物屬�? */
USTRUCT(BlueprintType)
struct FHistoricalCharacterAttributes
{
    GENERATED_BODY()

    // ?��X(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Leadership;

    // 軍�X�能 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Military;

    // 外交?��? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Diplomacy;

    // 經�X�能 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Economics;

    // ?��?水平 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Intelligence;

    // 魅�X(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Charisma;

    // 行政?��? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Administration;

    // ?�略?�維 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Strategy;

    // 演�X��? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Oratory;

    // 寫�X��? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Writing;

    // 科學?��? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Science;

    // 工�X��?(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Engineering;

    // ?�學?��? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Medicine;

    // ?��X�能 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Art;

    // 經�X(0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    int32 Experience;

    // 等�? (1-10)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    int32 Level;

    FHistoricalCharacterAttributes()
        : Leadership(50.0f)
        , Military(50.0f)
        , Diplomacy(50.0f)
        , Economics(50.0f)
        , Intelligence(50.0f)
        , Charisma(50.0f)
        , Administration(50.0f)
        , Strategy(50.0f)
        , Oratory(50.0f)
        , Writing(50.0f)
        , Science(50.0f)
        , Engineering(50.0f)
        , Medicine(50.0f)
        , Art(50.0f)
        , Experience(0)
        , Level(1)
    {}
};

/**
 * 人物?�X */
USTRUCT(BlueprintType)
struct FHistoricalCharacterSkill
{
    GENERATED_BODY()

    // ?�?��X    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    ECharacterSkillType SkillType;

    // ?�?��?�?    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillName;

    // ?�?��?�?    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillDescription;

    // ?�?��?�?(0-10)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillLevel;

    // ?�?��?�?(0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillExperience;

    // ?�否已解X    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    bool bIsUnlocked;

    // ?�?�冷?��X(�?
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float CooldownTime;

    // 上次使用?��?
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float LastUsedTime;

    // ?�?��X    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    TArray<FString> SkillEffects;

    FHistoricalCharacterSkill()
        : SkillType(ECharacterSkillType::Leadership)
        , SkillName(TEXT(""))
        , SkillDescription(TEXT(""))
        , SkillLevel(1)
        , SkillExperience(0)
        , bIsUnlocked(false)
        , CooldownTime(0.0f)
        , LastUsedTime(0.0f)
    {}
};

/**
 * 人物?��?
 */
USTRUCT(BlueprintType)
struct FCharacterRelationship
{
    GENERATED_BODY()

    // ?��?類�?
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    ECharacterRelationshipType RelationshipType;

    // ?��?對象ID
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString TargetCharacterID;

    // ?��?強度 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float RelationshipStrength;

    // ?��X�述
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString RelationshipDescription;

    // 建�X��?
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float EstablishedTime;

    // ?�否?��?
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    bool bIsPublic;

    // 互�?次數
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    int32 InteractionCount;

    // ?�後�X��X    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float LastInteractionTime;

    FCharacterRelationship()
        : RelationshipType(ECharacterRelationshipType::Unknown)
        , TargetCharacterID(TEXT(""))
        , RelationshipStrength(50.0f)
        , RelationshipDescription(TEXT(""))
        , EstablishedTime(0.0f)
        , bIsPublic(true)
        , InteractionCount(0)
        , LastInteractionTime(0.0f)
    {}
};

/**
 * 對話?��?
 */
USTRUCT(BlueprintType)
struct FDialogueOption
{
    GENERATED_BODY()

    // ?��?ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionID;

    // ?��X�本
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionText;

    // ?��X�述
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionDescription;

    // ?��?條件
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Conditions;

    // ?��?後�?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Consequences;

    // ?��?權�?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    float Weight;

    // ?�否?�用
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    bool bIsAvailable;

    FDialogueOption()
        : OptionID(TEXT(""))
        , OptionText(TEXT(""))
        , OptionDescription(TEXT(""))
        , Weight(1.0f)
        , bIsAvailable(true)
    {}
};

/**
 * 對話事件
 */
USTRUCT(BlueprintType)
struct FDialogueEvent
{
    GENERATED_BODY()

    // 事件ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString EventID;

    // 對話?�本
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueText;

    // 說話?�ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString SpeakerID;

    // ?�眾ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString AudienceID;

    // 對話?��?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    TArray<FDialogueOption> DialogueOptions;

    // 對話?��?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    float DialogueTime;

    // 對話?��?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueLocation;

    // 對話主�?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueTopic;

    // 對話?��?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueMood;

    // 對話?��X    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    float DialogueImportance;

    FDialogueEvent()
        : EventID(TEXT(""))
        , DialogueText(TEXT(""))
        , SpeakerID(TEXT(""))
        , AudienceID(TEXT(""))
        , DialogueTime(0.0f)
        , DialogueLocation(TEXT(""))
        , DialogueTopic(TEXT(""))
        , DialogueMood(TEXT(""))
        , DialogueImportance(0.5f)
    {}
};

/**
 * 民�?歷史人物
 */
USTRUCT(BlueprintType)
struct FMingStrategicHistoricalCharacter
{
    GENERATED_BODY()

    // 人物ID
    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    // 人物姓�?
    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    // 人物字�?
    UPROPERTY(BlueprintReadOnly)
    FString CourtesyName;

    // 人物?��X    UPROPERTY(BlueprintReadOnly)
    FString EnglishName;

    // 人物?�述
    UPROPERTY(BlueprintReadOnly)
    FString CharacterDescription;

    // 人物?��?
    UPROPERTY(BlueprintReadOnly)
    FString PortraitPath;

    // ?�別
    UPROPERTY(BlueprintReadOnly)
    ECharacterGender Gender;

    // ?�業/身份
    UPROPERTY(BlueprintReadOnly)
    ECharacterProfession Profession;

    // X/派系
    UPROPERTY(BlueprintReadOnly)
    ECharacterFaction Faction;

    // ?��?年份
    UPROPERTY(BlueprintReadOnly)
    int32 BirthYear;

    // 死亡年份
    UPROPERTY(BlueprintReadOnly)
    int32 DeathYear;

    // 人物?�X    UPROPERTY(BlueprintReadOnly)
    ECharacterState State;

    // 人物屬�?    UPROPERTY(BlueprintReadOnly)
    FHistoricalCharacterAttributes Attributes;

    // 人物?�X    UPROPERTY(BlueprintReadOnly)
    TArray<FHistoricalCharacterSkill> Skills;

    // 人物?��?
    UPROPERTY(BlueprintReadOnly)
    TArray<FCharacterRelationship> Relationships;

    // 人物標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CharacterTags;

    // 歷史事�?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> HistoricalEvents;

    // ?��X�就
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Achievements;

    // 影響?��?(0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Influence;

    // ?��X(0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Reputation;

    // 忠�?�?(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Loyalty;

    // ?�康?��?(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Health;

    // 精�X�X(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Morale;

    // ?��?位置
    UPROPERTY(BlueprintReadOnly)
    FString CurrentLocation;

    // ?��X��?
    UPROPERTY(BlueprintReadOnly)
    FString CurrentPosition;

    // ?�否?��X�人X    UPROPERTY(BlueprintReadOnly)
    bool bIsKeyCharacter;

    // ?�否?��X    UPROPERTY(BlueprintReadOnly)
    bool bIsRecruitable;

    // ?��X�本
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> RecruitmentCost;

    FMingHistoricalCharacter()
        : CharacterID(TEXT(""))
        , CharacterName(TEXT(""))
        , CourtesyName(TEXT(""))
        , EnglishName(TEXT(""))
        , CharacterDescription(TEXT(""))
        , PortraitPath(TEXT(""))
        , Gender(ECharacterGender::Unknown)
        , Profession(ECharacterProfession::Unknown)
        , Faction(ECharacterFaction::Unknown)
        , BirthYear(0)
        , DeathYear(0)
        , State(ECharacterState::Active)
        , Influence(0.0f)
        , Reputation(0.0f)
        , Loyalty(100.0f)
        , Health(100.0f)
        , Morale(100.0f)
        , CurrentLocation(TEXT(""))
        , CurrentPosition(TEXT(""))
        , bIsKeyCharacter(false)
        , bIsRecruitable(true)
    {}
};

/**
 * 人物互�?結�?
 */
USTRUCT(BlueprintType)
struct FCharacterInteractionResult
{
    GENERATED_BODY()

    // ?�否?��?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 互�?類�?
    UPROPERTY(BlueprintReadOnly)
    FString InteractionType;

    // 互�?對象ID
    UPROPERTY(BlueprintReadOnly)
    FString TargetCharacterID;

    // 互�X��?
    UPROPERTY(BlueprintReadOnly)
    float InteractionTime;

    // 互�X��?
    UPROPERTY(BlueprintReadOnly)
    FString InteractionLocation;

    // 互�?結�X�述
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // 影響?�人?�屬X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> AffectedAttributes;

    // 影響?�人?��?�?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRelationships;

    // ?��X��?�?    UPROPERTY(BlueprintReadOnly)
    int32 GainedExperience;

    // 互�X�數
    UPROPERTY(BlueprintReadOnly)
    float InteractionScore;

    FCharacterInteractionResult()
        : bSuccess(false)
        , InteractionType(TEXT(""))
        , TargetCharacterID(TEXT(""))
        , InteractionTime(0.0f)
        , InteractionLocation(TEXT(""))
        , ResultDescription(TEXT(""))
        , GainedExperience(0)
        , InteractionScore(0.0f)
    {}
};

/**
 * 歷史人物管�X * 管�X�?��X�歷?�人?��?互�X��?�? */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalCharacterManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalCharacterManager(};

    /**
     * ?��X�人?�管?�器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Initialize(};

    /**
     * ?��?人物管�X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Shutdown(};

    /**
     * 註�?歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character};

    /**
     * ?��?註�?歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UnregisterHistoricalCharacter(const FString& CharacterID};

    /**
     * ?��?歷史人物
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID) const;

    /**
     * ?��X�?�歷?�人X     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters() const;

    /**
     * ?��X��X�人X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction) const;

    /**
     * ?��X��X�業?�人X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession) const;

    /**
     * ?��X�鍵人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetKeyCharacters() const;

    /**
     * ?��X��X�人X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecruitableCharacters() const;

    /**
     * ?��?活�?人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetActiveCharacters() const;

    /**
     * 人物互�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails};

    /**
     * 對話系統
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic};

    /**
     * ?��?對話?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID};

    /**
     * ?��?人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RecruitCharacter(const FString& CharacterID};

    /**
     * �?��人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool DismissCharacter(const FString& CharacterID};

    /**
     * ?��?人物?�X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType};

    /**
     * ?��?人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID) const;

    /**
     * 添�?人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength};

    /**
     * ?�新人物?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength};

    /**
     * ?��?人物統�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TMap<FString, int32> GetCharacterStatistics() const;

    /**
     * ?��?人物影響?��X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking() const;

    /**
     * ?��X�薦人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction) const;

protected:
    // 已註?��?歷史人物
    UPROPERTY()
    TArray<FMingHistoricalCharacter> RegisteredCharacters;

    // 人物ID?�人?��X��?
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> CharacterMap;

    // 人物?��?網絡 - 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<FString, TArray<FCharacterRelationship>> RelationshipNetwork;

    // 對話事件記�?
    UPROPERTY()
    TArray<FDialogueEvent> DialogueHistory;

    // 人物統�?
    UPROPERTY()
    TMap<FString, int32> CharacterStatistics;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?��X��X�歷?�人?�庫
    void InitializeHistoricalCharacterLibrary(};

    // ?�建?�治人物
    void CreatePoliticalCharacters(};

    // ?�建軍�?人物
    void CreateMilitaryCharacters(};

    // ?�建經�?人物
    void CreateBusinessCharacters(};

    // ?�建學�?人物
    void CreateScholarCharacters(};

    // ?�建?�命人物
    void CreateRevolutionaryCharacters(};

    // ?�建外交人物
    void CreateDiplomaticCharacters(};

    // ?�建?��?人物
    void CreateCulturalCharacters(};

    // ?��?人物互�X�輯
    void HandleCharacterInteraction(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result};

    // ?��?對話?�輯
    void HandleDialogueLogic(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, FDialogueEvent& Event};

    // 計�?互�X�數
    float CalculateInteractionScore(const FString& CharacterID, const FString& InteractionType) const;

    // ?�新人物?��?
    void UpdateCharacterRelationships(const FString& CharacterID, const FString& TargetCharacterID, const FString& InteractionType};

    // ?��?人物?�?��?�?    int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType) const;

    // 檢查互�?條件
    bool CheckInteractionConditions(const FString& CharacterID, const FString& InteractionType) const;

    // ?�用互�X��?
    void ApplyInteractionEffects(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result};

    // ?��?人物影響X    float CalculateCharacterInfluence(const FMingHistoricalCharacter& Character) const;

    // ?��?人物?��?
    float CalculateCharacterReputation(const FMingHistoricalCharacter& Character) const;

    // ?�新人物?�X    void UpdateCharacterState(const FString& CharacterID, ECharacterState NewState};

    // 記�?對話事件
    void RecordDialogueEvent(const FDialogueEvent& Event};

    // ?��?人物?�薦?�數
    float GetCharacterRecommendationScore(const FMingHistoricalCharacter& Character, const FString& PlayerFaction) const;

    // 驗�?人物?�置
    bool ValidateCharacterConfig(const FMingHistoricalCharacter& Character) const;

    // 記�?人物?��?
    void LogCharacterEvent(const FString& CharacterID, const FString& Message};

    // ?��?人物?��?
    FString GenerateCharacterReport(const FString& CharacterID) const;

    // ?��?人物?�述
    FString GetCharacterDescription(const FString& CharacterID) const;

    // ?��?人物?�?��?�?    FString GetSkillDescription(ECharacterSkillType SkillType) const;

    // ?��X��X�述
    FString GetRelationshipDescription(ECharacterRelationshipType RelationshipType) const;
};

