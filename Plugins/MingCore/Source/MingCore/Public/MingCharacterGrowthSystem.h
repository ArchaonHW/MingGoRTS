#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.generated.h"

// ËßíËâ≤?∑Ê•≠È°ûÂ?
UENUM(BlueprintType)
enum class EMingCharacterClass : uint8
{
    Infantry,          // Ê≠•ÂÖµËªçÂ?
    Cavalry,           // È®éÂÖµËªçÂ?
    Artillery,         // ?≤ÂÖµËªçÂ?
    Engineer,           // Â∑•Á?Â∏?    Medic,             // ?´Â?
    Scout,             // ?µÂ???    Commander,         // ?áÊèÆÂÆ?    Diplomat           // Â§ñ‰∫§ÂÆ?};

// ?Ä?ΩÈ???UENUM(BlueprintType)
enum class EMingSkillType : uint8
{
    Combat,            // ?∞È¨•?Ä??    Leadership,         // ?áÊèÆ?Ä??    Strategy,          // ?∞Áï•?Ä??    Diplomacy,         // Â§ñ‰∫§?Ä??    Engineering,       // Â∑•Á??Ä??    Medical,           // ?´Á??Ä??    Scouting,          // ?µÂ??Ä??    Survival           // ?üÂ??Ä??};

// Á∂ìÈ?‰æÜÊ?
UENUM(BlueprintType)
enum class EMingExperienceSource : uint8
{
    Combat,            // ?∞È¨•Á∂ìÈ?
    Mission,           // ‰ªªÂ?Á∂ìÈ?
    Exploration,       // ?¢Á¥¢Á∂ìÈ?
    Diplomacy,         // Â§ñ‰∫§Á∂ìÈ?
    Training,          // Ë®ìÁ∑¥Á∂ìÈ?
    Achievement,       // ?êÂ∞±Á∂ìÈ?
    Story              // ?Ö‰?Á∂ìÈ?
};

// ËßíËâ≤Â±¨ÊÄ?USTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterAttributes
{
    GENERATED_BODY()

    // ?∫Á?Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Strength;          // ?õÈ?

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Agility;           // ?èÊç∑

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intelligence;       // ?∫Â?

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Charisma;          // È≠ÖÂ?

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Constitution;      // È´îË≥™

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Wisdom;            // ?∫ÊÖß

    // ?∞È¨•Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatSkill;        // ?∞È¨•?Ä??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;         // ?òÂ???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TacticalSense;      // ?∞Ë??üË¶∫

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Marksmanship;       // Â∞ÑÊ?Á≤æÂ∫¶

    // Á§æ‰∫§Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Diplomacy;         // Â§ñ‰∫§?ΩÂ?

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Negotiation;        // Ë´áÂà§?ÄÂ∑?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intimidation;       // Â®ÅÂ??ΩÂ?

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Persuasion;        // Ë™™Ê???
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

// ?Ä?ΩÊï∏??USTRUCT(BlueprintType)
struct MINGCORE_API FMingSkillData
{
    GENERATED_BODY()

    // ?Ä?ΩID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillID;

    // ?Ä?ΩÂ?Á®?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillName;

    // ?Ä?ΩÊ?Ëø?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillDescription;

    // ?Ä?ΩÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSkillType SkillType;

    // ?∂Â?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    // ?ÄÂ§ßÁ?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxLevel;

    // ?∂Â?Á∂ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentExperience;

    // ?áÁ??Ä?ÄÁ∂ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceToNextLevel;

    // ?Ä?ΩÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> SkillEffects;

    // ?çÁΩÆ?Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> PrerequisiteSkills;

    // Ëß??Ê¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // ?Ä?ΩÂ?Ê®ôË∑ØÂæ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillIconPath;

    // ?ØÂê¶?∫‰∏ª?ïÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActiveSkill;

    // ?∑Âçª?ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownTime;

    // Ê∂àËÄóË?Ê∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ResourceCost;

    FMingSkillData()
    {
        SkillID = -1;
        SkillName = TEXT("");
        SkillDescription = TEXT("");
        SkillType = EMingSkillType::Combat;
        CurrentLevel = 1;
        MaxLevel = 10;
        CurrentExperience = 0.0f;
        ExperienceToNextLevel = 100.0f;
        SkillEffects.Empty();
        PrerequisiteSkills.Empty();
        UnlockConditions.Empty();
        SkillIconPath = TEXT("");
        bIsActiveSkill = false;
        CooldownTime = 0.0f;
        ResourceCost.Empty();
    }
};

// ËßíËâ≤Á≠âÁ??∏Ê?
USTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterLevel
{
    GENERATED_BODY()

    // ?∂Â?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    // ?∂Â?Á∂ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentExperience;

    // ?áÁ??Ä?ÄÁ∂ìÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceToNextLevel;

    // Á∏ΩÁ?È©óÂÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalExperience;

    // Á≠âÁ?Ê®ôÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelTitle;

    // Á≠âÁ?Ê¨äÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelPermissions;

    // Á≠âÁ??éÂãµ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelRewards;

    // ?Ä?ΩÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillPoints;

    // Â±¨ÊÄßÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttributePoints;

    FMingCharacterLevel()
    {
        CurrentLevel = 1;
        CurrentExperience = 0.0f;
        ExperienceToNextLevel = 100.0f;
        TotalExperience = 0.0f;
        LevelTitle = TEXT("?∞ÂÖµ");
        LevelPermissions.Empty();
        LevelRewards.Empty();
        SkillPoints = 0;
        AttributePoints = 0;
    }
};

// Ë£ùÂ??©Â??∏Ê?
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEquipmentItem
{
    GENERATED_BODY()

    // ?©Â?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    // ?©Â??çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    // ?©Â??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemDescription;

    // ?©Â?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemType;

    // ?©Â?Á®Ä?âÂ∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemRarity;

    // ?©Â?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    // ?©Â?Â±¨ÊÄßÂ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ItemAttributes;

    // Ë£ùÂ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipSlot;

    // ?ØÂê¶Â∑≤Ë???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // ?©Â??∏È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemQuantity;

    // ?©Â??ê‰?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemDurability;

    // ?ÄÂ§ßËÄê‰?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // ?©Â??ñÊ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // ?©Â?3DÊ®°Â?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    FMingEquipmentItem()
    {
        ItemID = -1;
        ItemName = TEXT("");
        ItemDescription = TEXT("");
        ItemType = TEXT("");
        ItemRarity = TEXT("?ÆÈÄ?);
        ItemLevel = 1;
        ItemAttributes.Empty();
        EquipSlot = TEXT("");
        bIsEquipped = false;
        ItemQuantity = 1;
        ItemDurability = 100.0f;
        MaxDurability = 100.0f;
        ItemIconPath = TEXT("");
        ItemModelPath = TEXT("");
    }
};

// ËßíËâ≤?êÈï∑‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterLevelUp, int32, NewLevel, const FMingCharacterAttributes&, NewAttributes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelUp, int32, SkillID, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceGained, EMingExperienceSource, Source, float, ExperienceAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, int32, ItemID, const FString&, EquipSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, const FString&, AttributeName, float, NewValue);

/**
 * ËßíËâ≤?êÈï∑Á≥ªÁµ±
 * Ë≤†Ë≤¨ÁÆ°Á?ËßíËâ≤Á≠âÁ??ÅÊ??Ω„ÄÅÁ?È©óÂ?Ë£ùÂ??êÈï∑
 */
UCLASS(ClassGroup = (Character), Blueprintable, BlueprintType)
class MINGCORE_API UMingCharacterGrowthSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCharacterGrowthSystem();

    // ?ùÂ??ñË??≤Ê??∑Á≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool InitializeGrowthSystem();

    // ?µÂª∫?∞Ë???    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool CreateCharacter(const FString& CharacterName, EMingCharacterClass CharacterClass);

    // ?≤Â?Á∂ìÈ?
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool GainExperience(EMingExperienceSource Source, float ExperienceAmount);

    // ?áÁ?ËßíËâ≤
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LevelUpCharacter();

    // ?ÜÈ?Â±¨ÊÄßÈ?
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateAttributePoint(const FString& AttributeName, int32 Points);

    // ?ÜÈ??Ä?ΩÈ?
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateSkillPoint(int32 SkillID);

    // Â≠∏Á??Ä??    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LearnSkill(int32 SkillID);

    // ?áÁ??Ä??    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool UpgradeSkill(int32 SkillID);

    // ?≤Â?ËßíËâ≤‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterAttributes GetCharacterAttributes() const;

    // ?≤Â?ËßíËâ≤Á≠âÁ?
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterLevel GetCharacterLevel() const;

    // ?≤Â??Ä?ΩÂ?Ë°?    UFUNCTION(BlueprintPure, Category = "Character Growth")
    TArray<FMingSkillData> GetCharacterSkills() const;

    // ?≤Â??πÂ??Ä??    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingSkillData GetSkill(int32 SkillID) const;

    // Ë£ùÂ??©Â?
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool EquipItem(int32 ItemID, const FString& EquipSlot);

    // ?∏‰??©Â?
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool UnequipItem(const FString& EquipSlot);

    // ?≤Â?Ë£ùÂ??©Â?
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    TArray<FMingEquipmentItem> GetEquippedItems() const;

    // Ë®àÁ??∞È¨•??    UFUNCTION(BlueprintPure, Category = "Character Growth")
    float CalculateCombatPower() const;

    // Ë®àÁ?Á∏ΩÈ?ÂØ¶Â?
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    float CalculateOverallPower() const;

    // ‰øùÂ?ËßíËâ≤?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool SaveCharacterData();

    // ËºâÂÖ•ËßíËâ≤?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LoadCharacterData();

    // ?çÁΩÆËßíËâ≤
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    void ResetCharacter();

    // ‰∫ã‰ª∂ÂßîË?
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
    // ËßíËâ≤Â±¨ÊÄ?    UPROPERTY()
    FMingCharacterAttributes CharacterAttributes;

    // ËßíËâ≤Á≠âÁ?
    UPROPERTY()
    FMingCharacterLevel CharacterLevel;

    // ?Ä?ΩÊï∏?öÂ∫´
    UPROPERTY()
    TMap<int32, FMingSkillData> SkillDatabase;

    // Ë£ùÂ??©Â?Â∫?    UPROPERTY()
    TMap<FString, FMingEquipmentItem> EquippedItems;

    // ?åÂ??©Â?
    UPROPERTY()
    TArray<FMingEquipmentItem> InventoryItems;

    // ËßíËâ≤?∑Ê•≠
    UPROPERTY()
    EMingCharacterClass CharacterClass;

    // ËßíËâ≤?çÁ®±
    UPROPERTY()
    FString CharacterName;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠?Ä?ΩÊï∏??    void LoadDefaultSkills();

    // ËºâÂÖ•?êË®≠Ë£ùÂ??∏Ê?
    void LoadDefaultEquipment();

    // Ë®àÁ??áÁ??Ä?ÄÁ∂ìÈ?
    float CalculateExperienceToNextLevel(int32 CurrentLevel) const;

    // Ë®àÁ??Ä?ΩÂ?Á¥öÊ??ÄÁ∂ìÈ?
    float CalculateSkillExperienceToNextLevel(int32 CurrentLevel) const;

    // ?âÁî®Á≠âÁ??éÂãµ
    void ApplyLevelRewards(int32 NewLevel);

    // ?âÁî®?Ä?ΩÊ???    void ApplySkillEffects(int32 SkillID, int32 NewLevel);

    // ?âÁî®Ë£ùÂ??àÊ?
    void ApplyEquipmentEffects(const FMingEquipmentItem& Item);

    // ÁßªÈô§Ë£ùÂ??àÊ?
    void RemoveEquipmentEffects(const FMingEquipmentItem& Item);

    // È©óË??Ä?ΩÂ≠∏ÁøíÊ?‰ª?    bool CanLearnSkill(int32 SkillID) const;

    // Ë®àÁ?Â±¨ÊÄßÂΩ±??    float CalculateAttributeModifier(const FString& AttributeName) const;

    // ?üÊ??®Ê?Ë£ùÂ?
    FMingEquipmentItem GenerateRandomEquipment(int32 ItemLevel) const;

    // ?¥Êñ∞ËßíËâ≤?∞È¨•??    void UpdateCharacterPower();

    // ‰øùÂ?ËßíËâ≤Âø´ÁÖß
    void SaveCharacterSnapshot();

    // ËºâÂÖ•ËßíËâ≤Âø´ÁÖß
    void LoadCharacterSnapshot();

    // È©óË?ËßíËâ≤?∏Ê?
    bool ValidateCharacterData() const;

    // Ë®àÁ??Ä?ΩÊ®π
    TArray<int32> CalculateSkillTree() const;

    // Ëß???∞Ê???    void UnlockNewSkills(int32 NewLevel);

    // Ë®àÁ??∑Ê•≠?†Ê?
    TMap<FString, float> CalculateClassBonuses() const;
};
