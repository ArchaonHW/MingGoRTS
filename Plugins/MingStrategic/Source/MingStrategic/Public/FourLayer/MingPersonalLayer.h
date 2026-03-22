#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FourLayer/IMingGameLayer.h"
#include "MingPersonalLayer.generated.h"

// ËßíËâ≤?∑Ê•≠È°ûÂ?
UENUM(BlueprintType)
enum class EMingCharacterProfession : uint8
{
    Military,       // Ëªç‰∫∫
    Politician,     // ?øÊ≤ªÂÆ?    Scholar,        // Â≠∏ËÄ?    Merchant,       // ?Ü‰∫∫
    Farmer,         // Ëæ≤Ê?
    Worker,         // Â∑•‰∫∫
    Doctor,         // ?´Á?
    Teacher,        // ?ôÂ∏´
    Journalist,     // Ë®òËÄ?    Artist          // ?ùË?ÂÆ?};

// ËßíËâ≤?Ä?ΩÈ???UENUM(BlueprintType)
enum class EMingCharacterSkill : uint8
{
    Leadership,      // ?òÂ???    Combat,         // ?∞È¨•
    Diplomacy,      // Â§ñ‰∫§
    Economics,      // Á∂ìÊ?
    Scholarship,    // Â≠∏Ë?
    Medicine,       // ?´Â≠∏
    Engineering,    // Â∑•Á?
    Art,            // ?ùË?
    Speech,         // ÊºîË?
    Survival        // ?üÂ?
};

// ?ú‰??Ä??UENUM(BlueprintType)
enum class EMingRelationshipStatus : uint8
{
    Stranger,       // ?åÁ?‰∫?    Acquaintance,   // Ë™çË?
    Friend,         // ?ãÂ?
    CloseFriend,    // Â•ΩÊ???    Family,         // ÂÆ∂‰∫∫
    Lover,          // ?Ö‰∫∫
    Rival,          // Â∞çÊ?
    Enemy,          // ?µ‰∫∫
    Mentor,         // Â∞éÂ∏´
    Student         // Â≠∏Á?
};

// ËßíËâ≤?Ä??USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingCharacterStatus
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    class AActor* CharacterActor;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly)
    EMingCharacterProfession Profession;

    UPROPERTY(BlueprintReadOnly)
    int32 Level;

    UPROPERTY(BlueprintReadOnly)
    float Experience;

    UPROPERTY(BlueprintReadOnly)
    float Health;

    UPROPERTY(BlueprintReadOnly)
    float Morale;

    UPROPERTY(BlueprintReadOnly)
    float Reputation;

    UPROPERTY(BlueprintReadOnly)
    float Influence;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingCharacterSkill, float> Skills;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EMingRelationshipStatus> Relationships;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Inventory;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ActiveQuests;
};

// ?ã‰∫∫Ê±∫Á?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingPersonalDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    TArray<EMingCharacterSkill> RequiredSkills;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredItems;

    UPROPERTY(BlueprintReadOnly)
    float DecisionImpact;

    UPROPERTY(BlueprintReadOnly)
    float PersonalRisk;
};

// ?ã‰∫∫‰∫ã‰ª∂
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingPersonalEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventDescription;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedCharacters;

    UPROPERTY(BlueprintReadOnly)
    float PersonalImpact;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EventTime;
};

// ËßíËâ≤?êÈï∑Ë®òÈ?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingCharacterGrowth
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(BlueprintReadOnly)
    TMap<EMingCharacterSkill, float> SkillProgress;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> RelationshipChanges;

    UPROPERTY(BlueprintReadOnly)
    float ReputationChange;

    UPROPERTY(BlueprintReadOnly)
    float InfluenceChange;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> UnlockedAbilities;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CompletedQuests;
};

/**
 * ?ã‰∫∫Â±§Á≥ªÁµ? * Ë≤†Ë≤¨ËßíËâ≤?ÆÊ??ÅÂÄã‰∫∫?êÈï∑?ÅÈ?‰øÇÁÆ°?ÜÁ?
 */
UCLASS(BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingPersonalLayer : public UObject, public IMingGameLayer
{
    GENERATED_BODY()

public:
    // Âª∫Ê?Â≠?    UMingPersonalLayer();

    // ÂØ¶Áèæ‰ªãÈù¢?πÊ?
    virtual void InitializeLayer_Implementation() override;
    virtual void UpdateLayer_Implementation(float DeltaTime) override;
    virtual EMingLayer GetLayerType_Implementation() const override;
    virtual FString GetLayerName_Implementation() const override;
    virtual float GetLayerPriority_Implementation() const override;
    virtual void HandleLayerEvent_Implementation(const FMingGameEvent& Event) override;
    virtual void HandleLayerDecision_Implementation(const FMingGameDecision& Decision) override;

    // ËßíËâ≤ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void CreateCharacter(const FString& CharacterID, const FString& CharacterName, EMingCharacterProfession Profession);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void UpdateCharacter(const FString& CharacterID, const FMingCharacterStatus& Status);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingCharacterStatus GetCharacter(const FString& CharacterID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TArray<FMingCharacterStatus> GetAllCharacters() const;

    // ?Ä?ΩÁÆ°??    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void ImproveSkill(const FString& CharacterID, EMingCharacterSkill Skill, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    float GetSkillLevel(const FString& CharacterID, EMingCharacterSkill Skill) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TMap<EMingCharacterSkill, float> GetAllSkills(const FString& CharacterID) const;

    // ?ú‰?ÁÆ°Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void UpdateRelationship(const FString& CharacterID, const FString& TargetID, EMingRelationshipStatus Status);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    EMingRelationshipStatus GetRelationship(const FString& CharacterID, const FString& TargetID) const;

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TMap<FString, EMingRelationshipStatus> GetAllRelationships(const FString& CharacterID) const;

    // ?ã‰∫∫Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void MakePersonalDecision(const FMingPersonalDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void ProcessPersonalDecisions();

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    TArray<FMingPersonalDecision> GetPersonalDecisions() const;

    // ËßíËâ≤?êÈï∑
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void ProcessCharacterGrowth(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingCharacterGrowth GetCharacterGrowth(const FString& CharacterID) const;

    // ?≤Â??ã‰∫∫?Ä??    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingPersonalState GetPersonalState() const;

    // ?≤Â?‰∏ªË?ËßíËâ≤
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    FMingCharacterStatus GetMainCharacter() const;

    // Ë®≠ÁΩÆ‰∏ªË?ËßíËâ≤
    UFUNCTION(BlueprintCallable, Category = "Ming|Personal")
    void SetMainCharacter(const FString& CharacterID);

protected:
    // ?ã‰∫∫?Ä??    UPROPERTY(BlueprintReadOnly)
    FMingPersonalState PersonalState;

    // ËßíËâ≤?óË"
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingCharacterStatus> Characters;

    // ?ã‰∫∫Ê±∫Á?
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingPersonalDecision> PersonalDecisions;

    // ?ã‰∫∫‰∫ã‰ª∂Ê≠∑Âè≤
    UPROPERTY(BlueprintReadOnly)
    TArray<FMingPersonalEvent> PersonalHistory;

    // ËßíËâ≤?êÈï∑Ë®òÈ?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FMingCharacterGrowth> CharacterGrowthRecords;

    // ‰∏ªË?ËßíËâ≤ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    FString MainCharacterID;

    // ËßíËâ≤?ºÂ?È¢®Ê†º
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    FString CharacterDevelopmentStyle;

    // ?ú‰?ÁÆ°Á??èÂ•Ω
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    float RelationshipManagementPreference;

    // ?Ä?ΩÁôºÂ±ïÂ?Â•?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal|Settings")
    TMap<EMingCharacterSkill, float> SkillDevelopmentPreferences;

    // ?ÜÊ?ËßíËâ≤?Ä??    void AnalyzeCharacterStatus(const FString& CharacterID);

    // Ë©ï‰º∞?ú‰?Á∂≤Áµ°
    void AssessRelationshipNetwork(const FString& CharacterID);

    // Ë®àÁ?ËßíËâ≤ÂΩ±Èüø??    float CalculateCharacterInfluence(const FString& CharacterID);

    // ?êÊ∏¨Ê±∫Á?ÂæåÊ?
    TArray<FString> PredictDecisionOutcome(const FMingPersonalDecision& Decision);

    // Ê∞ëÂ??πËâ≤?ã‰∫∫?ºÂ?
    void ApplyRepublicanEraCharacterDevelopment();

    // ?∞Ê??ñÈ??ïÂΩ±??    void ApplyNewCultureMovementInfluence(const FString& CharacterID);

    // ?ôËÇ≤?πÈù©Ê©üÈ?
    void ApplyEducationReformOpportunities(const FString& CharacterID);

    // Â•≥ÊÄßËß£?æÈ???    void ApplyWomenLiberationMovement(const FString& CharacterID);

    // ?•Ë??ÜÂ??∏Ê?
    void ApplyIntellectualChoices(const FString& CharacterID);

    // ?ÜÊ•≠?ºÂ?Ê©üÈ?
    void ApplyBusinessDevelopmentOpportunities(const FString& CharacterID);

    // Ëªç‰??üÊ∂Ø?ºÂ?
    void ApplyMilitaryCareerDevelopment(const FString& CharacterID);

private:
    // ?ã‰∫∫?¥Êñ∞?ìÈ?
    float PersonalUpdateInterval;

    // ‰∏äÊ¨°?¥Êñ∞?ÇÈ?
    float LastPersonalUpdate;

    // ËßíËâ≤Á≠âÁ?‰∏äÈ?
    int32 MaxCharacterLevel;

    // ?Ä?ΩÁ?Á¥ö‰???    float MaxSkillLevel;

    // ?ú‰?ÂΩ±Èüø?äÂ?
    float RelationshipInfluenceRadius;

    // ?ã‰∫∫Â®ÅË?Ë©ï‰º∞
    TMap<FString, float> PersonalThreats;

    // ?ã‰∫∫Ê©üÊ?Ë©ï‰º∞
    TMap<FString, float> PersonalOpportunities;

    // ?ã‰∫∫Ê±∫Á?Ê≠∑Âè≤
    TArray<FMingPersonalDecision> DecisionHistory;
};
