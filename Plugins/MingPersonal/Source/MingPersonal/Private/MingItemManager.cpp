#include "MingItemManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Algo/RandomStream.h"

UMingItemManager::UMingItemManager()
{
    bInitialized = false;
    ItemDatabase.Empty();
    EquipmentDatabase.Empty();
    InventoryItems.Empty();
    EquippedItems.Empty();
    CraftingRecipeDatabase.Empty();
    InventoryCapacity = 100;
    InventoryUsed = 0;
}

bool UMingItemManager::InitializeItemSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("物品系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化物品系統..."));

    // 載入預設物品
    LoadDefaultItems();

    // 載入預設配方
    LoadDefaultRecipes();

    // 載入已保存的物品數據
    LoadItemData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("物品系統初始化完成"));
    
    return true;
}

bool UMingItemManager::AddItemToInventory(const FMingItemData& Item, int32 Quantity)
{
    if (!ValidateItemData(Item))
    {
        UE_LOG(LogTemp, Error, TEXT("物品數據驗證失敗"));
        return false;
    }

    if (!HasInventorySpace(Item, Quantity))
    {
        UE_LOG(LogTemp, Warning, TEXT("背包空間不足"));
        return false;
    }

    // 檢查是否已存在相同物品
    int32 ExistingItemID = -1;
    for (const FMingItemData& InventoryItem : InventoryItems)
    {
        if (InventoryItem.ItemID == Item.ItemID)
        {
            ExistingItemID = InventoryItem.ItemID;
            break;
        }
    }

    if (ExistingItemID != -1)
    {
        // 合併堆疊物品
        for (FMingItemData& InventoryItem : InventoryItems)
        {
            if (InventoryItem.ItemID == Item.ItemID)
            {
                InventoryItem.CurrentStack += Quantity;
                break;
            }
        }
    }
    else
    {
        // 添加新物品
        FMingItemData NewItem = Item;
        NewItem.CurrentStack = Quantity;
        NewItem.AcquiredTime = FDateTime::Now();
        NewItem.ItemSource = TEXT("獲取");
        InventoryItems.Add(NewItem);
    }

    // 更新背包使用量
    UpdateInventoryUsage();

    // 觸發物品獲取事件
    OnItemAcquired.Broadcast(Item, Quantity);

    UE_LOG(LogTemp, Log, TEXT("添加物品到背包: %s x%d"), *Item.ItemName, Quantity);
    
    return true;
}

bool UMingItemManager::RemoveItemFromInventory(int32 ItemID, int32 Quantity)
{
    int32 ItemIndex = -1;
    for (int32 i = 0; i < InventoryItems.Num(); i++)
    {
        if (InventoryItems[i].ItemID == ItemID)
        {
            ItemIndex = i;
            break;
        }
    }

    if (ItemIndex == -1)
    {
        UE_LOG(LogTemp, Warning, TEXT("物品ID %d 不存在於背包中"), ItemID);
        return false;
    }

    FMingItemData& Item = InventoryItems[ItemIndex];
    
    if (Item.CurrentStack < Quantity)
    {
        UE_LOG(LogTemp, Warning, TEXT("物品數量不足"));
        return false;
    }

    Item.CurrentStack -= Quantity;

    // 如果數量為0，從背包移除
    if (Item.CurrentStack <= 0)
    {
        InventoryItems.RemoveAt(ItemIndex);
    }

    // 更新背包使用量
    UpdateInventoryUsage();

    // 觸發物品遺失事件
    OnItemLost.Broadcast(Item, Quantity);

    UE_LOG(LogTemp, Log, TEXT("從背包移除物品: %s x%d"), *Item.ItemName, Quantity);
    
    return true;
}

bool UMingItemManager::EquipItem(int32 ItemID, const FString& EquipSlot)
{
    if (!CanEquipItem(ItemID, EquipSlot))
    {
        UE_LOG(LogTemp, Warning, TEXT("無法裝備物品 %d 到槽位 %s"), ItemID, *EquipSlot);
        return false;
    }

    // 獲取物品信息
    FMingItemData ItemData = GetItemInfo(ItemID);
    if (ItemData.ItemID == -1)
    {
        UE_LOG(LogTemp, Error, TEXT("物品ID %d 不存在"), ItemID);
        return false;
    }

    // 檢查該槽位是否已有物品
    if (EquippedItems.Contains(EquipSlot))
    {
        // 先卸下原有物品
        UnequipItem(EquipSlot);
    }

    // 創建裝備數據
    FMingEquipmentData Equipment;
    Equipment.EquipmentID = ItemID;
    Equipment.EquipmentName = ItemData.ItemName;
    Equipment.EquipmentDescription = ItemData.ItemDescription;
    Equipment.EquipmentType = ItemData.ItemType;
    Equipment.EquipmentRarity = ItemData.ItemRarity;
    Equipment.EquipmentLevel = ItemData.ItemLevel;
    Equipment.EquipmentAttributes = ItemData.ItemAttributes;
    Equipment.EquipSlot = EquipSlot;
    Equipment.bIsEquipped = true;
    Equipment.CurrentDurability = ItemData.CurrentDurability;
    Equipment.MaxDurability = ItemData.MaxDurability;
    Equipment.EnhancementType = ItemData.EnhancementType;
    Equipment.EnhancementLevel = ItemData.EnhancementLevel;
    Equipment.EquipmentIconPath = ItemData.ItemIconPath;
    Equipment.EquipmentModelPath = ItemData.ItemModelPath;
    Equipment.EquipmentMaterialPath = ItemData.ItemMaterialPath;
    Equipment.EquipmentEffectPath = ItemData.ItemEffectPath;
    Equipment.EquipmentSoundPath = ItemData.ItemSoundPath;
    Equipment.AcquiredTime = ItemData.AcquiredTime;
    Equipment.EquipmentSource = ItemData.ItemSource;

    // 添加到裝備數據庫
    EquipmentDatabase.Add(ItemID, Equipment);
    EquippedItems.Add(EquipSlot, Equipment);

    // 從背包移除物品
    RemoveItemFromInventory(ItemID, 1);

    // 應用裝備效果
    ApplyEquipmentEffects(Equipment);

    // 觸發裝備事件
    OnItemEquipped.Broadcast(Equipment, EquipSlot);

    UE_LOG(LogTemp, Log, TEXT("裝備物品: %s 到槽位 %s"), *Equipment.EquipmentName, *EquipSlot);
    
    return true;
}

bool UMingItemManager::UnequipItem(const FString& EquipSlot)
{
    if (!EquippedItems.Contains(EquipSlot))
    {
        UE_LOG(LogTemp, Warning, TEXT("槽位 %s 沒有裝備物品"), *EquipSlot);
        return false;
    }

    FMingEquipmentData& Equipment = EquippedItems[EquipSlot];
    int32 ItemID = Equipment.EquipmentID;

    // 移除裝備效果
    RemoveEquipmentEffects(Equipment);

    // 轉換為物品數據
    FMingItemData ItemData;
    ItemData.ItemID = Equipment.EquipmentID;
    ItemData.ItemName = Equipment.EquipmentName;
    ItemData.ItemDescription = Equipment.EquipmentDescription;
    ItemData.ItemType = Equipment.EquipmentType;
    ItemData.ItemRarity = Equipment.EquipmentRarity;
    ItemData.ItemLevel = Equipment.EquipmentLevel;
    ItemData.ItemValue = CalculateItemValue(ItemData);
    ItemData.ItemWeight = 0.0f; // 裝備重量
    ItemData.StackSize = 1;
    ItemData.CurrentStack = 1;
    ItemData.ItemAttributes = Equipment.EquipmentAttributes;
    ItemData.EquipSlot = Equipment.EquipSlot;
    ItemData.bIsEquipped = false;
    ItemData.CurrentDurability = Equipment.CurrentDurability;
    ItemData.MaxDurability = Equipment.MaxDurability;
    ItemData.EnhancementType = Equipment.EnhancementType;
    ItemData.EnhancementLevel = Equipment.EnhancementLevel;
    ItemData.ItemIconPath = Equipment.EquipmentIconPath;
    ItemData.ItemModelPath = Equipment.EquipmentModelPath;
    ItemData.ItemMaterialPath = Equipment.EquipmentMaterialPath;
    ItemData.ItemEffectPath = Equipment.EquipmentEffectPath;
    ItemData.ItemSoundPath = Equipment.EquipmentSoundPath;
    ItemData.bIsUnique = false;
    ItemData.bIsSoulbound = false;
    ItemData.ItemTags.Empty();
    ItemData.AcquiredTime = Equipment.AcquiredTime;
    ItemData.ItemSource = Equipment.EquipmentSource;

    // 添加回背包
    AddItemToInventory(ItemData, 1);

    // 從裝備數據庫移除
    EquipmentDatabase.Remove(ItemID);
    EquippedItems.Remove(EquipSlot);

    // 觸發卸下事件
    OnItemUnequipped.Broadcast(Equipment, EquipSlot);

    UE_LOG(LogTemp, Log, TEXT("卸下物品: %s 從槽位 %s"), *Equipment.EquipmentName, *EquipSlot);
    
    return true;
}

TArray<FMingItemData> UMingItemManager::GetInventoryItems() const
{
    return InventoryItems;
}

TArray<FMingEquipmentData> UMingItemManager::GetEquippedItems() const
{
    TArray<FMingEquipmentData> Equipped;
    
    for (const TPair<FString, FMingEquipmentData>& Pair : EquippedItems)
    {
        Equipped.Add(Pair.Value);
    }
    
    return Equipped;
}

FMingEquipmentData UMingItemManager::GetEquippedItem(const FString& EquipSlot) const
{
    if (EquippedItems.Contains(EquipSlot))
    {
        return EquippedItems[EquipSlot];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("槽位 %s 沒有裝備物品"), *EquipSlot);
    return FMingEquipmentData();
}

bool UMingItemManager::CanEquipItem(int32 ItemID, const FString& EquipSlot) const
{
    FMingItemData ItemData = GetItemInfo(ItemID);
    if (ItemData.ItemID == -1)
    {
        return false;
    }

    // 檢查物品類型是否匹配槽位
    if (ItemData.ItemType == EMingItemType::Weapon && EquipSlot != TEXT("MainHand"))
    {
        return false;
    }

    // 檢查稀有度限制
    if (ItemData.ItemRarity == EMingItemRarity::Legendary && !EquippedItems.Contains(EquipSlot))
    {
        return true; // 傳奇物品可以裝備任何槽位
    }

    // 檢查等級要求
    if (ItemData.ItemLevel > 10) // 假設最高等級要求
    {
        return false;
    }

    return true;
}

bool UMingItemManager::EnhanceItem(int32 ItemID, EMingEnhancementType EnhancementType)
{
    if (!EquipmentDatabase.Contains(ItemID))
    {
        UE_LOG(LogTemp, Warning, TEXT("物品ID %d 不存在於裝備中"), ItemID);
        return false;
    }

    FMingEquipmentData& Equipment = EquipmentDatabase[ItemID];
    
    if (Equipment.EnhancementLevel >= 5) // 最高強化等級
    {
        UE_LOG(LogTemp, Warning, TEXT("物品已達最高強化等級"));
        return false;
    }

    // 更新強化信息
    Equipment.EnhancementType = EnhancementType;
    Equipment.EnhancementLevel++;

    // 應用強化效果
    switch (EnhancementType)
    {
    case EMingEnhancementType::Sharp:
        Equipment.EquipmentAttributes.Add(TEXT("AttackPower"), 10.0f * Equipment.EnhancementLevel);
        break;
    case EMingEnhancementType::Durable:
        Equipment.MaxDurability += 50.0f * Equipment.EnhancementLevel;
        Equipment.CurrentDurability = Equipment.MaxDurability;
        break;
    case EMingEnhancementType::Powerful:
        Equipment.EquipmentAttributes.Add(TEXT("Damage"), 15.0f * Equipment.EnhancementLevel);
        break;
    case EMingEnhancementType::Swift:
        Equipment.EquipmentAttributes.Add(TEXT("AttackSpeed"), 5.0f * Equipment.EnhancementLevel);
        break;
    default:
        break;
    }

    // 重新應用裝備效果
    ApplyEquipmentEffects(Equipment);

    // 觸發強化事件
    OnItemEnhanced.Broadcast(Equipment, EnhancementType);

    UE_LOG(LogTemp, Log, TEXT("強化物品: %s 到等級 %d"), *Equipment.EquipmentName, Equipment.EnhancementLevel);
    
    return true;
}

bool UMingItemManager::RepairItem(int32 ItemID, float RepairAmount)
{
    if (!EquipmentDatabase.Contains(ItemID))
    {
        UE_LOG(LogTemp, Warning, TEXT("物品ID %d 不存在於裝備中"), ItemID);
        return false;
    }

    FMingEquipmentData& Equipment = EquipmentDatabase[ItemID];
    
    if (Equipment.CurrentDurability >= Equipment.MaxDurability)
    {
        UE_LOG(LogTemp, Warning, TEXT("物品耐久度已滿"));
        return false;
    }

    // 修理物品
    float OldDurability = Equipment.CurrentDurability;
    Equipment.CurrentDurability = FMath::Min(Equipment.MaxDurability, Equipment.CurrentDurability + RepairAmount);

    // 觸發修理事件
    OnItemRepaired.Broadcast(Equipment, Equipment.CurrentDurability - OldDurability);

    UE_LOG(LogTemp, Log, TEXT("修理物品: %s (%.1f -> %.1f)"), 
        *Equipment.EquipmentName, OldDurability, Equipment.CurrentDurability);
    
    return true;
}

bool UMingItemManager::CraftItem(int32 RecipeID)
{
    if (!CraftingRecipeDatabase.Contains(RecipeID))
    {
        UE_LOG(LogTemp, Warning, TEXT("配方ID %d 不存在"), RecipeID);
        return false;
    }

    const FMingCraftingRecipe& Recipe = CraftingRecipeDatabase[RecipeID];
    
    if (!CanCraftRecipe(RecipeID))
    {
        UE_LOG(LogTemp, Warning, TEXT("不滿足合成條件"));
        return false;
    }

    // 消耗材料
    if (!ConsumeCraftingMaterials(RecipeID))
    {
        UE_LOG(LogTemp, Warning, TEXT("材料不足"));
        return false;
    }

    // 計算成功率
    float SuccessRate = CalculateCraftingSuccessRate(RecipeID);
    
    // 隨機判定是否成功
    bool bSuccess = (FMath::FRand() < SuccessRate);
    
    if (bSuccess)
    {
        // 添加合成結果
        AddCraftingResults(RecipeID);
        
        // 觸發合成事件
        OnItemCrafted.Broadcast(Recipe, Recipe.ResultItems);
        
        UE_LOG(LogTemp, Log, TEXT("合成成功: %s"), *Recipe.RecipeName);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("合成失敗: %s"), *Recipe.RecipeName);
    }
    
    return bSuccess;
}

bool UMingItemManager::DisassembleItem(int32 ItemID)
{
    FMingItemData ItemData = GetItemInfo(ItemID);
    if (ItemData.ItemID == -1)
    {
        UE_LOG(LogTemp, Warning, TEXT("物品ID %d 不存在"), ItemID);
        return false;
    }

    // 檢查是否可以分解
    if (ItemData.ItemType != EMingItemType::Weapon && 
        ItemData.ItemType != EMingItemType::Armor)
    {
        UE_LOG(LogTemp, Warning, TEXT("該物品類型無法分解"));
        return false;
    }

    // 移除物品
    RemoveItemFromInventory(ItemID, 1);

    // 生成分解材料
    TArray<FMingItemData> Materials;
    
    // 基於稀有度生成材料
    switch (ItemData.ItemRarity)
    {
    case EMingItemRarity::Common:
        // 普通物品分解為基礎材料
        break;
    case EMingItemRarity::Uncommon:
        // 不常見物品分解為稀有材料
        break;
    case EMingItemRarity::Rare:
        // 稀有物品分解為史詩材料
        break;
    default:
        break;
    }

    // 添加材料到背包
    for (const FMingItemData& Material : Materials)
    {
        AddItemToInventory(Material, 1);
    }

    UE_LOG(LogTemp, Log, TEXT("分解物品: %s"), *ItemData.ItemName);
    
    return true;
}

int32 UMingItemManager::GetItemCount(int32 ItemID) const
{
    for (const FMingItemData& Item : InventoryItems)
    {
        if (Item.ItemID == ItemID)
        {
            return Item.CurrentStack;
        }
    }
    
    return 0;
}

FMingItemData UMingItemManager::GetItemInfo(int32 ItemID) const
{
    for (const FMingItemData& Item : InventoryItems)
    {
        if (Item.ItemID == ItemID)
        {
            return Item;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("物品ID %d 不存在於背包中"), ItemID);
    return FMingItemData();
}

FMingEquipmentData UMingItemManager::GetEquipmentInfo(int32 EquipmentID) const
{
    if (EquipmentDatabase.Contains(EquipmentID))
    {
        return EquipmentDatabase[EquipmentID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("裝備ID %d 不存在"), EquipmentID);
    return FMingEquipmentData();
}

TArray<FMingCraftingRecipe> UMingItemManager::GetAvailableRecipes() const
{
    TArray<FMingCraftingRecipe> AvailableRecipes;
    
    for (const TPair<int32, FMingCraftingRecipe>& Pair : CraftingRecipeDatabase)
    {
        if (CanCraftRecipe(Pair.Key))
        {
            AvailableRecipes.Add(Pair.Value);
        }
    }
    
    return AvailableRecipes;
}

bool UMingItemManager::CanCraftRecipe(int32 RecipeID) const
{
    if (!CraftingRecipeDatabase.Contains(RecipeID))
    {
        return false;
    }

    const FMingCraftingRecipe& Recipe = CraftingRecipeDatabase[RecipeID];
    
    // 檢查材料
    for (const FMingItemData& Material : Recipe.RequiredMaterials)
    {
        if (GetItemCount(Material.ItemID) < Material.CurrentStack)
        {
            return false;
        }
    }
    
    // 檢查技能等級
    if (Recipe.RequiredLevel > 10) // 假設玩家等級
    {
        return false;
    }
    
    return true;
}

int32 UMingItemManager::GetTotalInventoryValue() const
{
    int32 TotalValue = 0;
    
    for (const FMingItemData& Item : InventoryItems)
    {
        TotalValue += CalculateItemValue(Item) * Item.CurrentStack;
    }
    
    return TotalValue;
}

int32 UMingItemManager::GetInventoryCapacity() const
{
    return InventoryCapacity;
}

int32 UMingItemManager::GetInventoryUsed() const
{
    return InventoryUsed;
}

void UMingItemManager::SortInventory(EMingItemType SortBy, bool bAscending)
{
    // 根據物品類型排序
    InventoryItems.Sort([SortBy, bAscending](const FMingItemData& A, const FMingItemData& B)
    {
        if (A.ItemType == SortBy && B.ItemType != SortBy)
        {
            return bAscending ? -1 : 1;
        }
        else if (A.ItemType != SortBy && B.ItemType == SortBy)
        {
            return bAscending ? 1 : -1;
        }
        else
        {
            return A.ItemName.Compare(B.ItemName);
        }
    });
    
    UE_LOG(LogTemp, Log, TEXT("背包已排序"));
}

bool UMingItemManager::SaveItemData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("ItemData.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("物品數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個物品，%d 個裝備，%d 個配方"), 
        InventoryItems.Num(), EquippedItems.Num(), CraftingRecipeDatabase.Num());
    
    return true;
}

bool UMingItemManager::LoadItemData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("ItemData.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入物品數據"), *LoadPath);
    
    return true;
}

void UMingItemManager::ClearAllItemData()
{
    ItemDatabase.Empty();
    EquipmentDatabase.Empty();
    InventoryItems.Empty();
    EquippedItems.Empty();
    CraftingRecipeDatabase.Empty();
    InventoryUsed = 0;
    
    UE_LOG(LogTemp, Log, TEXT("所有物品數據已清除"));
}

void UMingItemManager::LoadDefaultItems()
{
    // 創建一些預設物品
    
    // 漢陽八八式步槍
    FMingItemData Rifle;
    Rifle.ItemID = 1001;
    Rifle.ItemName = TEXT("漢陽八八式步槍");
    Rifle.ItemDescription = TEXT("民國時期標準制式步槍，性能可靠");
    Rifle.ItemType = EMingItemType::Weapon;
    Rifle.ItemRarity = EMingItemRarity::Common;
    Rifle.ItemLevel = 1;
    Rifle.ItemValue = 100;
    Rifle.ItemWeight = 4.5f;
    Rifle.StackSize = 1;
    Rifle.CurrentStack = 1;
    Rifle.ItemAttributes.Add(TEXT("AttackPower"), 25.0f);
    Rifle.ItemAttributes.Add(TEXT("Accuracy"), 0.7f);
    Rifle.ItemAttributes.Add(TEXT("Range"), 200.0f);
    Rifle.EquipSlot = EMingEquipSlot::MainHand;
    Rifle.bIsEquipped = false;
    Rifle.CurrentDurability = 100.0f;
    Rifle.MaxDurability = 100.0f;
    Rifle.EnhancementType = EMingEnhancementType::None;
    Rifle.EnhancementLevel = 0;
    Rifle.ItemIconPath = TEXT("Icons/Items/Rifle_88.png");
    Rifle.ItemModelPath = TEXT("Models/Weapons/Rifle_88.fbx");
    Rifle.ItemMaterialPath = TEXT("Materials/Weapons/Rifle_88.mat");
    Rifle.ItemEffectPath = TEXT("Effects/Weapons/Rifle_88.ni");
    Rifle.ItemSoundPath = TEXT("Sounds/Weapons/Rifle_88.wav");
    Rifle.bIsUnique = false;
    Rifle.bIsSoulbound = false;
    Rifle.ItemTags.Add(TEXT("weapon"));
    Rifle.ItemTags.Add(TEXT("rifle"));
    Rifle.ItemTags.Add(TEXT("chinese"));
    Rifle.AcquiredTime = FDateTime::Now();
    Rifle.ItemSource = TEXT("預設");
    ItemDatabase.Add(Rifle.ItemID, Rifle);
    
    // 軍用制服
    FMingItemData Uniform;
    Uniform.ItemID = 1002;
    Uniform.ItemName = TEXT("軍用制服");
    Uniform.ItemDescription = TEXT("標準軍用制服，提供基礎防護");
    Uniform.ItemType = EMingItemType::Armor;
    Uniform.ItemRarity = EMingItemRarity::Common;
    Uniform.ItemLevel = 1;
    Uniform.ItemValue = 150;
    Uniform.ItemWeight = 2.0f;
    Uniform.StackSize = 1;
    Uniform.CurrentStack = 1;
    Uniform.ItemAttributes.Add(TEXT("Defense"), 15.0f);
    Uniform.ItemAttributes.Add(TEXT("Durability"), 20.0f);
    Uniform.EquipSlot = EMingEquipSlot::Body;
    Uniform.bIsEquipped = false;
    Uniform.CurrentDurability = 100.0f;
    Uniform.MaxDurability = 100.0f;
    Uniform.EnhancementType = EMingEnhancementType::None;
    Uniform.EnhancementLevel = 0;
    Uniform.ItemIconPath = TEXT("Icons/Items/Uniform.png");
    Uniform.ItemModelPath = TEXT("Models/Armor/Uniform.fbx");
    Uniform.ItemMaterialPath = TEXT("Materials/Armor/Uniform.mat");
    Uniform.ItemEffectPath = TEXT("Effects/Armor/Uniform.ni");
    Uniform.ItemSoundPath = TEXT("Sounds/Armor/Uniform.wav");
    Uniform.bIsUnique = false;
    Uniform.bIsSoulbound = false;
    Uniform.ItemTags.Add(TEXT("armor"));
    Uniform.ItemTags.Add(TEXT("uniform"));
    Uniform.ItemTags.Add(TEXT("chinese"));
    Uniform.AcquiredTime = FDateTime::Now();
    Uniform.ItemSource = TEXT("預設"));
    ItemDatabase.Add(Uniform.ItemID, Uniform);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設物品"), ItemDatabase.Num());
}

void UMingItemManager::LoadDefaultRecipes()
{
    // 創建一些預設配方
    
    // 步槍彈藥合成
    FMingCraftingRecipe AmmoRecipe;
    AmmoRecipe.RecipeID = 2001;
    AmmoRecipe.RecipeName = TEXT("步槍彈藥");
    AmmoRecipe.RecipeDescription = TEXT("合成5.56mm步槍彈藥");
    AmmoRecipe.CraftingTime = 5.0f;
    AmmoRecipe.SuccessRate = 0.9f;
    AmmoRecipe.RecipeRarity = EMingItemRarity::Common;
    AmmoRecipe.RequiredLevel = 1;
    AmmoRecipe.RecipeIconPath = TEXT("Icons/Recipes/Ammo.png");
    
    // 添加所需材料
    FMingItemData Material1;
    Material1.ItemID = 3001;
    Material1.ItemName = TEXT("銅殼");
    Material1.CurrentStack = 5;
    AmmoRecipe.RequiredMaterials.Add(Material1);
    
    FMingItemData Material2;
    Material2.ItemID = 3002;
    Material2.ItemName = TEXT("火藥");
    Material2.CurrentStack = 3;
    AmmoRecipe.RequiredMaterials.Add(Material2);
    
    // 添加結果
    FMingItemData Result;
    Result.ItemID = 4001;
    Result.ItemName = TEXT("5.56mm彈藥");
    Result.CurrentStack = 20;
    AmmoRecipe.ResultItems.Add(Result);
    
    CraftingRecipeDatabase.Add(AmmoRecipe.RecipeID, AmmoRecipe);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設配方"), CraftingRecipeDatabase.Num());
}

bool UMingItemManager::ValidateItemData(const FMingItemData& Item) const
{
    if (Item.ItemID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("物品ID必須大於0"));
        return false;
    }
    
    if (Item.ItemName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("物品名稱不能為空"));
        return false;
    }
    
    if (Item.ItemWeight < 0.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("物品重量不能為負數"));
        return false;
    }
    
    return true;
}

bool UMingItemManager::ValidateEquipmentData(const FMingEquipmentData& Equipment) const
{
    if (Equipment.EquipmentID <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("裝備ID必須大於0"));
        return false;
    }
    
    if (Equipment.EquipmentName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("裝備名稱不能為空"));
        return false;
    }
    
    return true;
}

float UMingItemManager::CalculateTotalInventoryWeight() const
{
    float TotalWeight = 0.0f;
    
    for (const FMingItemData& Item : InventoryItems)
    {
        TotalWeight += Item.ItemWeight * Item.CurrentStack;
    }
    
    return TotalWeight;
}

TMap<FString, float> UMingItemManager::CalculateEquipmentBonuses() const
{
    TMap<FString, float> Bonuses;
    
    for (const TPair<FString, FMingEquipmentData>& Pair : EquippedItems)
    {
        const FMingEquipmentData& Equipment = Pair.Value;
        
        for (const TPair<FString, float>& Attribute : Equipment.EquipmentAttributes)
        {
            if (Bonuses.Contains(Attribute.Key))
            {
                Bonuses[Attribute.Key] += Attribute.Value;
            }
            else
            {
                Bonuses.Add(Attribute.Key, Attribute.Value);
            }
        }
    }
    
    return Bonuses;
}

void UMingItemManager::ApplyEquipmentEffects(const FMingEquipmentData& Equipment)
{
    // 這裡需要與角色成長系統集成
    // 應用裝備加成到角色屬性
    
    UE_LOG(LogTemp, Log, TEXT("應用裝備效果: %s"), *Equipment.EquipmentName);
}

void UMingItemManager::RemoveEquipmentEffects(const FMingEquipmentData& Equipment)
{
    // 這裡需要與角色成長系統集成
    // 移除裝備加成從角色屬性
    
    UE_LOG(LogTemp, Log, TEXT("移除裝備效果: %s"), *Equipment.EquipmentName);
}

void UMingItemManager::UpdateInventoryUsage()
{
    InventoryUsed = 0;
    
    for (const FMingItemData& Item : InventoryItems)
    {
        InventoryUsed += Item.CurrentStack;
    }
}

bool UMingItemManager::HasInventorySpace(const FMingItemData& Item, int32 Quantity) const
{
    return (InventoryUsed + Quantity) <= InventoryCapacity;
}

int32 UMingItemManager::CalculateItemValue(const FMingItemData& Item) const
{
    // 基礎價值計算
    int32 BaseValue = Item.ItemValue;
    
    // 稀有度加成
    switch (Item.ItemRarity)
    {
    case EMingItemRarity::Uncommon:
        BaseValue *= 2;
        break;
    case EMingItemRarity::Rare:
        BaseValue *= 5;
        break;
    case EMingItemRarity::Epic:
        BaseValue *= 10;
        break;
    case EMingItemRarity::Legendary:
        BaseValue *= 20;
        break;
    default:
        break;
    }
    
    // 等級加成
    BaseValue *= Item.ItemLevel;
    
    return BaseValue;
}

FLinearColor UMingItemManager::GetRarityColor(EMingItemRarity Rarity) const
{
    switch (Rarity)
    {
    case EMingItemRarity::Common:
        return FLinearColor::Gray;
    case EMingItemRarity::Uncommon:
        return FLinearColor::Green;
    case EMingItemRarity::Rare:
        return FLinearColor::Blue;
    case EMingItemRarity::Epic:
        return FLinearColor::Purple;
    case EMingItemRarity::Legendary:
        return FLinearColor::Orange;
    default:
        return FLinearColor::White;
    }
}

FString UMingItemManager::GetItemTypeIcon(EMingItemType ItemType) const
{
    switch (ItemType)
    {
    case EMingItemType::Weapon:
        return TEXT("Icons/Types/Weapon.png");
    case EMingItemType::Armor:
        return TEXT("Icons/Types/Armor.png");
    case EMingItemType::Accessory:
        return TEXT("Icons/Types/Accessory.png");
    case EMingItemType::Consumable:
        return TEXT("Icons/Types/Consumable.png");
    case EMingItemType::Material:
        return TEXT("Icons/Types/Material.png");
    default:
        return TEXT("Icons/Types/Default.png");
    }
}

float UMingItemManager::CalculateCraftingSuccessRate(int32 RecipeID) const
{
    if (!CraftingRecipeDatabase.Contains(RecipeID))
    {
        return 0.0f;
    }

    const FMingCraftingRecipe& Recipe = CraftingRecipeDatabase[RecipeID];
    
    // 基礎成功率
    float BaseRate = Recipe.SuccessRate;
    
    // 技能等級影響
    // 這裡可以根據玩家技能等級調整成功率
    
    return FMath::Clamp(BaseRate, 0.0f, 1.0f);
}

bool UMingItemManager::ConsumeCraftingMaterials(int32 RecipeID)
{
    if (!CraftingRecipeDatabase.Contains(RecipeID))
    {
        return false;
    }

    const FMingCraftingRecipe& Recipe = CraftingRecipeDatabase[RecipeID];
    
    // 消耗所有材料
    for (const FMingItemData& Material : Recipe.RequiredMaterials)
    {
        if (!RemoveItemFromInventory(Material.ItemID, Material.CurrentStack))
        {
            return false;
        }
    }
    
    return true;
}

bool UMingItemManager::AddCraftingResults(int32 RecipeID)
{
    if (!CraftingRecipeDatabase.Contains(RecipeID))
    {
        return false;
    }

    const FMingCraftingRecipe& Recipe = CraftingRecipeDatabase[RecipeID];
    
    // 添加所有結果物品
    for (const FMingItemData& Result : Recipe.ResultItems)
    {
        if (!AddItemToInventory(Result, Result.CurrentStack))
        {
            return false;
        }
    }
    
    return true;
}

TMap<EMingItemType, int32> UMingItemManager::GetItemStatistics() const
{
    TMap<EMingItemType, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingItemType::Weapon, 0);
    Statistics.Add(EMingItemType::Armor, 0);
    Statistics.Add(EMingItemType::Accessory, 0);
    Statistics.Add(EMingItemType::Consumable, 0);
    Statistics.Add(EMingItemType::Material, 0);
    Statistics.Add(EMingItemType::Blueprint, 0);
    Statistics.Add(EMingItemType::Currency, 0);
    Statistics.Add(EMingItemType::Quest, 0);
    Statistics.Add(EMingItemType::Cosmetic, 0);
    
    // 統計物品類型
    for (const FMingItemData& Item : InventoryItems)
    {
        int32& Count = Statistics[Item.ItemType];
        Count++;
    }
    
    return Statistics;
}

TMap<EMingItemRarity, int32> UMingItemManager::GetRarityStatistics() const
{
    TMap<EMingItemRarity, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingItemRarity::Common, 0);
    Statistics.Add(EMingItemRarity::Uncommon, 0);
    Statistics.Add(EMingItemRarity::Rare, 0);
    Statistics.Add(EMingItemRarity::Epic, 0);
    Statistics.Add(EMingItemRarity::Legendary, 0);
    
    // 統計稀有度
    for (const FMingItemData& Item : InventoryItems)
    {
        int32& Count = Statistics[Item.ItemRarity];
        Count++;
    }
    
    return Statistics;
}

void UMingItemManager::SaveItemSnapshot()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("ItemSnapshot.json");
    
    UE_LOG(LogTemp, Log, TEXT("保存物品快照到: %s"), *SavePath);
}

void UMingItemManager::LoadItemSnapshot()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("ItemSnapshot.json");
    
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入物品快照"), *LoadPath);
}
