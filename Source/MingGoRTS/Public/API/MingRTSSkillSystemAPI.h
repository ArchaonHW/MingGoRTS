#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingRTSSkillSystemAPI.generated.h"

/**
 * 技能系統API
 * 專門負責角色技能和專精化系統功能 (從PythonAPI分配而來)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSkillSystemAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSSkillSystemAPI();

    // 初始化技能系統
    UFUNCTION(BlueprintCallable, Category = "Skill System API")
    bool InitializeSkillSystem();

    // 技能管理功能
    UFUNCTION(BlueprintCallable, Category = "Skill System|Management")
    bool UnlockSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Management")
    bool LockSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Management")
    bool AddSkillExperience(const FString& SkillID, int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Management")
    bool UpgradeSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Management")
    bool ResetSkill(const FString& SkillID);

    // 技能查詢功能
    UFUNCTION(BlueprintCallable, Category = "Skill System|Query")
    TArray<FSkillData> GetAllSkills();

    UFUNCTION(BlueprintCallable, Category = "Skill System|Query")
    FSkillData GetSkillData(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Query")
    TArray<FSkillData> GetUnlockedSkills();

    UFUNCTION(BlueprintCallable, Category = "Skill System|Query")
    TArray<FSkillData> GetSkillsByCategory(ESkillCategory Category);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Query")
    TArray<FSkillData> GetAvailableSkills(const FString& CharacterID);

    // 專精化系統
    UFUNCTION(BlueprintCallable, Category = "Skill System|Specialization")
    bool ChooseSpecializationPath(const FString& Path);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Specialization")
    bool ChangeSpecializationPath(const FString& NewPath);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Specialization")
    ESpecializationPath GetCurrentSpecialization() const;

    UFUNCTION(BlueprintCallable, Category = "Skill System|Specialization")
    TArray<ESpecializationPath> GetAvailableSpecializations();

    UFUNCTION(BlueprintCallable, Category = "Skill System|Specialization")
    TMap<ESpecializationPath, TArray<FString>> GetSpecializationRequirements();

    // 技能效果計算
    UFUNCTION(BlueprintCallable, Category = "Skill System|Effects")
    float CalculateSkillEffect(const FString& SkillID, const FString& EffectType);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Effects")
    TMap<FString, float> GetAllSkillEffects(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Effects")
    float CalculateTotalEffect(const FString& EffectType);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Effects")
    TArray<FString> GetActiveEffects();

    // 技能樹系統
    UFUNCTION(BlueprintCallable, Category = "Skill System|Skill Tree")
    TArray<FSkillTreeNode> GetSkillTree(const FString& TreeID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Skill Tree")
    TArray<FString> GetSkillPrerequisites(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Skill Tree")
    bool CanUnlockSkill(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Skill Tree")
    TArray<FString> GetNextAvailableSkills();

    UFUNCTION(BlueprintCallable, Category = "Skill System|Skill Tree")
    bool ValidateSkillTreeIntegrity();

    // 技能經驗和進度
    UFUNCTION(BlueprintCallable, Category = "Skill System|Progress")
    int32 GetSkillExperience(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Progress")
    int32 GetSkillLevel(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Progress")
    int32 GetExperienceToNextLevel(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Progress")
    float GetSkillProgress(const FString& SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Progress")
    TArray<FSkillProgress> GetSkillHistory(const FString& SkillID);

    // 技能推薦系統
    UFUNCTION(BlueprintCallable, Category = "Skill System|Recommendations")
    TArray<FString> GetRecommendedSkills(const FString& CharacterID, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Recommendations")
    TArray<FString> GetOptimalSkillPath(const FString& CharacterID, ESpecializationPath Path);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Recommendations")
    float CalculateSkillSynergy(const FString& SkillID1, const FString& SkillID2);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Recommendations")
    TMap<FString, float> GetSkillPriorities(const FString& Context);

    // 技能配置管理
    UFUNCTION(BlueprintCallable, Category = "Skill System|Configuration")
    bool LoadSkillConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Configuration")
    bool SaveSkillConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Skill System|Configuration")
    bool ResetSkillSystem();

    UFUNCTION(BlueprintCallable, Category = "Skill System|Configuration")
    bool ValidateSkillConfiguration();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSkillUnlocked, const FString&, SkillID, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSkillUpgraded, const FString&, SkillID, int32, NewLevel);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnExperienceGained, const FString&, SkillID, int32, Amount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecializationChosen, ESpecializationPath, Path);

    UPROPERTY(BlueprintAssignable, Category = "Skill System|Events")
    FOnSkillUnlocked OnSkillUnlocked;

    UPROPERTY(BlueprintAssignable, Category = "Skill System|Events")
    FOnSkillUpgraded OnSkillUpgraded;

    UPROPERTY(BlueprintAssignable, Category = "Skill System|Events")
    FOnExperienceGained OnExperienceGained;

    UPROPERTY(BlueprintAssignable, Category = "Skill System|Events")
    FOnSpecializationChosen OnSpecializationChosen;

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Skill System|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Skill System|Status")
    FString GetSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Skill System|Status")
    TArray<FString> GetAvailableFunctions() const;

    UFUNCTION(BlueprintCallable, Category = "Skill System|Status")
    TMap<FString, int32> GetSkillStatistics() const;

private:
    bool bIsInitialized;
    
    // 技能數據
    TMap<FString, FSkillData> SkillDatabase;
    
    // 技能經驗數據
    TMap<FString, int32> SkillExperience;
    
    // 已解鎖技能
    TArray<FString> UnlockedSkills;
    
    // 當前專精化
    ESpecializationPath CurrentSpecialization;
    
    // 技能樹數據
    TMap<FString, TArray<FSkillTreeNode>> SkillTrees;
    
    // 技能歷史記錄
    TArray<FSkillProgress> SkillHistory;

    // 內部輔助函數
    bool LoadSkillDatabase();
    bool LoadSkillTrees();
    bool LoadSkillConfiguration();
    
    bool ValidateSkillID(const FString& SkillID) const;
    bool CanAddExperience(const FString& SkillID, int32 Amount) const;
    void ProcessSkillLevelUp(const FString& SkillID);
    
    int32 CalculateExperienceRequirement(int32 Level) const;
    float CalculateSkillEffectValue(const FString& SkillID, const FString& EffectType) const;
    
    // 數據持久化
    TSharedPtr<FJsonObject> SaveSkillData() const;
    bool LoadSkillData(TSharedPtr<FJsonObject> SaveData);
};

// 技能類別枚舉
UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat        UMETA(DisplayName = "Combat"),
    Strategy      UMETA(DisplayName = "Strategy"),
    Diplomacy     UMETA(DisplayName = "Diplomacy"),
    Leadership    UMETA(DisplayName = "Leadership"),
    Intelligence  UMETA(DisplayName = "Intelligence"),
    Economic      UMETA(DisplayName = "Economic")
};

// 技能類型枚舉
UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Active   UMETA(DisplayName = "Active"),
    Passive  UMETA(DisplayName = "Passive"),
    Toggle   UMETA(DisplayName = "Toggle")
};

// 專精化路徑枚舉
UENUM(BlueprintType)
enum class ESpecializationPath : uint8
{
    Commander   UMETA(DisplayName = "Commander"),
    Tactician   UMETA(DisplayName = "Tactician"),
    Diplomat    UMETA(DisplayName = "Diplomat"),
    Spy         UMETA(DisplayName = "Spy"),
    Economist   UMETA(DisplayName = "Economist"),
    Generalist  UMETA(DisplayName = "Generalist")
};

// 技能數據結構
USTRUCT(BlueprintType)
struct FSKILL_DATA
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    ESkillType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 MaxLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<FString> Prerequisites;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TMap<FString, float> Effects;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    TArray<ESpecializationPath> RequiredSpecializations;

    FSKILL_DATA()
    {
        SkillID = TEXT("");
        Name = TEXT("");
        Description = TEXT("");
        Category = ESkillCategory::Combat;
        Type = ESkillType::Active;
        MaxLevel = 10;
    }
};

// 技能樹節點結構
USTRUCT(BlueprintType)
struct FSKILL_TREE_NODE
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    FString SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    FVector2D Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    TArray<FString> Connections;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    bool bIsRoot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Tree")
    int32 Tier;

    FSKILL_TREE_NODE()
    {
        SkillID = TEXT("");
        Position = FVector2D::ZeroVector;
        bIsRoot = false;
        Tier = 1;
    }
};

// 技能進度結構
USTRUCT(BlueprintType)
struct FSKILL_PROGRESS
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Progress")
    FString SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Progress")
    int32 ExperienceGained;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Progress")
    FString Source;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Progress")
    FDateTime Timestamp;

    FSKILL_PROGRESS()
    {
        SkillID = TEXT("");
        ExperienceGained = 0;
        Source = TEXT("");
    }
};
