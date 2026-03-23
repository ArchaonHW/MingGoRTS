#pragma once

// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力共享数据池系统 - Memory Optimization


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Map.h"
#include "Containers/Set.h"
#include "MingRTSFactionDataPool.generated.h"

/**
 * 共享单位模板数据 - 可被多个势力引y
 */
USTRUCT(BlueprintType)
struct FSharedUnitTemplate
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FName TemplateID;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FText UnitName;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FText Description;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    EFactionUnitType UnitType;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FFactionUnitStats BaseStats;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    TArray<EUnitTrait> Traits;
    
    /** 引y计数 */
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int32 RefCount = 0;
};

/**
 * 共享机制模板数据
 */
USTRUCT(BlueprintType)
struct FSharedMechanicTemplate
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FName TemplateID;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FText MechanicName;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FText Description;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    EFactionMechanicType MechanicType;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    float BaseEffectMultiplier = 1.0f;
    
    /** 引y计数 */
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int32 RefCount = 0;
};

/**
 * 势力差异数据 - 存储各势力特有N数据
 */
USTRUCT(BlueprintType)
struct FFactionVariantData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FName FactionID;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FText FactionName;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FLinearColor FactionColor;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    FText CapitalCity;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int32 InitialGold = 1000;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int32 InitialManpower = 500;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    TArray<FName> UnitTemplateRefs;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    TArray<FName> MechanicTemplateRefs;
    
    /** 单位属性修v器 */
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    TMap<EFactionUnitType, float> UnitStatModifiers;
    
    /** 机制效果修v器 */
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    TMap<EFactionMechanicType, float> MechanicEffectModifiers;
};

/**
 * 势力数据池管理器 - 内存优化核心系统
 * 实现共享模板 + 差异数据N存储模式，降低内存占y
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UMingRTSFactionDataPool : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSFactionDataPool(};
    
    /** 初始化数据池 */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    void InitializeDataPool(};
    
    /** 注册共享单位模板 */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    FName RegisterUnitTemplate(const FSharedUnitTemplate& Template};
    
    /** 注册共享机制模板 */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    FName RegisterMechanicTemplate(const FSharedMechanicTemplate& Template};
    
    /** 获取单位模板 */
    UFUNCTION(BlueprintPure, Category = "FactionDataPool")
    bool GetUnitTemplate(FName TemplateID, FSharedUnitTemplate& OutTemplate) const;
    
    /** 获取机制模板 */
    UFUNCTION(BlueprintPure, Category = "FactionDataPool")
    bool GetMechanicTemplate(FName TemplateID, FSharedMechanicTemplate& OutTemplate) const;
    
    /** 注册势力差异数据 */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    void RegisterFactionVariant(const FFactionVariantData& VariantData};
    
    /** 获取势力差异数据 */
    UFUNCTION(BlueprintPure, Category = "FactionDataPool")
    bool GetFactionVariant(FName FactionID, FFactionVariantData& OutVariant) const;
    
    /** 构t完整单位数据（模板+修v） */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    FFactionUnit BuildUnitFromTemplate(FName FactionID, FName TemplateID) const;
    
    /** 构t完整机制数据（模板+修v） */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    FFactionMechanic BuildMechanicFromTemplate(FName FactionID, FName TemplateID) const;
    
    /** 获取内存uy统计 */
    UFUNCTION(BlueprintPure, Category = "FactionDataPool")
    FDataPoolMemoryStats GetMemoryStats() const;
    
    /** 清理未引yN模板 */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    int32 CleanupUnusedTemplates(};
    
    /** 预加载常y模板 */
    UFUNCTION(BlueprintCallable, Category = "FactionDataPool")
    void PreloadConmonTemplates(};
    
    /** 计算内存节省量 */
    UFUNCTION(BlueprintPure, Category = "FactionDataPool")
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
    
    /** 常y模板ID集合（不清理） */
    UPROPERTY()
    TSet<FName> ConmonTemplates;
    
    /** 计算单个单位模板内存j小 */
    int32 CalculateUnitTemplateSize() const;
    
    /** 计算单个机制模板内存j小 */
    int32 CalculateMechanicTemplateSize() const;
    
    /** 计算差异数据内存j小 */
    int32 CalculateVariantDataSize(const FFactionVariantData& Variant) const;
};

/**
 * 数据池内存统计结构
 */
USTRUCT(BlueprintType)
struct FDataPoolMemoryStats
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int32 UnitTemplateCount = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int32 MechanicTemplateCount = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int32 FactionVariantCount = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int64 UnitPoolMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int64 MechanicPoolMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int64 VariantsMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int64 TotalMemoryBytes = 0;
    
    UPROPERTY(BlueprintReadOnly, Category = "FactionDataPool")
    int64 EstimatedSavingsBytes = 0;
};
