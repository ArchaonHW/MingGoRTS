// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 民�X�詩?��?系統 - ?��X��?結�?定義

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "RepublicEraFactions.generated.h"

/**
 * ?��X��?��?
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
 * ?��?類�X��?
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
 * ?��X��?類�X��?
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
 * ?��X�色機制結�?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionSpecialMechanic
{
    GENERATED_BODY()

    /** 機制?�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicName;

    /** 機制?�述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString Description;

    /** 機制類�? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicType;

    /** ?��X��?*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    float EffectValue;

    /** ?�否?�用 */
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
 * ?��X��X��?結�?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionUnit
{
    GENERATED_BODY()

    /** ?��X�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString UnitName;

    /** ?��?類�? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    EFactionUnitType UnitType;

    /** ?��X�述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString Description;

    /** ?��X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Attack;

    /** ?�禦X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Defense;

    /** 移�X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Movement;

    /** 建造�X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Cost;

    /** 維護費用 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Upkeep;

    /** ?��X��? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString SpecialAbility;

    /** �X條件 */
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
 * ?��X��?結�?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionData
{
    GENERATED_BODY()

    /** ?��?ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 FactionID;

    /** ?��X�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString FactionName;

    /** ?��?類�? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionType FactionType;

    /** X��等�? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionDifficulty Difficulty;

    /** 起�X��? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString StartingLocation;

    /** 起�?年份 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 StartYear;

    /** ?��X�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString LeaderName;

    /** ?��X�述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString Description;

    /** ?�色機制 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionSpecialMechanic> SpecialMechanics;

    /** 專屬?��? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionUnit> UniqueUnits;

    /** ?�勢?�表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Advantages;

    /** X��?�表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Disadvantages;

    /** ?�薦?��? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> RecommendedStrategies;

    /** ?��?資�? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialResources;

    /** ?��?人口 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialPopulation;

    /** ?��?軍�X��? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialMilitary;

    /** 外交?��?修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float DiplomaticModifier;

    /** 經�X��?修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float EconomicModifier;

    /** 軍�?實�?修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float MilitaryModifier;

    /** 科�X��?修正 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float TechnologyModifier;

    /** ?��?影響?�修�?*/
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
 * ?��X��?表�?結�?
 */
USTRUCT(BlueprintType)
struct FFACTIONMECHANICS_API FFactionDataRow : public FTableRowBase
{
    GENERATED_BODY()

    /** ?��X��? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FFactionData FactionData;
};

/**
 * ?��?系統管�X */
UCLASS(BlueprintType, Blueprintable)
class FFACTIONMECHANICS_API URepublicEraFactionSystem : public UObject
{
    GENERATED_BODY()

public:
    URepublicEraFactionSystem(};

    /** ?��X�勢?�系�?*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void InitializeFactionSystem(};

    /** ?��X�?�勢?�數X*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionData> GetAllFactions() const;

    /** ?��X��X��X��? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionData(int32 FactionID) const;

    /** ?��X��X��X��?（�X�稱�?*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionDataByName(const FString& FactionName) const;

    /** ?��X��X��? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionUnit> GetFactionUnits(int32 FactionID) const;

    /** ?��X��X�色機制 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionSpecialMechanic> GetFactionMechanics(int32 FactionID) const;

    /** 檢查?��X�否?�用 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool IsFactionAvailable(int32 FactionID) const;

    /** ?��X��X��? */
    UFUNCTION(BlueprintPure, Category = "Faction System")
    int32 GetFactionCount() const;

    /** ?�建?��?實�? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData CreateFactionInstance(int32 FactionID};

    /** ?�用?��?修正 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void ApplyFactionModifiers(FFactionData& FactionData};

    /** 驗�X��X��? */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool ValidateFactionData(const FFactionData& FactionData) const;

protected:
    /** ?��X��X��? */
    void LoadFactionData(};

    /** ?��X��?認勢X*/
    void InitializeDefaultFactions(};

    /** 註�X��? */
    void RegisterFaction(const FFactionData& FactionData};

private:
    /** ?�?�勢?�數X*/
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    TArray<FFactionData> AllFactions;

    /** ?��X��?�?*/
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    UDataTable* FactionDataTable;

    /** 系統?�否已�?始�? */
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;
};

