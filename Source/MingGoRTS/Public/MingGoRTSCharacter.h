#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MingGoRTSCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterBackground : uint8
{
    MilitaryAcademy     UMETA(DisplayName = "軍校出身"),
    WarlordSon         UMETA(DisplayName = "軍閥之子"),
    Revolutionary      UMETA(DisplayName = "革命志士"),
    ScholarOfficial    UMETA(DisplayName = "學官轉任"),
    Merchant           UMETA(DisplayName = "商賈從軍"),
    CommonSoldier      UMETA(DisplayName = "普通士兵")
};

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Leadership;      // 統帥：影響部隊指揮效率

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Intelligence;    // 智謀：影響策略和外交

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Courage;         // 勇武：影響個人戰鬥和士氣

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Charisma;        // 魅力：影響說服和招募

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
    float Constitution;    // 體質：影響健康和耐力

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
    // 角色基本信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    ECharacterBackground Background;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    int32 Age;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FString Biography;

    // 角色屬性
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    FCharacterAttributes Attributes;

    // 角色技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    TArray<FCharacterSkill> Skills;

    // 角色經驗和等級
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    int32 Experience;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    int32 Level;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
    float Reputation;      // 聲望值

    // 初始化角色
    UFUNCTION(BlueprintCallable, Category = "Character")
    void InitializeCharacter(const FString& Name, ECharacterBackground CharBackground, const FCharacterAttributes& InitialAttributes);

    // 分配屬性點
    UFUNCTION(BlueprintCallable, Category = "Character")
    void AllocateAttributePoints(float LeadershipDelta, float IntelligenceDelta, float CourageDelta, float CharismaDelta, float ConstitutionDelta);

    // 添加技能
    UFUNCTION(BlueprintCallable, Category = "Character")
    void AddSkill(const FCharacterSkill& NewSkill);

    // 升級技能
    UFUNCTION(BlueprintCallable, Category = "Character")
    bool UpgradeSkill(const FName& SkillID);

    // 獲得經驗
    UFUNCTION(BlueprintCallable, Category = "Character")
    void AddExperience(int32 ExpAmount);

    // 計算等級
    UFUNCTION(BlueprintCallable, Category = "Character")
    void CalculateLevel();

    // 獲取屬性修正值
    UFUNCTION(BlueprintPure, Category = "Character")
    float GetAttributeModifier(ECharacterBackground InBackground) const;

    // 保存角色數據
    UFUNCTION(BlueprintCallable, Category = "Character")
    void SaveCharacterData();

    // 載入角色數據
    UFUNCTION(BlueprintCallable, Category = "Character")
    bool LoadCharacterData(const FString& SaveSlotName);

protected:
    // 屬性點總數
    UPROPERTY()
    int32 AvailableAttributePoints;

    // 最大等級
    UPROPERTY()
    int32 MaxLevel;

    // 每級所需經驗
    UPROPERTY()
    int32 ExperiencePerLevel;

    // 內部函數
    void OnLevelUp();
    void ApplyBackgroundBonuses();
};
