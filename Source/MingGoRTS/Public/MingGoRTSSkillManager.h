#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSSkillSystem.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillManager.generated.h"

UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
    Unlock         UMETA(DisplayName = "解鎖技能"),
    Upgrade        UMETA(DisplayName = "升級技能"),
    Use            UMETA(DisplayName = "使用技能"),
    Reset          UMETA(DisplayName = "重置技能")
};

USTRUCT(BlueprintType)
struct FSkillExperienceGain
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Experience")
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Experience")
    int32 ExperienceAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Experience")
    FString GainReason;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Experience")
    FDateTime GainTime;

    FSkillExperienceGain()
    {
        SkillID = NAME_None;
        ExperienceAmount = 0;
        GainReason = TEXT("");
        GainTime = FDateTime::Now();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillExperienceGained, const FName&, SkillID, int32, ExperienceAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillPointsChanged, int32, OldPoints, int32, NewPoints);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSSkillManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSSkillManager();

    // 技能管理器事件
    UPROPERTY(BlueprintAssignable, Category = "Skill Manager")
    FOnSkillExperienceGained OnSkillExperienceGained;

    UPROPERTY(BlueprintAssignable, Category = "Skill Manager")
    FOnSkillPointsChanged OnSkillPointsChanged;

    // 初始化技能管理器
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void InitializeSkillManager(UMingGoRTSSkillSystem* SkillSystem, AMingGoRTSCharacter* Character);

    // 獲得技能經驗
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void GrantSkillExperience(const FName& SkillID, int32 Amount, const FString& Reason = TEXT(""));

    // 獲得技能點
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void GrantSkillPoints(int32 Points, const FString& Reason = TEXT(""));

    // 處理戰鬥結束，分配經驗
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessCombatEnd(bool bVictory, int32 EnemyCount, float CombatDuration);

    // 處理任務完成，分配經驗
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessQuestCompletion(const FString& QuestID, int32 QuestDifficulty);

    // 處理歷史事件，分配相關技能經驗
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessHistoricalEvent(const FString& EventID, const TArray<FName>& RelatedSkills);

    // 自動分配技能經驗
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void AutoDistributeExperience(int32 TotalExperience, ESkillCategory PrimaryCategory);

    // 計算角色等級獎勵
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessCharacterLevelUp(int32 NewLevel);

    // 驗證技能操作
    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    bool ValidateSkillAction(const FName& SkillID, ESkillActionType ActionType);

    // 執行技能操作
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool ExecuteSkillAction(const FName& SkillID, ESkillActionType ActionType);

    // 獲取技能經驗歷史
    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    TArray<FSkillExperienceGain> GetSkillExperienceHistory(const FName& SkillID) const;

    // 獲取推薦技能
    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    TArray<FName> GetRecommendedSkills() const;

    // 計算技能發展建議
    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    FString GetSkillDevelopmentAdvice() const;

    // 重置技能系統
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool ResetSkillSystem();

    // 保存技能管理器數據
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool SaveSkillManagerData(const FString& SaveSlotName);

    // 載入技能管理器數據
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool LoadSkillManagerData(const FString& SaveSlotName);

protected:
    // 技能系統引用
    UPROPERTY()
    TObjectPtr<UMingGoRTSSkillSystem> SkillSystem;

    // 角色引用
    UPROPERTY()
    TObjectPtr<AMingGoRTSCharacter> Character;

    // 技能經驗歷史
    UPROPERTY()
    TArray<FSkillExperienceGain> SkillExperienceHistory;

    // 可用技能點
    UPROPERTY()
    int32 AvailableSkillPoints;

    // 經驗獲得倍率
    UPROPERTY()
    float ExperienceGainMultiplier;

    // 技能點獲得倍率
    UPROPERTY()
    float SkillPointGainMultiplier;

    // 是否已初始化
    bool bIsInitialized;

    // 內部方法
    void AddExperienceToHistory(const FName& SkillID, int32 Amount, const FString& Reason);
    void CalculateCombatExperience(bool bVictory, int32 EnemyCount, float CombatDuration);
    void CalculateQuestExperience(const FString& QuestID, int32 QuestDifficulty);
    void CalculateEventExperience(const FString& EventID, const TArray<FName>& RelatedSkills);
    void DistributeExperienceToSkills(const TMap<FName, int32>& SkillExperienceMap);
    void UpdateSkillPoints(int32 OldPoints, int32 NewPoints);
    void ApplySkillEffectsToCharacter();
    void CheckSkillMilestones();

    // 經驗計算輔助方法
    int32 CalculateBaseExperience(int32 Difficulty, float DurationMultiplier = 1.0f) const;
    float GetCategoryExperienceMultiplier(ESkillCategory Category) const;
    int32 CalculateSkillPointReward(int32 ExperienceGained) const;

    // 技能發展分析
    TArray<ESkillCategory> GetCharacterSkillFocus() const;
    TArray<FName> GetUnderdevelopedSkills() const;
    TArray<FName> GetUpgradeableSkills() const;
};
