#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillSystem.generated.h"

UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat         UMETA(DisplayName = "?∞È¨•?Ä??),
    Strategy       UMETA(DisplayName = "Á≠ñÁï•?Ä??),
    Diplomacy      UMETA(DisplayName = "Â§ñ‰∫§?Ä??),
    Leadership     UMETA(DisplayName = "?òÂ??Ä??),
    Intelligence   UMETA(DisplayName = "?ÖÂ†±?Ä??),
    Economic       UMETA(DisplayName = "Á∂ìÊ??Ä??)
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Active         UMETA(DisplayName = "‰∏ªÂ??Ä??),
    Passive        UMETA(DisplayName = "Ë¢´Â??Ä??),
    Toggle         UMETA(DisplayName = "?ãÈ??Ä??)
};

UENUM(BlueprintType)
enum class ESpecializationPath : uint8
{
    Commander      UMETA(DisplayName = "?áÊèÆÂÆòË∑ØÂæ?),
    Tactician      UMETA(DisplayName = "?∞Ë?ÂÆ∂Ë∑ØÂæ?),
    Diplomat       UMETA(DisplayName = "Â§ñ‰∫§ÂÆòË∑ØÂæ?),
    Spy            UMETA(DisplayName = "?ìË?Ë∑ØÂ?"),
    Economist      UMETA(DisplayName = "Á∂ìÊ?ÂÆ∂Ë∑ØÂæ?),
    Generalist     UMETA(DisplayName = "?öÊ?Ë∑ØÂ?")
};

USTRUCT(BlueprintType)
struct FSkillRequirement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Requirement")
    FName RequiredSkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Requirement")
    int32 RequiredLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Requirement")
    int32 MinimumCharacterLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Requirement")
    ECharacterBackground RequiredBackground;

    FSkillRequirement()
    {
        RequiredSkillID = NAME_None;
        RequiredLevel = 1;
        MinimumCharacterLevel = 1;
        RequiredBackground = ECharacterBackground::MilitaryAcademy;
    }
};

USTRUCT(BlueprintType)
struct FSkillEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Effect")
    FString EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Effect")
    float BaseValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Effect")
    float ScalingPerLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Effect")
    FString TargetAttribute;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Effect")
    FString Description;

    FSkillEffect()
    {
        EffectType = TEXT("");
        BaseValue = 0.0f;
        ScalingPerLevel = 0.0f;
        TargetAttribute = TEXT("");
        Description = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingSkill
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString IconPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType SkillType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESpecializationPath SpecializationPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 MaxLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 CurrentLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 CurrentExperience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 ExperienceToNextLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FSkillRequirement> Requirements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FSkillEffect> Effects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FName> UnlockedSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool bIsUnlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    bool bIsActive;

    FMingSkill()
    {
        SkillID = NAME_None;
        SkillName = TEXT("");
        Description = TEXT("");
        IconPath = TEXT("");
        Category = ESkillCategory::Combat;
        SkillType = ESkillType::Passive;
        SpecializationPath = ESpecializationPath::Generalist;
        MaxLevel = 5;
        CurrentLevel = 0;
        CurrentExperience = 0;
        ExperienceToNextLevel = 100;
        bIsUnlocked = false;
        bIsActive = false;
    }
};

USTRUCT(BlueprintType)
struct FSkillTree
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    ESpecializationPath Path;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    FString PathName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    FString PathDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    TArray<FName> RootSkills;

    // Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<FName, TArray<FName>> SkillConnections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    FLinearColor PathColor;

    FSkillTree()
    {
        Path = ESpecializationPath::Generalist;
        PathName = TEXT("");
        PathDescription = TEXT("");
        PathColor = FLinearColor::White;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelUp, const FName&, SkillID, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillUnlocked, const FName&, SkillID, const FMingSkill&, SkillData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecializationChosen, ESpecializationPath, ChosenPath);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSSkillSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSSkillSystem();

    // ?Ä?ΩÁ≥ªÁµ±‰?‰ª?    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillLevelUp OnSkillLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillUnlocked OnSkillUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSpecializationChosen OnSpecializationChosen;

    // ?ùÂ??ñÊ??ΩÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void InitializeSkillSystem();

    // ?≤Â??Ä?âÊ???    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAllSkills() const;

    // ?≤Â??Ä??    UFUNCTION(BlueprintPure, Category = "Skill System")
    FMingSkill GetSkill(const FName& SkillID) const;

    // Ëß???Ä??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UnlockSkill(const FName& SkillID);

    // ?áÁ??Ä??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UpgradeSkill(const FName& SkillID, int32 ExperienceAmount = 0);

    // Ê∑ªÂ??Ä?ΩÁ?È©?    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void AddSkillExperience(const FName& SkillID, int32 ExperienceAmount);

    // Ê™¢Êü•?Ä?ΩÊòØ?¶ÂèØ‰ª•Ëß£??    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUnlockSkill(const FName& SkillID, const FMingCharacterData& CharacterData) const;

    // Ê™¢Êü•?Ä?ΩÊòØ?¶ÂèØ‰ª•Â?Á¥?    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUpgradeSkill(const FName& SkillID) const;

    // ?≤Â??Ä?ΩÊ®π
    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FSkillTree> GetSkillTrees() const;

    // ?∏Ê?Â∞àÁ≤æË∑ØÂ?
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ChooseSpecializationPath(ESpecializationPath Path);

    // ?≤Â??∂Â?Â∞àÁ≤æË∑ØÂ?
    UFUNCTION(BlueprintPure, Category = "Skill System")
    ESpecializationPath GetCurrentSpecializationPath() const;

    // Ë®àÁ??Ä?ΩÊ???    UFUNCTION(BlueprintPure, Category = "Skill System")
    float CalculateSkillEffect(const FName& SkillID, const FString& EffectType) const;

    // ?≤Â?Â∑≤Ëß£?ñÊ???    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetUnlockedSkills() const;

    // ?≤Â??ØËß£?ñÊ???    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAvailableSkills(const FMingCharacterData& CharacterData) const;

    // ?çÁΩÆ?Ä?ΩÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ResetSkillSystem();

    // ‰øùÂ??Ä?ΩÊï∏??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool SaveSkillData(const FString& SaveSlotName);

    // ËºâÂÖ•?Ä?ΩÊï∏??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool LoadSkillData(const FString& SaveSlotName);

protected:
    // ?Ä?âÊ??ΩÊï∏??    UPROPERTY()
    TArray<FMingSkill> AllSkills;

    // ?Ä?ΩID?∞Ê??ΩÁ??†Â?
    UPROPERTY()
    TMap<FName, FMingSkill> SkillMap;

    // ?Ä?ΩÊ®π?∏Ê?
    UPROPERTY()
    TArray<FSkillTree> SkillTrees;

    // ?∂Â?Â∞àÁ≤æË∑ØÂ?
    UPROPERTY()
    ESpecializationPath CurrentSpecializationPath;

    // ?ØÁî®?Ä?ΩÈ???    UPROPERTY()
    int32 AvailableSkillPoints;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ??ΩÂ∫´
    void InitializeSkillLibrary();

    // ?µÂª∫?∞È¨•?Ä??    void CreateCombatSkills();

    // ?µÂª∫Á≠ñÁï•?Ä??    void CreateStrategySkills();

    // ?µÂª∫Â§ñ‰∫§?Ä??    void CreateDiplomacySkills();

    // ?µÂª∫?òÂ??Ä??    void CreateLeadershipSkills();

    // ?µÂª∫?ÖÂ†±?Ä??    void CreateIntelligenceSkills();

    // ?µÂª∫Á∂ìÊ??Ä??    void CreateEconomicSkills();

    // ?ùÂ??ñÊ??ΩÊ®π
    void InitializeSkillTrees();

    // È©óË??Ä?ΩÈ?Ê±?    bool ValidateSkillRequirements(const FSkillRequirement& Requirement, const FMingCharacterData& CharacterData) const;

    // Ë®àÁ??Ä?ΩÁ?È©óÈ?Ê±?    int32 CalculateExperienceRequirement(int32 SkillLevel) const;

    // ?âÁî®?Ä?ΩÊ???    void ApplySkillEffects(const FName& SkillID);

    // Ëß????é•?Ä??    void UnlockConnectedSkills(const FName& SkillID);

    // ?≤Â??Ä?ΩË∑ØÂæëÈ???    FLinearColor GetSpecializationPathColor(ESpecializationPath Path) const;

    // ?üÊ??Ä?ΩID
    FName GenerateSkillID(const FString& BaseName, ESkillCategory Category) const;
};
