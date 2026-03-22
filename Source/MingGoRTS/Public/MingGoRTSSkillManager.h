#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSSkillSystem.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillManager.generated.h"

UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
    Unlock, UMETA(DisplayName = "�X�X),
    Upgrade, UMETA(DisplayName = "?��X�X),
    Use, UMETA(DisplayName = "使用?�X),
    Reset, UMETA(DisplayName = "?�置?�X)
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
        GainReason = TEXT(""};
        GainTime = FDateTime::Now(};
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillExperienceGained, const FName&, SkillID, int32, ExperienceAmount};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillPointsChanged, int32, OldPoints, int32, NewPoints};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSSkillManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSSkillManager(};

    // ?�?�管?�器事件
    UPROPERTY(BlueprintAssignable, Category = "Skill Manager")
    FOnSkillExperienceGained OnSkillExperienceGained;

    UPROPERTY(BlueprintAssignable, Category = "Skill Manager")
    FOnSkillPointsChanged OnSkillPointsChanged;

    // ?��X��X�管?�器
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void InitializeSkillManager(UMingGoRTSSkillSystem* SkillSystem, AMingGoRTSCharacter* Character};

    // ?��X�?��?�?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void GrantSkillExperience(const FName& SkillID, int32 Amount, const FString& Reason = TEXT("")};

    // ?��X�?��?
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void GrantSkillPoints(int32 Points, const FString& Reason = TEXT("")};

    // ?��X�鬥結�?，�X��?�?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessCombatEnd(bool bVictory, int32 EnemyCount, float CombatDuration};

    // ?��?任�?完�?，�X��?�?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessQuestCompletion(const FString& QuestID, int32 QuestDifficulty};

    // ?��?歷史事件，�X�相?��X��?�?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessHistoricalEvent(const FString& EventID, const TArray<FName>& RelatedSkills};

    // ?��X��X�?��?�?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void AutoDistributeExperience(int32 TotalExperience, ESkillCategory PrimaryCategory};

    // 計�?角色等�X�勵
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessCharacterLevelUp(int32 NewLevel};

    // 驗�X�?��?�?    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    bool ValidateSkillAction(const FName& SkillID, ESkillActionType ActionType};

    // ?��X�?��?�?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool ExecuteSkillAction(const FName& SkillID, ESkillActionType ActionType};

    // ?��X�?��?驗歷X    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    TArray<FSkillExperienceGain> GetSkillExperienceHistory(const FName& SkillID) const;

    // ?��X�薦?�X    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    TArray<FName> GetRecommendedSkills() const;

    // 計�X�?�發展建�?    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    FString GetSkillDevelopmentAdvice() const;

    // ?�置?�?�系�?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool ResetSkillSystem(};

    // 保�X�?�管?�器?��?
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool SaveSkillManagerData(const FString& SaveSlotName};

    // 載入?�?�管?�器?��?
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool LoadSkillManagerData(const FString& SaveSlotName};

protected:
    // ?�?�系統�X    UPROPERTY()
    TObjectPtr<UMingGoRTSSkillSystem> SkillSystem;

    // 角色引用
    UPROPERTY()
    TObjectPtr<AMingGoRTSCharacter> Character;

    // ?�?��?驗歷X    UPROPERTY()
    TArray<FSkillExperienceGain> SkillExperienceHistory;

    // ?�用?�?��?
    UPROPERTY()
    int32 AvailableSkillPoints;

    // 經�X��X��?
    UPROPERTY()
    float ExperienceGainMultiplier;

    // ?�?��X��X��?
    UPROPERTY()
    float SkillPointGainMultiplier;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?�部?��?
    void AddExperienceToHistory(const FName& SkillID, int32 Amount, const FString& Reason};
    void CalculateCombatExperience(bool bVictory, int32 EnemyCount, float CombatDuration};
    void CalculateQuestExperience(const FString& QuestID, int32 QuestDifficulty};
    void CalculateEventExperience(const FString& EventID, const TArray<FName>& RelatedSkills};
    void DistributeExperienceToSkills(const TMap<FName, int32>& SkillExperienceMap};
    void UpdateSkillPoints(int32 OldPoints, int32 NewPoints};
    void ApplySkillEffectsToCharacter(};
    void CheckSkillMilestones(};

    // 經�?計�?輔助?��?
    int32 CalculateBaseExperience(int32 Difficulty, float DurationMultiplier = 1.0f) const;
    float GetCategoryExperienceMultiplier(ESkillCategory Category) const;
    int32 CalculateSkillPointReward(int32 ExperienceGained) const;

    // ?�?�發展�X    TArray<ESkillCategory> GetCharacterSkillFocus() const;
    TArray<FName> GetUnderdevelopedSkills() const;
    TArray<FName> GetUpgradeableSkills() const;
};

