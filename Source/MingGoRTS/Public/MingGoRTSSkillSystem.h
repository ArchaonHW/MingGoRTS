#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillSystem.generated.h"

UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat         UMETA(DisplayName = "戰鬥技能"),
    Strategy       UMETA(DisplayName = "策略技能"),
    Diplomacy      UMETA(DisplayName = "外交技能"),
    Leadership     UMETA(DisplayName = "領導技能"),
    Intelligence   UMETA(DisplayName = "情報技能"),
    Economic       UMETA(DisplayName = "經濟技能")
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Active         UMETA(DisplayName = "主動技能"),
    Passive        UMETA(DisplayName = "被動技能"),
    Toggle         UMETA(DisplayName = "開關技能")
};

UENUM(BlueprintType)
enum class ESpecializationPath : uint8
{
    Commander      UMETA(DisplayName = "指揮官路徑"),
    Tactician      UMETA(DisplayName = "戰術家路徑"),
    Diplomat       UMETA(DisplayName = "外交官路徑"),
    Spy            UMETA(DisplayName = "間諜路徑"),
    Economist      UMETA(DisplayName = "經濟家路徑"),
    Generalist     UMETA(DisplayName = "通才路徑")
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

    // 注意：TMap<TArray> 不支持 UPROPERTY
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

    // 技能系統事件
    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillLevelUp OnSkillLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSkillUnlocked OnSkillUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Skill System")
    FOnSpecializationChosen OnSpecializationChosen;

    // 初始化技能系統
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void InitializeSkillSystem();

    // 獲取所有技能
    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAllSkills() const;

    // 獲取技能
    UFUNCTION(BlueprintPure, Category = "Skill System")
    FMingSkill GetSkill(const FName& SkillID) const;

    // 解鎖技能
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UnlockSkill(const FName& SkillID);

    // 升級技能
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UpgradeSkill(const FName& SkillID, int32 ExperienceAmount = 0);

    // 添加技能經驗
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void AddSkillExperience(const FName& SkillID, int32 ExperienceAmount);

    // 檢查技能是否可以解鎖
    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUnlockSkill(const FName& SkillID, const FMingCharacterData& CharacterData) const;

    // 檢查技能是否可以升級
    UFUNCTION(BlueprintPure, Category = "Skill System")
    bool CanUpgradeSkill(const FName& SkillID) const;

    // 獲取技能樹
    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FSkillTree> GetSkillTrees() const;

    // 選擇專精路徑
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ChooseSpecializationPath(ESpecializationPath Path);

    // 獲取當前專精路徑
    UFUNCTION(BlueprintPure, Category = "Skill System")
    ESpecializationPath GetCurrentSpecializationPath() const;

    // 計算技能效果
    UFUNCTION(BlueprintPure, Category = "Skill System")
    float CalculateSkillEffect(const FName& SkillID, const FString& EffectType) const;

    // 獲取已解鎖技能
    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetUnlockedSkills() const;

    // 獲取可解鎖技能
    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FMingSkill> GetAvailableSkills(const FMingCharacterData& CharacterData) const;

    // 重置技能系統
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void ResetSkillSystem();

    // 保存技能數據
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool SaveSkillData(const FString& SaveSlotName);

    // 載入技能數據
    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool LoadSkillData(const FString& SaveSlotName);

protected:
    // 所有技能數據
    UPROPERTY()
    TArray<FMingSkill> AllSkills;

    // 技能ID到技能的映射
    UPROPERTY()
    TMap<FName, FMingSkill> SkillMap;

    // 技能樹數據
    UPROPERTY()
    TArray<FSkillTree> SkillTrees;

    // 當前專精路徑
    UPROPERTY()
    ESpecializationPath CurrentSpecializationPath;

    // 可用技能點數
    UPROPERTY()
    int32 AvailableSkillPoints;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化技能庫
    void InitializeSkillLibrary();

    // 創建戰鬥技能
    void CreateCombatSkills();

    // 創建策略技能
    void CreateStrategySkills();

    // 創建外交技能
    void CreateDiplomacySkills();

    // 創建領導技能
    void CreateLeadershipSkills();

    // 創建情報技能
    void CreateIntelligenceSkills();

    // 創建經濟技能
    void CreateEconomicSkills();

    // 初始化技能樹
    void InitializeSkillTrees();

    // 驗證技能需求
    bool ValidateSkillRequirements(const FSkillRequirement& Requirement, const FMingCharacterData& CharacterData) const;

    // 計算技能經驗需求
    int32 CalculateExperienceRequirement(int32 SkillLevel) const;

    // 應用技能效果
    void ApplySkillEffects(const FName& SkillID);

    // 解鎖連接技能
    void UnlockConnectedSkills(const FName& SkillID);

    // 獲取技能路徑顏色
    FLinearColor GetSpecializationPathColor(ESpecializationPath Path) const;

    // 生成技能ID
    FName GenerateSkillID(const FString& BaseName, ESkillCategory Category) const;
};
