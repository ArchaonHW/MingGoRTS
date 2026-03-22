#pragma once

#include "CoreMinimal.h"
#include "UObject/NoEportTypes.h"
#include "MingCharacterGrowthSystem.generated.h"

// ?????~??
UENUM(BlueprintType)
enum class EMingCharacterClass : uint8
{
    Infantry,          // ?B?L?
    Cavalry,           // ?M?L?
    Artillery,         // ?L?
    Engineer,         // ?u?{?v
    Medic,           // ?????L
    Scout,           // ????L
    Commander,       // ?????
    Diplomat         // ?~??
};

// UENUM(BlueprintType)
enum class EMingSkillType : uint8
{
    Combat,           // ??????
    Leadership,      // ???????
    Strategy,         // ??????
    Diplomacy,        // ?~????
    Engineering,      // ?u?{???
    Medical,          // ???????
    Scouting,         // ??????
    Survival          // ??s???
};

// ?g
UENUM(BlueprintType)
enum class EMingEperienceSource : uint8
{
    Combat,            // ???g
    Mission,           // ???g
    Eploration,       // ???g
    Diplomacy,         // ?~??g
    Training,          // ?V?m?g
    Achievement,       // ?N?g
    Story              // ?g
};

// ??????USTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterAttributes
{
    GENERATED_BODY()

    
    float Strength;          // 

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Agility;           // ??

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intelligence;       // 

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Charisma;          // ?y

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Constitution;      // ???

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Wisdom;            // ?z

    
    float CombatSkill;        // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;         // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TacticalSense;      // ?

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Marksmanship;       // ?g???

    
    float Diplomacy;         // ?~??

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Negotiation;        // ??P
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intimidation;       // ??

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Persuasion;        // ??
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

// USTRUCT(BlueprintType)
struct MINGCORE_API FMingSkillData
{
    GENERATED_BODY()

    // ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillID;

    
    FString SkillName;

    
    FString SkillDescription;

    
    EMingSkillType SkillType;

    // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    
    int32 MaLevel;

    // ?g
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentEperience;

    // ?g
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EperienceToNetLevel;

    
    TMap<FString, float> SkillEffects;

    
    TArray<int32> PrerequisiteSkills;

    // ????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    
    FString SkillIconPath;

    
    bool bIsActiveSkill;

    // ?o
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownTime;

    
    TMap<FString, float> ResourceCost;

    FMingSkillData()
    {
        SkillID = -1;
        SkillName = TET(""};
        SkillDescription = TET(""};
        SkillType = EMingSkillType::Combat;
        CurrentLevel = 1;
        MaLevel = 10;
        CurrentEperience = 0.0f;
        EperienceToNetLevel = 100.0f;
        SkillEffects.Empty();
        PrerequisiteSkills.Empty();
        UnlockConditions.Empty();
        SkillIconPath = TET(""};
        bIsActiveSkill = false;
        CooldownTime = 0.0f;
        ResourceCost.Empty();
    }
};

// ????
USTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterLevel
{
    GENERATED_BODY()

    // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    // ?g
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentEperience;

    // ?g
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EperienceToNetLevel;

    
    float TotalEperience;

    // ??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelTitle;

    // ???v
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelPermissions;

    // ???y
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelRewards;

    
    int32 SkillPoints;

    
    int32 AttributePoints;

    FMingCharacterLevel()
    {
        CurrentLevel = 1;
        CurrentEperience = 0.0f;
        EperienceToNetLevel = 100.0f;
        TotalEperience = 0.0f;
        LevelTitle = TET("?L"};
        LevelPermissions.Empty();
        LevelRewards.Empty();
        SkillPoints = 0;
        AttributePoints = 0;
    }
};

// ??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEquipmentItem
{
    GENERATED_BODY()

    // ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    // ??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    // ?z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemDescription;

    // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemType;

    // ?}??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemRarity;

    // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    
    TMap<FString, float> ItemAttributes;

    // ????m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipSlot;

    
    bool bIsEquipped;

    // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemQuantity;

    
    float ItemDurability;

    
    float MaDurability;

    // 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // 3D??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    FMingEquipmentItem()
    {
        ItemID = -1;
        ItemName = TET(""};
        ItemDescription = TET(""};
        ItemType = TET(""};
        ItemRarity = TET("???q"};
        ItemLevel = 1;
        ItemAttributes.Empty();
        EquipSlot = TET(""};
        bIsEquipped = false;
        ItemQuantity = 1;
        ItemDurability = 100.0f;
        MaDurability = 100.0f;
        ItemIconPath = TET(""};
        ItemModelPath = TET(""};
    }
};

// ?????????e
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterLevelUp, int32, NewLevel, const FMingCharacterAttributes&, NewAttributes};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelUp, int32, SkillID, int32, NewLevel};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEperienceGained, EMingEperienceSource, Source, float, EperienceAmount};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, int32, ItemID, const FString&, EquipSlot};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, const FString&, AttributeName, float, NewValue};

/**
 * ???????t??
 * ?t?d???B????
 */
UCLASS(ClassGroup = (Character), Blueprintable, BlueprintType)
class MINGCORE_API UMingCharacterGrowthSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCharacterGrowthSystem();

    
    bool InitializeGrowthSystem();

    
    bool CreateCharacter(const FString& CharacterName, EMingCharacterClass CharacterClass};

    // ?g
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool GainEperience(EMingEperienceSource Source, float EperienceAmount};

    // ????
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LevelUpCharacter();

    // 
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateAttributePoint(const FString& AttributeName, int32 Points};

    // 
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateSkillPoint(int32 SkillID};

    
    bool LearnSkill(int32 SkillID};

    
    bool UpgradeSkill(int32 SkillID};

    // ????H??
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterAttributes GetCharacterAttributes() const;

    // 
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterLevel GetCharacterLevel() const;

    
    TArray<FMingSkillData> GetCharacterSkills() const;

    
    FMingSkillData GetSkill(int32 SkillID) const;

    // ??
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool EquipItem(int32 ItemID, const FString& EquipSlot) {};

    // 
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool UnequipItem(const FString& EquipSlot) {};

    // 
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    TArray<FMingEquipmentItem> GetEquippedItems() const;

    
    float CalculateCombatPower() const;

    // ?p?`
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    float CalculateOverallPower() const;

    // ?O
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool SaveCharacterData();

    // ???J????
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LoadCharacterData();

    // ?m????
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    void ResetCharacter();

    // ???e
    UPROPERTY(BlueprintAssignable)
    FOnCharacterLevelUp OnCharacterLevelUp;

    UPROPERTY(BlueprintAssignable)
    FOnSkillLevelUp OnSkillLevelUp;

    UPROPERTY(BlueprintAssignable)
    FOnEperienceGained OnEperienceGained;

    UPROPERTY(BlueprintAssignable)
    FOnItemEquipped OnItemEquipped;

    UPROPERTY(BlueprintAssignable)
    FOnAttributeChanged OnAttributeChanged;

protected:
    
    FMingCharacterAttributes CharacterAttributes;

    // ????
    UPROPERTY()
    FMingCharacterLevel CharacterLevel;

    // ?w
    UPROPERTY()
    TMap<int32, FMingSkillData> SkillDatabase;

    
    TMap<FString, FMingEquipmentItem> EquippedItems;

    // 
    UPROPERTY()
    TArray<FMingEquipmentItem> InventoryItems;

    // ?????~
    UPROPERTY()
    EMingCharacterClass CharacterClass;

    // ??????
    UPROPERTY()
    FString CharacterName;

    // ?_?w?l
    UPROPERTY()
    bool bInitialized;

private:
    // ???J?]    void LoadDefaultSkills();

    // ???J?]??
    void LoadDefaultEquipment();

    // ?p?g
    float CalculateEperienceToNetLevel(int32 CurrentLevel) const;

    // ?p?g
    float CalculateSkillEperienceToNetLevel(int32 CurrentLevel) const;

    // ?£`??y
    void ApplyLevelRewards(int32 NewLevel};

    //     void ApplySkillEffects(int32 SkillID, int32 NewLevel};

    // ?£c?
    void ApplyEquipmentEffects(const FMingEquipmentItem& Item) {};

    // ??????
    void RemoveEquipmentEffects(const FMingEquipmentItem& Item) {};

    // ??    bool CanLearnSkill(int32 SkillID) const;

    // ?p???v    float CalculateAttributeModifier(const FString& AttributeName) const;

    // 
    FMingEquipmentItem GenerateRandomEquipment(int32 ItemLevel) const;

    // ?s????    void UpdateCharacterPower();

    // ?O??????
    void SaveCharacterSnapshot();

    // ???J??????
    void LoadCharacterSnapshot();

    // ??
    bool ValidateCharacterData() const;

    // ?p??
    TArray<int32> CalculateSkillTree() const;

    //     void UnlockNewSkills(int32 NewLevel};

    // ?p?~
    TMap<FString, float> CalculateClassBonuses() const;
};

