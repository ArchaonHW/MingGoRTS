#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSSkillSystem.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillManager.generated.h"

UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
    Unlock, UMETA(DisplayName = "Ëß???Ä??),
    Upgrade, UMETA(DisplayName = "?áÁ??Ä??),
    Use, UMETA(DisplayName = "‰ΩøÁî®?Ä??),
    Reset, UMETA(DisplayName = "?çÁΩÆ?Ä??)
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

    // ?Ä?ΩÁÆ°?ÜÂô®‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Skill Manager")
    FOnSkillExperienceGained OnSkillExperienceGained;

    UPROPERTY(BlueprintAssignable, Category = "Skill Manager")
    FOnSkillPointsChanged OnSkillPointsChanged;

    // ?ùÂ??ñÊ??ΩÁÆ°?ÜÂô®
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void InitializeSkillManager(UMingGoRTSSkillSystem* SkillSystem, AMingGoRTSCharacter* Character};

    // ?≤Â??Ä?ΩÁ?È©?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void GrantSkillExperience(const FName& SkillID, int32 Amount, const FString& Reason = TEXT("")};

    // ?≤Â??Ä?ΩÈ?
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void GrantSkillPoints(int32 Points, const FString& Reason = TEXT("")};

    // ?ïÁ??∞È¨•ÁµêÊ?ÔºåÂ??çÁ?È©?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessCombatEnd(bool bVictory, int32 EnemyCount, float CombatDuration};

    // ?ïÁ?‰ªªÂ?ÂÆåÊ?ÔºåÂ??çÁ?È©?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessQuestCompletion(const FString& QuestID, int32 QuestDifficulty};

    // ?ïÁ?Ê≠∑Âè≤‰∫ã‰ª∂ÔºåÂ??çÁõ∏?úÊ??ΩÁ?È©?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessHistoricalEvent(const FString& EventID, const TArray<FName>& RelatedSkills};

    // ?™Â??ÜÈ??Ä?ΩÁ?È©?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void AutoDistributeExperience(int32 TotalExperience, ESkillCategory PrimaryCategory};

    // Ë®àÁ?ËßíËâ≤Á≠âÁ??éÂãµ
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void ProcessCharacterLevelUp(int32 NewLevel};

    // È©óË??Ä?ΩÊ?‰Ω?    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    bool ValidateSkillAction(const FName& SkillID, ESkillActionType ActionType};

    // ?∑Ë??Ä?ΩÊ?‰Ω?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool ExecuteSkillAction(const FName& SkillID, ESkillActionType ActionType};

    // ?≤Â??Ä?ΩÁ?È©óÊ≠∑??    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    TArray<FSkillExperienceGain> GetSkillExperienceHistory(const FName& SkillID) const;

    // ?≤Â??®Ëñ¶?Ä??    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    TArray<FName> GetRecommendedSkills() const;

    // Ë®àÁ??Ä?ΩÁôºÂ±ïÂª∫Ë≠?    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    FString GetSkillDevelopmentAdvice() const;

    // ?çÁΩÆ?Ä?ΩÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool ResetSkillSystem(};

    // ‰øùÂ??Ä?ΩÁÆ°?ÜÂô®?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool SaveSkillManagerData(const FString& SaveSlotName};

    // ËºâÂÖ•?Ä?ΩÁÆ°?ÜÂô®?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool LoadSkillManagerData(const FString& SaveSlotName};

protected:
    // ?Ä?ΩÁ≥ªÁµ±Â???    UPROPERTY()
    TObjectPtr<UMingGoRTSSkillSystem> SkillSystem;

    // ËßíËâ≤ÂºïÁî®
    UPROPERTY()
    TObjectPtr<AMingGoRTSCharacter> Character;

    // ?Ä?ΩÁ?È©óÊ≠∑??    UPROPERTY()
    TArray<FSkillExperienceGain> SkillExperienceHistory;

    // ?ØÁî®?Ä?ΩÈ?
    UPROPERTY()
    int32 AvailableSkillPoints;

    // Á∂ìÈ??≤Â??çÁ?
    UPROPERTY()
    float ExperienceGainMultiplier;

    // ?Ä?ΩÈ??≤Â??çÁ?
    UPROPERTY()
    float SkillPointGainMultiplier;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ßÈÉ®?πÊ?
    void AddExperienceToHistory(const FName& SkillID, int32 Amount, const FString& Reason};
    void CalculateCombatExperience(bool bVictory, int32 EnemyCount, float CombatDuration};
    void CalculateQuestExperience(const FString& QuestID, int32 QuestDifficulty};
    void CalculateEventExperience(const FString& EventID, const TArray<FName>& RelatedSkills};
    void DistributeExperienceToSkills(const TMap<FName, int32>& SkillExperienceMap};
    void UpdateSkillPoints(int32 OldPoints, int32 NewPoints};
    void ApplySkillEffectsToCharacter(};
    void CheckSkillMilestones(};

    // Á∂ìÈ?Ë®àÁ?ËºîÂä©?πÊ?
    int32 CalculateBaseExperience(int32 Difficulty, float DurationMultiplier = 1.0f) const;
    float GetCategoryExperienceMultiplier(ESkillCategory Category) const;
    int32 CalculateSkillPointReward(int32 ExperienceGained) const;

    // ?Ä?ΩÁôºÂ±ïÂ???    TArray<ESkillCategory> GetCharacterSkillFocus() const;
    TArray<FName> GetUnderdevelopedSkills() const;
    TArray<FName> GetUpgradeableSkills() const;
};

