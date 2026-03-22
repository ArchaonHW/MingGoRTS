#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCharacterGrowthSystem.h"
#include "MingItemManager.generated.h"

// ?©Â?Á®Ä?âÂ∫¶
UENUM(BlueprintType)
enum class EMingItemRarity : uint8
{
    Common,            // ?ÆÈÄ?    Uncommon,          // ‰∏çÂ∏∏Ë¶?    Rare,              // Á®Ä??    Epic,              // ?≤Ë©©
    Legendary           // ?≥Â?
};

// ?©Â?È°ûÂ?
UENUM(BlueprintType)
enum class EMingItemType : uint8
{
    Weapon,            // Ê≠¶Âô®
    Armor,             // Ë≠∑Áî≤
    Accessory,         // ?çÈ£æ
    Consumable,        // Ê∂àËÄóÂ?
    Material,          // ?êÊ?
    Blueprint,         // ?çÂ?
    Currency,          // Ë≤®Âπ£
    Quest,             // ‰ªªÂ??©Â?
    Cosmetic           // Â§ñË??©Â?
};

// Ë£ùÂ?ÊßΩ‰?
UENUM(BlueprintType)
enum class EMingEquipSlot : uint8
{
    MainHand,          // ‰∏ªÊ?
    OffHand,           // ?ØÊ?
    Head,              // ?≠ÈÉ®
    Body,              // Ë∫´È?
    Hands,             // ?ãÈÉ®
    Legs,              // ?øÈÉ®
    Feet,              // ?≥ÈÉ®
    Neck,              // ?∏ÈÉ®
    Ring1,             // ?íÊ?1
    Ring2,             // ?íÊ?2
    Trinket1,          // È£æÁâ©1
    Trinket2,          // È£æÁâ©2
    Backpack           // ?åÂ?
};

// ?©Â?Âº∑Â?È°ûÂ?
UENUM(BlueprintType)
enum class EMingEnhancementType : uint8
{
    None,               // ?°Âº∑??    Sharp,             // ?íÂà©
    Durable,           // ?ê‰?
    Powerful,          // Â®ÅÂ?
    Swift,             // ËøÖÊç∑
    Magical,           // È≠îÊ?
    Blessed,           // Á•ùÁ?
    Cursed             // Ë©õÂ?
};

// ?©Â??∏Ê?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingItemData
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
    EMingItemType ItemType;

    // ?©Â?Á®Ä?âÂ∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity ItemRarity;

    // ?©Â?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemLevel;

    // ?©Â??πÂÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemValue;

    // ?©Â??çÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ItemWeight;

    // ?©Â??ÜÁ??∏È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackSize;

    // ?∂Â??ÜÁ??∏È?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentStack;

    // ?©Â?Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ItemAttributes;

    // Ë£ùÂ?ÊßΩ‰?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // ?ØÂê¶Â∑≤Ë???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // ?©Â??ê‰?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // ?ÄÂ§ßËÄê‰?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // ?©Â?Âº∑Â?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEnhancementType EnhancementType;

    // Âº∑Â?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancementLevel;

    // ?©Â??ñÊ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemIconPath;

    // ?©Â?3DÊ®°Â?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemModelPath;

    // ?©Â??êË≥™Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemMaterialPath;

    // ?©Â??πÊ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemEffectPath;

    // ?©Â??≥Ê?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemSoundPath;

    // ?ØÂê¶?∫ÂîØ‰∏Ä?©Â?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsUnique;

    // ?ØÂê¶?∫Á?ÂÆöÁâ©??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsSoulbound;

    // ?©Â?Ê®ôÁ±§
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ItemTags;

    // ?©Â??≤Â??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AcquiredTime;

    // ?©Â?‰æÜÊ?
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

// Ë£ùÂ??∏Ê?
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingEquipmentData
{
    GENERATED_BODY()

    // Ë£ùÂ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentID;

    // Ë£ùÂ??çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentName;

    // Ë£ùÂ??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentDescription;

    // Ë£ùÂ?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemType EquipmentType;

    // Ë£ùÂ?Á®Ä?âÂ∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity EquipmentRarity;

    // Ë£ùÂ?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EquipmentLevel;

    // Ë£ùÂ?Â±¨ÊÄ?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> EquipmentAttributes;

    // Ë£ùÂ?ÊßΩ‰?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEquipSlot EquipSlot;

    // ?ØÂê¶Â∑≤Ë???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsEquipped;

    // Ë£ùÂ??ê‰?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentDurability;

    // ?ÄÂ§ßËÄê‰?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDurability;

    // Ë£ùÂ?Âº∑Â?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingEnhancementType EnhancementType;

    // Âº∑Â?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EnhancementLevel;

    // Ë£ùÂ??ñÊ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentIconPath;

    // Ë£ùÂ?3DÊ®°Â?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentModelPath;

    // Ë£ùÂ??êË≥™Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentMaterialPath;

    // Ë£ùÂ??πÊ?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentEffectPath;

    // Ë£ùÂ??≥Ê?Ë∑ØÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EquipmentSoundPath;

    // Ë£ùÂ??ÄÊ±?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> EquipmentRequirements;

    // Ë£ùÂ??≤Â??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AcquiredTime;

    // Ë£ùÂ?‰æÜÊ?
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

// ?©Â??àÊ??çÊñπ
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingCraftingRecipe
{
    GENERATED_BODY()

    // ?çÊñπID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RecipeID;

    // ?çÊñπ?çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeName;

    // ?çÊñπ?èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RecipeDescription;

    // ?Ä?Ä?êÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> RequiredMaterials;

    // ?àÊ?ÁµêÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingItemData> ResultItems;

    // ?àÊ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CraftingTime;

    // ?àÊ??üÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;

    // ?çÊñπÁ®Ä?âÂ∫¶
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingItemRarity RecipeRarity;

    // ?çÊñπ?ÄÊ±ÇÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> RequiredSkills;

    // ?çÊñπ?ÄÊ±ÇÁ?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredLevel;

    // ?çÊñπ?ñÊ?Ë∑ØÂ?
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

// ?©Â?ÁÆ°Á?‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemAcquired, const FMingItemData&, Item, int32, Quantity};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemLost, const FMingItemData&, Item, int32, Quantity};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEquipped, const FMingEquipmentData&, Equipment, const FString&, Slot};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUnequipped, const FMingEquipmentData&, Equipment, const FString&, Slot};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemEnhanced, const FMingEquipmentData&, Equipment, EMingEnhancementType, EnhancementType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemCrafted, const FMingCraftingRecipe&, Recipe, const TArray<FMingItemData>&, Results};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemRepaired, const FMingEquipmentData&, Equipment, float, RepairAmount};

/**
 * ?©Â?ÁÆ°Á??? * Ë≤†Ë≤¨ÁÆ°Á??ã‰∫∫Ë£ùÂ??åÁâ©?ÅÁ≥ªÁµ? */
UCLASS(ClassGroup = (Personal), Blueprintable, BlueprintType)
class MINGPERSONAL_API UMingItemManager : public UObject
{
    GENERATED_BODY()

public:
    UMingItemManager(};

    // ?ùÂ??ñÁâ©?ÅÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool InitializeItemSystem(};

    // Ê∑ªÂ??©Â??∞Ë???    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool AddItemToInventory(const FMingItemData& Item, int32 Quantity = 1};

    // ÂæûË??ÖÁßª?§Áâ©??    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool RemoveItemFromInventory(int32 ItemID, int32 Quantity = 1};

    // Ë£ùÂ??©Â?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool EquipItem(int32 ItemID, const FString& EquipSlot};

    // ?∏‰??©Â?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool UnequipItem(const FString& EquipSlot};

    // ?≤Â??åÂ??©Â?
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingItemData> GetInventoryItems() const;

    // ?≤Â?Â∑≤Ë??ôÁâ©??    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingEquipmentData> GetEquippedItems() const;

    // ?≤Â??áÂ?ÊßΩ‰??©Â?
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingEquipmentData GetEquippedItem(const FString& EquipSlot) const;

    // Ê™¢Êü•?ØÂê¶?Ø‰ª•Ë£ùÂ?
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    bool CanEquipItem(int32 ItemID, const FString& EquipSlot) const;

    // Âº∑Â??©Â?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool EnhanceItem(int32 ItemID, EMingEnhancementType EnhancementType};

    // ‰øÆÁ??©Â?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool RepairItem(int32 ItemID, float RepairAmount = 100.0f};

    // ?àÊ??©Â?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool CraftItem(int32 RecipeID};

    // ?ÜËß£?©Â?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool DisassembleItem(int32 ItemID};

    // ?≤Â??©Â??∏È?
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetItemCount(int32 ItemID) const;

    // ?≤Â??©Â?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingItemData GetItemInfo(int32 ItemID) const;

    // ?≤Â?Ë£ùÂ?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    FMingEquipmentData GetEquipmentInfo(int32 EquipmentID) const;

    // ?≤Â??ØÁî®?çÊñπ
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    TArray<FMingCraftingRecipe> GetAvailableRecipes() const;

    // Ê™¢Êü•?àÊ?Ê¢ù‰ª∂
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    bool CanCraftRecipe(int32 RecipeID) const;

    // ?≤Â??©Â?Á∏ΩÂÉπ??    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetTotalInventoryValue() const;

    // ?≤Â??åÂ?ÂÆπÈ?
    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryCapacity() const;

    // ?≤Â??åÂ?‰ΩøÁî®??    UFUNCTION(BlueprintPure, Category = "Item Manager")
    int32 GetInventoryUsed() const;

    // ?íÂ??åÂ??©Â?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    void SortInventory(EMingItemType SortBy, bool bAscending = true};

    // ‰øùÂ??©Â??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool SaveItemData(};

    // ËºâÂÖ•?©Â??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    bool LoadItemData(};

    // Ê∏ÖÈô§?Ä?âÁâ©?ÅÊï∏??    UFUNCTION(BlueprintCallable, Category = "Item Manager")
    void ClearAllItemData(};

    // ‰∫ã‰ª∂ÂßîË?
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
    // ?©Â??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingItemData> ItemDatabase;

    // Ë£ùÂ??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingEquipmentData> EquipmentDatabase;

    // ?åÂ??©Â?
    UPROPERTY()
    TArray<FMingItemData> InventoryItems;

    // Â∑≤Ë??ôÁâ©??    UPROPERTY()
    TMap<FString, FMingEquipmentData> EquippedItems;

    // ?àÊ??çÊñπ?∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingCraftingRecipe> CraftingRecipeDatabase;

    // ?åÂ?ÂÆπÈ?
    UPROPERTY()
    int32 InventoryCapacity;

    // ?åÂ?‰ΩøÁî®??    UPROPERTY()
    int32 InventoryUsed;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠?©Â?
    void LoadDefaultItems(};

    // ËºâÂÖ•?êË®≠?çÊñπ
    void LoadDefaultRecipes(};

    // È©óË??©Â??∏Ê?
    bool ValidateItemData(const FMingItemData& Item) const;

    // È©óË?Ë£ùÂ??∏Ê?
    bool ValidateEquipmentData(const FMingEquipmentData& Equipment) const;

    // Ë®àÁ??©Â?Á∏ΩÈ???    float CalculateTotalInventoryWeight() const;

    // Ë®àÁ?Ë£ùÂ??†Ê?
    TMap<FString, float> CalculateEquipmentBonuses() const;

    // ?âÁî®Ë£ùÂ??àÊ?
    void ApplyEquipmentEffects(const FMingEquipmentData& Equipment};

    // ÁßªÈô§Ë£ùÂ??àÊ?
    void RemoveEquipmentEffects(const FMingEquipmentData& Equipment};

    // ?¥Êñ∞?åÂ?ÂÆπÈ?‰ΩøÁî®
    void UpdateInventoryUsage(};

    // Ê™¢Êü•?åÂ?Á©∫È?
    bool HasInventorySpace(const FMingItemData& Item, int32 Quantity) const;

    // ?üÊ??®Ê??©Â?
    FMingItemData GenerateRandomItem(EMingItemRarity Rarity, EMingItemType ItemType) const;

    // Ë®àÁ??©Â??πÂÄ?    int32 CalculateItemValue(const FMingItemData& Item) const;

    // ?≤Â??©Â?Á®Ä?âÂ∫¶È°èËâ≤
    FLinearColor GetRarityColor(EMingItemRarity Rarity) const;

    // ?≤Â??©Â?È°ûÂ??ñÊ?
    FString GetItemTypeIcon(EMingItemType ItemType) const;

    // ?¥Êñ∞?©Â??ê‰?Â∫?    void UpdateItemDurability(int32 ItemID, float Delta};

    // Ê™¢Êü•?©Â??ØÂê¶?çÂ?
    bool IsItemDamaged(int32 ItemID) const;

    // ‰øÆÂæ©?çÂ??©Â?
    void RepairDamagedItems(};

    // ?™Â??¥Á??åÂ?
    void AutoOrganizeInventory(};

    // ?à‰Ωµ?∏Â??©Â?
    void MergeStackableItems(};

    // ?ÜÈõ¢?©Â??ÜÁ?
    void SplitItemStack(int32 ItemID, int32 SplitQuantity};

    // Ë®àÁ??àÊ??êÂ???    float CalculateCraftingSuccessRate(int32 RecipeID) const;

    // Ê∂àËÄóÂ??êÊ???    bool ConsumeCraftingMaterials(int32 RecipeID};

    // Ê∑ªÂ??àÊ?ÁµêÊ?
    bool AddCraftingResults(int32 RecipeID};

    // ?≤Â??©Â?Áµ±Ë?
    TMap<EMingItemType, int32> GetItemStatistics() const;

    // ?≤Â?Á®Ä?âÂ∫¶Áµ±Ë?
    TMap<EMingItemRarity, int32> GetRarityStatistics() const;

    // ‰øùÂ??©Â?Âø´ÁÖß
    void SaveItemSnapshot(};

    // ËºâÂÖ•?©Â?Âø´ÁÖß
    void LoadItemSnapshot(};
};

