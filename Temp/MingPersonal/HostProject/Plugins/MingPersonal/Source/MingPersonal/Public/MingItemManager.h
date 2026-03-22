#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.generated.h"

// 物品稀有度
UENUM(BlueprintType)
enum class EMingItemRarity : uint8
{
    Common,            // 普通
    Uncommon,          // 不常見
    Rare,              // 稀有
    Epic,              // 史詩
    Legendary           // 傳奇
};

// 物品類型
UENUM(BlueprintType)
enum class EMingItemType : uint8
{
    Weapon,            // 武器
    Armor,             // 護甲
    Accessory,         // 配飾
    Consumable,        // 消耗品
    Material,          // 材料
    Blueprint,         // 藍圖
    Currency,          // 貨幣
    Quest,             // 任務物品
    Cosmetic           // 外觀物品
};

// 裝備槽位
UENUM(BlueprintType)
enum class EMingEquipSlot : uint8
{
    MainHand,          // 主手
    OffHand,           // 副手
    Head,              // 頭部
    Body,              // 身體
    Hands,             // 手部
    Legs,              // 腿部
    Feet,              // 腳部
    Neck,              // 頸部
    Ring1,             // 戒指1
    Ring2,             // 戒指2
    Trinket1,          // 飾物1
    Trinket2,          // 飾物2
    Backpack           // 背包
};

// 物品強化類型
UENUM(BlueprintType)
enum class EMingEnhancementType : uint8
{
    None,               // 無強化
    Sharp,             // 鋒利
    Durable,           // 耐久
    Powerful,          // 威力
    Swift,             // 迅捷
    Magical,           // 魔法
    Blessed,           // 祝福
    Cursed             // 詛咒
};

// 物品數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingItemData
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
    EMingItemType ItemType;

    // 物品稀有度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity ItemRarity;

    // 物品等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    // 物品價值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemValue;

    // 物品重量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemWeight;

    // 物品堆疊數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackSize;

    // 當前堆疊數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentStack;

    // 物品屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ItemAttributes;

    // 裝備槽位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // 是否已裝備
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // 物品耐久度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // 最大耐久度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // 物品強化類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEnhancementType EnhancementType;

    // 強化等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancementLevel;

    // 物品圖標路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // 物品3D模型路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    // 物品材質路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemMaterialPath;

    // 物品特效路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemEffectPath;

    // 物品音效路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemSoundPath;

    // 是否為唯一物品
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnique;

    // 是否為綁定物品
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSoulbound;

    // 物品標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ItemTags;

    // 物品獲取時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AcquiredTime;

    // 物品來源
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemSource;

    FMingItemData()
    {
        ItemID = -1;
        ItemName = TEXT("");
        ItemDescription = TEXT("");
        ItemType = EMingItemType::Weapon;
        ItemRarity = EMingItemRarity::Common;
        ItemLevel = 1;
        ItemValue = 0;
        ItemWeight = 0.0f;
        StackSize = 1;
        CurrentStack = 1;
        ItemAttributes.Empty();
        EquipSlot = EMingEquipSlot::MainHand;
        bIsEquipped = false;
        CurrentDurability = 100.0f;
        MaxDurability = 100.0f;
        EnhancementType = EMingEnhancementType::None;
        EnhancementLevel = 0;
        ItemIconPath = TEXT("");
        ItemModelPath = TEXT("");
        ItemMaterialPath = TEXT("");
        ItemEffectPath = TEXT("");
        ItemSoundPath = TEXT("");
        bIsUnique = false;
        bIsSoulbound = false;
        ItemTags.Empty();
        AcquiredTime = FDateTime::Now();
        ItemSource = TEXT("");
    }
};

// 裝備數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingEquipmentData
{
    GENERATED_BODY()

    // 裝備ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentID;

    // 裝備名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentName;

    // 裝備描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentDescription;

    // 裝備類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemType EquipmentType;

    // 裝備稀有度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity EquipmentRarity;

    // 裝備等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentLevel;

    // 裝備屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> EquipmentAttributes;

    // 裝備槽位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // 是否已裝備
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // 裝備耐久度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // 最大耐久度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // 裝備強化類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEnhancementType EnhancementType;

    // 強化等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancementLevel;

    // 裝備圖標路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentIconPath;

    // 裝備3D模型路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentModelPath;

    // 裝備材質路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentMaterialPath;

    // 裝備特效路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentEffectPath;

    // 裝備音效路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentSoundPath;

    // 裝備需求
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> EquipmentRequirements;

    // 裝備獲取時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AcquiredTime;

    // 裝備來源
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentSource;

    FMingEquipmentData()
    {
        EquipmentID = -1;
        EquipmentName = TEXT("");
        EquipmentDescription = TEXT("");
        EquipmentType = EMingItemType::Weapon;
        EquipmentRarity = EMingItemRarity::Common;
        EquipmentLevel = 1;
        EquipmentAttributes.Empty();
        EquipSlot = EMingEquipSlot::MainHand;
        bIsEquipped = false;
        CurrentDurability = 100.0f;
        MaxDurability = 100.0f;
        EnhancementType = EMingEnhancementType::None;
        EnhancementLevel = 0;
        EquipmentIconPath = TEXT("");
        EquipmentModelPath = TEXT("");
        EquipmentMaterialPath = TEXT("");
        EquipmentEffectPath = TEXT("");
        EquipmentSoundPath = TEXT("");
        EquipmentRequirements.Empty();
        AcquiredTime = FDateTime::Now();
        EquipmentSource = TEXT("");
    }
};

// 物品合成配方
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingCraftingRecipe
{
    GENERATED_BODY()

    // 配方ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RecipeID;

    // 配方名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeName;

    // 配方描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeDescription;

    // 所需材料
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> RequiredMaterials;

    // 合成結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ResultItems;

    // 合成時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CraftingTime;

    // 合成功率
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;

    // 配方稀有度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity RecipeRarity;

    // 配方需求技能
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> RequiredSkills;

    // 配方需求等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredLevel;

    // 配方圖標路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeIconPath;

    FMingCraftingRecipe()
    {
        RecipeID = -1;
        RecipeName = TEXT("");
        RecipeDescription = TEXT("");
        RequiredMaterials.Empty();
        ResultItems.Empty();
        CraftingTime = 0.0f;
        SuccessRate = 1.0f;
        RecipeRarity = EMingItemRarity::Common;
        RequiredSkills.Empty();
        RequiredLevel = 1;
        RecipeIconPath = TEXT("");
    }
};

// 物品管理事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAcquired, const FMingItemData&, Item, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemLost, const FMingItemData&, Item, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, const FMingEquipmentData&, Equipment, const FString&, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUnequipped, const FMingEquipmentData&, Equipment, const FString&, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEnhanced, const FMingEquipmentData&, Equipment, EMingEnhancementType, EnhancementType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemCrafted, const FMingCraftingRecipe&, Recipe, const TArray<FMingItemData>&, Results);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRepaired, const FMingEquipmentData&, Equipment, float, RepairAmount);

/**
 * 物品管理器
 * 負責管理個人裝備和物品系統
 */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingItemManager : public UObject
{
    GENERATED_BODY()

public:
    UMingItemManager();

    // 初始化物品系統
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool InitializeItemSystem();

    // 添加物品到背包
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool AddItemToInventory(const FMingItemData& Item, int32 Quantity = 1);

    // 從背包移除物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool RemoveItemFromInventory(int32 ItemID, int32 Quantity = 1);

    // 裝備物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool EquipItem(int32 ItemID, const FString& EquipSlot);

    // 卸下物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool UnequipItem(const FString& EquipSlot);

    // 獲取背包物品
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingItemData> GetInventoryItems() const;

    // 獲取已裝備物品
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingEquipmentData> GetEquippedItems() const;

    // 獲取指定槽位物品
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingEquipmentData GetEquippedItem(const FString& EquipSlot) const;

    // 檢查是否可以裝備
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    bool CanEquipItem(int32 ItemID, const FString& EquipSlot) const;

    // 強化物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool EnhanceItem(int32 ItemID, EMingEnhancementType EnhancementType);

    // 修理物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool RepairItem(int32 ItemID, float RepairAmount = 100.0f);

    // 合成物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool CraftItem(int32 RecipeID);

    // 分解物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool DisassembleItem(int32 ItemID);

    // 獲取物品數量
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetItemCount(int32 ItemID) const;

    // 獲取物品信息
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingItemData GetItemInfo(int32 ItemID) const;

    // 獲取裝備信息
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingEquipmentData GetEquipmentInfo(int32 EquipmentID) const;

    // 獲取可用配方
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingCraftingRecipe> GetAvailableRecipes() const;

    // 檢查合成條件
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    bool CanCraftRecipe(int32 RecipeID) const;

    // 獲取物品總價值
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetTotalInventoryValue() const;

    // 獲取背包容量
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryCapacity() const;

    // 獲取背包使用量
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryUsed() const;

    // 排序背包物品
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    void SortInventory(EMingItemType SortBy, bool bAscending = true);

    // 保存物品數據
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool SaveItemData();

    // 載入物品數據
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool LoadItemData();

    // 清除所有物品數據
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    void ClearAllItemData();

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnItemAcquired OnItemAcquired;

    UPROPERTY(BlueprintAssignable)
    FOnItemLost OnItemLost;

    UPROPERTY(BlueprintAssignable)
    FOnItemEquipped OnItemEquipped;

    UPROPERTY(BlueprintAssignable)
    FOnItemUnequipped OnItemUnequipped;

    UPROPERTY(BlueprintAssignable)
    FOnItemEnhanced OnItemEnhanced;

    UPROPERTY(BlueprintAssignable)
    FOnItemCrafted OnItemCrafted;

    UPROPERTY(BlueprintAssignable)
    FOnItemRepaired OnItemRepaired;

protected:
    // 物品數據庫
    UPROPERTY()
    TMap<int32, FMingItemData> ItemDatabase;

    // 裝備數據庫
    UPROPERTY()
    TMap<int32, FMingEquipmentData> EquipmentDatabase;

    // 背包物品
    UPROPERTY()
    TArray<FMingItemData> InventoryItems;

    // 已裝備物品
    UPROPERTY()
    TMap<FString, FMingEquipmentData> EquippedItems;

    // 合成配方數據庫
    UPROPERTY()
    TMap<int32, FMingCraftingRecipe> CraftingRecipeDatabase;

    // 背包容量
    UPROPERTY()
    int32 InventoryCapacity;

    // 背包使用量
    UPROPERTY()
    int32 InventoryUsed;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設物品
    void LoadDefaultItems();

    // 載入預設配方
    void LoadDefaultRecipes();

    // 驗證物品數據
    bool ValidateItemData(const FMingItemData& Item) const;

    // 驗證裝備數據
    bool ValidateEquipmentData(const FMingEquipmentData& Equipment) const;

    // 計算物品總重量
    float CalculateTotalInventoryWeight() const;

    // 計算裝備加成
    TMap<FString, float> CalculateEquipmentBonuses() const;

    // 應用裝備效果
    void ApplyEquipmentEffects(const FMingEquipmentData& Equipment);

    // 移除裝備效果
    void RemoveEquipmentEffects(const FMingEquipmentData& Equipment);

    // 更新背包容量使用
    void UpdateInventoryUsage();

    // 檢查背包空間
    bool HasInventorySpace(const FMingItemData& Item, int32 Quantity) const;

    // 生成隨機物品
    FMingItemData GenerateRandomItem(EMingItemRarity Rarity, EMingItemType ItemType) const;

    // 計算物品價值
    int32 CalculateItemValue(const FMingItemData& Item) const;

    // 獲取物品稀有度顏色
    FLinearColor GetRarityColor(EMingItemRarity Rarity) const;

    // 獲取物品類型圖標
    FString GetItemTypeIcon(EMingItemType ItemType) const;

    // 更新物品耐久度
    void UpdateItemDurability(int32 ItemID, float Delta);

    // 檢查物品是否損壞
    bool IsItemDamaged(int32 ItemID) const;

    // 修復損壞物品
    void RepairDamagedItems();

    // 自動整理背包
    void AutoOrganizeInventory();

    // 合併相同物品
    void MergeStackableItems();

    // 分離物品堆疊
    void SplitItemStack(int32 ItemID, int32 SplitQuantity);

    // 計算合成成功率
    float CalculateCraftingSuccessRate(int32 RecipeID) const;

    // 消耗合成材料
    bool ConsumeCraftingMaterials(int32 RecipeID);

    // 添加合成結果
    bool AddCraftingResults(int32 RecipeID);

    // 獲取物品統計
    TMap<EMingItemType, int32> GetItemStatistics() const;

    // 獲取稀有度統計
    TMap<EMingItemRarity, int32> GetRarityStatistics() const;

    // 保存物品快照
    void SaveItemSnapshot();

    // 載入物品快照
    void LoadItemSnapshot();
};
