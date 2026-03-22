// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力共享数据池系统 - Memory Optimization

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Map.h"
#include "Containers/Set.h"
#include "MingRTSFactionDataPool.generated.h"

/**
 * 共享单位模板数据 - 可被多个势力引用
 */
USTRUCT(BlueprintType)
struct FSharedUnitTemplate
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FName TemplateID;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FText UnitName;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FText Description;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    EFactionUnitType UnitType;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FFactionUnitStats BaseStats;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    TArray<EUnitTrait> Traits;
    
    /** 引用计数 */
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int32 RefCount = 0;
};

/**
 * 共享机制模板数据
 */
USTRUCT(BlueprintType)
struct FSharedMechanicTemplate
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FName TemplateID;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FText MechanicName;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FText Description;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    EFactionMechanicType MechanicType;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    float BaseEffectMultiplier = 1.0f;
    
    /** 引用计数 */
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int32 RefCount = 0;
};

/**
 * 势力差异数据 - 存储各势力特有的数据
 */
USTRUCT(BlueprintType)
struct FFactionVariantData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FName FactionID;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FText FactionName;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FLinearColor FactionColor;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    FText CapitalCity;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int32 InitialGold = 1000;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int32 InitialManpower = 500;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    TArray<FName> UnitTemplateRefs;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    TArray<FName> MechanicTemplateRefs;
    
    /** 单位属性修正器 */
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    TMap<EFactionUnitType, float> UnitStatModifiers;
    
    /** 机制效果修正器 */
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    TMap<EFactionMechanicType, float> MechanicEffectModifiers;
};

/**
 * 势力数据池管理器 - 内存优化核心系统
 * 实现共享模板 + 差异数据的存储模式，降低内存占用
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UMingRTSFactionDataPool : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSFactionDataPool();
    
    /** 初始化数据池 */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    void InitializeDataPool();
    
    /** 注册共享单位模板 */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    FName RegisterUnitTemplate(const FSharedUnitTemplate& Template);
    
    /** 注册共享机制模板 */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    FName RegisterMechanicTemplate(const FSharedMechanicTemplate& Template);
    
    /** 获取单位模板 */
    UFUNCTION(BlueprintPure, Category = "Faction|DataPool")
    bool GetUnitTemplate(FName TemplateID, FSharedUnitTemplate& OutTemplate) const;
    
    /** 获取机制模板 */
    UFUNCTION(BlueprintPure, Category = "Faction|DataPool")
    bool GetMechanicTemplate(FName TemplateID, FSharedMechanicTemplate& OutTemplate) const;
    
    /** 注册势力差异数据 */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    void RegisterFactionVariant(const FFactionVariantData& VariantData);
    
    /** 获取势力差异数据 */
    UFUNCTION(BlueprintPure, Category = "Faction|DataPool")
    bool GetFactionVariant(FName FactionID, FFactionVariantData& OutVariant) const;
    
    /** 构建完整单位数据（模板+修正） */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    FFactionUnit BuildUnitFromTemplate(FName FactionID, FName TemplateID) const;
    
    /** 构建完整机制数据（模板+修正） */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    FFactionMechanic BuildMechanicFromTemplate(FName FactionID, FName TemplateID) const;
    
    /** 获取内存使用统计 */
    UFUNCTION(BlueprintPure, Category = "Faction|DataPool")
    FDataPoolMemoryStats GetMemoryStats() const;
    
    /** 清理未引用的模板 */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    int32 CleanupUnusedTemplates();
    
    /** 预加载常用模板 */
    UFUNCTION(BlueprintCallable, Category = "Faction|DataPool")
    void PreloadCommonTemplates();
    
    /** 计算内存节省量 */
    UFUNCTION(BlueprintPure, Category = "Faction|DataPool")
    int64 CalculateMemorySavings() const;

private:
    /** 共享单位模板池 */
    UPROPERTY()
    TMap<FName, FSharedUnitTemplate> UnitTemplatePool;
    
    /** 共享机制模板池 */
    UPROPERTY()
    TMap<FName, FSharedMechanicTemplate> MechanicTemplatePool;
    
    /** 势力差异数据存储 */
    UPROPERTY()
    TMap<FName, FFactionVariantData> FactionVariants;
    
    /** 常用模板ID集合（不清理） */
    UPROPERTY()
    TSet<FName> CommonTemplates;
    
    /** 计算单个单位模板内存大小 */
    int32 CalculateUnitTemplateSize() const;
    
    /** 计算单个机制模板内存大小 */
    int32 CalculateMechanicTemplateSize() const;
    
    /** 计算差异数据内存大小 */
    int32 CalculateVariantDataSize(const FFactionVariantData& Variant) const;
};

/**
 * 数据池内存统计结构
 */
USTRUCT(BlueprintType)
struct FDataPoolMemoryStats
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int32 UnitTemplateCount = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int32 MechanicTemplateCount = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int32 FactionVariantCount = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int64 UnitPoolMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int64 MechanicPoolMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int64 VariantsMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int64 TotalMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "Faction|DataPool")
    int64 EstimatedSavingsBytes = 0;
};
