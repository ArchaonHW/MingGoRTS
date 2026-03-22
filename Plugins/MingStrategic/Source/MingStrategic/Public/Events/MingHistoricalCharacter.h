#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalCharacter.generated.h"

/**
 * 人物性別
 */
UENUM(BlueprintType)
enum class ECharacterGender : uint8
{
    Male                UMETA(DisplayName = "Male"),
    Female              UMETA(DisplayName = "Female"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * 人物職業/身份
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
 * 人物陣營/派系
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
 * 人物狀態
 */
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
 * 人物關係類型
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
 * 人物技能類型
 */
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
 * 人物屬性
 */
USTRUCT(BlueprintType)
struct FHistoricalCharacterAttributes
{
    GENERATED_BODY()

    // 領導力 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Leadership;

    // 軍事才能 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Military;

    // 外交能力 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Diplomacy;

    // 經濟才能 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Economics;

    // 智力水平 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Intelligence;

    // 魅力值 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Charisma;

    // 行政能力 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Administration;

    // 戰略思維 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Strategy;

    // 演講能力 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Oratory;

    // 寫作能力 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Writing;

    // 科學知識 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Science;

    // 工程技術 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Engineering;

    // 醫學知識 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Medicine;

    // 藝術才能 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    float Art;

    // 經驗值 (0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Attributes")
    int32 Experience;

    // 等級 (1-10)
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
 * 人物技能
 */
USTRUCT(BlueprintType)
struct FHistoricalCharacterSkill
{
    GENERATED_BODY()

    // 技能類型
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    ECharacterSkillType SkillType;

    // 技能名稱
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillName;

    // 技能描述
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    FString SkillDescription;

    // 技能等級 (0-10)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillLevel;

    // 技能經驗 (0-1000)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    int32 SkillExperience;

    // 是否已解鎖
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    bool bIsUnlocked;

    // 技能冷卻時間 (秒)
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float CooldownTime;

    // 上次使用時間
    UPROPERTY(BlueprintReadWrite, Category = "Character Skill")
    float LastUsedTime;

    // 技能效果
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
 * 人物關係
 */
USTRUCT(BlueprintType)
struct FCharacterRelationship
{
    GENERATED_BODY()

    // 關係類型
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    ECharacterRelationshipType RelationshipType;

    // 關係對象ID
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString TargetCharacterID;

    // 關係強度 (0-100)
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float RelationshipStrength;

    // 關係描述
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    FString RelationshipDescription;

    // 建立時間
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    float EstablishedTime;

    // 是否公開
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    bool bIsPublic;

    // 互動次數
    UPROPERTY(BlueprintReadWrite, Category = "Character Relationship")
    int32 InteractionCount;

    // 最後互動時間
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
 * 對話選項
 */
USTRUCT(BlueprintType)
struct FDialogueOption
{
    GENERATED_BODY()

    // 選項ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionID;

    // 選項文本
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionText;

    // 選項描述
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    FString OptionDescription;

    // 選項條件
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Conditions;

    // 選項後果
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    TArray<FString> Consequences;

    // 選項權重
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Option")
    float Weight;

    // 是否可用
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

    // 對話文本
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueText;

    // 說話者ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString SpeakerID;

    // 聽眾ID
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString AudienceID;

    // 對話選項
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    TArray<FDialogueOption> DialogueOptions;

    // 對話時間
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    float DialogueTime;

    // 對話地點
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueLocation;

    // 對話主題
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueTopic;

    // 對話情緒
    UPROPERTY(BlueprintReadWrite, Category = "Dialogue Event")
    FString DialogueMood;

    // 對話重要性
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
 * 民國歷史人物
 */
USTRUCT(BlueprintType)
struct FMingStrategicHistoricalCharacter
{
    GENERATED_BODY()

    // 人物ID
    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    // 人物姓名
    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    // 人物字號
    UPROPERTY(BlueprintReadOnly)
    FString CourtesyName;

    // 人物英文名
    UPROPERTY(BlueprintReadOnly)
    FString EnglishName;

    // 人物描述
    UPROPERTY(BlueprintReadOnly)
    FString CharacterDescription;

    // 人物肖像
    UPROPERTY(BlueprintReadOnly)
    FString PortraitPath;

    // 性別
    UPROPERTY(BlueprintReadOnly)
    ECharacterGender Gender;

    // 職業/身份
    UPROPERTY(BlueprintReadOnly)
    ECharacterProfession Profession;

    // 陣營/派系
    UPROPERTY(BlueprintReadOnly)
    ECharacterFaction Faction;

    // 出生年份
    UPROPERTY(BlueprintReadOnly)
    int32 BirthYear;

    // 死亡年份
    UPROPERTY(BlueprintReadOnly)
    int32 DeathYear;

    // 人物狀態
    UPROPERTY(BlueprintReadOnly)
    ECharacterState State;

    // 人物屬性
    UPROPERTY(BlueprintReadOnly)
    FHistoricalCharacterAttributes Attributes;

    // 人物技能
    UPROPERTY(BlueprintReadOnly)
    TArray<FHistoricalCharacterSkill> Skills;

    // 人物關係
    UPROPERTY(BlueprintReadOnly)
    TArray<FCharacterRelationship> Relationships;

    // 人物標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CharacterTags;

    // 歷史事蹟
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> HistoricalEvents;

    // 重要成就
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Achievements;

    // 影響力值 (0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Influence;

    // 聲望值 (0-1000)
    UPROPERTY(BlueprintReadOnly)
    float Reputation;

    // 忠誠度 (0-100)
    UPROPERTY(BlueprintReadOnly)
    float Loyalty;

    // 健康狀況 (0-100)
    UPROPERTY(BlueprintReadOnly)
    float Health;

    // 精神狀態 (0-100)
    UPROPERTY(BlueprintReadOnly)
    float Morale;

    // 當前位置
    UPROPERTY(BlueprintReadOnly)
    FString CurrentLocation;

    // 當前職位
    UPROPERTY(BlueprintReadOnly)
    FString CurrentPosition;

    // 是否為關鍵人物
    UPROPERTY(BlueprintReadOnly)
    bool bIsKeyCharacter;

    // 是否可招募
    UPROPERTY(BlueprintReadOnly)
    bool bIsRecruitable;

    // 招募成本
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
 * 人物互動結果
 */
USTRUCT(BlueprintType)
struct FCharacterInteractionResult
{
    GENERATED_BODY()

    // 是否成功
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 互動類型
    UPROPERTY(BlueprintReadOnly)
    FString InteractionType;

    // 互動對象ID
    UPROPERTY(BlueprintReadOnly)
    FString TargetCharacterID;

    // 互動時間
    UPROPERTY(BlueprintReadOnly)
    float InteractionTime;

    // 互動地點
    UPROPERTY(BlueprintReadOnly)
    FString InteractionLocation;

    // 互動結果描述
    UPROPERTY(BlueprintReadOnly)
    FString ResultDescription;

    // 影響的人物屬性
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> AffectedAttributes;

    // 影響的人物關係
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedRelationships;

    // 獲得的經驗
    UPROPERTY(BlueprintReadOnly)
    int32 GainedExperience;

    // 互動分數
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
 * 歷史人物管理器
 * 管理所有民國歷史人物的互動和對話
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalCharacterManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalCharacterManager();

    /**
     * 初始化人物管理器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Initialize();

    /**
     * 關閉人物管理器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    void Shutdown();

    /**
     * 註冊歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RegisterHistoricalCharacter(const FMingHistoricalCharacter& Character);

    /**
     * 取消註冊歷史人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UnregisterHistoricalCharacter(const FString& CharacterID);

    /**
     * 獲取歷史人物
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    FMingHistoricalCharacter GetHistoricalCharacter(const FString& CharacterID) const;

    /**
     * 獲取所有歷史人物
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetAllHistoricalCharacters() const;

    /**
     * 獲取指定陣營的人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByFaction(ECharacterFaction Faction) const;

    /**
     * 獲取指定職業的人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharactersByProfession(ECharacterProfession Profession) const;

    /**
     * 獲取關鍵人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetKeyCharacters() const;

    /**
     * 獲取可招募人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecruitableCharacters() const;

    /**
     * 獲取活躍人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetActiveCharacters() const;

    /**
     * 人物互動
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FCharacterInteractionResult InteractWithCharacter(const FString& CharacterID, const FString& InteractionType, const FString& InteractionDetails);

    /**
     * 對話系統
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent StartDialogue(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic);

    /**
     * 處理對話選項
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    FDialogueEvent ProcessDialogueOption(const FString& DialogueEventID, const FString& OptionID);

    /**
     * 招募人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool RecruitCharacter(const FString& CharacterID);

    /**
     * 解僱人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool DismissCharacter(const FString& CharacterID);

    /**
     * 提升人物技能
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpgradeCharacterSkill(const FString& CharacterID, ECharacterSkillType SkillType);

    /**
     * 獲取人物關係
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FCharacterRelationship> GetCharacterRelationships(const FString& CharacterID) const;

    /**
     * 添加人物關係
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool AddCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, ECharacterRelationshipType RelationshipType, float Strength);

    /**
     * 更新人物關係
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    bool UpdateCharacterRelationship(const FString& CharacterID, const FString& TargetCharacterID, float NewStrength);

    /**
     * 獲取人物統計
     */
    UFUNCTION(BlueprintPure, Category = "Historical Character")
    TMap<FString, int32> GetCharacterStatistics() const;

    /**
     * 獲取人物影響力排名
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetCharacterInfluenceRanking() const;

    /**
     * 獲取推薦人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Character")
    TArray<FMingHistoricalCharacter> GetRecommendedCharacters(const FString& PlayerFaction) const;

protected:
    // 已註冊的歷史人物
    UPROPERTY()
    TArray<FMingHistoricalCharacter> RegisteredCharacters;

    // 人物ID到人物的映射
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> CharacterMap;

    // 人物關係網絡 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<FCharacterRelationship>> RelationshipNetwork;

    // 對話事件記錄
    UPROPERTY()
    TArray<FDialogueEvent> DialogueHistory;

    // 人物統計
    UPROPERTY()
    TMap<FString, int32> CharacterStatistics;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化民國歷史人物庫
    void InitializeHistoricalCharacterLibrary();

    // 創建政治人物
    void CreatePoliticalCharacters();

    // 創建軍事人物
    void CreateMilitaryCharacters();

    // 創建經濟人物
    void CreateBusinessCharacters();

    // 創建學術人物
    void CreateScholarCharacters();

    // 創建革命人物
    void CreateRevolutionaryCharacters();

    // 創建外交人物
    void CreateDiplomaticCharacters();

    // 創建文化人物
    void CreateCulturalCharacters();

    // 處理人物互動邏輯
    void HandleCharacterInteraction(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result);

    // 處理對話邏輯
    void HandleDialogueLogic(const FString& SpeakerID, const FString& AudienceID, const FString& DialogueTopic, FDialogueEvent& Event);

    // 計算互動分數
    float CalculateInteractionScore(const FString& CharacterID, const FString& InteractionType) const;

    // 更新人物關係
    void UpdateCharacterRelationships(const FString& CharacterID, const FString& TargetCharacterID, const FString& InteractionType);

    // 獲取人物技能等級
    int32 GetCharacterSkillLevel(const FString& CharacterID, ECharacterSkillType SkillType) const;

    // 檢查互動條件
    bool CheckInteractionConditions(const FString& CharacterID, const FString& InteractionType) const;

    // 應用互動效果
    void ApplyInteractionEffects(const FString& CharacterID, const FString& InteractionType, FCharacterInteractionResult& Result);

    // 獲取人物影響力
    float CalculateCharacterInfluence(const FMingHistoricalCharacter& Character) const;

    // 獲取人物聲望
    float CalculateCharacterReputation(const FMingHistoricalCharacter& Character) const;

    // 更新人物狀態
    void UpdateCharacterState(const FString& CharacterID, ECharacterState NewState);

    // 記錄對話事件
    void RecordDialogueEvent(const FDialogueEvent& Event);

    // 獲取人物推薦分數
    float GetCharacterRecommendationScore(const FMingHistoricalCharacter& Character, const FString& PlayerFaction) const;

    // 驗證人物配置
    bool ValidateCharacterConfig(const FMingHistoricalCharacter& Character) const;

    // 記錄人物日誌
    void LogCharacterEvent(const FString& CharacterID, const FString& Message);

    // 生成人物報告
    FString GenerateCharacterReport(const FString& CharacterID) const;

    // 獲取人物描述
    FString GetCharacterDescription(const FString& CharacterID) const;

    // 獲取人物技能描述
    FString GetSkillDescription(ECharacterSkillType SkillType) const;

    // 獲取關係描述
    FString GetRelationshipDescription(ECharacterRelationshipType RelationshipType) const;
};
