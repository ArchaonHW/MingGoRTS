#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.generated.h"

// 角色職業類型
UENUM(BlueprintType)
enum class EMingCharacterClass : uint8
{
    Infantry,          // 步兵軍官
    Cavalry,           // 騎兵軍官
    Artillery,         // 砲兵軍官
    Engineer,           // 工程師
    Medic,             // 醫官
    Scout,             // 偵察兵
    Commander,         // 指揮官
    Diplomat           // 外交官
};

// 技能類型
UENUM(BlueprintType)
enum class EMingSkillType : uint8
{
    Combat,            // 戰鬥技能
    Leadership,         // 指揮技能
    Strategy,          // 戰略技能
    Diplomacy,         // 外交技能
    Engineering,       // 工程技能
    Medical,           // 醫療技能
    Scouting,          // 偵察技能
    Survival           // 生存技能
};

// 經驗來源
UENUM(BlueprintType)
enum class EMingExperienceSource : uint8
{
    Combat,            // 戰鬥經驗
    Mission,           // 任務經驗
    Exploration,       // 探索經驗
    Diplomacy,         // 外交經驗
    Training,          // 訓練經驗
    Achievement,       // 成就經驗
    Story              // 故事經驗
};

// 角色屬性
USTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterAttributes
{
    GENERATED_BODY()

    // 基礎屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Strength;          // 力量

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Agility;           // 敏捷

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intelligence;       // 智力

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Charisma;          // 魅力

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Constitution;      // 體質

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Wisdom;            // 智慧

    // 戰鬥屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatSkill;        // 戰鬥技能

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Leadership;         // 領導力

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TacticalSense;      // 戰術感覺

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Marksmanship;       // 射擊精度

    // 社交屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Diplomacy;         // 外交能力

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Negotiation;        // 談判技巧

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Intimidation;       // 威嚇能力

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Persuasion;        // 說服力

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

// 技能數據
USTRUCT(BlueprintType)
struct MINGCORE_API FMingSkillData
{
    GENERATED_BODY()

    // 技能ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillID;

    // 技能名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillName;

    // 技能描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillDescription;

    // 技能類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingSkillType SkillType;

    // 當前等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    // 最大等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxLevel;

    // 當前經驗
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentExperience;

    // 升級所需經驗
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceToNextLevel;

    // 技能效果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> SkillEffects;

    // 前置技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> PrerequisiteSkills;

    // 解鎖條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UnlockConditions;

    // 技能圖標路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SkillIconPath;

    // 是否為主動技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActiveSkill;

    // 冷卻時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CooldownTime;

    // 消耗資源
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// 角色等級數據
USTRUCT(BlueprintType)
struct MINGCORE_API FMingCharacterLevel
{
    GENERATED_BODY()

    // 當前等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentLevel;

    // 當前經驗
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentExperience;

    // 升級所需經驗
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExperienceToNextLevel;

    // 總經驗值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalExperience;

    // 等級標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelTitle;

    // 等級權限
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelPermissions;

    // 等級獎勵
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> LevelRewards;

    // 技能點數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkillPoints;

    // 屬性點數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttributePoints;

    FMingCharacterLevel()
    {
        CurrentLevel = 1;
        CurrentExperience = 0.0f;
        ExperienceToNextLevel = 100.0f;
        TotalExperience = 0.0f;
        LevelTitle = TEXT("新兵");
        LevelPermissions.Empty();
        LevelRewards.Empty();
        SkillPoints = 0;
        AttributePoints = 0;
    }
};

// 裝備物品數據
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEquipmentItem
{
    GENERATED_BODY()

    // 物品ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    // 物品名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    // 物品描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemDescription;

    // 物品類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemType;

    // 物品稀有度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemRarity;

    // 物品等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    // 物品屬性加成
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ItemAttributes;

    // 裝備位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipSlot;

    // 是否已裝備
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // 物品數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemQuantity;

    // 物品耐久度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemDurability;

    // 最大耐久度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // 物品圖標路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // 物品3D模型路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    FMingEquipmentItem()
    {
        ItemID = -1;
        ItemName = TEXT("");
        ItemDescription = TEXT("");
        ItemType = TEXT("");
        ItemRarity = TEXT("普通");
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

// 角色成長事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterLevelUp, int32, NewLevel, const FMingCharacterAttributes&, NewAttributes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillLevelUp, int32, SkillID, int32, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceGained, EMingExperienceSource, Source, float, ExperienceAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, int32, ItemID, const FString&, EquipSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChanged, const FString&, AttributeName, float, NewValue);

/**
 * 角色成長系統
 * 負責管理角色等級、技能、經驗和裝備成長
 */
UCLASS(ClassGroup = (Character), Blueprintable, BlueprintType)
class MINGCORE_API UMingCharacterGrowthSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingCharacterGrowthSystem();

    // 初始化角色成長系統
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool InitializeGrowthSystem();

    // 創建新角色
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool CreateCharacter(const FString& CharacterName, EMingCharacterClass CharacterClass);

    // 獲得經驗
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool GainExperience(EMingExperienceSource Source, float ExperienceAmount);

    // 升級角色
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LevelUpCharacter();

    // 分配屬性點
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateAttributePoint(const FString& AttributeName, int32 Points);

    // 分配技能點
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool AllocateSkillPoint(int32 SkillID);

    // 學習技能
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LearnSkill(int32 SkillID);

    // 升級技能
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool UpgradeSkill(int32 SkillID);

    // 獲取角色信息
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterAttributes GetCharacterAttributes() const;

    // 獲取角色等級
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingCharacterLevel GetCharacterLevel() const;

    // 獲取技能列表
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    TArray<FMingSkillData> GetCharacterSkills() const;

    // 獲取特定技能
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    FMingSkillData GetSkill(int32 SkillID) const;

    // 裝備物品
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool EquipItem(int32 ItemID, const FString& EquipSlot);

    // 卸下物品
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool UnequipItem(const FString& EquipSlot);

    // 獲取裝備物品
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    TArray<FMingEquipmentItem> GetEquippedItems() const;

    // 計算戰鬥力
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    float CalculateCombatPower() const;

    // 計算總體實力
    UFUNCTION(BlueprintPure, Category = "Character Growth")
    float CalculateOverallPower() const;

    // 保存角色數據
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool SaveCharacterData();

    // 載入角色數據
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    bool LoadCharacterData();

    // 重置角色
    UFUNCTION(BlueprintCallable, Category = "Character Growth")
    void ResetCharacter();

    // 事件委託
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
    // 角色屬性
    UPROPERTY()
    FMingCharacterAttributes CharacterAttributes;

    // 角色等級
    UPROPERTY()
    FMingCharacterLevel CharacterLevel;

    // 技能數據庫
    UPROPERTY()
    TMap<int32, FMingSkillData> SkillDatabase;

    // 裝備物品庫
    UPROPERTY()
    TMap<FString, FMingEquipmentItem> EquippedItems;

    // 背包物品
    UPROPERTY()
    TArray<FMingEquipmentItem> InventoryItems;

    // 角色職業
    UPROPERTY()
    EMingCharacterClass CharacterClass;

    // 角色名稱
    UPROPERTY()
    FString CharacterName;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設技能數據
    void LoadDefaultSkills();

    // 載入預設裝備數據
    void LoadDefaultEquipment();

    // 計算升級所需經驗
    float CalculateExperienceToNextLevel(int32 CurrentLevel) const;

    // 計算技能升級所需經驗
    float CalculateSkillExperienceToNextLevel(int32 CurrentLevel) const;

    // 應用等級獎勵
    void ApplyLevelRewards(int32 NewLevel);

    // 應用技能效果
    void ApplySkillEffects(int32 SkillID, int32 NewLevel);

    // 應用裝備效果
    void ApplyEquipmentEffects(const FMingEquipmentItem& Item);

    // 移除裝備效果
    void RemoveEquipmentEffects(const FMingEquipmentItem& Item);

    // 驗證技能學習條件
    bool CanLearnSkill(int32 SkillID) const;

    // 計算屬性影響
    float CalculateAttributeModifier(const FString& AttributeName) const;

    // 生成隨機裝備
    FMingEquipmentItem GenerateRandomEquipment(int32 ItemLevel) const;

    // 更新角色戰鬥力
    void UpdateCharacterPower();

    // 保存角色快照
    void SaveCharacterSnapshot();

    // 載入角色快照
    void LoadCharacterSnapshot();

    // 驗證角色數據
    bool ValidateCharacterData() const;

    // 計算技能樹
    TArray<int32> CalculateSkillTree() const;

    // 解鎖新技能
    void UnlockNewSkills(int32 NewLevel);

    // 計算職業加成
    TMap<FString, float> CalculateClassBonuses() const;
};
