#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSDialogueSystem.h"
#include "MingGoRTSHistoricalCharacters.generated.h"

UENUM(BlueprintType)
enum class EHistoricalFaction : uint8
{
    Nationalist     UMETA(DisplayName = "國民黨"),
    Communist       UMETA(DisplayName = "共產黨"),
    Warlord         UMETA(DisplayName = "軍閥"),
    Independent     UMETA(DisplayName = "獨立派系"),
    Foreign         UMETA(DisplayName = "外國勢力")
};

UENUM(BlueprintType)
enum class ECharacterRole : uint8
{
    Leader          UMETA(DisplayName = "領袖"),
    Military        UMETA(DisplayName = "軍事人物"),
    Political       UMETA(DisplayName = "政治人物"),
    Diplomatic      UMETA(DisplayName = "外交人物"),
    Intelligence    UMETA(DisplayName = "情報人物"),
    Economic        UMETA(DisplayName = "經濟人物"),
    Cultural        UMETA(DisplayName = "文化人物"),
    Civilian        UMETA(DisplayName = "平民")
};

USTRUCT(BlueprintType)
struct FMingGameCharacterRelationship
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Relationship")
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Relationship")
    float RelationshipValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Relationship")
    FString LastInteractionDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Relationship")
    TArray<FString> SharedHistory;

    FCharacterRelationship()
    {
        CharacterID = TEXT("");
        RelationshipValue = 50.0f; // 默認中立
        LastInteractionDate = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FCharacterPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Openness;          // 開放性

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Conscientiousness; // 責任感

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Extraversion;      // 外向性

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Agreeableness;     // 宜人性

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Neuroticism;       // 神經質

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Patriotism;        // 愛國主義

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Ambition;          // 野心

    FCharacterPersonality()
    {
        Openness = 0.5f;
        Conscientiousness = 0.5f;
        Extraversion = 0.5f;
        Agreeableness = 0.5f;
        Neuroticism = 0.5f;
        Patriotism = 0.7f;
        Ambition = 0.5f;
    }
};

USTRUCT(BlueprintType)
struct FMingGameHistoricalCharacter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString CourtesyName;    // 字

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString BirthDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString DeathDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    ECharacterRole Role;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    EHistoricalFaction Faction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FCharacterPersonality Personality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    TArray<FString> KnownLocations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    TArray<FString> KeyEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    TArray<FString> Relationships;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    TArray<FString> DialogueIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString PortraitPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    FString VoiceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    bool bIsAlive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    bool bIsAvailable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Character")
    float Influence;

    FMingHistoricalCharacter()
    {
        CharacterID = TEXT("");
        CharacterName = TEXT("");
        CourtesyName = TEXT("");
        BirthDate = TEXT("");
        DeathDate = TEXT("");
        Role = ECharacterRole::Civilian;
        Faction = EHistoricalFaction::Independent;
        Title = TEXT("");
        Description = TEXT("");
        PortraitPath = TEXT("");
        VoiceType = TEXT("");
        bIsAlive = true;
        bIsAvailable = true;
        Influence = 50.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterRelationshipChanged, const FString&, CharacterID, float, NewRelationshipValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAvailabilityChanged, const FString&, CharacterID, bool, bIsAvailable);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalCharacters : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalCharacters();

    // 角色系統事件
    UPROPERTY(BlueprintAssignable, Category = "Character System")
    FOnCharacterRelationshipChanged OnCharacterRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Character System")
    FOnCharacterAvailabilityChanged OnCharacterAvailabilityChanged;

    // 初始化角色系統
    UFUNCTION(BlueprintCallable, Category = "Character System")
    void InitializeCharacterSystem();

    // 獲取所有歷史人物
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetAllCharacters() const;

    // 獲取歷史人物
    UFUNCTION(BlueprintPure, Category = "Character System")
    FMingHistoricalCharacter GetCharacter(const FString& CharacterID) const;

    // 獲取角色關係
    UFUNCTION(BlueprintPure, Category = "Character System")
    float GetCharacterRelationship(const FString& CharacterID) const;

    // 更新角色關係
    UFUNCTION(BlueprintCallable, Category = "Character System")
    void UpdateCharacterRelationship(const FString& CharacterID, float RelationshipChange);

    // 獲取可用角色
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetAvailableCharacters() const;

    // 獲取特定派系角色
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersByFaction(EHistoricalFaction Faction) const;

    // 獲取特定角色類型
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersByRole(ECharacterRole Role) const;

    // 獲取在特定地點的角色
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersAtLocation(const FString& Location) const;

    // 獲取參與特定事件的角色
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersInEvent(const FString& EventID) const;

    // 檢查角色是否可用
    UFUNCTION(BlueprintPure, Category = "Character System")
    bool IsCharacterAvailable(const FString& CharacterID) const;

    // 設置角色可用性
    UFUNCTION(BlueprintCallable, Category = "Character System")
    void SetCharacterAvailability(const FString& CharacterID, bool bIsAvailable);

    // 獲取角色對話選項
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FString> GetCharacterDialogueOptions(const FString& CharacterID) const;

    // 生成角色回應
    UFUNCTION(BlueprintPure, Category = "Character System")
    FString GenerateCharacterResponse(const FString& CharacterID, EDialogueAttitude PlayerAttitude, const FString& Context) const;

    // 計算角色影響力
    UFUNCTION(BlueprintPure, Category = "Character System")
    float CalculateCharacterInfluence(const FString& CharacterID) const;

    // 獲取角色推薦對話
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FString> GetRecommendedDialogues(const FString& CharacterID, const FMingCharacterData& PlayerCharacter) const;

    // 保存角色數據
    UFUNCTION(BlueprintCallable, Category = "Character System")
    bool SaveCharacterData(const FString& SaveSlotName);

    // 載入角色數據
    UFUNCTION(BlueprintCallable, Category = "Character System")
    bool LoadCharacterData(const FString& SaveSlotName);

protected:
    // 所有歷史人物
    UPROPERTY()
    TArray<FMingHistoricalCharacter> AllCharacters;

    // 角色ID到角色的映射
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> CharacterMap;

    // 角色關係
    UPROPERTY()
    TMap<FString, float> CharacterRelationships;

    // 角色可用性
    UPROPERTY()
    TMap<FString, bool> CharacterAvailability;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化歷史人物庫
    void InitializeHistoricalCharacterLibrary();

    // 創建國民黨人物
    void CreateNationalistCharacters();

    // 創建共產黨人物
    void CreateCommunistCharacters();

    // 創建軍閥人物
    void CreateWarlordCharacters();

    // 創建獨立派系人物
    void CreateIndependentCharacters();

    // 創建外國人物
    void CreateForeignCharacters();

    // 創建平民人物
    void CreateCivilianCharacters();

    // 生成角色回應文本
    FString GenerateResponseText(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude, const FString& Context) const;

    // 計算關係變化
    float CalculateRelationshipChange(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // 檢查角色性格匹配
    bool CheckPersonalityMatch(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // 獲取角色回應模板
    FString GetResponseTemplate(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // 生成角色ID
    FString GenerateCharacterID(const FString& BaseName) const;

    // 更新角色影響力
    void UpdateCharacterInfluence(const FString& CharacterID);

    // 檢查歷史時間一致性
    bool CheckHistoricalConsistency(const FString& CharacterID, const FString& CurrentDate) const;
};
