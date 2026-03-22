#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.generated.h"

// ����X�~��X
UENUM(BlueprintType)
enum class EMingCharacterClass : uint8
{
    Infantry,          // �B�L�xX
    Cavalry,           // �M�L�xX
    Artillery,         // X�L�xX
    Engineer,         // �u�{�v
    Medic,           // �����L
    Scout,           // ����L
    Commander,       // �����x
    Diplomat         // �~��x
};

// XUENUM(BlueprintType)
enum class EMingSkillType : uint8
{
    Combat,           // �԰��ޯ�
    Leadership,      // �����ޯ�
    Strategy,         // �Բ��ޯ�
    Diplomacy,        // �~��ޯ�
    Engineering,      // �u�{�ޯ�
    Medical,          // �����ޯ�
    Scouting,         // ����ޯ�
    Survival          // �ͦs�ޯ�
};

// �gX��X
UENUM(BlueprintType)
enum class EMingExperienceSource : uint8
{
    Combat,            // X���gX
    Mission,           // ��X�gX
    Exploration,       // X���gX
    Diplomacy,         // �~��gX
    Training,          // �V�m�gX
    Achievement,       // X�N�gX
    Story              // X�gX
};

// ������XUSTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterAttributes
{
    GENERATED_BODY()

    // X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Strength;          // X

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Agility;           // X��

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intelligence;       // X

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Charisma;          // �yX

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Constitution;      // ���

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Wisdom;            // X�z

    // X����X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatSkill;        // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;         // X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TacticalSense;      // Xı

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Marksmanship;       // �gX���

    // ������X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Diplomacy;         // �~��X

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Negotiation;        // �ͧPX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intimidation;       // ��X

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Persuasion;        // ��X
    FMingCharacterAttributes()
    {
        Strength = 50.0f;
        Agility = 50.0f;
        Intelligence = 50.0f;
        Charisma = 50.0f;
        Constitution = 50.0f;
        Wisdom = 50.0f;
        CombatSkill = 50.0f;
        Leadership = 50.0f;
        TacticalSense = 50.0f;
        Marksmanship = 50.0f;
        Diplomacy = 50.0f;
        Negotiation = 50.0f;
        Intimidation = 50.0f;
        Persuasion = 50.0f;
    }
};

// X��XUSTRUCT(BlueprintType)
struct MINGCORE_API FMingSkillData
{
    GENERATED_BODY()

    // XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillID;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillName;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillDescription;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSkillType SkillType;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    // X�jX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxLevel;

    // X�gX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentExperience;

    // X�gX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceToNextLevel;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> SkillEffects;

    // X�mX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> PrerequisiteSkills;

    // X����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // X�и�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillIconPath;

    // X�_X�DX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActiveSkill;

    // X�oX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownTime;

    // ����X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ResourceCost;

    FMingSkillData()
    {
        SkillID = -1;
        SkillName = TEXT(""};
        SkillDescription = TEXT(""};
        SkillType = EMingSkillType::Combat;
        CurrentLevel = 1;
        MaxLevel = 10;
        CurrentExperience = 0.0f;
        ExperienceToNextLevel = 100.0f;
        SkillEffects.Empty(};
        PrerequisiteSkills.Empty(};
        UnlockConditions.Empty(};
        SkillIconPath = TEXT(""};
        bIsActiveSkill = false;
        CooldownTime = 0.0f;
        ResourceCost.Empty(};
    }
};

// ���ⵥX
USTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterLevel
{
    GENERATED_BODY()

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    // X�gX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentExperience;

    // X�gX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceToNextLevel;

    // �`X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalExperience;

    // ��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelTitle;

    // ��X�vX
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelPermissions;

    // ��X�y
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelRewards;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillPoints;

    // �ݩ�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttributePoints;

    FMingCharacterLevel()
    {
        CurrentLevel = 1;
        CurrentExperience = 0.0f;
        ExperienceToNextLevel = 100.0f;
        TotalExperience = 0.0f;
        LevelTitle = TEXT("X�L"};
        LevelPermissions.Empty(};
        LevelRewards.Empty(};
        SkillPoints = 0;
        AttributePoints = 0;
    }
};

// ��X
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEquipmentItem
{
    GENERATED_BODY()

    // XID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    // X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    // X�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemDescription;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemType;

    // X�}X��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemRarity;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    // X�ݩ�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ItemAttributes;

    // ��X��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipSlot;

    // X�_�wX    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemQuantity;

    // X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemDurability;

    // X�j�@X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // X3D��X��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    FMingEquipmentItem()
    {
        ItemID = -1;
        ItemName = TEXT(""};
        ItemDescription = TEXT(""};
        ItemType = TEXT(""};
        ItemRarity = TEXT("���q"};
        ItemLevel = 1;
        ItemAttributes.Empty(};
        EquipSlot = TEXT(""};
        bIsEquipped = false;
        ItemQuantity = 1;
        ItemDurability = 100.0f;
        MaxDurability = 100.0f;
        ItemIconPath = TEXT(""};
        ItemModelPath = TEXT(""};
    }
};

// ����X���ƥ�eX
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterLevelUp, int32, NewLevel, const FMingCharacterAttributes&, NewAttributes};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelUp, int32, SkillID, int32, NewLevel};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceGained, EMingExperienceSource, Source, float, ExperienceAmount};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, int32, ItemID, const FString&, EquipSlot};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, const FString&, AttributeName, float, NewValue};

/**
 * ����X���t��
 * �t�d��X���ⵥX�BX��X��X��
 */
UCLASS(ClassGroup = (Character), Blueprintable, BlueprintType)
class MINGCORE_API UMingCharacterGrowthSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCharacterGrowthSystem(};

    // X�tX    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool InitializeGrowthSystem(};

    // X��X    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool CreateCharacter(const FString& CharacterName, EMingCharacterClass CharacterClass};

    // X�gX
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool GainExperience(EMingExperienceSource Source, float ExperienceAmount};

    // X����
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LevelUpCharacter(};

    // X�ݩ�X
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateAttributePoint(const FString& AttributeName, int32 Points};

    // X
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateSkillPoint(int32 SkillID};

    // ��X    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LearnSkill(int32 SkillID};

    // X    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool UpgradeSkill(int32 SkillID};

    // X����H��
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterAttributes GetCharacterAttributes() const;

    // X���ⵥX
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterLevel GetCharacterLevel() const;

    // X    UFUNCTION(BlueprintPure, Category = "Character Growth")
    TArray<FMingSkillData> GetCharacterSkills() const;

    // X    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingSkillData GetSkill(int32 SkillID) const;

    // ��X
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool EquipItem(int32 ItemID, const FString& EquipSlot) {};

    // X
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool UnequipItem(const FString& EquipSlot) {};

    // X��X
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    TArray<FMingEquipmentItem> GetEquippedItems() const;

    // �pX��X    UFUNCTION(BlueprintPure, Category = "Character Growth")
    float CalculateCombatPower() const;

    // �pX�`X��X
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    float CalculateOverallPower() const;

    // �OX����X
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool SaveCharacterData(};

    // ���J����X
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LoadCharacterData(};

    // X�m����
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    void ResetCharacter(};

    // �ƥ�eX
    UPROPERTY(BlueprintAssignable)
    FOnCharacterLevelUp OnCharacterLevelUp;

    UPROPERTY(BlueprintAssignable)
    FOnSkillLevelUp OnSkillLevelUp;

    UPROPERTY(BlueprintAssignable)
    FOnExperienceGained OnExperienceGained;

    UPROPERTY(BlueprintAssignable)
    FOnItemEquipped OnItemEquipped;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChanged OnAttributeChanged;

protected:
    // ������X    UPROPERTY()
    FMingCharacterAttributes CharacterAttributes;

    // ���ⵥX
    UPROPERTY()
    FMingCharacterLevel CharacterLevel;

    // X��X�w
    UPROPERTY()
    TMap<int32, FMingSkillData> SkillDatabase;

    // ��X    UPROPERTY()
    TMap<FString, FMingEquipmentItem> EquippedItems;

    // X
    UPROPERTY()
    TArray<FMingEquipmentItem> InventoryItems;

    // ����X�~
    UPROPERTY()
    EMingCharacterClass CharacterClass;

    // ����X��
    UPROPERTY()
    FString CharacterName;

    // X�_�wX�lX
    UPROPERTY()
    bool bInitialized;

private:
    // ���JX�]X��X    void LoadDefaultSkills(};

    // ���JX�]��X
    void LoadDefaultEquipment(};

    // �pX�gX
    float CalculateExperienceToNextLevel(int32 CurrentLevel) const;

    // �pX��X�gX
    float CalculateSkillExperienceToNextLevel(int32 CurrentLevel) const;

    // X�ε�X�y
    void ApplyLevelRewards(int32 NewLevel};

    // X��X    void ApplySkillEffects(int32 SkillID, int32 NewLevel};

    // X�θ�X
    void ApplyEquipmentEffects(const FMingEquipmentItem& Item) {};

    // ������X
    void RemoveEquipmentEffects(const FMingEquipmentItem& Item) {};

    // ��X�ǲ�X    bool CanLearnSkill(int32 SkillID) const;

    // �pX�ݩʼvX    float CalculateAttributeModifier(const FString& AttributeName) const;

    // X��X
    FMingEquipmentItem GenerateRandomEquipment(int32 ItemLevel) const;

    // X�s����X��X    void UpdateCharacterPower(};

    // �OX����ַ�
    void SaveCharacterSnapshot(};

    // ���J����ַ�
    void LoadCharacterSnapshot(};

    // ��X����X
    bool ValidateCharacterData() const;

    // �pX��
    TArray<int32> CalculateSkillTree() const;

    // X    void UnlockNewSkills(int32 NewLevel};

    // �pX�~X
    TMap<FString, float> CalculateClassBonuses() const;
};

