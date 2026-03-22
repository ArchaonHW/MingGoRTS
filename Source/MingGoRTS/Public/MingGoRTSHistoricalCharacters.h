#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSDialogueSystem.h"
#include "MingGoRTSHistoricalCharacters.generated.h"

UENUM(BlueprintType)
enum class EHistoricalFaction : uint8
{
    Nationalist     UMETA(DisplayName = "?ãÊ?Èª?),
    Communist       UMETA(DisplayName = "?±Áî¢Èª?),
    Warlord         UMETA(DisplayName = "ËªçÈñ•"),
    Independent     UMETA(DisplayName = "?®Á?Ê¥æÁ≥ª"),
    Foreign         UMETA(DisplayName = "Â§ñÂ??¢Â?")
};

UENUM(BlueprintType)
enum class ECharacterRole : uint8
{
    Leader          UMETA(DisplayName = "?òË?"),
    Military        UMETA(DisplayName = "Ëªç‰?‰∫∫Áâ©"),
    Political       UMETA(DisplayName = "?øÊ≤ª‰∫∫Áâ©"),
    Diplomatic      UMETA(DisplayName = "Â§ñ‰∫§‰∫∫Áâ©"),
    Intelligence    UMETA(DisplayName = "?ÖÂ†±‰∫∫Áâ©"),
    Economic        UMETA(DisplayName = "Á∂ìÊ?‰∫∫Áâ©"),
    Cultural        UMETA(DisplayName = "?áÂ?‰∫∫Áâ©"),
    Civilian        UMETA(DisplayName = "Âπ≥Ê?")
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
        RelationshipValue = 50.0f; // ÈªòË?‰∏≠Á?
        LastInteractionDate = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FCharacterPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Openness;          // ?ãÊîæ??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Conscientiousness; // Ë≤¨‰ªª??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Extraversion;      // Â§ñÂ???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Agreeableness;     // ÂÆú‰∫∫??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Neuroticism;       // Á•ûÁ?Ë≥?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Patriotism;        // ?õÂ?‰∏ªÁæ©

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Personality")
    float Ambition;          // ?éÂ?

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
    FString CourtesyName;    // Â≠?
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

    // ËßíËâ≤Á≥ªÁµ±‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Character System")
    FOnCharacterRelationshipChanged OnCharacterRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Character System")
    FOnCharacterAvailabilityChanged OnCharacterAvailabilityChanged;

    // ?ùÂ??ñË??≤Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Character System")
    void InitializeCharacterSystem();

    // ?≤Â??Ä?âÊ≠∑?≤‰∫∫??    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetAllCharacters() const;

    // ?≤Â?Ê≠∑Âè≤‰∫∫Áâ©
    UFUNCTION(BlueprintPure, Category = "Character System")
    FMingHistoricalCharacter GetCharacter(const FString& CharacterID) const;

    // ?≤Â?ËßíËâ≤?ú‰?
    UFUNCTION(BlueprintPure, Category = "Character System")
    float GetCharacterRelationship(const FString& CharacterID) const;

    // ?¥Êñ∞ËßíËâ≤?ú‰?
    UFUNCTION(BlueprintCallable, Category = "Character System")
    void UpdateCharacterRelationship(const FString& CharacterID, float RelationshipChange);

    // ?≤Â??ØÁî®ËßíËâ≤
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetAvailableCharacters() const;

    // ?≤Â??πÂ?Ê¥æÁ≥ªËßíËâ≤
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersByFaction(EHistoricalFaction Faction) const;

    // ?≤Â??πÂ?ËßíËâ≤È°ûÂ?
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersByRole(ECharacterRole Role) const;

    // ?≤Â??®ÁâπÂÆöÂú∞ÈªûÁ?ËßíËâ≤
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersAtLocation(const FString& Location) const;

    // ?≤Â??ÉË??πÂ?‰∫ã‰ª∂?ÑË???    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FMingHistoricalCharacter> GetCharactersInEvent(const FString& EventID) const;

    // Ê™¢Êü•ËßíËâ≤?ØÂê¶?ØÁî®
    UFUNCTION(BlueprintPure, Category = "Character System")
    bool IsCharacterAvailable(const FString& CharacterID) const;

    // Ë®≠ÁΩÆËßíËâ≤?ØÁî®??    UFUNCTION(BlueprintCallable, Category = "Character System")
    void SetCharacterAvailability(const FString& CharacterID, bool bIsAvailable);

    // ?≤Â?ËßíËâ≤Â∞çË©±?∏È?
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FString> GetCharacterDialogueOptions(const FString& CharacterID) const;

    // ?üÊ?ËßíËâ≤?ûÊ?
    UFUNCTION(BlueprintPure, Category = "Character System")
    FString GenerateCharacterResponse(const FString& CharacterID, EDialogueAttitude PlayerAttitude, const FString& Context) const;

    // Ë®àÁ?ËßíËâ≤ÂΩ±Èüø??    UFUNCTION(BlueprintPure, Category = "Character System")
    float CalculateCharacterInfluence(const FString& CharacterID) const;

    // ?≤Â?ËßíËâ≤?®Ëñ¶Â∞çË©±
    UFUNCTION(BlueprintPure, Category = "Character System")
    TArray<FString> GetRecommendedDialogues(const FString& CharacterID, const FMingCharacterData& PlayerCharacter) const;

    // ‰øùÂ?ËßíËâ≤?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Character System")
    bool SaveCharacterData(const FString& SaveSlotName);

    // ËºâÂÖ•ËßíËâ≤?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Character System")
    bool LoadCharacterData(const FString& SaveSlotName);

protected:
    // ?Ä?âÊ≠∑?≤‰∫∫??    UPROPERTY()
    TArray<FMingHistoricalCharacter> AllCharacters;

    // ËßíËâ≤ID?∞Ë??≤Á??†Â?
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> CharacterMap;

    // ËßíËâ≤?ú‰?
    UPROPERTY()
    TMap<FString, float> CharacterRelationships;

    // ËßíËâ≤?ØÁî®??    UPROPERTY()
    TMap<FString, bool> CharacterAvailability;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ≠∑?≤‰∫∫?©Â∫´
    void InitializeHistoricalCharacterLibrary();

    // ?µÂª∫?ãÊ?Èª®‰∫∫??    void CreateNationalistCharacters();

    // ?µÂª∫?±Áî¢Èª®‰∫∫??    void CreateCommunistCharacters();

    // ?µÂª∫ËªçÈñ•‰∫∫Áâ©
    void CreateWarlordCharacters();

    // ?µÂª∫?®Á?Ê¥æÁ≥ª‰∫∫Áâ©
    void CreateIndependentCharacters();

    // ?µÂª∫Â§ñÂ?‰∫∫Áâ©
    void CreateForeignCharacters();

    // ?µÂª∫Âπ≥Ê?‰∫∫Áâ©
    void CreateCivilianCharacters();

    // ?üÊ?ËßíËâ≤?ûÊ??áÊú¨
    FString GenerateResponseText(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude, const FString& Context) const;

    // Ë®àÁ??ú‰?ËÆäÂ?
    float CalculateRelationshipChange(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // Ê™¢Êü•ËßíËâ≤?ßÊ†º?πÈ?
    bool CheckPersonalityMatch(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // ?≤Â?ËßíËâ≤?ûÊ?Ê®°Êùø
    FString GetResponseTemplate(const FMingHistoricalCharacter& Character, EDialogueAttitude PlayerAttitude) const;

    // ?üÊ?ËßíËâ≤ID
    FString GenerateCharacterID(const FString& BaseName) const;

    // ?¥Êñ∞ËßíËâ≤ÂΩ±Èüø??    void UpdateCharacterInfluence(const FString& CharacterID);

    // Ê™¢Êü•Ê≠∑Âè≤?ÇÈ?‰∏Ä?¥ÊÄ?    bool CheckHistoricalConsistency(const FString& CharacterID, const FString& CurrentDate) const;
};
