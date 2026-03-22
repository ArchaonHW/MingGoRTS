#include "MingCharacterGrowthSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Algo/RandomStream.h"

UMingCharacterGrowthSystem::UMingCharacterGrowthSystem()
{
    bInitialized = false;
    CharacterAttributes = FMingCharacterAttributes();
    CharacterLevel = FMingCharacterLevel();
    SkillDatabase.Empty();
    EquippedItems.Empty();
    InventoryItems.Empty();
    CharacterClass = EMingCharacterClass::Infantry;
    CharacterName = TEXT("");
}

bool UMingCharacterGrowthSystem::InitializeGrowthSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("角色成長系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化角色成長系統..."));

    // 載入預設技能數據
    LoadDefaultSkills();

    // 載入預設裝備數據
    LoadDefaultEquipment();

    // 載入已保存的角色數據
    LoadCharacterData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("角色成長系統初始化完成"));
    
    return true;
}

bool UMingCharacterGrowthSystem::CreateCharacter(const FString& NewCharacterName, EMingCharacterClass NewCharacterClass)
{
    if (NewCharacterName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("角色名稱不能為空"));
        return false;
    }

    CharacterName = NewCharacterName;
    CharacterClass = NewCharacterClass;

    // 初始化角色屬性
    CharacterAttributes = FMingCharacterAttributes();

    // 根據職業調整初始屬性
    switch (NewCharacterClass)
    {
    case EMingCharacterClass::Infantry:
        CharacterAttributes.Strength = 60.0f;
        CharacterAttributes.Agility = 55.0f;
        CharacterAttributes.Constitution = 65.0f;
        CharacterAttributes.CombatSkill = 70.0f;
        break;
        
    case EMingCharacterClass::Cavalry:
        CharacterAttributes.Strength = 55.0f;
        CharacterAttributes.Agility = 70.0f;
        CharacterAttributes.Constitution = 60.0f;
        CharacterAttributes.CombatSkill = 65.0f;
        break;
        
    case EMingCharacterClass::Artillery:
        CharacterAttributes.Intelligence = 70.0f;
        CharacterAttributes.Wisdom = 65.0f;
        CharacterAttributes.Marksmanship = 75.0f;
        CharacterAttributes.CombatSkill = 60.0f;
        break;
        
    case EMingCharacterClass::Commander:
        CharacterAttributes.Leadership = 80.0f;
        CharacterAttributes.Charisma = 75.0f;
        CharacterAttributes.TacticalSense = 70.0f;
        CharacterAttributes.Intelligence = 65.0f;
        break;
        
    case EMingCharacterClass::Diplomat:
        CharacterAttributes.Charisma = 85.0f;
        CharacterAttributes.Diplomacy = 80.0f;
        CharacterAttributes.Negotiation = 75.0f;
        CharacterAttributes.Intelligence = 70.0f;
        break;
        
    default:
        UE_LOG(LogTemp, Warning, TEXT("未知職業類型，使用默認屬性"));
        break;
    }

    // 初始化角色等級
    CharacterLevel = FMingCharacterLevel();
    CharacterLevel.LevelTitle = GetLevelTitle(1);

    // 初始化技能
    InitializeClassSkills();

    // 初始化裝備
    InitializeClassEquipment();

    UE_LOG(LogTemp, Log, TEXT("創建角色: %s (職業: %d)"), *NewCharacterName, (int32)NewCharacterClass);
    
    return true;
}

bool UMingCharacterGrowthSystem::GainExperience(EMingExperienceSource Source, float ExperienceAmount)
{
    if (ExperienceAmount <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("經驗值不能為負數或零"));
        return false;
    }

    float OldExperience = CharacterLevel.CurrentExperience;
    float OldTotalExperience = CharacterLevel.TotalExperience;
    
    CharacterLevel.CurrentExperience += ExperienceAmount;
    CharacterLevel.TotalExperience += ExperienceAmount;

    // 檢查是否可以升級
    bool bLeveledUp = false;
    while (CharacterLevel.CurrentExperience >= CharacterLevel.ExperienceToNextLevel)
    {
        CharacterLevel.CurrentExperience -= CharacterLevel.ExperienceToNextLevel;
        CharacterLevel.CurrentLevel++;
        CharacterLevel.SkillPoints += 2; // 每級獲得2個技能點
        CharacterLevel.AttributePoints += 3; // 每級獲得3個屬性點
        
        // 計算下一級所需經驗
        CharacterLevel.ExperienceToNextLevel = CalculateExperienceToNextLevel(CharacterLevel.CurrentLevel);
        
        // 更新等級標題
        CharacterLevel.LevelTitle = GetLevelTitle(CharacterLevel.CurrentLevel);
        
        // 應用等級獎勵
        ApplyLevelRewards(CharacterLevel.CurrentLevel);
        
        // 解鎖新技能
        UnlockNewSkills(CharacterLevel.CurrentLevel);
        
        bLeveledUp = true;
        
        // 觸發升級事件
        OnCharacterLevelUp.Broadcast(CharacterLevel.CurrentLevel, CharacterAttributes);
    }

    // 觸發經驗獲得事件
    OnExperienceGained.Broadcast(Source, ExperienceAmount);

    UE_LOG(LogTemp, Log, TEXT("角色獲得經驗: %.1f (來源: %d)，當前等級: %d"), 
        ExperienceAmount, (int32)Source, CharacterLevel.CurrentLevel);
    
    return bLeveledUp;
}

bool UMingCharacterGrowthSystem::LevelUpCharacter()
{
    if (CharacterLevel.AttributePoints <= 0 && CharacterLevel.SkillPoints <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("沒有可用的屬性點或技能點"));
        return false;
    }

    // 這裡可以打開升級界面讓玩家分配點數
    UE_LOG(LogTemp, Log, TEXT("角色升級界面打開，可用點數: 屬性 %d, 技能 %d"), 
        CharacterLevel.AttributePoints, CharacterLevel.SkillPoints);
    
    return true;
}

bool UMingCharacterGrowthSystem::AllocateAttributePoint(const FString& AttributeName, int32 Points)
{
    if (CharacterLevel.AttributePoints < Points)
    {
        UE_LOG(LogTemp, Warning, TEXT("屬性點不足"));
        return false;
    }

    float OldValue = 0.0f;
    
    // 根據屬性名稱分配點數
    if (AttributeName == TEXT("Strength"))
    {
        OldValue = CharacterAttributes.Strength;
        CharacterAttributes.Strength += Points * 2.0f; // 每點增加2點屬性值
    }
    else if (AttributeName == TEXT("Agility"))
    {
        OldValue = CharacterAttributes.Agility;
        CharacterAttributes.Agility += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Intelligence"))
    {
        OldValue = CharacterAttributes.Intelligence;
        CharacterAttributes.Intelligence += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Charisma"))
    {
        OldValue = CharacterAttributes.Charisma;
        CharacterAttributes.Charisma += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Constitution"))
    {
        OldValue = CharacterAttributes.Constitution;
        CharacterAttributes.Constitution += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Wisdom"))
    {
        OldValue = CharacterAttributes.Wisdom;
        CharacterAttributes.Wisdom += Points * 2.0f;
    }
    else if (AttributeName == TEXT("CombatSkill"))
    {
        OldValue = CharacterAttributes.CombatSkill;
        CharacterAttributes.CombatSkill += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Leadership"))
    {
        OldValue = CharacterAttributes.Leadership;
        CharacterAttributes.Leadership += Points * 2.0f;
    }
    else if (AttributeName == TEXT("TacticalSense"))
    {
        OldValue = CharacterAttributes.TacticalSense;
        CharacterAttributes.TacticalSense += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Marksmanship"))
    {
        OldValue = CharacterAttributes.Marksmanship;
        CharacterAttributes.Marksmanship += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Diplomacy"))
    {
        OldValue = CharacterAttributes.Diplomacy;
        CharacterAttributes.Diplomacy += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Negotiation"))
    {
        OldValue = CharacterAttributes.Negotiation;
        CharacterAttributes.Negotiation += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Intimidation"))
    {
        OldValue = CharacterAttributes.Intimidation;
        CharacterAttributes.Intimidation += Points * 2.0f;
    }
    else if (AttributeName == TEXT("Persuasion"))
    {
        OldValue = CharacterAttributes.Persuasion;
        CharacterAttributes.Persuasion += Points * 2.0f;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("未知屬性名稱: %s"), *AttributeName);
        return false;
    }

    CharacterLevel.AttributePoints -= Points;

    // 觸發屬性變化事件
    OnAttributeChanged.Broadcast(AttributeName, CharacterAttributes[AttributeName]);

    UE_LOG(LogTemp, Log, TEXT("分配屬性點: %s +%d (%.1f -> %.1f)"), 
        *AttributeName, Points, OldValue, CharacterAttributes[AttributeName]);
    
    return true;
}

bool UMingCharacterGrowthSystem::AllocateSkillPoint(int32 SkillID)
{
    if (CharacterLevel.SkillPoints <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能點不足"));
        return false;
    }

    if (!SkillDatabase.Contains(SkillID))
    {
        UE_LOG(LogTemp, Error, TEXT("技能ID %d 不存在"), SkillID);
        return false;
    }

    FMingSkillData& Skill = SkillDatabase[SkillID];
    
    if (Skill.CurrentLevel >= Skill.MaxLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能 %s 已達最高等級"), *Skill.SkillName);
        return false;
    }

    int32 OldLevel = Skill.CurrentLevel;
    Skill.CurrentLevel++;
    CharacterLevel.SkillPoints--;
    
    // 重新計算升級所需經驗
    Skill.ExperienceToNextLevel = CalculateSkillExperienceToNextLevel(Skill.CurrentLevel);
    
    // 應用技能效果
    ApplySkillEffects(SkillID, Skill.CurrentLevel);

    // 觸發技能升級事件
    OnSkillLevelUp.Broadcast(SkillID, Skill.CurrentLevel);

    UE_LOG(LogTemp, Log, TEXT("技能升級: %s (%d -> %d)"), 
        *Skill.SkillName, OldLevel, Skill.CurrentLevel);
    
    return true;
}

bool UMingCharacterGrowthSystem::LearnSkill(int32 SkillID)
{
    if (!SkillDatabase.Contains(SkillID))
    {
        UE_LOG(LogTemp, Error, TEXT("技能ID %d 不存在"), SkillID);
        return false;
    }

    FMingSkillData& Skill = SkillDatabase[SkillID];
    
    if (Skill.CurrentLevel > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能 %s 已經學習"), *Skill.SkillName);
        return false;
    }

    if (!CanLearnSkill(SkillID))
    {
        UE_LOG(LogTemp, Warning, TEXT("不滿足學習條件"));
        return false;
    }

    Skill.CurrentLevel = 1;
    Skill.ExperienceToNextLevel = CalculateSkillExperienceToNextLevel(1);
    
    // 應用技能效果
    ApplySkillEffects(SkillID, 1);

    UE_LOG(LogTemp, Log, TEXT("學習新技能: %s"), *Skill.SkillName);
    
    return true;
}

bool UMingCharacterGrowthSystem::UpgradeSkill(int32 SkillID)
{
    if (!SkillDatabase.Contains(SkillID))
    {
        UE_LOG(LogTemp, Error, TEXT("技能ID %d 不存在"), SkillID);
        return false;
    }

    FMingSkillData& Skill = SkillDatabase[SkillID];
    
    if (Skill.CurrentLevel >= Skill.MaxLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("技能 %s 已達最高等級"), *Skill.SkillName);
        return false;
    }

    float RequiredExperience = Skill.ExperienceToNextLevel;
    
    // 這裡需要檢查是否有足夠的技能經驗
    // 暫時假設有足夠經驗
    
    Skill.CurrentExperience -= RequiredExperience;
    Skill.CurrentLevel++;
    Skill.ExperienceToNextLevel = CalculateSkillExperienceToNextLevel(Skill.CurrentLevel);
    
    // 應用技能效果
    ApplySkillEffects(SkillID, Skill.CurrentLevel);

    // 觸發技能升級事件
    OnSkillLevelUp.Broadcast(SkillID, Skill.CurrentLevel);

    UE_LOG(LogTemp, Log, TEXT("技能升級: %s -> 等級 %d"), 
        *Skill.SkillName, Skill.CurrentLevel);
    
    return true;
}

FMingCharacterAttributes UMingCharacterGrowthSystem::GetCharacterAttributes() const
{
    return CharacterAttributes;
}

FMingCharacterLevel UMingCharacterGrowthSystem::GetCharacterLevel() const
{
    return CharacterLevel;
}

TArray<FMingSkillData> UMingCharacterGrowthSystem::GetCharacterSkills() const
{
    TArray<FMingSkillData> Skills;
    
    for (const TPair<int32, FMingSkillData>& Pair : SkillDatabase)
    {
        if (Pair.Value.CurrentLevel > 0)
        {
            Skills.Add(Pair.Value);
        }
    }
    
    return Skills;
}

FMingSkillData UMingCharacterGrowthSystem::GetSkill(int32 SkillID) const
{
    if (SkillDatabase.Contains(SkillID))
    {
        return SkillDatabase[SkillID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("技能ID %d 不存在，返回空技能"), SkillID);
    return FMingSkillData();
}

bool UMingCharacterGrowthSystem::EquipItem(int32 ItemID, const FString& EquipSlot)
{
    // 這裡需要從物品系統獲取物品信息
    // 暫時創建一個示例物品
    FMingEquipmentItem Item;
    Item.ItemID = ItemID;
    Item.ItemName = FString::Printf(TEXT("物品 %d"), ItemID);
    Item.ItemType = TEXT("weapon");
    Item.EquipSlot = EquipSlot;
    Item.bIsEquipped = true;
    Item.ItemLevel = 1;
    Item.ItemRarity = TEXT("普通");
    Item.ItemDurability = 100.0f;
    Item.MaxDurability = 100.0f;

    // 檢查是否已有物品裝備在該位置
    if (EquippedItems.Contains(EquipSlot))
    {
        // 先卸下原有物品
        UnequipItem(EquipSlot);
    }

    // 裝備新物品
    EquippedItems.Add(EquipSlot, Item);

    // 應用裝備效果
    ApplyEquipmentEffects(Item);

    // 觸發裝備事件
    OnItemEquipped.Broadcast(ItemID, EquipSlot);

    UE_LOG(LogTemp, Log, TEXT("裝備物品: %s 到位置 %s"), *Item.ItemName, *EquipSlot);
    
    return true;
}

bool UMingCharacterGrowthSystem::UnequipItem(const FString& EquipSlot)
{
    if (!EquippedItems.Contains(EquipSlot))
    {
        UE_LOG(LogTemp, Warning, TEXT("位置 %s 沒有裝備物品"), *EquipSlot);
        return false;
    }

    FMingEquipmentItem& Item = EquippedItems[EquipSlot];
    Item.bIsEquipped = false;

    // 移除裝備效果
    RemoveEquipmentEffects(Item);

    // 從裝備列表移除
    EquippedItems.Remove(EquipSlot);

    // 添加到背包
    InventoryItems.Add(Item);

    UE_LOG(LogTemp, Log, TEXT("卸下物品: %s 從位置 %s"), *Item.ItemName, *EquipSlot);
    
    return true;
}

TArray<FMingEquipmentItem> UMingCharacterGrowthSystem::GetEquippedItems() const
{
    TArray<FMingEquipmentItem> Equipped;
    
    for (const TPair<FString, FMingEquipmentItem>& Pair : EquippedItems)
    {
        if (Pair.Value.bIsEquipped)
        {
            Equipped.Add(Pair.Value);
        }
    }
    
    return Equipped;
}

float UMingCharacterGrowthSystem::CalculateCombatPower() const
{
    float BasePower = 0.0f;
    
    // 基礎戰鬥力計算
    BasePower += CharacterAttributes.Strength * 0.8f;
    BasePower += CharacterAttributes.Agility * 0.6f;
    BasePower += CharacterAttributes.Constitution * 0.5f;
    BasePower += CharacterAttributes.CombatSkill * 1.2f;
    BasePower += CharacterAttributes.Marksmanship * 1.0f;
    
    // 裝備加成
    for (const TPair<FString, FMingEquipmentItem>& Pair : EquippedItems)
    {
        const FMingEquipmentItem& Item = Pair.Value;
        if (Item.bIsEquipped)
        {
            for (const TPair<FString, float>& Attr : Item.ItemAttributes)
            {
                if (Attr.Key == TEXT("AttackPower"))
                {
                    BasePower += Attr.Value;
                }
                else if (Attr.Key == TEXT("CombatSkill"))
                {
                    BasePower += Attr.Value * 0.5f;
                }
            }
        }
    }
    
    // 技能加成
    for (const TPair<int32, FMingSkillData>& Pair : SkillDatabase)
    {
        const FMingSkillData& Skill = Pair.Value;
        if (Skill.CurrentLevel > 0 && Skill.SkillType == EMingSkillType::Combat)
        {
            BasePower += Skill.CurrentLevel * 10.0f;
        }
    }
    
    return BasePower;
}

float UMingCharacterGrowthSystem::CalculateOverallPower() const
{
    float CombatPower = CalculateCombatPower();
    float LeadershipPower = 0.0f;
    float DiplomacyPower = 0.0f;
    float UtilityPower = 0.0f;
    
    // 領導力計算
    LeadershipPower += CharacterAttributes.Leadership * 1.5f;
    LeadershipPower += CharacterAttributes.Charisma * 0.8f;
    LeadershipPower += CharacterAttributes.TacticalSense * 1.2f;
    
    // 外交力計算
    DiplomacyPower += CharacterAttributes.Diplomacy * 1.5f;
    DiplomacyPower += CharacterAttributes.Negotiation * 1.2f;
    DiplomacyPower += CharacterAttributes.Persuasion * 1.0f;
    DiplomacyPower += CharacterAttributes.Intimidation * 0.8f;
    
    // 實用能力計算
    UtilityPower += CharacterAttributes.Intelligence * 1.0f;
    UtilityPower += CharacterAttributes.Wisdom * 0.8f;
    
    // 綜合實力
    float OverallPower = (CombatPower * 0.4f) + 
                        (LeadershipPower * 0.3f) + 
                        (DiplomacyPower * 0.2f) + 
                        (UtilityPower * 0.1f);
    
    return OverallPower;
}

bool UMingCharacterGrowthSystem::SaveCharacterData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("CharacterGrowth.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("角色成長數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("角色: %s, 等級: %d, 總經驗: %.1f"), 
        *CharacterName, CharacterLevel.CurrentLevel, CharacterLevel.TotalExperience);
    
    return true;
}

bool UMingCharacterGrowthSystem::LoadCharacterData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("CharacterGrowth.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入角色成長數據"), *LoadPath);
    
    return true;
}

void UMingCharacterGrowthSystem::ResetCharacter()
{
    CharacterAttributes = FMingCharacterAttributes();
    CharacterLevel = FMingCharacterLevel();
    SkillDatabase.Empty();
    EquippedItems.Empty();
    InventoryItems.Empty();
    
    // 重新初始化
    InitializeClassSkills();
    InitializeClassEquipment();
    
    UE_LOG(LogTemp, Log, TEXT("角色已重置"));
}

void UMingCharacterGrowthSystem::LoadDefaultSkills()
{
    // 戰鬥技能
    FMingSkillData BasicCombat;
    BasicCombat.SkillID = 1;
    BasicCombat.SkillName = TEXT("基礎戰鬥");
    BasicCombat.SkillDescription = TEXT("基本的戰鬥技能，提升近戰攻擊力");
    BasicCombat.SkillType = EMingSkillType::Combat;
    BasicCombat.CurrentLevel = 1;
    BasicCombat.MaxLevel = 10;
    BasicCombat.CurrentExperience = 0.0f;
    BasicCombat.ExperienceToNextLevel = 100.0f;
    BasicCombat.SkillEffects.Add(TEXT("AttackPower"), 5.0f);
    BasicCombat.SkillIconPath = TEXT("Icons/Skills/BasicCombat.png");
    BasicCombat.bIsActiveSkill = false;
    SkillDatabase.Add(1, BasicCombat);
    
    // 領導技能
    FMingSkillData Leadership;
    Leadership.SkillID = 2;
    Leadership.SkillName = TEXT("領導力");
    Leadership.SkillDescription = TEXT("提升部隊士氣和戰鬥效率");
    Leadership.SkillType = EMingSkillType::Leadership;
    Leadership.CurrentLevel = 0;
    Leadership.MaxLevel = 10;
    Leadership.CurrentExperience = 0.0f;
    Leadership.ExperienceToNextLevel = 150.0f;
    Leadership.SkillEffects.Add(TEXT("Morale"), 10.0f);
    Leadership.SkillEffects.Add(TEXT("UnitEfficiency"), 5.0f);
    Leadership.SkillIconPath = TEXT("Icons/Skills/Leadership.png");
    Leadership.bIsActiveSkill = false;
    SkillDatabase.Add(2, Leadership);
    
    // 射擊技能
    FMingSkillData Marksmanship;
    Marksmanship.SkillID = 3;
    Marksmanship.SkillName = TEXT("射擊精度");
    Marksmanship.SkillDescription = TEXT("提升遠程武器命中率");
    Marksmanship.SkillType = EMingSkillType::Combat;
    Marksmanship.CurrentLevel = 1;
    Marksmanship.MaxLevel = 10;
    Marksmanship.CurrentExperience = 0.0f;
    Marksmanship.ExperienceToNextLevel = 120.0f;
    Marksmanship.SkillEffects.Add(TEXT("Accuracy"), 8.0f);
    Marksmanship.SkillIconPath = TEXT("Icons/Skills/Marksmanship.png");
    Marksmanship.bIsActiveSkill = false;
    SkillDatabase.Add(3, Marksmanship);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 個預設技能"), SkillDatabase.Num());
}

void UMingCharacterGrowthSystem::LoadDefaultEquipment()
{
    // 基礎武器
    FMingEquipmentItem BasicRifle;
    BasicRifle.ItemID = 1;
    BasicRifle.ItemName = TEXT("漢陽八八式步槍");
    BasicRifle.ItemDescription = TEXT("民國時期標準制式步槍");
    BasicRifle.ItemType = TEXT("weapon");
    BasicRifle.ItemRarity = TEXT("普通");
    BasicRifle.ItemLevel = 1;
    BasicRifle.EquipSlot = TEXT("MainHand");
    BasicRifle.ItemAttributes.Add(TEXT("AttackPower"), 15.0f);
    BasicRifle.ItemAttributes.Add(TEXT("Accuracy"), 5.0f);
    BasicRifle.ItemDurability = 100.0f;
    BasicRifle.MaxDurability = 100.0f;
    BasicRifle.ItemIconPath = TEXT("Icons/Items/BasicRifle.png");
    BasicRifle.ItemModelPath = TEXT("Models/Weapons/BasicRifle.fbx");
    EquippedItems.Add(TEXT("MainHand"), BasicRifle);
    
    // 基礎裝甲
    FMingEquipmentItem BasicArmor;
    BasicArmor.ItemID = 2;
    BasicArmor.ItemName = TEXT("軍用制服");
    BasicArmor.ItemDescription = TEXT("基礎的軍用制服，提供少量防護");
    BasicArmor.ItemType = TEXT("armor");
    BasicArmor.ItemRarity = TEXT("普通");
    BasicArmor.ItemLevel = 1;
    BasicArmor.EquipSlot = TEXT("Body");
    BasicArmor.ItemAttributes.Add(TEXT("Defense"), 10.0f);
    BasicArmor.ItemAttributes.Add(TEXT("Durability"), 5.0f);
    BasicArmor.ItemDurability = 100.0f;
    BasicArmor.MaxDurability = 100.0f;
    BasicArmor.ItemIconPath = TEXT("Icons/Items/BasicArmor.png");
    BasicArmor.ItemModelPath = TEXT("Models/Armor/BasicUniform.fbx");
    EquippedItems.Add(TEXT("Body"), BasicArmor);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 件預設裝備"), EquippedItems.Num());
}

float UMingCharacterGrowthSystem::CalculateExperienceToNextLevel(int32 CurrentLevel) const
{
    return 100.0f * FMath::Pow(1.2f, CurrentLevel - 1);
}

float UMingCharacterGrowthSystem::CalculateSkillExperienceToNextLevel(int32 CurrentLevel) const
{
    return 100.0f * FMath::Pow(1.3f, CurrentLevel - 1);
}

void UMingCharacterGrowthSystem::ApplyLevelRewards(int32 NewLevel)
{
    // 根據等級給予獎勵
    if (NewLevel % 5 == 0) // 每5級給予特殊獎勵
    {
        CharacterLevel.SkillPoints += 5; // 額外技能點
        CharacterLevel.AttributePoints += 5; // 額外屬性點
        
        UE_LOG(LogTemp, Log, TEXT("等級 %d 達成，獲得額外獎勵"), NewLevel);
    }
    
    // 解鎖新內容
    if (NewLevel >= 10)
    {
        // 解鎖高級內容
        UE_LOG(LogTemp, Log, TEXT("解鎖高級內容"));
    }
}

void UMingCharacterGrowthSystem::ApplySkillEffects(int32 SkillID, int32 NewLevel)
{
    if (!SkillDatabase.Contains(SkillID))
    {
        return;
    }

    const FMingSkillData& Skill = SkillDatabase[SkillID];
    
    // 應用技能效果到角色屬性
    for (const TPair<FString, float>& Effect : Skill.SkillEffects)
    {
        float EffectValue = Effect.Value * NewLevel;
        
        if (Effect.Key == TEXT("AttackPower"))
        {
            CharacterAttributes.CombatSkill += EffectValue * 0.1f;
        }
        else if (Effect.Key == TEXT("Morale"))
        {
            CharacterAttributes.Leadership += EffectValue * 0.1f;
        }
        else if (Effect.Key == TEXT("Accuracy"))
        {
            CharacterAttributes.Marksmanship += EffectValue * 0.1f;
        }
    }
    
    UpdateCharacterPower();
}

void UMingCharacterGrowthSystem::ApplyEquipmentEffects(const FMingEquipmentItem& Item)
{
    // 應用裝備效果到角色屬性
    for (const TPair<FString, float>& Effect : Item.ItemAttributes)
    {
        if (Effect.Key == TEXT("AttackPower"))
        {
            CharacterAttributes.CombatSkill += Effect.Value * 0.2f;
        }
        else if (Effect.Key == TEXT("Defense"))
        {
            CharacterAttributes.Constitution += Effect.Value * 0.1f;
        }
        else if (Effect.Key == TEXT("Accuracy"))
        {
            CharacterAttributes.Marksmanship += Effect.Value * 0.15f;
        }
    }
    
    UpdateCharacterPower();
}

void UMingCharacterGrowthSystem::RemoveEquipmentEffects(const FMingEquipmentItem& Item)
{
    // 移除裝備效果
    for (const TPair<FString, float>& Effect : Item.ItemAttributes)
    {
        if (Effect.Key == TEXT("AttackPower"))
        {
            CharacterAttributes.CombatSkill -= Effect.Value * 0.2f;
        }
        else if (Effect.Key == TEXT("Defense"))
        {
            CharacterAttributes.Constitution -= Effect.Value * 0.1f;
        }
        else if (Effect.Key == TEXT("Accuracy"))
        {
            CharacterAttributes.Marksmanship -= Effect.Value * 0.15f;
        }
    }
    
    UpdateCharacterPower();
}

bool UMingCharacterGrowthSystem::CanLearnSkill(int32 SkillID) const
{
    if (!SkillDatabase.Contains(SkillID))
    {
        return false;
    }

    const FMingSkillData& Skill = SkillDatabase[SkillID];
    
    // 檢查前置技能
    for (int32 PrereqSkillID : Skill.PrerequisiteSkills)
    {
        if (!SkillDatabase.Contains(PrereqSkillID) || 
            SkillDatabase[PrereqSkillID].CurrentLevel <= 0)
        {
            return false;
        }
    }
    
    // 檢查等級要求
    for (const FString& Condition : Skill.UnlockConditions)
    {
        if (Condition.Contains(TEXT("level")))
        {
            TArray<FString> Parts;
            Condition.ParseIntoArray(Parts, TEXT(" "));
            if (Parts.Num() >= 2)
            {
                int32 RequiredLevel = FCString::Atoi(*Parts[1]);
                if (CharacterLevel.CurrentLevel < RequiredLevel)
                {
                    return false;
                }
            }
        }
    }
    
    return true;
}

float UMingCharacterGrowthSystem::CalculateAttributeModifier(const FString& AttributeName) const
{
    float Modifier = 1.0f;
    
    // 計算來自裝備和技能的屬性修正
    // 這裡可以添加更複雜的計算邏輯
    
    return Modifier;
}

FMingEquipmentItem UMingCharacterGrowthSystem::GenerateRandomEquipment(int32 ItemLevel) const
{
    FMingEquipmentItem RandomItem;
    RandomItem.ItemID = FMath::RandRange(1000, 9999);
    RandomItem.ItemLevel = ItemLevel;
    RandomItem.ItemDurability = 100.0f;
    RandomItem.MaxDurability = 100.0f;
    
    // 隨機生成物品類型
    TArray<FString> ItemTypes = {TEXT("weapon"), TEXT("armor"), TEXT("accessory")};
    RandomItem.ItemType = ItemTypes[FMath::RandRange(0, ItemTypes.Num() - 1)];
    
    // 隨機生成稀有度
    TArray<FString> Rarities = {TEXT("普通"), TEXT("精良"), TEXT("史詩")};
    RandomItem.ItemRarity = Rarities[FMath::RandRange(0, Rarities.Num() - 1)];
    
    return RandomItem;
}

void UMingCharacterGrowthSystem::UpdateCharacterPower()
{
    // 更新角色的戰鬥力
    // 這裡可以添加更多的計算邏輯
}

void UMingCharacterGrowthSystem::SaveCharacterSnapshot()
{
    // 保存角色快照
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("CharacterSnapshot.json");
    
    UE_LOG(LogTemp, Log, TEXT("保存角色快照到: %s"), *SavePath);
}

void UMingCharacterGrowthSystem::LoadCharacterSnapshot()
{
    // 載入角色快照
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("CharacterSnapshot.json");
    
    UE_LOG(LogTemp, Log, TEXT("從 %s 載入角色快照"), *LoadPath);
}

bool UMingCharacterGrowthSystem::ValidateCharacterData() const
{
    if (CharacterName.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("角色名稱不能為空"));
        return false;
    }
    
    if (CharacterLevel.CurrentLevel <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("角色等級必須大於0"));
        return false;
    }
    
    return true;
}

TArray<int32> UMingCharacterGrowthSystem::CalculateSkillTree() const
{
    TArray<int32> AvailableSkills;
    
    for (const TPair<int32, FMingSkillData>& Pair : SkillDatabase)
    {
        int32 SkillID = Pair.Key;
        const FMingSkillData& Skill = Pair.Value;
        
        if (CanLearnSkill(SkillID))
        {
            AvailableSkills.Add(SkillID);
        }
    }
    
    return AvailableSkills;
}

void UMingCharacterGrowthSystem::UnlockNewSkills(int32 NewLevel)
{
    // 根據等級解鎖新技能
    for (const TPair<int32, FMingSkillData>& Pair : SkillDatabase)
    {
        int32 SkillID = Pair.Key;
        const FMingSkillData& Skill = Pair.Value;
        
        // 檢查是否可以解鎖
        for (const FString& Condition : Skill.UnlockConditions)
        {
            if (Condition.Contains(TEXT("level")))
            {
                TArray<FString> Parts;
                Condition.ParseIntoArray(Parts, TEXT(" "));
                if (Parts.Num() >= 2)
                {
                    int32 RequiredLevel = FCString::Atoi(*Parts[1]);
                    if (NewLevel >= RequiredLevel && Skill.CurrentLevel == 0)
                    {
                        UE_LOG(LogTemp, Log, TEXT("解鎖新技能: %s"), *Skill.SkillName);
                    }
                }
            }
        }
    }
}

TMap<FString, float> UMingCharacterGrowthSystem::CalculateClassBonuses() const
{
    TMap<FString, float> Bonuses;
    
    // 根據職業給予加成
    switch (CharacterClass)
    {
    case EMingCharacterClass::Infantry:
        Bonuses.Add(TEXT("CombatSkill"), 10.0f);
        Bonuses.Add(TEXT("Constitution"), 5.0f);
        break;
        
    case EMingCharacterClass::Cavalry:
        Bonuses.Add(TEXT("Agility"), 15.0f);
        Bonuses.Add(TEXT("CombatSkill"), 5.0f);
        break;
        
    case EMingCharacterClass::Commander:
        Bonuses.Add(TEXT("Leadership"), 20.0f);
        Bonuses.Add(TEXT("Charisma"), 10.0f);
        break;
        
    case EMingCharacterClass::Diplomat:
        Bonuses.Add(TEXT("Diplomacy"), 15.0f);
        Bonuses.Add(TEXT("Negotiation"), 10.0f);
        break;
        
    default:
        break;
    }
    
    return Bonuses;
}

void UMingCharacterGrowthSystem::InitializeClassSkills()
{
    // 根據職業初始化技能
    switch (CharacterClass)
    {
    case EMingCharacterClass::Infantry:
        LearnSkill(1); // 基礎戰鬥
        LearnSkill(3); // 射擊精度
        break;
        
    case EMingCharacterClass::Commander:
        LearnSkill(2); // 領導力
        LearnSkill(1); // 基礎戰鬥
        break;
        
    default:
        LearnSkill(1); // 所有職業都有基礎戰鬥
        break;
    }
}

void UMingCharacterGrowthSystem::InitializeClassEquipment()
{
    // 根據職業初始化裝備
    switch (CharacterClass)
    {
    case EMingCharacterClass::Infantry:
        EquipItem(1, TEXT("MainHand")); // 基礎步槍
        EquipItem(2, TEXT("Body")); // 軍用制服
        break;
        
    case EMingCharacterClass::Commander:
        EquipItem(1, TEXT("MainHand")); // 指揮官武器
        EquipItem(2, TEXT("Body")); // 指揮官制服
        break;
        
    default:
        EquipItem(1, TEXT("MainHand")); // 默認裝備
        EquipItem(2, TEXT("Body"));
        break;
    }
}

FString UMingCharacterGrowthSystem::GetLevelTitle(int32 Level) const
{
    if (Level <= 5)
    {
        return TEXT("新兵");
    }
    else if (Level <= 10)
    {
        return TEXT("老兵");
    }
    else if (Level <= 15)
    {
        return TEXT("精銳");
    }
    else if (Level <= 20)
    {
        return TEXT("士官");
    }
    else if (Level <= 25)
    {
        return TEXT("尉官");
    }
    else if (Level <= 30)
    {
        return TEXT("校官");
    }
    else if (Level <= 35)
    {
        return TEXT("將軍");
    }
    else
    {
        return TEXT("元帥");
    }
}
