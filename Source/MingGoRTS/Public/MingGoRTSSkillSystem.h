#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillSystem.generated.h"

UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat, UMETA(DisplayName = "X��X),
    Strategy, UMETA(DisplayName = "����X),
    Diplomacy, UMETA(DisplayName = "�~��X),
    Leadership, UMETA(DisplayName = "X),
    Intelligence, UMETA(DisplayName = "X��X),
    Economic, UMETA(DisplayName = "�gX)
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Active, UMETA(DisplayName = "�DX),
    Passive, UMETA(DisplayName = "�QX),
    Toggle, UMETA(DisplayName = "X)
};

UENUM(BlueprintType)
enum class ESpecializationPath : uint8
{
    Commander, UMETA(DisplayName = "X���x��X),
    Tactician, UMETA(DisplayName = "X�a��X),
    Diplomat, UMETA(DisplayName = "�~��x��X),
    Spy, UMETA(DisplayName = "X��X"),
    Economist, UMETA(DisplayName = "�gX�a��X),
    Generalist, UMETA(DisplayName = "X��X")
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
        EffectType = TEXT(""};
        BaseValue = 0.0f;
        ScalingPerLevel = 0.0f;
        TargetAttribute = TEXT(""};
        Description = TEXT(""};
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
        SkillName = TEXT(""};
        Description = TEXT(""};
        IconPath = TEXT(""};
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

    // �`X�GTMap<TArray> ����XUPROPERTY
    TMap<FName, TArray<FName>> SkillConnections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    FLinearColor PathColor;

    FSkillTree()
    {
        Path = ESpecializationPath::Generalist;
        PathName = TEXT(""};
        PathDescription = TEXT(""};
        PathColor = FLinearColor::White;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelUp, const FName&, SkillID, int32, NewLevel};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillUnlocked, const FName&, SkillID, const FMingSkill&, SkillData};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecializationChosen, ESpecializationPath, ChosenPath};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSSkillSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSSkillSystem(};

    // X�t��X    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillLevelUp OnSkillLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillUnlocked OnSkillUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSpecializationChosen OnSpecializationChosen;

    // X�tX    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void InitializeSkillSystem(};

    // X    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAllSkills() const;

    // X    UFUNCTION(BlueprintPure, Category = "Skill System")
    FMingSkill GetSkill(const FName& SkillID) const;

    // X    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UnlockSkill(const FName& SkillID};

    // X    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UpgradeSkill(const FName& SkillID, int32 ExperienceAmount = 0};

    // �KX    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void AddSkillExperience(const FName& SkillID, int32 ExperienceAmount};

    // �ˬdX�OX�i�H��X    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUnlockSkill(const FName& SkillID, const FMingCharacterData& CharacterData) const;

    // �ˬdX�OX�i�HX    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUpgradeSkill(const FName& SkillID) const;

    // X��
    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FSkillTree> GetSkillTrees() const;

    // X�M���X
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ChooseSpecializationPath(ESpecializationPath Path};

    // X�M���X
    UFUNCTION(BlueprintPure, Category = "Skill System")
    ESpecializationPath GetCurrentSpecializationPath() const;

    // �pX    UFUNCTION(BlueprintPure, Category = "Skill System")
    float CalculateSkillEffect(const FName& SkillID, const FString& EffectType) const;

    // X�w��X    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetUnlockedSkills() const;

    // X��X    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAvailableSkills(const FMingCharacterData& CharacterData) const;

    // X�mX�tX    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ResetSkillSystem(};

    // �OX��X    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool SaveSkillData(const FString& SaveSlotName};

    // ���JX��X    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool LoadSkillData(const FString& SaveSlotName};

protected:
    // X��X    UPROPERTY()
    TArray<FMingSkill> AllSkills;

    // XIDX
    UPROPERTY()
    TMap<FName, FMingSkill> SkillMap;

    // X��X
    UPROPERTY()
    TArray<FSkillTree> SkillTrees;

    // X�M���X
    UPROPERTY()
    ESpecializationPath CurrentSpecializationPath;

    // X��X    UPROPERTY()
    int32 AvailableSkillPoints;

    // X�_�wX�lX
    bool bIsInitialized;

    // X�w
    void InitializeSkillLibrary(};

    // X��X��X    void CreateCombatSkills(};

    // X�ص���X    void CreateStrategySkills(};

    // X�إ~��X    void CreateDiplomacySkills(};

    // X��X    void CreateLeadershipSkills(};

    // X��X��X    void CreateIntelligenceSkills(};

    // X�ظgX    void CreateEconomicSkills(};

    // X��
    void InitializeSkillTrees(};

    // ��X    bool ValidateSkillRequirements(const FSkillRequirement& Requirement, const FMingCharacterData& CharacterData) const;

    // �pX��X    int32 CalculateExperienceRequirement(int32 SkillLevel) const;

    // X��X    void ApplySkillEffects(const FName& SkillID};

    // X    void UnlockConnectedSkills(const FName& SkillID};

    // X��|X    FLinearColor GetSpecializationPathColor(ESpecializationPath Path) const;

    // XID
    FName GenerateSkillID(const FString& BaseName, ESkillCategory Category) const;
};

