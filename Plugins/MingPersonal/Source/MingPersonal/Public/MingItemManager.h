#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.generated.h"

// ????稀??度
UENUM(BlueprintType)
enum class EMingItemRarity : uint8
{
    Common,            // ????    Uncommon,          // 不常??    Rare,              // 稀X    Epic,              // ??詩
    Legendary           // ????
};

// ????類??
UENUM(BlueprintType)
enum class EMingItemType : uint8
{
    Weapon,            // 武器
    Armor,             // 護甲
    Accessory,         // ??飾
    Consumable,        // 消耗??
    Material,          // ????
    Blueprint,         // ????
    Currency,          // 貨幣
    Quest,             // 任?X???
    Cosmetic           // 外?X???
};

// 裝??槽??
UENUM(BlueprintType)
enum class EMingEquipSlot : uint8
{
    MainHand,          // 主??
    OffHand,           // ????
    Head,              // ??部
    Body,              // 身??
    Hands,             // ??部
    Legs,              // ??部
    Feet,              // ??部
    Neck,              // ??部
    Ring1,             // ????1
    Ring2,             // ????2
    Trinket1,          // 飾物1
    Trinket2,          // 飾物2
    Backpack           // ????
};

// ????強??類??
UENUM(BlueprintType)
enum class EMingEnhancementType : uint8
{
    None,               // ??強X    Sharp,             // ??利
    Durable,           // ????
    Powerful,          // 威??
    Swift,             // 迅捷
    Magical,           // 魔??
    Blessed,           // 祝??
    Cursed             // 詛??
};

// ???X???
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingItemData
{
    GENERATED_BODY()

    // ????ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemID;

    // ???X?稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemName;

    // ???X?述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemDescription;

    // ????類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemType ItemType;

    // ????稀??度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity ItemRarity;

    // ????等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    // ???X???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemValue;

    // ???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemWeight;

    // ???X??X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackSize;

    // ???X??X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentStack;

    // ????屬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ItemAttributes;

    // 裝??槽??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // ??否已?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // ???X?????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // ??大耐????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // ????強??類??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEnhancementType EnhancementType;

    // 強??等??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancementLevel;

    // ???X???路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // ????3D模??路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    // ???X?質路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemMaterialPath;

    // ???X???路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemEffectPath;

    // ???X???路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemSoundPath;

    // ??否??唯一????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnique;

    // ??否????定物X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSoulbound;

    // ????標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ItemTags;

    // ???X??X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AcquiredTime;

    // ????來??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemSource;

    FMingItemData()
    {
        ItemID = -1;
        ItemName = TEXT(""};
        ItemDescription = TEXT(""};
        ItemType = EMingItemType::Weapon;
        ItemRarity = EMingItemRarity::Common;
        ItemLevel = 1;
        ItemValue = 0;
        ItemWeight = 0.0f;
        StackSize = 1;
        CurrentStack = 1;
        ItemAttributes.Empty(};
        EquipSlot = EMingEquipSlot::MainHand;
        bIsEquipped = false;
        CurrentDurability = 100.0f;
        MaxDurability = 100.0f;
        EnhancementType = EMingEnhancementType::None;
        EnhancementLevel = 0;
        ItemIconPath = TEXT(""};
        ItemModelPath = TEXT(""};
        ItemMaterialPath = TEXT(""};
        ItemEffectPath = TEXT(""};
        ItemSoundPath = TEXT(""};
        bIsUnique = false;
        bIsSoulbound = false;
        ItemTags.Empty(};
        AcquiredTime = FDateTime::Now(};
        ItemSource = TEXT(""};
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

    // 裝??屬??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> EquipmentAttributes;

    // 裝??槽??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // ??否已?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // 裝?X?????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // ??大耐????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

    // 裝?X???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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
        EquipmentName = TEXT(""};
        EquipmentDescription = TEXT(""};
        EquipmentType = EMingItemType::Weapon;
        EquipmentRarity = EMingItemRarity::Common;
        EquipmentLevel = 1;
        EquipmentAttributes.Empty(};
        EquipSlot = EMingEquipSlot::MainHand;
        bIsEquipped = false;
        CurrentDurability = 100.0f;
        MaxDurability = 100.0f;
        EnhancementType = EMingEnhancementType::None;
        EnhancementLevel = 0;
        EquipmentIconPath = TEXT(""};
        EquipmentModelPath = TEXT(""};
        EquipmentMaterialPath = TEXT(""};
        EquipmentEffectPath = TEXT(""};
        EquipmentSoundPath = TEXT(""};
        EquipmentRequirements.Empty(};
        AcquiredTime = FDateTime::Now(};
        EquipmentSource = TEXT(""};
    }
};

// ???X??X?方
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

    // ????????
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> RequiredMaterials;

    // ????結??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ResultItems;

    // ???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CraftingTime;

    // ???X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;

    // ??方稀??度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity RecipeRarity;

    // ??方??求?X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> RequiredSkills;

    // ??方??求????    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredLevel;

    // ??方????路??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeIconPath;

    FMingCraftingRecipe()
    {
        RecipeID = -1;
        RecipeName = TEXT(""};
        RecipeDescription = TEXT(""};
        RequiredMaterials.Empty(};
        ResultItems.Empty(};
        CraftingTime = 0.0f;
        SuccessRate = 1.0f;
        RecipeRarity = EMingItemRarity::Common;
        RequiredSkills.Empty(};
        RequiredLevel = 1;
        RecipeIconPath = TEXT(""};
    }
};

// ????管??事件委??
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAcquired, const FMingItemData&, Item, int32, Quantity};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemLost, const FMingItemData&, Item, int32, Quantity};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, const FMingEquipmentData&, Equipment, const FString&, Slot};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUnequipped, const FMingEquipmentData&, Equipment, const FString&, Slot};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEnhanced, const FMingEquipmentData&, Equipment, EMingEnhancementType, EnhancementType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemCrafted, const FMingCraftingRecipe&, Recipe, const TArray<FMingItemData>&, Results};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRepaired, const FMingEquipmentData&, Equipment, float, RepairAmount);

/**
 * ????管?X * 負責管?X?人裝?X?物??系?? */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingItemManager : public UObject
{
    GENERATED_BODY()

public:
    UMingItemManager(};

    // ???X?物??系??    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool InitializeItemSystem();

    // 添?X??X??X    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool AddItemToInventory(const FMingItemData& Item, int32 Quantity = 1);

    // 從?X?移??物X    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool RemoveItemFromInventory(int32 ItemID, int32 Quantity = 1);

    // 裝?X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool EquipItem(int32 ItemID, const FString& EquipSlot);

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool UnequipItem(const FString& EquipSlot);

    // ???X??X???
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingItemData> GetInventoryItems() const;

    // ????已?X?物X    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingEquipmentData> GetEquippedItems() const;

    // ???X???槽?X???
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

    // ???X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool CraftItem(int32 RecipeID);

    // ??解????
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool DisassembleItem(int32 ItemID);

    // ???X??X???
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetItemCount(int32 ItemID) const;

    // ???X???信息
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingItemData GetItemInfo(int32 ItemID) const;

    // ????裝??信息
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingEquipmentData GetEquipmentInfo(int32 EquipmentID) const;

    // ???X?用??方
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingCraftingRecipe> GetAvailableRecipes() const;

    // 檢查????條件
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    bool CanCraftRecipe(int32 RecipeID) const;

    // ???X???總價X    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetTotalInventoryValue() const;

    // ???X???容??
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryCapacity() const;

    // ???X???使用X    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryUsed() const;

    // ???X??X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    void SortInventory(EMingItemType SortBy, bool bAscending = true);

    // 保?X??X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool SaveItemData();

    // 載入???X???
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool LoadItemData();

    // 清除????物??數X    UFUNCTION(BlueprintCallable, Category = "Item Manager")
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
    // ???X?????    UPROPERTY()
    TMap<int32, FMingItemData> ItemDatabase;

    // 裝?X?????    UPROPERTY()
    TMap<int32, FMingEquipmentData> EquipmentDatabase;

    // ???X???
    UPROPERTY()
    TArray<FMingItemData> InventoryItems;

    // 已?X?物X    UPROPERTY()
    TMap<FString, FMingEquipmentData> EquippedItems;

    // ???X?方??????    UPROPERTY()
    TMap<int32, FMingCraftingRecipe> CraftingRecipeDatabase;

    // ????容??
    UPROPERTY()
    int32 InventoryCapacity;

    // ????使用X    UPROPERTY()
    int32 InventoryUsed;

    // ??否已??始??
    UPROPERTY()
    bool bInitialized;

private:
    // 載入??設????
    void LoadDefaultItems(};

    // 載入??設??方
    void LoadDefaultRecipes();

    // 驗?X??X???
    bool ValidateItemData(const FMingItemData& Item) const;

    // 驗??裝?X???
    bool ValidateEquipmentData(const FMingEquipmentData& Equipment) const;

    // 計?X???總?X    float CalculateTotalInventoryWeight() const;

    // 計??裝?X???
    TMap<FString, float> CalculateEquipmentBonuses() const;

    // ??用裝?X???
    void ApplyEquipmentEffects(const FMingEquipmentData& Equipment};

    // 移除裝?X???
    void RemoveEquipmentEffects(const FMingEquipmentData& Equipment};

    // ??新????容??使用
    void UpdateInventoryUsage();

    // 檢查????空??
    bool HasInventorySpace(const FMingItemData& Item, int32 Quantity) const;

    // ???X??X???
    FMingItemData GenerateRandomItem(EMingItemRarity Rarity, EMingItemType ItemType) const;

    // 計?X??X???    int32 CalculateItemValue(const FMingItemData& Item) const;

    // ???X???稀??度顏色
    FLinearColor GetRarityColor(EMingItemRarity Rarity) const;

    // ???X???類?X???
    FString GetItemTypeIcon(EMingItemType ItemType) const;

    // ??新???X?????    void UpdateItemDurability(int32 ItemID, float Delta);

    // 檢查???X?否????
    bool IsItemDamaged(int32 ItemID) const;

    // 修復???X???
    void RepairDamagedItems(};

    // ???X??X???
    void AutoOrganizeInventory(};

    // ??併???X???
    void MergeStackableItems(};

    // ??離???X???
    void SplitItemStack(int32 ItemID, int32 SplitQuantity);

    // 計?X??X??X    float CalculateCraftingSuccessRate(int32 RecipeID) const;

    // 消耗?X??X    bool ConsumeCraftingMaterials(int32 RecipeID};

    // 添?X???結??
    bool AddCraftingResults(int32 RecipeID);

    // ???X???統??
    TMap<EMingItemType, int32> GetItemStatistics() const;

    // ????稀??度統??
    TMap<EMingItemRarity, int32> GetRarityStatistics() const;

    // 保?X???快照
    void SaveItemSnapshot(};

    // 載入????快照
    void LoadItemSnapshot(};
);

