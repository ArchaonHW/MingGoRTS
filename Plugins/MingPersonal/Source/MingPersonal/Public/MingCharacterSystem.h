#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterSystem.generated.h"

UENUM(BlueprintType)
enum class EMingCharacterRole : uint8
{
    Leader UMETA(DisplayName = "Leader"),
    Commander UMETA(DisplayName = "Commander"),
    Diplomat UMETA(DisplayName = "Diplomat"),
    Scholar UMETA(DisplayName = "Scholar"),
    Merchant UMETA(DisplayName = "Merchant"),
    Spy UMETA(DisplayName = "Spy"),
    Advisor UMETA(DisplayName = "Advisor"),
    Revolutionary UMETA(DisplayName = "Revolutionary")
};

UENUM(BlueprintType)
enum class EMingPersonalSkillType : uint8
{
    Leadership UMETA(DisplayName = "Leadership"),
    Military UMETA(DisplayName = "Military"),
    Diplomacy UMETA(DisplayName = "Diplomacy"),
    Economics UMETA(DisplayName = "Economics"),
    Culture UMETA(DisplayName = "Culture"),
    Intelligence UMETA(DisplayName = "Intelligence"),
    Administration UMETA(DisplayName = "Administration"),
    Combat UMETA(DisplayName = "Combat")
};

USTRUCT(BlueprintType)
struct FMingCharacterSkill
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSkillType SkillType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Experience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ExperienceToNext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SkillEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Prerequisites;

    UPROPERTY(BlueprintReadOnly)
    bool bIsMaxLevel;

    FMingCharacterSkill()
        : SkillType(EMingSkillType::Leadership)
        , Level(1)
        , MaxLevel(10)
        , Experience(0)
        , ExperienceToNext(100)
        , bIsMaxLevel(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingCharacterTrait
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TraitID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TraitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PositiveEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> NegativeEffects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInherited;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float InfluenceWeight;

    FMingCharacterTrait()
        : bIsInherited(false)
        , InfluenceWeight(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingPersonalCharacter
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    EMingCharacterRole PrimaryRole;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<EMingCharacterRole> SecondaryRoles;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Biography;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString HistoricalSignificance;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingCharacterSkill> Skills;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FMingCharacterTrait> Traits;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 BirthYear;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 DeathYear;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString BirthPlace;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FString> MajorAchievements;

    UPROPERTY(BlueprintReadOnly)
    bool bIsAvailable;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRecruited;

    UPROPERTY(BlueprintReadOnly)
    float Loyalty;

    UPROPERTY(BlueprintReadOnly)
    float Influence;

    FMingHistoricalCharacter()
        : PrimaryRole(EMingCharacterRole::Leader)
        , BirthYear(1866)
        , DeathYear(1925)
        , bIsAvailable(true)
        , bIsRecruited(false)
        , Loyalty(50.0f)
        , Influence(0.0f)
    {}
};

/**
 * Character System for MingGoRTS
 * Manages historical characters, skills, and development
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGPERSONAL_API UMingCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCharacterSystem(};

    // Character Management
    UFUNCTION(BlueprintCallable, Category = "Character")
    void InitializeCharacters(};

    UFUNCTION(BlueprintCallable, Category = "Character")
    void RecruitCharacter(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Character")
    void DismissCharacter(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Character")
    void AssignCharacterRole(const FString& CharacterID, EMingCharacterRole Role};

    UFUNCTION(BlueprintPure, Category = "Character")
    TArray<FMingHistoricalCharacter> GetAllCharacters() const;

    UFUNCTION(BlueprintPure, Category = "Character")
    TArray<FMingHistoricalCharacter> GetAvailableCharacters() const;

    UFUNCTION(BlueprintPure, Category = "Character")
    TArray<FMingHistoricalCharacter> GetRecruitedCharacters() const;

    UFUNCTION(BlueprintPure, Category = "Character")
    FMingHistoricalCharacter GetCharacter(const FString& CharacterID) const;

    // Skill System
    UFUNCTION(BlueprintCallable, Category = "Skills")
    void AddSkillExperience(const FString& CharacterID, EMingSkillType SkillType, int32 Experience};

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void LevelUpSkill(const FString& CharacterID, EMingSkillType SkillType};

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void LearnNewSkill(const FString& CharacterID, const FString& SkillID};

    UFUNCTION(BlueprintPure, Category = "Skills")
    TArray<FMingCharacterSkill> GetCharacterSkills(const FString& CharacterID) const;

    UFUNCTION(BlueprintPure, Category = "Skills")
    FMingCharacterSkill GetSkill(const FString& CharacterID, EMingSkillType SkillType) const;

    UFUNCTION(BlueprintPure, Category = "Skills")
    int32 GetSkillLevel(const FString& CharacterID, EMingSkillType SkillType) const;

    UFUNCTION(BlueprintPure, Category = "Skills")
    float GetSkillBonus(const FString& CharacterID, EMingSkillType SkillType) const;

    // Character Development
    UFUNCTION(BlueprintCallable, Category = "Development")
    void TrainCharacter(const FString& CharacterID, EMingSkillType SkillType, float Duration};

    UFUNCTION(BlueprintCallable, Category = "Development")
    void PromoteCharacter(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Development")
    void DevelopTrait(const FString& CharacterID, const FString& TraitID};

    UFUNCTION(BlueprintPure, Category = "Development")
    float GetCharacterExperience(const FString& CharacterID) const;

    UFUNCTION(BlueprintPure, Category = "Development")
    int32 GetCharacterLevel(const FString& CharacterID) const;

    // Character Relationships
    UFUNCTION(BlueprintCallable, Category = "Relationships")
    void SetCharacterLoyalty(const FString& CharacterID, float Loyalty};

    UFUNCTION(BlueprintCallable, Category = "Relationships")
    void ModifyRelationship(const FString& CharacterID1, const FString& CharacterID2, float Modifier};

    UFUNCTION(BlueprintPure, Category = "Relationships")
    float GetCharacterLoyalty(const FString& CharacterID) const;

    UFUNCTION(BlueprintPure, Category = "Relationships")
    float GetRelationship(const FString& CharacterID1, const FString& CharacterID2) const;

    // Character Effects
    UFUNCTION(BlueprintPure, Category = "Effects")
    TArray<FString> GetActiveEffects(const FString& CharacterID) const;

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void ApplyCharacterEffect(const FString& CharacterID, const FString& EffectID};

    UFUNCTION(BlueprintCallable, Category = "Effects")
    void RemoveCharacterEffect(const FString& CharacterID, const FString& EffectID};

    // Historical Context
    UFUNCTION(BlueprintCallable, Category = "Historical")
    void ShowCharacterBiography(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Historical")
    void AddHistoricalEvent(const FString& CharacterID, const FString& Event};

    UFUNCTION(BlueprintPure, Category = "Historical")
    TArray<FString> GetCharacterEvents(const FString& CharacterID) const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterRecruited, const FMingHistoricalCharacter&, Character, const FString&, Role};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelUp, const FString&, CharacterID, const FMingCharacterSkill&, Skill};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterPromoted, const FString&, CharacterID, int32, NewLevel};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTraitDeveloped, const FString&, CharacterID, const FString&, TraitID};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCharacterRecruited OnCharacterRecruited;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSkillLevelUp OnSkillLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCharacterPromoted OnCharacterPromoted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTraitDeveloped OnTraitDeveloped;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetRoleName(EMingCharacterRole Role};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetSkillTypeName(EMingSkillType SkillType};

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SaveCharacterData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadCharacterData(const FString& JsonString};

protected:
    UPROPERTY()
    TMap<FString, FMingHistoricalCharacter> Characters;

    // �?：TMap 不能X��XTMap XUPROPERTY X    // TMap<FString, TMap<FString, float>> CharacterRelationships;

    // �?：TArray 不能X��XTMap XUPROPERTY X    // TMap<FString, TArray<FString>> CharacterEffects;

    // �?：TArray 不能X��XTMap XUPROPERTY X    // TMap<FString, TArray<FString>> HistoricalEvents;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Character Setup
    void SetupRevolutionaryCharacters(};
    void SetupMilitaryCharacters(};
    void SetupPoliticalCharacters(};
    void SetupCulturalCharacters(};

    // Skill Management
    void ProcessSkillExperience(FMingCharacterSkill& Skill, int32 Experience};
    bool CanLevelUpSkill(const FMingCharacterSkill& Skill) const;
    void ApplySkillEffects(const FString& CharacterID, const FMingCharacterSkill& Skill};

    // Internal Functions
    void UpdateCharacterStats(const FString& CharacterID};
    void CheckSkillPrerequisites(const FString& CharacterID, const FString& SkillID};
    void ProcessTraitEffects(const FString& CharacterID, const FMingCharacterTrait& Trait};

    // Helpers
    FMingHistoricalCharacter* FindCharacter(const FString& CharacterID};
    FMingCharacterSkill* FindCharacterSkill(const FString& CharacterID, EMingSkillType SkillType};
};

