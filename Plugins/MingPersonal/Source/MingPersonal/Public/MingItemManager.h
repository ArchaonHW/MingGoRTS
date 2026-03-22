#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.generated.h"

// 摧毀稀??度
UENUM(BlueprintType)
enum class EMingItemRarity: uint8 {
    Common,            // 摧毀
    Uncommon,          // 不常??
    Rare,              // 稀X
    Epic,              // ??詩
    Legendary           // 摧毀
};

// 選項類型
UENUM(BlueprintType)
enum class EMingItemType: uint8 {
    Weapon,            // 武器
    Armor,             // 護甲
    Accessory,         // ??飾
    Consumable,        // 消耗??
    Material,          // 摧毀
    Blueprint,         // 摧毀
    Currency,          // 貨幣
    Quest,             // 任務目標
    Cosmetic           // 外?X???
};

// 裝??槽??
UENUM(BlueprintType)
enum class EMingEquipSlot: uint8 {
    MainHand,          // 主??
    OffHand,           // 摧毀
    Head,              // ??部
    Body,              // 身??
    Hands,             // ??部
    Legs,              // ??部
    Feet,              // ??部
    Neck,              // ??部
    Ring1,             // 摧毀1
    Ring2,             // 摧毀2
    Trinket1,          // 飾物1
    Trinket2,          // 飾物2
    Backpack           // 摧毀
};

// 摧毀強??類??
UENUM(BlueprintType)
enum class EMingEnhancementType: uint8 {
    None,               // ??強X
    Sharp,             // ??利
    Durable,           // 摧毀
    Powerful,          // 威??
    Swift,             // 迅捷
    Magical,           // 魔??
    Blessed,           // 祝??
    Cursed             // 詛??
};

// 目標數量
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingItemData
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    // ???X?稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    // 選項描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemDescription;

    // 選項類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemType ItemType;

    // 摧毀稀??度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity ItemRarity;

    // 摧毀等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemValue;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemWeight;

    // 故事選項?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackSize;

    // 故事選項?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentStack;

    // 摧毀屬??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ItemAttributes;

    // 裝??槽??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // ??否已完成
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // 目標數量??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // ??大耐摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // 摧毀強??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEnhancementType EnhancementType;

    // 強??等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancementLevel;

    // 目標數量路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // 摧毀3D模??路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    // ???X?質路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemMaterialPath;

    // 目標數量路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemEffectPath;

    // 目標數量路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemSoundPath;

    // ??否??唯一摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnique;

    // 是否可見定物X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSoulbound;

    // 摧毀標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ItemTags;

    // 故事選項?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AcquiredTime;

    // 摧毀來??
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

// 裝?X???
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingEquipmentData
{
    GENERATED_BODY()

    // 裝??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentID;

    // 裝?X?稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentName;

    // 裝?X?述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentDescription;

    // 裝??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemType EquipmentType;

    // 裝??稀??度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity EquipmentRarity;

    // 裝??等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentLevel;

    // 裝??屬??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> EquipmentAttributes;

    // 裝??槽??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // ??否已完成
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // 裝?X摧毀?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // ??大耐摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // 裝??強??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEnhancementType EnhancementType;

    // 強??等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancementLevel;

    // 裝?X???路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentIconPath;

    // 裝??3D模??路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentModelPath;

    // 裝?X?質路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentMaterialPath;

    // 裝?X???路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentEffectPath;

    // 裝?X???路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentSoundPath;

    // 裝?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> EquipmentRequirements;

    // 裝?X??X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AcquiredTime;

    // 裝??來??
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

// 故事重要性?方
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingCraftingRecipe
{
    GENERATED_BODY()

    // ??方ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RecipeID;

    // ??方??稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeName;

    // ??方??述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeDescription;

    // 摧毀摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> RequiredMaterials;

    // 摧毀結??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ResultItems;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CraftingTime;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;

    // ??方稀??度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity RecipeRarity;

    // ??方??求?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> RequiredSkills;

    // ??方??求摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredLevel;

    // ??方摧毀路??
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

// 摧毀管??事件委??






DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRepaired, const FMingEquipmentData&, Equipment, float, RepairAmount);

/**
 * 摧毀管?X * 負責管?X?人裝?X?物??系?? */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingItemManager : public UObject
{
    GENERATED_BODY()

public:
    UMingItemManager();

    // ???X?物??系??
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool InitializeItemSystem();

    // 添?X??X??X
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool AddItemToInventory(const FMingItemData& Item, int32 Quantity = 1);

    // 從?X?移??物X
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool RemoveItemFromInventory(int32 ItemID, int32 Quantity = 1);

    // 裝?X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool EquipItem(int32 ItemID, const FString& EquipSlot);

    // 目標數量
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool UnequipItem(const FString& EquipSlot);

    // 故事選項?
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingItemData> GetInventoryItems() const;

    // 摧毀已完成?物X
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingEquipmentData> GetEquippedItems() const;

    // 目標數量槽?X???
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingEquipmentData GetEquippedItem(const FString& EquipSlot) const;

    // 檢查??否??以裝??
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    bool CanEquipItem(int32 ItemID, const FString& EquipSlot) const;

    // 強?X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool EnhanceItem(int32 ItemID, EMingEnhancementType EnhancementType);

    // 修?X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool RepairItem(int32 ItemID, float RepairAmount = 100.0f);

    // 目標數量
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool CraftItem(int32 RecipeID);

    // ??解摧毀
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool DisassembleItem(int32 ItemID);

    // 故事選項?
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetItemCount(int32 ItemID) const;

    // 目標數量信息
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingItemData GetItemInfo(int32 ItemID) const;

    // 摧毀裝??信息
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingEquipmentData GetEquipmentInfo(int32 EquipmentID) const;

    // ???X?用??方
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingCraftingRecipe> GetAvailableRecipes() const;

    // 檢查摧毀條件
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    bool CanCraftRecipe(int32 RecipeID) const;

    // 目標數量總價X
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetTotalInventoryValue() const;

    // 目標數量容??
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryCapacity() const;

    // 目標數量使用X
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryUsed() const;

    // 故事選項?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    void SortInventory(EMingItemType SortBy, bool bAscending = true);

    // 保?X??X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool SaveItemData();

    // 載入目標數量
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool LoadItemData();

    // 清除摧毀物??數X
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    void ClearAllItemData();

    // 事件委??
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
    // 目標數量??
    UPROPERTY()
    TMap<int32, FMingItemData> ItemDatabase;

    // 裝?X摧毀?
    UPROPERTY()
    TMap<int32, FMingEquipmentData> EquipmentDatabase;

    // 目標數量
    UPROPERTY()
    TArray<FMingItemData> InventoryItems;

    // 已完成?物X
    UPROPERTY()
    TMap<FString, FMingEquipmentData> EquippedItems;

    // ???X?方摧毀??
    UPROPERTY()
    TMap<int32, FMingCraftingRecipe> CraftingRecipeDatabase;

    // 摧毀容??
    UPROPERTY()
    int32 InventoryCapacity;

    // 摧毀使用X
    UPROPERTY()
    int32 InventoryUsed;

    // ??否已??始??
    UPROPERTY()
    bool bInitialized;

private:
    // 載入??設摧毀
    void LoadDefaultItems();

    // 載入默認配方
    void LoadDefaultRecipes();

    // 驗證物品數據
    bool ValidateItemData(const FMingItemData& Item) const;

    // 驗證裝備數據
    bool ValidateEquipmentData(const FMingEquipmentData& Equipment) const;

    // 計算背包總重
    float CalculateTotalInventoryWeight() const;

    // 計算裝備加成
    TMap<FString, float> CalculateEquipmentBonuses() const;

    // 應用裝備效果
    void ApplyEquipmentEffects(const FMingEquipmentData& Equipment);

    // 移除裝備效果
    void RemoveEquipmentEffects(const FMingEquipmentData& Equipment);

    // ??新摧毀容??使用
    void UpdateInventoryUsage();

    // 檢查摧毀空??
    bool HasInventorySpace(const FMingItemData& Item, int32 Quantity) const;

    // 故事選項?
    FMingItemData GenerateRandomItem(EMingItemRarity Rarity, EMingItemType ItemType) const;

    // 計算物品價值
    int32 CalculateItemValue(const FMingItemData& Item) const;

    // 目標數量稀??度顏色
    FLinearColor GetRarityColor(EMingItemRarity Rarity) const;

    // 目標數量類?X???
    FString GetItemTypeIcon(EMingItemType ItemType) const;

    // ??新目標數量??
    void UpdateItemDurability(int32 ItemID, float Delta);

    // 檢查???X?否摧毀
    bool IsItemDamaged(int32 ItemID) const;

    // 修復目標數量
    void RepairDamagedItems();

    // 故事選項?
    void AutoOrganizeInventory();

    // ??併目標數量
    void MergeStackableItems();

    // ??離目標數量
    void SplitItemStack(int32 ItemID, int32 SplitQuantity);

    // 計算製作成功率
    float CalculateCraftingSuccessRate(int32 RecipeID) const;

    // 消耗製作材料
    bool ConsumeCraftingMaterials(int32 RecipeID);

    // 添加製作結果
    bool AddCraftingResults(int32 RecipeID);

    // 目標數量統??
    TMap<EMingItemType, int32> GetItemStatistics() const;

    // 摧毀稀??度統??
    TMap<EMingItemRarity, int32> GetRarityStatistics() const;

    // 保存物品快照
    void SaveItemSnapshot();

    // 載入摧毀快照
    void LoadItemSnapshot();
};

