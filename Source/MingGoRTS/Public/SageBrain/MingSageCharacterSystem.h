#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MingSageCharacterSystem.generated.h"

UENUM(BlueprintType)
enum class ESageCharacterType : uint8
{
    None            UMETA(DisplayName = "無"),
    Sage            UMETA(DisplayName = "聖者"),
    DemonKing       UMETA(DisplayName = "魔王"),
    FalseSage       UMETA(DisplayName = "偽聖者")
};

USTRUCT(BlueprintType)
struct FSageCharacterTraits
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ESageCharacterType CharacterType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoralAuthority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StrategicThinking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryCommand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CharismaBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RiskTolerance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxFollowers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SpecialAbilities;

    FSageCharacterTraits()
    {
        CharacterType = ESageCharacterType::None;
        MoralAuthority = 50.0f;
        StrategicThinking = 50.0f;
        MilitaryCommand = 50.0f;
        CharismaBonus = 0.0f;
        LearningSpeed = 1.0f;
        DecisionSpeed = 1.0f;
        RiskTolerance = 0.5f;
        MaxFollowers = 100;
    }
};

USTRUCT(BlueprintType)
struct FSageCharacterProgression
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Experience;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceToNext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockedAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> SkillLevels;

    FSageCharacterProgression()
    {
        CurrentLevel = 1;
        Experience = 0.0f;
        ExperienceToNext = 100.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterTypeChanged, ESageCharacterType, OldType, ESageCharacterType, NewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelUp, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityUnlocked, const FString&, AbilityName);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageCharacterSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSageCharacterSystem();

    UFUNCTION(BlueprintCallable, Category = "Sage Character")
    void InitializeCharacter(ESageCharacterType CharacterType);

    UFUNCTION(BlueprintCallable, Category = "Sage Character")
    void SetCharacterType(ESageCharacterType NewType);

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    ESageCharacterType GetCharacterType() const { return CurrentCharacterType; }

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    FSageCharacterTraits GetCharacterTraits() const { return CurrentTraits; }

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    FSageCharacterProgression GetProgression() const { return CharacterProgression; }

    UFUNCTION(BlueprintCallable, Category = "Sage Character")
    void AddExperience(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Sage Character")
    bool UnlockAbility(const FString& AbilityName);

    UFUNCTION(BlueprintCallable, Category = "Sage Character")
    void UpgradeSkill(const FString& SkillName, float Amount);

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    bool HasAbility(const FString& AbilityName) const;

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    float GetSkillLevel(const FString& SkillName) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Character")
    void ApplyCharacterModifiers();

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    TArray<FString> GetAvailableAbilities() const;

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    bool CanEvolveToNextStage() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Character")
    void EvolveCharacter();

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    FString GetCharacterDescription() const;

    UFUNCTION(BlueprintPure, Category = "Sage Character")
    FString GetCharacterTitle() const;

    UPROPERTY(BlueprintAssignable, Category = "Sage Character")
    FOnCharacterTypeChanged OnCharacterTypeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Sage Character")
    FOnCharacterLevelUp OnCharacterLevelUp;

    UPROPERTY(BlueprintAssignable, Category = "Sage Character")
    FOnAbilityUnlocked OnAbilityUnlocked;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Character")
    void OnCharacterInitialized(ESageCharacterType CharacterType);

    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Character")
    void OnTraitsModified(const FSageCharacterTraits& NewTraits);

    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Character")
    void OnAbilityLearned(const FString& AbilityName);

private:
    void ApplySageTraits();
    void ApplyDemonKingTraits();
    void ApplyFalseSageTraits();
    void CalculateExperienceToNext();
    void CheckForLevelUp();
    void InitializeDefaultSkills();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Character")
    ESageCharacterType CurrentCharacterType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Character")
    FSageCharacterTraits CurrentTraits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Character")
    FSageCharacterProgression CharacterProgression;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Character")
    bool bIsInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Character")
    float ExperienceMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Character")
    int32 MaxCharacterLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sage Character")
    TMap<ESageCharacterType, FSageCharacterTraits> DefaultTraitsMap;
};
