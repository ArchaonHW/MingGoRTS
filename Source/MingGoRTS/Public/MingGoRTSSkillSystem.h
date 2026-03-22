#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSSkillSystem.generated.h"

UENUM(BlueprintType)
enum class ESkillCategory : uint8
{
    Combat, UMETA(DisplayName = "Combat"),
    Strategy, UMETA(DisplayName = "Strategy"),
    Diplomacy, UMETA(DisplayName = "Diplomacy"),
    Leadership, UMETA(DisplayName = "Leadership"),
    Intelligence, UMETA(DisplayName = "Intelligence"),
    Economic, UMETA(DisplayName = "Economic")
};

UENUM(BlueprintType)
enum class ESkillType : uint8
{
    Active, UMETA(DisplayName = "Active"),
    Passive, UMETA(DisplayName = "Passive"),
    Toggle, UMETA(DisplayName = "Toggle"),
    Ultimate, UMETA(DisplayName = "Ultimate")
};

USTRUCT(BlueprintType)
struct FSkillData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    FString SkillName;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    ESkillCategory Category;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    ESkillType Type;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    int32 Level;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    int32 Experience;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    int32 MaxExperience;

    UPROPERTY(BlueprintReadOnly, Category = "Skill")
    bool IsUnlocked;

    FSkillData()
        : SkillName(TEXT(""))
        , Category(ESkillCategory::Combat)
        , Type(ESkillType::Active)
        , Level(1)
        , Experience(0)
        , MaxExperience(100)
        , IsUnlocked(false)
    {}
};

/**
 * MingGoRTS Skill System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSSkillSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSSkillSystem();

    UFUNCTION(BlueprintCallable, Category = "Skill System")
    void InitializeSkillSystem();

    UFUNCTION(BlueprintPure, Category = "Skill System")
    TArray<FSkillData> GetAllSkills();

    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UnlockSkill(const FString& SkillName);

    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UpgradeSkill(const FString& SkillName);

    UFUNCTION(BlueprintCallable, Category = "Skill System")
    bool UseSkill(const FString& SkillName);

protected:
    UPROPERTY()
    TArray<FSkillData> AvailableSkills;

    void InitializeDefaultSkills();
};
