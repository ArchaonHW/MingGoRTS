#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MingGoRTSCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterBackground : uint8
{
    MilitaryAcademy     UMETA(DisplayName = "è»æ ¡?ºèº«"),
    WarlordSon         UMETA(DisplayName = "è»é–¥ä¹‹å?"),
    Revolutionary      UMETA(DisplayName = "?©å‘½å¿—å£«"),
    ScholarOfficial    UMETA(DisplayName = "å­¸å?è½‰ä»»"),
    Merchant           UMETA(DisplayName = "?†è?å¾è?"),
    CommonSoldier      UMETA(DisplayName = "?®é€šå£«??)
};

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Leadership;      // çµ±å¸¥ï¼šå½±?¿éƒ¨?Šæ??®æ???
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Intelligence;    // ?ºè?ï¼šå½±?¿ç??¥å?å¤–äº¤

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Courage;         // ?‡æ­¦ï¼šå½±?¿å€‹äºº?°é¬¥?Œå£«æ°?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Charisma;        // é­…å?ï¼šå½±?¿èªª?å??›å?

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Constitution;    // é«”è³ªï¼šå½±?¿å¥åº·å??å?

    FCharacterAttributes()
    {
        Leadership = 50.0f;
        Intelligence = 50.0f;
        Courage = 50.0f;
        Charisma = 50.0f;
        Constitution = 50.0f;
    }
};

USTRUCT(BlueprintType)
struct FCharacterSkill
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FName SkillID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString SkillName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
    int32 MaxLevel;

    FCharacterSkill()
    {
        SkillID = NAME_None;
        SkillName = TEXT("");
        Description = TEXT("");
        Level = 1;
        MaxLevel = 5;
    }
};

UCLASS()
class MINGGORTS_API AMingGoRTSCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMingGoRTSCharacter();

protected:
    virtual void BeginPlay() override;

public:
    // è§’è‰²?ºæœ¬ä¿¡æ¯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    ECharacterBackground Background;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    int32 Age;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FString Biography;

    // è§’è‰²å±¬æ€?    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    FCharacterAttributes Attributes;

    // è§’è‰²?€??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    TArray<FCharacterSkill> Skills;

    // è§’è‰²ç¶“é??Œç?ç´?    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    int32 Experience;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    int32 Level;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float Reputation;      // ?²æ???
    // ?å??–è???    UFUNCTION(BlueprintCallable, Category = "Character")
    void InitializeCharacter(const FString& Name, ECharacterBackground CharBackground, const FCharacterAttributes& InitialAttributes);

    // ?†é?å±¬æ€§é?
    UFUNCTION(BlueprintCallable, Category = "Character")
    void AllocateAttributePoints(float LeadershipDelta, float IntelligenceDelta, float CourageDelta, float CharismaDelta, float ConstitutionDelta);

    // æ·»å??€??    UFUNCTION(BlueprintCallable, Category = "Character")
    void AddSkill(const FCharacterSkill& NewSkill);

    // ?‡ç??€??    UFUNCTION(BlueprintCallable, Category = "Character")
    bool UpgradeSkill(const FName& SkillID);

    // ?²å?ç¶“é?
    UFUNCTION(BlueprintCallable, Category = "Character")
    void AddExperience(int32 ExpAmount);

    // è¨ˆç?ç­‰ç?
    UFUNCTION(BlueprintCallable, Category = "Character")
    void CalculateLevel();

    // ?²å?å±¬æ€§ä¿®æ­?€?    UFUNCTION(BlueprintPure, Category = "Character")
    float GetAttributeModifier(ECharacterBackground InBackground) const;

    // ä¿å?è§’è‰²?¸æ?
    UFUNCTION(BlueprintCallable, Category = "Character")
    void SaveCharacterData();

    // è¼‰å…¥è§’è‰²?¸æ?
    UFUNCTION(BlueprintCallable, Category = "Character")
    bool LoadCharacterData(const FString& SaveSlotName);

protected:
    // å±¬æ€§é?ç¸½æ•¸
    UPROPERTY()
    int32 AvailableAttributePoints;

    // ?€å¤§ç?ç´?    UPROPERTY()
    int32 MaxLevel;

    // æ¯ç??€?€ç¶“é?
    UPROPERTY()
    int32 ExperiencePerLevel;

    // ?§éƒ¨?½æ•¸
    void OnLevelUp();
    void ApplyBackgroundBonuses();
};
