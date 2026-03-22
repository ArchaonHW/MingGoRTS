// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 民國史詩勢力系統 - 勢力數據結構定義

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "RepublicEraFactions.generated.h"

/**
 * 勢力難度枚舉
 */
UENUM(BlueprintType)
enum class EFactionDifficulty : uint8
{
    VeryEasy	UMETA(DisplayName = "Very Easy"),
    Easy		UMETA(DisplayName = "Easy"),
    Medium		UMETA(DisplayName = "Medium"),
    Hard		UMETA(DisplayName = "Hard"),
    VeryHard	UMETA(DisplayName = "Very Hard")
};

/**
 * 勢力類型枚舉
 */
UENUM(BlueprintType)
enum class EFactionType : uint8
{
    Central		UMETA(DisplayName = "Central Government"),
    Warlord		UMETA(DisplayName = "Warlord"),
    Revolutionary	UMETA(DisplayName = "Revolutionary"),
    Regional	UMETA(DisplayName = "Regional Power"),
    Minority	UMETA(DisplayName = "Minority Group")
};

/**
 * 勢力單位類型枚舉
 */
UENUM(BlueprintType)
enum class EFactionUnitType : uint8
{
    Infantry	UMETA(DisplayName = "Infantry"),
    Cavalry		UMETA(DisplayName = "Cavalry"),
    Artillery	UMETA(DisplayName = "Artillery"),
    Elite		UMETA(DisplayName = "Elite Unit"),
    Guard		UMETA(DisplayName = "Guard Unit"),
    Militia		UMETA(DisplayName = "Militia"),
    Political	UMETA(DisplayName = "Political Unit"),
    Air		UMETA(DisplayName = "Air Unit")
};

/**
 * 勢力特色機制結構
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionSpecialMechanic
{
    GENERATED_BODY()

    /** 機制名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicName;

    /** 機制描述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString Description;

    /** 機制類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicType;

    /** 效果數值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    float EffectValue;

    /** 是否啟用 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    bool bEnabled;

    FFactionSpecialMechanic()
        : MechanicName(TEXT(""))
        , Description(TEXT(""))
        , MechanicType(TEXT(""))
        , EffectValue(0.0f)
        , bEnabled(true)
    {
    }
};

/**
 * 勢力單位數據結構
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionUnit
{
    GENERATED_BODY()

    /** 單位名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString UnitName;

    /** 單位類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    EFactionUnitType UnitType;

    /** 單位描述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString Description;

    /** 攻擊力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Attack;

    /** 防禦力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Defense;

    /** 移動力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Movement;

    /** 建造成本 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Cost;

    /** 維護費用 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Upkeep;

    /** 特殊能力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString SpecialAbility;

    /** 解鎖條件 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString UnlockCondition;

    FFactionUnit()
        : UnitName(TEXT(""))
        , UnitType(EFactionUnitType::Infantry)
        , Description(TEXT(""))
        , Attack(5)
        , Defense(5)
        , Movement(4)
        , Cost(100)
        , Upkeep(10)
        , SpecialAbility(TEXT(""))
        , UnlockCondition(TEXT(""))
    {
    }
};

/**
 * 勢力數據結構
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionData
{
    GENERATED_BODY()

    /** 勢力ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 FactionID;

    /** 勢力名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString FactionName;

    /** 勢力類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionType FactionType;

    /** 難度等級 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionDifficulty Difficulty;

    /** 起始地點 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString StartingLocation;

    /** 起始年份 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 StartYear;

    /** 領袖名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString LeaderName;

    /** 勢力描述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString Description;

    /** 特色機制 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionSpecialMechanic> SpecialMechanics;

    /** 專屬單位 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionUnit> UniqueUnits;

    /** 優勢列表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Advantages;

    /** 劣勢列表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Disadvantages;

    /** 推薦玩法 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> RecommendedStrategies;

    /** 初始資源 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialResources;

    /** 初始人口 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialPopulation;

    /** 初始軍隊數量 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialMilitary;

    /** 外交關係修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float DiplomaticModifier;

    /** 經濟發展修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float EconomicModifier;

    /** 軍事實力修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float MilitaryModifier;

    /** 科技發展修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float TechnologyModifier;

    /** 文化影響力修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float CultureModifier;

    FFactionData()
        : FactionID(0)
        , FactionName(TEXT(""))
        , FactionType(EFactionType::Warlord)
        , Difficulty(EFactionDifficulty::Medium)
        , StartingLocation(TEXT(""))
        , StartYear(1912)
        , LeaderName(TEXT(""))
        , Description(TEXT(""))
        , InitialResources(1000)
        , InitialPopulation(100000)
        , InitialMilitary(100)
        , DiplomaticModifier(0.0f)
        , EconomicModifier(0.0f)
        , MilitaryModifier(0.0f)
        , TechnologyModifier(0.0f)
        , CultureModifier(0.0f)
    {
    }
};

/**
 * 勢力數據表行結構
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionDataRow : public FTableRowBase
{
    GENERATED_BODY()

    /** 勢力數據 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FFactionData FactionData;
};

/**
 * 勢力系統管理器
 */
UCLASS(BlueprintType, Blueprintable)
class FFACTIONMECHANICS_API URepublicEraFactionSystem : public UObject
{
    GENERATED_BODY()

public:
    URepublicEraFactionSystem();

    /** 初始化勢力系統 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void InitializeFactionSystem();

    /** 獲取所有勢力數據 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionData> GetAllFactions() const;

    /** 獲取指定勢力數據 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionData(int32 FactionID) const;

    /** 獲取指定勢力數據（按名稱） */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionDataByName(const FString& FactionName) const;

    /** 獲取勢力單位 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionUnit> GetFactionUnits(int32 FactionID) const;

    /** 獲取勢力特色機制 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionSpecialMechanic> GetFactionMechanics(int32 FactionID) const;

    /** 檢查勢力是否可用 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool IsFactionAvailable(int32 FactionID) const;

    /** 獲取勢力數量 */
    UFUNCTION(BlueprintPure, Category = "Faction System")
    int32 GetFactionCount() const;

    /** 創建勢力實例 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData CreateFactionInstance(int32 FactionID);

    /** 應用勢力修正 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void ApplyFactionModifiers(FFactionData& FactionData);

    /** 驗證勢力數據 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool ValidateFactionData(const FFactionData& FactionData) const;

protected:
    /** 加載勢力數據 */
    void LoadFactionData();

    /** 初始化默認勢力 */
    void InitializeDefaultFactions();

    /** 註冊勢力 */
    void RegisterFaction(const FFactionData& FactionData);

private:
    /** 所有勢力數據 */
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    TArray<FFactionData> AllFactions;

    /** 勢力數據表 */
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    UDataTable* FactionDataTable;

    /** 系統是否已初始化 */
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;
};
