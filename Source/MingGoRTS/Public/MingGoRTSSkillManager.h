#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSSkillSystem.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillManager.generated.h"

UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
    Unlock, UMETA(DisplayName = "Unlock"),
    Upgrade, UMETA(DisplayName = "Upgrade"),
    Use, UMETA(DisplayName = "Use"),
    Reset, UMETA(DisplayName = "Reset")
};

USTRUCT(BlueprintType)
struct FSkillExperienceGain
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    FString SkillName;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    int32 ExperienceGained;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    float ExperienceMultiplier;

    FSkillExperienceGain()
        : SkillName(TEXT(""))
        , ExperienceGained(0)
        , ExperienceMultiplier(1.0f)
    {}
};

/**
 * MingGoRTS Skill Manager
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSSkillManager : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSSkillManager();

    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    void InitializeSkillManager();

    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool PerformSkillAction(const FString& CharacterName, const FString& SkillName, ESkillActionType Action);

    UFUNCTION(BlueprintPure, Category = "Skill Manager")
    TArray<FSkillData> GetCharacterSkills(const FString& CharacterName);

    UFUNCTION(BlueprintCallable, Category = "Skill Manager")
    bool AddSkillExperience(const FString& CharacterName, const FString& SkillName, int32 Experience);

protected:
    UPROPERTY()
    TMap<FString, TArray<FSkillData>> CharacterSkills;

    void InitializeDefaultSkills();
};
