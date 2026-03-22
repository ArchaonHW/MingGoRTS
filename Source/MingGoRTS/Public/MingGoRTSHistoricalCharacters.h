#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSDialogueSystem.h"
#include "MingGoRTSHistoricalCharacters.generated.h"

UENUM(BlueprintType)
enum class EHistoricalFaction : uint8
{
    Nationalist, UMETA(DisplayName = "???),
    Communist, UMETA(DisplayName = "?產?),
    Warlord, UMETA(DisplayName = "軍閥"),
    Independent, UMETA(DisplayName = "??派系"),
    Foreign, UMETA(DisplayName = "外???")
};

UENUM(BlueprintType)
enum class ECharacterRole : uint8
{
    Leader, UMETA(DisplayName = "??"),
    Military, UMETA(DisplayName = "軍?人物"),
    Political, UMETA(DisplayName = "?治人物"),
    Diplomatic, UMETA(DisplayName = "外交人物"),
    Intelligence, UMETA(DisplayName = "?報人物"),
    Economic, UMETA(DisplayName = "經?人物"),
    Cultural, UMETA(DisplayName = "??人物"),
    Civilian, UMETA(DisplayName = "平?")
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
        CharacterID = TEXT(""};
        RelationshipValue = 50.0f; // 默?中?
        LastInteractionDate = TEXT(""};
    }
};

USTRUCT(BlueprintType)
struct FCharacterPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Openness;          // ?放??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Conscientiousness; // 責任??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Extraversion;      // 外???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Agreeableness;     // 宜人??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Neuroticism;       // 神??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Patriotism;        // ??主義

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Ambition;          // ??

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
    FString CourtesyName;    // ?
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
        CharacterID = TEXT(""};
        CharacterName = TEXT(""};
        CourtesyName = TEXT(""};
        BirthDate = TEXT(""};
        DeathDate = TEXT(""};
        Role = ECharacterRole::Civilian;
        Faction = EHistoricalFaction::Independent;
        Title = TEXT(""};
        Description = TEXT(""};
        PortraitPath = TEXT(""};
        VoiceType = TEXT(""};
        bIsAlive = true;
        bIsAvailable = true;
        Influence = 50.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterRelationshipChanged, const FString&, CharacterID, float, NewRelationshipValue};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterAvailabilityChanged, const FString&, CharacterID, bool, bIsAvailable};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalCharacters : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalCharacters(};

    // 角色系統事件
    UPROPERTY(BlueprintAssignable, Category = "Character System")
    FOnCharacterRelationshipChanged OnCharacterRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Character System")
    FOnCharacterAvailabilityChanged OnCharacterAvailabilityChanged;

    // ?????系?    UFUNCTION(BlueprintCallable, Category = "Character System")
    void InitializeCharacterSystem(};

    // ????歷?人??    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetAllCharacters() const;

    // ??歷史人物
    UFUNCTION(BlueprintPure, Category = "Character System")
    FMingHistoricalCharacter GetCharacter(const FString& CharacterID) const;

    // ??角色??
    UFUNCTION(BlueprintPure, Category = "Character System")
    float GetCharacterRelationship(const FString& CharacterID) const;

    // ?新角色??
    UFUNCTION(BlueprintCallable, Category = "Character System")
    void UpdateCharacterRelationship(const FString& CharacterID, float RelationshipChange};

    // ???用角色
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetAvailableCharacters() const;

    // ????派系角色
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersByFaction(EHistoricalFaction Faction) const;

    // ????角色類?
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersByRole(ECharacterRole Role) const;

    // ???特定地點?角色
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersAtLocation(const FString& Location) const;

    // ??????事件????    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersInEvent(const FString& EventID) const;

    // 檢查角色?否?用
    UFUNCTION(BlueprintPure, Category = "Character System")
    bool IsCharacterAvailable(const FString& CharacterID) const;

    // 設置角色?用??    UFUNCTION(BlueprintCallable, Category = "Character System")
    void SetCharacterAvailability(const FString& CharacterID, bool bIsAvailable};

    // ??角色對話??
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FString> GetCharacterDialogueOptions(const FString& CharacterID) const;

    // ??角色??
    UFUNCTION(BlueprintPure, Category = "Character System")
    FString GenerateCharacterResponse(const FString& CharacterID, EDialogueAttitude PlayerAttitude, const FString& Context) const;

    // 計?角色影響??    UFUNCTION(BlueprintPure, Category = "Character System")
    float CalculateCharacterInfluence(const FString& CharacterID) const;

    // ??角色?薦對話
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FString> GetRecommendedDialogues(const FString& CharacterID, const FMingCharacterData& PlayerCharacter) const;

    // 保?角色??
    UFUNCTION(BlueprintCallable, Category = "Character System")
    bool SaveCharacterData(const FString& SaveSlotName};

    // 載入角色??
    UFUNCTION(BlueprintCallable, Category = "Character System")
    bool LoadCharacterData(const FString& SaveSlotName};

protected:
    // ??歷?人??    UPROPERTY()
    TArray<FMingHistoricalCharacter> AllCharacters;

    // 角色ID??????
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> CharacterMap;

    // 角色??
    UPROPERTY()
    TMap<FString, float> CharacterRelationships;

    // 角色?用??    UPROPERTY()
    TMap<FString, bool> CharacterAvailability;

    // ?否已?始?
    bool bIsInitialized;

    // ???歷?人?庫
    void InitializeHistoricalCharacterLibrary(};

    // ?建??黨人??    void CreateNationalistCharacters(};

    // ?建?產黨人??    void CreateCommunistCharacters(};

    // ?建軍閥人物
    void CreateWarlordCharacters(};

    // ?建??派系人物
    void CreateIndependentCharacters(};

    // ?建外?人物
    void CreateForeignCharacters(};

    // ?建平?人物
    void CreateCivilianCharacters(};

    // ??角色???本
    FString GenerateResponseText(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude, const FString& Context) const;

    // 計???變?
    float CalculateRelationshipChange(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // 檢查角色?格??
    bool CheckPersonalityMatch(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // ??角色??模板
    FString GetResponseTemplate(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // ??角色ID
    FString GenerateCharacterID(const FString& BaseName) const;

    // ?新角色影響??    void UpdateCharacterInfluence(const FString& CharacterID};

    // 檢查歷史??一??    bool CheckHistoricalConsistency(const FString& CharacterID, const FString& CurrentDate) const;
};

