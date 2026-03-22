#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillSystem.generated.h"

UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat, UMETA(DisplayName = "??鬥????),
    Strategy, UMETA(DisplayName = "策略????),
    Diplomacy, UMETA(DisplayName = "外交????),
    Leadership, UMETA(DisplayName = "????????),
    Intelligence, UMETA(DisplayName = "??報????),
    Economic, UMETA(DisplayName = "經??????)
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Active, UMETA(DisplayName = "主??????),
    Passive, UMETA(DisplayName = "被??????),
    Toggle, UMETA(DisplayName = "????????)
};

UENUM(BlueprintType)
enum class ESpecializationPath : uint8
{
    Commander, UMETA(DisplayName = "??揮官路??),
    Tactician, UMETA(DisplayName = "????家路??),
    Diplomat, UMETA(DisplayName = "外交官路??),
    Spy, UMETA(DisplayName = "????路??"),
    Economist, UMETA(DisplayName = "經??家路??),
    Generalist, UMETA(DisplayName = "????路??")
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

    // 注??：TMap<TArray> 不支??UPROPERTY
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

    // ????系統????    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillLevelUp OnSkillLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillUnlocked OnSkillUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSpecializationChosen OnSpecializationChosen;

    // ??????????系??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void InitializeSkillSystem(};

    // ????????????    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAllSkills() const;

    // ????????    UFUNCTION(BlueprintPure, Category = "Skill System")
    FMingSkill GetSkill(const FName& SkillID) const;

    // ???????    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UnlockSkill(const FName& SkillID};

    // ????????    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UpgradeSkill(const FName& SkillID, int32 ExperienceAmount = 0};

    // 添??????????    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void AddSkillExperience(const FName& SkillID, int32 ExperienceAmount};

    // 檢查????是??可以解??    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUnlockSkill(const FName& SkillID, const FMingCharacterData& CharacterData) const;

    // 檢查????是??可以????    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUpgradeSkill(const FName& SkillID) const;

    // ????????樹
    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FSkillTree> GetSkillTrees() const;

    // ????專精路??
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ChooseSpecializationPath(ESpecializationPath Path};

    // ????????專精路??
    UFUNCTION(BlueprintPure, Category = "Skill System")
    ESpecializationPath GetCurrentSpecializationPath() const;

    // 計??????????    UFUNCTION(BlueprintPure, Category = "Skill System")
    float CalculateSkillEffect(const FName& SkillID, const FString& EffectType) const;

    // ????已解??????    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetUnlockedSkills() const;

    // ??????解??????    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAvailableSkills(const FMingCharacterData& CharacterData) const;

    // ??置????系??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ResetSkillSystem(};

    // 保??????數??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool SaveSkillData(const FString& SaveSlotName};

    // 載入????數??    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool LoadSkillData(const FString& SaveSlotName};

protected:
    // ????????數??    UPROPERTY()
    TArray<FMingSkill> AllSkills;

    // ????ID????????????
    UPROPERTY()
    TMap<FName, FMingSkill> SkillMap;

    // ????樹????
    UPROPERTY()
    TArray<FSkillTree> SkillTrees;

    // ????專精路??
    UPROPERTY()
    ESpecializationPath CurrentSpecializationPath;

    // ??用????????    UPROPERTY()
    int32 AvailableSkillPoints;

    // ??否已??始??
    bool bIsInitialized;

    // ??????????庫
    void InitializeSkillLibrary(};

    // ??建??鬥????    void CreateCombatSkills(};

    // ??建策略????    void CreateStrategySkills(};

    // ??建外交????    void CreateDiplomacySkills(};

    // ??建????????    void CreateLeadershipSkills(};

    // ??建??報????    void CreateIntelligenceSkills(};

    // ??建經??????    void CreateEconomicSkills(};

    // ??????????樹
    void InitializeSkillTrees(};

    // 驗??????????    bool ValidateSkillRequirements(const FSkillRequirement& Requirement, const FMingCharacterData& CharacterData) const;

    // 計????????驗????    int32 CalculateExperienceRequirement(int32 SkillLevel) const;

    // ??用????????    void ApplySkillEffects(const FName& SkillID};

    // ???????????    void UnlockConnectedSkills(const FName& SkillID};

    // ????????路徑????    FLinearColor GetSpecializationPathColor(ESpecializationPath Path) const;

    // ????????ID
    FName GenerateSkillID(const FString& BaseName, ESkillCategory Category) const;
};

