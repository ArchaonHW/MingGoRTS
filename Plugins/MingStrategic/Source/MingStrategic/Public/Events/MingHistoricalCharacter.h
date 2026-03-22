#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalCharacter.generated.h"

/**
 * �H��??�O
 */
UENUM(BlueprintType)
enum class ECharacterGender: uint8 {
    Male                UMETA(DisplayName = "Male"),
    Female              UMETA(DisplayName = "Female"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * �H��??�~/����
 */
UENUM(BlueprintType)
enum class ECharacterProfession: uint8 {
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
 * �H��X/���t
 */
UENUM(BlueprintType)
enum class ECharacterFaction: uint8 {
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
 * �H��??X */
UENUM(BlueprintType)
enum class ECharacterState: uint8 {
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
 * �H��摧毀��??
 */
UENUM(BlueprintType)
enum class ECharacterRelationshipType: uint8 {
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
 * �H��摧毀?X */
UENUM(BlueprintType)
enum class ECharacterSkillType: uint8 {
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
 * �H����?? */
USTRUCT(BlueprintType)
struct FHistoricalCharacterAttributes
{
    GENERATED_BODY()

    // ???X(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Leadership;

    // �x?X?�� (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Military;

    // �~��摧毀 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Diplomacy;

    // �g?X?�� (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Economics;

    // 摧毀��� (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Intelligence;

    // �y?X(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Charisma;

    // ��F摧毀 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Administration;

    // ??��??�� (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Strategy;

    // �t?X??? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Oratory;

    // �g?X??? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Writing;

    // ���摧毀 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Science;

    // �u?X???(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Engineering;

    // ??��摧毀 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Medicine;

    // ???X?�� (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Art;

    // �g?X(0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    int32 Experience;

    // ��?? (1-10)
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
 * �H��??X */
USTRUCT(BlueprintType)
struct FHistoricalCharacterSkill
{
    GENERATED_BODY()

    // 摧毀?X
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    ECharacterSkillType SkillType;

    // 摧毀摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillName;

    // 摧毀摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillDescription;

    // 摧毀摧毀(0-10)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillLevel;

    // 摧毀摧毀(0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillExperience;

    // ??�_�w��X
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    bool bIsUnlocked;

    // 摧毀�N???X(??
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float CooldownTime;

    // �W���ϥ�摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float LastUsedTime;

    // 摧毀?X
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
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
 * �H��摧毀
 */
USTRUCT(BlueprintType)
struct FCharacterRelationship
{
    GENERATED_BODY()

    // 摧毀��??
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    ECharacterRelationshipType RelationshipType;

    // 摧毀��HID
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString TargetCharacterID;

    // 摧毀�j�� (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float RelationshipStrength;

    // ???X?�z
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString RelationshipDescription;

    // ��?X???
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float EstablishedTime;

    // ??�_摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    bool bIsPublic;

    // ��??����
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    int32 InteractionCount;

    // ??��?X??X
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
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
 * ���摧毀
 */
USTRUCT(BlueprintType)
struct FDialogueOption
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionID;

    // ???X?��
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionText;

    // ???X?�z
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionDescription;

    // 摧毀����
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Conditions;

    // 摧毀��??
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Consequences;

    // 摧毀�v??
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    float Weight;

    // ??�_??��
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
 * ��ܨƥ�
 */
USTRUCT(BlueprintType)
struct FDialogueEvent
{
    GENERATED_BODY()

    // �ƥ�ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString EventID;

    // ���??��
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueText;

    // ����??ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString SpeakerID;

    // ??��ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString AudienceID;

    // ���摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    TArray<FDialogueOption> DialogueOptions;

    // ���摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    float DialogueTime;

    // ���摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueLocation;

    // ��ܥD??
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueTopic;

    // ���摧毀
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueMood;

    // ���???X
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
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
 * ��??���v�H��
 */
USTRUCT(BlueprintType)
struct FMingStrategicHistoricalCharacter
{
    GENERATED_BODY()

    // �H��ID
    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    // �H���m??
    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    // �H���r??
    UPROPERTY(BlueprintReadOnly)
    FString CourtesyName;

    // �H��???X
    UPROPERTY(BlueprintReadOnly)
    FString EnglishName;

    // �H��??�z
    UPROPERTY(BlueprintReadOnly)
    FString CharacterDescription;

    // �H��摧毀
    UPROPERTY(BlueprintReadOnly)
    FString PortraitPath;

    // ??�O
    UPROPERTY(BlueprintReadOnly)
    ECharacterGender Gender;

    // ??�~/����
    UPROPERTY(BlueprintReadOnly)
    ECharacterProfession Profession;

    // X/���t
    UPROPERTY(BlueprintReadOnly)
    ECharacterFaction Faction;

    // 摧毀�~��
    UPROPERTY(BlueprintReadOnly)
    int32 BirthYear;

    // ���`�~��
    UPROPERTY(BlueprintReadOnly)
    int32 DeathYear;

    // �H��??X
    UPROPERTY(BlueprintReadOnly)
    ECharacterState State;

    // �H����??
    UPROPERTY(BlueprintReadOnly)
    FHistoricalCharacterAttributes Attributes;

    // �H��??X
    UPROPERTY(BlueprintReadOnly)
    TArray<FHistoricalCharacterSkill> Skills;

    // �H��摧毀
    UPROPERTY(BlueprintReadOnly)
    TArray<FCharacterRelationship> Relationships;

    // �H������
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CharacterTags;

    // ���v��??
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> HistoricalEvents;

    // ???X?�N
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Achievements;

    // �v�T摧毀(0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Influence;

    // ???X(0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Reputation;

    // ��摧毀(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Loyalty;

    // ??�d摧毀(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Health;

    // ��?X?X(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Morale;

    // 摧毀��m
    UPROPERTY(BlueprintReadOnly)
    FString CurrentLocation;

    // 目標數量
    UPROPERTY(BlueprintReadOnly)
    FString CurrentPosition;

    // ??�_???X?�HX
    UPROPERTY(BlueprintReadOnly)
    bool bIsKeyCharacter;

    // ??�_???X
    UPROPERTY(BlueprintReadOnly)
    bool bIsRecruitable;

    // ???X?��
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
 * �H����??��??
 */
USTRUCT(BlueprintType)
struct FCharacterInteractionResult
{
    GENERATED_BODY()

    // ??�_摧毀
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // ��??��??
    UPROPERTY(BlueprintReadOnly)
    FString InteractionType;

    // ��??��HID
    UPROPERTY(BlueprintReadOnly)
    FString TargetCharacterID;

    // ��?X???
    UPROPERTY(BlueprintReadOnly)
    float InteractionTime;

    // ��?X???
    UPROPERTY(BlueprintReadOnly)
    FString InteractionLocation;

    // ��??��?X?�z
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // �v�T??�H??��X
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> AffectedAttributes;

    // �v�T??�H摧毀??
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRelationships;

    // 目標數量??
    UPROPERTY(BlueprintReadOnly)
    int32 GainedExperience;

    // ��?X?��
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
 * ���v�H����?X * ��?X摧毀X?��??�H摧毀��?X摧毀? */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalCharacterManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalCharacterManager();

    /**
     * ???X?�H??��??��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Initialize();

    /**
     * 摧毀�H����?X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Shutdown();

    /**
     * ��??���v�H��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character);

    /**
     * 摧毀��??���v�H��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UnregisterHistoricalCharacter(const FString& CharacterID);

    /**
     * 摧毀���v�H��
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID) const;

    /**
     * 目標數量��??�HX     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters() const;

    /**
     * 故事重要性?�HX     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction) const;

    /**
     * 故事重要性?�~??�HX     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession) const;

    /**
     * ???X?��H��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetKeyCharacters() const;

    /**
     * 故事重要性?�HX     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecruitableCharacters() const;

    /**
     * 摧毀��??�H��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetActiveCharacters() const;

    /**
     * �H����??
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails);

    /**
     * ��ܨt��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic);

    /**
     * 摧毀���摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID);

    /**
     * 摧毀�H��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RecruitCharacter(const FString& CharacterID);

    /**
     * 摧毀�H��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool DismissCharacter(const FString& CharacterID);

    /**
     * 摧毀�H��??X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 摧毀�H��摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID) const;

    /**
     * �K??�H��摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength);

    /**
     * ??�s�H��摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength);

    /**
     * 摧毀�H����??
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TMap<FString, int32> GetCharacterStatistics() const;

    /**
     * 摧毀�H���v�T???X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking() const;

    /**
     * ???X?�ˤH��
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction) const;

protected:
    // �w��摧毀���v�H��
    UPROPERTY()
    TArray<FMingHistoricalCharacter> RegisteredCharacters;

    // �H��ID??�H目標數量
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> CharacterMap;

    // �H��摧毀��� - �`??�GTMap<TArray> ����XUPROPERTY
    TMap<FString, TArray<FCharacterRelationship>> RelationshipNetwork;

    // ��ܨƥ�O??
    UPROPERTY()
    TArray<FDialogueEvent> DialogueHistory;

    // �H����??
    UPROPERTY()
    TMap<FString, int32> CharacterStatistics;

    // ??�_�w??�l??
    bool bIsInitialized;

    // 故事重要性?��??�H??�w
    void InitializeHistoricalCharacterLibrary();

    // ??��??�v�H��
    void CreatePoliticalCharacters();

    // ??�حx??�H��
    void CreateMilitaryCharacters();

    // ??�ظg??�H��
    void CreateBusinessCharacters();

    // ??�ؾ�??�H��
    void CreateScholarCharacters();

    // ??��??�R�H��
    void CreateRevolutionaryCharacters();

    // ??�إ~��H��
    void CreateDiplomaticCharacters();

    // ??��摧毀�H��
    void CreateCulturalCharacters();

    // 摧毀�H����?X?��
    void HandleCharacterInteraction(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result);

    // 摧毀���??��
    void HandleDialogueLogic(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, FDialogueEvent& Event);

    // �p??��?X?��
    float CalculateInteractionScore(const FString& CharacterID, const FString& InteractionType) const;

    // ??�s�H��摧毀
    void UpdateCharacterRelationships(const FString& CharacterID, const FString& TargetCharacterID, const FString& InteractionType);

    // 摧毀�H��摧毀摧毀
    int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType) const;

    // �ˬd��??����
    bool CheckInteractionConditions(const FString& CharacterID, const FString& InteractionType) const;

    // ??�Τ�?X???
    void ApplyInteractionEffects(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result);

    // 摧毀�H���v�TX
    float CalculateCharacterInfluence(const FMingHistoricalCharacter& Character) const;

    // 摧毀�H��摧毀
    float CalculateCharacterReputation(const FMingHistoricalCharacter& Character) const;

    // ??�s�H��??X
    void UpdateCharacterState(const FString& CharacterID, ECharacterState NewState);

    // �O??��ܨƥ�
    void RecordDialogueEvent(const FDialogueEvent& Event);

    // 摧毀�H��??��??��
    float GetCharacterRecommendationScore(const FMingHistoricalCharacter& Character, const FString& PlayerFaction) const;

    // ��??�H��??�m
    bool ValidateCharacterConfig(const FMingHistoricalCharacter& Character) const;

    // �O??�H��摧毀
    void LogCharacterEvent(const FString& CharacterID, const FString& Message);

    // 摧毀�H��摧毀
    FString GenerateCharacterReport(const FString& CharacterID) const;

    // 摧毀�H��??�z
    FString GetCharacterDescription(const FString& CharacterID) const;

    // 摧毀�H��摧毀摧毀
    FString GetSkillDescription(ECharacterSkillType SkillType) const;

    // 故事重要性?�z
    FString GetRelationshipDescription(ECharacterRelationshipType RelationshipType) const;
};

