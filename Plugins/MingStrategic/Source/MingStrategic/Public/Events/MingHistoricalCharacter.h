#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalCharacter.generated.h"

/**
 * ‰∫∫Áâ©?ßÂà•
 */
UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
    Male                UMETA(DisplayName = "Male"),
    Female              UMETA(DisplayName = "Female"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * ‰∫∫Áâ©?∑Ê•≠/Ë∫´‰ªΩ
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
 * ‰∫∫Áâ©???/Ê¥æÁ≥ª
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
 * ‰∫∫Áâ©?Ä?? */
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
 * ‰∫∫Áâ©?ú‰?È°ûÂ?
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
 * ‰∫∫Áâ©?Ä?ΩÈ??? */
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
 * ‰∫∫Áâ©Â±¨ÊÄ? */
USTRUCT(BlueprintType)
struct FHistoricalCharacterAttributes
{
    GENERATED_BODY()

    // ?òÂ???(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Leadership;

    // Ëªç‰??çËÉΩ (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Military;

    // Â§ñ‰∫§?ΩÂ? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Diplomacy;

    // Á∂ìÊ??çËÉΩ (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Economics;

    // ?∫Â?Ê∞¥Âπ≥ (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Intelligence;

    // È≠ÖÂ???(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Charisma;

    // Ë°åÊîø?ΩÂ? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Administration;

    // ?∞Áï•?ùÁ∂≠ (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Strategy;

    // ÊºîË??ΩÂ? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Oratory;

    // ÂØ´‰??ΩÂ? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Writing;

    // ÁßëÂ≠∏?•Ë? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Science;

    // Â∑•Á??ÄË°?(0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Engineering;

    // ?´Â≠∏?•Ë? (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Medicine;

    // ?ùË??çËÉΩ (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Art;

    // Á∂ìÈ???(0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    int32 Experience;

    // Á≠âÁ? (1-10)
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
 * ‰∫∫Áâ©?Ä?? */
USTRUCT(BlueprintType)
struct FHistoricalCharacterSkill
{
    GENERATED_BODY()

    // ?Ä?ΩÈ???    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    ECharacterSkillType SkillType;

    // ?Ä?ΩÂ?Á®?    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillName;

    // ?Ä?ΩÊ?Ëø?    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillDescription;

    // ?Ä?ΩÁ?Á¥?(0-10)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillLevel;

    // ?Ä?ΩÁ?È©?(0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillExperience;

    // ?ØÂê¶Â∑≤Ëß£??    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    bool bIsUnlocked;

    // ?Ä?ΩÂÜ∑?ªÊ???(Áß?
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float CooldownTime;

    // ‰∏äÊ¨°‰ΩøÁî®?ÇÈ?
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float LastUsedTime;

    // ?Ä?ΩÊ???    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
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
 * ‰∫∫Áâ©?ú‰?
 */
USTRUCT(BlueprintType)
struct FCharacterRelationship
{
    GENERATED_BODY()

    // ?ú‰?È°ûÂ?
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    ECharacterRelationshipType RelationshipType;

    // ?ú‰?Â∞çË±°ID
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString TargetCharacterID;

    // ?ú‰?Âº∑Â∫¶ (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float RelationshipStrength;

    // ?ú‰??èËø∞
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString RelationshipDescription;

    // Âª∫Á??ÇÈ?
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float EstablishedTime;

    // ?ØÂê¶?¨È?
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    bool bIsPublic;

    // ‰∫íÂ?Ê¨°Êï∏
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    int32 InteractionCount;

    // ?ÄÂæå‰??ïÊ???    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
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
 * Â∞çË©±?∏È?
 */
USTRUCT(BlueprintType)
struct FDialogueOption
{
    GENERATED_BODY()

    // ?∏È?ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionID;

    // ?∏È??áÊú¨
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionText;

    // ?∏È??èËø∞
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionDescription;

    // ?∏È?Ê¢ù‰ª∂
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Conditions;

    // ?∏È?ÂæåÊ?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Consequences;

    // ?∏È?Ê¨äÈ?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    float Weight;

    // ?ØÂê¶?ØÁî®
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
 * Â∞çË©±‰∫ã‰ª∂
 */
USTRUCT(BlueprintType)
struct FDialogueEvent
{
    GENERATED_BODY()

    // ‰∫ã‰ª∂ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString EventID;

    // Â∞çË©±?áÊú¨
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueText;

    // Ë™™Ë©±?ÖID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString SpeakerID;

    // ?ΩÁúæID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString AudienceID;

    // Â∞çË©±?∏È?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    TArray<FDialogueOption> DialogueOptions;

    // Â∞çË©±?ÇÈ?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    float DialogueTime;

    // Â∞çË©±?∞È?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueLocation;

    // Â∞çË©±‰∏ªÈ?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueTopic;

    // Â∞çË©±?ÖÁ?
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueMood;

    // Â∞çË©±?çË???    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
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
 * Ê∞ëÂ?Ê≠∑Âè≤‰∫∫Áâ©
 */
USTRUCT(BlueprintType)
struct FMingStrategicHistoricalCharacter
{
    GENERATED_BODY()

    // ‰∫∫Áâ©ID
    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    // ‰∫∫Áâ©ÂßìÂ?
    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    // ‰∫∫Áâ©Â≠óË?
    UPROPERTY(BlueprintReadOnly)
    FString CourtesyName;

    // ‰∫∫Áâ©?±Ê???    UPROPERTY(BlueprintReadOnly)
    FString EnglishName;

    // ‰∫∫Áâ©?èËø∞
    UPROPERTY(BlueprintReadOnly)
    FString CharacterDescription;

    // ‰∫∫Áâ©?ñÂ?
    UPROPERTY(BlueprintReadOnly)
    FString PortraitPath;

    // ?ßÂà•
    UPROPERTY(BlueprintReadOnly)
    ECharacterGender Gender;

    // ?∑Ê•≠/Ë∫´‰ªΩ
    UPROPERTY(BlueprintReadOnly)
    ECharacterProfession Profession;

    // ???/Ê¥æÁ≥ª
    UPROPERTY(BlueprintReadOnly)
    ECharacterFaction Faction;

    // ?∫Á?Âπ¥‰ªΩ
    UPROPERTY(BlueprintReadOnly)
    int32 BirthYear;

    // Ê≠ª‰∫°Âπ¥‰ªΩ
    UPROPERTY(BlueprintReadOnly)
    int32 DeathYear;

    // ‰∫∫Áâ©?Ä??    UPROPERTY(BlueprintReadOnly)
    ECharacterState State;

    // ‰∫∫Áâ©Â±¨ÊÄ?    UPROPERTY(BlueprintReadOnly)
    FHistoricalCharacterAttributes Attributes;

    // ‰∫∫Áâ©?Ä??    UPROPERTY(BlueprintReadOnly)
    TArray<FHistoricalCharacterSkill> Skills;

    // ‰∫∫Áâ©?ú‰?
    UPROPERTY(BlueprintReadOnly)
    TArray<FCharacterRelationship> Relationships;

    // ‰∫∫Áâ©Ê®ôÁ±§
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CharacterTags;

    // Ê≠∑Âè≤‰∫ãË?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> HistoricalEvents;

    // ?çË??êÂ∞±
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Achievements;

    // ÂΩ±Èüø?õÂÄ?(0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Influence;

    // ?≤Ê???(0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Reputation;

    // Âø†Ë?Â∫?(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Loyalty;

    // ?•Â∫∑?ÄÊ≥?(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Health;

    // Á≤æÁ??Ä??(0-100)
    UPROPERTY(BlueprintReadOnly)
    float Morale;

    // ?∂Â?‰ΩçÁΩÆ
    UPROPERTY(BlueprintReadOnly)
    FString CurrentLocation;

    // ?∂Â??∑‰?
    UPROPERTY(BlueprintReadOnly)
    FString CurrentPosition;

    // ?ØÂê¶?∫È??µ‰∫∫??    UPROPERTY(BlueprintReadOnly)
    bool bIsKeyCharacter;

    // ?ØÂê¶?ØÊ???    UPROPERTY(BlueprintReadOnly)
    bool bIsRecruitable;

    // ?õÂ??êÊú¨
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
 * ‰∫∫Áâ©‰∫íÂ?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FCharacterInteractionResult
{
    GENERATED_BODY()

    // ?ØÂê¶?êÂ?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // ‰∫íÂ?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    FString InteractionType;

    // ‰∫íÂ?Â∞çË±°ID
    UPROPERTY(BlueprintReadOnly)
    FString TargetCharacterID;

    // ‰∫íÂ??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float InteractionTime;

    // ‰∫íÂ??∞È?
    UPROPERTY(BlueprintReadOnly)
    FString InteractionLocation;

    // ‰∫íÂ?ÁµêÊ??èËø∞
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // ÂΩ±Èüø?Ñ‰∫∫?©Â±¨??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> AffectedAttributes;

    // ÂΩ±Èüø?Ñ‰∫∫?©È?‰ø?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRelationships;

    // ?≤Â??ÑÁ?È©?    UPROPERTY(BlueprintReadOnly)
    int32 GainedExperience;

    // ‰∫íÂ??ÜÊï∏
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
 * Ê≠∑Âè≤‰∫∫Áâ©ÁÆ°Á??? * ÁÆ°Á??Ä?âÊ??ãÊ≠∑?≤‰∫∫?©Á?‰∫íÂ??åÂ?Ë©? */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalCharacterManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalCharacterManager();

    /**
     * ?ùÂ??ñ‰∫∫?©ÁÆ°?ÜÂô®
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Initialize();

    /**
     * ?úÈ?‰∫∫Áâ©ÁÆ°Á???     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Shutdown();

    /**
     * Ë®ªÂ?Ê≠∑Âè≤‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character);

    /**
     * ?ñÊ?Ë®ªÂ?Ê≠∑Âè≤‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UnregisterHistoricalCharacter(const FString& CharacterID);

    /**
     * ?≤Â?Ê≠∑Âè≤‰∫∫Áâ©
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID) const;

    /**
     * ?≤Â??Ä?âÊ≠∑?≤‰∫∫??     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters() const;

    /**
     * ?≤Â??áÂ?????Ñ‰∫∫??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction) const;

    /**
     * ?≤Â??áÂ??∑Ê•≠?Ñ‰∫∫??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession) const;

    /**
     * ?≤Â??úÈçµ‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetKeyCharacters() const;

    /**
     * ?≤Â??ØÊ??ü‰∫∫??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecruitableCharacters() const;

    /**
     * ?≤Â?Ê¥ªË?‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetActiveCharacters() const;

    /**
     * ‰∫∫Áâ©‰∫íÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails);

    /**
     * Â∞çË©±Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic);

    /**
     * ?ïÁ?Â∞çË©±?∏È?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID);

    /**
     * ?õÂ?‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RecruitCharacter(const FString& CharacterID);

    /**
     * Ëß?É±‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool DismissCharacter(const FString& CharacterID);

    /**
     * ?êÂ?‰∫∫Áâ©?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * ?≤Â?‰∫∫Áâ©?ú‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID) const;

    /**
     * Ê∑ªÂ?‰∫∫Áâ©?ú‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength);

    /**
     * ?¥Êñ∞‰∫∫Áâ©?ú‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength);

    /**
     * ?≤Â?‰∫∫Áâ©Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TMap<FString, int32> GetCharacterStatistics() const;

    /**
     * ?≤Â?‰∫∫Áâ©ÂΩ±Èüø?õÊ???     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking() const;

    /**
     * ?≤Â??®Ëñ¶‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction) const;

protected:
    // Â∑≤Ë®ª?äÁ?Ê≠∑Âè≤‰∫∫Áâ©
    UPROPERTY()
    TArray<FMingHistoricalCharacter> RegisteredCharacters;

    // ‰∫∫Áâ©ID?∞‰∫∫?©Á??†Â?
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> CharacterMap;

    // ‰∫∫Áâ©?ú‰?Á∂≤Áµ° - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<FString, TArray<FCharacterRelationship>> RelationshipNetwork;

    // Â∞çË©±‰∫ã‰ª∂Ë®òÈ?
    UPROPERTY()
    TArray<FDialogueEvent> DialogueHistory;

    // ‰∫∫Áâ©Áµ±Ë?
    UPROPERTY()
    TMap<FString, int32> CharacterStatistics;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ??ãÊ≠∑?≤‰∫∫?©Â∫´
    void InitializeHistoricalCharacterLibrary();

    // ?µÂª∫?øÊ≤ª‰∫∫Áâ©
    void CreatePoliticalCharacters();

    // ?µÂª∫Ëªç‰?‰∫∫Áâ©
    void CreateMilitaryCharacters();

    // ?µÂª∫Á∂ìÊ?‰∫∫Áâ©
    void CreateBusinessCharacters();

    // ?µÂª∫Â≠∏Ë?‰∫∫Áâ©
    void CreateScholarCharacters();

    // ?µÂª∫?©ÂëΩ‰∫∫Áâ©
    void CreateRevolutionaryCharacters();

    // ?µÂª∫Â§ñ‰∫§‰∫∫Áâ©
    void CreateDiplomaticCharacters();

    // ?µÂª∫?áÂ?‰∫∫Áâ©
    void CreateCulturalCharacters();

    // ?ïÁ?‰∫∫Áâ©‰∫íÂ??èËºØ
    void HandleCharacterInteraction(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result);

    // ?ïÁ?Â∞çË©±?èËºØ
    void HandleDialogueLogic(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, FDialogueEvent& Event);

    // Ë®àÁ?‰∫íÂ??ÜÊï∏
    float CalculateInteractionScore(const FString& CharacterID, const FString& InteractionType) const;

    // ?¥Êñ∞‰∫∫Áâ©?ú‰?
    void UpdateCharacterRelationships(const FString& CharacterID, const FString& TargetCharacterID, const FString& InteractionType);

    // ?≤Â?‰∫∫Áâ©?Ä?ΩÁ?Á¥?    int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType) const;

    // Ê™¢Êü•‰∫íÂ?Ê¢ù‰ª∂
    bool CheckInteractionConditions(const FString& CharacterID, const FString& InteractionType) const;

    // ?âÁî®‰∫íÂ??àÊ?
    void ApplyInteractionEffects(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result);

    // ?≤Â?‰∫∫Áâ©ÂΩ±Èüø??    float CalculateCharacterInfluence(const FMingHistoricalCharacter& Character) const;

    // ?≤Â?‰∫∫Áâ©?≤Ê?
    float CalculateCharacterReputation(const FMingHistoricalCharacter& Character) const;

    // ?¥Êñ∞‰∫∫Áâ©?Ä??    void UpdateCharacterState(const FString& CharacterID, ECharacterState NewState);

    // Ë®òÈ?Â∞çË©±‰∫ã‰ª∂
    void RecordDialogueEvent(const FDialogueEvent& Event);

    // ?≤Â?‰∫∫Áâ©?®Ëñ¶?ÜÊï∏
    float GetCharacterRecommendationScore(const FMingHistoricalCharacter& Character, const FString& PlayerFaction) const;

    // È©óË?‰∫∫Áâ©?çÁΩÆ
    bool ValidateCharacterConfig(const FMingHistoricalCharacter& Character) const;

    // Ë®òÈ?‰∫∫Áâ©?•Ë?
    void LogCharacterEvent(const FString& CharacterID, const FString& Message);

    // ?üÊ?‰∫∫Áâ©?±Â?
    FString GenerateCharacterReport(const FString& CharacterID) const;

    // ?≤Â?‰∫∫Áâ©?èËø∞
    FString GetCharacterDescription(const FString& CharacterID) const;

    // ?≤Â?‰∫∫Áâ©?Ä?ΩÊ?Ëø?    FString GetSkillDescription(ECharacterSkillType SkillType) const;

    // ?≤Â??ú‰??èËø∞
    FString GetRelationshipDescription(ECharacterRelationshipType RelationshipType) const;
};
