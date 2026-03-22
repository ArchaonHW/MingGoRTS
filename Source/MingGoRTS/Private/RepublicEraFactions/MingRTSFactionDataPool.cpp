// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力共享数据池系统实现

#include "RepublicEraFactions/MingRTSFactionDataPool.h"

DEFINE_LOG_CATEGORY_STATIC(LogFactionDataPool, Log, All);

UMingRTSFactionDataPool::UMingRTSFactionDataPool()
{
}

void UMingRTSFactionDataPool::InitializeDataPool()
{
    UE_LOG(LogFactionDataPool, Log, TEXT("Initializing Faction Data Pool..."));
    
    // 注册常用单位模板
    RegisterCommonUnitTemplates();
    
    // 注册常用机制模板
    RegisterCommonMechanicTemplates();
    
    // 标记常用模板（不清理）
    PreloadCommonTemplates();
    
    UE_LOG(LogFactionDataPool, Log, TEXT("Data Pool initialized with %d unit templates and %d mechanic templates"),
        UnitTemplatePool.Num(), MechanicTemplatePool.Num());
}

void UMingRTSFactionDataPool::RegisterCommonUnitTemplates()
{
    // 注册基础步兵模板
    {
        FSharedUnitTemplate InfantryTemplate;
        InfantryTemplate.TemplateID = FName("Template_Infantry_Basic");
        InfantryTemplate.UnitName = FText::FromString(TEXT("基础步兵"));
        InfantryTemplate.Description = FText::FromString(TEXT("标准步兵单位"));
        InfantryTemplate.UnitType = EFactionUnitType::Infantry;
        InfantryTemplate.BaseStats.AttackPower = 100;
        InfantryTemplate.BaseStats.DefensePower = 100;
        InfantryTemplate.BaseStats.MovementSpeed = 90;
        InfantryTemplate.BaseStats.HealthPoints = 100;
        InfantryTemplate.BaseStats.ProductionCost = 100;
        InfantryTemplate.BaseStats.UpkeepCost = 10;
        InfantryTemplate.BaseStats.TrainingTime = 8.0f;
        InfantryTemplate.Traits.Add(EUnitTrait::Infantry);
        RegisterUnitTemplate(InfantryTemplate);
    }
    
    // 注册精英步兵模板
    {
        FSharedUnitTemplate EliteInfantryTemplate;
        EliteInfantryTemplate.TemplateID = FName("Template_Infantry_Elite");
        EliteInfantryTemplate.UnitName = FText::FromString(TEXT("精英步兵"));
        EliteInfantryTemplate.Description = FText::FromString(TEXT("训练有素的精英步兵"));
        EliteInfantryTemplate.UnitType = EFactionUnitType::Infantry;
        EliteInfantryTemplate.BaseStats.AttackPower = 130;
        EliteInfantryTemplate.BaseStats.DefensePower = 120;
        EliteInfantryTemplate.BaseStats.MovementSpeed = 95;
        EliteInfantryTemplate.BaseStats.HealthPoints = 130;
        EliteInfantryTemplate.BaseStats.ProductionCost = 150;
        EliteInfantryTemplate.BaseStats.UpkeepCost = 15;
        EliteInfantryTemplate.BaseStats.TrainingTime = 12.0f;
        EliteInfantryTemplate.Traits.Add(EUnitTrait::Infantry);
        EliteInfantryTemplate.Traits.Add(EUnitTrait::Veteran);
        RegisterUnitTemplate(EliteInfantryTemplate);
    }
    
    // 注册基础骑兵模板
    {
        FSharedUnitTemplate CavalryTemplate;
        CavalryTemplate.TemplateID = FName("Template_Cavalry_Basic");
        CavalryTemplate.UnitName = FText::FromString(TEXT("基础骑兵"));
        CavalryTemplate.Description = FText::FromString(TEXT("标准骑兵单位"));
        CavalryTemplate.UnitType = EFactionUnitType::Cavalry;
        CavalryTemplate.BaseStats.AttackPower = 120;
        CavalryTemplate.BaseStats.DefensePower = 80;
        CavalryTemplate.BaseStats.MovementSpeed = 140;
        CavalryTemplate.BaseStats.HealthPoints = 90;
        CavalryTemplate.BaseStats.ProductionCost = 120;
        CavalryTemplate.BaseStats.UpkeepCost = 12;
        CavalryTemplate.BaseStats.TrainingTime = 10.0f;
        CavalryTemplate.Traits.Add(EUnitTrait::CavalryCharge);
        RegisterUnitTemplate(CavalryTemplate);
    }
    
    // 注册防御步兵模板
    {
        FSharedUnitTemplate DefensiveTemplate;
        DefensiveTemplate.TemplateID = FName("Template_Infantry_Defensive");
        DefensiveTemplate.UnitName = FText::FromString(TEXT("防御步兵"));
        DefensiveTemplate.Description = FText::FromString(TEXT("专精防守的步兵单位"));
        DefensiveTemplate.UnitType = EFactionUnitType::Infantry;
        DefensiveTemplate.BaseStats.AttackPower = 90;
        DefensiveTemplate.BaseStats.DefensePower = 130;
        DefensiveTemplate.BaseStats.MovementSpeed = 80;
        DefensiveTemplate.BaseStats.HealthPoints = 120;
        DefensiveTemplate.BaseStats.ProductionCost = 110;
        DefensiveTemplate.BaseStats.UpkeepCost = 11;
        DefensiveTemplate.BaseStats.TrainingTime = 9.0f;
        DefensiveTemplate.Traits.Add(EUnitTrait::Defensive);
        RegisterUnitTemplate(DefensiveTemplate);
    }
    
    // 注册民兵模板
    {
        FSharedUnitTemplate MilitiaTemplate;
        MilitiaTemplate.TemplateID = FName("Template_Militia");
        MilitiaTemplate.UnitName = FText::FromString(TEXT("民兵"));
        MilitiaTemplate.Description = FText::FromString(TEXT("低成本的地方民兵"));
        MilitiaTemplate.UnitType = EFactionUnitType::Infantry;
        MilitiaTemplate.BaseStats.AttackPower = 70;
        MilitiaTemplate.BaseStats.DefensePower = 70;
        MilitiaTemplate.BaseStats.MovementSpeed = 85;
        MilitiaTemplate.BaseStats.HealthPoints = 70;
        MilitiaTemplate.BaseStats.ProductionCost = 40;
        MilitiaTemplate.BaseStats.UpkeepCost = 4;
        MilitiaTemplate.BaseStats.TrainingTime = 4.0f;
        MilitiaTemplate.Traits.Add(EUnitTrait::Infantry);
        RegisterUnitTemplate(MilitiaTemplate);
    }
    
    UE_LOG(LogFactionDataPool, Log, TEXT("Registered %d common unit templates"), UnitTemplatePool.Num());
}

void UMingRTSFactionDataPool::RegisterCommonMechanicTemplates()
{
    // 注册军事机制模板
    {
        FSharedMechanicTemplate MilitaryTemplate;
        MilitaryTemplate.TemplateID = FName("Template_Mechanic_Military");
        MilitaryTemplate.MechanicName = FText::FromString(TEXT("军事优势"));
        MilitaryTemplate.Description = FText::FromString(TEXT("提升部队战斗力的机制"));
        MilitaryTemplate.MechanicType = EFactionMechanicType::Military;
        MilitaryTemplate.BaseEffectMultiplier = 1.2f;
        RegisterMechanicTemplate(MilitaryTemplate);
    }
    
    // 注册经济机制模板
    {
        FSharedMechanicTemplate EconomicTemplate;
        EconomicTemplate.TemplateID = FName("Template_Mechanic_Economic");
        EconomicTemplate.MechanicName = FText::FromString(TEXT("经济繁荣"));
        EconomicTemplate.Description = FText::FromString(TEXT("提升资源产出的机制"));
        EconomicTemplate.MechanicType = EFactionMechanicType::Economic;
        EconomicTemplate.BaseEffectMultiplier = 1.25f;
        RegisterMechanicTemplate(EconomicTemplate);
    }
    
    // 注册防御机制模板
    {
        FSharedMechanicTemplate DefenseTemplate;
        DefenseTemplate.TemplateID = FName("Template_Mechanic_Defense");
        DefenseTemplate.MechanicName = FText::FromString(TEXT("防御专精"));
        DefenseTemplate.Description = FText::FromString(TEXT("提升防守能力的机制"));
        DefenseTemplate.MechanicType = EFactionMechanicType::Military;
        DefenseTemplate.BaseEffectMultiplier = 1.3f;
        RegisterMechanicTemplate(DefenseTemplate);
    }
    
    // 注册政治机制模板
    {
        FSharedMechanicTemplate PoliticalTemplate;
        PoliticalTemplate.TemplateID = FName("Template_Mechanic_Political");
        PoliticalTemplate.MechanicName = FText::FromString(TEXT("政治影响"));
        PoliticalTemplate.Description = FText::FromString(TEXT("提升政治影响力的机制"));
        PoliticalTemplate.MechanicType = EFactionMechanicType::Political;
        PoliticalTemplate.BaseEffectMultiplier = 1.15f;
        RegisterMechanicTemplate(PoliticalTemplate);
    }
    
    UE_LOG(LogFactionDataPool, Log, TEXT("Registered %d common mechanic templates"), MechanicTemplatePool.Num());
}

FName UMingRTSFactionDataPool::RegisterUnitTemplate(const FSharedUnitTemplate& Template)
{
    FName TemplateID = Template.TemplateID;
    
    if (UnitTemplatePool.Contains(TemplateID))
    {
        UE_LOG(LogFactionDataPool, Warning, TEXT("Unit template %s already exists, updating..."), *TemplateID.ToString());
    }
    
    UnitTemplatePool.Add(TemplateID, Template);
    return TemplateID;
}

FName UMingRTSFactionDataPool::RegisterMechanicTemplate(const FSharedMechanicTemplate& Template)
{
    FName TemplateID = Template.TemplateID;
    
    if (MechanicTemplatePool.Contains(TemplateID))
    {
        UE_LOG(LogFactionDataPool, Warning, TEXT("Mechanic template %s already exists, updating..."), *TemplateID.ToString());
    }
    
    MechanicTemplatePool.Add(TemplateID, Template);
    return TemplateID;
}

bool UMingRTSFactionDataPool::GetUnitTemplate(FName TemplateID, FSharedUnitTemplate& OutTemplate) const
{
    const FSharedUnitTemplate* FoundTemplate = UnitTemplatePool.Find(TemplateID);
    if (FoundTemplate)
    {
        OutTemplate = *FoundTemplate;
        return true;
    }
    return false;
}

bool UMingRTSFactionDataPool::GetMechanicTemplate(FName TemplateID, FSharedMechanicTemplate& OutTemplate) const
{
    const FSharedMechanicTemplate* FoundTemplate = MechanicTemplatePool.Find(TemplateID);
    if (FoundTemplate)
    {
        OutTemplate = *FoundTemplate;
        return true;
    }
    return false;
}

void UMingRTSFactionDataPool::RegisterFactionVariant(const FFactionVariantData& VariantData)
{
    FactionVariants.Add(VariantData.FactionID, VariantData);
    
    // 增加引用计数
    for (FName UnitRef : VariantData.UnitTemplateRefs)
    {
        FSharedUnitTemplate* Template = UnitTemplatePool.Find(UnitRef);
        if (Template)
        {
            Template->RefCount++;
        }
    }
    
    for (FName MechanicRef : VariantData.MechanicTemplateRefs)
    {
        FSharedMechanicTemplate* Template = MechanicTemplatePool.Find(MechanicRef);
        if (Template)
        {
            Template->RefCount++;
        }
    }
    
    UE_LOG(LogFactionDataPool, Log, TEXT("Registered faction variant: %s"), *VariantData.FactionID.ToString());
}

bool UMingRTSFactionDataPool::GetFactionVariant(FName FactionID, FFactionVariantData& OutVariant) const
{
    const FFactionVariantData* FoundVariant = FactionVariants.Find(FactionID);
    if (FoundVariant)
    {
        OutVariant = *FoundVariant;
        return true;
    }
    return false;
}

FFactionUnit UMingRTSFactionDataPool::BuildUnitFromTemplate(FName FactionID, FName TemplateID) const
{
    FFactionUnit Result;
    
    FSharedUnitTemplate Template;
    if (!GetUnitTemplate(TemplateID, Template))
    {
        UE_LOG(LogFactionDataPool, Error, TEXT("Unit template %s not found"), *TemplateID.ToString());
        return Result;
    }
    
    FFactionVariantData Variant;
    if (!GetFactionVariant(FactionID, Variant))
    {
        UE_LOG(LogFactionDataPool, Error, TEXT("Faction variant %s not found"), *FactionID.ToString());
        return Result;
    }
    
    // 应用模板基础数据
    Result.UnitID = FName(*FString::Printf(TEXT("%s_%s"), *FactionID.ToString(), *TemplateID.ToString()));
    Result.UnitName = Template.UnitName;
    Result.Description = Template.Description;
    Result.UnitType = Template.UnitType;
    Result.BaseStats = Template.BaseStats;
    Result.Traits = Template.Traits;
    
    // 应用势力修正
    float Modifier = 1.0f;
    if (Variant.UnitStatModifiers.Contains(Template.UnitType))
    {
        Modifier = Variant.UnitStatModifiers[Template.UnitType];
    }
    
    Result.BaseStats.AttackPower = FMath::RoundToInt(Result.BaseStats.AttackPower * Modifier);
    Result.BaseStats.DefensePower = FMath::RoundToInt(Result.BaseStats.DefensePower * Modifier);
    Result.BaseStats.MovementSpeed = FMath::RoundToInt(Result.BaseStats.MovementSpeed * Modifier);
    Result.BaseStats.HealthPoints = FMath::RoundToInt(Result.BaseStats.HealthPoints * Modifier);
    
    return Result;
}

FFactionMechanic UMingRTSFactionDataPool::BuildMechanicFromTemplate(FName FactionID, FName TemplateID) const
{
    FFactionMechanic Result;
    
    FSharedMechanicTemplate Template;
    if (!GetMechanicTemplate(TemplateID, Template))
    {
        UE_LOG(LogFactionDataPool, Error, TEXT("Mechanic template %s not found"), *TemplateID.ToString());
        return Result;
    }
    
    FFactionVariantData Variant;
    if (!GetFactionVariant(FactionID, Variant))
    {
        UE_LOG(LogFactionDataPool, Error, TEXT("Faction variant %s not found"), *FactionID.ToString());
        return Result;
    }
    
    // 应用模板基础数据
    Result.MechanicID = FName(*FString::Printf(TEXT("%s_%s"), *FactionID.ToString(), *TemplateID.ToString()));
    Result.MechanicName = Template.MechanicName;
    Result.Description = Template.Description;
    Result.MechanicType = Template.MechanicType;
    
    // 应用势力修正
    float Modifier = 1.0f;
    if (Variant.MechanicEffectModifiers.Contains(Template.MechanicType))
    {
        Modifier = Variant.MechanicEffectModifiers[Template.MechanicType];
    }
    
    Result.EffectMultiplier = Template.BaseEffectMultiplier * Modifier;
    
    return Result;
}

FDataPoolMemoryStats UMingRTSFactionDataPool::GetMemoryStats() const
{
    FDataPoolMemoryStats Stats;
    
    Stats.UnitTemplateCount = UnitTemplatePool.Num();
    Stats.MechanicTemplateCount = MechanicTemplatePool.Num();
    Stats.FactionVariantCount = FactionVariants.Num();
    
    Stats.UnitPoolMemoryBytes = UnitTemplatePool.Num() * CalculateUnitTemplateSize();
    Stats.MechanicPoolMemoryBytes = MechanicTemplatePool.Num() * CalculateMechanicTemplateSize();
    
    for (const auto& Pair : FactionVariants)
    {
        Stats.VariantsMemoryBytes += CalculateVariantDataSize(Pair.Value);
    }
    
    Stats.TotalMemoryBytes = Stats.UnitPoolMemoryBytes + Stats.MechanicPoolMemoryBytes + Stats.VariantsMemoryBytes;
    
    // 估算节省内存 = 传统方式内存 - 数据池方式内存
    // 传统方式：每个势力独立存储所有单位数据
    int32 TraditionalMemory = FactionVariants.Num() * 4 * CalculateUnitTemplateSize(); // 4 units per faction
    Stats.EstimatedSavingsBytes = TraditionalMemory - Stats.TotalMemoryBytes;
    
    return Stats;
}

int32 UMingRTSFactionDataPool::CleanupUnusedTemplates()
{
    int32 CleanedCount = 0;
    
    // 清理未引用的单位模板
    TArray<FName> UnitTemplatesToRemove;
    for (const auto& Pair : UnitTemplatePool)
    {
        if (Pair.Value.RefCount == 0 && !CommonTemplates.Contains(Pair.Key))
        {
            UnitTemplatesToRemove.Add(Pair.Key);
        }
    }
    
    for (FName TemplateID : UnitTemplatesToRemove)
    {
        UnitTemplatePool.Remove(TemplateID);
        CleanedCount++;
    }
    
    // 清理未引用的机制模板
    TArray<FName> MechanicTemplatesToRemove;
    for (const auto& Pair : MechanicTemplatePool)
    {
        if (Pair.Value.RefCount == 0 && !CommonTemplates.Contains(Pair.Key))
        {
            MechanicTemplatesToRemove.Add(Pair.Key);
        }
    }
    
    for (FName TemplateID : MechanicTemplatesToRemove)
    {
        MechanicTemplatePool.Remove(TemplateID);
        CleanedCount++;
    }
    
    UE_LOG(LogFactionDataPool, Log, TEXT("Cleaned up %d unused templates"), CleanedCount);
    return CleanedCount;
}

void UMingRTSFactionDataPool::PreloadCommonTemplates()
{
    // 标记常用模板不被清理
    for (const auto& Pair : UnitTemplatePool)
    {
        CommonTemplates.Add(Pair.Key);
    }
    
    for (const auto& Pair : MechanicTemplatePool)
    {
        CommonTemplates.Add(Pair.Key);
    }
    
    UE_LOG(LogFactionDataPool, Log, TEXT("Preloaded %d common templates"), CommonTemplates.Num());
}

int64 UMingRTSFactionDataPool::CalculateMemorySavings() const
{
    return GetMemoryStats().EstimatedSavingsBytes;
}

int32 UMingRTSFactionDataPool::CalculateUnitTemplateSize() const
{
    // 估算单位模板内存大小
    return sizeof(FSharedUnitTemplate) + 64; // 基础大小 + 字符串开销
}

int32 UMingRTSFactionDataPool::CalculateMechanicTemplateSize() const
{
    // 估算机制模板内存大小
    return sizeof(FSharedMechanicTemplate) + 32;
}

int32 UMingRTSFactionDataPool::CalculateVariantDataSize(const FFactionVariantData& Variant) const
{
    // 估算差异数据内存大小
    int32 Size = sizeof(FFactionVariantData);
    Size += Variant.UnitTemplateRefs.Num() * sizeof(FName);
    Size += Variant.MechanicTemplateRefs.Num() * sizeof(FName);
    Size += Variant.UnitStatModifiers.Num() * (sizeof(EFactionUnitType) + sizeof(float));
    Size += Variant.MechanicEffectModifiers.Num() * (sizeof(EFactionMechanicType) + sizeof(float));
    return Size;
}
