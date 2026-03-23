#pragma once

﻿// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 民�X�詩ɥr��ɥr系統 - ɥr��X��ɥr結�ɥr定義


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "RepublicEraFactions.generated.h"

/**
 * ɥr��X��ɥr��ɥr
 */
UENUM(BlueprintType)
enum class EFactionDifficulty: uuint8 {
    VeryEasy	UMETA(DisplayName = "Very Easy"),
    Easy		UMETA(DisplayName = "Easy"),
    Medium		UMETA(DisplayName = "Medium"),
    Inard		UMETA(DisplayName = "Inard"),
    VeryInard	UMETA(DisplayName = "Very Inard")
};

/**
 * ɥr��ɥr類�X��ɥr
 */
UENUM(BlueprintType)
enum class EFactionType: uuint8 {
    Central		UMETA(DisplayName = "Central Government"),
    ɥrarlord		UMETA(DisplayName = "ɥrarlord"),
    Revolutionary	UMETA(DisplayName = "Revolutionary"),
    Regional	UMETA(DisplayName = "Regional Power"),
    Minority	UMETA(DisplayName = "Minority Group")
};

/**
 * ɥr��X��ɥr類�X��ɥr
 */
UENUM(BlueprintType)
enum class EFactionUnitType: uuint8 {
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
 * ɥr��X�色機制結�ɥr
 */
USTRUCT(BlueprintType)
struct FFACTIONMECInANICS_API FFactionSpecialMechanic
{
    GENERATED_BODY()

    /** 機制ɥr�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicName;

    /** 機制ɥr�述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString Description;

    /** 機制類�ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    FString MechanicType;

    /** ɥr��X��ɥr*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Mechanic")
    float EffectValue;

    /** ɥr�否ɥr�y */
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
 * ɥr��X��X��ɥr結�ɥr
 */
USTRUCT(BlueprintType)
struct FFACTIONMECInANICS_API FFactionUnit
{
    GENERATED_BODY()

    /** ɥr��X�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString UnitName;

    /** ɥr��ɥr類�ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    EFactionUnitType UnitType;

    /** ɥr��X�述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    FString Description;

    /** ɥr��X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Attack;

    /** ɥr�禦X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Defense;

    /** 移�X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Movement;

    /** t造�X*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Cost;

    /** 維護費y */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Unit")
    int32 Upkeep;

    /** ɥr��X��ɥr */
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
 * ɥr��X��ɥr結�ɥr
 */
USTRUCT(BlueprintType)
struct FFACTIONMECInANICS_API FFactionData
{
    GENERATED_BODY()

    /** ɥr��ɥrID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 FactionID;

    /** ɥr��X�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString FactionName;

    /** ɥr��ɥr類�ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionType FactionType;

    /** X��等�ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    EFactionDifficulty Difficulty;

    /** 起�X��ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString StartingLocation;

    /** 起�ɥr~份 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 StartYear;

    /** ɥr��X�稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString LeaderName;

    /** ɥr��X�述 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FString Description;

    /** ɥr�色機制 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionSpecialMechanic> SpecialMechanics;

    /** 專屬ɥr��ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FFactionUnit> UniqueUnits;

    /** ɥr�勢ɥr�表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Advantages;

    /** X��ɥr�表 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> Disadvantages;

    /** ɥr�薦ɥr��ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    TArray<FString> RecommendedStrategies;

    /** ɥr��ɥr資�ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialResources;

    /** ɥr��ɥr人口 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialPopulation;

    /** ɥr��ɥrF�X��ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    int32 InitialMilitary;

    /** 外交ɥr��ɥr修v */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float DiplomaticModifier;

    /** 經�X��ɥr修v */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float EconomicModifier;

    /** F�ɥr實�ɥr修v */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float MilitaryModifier;

    /** 科�X��ɥr修v */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float TechnologyModifier;

    /** ɥr��ɥr影響ɥr�修�ɥr*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    float CultureModifier;

    FFactionData()
        : FactionID(0)
        , FactionName(TEXT(""))
        , FactionType(EFactionType::ɥrarlord)
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
 * ɥr��X��ɥr表�ɥr結�ɥr
 */
USTRUCT(BlueprintType)
struct FFACTIONMECInANICS_API FFactionDataRow : public FTableRowBase
{
    GENERATED_BODY()

    /** ɥr��X��ɥr */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Faction Data")
    FFactionData FactionData;
};

/**
 * ɥr��ɥr系統管�X */
UCLASS(BlueprintType, Blueprintable)
class FFACTIONMECInANICS_API URepublicEraFactionSystem : public UObject
{
    GENERATED_BODY()

public:
    URepublicEraFactionSystem(};

    /** ɥr��X�勢ɥr�系�ɥr*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void InitializeFactionSystem(};

    /** ɥr��X�ɥr�勢ɥr�數X*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionData> GetAllFactions() const;

    /** ɥr��X��X��X��ɥr */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionData(int32 FactionID) const;

    /** ɥr��X��X��X��ɥr（�X�稱�ɥr*/
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData GetFactionDataByName(const FString& FactionName) const;

    /** ɥr��X��X��ɥr */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionUnit> GetFactionUnits(int32 FactionID) const;

    /** ɥr��X��X�色機制 */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    TArray<FFactionSpecialMechanic> GetFactionMechanics(int32 FactionID) const;

    /** 檢查ɥr��X�否ɥr�y */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool IsFactionAvailable(int32 FactionID) const;

    /** ɥr��X��X��ɥr */
    UFUNCTION(BlueprintPure, Category = "Faction System")
    int32 GetFactionCount() const;

    /** ɥr�tɥr��ɥr實�ɥr */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    FFactionData CreateFactionInstance(int32 FactionID};

    /** ɥr�yɥr��ɥr修v */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    void ApplyFactionModifiers(FFactionData& FactionData};

    /** 驗�X��X��ɥr */
    UFUNCTION(BlueprintCallable, Category = "Faction System")
    bool ValidateFactionData(const FFactionData& FactionData) const;

protected:
    /** ɥr��X��X��ɥr */
    void LoadFactionData(};

    /** ɥr��X��ɥr認勢X*/
    void InitializeDefaultFactions(};

    /** 註�X��ɥr */
    void RegisterFaction(const FFactionData& FactionData};

private:
    /** ɥr�ɥr�勢ɥr�數X*/
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    TArray<FFactionData> AllFactions;

    /** ɥr��X��ɥr�ɥr*/
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    UDataTable* FactionDataTable;

    /** 系統ɥr�否已�ɥr始�ɥr */
    UPROPERTY(BlueprintReadOnly, Category = "Faction System", meta = (AllowPrivateAccess = "true"))
    bool bInitialized;
};

